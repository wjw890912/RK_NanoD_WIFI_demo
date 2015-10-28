/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                      (c) Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                                         All rights reserved.
*File    : TaskPlugin.C
* By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            DESCRIPTION
*  RK_NANO_OS is a system designed specifically for real-time embedded SOC operating system ,before using
*RK_NANO_OS sure you read the user's manual
*  The TASK NAME TABLE:
*
*
*  The DEVICE NAME TABLE:
*  "UartDevice",              Uart Serial communication devices
*  "ADCDevice",               The analog signal is converted to a digital signal device
*  "KeyDevice",               Key driver device
*
*
*
*
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
#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "AppInclude.h"
#include "GUITask.h"
#include "DeviceInclude.h"


/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/
#define MINIMAL_DATA_SIZE 32
#define MAXIMAL_DATA_SIZE 512

#define MINIMAL_STACK_SIZE 32
#define MAXIMAL_STACK_SIZE 512

//TaskPriority
//[real time task >=127]
#define TASK_PRIORITY_TIMER 128
#define TASK_PRIORITY_AUDIO_CONTROL 127

//[100 < system task <= 126]
//[0 < system task <= 10]
#define TASK_PRIORITY_SYSTICK 126
#define TASK_PRIORITY_TASK_MANAGER 125
#define TASK_PRIORITY_DEVICE_MANAGER 124
#define TASK_PRIORITY_IDLE   1
#define TASK_PRIORITY_ETH    2
#define TASK_PRIORITY_SHELL  3

//[70 < protocal task <= 100]
#define TASK_PRIORITY_STREAM_CONTROL 100
#define TASK_PRIORITY_TCP  99
#define TASK_PRIORITY_HTTP 101
#define TASK_PRIORITY_WIFI 99
#define TASK_PRIORITY_WIFI_APP 98



//[50 < device task <= 70]
#define TASK_PRIORITY_BCORE  70
#define TASK_PRIORITY_SDIO   70
#define TASK_PRIORITY_USBOTG 70
#define TASK_PRIORITY_USBHUB 69
#define TASK_PRIORITY_USBETH 69
#define TASK_PRIORITY_USBMSC 69
#define TASK_PRIORITY_GUI    70


//[30< app task <= 50]
#define TASK_PRIORITY_MEDIA 50
#define TASK_PRIORITY_FILE_STREAM 49

//[10 < ui task <= 30]
#define TASK_PRIORITY_MAINTASK 30




//will be repeat when the overlay is success
static RK_TASK_CLASS InitTaskItem[TASK_ID_NUM] =
{
    //add new task info here
    //......
#ifdef __WIFI_DLNA_C__
    {
        .TaskInitFun   = DLNATask_Init,
        .TaskDeInitFun = DLNATask_DeInit,
        .TaskFun       = DLNATask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,//MODULE_ID_DLNA_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = 40,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_DLNA,
    },
#endif

#ifdef __APP_MAIN_TASK_MAIN_TASK_C__
    {
        .TaskInitFun   = MainTask_Init,
        .TaskDeInitFun = MainTask_DeInit,
        .TaskFun       = MainTask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_MAIN_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_MAINTASK,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_MAIN,
    },
#endif

#ifdef __WICE_TCPIP_C__
    {
        .TaskInitFun   = tcpip_threadTask_Init,
        .TaskDeInitFun = tcpip_threadTask_DeInit,
        .TaskFun       = tcpip_threadTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_TCP,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_TCPIP_THREAD,
    },
#endif

#ifdef __WICE_WIFI_TRANS_C__
    {
        .TaskInitFun   = wwdTask_Init,
        .TaskDeInitFun = wwdTask_DeInit,
        .TaskFun       = wwdTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_WIFI,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_WWD,
    },
#endif

#ifdef __WICE_ETH_TRANS_C__
    {
        .TaskInitFun   = ethTask_Init,
        .TaskDeInitFun = ethTask_DeInit,
        .TaskFun       = ethTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_ETH,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_ETH,
    },
#endif


#ifdef __WICE_NETWORKING_C__
    {
        .TaskInitFun   = networkingTask_Init,
        .TaskDeInitFun = networkingTask_DeInit,
        .TaskFun       = networkingTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 56,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_NETWORKING,
    },
#endif

#ifdef __WICE_HARD_TO_WOKR_C__
    {
        .TaskInitFun   = hardware_to_workTask_Init,
        .TaskDeInitFun = hardware_to_workTask_DeInit,
        .TaskFun       = hardware_to_workTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 54,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE,
        .TaskClassID   = TASK_ID_HARDWARE_TO_WORK,
    },
#endif

#ifdef __WICE_WIFITHREAD_C__
    {
        .TaskInitFun   = wifi_applicationTask_Init,
        .TaskDeInitFun = wifi_applicationTask_DeInit,
        .TaskFun       = wifi_applicationTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_WIFI_APP,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_WIFI_APPLICATION,
    },
#endif

#ifdef __WICE_HTTP_C__
    {
        .TaskInitFun   = HTTPTask_Init,
        .TaskDeInitFun = HTTPTask_DeInit,
        .TaskFun       = HTTPTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_HTTP,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 4,
        .TaskClassID   = TASK_ID_HTTP,
    },
#endif


#ifdef __GUI_GUITASK_C__
    {
        //.TaskName      = {"GUITask"},
        .TaskInitFun   = GUITask_Init,
        .TaskDeInitFun = GUITask_DeInit,
        .TaskFun       = GUITask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_GUI_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_GUI,
        .TaskStackSize = MINIMAL_STACK_SIZE * 10,
        .TaskClassID   = TASK_ID_GUI,
    },
#endif

#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    {
        //.TaskName      = {"TimerTask"},
        .TaskInitFun   = TimerDev_Task_Init,
        .TaskDeInitFun = TimerDev_Task_DeInit,
        .TaskFun       = TimerDev_Task_Enter,
        .hTask   = NULL,
        .OverlayModule = MODULE_ID_TIMER_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_TIMER,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_TIMER,
    },
#endif


#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    {
        .TaskInitFun   = BcoreDev_Task_Init,
        .TaskDeInitFun = BcoreDev_Task_DeInit,
        .TaskFun       = BcoreDev_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_BCORE,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_BCORE,
    },
#endif

#ifdef __APP_FILESTREAM_STREAMCONTROLTASK_C__
    {
        .TaskInitFun   = StreamControlTask_Init,
        .TaskDeInitFun = StreamControlTask_DeInit,
        .TaskFun       = StreamControlTask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_STREAMCONTROL_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_STREAM_CONTROL,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_STREAMCONTROL,
    },
#endif

#ifdef __APP_AUDIO_AUDIOCONTROLTASK_C__
    {
//        .TaskName      = {"AudioControlTask"},
        .TaskInitFun   = AudioControlTask_Init,
        .TaskDeInitFun = AudioControlTask_DeInit,
        .TaskFun       = AudioControlTask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_AUDIOCONTROL_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_AUDIO_CONTROL,
        .TaskStackSize = MAXIMAL_STACK_SIZE * 4,
        .TaskClassID   = TASK_ID_AUDIOCONTROL,
    },
#endif

#ifdef __APP_AUDIO_AUDIOSUBTASK_C__
    {
        //.TaskName      = {"AudioSubTask"},
        .TaskInitFun   = AudioSubTask_Init,
        .TaskDeInitFun = AudioSubTask_DeInit,
        .TaskFun       = AudioSubTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 24,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_AUDIOSUB,
    },
#endif

#ifdef __APP_AUDIO_AUDIOMAINTASK_C__
    {
        //.TaskName      = {"AudioMainTask"},
        .TaskInitFun   = AudioMainTask_Init,
        .TaskDeInitFun = AudioMainTask_DeInit,
        .TaskFun       = AudioMainTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 24,
        .TaskStackSize = configSYS_SERVICE_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_AUDIOMAIN,
    },
#endif
#ifdef __DRIVER_USBHUB_USBHUBDEVICE_C__
    {
        //.TaskName      = {"USBMSCTask"},
        .TaskInitFun   = USBHUBDev_Task_Init,
        .TaskDeInitFun = USBHUBDev_Task_DeInit,
        .TaskFun       = USBHUBDev_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_USBHUB,
        .TaskStackSize = MINIMAL_STACK_SIZE * 15,
        .TaskClassID   = TASK_ID_USBHUB,
    },
#endif

#ifdef __DRIVER_USBETH_USBETHDEVICE_C__
    {
        //.TaskName      = {"USBMSCTask"},
        .TaskInitFun   = USBETHDev_Task_Init,
        .TaskDeInitFun = USBETHDev_Task_DeInit,
        .TaskFun       = USBETHDev_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_USBETH,
        .TaskStackSize = MINIMAL_STACK_SIZE * 15,
        .TaskClassID   = TASK_ID_USBETH,
    },
#endif


#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    {
        //.TaskName      = {"USBMSCTask"},
        .TaskInitFun   = USBMSCDev_Task_Init,
        .TaskDeInitFun = USBMSCDev_Task_DeInit,
        .TaskFun       = USBMSCDev_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_USBMSC,
        .TaskStackSize = MINIMAL_STACK_SIZE * 15,
        .TaskClassID   = TASK_ID_USBMSC,
    },
#endif

#ifdef __DRIVER_USB_USBOTGDEV_C__
    {
        .TaskInitFun   = UsbOtgDev0_Task_Init,
        .TaskDeInitFun = UsbOtgDev0_Task_DeInit,
        .TaskFun       = UsbOtgDev0_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_USBOTG,
        .TaskStackSize = MINIMAL_STACK_SIZE * 15,
        .TaskClassID   = TASK_ID_USBOTG0,
    },

    {
        .TaskInitFun   = UsbOtgHost_Task_Init,
        .TaskDeInitFun = UsbOtgHost_Task_DeInit,
        .TaskFun       = UsbOtgHost_Task_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_USBOTG,
        .TaskStackSize = MINIMAL_STACK_SIZE * 15,
        .TaskClassID   = TASK_ID_USBOTG1,
    },
#endif

#ifdef __WEB_TASK_WEBTRAN_C__
    {
        .TaskInitFun   = WenTranTask_Init,
        .TaskDeInitFun = WenTranTask_DevInit,
        .TaskFun       = WenTranTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 2,
        .TaskStackSize = MINIMAL_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_WEB_TASK,
    },
#endif

#ifdef __APP_MEDIA_MEDIATASK_C__
    {
        .TaskInitFun   = MediaTask_Init,
        .TaskDeInitFun = MediaTask_DevInit,
        .TaskFun       = MediaTask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_MEDIA_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_MEDIA,
        .TaskStackSize = MINIMAL_STACK_SIZE * 360,
        .TaskClassID   = TASK_ID_MEDIA,
    },
#endif

#ifdef __APP_FILESTREAM_FILESTREAMTASK_C__
    {
        .TaskInitFun   = FileStreamTask_Init,
        .TaskDeInitFun = FileStreamTask_DevInit,
        .TaskFun       = FileStreamTask_Enter,
        .hTask         = NULL,
        .OverlayModule = MODULE_ID_FILESTREAM_INIT,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_FILE_STREAM,
        .TaskStackSize = MINIMAL_STACK_SIZE * 360,
        .TaskClassID   = TASK_ID_FILESTREAM,
    },
#endif

#ifdef __APP_EXAMPLE_TASKFORMAT_C__
    {
        .TaskInitFun   = TestTask_Init,
        .TaskDeInitFun = TestTask_DevInit,
        .TaskFun       = TestTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 2,
        .TaskStackSize = MINIMAL_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_TEST,
    },
#endif

#ifdef _USE_SHELL_
    {
        .TaskInitFun   = ShellTaskInit,
        .TaskDeInitFun = ShellTaskDeInit,
        .TaskFun       = ShellTask,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_SHELL,
        .TaskStackSize = MAXIMAL_STACK_SIZE * 5,
        .TaskClassID   = TASK_ID_SHELL,
    },
#endif

#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__
    {
        .TaskInitFun   = DeviceTaskInit,
        .TaskDeInitFun = DeviceTaskDeInit,
        .TaskFun       = DeviceTask,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_DEVICE_MANAGER,
        .TaskStackSize = MAXIMAL_STACK_SIZE,
        .TaskClassID   = TASK_ID_DEVICE_TASK,
    },
#endif

#ifdef __WEB_TASK_WEBTASK_C__
    {
        .TaskInitFun   = WebTask_Init,
        .TaskDeInitFun = WebTask_DeInit,
        .TaskFun       = WebTask_Enter,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = 7,
        .TaskStackSize = MINIMAL_STACK_SIZE * 2,
        .TaskClassID   = TASK_ID_WEB_TASK,
    },
#endif

#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    {
        .TaskInitFun   = SdioIntIrqInit,
        .TaskDeInitFun = SdioIntIrqDeInit,
        .TaskFun       = SdioIrqTask,
        .hTask         = NULL,
        .OverlayModule = NULL,
        .TaskFlag      = NULL,
        .TaskPriority  = TASK_PRIORITY_SDIO,
        .TaskStackSize = 100*2 ,
        .TaskClassID   = TASK_ID_SDIO_IRQ_TASK,
    },
#endif

    {
        .TaskInitFun   = 0,
        .TaskDeInitFun = 0,
        .TaskFun       = 0,
        .hTask         = 0,
        .OverlayModule = 0,
        .TaskFlag      = 0,
        .TaskPriority  = 0,
        .TaskStackSize = 0 * 2,
        .TaskClassID   = 0,
    },

};


#define TASK_QUEUE_SIZE         sizeof(RK_TASK_QUEUE_ITEM)
#define TASK_QUEUE_LENTH        1

#define RK_TASK_EVENT_CREATE          1
#define RK_TASK_EVENT_DELETE          2
#define RK_TASK_EVENT_CREATE_SUCCESS  3
#define RK_TASK_EVENT_DELETE_SUCCESS  4
#define RK_TASK_EVENT_CREATE_FAIL     5
#define RK_TASK_EVENT_DELETE_FAIL     6

#define prvGetTCBFromHandle( pxHandle ) ( ( ( pxHandle ) == NULL ) ? ( tskTCB * ) pxCurrentTCB : ( tskTCB * ) ( pxHandle ) )

#define _OS_PLUGIN_TASKPLUGIN_READ_  __attribute__((section("os_plugin_taskplugin_read")))
#define _OS_PLUGIN_TASKPLUGIN_WRITE_ __attribute__((section("os_plugin_taskplugin_write")))
#define _OS_PLUGIN_TASKPLUGIN_INIT_  __attribute__((section("os_plugin_taskplugin_init")))
#define _OS_PLUGIN_TASKPLUGIN_SHELL_  __attribute__((section("os_plugin_taskplugin_shell")))

/*
*********************************************************************************************************
*                                        Variable Define
*********************************************************************************************************
*/
RK_TASK_CLASS * TaskListHead;
static pQueue  TaskAskQueue;
static pQueue  TaskRespQueue;

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void OSSchedulerTask(void * p);
rk_err_t RKTaskRegister(RK_TASK_CLASS * TaskHandler);
rk_err_t RKTaskUnRegister(RK_TASK_CLASS * RKTask);


/*
*********************************************************************************************************
*                                              RKTaskListInitALL()
*
* Description :  操作系统任务链表初始化
*
* Argument(s) : none
*
* Return(s)   : rk_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
_OS_PLUGIN_TASKPLUGIN_INIT_
rk_err_t RKTaskListInitALL(void)
{
    TaskAskQueue = rkos_queue_create(TASK_QUEUE_LENTH,TASK_QUEUE_SIZE);
    if(TaskAskQueue == NULL)
    {
        return RK_ERROR;
    }

    TaskRespQueue = rkos_queue_create(TASK_QUEUE_LENTH,TASK_QUEUE_SIZE);
    if(TaskRespQueue == NULL)
    {
        rkos_queue_delete(TaskAskQueue);
        return RK_ERROR;
    }
    return RK_SUCCESS;
}
/*
*********************************************************************************************************
*                                 RK_err_t RKTaskRegister(uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void))
*
* Description : 向操作系统注册设备.
*
* Argument(s) : uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void)
*
* Return(s)   : RK_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKGetTaskMSG(uint32 TaskClassID,RK_TASK_CLASS *pTaskItem)
{
    if (TaskClassID < TASK_ID_NUM)
    {
        memcpy(pTaskItem,&InitTaskItem[TaskClassID],sizeof(RK_TASK_CLASS));
        return RK_SUCCESS;
    }
    return RK_ERROR;

}

/*
*********************************************************************************************************
*                                     void OSSchedulerTask(void *pvParameters)
*
* Description:  This function is the Task to Start.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
unsigned char*  RKTaskStackAdd(HTC xTask)
{
    tskTCB *pxTCB;
    unsigned char* ucReturn;

    pxTCB = prvGetTCBFromHandle((xTaskHandle) xTask );

#if portSTACK_GROWTH < 0
    {
        ucReturn = ( unsigned char * ) pxTCB->pxStack;
    }
#else
    {
        ucReturn = ( unsigned char * ) pxTCB->pxEndOfStack;
    }
#endif

    return ucReturn;
}


TASK_STATE  RKTaskState(HTC xTask)
{
    return eTaskGetState(xTask);
}

uint32  RKTaskStackRemain(HTC xTask)
{
    return (uxTaskGetStackHighWaterMark(xTask) * sizeof(portSTACK_TYPE));
}

uint32  RKTaskStackSize(HTC xTask)
{
    RK_TASK_CLASS * p = (RK_TASK_CLASS * )xTask;

    return (p->TaskStackSize * 4);
}

uint32 RKTaskHeapFree()
{
    return xPortGetFreeHeapSize();
}

uint32 RKTaskHeapTotal()
{
    return configTOTAL_HEAP_SIZE;
}


/*
*********************************************************************************************************
*                                 RK_err_t RKTaskRegister(uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void))
*
* Description : 向操作系统注册设备.
*
* Argument(s) : uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void)
*
* Return(s)   : RK_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskRegister(RK_TASK_CLASS * TaskHandler)
{
    RK_TASK_CLASS * TempTaskHandler;
    if (TaskHandler == NULL)
        goto TaskRegisterErrExit;                   //任务句柄不等于NULL

    if (TaskListHead == NULL)
    {
        TaskListHead = TaskHandler;
        TaskListHead->NextTCB = NULL;
    }
    else
    {

        TempTaskHandler = TaskListHead;
        while (TempTaskHandler->NextTCB != NULL)
            TempTaskHandler = TempTaskHandler->NextTCB;
        TempTaskHandler->NextTCB = TaskHandler;
        TaskHandler->NextTCB = NULL;
    }
    return RK_SUCCESS;
TaskRegisterErrExit:
    return RK_ERROR;
}
/*
*********************************************************************************************************
*                                   RK_err_t RKTaskUnRegister(RKTaskHandler RKTask)
*
* Description:  将任务控制块从注册链表中删除.
*
* Argument(s) : none
*
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskUnRegister(RK_TASK_CLASS * TaskHandle)
{
    RK_TASK_CLASS * TempTaskHandle;

    if (TaskListHead == NULL)
        return RK_ERROR;

    TempTaskHandle = TaskListHead;

    if(TaskListHead == TaskHandle)
    {
        TaskListHead = NULL;
    }

    while (TempTaskHandle->NextTCB != NULL)
    {
        if (TempTaskHandle->NextTCB == TaskHandle)
            break;
        TempTaskHandle = TempTaskHandle->NextTCB;
    }

    if (TempTaskHandle->NextTCB != NULL)
    {
        TempTaskHandle->NextTCB = TaskHandle->NextTCB;
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}

/*
*********************************************************************************************************
*                                 RK_err_t RKTaskRegister(uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void))
*
* Description : 向操作系统注册设备.
*
* Argument(s) : uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void)
*
* Return(s)   : RK_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
RK_TASK_CLASS * RKTaskFind(uint32 TaskClassID, uint32 TaskObjectID)
{
    RK_TASK_CLASS * pTask;

    if (TaskListHead == NULL)
    {
        return NULL;
    }
    else
    {
        pTask = TaskListHead;
        while (pTask != NULL)
        {
            if ((pTask->TaskClassID == TaskClassID) && (pTask->TaskObjectID == TaskObjectID))
            {
                return pTask;
            }
            pTask = pTask->NextTCB;
        }
    }
    return NULL;
}


/*
*********************************************************************************************************
*                                              RKTaskListInitALL()
*
* Description :  操作系统任务链表初始化
*
* Argument(s) : none
*
* Return(s)   : RK_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
RK_TASK_CLASS * GetTaskList(void)
{
    return TaskListHead;
}

/*
*********************************************************************************************************
*                                     RK_err_t rkos_task_create(RKTaskHandler TaskHandler)
*
* Description : 创建任务，往队列发送任务.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskCreate(uint32 TaskClassID, uint32 TaskObjectID, void * arg)
{
    RK_TASK_QUEUE_ITEM TaskItem;
    RK_TASK_CLASS * NewTaskClassHandle = NULL;
    RK_TASK_ARG * NewTaskClassArg = NULL;


    if (TaskClassID > TASK_ID_NUM)
        goto ERR_EIXT;

    NewTaskClassHandle = (RK_TASK_CLASS *) rkos_memory_malloc(sizeof(RK_TASK_CLASS));


    if (NewTaskClassHandle == NULL)
        goto ERR_EIXT;

    RKGetTaskMSG(TaskClassID,NewTaskClassHandle);

    if (arg != NULL)
    {
        NewTaskClassArg = (RK_TASK_ARG *) rkos_memory_malloc(sizeof(RK_TASK_ARG));
        if (NewTaskClassArg == NULL)
            goto ERR_EIXT;
        MemCpy((uint8 *)NewTaskClassArg, (uint8 *)arg, sizeof(RK_TASK_ARG));
    }

    NewTaskClassHandle->TaskObjectID = TaskObjectID;

    TaskItem.TaskHandler = NewTaskClassHandle;
    TaskItem.RkTaskEvent = RK_TASK_EVENT_CREATE;
    TaskItem.TaskArg = NewTaskClassArg;

    rkos_queue_send(TaskAskQueue, &TaskItem, MAX_DELAY);

    rkos_queue_receive(TaskRespQueue, &TaskItem, MAX_DELAY);

    if(TaskItem.RkTaskEvent == RK_TASK_EVENT_CREATE_SUCCESS)
    {
        return RK_SUCCESS;
    }

ERR_EIXT:

    if (NewTaskClassHandle != NULL)
    {
        rkos_memory_free(NewTaskClassHandle);
    }

    if (NewTaskClassArg != NULL)
    {
        rkos_memory_free(NewTaskClassArg);
    }
    return RK_ERROR;
}


/*
*********************************************************************************************************
*                                     RK_err_t RKTaskCreateOperating(RKTaskHandler TaskHandler)
*
* Description : 创建任务，往队列发送任务.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskCreateOperating(RK_TASK_CLASS * TaskHandler, void * arg)
{
    if (TaskHandler->OverlayModule != NULL)
    {
        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_ModuleOverlay(TaskHandler->OverlayModule, MODULE_OVERLAY_ALL);
        #endif
    }

    if (TaskHandler->TaskInitFun(TaskHandler, arg) != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    rkos_memory_free(arg);

    if(TaskHandler->OverlayModule != NULL)
    {
        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_RemoveModule(TaskHandler->OverlayModule);
        #endif
    }
    char taskname[20];
    memset(taskname,0,20);
    taskname[0] = '0'+TaskHandler->TaskClassID/100;
    taskname[1] = '0'+(TaskHandler->TaskClassID%100)/10;
    taskname[2] = '0'+(TaskHandler->TaskClassID%10);
    TaskHandler->hTask = rkos_task_create(TaskHandler->TaskFun,taskname, NULL, TaskHandler->TaskStackSize, TaskHandler->TaskPriority, NULL);

    if (TaskHandler->hTask != NULL)
    {
        if (RKTaskRegister(TaskHandler) ==  RK_ERROR)
        {
            return RK_ERROR;
        }
        else
        {
            return RK_SUCCESS;
        }
    }
    else
    {
        rkos_memory_free(TaskHandler);
        return RK_ERROR;

    }
}

/*
*********************************************************************************************************
*                                     RK_err_t RKTaskDelete(RKTaskHandler TaskHandler)
*
* Description:  删除任务，往删除队列发送要被删除的任务句柄.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskDelete(uint32 TaskClassID, uint32 TaskObjectID, uint32 Mode)
{
    RK_TASK_QUEUE_ITEM TaskItem;

    TaskItem.TaskHandler = RKTaskFind(TaskClassID, TaskObjectID);
    if(TaskItem.TaskHandler == NULL)
    {
        return RK_ERROR;
    }

    TaskItem.RkTaskEvent = RK_TASK_EVENT_DELETE;
    TaskItem.Mode = Mode;

    rkos_queue_send(TaskAskQueue, &TaskItem, MAX_DELAY);


    if(Mode == SYNC_MODE)
    {
        rkos_queue_receive(TaskRespQueue, &TaskItem, MAX_DELAY);

        if(TaskItem.RkTaskEvent == RK_TASK_EVENT_DELETE_SUCCESS)
        {
            return RK_SUCCESS;
        }
        else
        {
            return RK_ERROR;
        }
    }
    return RK_SUCCESS;

}

/*
*********************************************************************************************************
*                                     RK_err_t RKTaskDeleteOperating(RKTaskHandler TaskHandler)
*
* Description:  删除任务，往删除队列发送要被删除的任务句柄.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKTaskDeleteOperating(RK_TASK_CLASS * TaskHandler)
{
    uint32 i;

    if (TaskHandler->OverlayModule != NULL)
    {
        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_ModuleOverlay(TaskHandler->OverlayModule, MODULE_OVERLAY_ALL);
        #endif
    }

    TaskHandler->TaskDeInitFun(NULL);

    if(TaskHandler->OverlayModule != NULL)
    {
        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_RemoveModule(TaskHandler->OverlayModule);
        #endif
    }

    rkos_task_delete(TaskHandler->hTask);                //删除任务
    RKTaskUnRegister(TaskHandler);
    rkos_memory_free(TaskHandler);

}


/*
*********************************************************************************************************
*                                     void SchedulerTaskCreate(void *pvParameters)
*
* Description:  This function is the Task to Start.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void SchedulerTaskCreate(void)
{
    rkos_task_create(OSSchedulerTask,NULL, NULL, 2048, TASK_PRIORITY_TASK_MANAGER, NULL);
}

/*
*********************************************************************************************************
*                                     void OSSchedulerTask(void *pvParameters)
*
* Description:  This function is the Task to Start.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void OSSchedulerTask(void * p)
{
    RK_TASK_QUEUE_ITEM TempTaskQueueItem;
    RK_TASK_CLASS  NewTaskClassHandle;

    RKGetTaskMSG(TASK_ID_DEVICE_TASK,&NewTaskClassHandle);
    RKTaskCreateOperating(&NewTaskClassHandle,NULL);

    while(1)
    {
        rkos_queue_receive(TaskAskQueue, &TempTaskQueueItem, MAX_DELAY);

        if (TempTaskQueueItem.RkTaskEvent == RK_TASK_EVENT_DELETE)
        {
            rk_printf("delete thread...");
            if(RKTaskDeleteOperating(TempTaskQueueItem.TaskHandler) == RK_SUCCESS)
            {
                TempTaskQueueItem.RkTaskEvent = RK_TASK_EVENT_DELETE_SUCCESS;
            }
            else
            {
                TempTaskQueueItem.RkTaskEvent = RK_TASK_EVENT_DELETE_FAIL;
            }

            if(TempTaskQueueItem.Mode == SYNC_MODE)
            {
                rkos_queue_send(TaskRespQueue, &TempTaskQueueItem, MAX_DELAY);
            }
        }
        else if (TempTaskQueueItem.RkTaskEvent == RK_TASK_EVENT_CREATE)
        {
            rk_printf("create thread...");

            if(RKTaskCreateOperating(TempTaskQueueItem.TaskHandler,  TempTaskQueueItem.TaskArg) == RK_SUCCESS)
            {
                TempTaskQueueItem.RkTaskEvent = RK_TASK_EVENT_CREATE_SUCCESS;
            }
            else
            {
                TempTaskQueueItem.RkTaskEvent = RK_TASK_EVENT_CREATE_FAIL;
            }

            rkos_queue_send(TaskRespQueue, &TempTaskQueueItem, MAX_DELAY);
        }

    }

}

/*
*********************************************************************************************************
*                                     void vApplicationStackOverflowHook(void *pvParameters)
*
* Description:  This function is the Task to Start.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void vApplicationStackOverflowHook(xTaskHandle xTask,
                                   signed char *pcTaskName)
{

    RK_TASK_CLASS * pTask =  TaskListHead;

    while (pTask != NULL)
    {
        if (pTask->hTask == (HTC)xTask)
        {
            printf("task ID = %d, stack overflow\n", pTask->TaskClassID);
        }

        pTask = pTask->NextTCB;
    }

    while(1);
}
