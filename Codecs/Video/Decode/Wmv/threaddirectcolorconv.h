//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************

#ifndef __THREADDIRECTCOLORCONV_H_
#define __THREADDIRECTCOLORCONV_H_

 #include "xplatform_wmv.h"
#define _INTEL_OPTIMS_COLOR_CONV_
//#pragma pack (1)

#ifdef __MFC_
#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW
#endif // __MFC_

#ifndef _WIN32_WCE
#pragma warning(disable: 4068)
#pragma AVRT_BSS_BEGIN
#endif //_WIN32_WCE

#define BLUE_MASK  0x1f
#define BLUE_MASK_DST 0x001f001f
#define BLUE_SCALE 3 // 8-5

extern I32_WMV g_rgiClipTabArray[510*2];
extern U8_WMV g_rgDitherMap[4][4][3][256];
extern const I32_WMV g_magic4x4[4][4];


#define DITHER_OFFSET 10
#define wDITH24(xy,r,g,b) \
    (rgDitherMap [xy       + r] + \
     rgDitherMap [xy + 256 + g] + \
     rgDitherMap [xy + 512 + b]  )
#ifndef _WIN32_WCE
#pragma AVRT_BSS_END
#endif //_WIN32_WCE


#ifndef _WIN32_WCE
#pragma AVRT_VTABLES_BEGIN
#endif //_WIN32_WCE

typedef     enum FrameType {
	PACKED,
	PLANAR,
    UNKNOWN
}FrameType;

typedef  struct CRct
{

    I32_WMV left, top, right, bottom;
}CRct;



typedef  struct DIRECTCOLORCONVFRM {
    BITMAPINFOHEADER *m_pbihInput;  // Source BITMAPINFOHEADER
    BITMAPINFOHEADER *m_pbihOutput; // Destination (output) BITMAPINFOHEADER

    const BITMAPINFOHEADER *m_pbihSrcFor8bit; // Source BITMAPINFOHEADER for 8bit src
    FrameType m_InputType;
    FrameType m_OutputType;

    const U8_WMV *m_pucInput;  // Pointer point to the packed  input bitmap buffer
    const U8_WMV *m_pucInputY; // Pointer point to the planarY input bitmap buffer
    const U8_WMV *m_pucInputU; // Pointer point to the planarU input bitmap buffer
    const U8_WMV *m_pucInputV; // Pointer point to the planarV input bitmap buffer

    U8_WMV *m_pucOutput;  // Output pointer in case of RGB output
    U8_WMV *m_pucOutputY; // Output Y buffer.   Memory controlled (allocated) by externals.
    U8_WMV *m_pucOutputU; // Output U buffer.   Memory controlled (allocated) by externals.
    U8_WMV *m_pucOutputV; // Output V buffer.   Memory controlled (allocated) by externals.

    U32_WMV m_uiSrcBMPOffset;  // Input packed BMP offset
    U32_WMV m_uiInputYOffset;  // Input Y offset
    U32_WMV m_uiInputUOffset;  // Input U offset
    U32_WMV m_uiInputVOffset;  // Input V offset

    U32_WMV m_uiDstBMPOffset;  // Output packed BMP offset for RGB
    U32_WMV m_uiOutputUOffset; // Output U offset
    U32_WMV m_uiOutputVOffset; // Output V offset

    U32_WMV m_uiStart1, m_uiStart2, m_uiStart3;


    I32_WMV m_iRedMaskDst, m_iRedScaleDst, m_iGreenMaskDst, m_iGreenScaleDst; // For field BITMAP
    I32_WMV m_iRedMaskSrc, m_iRedScaleSrc, m_iGreenMaskSrc, m_iGreenScaleSrc; // For field BITMAP
    I32_WMV m_iRtoY [256];
    I32_WMV m_iRtoU [256];
    I32_WMV m_iRtoV [256];
    I32_WMV m_iGtoY [256];
    I32_WMV m_iGtoU [256];
    I32_WMV m_iGtoV [256];
    I32_WMV m_iBtoYPlus16 [256];
    I32_WMV m_iBtoUPlus128 [256];
    I32_WMV m_iBtoVPlus128 [256];
    I32_WMV m_iVtoR [256];
    I32_WMV m_iVtoG [256];
    I32_WMV m_iUtoG [256];
    I32_WMV m_iUtoB [256];
    I32_WMV m_iYscale [256];
    I32_WMV *m_piClipTab;
    int m_b709;

    I32_WMV m_iSignOfHeightSrc;
    I32_WMV m_iSignOfHeightDst;

    I32_WMV m_iDstWidth;
    I32_WMV m_iDstHeight;
    I32_WMV m_iDstUVWidth;
    I32_WMV m_iDstBMPWidth;
    I32_WMV m_iDstDoubleBMPWidth;
    I32_WMV m_iDstBitsOffset;
    I32_WMV m_iDstYOffset;
    I32_WMV m_iDstUOffset;
    I32_WMV m_iDstVOffset;

    I32_WMV m_iSrcWidth;
    I32_WMV m_iSrcHeight;
    I32_WMV m_iSrcUVWidth;
    I32_WMV m_iSrcBMPWidth;
    I32_WMV m_iSrcDoubleBMPWidth;
    I32_WMV m_iSrcBitsOffset;
    I32_WMV m_iSrcYOffset;
    I32_WMV m_iSrcUOffset;
    I32_WMV m_iSrcVOffset;

    int m_bThreadInited;
    int m_bShutdownThreads;
    U32_WMV m_uiNumProcessors;
    HANDLE hPrcEvent0, hPrcDone0;
    HANDLE hPrcEvent1, hPrcDone1;
    HANDLE hPrcEvent2, hPrcDone2;
    HANDLE hPrcEvent3, hPrcDone3;
    HANDLE threadPrc0, threadPrc1, threadPrc2, threadPrc3;

    // for encoder CC
    CRct m_rctCropped;
    int m_bCropped;

    // for Full Cropping
    int m_bFullCropped;
    I32_WMV  m_iFullSrcWidth;
    I32_WMV  m_iFullSrcHeight;
    I32_WMV  m_iFullDstWidth;
    I32_WMV  m_iFullDstHeight;
    I32_WMV  m_iSrcCropLeft;
    I32_WMV  m_iSrcCropTop;
    I32_WMV  m_iDstCropLeft;
    I32_WMV  m_iDstCropTop;

    I32_WMV  m_mode;

    I32_WMV  m_iStrideY;
    I32_WMV  m_iStrideU;
    I32_WMV  m_iStrideV;

    I32_WMV  m_iPitchSrc;
    I32_WMV  m_iPitchDst;
    I32_WMV  m_iSrcUVStride;
    I32_WMV  m_iDstUVStride;

    //unsigned char*        m_pucTempSrc;
    //unsigned char*        m_pucTempDst;
    //BITMAPINFOHEADER*     m_pbihCropSrc;
    //BITMAPINFOHEADER*     m_pbihCropDst;

  Void_WMV (*m_pPacked2PackedColorConvProc)(
    const U8_WMV*           pucInput,       // Pointer points to the input packed bitmap buffer
	U8_WMV*                 pucOutput,      // Pointer points to the output packed RGB buffer
	const I32_WMV           iFirst,         // first line
	const I32_WMV           iLast,          // last line
	struct DIRECTCOLORCONVFRM* diccs
  );

  Void_WMV (*m_pPacked2PlanarColorConvProc)(
    const U8_WMV*           pucInput,       // Pointer points to the input packed bitmap buffer
	U8_WMV*                 pucOutputY,     // Pointer points to the output planar Y buffer
	U8_WMV*                 pucOutputU,     // Pointer points to the output planar U buffer
	U8_WMV*                 pucOutputV,     // Pointer points to the output planar V buffer
	const I32_WMV           iFirst,         // first line
	const I32_WMV           iLast,          // last line
	struct DIRECTCOLORCONVFRM* diccs
  );

  Void_WMV (*m_pPlanar2PlanarColorConvProc)(
    const U8_WMV*           pucInputY,      // Pointer points to the input planar Y buffer
	const U8_WMV*           pucInputU,      // Pointer points to the input planar U buffer
	const U8_WMV*           pucInputV,      // Pointer points to the input planar V buffer
	U8_WMV*                 pucOutputY,     // Pointer points to the output planar Y buffer
	U8_WMV*                 pucOutputU,     // Pointer points to the output planar U buffer
	U8_WMV*                 pucOutputV,     // Pointer points to the output planar V buffer
	const I32_WMV           iFirst,         // first line
	const I32_WMV           iLast,          // last line
	struct DIRECTCOLORCONVFRM* diccs
  );

  Void_WMV (*m_pPlanar2PackedColorConvProc)(
    U8_WMV*                 pucOutput,     // Pointer to the output packed bitmap buffer
	const U8_WMV*           pucInputY,     // Pointer to the input planar Y buffer
	const U8_WMV*           pucInputU,     // Pointer to the input planar U buffer
	const U8_WMV*           pucInputV,     // Pointer to the input planar V buffer
	const I32_WMV           iFirst,        // first line
	const I32_WMV           iLast,         // last line
	struct DIRECTCOLORCONVFRM* diccs
  );
} DIRECTCC;


Void_WMV colorconv_process0 ( DIRECTCC* diccs );
Void_WMV colorconv_process1 ( DIRECTCC* diccs );
Void_WMV colorconv_process2 ( DIRECTCC* diccs );
Void_WMV colorconv_process3 ( DIRECTCC* diccs );

/*
Void_WMV InitThreads ( DIRECTCC* diccs );
Void_WMV CloseThreads ( DIRECTCC* diccs );
Void_WMV FinishThreads( DIRECTCC* diccs );




Void_WMV Set709ColorConversion( DIRECTCC* diccs );
*/

Void_WMV PrepareThreads( DIRECTCC* diccs );
#if defined (WIN32) && defined (WMV_TARGET_X86) && !defined (UNDER_CE)
int VerticalFlipPackedYUVRegistryKeySet ();
#endif

Void_WMV SetSrcYVUtoRGBTable ( DIRECTCC* diccs );
Void_WMV SetSrcRGBtoYVUTable ( DIRECTCC* diccs );
Void_WMV SetDstDitherMap ( DIRECTCC* diccs );
tColorConvertStatus InitSrcRGBInfo (const BITMAPINFOHEADER *pbmih, DIRECTCC* diccs );
tColorConvertStatus InitRGBInfo (DIRECTCC* diccs );
Void_WMV Init(DIRECTCC* diccs);
tColorConvertStatus ConfigParam( DIRECTCC* diccs );

tColorConvertStatus CheckSizeD( DIRECTCC* diccs );
Void_WMV SetSequencePointers( DIRECTCC* diccs);

Void_WMV ReSetSequencePointers( DIRECTCC* diccs,
                            I32_WMV       iFullSrcWidth,
                            I32_WMV       iFullSrcHeight,
                            I32_WMV       iFullDstWidth,
                            I32_WMV       iFullDstHeight );


tColorConvertStatus directVideoColorConv_MultiThreads (
    const U8_WMV *pucInput, // Pointer points to the input bitmap buffer
    U8_WMV *pucOutput,      // Output bitmap buffer.  Memory controlled
								        // (allocated) by externals.
	DIRECTCC* diccs,
    U8_WMV *pucInputU ,      // Output bitmap buffer.  Memory controlled
								        // (allocated) by externals.
    U8_WMV *pucInputV       // Output bitmap buffer.  Memory controlled
								        // (allocated) by externals.
);

Void_WMV CopyBuffer(U8_WMV*       puiDst,
				const U8_WMV* puiSrc,
				I32_WMV       iSrcWidth,
				I32_WMV       iSrcHeight,
				I32_WMV       iDstWidth,
				I32_WMV       iDstHeight,
				int       iSrcCropLeft,
				int       iSrcCropTop,
				int       iDstCropLeft,
				int       iDstCropTop,
				int       iCropWidth,
				int       iCropHeight,
				DWORD     biCompression,
				I32_WMV       iBitCount,
				const U8_WMV* puiSrcU ,
				const U8_WMV* puiSrcV
                );



Void_WMV RefreshSrcFrmFromI420 (
  const U8_WMV*   pBitsY,
	const U8_WMV*   pBitsU,
	const U8_WMV*   pBitsV,
	U8_WMV*     ppxliCurrQYFrm,
	U8_WMV*     ppxliCurrQUFrm,
	U8_WMV*     ppxliCurrQVFrm,
	const I32_WMV iStartLineNumY,
	const I32_WMV iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshSrcI420From411 (
  const U8_WMV*   pBitsY,
	const U8_WMV*   pBitsU,
	const U8_WMV*   pBitsV,
	U8_WMV*     ppxliCurrQYFrm,
	U8_WMV*     ppxliCurrQUFrm,
	U8_WMV*     ppxliCurrQVFrm,
	const I32_WMV     iStartLineNumY,
	const I32_WMV     iEndLineNumY,
	DIRECTCC*   diccs
);


Void_WMV RefreshI420FromP422(
    const U8_WMV*   pBitsY,
    const U8_WMV*   pBitsU,
    const U8_WMV*   pBitsV,
    U8_WMV*         ppxliCurrQYFrm,
    U8_WMV*         ppxliCurrQUFrm,
    U8_WMV*         ppxliCurrQVFrm,
    const I32_WMV   iStartLineNumY,
    const I32_WMV   iEndLineNumY,
    DIRECTCC*   ccs
    );

Void_WMV RefreshYUY2FromRGB16 (
    const U8_WMV*   pBitsIn,
    U8_WMV*         pBitsOut,
    const I32_WMV         iStartLineNumY,
    const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshYUY2FromRGB24 (
	const U8_WMV*   pBitsIn,
	U8_WMV*         pBitsOut,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshYUY2FromRGB32 (
  const U8_WMV*   pBitsIn,
	U8_WMV*         pBitsOut,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs
);

Void_WMV RefreshSrcYUY2From420(
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs);



Void_WMV RefreshSrcYUY2From420KNI(
  U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV         iStartLineNumY,
	const I32_WMV         iEndLineNumY,
	DIRECTCC*   diccs);

Void_WMV RefreshSrcYUY2From411 (
  U8_WMV*        pBits,
	const U8_WMV*  ppxliCurrQYFrm,
	const U8_WMV*  ppxliCurrQUFrm,
	const U8_WMV*  ppxliCurrQVFrm,
	const I32_WMV        iStartLineNumY,
	const I32_WMV        iEndLineNumY,
	DIRECTCC*   diccs
	);



Void_WMV RefreshSrcYUY2From420Interlace(
    U8_WMV*         pBits,
	const U8_WMV*   ppxliCurrQYFrm,
	const U8_WMV*   ppxliCurrQUFrm,
	const U8_WMV*   ppxliCurrQVFrm,
	const I32_WMV   iStartLineNumY,
	const I32_WMV   iEndLineNumY,
	DIRECTCC*   diccs);



Void_WMV RefreshSrcYUY2FromUYVY (
	const U8_WMV*   pBitsIn,
	U8_WMV*     pBitsOut,
	const I32_WMV iStartLineNumY,
	const I32_WMV iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV RefreshSrcYUY2FromYVYU (
	const U8_WMV*   pBitsIn,
	U8_WMV*     pBitsOut,
	const I32_WMV iStartLineNumY,
	const I32_WMV iEndLineNumY,
	DIRECTCC*   diccs
	);

Void_WMV UpSampleInterlace
(   const U8_WMV* pSrcU,
    const U8_WMV* pSrcV,
    U8_WMV*       pDstU,
    U8_WMV*       pDstV,
    I32_WMV       iSrcStride,
    I32_WMV       iDstStride,
    I32_WMV       iSrcJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iDstJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iWidth,
    I32_WMV       iHeight,
    int      bDoTop,                  // indicate whether or not to process top line.
    int      bDoBottom,               // indicate whether or not to process bottom line.
    int      bDoBottomRepeat,         // indicate whether or not to repeat bottom line.
    I32_WMV       iParam0,
    I32_WMV       iParam1,
    I32_WMV       iParam2,
    I32_WMV       iParam3 );

Void_WMV YUVConversion
(   const U8_WMV* pSrc,
    U8_WMV*       pDst,
    I32_WMV       iSrcStride,
    I32_WMV       iDstStride,
    I32_WMV       iSrcJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iDstJumpPeriod, // 420, 422 are 2; YUY2, UYVY, YVYU are 4; ...
    I32_WMV       iWidth,
    I32_WMV       iHeight,
    I32_WMV       iDup);

Void_WMV RefreshSrcI420From411 (
  const U8_WMV*   pBitsY,
	const U8_WMV*   pBitsU,
	const U8_WMV*   pBitsV,
	U8_WMV*     ppxliCurrQYFrm,
	U8_WMV*     ppxliCurrQUFrm,
	U8_WMV*     ppxliCurrQVFrm,
	const I32_WMV     iStartLineNumY,
	const I32_WMV     iEndLineNumY,
	DIRECTCC*   diccs
);





#ifndef _WIN32_WCE
#pragma AVRT_VTABLES_END
#endif //_WIN32_WCE


#endif
//#pragma pack ()

