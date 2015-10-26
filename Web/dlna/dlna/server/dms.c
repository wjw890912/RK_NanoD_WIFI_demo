/*
 *
 *  Rockchip device media server.
 *
 *    2014-01-14 by rockchip IOT team.
 *
 *
 */
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

// Log tag.
#include <rtos_config.h>
 #if !defined(FREE_RTOS)
#define LOG_TAG "MediaServer"
#include <utils/Log.h>
#else
#include <utils/dlna_log.h>
#endif

#include <server/dms.h>
#include <utils/udn.h>
#include <utils/descriptions_load.h>
#include <server/dms-http.h>
#include <cybergarage/upnp/caction.h>
#include <server/dms-action.h>
#include <server/dms_filesys.h>
#include "dlna_config.h"

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#ifdef HAVE_MEDIA_SERVER

void dms_restart(CgUpnpAvServer * dms)
{
    cg_upnpav_dms_lock(dms);

    cg_upnpav_dms_start(dms);

    cg_upnpav_dms_unlock(dms);
}

CgUpnpAvServer * dms_create(char * suffix)
{
    CgUpnpAvServer * dms = NULL;
    char udn[UDN_LENGTH] = { 0 };
    char name[128] = { 0 };
    CgUpnpAvProtocolInfo *protocolInfo = NULL;
    printf("--test--gonin dms create");
    dms = cg_upnpav_dms_new();
    if (!dms) {
        ALOGE("Can not create a new media server.\n");
        return NULL;
    }

    if(local_descriptions_load(dms->dev) == FALSE)
        ALOGE("Can not load local descriptions\n");

    // Set device udn.
    if (!create_udn(udn, UDN_LENGTH))
        cg_upnp_device_setudn(dms->dev, udn);

    // Set device friendly name.
    sprintf(name, "Rockchip Media Server[%s]", suffix);
    cg_upnp_device_setfriendlyname(dms->dev, name);


    cg_upnpav_dms_sethttplistener(dms, dms_http_request);
    cg_upnpav_dms_setactionlistener(dms, dms_actionreceived);

    // Set protocol info.sss
    protocolInfo = cg_upnpav_protocolinfo_new();
    if (protocolInfo) {
        cg_upnpav_protocolinfo_setprotocol(protocolInfo, "http-get");
        cg_upnpav_protocolinfo_setmimetype(protocolInfo, "audio/mpeg;audio/ape");
        cg_upnpav_dms_addprotocolinfo(dms, protocolInfo);
    }

    //set root pubdirect
    cg_upnpav_dms_filesys_setpublicationdirectory(dms, ROOT_PUBDIRECT);
    //dms_filesys_updaterootcontentlist(dms);

    // Start dms.
    cg_upnpav_dms_start(dms);

    ALOGD("Media server start, %s.\n", cg_upnp_device_getudn(dms->dev));

    return dms;
}

void dms_destroy(CgUpnpAvServer * dms)
{
    if (!dms)
        return;

    cg_upnpav_dms_stop(dms);
    cg_upnpav_dms_delete(dms);
    ALOGD("Media server stop.\n");

    return;
}

#endif

//#pragma arm section code

#endif
