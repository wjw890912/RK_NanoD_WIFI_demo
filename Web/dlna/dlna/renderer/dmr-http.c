//This is http
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#include <rk_heap.h>

 #if !defined(FREE_RTOS)
#define LOG_TAG "MediaRenderer"
#include <utils/Log.h>
#else
#include <utils/dlna_log.h>
#endif


#include <utils/descriptions_load.h>
#include <cybergarage/upnp/cdevice.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


 static CgUpnpIcon *dmr_upnp_device_geticonbyscpdurl(CgUpnpDevice *dev, char *url)
{
    CgUpnpIcon  *icon;
    CgUpnpDevice *childDev;

    if (cg_strlen(url) <= 0)
        return NULL;

    for (icon = cg_upnp_device_geticons(dev); icon != NULL; icon = cg_upnp_icon_next(icon)) {
         if (cg_streq(url, cg_upnp_icon_geturl(icon))) {
            return icon;
         }
    }

    for (childDev = cg_upnp_device_getdevices(dev); childDev != NULL; childDev = cg_upnp_device_next(childDev)) {
        icon = dmr_upnp_device_geticonbyscpdurl(childDev, url);
        if (icon != NULL)
            return icon;
    }

    return NULL;
}

 static BOOL dmr_add_icon(CgHttpRequest *httpReq, CgUpnpIcon *icon, char *url)
{
    char *description;
    int filelen = 0;
    CgHttpResponse *httpRes;

    filelen = file_getlength(url);
    if (filelen < 0) {
        ALOGE("get filelen fail");
    }

    description = (char *)malloc(filelen + 1);
//    bzero(description, filelen + 1);
    if (!file_load(description, url, filelen)) {
        ALOGE("get file fail");
        free(description);
        return FALSE;
    }

    httpRes = cg_http_response_new();

    cg_http_response_setcontenttype(httpRes, cg_upnp_icon_getmimetype(icon));
    cg_http_response_setversion(httpRes, CG_HTTP_VER11);
    cg_http_response_setstatuscode(httpRes, CG_HTTP_STATUS_OK);
    cg_http_response_setcontentlength(httpRes, filelen);
     cg_http_packet_setncontent(httpRes, description, filelen);

    cg_http_response_print(httpRes);
    cg_http_request_postresponse(httpReq, httpRes);//get

    cg_http_response_delete(httpRes);
    free(description);

    return TRUE;

}

static BOOL dmr_upnp_device_getrequestrecieved(CgUpnpDevice *dev, CgHttpRequest *httpReq)
{
    char *url;
    CgUpnpIcon  *embIcon;

    url = cg_http_request_geturi(httpReq);
    if (cg_strlen(url) <= 0) {
        return FALSE;
    }
     if ((embIcon = dmr_upnp_device_geticonbyscpdurl(dev, url)) != NULL)
        if (dmr_add_icon(httpReq, embIcon, url) == TRUE)
            return TRUE;

    return FALSE;
}

static BOOL dmr_upnp_device_httprequestrecieved(CgHttpRequest *httpReq)
{
    CgUpnpDevice *dev;
    CgString *unescapedUrl;
    char *url;

    dev = (CgUpnpDevice *)cg_http_request_getuserdata(httpReq);//dev

    url = cg_http_request_geturi(httpReq);
    if (0 < cg_strlen(url)) {
        unescapedUrl = cg_string_new();
        cg_net_uri_unescapestring(url, 0, unescapedUrl);
        if (0 < cg_string_length(unescapedUrl))
            cg_http_request_seturi(httpReq, cg_string_getvalue(unescapedUrl));
        cg_string_delete(unescapedUrl);
    }

    if (cg_http_request_isgetrequest(httpReq) == TRUE ||
        cg_http_request_isheadrequest(httpReq) == TRUE) {
        if( dmr_upnp_device_getrequestrecieved(dev, httpReq) == TRUE)
            return TRUE;

    }

    return FALSE;
}

BOOL  dmr_http_request(CgHttpRequest *httpReq)
{
    if (dmr_upnp_device_httprequestrecieved(httpReq) == TRUE)
        return TRUE;
    return FALSE;
}
//#pragma arm section code

#endif

