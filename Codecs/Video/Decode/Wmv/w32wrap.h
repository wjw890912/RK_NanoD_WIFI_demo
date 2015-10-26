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
//  Copyright (C) Microsoft Corporation, 1999 - 1999
//
//  File:       w32wrap.h
//
//--------------------------------------------------------------------------

// w32wrap.h

#ifndef _W32_WRAP_H_
#define _W32_WRAP_H_

// for each non-WIN32 platforms...

#ifdef __ARMCC_VERSION
typedef char CHAR;
typedef short SHORT;
typedef long LONG;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
#endif  // __ARMCC_VERSION

// for non-platform specific stuff

#ifndef VOID
#define VOID void
#endif  // VOID

typedef DWORD HMODULE;

// HRESULTs
typedef LONG HRESULT;

#define NOERROR         0L
#define E_FAIL          0x80004005L
#define E_OUTOFMEMORY   0x8007000EL

// S_*

#define S_OK    0
#define S_FALSE 1

// some functions

#define ZeroMemory(p, s)    memset((p), 0, (s))

#endif  // _W32_WRAP_H_
