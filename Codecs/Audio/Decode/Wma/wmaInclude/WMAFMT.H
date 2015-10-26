//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************


//#include "wmatyps.h"
#include "pcmfmt.h"
#include "config.h"

#ifndef __WMAFMT_COM_H__
#define __WMAFMT_COM_H__

//#pragma pack (push)
//#pragma pack () // default
typedef struct _WMAFormat
{
    WMA_U16 wFormatTag;
    WMA_U16 nChannels;
    WMA_U32 nSamplesPerSec;
    WMA_U32 nAvgBytesPerSec;
    WMA_U16 nBlockAlign;
    WMA_U16 nValidBitsPerSample;
    WMA_U32 nChannelMask;
    WMA_U16 wEncodeOpt;
} WMAFormat;
//#pragma pack (pop)
#endif // __WMAFMT_COM_H__

//#ifndef __WMAFMT_H__
//#define __WMAFMT_H__

//#ifdef __cplusplus
//extern "C"
//{
//#endif

//#ifdef _WAVEFORMATEX_
//    void WaveFormat2WMAFormat(WAVEFORMATEX* wfx, WMAFormat* wmafmt);
//    void WMAFormatInfo2WaveFormat(WMAFormatInfo* wmafmt, WAVEFORMATEX* wfx);
//#endif
//
//    void WMAFormatInfo2WMAFormat(WMAFormatInfo* pInfo, WMAFormat* pFormat);
//    void WMAFormat2PCMFormat(WMAFormat* pSrc, PCMFormat* pDst);
//    WMA_Bool ValidateWMAWaveFormat(WMA_U8* pb, WMA_U32 cb);
//    WMA_U32 WMAExtraDataSizeFromFormatTag(WMA_U16 wFormatTag);

//#ifdef __cplusplus
//}
//#endif

//#endif//__WMAFMT_H__

