/*
 *
 *  UDN for dlna devices.
 *
 *    2014-01-14 by rockchip IOT team.
 *
 *
 */
#include "source_macro.h"
#ifdef __WIFI_DLNA_C__

#include <utils/udn.h>

#include <rtos_config.h>
#if !defined(FREE_RTOS)
#include <fcntl.h>
#else
#include <stdlib.h>
#include <utils/dlna_log.h>
#include "portmacro.h"
#endif

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#define UUID_LEN (36)


int create_udn(char * udn, int len)
{
    char uuid[128] = { 0 };
    int fd;
    int num;

#if defined(HAVE_FILE_SYS)
    fd = open("/proc/sys/kernel/random/uuid", O_RDONLY);
    if (fd < 0)
        return -1;

    read(fd, uuid, UUID_LEN);
    sprintf(udn, "uuid:%s", uuid);

    close(fd);
#else
    portTickType seed = xTaskGetTickCount();
    srand(seed);
    for (num = 0; num < UUID_LEN; num++) {
        uuid[num] = rand()%26 + 'a';
    }
    sprintf(udn, "uuid:%s", uuid);
 #endif

    return 0;
}
//#pragma arm section code
#endif

