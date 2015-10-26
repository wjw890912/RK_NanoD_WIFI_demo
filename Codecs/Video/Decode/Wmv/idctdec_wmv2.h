//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) 1996 -- 1998  Microsoft Corporation

Module Name:

	idctdec_wmv2.h

Abstract:

	Inverse DCT routines used by WMV2 decoder

Author:

	Marc Holder (marchold@microsoft.com)    05/98
    Bruce Lin (blin@microsoft.com)          06/98
    Ming-Chieh Lee (mingcl@microsoft.com)   06/98

Revision History:

*************************************************************************/

#ifndef __IDCTDEC_WMV2_H_
#define __IDCTDEC_WMV2_H_

#include "wmvdec_api.h"

///////////////////////////////////////////////////////////////////////////
//
// Intra IDCT Functions
//
///////////////////////////////////////////////////////////////////////////

extern Void_WMV g_IDCTDec_WMV2_Intra (U8_WMV* piDst, I32_WMV  iOffsetToNextRowForDCT, const I32_WMV* rgiCoefRecon);


#ifdef _WMV_TARGET_X86_
    Void_WMV g_IDCTDecMMX_WMV2 (U8_WMV __huge* piDst, I32_WMV  iOffsetToNextRowForDCT, const I32_WMV __huge* rgiCoefRecon);
#endif //_WMV_TARGET_X86_

#ifdef	SH3_DSP_ASM_IDCT
    Void_WMV g_IDCTDec_WMV2_init ();
#endif

///////////////////////////////////////////////////////////////////////////
//
// Inter IDCT Functions
//
///////////////////////////////////////////////////////////////////////////


extern  Void_WMV g_IDCTDec16_WMV2 (union Buffer* piDst, union Buffer* piSrc, I32_WMV  iOffsetToNextRowForDCT, I32_WMV  iDCTHorzFlags);

Void_WMV g_IDCTDec_WMV3 (U8_WMV* piDst, I32_WMV  iOffsetToNextRowForDCT, const I32_WMV* rgiCoefRecon);
Void_WMV g_IDCTDec16_WMV3 (union Buffer * piDst, union Buffer * piSrc, I32_WMV iOffsetToNextRowForDCT, I32_WMV iDCTHorzFlags);
Void_WMV g_8x4IDCTDec_WMV3 (union Buffer * piDstBuf, I32_WMV iOffsetToNextRowForDCT, const union Buffer * rgiCoefReconBuf, I32_WMV iHalf);
Void_WMV g_4x8IDCTDec_WMV3 (union Buffer * piDstBuf, I32_WMV iOffsetToNextRowForDCT, const union Buffer * rgiCoefReconBuf, I32_WMV iHalf);
Void_WMV g_4x4IDCTDec_WMV3 (union Buffer * piDstBuf, I32_WMV iOffsetToNextRowForDCT, const union Buffer * rgiCoefReconBuf, I32_WMV iHalf);


#ifndef _EMB_WMV2_
    Void_WMV g_IDCTDec_WMV2_Inter (union Buffer* piDst, union Buffer* piSrc, I32_WMV iOffsetToNextRowForDCT, I32_WMV iDCTHorzFlags);
    Void_WMV g_8x4IDCTDec_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
    Void_WMV g_4x8IDCTDec_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
    Void_WMV g_4x4IDCTDec_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iQuadrant);
#endif //_EMB_WMV2_

    Void_WMV g_8x4IDCTDec16_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
    Void_WMV g_4x8IDCTDec16_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
    Void_WMV g_4x4IDCTDec16_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iQuadrant);


#ifdef _WMV_TARGET_X86_
    Void_WMV g_IDCTDecMMX_WMV2_Inter (union Buffer* piDst, union Buffer* piSrc, I32_WMV  iOffsetToNextRowForDCT, I32_WMV  iDCTHorzFlags);
Void g_IDCTDecMMX_WMV3 (Buffer __huge* piDst, Buffer __huge* piSrc, Int iOffsetToNextRowForDCT, Int iDCTHorzFlags);
Void g_8x4IDCTDecMMX_WMV3 (Buffer __huge* piDstBuf, Int iOffsetToNextRowForDCT, const Buffer __huge* rgiCoefReconBuf, Int iHalf);
Void g_4x8IDCTDecMMX_WMV3 (Buffer __huge* piDstBuf, Int iOffsetToNextRowForDCT, const Buffer __huge* rgiCoefReconBuf, Int iHalf);

#endif //_WMV_TARGET_X86_

#if defined(_WMV_TARGET_X86_) || defined(_Embedded_x86)
    Void_WMV g_8x4IDCTDecMMX_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
    Void_WMV g_4x8IDCTDecMMX_WMV2 (union Buffer* piDstBuf, I32_WMV  iOffsetToNextRowForDCT, const union Buffer* rgiCoefReconBuf, I32_WMV  iHalf);
#endif //_WMV_TARGET_X86_

Void_WMV g_InitIDCTTablesDec ();

#endif // __IDCTDEC_H_
