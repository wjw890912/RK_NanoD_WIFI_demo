/*
 *
 *  Rockchip device media renderer action process.
 *
 *    2014-01-16 by rockchip IOT team.
 *
 *
 */
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#if !defined(FREE_RTOS)
#define LOG_TAG "MediaRenderer"
#include <utils/Log.h>
#include "../../lollipop_wifi/socket_ipc/lollipop_socket_ipc.h"
#else
#include <utils/dlna_log.h>
#endif

#include <renderer/dmr-action.h>
#include <player/player.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"

#define BUF_SIZE (512)

extern void stop_dlna_device(void);

static void reset_action()
{
#if !defined(FREE_RTOS)
    char msg[256] = {0};

    stop_dlna_device();

    sprintf(msg, "%s %s", IPC_RESTART_OR_RESET, "resetmode");

    lollipop_socket_client_send(SOCK_FILE_MAIN, msg);
#endif
}

static BOOL dmr_avtransport_actionreceived(CgUpnpAction * action)
{
    CgUpnpAvRenderer *dmr;
    CgUpnpDevice *dev;
    char *actionName;
    CgUpnpArgument *arg;
    char buf[BUF_SIZE] = { 0 };

    actionName = (char *)cg_upnp_action_getname(action);
    if (cg_strlen(actionName) <= 0)
        return FALSE;

    dev = (CgUpnpDevice *)cg_upnp_service_getdevice(cg_upnp_action_getservice(action));
    if (!dev)
        return FALSE;

    dmr = (CgUpnpAvRenderer *)cg_upnp_device_getuserdata(dev);
    if (!dmr)
        return FALSE;

    //ALOGD("Received a avtransport action: %s.", actionName);
    //return TRUE;
    /* GetTransportInfo*/
    if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_GETTRANSPORTINFO)) {
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATE);
        if (!arg)
            return FALSE;

        /* Set currnet transport state. */
        switch (player_get_state(cg_upnpav_dmr_getuserdata(dmr)))
        {
            case PLAYER_PLAYING:
                cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATE_PLAYING);
                break;

            case PLAYER_STOPPED:
                ALOGD("Received a avtransport action: %s.", actionName);
                cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATE_STOPPED);
                break;

            case PLAYER_PAUSED:
                cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATE_PAUSED_PLAYBACK);
                break;

            case PLAYER_TRANSITIONING:
                cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATE_TRANSITIONING);
                break;
        }

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATUS);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTTRANSPORTSTATUS_OK);


        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTSPEED);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, "1");

        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_GETPOSITIONINFO)) {
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_TRACK);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, "1");

        // Track duration.
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_TRACK_DURATION);
        if (!arg)
            return FALSE;

        memset(buf, BUF_SIZE, 0);
        if (!player_get_duration(cg_upnpav_dmr_getuserdata(dmr), buf, BUF_SIZE))
            cg_upnp_argument_setvalue(arg, buf);

        // Track meta.
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_TRACK_METADATA);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_NOT_IMPLEMENTED);

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_TRACK_URL);
        if (!arg)
            return FALSE;
        //cg_upnp_argument_setvalue(arg, "1");

        // Relative time.
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_REL_TIME);
        if (!arg)
            return FALSE;

        memset(buf, BUF_SIZE, 0);

        if (!player_current_time(cg_upnpav_dmr_getuserdata(dmr), buf, BUF_SIZE))
            cg_upnp_argument_setvalue(arg, buf);

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_ABS_TIME);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, CG_UPNPAV_DMR_AVTRANSPORT_NOT_IMPLEMENTED);

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_REL_COUNT);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, "65535");

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_ABS_COUNT);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, "65535");

        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_SETAVTRANSPORTURI)){
        ALOGI("AV transport action received, action name: %s!", actionName);
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTURI);
        if (!arg)
            return FALSE;

        ALOGI("Set url: %s!", cg_upnp_argument_getvalue(arg));

        player_set_url(cg_upnpav_dmr_getuserdata(dmr), cg_upnp_argument_getvalue(arg));
        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_PLAY)) {
        player_play(cg_upnpav_dmr_getuserdata(dmr));
        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_PAUSE)) {
        player_pause(cg_upnpav_dmr_getuserdata(dmr));
        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_STOP)) {
        printf("\napp................................................stop...................");
        player_stop(cg_upnpav_dmr_getuserdata(dmr));
        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_SEEK)) {
        CgUpnpArgument * unit, *target;

        unit = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_UNIT);
        ALOGI("Seek, Unit: %s\n", cg_upnp_argument_getvalue(unit));
        if (unit && cg_streq(cg_upnp_argument_getvalue(unit), CG_UPNPAV_DMR_AVTRANSPORT_RELTIME)) {
            target = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_TARGET);

            ALOGI("Seek, target: %s\n", cg_upnp_argument_getvalue(target));

            if (target) {
                unsigned int hour, minute, second;

                sscanf(cg_upnp_argument_getvalue(target), "%d:%d:%d", &hour, &minute, &second);

                ALOGI("Read time: hour - %d, minute - %d, second - %d, msecond - %d\n", hour, minute, second, (((hour * 60) + minute) * 60 + second) * 1000);

                player_seek(cg_upnpav_dmr_getuserdata(dmr), (((hour * 60) + minute) * 60 + second) * 1000);
                return TRUE;
            }
        }

        return FALSE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_RESET)) {
        ALOGD("Ready to reset.");
        reset_action();
        return TRUE;
    } else if (cg_streq(actionName, CG_UPNPAV_DMR_AVTRANSPORT_GETMEDIAINFO)) {
        // NrTracks
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_NRTRACKS);
        if (arg)
            cg_upnp_argument_setvalue(arg, "1");

        // MediaDuration
        arg = NULL;
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_MEDIA_DURATION);
        if (arg) {
            if (!player_get_duration(cg_upnpav_dmr_getuserdata(dmr), buf, BUF_SIZE))
                cg_upnp_argument_setvalue(arg, buf);
        }

        // CurrentURI
        arg = NULL;
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_AVTRANSPORT_CURRENTURI);
        if (arg) {
            cg_upnp_argument_setvalue(arg, player_get_url(cg_upnpav_dmr_getuserdata(dmr)));
        }

        // CurrentURIMetaData

        // NextURI

        // NextURIMetaData

        // PlayMedium

        // RecordMedium

        // WriteStatus

        return TRUE;
    } else {
        ALOGE("AV transport unknown action received, action name: %s!", actionName);
    }

    return FALSE;
}

static BOOL dmr_renderingctrl_actionreceived(CgUpnpAction * action)
{
    CgUpnpAvRenderer *dmr;
    CgUpnpDevice *dev;
    char *actionName;
    CgUpnpArgument *arg;

    actionName = (char *)cg_upnp_action_getname(action);
    if (cg_strlen(actionName) <= 0)
        return FALSE;

    dev = (CgUpnpDevice *)cg_upnp_service_getdevice(cg_upnp_action_getservice(action));
    if (!dev)
        return FALSE;

    dmr = (CgUpnpAvRenderer *)cg_upnp_device_getuserdata(dev);
    if (!dmr)
        return FALSE;

    /* GetMute*/
    if (cg_streq(actionName, CG_UPNPAV_DMR_RENDERINGCONTROL_GETMUTE)) {
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_RENDERINGCONTROL_CURRENTMUTE);
        if (!arg)
            return FALSE;
        cg_upnp_argument_setvalue(arg, "0");
        return TRUE;
    }

    /* GetVolume */
    if (cg_streq(actionName, CG_UPNPAV_DMR_RENDERINGCONTROL_GETVOLUME)) {
        char vol[128] = { 0 };

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_RENDERINGCONTROL_CURRENTVOLUME);
        if (!arg)
            return FALSE;

        sprintf(vol, "%d", player_get_volume(cg_upnpav_dmr_getuserdata(dmr)));
        ALOGD("Get volume: %s\n", vol);
        cg_upnp_argument_setvalue(arg, vol);
        return TRUE;
    }

    /* SetVolume. */
    if (cg_streq(actionName, CG_UPNPAV_DMR_RENDERINGCONTROL_SETVOLUME)) {
        int volume;

        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_RENDERINGCONTROL_DESIREDVOLUME);
        if (!arg)
            return FALSE;

        sscanf(cg_upnp_argument_getvalue(arg), "%d", &volume);

        ALOGD("Set volume: %d\n", volume);
        player_set_volume(cg_upnpav_dmr_getuserdata(dmr), volume);
        return TRUE;
    }

    /* SetMute. */
    if (cg_streq(actionName, CG_UPNPAV_DMR_RENDERINGCONTROL_SETMUTE)) {
        player_set_volume(cg_upnpav_dmr_getuserdata(dmr), 0);
        return TRUE;
    }

    return FALSE;
}

static BOOL dmr_conmgr_actionreceived(CgUpnpAction * action)
{
    CgUpnpAvRenderer *dmr;
    CgUpnpDevice *dev;
    char *actionName;
    CgUpnpArgument *arg;
    CgString *protocolInfos;
    CgUpnpAvProtocolInfo *protocolInfo;

    actionName = (char *)cg_upnp_action_getname(action);
    if (cg_strlen(actionName) <= 0)
        return FALSE;

    dev = (CgUpnpDevice *)cg_upnp_service_getdevice(cg_upnp_action_getservice(action));
    if (!dev)
        return FALSE;

    dmr = (CgUpnpAvRenderer *)cg_upnp_device_getuserdata(dev);
    if (!dmr)
        return FALSE;

    /* GetProtocolInfo*/
    if (cg_streq(actionName, CG_UPNPAV_DMR_CONNECTIONMANAGER_GET_PROTOCOL_INFO)) {
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMR_CONNECTIONMANAGER_SINK);
        if (!arg)
            return FALSE;
        protocolInfos = cg_string_new();
        for (protocolInfo = cg_upnpav_dmr_getprotocolinfos(dmr); protocolInfo; protocolInfo = cg_upnpav_protocolinfo_next(protocolInfo)) {
            if (0 < cg_string_length(protocolInfos))
                cg_string_addvalue(protocolInfos, ",");
            cg_string_addvalue(protocolInfos, cg_upnpav_protocolinfo_getstring(protocolInfo));
        }
        ALOGD("GetProtocolInfo: %s", cg_string_getvalue(protocolInfos));
        cg_upnp_argument_setvalue(arg, cg_string_getvalue(protocolInfos));
        cg_string_delete(protocolInfos);
        return TRUE;
    }

    return FALSE;
}

BOOL dmr_actionreceived(CgUpnpAction * action)
{
    BOOL res = FALSE;
    CgUpnpDevice *dev;
    CgUpnpService *service;
    CgUpnpAvRenderer *dmr;

    //ALOGE("Received an action, %s", cg_upnp_action_getname(action));

    service = cg_upnp_action_getservice(action);
    if (!service)
        return FALSE;

    dev = (CgUpnpDevice *)cg_upnp_service_getdevice(service);
    if (!dev)
        return FALSE;

    dmr = (CgUpnpAvRenderer *)cg_upnp_device_getuserdata(dev);
    if (!dmr)
        return FALSE;

    //printf("AV Action start, %s\r\n", cg_upnp_action_getname(action));
    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMR_AVTRANSPORT_SERVICE_TYPE)) {
        //ALOGE("AV Action start, %s", cg_upnp_action_getname(action));
        res = dmr_avtransport_actionreceived(action);
        //ALOGE("AV Action end.");
        return res;
    }

    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMR_RENDERINGCONTROL_SERVICE_TYPE)) {
        res = dmr_renderingctrl_actionreceived(action);
        return res;
    }

    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMR_CONNECTIONMANAGER_SERVICE_TYPE)) {
        res = dmr_conmgr_actionreceived(action);
        return res;
    }

    return FALSE;
}
//#pragma arm section code
#endif

