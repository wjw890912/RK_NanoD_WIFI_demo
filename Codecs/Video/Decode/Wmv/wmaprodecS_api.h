//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef _WMAPRODECRAW_API_H_
#define _WMAPRODECRAW_API_H_

// define basic types
typedef int WMARESULT;
typedef unsigned long WMA_U32;
typedef long WMA_I32;
typedef unsigned short WMA_U16;
typedef short WMA_I16;
typedef unsigned char WMA_U8;
typedef void  WMA_VOID;
typedef __int64 WMA_I64;
typedef WMA_I32 WMA_Bool;

#ifndef __PLAYROPT_H__
#define __PLAYROPT_H__
//#pragma pack (1)

// **************************************************************************
// Player Options
// **************************************************************************
#define PLAYOPT_HALFTRANSFORM      0x0002
#define PLAYOPT_PAD2XTRANSFORM     0x0008
#define PLAYOPT_DYNAMICRANGECOMPR  0x0080
#define PLAYOPT_LTRT               0x0100
// defines for WMAVoice
// in this CPU_RESOURCE field, 0 and 4 both equate to a 206 MHz StrongARM level of cpu performance.
// Some future release may implement auomatic adaption for 0 but not for 4.
// 1 is suitable for an ARM7 or ARM9 implementation that runs at about 70 MHz.
// other CPU's should probably pass 0 but can try other values.
// the codec may apply higher levels of post processing for higher levels of this value which will result in higher quality.
// However, raising the number too far will cause the codec to try too complex post processing and result in stuttering audio.
#define PLAYOPT_CPU_RESOURCE_MASK   0x000F0000
#define PLAYOPT_CPU_RESOURCE_AUTO   0
#define PLAYOPT_CPU_RESOURCE(a) ((a)<<16)
#define PLAYOPT_CPU_RESOURCE_LOW    PLAYOPT_CPU_RESOURCE(2)
#define PLAYOPT_CPU_RESOURCE_NORMAL PLAYOPT_CPU_RESOURCE(8)

// **************************************************************************
// Dynamic Range Control settings
// **************************************************************************
#define WMA_DRC_HIGH 0
#define WMA_DRC_MED 1
#define WMA_DRC_LOW 2

// **************************************************************************
// Player Info Structure
// **************************************************************************

typedef  struct _WMAPlayerInfo
{
    WMA_U16 nPlayerOpt;        // Bit fields based on above defines
    WMA_I32 *rgiMixDownMatrix; // Can be null to invoke defaults
    WMA_I32 iPeakAmplitudeRef;
    WMA_I32 iRmsAmplitudeRef;
    WMA_I32 iPeakAmplitudeTarget;
    WMA_I32 iRmsAmplitudeTarget;
    WMA_I16 nDRCSetting;       // Dynamic range control setting
} WMAPlayerInfo;

#endif//__PLAYROPT_H__


//PCMFormat
#ifndef __PCMFMT_COM_H__
#define __PCMFMT_COM_H__

typedef     enum _PCMData {
    PCMDataPCM        = 0,
    PCMDataIEEE_FLOAT = 1,
} PCMData;

#pragma pack (push)
//#pragma pack () // default
typedef  struct _PCMFormat {
    WMA_U32 nSamplesPerSec;
    WMA_U32 nChannels;
    WMA_U32 nChannelMask;
    WMA_U32 nValidBitsPerSample;
    WMA_U32 cbPCMContainerSize;
    PCMData pcmData;
} PCMFormat;
#pragma pack (pop)

#endif//__PCMFMT_COM_H__

//WMAFORMAT
#ifndef __WMAFMT_COM_H__
#define __WMAFMT_COM_H__

#pragma pack (push)
//#pragma pack () // default
typedef  struct _WMAFormat {
    WMA_U16 wFormatTag;
    WMA_U16 nChannels;
    WMA_U32 nSamplesPerSec;
    WMA_U32 nAvgBytesPerSec;
    WMA_U16 nBlockAlign;
    WMA_U16 nValidBitsPerSample;
    WMA_U32 nChannelMask;
    WMA_U16 wEncodeOpt;
} WMAFormat;
#pragma pack (pop)

#endif//__WMAFMT_COM_H__

/// WMA Decode Interface
typedef void* WMAProRawDecHandle;

typedef  struct _WmaProGetDataParam
{
    WMA_U32  m_dwUser;
    WMA_Bool m_fTimeIsValid;
    WMA_I64  m_iTime;
} WmaProGetDataParam;

typedef     enum _WMAProRawDecState {
  WMAProRawDecStateDone = 0,
  WMAProRawDecStateInput,
  WMAProRawDecStateDecode,
  WMAProRawDecStateGetPCM
} WMAProRawDecState;

typedef  struct _WMAProRawDecInputBufferInfo {
    WMA_U8*   pbIn;
    WMA_U32   cbIn;
    WMA_Bool  fNewPacket;
    WMA_Bool  fNoMoreInput;
    WMA_Bool  fTime;
    WMA_I64   rtTime;
} WMAProRawDecInputBufferInfo;

typedef WMARESULT (*PFNWMAPROGETMOREDATA)(WMAProRawDecInputBufferInfo*);

// If you use any of the structures below, please memset() the entire struct to 0 before setting its fields.
// This will ensure reasonable default behavior if a new member is added but the calling code is not updated.
typedef  struct _WMAProRawDecInitParams {
    WMA_I32 iMemBufUsed;
    PFNWMAPROGETMOREDATA pfnGetMoreData;
    WMA_U32 dwUser;
    WMA_Bool fSPDIF;
} WMAProRawDecInitParams;

typedef    struct _WMAProRawDecInputParams {
    WMA_I32 cSkipBits;         // SPDIF only
} WMAProRawDecInputParams;

typedef    struct _WMAProRawDecDecodeParams {
    WMA_I32 cbFrameBytesInLastBuffer; // SPDIF only
} WMAProRawDecDecodeParams;

typedef    struct _WMAProRawDecGetPCMParams {
    WMA_U32 dwReserved;
} WMAProRawDecGetPCMParams;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

WMARESULT WMAProRawDecNew (
    WMAProRawDecHandle* pphWMAProRawDec,
    void *pMemBuf,
    const WMA_I32 iMemBufSize
);

WMARESULT WMAProRawDecInit (
    WMAProRawDecHandle phWMAProRawDec,
    WMAFormat* pWMAFormat,
    PCMFormat* pPCMFormat,
    WMAPlayerInfo *pPI,
    WMAProRawDecState *paudecState,
    WMAProRawDecInitParams* pParams
);

WMARESULT WMAProRawDecDelete (WMAProRawDecHandle phWMAProRawDec);

WMARESULT WMAProRawDecReset (WMAProRawDecHandle phWMAProRawDec);

WMARESULT WMAProRawDecCheckInitParams (
    WMAFormat* pWMAFormat,
    PCMFormat* pPCMFormat,
    WMAPlayerInfo *pPI,
    WMAProRawDecInitParams* pParams
);

WMARESULT WMAProRawDecInput (
    WMAProRawDecHandle phWMAProRawDec,
    WMA_U8* pbIn,
    WMA_U32 cbIn,
    WMA_Bool fNewPacket,
    WMA_Bool fNoMoreInput,
    WMA_Bool fTime,
    WMA_I64 rtTime,
    WMAProRawDecState* paudecState,
    WMAProRawDecInputParams* pParams
);

WMARESULT WMAProRawDecDecode (
    WMAProRawDecHandle phWMAProRawDec,
    WMA_U32* pcSamplesReady,
    WMAProRawDecState* paudecState,
    WMAProRawDecDecodeParams* pParams
);

WMARESULT WMAProRawDecGetPCM (
    WMAProRawDecHandle phWMAProRawDec,
    WMA_U32 cSamplesRequested,
    WMA_U32 *pcSamplesReturned,
    WMA_U8* pbDst,
    WMA_U32 cbDstLength,
    WMA_U32* pcbDstUsed,
    WMA_I64* prtTime, // time stamp (in 100ns) of 1st sample returned
    WMAProRawDecState *paudecState,
    WMAPlayerInfo *pWmaPI,
    WMAProRawDecGetPCMParams* pParams
);


///////////////////////////////////////////
// Helper functions
///////////////////////////////////////////
WMA_I32 WMAProGetSamplePerFrame (
    WMA_I32   cSamplePerSec,
    WMA_U32   dwBitPerSec,
    WMA_I32   cNumChannels,
    WMA_I32   iVersion,
    WMA_U16   wEncOpt
);

WMA_U32  WMAProDefaultChannelMask(WMA_U32 nChannels);
WMA_VOID WMAProWMAFormat2PCMFormat(WMAFormat* pSrc, PCMFormat* pDst);

#ifdef __cplusplus
}
#endif  // __cplusplus


#endif // _WMAPRODECRAW_API_H_
//#pragma pack ()

