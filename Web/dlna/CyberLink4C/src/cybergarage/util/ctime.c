/******************************************************************
*
*    CyberUtil for C
*
*    Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*    File: ctime.cpp
*
*    Revision:
*
*    02/07/05
*        - first revision
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <rtos_config.h>
#include "FreeRTOS.h"
#include "task.h"

#include <cybergarage/util/ctime.h>
#include <cybergarage/util/clog.h>

#include <limits.h>

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"


#if defined(WIN32) && !defined(ITRON) && !defined (WINCE)
#include <windows.h>
#include <time.h>
#elif defined(WIN32) && defined (WINCE)
#include <windows.h>
#include <time.h>
//#include <altcecrt.h>
#elif defined(BTRON)
#include <btron/proctask.h>
#include <btron/clk.h>
#elif defined(ITRON)
#include <kernel.h>
#elif defined(TENGINE) && !defined(PROCESS_BASE)
#include <tk/tkernel.h>
#include <time.h>
#elif defined(TENGINE) && defined(PROCESS_BASE)
#include <tk/tkernel.h>
#include <btron/proctask.h>
#include <time.h>
#elif defined(FREE_RTOS)

#else
#include <unistd.h>
#include <time.h>
#endif

/****************************************
* cg_time_wait
****************************************/
#if defined(FREE_RTOS)
int rk_time(int itme)
{
    return xTaskGetTickCount()/100;
}
#endif
void cg_wait(CgTime mtime)
{
    cg_log_debug_l4("Entering...\n");

#if defined(WIN32) && !defined(ITRON)
    Sleep(mtime);
#elif defined(BTRON)
    slp_tsk(mtime);
#elif defined(ITRON)
    tslp_tsk(mtime);
#elif defined(TENGINE) && !defined(PROCESS_BASE)
    tk_slp_tsk(mtime);
#elif defined(TENGINE) && defined(PROCESS_BASE)
    b_slp_tsk(mtime);
#elif defined(FREE_RTOS)
        vTaskDelay((mtime/10) ? (mtime/10):1);
#else
    usleep(((useconds_t)(mtime * 1000)));
#endif

    cg_log_debug_l4("Leaving...\n");
}

/****************************************
* cg_time_wait
****************************************/

void cg_waitrandom(CgTime mtime)
{
    double factor;
    long waitTime;

    cg_log_debug_l4("Entering...\n");

    factor = (double)rand() / (double)RAND_MAX;
    waitTime = (long)((double)mtime * factor);
    cg_wait((int)waitTime);

    cg_log_debug_l4("Leaving...\n");
}

/****************************************
* cg_time_wait
****************************************/

CgTime cg_getcurrentsystemtime()
{
#if defined(BTRON)
    STIME CgTime;
    TIMEZONE tz;
    STIME localtime;
    if (get_tim(&CgTime, &tz) != 0)
        return 0;
    localtime = CgTime - tz.adjust + (tz.dst_flg ? (tz.dst_adj*60): 0);
#elif defined(ITRON)
    static BOOL initialized = FALSE;
    SYSTIM sysTim;
    if (initialized == FALSE) {
        sysTim.utime = 0;
        sysTim.ltime = 0;
        set_tim(&sysTim);
    }
    get_tim(&sysTim);
#endif

    cg_log_debug_l4("Entering...\n");

    cg_log_debug_l4("Leaving...\n");

#if defined(BTRON)
    return localtime;
#elif defined(ITRON)
    return ((sysTim.utime / 1000) << 32) + (sysTim.ltime / 1000);
#elif defined(FREE_RTOS)
    return rk_time(0);
#else
    return time(NULL);
#endif
}

/****************************************
* cg_random
****************************************/

float cg_random()
{
    static BOOL seedDone = FALSE;

    cg_log_debug_l4("Entering...\n");

    if (seedDone == FALSE) {
        srand((int)(cg_getcurrentsystemtime() % INT_MAX));
        seedDone = TRUE;
    }

    cg_log_debug_l4("Leaving...\n");

    return (float)rand() / (float)RAND_MAX;
}

//#pragma arm section code
#endif
