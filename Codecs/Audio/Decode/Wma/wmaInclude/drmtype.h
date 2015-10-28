/*
************************************************************************************************************************
*
*  Copyright (C),2006, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     drmpd.h
*  Author:         hwg, zsx, zym
*  Description:
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*                         06/8/21       1.0
*
************************************************************************************************************************
*/
#ifndef __DRMTYPE_H__
#define __DRMTYPE_H__

#include "..\wmaInclude\predefine.h"
#ifndef WMAAPI_NO_DRM

#ifdef ZSP400
typedef unsigned short    BYTE,    DRM_BYTE;
typedef unsigned short   UWORD16;
typedef          short   WORD16, LENGTH_TYPE, COUNT_TYPE, DRM_RESULT;
typedef unsigned long    UWORD32, DRM_DWORD, DRM_UINT;
typedef          long    WORD32,  DRM_INT;
#define SIXTEEN_BIT_ADDRESSING 1
#else
//// basic types
typedef unsigned char    BYTE,    DRM_BYTE;
typedef unsigned short   UWORD16;
typedef          short   WORD16, LENGTH_TYPE, COUNT_TYPE, DRM_RESULT;
typedef unsigned long    UWORD32, DRM_DWORD, DRM_UINT;
typedef          long    WORD32,  DRM_INT;

//typedef unsigned __int64 UWORD64;
//typedef          __int64 WORD64;
#endif
#endif

#endif
