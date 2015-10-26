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
//  File:       bitfields.h
//
//--------------------------------------------------------------------------

#ifndef __BITFIELDS_H_
#define __BITFIELDS_H_
#include "wmc_typedef.h"
//#ifndef _WINGDI_

#if defined(__MACVIDEO__) || defined(__UNIXVIDEO__) || defined(_Embedded_x86) || defined(__arm) || defined(_TI_C55X_) || defined(_XBOX) || defined(_APOGEE_)

#   ifndef _COLORSCHEME
#       define _COLORSCHEME
#       ifndef _XBOX
#           define BI_RGB        0L
#       endif
#       define BI_RLE8       1L
#       define BI_RLE4       2L
#       define BI_BITFIELDS  3L
#   endif

#   ifndef _BITMAPINFOHEADER
#       define _BITMAPINFOHEADER
        typedef  struct tagBITMAPINFOHEADER{
            U32_WMV      biSize;
            Long       biWidth;
            Long       biHeight;
            U16_WMV       biPlanes;
            U16_WMV       biBitCount;
            U32_WMV      biCompression;
            U32_WMV      biSizeImage;
            Long       biXPelsPerMeter;
            Long       biYPelsPerMeter;
            U32_WMV      biClrUsed;
            U32_WMV      biClrImportant;
        } BITMAPINFOHEADER, *LPBITMAPINFOHEADER;//, *PBITMAPINFOHEADER;

#   endif

    typedef  struct tagRGBQUAD {
            U8_WMV    rgbBlue;
            U8_WMV    rgbGreen;
            U8_WMV    rgbRed;
            U8_WMV    rgbReserved;
    } RGBQUAD;

    typedef  struct tagBITMAPINFO {
        BITMAPINFOHEADER    bmiHeader;
#       ifdef __arm
            RGBQUAD             bmiColors[3];
#       else
            RGBQUAD             bmiColors[1];
#       endif
    } BITMAPINFO;// FAR *LPBITMAPINFO, *PBITMAPINFO;

#else
#   if defined(_WIN32) || defined(_WIN16)
#       include <windows.h>
#       ifdef UNDER_CE
#           include "vfw_ce.h"
#       else
#           include <vfw.h>
#       endif // UNDER_CE
#   endif
#endif

#if !defined(_WIN32)
#if defined(_Embedded_x86)||defined(_APOGEE_) || defined (_TI_C55X_) || defined (__arm)
    typedef  struct tagBITMAPINFOHEADER_BITFIELDS{
        U32_WMV      biSize;
        Long       biWidth;
        Long       biHeight;
        U16_WMV       biPlanes;
        U16_WMV       biBitCount;
        U32_WMV      biCompression;
        U32_WMV      biSizeImage;
        Long       biXPelsPerMeter;
        Long       biYPelsPerMeter;
        U32_WMV      biClrUsed;
        U32_WMV      biClrImportant;
        U32_WMV dwMask[3];
    } BITMAPINFOHEADER_BITFIELDS;
#else    // _Embedded_x86
    struct BITMAPINFOHEADER_BITFIELDS : tagBITMAPINFOHEADER {
        U32_WMV dwMask[3];
    };
#endif   // _Embedded_x86
#else   // _WIN32
    typedef  struct tagBITMAPINFOHEADER_BITFIELDS{
        U32_WMV      biSize;
        I32_WMV       biWidth;
        I32_WMV       biHeight;
        U16_WMV       biPlanes;
        U16_WMV       biBitCount;
        U32_WMV      biCompression;
        U32_WMV      biSizeImage;
        I32_WMV       biXPelsPerMeter;
        I32_WMV       biYPelsPerMeter;
        U32_WMV      biClrUsed;
        U32_WMV      biClrImportant;
        U32_WMV dwMask[3];
    } BITMAPINFOHEADER_BITFIELDS;
#endif  // _WIN32

#endif  // __BITFIELDS_H_
