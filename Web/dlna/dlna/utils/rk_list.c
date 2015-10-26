#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <rtos_config.h>
 #if !defined(FREE_RTOS)

#define LOG_TAG "SP_list"
#include <utils/Log.h>
#else
#include <utils/dlna_log.h>
#endif

#include <utils/list.h>

#ifndef NULL
#define NULL 0
#endif

#include <stdlib.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"

char* dms_add_nullstr()
{
    char *str = NULL;
    str = (char *)malloc(1);
    memset(str, 0, 1);
    return str;
}

int dms_strlen(char *str)
{
    if (NULL == str)
        return 0;
    return strlen(str);
}
void dms_string_addvalue (char **str, const char *value)
{
    int value_len = 0;
    int str_len = 0;
    int newstr_len = 0;
    char *new_str = NULL;

    value_len = dms_strlen((char *)value);
    if (NULL == value)
        return;

    str_len = dms_strlen(*str);
    newstr_len =  str_len + value_len + 1;
    new_str = realloc(*str, newstr_len * sizeof(char));
    if (new_str == NULL)
    {
        /* Memory allocation failed, bail out */
        return ;
    }

    *str = new_str;
    memcpy(*str + str_len, value, value_len);
    *(*str + str_len + value_len) = '\0';
    return;
}

void dms_free(void *str)
{
    if (NULL != str)
        free(str);
    return;
}
void dms_list_header_init(CgList *list)
{

    if (NULL == list)
        return;

    list->prev = list->next = NULL;

}

void dms_list_remove(CgList *list)
{
    if(list == NULL)
        return;

    list->next->prev = list->prev;
    list->prev->next = list->next;
    list->prev = list->next = list;

    return;

}

void dms_list_add(CgList *listprev, CgList *list)
{
    if(list == NULL || listprev == NULL) {

        return;
    }
    if (listprev->next == NULL) {

        listprev->next = list;
        list->prev = listprev;
        return;
    }
    list->prev = listprev;
    list->next = listprev->next;
    listprev->next->prev = list;
    listprev->next = list;

    return;
}

void dms_list_clear(CgList *headlist, CG_LIST_DESTRUCTORFUNC desfuc)
{
    CgList *list = NULL;
    CgList *list_ent = NULL;

    if(headlist == NULL ||headlist->next == NULL )
        return ;

    list = headlist->next;

    while (list->next != NULL) {
        list_ent = list->next;
        dms_list_remove(list);
        if(NULL != desfuc)
            desfuc(list);
        else
            dms_free(list);
        list = list_ent;
    }


    if(NULL != desfuc)
        desfuc(list);
    else
        dms_free(list);

}
//#pragma arm section code
#endif

