/************************************************************
*
*    CyberLink for C
*
*    Copyright (C) Satoshi Konno 2005
*
*    File: cdms_filesys.c
*
*    Revision:
*       05/11/05
*               - first release.
*
************************************************************/
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#include "server/dms_filesys.h"
#include <cybergarage/upnp/std/av/cmediaserver.h>
#include <cybergarage/net/cinterface.h>

#include <stdio.h>
#include <ctype.h>

#if defined(WIN32)
#include <windows.h>
#include <tchar.h>
#if !defined(WINCE)
#include <sys/stat.h>
#include <conio.h>
#endif

#elif defined(FREE_RTOS)

#else
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif
//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


/****************************************
* cg_upnpav_dms_filesys_getmimetype
****************************************/

char *cg_upnpav_dms_filesys_getmimetype(char *ext)
{
    if (cg_strcaseeq(ext, "jpeg") || cg_strcaseeq(ext, "jpg"))
        return CG_UPNPAV_MIMETYPE_JPEG;
    if (cg_strcaseeq(ext, "mpeg") || cg_strcaseeq(ext, "mpg"))
        return CG_UPNPAV_MIMETYPE_MPEG;
    if (cg_strcaseeq(ext, "mp3"))
        return CG_UPNPAV_MIMETYPE_MP3;
    return NULL;
}

/****************************************
* cg_upnpav_dms_filesys_getupnpclass
****************************************/

char *cg_upnpav_dms_filesys_getupnpclass(char *ext)
{
    if (cg_strcaseeq(ext, "jpeg") || cg_strcaseeq(ext, "jpg"))
        return CG_UPNPAV_UPNPCLASS_PHOTO;
    if (cg_strcaseeq(ext, "mpeg") || cg_strcaseeq(ext, "mpg"))
        return CG_UPNPAV_UPNPCLASS_MOVIE;
    if (cg_strcaseeq(ext, "mp3"))
        return CG_UPNPAV_UPNPCLASS_MUSIC;
    return NULL;
}

/****************************************
* cg_upnpav_dms_filesys_updatecontentlist
****************************************/
/****************************************
* cg_upnpav_dms_filesys_updaterootcontentlist
****************************************/

/****************************************
* cg_upnpav_dms_filesys_setpublicationdirectory
****************************************/

void cg_upnpav_dms_filesys_setpublicationdirectory(CgUpnpMediaServer *dms, char *pubdir)
{
    CgUpnpMediaContent *rootContent;

    rootContent = cg_upnpav_dms_getrootcontent(dms);
    if (!rootContent)
        return;

    cg_upnpav_dms_filesys_content_setpubicdirectory(rootContent, pubdir);
}

char *cg_upnpav_dms_filesys_getpublicationdirectory(CgUpnpMediaServer *dms)
{
    CgUpnpMediaContent *rootContent;

    rootContent = cg_upnpav_dms_getrootcontent(dms);
    if (!rootContent)
        return NULL;

    return cg_upnpav_dms_filesys_content_getpubicdirectory(rootContent);
}
/****************************************
* cg_upnpav_dms_filesys_actionlistner
****************************************/


/****************************************
* cg_upnpav_dms_filesys_new
****************************************/


/****************************************
* cg_upnpav_dms_filesys_delete
****************************************/
//#pragma arm section code
#endif

