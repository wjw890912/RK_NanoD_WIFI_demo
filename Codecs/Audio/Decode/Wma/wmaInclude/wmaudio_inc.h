//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************

#ifndef __WMAUDIO_INC_H_
#define __WMAUDIO_INC_H_

//#define STDCALL __stdcall // For drmpd_ext.h

//#include <stdio.h>
//#include "malloc.h"

//#pragma warning( disable : 4005 )
//#if ( defined(BOOL) || defined(_WIN32_WCE) ) && !defined(_BOOL_DEFINED)
//#define _BOOL_DEFINED
//#endif
#include "wmatypes.h"
#include "wmaudio.h"
#include "loadstuff.h"
#include "wmaudio_type.h"
#include "wmaguids.h"
#include "AutoProfile.h"


#include "msaudiodec.h"
#include "wmadec.h"

#define MIN(x, y)  ((x) < (y) ? (x) : (y))


#define WMAAPI_DEMO_LIMIT 60
#define CALLBACK_BUFFERSIZE WMA_MAX_DATA_REQUESTED*2

#define WMAFileGetInputParam WmaGetDataParam

WMARESULT WMAFileGetInput(tWMAFileStateInternal *pInt, U8 **ppBuffer, U32 *pcbBuffer, WMAFileGetInputParam*);
tWMAFileStatus WMAF_UpdateNewPayload(tWMAFileStateInternal *pInt);

#endif // wmaudio_inc.h