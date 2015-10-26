//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (c) 1998  Microsoft Corporation

Module Name:

    dectable_mp4x.h

Abstract:

    MMIDRATE4S - MMIDRATE Decoder Huffmane tables

Author:

    Bruce Lin (blin@microsoft.com) June-1998

Revision History:

*************************************************************************/
#include "xplatform_wmv.h"
#include "wmc_typedef.h"
extern U8_WMV gDecodeCodeTableCBPY[128];
extern U8_WMV gDecodeCodeTableMCBPCintra_New [16];
extern U8_WMV gDecodeCodeTableMCBPCinter_New [256];

extern U8_WMV gDecodeCodeTableMCBPCintra[1024];
extern U8_WMV gDecodeCodeTableMCBPCinter[1024];

extern U8_WMV gDecodeCodeTableIntraDCy[256];
extern U8_WMV gDecodeCodeTableIntraDCc[512];

extern U8_WMV gDecodeCodeTableMV[16384];

extern U8_WMV gDecodeCodeTableIntraDCMMIDRATEy[4096];
extern U8_WMV gDecodeCodeTableIntraDCMMIDRATEc[8192];
