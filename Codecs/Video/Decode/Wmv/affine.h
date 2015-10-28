//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
tWMVDecodeStatus Affine_Init (  HWMVDecoder* phWMVDecoder,
                    int iOldWidth,
                    int iOldHeight,
                    int iNewWidth,
                    int iNewHeight);
tWMVDecodeStatus Affine_PanC (HWMVDecoder* phWMVDecoder, double fX, double fY);

tWMVDecodeStatus Affine_PanCFading (HWMVDecoder* phWMVDecoder, double fX, double fY, double fFading);

tWMVDecodeStatus Affine_PanZoomC ( HWMVDecoder* phWMVDecoder,double fZoom, double fX, double fY);
tWMVDecodeStatus Affine_PanZoomCFading ( HWMVDecoder* phWMVDecoder,double fZoom, double fX, double fY, double fFading);

tWMVDecodeStatus Affine_PanStretchC ( HWMVDecoder* phWMVDecoder, double fZoomX, double fX, double fZoomY, double fY);
tWMVDecodeStatus Affine_PanStretchCFading ( HWMVDecoder* phWMVDecoder,double fZoomX, double fX, double fZoomY, double fY, double fFading);

//tWMVDecodeStatus Affine_ResizeInt2 ( HWMVDecoder* phWMVDecoder,double fA, double fC, double fE, double fF);
tWMVDecodeStatus Affine_TransformFloat2 ( HWMVDecoder* phWMVDecoder,double fA, double fB, double fC, double fD, double fE, double fF);

tWMVDecodeStatus Affine_Transform(HWMVDecoder* phWMVDecoder,
                      const unsigned char* pucSrcY,
                      const unsigned char* pucSrcU,
                      const unsigned char* pucSrcV,
                      unsigned char *pFrameY,
                      unsigned char *pFrameU,
                      unsigned char *pFrameV,
                      double fA, double fB, double fC,
                      double fD, double fE, double fF, double fFadding);
