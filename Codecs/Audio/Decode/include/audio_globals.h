/*
********************************************************************************
*                   Copyright (C),2004-2015, Fuzhou Rockchip Electronics Co.,Ltd.
*                         All rights reserved.
*
* File Name£º   audio_globals.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             Vincent Hsiung     2009-1-8          1.0
*    desc:    ORG.
********************************************************************************
*/
#include "RKOS.h"

#ifndef _AUDIO_GLOBALS_H_
#define _AUDIO_GLOBALS_H_

#include "audio_main.h"
#include "effect.h"

//****************************************************************************
// The following values are the IOCTLs which are sent to the individual codec
// drivers.
//****************************************************************************

enum
{
    SUBFN_CODEC_GETNAME,
    SUBFN_CODEC_GETARTIST,
    SUBFN_CODEC_GETTITLE,
    SUBFN_CODEC_GETBITRATE,
    SUBFN_CODEC_GETSAMPLERATE,
    SUBFN_CODEC_GETCHANNELS,
    SUBFN_CODEC_GETLENGTH,
    SUBFN_CODEC_GETTIME,
    SUBFN_CODEC_OPEN_DEC,
    SUBFN_CODEC_OPEN_ENC,
    SUBFN_CODEC_GETBUFFER,
    SUBFN_CODEC_SETBUFFER,
    SUBFN_CODEC_DECODE,
    SUBFN_CODEC_ENCODE,
    SUBFN_CODEC_SEEK,
    SUBFN_CODEC_CLOSE,
    SUBFN_CODEC_ZOOM,
    SUBFN_CODEC_GETBPS,
    SUBFN_CODEC_GET_FRAME_LEN,
};

//****************************************************************************
// The following are the flags passed to CodecOpen.
//****************************************************************************
#define CODEC_OPEN_ENCODE       0x00000001
#define CODEC_OPEN_DECODE       0x00000002
#define CODEC_OPEN_PLAYLIST     0x00000004

//****************************************************************************
// The following are the flags passed to CodecSeek.
//****************************************************************************
#define CODEC_SEEK_FF           0x00000000
#define CODEC_SEEK_REW          0x00000001
#define CODEC_SEEK_OTHER        0x00000002

//****************************************************************************
// Function prototypes and global variables.
//****************************************************************************

// pMP3.C
extern unsigned long MP3Function(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pWMA.C
extern unsigned long WMAFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

//pFLAC.C
extern unsigned long FLACDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pAPE.C
extern unsigned long APEDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pMSADPCM.C
extern unsigned long MSADPCMFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                    unsigned long ulParam2,unsigned long ulParam3);

// pWAV.C
extern unsigned long PCMFunction(unsigned long ulIoctl, unsigned long ulParam1,
                                unsigned long ulParam2, unsigned long ulParam3);

//pAAC.c
extern unsigned long
AACDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
               unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long
OGGDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
               unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_APEDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_ALACDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_FLACDecFunction(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);


// pMP3.C
extern unsigned long MP3Function2(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pWMA.C
extern unsigned long WMAFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

//pFLAC.C
extern unsigned long FLACDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pAPE.C
extern unsigned long APEDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                 unsigned long ulParam2,unsigned long ulParam3);

// pMSADPCM.C
extern unsigned long MSADPCMFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                    unsigned long ulParam2,unsigned long ulParam3);

// pWAV.C
extern unsigned long PCMFunction2(unsigned long ulIoctl, unsigned long ulParam1,
                                unsigned long ulParam2, unsigned long ulParam3);

//pAAC.c
extern unsigned long
AACDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
               unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long
OGGDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
               unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_APEDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_ALACDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);
extern unsigned long  HIFI_FLACDecFunction2(unsigned long ulSubFn, unsigned long ulParam1,
                                              unsigned long ulParam2, unsigned long ulParam3);



// From codec.c
///////////////////////////////////////////////////////////////////////////////
extern unsigned long CodecOpen(unsigned long ulCodec, unsigned long ulFlags);
extern unsigned long CodecGetSampleRate(unsigned long *pulSampleRate);
extern unsigned long CodecGetChannels(unsigned long *pulChannels);
extern unsigned long CodecGetLength(unsigned long *pulLength);
extern unsigned long CodecGetTime(unsigned long *pulTime);
extern unsigned long CodecGetCaptureBuffer(short *ppsBuffer, long *plLength);
extern unsigned long CodecSetBuffer(short *psBuffer);
extern unsigned long CodecDecode(void);
extern unsigned long CodecEncode(void);
extern unsigned long CodecSeek(unsigned long ulTime, unsigned long ulSeekType);
extern unsigned long CodecClose(void);
extern unsigned long CodecGetBitrate(unsigned long *pulBitrate);
extern unsigned long CodecGetFrameLen(long *plLength);

//------------------------------------------------------------
enum {
    CODEC_MP3_DEC = 0,
#ifdef  WMA_DEC_INCLUDE
    CODEC_WMA_DEC,
#endif

#ifdef  AAC_DEC_INCLUDE
    CODEC_AAC_DEC,
#endif

#ifdef WAV_DEC_INCLUDE
    CODEC_WAV_DEC,
#endif

#ifdef APE_DEC_INCLUDE
    CODEC_APE_DEC,
#endif

#ifdef FLAC_DEC_INCLUDE
    CODEC_FLAC_DEC,
#endif

#ifdef OGG_DEC_INCLUDE
    CODEC_OGG_DEC,
#endif

#ifdef HIFI_APE_DECODE
   CODEC_HIFI_APE_DEC,
#endif

#ifdef HIFI_FlAC_DECODE
  CODEC_HIFI_FLAC_DEC,
#endif

#ifdef HIFI_AlAC_DECODE
  CODEC_HIFI_ALAC_DEC,
#endif

    NUMCODECS
};

//-------------------------------------------------------------

typedef struct mediaBlock
{
    unsigned long CurrentPlayTime;
    unsigned long TotalPlayTime;
    unsigned int BitRate;
    unsigned int Bps;
    unsigned int SampleRate;
    unsigned int Channel;
    unsigned int Total_length;
    unsigned int Outptr;
    unsigned int OutLength;
    unsigned int DecodeOver;
    unsigned int Decoding;
    unsigned int DecodeErr;
    unsigned int needDecode;

}MediaBlock;



typedef struct _FILE_HANDLE_
{
    unsigned char handle1;
    unsigned char handle2;
    unsigned char handle3;
    unsigned long filesize;
    unsigned long curfileoffset[3];
    int           codecType;

}FILE_HANDLE_t;

typedef struct _RecFileHdl_t
{
    short        fileHandle;
    unsigned int fileOffset;
}RecFileHdl_t;


typedef struct _RecordBlock
{
    unsigned int enc_bufptr;        //encoded data buffer pointer
    unsigned int unenc_bufptr;      //unencode data buffer pointer
    unsigned int length;            //unencode data buffer length
    unsigned int encodeOver;        //flag of encode if or not done
    unsigned int encoding;          //flag of encode status
    unsigned int encodeErr;         //flag of encode error status
    unsigned int PCM_source;        //pcm struct data

}RecordBlock;


typedef struct _FILE_SEEK_OP
{
    int offset;
    unsigned int whence;
    unsigned long handle;
}FILE_SEEK_OP_t;

typedef struct _FILE_READ_OP
{
    unsigned char *pData;
    unsigned int  NumBytes;
    unsigned long handle;
}FILE_READ_OP_t;

typedef struct _FILE_WRITE_OP
{
    unsigned char *buf;
    unsigned int fileOffset;
    unsigned int size;
    unsigned long handle;
}FILE_WRITE_OP_t;

typedef struct _FLAC_SEEKFAST_OP
{
    int offset;
    int clus;
    int fp;
}FLAC_SEEKFAST_OP_t;

typedef struct _AUDIO_EQ_ARG
{
    int32  *audioLen;
    uint32 *playVolume;
    uint32 *isEQUpdate;

}AUDIO_EQ_ARG;

typedef struct
{
    short  *pPCMBuf;

    short  *EncOutBuf;

    long   flag;

    RKEffect EffectCtl;
}AudioInOut_Type;

#define TRACK_INFO_STRING_LEN   32
typedef struct
{
    unsigned long  TotalTime;
    unsigned long  CurrentTime;
    int            samplerate;
    int            bitrate;
    int            channels;
    int            total_length;
    int            cur_pos;
    int            RepeatMode;
    int            PlayOrder; //lyrics or order
    int            AudioFileDirOrAll;//directory files or all files.
    RKEffect       UserEQ;
    char           artist[TRACK_INFO_STRING_LEN];
    char           title[TRACK_INFO_STRING_LEN];
    char           albumb[TRACK_INFO_STRING_LEN];
    int            IsEQUpdate;
    int            bps;
}track_info;

extern  uint32 gDecCmd;
extern  uint32 gDecData;
extern pSemaphore osAudioDecodeOk;
extern HDC hAudioDma;
extern  uint32 gError;
extern  uint32 gACKDone;
//extern AudioInOut_Type AudioIOBuf;

extern  FILE_SEEK_OP_t    *gpFileSeekParm;
extern  FILE_READ_OP_t    *gpFileReadParm;
extern  FILE_WRITE_OP_t   *gpFileWriteParm;
extern  FLAC_SEEKFAST_OP_t *gpFlacSeekFastParm;

extern  MediaBlock    gpMediaBlock;


extern unsigned char  * DecDataBuf[2];

extern unsigned char DecBufID;


//global variables
extern int CurrentCodec;

#endif
