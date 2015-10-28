//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************

#ifndef __DECOLORCONV_SHARED_H_
#define __DECOLORCONV_SHARED_H_

//#include "windows.h"   // for bitmap stuff
//#include <windowsx.h>
//#include "typincld.h"
//#include "fourcc_guid.h"


#ifdef __cplusplus
#define EXTERN_C extern "C"
#else // __cplusplus
#define EXTERN_C extern
#endif // __cplusplus


typedef     enum tagColorConvertStatus
{
	CCS_Succeeded = 0,
	CCS_Failed = 1,
	CCS_BadMemory = 2,
	CCS_UnSupportedInputFormat = 3,
	CCS_UnSupportedOutputFormat = 4,
	CCS_UnSupportedFormatPair = 5,
	CCS_BadSize = 6,
	CCS_NoConversionNeeded = 7
} tColorConvertStatus;

typedef void * videoCC_HANDLE;



#endif
