/* Copyright (C) 2007 ROCK-CHIPS FUZHOU . All Rights Reserved. */
/*
File : \Audio\ADPCM
Desc : WAV解码。包括PCM WAV , IMA-ADPCM WAV , MS-ADPCM WAV 。

Author : FSH , Vincent Hisung
Date : 2007-08-xx
Notes :

$Log :
* vincent     2007/08/xx 建立此文件
*
*/
/****************************************************************/
//#include "stdio.h"
#include "../include/audio_main.h"
#include "../include/audio_globals.h"

#ifdef A_CORE_DECODE
#ifdef WAV_DEC_INCLUDE

//#include "../buffer/buffer.h"
#include "PCM.H"
//#include "../common/mRECORD.H"
#include "../include/audio_file_access.h"

#include "ImaDecoder.h"

#define ADPCM_FILL_HDR	90
extern unsigned char ADPCM_Hdr[ADPCM_FILL_HDR];

//*************************************************************************************************************//
#define WAVE_DEFINE_GUID(name, b1, b2, b3, b4) \
			_ATTR_WAVDEC_DATA_ \
			WAVFOURCCID name; \
			WAVFOURCCID name = {b1,b2,b3,b4}

#define WAVE_EqualFOURCC(rguid1, rguid2) (!memcmp((void *)rguid1, (void *)rguid2, sizeof(WAVFOURCCID)))

WAVE_DEFINE_GUID(FOURCC_FMT, 0x66,0x6d,0x74,0x20);
WAVE_DEFINE_GUID(FOURCC_DATA, 0x64,0x61,0x74,0x61);
WAVE_DEFINE_GUID(FOURCC_LIST, 0x4c,0x49,0x53,0x54);
WAVE_DEFINE_GUID(FOURCC_INFO, 0x49,0x4e,0x46,0x4f);
WAVE_DEFINE_GUID(FOURCC_IART, 0x49,0x41,0x52,0x54);


WAVE_DEFINE_GUID(FOURCC_INAM, 0x49,0x4e,0x41,0x4d);
WAVE_DEFINE_GUID(FOURCC_IPRD, 0x49,0x50,0x52,0x44);
WAVE_DEFINE_GUID(FOURCC_IGNR, 0x49,0x47,0x4e,0x52);
//*************************************************************************************************************//

#define	INFO_BYTE_SIZE	32
typedef struct
{
    char Artist[INFO_BYTE_SIZE];
    char Name[INFO_BYTE_SIZE];
    char Product[INFO_BYTE_SIZE];
    char Genre[INFO_BYTE_SIZE];
} tINFO;

//****************************************************************************
//
// WAV_PARSER.cs
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
long	WAVE_LoadChunkFOURCC(tPCM *pPCM,
                          WAVFOURCCID *pObjectId,
                          unsigned long *Size)
{

    long	read_size;

    RKFIO_FSeek( pPCM->usOffset , 0 , pRawFileCache );

    read_size = RKFIO_FRead( pPCM->pucEncodedData, WAVE_CHUNK_FORCC_SIZE_BYTE , pRawFileCache );

    /* If there is no more data in the file, then return a failure since we
        could not find the "data" chunk. */
    if (read_size !=WAVE_CHUNK_FORCC_SIZE_BYTE)
    {
        return PCM_NOT_SUPPORT;
    }

    /* Get the FOURCC */
    pObjectId->Data1 = pPCM->pucEncodedData[0];
    pObjectId->Data2 = pPCM->pucEncodedData[1];
    pObjectId->Data3 = pPCM->pucEncodedData[2];
    pObjectId->Data4 = pPCM->pucEncodedData[3];

    /* Get the length of the chunk. */
    *Size =	(pPCM->pucEncodedData[7] << 24) |
            (pPCM->pucEncodedData[6] << 16) |
            (pPCM->pucEncodedData[5] << 8) |
            pPCM->pucEncodedData[4];

    /* update file offset */
    pPCM->usOffset +=WAVE_CHUNK_FORCC_SIZE_BYTE;	// pPCM->usOffset = 20;
    //RIFF"SIZE"WAVE"fmt*"Chunk or // RIFF"SIZE"WAVE"fmt*"Chunk~~"DATA"SIZE

    return PCM_SUPPORT_FORMAT;
}

_ATTR_WAVDEC_TEXT_
long	WAVE_LoadDATAChunk(tPCM *pPCM,unsigned long ulLength )
{
    /* Get the starting position of the "data" chunk. */
    pPCM->ulDataStart = pPCM->usOffset;  // RIFF"SIZE"WAVE"fmt*"Chunk~~"DATA"SIZE

    /* Get the length of the "data" chunk. */
    if(ulLength > RKFIO_FLength(pRawFileCache))
    {
      ulLength = RKFIO_FLength(pRawFileCache);
    }
    pPCM->ulLength = ulLength;
    return PCM_SUPPORT_FORMAT;
}

_ATTR_WAVDEC_TEXT_
long	WAVE_LoadINFOChunk(tPCM *pPCM, unsigned long ChunkSize, unsigned long mem_info_size )
{
    WAVFOURCCID fourccINFOID;
    unsigned long ulLength=0;
    unsigned long subChunkSize=0;
    unsigned long info_size;
    long			read_size;

    tINFO pINFO;

    ulLength = WAVE_FORCC_INFO_SIZE; //for INFO

    while ( ulLength < ChunkSize )
    {
        if (WAVE_LoadChunkFOURCC(pPCM,&fourccINFOID,&subChunkSize) != PCM_SUPPORT_FORMAT)
            return PCM_NOT_SUPPORT;


        if ( subChunkSize %2 == 1)
            subChunkSize++;


        ulLength += (subChunkSize+8);

        if ( subChunkSize > mem_info_size )
            info_size = mem_info_size;
        else
            info_size = subChunkSize;


        /* Artist */
        if ( WAVE_EqualFOURCC(&FOURCC_IART, &fourccINFOID))
        {
            read_size = RKFIO_FRead( &pINFO.Artist[0], info_size , pRawFileCache );
            if (read_size != info_size)
                return PCM_NOT_SUPPORT;

        }

        /* Name */
        else if ( WAVE_EqualFOURCC(&FOURCC_INAM, &fourccINFOID))
        {
            read_size = RKFIO_FRead( &pINFO.Name[0], info_size , pRawFileCache );
            if (read_size != info_size)
                return PCM_NOT_SUPPORT;

        }

        /* Product */
        else if ( WAVE_EqualFOURCC(&FOURCC_IPRD, &fourccINFOID))
        {
            read_size = RKFIO_FRead( &pINFO.Product[0], info_size , pRawFileCache );
            if (read_size != info_size)
                return PCM_NOT_SUPPORT;

        }
        /* Genre */
        else if ( WAVE_EqualFOURCC(&FOURCC_IGNR, &fourccINFOID))
        {
            read_size = RKFIO_FRead( &pINFO.Genre[0], info_size , pRawFileCache );
            if (read_size != info_size)
                return PCM_NOT_SUPPORT;

        }
        pPCM->usOffset += subChunkSize;

    }
    return PCM_SUPPORT_FORMAT;
}

/* Get the FOURCC for INFO chunk and processing the INFO chunk */
_ATTR_WAVDEC_TEXT_
long	WAVE_LoadLISTChunk(tPCM *pPCM, unsigned long ChunkSize )
{
    WAVFOURCCID fourccLISTID;
    long	wav_ret;
    long	read_size;
    //By Vincent Hsiung, Jan 16,2008 : Avoid Parse LIST Chunk (Bug exist)
    //---------------------------------------
    pPCM->usOffset +=ChunkSize;
    return PCM_SUPPORT_FORMAT;
    //---------------------------------------

    read_size = RKFIO_FRead( pPCM->pucEncodedData, WAVE_CHUNK_SIZE_BYTE , pRawFileCache );

    if ( read_size != WAVE_CHUNK_SIZE_BYTE)
        return PCM_NOT_SUPPORT;

    /* Get the FOURCC */
    fourccLISTID.Data1 = pPCM->pucEncodedData[0];
    fourccLISTID.Data2 = pPCM->pucEncodedData[1];
    fourccLISTID.Data3 = pPCM->pucEncodedData[2];
    fourccLISTID.Data4 = pPCM->pucEncodedData[3];

    /* update the file offset */
    pPCM->usOffset +=WAVE_CHUNK_SIZE_BYTE;

    /* Check INFO chunk */
    if ( WAVE_EqualFOURCC(&FOURCC_INFO, &fourccLISTID))
    {
        /* load INFO chunk data*/
        wav_ret=WAVE_LoadINFOChunk(pPCM,ChunkSize,INFO_BYTE_SIZE);

        if (wav_ret != PCM_SUPPORT_FORMAT)
            return PCM_NOT_SUPPORT;
    }
    return PCM_SUPPORT_FORMAT;
}
_ATTR_WAVDEC_DATA_
 unsigned char FF_MEDIASUBTYPE_BASE_GUID[] ={
    0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};
_ATTR_WAVDEC_TEXT_
long	WAVE_LoadFMTChunk(tPCM *pPCM,unsigned long ulLength )
{
    PCMWAVEFORMAT sWaveFormat;
    long	read_size;

    /* clear the sWaveFormat structure */
    memset((void *)&sWaveFormat,0, sizeof(sWaveFormat));

    /* Make sure that the entire "fmt " chunk is in the data buffer. */

    RKFIO_FSeek( pPCM->usOffset , 0 , pRawFileCache );

    read_size = RKFIO_FRead( pPCM->pucEncodedData, ulLength , pRawFileCache );

    if (read_size !=ulLength)
    {
        return PCM_NOT_SUPPORT;
    }

    /* update the file offset */
    pPCM->usOffset +=ulLength;

    // Copy the wave format structure from the "fmt " chunk into a
    // local.  We do this instead of grabbing the values directly from
    // the data buffer since the "fmt " chunk can occur with any
    // alignment, making it a lot easier to simply copy the structure.
    //
    if (pPCM->pucEncodedData[0] == WAVE_FORMAT_ADPCM)
    {

        // If the length of the "fmt " chunk is not the same as the size
        // of the MS ADPCM wave format sturcture, then we can not decode
        // this file.
        //
        if (ulLength != sizeof(PCMWAVEFORMAT)-2)	//
        {
            return PCM_NOT_SUPPORT;
        }

        memcpy((void *)&sWaveFormat,
               pPCM->pucEncodedData, sizeof(PCMWAVEFORMAT));

        /* sanity check */
        if ((sWaveFormat.nChannels > 2) ||
                (sWaveFormat.nBlockAlign > 4096) ||
                (sWaveFormat.wBitsPerSample != 4) ||
                (sWaveFormat.cbSize != 32) ||
                (sWaveFormat.wSamplesPerBlock > MSADPCM_MAX_PCM_LENGTH) ||
                (sWaveFormat.wNumCoef != 7))
        {
            //
            // The wave format does not pass the sanity checks, so we can
            // not decode this file.
            //
            return PCM_NOT_SUPPORT;
        }

#if 0
        /* Make sure that the coefficients match the MS ADPCM coefficients. */
        for (read_size = 0; read_size < 7; read_size++)
        {
            if ((sWaveFormat.aCoef[read_size].iCoef1 != psCoefficient1[read_size])
                    || 	(sWaveFormat.aCoef[read_size].iCoef2 !=	psCoefficient2[read_size]))
            {
                // The coefficients do not match the MS ADPCM coefficients,
                // so we can not decode this file.
                return PCM_NOT_SUPPORT;
            }
        }
#endif
    }
    else if ( pPCM->pucEncodedData[0] == WAVE_FORMAT_DVI_ADPCM)
    {
        //
        // If the length of the "fmt " chunk is not the same as the size
        // of the IMA ADPCM wave format sturcture, then we can not decode
        // this file.

        /* 取消这项检查，支持更多损坏的IMA ADPCM文件 //2009-01-13
        if(ulLength != (sizeof(PCMWAVEFORMAT)-0x20))	//2005-11-23
        {
            return PCM_NOT_SUPPORT;
        }
        */

        memcpy((void *)&sWaveFormat, pPCM->pucEncodedData, 20);

        /* sanity check */
        if ((sWaveFormat.nChannels > 2) ||
                (sWaveFormat.nBlockAlign > 4096) ||
                (sWaveFormat.cbSize != 2) ||
                (sWaveFormat.wSamplesPerBlock > IMAADPCM_MAX_PCM_LENGTH))
        {
            //
            // The wave format does not pass the sanity checks, so we can
            // not decode this file.
            //
            return PCM_NOT_SUPPORT;
        }

        /* IMA ADPCM supports 3 or 4 bits per sample. */
        if (!( sWaveFormat.wBitsPerSample ==3 || sWaveFormat.wBitsPerSample ==4 ))
        {
            return PCM_NOT_SUPPORT;

        }
    }
    else if (pPCM->pucEncodedData[0] == WAVE_FORMAT_PCM)
    {
        if(ulLength > sizeof(PCMWAVEFORMAT))
		{
		    	memcpy((void *)&sWaveFormat,
               pPCM->pucEncodedData ,
                sizeof(PCMWAVEFORMAT));

		}
		else
		{

	    	memcpy((void *)&sWaveFormat,
               pPCM->pucEncodedData ,
               ulLength);
		}

        /* sanity check */
        if ((sWaveFormat.nChannels > 2)
                ||(sWaveFormat.nBlockAlign > 4096)
            	||((sWaveFormat.wBitsPerSample != 8)
            	&&(sWaveFormat.wBitsPerSample != 16)
            	&&(sWaveFormat.wBitsPerSample != 24)
            	&&(sWaveFormat.wBitsPerSample != 32)) )
        {
            //
            // The wave format does not pass the sanity checks, so we can
            // not decode this file
            //
            //DEBUG("nBlockAlign =%d ",sWaveFormat.nBlockAlign);
            return PCM_NOT_SUPPORT;
        }
    }
    else if((pPCM->pucEncodedData[1] == 0xFF)&&(pPCM->pucEncodedData[0] == 0xFE))
    {
      WAVEFORMATEXTENSIBLE   sExtenWaveFormat;
      unsigned int *format;

      memcpy((void *)&sExtenWaveFormat,
               pPCM->pucEncodedData, sizeof(WAVEFORMATEXTENSIBLE));
        if (!memcmp(sExtenWaveFormat.SubFormat + 4,FF_MEDIASUBTYPE_BASE_GUID , 12))
        {

          format = (unsigned int *)&sExtenWaveFormat.SubFormat;
          memcpy(&sWaveFormat,&sExtenWaveFormat,sizeof(PCMWAVEFORMAT));
          sWaveFormat.wFormatTag  = *format;
          if(sWaveFormat.wFormatTag == WAVE_FORMAT_PCM)
            {
                if ( (sWaveFormat.nChannels > 2)
                   ||(sWaveFormat.nBlockAlign > 4096)
                	||((sWaveFormat.wBitsPerSample != 8)
                	&&(sWaveFormat.wBitsPerSample != 16)
                	&&(sWaveFormat.wBitsPerSample != 24)
                	&&(sWaveFormat.wBitsPerSample != 32)))
                {

                    //DEBUG("nBlockAlign =%d ",sWaveFormat.nBlockAlign);
                    return PCM_NOT_SUPPORT;
                }
            }
          else
            {
               return PCM_NOT_SUPPORT;
            }

        }
        else
        {
           return PCM_NOT_SUPPORT;
        }


    }
    else
    {
        //DEBUG("TYPE 0X%x  ",pPCM->pucEncodedData[0]);
        return PCM_NOT_SUPPORT;
    }

//--------------------------------------------------------------

    /* Save information about this file which we will need. */
    pPCM->usByteRate = sWaveFormat.nAvgBytesPerSec;
    pPCM->usSampleRate = (unsigned long)sWaveFormat.nSamplesPerSec;
    pPCM->usBytesPerBlock = sWaveFormat.nBlockAlign;

    pPCM->ucChannels = (unsigned char)sWaveFormat.nChannels;
    pPCM->sPCMHeader.ucChannels = (unsigned long)sWaveFormat.nChannels;

    pPCM->usSamplesPerBlock = sWaveFormat.wSamplesPerBlock;
    pPCM->sPCMHeader.usSamplesPerBlock = (unsigned short)sWaveFormat.wSamplesPerBlock;

    pPCM->uBitsPerSample = sWaveFormat.wBitsPerSample;
    pPCM->sPCMHeader.uBitsPerSample = (unsigned short)sWaveFormat.wBitsPerSample;

    pPCM->wFormatTag = sWaveFormat.wFormatTag;

    return PCM_SUPPORT_FORMAT;
}
_ATTR_WAVDEC_TEXT_
unsigned long CHeck_IS_DTS(tPCM *pPCM)
{
   char *buf;
   int i = 0;
   pPCM->usValid = RKFIO_FRead( pPCM->pucEncodedData, MSADPCM_MAX_ENCBUF_LENGTH , pRawFileCache );
   while(i< (MSADPCM_MAX_ENCBUF_LENGTH -6))
    {
       buf = &pPCM->pucEncodedData[i];
       i++;
       /* 14 bits and little endian bitstream */
        if (buf[0] == 0xff && buf[1] == 0x1f &&
            buf[2] == 0x00 && buf[3] == 0xe8 &&
            (buf[4] & 0xf0) == 0xf0 && buf[5] == 0x07)
        {
             return 1;
        }

        /* 14 bits and big endian bitstream */
        else if (buf[0] == 0x1f && buf[1] == 0xff &&
            buf[2] == 0xe8 && buf[3] == 0x00 &&
            buf[4] == 0x07 && (buf[5] & 0xf0) == 0xf0)
        {
            return 1;
        }

        /* 16 bits and little endian bitstream */
        else if (buf[0] == 0xfe && buf[1] == 0x7f &&
            buf[2] == 0x01 && buf[3] == 0x80)
        {
           return 1;
        }

        /* 16 bits and big endian bitstream */
        else  if (buf[0] == 0x7f && buf[1] == 0xfe &&
            buf[2] == 0x80 && buf[3] == 0x01)
        {
            return 1;
        }

    }
   RKFIO_FSeek(pPCM->ulDataStart,SEEK_SET,pRawFileCache);
   return 0;
}
//****************************************************************************
//
// InitPCM prepares play WMA file
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
unsigned long InitPCMDecoder(tPCM *pPCM)
{
    WAVFOURCCID fourccID;
    long	wav_ret;
    unsigned long ulLength;

    /* Seek to the beginning of the file. */
    RKFIO_FSeek( 0 , 0 , pRawFileCache );

    /* Read the first portion of the file. */

    pPCM->usValid = RKFIO_FRead( pPCM->pucEncodedData, WAVE_FIRST_CHUNK_OFFSET , pRawFileCache );

    /* Make sure that this is a RIFF WAVE file. */
    if ((pPCM->usValid < 12) ||
            (pPCM->pucEncodedData[0] != 'R') ||
            (pPCM->pucEncodedData[1] != 'I') ||
            (pPCM->pucEncodedData[2] != 'F') ||
            (pPCM->pucEncodedData[3] != 'F') ||
            (pPCM->pucEncodedData[8] != 'W') ||
            (pPCM->pucEncodedData[9] != 'A') ||
            (pPCM->pucEncodedData[10] != 'V') ||
            (pPCM->pucEncodedData[11] != 'E'))
    {
		//DEBUG();
        return PCM_NOT_SUPPORT;
    }

    //
    // Set the sample rate to the default value.
    // This will be set correctly if we find a "fmt " chunk; if we process
    // all the chunks in the file and the sample rate is still zero,
    // it indicates that we did not find the "fmt " chunk.
    pPCM->usSampleRate = 0;	//2005-01-19

    /* The first chunk is at offset 12. */
    pPCM->usOffset = WAVE_FIRST_CHUNK_OFFSET;

    //
    // Look for chunks in the file.  We need the "fmt " chunk and the "data" chunk.

    while (1)
    {
        /* Get the FOURCC for ChunkID  */
        if (WAVE_LoadChunkFOURCC(pPCM,&fourccID,&ulLength) != PCM_SUPPORT_FORMAT )
        {
			//DEBUG();
            return PCM_NOT_SUPPORT;
        }
        /* Is this the "LIST" chunk? */
        if ( WAVE_EqualFOURCC(&FOURCC_LIST, &fourccID))
        {
            /* Load LIST chunk data */
            wav_ret=WAVE_LoadLISTChunk(pPCM,ulLength);
            if (wav_ret != PCM_SUPPORT_FORMAT)
            {
				//DEBUG();
                return PCM_NOT_SUPPORT;
            }
        }

        /* Is this the "fmt " chunk? */
        else if ( WAVE_EqualFOURCC(&FOURCC_FMT, &fourccID))
        {
            /* Load FMT chunk data */

            wav_ret=WAVE_LoadFMTChunk(pPCM,ulLength);
            if (wav_ret != PCM_SUPPORT_FORMAT)
            {
				//DEBUG();
                return PCM_NOT_SUPPORT;
            }
        }

        /* Is this the "data " chunk? */
        else if (WAVE_EqualFOURCC(&FOURCC_DATA, &fourccID))
        {
            /* Load Data chunk data */
            wav_ret=WAVE_LoadDATAChunk(pPCM,ulLength);

            if (wav_ret != PCM_SUPPORT_FORMAT)
            {
				//DEBUG();
                return PCM_NOT_SUPPORT;
            }
           wav_ret =  CHeck_IS_DTS(pPCM);
            if (wav_ret )
            {
				wav_printf("DTS文件");
                return PCM_NOT_SUPPORT;
            }
            break;
        }

        else
            pPCM->usOffset += ulLength;

    } //while(1)


    //
    // If the sample rate is still zero, then we did not find a "fmt " chunk.
    // Therefore, we can not decode this file.
    //

    switch (pPCM->usSampleRate)
    {
		case 6000:
        case 8000:
        case 11025:
        case 12000:
        case 16000:
        case 22050:
        case 24000:
        case 32000:
        case 44100:
        case 48000:
        case 64000:
        case 88200:
        case 96000:
        case 128000:
        case 176400:
        case 192000:
            break;
        default:
			//DEBUG("采样率 %d \n",pPCM->usSampleRate);
            return PCM_NOT_SUPPORT;

    }

    /* Get the time length of the file.(ms) */
    pPCM->ulTimeLength = ((pPCM->ulLength / pPCM->usByteRate) * 1000) +
                         (((pPCM->ulLength % pPCM->usByteRate) * 1000) / pPCM->usByteRate);

    /* The position is zero. */
    pPCM->ulTimePos = 0;

    /* Initially, all the data in the "data" chunk is valid. */
    pPCM->ulDataValid = pPCM->ulLength;

    RKFIO_FSeek( pPCM->ulDataStart, 0 , pRawFileCache );

    pPCM->usOffset = 0;

    if (pPCM->wFormatTag==WAVE_FORMAT_PCM)
    {
        pPCM->usSamplesPerBlock = 512;
    }
    else
    {
        pPCM->usValid = RKFIO_FRead( pPCM->pucEncodedData, MSADPCM_MAX_ENCBUF_LENGTH , pRawFileCache );
    }

    //GetInitDiff();// ADD BY VINCENT , FOR READ ECY DATA
    /* Success. */
    return PCM_SUPPORT_FORMAT;
}

#if 1

/* moved to msadpcm_enc.c */
#if 0
//****************************************************************************
//
// InitADPCMEncoder prepares to encode a file using the MS ADPCM encoder.
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
void InitADPCMEncoder(tPCM_enc *pPCM)
{
    switch (pPCM->usSampleRate)
    {
        case	8000:
        case	11025:
            pPCM->usBytesPerBlock = 256;
            break;

        case	22050:
            pPCM->usBytesPerBlock = 512;
            break;

        default:
        case	32000:
        case	44100:
            pPCM->usBytesPerBlock = 1024;
    }

    pPCM->usSamplesPerBlock = ((pPCM->usBytesPerBlock << 1) >> (pPCM->ucChannels - 1)) - 12;
    pPCM->usByteRate = pPCM->usSampleRate * pPCM->usBytesPerBlock / pPCM->usSamplesPerBlock;
    pPCM->sPCMHeader.ucChannels = pPCM->ucChannels;
    pPCM->sPCMHeader.usSamplesPerBlock = pPCM->usSamplesPerBlock;

    pPCM->usValid = 90;

    pPCM->ulLength = 0;
    pPCM->ulTimePos = 0;
    pPCM->ulDataValid = 0;
//	pPCM->pOutput = 0;
    pPCM->wFormatTag = WAVE_FORMAT_ADPCM;
}

//****************************************************************************
//
// Init_LPCMEncoder prepares to encode a file using the Linear PCM encoder.
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
void Init_LPCMEncoder( tPCM *pPCM )
{
    /*
    	RKFIO_FWrite((MY_FILE*)TRACK_GetFileHandleOfRecord(), pPCM->pucEncodedData,44);  //MPEG4_INDEX_BUFFERSIZE

    	pPCM->usByteRate = pPCM->usSampleRate * pPCM->ucChannels * 2;

    	pPCM->ulLength = 0;
    	pPCM->ulTimePos = 0;
    	pPCM->ulDataValid = 0;
    	pPCM->usValid = 0;
    	pPCM->usSamplesPerBlock = 2728;
    	pPCM->wFormatTag = WAVE_FORMAT_PCM;
    */
}

//****************************************************************************
//
// The close the Linear PCM encoder.
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
void Close_LPCM(tPCM *pPCM)
{
    /*
    	PCMWAVEFORMAT sWaveFormat;

    	RKFIO_FSeek( 0, 0 , ...);
    	pPCM->pucEncodedData[36] = 'd';
    	pPCM->pucEncodedData[37] = 'a';
    	pPCM->pucEncodedData[38] = 't';
    	pPCM->pucEncodedData[39] = 'a';

    	pPCM->ulDataValid=pPCM->ulDataValid-44;//hug

    	pPCM->pucEncodedData[40] = pPCM->ulDataValid;
    	pPCM->pucEncodedData[41] = pPCM->ulDataValid >> 8;
    	pPCM->pucEncodedData[42] = pPCM->ulDataValid >> 16;
    	pPCM->pucEncodedData[43] = pPCM->ulDataValid >> 24;

    	pPCM->ulDataValid += 36;

    	pPCM->pucEncodedData[0] = 'R';
    	pPCM->pucEncodedData[1] = 'I';
    	pPCM->pucEncodedData[2] = 'F';
    	pPCM->pucEncodedData[3] = 'F';

    	pPCM->pucEncodedData[4] = pPCM->ulDataValid;
    	pPCM->pucEncodedData[5] = pPCM->ulDataValid >> 8;
    	pPCM->pucEncodedData[6] = pPCM->ulDataValid >> 16;
    	pPCM->pucEncodedData[7] = pPCM->ulDataValid >> 24;

    	pPCM->pucEncodedData[8] = 'W';
    	pPCM->pucEncodedData[9] = 'A';
    	pPCM->pucEncodedData[10] = 'V';
    	pPCM->pucEncodedData[11] = 'E';

    	pPCM->pucEncodedData[12] = 'f';
    	pPCM->pucEncodedData[13] = 'm';
    	pPCM->pucEncodedData[14] = 't';
    	pPCM->pucEncodedData[15] = ' ';

    	pPCM->pucEncodedData[16] = 16;	//16 for PCM
    	pPCM->pucEncodedData[17] = 0;
    	pPCM->pucEncodedData[18] = 0;
    	pPCM->pucEncodedData[19] = 0;

    	sWaveFormat.wFormatTag = 1;
    	sWaveFormat.nChannels = pPCM->ucChannels;
    	sWaveFormat.nSamplesPerSec = pPCM->usSampleRate;
    	sWaveFormat.nAvgBytesPerSec = pPCM->usByteRate;
    	sWaveFormat.nBlockAlign = pPCM->ucChannels*2;//*2;//pPCM->usSamplesPerBlock*pPCM->ucChannels*2; //pPCM->ulDataValid;//pPCM->usBytesPerBlock;
    	sWaveFormat.wBitsPerSample = 16;

    	memcpy(pPCM->pucEncodedData + 20, (void *)&sWaveFormat, 16);

    	memcpy(ADPCM_Hdr, pPCM->pucEncodedData, 44);

    	//bug fix by Vincent Hisung
    	RKFIO_FWrite((MY_FILE*)TRACK_GetFileHandleOfRecord(), pPCM->pucEncodedData,44);  //MPEG4_INDEX_BUFFERSIZE
    */
}
#endif

//****************************************************************************
//
// MS-ADPCM format file Decoding
//
//****************************************************************************
_ATTR_WAVDEC_TEXT_
unsigned long MSADPCM_FORMAT(tPCM *pPCM)
{
#if 0
    long lLength, lBlockSize;
    long	generatedSampleSize;
    short *psLeft, *psRight;
    unsigned int Counter = 0,i;

    if (((pPCM->usOffset + pPCM->usBytesPerBlock) > pPCM->usValid)
            && (pPCM->usValid < pPCM->ulDataValid))
    {
        pPCM->usValid -= (pPCM->usOffset & ~3);

        memcpy(pPCM->pucEncodedData, pPCM->pucEncodedData+(pPCM->usOffset&~3), pPCM->usValid);

        pPCM->ulDataValid -= (pPCM->usOffset & ~3);

        pPCM->usValid += RKFIO_FRead(
                             pPCM->pucEncodedData + pPCM->usValid,
                             (MSADPCM_MAX_ENCBUF_LENGTH - pPCM->usValid) & ~511 ,
                             pRawFileCache );

        if (pPCM->usValid > pPCM->ulDataValid)
            pPCM->usValid = pPCM->ulDataValid;

        pPCM->usOffset &= 3;
    }

    if ((pPCM->usValid - pPCM->usOffset) >=	pPCM->usBytesPerBlock)
        lBlockSize = pPCM->usBytesPerBlock;

    else
        lBlockSize = pPCM->usValid - pPCM->usOffset;

    BufferGetWritePointer(pPCM->pOutput, &psLeft, &psRight, &lLength);

    generatedSampleSize = DecodeMSADPCM(&pPCM->sPCMHeader, pPCM->pucEncodedData + pPCM->usOffset ,
                                        psLeft /*+ ((ADPCM_COFE[0]-ADPCM_DIV_COFE[0])&0x0ff)*/,
                                        psRight /*+ ((ADPCM_COFE[0]-ADPCM_DIV_COFE[0])&0x0ff)*/,
                                        lBlockSize);//mod by vincent hisung ,Oct 17,2007

    if (generatedSampleSize == 0)
        return(0);
    //generatedSampleSize+= ((ADPCM_COFE[1]-ADPCM_DIV_COFE[0])&0x0ff);//added by vincent hisung ,Oct 17,2007

    /* Increment the encoded data buffer pointer. */
    pPCM->usOffset += lBlockSize;

    /* Increment the time based on the number of samples. */
    pPCM->ulTimePos += generatedSampleSize;

    return generatedSampleSize;
#endif
}

#endif

#endif
#endif
