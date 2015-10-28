//This is http
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#include <rk_heap.h>

 #if !defined(FREE_RTOS)
#define LOG_TAG "MediaServer"
#include <utils/Log.h>
#include <sys/stat.h>
#else
#include <utils/dlna_log.h>
#endif

#include <utils/descriptions_load.h>
#include <cybergarage/upnp/cdevice.h>

#include <server/dms_media_type.h>
#include "dlna_config.h"

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#ifdef HAVE_MEDIA_SERVER

#define CG_FILE_READ_CHUNK_SIZE (32*1024)
#define CG_USE_CHUNKED_STREAM (1)
#define CG_UPNP_DMS_SOCKET_SEND_RETRY_CNT (10)
#define ID_BUF_LEN (20)
#define SOCK_ONLY (0)

extern const char *playlist_get_path(int id);

#if (SOCK_ONLY == 1)
typedef enum _SOCK_FLAG {
    SOCK_STOPED,
    SOCK_STARTING,
    SOCK_BEGING
}SOCK_FLAG;
SOCK_FLAG sock_flag = SOCK_STOPED;
#endif


void dms_http_request_print(CgHttpRequest *httpReq)
{
    CgHttpPacket *httpPkt;
    CgHttpHeader *header;
    char *content;
    long contentLen;
    ALOGD("method = %s uri = %s version = %s\n",
    cg_http_request_getmethod(httpReq),
    cg_http_request_geturi(httpReq),
    cg_http_request_getversion(httpReq));

    httpPkt = (CgHttpPacket *)httpReq;

    /**** print headers ****/
    for (header = cg_http_packet_getheaders(httpPkt); header != NULL; header = cg_http_header_next(header)) {
        ALOGD("name = %s: value = %s\n",
        cg_http_header_getname(header),
        cg_http_header_getvalue(header));
    }

    return;
}



BOOL dms_file_http_response(CgHttpRequest *httpReq)
{
#if defined(HAVE_FILE_SYS)

    CgUpnpMediaServer *dms;
    CgUpnpDevice *dev;
    char *httpURI;
    int contentMD5Idx;
    int id = 0;
    char id_buf [ID_BUF_LEN] = {0};
    char *contentMd5;
    CgUpnpMediaContent *content;
    CgUpnpMediaResource *resource;
    char *pubDir;
    char *path = NULL;
    CgHttpResponse *httpRes;
    CgSocket *sock;
    char chunkedChar[32];
    BOOL isHeadRequest;
    int http_version = 0;
#if !defined(WINCE)
    struct stat fileStat;
    off_t fileSize;
#else
    fpos_t fileSize;
#endif
    FILE *fp;
    char readBuf[CG_FILE_READ_CHUNK_SIZE];
    int nRead;
#if defined(WINCE) && defined(UNICODE)
    TCHAR wCharBuf[MAX_PATH];
#endif

    dev = (CgUpnpDevice *)cg_http_request_getuserdata(httpReq);
    if (!dev) {
        cg_http_request_postbadrequest(httpReq);
        return FALSE;
    }

    dms = (CgUpnpMediaServer *)cg_upnp_device_getuserdata(dev);
    if (!dms) {
        cg_http_request_postbadrequest(httpReq);
        return FALSE;
    }

    httpURI = cg_http_request_geturi(httpReq);
    if (cg_strlen(httpURI) <= 0) {
        cg_http_request_postbadrequest(httpReq);
        return FALSE;
    }

    contentMD5Idx = cg_strrchr(httpURI, "/", 1);

    if (contentMD5Idx < 0)
        return FALSE;

    contentMd5 = httpURI + contentMD5Idx + 1;
    strcpy(id_buf, contentMd5);
    id = atoi(id_buf);
    //ALOGD("----http request-- conten = %s id_buf = %s ,id = %d,", contentMd5, id_buf, id);
#if (SOCK_ONLY == 1)
    sock_flag = SOCK_STARTING;// enter
#endif
    cg_upnpav_dms_lock(dms);

    content = cg_upnpav_dms_findcontentbyid(dms, contentMd5);
    if (content == NULL) {
        cg_upnpav_dms_unlock(dms);
        return FALSE;
    }

    path = (char *)playlist_get_path(id);
    if (NULL == path) {
        cg_upnpav_dms_unlock(dms);
        return FALSE;
    }

    // unlock dms
    cg_upnpav_dms_unlock(dms);

    dms_string_addvalue(&pubDir, path);
    resource = cg_upnpav_content_getresources(content);
    if (!pubDir || !resource) {
        cg_upnpav_dms_unlock(dms);
        return FALSE;
    }

    isHeadRequest = cg_http_request_isheadrequest(httpReq);

    httpRes = cg_http_response_new();
    if (0 == strcmp(cg_http_request_getversion(httpReq), CG_HTTP_VER10))
        http_version = 1;

    if (0 == http_version)
        cg_http_response_setversion(httpRes, CG_HTTP_VER11);
    else
        cg_http_response_setversion(httpRes, CG_HTTP_VER10);

    cg_http_response_setstatuscode(httpRes, CG_HTTP_STATUS_OK);
    cg_http_response_setcontenttype(httpRes, cg_upnpav_resource_getmimetype(resource));

    sock = cg_http_request_getsocket(httpReq);

#if defined(WINCE)
    #if defined(UNICODE)
    MultiByteToWideChar(CP_UTF8, 0, pubDir, -1, wCharBuf, (MAX_PATH-1));
    #endif
#endif

    fileSize = 0;

#if !defined(WINCE)
    if (stat(pubDir, &fileStat) == 0)
        fileSize = fileStat.st_size;
#else
    #if defined(UNICODE)
    fp = _tfopen(wCharBuf, _T("rb"));
    #else
    fp = fopen(pubDir,"rb");
    #endif
    if (fp) {
        fseek(fp, 0, SEEK_END);
        fgetpos(fp, &fileSize);
        fclose(fp);
    }
#endif

    if (0 == http_version)
        cg_http_packet_setheadervalue((CgHttpPacket*)httpRes, "Transfer-Encoding", "chunked");
    else
        cg_http_response_setcontentlength(httpRes, fileSize);

    cg_http_request_postresponse(httpReq, httpRes);

    if (0 < fileSize) {
#if defined(WINCE) && defined(UNICODE)
        fp = _tfopen(wCharBuf, _T("rb"));
#else
        fp = fopen(pubDir, "rb");
#endif
        if (fp) {
            nRead = fread(readBuf, sizeof(char), CG_FILE_READ_CHUNK_SIZE, fp);
            while (0 < nRead) {
                if (0 == http_version) {
                    sprintf(chunkedChar, "%x%s", nRead, CG_HTTP_CRLF);
                    cg_socket_write(sock, chunkedChar, cg_strlen(chunkedChar));
                }

                cg_socket_write(sock, readBuf, nRead);
                if (0 == http_version)
                    cg_socket_write(sock, CG_HTTP_CRLF, sizeof(CG_HTTP_CRLF)-1);

                nRead = fread(readBuf, sizeof(char), CG_FILE_READ_CHUNK_SIZE, fp);
#if (SOCK_ONLY == 1)
                if (sock_flag == SOCK_BEGING)
                    break;
#endif
            }
            fclose(fp);
        }
    }

    if (0 == http_version) {
        sprintf(chunkedChar, "%x%s", 0, CG_HTTP_CRLF);
        cg_socket_write(sock, chunkedChar, cg_strlen(chunkedChar));
    }

    cg_socket_close(sock);
    cg_http_response_delete(httpRes);
    dms_free(pubDir);

#if (SOCK_ONLY == 1)
    sock_flag = SOCK_STOPED ;
#endif

    return TRUE;
#else
    return FALSE;
#endif
}

 static CgUpnpIcon *dms_upnp_device_geticonbyscpdurl(CgUpnpDevice *dev, char *url)
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
        icon = dms_upnp_device_geticonbyscpdurl(childDev, url);
        if (icon != NULL)
            return icon;
    }

    return NULL;
}

 static BOOL dms_add_icon(CgHttpRequest *httpReq, CgUpnpIcon *icon, char *url)
{
    char *description;
    int filelen = 0;
    CgHttpResponse *httpRes;

    filelen = file_getlength(url);
    if (filelen < 0) {
        ALOGE("get filelen fail");
        return FALSE;
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

static BOOL dms_upnp_device_getrequestrecieved(CgUpnpDevice *dev, CgHttpRequest *httpReq)
{
    char *url;
    CgUpnpIcon  *embIcon;

    url = cg_http_request_geturi(httpReq);
    if (cg_strlen(url) <= 0) {
        return FALSE;
    }

    if (0 != strstr(url, DMS_RESURL_PATH)) {

#if (SOCK_ONLY == 1)
        if (SOCK_STARTING == sock_flag) {
            sock_flag = SOCK_BEGING;
            while(sock_flag == SOCK_BEGING)
                usleep(100000);
        }
#endif

        if (TRUE == dms_file_http_response(httpReq))
            return TRUE;
    }

    if ((embIcon = dms_upnp_device_geticonbyscpdurl(dev, url)) != NULL)
        if (TRUE == dms_add_icon(httpReq, embIcon, url))
            return TRUE;

    return FALSE;
}

static BOOL dms_upnp_device_httprequestrecieved(CgHttpRequest *httpReq)
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
        if( dms_upnp_device_getrequestrecieved(dev, httpReq) == TRUE)
            return TRUE;
    }

    return FALSE;
}

BOOL  dms_http_request(CgHttpRequest *httpReq)
{
    if (dms_upnp_device_httprequestrecieved(httpReq) == TRUE)
        return TRUE;

    return FALSE;
}
#endif
//#pragma arm section code
#endif

