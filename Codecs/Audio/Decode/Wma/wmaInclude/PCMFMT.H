//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************

#ifndef __PCMFMT_COM_H__
#define __PCMFMT_COM_H__

#include "wmatypes.h"

typedef enum _PCMData
{
    PCMDataPCM        = 0,
    PCMDataIEEE_FLOAT = 1,
} PCMData;

typedef struct _PCMFormat
{
    U32 nSamplesPerSec;
    U32 nChannels;
    U32 nChannelMask;
    U32 nValidBitsPerSample;
    U32 cbPCMContainerSize;
    PCMData pcmData;
} PCMFormat;
//#pragma pack (pop)

#endif//__PCMFMT_COM_H__

//#ifndef __PCMFMT_H__
//#define __PCMFMT_H__
//
//#ifdef __cplusplus
//extern "C"
//{
//#endif

//#ifdef _WAVEFORMATEX_
//    void WaveFormatEx2PCMFormat(WAVEFORMATEX* wfx, PCMFormat* pFormat);
//    void PCMFormat2WaveFormatEx(PCMFormat* pFormat, WAVEFORMATEX* wfx);
//    Bool ValidateWaveFormat(WAVEFORMATEX* wfx);
//    Bool ValidateUncompressedFormat(WAVEFORMATEX* wfx);
//
//#ifdef _WAVEFORMATEXTENSIBLE_
//    void PCMFormat2WaveFormatExtensible(PCMFormat* pFormat, WAVEFORMATEXTENSIBLE* wfx);
//#endif//_WAVEFORMATEXTENSIBLE_
//#endif//_WAVEFORMATEX_

//#ifdef GUID_DEFINED
//    void FormatTag2GUID(U16 wFormatTag, GUID* pGUID);
//#endif

//    Bool NeedExtensible(U32 nBitsPerSample, U32 nChannels, U32 nChannelMask);
//    Bool PCMFormatEquivalent(PCMFormat*, PCMFormat*);
//    U32 DefaultChannelMask(U32 nChannels);

//#ifdef __cplusplus
//}
//#endif
//
//#endif//__PCMFMT_H__

