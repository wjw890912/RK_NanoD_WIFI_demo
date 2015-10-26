/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                                    (c) Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                                         All rights reserved.
*File    : NanoShell.C
*By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            DESCRIPTION
*    RK_NANO_OS is a system designed specifically for real-time embedded SOC operating system ,before using
*RK_NANO_OS sure you read the user's manual
*    This file is to provide Debug shell
*    The FreeRTOS provide these macro to trace:
*    宏定义                                                 说明
*    traceTASK_INCREMENT_TICK()                        在节拍中断调用。
*    traceTASK_SWITCHED_OUT()                        在选择新任务运行前调用。这时 pxCurrentTCB 包含了将脱离运行状态任务的句柄。
*    traceTASK_SWITCHED_IN()                        在选择运行任务后运行。这时 pxCurrentTCB 包含了即将进入运行状态任务的局部。
*    traceBLOCKING_ON_QUEUE_RECEIVE(pxQueue)        代表当前运行任务被阻塞因为试图读取空队列，或者试图'获取'空信号或互斥。
*    traceBLOCKING_ON_QUEUE_SEND(pxQueue)            代表当前任务被阻塞因为试图写入一个已经满的队列。
*    traceGIVE_MUTEX_RECURSIVE(pxMutex)                如果队列创建成功在 xSemaphoreGiveRecursive() 中调用。
*    traceQUEUE_CREATE()                            在 xQueueCreate() 中调用。
*    traceQUEUE_CREATE_FAILED()                        如果队列因为堆内存不足而创建不成功在 xQueueCreate() 中调用。
*    traceCREATE_MUTEX()                            如果互斥创建成功在 xSemaphoreCreateMutex() 中调用。
*    traceCREATE_MUTEX_FAILED()                        如果互斥因为堆内存不足而创建不成功在 xSemaphoreCreateMutex() 中调用。
*    traceGIVE_MUTEX_RECURSIVE(pxMutex)                如果'获取'互斥成功在 xSemaphoreGiveRecursive() 中调用。
*    traceGIVE_MUTEX_RECURSIVE_FAILED(pxMutex)        如果调用者不是互斥所有者在 xSemaphoreGiveRecursive() 中调用。
*    traceTAKE_MUTEX_RECURSIVE(pxMutex)                在 xQueueTakeMutexRecursive() 中调用。
*    traceCREATE_COUNTING_SEMAPHORE()                如果信号灯创建成功在 xSemaphoreCreateCounting() 中调用。
*    traceCREATE_COUNTING_SEMAPHORE_FAILED()        如果信号灯因为堆内存不足创建不成功时在 xSemaphoreCreateCounting() 中调用。
*    traceQUEUE_SEND(pxQueue)                        当队列发送成功时，在 xQueueSend(), xQueueSendToFront(), xQueueSendToBack(), 或任何信号灯 '给出'函数中调用。
*    traceQUEUE_SEND_FAILED(pxQueue)                当因为队列满(在达到指定阻塞时间)造成发送失败后在 xQueueSend(), xQueueSendToFront(), xQueueSendToBack(), 或其他信号灯 '给出' 函数中调用。
*    traceQUEUE_RECEIVE(pxQueue)                    当队列接收成功后在 xQueueReceive() 或其他信号灯 '获取'函数中调用。
*    traceQUEUE_RECEIVE_FAILED()                    在因为队列空(在达到指定阻塞时间后)造成接收失败后，在 xQueueReceive() 或其他信号灯 '获取' 函数中调用。
*    traceQUEUE_PEEK()                                在 xQueuePeek() 中调用。
*    traceQUEUE_SEND_FROM_ISR(pxQueue)                当发送成功时在 xQueueSendFromISR() 中调用。
*    traceQUEUE_SEND_FROM_ISR_FAILED(pxQueue)        当因为队列满造成发送失败时在 xQueueSendFromISR() 中调用。
*    traceQUEUE_RECEIVE_FROM_ISR(pxQueue)            当接收成功时在 xQueueReceiveFromISR() 中调用。
*    traceQUEUE_RECEIVE_FROM_ISR_FAILED(pxQueue)    当因为队列空造成接收失败时在 xQueueReceiveFromISR() 中调用。
*    traceQUEUE_DELETE(pxQueue)                        在 vQueueDelete() 中调用。
*    traceTASK_CREATE(pxTask)                        当任务创建成功时在 xTaskCreate() 中调用。
*    traceTASK_CREATE_FAILED()                        当任务因为堆空间不足创建失败时在 xTaskCreate() 中调用。
*    traceTASK_DELETE(pxTask)                        在 vTaskDelete() 中调用。
*    traceTASK_DELAY_UNTIL()                        在 vTaskDelayUntil() 中调用。
*    traceTASK_DELAY()                                在 vTaskDelay() 中调用。
*    traceTASK_PRIORITY_SET(pxTask,uxNewPriority)    在 vTaskPrioritySet() 中调用。
*    traceTASK_SUSPEND(pxTask)                        在 vTaskSuspend() 中调用。
*    traceTASK_RESUME(pxTask)                        在 vTaskResume() 中调用。
*    traceTASK_RESUME_FROM_ISR(pxTask)                在 xTaskResumeFromISR() 中调用。
*
*
*
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "BspConfig.h"

#ifdef SHELL_DEV_PAR

#include "RKOS.h"
#include "BSP.h"

static char* ShellDeviceName[] =
{
    "list",
    "create",
    "\b"
};

static char* ShellDeviceList[] =
{
    "emmccode",
    "emmcdata",
    "emmcuser1",
    "sdiodevlist",
    "\b"
};

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellDevCreate(HDC dev, char * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellDeviceList, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;

    switch (i)
    {
        case 0x00:
#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__
            ret = DeviceTask_CreateDeviceList(0);
#endif
            if (ret != RK_SUCCESS)
            {
                UartDev_Write(dev,"emmc code create fail",21,SYNC_MODE,NULL);
                ret = RK_SUCCESS;
            }
            break;

        case 0x01:
            //create
            break;

        case 0x02:
            //connect
            break;

        case 0x03:
#ifdef __DRIVER_SDIO_SDIODEVICE_C__
            ret = DeviceTask_CreateDeviceList(DEVICE_LIST_SDIO_FIFO);
#endif
            if (ret != RK_SUCCESS)
            {
                UartDev_Write(dev,"sdc code create fail",20,SYNC_MODE,NULL);
                ret = RK_SUCCESS;
            }
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}



/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellDeviceParsing(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;


    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellDeviceName, pItem, StrCnt);

    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;

    switch (i)
    {
        case 0x00:
            //list
            break;

        case 0x01:
            //create
            ret = ShellDevCreate(dev, pItem);
            break;

        case 0x02:
            //remove
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}
#endif

