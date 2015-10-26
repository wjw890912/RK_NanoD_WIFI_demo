/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: WICE\wifithread.c
* Owner: linyb
* Date: 2015.7.27
* Time: 8:48:53
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    linyb     2015.7.27     8:48:53   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef _DRIVER_AP6181_WIFI_C__
//#pragma arm section code = "ap6181wifiCode", rodata = "ap6181wifiCode", rwdata = "ap6181wifidata", zidata = "ap6181wifidata"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"
#include "wifithread.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#if 0
typedef  struct _TCPIP_THREAD_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}TCPIP_THREAD_RESP_QUEUE;
typedef  struct _TCPIP_THREAD_ASK_QUEUE
{
    uint32 cmd;

}TCPIP_THREAD_ASK_QUEUE;
typedef  struct _TCPIP_THREAD_TASK_DATA_BLOCK
{
    pQueue  tcpip_threadAskQueue;
    pQueue  tcpip_threadRespQueue;

}TCPIP_THREAD_TASK_DATA_BLOCK;

typedef  struct _WWD_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}WWD_RESP_QUEUE;
typedef  struct _WWD_ASK_QUEUE
{
    uint32 cmd;

}WWD_ASK_QUEUE;
typedef  struct _WWD_TASK_DATA_BLOCK
{
    pQueue  wwdAskQueue;
    pQueue  wwdRespQueue;

}WWD_TASK_DATA_BLOCK;
typedef  struct _NETWORKING_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}NETWORKING_RESP_QUEUE;
typedef  struct _NETWORKING_ASK_QUEUE
{
    uint32 cmd;

}NETWORKING_ASK_QUEUE;
typedef  struct _NETWORKING_TASK_DATA_BLOCK
{
    pQueue  networkingAskQueue;
    pQueue  networkingRespQueue;

}NETWORKING_TASK_DATA_BLOCK;

typedef  struct _HARDWARE_TO_WORK_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}HARDWARE_TO_WORK_RESP_QUEUE;
typedef  struct _HARDWARE_TO_WORK_ASK_QUEUE
{
    uint32 cmd;

}HARDWARE_TO_WORK_ASK_QUEUE;
typedef  struct _HARDWARE_TO_WORK_TASK_DATA_BLOCK
{
    pQueue  hardware_to_workAskQueue;
    pQueue  hardware_to_workRespQueue;

}HARDWARE_TO_WORK_TASK_DATA_BLOCK;

typedef  struct _WIFI_APPLICATION_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}WIFI_APPLICATION_RESP_QUEUE;
typedef  struct _WIFI_APPLICATION_ASK_QUEUE
{
    uint32 cmd;

}WIFI_APPLICATION_ASK_QUEUE;
typedef  struct _WIFI_APPLICATION_TASK_DATA_BLOCK
{
    pQueue  wifi_applicationAskQueue;
    pQueue  wifi_applicationRespQueue;

}WIFI_APPLICATION_TASK_DATA_BLOCK;
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#if 0
static TCPIP_THREAD_TASK_DATA_BLOCK * gpsttcpip_threadData;
static WWD_TASK_DATA_BLOCK * gpstwwdData;
static NETWORKING_TASK_DATA_BLOCK * gpstnetworkingData;
static HARDWARE_TO_WORK_TASK_DATA_BLOCK * gpsthardware_to_workData;
static WIFI_APPLICATION_TASK_DATA_BLOCK * gpstwifi_applicationData;
#endif



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

extern void wifi_application_thread(void * arg);
extern void wwd_thread_func( unsigned long /*@unused@*/thread_input );
extern void tcpip_thread(void *arg);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: tcpip_threadTask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 13:49:37
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
#ifdef __WICE_TCPIP_C__
COMMON API rk_err_t tcpip_threadTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: tcpip_threadTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 13:49:37
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t tcpip_threadTask_Suspend(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: tcpip_threadTask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.27
** Time: 13:49:37
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API void tcpip_threadTask_Enter(void)
{
    tcpip_thread(NULL);
}
#endif
/*******************************************************************************
** Name: wwdTask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 11:57:20
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
#ifdef __WICE_WIFI_TRANS_C__
COMMON API rk_err_t wwdTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wwdTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 11:57:20
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t wwdTask_Suspend(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wwdTask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.27
** Time: 11:57:20
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API void wwdTask_Enter(void)
{
    wwd_thread_func(NULL);
}
#endif
#if 0
/*******************************************************************************
** Name: networkingTask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:48:08
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t networkingTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: networkingTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:48:08
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t networkingTask_Suspend(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: networkingTask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.27
** Time: 10:48:08
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API void networkingTask_Enter(void)
{
    while(1)
    {rkos_sleep(7000);}
}
/*******************************************************************************
** Name: hardware_to_workTask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:32:09
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t hardware_to_workTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: hardware_to_workTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:32:09
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t hardware_to_workTask_Suspend(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: hardware_to_workTask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.27
** Time: 10:32:09
*******************************************************************************/
_WICE_WIFITHREAD_COMMON_
COMMON API void hardware_to_workTask_Enter(void)
{
    while(1)
    {rkos_sleep(7000);}
}
#endif
/*******************************************************************************
** Name: wifi_applicationTask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 8:54:16
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
#ifdef __WICE_WIFITHREAD_C__
COMMON API rk_err_t wifi_applicationTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wifi_applicationTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 8:54:16
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API rk_err_t wifi_applicationTask_Suspend(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wifi_applicationTask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.27
** Time: 8:54:16
*******************************************************************************/
//_WICE_WIFITHREAD_COMMON_
COMMON API void wifi_applicationTask_Enter(void)
{
    wifi_application_thread(NULL);
}

#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: tcpip_threadTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 13:49:37
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
#ifdef __WICE_TCPIP_C__
INIT API rk_err_t tcpip_threadTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpsttcpip_threadData->tcpip_threadAskQueue);
    rkos_queue_delete(gpsttcpip_threadData->tcpip_threadRespQueue);
    rkos_memory_free(gpsttcpip_threadData);
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: tcpip_threadTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 13:49:37
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
INIT API rk_err_t tcpip_threadTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   ptcpip_threadTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_TCPIP_THREAD_ARG * pArg = (RK_TASK_TCPIP_THREAD_ARG *)arg;
    TCPIP_THREAD_TASK_DATA_BLOCK*  ptcpip_threadTaskData;
    if (ptcpip_threadTask == NULL)
        return RK_PARA_ERR;

    ptcpip_threadTaskData = rkos_memory_malloc(sizeof(TCPIP_THREAD_TASK_DATA_BLOCK));
    memset(ptcpip_threadTaskData, NULL, sizeof(TCPIP_THREAD_TASK_DATA_BLOCK));
#endif
    return RK_SUCCESS;
}
#endif
/*******************************************************************************
** Name: wwdTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 11:57:20
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
#ifdef __WICE_WIFI_TRANS_C__
INIT API rk_err_t wwdTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpstwwdData->wwdAskQueue);
    rkos_queue_delete(gpstwwdData->wwdRespQueue);
    rkos_memory_free(gpstwwdData);
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wwdTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 11:57:20
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
INIT API rk_err_t wwdTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   pwwdTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_WWD_ARG * pArg = (RK_TASK_WWD_ARG *)arg;
    WWD_TASK_DATA_BLOCK*  pwwdTaskData;
    if (pwwdTask == NULL)
        return RK_PARA_ERR;

    pwwdTaskData = rkos_memory_malloc(sizeof(WWD_TASK_DATA_BLOCK));
    memset(pwwdTaskData, NULL, sizeof(WWD_TASK_DATA_BLOCK));
#endif
    return RK_SUCCESS;
}
#endif
#if 0
/*******************************************************************************
** Name: networkingTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:48:08
*******************************************************************************/
_WICE_WIFITHREAD_INIT_
INIT API rk_err_t networkingTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpstnetworkingData->networkingAskQueue);
    rkos_queue_delete(gpstnetworkingData->networkingRespQueue);
    rkos_memory_free(gpstnetworkingData);
#endif
}
/*******************************************************************************
** Name: networkingTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:48:08
*******************************************************************************/
_WICE_WIFITHREAD_INIT_
INIT API rk_err_t networkingTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   pnetworkingTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_NETWORKING_ARG * pArg = (RK_TASK_NETWORKING_ARG *)arg;
    NETWORKING_TASK_DATA_BLOCK*  pnetworkingTaskData;
    if (pnetworkingTask == NULL)
        return RK_PARA_ERR;

    pnetworkingTaskData = rkos_memory_malloc(sizeof(NETWORKING_TASK_DATA_BLOCK));
    memset(pnetworkingTaskData, NULL, sizeof(NETWORKING_TASK_DATA_BLOCK));
#endif
}
/*******************************************************************************
** Name: hardware_to_workTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:32:09
*******************************************************************************/
_WICE_WIFITHREAD_INIT_
INIT API rk_err_t hardware_to_workTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpsthardware_to_workData->hardware_to_workAskQueue);
    rkos_queue_delete(gpsthardware_to_workData->hardware_to_workRespQueue);
    rkos_memory_free(gpsthardware_to_workData);
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: hardware_to_workTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 10:32:09
*******************************************************************************/
_WICE_WIFITHREAD_INIT_
INIT API rk_err_t hardware_to_workTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   phardware_to_workTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_HARDWARE_TO_WORK_ARG * pArg = (RK_TASK_HARDWARE_TO_WORK_ARG *)arg;
    HARDWARE_TO_WORK_TASK_DATA_BLOCK*  phardware_to_workTaskData;
    if (phardware_to_workTask == NULL)
        return RK_PARA_ERR;

    phardware_to_workTaskData = rkos_memory_malloc(sizeof(HARDWARE_TO_WORK_TASK_DATA_BLOCK));
    memset(phardware_to_workTaskData, NULL, sizeof(HARDWARE_TO_WORK_TASK_DATA_BLOCK));
#endif
    return RK_SUCCESS;
}
#endif
/*******************************************************************************
** Name: wifi_applicationTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 8:54:16
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
#ifdef __WICE_WIFITHREAD_C__
INIT API rk_err_t wifi_applicationTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpstwifi_applicationData->wifi_applicationAskQueue);
    rkos_queue_delete(gpstwifi_applicationData->wifi_applicationRespQueue);
    rkos_memory_free(gpstwifi_applicationData);
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: wifi_applicationTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.27
** Time: 8:54:16
*******************************************************************************/
//_WICE_WIFITHREAD_INIT_
INIT API rk_err_t wifi_applicationTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   pwifi_applicationTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_WIFI_APPLICATION_ARG * pArg = (RK_TASK_WIFI_APPLICATION_ARG *)arg;
    WIFI_APPLICATION_TASK_DATA_BLOCK*  pwifi_applicationTaskData;
    if (pwifi_applicationTask == NULL)
        return RK_PARA_ERR;

    pwifi_applicationTaskData = rkos_memory_malloc(sizeof(WIFI_APPLICATION_TASK_DATA_BLOCK));
    memset(pwifi_applicationTaskData, NULL, sizeof(WIFI_APPLICATION_TASK_DATA_BLOCK));
#endif
    return RK_SUCCESS;
}
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

//#pragma arm section code


