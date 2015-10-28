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
//  File:       wmadec.h
//
//--------------------------------------------------------------------------
//#pragma pack (1)

#ifndef WMADEC_H
#define WMADEC_H
/*
#if !defined(_Embedded_x86) && !defined(HITACHI)
#ifdef macintosh
#include <MacTypes.h>
#include <Errors.h>
#include <Endian.h>
#include <MacMemory.h>
#define BIGENDIAN
#elif defined(_XBOX)
#include <xtl.h>
#else
////#include <windows.h>
//#include <memory.h>
//#include <mmsystem.h>
#endif
#endif

#include "wmftypes.h"

//#include "wmadec_api.h"
#include "wmadecS_api.h"
#ifndef WMCAPI_NO_DRM
#ifdef _ZMDRM_
#include "drmmanager_api.h"
#else
#include "drmbase.h"
#include "drmpd_ext.h"
#endif
#endif
#include "wmc_type.h"
#include "wmcdecode.h"
#include "wmcguids.h" */

#define WMCDEMO_TIME  60000


typedef    struct tPacketInfo
{
    U32_WMC nSamplesPerSec;
    U64_WMC cbPacketOffset;
    U64_WMC cbLastPacketOffset;
    U32_WMC cbPacketSize;
    U32_WMC cbMaxAudioSize;
    U16_WMC nChannels;
    U32_WMC fccHandler;
    U32_WMC dwScale;
    U32_WMC dwRate;
    BITMAPINFOHEADER BitmapInfo;
    WAVEFORMATEX        tWaveFormat;
    U8_WMC  bHasValidVideo;
    U8_WMC  bHasValidAudio;
    U8_WMC  bInit;
} WMINITINFO;

#ifndef WMCAPI_NO_DRM
#include "drmpd_ext.h"
#endif /* WMCAPI_NO_DRM */

typedef    struct tagStreamNames
{
    U16_WMC u16LangIdIndex;
    U16_WMC u16StreamNamelen;

    /* these are two-byte strings
     *   for ASCII-type character set, to convert these
     *   into a single-byte characters, simply skip
     *   over every other bytes.
     */
    U16Char_WMC *pchStreamName;
} STREAMNAMES;

typedef    struct tagPayloadExtnSystems
{
    GUID_WMC GuidExtnSystemId;
    U16_WMC u16ExtnDataSize;
    U32_WMC u32ExtnSystemInfoLen;
    U8_WMC *pchExtnSystemInfo;
} PAYLOADEXTNSYSTEM;


typedef    struct tExtendedStreamProperties
{
    U64_WMC u64StartTime;
    U64_WMC u64EndTime;
    U32_WMC u32AvDataBitrate;
    U32_WMC u32AvBufferSize;
    U32_WMC u32AvInitBufferFullness;
    U32_WMC u32AltDataBitrate;
    U32_WMC u32AltBufferSize;
    U32_WMC u32AltInitBufferFullness;
    U32_WMC u32MaxObjectSize;
    U32_WMC u32Flags;
    U16_WMC u16StreamId;
    U16_WMC u16StreamLangIdIndex;
    U64_WMC u64AvTimePerFrame;
    U16_WMC u16StreamNameCount;
    U16_WMC u16PayloadExtnSystemCount;
    STREAMNAMES *pStreamNames;
    PAYLOADEXTNSYSTEM *pPayloadExtnSystems;
} EXTENDEDSTREAMPROPERTIES;

typedef     enum tagWMCParseState
{
    csWMC_NotValid = 0,
    csWMCHeaderStart,
    csWMCHeaderError,
    csWMCNewAsfPacket,

    /* packet parsing states */

    csWMCDecodePayloadStart,
    csWMCDecodePayload,
    csWMCDecodePayloadHeader,
    csWMCDecodeLoopStart,
    csWMCDecodePayloadEnd,
    csWMCDecodeCompressedPayload,
    csWMCEnd
} tWMCParseState;

typedef    struct tBitRateInfo
{
    U16_WMC   wStreamId;
    U32_WMC   dwBitRate;
} BITRATEINFO;

#if 0 //sch2014610
typedef    struct tHeaderInfo
{
    U64_WMC     cbPacketOffset;
    U64_WMC     cbLastPacketOffset;
    U64_WMC     cbCurrentPacketOffset;
    U64_WMC     cbFirstPacketOffset;
    U32_WMC     cbHeader;
    U64_WMC     cbNextPacketOffset;
    U32_WMC     cbPacketSize;
    U32_WMC     cPackets;
    U32_WMC     msDuration;
    U32_WMC     msPreroll;
    U32_WMC     msSendDuration;
    U32_WMC     dwMaxBitrate;
    U16_WMC     wNoOfStreams;
    U16_WMC     wNoOfAudioStreams;
    U16_WMC     wNoOfVideoStreams;
    U16_WMC     wNoOfBinaryStreams;

    BITRATEINFO tBitRateInfo[127];
    PACKET_PARSE_INFO_EX ppex;

    PAYLOAD_MAP_ENTRY_EX payload;
    U32_WMC     iPayload;
    U16_WMC     wPayStart;
    Bool_WMC    bHasBitRateInfo;
    U32_WMC     dwPayloadOffset;
} HEADERINFO;
#else
typedef    struct tHeaderInfo
{
    U32_WMC     cbPacketOffset;
    U32_WMC     cbLastPacketOffset;
    U32_WMC     cbCurrentPacketOffset;
    U32_WMC     cbFirstPacketOffset;
    U32_WMC     cbHeader;
    U32_WMC     cbNextPacketOffset;
    U32_WMC     cbPacketSize;
    U32_WMC     cPackets;
    U32_WMC     msDuration;
    U32_WMC     msPreroll;
    U32_WMC     msSendDuration;
    U32_WMC     dwMaxBitrate;
    U16_WMC     wNoOfStreams;
    U16_WMC     wNoOfAudioStreams;
    U16_WMC     wNoOfVideoStreams;
    U16_WMC     wNoOfBinaryStreams;

    BITRATEINFO tBitRateInfo[127];
    PACKET_PARSE_INFO_EX ppex;

    PAYLOAD_MAP_ENTRY_EX payload;
    U32_WMC     iPayload;
    U16_WMC     wPayStart;
    Bool_WMC    bHasBitRateInfo;
    U32_WMC     dwPayloadOffset;
} HEADERINFO;
#endif

#if !defined(WMF_EXCLUDE_AUDIO)

typedef    struct WmaGetDataParam
{
    U32_WMC  m_dwUser;
    Bool_WMC m_fTimeIsValid;
    I64_WMC  m_iTime;
} WmaGetDataParam;

#if defined(_V9WMAPRODEC_)

typedef int WMARESULT;
// define basic types
typedef long I32_WMARawDec;
typedef unsigned long U32_WMARawDec;
typedef long I32_WMARawDec;
typedef unsigned short U16_WMARawDec;
typedef short I16_WMARawDec;
typedef unsigned char U8_WMARawDec;

// SUCCESS codes
static const WMARESULT WMA_OK               = 0x00000000;
static const WMARESULT WMA_S_FALSE          = 0x00000001;
static const WMARESULT WMA_S_BUFUNDERFLOW   = 0x00000002;
static const WMARESULT WMA_S_NEWPACKET      = 0x00000003;
static const WMARESULT WMA_S_NO_MORE_FRAME  = 0x00000004;
static const WMARESULT WMA_S_NO_MORE_SRCDATA= 0x00000005;

// ERROR codes
static const WMARESULT WMA_E_FAIL           = 0x80004005;
static const WMARESULT WMA_E_OUTOFMEMORY    = 0x8007000E;
static const WMARESULT WMA_E_INVALIDARG     = 0x80070057;
static const WMARESULT WMA_E_NOTSUPPORTED   = 0x80040000;
static const WMARESULT WMA_E_LOSTPACKET     = 0x80040001;
static const WMARESULT WMA_E_BROKEN_FRAME   = 0x80040002;
static const WMARESULT WMA_E_BUFFEROVERFLOW = 0x80040003;
static const WMARESULT WMA_E_ONHOLD         = 0x80040004;
static const WMARESULT WMA_E_NOMOREINPUT    = 0x8004000A; // Streaming mode: absolutely no more data (returned via GetMoreData callback)

#define WMARAW_SUCCEEDED(Status) ((WMARESULT)(Status) >= 0)
#define WMARAW_FAILED(Status) ((WMARESULT)(Status)<0)

#endif //_V9WMAPRODEC_
#endif //WMF_EXCLUDE_AUDIO

typedef    struct tAudioStreamInfo
{
    U16_WMC    nVersion;
    U16_WMC    wFormatTag;
    U32_WMC    nSamplesPerSec;
    U32_WMC    nAvgBytesPerSec;
    U32_WMC    nBlockAlign;
    U16_WMC    nChannels;

    U32_WMC    nSamplesPerBlock;
    U16_WMC    nEncodeOpt;

    I32_WMC    nBitsPerSample;
    U16_WMC    wValidBitsPerSample; // actual valid data depth in the decoded stream
    U32_WMC    dwChannelMask;
    GUID_WMC   SubFormat;
    U16_WMC    wOriginalBitDepth; // actual valid data depth in the original source (informational)
    U16_WMC    wStreamId;
    U32_WMC    cbAudioSize;
#ifndef WMF_EXCLUDE_AUDIO

#ifdef _V9WMAPRODEC_

    WMAProRawDecHandle      hMSA;
    WMAProRawDecState       decState;
    PCMFormat               tPCMFormat; //Record output PCM Format.
    WMAPlayerInfo           tPlayerInfo;
    I32_WMC                 cSampleReady;
#else
    WMARawDecHandle     hMSA;
#endif //_V9WMAPRODEC_

#endif // WMF_EXCLUDE_AUDIO
    Bool_WMC   bTobeDecoded;
    Bool_WMC   bIsDecodable;
    Bool_WMC   bWantOutput;
#ifndef WMF_EXCLUDE_AUDIO
    WMARESULT  wmar;
#endif // WMF_EXCLUDE_AUDIO
    Bool_WMC   bTimeToChange;
    Bool_WMC   bTimeToChangex;
    Bool_WMC   bFirstTime;
    U8_WMC     bAudioBuffer[MIN_WANTED];
    U8_WMC     *pbAudioBuffer;
    U32_WMC    dwFrameSize;
    U32_WMC    dwAudioBufSize;
    U32_WMC    dwAudioBufCurOffset;
    U32_WMC    cbNbFramesAudBuf;
    U32_WMC    dwAudioBufDecoded;
    U32_WMC    dwAudioPayloadSize;
    U8_WMC     bBlockStart;
    U32_WMC    dwBlockLeft;
    U32_WMC    dwPayloadLeft;
    U32_WMC    dwAudPayloadPresTime;
    Double_WMC dwAudioTimeStamp;
    U8_WMC*    pStoreFrameStartPointer;
    U8_WMC*    pStoreNextFrameStartPointer;
    U8_WMC*    pDecodeFrameStartPointer;
    U8_WMC*    pDecodeNextFrameStartPointer;
    Bool_WMC   bBufferIncreased;
    Bool_WMC   bStopReading;
    Bool_WMC   bGotCompOutput;
    Bool_WMC   bOutputisReady;
//  for non buffer mode
//#ifdef WMC_NO_BUFFER_MODE
    U64_WMC     cbPacketOffset;
    U64_WMC     cbCurrentPacketOffset;
    U64_WMC     cbNextPacketOffset;
    PACKET_PARSE_INFO_EX ppex;
    PAYLOAD_MAP_ENTRY_EX payload;
    U32_WMC     iPayload;
    U16_WMC     wPayStart;
    U32_WMC     dwPayloadOffset;
    tWMCParseState         parse_state;
    Bool_WMC   bPayloadGiven;

    Bool_WMC   bHasGivenAnyOutput;
    Bool_WMC   bNowStopReadingAndDecoding;
    Bool_WMC   bWaitingToGetScheduled;
    Bool_WMC   bBufferLimitReached;
    Bool_WMC   bBuffer15Full;

    EXTENDEDSTREAMPROPERTIES *pExtnStreamProperties;
    U16_WMC     u16ExtnDataSize;
//#endif

#ifdef USE_MERCURY
    U32_WMC               dwUDLRepIndex;
    U32_WMC               dwUDLRepSize;
#endif

} AUDIOSTREAMINFO;

typedef    struct tVideoStreamInfo
{
    U32_WMC    windowWidth;
    U32_WMC    windowHeight;
    U8_WMC     flags;
    U16_WMC    imageInfoLen;

    U32_WMC    biSize;
    I32_WMC    biWidth;
    I32_WMC    biHeight;
    U16_WMC    biPlanes;
    U16_WMC    biBitCount;
    U32_WMC    biCompression;
    U32_WMC    biSizeImage;
    I32_WMC    biXPelsPerMeter;
    I32_WMC    biYPelsPerMeter;
    U32_WMC    biClrUsed;
    U32_WMC    biClrImportant;

    U16_WMC    biBitCountOut;
    U16_WMC    biBitCountCodec;
    U32_WMC    biCompressionOut;
    U32_WMC    biSizeImageOut;

    void       *pVDec;
    U16_WMC    wStreamId;
    Bool_WMC   bTobeDecoded;
    Bool_WMC   bIsDecodable;
    Bool_WMC   bWantOutput;
    Bool_WMC   bFirst;
    U8_WMC     *pbVideoBuffer;
    U8_WMC     *pbVideoOutputBuffer;
    U32_WMC    dwVideoBufSize;
    U32_WMC    dwVideoBufCurOffset;
    U8_WMC     bBlockStart;
    U32_WMC    dwBlockLeft;
    U32_WMC    dwPayloadLeft;
    U32_WMC    dwFrameLeft;
    U32_WMC    cbNbFramesVidBuf;
    U32_WMC    dwVideoBufDecoded;
//  U8_WMC     *pVideoBuffer;
    U32_WMC    dwVideoTimeStamp;
    U32_WMC    dwNbFrames;
    U8_WMC     bAdditionalInfo[1024];
    U8_WMC     bPalette[3*256];
    U8_WMC     bVideoBuffer[MIN_WANTED+10];
    Bool_WMC   bInitStage;
    U32_WMC    cbUsed;
    U32_WMC    dwFrameSize;
    U32_WMC    cbFrame;
    Bool_WMC   bIsKeyFrame;
    U8_WMC*    pSCInBuffer;
    U8_WMC*    pSCOutBuffer;
    Bool_WMC   bScOutGiven;
    U8_WMC*    pStoreFrameStartPointer;
    U8_WMC*    pStoreNextFrameStartPointer;
    U8_WMC*    pDecodeFrameStartPointer;
    U8_WMC*    pDecodeNextFrameStartPointer;
    Bool_WMC   bBufferIncreased;
    Bool_WMC   bStopReading;
    Bool_WMC   bGotCompOutput;
    Bool_WMC   bOutputisReady;
    Bool_WMC   bHasGivenAnyOutput;
    Bool_WMC   bNowStopReadingAndDecoding;
    tVideoFormat_WMC tOutVideoFormat;

//#ifdef WMC_NO_BUFFER_MODE
    U32_WMC     cbPacketOffset;
    U32_WMC     cbCurrentPacketOffset;
    U32_WMC     cbNextPacketOffset;

    PACKET_PARSE_INFO_EX ppex;
    PAYLOAD_MAP_ENTRY_EX payload;
    U32_WMC     iPayload;
    U16_WMC     wPayStart;
    U32_WMC     dwPayloadOffset;
    tWMCParseState         parse_state;
    U32_WMC    dwNextVideoTimeStamp;
    Bool_WMC   bFrameGiven;
    Bool_WMC   bWaitingToGetScheduled;
    Bool_WMC   bSkipToNextI;
#ifdef __USE_WMV9__
    Bool_WMC   bV9FrameGiven;
#endif
    EXTENDEDSTREAMPROPERTIES *pExtnStreamProperties;
    U16_WMC     u16ExtnDataSize;
    Bool_WMC   bBufferLimitReached;
    Bool_WMC   bBuffer15Full;

#ifdef USE_MERCURY
    U32_WMC               dwUDLRepIndex;
    U32_WMC               dwUDLRepSize;
#endif
//#endif
} VIDEOSTREAMINFO;


typedef    struct tBinaryStreamInfo
{
//    Bool_WMC   bTobeDecoded;
    U16_WMC    wStreamId;
    Bool_WMC   bWantOutput;
    U32_WMC    dwBinaryBufSize;
    U8_WMC     *pbBinaryBuffer;
    U32_WMC    dwBinaryBufCurOffset;
    U32_WMC    dwBinaryTimeStamp;
    U32_WMC    cbNbFramesBinBuf;
    U32_WMC    dwBlockLeft;
    U32_WMC    dwPayloadLeft;
    U32_WMC    dwFrameLeft;
    U32_WMC    cbUsed;
    U32_WMC    dwFrameSize;
    U32_WMC    cbFrame;
    U32_WMC    dwNbFrames;
    U32_WMC    dwBinaryBufDecoded;
    U8_WMC*    pStoreFrameStartPointer;
    U8_WMC*    pStoreNextFrameStartPointer;
    U8_WMC*    pDecodeFrameStartPointer;
    U8_WMC*    pDecodeNextFrameStartPointer;
    Bool_WMC   bBufferIncreased;
    Bool_WMC   bStopReading;
    Bool_WMC   bGotCompOutput;
    Bool_WMC   bOutputisReady;

    Bool_WMC   bHasGivenAnyOutput;
    Bool_WMC   bNowStopReadingAndDecoding;

//#ifdef WMC_NO_BUFFER_MODE
    U64_WMC     cbPacketOffset;
    U64_WMC     cbCurrentPacketOffset;
    U64_WMC     cbNextPacketOffset;
    PACKET_PARSE_INFO_EX ppex;
    PAYLOAD_MAP_ENTRY_EX payload;
    U32_WMC     iPayload;
    U16_WMC     wPayStart;
    U32_WMC     dwPayloadOffset;
    tWMCParseState         parse_state;
    U32_WMC    dwNextBinaryTimeStamp;
    Bool_WMC   bFrameGiven;
    Bool_WMC   bWaitingToGetScheduled;
    EXTENDEDSTREAMPROPERTIES *pExtnStreamProperties;
    U16_WMC     u16ExtnDataSize;
    Bool_WMC   bBufferLimitReached;
    Bool_WMC   bBuffer15Full;
//#endif

#ifdef USE_MERCURY
    U32_WMC               dwUDLRepIndex;
    U32_WMC               dwUDLRepSize;
#endif

} BINARYSTREAMINFO;

typedef    struct tWMFDecoderEx
{
    U32_WMC               dwCheck;
    HEADERINFO            tHeaderInfo;
    AUDIOSTREAMINFO *     tAudioStreamInfo[127];
    VIDEOSTREAMINFO *     tVideoStreamInfo[127];
    BINARYSTREAMINFO *    tBinaryStreamInfo[127];

#ifdef USE_MERCURY
    void                 *pUDLData;
    U8_WMC                cbIsUDRMEncrypted;
    U8_WMC                cUDLRepSalt[4];
#endif

#ifndef WMCAPI_NO_DRM
#ifdef _ZMDRM_
    HDRMMGR hDRM;
#else
    VOID *hDRM;
#endif
#endif

    U8_WMC*                pszType;
    U8_WMC*                pszKeyID;

    U32_WMC                cbEnhData;
    U8_WMC*                pbEnhData;
    U32_WMC                cbSecData;
    U8_WMC*                pbSecData;

    U32_WMC                dwCryptoVersion;
    U32_WMC                cbUDLData;
    U8_WMC*                pbUDLData;

    tWMCParseState         parse_state;
    /* Marker */
    U32_WMC                m_dwMarkerNum;
    WMCContentDescription  *m_pContentDesc;
    WMCMarkerEntry         *m_pMarkers;
    WMCExtendedContentDesc *m_pExtendedContentDesc;
    WMCScriptCommand       *m_pScriptCommand;
        /* License Store */
    U32_WMC                m_dwLicenseLen;
    U8_WMC                 *m_pLicData;
    U8_WMC                 bHasDRM;
    // Adding for index entry
    U8_WMC                 cTotalIndexs;
    WMCINDEXINFO           tIndexInfo[127];
    Bool_WMC               bParsed;
    I32_WMC                i32CurrentAudioStreamIndex;
    I32_WMC                i32CurrentVideoStreamIndex;
    I32_WMC                i32CurrentBinaryStreamIndex;
    tPlannedOutputWMC      tPlannedOutputInfo;
    tStreamIdnMediaType_WMC *pStreamIdnMediaType;
    U32_WMC                 u32UserData;
    U32_WMC                m_dwNumCodec;
    WMCCodecEntry          *m_pCodecEntry;
    U64_WMC                u64SeekTimeRequest;
    Bool_WMC               bInSeekState;
    tWMCDecodeDispRotateDegree tDispRotateDegree;
    I32_WMC                 i32PostProc;
    I32_WMC                 iHurryUp;
    Bool_WMC                bIsNoBufferMode;
    WMCMetaDataEntry        tMetaDataEntry;
    EXTENDEDSTREAMPROPERTIES *tExtnStreamProperties[127];
    U16_WMC                 wExtnPropertiesCount;
    WMCNEWINDEX             tNewIndex;
    U32_WMC                 u32SCEnableMask;
}WMFDecoderEx;

#ifndef _ASFPARSE_ONLY_
#ifdef __USE_WMV9__
//#include "vfw.h"
#define tWMVDecodeStatus int
#define Bool_WMV long
#define TRUE_WMV TRUE_WMC
#define FALSE_WMV FALSE_WMC
#define U8_WMV unsigned char
#define U16_WMV unsigned short
#define U32_WMV unsigned long
#define I8_WMV char
#define I16_WMV short
#define I32_WMV long
#define Float_WMV float
#define WMV_Succeeded 0L
#define WMV_Failed -100L
#define WMVDec_BadMemory -3L
#define WMV_EndOfFrame WMV_Failed
#define WMV_InValidArguments WMV_Failed
#endif
#endif


#endif

//#pragma pack ()

