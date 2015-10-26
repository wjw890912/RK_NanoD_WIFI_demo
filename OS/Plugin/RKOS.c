/*
********************************************************************************************
*
*               Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: OS\Plugin\RKOS.c
* Owner: aaron.sun
* Date: 2014.10.15
* Time: 10:00:08
* Desc: RKOS.c
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2014.10.15     10:00:08   1.0
********************************************************************************************
*/
#define __OS_PLUGIN_RKOS_C__
#include "BspConfig.h"
#ifdef __OS_PLUGIN_RKOS_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "RKOS.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _OS_PLUGIN_RKOS_READ_  __attribute__((section("os_plugin_rkos_read")))
#define _OS_PLUGIN_RKOS_WRITE_ __attribute__((section("os_plugin_rkos_write")))
#define _OS_PLUGIN_RKOS_INIT_  __attribute__((section("os_plugin_rkos_init")))
#define _OS_PLUGIN_RKOS_SHELL_  __attribute__((section("os_plugin_rkos_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



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
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: rkos_task_delete
** Input:HTC hTask
** Return: void
** Owner:aaron.sun
** Date: 2014.12.18
** Time: 15:44:52
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_task_delete(HTC hTask)
{
    vTaskDelete(hTask);
}

/*******************************************************************************
** Name: rkos_task_create
** Input:pRkosTaskCode TaskCode, uint32 StackDeep, uint32 p, void * para
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.18
** Time: 14:44:46
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API HTC rkos_task_create(pRkosTaskCode TaskCode,char *name, uint32 StatckBase, uint32 StackDeep, uint32 Priority, void * para)
{
    HTC hTask;
    xTaskCreate(TaskCode, name, StackDeep, para, Priority, &hTask, (uint32 *)StatckBase);
    return hTask;
}

/*******************************************************************************
** Name: rkos_queue_recive_fromisr
** Input:pQueue pQue, void * buf
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.18
** Time: 14:26:40
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API rk_err_t rkos_queue_recive_fromisr(pQueue pQue, void * buf)
{

}

/*******************************************************************************
** Name: rkos_queue_send_fromisr
** Input:pQueue pQue, void * buf
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.18
** Time: 14:25:02
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API rk_err_t rkos_queue_send_fromisr(pQueue pQue, void * buf)
{
    portBASE_TYPE xHigherPriorityTaskWoken;

    if (pdPASS == xQueueSendFromISR(pQue, buf, &xHigherPriorityTaskWoken))
    {
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}

/*******************************************************************************
** Name: rkos_start_timer
** Input:pTimer timer
** Return: void
** Owner:aaron.sun
** Date: 2014.11.10
** Time: 13:53:20
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API rk_err_t rkos_start_timer(pTimer timer)
{
    if (xTimerStart(timer,0) == pdFAIL)
    {
        return RK_ERROR;
    }
    else
    {
        return RK_SUCCESS;
    }
}

/*******************************************************************************
** Name: rkos_delete_timer
** Input:pTimer timer
** Return: void
** Owner:aaron.sun
** Date: 2014.11.10
** Time: 13:45:01
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_delete_timer(pTimer timer)
{
    xTimerDelete(timer, 0);
}

/*******************************************************************************
** Name: rkos_create_timer
** Input:uint32 period, uint32 reload, pRkosTimerCallBack pfCall
** Return: pTimer
** Owner:aaron.sun
** Date: 2014.11.10
** Time: 11:57:47
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API pTimer rkos_create_timer(uint32 period, uint32 reload, pRkosTimerCallBack pfCall)
{
    xTimerCreate(NULL, period ,reload, NULL, pfCall);
}

/*******************************************************************************
** Name: rkos_resume_all_task
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.11.10
** Time: 11:41:49
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_resume_all_task(void)
{
    xTaskResumeAll();
}

/*******************************************************************************
** Name: rkos_suspend_all_task
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.11.10
** Time: 11:41:09
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_suspend_all_task(void)
{
    vTaskSuspendAll();
}

/*******************************************************************************
** Name: rkos_queue_create
** Input:uint32 blockcnt, uint32 blocksize
** Return: pQueue
** Owner:aaron.sun
** Date: 2014.10.20
** Time: 15:06:20
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API pQueue rkos_queue_create(uint32 blockcnt, uint32 blocksize)
{
    return xQueueCreate(blockcnt, blocksize);
}

/*******************************************************************************
** Name: rkos_queue_delete
** Input:pQueue pQue
** Return: void
** Owner:aaron.sun
** Date: 2014.10.20
** Time: 15:02:52
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_queue_delete(pQueue pQue)
{
    vQueueDelete(pQue);
}

/*******************************************************************************
** Name: rkos_queue_send
** Input:pQueue pQue, void * buf, uint32 time
** Return: void
** Owner:aaron.sun
** Date: 2014.10.20
** Time: 15:01:04
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_queue_send(pQueue pQue, void * buf, uint32 time)
{
    xQueueSend(pQue, buf, time);
}

/*******************************************************************************
** Name: rkos_queue_receive
** Input:pQueue pQue, void * buf, uint32 time
** Return: void
** Owner:aaron.sun
** Date: 2014.10.20
** Time: 14:57:50
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API rk_err_t rkos_queue_receive(pQueue pQue, void * buf, uint32 time)
{
    if (xQueueReceive(pQue, buf, time) == pdPASS)
    {
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}

/*******************************************************************************
** Name: rkos_semaphore_give_fromisr
** Input:pSemaphore pSem
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 11:54:06
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_semaphore_give_fromisr(pSemaphore pSem)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(pSem, &xHigherPriorityTaskWoken);
}

/*******************************************************************************
** Name: rkos_start
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 11:00:08
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_start(void)
{
    vTaskStartScheduler();
    while (1);
}

/*******************************************************************************
** Name: rkos_init
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:59:35
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_init(void)
{
    RKTaskListInitALL();
    SchedulerTaskCreate();

#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__
    //RKTaskCreate(TASK_ID_DEVICE_TASK,0, NULL);
#endif
}

/*******************************************************************************
** Name: rkos_memory_free
** Input:void * buf
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:51:43
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_memory_free(void * buf)
{
    vPortFree(buf);
}

/*******************************************************************************
** Name: rkos_memory_malloc
** Input:uint32 size
** Return: void *
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:38:53
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void * rkos_memory_malloc(uint32 size)
{
    uint8 * p;

    p = pvPortMalloc(size);

    if(p != NULL)
    {
        memset(p, 0, size);
        return p;
    }
    else
    {
        return NULL;
    }
}



/******************************************************************************
 * rkos_memcpy -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2015.4.30  written
 * Time : 15:18:06
 * --------------------
 ******************************************************************************/
API void rkos_memcpy(void* dst, void* src, uint32 sz)
{
    memcpy(dst, src, sz);
}

/******************************************************************************
 * rkos_memcmp -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2015.4.30  written
 * Time : 15:18:31
 * --------------------
 ******************************************************************************/
API int32 rkos_memcmp(void *dst, void *src, uint32 sz)
{
    //under spread sdk, the return value of
    //memcmp for two same mem. chunk is 0
    if (!memcmp(dst, src, sz))
        return TRUE;
    else
        return FALSE;
}

/******************************************************************************
 * rkos_memset -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2015.4.30  written
 * Time : 15:18:43
 * --------------------
 ******************************************************************************/
API void rkos_memset(void *pbuf, int32 c, uint32 sz)
{
    memset(pbuf, c, sz);
}


/*******************************************************************************
** Name: rkos_semaphore_delete
** Input:pSemaphore pSem
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:36:22
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_semaphore_delete(pSemaphore pSem)
{
    vSemaphoreDelete(pSem);
}

/*******************************************************************************
** Name: rkos_semaphore_create
** Input:uint32 MaxCnt, uint32 InitCnt
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:28:20
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API pSemaphore rkos_semaphore_create(uint32 MaxCnt, uint32 InitCnt)
{
    return xSemaphoreCreateCounting(MaxCnt,InitCnt);
}

/*******************************************************************************
** Name: rkos_semaphore_give
** Input:pSemaphore pSem
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:23:01
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_semaphore_give(pSemaphore pSem)
{
    xSemaphoreGive(pSem);
}

/*******************************************************************************
** Name: rkos_semaphore_take
** Input:pSemaphore pSem, uint32 time
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:18:46
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_semaphore_take(pSemaphore pSem, uint32 time)
{
    xSemaphoreTake(pSem, time);
}

/*******************************************************************************
** Name: rkos_exit_critical
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:12:07
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_exit_critical(void)
{
    portEXIT_CRITICAL();
}

/*******************************************************************************
** Name: rkos_enter_critical
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.10.15
** Time: 10:11:34
*******************************************************************************/
_OS_PLUGIN_RKOS_READ_
COMMON API void rkos_enter_critical(void)
{
    portENTER_CRITICAL();
}

API void rkos_sleep(uint32 ms)
{
    uint delay = ms/portTICK_RATE_MS;
    if (delay == 0)
    {
        delay = 1; /*the min time is 1 tick*/
    }
    vTaskDelay(delay);
}


API void rkos_delay(uint32 ms)
{
    uint delay = ms/portTICK_RATE_MS;
    if (delay == 0)
    {
        delay = 1; /*the min time is 1 tick*/
    }
    vTaskDelay(delay);
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
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

