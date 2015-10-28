//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// Modual: resizer
//
// Supported color format
//    high quality: IYUV, I420, P411
//    low quality:  RGB, YUY2, UYVY, YVU9, IYUV, YV12, I420
//
//*@@@---@@@@******************************************************************
#ifndef WMA_RESZ_H
#define WMA_RESZ_H
//#pragma pack (1)

//#include "video_common.h"

#if !(defined(_Embedded_x86) || defined(UNDER_CE))
#ifndef __MFC_
//#include <windows.h>
#endif // __MFC_
#endif

#if !(defined(_Embedded_x86) || defined(UNDER_CE) || defined(macintosh))
//#include <mmsystem.h>
//#include <vfw.h>
#endif

#if defined(macintosh)
#define S_OK                                   ((I32_WMV)0x00000000L)
#define S_FALSE                                ((I32_WMV)0x00000001L)
#define BI_BITFIELDS  3L
#endif

#define WMAERR_OK               0
#define WMAERR_FAIL             1
#define WMAERR_INVALIDARG       2
#define WMAERR_BUFFERTOOSMALL   3
#define WMAERR_INVALIDHEADER    4
#define WMAERR_OUTOFMEMORY      5
#define WMAERR_CORRUPTDATA      6

#define RZ_NOCHANGE            -1
#define RZ_PROGRESSIVE          0
#define RZ_INTERLACE            1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef WMAPI
#define WMAPI  __stdcall
#endif

typedef Void_WMV * HWMARESIZER;
#if defined(_Embedded_x86) || defined(UNDER_CE)
typedef I32_WMV I32_WMV;
#undef WMAPI
#define WMAPI
#endif

#ifdef UNDER_CE
#include <wingdi.h>
#endif

#pragma warning(disable: 4068)
#pragma AVRT_VTABLES_BEGIN

/****************************************************************************/
int WMOpenResizer_C(
                    const BITMAPINFOHEADER *pbHeader,
                    HWMARESIZER *phResizer,
                    I32_WMV dwNewWidth,
                    I32_WMV dwNewHeight,
                    Bool_WMV fUseHighQualityResizer,
                    Bool_WMV bInterlace411);
#ifndef _SUPPORTVCM_
int WMOpenResizer_old(
                    const BITMAPINFOHEADER *pbHeader,
                    HWMARESIZER *phResizer,
                    I32_WMV dwNewWidth,
                    I32_WMV dwNewHeight,
                    Bool_WMV fUseHighQualityResizer ,
                    Bool_WMV bInterlace411 );
#endif
/****************************************************************************/
int WMResize_old(
                    HWMARESIZER hResizer,
                    I8_WMV    *pbSrc,
                    I32_WMV   dwSrc,
                    I32_WMV   *pdwSrcUsed,
                    I8_WMV    *pbDst,
                    I32_WMV   dwDst,
                    I32_WMV   *pdwDstUsed );

/****************************************************************************/
int WMCloseResizer(
                    HWMARESIZER hResizer );


//////////////////////////////////////////////////////////////
/////////////    ClipResizer   ///////////////////////////////

int WMOpenClipResizer_C(
                    const BITMAPINFOHEADER *pbHeader,
                    HWMARESIZER *phResizer,
					I32_WMV dwClipOriXSrc,
					I32_WMV dwClipOriYSrc,
					I32_WMV dwClipWidthSrc,
					I32_WMV dwClipHeightSrc,
                    I32_WMV dwNewWidth,
                    I32_WMV dwNewHeight,
                    Bool_WMV fUseHighQualityResizer,
                    Bool_WMV bInterlace411);

#ifdef _SUPPORTVCM_
int WMOpenClipResizer(const BITMAPINFOHEADER *pbHeader,
								HWMARESIZER *phResizer,
								I32_WMV dwClipOriXSrc,
								I32_WMV dwClipOriYSrc,
								I32_WMV dwClipWidthSrc,
								I32_WMV dwClipHeightSrc,
								I32_WMV dwNewWidth,
								I32_WMV dwNewHeight,
								Bool_WMV  fUseHighQualityResizer,
								Bool_WMV  bInterlace411);
#else
int WMOpenClipResizer(const BITMAPINFOHEADER *pbHeader,
								HWMARESIZER *phResizer,
								I32_WMV dwClipOriXSrc,
								I32_WMV dwClipOriYSrc,
								I32_WMV dwClipWidthSrc,
								I32_WMV dwClipHeightSrc,
								I32_WMV dwNewWidth,
								I32_WMV dwNewHeight,
								Bool_WMV  fUseHighQualityResizer      ,
								Bool_WMV  bInterlace411               );
#endif

int WMClipResize(HWMARESIZER hResizer,
						   I8_WMV*       pbSrc,
						   I32_WMV       dwSrc,
						   I32_WMV*      pdwSrcUsed,
						   I8_WMV*       pbDst,
						   I32_WMV       dwDst,
						   I32_WMV*      pdwDstUsed
						   );

int WMCloseClipResizer(HWMARESIZER hResizer);



#ifdef _SUPPORTVCM_
int WMOpenResizer(const BITMAPINFOHEADER* pbHeader,
							HWMARESIZER*            phResizer,
							I32_WMV                   dwNewWidth,
							I32_WMV                   dwNewHeight,
							Bool_WMV                    fUseHighQualityResizer,
							Bool_WMV                    bInterlace411,
							I32_WMV                   dwClipOriXSrc,
							I32_WMV                   dwClipOriYSrc,
							I32_WMV                   dwClipWidthSrc,
							I32_WMV                   dwClipHeightSrc);
#else
int WMOpenResizer(const BITMAPINFOHEADER* pbHeader,
							HWMARESIZER*            phResizer,
							I32_WMV                   dwNewWidth,
							I32_WMV                   dwNewHeight,
							Bool_WMV                    fUseHighQualityResizer ,
							Bool_WMV                    bInterlace411          ,
							I32_WMV                   dwClipOriXSrc          ,
							I32_WMV                   dwClipOriYSrc          ,
							I32_WMV                   dwClipWidthSrc         ,
							I32_WMV                   dwClipHeightSrc        );
#endif
int WMResize(
    HWMARESIZER             hResizer,
    I8_WMV*                   pbSrc,
    I32_WMV                   dwSrc,
    I32_WMV*                  pdwSrcUsed,
    I8_WMV*                   pbDst,
    I32_WMV                   dwDst,
    I32_WMV*                  pdwDstUsed
    );

int WMOpenResizerWithFullCropping(
    const BITMAPINFOHEADER* pbHeader,
    HWMARESIZER*            phResizer,
    // Size of destination image. Format must be same as source
     I32_WMV                iDstWidth,
     I32_WMV                iDstHeight,
    // size of cropping region in source image
     I32_WMV                iSrcCropLeft,
     I32_WMV                iSrcCropTop,
     I32_WMV                iSrcCropWidth,
     I32_WMV                iSrcCropHeight,
    // size of cropping region in destination image
     I32_WMV                iDstCropLeft,
     I32_WMV                iDstCropTop,
     I32_WMV                iDstCropWidth,
     I32_WMV                iDstCropHeight,
    // quality
    Bool_WMV                    fUseHighQualityResizer,
    Bool_WMV                    bInterlace411);

int WMResizeWithFullCropping(
    HWMARESIZER             hResizer,
	I8_WMV*                   pbSrc,
	I32_WMV                   dwSrc,
	I32_WMV*                  pdwSrcUsed,
	I8_WMV*                   pbDst,
	I32_WMV                   dwDst,
	I32_WMV*                  pdwDstUsed	);

    /////////////////////////////////////////////////////////
    /////   support interlace

int WMNewOpenResizerBasic(
    const BITMAPINFOHEADER* pbHeader,
    HWMARESIZER*            phResizer,
    I32_WMV                   dwNewWidth,
    I32_WMV                   dwNewHeight,
    Bool_WMV                    fUseHighQualityResizer,
    Bool_WMV                    bInterlace);

// handle both interlace and progressive
int WMNewOpenResizerWithFullCropping(
    const BITMAPINFOHEADER* pbHeader,
    HWMARESIZER*            phResizer,
    // Size of destination image. Format must be same as source
    I32_WMV                iDstWidth,
    I32_WMV                iDstHeight,
    // size of cropping region in source image
    I32_WMV                iSrcCropLeft,
    I32_WMV                iSrcCropTop,
    I32_WMV                iSrcCropWidth,
    I32_WMV                iSrcCropHeight,
    // size of cropping region in destination image
    I32_WMV                iDstCropLeft,
    I32_WMV                iDstCropTop,
    I32_WMV                iDstCropWidth,
    I32_WMV                iDstCropHeight,
    // quality
    Bool_WMV                    fUseHighQualityResizer,
    // interlace
    Bool_WMV                    bInterlace
);

int WMNewResize(
    HWMARESIZER             hResizer,
    I8_WMV*                   pbSrc,
    I32_WMV                   dwSrc,
    I32_WMV*                  pdwSrcUsed,
    I8_WMV*                   pbDst,
    I32_WMV                   dwDst,
    I32_WMV*                  pdwDstUsed
);

int WMNewResizePerFrame(
    HWMARESIZER             hResizer,
    I8_WMV*                   pbSrc,
    I32_WMV                   dwSrc,
    I32_WMV*                  pdwSrcUsed,
    I8_WMV*                   pbDst,
    I32_WMV                   dwDst,
    I32_WMV*                  pdwDstUsed,
    I32_WMV                iModeChange  // options: RZ_NOCHANGE/RZ_PROGRESSIVE/RZ_INTERLACE
);

int WMNewCloseResizer(
    HWMARESIZER             hResizer
    );
int WMResizerGetQuality(
    HWMARESIZER             hResizer,
    Bool_WMV                    bInterlace,
    Bool_WMV*                   bQuality
    );

int WMResetCropping(
    HWMARESIZER*            phResizer,
    // size of cropping region in source image
    I32_WMV                iSrcCropLeft,
    I32_WMV                iSrcCropTop,
    I32_WMV                iSrcCropWidth,
    I32_WMV                iSrcCropHeight,
    // size of cropping region in destination image
    I32_WMV                iDstCropLeft,
    I32_WMV                iDstCropTop,
    I32_WMV                iDstCropWidth,
    I32_WMV                iDstCropHeight);


#pragma AVRT_VTABLES_END

#ifdef __cplusplus
}
#endif
//#pragma pack ()

#endif // WMA_RESZ_H
