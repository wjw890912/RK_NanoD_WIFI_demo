/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: OS\Plugin\RKOS.h
* Owner: Aaron.sun
* Date: 2014.5.28
* Time: 14:52:29
* Desc: RKOS H
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    Aaron.sun     2014.5.28     14:52:29   1.0
********************************************************************************************
*/

#ifndef __OS_PLUGIN_RKOS_H__
#define __OS_PLUGIN_RKOS_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "typedef.h"
#include "TaskPlugin.h"
#include "device.h"
#include "DeviceManagerTask.h"
#include "ShellTask.h"
#include "FwAnalysis.h"
#include "ModuleInfoTab.h"
#include "PowerManager.h"
#include "SysResume.h"
#include "DriverInclude.h"
#include "DeviceInclude.h"
#include "AppInclude.h"
#include "global.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef void * pSemaphore;
typedef void * pQueue;
typedef void * pTimer;
typedef void (* pRkosTimerCallBack)(pTimer);
typedef void (* pRkosTaskCode)( void * );


#define MAX_DELAY 0xffffffff

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
extern void rkos_task_delete(HTC hTask);
extern HTC rkos_task_create(pRkosTaskCode TaskCode,char *name, uint32 StatckBase, uint32 StackDeep, uint32 Priority, void * para);
extern rk_err_t rkos_queue_recive_fromisr(pQueue pQue, void * buf);
extern rk_err_t rkos_queue_send_fromisr(pQueue pQue, void * buf);
extern rk_err_t rkos_start_timer(pTimer timer);
extern void rkos_delete_timer(pTimer timer);
extern pTimer rkos_create_timer(uint32 period, uint32 reload, pRkosTimerCallBack pfCall);
extern void rkos_resume_all_task(void);
extern void rkos_suspend_all_task(void);
extern pQueue rkos_queue_create(uint32 blockcnt, uint32 blocksize);
extern void rkos_queue_delete(pQueue pQue);
extern void rkos_queue_send(pQueue pQue, void * buf, uint32 time);
extern rk_err_t rkos_queue_receive(pQueue pQue, void * buf, uint32 time);
extern void rkos_semaphore_give_fromisr(pSemaphore pSem);
extern void rkos_start(void);
extern void rkos_init(void);
extern void rkos_memory_free(void * buf);
extern void * rkos_memory_malloc(uint32 size);
extern void rkos_semaphore_delete(pSemaphore pSem);
extern pSemaphore rkos_semaphore_create(uint32 MaxCnt, uint32 InitCnt);
extern void rkos_semaphore_give(pSemaphore pSem);
extern void rkos_semaphore_take(pSemaphore pSem, uint32 time);
extern void rkos_exit_critical(void);
extern void rkos_enter_critical(void);



#endif

