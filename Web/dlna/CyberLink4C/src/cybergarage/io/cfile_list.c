/******************************************************************
*
*    CyberNet for C
*
*    Copyright (C) Nokia Corporation 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license with patent exclusion,
*       see file COPYING.
*
*    File: cfile_list.c
*
*    Revision:
*
*    01/18/07
*        - first revision
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <cybergarage/io/cfile.h>
#include <rk_heap.h>

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"

#if defined(CG_USE_CFILE)

#include <cybergarage/util/clog.h>

/****************************************
* cg_filelist_new
****************************************/

CgFileList *cg_filelist_new()
{
    CgFileList *fileList;

    cg_log_debug_l4("Entering...\n");

    fileList = (CgFileList *)malloc(sizeof(CgFileList));

    if ( NULL != fileList )
    {
        cg_list_header_init((CgList *)fileList);
        fileList->name = NULL;
        fileList->path = NULL;
        fileList->content = NULL;
    }

    cg_log_debug_l4("Leaving...\n");

    return fileList;
}

/****************************************
* cg_filelist_delete
****************************************/

void cg_filelist_delete(CgFileList *fileList)
{
    cg_log_debug_l4("Entering...\n");

    cg_filelist_clear(fileList);
    free(fileList);

    cg_log_debug_l4("Leaving...\n");
}

#endif

//#pragma arm section code
#endif
