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
//  Copyright (C) Microsoft Corporation, 1998 - 1998
//
//  File:       postfilter.h
//
//--------------------------------------------------------------------------

#ifndef __POSTFILTER_H__
#define __POSTFILTER_H__
//#pragma pack (1)

#include "wmvdec_api.h"

#define DEBLOCKMB_ARGS      tWMVDecInternalMember *pWMVDec, \
                            U8_WMV __huge        *ppxliY,           \
                            U8_WMV __huge        *ppxliU,           \
                            U8_WMV __huge        *ppxliV,           \
                            Bool_WMV                  bDoLeft,          \
                            Bool_WMV                  bDoRightHalfEdge, \
                            Bool_WMV                  bDoTop,           \
                            Bool_WMV                  bDoBottomHalfEdge,\
                            Bool_WMV                  bDoMiddle,        \
                            I32_WMV                   iStepSize,        \
                            I32_WMV                   iWidthPrevY,      \
                            I32_WMV                   iWidthPrevUV

#define APPLYSMOOTHING_ARGS U8_WMV                *pixel,           \
                            I32_WMV                   width,            \
                            I32_WMV                   max_diff,         \
                            I32_WMV                   thr

#define DETERMINETHR_ARGS   U8_WMV                *ptr,             \
                            I32_WMV                   *thr,             \
                            I32_WMV                   *range,           \
                            I32_WMV                   width

#define DERINGMB_ARGS       U8_WMV __huge        *ppxlcY,           \
                            U8_WMV __huge        *ppxlcU,           \
                            U8_WMV __huge        *ppxlcV,           \
                            I32_WMV                  iStepSize,         \
                            I32_WMV                  iWidthPrevY,       \
                            I32_WMV                  iWidthPrevUV


extern Void_WMV (*g_pDeblockMB)(DEBLOCKMB_ARGS);
extern Void_WMV (*g_pApplySmoothing)(APPLYSMOOTHING_ARGS);
extern Void_WMV (*g_pDetermineThreshold)(DETERMINETHR_ARGS);
extern Void_WMV (*g_pDeringMB)(DERINGMB_ARGS);

Void_WMV	DeblockMB(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_Short(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_Improved(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_MMX(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_MMX_Improved(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_FASTEST_MMX(DEBLOCKMB_ARGS);
Void_WMV	DeblockMB_KNI(DEBLOCKMB_ARGS);

Void_WMV	DeblockMBInterlace411(DEBLOCKMB_ARGS);
Void_WMV	DeblockMBInterlace411_Short(DEBLOCKMB_ARGS);

Void_WMV DeringMB(DERINGMB_ARGS);

Void_WMV ApplySmoothing(APPLYSMOOTHING_ARGS);
Void_WMV ApplySmoothing_Improved(APPLYSMOOTHING_ARGS);
Void_WMV ApplySmoothing_MMX(APPLYSMOOTHING_ARGS);
Void_WMV ApplySmoothing_MMX_Improved(APPLYSMOOTHING_ARGS);

Void_WMV	DetermineThreshold(DETERMINETHR_ARGS);
Void_WMV	DetermineThreshold_MMX(DETERMINETHR_ARGS);
Void_WMV	DetermineThreshold_MMX_Improved(DETERMINETHR_ARGS);
Void_WMV	DetermineThreshold_KNI(DETERMINETHR_ARGS);
Void_WMV	DetermineThreshold_KNI_Improved(DETERMINETHR_ARGS);

Void_WMV g_InitPostFilter (Bool_WMV bFastDeblock, Bool_WMV bYUV411);
#endif

//#pragma pack ()

