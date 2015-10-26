/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Gui\GUITask.c
* Owner: zhuzhe
* Date: 2014.5.30
* Time: 17:14:35
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.5.30     17:14:35   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __GUI_GUITASK_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "GUITask.h"
#include "KeyDevice.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define GUI_CMD_NONE                 0x00
#define GUI_CMD_KEYVAL               0x01
#define GUI_CMD_LABELUPDATE          0x02
#define GUI_CMD_BUTTONUPDATE         0x03
#define GUI_CMD_TXTUPDATE            0x04
#define GUI_CMD_DREIVERCONFIG        0x05
#define GUI_CMD_REGISTER             0x06

#define GUI_EVENT_QUEUE_LENTH         1 //8
#define GUI_EVENT_QUEUE_SIZE          sizeof(GUI_EVENT_CLASS)

typedef enum CUI_LABEL_CMD
{
    GUI_LABEL_NONE,
};
typedef enum CUI_BUTTON_CMD
{
    GUI_BUTTON_NONE,
};
typedef enum CUI_TXT_CMD
{
    GUI_TXT_NONE,
};
typedef enum CUI_DREIVERCONFIG_CMD
{
    GUI_DREIVERCONFIG_NONE,
};


typedef  struct _GUI_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}GUI_RESP_QUEUE;

typedef  struct _GUI_ASK_QUEUE
{
    uint32 cmd;

}GUI_ASK_QUEUE;

typedef  struct _GUI_EVENT_CLASS
{
    uint32 cmd;
    uint32 para;
    void*  buf;

}GUI_EVENT_CLASS;

typedef  struct _GUI_TASK_CTRL_BLOCK
{
    uint8* pData;
    uint8* pDataEnd;
    pQueue  GUIEventQueue;

}GUI_TASK_CTRL_BLOCK;

/*
typedef  struct _GUI_TASK_DATA_BLOCK
{
    xQueueHandle  GUIAskQueue;
    xQueueHandle  GUIRespQueue;

}GUI_TASK_DATA_BLOCK;
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//static GUI_TASK_CTRL_BLOCK  gpstGUITaskControlBlock;
static GUI_TASK_CTRL_BLOCK * gpstGUICtrlBlock; //= &gpstGUITaskControlBlock

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
rk_err_t GUITaskShellTest(HDC dev, uint8 * pstr);
rk_err_t GUITaskShellHelp(HDC dev, uint8 * pstr);
rk_err_t GUITaskShellDel(HDC dev, uint8 * pstr);
rk_err_t GUITaskShellMc(HDC dev, uint8 * pstr);
rk_err_t GUITaskShellPcb(HDC dev, uint8 * pstr);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: GUITask_Connect
** Input:viod
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.25
** Time: 10:46:59
*******************************************************************************/
/*
_GUI_GUITASK_COMMON_
COMMON API rk_err_t GUITask_Connect(HDC dev, uint32 tmerNum, uint32 TimerClk, uint64 usTick)
{
    rk_err_t ret;
    printf ("GUITask_Connect \n");
    RKTaskCreate(TASK_ID_GUI, 0, NULL);

    ret = TimerDev_Connect(dev, tmerNum, TimerClk, usTick);
    if (ret != RK_SUCCESS)
    {
        printf ("TIMER0 connect faile\n");
        return RK_ERROR;
    }
    return RK_SUCCESS;
}
*/
/*******************************************************************************
** Name: GUIKeyCallBack
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.9
** Time: 9:20:38
*******************************************************************************/
_GUI_GUITASK_COMMON_
COMMON API void GUIKeyCallBack(uint32 para)
{

    GUI_TASK_CTRL_BLOCK*  pGUITaskControlBlock = gpstGUICtrlBlock;
    GUI_EVENT_CLASS TempEvent;
    //printf("para = %d\n", para);
    TempEvent.cmd = GUI_CMD_KEYVAL;
    TempEvent.para = para; //KEY_VAL_PLAY_SHORT_UP;

    rkos_queue_send_fromisr( pGUITaskControlBlock->GUIEventQueue,&TempEvent);

}
/*******************************************************************************
** Name: GUITask_Enter
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.30
** Time: 17:15:15
*******************************************************************************/
_GUI_GUITASK_COMMON_
COMMON API void GUITask_Enter(void)
{
    GUI_TASK_CTRL_BLOCK*  pGUITaskControlBlock = gpstGUICtrlBlock;
    GUI_EVENT_CLASS TempEvent;
    //printf("^^^^^^GUITask_Enter^^^^^");
//    KeyDevRead(pGUITaskControlBlock->KeyHandle,&(pGUITaskControlBlock->KeyVal), GUIKeyCallBack);

    while (1)
    {
        rkos_queue_receive(pGUITaskControlBlock->GUIEventQueue,&TempEvent,MAX_DELAY);
        switch (TempEvent.cmd)
        {
            case GUI_CMD_KEYVAL:
            {
                switch (TempEvent.para)
                {
                    case KEY_VAL_HOLD_PRESS_START:
                        printf("!^^MENU+PLAY LONG PRESS START^^\n");
                        break;
                    case KEY_VAL_HOLD_SHORT_UP:
                        printf("!^^MENU+PLAY SHORT UP^^\n");
                        break;
                    case KEY_VAL_HOLD_LONG_UP:
                        printf("!^^MENU+PLAY LONG UP^^\n");
                        break;
                    case KEY_VAL_PLAY_LONG_UP:
                        printf("!^^^^^^PLAY LONG UP^^^^^\n");
                        break;
                    case KEY_VAL_PLAY_SHORT_UP:
                        printf("!^^^^^^PLAY SHORT UP^^^^^\n");
                        break;
                    case KEY_VAL_UP_SHORT_UP:
                        printf("!^^^^^^UP SHORT UP^^^^^\n");
                        break;
                    case KEY_VAL_DOWN_SHORT_UP:
                        printf("!DOWN SHORT UP\n");
                        break;
                    case KEY_VAL_ESC_SHORT_UP:
                        printf("!ESC SHORT UP\n");
                        break;
                    case KEY_VAL_MENU_SHORT_UP:
                        printf("!MENU SHORT UP\n");
                        break;
                    case KEY_VAL_FFW_SHORT_UP:
                        printf("!FFW SHORT UP\n");
                        break;
                    case KEY_VAL_FFD_SHORT_UP:
                        printf("!FFD SHORT UP\n");
                        break;
                    case KEY_VAL_UP_LONG_UP:
                        rk_printf("!UP LONG UP\n");
                        break;
                    case KEY_VAL_DOWN_LONG_UP:
                        rk_printf("!DOWN LONG UP\n");
                        break;
                    case KEY_VAL_ESC_LONG_UP:
                        rk_printf("!ESC LONG UP\n");
                        break;
                    case KEY_VAL_MENU_LONG_UP:
                        rk_printf("!MENU LONG UP\n");
                        break;
                    case KEY_VAL_FFW_LONG_UP:
                        rk_printf("!FFW LONG UP\n");
                        break;
                    case KEY_VAL_FFD_LONG_UP:
                        rk_printf("!FFD LONG UP\n");
                        break;
                    case KEY_VAL_PLAY_PRESS_START:
                        rk_printf("!PLAY LONG PRESS START\n");
                        break;
                    case KEY_VAL_UP_PRESS_START:
                        rk_printf("!UP LONG PRESS START\n");
                        break;
                    case KEY_VAL_DOWN_PRESS_START:
                        rk_printf("!DOWN LONG PRESS START\n");
                        break;
                    case KEY_VAL_ESC_PRESS_START:
                        rk_printf("!ESC LONG PRESS START\n");
                        break;
                    case KEY_VAL_MENU_PRESS_START:
                        rk_printf("!MENU LONG PRESS START\n");
                        break;
                    case KEY_VAL_FFW_PRESS_START:
                        rk_printf("!FFW LONG PRESS START\n");
                        break;
                    case KEY_VAL_FFD_PRESS_START:
                        rk_printf("!FFD LONG PRESS START\n");
                        break;

                        // other key val will be writen later
                    default :
                        break;
                }
                break;
            }
            case GUI_CMD_LABELUPDATE:
            {
                break;
            }
            case GUI_CMD_DREIVERCONFIG:
            {
                break;
            }
            default :
                break;
        }
    }
}



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
** Name: GUITask_DevInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.30
** Time: 17:15:15
*******************************************************************************/
_GUI_GUITASK_INIT_
INIT API rk_err_t GUITask_DeInit(void *pvParameters)
{
    RK_TASK_CLASS* TaskHandler = (RK_TASK_CLASS*)pvParameters;
     printf("$$$$GUITask_DeInit$$$$\r\n");
    rkos_enter_critical();

    rkos_queue_delete(gpstGUICtrlBlock->GUIEventQueue);
    rkos_memory_free(gpstGUICtrlBlock);

    rkos_exit_critical();
    return RK_SUCCESS;

exit:
    rkos_exit_critical();
    return RK_ERROR;

}
/*******************************************************************************
** Name: GUITask_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.30
** Time: 17:15:15
*******************************************************************************/
_GUI_GUITASK_INIT_
INIT API rk_err_t GUITask_Init(void *pvParameters, void *arg)
{

    RK_TASK_CLASS*   pGUITask = (RK_TASK_CLASS*)pvParameters;
    GUI_TASK_CTRL_BLOCK*  pGUITaskControlBlock;
    printf("$$$$GUITask_Init$$$$\r\n");
    if (pGUITask == NULL)
    {
        printf("$$$$pGUITask NULL$$$$\r\n");
        return RK_PARA_ERR;
    }
    pGUITaskControlBlock = (GUI_TASK_CTRL_BLOCK*) rkos_memory_malloc(sizeof(GUI_TASK_CTRL_BLOCK));
    if (pGUITaskControlBlock == NULL)
    {
        printf("$$$$GUITask_Create rkos_memory_malloc faile$$$$\r\n");
        return RK_ERROR;
    }
    memset(pGUITaskControlBlock, NULL, sizeof(GUI_TASK_CTRL_BLOCK));

    gpstGUICtrlBlock = pGUITaskControlBlock;
    gpstGUICtrlBlock->GUIEventQueue = rkos_queue_create(GUI_EVENT_QUEUE_LENTH,GUI_EVENT_QUEUE_SIZE);

    printf("queue address:%x\n", gpstGUICtrlBlock->GUIEventQueue);

    if (gpstGUICtrlBlock->GUIEventQueue == 0)
    {
        printf("queue CREATE FRIL\n");
    }
    else
    {
        printf("queue CREATE SUCCESS\n");
    }

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_GUI_TASK, MODULE_OVERLAY_ALL);
#endif

    return RK_SUCCESS;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#endif

