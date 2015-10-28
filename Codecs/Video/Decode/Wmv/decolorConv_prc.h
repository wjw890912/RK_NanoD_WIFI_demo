//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (c) Microsoft Corporation 1999

Module Name:

	colorConv_prc.h

Abstract:

    API's for colorConvert routines.

Author:

	Bruce Lin (blin@microsoft.com) 02-Decmber-1999

Revision History:


*************************************************************************/

#ifndef __DECOLORCONV_PRC_H_
#define __DECOLORCONV_PRC_H_

#include "xplatform_wmv.h"
#include "wmvdec_api.h"
#include "wmvdec_member.h"
#include "wmvdec_function.h"
#include "constants_wmv.h"
#include "decolorconv_shared.h"

// ===========================================================================
// VideoDICreate()
//
// Creates a Microsoft Video ColorConvert object and returns it.
//
// Input:
//      pbihInput       Source BITMAPINFOHEADER
//      pbihOutput      Destination BITMAPINFOHEADER
//
//
// Returns NULL if not successful.
//
///////////////////////////////////////////////////////////////////////////////////////

videoCC_HANDLE VideoDCCCreate (
    tColorConvertStatus *ptStatus,
    const BITMAPINFOHEADER *pbihInput, // Source BITMAPINFOHEADER
    const BITMAPINFOHEADER *pbihOutput, // Destination (output)
                                        // BITMAPINFOHEADER, assigned
                                        // outside but checked by the
                                        // routines
    int                     mode
);

videoCC_HANDLE VideoDCCCreateWithStride (
    tColorConvertStatus *ptStatus,
    const BITMAPINFOHEADER *pbihInput, // Source BITMAPINFOHEADER
    const BITMAPINFOHEADER *pbihOutput, // Destination (output) BITMAPINFOHEADER, assigned outside but checked by the routines
    long int  iActualWidth ,    // support stride.
    long int   mode
);

videoCC_HANDLE VideoDCCCreateWithFullCropping (
	tColorConvertStatus    *ptStatus,
	const BITMAPINFOHEADER *pbihInput,
	const BITMAPINFOHEADER *pbihOutput,
	int                     iSrcCropLeft,
	int                     iSrcCropTop,
	int                     iDstCropLeft,
	int                     iDstCropTop,
	int                     iCropWidth,
	int                     iCropHeight,
    int                     mode ,
    int                     StrideY ,
    int                     StrideU ,
    int                     StrideV
	);


// ===========================================================================
// msadDestroy()
//
// Destroys the given Microsoft Video ColorConvert object handle.
//
// Input:
//      hVideoColorConv   Handle to ColorConvert handle
//
// Returns 0 if successful.
//
///////////////////////////////////////////////////////////////////////////////////////

//MSVDERROR
tColorConvertStatus VideoDCCDestroy (videoCC_HANDLE hVideoColorConv);

// ===========================================================================
// VideoDIColorConv()
//
// ColorConvert video frame using the given Microsoft ColorConvert handle.
//
//
// Input:
//      hVideoColorConv   Handle to ColorConvert handle
//      pucInput            Pointer to the input bitmap buffer
//      pucOutput          Pointer to the output bitmap buffer
//
// Returns 0 if successful.
///////////////////////////////////////////////////////////////////////////////////////

tColorConvertStatus VideoDCCColorConv (
    videoCC_HANDLE hVideoColorConv,
    const unsigned char *pucInput, // Pointer points to the input bitmap buffer
    unsigned char *pucOutput, // Output bitmap buffer.  Memory controlled (allocated) by externals.
    const unsigned char *pucInputU , // Pointer points to the input bitmap buffer
    const unsigned char *pucInputV  // Pointer points to the input bitmap buffer

);

tColorConvertStatus VideoDCCColorConvPitch (
    videoCC_HANDLE         hVideoColorConv,
    const unsigned char*   pucInput, // Pointer points to the input bitmap buffer
    unsigned char*         pucOutput, // Output bitmap buffer.  Memory controlled (allocated) by externals.
    long int               iPitchSrc ,
    long int               iPitchDst ,
    const unsigned char*   pucInputU , // Pointer points to the input bitmap buffer
    const unsigned char*   pucInputV   // Pointer points to the input bitmap buffer

);

tColorConvertStatus  VideoDCCReset
(
 videoCC_HANDLE          hVideoColorConv,
 const BITMAPINFOHEADER *pbihInput,
 const BITMAPINFOHEADER *pbihOutput,
 int                     iSrcCropLeft,
 int                     iSrcCropTop,
 int                     iDstCropLeft,
 int                     iDstCropTop,
 int                     iCropWidth,
 int                     iCropHeight,
 int                     mode,
 int                     StrideY,
 int                     StrideU,
 int                     StrideV
 );

//tColorConvertStatus VideoDCCResetHeight(videoCC_HANDLE hVideoColorConv, int iPitchSrc, int iPitchDst);

#endif
