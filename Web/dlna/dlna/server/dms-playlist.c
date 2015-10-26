#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
#include <rk_heap.h>

 #if !defined(FREE_RTOS)
#define LOG_TAG "MediaServer"
#include <utils/Log.h>
#include "MediaRecord.h"
#include "MedialibClientProxy.h"
#include <libgen.h>
#else
   #define MEDIARECORD_TITLE "title"
   #define MEDIARECORD_PATH "path"
   #include <utils/dlna_log.h>
#endif

#include "utils/list.h"
#include <server/dms-playlist.h>
#include "dlna_config.h"

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"

#define  MUSICLIST_LEN (100)
#define STRING_LEN (255)
#define OPEN_LOG (0)

int ID_MAX;
static char NULL_STRING[1] = {0};
static  char playlistname_null[1] = {0};

extern int get_item_id(int index);
void playlist_delet(PLAYLIST *playlist);

const char *playlist_load_string(int id, const char *type)
{
    return NULL;
}

int playlist_load_integer(int id, const char *type)
{
    return 0;
}

const char *playlist_get_path(int id)
{
    return NULL;
}

int playlist_get_id(int index)
{
    return  0;
}

char  *playlist_get_enconding(int id)
{
    return NULL;
}

char *playlist_load_fortype(int id, const char *type)
{
    char *string = NULL;
    const char *ret = NULL;
    int  length = 0;

    ret = playlist_load_string(id, type);
    if (NULL == ret) {
        string = (char *)malloc(sizeof(char)*2);
        memset(string, 0, sizeof(char)*2);

    } else {
        length = strlen(ret);
        string = (char *)malloc(length + 1);
        if(string == NULL) {
            ALOGE("malloc string = NULL");
            return NULL;
        }

        memset(string, 0, length + 1);
        strncpy(string, ret, length);
    }

    return string;
}

int playlist_max_count()
{
    // return get_item_count();
    return 0;
}

void musiclist_freename(MUSICLIST *musiclist)
{
    dms_free(musiclist->name);

    return;
}

void musiclist_delet(PLAYLIST *playlist)
{
    if (NULL == playlist->musiclist)
        return ;
    dms_list_clear((CgList *)(playlist->musiclist), (CG_LIST_DESTRUCTORFUNC)musiclist_freename);
    musiclist_freename(playlist->musiclist);
    free(playlist->musiclist);
    playlist->musiclist = NULL;

    return;
}

PLAYLIST* playlist_new()
{
    PLAYLIST * playlist = NULL;

    playlist = (PLAYLIST *)malloc(sizeof(PLAYLIST));
    if(playlist == NULL)
        return NULL;
    memset(playlist, 0, sizeof(PLAYLIST));

    return playlist;
}

MUSICLIST* musiclist_new()
{
    MUSICLIST * musiclist = NULL;

    musiclist = (MUSICLIST *)malloc(sizeof(MUSICLIST));
    if(musiclist == NULL)
        return NULL;
    memset(musiclist, 0, sizeof(MUSICLIST));

    return musiclist;
}

BOOL playlist_insert(PLAYLIST **playlist, int id, int index, const char *type)
{
    PLAYLIST *playlistnode = NULL;
    PLAYLIST *playlistprev = *playlist;
    const char *playlistname = NULL;
    const char *playlistnameprev = NULL;
    int ret;
    int i = 0;

    if (*playlist == NULL)
        return FALSE;

    playlistnode = playlist_new();
    playlistnode->id = id;
    playlistnode->index = index;
    playlistnode->music_count = 1;//have one

    playlistname = playlist_load_string(id, type);
    if (NULL == playlistname)
        playlistname = NULL_STRING;
    dms_string_addvalue(&playlistnode->name, playlistname);

    for (ret = 0; ret < ID_MAX; ret++) {
        playlistnameprev = playlistprev->name;
        i = strcasecmp(playlistnameprev, playlistname);
        //ALOGD("i = %d,prev = %s,name = %s",i,playlistnameprev,playlistname);

        if (i == 0) {
            playlistprev->music_count++;
            playlist_delet(playlistnode);
            return TRUE;
        } else if (i < 0) {
            if (playlistprev->next == NULL) {
                    dms_list_add((CgList*)playlistprev, (CgList *)playlistnode);
                return TRUE;
            } else
                playlistprev = playlistprev->next;
        } else { //name is same if last;
            if (playlistprev->prev == NULL) {
                dms_list_add((CgList*)playlistnode, (CgList*)playlistprev);
                *playlist = playlistnode;
                return TRUE;
            } else {
                dms_list_add((CgList*)playlistprev->prev,(CgList*)playlistnode);
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL playlist_create(PLAYLIST **playlist, const char *type)
{
    int ret = 0;
    int count = 0;
    PLAYLIST *p = NULL;
    int id_exist = 0;
    const char *str = NULL;

    if (NULL != *playlist)  {
        playlist_delet(*playlist);
    }

    count = ID_MAX;
    p = playlist_new();
    p->id = playlist_get_id(1);
    p->index = 1;
    p->music_count = 1;

    str = playlist_load_string(p->id, type);
    if (NULL == str)
        str = NULL_STRING;
    dms_string_addvalue(&p->name, str);

    if (strcmp(type, MEDIARECORD_TITLE) == 0) {//title only a playlist
        p->music_count = count;//only one
        *playlist = p;
        return TRUE;
    }

    for (ret = 2; ret <= count; ret++) {
        id_exist = playlist_get_id(ret);
        playlist_insert(&p, id_exist, ret, type);
        //ALOGD("-----playlsit_test--playlist_create  id =%d++",id_exist);
    }

    *playlist = p;

     return TRUE;
}

void playlist_delet(PLAYLIST *playlist)
{
    if (NULL != playlist) {
        dms_list_clear((CgList *)playlist, (CG_LIST_DESTRUCTORFUNC)musiclist_delet);
        musiclist_delet(playlist);
        dms_free(playlist->name);
        dms_free(playlist);
    }

    return ;
}

BOOL musiclist_create(PLAYLIST *playlist, const char *type)
{
    const char *playlistname = NULL;
    const char *musiclistname= NULL;
    int ret = 0;
    MUSICLIST *musiclistprev = NULL;
    MUSICLIST *musiclistnode = NULL;
    int i = 0;
    int id = 0;

    if (NULL != playlist->musiclist) {
        musiclist_delet(playlist);
    }
    playlistname = playlist->name;

    for (ret = 1 ;  i < playlist->music_count ; ret++) {
        id = playlist_get_id(ret);
        musiclistname = playlist_load_string(id, type);
    #if (OPEN_LOG == 1)
        ALOGD("playlist idnex = %d", ret);
    #endif
        if (musiclistname == NULL)
            musiclistname  = NULL_STRING;

        if (strcasecmp(playlistname, musiclistname) == 0 || !strcmp(type, MEDIARECORD_TITLE) ) {//!!!!
            musiclistnode =  musiclist_new();
            musiclistnode->index = ret;
            musiclistnode->id = id;

            musiclistname = playlist_load_string(id, MEDIARECORD_TITLE);//get
            if (NULL == musiclistname) {
                musiclistname = playlist_load_string(id, MEDIARECORD_PATH);//load path name
                //musiclistname = basename(musiclistname);//load file name
            }

            dms_string_addvalue(&musiclistnode->name, musiclistname);
            if (i == 0) {
                musiclistprev = musiclistnode;
                playlist->musiclist = musiclistprev;
                i++;
                continue;
            }
            dms_list_add((CgList*)musiclistprev, (CgList *)musiclistnode);
            musiclistprev = musiclistnode;
            i++;
        }
    }

    if( i == 0 ) {
        ALOGE("-----musiclist_create----fail");
        return FALSE;
    }
    playlist->musiclist->prev = NULL;
    playlist->music_current_disctance = 1;

    return TRUE;
}

BOOL playlist_create_condition ()
{
    int count = 0;

    count = playlist_max_count();
    ALOGD("---playlist_condition--id max = %d",count);
    ID_MAX = count;
    if (count <= 0 ) {
        ALOGE("the playlist cannot be create, browse get fail");
        return FALSE;
    }
    return TRUE;
}


int  playlist_load_name(char ***name, int **id, PLAYLIST *playlist)
{
    PLAYLIST *cur_playlist = NULL;
    int count = 0;
    char ** cur_name = NULL;
    char ** new = NULL;
    int *cur_id = NULL;
    int *new_id = NULL;

    if (NULL == playlist)
        return 0;

    for (cur_playlist = playlist ; cur_playlist != NULL; cur_playlist = cur_playlist->next, count++) {
        new = (char **) realloc (cur_name, (count + 1) * sizeof (*cur_name));
        if (new == NULL)
            break;
        cur_name = new;
        cur_name[count] = cur_playlist->name;
        //ALOGD("----load_name id = %d, name = %s", cur_playlist->id, cur_playlist->name);

        new_id = (int*) realloc (cur_id, (count + 2) * sizeof (*cur_id));
        if (new_id == NULL)
            break;
        cur_id = new_id;
        cur_id[count] = cur_playlist->id;
    }
    *name = cur_name;
    *id = cur_id;

    return count;
}

int  musiclist_load_msg(char ***name, int **id, MUSICLIST *musiclist)
{
    MUSICLIST *cur_playlist = NULL;
    int count = 0;
    char ** cur_name = NULL;
    char ** new_name = NULL;
    int *cur_id = NULL;
    int *new_id = NULL;

    if (NULL == musiclist)
        return 0;

    for (cur_playlist = musiclist ; cur_playlist != NULL; cur_playlist = cur_playlist->next, count++) {
        new_name = (char **) realloc (cur_name, (count + 2) * sizeof (*cur_name));
        if (new_name == NULL)
            break;
        cur_name = new_name;
        cur_name[count] = cur_playlist->name;

        new_id = (int*) realloc (cur_id, (count + 2) * sizeof (*cur_id));
        if (new_id == NULL)
            break;
        cur_id = new_id;
        cur_id[count] = cur_playlist->id;
    }
    *name = cur_name;
    *id = cur_id;

    return count;
}

PLAYLIST * playlist_load_playlist_forname(const char *name, PLAYLIST *playlist)
{
    PLAYLIST *cur_playlist = NULL;

    if (NULL == playlist)
        return NULL;

    for (cur_playlist = playlist; cur_playlist != NULL; cur_playlist = cur_playlist->next)
        if(0 == strcmp(name, cur_playlist->name))
            return cur_playlist;

    return NULL;
}

//#pragma arm section code
#endif

