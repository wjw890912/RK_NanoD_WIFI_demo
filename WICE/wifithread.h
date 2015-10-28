/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: WICE\wifithread.h
* Owner: linyb
* Date: 2015.7.27
* Time: 8:53:50
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    linyb     2015.7.27     8:53:50   1.0
********************************************************************************************
*/


#ifndef __WICE_WIFITHREAD_H__
#define __WICE_WIFITHREAD_H__
#include "typedef.h"
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#if 0
typedef struct _RK_TASK_TCPIP_THREAD_ARG
{
    uint32 xxx;
}RK_TASK_TCPIP_THREAD_ARG;

typedef struct _RK_TASK_WWD_ARG
{
    uint32 xxx;
}RK_TASK_WWD_ARG;

typedef struct _RK_TASK_NETWORKING_ARG
{
    uint32 xxx;
}RK_TASK_NETWORKING_ARG;

typedef struct _RK_TASK_HARDWARE_TO_WORK_ARG
{
    uint32 xxx;
}RK_TASK_HARDWARE_TO_WORK_ARG;

typedef struct _RK_TASK_WIFI_APPLICATION_ARG
{
    uint32 xxx;
}RK_TASK_WIFI_APPLICATION_ARG;
#endif

//#define _WICE_WIFITHREAD_COMMON_  __attribute__((section("wice_wifithread_common")))
//#define _WICE_WIFITHREAD_INIT_  __attribute__((section("wice_wifithread_init")))
//#define _WICE_WIFITHREAD_SHELL_  __attribute__((section("wice_wifithread_shell")))
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t tcpip_threadTask_Resume(void);
extern rk_err_t tcpip_threadTask_Suspend(void);
extern void tcpip_threadTask_Enter(void);
extern rk_err_t tcpip_threadTask_DeInit(void *pvParameters);
extern rk_err_t tcpip_threadTask_Init(void *pvParameters, void *arg);
extern rk_err_t wwdTask_Resume(void);
extern rk_err_t wwdTask_Suspend(void);
extern void wwdTask_Enter(void);
extern rk_err_t wwdTask_DeInit(void *pvParameters);
extern rk_err_t wwdTask_Init(void *pvParameters, void *arg);
#if 0
extern rk_err_t networkingTask_Resume(void);
extern rk_err_t networkingTask_Suspend(void);
extern void networkingTask_Enter(void);
extern rk_err_t networkingTask_DeInit(void *pvParameters);
extern rk_err_t networkingTask_Init(void *pvParameters, void *arg);
extern rk_err_t hardware_to_workTask_Resume(void);
extern rk_err_t hardware_to_workTask_Suspend(void);
extern void hardware_to_workTask_Enter(void);
extern rk_err_t hardware_to_workTask_DeInit(void *pvParameters);
extern rk_err_t hardware_to_workTask_Init(void *pvParameters, void *arg);
#endif
extern rk_err_t wifi_applicationTask_Resume(void);
extern rk_err_t wifi_applicationTask_Suspend(void);
extern void wifi_applicationTask_Enter(void);
extern rk_err_t wifi_applicationTask_DeInit(void *pvParameters);
extern rk_err_t wifi_applicationTask_Init(void *pvParameters, void *arg);



#endif
