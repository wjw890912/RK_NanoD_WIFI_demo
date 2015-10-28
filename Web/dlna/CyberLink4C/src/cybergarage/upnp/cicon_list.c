/******************************************************************
*
*    CyberLink for C
*
*    Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*    File: cicon_list.c
*
*    Revision:
*
*    02/23/05
*        - first revision
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <cybergarage/upnp/cicon.h>
#include <cybergarage/util/clog.h>
#include <rk_heap.h>

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"

/****************************************
* cg_upnp_iconr_new
****************************************/

CgUpnpIconList *cg_upnp_iconlist_new()
{
    CgUpnpIcon *iconList;

    cg_log_debug_l4("Entering...\n");

    iconList = (CgUpnpIcon *)malloc(sizeof(CgUpnpIcon));

    if ( NULL != iconList )
        cg_list_header_init((CgList *)iconList);

    return iconList;

    cg_log_debug_l4("Leaving...\n");
}

/****************************************
* cg_upnp_iconr_delete
****************************************/

void cg_upnp_iconlist_delete(CgUpnpIconList *iconList)
{
    cg_log_debug_l4("Entering...\n");

    cg_list_remove((CgList *)iconList);
    free(iconList);

    cg_log_debug_l4("Leaving...\n");
}

//#pragma arm section code
#endif
