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
//  Copyright (C) Microsoft Corporation, 1998 - 1999
//
//  File:       msmton.hpp
//
//--------------------------------------------------------------------------

#ifndef __MSMtoN_HPP_
#define __MSMtoN_HPP_
//#pragma pack (1)


#if !(defined(_Embedded_x86) || defined(UNDER_CE) || defined(macintosh))
//#include <mmsystem.h>
//#include <vfw.h>
#endif

#pragma warning(disable: 4068)
#pragma AVRT_VTABLES_BEGIN

//#include "typincld.h"


//#if !(defined(_Embedded_x86) || defined(UNDER_CE) || defined(macintosh))
/*int  NSStretchDIBits(
  HDC hdc,                // handle to device context
  int XDest,              // x-coordinate of upper-left corner of dest. rectangle
  int YDest,              // y-coordinate of upper-left corner of dest. rectangle
  int nDestWidth,         // width of destination rectangle
  int nDestHeight,        // height of destination rectangle
  int XSrc,               // x-coordinate of upper-left corner of source rectangle
  int YSrc,               // y-coordinate of upper-left corner of source rectangle
  int nSrcWidth,          // width of source rectangle
  int nSrcHeight,         // height of source rectangle
  void *lpBits,            // address of bitmap bits
  BITMAPINFO *lpBitsInfo,  // address of bitmap data
  U32_WMV iUsage,                   // usage flags
  DWORD dwRop                    // raster operation code
);		*/
//#endif



typedef  struct tagCMSMtoN
{

    // thread handling stuff, need to be public

    /*
    U32_WMV m_hEvent0, m_hDone0;
    U32_WMV m_hEvent1, m_hDone1;
    U32_WMV m_hEvent2, m_hDone2;
    U32_WMV m_hEvent3, m_hDone3;
    U32_WMV m_hThread0;
    U32_WMV m_hThread1;
    U32_WMV m_hThread2;
    U32_WMV m_hThread3;
    unsigned long m_uiThreadID0;
    unsigned long m_uiThreadID1;
    unsigned long m_uiThreadID2;
    unsigned long m_uiThreadID3;
    */
    Bool_WMV m_bShutdownThreads;
    Bool_WMV m_bResizeWidth;
    Bool_WMV m_bResizeHeight;

    Void_WMV    (* m_preszWidth ) (  struct tagCMSMtoN * pThis, int iStart, int iEnd);
    Void_WMV    (* m_preszHeight) (  struct tagCMSMtoN * pThis, int iStart, int iEnd);
    U32_WMV    m_uiNumSlices;
    int     m_iOldWidth;
    int     m_iOldHeight;
    int     m_iNewWidth;
    int     m_iNewHeight;
    int     m_iField;
    int     m_HeightFactor;
    //inline void ResizeHeight (int iStart, int iEnd) {(this->*m_preszHeight) (iStart, iEnd);};
    //inline void ResizeWidth  (int iStart, int iEnd) {(this->*m_preszWidth ) (iStart, iEnd);};


    /*
    void ResizeRGB32Height             (int iStart, int iEnd);
    void ResizeRGB32Width              (int iStart, int iEnd);
    void ResizeRGB24Height             (int iStart, int iEnd);
    void ResizeRGB24Width              (int iStart, int iEnd);
    void ResizeRGB16Height             (int iStart, int iEnd);
    void ResizeRGB16Width              (int iStart, int iEnd);
    void ResizeRGB8Height             (int iStart, int iEnd);
    void ResizeRGB8Width              (int iStart, int iEnd);
    void ResizeRGB32HeightMMX          (int iStart, int iEnd);
    void ResizeRGB32WidthMMX           (int iStart, int iEnd);
    void ResizeRGB24HeightMMX          (int iStart, int iEnd);
    void ResizeRGB24WidthMMX           (int iStart, int iEnd);
    void ResizeYUY2_UYVYHeight         (int iStart, int iEnd);
    void ResizeYUY2Width               (int iStart, int iEnd);
    void ResizeUYVYWidth               (int iStart, int iEnd);
    void ResizeYUY2_UYVYToIYUVHeight   (int iStart, int iEnd);
    void ResizeV216Width               (int iStart, int iEnd);
    void ResizeV216Height              (int iStart, int iEnd);
    void ResizeYUY2ToIYUVWidth         (int iStart, int iEnd);
    void ResizeUYVYToIYUVWidth         (int iStart, int iEnd);
    void ResizeYUVPlanerHeight         (int iStart, int iEnd);
    void ResizeYUVPlanerHeightMMX      (int iStart, int iEnd);
    void ResizeYUVPlanerWidth          (int iStart, int iEnd);
	void ResizeYUVPlanerWidth9to8MMX   (int iStart, int iEnd);
    void ResizeYVU9Height		       (int iStart, int iEnd);
    void ResizeYVU9Width	           (int iStart, int iEnd);
    void ResizeYUVPlanerMMX            (int iStart, int iEnd); //Single Pass Resize YUV Planer
    void Dummy                         (int iStart, int iEnd); //Empty function for other pass of single pass
    */
    BITMAPINFOHEADER* m_dibPtr;
    const unsigned char* m_dibBits;
    unsigned char* m_pFrame;
    unsigned char* m_ptFrame;
    unsigned char* ptFrame;

    Bool_WMV m_bSinglePass;

}CMSMtoN;

Void_WMV CMSMtoN_DestructCMSMtoN (CMSMtoN * pThis);
Void_WMV CMSMtoN_CMSMtoN (CMSMtoN * pThis);
    Bool_WMV CMSMtoN_Init  (CMSMtoN * pThis, int iOldWidth, int iOldHeight, int iNewWidth, int iNewHeight);
    void CMSMtoN_Reset (CMSMtoN * pThis, int iOldWidth, int iOldHeight, int iNewWidth, int iNewHeight);

    Bool_WMV CMSMtoN_Resize (CMSMtoN * pThis,
        BITMAPINFOHEADER *dibPtr, // for input (original) data
        const unsigned char* pucSrc, // source data
        unsigned char *pucDst, // resized data
        int iWidthDst, int iHeightDst // resized width and height
    );

#pragma AVRT_VTABLES_END
#endif
//#pragma pack ()

