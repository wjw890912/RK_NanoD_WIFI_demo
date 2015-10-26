//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef __WMAERROR_H__
#define __WMAERROR_H__

#include "wmatyps.h"

// SUCCESS codes
static const WMARESULT WMA_OK               = 0x00000000;
static const WMARESULT WMA_S_FALSE          = 0x00000001;
static const WMARESULT WMA_S_BUFUNDERFLOW   = 0x00000002;
static const WMARESULT WMA_S_NEWPACKET      = 0x00000003;
static const WMARESULT WMA_S_NO_MORE_FRAME  = 0x00000004;
static const WMARESULT WMA_S_NO_MORE_SRCDATA= 0x00000005;
static const WMARESULT WMA_S_LOSTPACKET     = 0x00000006;

// ERROR codes
static const WMARESULT WMA_E_FAIL           = 0x80004005;
static const WMARESULT WMA_E_OUTOFMEMORY    = 0x8007000E;
static const WMARESULT WMA_E_INVALIDARG     = 0x80070057;
static const WMARESULT WMA_E_NOTSUPPORTED   = 0x80040000;
static const WMARESULT WMA_E_BROKEN_FRAME   = 0x80040002;
static const WMARESULT WMA_E_BUFFEROVERFLOW = 0x80040003;
static const WMARESULT WMA_E_ONHOLD         = 0x80040004;
static const WMARESULT WMA_E_NO_MORE_SRCDATA= 0x80040005;
static const WMARESULT WMA_E_WRONGSTATE     = 0x8004000A;
static const WMARESULT WMA_E_NOMOREINPUT    = 0x8004000B; // Streaming mode: absolutely no more data (returned via GetMoreData callback)

#define WMA_SUCCEEDED(Status) ((WMARESULT)(Status) >= 0)
#define WMA_FAILED(Status) ((WMARESULT)(Status)<0)

#endif//__WMAERROR_H__
