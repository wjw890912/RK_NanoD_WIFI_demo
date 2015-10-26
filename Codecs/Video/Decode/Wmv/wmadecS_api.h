//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
//#pragma pack (1)

#ifndef _WMADECRAW_API_H_
#define _WMADECRAW_API_H_


#include "wmaerror.h"

#define WMARAW_SUCCEEDED WMA_SUCCEEDED
#define WMARAW_FAILED WMA_FAILED

#define WMA_E_NOMOREINPUT WMA_E_NO_MORE_SRCDATA
#define WMA_E_LOSTPACKET WMA_S_LOSTPACKET

// define basic types
typedef unsigned long U32_WMARawDec;
typedef long I32_WMARawDec;
typedef unsigned short U16_WMARawDec;
typedef short I16_WMARawDec;
typedef unsigned char U8_WMARawDec;
typedef __int64 I64_WMARawDec;



typedef void* WMARawDecHandle;

#include "playropt.h"
#include "wmatypes.h"


typedef U32UP U32UP_WMARawDec;
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus


WMARESULT WMARawDecInit (
    WMARawDecHandle* phWMARawDec,
    U32_WMARawDec dwUserData,
    U16_WMARawDec iVersionNumber,
    U16_WMARawDec cSamplesPerBlock,
    U32_WMARawDec cSamplesPerSecond,
    U16_WMARawDec cChannel,
    U32_WMARawDec nBytePerSample,
    U16_WMARawDec nValidBitsPerSample,
    U32_WMARawDec nChannelMask,
    U16_WMARawDec cBytePerSec,
    U16_WMARawDec cPacketLength,
    U16_WMARawDec cEncodeOpt,
    WMAPlayerInfo* pPlayerInfo
);



 WMARESULT WMARawDecClose (WMARawDecHandle* phWMARawDec);

 WMARESULT WMARawDecReset (WMARawDecHandle hWMARawDec);

 WMARESULT WMARawDecStatus (WMARawDecHandle hWMARawDec);

 WMARESULT WMARawDecDecodeData (WMARawDecHandle hWMARawDec, U32_WMARawDec *pnDecodedSamples, I32_WMARawDec *pnSeekAdj);

 WMARESULT WMARawDecGetPCM (WMARawDecHandle hWMARawDec, U16_WMARawDec* pcSampleReady, U8_WMARawDec* pbDst, U32_WMARawDec cbDstBufferLength, I64_WMARawDec* piTimeStamp, WMAPlayerInfo *pWmaPI);


  int WMARawDecCBGetData (U8_WMARawDec **ppBuffer, U32_WMARawDec *pcbBuffer, U32_WMARawDec dwUserData);


#ifdef __cplusplus
}
#endif  // __cplusplus


#endif // _WMADECRAW_API_H_
//#pragma pack ()

