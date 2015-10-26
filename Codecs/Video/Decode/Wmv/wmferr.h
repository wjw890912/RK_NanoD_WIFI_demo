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
//  File:       wmaerr.h
//
//--------------------------------------------------------------------------

#ifndef WMAERR_H
#define WMAERR_H

#include "wmf_types.h"

typedef I32_WMF WMAERR;

#define WMAERR_OK               0
#define WMAERR_FAIL             1
#define WMAERR_INVALIDARG       2
#define WMAERR_BUFFERTOOSMALL   3
#define WMAERR_INVALIDHEADER    4
#define WMAERR_OUTOFMEMORY      5
#define WMAERR_CORRUPTDATA      6
#define WMAERR_MOREDATA         7
#define WMAERR_DRMFAIL          8
#define WMAERR_DRMNOTSUPPORTED  9
#define WMAERR_NOTSUPPORTED		10
#endif // WMAERR_H
