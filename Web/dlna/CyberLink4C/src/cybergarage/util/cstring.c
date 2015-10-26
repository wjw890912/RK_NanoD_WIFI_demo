/******************************************************************
*
*    CyberUtil for C
*
*    Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006-2007 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*    File: cstring.c
*
*    Revision:
*
*    01/25/05
*        - first revision
*    12/13/07 Aapo Makela
*        - Changed memory reallocation policy to more efficient
*        - Fix crashes in out-of-memory situations
*    05/20/09
*        - Improved cg_string_*() not to abort when the string object is null.
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <cybergarage/util/cstring.h>
#include <cybergarage/util/clog.h>

#include <string.h>
#include <rtos_config.h>
#include <rk_heap.h>

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"

/* Define amount of extra characters allocated on each realloc, with this we
   can avoid many small subsequent reallocs, which takes lots of time */
#define CG_STRING_REALLOC_EXTRA        16

/****************************************
* cg_string_new
****************************************/
int string_new_cnt = 0;
static int is_cg_strint_init = 0;
static char * cg_string_buf = NULL;
#define CG_STRING_BUF_SIZE 928
static int * use_flag = NULL;
CgString *cg_string_new()
{
    CgString *str = NULL;
    int i;
    unsigned long CgStringSize = sizeof(CgString);
    //printf("a CgStringSize = %ld\n", CgStringSize);

    if(!is_cg_strint_init)
    {
        is_cg_strint_init = 1;
        cg_string_buf = malloc(sizeof(CgString)*CG_STRING_BUF_SIZE);
        if(NULL == cg_string_buf)
        {
            printf("cg_string_buf malloc fail\n");
            return str;
        }
        use_flag = (unsigned int *)malloc(CG_STRING_BUF_SIZE/8);
        if(NULL == use_flag)
        {
            free(cg_string_buf);
            printf("cstring use_flag malloc fail\n");
            return str;
        }
        memset(use_flag, 0 , CG_STRING_BUF_SIZE/8);
    }
    cg_log_debug_l5("Entering...\n");
    for(i=0; i<CG_STRING_BUF_SIZE;i++)
    {   int group = i/32;
        int index = i%32;
        if((use_flag[group] & (1<< index)) == 0)
        {
            use_flag[group] |= (1<< index);

            str = (CgString *)(cg_string_buf+i*sizeof(CgString));
            str->value = NULL;
            str->memSize = 0;
            str->valueSize = 0;
            string_new_cnt++;

           break;
        }
    }
    //printf("string_new_cnt = %d\n",string_new_cnt);
#if 1
    if(str == NULL)
    {
        str = (CgString *)malloc(sizeof(CgString));

        if (NULL != str) {
            str->value = NULL;
            str->memSize = 0;
            str->valueSize = 0;
        }
    }
#endif

    cg_log_debug_l5("Leaving...\n");

    return str;
}

/****************************************
* cg_string_delete
****************************************/

void cg_string_delete(CgString *str)
{
    cg_log_debug_l5("Entering...\n");
#if 0
    if (NULL != str) {
        cg_string_clear(str);
        free(str);
    }
#endif
    if(NULL != str)
    {
        cg_string_clear(str);

        if(((unsigned int)str >= (unsigned int)cg_string_buf) && ((unsigned int)str < (unsigned int)cg_string_buf + sizeof(CgString)*CG_STRING_BUF_SIZE))
        {
            int i = ((unsigned int)str-(unsigned int)cg_string_buf)/sizeof(CgString);
            int group = i/32;
            int index = i%32;
            use_flag[group] &= ~(1<< index);
            string_new_cnt--;
            //printf("delete string_new_cnt = %d\n",string_new_cnt);
            if(string_new_cnt == 0)
            {
                is_cg_strint_init = 0;
                free(cg_string_buf);
                free(use_flag);
                cg_string_buf = NULL;
                use_flag = NULL;
            }
        }
        else
        {
           free(str);
        }
    }
    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_delete
****************************************/

void cg_string_clear(CgString *str)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL != str) {
        if (str->value != NULL) {
            free(str->value);
            str->value = NULL;
            str->memSize = 0;
            str->valueSize = 0;
        }
    }

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_setvalue
****************************************/

void cg_string_setvalue(CgString *str, const char *value)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL != str) {
        if (value != NULL)
            cg_string_setnvalue(str, value, cg_strlen(value));
    }

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_setintvalue
****************************************/

void cg_string_setintvalue(CgString *str, int value)
{
    char buf[CG_STRING_INTEGER_BUFLEN];

    cg_log_debug_l5("Entering...\n");

    cg_string_setvalue(str, cg_int2str(value, buf, sizeof(buf)));

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_setlongvalue
****************************************/

void cg_string_setlongvalue(CgString *str, long value)
{
    char buf[CG_STRING_LONG_BUFLEN];

    cg_log_debug_l5("Entering...\n");

    cg_string_setvalue(str, cg_long2str(value, buf, sizeof(buf)));

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_setnvalue
****************************************/

void cg_string_setnvalue(CgString *str, const char *value, size_t len)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL != str) {
        cg_string_clear(str);
        if (value != NULL) {
            str->valueSize = len;
            str->memSize = str->valueSize + 1;
            str->value = (char *)malloc(str->memSize * sizeof(char));

            if ( NULL == str->value ) {
                cg_log_debug_s("Memory allocation failure!\n");
                return;
            }

            /* memcpy works better with non-zero-terminated data than strncpy */
            memcpy(str->value, value, len);
            str->value[len] = '\0';
        }
    }

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_setpointervalue
****************************************/

void cg_string_setpointervalue(CgString *str, char *value, size_t len)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL != str) {
        cg_string_clear(str);
        str->value = value;
        str->valueSize = len;
        str->memSize = str->valueSize + 1;
    }

    cg_log_debug_l5("Leaving...\n");
}

/****************************************
* cg_string_getvalue
****************************************/

char *cg_string_getvalue(CgString *str)
{
    cg_log_debug_l5("Entering...\n");

    cg_log_debug_l5("Leaving...\n");

    return (NULL != str) ? str->value : NULL;
}

/****************************************
* cg_string_getmemorysize
****************************************/

size_t cg_string_getmemorysize(CgString *str)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL == str)
        return 0;

    cg_log_debug_l5("Leaving...\n");

    return str->memSize;
}

/****************************************
* cg_string_length
****************************************/

size_t cg_string_length(CgString *str)
{
    cg_log_debug_l5("Entering...\n");

    if (NULL == str)
        return 0;

    if (str->value == NULL)
        return 0;

    cg_log_debug_l5("Leaving...\n");

    return str->valueSize;
}

/****************************************
* cg_string_add
****************************************/

char *cg_string_addvalue(CgString *str, const char *value)
{
    cg_log_debug_l5("Entering...\n");

    cg_log_debug_l5("Leaving...\n");

    return cg_string_naddvalue(str, value, cg_strlen(value));
}

/****************************************
* cg_string_add
****************************************/

char *cg_string_naddvalue(CgString *str, const char *value, size_t valueLen)
{
    char *newValue = NULL;
    size_t newMemSize = 0;

    cg_log_debug_l5("Entering...\n");

    if (NULL == str)
        return NULL;

    if (value == NULL || valueLen <= 0)
    {
        /* Empty string, nothing to add */
        return cg_string_getvalue(str);
    }

    /* Check, if we need to allocate memory for the new data */
    newMemSize = str->valueSize + valueLen + 1;
    if (newMemSize > str->memSize || str->value == NULL)
    {
        /* realloc also some extra in order to avoid multiple reallocs */
        newMemSize += CG_STRING_REALLOC_EXTRA;
#if !defined(FREE_RTOS)
        newValue = realloc(str->value, newMemSize * sizeof(char));
#else
        newValue = rk_realloc(str->value, str->memSize, newMemSize * sizeof(char));
#endif
        if (newValue == NULL)
        {
            /* Memory allocation failed, bail out */
            return NULL;
        }

        str->memSize = newMemSize;
        str->value = newValue;
    }

    /* memcpy works better with non-zero-terminated data
       than strncpy */
    memcpy(str->value + str->valueSize, value, valueLen);

    str->valueSize += valueLen;

    /* In case this is a string, append a termination character */
    str->value[str->valueSize] = '\0';

    cg_log_debug_l5("Leaving...\n");

    return cg_string_getvalue(str);
}

/****************************************
* cg_string_addrep
****************************************/

char *cg_string_addrepvalue(CgString *str, const char *value, size_t repeatCnt)
{
    int n;
    cg_log_debug_l5("Entering...\n");

    for (n = 0; n < repeatCnt; n++)
        cg_string_addvalue(str, value);

    cg_log_debug_l5("Leaving...\n");

    return cg_string_getvalue(str);
}

/****************************************
* cg_string_naddrep
****************************************/

char *cg_string_naddrepvalue(CgString *str, const char *value, size_t valueLen, size_t repeatCnt)
{
    int n;

    cg_log_debug_l5("Entering...\n");

    for (n = 0; n < repeatCnt; n++)
        cg_string_naddvalue(str, value, valueLen);

    cg_log_debug_l5("Leaving...\n");

    return cg_string_getvalue(str);
}

/****************************************
* cg_string_replace
****************************************/

char *cg_string_replace(CgString *str, char *fromStr[], char *toStr[], size_t fromStrCnt)
{
    char *orgValue = NULL;
    size_t orgValueLen = 0;
    int n = 0;
    int copyPos = 0;
    size_t *fromStrLen = NULL;
    CgString *repValue = NULL;
    BOOL isReplaced = FALSE;

    cg_log_debug_l5("Entering...\n");

    if (NULL == str )
        return NULL;

    repValue = cg_string_new();

    fromStrLen = (size_t *)malloc(sizeof(size_t) * fromStrCnt);

    if ( NULL == fromStrLen )
    {
        cg_log_debug_s("Memory allocation failure!\n");
        return NULL;
    }

    for (n=0; n<fromStrCnt; n++)
        fromStrLen[n] = cg_strlen(fromStr[n]);

    orgValue = cg_string_getvalue(str);
    orgValueLen = cg_string_length(str);

    copyPos = 0;
    while (copyPos<orgValueLen) {
        isReplaced = FALSE;
        for (n=0; n<fromStrCnt; n++) {
            if (strncmp(fromStr[n], orgValue + copyPos,  fromStrLen[n]) == 0) {
                cg_string_addvalue(repValue, toStr[n]);
                copyPos += fromStrLen[n];
                isReplaced = TRUE;
                continue;
            }
        }
        if (isReplaced == TRUE)
            continue;
        cg_string_naddvalue(repValue, orgValue + copyPos, 1);
        copyPos++;
    }

    free(fromStrLen);

    cg_string_setvalue(str, cg_string_getvalue(repValue));

    cg_string_delete(repValue);

    cg_log_debug_l5("Leaving...\n");

    return cg_string_getvalue(str);
}
//#pragma arm section code
#endif
