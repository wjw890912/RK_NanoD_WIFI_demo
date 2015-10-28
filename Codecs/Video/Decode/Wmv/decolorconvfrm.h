//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1998 - 1999

Module Name:

	decolorConvFrm.h

Abstract:

	Frame-based DeColor Conversion.

Author:

    Ming-Chieh Lee (mingcl@microsoft.com)
    Chuang Gu (chuanggu@microsoft.com)

	Dec-1999

Revision History:

*************************************************************************/

#ifndef __DECOLORCONVFRM_H_
#define __DECOLORCONVFRM_H_

#ifndef NO_WINDOWS
//#include "mfdevplat.h"
#include "windows.h"    // for bitmap stuff
#include <windowsx.h>
#endif

#include "bitfields.h"
#include "decolorconv_prc.h"
#include "threaddirectcolorconv.h"

#ifndef _WIN32_WCE
#pragma AVRT_VTABLES_BEGIN
#endif //_WIN32_WCE
Void_WMV initDeCC( tColorConvertStatus    *ptStatus,
               const BITMAPINFOHEADER *pbihInput,
               const BITMAPINFOHEADER *pbihOutput,
               DIRECTCC*               dccs,
               I32_WMV                     mode);

tColorConvertStatus CheckSupportedDCCFormat( DIRECTCC* dccs );
tColorConvertStatus ClassifyDCCConversion( DIRECTCC* diccs );

Void_WMV RefreshSrcRGB8From411  (
    U8_WMV*            pBits,
    const U8_WMV*  ppxliCurrQYFrm,
    const U8_WMV*  ppxliCurrQUFrm,
    const U8_WMV*  ppxliCurrQVFrm,
    const I32_WMV            iStartLineNumY,
    const I32_WMV            iEndLineNumY,
    DIRECTCC*      diccs
	);


Void_WMV RefreshSrcRGB16From411  (
    U8_WMV*            pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV            iStartLineNumY,
	const I32_WMV            iEndLineNumY,
	DIRECTCC*      diccs
	);

Void_WMV RefreshSrcRGB24From411  (
  U8_WMV*            pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV            iStartLineNumY,
	const I32_WMV            iEndLineNumY,
	DIRECTCC*      diccs
	);

Void_WMV RefreshSrcRGB32From411  (
  U8_WMV*            pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV            iStartLineNumY,
	const I32_WMV            iEndLineNumY,
	DIRECTCC*      diccs
	);


Void_WMV RefreshSrcUYVYFrom411 (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcUYVYFrom411KNI (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcUYVYFrom411MMX (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYVYUFrom411 (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYVYUFrom411KNI (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYVYUFrom411MMX (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYVYUFrom420 (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYVYUFrom420KNI(
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshSrcYVYUFrom420MMX(
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs);


Void_WMV RefreshPlanarSrcFrmToRGB32 (
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshPlanarSrcFrmToRGB24 (
	U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshPlanarSrcFrmToRGB16 (
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshPlanarSrcFrmToRGB8 (
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshSrcUYVYFrom420 (
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshSrcUYVYFrom420KNI(
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshSrcUYVYFrom420MMX(
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshSrcYVYUFrom420Interlace(
    U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV   iStartLineNumY,
	const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);


Void_WMV RefreshSrcUYVYFrom420Interlace(
    U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV   iStartLineNumY,
	const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshPlanarSrcFrmToRGB32Interlace (
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshPlanarSrcFrmToRGB24Interlace (
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshPlanarSrcFrmToRGB16Interlace (
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshPlanarSrcFrmToRGB8Interlace (
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshAYUVFrom420(
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshAYUVFrom420Interlace(
    U8_WMV*         pBits,
    const U8_WMV*   ppxliCurrQYFrm,
    const U8_WMV*   ppxliCurrQUFrm,
    const U8_WMV*   ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshNV11From420 (
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*     ppxliCurrQYFrm,
    U8_WMV*     ppxliCurrQUVFrm,
    U8_WMV*     pDummy,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   ccs);

Void_WMV RefreshNV12From420 (
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*     ppxliCurrQYFrm,
    U8_WMV*     ppxliCurrQUVFrm,
    U8_WMV*     pDummy,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   ccs);

Void_WMV RefreshNV11From411 (
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*     ppxliCurrQYFrm,
    U8_WMV*     ppxliCurrQUVFrm,
    U8_WMV*     pDummy,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   ccs);

Void_WMV RefreshYUY2From422 (
	U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV  iStartLineNumY,
	const I32_WMV  iEndLineNumY,
	DIRECTCC*  diccs 	);

Void_WMV RefreshUYVYFrom422 (
	U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV  iStartLineNumY,
	const I32_WMV  iEndLineNumY,
	DIRECTCC*  diccs 	);

Void_WMV RefreshYVYUFrom422 (
	U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV  iStartLineNumY,
	const I32_WMV  iEndLineNumY,
	DIRECTCC*  diccs 	);

Void_WMV RefreshSrcRGB8From422  (U8_WMV*            pBits,
							 const U8_WMV*  ppxliCurrQYFrm,
							 const U8_WMV*  ppxliCurrQUFrm,
							 const U8_WMV*  ppxliCurrQVFrm,
							 const I32_WMV      iStartLineNumY,
							 const I32_WMV      iEndLineNumY,
							 DIRECTCC*      diccs );

Void_WMV RefreshSrcRGB16From422  ( U8_WMV*            pBits,
							   const U8_WMV*  ppxliCurrQYFrm,
							   const U8_WMV*  ppxliCurrQUFrm,
							   const U8_WMV*  ppxliCurrQVFrm,
							   const I32_WMV      iStartLineNumY,
							   const I32_WMV      iEndLineNumY,
							   DIRECTCC*      diccs );

Void_WMV RefreshSrcRGB24From422  ( U8_WMV*            pBits,
							   const U8_WMV*  ppxliCurrQYFrm,
							   const U8_WMV*  ppxliCurrQUFrm,
							   const U8_WMV*  ppxliCurrQVFrm,
							   const I32_WMV      iStartLineNumY,
							   const I32_WMV      iEndLineNumY,
							   DIRECTCC*      diccs );

Void_WMV RefreshSrcRGB32From422  ( U8_WMV*            pBits,
							   const U8_WMV*  ppxliCurrQYFrm,
							   const U8_WMV*  ppxliCurrQUFrm,
							   const U8_WMV*  ppxliCurrQVFrm,
							   const I32_WMV      iStartLineNumY,
							   const I32_WMV      iEndLineNumY,
							   DIRECTCC*      diccs );

Void_WMV RefreshAYUVFrom422 (
	U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV  iStartLineNumY,
	const I32_WMV  iEndLineNumY,
	DIRECTCC*  diccs 	);

Void_WMV RefreshNV12From422 (
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*     ppxliCurrQYFrm,
    U8_WMV*     ppxliCurrQUVFrm,
    U8_WMV*     pDummy,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   diccs);

Void_WMV RefreshNV11From422 (
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*     ppxliCurrQYFrm,
    U8_WMV*     ppxliCurrQUVFrm,
    U8_WMV*     pDummy,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   diccs);

Void_WMV RefreshV216From422 (
	U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV  iStartLineNumY,
	const I32_WMV  iEndLineNumY,
	DIRECTCC*  diccs 	);

Void_WMV YUVCopy
(   const U8_WMV* pSrc,
    U8_WMV*       pDst,
    I32_WMV       iSrcStride,
    I32_WMV       iDstStride,
    I32_WMV       iSrcJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iDstJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iWidth,
    I32_WMV       iHeight,
    I32_WMV       iReserved            // 0: no dup, 1: dst vertical dup,
    );

Void_WMV UpSampling
(   const U8_WMV* pSrcU,
    const U8_WMV* pSrcV,
    U8_WMV*       pDstU,
    U8_WMV*       pDstV,
    I32_WMV       iSrcStride,              // source stride.
    I32_WMV       iDstStride,              // destination stride.
    I32_WMV       iSrcJumpPeriod,          // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iDstJumpPeriod,          // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iWidth,                  // processing width: number of pixels horizontally
    I32_WMV       iHeight,                 // processing width: number of lines vertically.
    Bool_WMV      bDoTop,                  // indicate whether or not to process top line.
    Bool_WMV      bDoBottom,               // indicate whether or not to process bottom line.
    I32_WMV       iBottomRepeatNum,        // num of lines to repeat at bottom.
    I32_WMV       iParam0,
    I32_WMV       iParam1,
    I32_WMV       iParam2,
    I32_WMV       iParam3 );

#ifndef _TESTSAMPLING_
#define _TESTSAMPLING_
#endif

Void_WMV UpSampling1
(
    const U8_WMV*      pSrcU,
    const U8_WMV*      pSrcV,
    U8_WMV*            pDstU,
    U8_WMV*            pDstV,
    I32_WMV            iSrcStride,              // source stride.
    I32_WMV            iDstStride,              // destination stride.
    I32_WMV            iSrcJumpPeriod,          //
    I32_WMV            iDstJumpPeriod,          //
    I32_WMV            iWidth,                  // width:  number of pixels horizontally
    I32_WMV            iHeight,                 // height: number of source lines vertically
    Bool_WMV           bDoTop,                  // indicate whether make top.
    Bool_WMV           bDoBottom,               // indicate whether make bottom.
    I32_WMV            iBottomRepeatNum,        // num of lines to repeat at bottom.
    I32_WMV            iParam0,                 // Interpolation parameters.
    I32_WMV            iParam1);

Void_WMV DownSampling
(
    const U8_WMV*      pSrcU,
    const U8_WMV*      pSrcV,
    U8_WMV*            pDstU,
    U8_WMV*            pDstV,
    I32_WMV            iSrcStride,              // source stride.
    I32_WMV            iDstStride,              // destination stride.
    I32_WMV            iSrcJumpPeriod,          //
    I32_WMV            iDstJumpPeriod,          //
    I32_WMV            iWidth,                  // width:  number of pixels horizontally
    I32_WMV            iHeight,                 // height: number of source lines vertically
    I32_WMV            iParam0,                 // Interpolation parameters.
    I32_WMV            iParam1);




#ifndef _WIN32_WCE
#pragma AVRT_VTABLES_END
#endif //_WIN32_WCE


#endif // __DECOLORCONVFRM_H_


