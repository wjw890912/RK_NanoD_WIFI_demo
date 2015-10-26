#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
 #if !defined(FREE_RTOS)
#define LOG_TAG "MediaServer"
#include <utils/Log.h>
#include <sys/stat.h>
#include <dirent.h>
#include <MediaRecord.h>
#include "MedialibClientProxy.h"
#include <utils/encoding_trans.h>
#else
   #define MEDIARECORD_ARTIST "artist"
   #define MEDIARECORD_TITLE "title"
   #define MEDIARECORD_ALBUM "album"
//#include <utlis/rewrite_api.h>
#include <utils/dlna_log.h>


#endif

#include <server/dms-playlist.h>
#include <cybergarage/upnp/caction.h>
#include <cybergarage/upnp/std/av/cmediaserver.h>
#include <server/dms_filesys.h>
#include <server/dms-action.h>
#include <server/dms_media_type.h>
#include "dlna_config.h"

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#ifdef HAVE_MEDIA_SERVER


#define DEFAULT_SIZE (1000)
#define UTF8_LEN (100)
#define NULL_STR "<unknown>"

/****************************************
* dms_filesys_updatecontentlist
****************************************/

static const char *root_folder[] = {
                    "All Tracks",
                    "Album",
                    "Artist"
                    };
static const char *root_folder_path[] = {
                    "Rockchip/All Tracks",
                    "Rockchip/Album",
                    "Rockchip/Artist"
                    };
static char NULL_STRING[1] = {0};

extern CgUpnpMediaFileSystemContentData *cg_upnpav_dms_filesys_content_getdata(CgUpnpMediaContent *con);
extern void cg_upnpav_dms_filesys_content_data_delete(CgUpnpMediaFileSystemContentData *data);
extern char *cg_upnpav_dms_filesys_getmimetype(char *ext);
extern char *cg_upnpav_dms_filesys_getpublicationdirectory(CgUpnpMediaServer *dms);

static PLAYLIST *playlist_path= NULL;
static PLAYLIST *playlist_album = NULL;
static PLAYLIST *playlist_title = NULL;
static PLAYLIST *playlist_artist= NULL;

BOOL dms_conmgr_actionreceived(CgUpnpAction *action)
{
    CgUpnpAvServer *dms;
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

    dms = (CgUpnpAvServer *)cg_upnp_device_getuserdata(dev);
    if (!dms)
        return FALSE;

    /* GetProtocolInfo*/
    if (cg_streq(actionName, CG_UPNPAV_DMS_CONNECTIONMANAGER_GET_PROTOCOL_INFO)) {
        arg = cg_upnp_action_getargumentbyname(action, CG_UPNPAV_DMS_CONNECTIONMANAGER_SINK);
        if (!arg)
            return FALSE;
        protocolInfos = cg_string_new();
        for (protocolInfo = cg_upnpav_dms_getprotocolinfos(dms); protocolInfo; protocolInfo = cg_upnpav_protocolinfo_next(protocolInfo)) {
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

static char *dms_filesys_getupnpclass(char *ext)
{
    if (cg_strcaseeq(ext, "jpeg") || cg_strcaseeq(ext, "jpg"))
        return CG_UPNPAV_UPNPCLASS_PHOTO;
    if (cg_strcaseeq(ext, "mpeg") || cg_strcaseeq(ext, "mpg"))
        return CG_UPNPAV_UPNPCLASS_MOVIE;
    if (cg_strcaseeq(ext, "mp3"))
        return CG_UPNPAV_UPNPCLASS_MUSIC;

    return NULL;
}

#if  defined(HAVE_FILE_SYS)
static void dms_initfilelist(CgUpnpMediaServer *dms, CgUpnpMediaContentList *parentCon,
                                       char *pubdir, const char **folder, int *id, int num, int contype)
{
    CgUpnpMediaContent *con;
    CgUpnpMediaResource *res;
    CgNetworkInterfaceList *netlflist;
    CgNetworkInterfaceList *netif;
    char *filename = NULL;
    CgString *fullpathstr;
    char *upnpclass = NULL;
    CgUpnpMediaFileSystemContentData *condata;
    char dlnaAttr[CG_UPNPAV_DLNAATTR_MAXLEN];
    char idmd5[CG_MD5_STRING_BUF_SIZE];
    int n;
    int fileExtIdx;
    char *fileExt;
    char *mimeType;
    char resURL[1024];
    char *path = NULL;
    char utf8_name[UTF8_LEN] = {0};

    int test = 0;
    int ret = 0;

#if !defined(WINCE)
    struct stat fileStat;
    off_t fileSize;
#else
    fpos_t fileSize;
    FILE *fp;
#endif

    ALOGD("----dms-test--init filesys in");

    for (con = cg_upnpav_content_getchildcontents(parentCon); con; con=cg_upnpav_content_next(con)) {
        condata =cg_upnpav_dms_filesys_content_getdata(con);
        if (!condata)
            continue;
        cg_upnpav_dms_filesys_content_data_delete(condata);
    }
    cg_upnpav_content_clearchildcontents(parentCon);//clear all

    if (cg_strlen(pubdir) <= 0)
        return;

    netlflist = cg_net_interfacelist_new();
    cg_net_gethostinterfaces(netlflist);
    netif = cg_net_interfacelist_gets(netlflist);
    if (!netif) {
        cg_net_interfacelist_delete(netlflist);
        return;
    }

    //contype = CG_UPNPAV_CONTENT_ITEM;
    if (contype != CG_UPNPAV_CONTENT_ITEM && contype != CG_UPNPAV_CONTENT_CONTAINER)
        return;

    if (contype == CG_UPNPAV_CONTENT_ITEM && id == NULL)
        return;

    for (n = 0; n < num; n++) {
        filename = folder[n];
        if (*filename == 0 && contype == CG_UPNPAV_CONTENT_CONTAINER)
            filename = NULL_STR;

        fullpathstr = cg_string_new();
        cg_string_addvalue(fullpathstr, pubdir);
        cg_string_addvalue(fullpathstr, "/");
        cg_string_addvalue(fullpathstr, filename);

    //    ALOGD("---dms_test--th fullpath = %s filename = %s", cg_string_getvalue(fullpathstr), filename);
        if ((contype == CG_UPNPAV_CONTENT_ITEM)) {
            if (*filename == 0 || id[n] <= 0) {
                ALOGD("fail");
                cg_string_delete(fullpathstr);
                continue;
            }
            fileExtIdx = cg_strrchr(filename, ".", 1);
            if (0 < fileExtIdx) {
                fileExt = (filename + fileExtIdx + 1);
                filename[fileExtIdx] = '\0';
                mimeType = cg_upnpav_dms_filesys_getmimetype(fileExt);
                upnpclass = dms_filesys_getupnpclass(fileExt);
                //ALOGD("---dms_test--th finaname = %s, mimetype =%s class = %s", filename, mimeType, upnpclass);
            }
            if (!mimeType || !upnpclass) {//default
                mimeType = CG_UPNPAV_MIMETYPE_MP3;
                upnpclass = CG_UPNPAV_UPNPCLASS_MUSIC;
            }
        }

        con = cg_upnpav_content_new();
        /* id */
        if (contype == CG_UPNPAV_CONTENT_ITEM)
            sprintf(idmd5,"%d",id[n]);
        else
            cg_str2md5((char *)cg_string_getvalue(fullpathstr), idmd5);
//#if defined(HAVE_DMS_ICONV)
        if (id != NULL) {
                /* title */

               if (0 == get_codeding(filename, utf8_name,UTF8_LEN, id[n]))
                   cg_upnpav_content_settitle(con, filename);
            else
                cg_upnpav_content_settitle(con, utf8_name);
        }
        else
            cg_upnpav_content_settitle(con, filename);


        cg_upnpav_content_setid(con, idmd5);

        /* parent id */
        cg_upnpav_content_setparentid(con, cg_upnpav_content_getid(parentCon));

        /* type */
        cg_upnpav_content_settype(con, contype);

        /* upnp:class */
        cg_upnpav_content_setupnpclass(con, upnpclass);


        if ((contype == CG_UPNPAV_CONTENT_ITEM)) {
            res = cg_upnpav_resource_new();
            snprintf(resURL, sizeof(resURL),
                        "http://%s:%d/%s/%s",
                    cg_net_interface_getaddress(netif),
                    cg_upnp_device_gethttpport(cg_upnpav_dms_getdevice(dms)),
                    DMS_RESURL_PATH,
                    idmd5
                    );
            cg_upnpav_resource_seturl(res, resURL);
            cg_upnpav_resource_setmimetype(res, mimeType);
            cg_upnpav_resource_setdlnaattribute(res, cg_upnpav_resource_getdlnaattributesbymimetype(res, dlnaAttr, sizeof(dlnaAttr)));
            //                    cg_upnpav_resource_setdlnaattribute(res, cg_upnpav_resource_getdlnapnfrommimetype(res, dlnaAttr, sizeof(dlnaAttr)));
            cg_upnpav_content_addresource(con, res);

            /* size */

            path = playlist_get_path(id[n]);
#if !defined(WINCE)
            fileSize = 0;
            if (stat(path, &fileStat) == 0)
                fileSize = fileStat.st_size;
#else
            fp = fopen(path, "rb");
            if (fp) {
                fseek(fp, 0, SEEK_END);
                fgetpos(fp, &fileSize);
                fclose(fp);
            }
#endif
            cg_upnpav_resource_setsize(res, (long)fileSize);
        }

        cg_upnpav_dms_filesys_content_setpubicdirectory(con, cg_string_getvalue(fullpathstr));

        cg_upnpav_content_addchildcontent(parentCon, con);
        cg_upnpav_dms_condir_updatesystemupdateid(dms);
        cg_string_delete(fullpathstr);

    }

    cg_net_interfacelist_delete(netlflist);

    ALOGD("----dms-test--init filesys out");

    return;
}
#else
static void dms_initfilelist(CgUpnpMediaServer *dms, CgUpnpMediaContentList *parentCon,
                                       char *pubdir, const char **folder, int *id, int num, int contype)
{
    return;
}
#endif
void dms_filesys_updaterootcontentlist(CgUpnpMediaServer *dms)
{
    char *pubdir;
    CgUpnpMediaContent *rootContent;

    pubdir = cg_upnpav_dms_filesys_getpublicationdirectory(dms);
    if (!pubdir)
        return;

    rootContent = cg_upnpav_dms_getrootcontent(dms);

    if (!rootContent)
        return;

    cg_upnpav_dms_lock(dms);

    dms_initfilelist(dms, rootContent, pubdir, root_folder, NULL, (sizeof(root_folder)/sizeof(root_folder[0])), CG_UPNPAV_CONTENT_CONTAINER);

    cg_upnpav_dms_unlock(dms);

    return;
}

static BOOL dms_all_tracks(CgUpnpMediaServer *dms, CgUpnpMediaContentList *parentCon, char *pubdir)
{
    BOOL ret = TRUE;
    char **name = NULL;
    int count = 0;
    int *id;

    playlist_create (&playlist_title, MEDIARECORD_TITLE);

    ret = musiclist_create(playlist_title, MEDIARECORD_TITLE);
    if (FALSE == ret)
        return FALSE;

    count = musiclist_load_msg(&name, &id, playlist_title->musiclist);
    if (count <= 0) {
        musiclist_delet(playlist_title);
        return FALSE;
    }

    dms_initfilelist(dms, parentCon, pubdir, name, id, count, CG_UPNPAV_CONTENT_ITEM);

    musiclist_delet(playlist_title);
    free(name);
    free(id);

    return ret;
}

static BOOL dms_artist(CgUpnpMediaServer *dms, CgUpnpMediaContentList *parentCon, char *pubdir)
{
    BOOL ret = TRUE;
    char **name = NULL ;
    int *id;
    char *dir_filename = NULL;
    PLAYLIST *playlist = NULL;
    int count = 0;

    if (0 == strcmp(pubdir, root_folder_path[2])) {
        playlist_create (&playlist_artist, MEDIARECORD_ARTIST);
        count = musiclist_load_msg(&name, &id, (MUSICLIST *)playlist_artist);
        if (count <= 0)
            return FALSE;

        dms_initfilelist(dms, parentCon, pubdir, name, id, count, CG_UPNPAV_CONTENT_CONTAINER);
    } else {
        dir_filename = (char *)basename(pubdir);
        if (0 == strcmp(dir_filename, NULL_STR))
            dir_filename = NULL_STRING;

        playlist = playlist_load_playlist_forname(dir_filename, playlist_artist);
        if (NULL == playlist)
            return FALSE;

        ret = musiclist_create(playlist, MEDIARECORD_ARTIST);
        if (FALSE == ret)
            return FALSE;

        count = musiclist_load_msg(&name, &id, playlist->musiclist);
        if (count <= 0) {
            musiclist_delet(playlist);
            return FALSE;
        }


        dms_initfilelist(dms, parentCon, pubdir, name, id, count, CG_UPNPAV_CONTENT_ITEM);

        musiclist_delet(playlist);
        free(name);
        free(id);
    }

    return ret;
}

static BOOL dms_album(CgUpnpMediaServer *dms, CgUpnpMediaContentList *parentCon, char *pubdir)
{
    BOOL ret = TRUE;
    char **name = NULL ;
    int *id;
    int count = 0;
    int i = 0;
    char *dir_filename = NULL;
    PLAYLIST *playlist = NULL;
    int t;

    if (0 == strcmp(pubdir, root_folder_path[1])) {
        playlist_create (&playlist_album, MEDIARECORD_ALBUM);
        count = musiclist_load_msg(&name, &id, (MUSICLIST *)playlist_album);
        if (count <= 0)
            return FALSE;

        dms_initfilelist(dms, parentCon, pubdir, name, id, count, CG_UPNPAV_CONTENT_CONTAINER);
    } else {
        dir_filename = (char *)basename(pubdir);
        if (0 == strcmp(dir_filename, NULL_STR))
            dir_filename = NULL_STRING;

        playlist = playlist_load_playlist_forname(dir_filename, playlist_album);
        if (NULL == playlist)
            return FALSE;

        ret = musiclist_create(playlist, MEDIARECORD_ALBUM);
        if (FALSE == ret)
            return FALSE;

        count = musiclist_load_msg(&name, &id, playlist->musiclist);
        if (count <= 0) {
            musiclist_delet(playlist);
            return FALSE;
        }

        dms_initfilelist(dms, parentCon, pubdir, name, id, count, CG_UPNPAV_CONTENT_ITEM);

        musiclist_delet(playlist);
        free(name);
        free(id);
    }

    return ret;
}

static BOOL dms_filesys_actionlistner(CgUpnpAction *action)
{
    CgUpnpMediaServer *dms;
    CgUpnpDevice *dev;
    char *contentID;
    CgUpnpMediaContent *content;
    char *contentDir;
    char *actionName;

    actionName = (char *)cg_upnp_action_getname(action);
    if (cg_strlen(actionName) <= 0)
        return FALSE;

    /* Browse */

    if (cg_streq(actionName, CG_UPNPAV_DMS_CONTENTDIRECTORY_BROWSE)) {
        contentID = cg_upnp_action_getargumentvaluebyname(action, CG_UPNPAV_DMS_CONTENTDIRECTORY_BROWSE_OBJECT_ID);
        if (cg_strlen(contentID) <=0)
            return FALSE;
        dev = cg_upnp_service_getdevice(cg_upnp_action_getservice(action));
        if (!dev)
            return FALSE;
        dms = (CgUpnpMediaServer *)cg_upnp_device_getuserdata(dev);
        if (!dms)
            return FALSE;
        cg_upnpav_dms_lock(dms);

        content = cg_upnpav_dms_findcontentbyid(dms, contentID);
        if (!content) {
            cg_upnpav_dms_unlock(dms);
            return FALSE;
        }

        contentDir = (char *)cg_upnpav_dms_filesys_content_getpubicdirectory(content);
        if (cg_strlen(contentDir) <= 0) {
            cg_upnpav_dms_unlock(dms);
            return FALSE;
        }

        if (FALSE == playlist_create_condition()) {
            cg_upnpav_dms_unlock(dms);
            return FALSE;
        }

        if (0 == strcmp(contentDir, ROOT_PUBDIRECT))
            dms_initfilelist(dms, content, contentDir, root_folder, NULL,
                                               (sizeof(root_folder)/sizeof(root_folder[0])), CG_UPNPAV_CONTENT_CONTAINER);
        else if (0 != strstr(contentDir, root_folder_path[0]))
            dms_all_tracks(dms, content, contentDir);
        else if (0 != strstr(contentDir, ROOT_PUBDIRECT) && 0 != strstr(contentDir, root_folder[1]))
            dms_album(dms, content, contentDir);
        else if (0 != strstr(contentDir, ROOT_PUBDIRECT) && 0 != strstr(contentDir, root_folder[2]))
            dms_artist(dms, content, contentDir);


        cg_upnpav_dms_unlock(dms);
    }

    return FALSE;
}

BOOL dms_actionreceived(CgUpnpAction *action)
{
    CgUpnpDevice *dev;
    CgUpnpService *service;
    CgUpnpAvServer *dms;
    CG_UPNP_ACTION_LISTNER userActionListener;
    const char *actionName = NULL;

    service = cg_upnp_action_getservice(action);
    if (!service)
        return FALSE;

    dev = (CgUpnpDevice *)cg_upnp_service_getdevice(service);
    if (!dev)
        return FALSE;

    dms = (CgUpnpAvServer *)cg_upnp_device_getuserdata(dev);
    if (!dms)
        return FALSE;

    actionName = cg_upnp_action_getname(action);
    if (cg_strlen(actionName) <= 0)
        return FALSE;

    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMS_CONTENTDIRECTORY_SERVICE_TYPE))
        return dms_filesys_actionlistner(action);
    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMS_MEDIARECEIVER_SERVICE_TYPE))
        ALOGD("--------dms_test--MEDIARECEIVE");
    if (cg_streq(cg_upnp_service_getservicetype(service), CG_UPNPAV_DMS_CONNECTIONMANAGER_SERVICE_TYPE))
        return dms_conmgr_actionreceived(action);

    return FALSE;
}
#endif

//#pragma arm section code
#endif

