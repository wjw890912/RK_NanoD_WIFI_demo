#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <errno.h>
#include <string.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"

#define MAXPATHLEN 1024


int basename_r(const char* path, char*  buffer, size_t  bufflen)
{
    const char *endp, *startp;
    int         len, result;
    char        temp[2];

    /* Empty or NULL string gets treated as "." */
    if (path == NULL || *path == '\0') {
        startp  = ".";
        len     = 1;
        goto Exit;
    }

    /* Strip trailing slashes */
    endp = path + strlen(path) - 1;
    while (endp > path && *endp == '/')
        endp--;

    /* All slashes becomes "/" */
    if (endp == path && *endp == '/') {
        startp = "/";
        len    = 1;
        goto Exit;
    }

    /* Find the start of the base */
    startp = endp;
    while (startp > path && *(startp - 1) != '/')
        startp--;

    len = endp - startp +1;

Exit:
    result = len;
    if (buffer == NULL) {
        return result;
    }
    if (len > (int)bufflen-1) {
        len    = (int)bufflen-1;
        result = -1;
        errno  = ERANGE;
    }

    if (len >= 0) {
        memcpy( buffer, startp, len );
        buffer[len] = 0;
    }
    return result;
}


char * basename(const char*  path)
{
    static char*  bname = NULL;
    int           ret;

    if (bname == NULL) {
        bname = (char *)malloc(MAXPATHLEN);
        if (bname == NULL)
            return(NULL);
    }
    ret = basename_r(path, bname, MAXPATHLEN);
    return (ret < 0) ? NULL : bname;
}

//#pragma arm section code
#endif

