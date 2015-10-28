/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                        Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                                        All rights reserved.
*File    : TaskPlugin.h
* By      : Zhu Zhe
* Version : V0.x
*
*********************************************************************************************************
*/


#ifndef _TASKPLUGIN_H_
#define _TASKPLUGIN_H_
/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/
typedef enum _TASK_ID
{
    //add new task info here
    //......
#ifdef __WIFI_DLNA_C__
    TASK_ID_DLNA,
#endif
#ifdef __APP_MAIN_TASK_MAIN_TASK_C__
    TASK_ID_MAIN,
#endif
#ifdef __WICE_TCPIP_C__
    TASK_ID_TCPIP_THREAD,
#endif
#ifdef __WICE_WIFI_TRANS_C__
    TASK_ID_WWD,
#endif
#ifdef __WICE_ETH_TRANS_C__
    TASK_ID_ETH,
#endif

#ifdef __WICE_NETWORKING_C__
    TASK_ID_NETWORKING,
#endif
#ifdef __WICE_HARD_TO_WOKR_C__
    TASK_ID_HARDWARE_TO_WORK,
#endif
#ifdef __WICE_WIFITHREAD_C__
    TASK_ID_WIFI_APPLICATION,
#endif
#ifdef __WICE_HTTP_C__
    TASK_ID_HTTP,
#endif
#ifdef __GUI_GUITASK_C__
    TASK_ID_GUI,
#endif
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    TASK_ID_TIMER,
#endif

#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    TASK_ID_BCORE,
#endif
#ifdef __APP_FILESTREAM_STREAMCONTROLTASK_C__
    TASK_ID_STREAMCONTROL,
#endif
#ifdef __APP_AUDIO_AUDIOCONTROLTASK_C__
    TASK_ID_AUDIOCONTROL,
#endif

#ifdef __APP_AUDIO_AUDIOSUBTASK_C__
    TASK_ID_AUDIOSUB,
#endif
#ifdef __APP_AUDIO_AUDIOMAINTASK_C__
    TASK_ID_AUDIOMAIN,
#endif
#ifdef __DRIVER_USBHUB_USBHUBDEVICE_C__
    TASK_ID_USBHUB,
#endif
#ifdef __DRIVER_USBETH_USBETHDEVICE_C__
    TASK_ID_USBETH,
#endif
#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    TASK_ID_USBMSC,
#endif
#ifdef __DRIVER_USB_USBOTGDEV_C__
    TASK_ID_USBOTG0,
    TASK_ID_USBOTG1,
#endif
#ifdef __APP_MEDIA_MEDIATASK_C__
    TASK_ID_MEDIA,
#endif
#ifdef __APP_FILESTREAM_FILESTREAMTASK_C__
    TASK_ID_FILESTREAM,
#endif
#ifdef __APP_EXAMPLE_TASKFORMAT_C__
    TASK_ID_TEST,
#endif
#ifdef _USE_SHELL_
    TASK_ID_SHELL,
#endif
#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__
    TASK_ID_DEVICE_TASK,
#endif
#ifdef __WEB_TASK_WEBTASK_C__
    TASK_ID_WEB_TASK,
#endif

#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    TASK_ID_SDIO_IRQ_TASK,
#endif

    TASK_ID_TEMP,
    TASK_ID_NUM

}TASK_ID;

typedef enum _TASK_STATE
{
    TASK_STATE_RUNING,
    TASK_STATE_READY,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPEND,
    TASK_STATE_DELETED,
    TASK_STATE_NUM

}TASK_STATE;



/*
--------------------------------------------------------------------------------

                        Struct Define

--------------------------------------------------------------------------------
*/
typedef rk_err_t (*pTaskInitFunType)(void * , void *);
typedef rk_err_t (*pTaskDeInitFunType)(void * );
typedef void (*pTaskFunType)(void *);
typedef void * HTC;


/* 任务控制块结构 */
typedef struct _RK_TASK_CLASS
{
    struct _RK_TASK_CLASS * NextTCB;
    pTaskInitFunType     TaskInitFun;
    pTaskFunType         TaskFun;
    pTaskDeInitFunType   TaskDeInitFun;
    HTC        hTask;/*任务传参回传的任务句柄*/
    uint8  TaskFlag;               /*认为状态信息标志位*/
    uint32 OverlayModule;          /*Overlay模块编号*/
    uint32 TaskPriority;           /*任务优先级*/
    uint32 TaskStackSize;          /*系统保存恢复堆栈大小*/
    uint32 TaskClassID;
    uint32 TaskObjectID;

}RK_TASK_CLASS;


typedef struct _RK_TASK_MEDIA_ARG
{
    uint32 file;

}RK_TASK_MEDIA_ARG;

typedef struct _RK_TASK_AUDIOCONTROL_ARG
{
    uint16 FileNum;
    uint8  filepath[255];
    uint32 ucSelPlayType;

}RK_TASK_AUDIOCONTROL_ARG;




/* 任务控制块结构 */
typedef union _RK_TASK_ARG
{
    RK_TASK_MEDIA_ARG stTaskMediaArg;
    RK_TASK_AUDIOCONTROL_ARG stTaskAudioControl;

}RK_TASK_ARG;


/* 任务队列元素结构 */
typedef struct _RK_TASK_QUEUE_ITEM
{
    RK_TASK_CLASS * TaskHandler;
    RK_TASK_ARG * TaskArg;
    uint32         RkTaskEvent;
    uint32  Mode;

}RK_TASK_QUEUE_ITEM;


/*
--------------------------------------------------------------------------------

                        Variable Define

--------------------------------------------------------------------------------
*/


/*
--------------------------------------------------------------------------------

                        Funtion Declaration

--------------------------------------------------------------------------------
*/
rk_err_t RKTaskListInitALL(void);
rk_err_t RKTaskRegisterALL(void);

rk_err_t RKTaskCreate(uint32 TaskClassID, uint32 TaskObjectID, void * arg);
rk_err_t RKTaskDelete(uint32 TaskClassID, uint32 TaskObjectID, uint32 Mode);

RK_TASK_CLASS * RKTaskFind(uint32 TaskClassID, uint32 TaskObjectID);


void SchedulerTaskCreate(void);
void RKTaskInitCreate(void);
RK_TASK_CLASS * GetTaskList(void);
unsigned char*  RKTaskStackAdd(HTC xTask);
uint32  RKTaskStackRemain(HTC xTask);

uint32  RKTaskStackSize(HTC xTask);
uint32 RKTaskHeapFree(void);
uint32 RKTaskHeapTotal(void);

#endif

