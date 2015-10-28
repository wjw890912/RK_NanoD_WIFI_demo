/*
 *
 *  Rockchip device media renderer.
 *
 *    2014-03-13 by rockchip IOT team.
 *
 *
 */
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#include <rk_heap.h>

#if !defined(FREE_RTOS)
#define LOG_TAG "MediaRenderer"
#include <utils/Log.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#else
#include <utils/dlna_log.h>
#endif

#include <renderer/dmr.h>
#include <string.h>
#include <cybergarage/upnp/cdevice.h>

#include <utils/udn.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#define DLNA_PATH "/etc/dlna/"

#define ICON_PATH DLNA_PATH"icon/"
#define DESCRIP_PATH DLNA_PATH"descriptions/"

#define RENDERER_PATH DESCRIP_PATH "renderer/"


#define RENDERER_DESCRIPTION_FILE (RENDERER_PATH  "description.xml")
#define DMR_CONNECTIONMANAGER_FILE (RENDERER_PATH "ConnectionManager.xml" )
#define DMR_AVTRANSPORT_FILE (RENDERER_PATH  "AVTransport.xml" )
#define DMR_RENDERINGCTRL_FILE (RENDERER_PATH  "RenderingControll.xml" )

#define SERVER_PATH DESCRIP_PATH "server/"
#define SERVER_DESCRIPTION_FILE    (SERVER_PATH  "description.xml")
#define DMS_CONNECTIONMANAGER_FILE (SERVER_PATH  "ConnectionManager.xml" )
#define DMS_CONTENTDIRECTORY_FILE (SERVER_PATH  "ContentDirectory.xml" )
#define DMS_MEDIARECEIVER_FILE (SERVER_PATH  "MediaReceiver.xml" )

#define SERVER_DESCRIPTIONURL    "/rockchip/server/description.xml"
#define RENDERER_DESCRIPTIONURL    "/rockchip/renderer/description.xml"

#define UPNPAVDUMP_DMS_SERVER "urn:schemas-upnp-org:device:MediaServer:1"
#define UPNPAVDUMP_DMS_RENDERER "urn:schemas-upnp-org:device:MediaRenderer:1"


extern BOOL cg_upnpav_dms_actionreceived(CgUpnpAction *action);
extern BOOL cg_upnpav_dms_queryreceived(CgUpnpStateVariable *statVar);
extern BOOL cg_upnpav_dmr_actionreceived(CgUpnpAction *action);
extern BOOL cg_upnpav_dmr_queryreceived(CgUpnpStateVariable *statVar);


#if defined(HAVE_FILE_SYS)

unsigned long file_getlength(const char *filename)
{
    unsigned long filesize = -1;
    struct stat statbuff;
    if (stat(filename, &statbuff) < 0)
        return filesize;
    else
        filesize = statbuff.st_size;

    return filesize;
}

BOOL file_load(char * descriptions, char *filename, int filelen)
{
    int fd;
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        ALOGE("%s can not open\n ", filename);
        return FALSE;
    }

    read(fd, descriptions, filelen);

    close(fd);
    return TRUE;
}

BOOL service_controldescriptions_load(CgUpnpDevice *dev, char *filename, char *type)
{
    CgUpnpService *service;
    CgUpnpAction *action;
    char *descriptions;
    unsigned long filelen = 0;

    filelen = file_getlength(filename);

    descriptions = (char *)malloc(filelen + 1);
    bzero(descriptions,  filelen + 1);
    if (!file_load(descriptions, filename, filelen )) {
        ALOGD("can not get descriptions");
        return FALSE;
    }


    service = cg_upnp_device_getservicebytype(dev, type);
    if (!service) {
        ALOGD("can not get service \n");
        goto  Fail;
    }

    if (cg_upnp_service_parsedescription(service, descriptions, cg_strlen(descriptions)) == FALSE) {
        ALOGD("paradescription not be use\r\n \n");
        goto  Fail;
    }

    cg_upnp_service_setuserdata(service, dev->userData);//device  userData is dmr
    for (action = cg_upnp_service_getactions(service); action; action = cg_upnp_action_next(action)) {
        cg_upnp_action_setuserdata(action, dev->userData);
    }

    free(descriptions);
    return TRUE;

Fail:

    free(descriptions);
    return FALSE;
}

/**
 * Load local devicedescriptions
 * the descriptions  save in the /etc/dlna/descriptions/
 *
 * @param deviceType A device type  (usually the result from
 *      \ref cg_upnp_device_getdevicetype)
 *
 * @return TRUE if successful; otherwise FALSE
 */
BOOL device_descriptions_load(CgUpnpDevice *dev, char *filename)
{
    char *descriptions;
    unsigned long filelen = 0;

    filelen = file_getlength(filename);

    descriptions = (char *)malloc(filelen + 1);
    bzero(descriptions,  filelen + 1);
    if (!file_load(descriptions, filename, filelen)) {
        ALOGE("can not get descriptions");
        return FALSE;
    }


    if (!cg_upnp_device_parsedescription(dev, descriptions, cg_strlen(descriptions))) {
        ALOGE("can load descriptionfile \n");
        free(descriptions);
        return FALSE;
    }

    free(descriptions);
    return TRUE;
}


/**
 * Load local service descriptions
 * the descriptions  save in the /etc/dlna/descriptions/
 *
 * @param deviceType A device type  (usually the result from
 *      \ref cg_upnp_device_getdevicetype)
 *
 * @return TRUE if successful; otherwise FALSE
 */
BOOL renderer_descriptions_load(CgUpnpDevice *dev)
{
    if (!service_controldescriptions_load(dev, DMR_CONNECTIONMANAGER_FILE, CG_UPNPAV_DMR_CONNECTIONMANAGER_SERVICE_TYPE))
        return FALSE;

    if (!service_controldescriptions_load(dev, DMR_RENDERINGCTRL_FILE, CG_UPNPAV_DMR_RENDERINGCONTROL_SERVICE_TYPE))
        return FALSE;

    if (!service_controldescriptions_load(dev, DMR_AVTRANSPORT_FILE, CG_UPNPAV_DMR_AVTRANSPORT_SERVICE_TYPE ))
        return FALSE;

    cg_upnp_device_setactionlistener(dev, cg_upnpav_dmr_actionreceived);
    cg_upnp_device_setquerylistener(dev, cg_upnpav_dmr_queryreceived);

    return TRUE;
}

/**
 * Load local service descriptions
 * the descriptions  save in the /etc/dlna/descriptions/
 *
 * @param deviceType A device type  (usually the result from
 *      \ref cg_upnp_device_getdevicetype)
 *
 * @return TRUE if successful; otherwise FALSE
 */
BOOL server_descriptions_load(CgUpnpDevice *dev)
{

    if (!service_controldescriptions_load(dev, DMS_MEDIARECEIVER_FILE, CG_UPNPAV_DMS_MEDIARECEIVER_SERVICE_TYPE))
        return FALSE;
    if (!service_controldescriptions_load(dev, DMS_CONTENTDIRECTORY_FILE, CG_UPNPAV_DMS_CONTENTDIRECTORY_SERVICE_TYPE ))
        return FALSE;
    if (!service_controldescriptions_load(dev, DMS_CONNECTIONMANAGER_FILE, CG_UPNPAV_DMS_CONNECTIONMANAGER_SERVICE_TYPE))
        return FALSE;
    cg_upnp_device_setactionlistener(dev, cg_upnpav_dms_actionreceived);
    cg_upnp_device_setquerylistener(dev, cg_upnpav_dms_queryreceived);

    return TRUE;
}

/**
 * Load local descriptionsurl
 * the descriptionsrul define int the "DEVICE_DESCRIPTIONURL"
 *
 * @param deviceType A device type  (usually the result from
 *      \ref cg_upnp_device_getdevicetype)
 *
 * @return TRUE
 */
BOOL device_descriptionsurl_load(CgUpnpDevice *dev,char *filename)
{
    cg_upnp_device_setdescriptionuri(dev, filename);
    return TRUE;
}

/**
 * Load local descriptions
 * the descriptions  save in the /etc/dlna/descriptions/
 *
 * @param deviceType A device type  (usually the result from
 *      \ref cg_upnp_device_getdevicetype)
 *
 * @return TRUE if successful; otherwise FALSE
 */
BOOL local_descriptions_load(CgUpnpDevice *dev)
{
    if (cg_upnp_device_isdevicetype(dev, UPNPAVDUMP_DMS_RENDERER)) {
        if (!device_descriptions_load(dev, RENDERER_DESCRIPTION_FILE  ))
            return FALSE;

        if (!device_descriptionsurl_load(dev, RENDERER_DESCRIPTIONURL))
            return FALSE;

        if (!renderer_descriptions_load(dev))
            return FALSE;
    }
    else if (cg_upnp_device_isdevicetype(dev, UPNPAVDUMP_DMS_SERVER)) {
        if (!device_descriptions_load(dev, SERVER_DESCRIPTION_FILE ))
            return FALSE;
        if (!device_descriptionsurl_load(dev, SERVER_DESCRIPTIONURL))
            return FALSE;
        if (!server_descriptions_load(dev))
            return FALSE;
    }
    else
        return FALSE;

    return TRUE;
}
#else
BOOL device_descriptionsurl_load(CgUpnpDevice *dev,char *filename)
{
    cg_upnp_device_setdescriptionuri(dev, filename);
    return TRUE;
}

BOOL local_descriptions_load(CgUpnpDevice *dev)
{
    if (cg_upnp_device_isdevicetype(dev, UPNPAVDUMP_DMS_RENDERER)) {
        if (!device_descriptionsurl_load(dev, RENDERER_DESCRIPTIONURL))
            return FALSE;
    }
    else if (cg_upnp_device_isdevicetype(dev, UPNPAVDUMP_DMS_SERVER)) {
        if (!device_descriptionsurl_load(dev, SERVER_DESCRIPTIONURL))
            return FALSE;
    }
    return TRUE;
}
unsigned long file_getlength(const char *filename)
{

    return 0;
}

BOOL file_load(char * descriptions, char *filename, int filelen)
{
    return FALSE;
}

#endif
//#pragma arm section code
#endif

