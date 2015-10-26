/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: System\Shell\ShellTaskCmd.c
* Owner: aaron.sun
* Date: 2015.7.24
* Time: 11:13:29
* Version: 1.0
* Desc: task shell cmd
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.7.24     11:13:29   1.0
********************************************************************************************
*/


#include "BspConfig.h"

#define __SYSTEM_SHELL_SHELLTASKCMD_C__
#ifdef __SYSTEM_SHELL_SHELLTASKCMD_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef SHELL_TASK_PAR
#include "RKOS.h"
#include "BSP.h"
#include "ShellTaskCmd.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static uint8* ShellTaskName[] =
{
    "list",
    "look",
    "help",
    "\b"
};

static uint8* ShellTaskState[] =
{
    "Runing   ",
    "Ready    ",
    "Blocked  ",
    "Suspended",
    "Deleted  ",
    "\b"
};


static uint8 * taskname[] =
{
    //add new task info here
#ifdef __WIFI_DLNA_C__
    "dlna",
#endif

#ifdef __APP_MAIN_TASK_MAIN_TASK_C__
    "Main",
#endif
    //......
#ifdef __WICE_TCPIP_C__
    "lwip",
#endif
#ifdef __WICE_WIFI_TRANS_C__
    "wwd",
#endif
#ifdef __WICE_NETWORKING_C__
    "net_work",
#endif
#ifdef __WICE_HARD_TO_WOKR_C__
    "hard_to_work",
#endif
#ifdef __WICE_WIFITHREAD_C__
    "wifi",
#endif
#ifdef __WICE_HTTP_C__
    "http",
#endif
#ifdef __GUI_GUITASK_C__
    "gui",
#endif
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    "timer",
#endif

#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    "bcore",
#endif
#ifdef __APP_FILESTREAM_STREAMCONTROLTASK_C__
    "stream_control",
#endif
#ifdef __APP_AUDIO_AUDIOCONTROLTASK_C__
    "audio_control",
#endif

#ifdef __APP_AUDIO_AUDIOSUBTASK_C__
    "audio_sub",
#endif
#ifdef __APP_AUDIO_AUDIOMAINTASK_C__
    "audio_main",
#endif
#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    "usb_msc",
#endif
#ifdef __DRIVER_USB_USBOTGDEV_C__
    "usb_otg0",
    "usb_otg1",
#endif
#ifdef __WEB_TASK_WEBTRAN_C__
    "web_trans",
#endif
#ifdef __APP_MEDIA_MEDIATASK_C__
    "media",
#endif
#ifdef __APP_FILESTREAM_FILESTREAMTASK_C__
    "file_stream",
#endif
#ifdef __APP_EXAMPLE_TASKFORMAT_C__
    "test",
#endif
#ifdef _USE_SHELL_
    "shell",
#endif
#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__
    "device_manager",
#endif
#ifdef __WEB_TASK_WEBTASK_C__
    "web_task",
#endif

#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    "sdio_irq",
#endif

    "\b"
};


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
rk_err_t ShellTaskLook(HDC dev, uint8 * pstr);
rk_err_t ShellHelpTaskParsing(HDC dev, uint8 * pstr);
rk_err_t ShellHelpTaskList(HDC dev, uint8 * pstr);
rk_err_t ShellTaskParsing(HDC dev, uint8 * pstr);
rk_err_t ShellTaskList(HDC dev, uint8 * pstr);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ShellTaskParsing
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.24
** Time: 11:17:38
*******************************************************************************/
_SYSTEM_SHELL_SHELLTASKCMD_COMMON_
COMMON API rk_err_t ShellTaskParsing(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellTaskName, pItem, StrCnt);
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
            ret = ShellTaskList(dev, pItem);
            break;

        case 0x01:
            ret = ShellTaskLook(dev, pItem);
            break;

        case 0x02:
#ifdef SHELL_HELP
            ret = ShellHelpTaskParsing(dev, pItem);
            break;
#endif

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ShellTaskLook
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.24
** Time: 11:28:28
*******************************************************************************/
_SYSTEM_SHELL_SHELLTASKCMD_COMMON_
COMMON FUN rk_err_t ShellTaskLook(HDC dev, uint8 * pstr)
{
    RK_TASK_CLASS * TempTaskHandler;

    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(taskname, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;


    TempTaskHandler = GetTaskList();

    if (TempTaskHandler == NULL)
    {
        rk_print_string("not find task!\r\n\r\n");
        return RK_SUCCESS;
    }


    while (TempTaskHandler != NULL)
    {
        if(TempTaskHandler->TaskClassID == i)
        {
            return RK_SUCCESS;
        }

        TempTaskHandler = TempTaskHandler->NextTCB;
    }


    rk_print_string("not find task!\r\n\r\n");
    return RK_SUCCESS;

}

#ifdef SHELL_HELP
/*******************************************************************************
** Name: ShellHelpTaskParsing
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.24
** Time: 11:20:25
*******************************************************************************/
_SYSTEM_SHELL_SHELLTASKCMD_COMMON_
COMMON FUN rk_err_t ShellHelpTaskParsing(HDC dev, uint8 * pstr)
{
    pstr--;

    if (StrLenA(pstr) != 0)
        return RK_ERROR;

    rk_print_string("task命令用于查看RKOS中所有已创TASK的信息\r\n");
    rk_print_string("task命令使用的格式:task.subcmd\r\n");
    rk_print_string("subcmd as follow:\r\n");
    rk_print_string("list  列出RKOS中所有需要被知道的TASK信息，RKOS自带TASK不被列出\r\n");
    rk_print_string("task.list\r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ShellHelpTaskList
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.24
** Time: 11:19:37
*******************************************************************************/
_SYSTEM_SHELL_SHELLTASKCMD_COMMON_
COMMON FUN rk_err_t ShellHelpTaskList(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    pstr--;
    if (StrLenA(pstr) == 0)
    {
        rk_print_string("task.list用于列出已知任务的信息，输出格式如下:\r\n");
        rk_print_string("State\t\tStackSize\tRemainStack\tStackBaseAdd\tPriority\tName\r\n");
        rk_print_string("-----------------------------------------------------------------------------------------\r\n");
        rk_print_string("Runing   \t0x00000000\t0x00000000\t0x00000000\t0x00000000\tName1\r\n");
        rk_print_string("Ready    \t0x00000000\t0x00000000\t0x00000000\t0x00000000\tName2\r\n");
        rk_print_string("Blocked  \t0x00000000\t0x00000000\t0x00000000\t0x00000000\tName3\r\n");
        rk_print_string("Suspended\t0x00000000\t0x00000000\t0x00000000\t0x00000000\tName4\r\n");
        rk_print_string("Deleted  \t0x00000000\t0x00000000\t0x00000000\t0x00000000\tName5\r\n");
    }
    else
    {
        rk_print_string("task.list没有参数或者子命令\r\n");
    }

    return RK_SUCCESS;
}

#endif

/*******************************************************************************
** Name: ShellTaskList
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.24
** Time: 11:15:26
*******************************************************************************/
_SYSTEM_SHELL_SHELLTASKCMD_COMMON_
COMMON FUN rk_err_t ShellTaskList(HDC dev, uint8 * pstr)
{
    RK_TASK_CLASS * TempTaskHandler;
    uint8  TxBuffer[128];
    uint8  TaskState;
    uint32 TaskRemainStack;
    uint32 StackAdd;
    uint16 BuffSize = 0;

    pstr--;

    if (dev == NULL)
    {
        return RK_SUCCESS;
    }

    if (StrLenA(pstr) == 0)
    {
        TempTaskHandler = GetTaskList();

        rk_print_string("\n    this cmd will list all task info!!\r\n\r\n");

        if (TempTaskHandler == NULL)
        {
            rk_print_string("not find task!\r\n\r\n");
            return RK_SUCCESS;
        }

        BuffSize = sprintf(TxBuffer,"State\t\tStackSize\tRemainStack\tStackBaseAdd\tPriority\tName\r\n");
        UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);
        BuffSize = sprintf(TxBuffer,"-----------------------------------------------------------------------------------------\r\n");
        UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

        while (TempTaskHandler != NULL)
        {

            TaskState = RKTaskState(TempTaskHandler->hTask);
            TaskRemainStack = RKTaskStackRemain(TempTaskHandler->hTask);
            StackAdd =  (uint32)RKTaskStackAdd(TempTaskHandler->hTask);


            BuffSize = sprintf(TxBuffer,"%s\t",ShellTaskState[TaskState]);
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

            BuffSize = sprintf(TxBuffer,"0x%08x\t",RKTaskStackSize((HTC)TempTaskHandler));
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

            BuffSize = sprintf(TxBuffer,"0x%08x\t",TaskRemainStack);
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

            BuffSize = sprintf(TxBuffer,"0x%08x\t",StackAdd);
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

            BuffSize = sprintf(TxBuffer,"0x%08x\t",TempTaskHandler->TaskPriority);
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);


            BuffSize = sprintf(TxBuffer,"%s\r\n",taskname[TempTaskHandler->TaskClassID]);
            UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

            TaskState = 0;
            TaskRemainStack = 0;
            StackAdd = 0;

            TempTaskHandler = TempTaskHandler->NextTCB;
        }

        return RK_SUCCESS;

    }
    else if (pstr[0] == ' ')
    {
        //list have parameter
        return RK_ERROR;
    }

#ifdef SHELL_HELP
    else if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;

        if (StrCmpA(pstr, "help", 4) == 0)
        {
            return(ShellHelpTaskList(dev,pstr + 5));
        }
        return RK_ERROR;
    }
#endif

    return RK_ERROR;
}



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
#endif
