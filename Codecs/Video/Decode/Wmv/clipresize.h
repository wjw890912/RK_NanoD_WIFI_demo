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
//  Copyright (C) Microsoft Corporation, 2003
//
//  File:       clipresize.hpp
//
//--------------------------------------------------------------------------
//#pragma pack (1)

#ifndef __CLIP_RESIZE_HPP_
#define __CLIP_RESIZE_HPP_

#if !(defined(_Embedded_x86) || defined(UNDER_CE) || defined(macintosh))
//#include <mmsystem.h>
//#include <vfw.h>
//#include "fourcc_guid.h"
#endif

#ifndef _WIN32_WCE

#pragma warning(disable: 4068)
#pragma AVRT_VTABLES_BEGIN
#endif //_WIN32_WCE
//PanZoom works with a table of function pointers wich are setup on a per frame basis. Each entry in the table
//indicates the optimal MMX function for processing a set of 4 pixels.
#define PanZoomFunParams unsigned char *pDst,unsigned char *pTmpRef,short *pIx,short *pRx,short *ry4_4way, int iOldWidth
typedef void (PanZoomFun)(PanZoomFunParams);


typedef struct tagCClipResize
{

    // thread handling stuff, need to be public

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
    Bool_WMV m_bShutdownThreads;
    Bool_WMV m_bResizeWidth;
    Bool_WMV m_bResizeHeight;


    U32_WMV    m_uiNumSlices;
    int     m_iOldWidth;
    int     m_iOldHeight;
    int     m_iNewWidth;
    int     m_iNewHeight;
    int     m_iField;




    unsigned char* m_dibBitsY;
    unsigned char* m_dibBitsU;
    unsigned char* m_dibBitsV;
    unsigned char* m_pFrameY;
    unsigned char* m_pFrameU;
    unsigned char* m_pFrameV;
    unsigned char* m_pBuffer;
    unsigned char* m_ptFrame;
    unsigned char* ptFrame;

}CClipResize;

Void_WMV CClipResize_DestructCClipResize (CClipResize * pThis);
Void_WMV CClipResize_CClipResize (CClipResize * pThis);

    void CClipResize_Init (CClipResize * pThis,
                int iOldWidth,
                int iOldHeight,
                int iNewWidth,
                int iNewHeight);

    void CClipResize_ClipResizeHighQuality (CClipResize * pThis,
        const unsigned char* pucSrcY,
        const unsigned char* pucSrcU,
        const unsigned char* pucSrcV,
        unsigned char *pucDstY, //
        unsigned char *pucDstU, //
        unsigned char *pucDstV, //
        double fZoomX, double fX,
        double fZoomY, double fY,
        DWORD dwFourCC //= 0x30323449 //FOURCC_I420
    );

    void CClipResize_ClipResizeLowQuality ( CClipResize * pThis,
         unsigned char* pucSrcY,
         unsigned char* pucSrcU,
         unsigned char* pucSrcV,
        unsigned char *pucDstY,
        unsigned char *pucDstU,
        unsigned char *pucDstV,
        double fZoomX, double fX,
        double fZoomY, double fY);

void CClipResize_ResizeInt2 (CClipResize * pThis,
        double fA, double fX,
        double fE, double fY);


    void CClipResize_ResizeP411Int2 (CClipResize * pThis,
        double fA, double fX,
        double fE, double fY);

    void CClipResize_ResizePackedYUYInt2 (CClipResize * pThis,
        double fA, double fX,
        double fE, double fY,
        Bool_WMV bYUY2);

    void CClipResize_ResizePacked32Int2 (CClipResize * pThis,
        double fA, double fX,
        double fE, double fY);

    void CClipResize_PanZoomC (CClipResize * pThis,
        double fZoomX, double fZoomY, double fX, double fY);



#ifndef _WIN32_WCE
#pragma AVRT_VTABLES_END
#endif //_WIN32_WCE
#endif
//#pragma pack ()

