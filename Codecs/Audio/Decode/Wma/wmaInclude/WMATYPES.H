//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 1999

Module Name:

    wmatypes.h

Abstract:

    Processor-specific data types.

Author:

    Kazuhito Koishida (kazukoi)       Jan 31, 2003

Revision History:

*************************************************************************/

#ifndef __WMATYPES_H
#define __WMATYPES_H
// ***************************************************
//            default fundamental typedefs
// ***************************************************

typedef unsigned __int64 U64;
typedef __int64 I64;
typedef unsigned long int U32;
typedef long int I32;

typedef unsigned short U16;
typedef short I16;

typedef unsigned char U8;
typedef signed char I8;

typedef float F32;
typedef double F64;

typedef double  Double;
typedef float   Float;
typedef void    Void;
typedef U32     UInt;
typedef I32     Int;
typedef I32     Bool; // In Win32, BOOL is an "int" == 4 bytes. Keep it this way to reduce problems.
typedef char    Char;


#define True 1
#define False 0


// define basic types
typedef __int64         WMA_I64;
typedef unsigned long   WMA_U32;
typedef long            WMA_I32;
typedef unsigned short  WMA_U16;
typedef short           WMA_I16;
typedef unsigned char   WMA_U8;
typedef WMA_I32         WMA_Bool;

typedef int WMARESULT;
#define APPSEC_LEN 4
#define DATE_LEN 4
#define RIGHTS_LEN 4
#define SAFE_DRM_SIZE 1196

#endif  // __WMATYPES_H
