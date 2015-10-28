//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
//+-------------------------------------------------------------------------
//
//  Microsoft Windows Media
//
//  Copyright (C) Microsoft Corporation, 1999 - 1999
//
//  File:       wmftypes.h
//
//--------------------------------------------------------------------------

#ifndef WMFTYPES_H
#define WMFTYPES_H
//#pragma pack (1)

#ifdef _MSC_VER
#define USE_PACK_HEADERS
#define PACKED
#elif defined(macintosh) || defined(HITACHI)
#define PACKED
#elif defined (_Embedded_x86) || defined(__arm)
//#include "wmc_type.h"
#define PACKED
#else
#define PACKED  __packed
#endif

#ifdef USE_PACK_HEADERS
#include <pshpack1.h>
#endif

/****************************************************************************/
#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#if (defined(_M_MRX000) || defined(_M_ALPHA) || defined(_M_PPC) || defined(UNDER_CE)) && !defined(x86)
#define UNALIGNED __unaligned
#else
#define UNALIGNED
#endif



/****************************************************************************/
//#ifndef __LONG_DEFINED__
//#define __LONG_DEFINED__
//typedef long LONG;
//#endif

#ifdef _Embedded_x86
typedef unsigned long ULONG;
#define BYTE U8_WMC
#define DWORD U32_WMC
#define BOOL Bool_WMC
#endif
#ifndef __QU16_WMC_DEFINED__
#define __QU16_WMC_DEFINED__
#endif

/****************************************************************************/
#ifndef GUID_DEFINED
#define GUID_DEFINED

typedef  struct tGUID
{
    U32_WMC Data1;
    U16_WMC Data2;
    U16_WMC Data3;
    U8_WMC  Data4[ 8 ];

}   GUID;

#endif

//#ifdef _XBOX
#if !defined(_WINGDI_H_) && !defined(_WINGDI_) // this is for now 03/21/2002 Need other method
#ifndef _BITMAPINFOHEADER
#define _BITMAPINFOHEADER
typedef  struct tag1BITMAPINFOHEADER{ // bmih
    U32_WMC  biSize;
    I32_WMC   biWidth;
    I32_WMC   biHeight;
    U16_WMC   biPlanes;
    U16_WMC   biBitCount;
    U32_WMC  biCompression;
    U32_WMC  biSizeImage;
    I32_WMC   biXPelsPerMeter;
    I32_WMC   biYPelsPerMeter;
    U32_WMC  biClrUsed;
    U32_WMC  biClrImportant;
} BITMAPINFOHEADER;
#endif // _BITMAPINFOHEADER
#endif // _WINGDI_
//#endif // xbox

/****************************************************************************/
#ifndef _WAVEFORMATEX_
#define _WAVEFORMATEX_


#ifndef _XBOX
typedef     struct tWAVEFORMATEX
{
    U16_WMC    wFormatTag;         /* format type */
    U16_WMC    nChannels;          /* number of channels (i.e. mono, stereo...) */
    U32_WMC   nSamplesPerSec;     /* sample rate */
    U32_WMC   nAvgBytesPerSec;    /* for buffer estimation */
    U16_WMC    nBlockAlign;        /* block size of data */
    U16_WMC    wBitsPerSample;     /* number of bits per sample of mono data */
    U16_WMC    cbSize;             /* the count in bytes of the size of */

}   WAVEFORMATEX, *PWAVEFORMATEX, *NPWAVEFORMATEX, *LPWAVEFORMATEX;
typedef const WAVEFORMATEX *LPCWAVEFORMATEX;
#endif
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef  struct tWAVEFORMATEXTENSIBLE
{
    WAVEFORMATEX Format;       /* The traditional wave file header */
    union {
        unsigned short wValidBitsPerSample; /* bits of precision */
        unsigned short wSamplesPerBlock;    /* valid if wBitsPerSample==0 */
        unsigned short wReserved;           /* If neither applies, set to zero */
    } Samples;
    unsigned long    dwChannelMask;        /* which channels are present in stream */
    GUID SubFormat;           /* specialization */
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE,  *NPWAVEFORMATEXTENSIBLE, *LPWAVEFORMATEXTENSIBLE;
typedef const WAVEFORMATEXTENSIBLE *LPCWAVEFORMATEXTENSIBLE;
#endif // _WAVEFORMATEXTENSIBLE_

#ifdef _Embedded_x86
//#ifndef _BITMAPINFOHEADER
//#define _BITMAPINFOHEADER
//typedef  struct tagBITMAPINFOHEADER{
//        U32_WMC      biSize;
//        I32_WMC       biWidth;
//        I32_WMC       biHeight;
//        U16_WMC       biPlanes;
//        U16_WMC       biBitCount;
//        U32_WMC      biCompression;
//        U32_WMC      biSizeImage;
//        I32_WMC       biXPelsPerMeter;
//        I32_WMC       biYPelsPerMeter;
//        U32_WMC      biClrUsed;
//        U32_WMC      biClrImportant;
//} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
//
//#endif
#ifndef _COLORSCHEME
#define _COLORSCHEME
#define BI_RGB  0
#define BI_BITFIELDS  3L
#endif
#define INT int
#define LPVOID void*
#define FALSE 0
#define FAILED(Status) ((HRESULT)(Status)<0)

#endif



#define WAVE_FORMAT_MSAUDIO1  0x0160
typedef    struct tMSAUDIO1WAVEFORMAT
{
    WAVEFORMATEX wfx;
    U16_WMC         wSamplesPerBlock;
    U16_WMC         wEncodeOptions;

}   MSAUDIO1WAVEFORMAT;

#define WAVE_FORMAT_WMAUDIO2  0x0161
typedef  struct tWMAUDIO2WAVEFORMAT
{
    WAVEFORMATEX wfx;
    U32_WMC        dwSamplesPerBlock;
    U16_WMC         wEncodeOptions;
    U32_WMC        dwSuperBlockAlign;

}   WMAUDIO2WAVEFORMAT;

#if !defined (WAVE_FORMAT_WMAUDIO3)
#define WAVE_FORMAT_WMAUDIO3  0x0162

typedef     struct wmaudio3waveformat_tag {
  WAVEFORMATEX wfx;
  U16_WMC         wValidBitsPerSample; // bits of precision
  U32_WMC        dwChannelMask;       // which channels are present in stream
  U32_WMC        dwReserved1;
  U32_WMC        dwReserved2;
  U16_WMC         wEncodeOptions;
  U16_WMC         wReserved3;
} WMAUDIO3WAVEFORMAT;
#endif
#if !defined (WAVE_FORMAT_WMAUDIO4)
#define WAVE_FORMAT_WMAUDIO4   0x163
#endif

#define WMAUDIO2_WFX_EXTRA_BYTES    (sizeof(WMAUDIO2WAVEFORMAT) - sizeof(WAVEFORMATEX))

#define WAVE_FORMAT_MSSPEECH  10

/****************************************************************************/


typedef  struct tAsfXAcmAudioErrorMaskingData
{
    U8_WMC    span;
    U16_WMC    virtualPacketLen;
    U16_WMC    virtualChunkLen;
    U16_WMC    silenceLen;
    U8_WMC    silence[1];

}   AsfXAcmAudioErrorMaskingData;


/****************************************************************************/
typedef  struct tAsfXSignatureAudioErrorMaskingData
{
    U32_WMC maxObjectSize;
    U16_WMC  chunkLen;
    U16_WMC  signatureLen;
    U8_WMC  signature[1];

}   AsfXSignatureAudioErrorMaskingData;
/****************************************************************************/
// Added by Amit on 02/24/00 for Video Parsing
typedef  struct tAsfXGenericImageTypeSpecific
{

    U32_WMC       windowWidth;
    U32_WMC       windowHeight;
    U8_WMC        flags;
    U16_WMC        imageInfoLen;
    U8_WMC        imageInfo[1];
}AsfXGenericImageTypeSpecific;

/****************************************************************************/
typedef     struct tPACKET_PARSE_INFO_EX
{
    Bool_WMC fParityPacket;
    U32_WMC cbParseOffset;

    Bool_WMC fEccPresent;
    U8_WMC bECLen;

    U8_WMC bPacketLenType;
    U32_WMC cbPacketLenTypeOffset;

    U8_WMC bPadLenType;
    U8_WMC bSequenceLenType;

    Bool_WMC fMultiPayloads;

    U8_WMC bOffsetBytes;

    U8_WMC bOffsetLenType;

    U32_WMC cbPacketLenOffset;

    U32_WMC cbExplicitPacketLength;

    U32_WMC cbSequenceOffset;

    U32_WMC dwSequenceNum;

    U32_WMC cbPadLenOffset;
    U32_WMC cbPadding;

    U32_WMC dwSCR;
    U16_WMC wDuration;

    U8_WMC bPayLenType;
    U8_WMC bPayBytes;
    U32_WMC cbPayLenTypeOffset;

    U32_WMC cPayloads;

}   PACKET_PARSE_INFO_EX;


/****************************************************************************/
typedef     struct tPAYLOAD_MAP_ENTRY_EX
{
    U16_WMC cbPacketOffset;
    U16_WMC cbTotalSize;

    U8_WMC bStreamId;
    U8_WMC bObjectId;

    U32_WMC cbObjectOffset;
    U32_WMC cbObjectSize;
    U32_WMC msObjectPres;

    U8_WMC cbRepData;
    U8_WMC bJunk;
    U16_WMC cbPayloadSize;
    U8_WMC bIsKeyFrame;
    U8_WMC bFrameRate;
    U16_WMC wTotalDataBytes;
    U32_WMC dwDeltaPresTime;
    U8_WMC bIsCompressedPayload;

    U16_WMC wBytesRead;
    U8_WMC bSubPayloadState;
    U8_WMC bNextSubPayloadSize;
    U16_WMC wSubpayloadLeft;
    U16_WMC  wSubCount;

}   PAYLOAD_MAP_ENTRY_EX;


#ifdef USE_PACK_HEADERS
#include <poppack.h>
#endif
//#pragma pack ()

#endif  /* WMFTYPES_H */

