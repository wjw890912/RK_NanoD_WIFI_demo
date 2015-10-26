//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*
 * Windows Media Audio (WMA) Decoder API (implementation)
 *
 * Copyright (c) Microsoft Corporation 1999.  All Rights Reserved.
 */

#ifndef _WMAUDIO_IMPL_H_
#define _WMAUDIO_IMPL_H_

#include "wmaerr.h"
#include "wmatypes.h"
#ifndef WMCAPI_NO_DRM
#include "drmpd.h"
#endif /* WMCAPI_NO_DRM */
#include "msaudiodec.h"
//#pragma pack (1)

#define MIN_OBJECT_SIZE     24
#define DATA_OBJECT_SIZE    50


/* parser state  */
typedef     enum
{
    csWMA_NotValid = 0,

    csWMA_HeaderStart,
    csWMA_HeaderError,

    csWMA_NewAsfPacket,

    /* packet parsing states */

    csWMA_DecodePayloadStart,
    csWMA_DecodePayload,
    csWMA_DecodePayloadHeader,
    csWMA_DecodeLoopStart,
    csWMA_DecodePayloadEnd,

    csWMA_End_

} tWMAParseState;


/* internal structure for the ASF header parsing */
typedef  struct tagWMAFileHdrStateInternal
{
    DWORD currPacketOffset;     /* not just for packets */
    DWORD nextPacketOffset;

    /* ASF header */

    DWORD   cbHeader;
    DWORD   cbPacketSize;
    DWORD   cbAudioSize;
    DWORD   cPackets;
    DWORD   msDuration;
    DWORD   msPreroll;
    DWORD   cbFirstPacketOffset;
    DWORD   cbLastPacketOffset;

    /* audio prop */

    WORD    nVersion;
    DWORD   nSamplesPerSec;
    DWORD   nAvgBytesPerSec;
    DWORD   nBlockAlign;
    WORD    nChannels;
    DWORD   nSamplesPerBlock;
    WORD    nEncodeOpt;

    /* DRM */

    BYTE    pbSecretData[32];
    DWORD   cbSecretData;
    BYTE    pbType[16];
    BYTE    pbKeyID[32];

    /* Content Description */

    DWORD   cbCDOffset;
    WORD    cbCDTitle;
    WORD    cbCDAuthor;
    WORD    cbCDCopyright;
    WORD    cbCDDescription;
    WORD    cbCDRating;

} tWMAFileHdrStateInternal;

//typedef void * WMADecHANDLE;

/* internal structure for the WMAudio library state */
typedef  struct tagWMAFileStateInternal
{
    tWMAFileHdrStateInternal hdr_parse;

    /* parse_state */

    tWMAParseState parse_state;

    /* payload parsing */

    DWORD cbPayloadOffset;
    DWORD cbPayloadLeft;

    BOOL  bBlockStart;
    DWORD cbBlockLeft;

    /* packet parsing */

    PACKET_PARSE_INFO_EX ppex;

    PAYLOAD_MAP_ENTRY_EX payload;
    DWORD iPayload;
    WORD wPayStart;

    /* other states */

    BOOL    bHasDRM;

    DWORD   nSampleCount;

    /* objects */

//    tWMAState wma_state;
//    WMADHANDLE hWMA;
    CAudioObjectDecoder* pWMA;
#ifndef WMCAPI_NO_DRM
    CDrmPD* pDRM_state;
#endif /* WMCAPI_NO_DRM */

} tWMAFileStateInternal;


/* ASF parsing functions */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/* use (only) for reading possibly large chunk */
DWORD WMA_GetBuffer(tWMAFileStateInternal *pInt,
                    DWORD cbOffset, DWORD cbWanted, BYTE *pBuffer);

WMAERR WMA_LoadObjectHeader(tWMAFileHdrStateInternal *pInt, GUID *pObjectId, QWORD *pqwSize);
WMAERR WMA_LoadHeaderObject(tWMAFileHdrStateInternal *pInt, int isFull);
WMAERR WMA_LoadPropertiesObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize, int isFull);
WMAERR WMA_LoadAudioObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize, int isFull);
WMAERR WMA_LoadEncryptionObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize);
WMAERR WMA_LoadContentDescriptionObject(tWMAFileHdrStateInternal *pInt, DWORD cbSize);

WMAERR WMA_ParseAsfHeader(tWMAFileHdrStateInternal *pInt, int isFull);
WMAERR WMA_ParsePacketHeader(tWMAFileStateInternal *pInt);
WMAERR WMA_ParsePayloadHeader(tWMAFileStateInternal *pInt);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _WMAUDIO_IMPL_H_ */
//#pragma pack ()

