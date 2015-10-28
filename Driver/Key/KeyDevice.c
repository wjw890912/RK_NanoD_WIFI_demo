/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\Key\KeyDevice.c
* Owner: aaron.sun
* Date: 2015.8.20
* Time: 14:10:42
* Version: 1.0
* Desc: Key Device
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.8.20     14:10:42   1.0
********************************************************************************************
*/
#include <BspConfig.h>
#ifdef __DRIVER_KEY_KEYDEVICE_C__
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "GUITask.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define KEY_TIME_DOWN               2    //按键按下
#define KEY_TIME_PRESS_START        20   //开始判定为长按
#define KEY_TIME_PRESS_STEP         4    //长按一步的时间范围

typedef struct KeyDeviceClass
{
    DEVICE_CLASS KeyDevice;            //device father class

    pSemaphore KeyReadSem;
    HDC        ADCHandler;
    pTimer     TimerHandler;
    uint8      StartFlag;
    uint32*    buf;

    uint16           ADCVal;
    uint32           KeyCnt;
    uint32           KeyBackup;
    void             (*KeyCallBack)(uint32);
}KeyDeviceClass;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static KeyDeviceClass* KeyISRDevHandle;


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
void KeyDevIsr(void);
rk_err_t KeyDevShellCreate(HDC dev, uint8 * pstr);
void KeyScanCallBack(HDC dev);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*
*********************************************************************************************************
*                                     KeyDevRead  (void *p_arg)
*
* Description:  This function is the Task to Start.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none
*********************************************************************************************************
*/
_DRIVER_KEY_KEYDEVICE_COMMON_
rk_size_t KeyDevRead(DEVICE_CLASS* dev,uint32* buffer,pFunc Rx_indicate)
{
    KeyDeviceClass * KeyDevHandle;
    KeyDevHandle = (KeyDeviceClass *)dev;
    rkos_semaphore_take(KeyDevHandle->KeyReadSem, MAX_DELAY);

    rkos_enter_critical();
    KeyDevHandle->StartFlag = 1;
    KeyDevHandle->buf = buffer;
    KeyDevHandle->KeyCallBack = Rx_indicate;
    rkos_exit_critical();

    ADCDev_Read(KeyDevHandle->ADCHandler,ADC_CHANEL_KEY, 1, 1, (uint16 *)(&KeyDevHandle->ADCVal),KeyScanCallBack);
    //printf("key%d adc value=%d\n",ADC_CHANEL_KEY,KeyDevHandle->ADCVal);
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerDev_Stop
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.21
** Time: 11:23:17
*******************************************************************************/
_DRIVER_KEY_KEYDEVICE_COMMON_
rk_err_t KeyDevStop(HDC dev)
{
    KeyDeviceClass * KeyDevHandle;
    KeyDevHandle = (KeyDeviceClass *)dev;

    KeyDevHandle->StartFlag = NULL;
    rkos_semaphore_give(KeyDevHandle->KeyReadSem);
    return RK_SUCCESS;
}


/*
*********************************************************************************************************
*                                     KeyDevControl (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
rk_err_t KeyDevSuspend(DEVICE_CLASS* dev)
{
    return RK_SUCCESS;
}
/*
*********************************************************************************************************
*                                     KeyDevControl (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
rk_err_t KeyDevResume(DEVICE_CLASS* dev)
{
    return RK_SUCCESS;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: KeyDevIsr
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 14:17:17
*******************************************************************************/
_DRIVER_KEY_KEYDEVICE_COMMON_
COMMON FUN void KeyDevIsr(void)
{
    rkos_start_timer(KeyISRDevHandle->TimerHandler);
    printf("t");
    //KeyDevRead(KeyISRDevHandle,&(KeyISRDevHandle->KeyVal), GUIKeyCallBack); //GUIKeyCallBack
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 Get_PlayKey_State(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
_DRIVER_KEY_KEYDEVICE_COMMON_
uint32 Get_PlayKey_State(void)
{
    uint8 temp;

    temp = Gpio_GetPinLevel(GPIO_CH2,GPIOPortA_Pin5);

    return (temp);
}

/*
*********************************************************************************************************
*                                    rk_err_t GetKey(UINT32 *pKeyVal)
*
* Description:  This function is the to get the key val.
*
* Argument(s) : UINT32 *pKeyVal
*
* Return(s)   : rk_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
_DRIVER_KEY_KEYDEVICE_COMMON_
rk_err_t ADCToKey(uint32 ADCVal,uint32 *pKeyVal)
{
    UINT32 KeyTemp = ADCVal;
    UINT32 PlayKeyTemp = 0;
    UINT32 RetVal = -1;

    if (Get_PlayKey_State() != 0)              //PLAY单独检测
    {
        //printf("paly GPIOA2_5 press\n");
        PlayKeyTemp = KEY_VAL_PLAY;
        RetVal  = RK_SUCCESS;
    }

    {
        if ((ADKEY2_MIN <= KeyTemp ) && (KeyTemp < ADKEY2_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY2;
            //printf("adc to keyval = MENU\n");
        }
        else if ((ADKEY3_MIN <= KeyTemp ) && (KeyTemp < ADKEY3_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY3;
            //printf("adc to keyval = UP\n");
        }
        else if ((ADKEY4_MIN <= KeyTemp ) && (KeyTemp < ADKEY4_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY4;
            //printf("adc to keyval = FFD\n");
        }
        else if ((ADKEY5_MIN <= KeyTemp ) && (KeyTemp < ADKEY5_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY5;
            //printf("adc to keyval = FFW\n");
        }
        else if ((ADKEY6_MIN <= KeyTemp ) && (KeyTemp < ADKEY6_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY6;
            //printf("adc to keyval = DOWN\n");
        }
        else if ((ADKEY7_MIN <= KeyTemp ) && (KeyTemp < ADKEY7_MAX))
        {
            *pKeyVal |= KEY_VAL_ADKEY7;
            //printf("adc to keyval = ESC\n");
        }
        RetVal = RK_SUCCESS;
    }

    *pKeyVal |= PlayKeyTemp;

    return RetVal;
}

/*******************************************************************************
** Name: KeyDev_GUIKeyCallBack
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.24
** Time: 8:46:38
*******************************************************************************/
/*
_DRIVER_KEY_KEYDEVICE_COMMON_
COMMON API void KeyDev_GUIKeyCallBack(void)
{
    GUI_EVENT_CLASS TempEvent;
    KeyDeviceClass* KeyDevHandle;

    printf("queue:%x\n", KeyDevHandle->buf);
    printf("\n\n****GUIKeyCallBack give GUIEventQueue****\n\n");
    TempEvent.cmd = GUI_CMD_KEYVAL;
    KeyDevHandle = KeyISRDevHandle; //gpstGUICtrlBlock->KeyHandle;
    */
    /*
    switch(KeyDevHandle.buf)
    {
        case KEY_VAL_PLAY_PRESS:
             TempEvent.para = KEY_VAL_PLAY_PRESS;
            break;
        case KEY_VAL_PLAY_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_PLAY_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_UP_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_UP_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_UP_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_DOWN_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_DOWN_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_DOWN_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_ESC_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_ESC_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_ESC_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_MENU_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_MENU_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_MENU_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFW_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFW_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFW_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFD_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFD_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_FFD_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_VOL_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_VOL_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_VOL_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_HOLD_PRESS:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_HOLD_SHORT_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        case KEY_VAL_HOLD_LONG_UP:
            TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
            break;
        default:
            break;
    }
*/
/*
   // TempEvent.para = KEY_VAL_PLAY_SHORT_UP;
    TempEvent.para = *(KeyDevHandle.buf);
    rkos_queue_send_fromisr( gpstGUICtrlBlock->GUIEventQueue,&TempEvent);

}
*/
/*
*********************************************************************************************************
*                                      void KeyScanService(void)
*
* Description:  This function is to scan the key.
*
* Argument(s) : void
*
* Return(s)   : void
*
* Note(s)     : none.
*********************************************************************************************************
*/
//static RKDeviceHandler KeyDev;                       //因为Key设备不会被删除，因此可以被一次性打开，减少获取设备句柄的次数
_DRIVER_KEY_KEYDEVICE_COMMON_
void KeyScanCallBack(HDC dev)
{
    uint32 ADCVal = 0;
    uint32 KeyVal = 0;
    uint32 KeyValBack = 0;
    KeyDeviceClass* KeyDevHandle = (KeyDeviceClass *)KeyISRDevHandle;
    //portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    //will write the adc val
    //printf("Enter KeyScanCallBack \n");
    if (KeyDevHandle != NULL)
    {
        if (KeyDevHandle->StartFlag != NULL)
        {
            ADCToKey(KeyISRDevHandle->ADCVal,&KeyVal);

            KeyValBack = KeyVal;
            if ((KeyVal != KEY_VAL_NONE) && (KeyVal ==  KeyDevHandle->KeyBackup))  //防止抖动
            {
                //printf("KeyVal != KEY_VAL_NONE $$$NO give$$$$ KeyReadSem\n");
                KeyDevHandle->KeyCnt++;

                if (KeyDevHandle->KeyCnt == 0)
                {
                    KeyDevHandle->KeyCnt = KEY_TIME_PRESS_START + KEY_TIME_PRESS_STEP;
                }
                else if (KeyDevHandle->KeyCnt == KEY_TIME_DOWN)
                {
                    //key down
                    KeyVal |= KEY_STATUS_DOWN;
                }
                else if (KeyDevHandle->KeyCnt == KEY_TIME_PRESS_START)
                {
                    //press start
                    KeyVal |= KEY_STATUS_PRESS_START;
                    *(KeyDevHandle->buf) = KeyVal;
                    if(KeyDevHandle->KeyCallBack != NULL)
                    {
                        //printf("&^*%$#@@2KeyCallBack KEY_STATUS_LONG_PESS KeyVal = %d\n",KeyVal);
                        KeyDevHandle->KeyCallBack(*(KeyDevHandle->buf));
                    }
                }
                else if (KeyDevHandle->KeyCnt > KEY_TIME_PRESS_START)
                {
                    //press
                    if (((KeyDevHandle->KeyCnt - KEY_TIME_PRESS_START) % KEY_TIME_PRESS_STEP) == 0)
                    {
                        KeyVal |= KEY_STATUS_PRESS;
                    }
                }
                rkos_semaphore_give_fromisr(KeyDevHandle->KeyReadSem);
                //ADCDev_Read(KeyDevHandle->ADCHandler,ADC_CHANEL_KEY,1,1*1000*1000, (uint16 *)(&KeyDevHandle->ADCVal),KeyScanCallBack);
            }
            else
            {
                if (KeyDevHandle->KeyCnt >= KEY_TIME_PRESS_START)
                {
                    KeyVal = (KeyDevHandle->KeyBackup | KEY_STATUS_LONG_UP);
                     *(KeyDevHandle->buf) = KeyVal;
                    if(KeyDevHandle->KeyCallBack != NULL)
                    {
                        //printf("&^*%$#@@1KeyCallBack KEY_STATUS_LONG_UP KeyVal = %d\n",KeyVal);
                        KeyDevHandle->KeyCallBack(*(KeyDevHandle->buf));
                    }
                     KeyDevHandle->KeyCnt = 0;
                }
                else if (KeyDevHandle->KeyCnt >= KEY_TIME_DOWN)
                {
                    KeyVal = (KeyDevHandle->KeyBackup | KEY_STATUS_SHORT_UP);
                    *(KeyDevHandle->buf) = KeyVal;
                    if(KeyDevHandle->KeyCallBack != NULL)
                    {
                        //printf("&^*%$#@@2KeyCallBack KEY_STATUS_SHORT_UP KeyVal = %d\n",KeyVal);
                        KeyDevHandle->KeyCallBack(*(KeyDevHandle->buf));
                    }
                    KeyDevHandle->KeyCnt = 0;
                }
                else
                {
                    KeyDevHandle->KeyCnt = 0;
                    //printf("$$$ disturb gan rao$$$$\n");
                    //ADCDev_Read(KeyDevHandle->ADCHandler,ADC_CHANEL_KEY,1,1, (uint16 *)(&KeyDevHandle->ADCVal),KeyScanCallBack);
                }
                rkos_semaphore_give_fromisr(KeyDevHandle->KeyReadSem);
            }

            KeyDevHandle->KeyBackup = KeyValBack;

        }
    }
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*
*********************************************************************************************************
*                                     KeyDevInit  (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
rk_err_t KeyDevInit(DEVICE_CLASS* dev)
{
    rk_err_t ret;
    KeyDeviceClass* KeyDevHandler = (KeyDeviceClass*)dev;

    printf("timer");
    if (NULL == KeyDevHandler )
        return RK_PARA_ERR;

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin5,IOMUX_GPIO2A5_IO);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin5,GPIO_IN);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin5,ENABLE);

    printf("timer");

    KeyDevHandler->TimerHandler = rkos_create_timer(10, 10, KeyDevIsr);
    rkos_start_timer(KeyDevHandler->TimerHandler);

    printf("\ntimer = %d",  KeyDevHandler->TimerHandler);

    return RK_SUCCESS;
}

/*
*********************************************************************************************************
*                                     KeyDevDeInit (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
rk_err_t KeyDevDeInit(DEVICE_CLASS* dev)
{
    KeyDeviceClass* KeyDevHandler = (KeyDeviceClass*)dev;

    return RK_SUCCESS;
}

/*
*********************************************************************************************************
*                                     KeyCreate (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
HDC KeyDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS*        KeyDevPublicHandler = NULL;
    KeyDeviceClass*      KeyDevHandler;
    uint32 i = 0;

    KeyDevHandler = (KeyDeviceClass *)rkos_memory_malloc(sizeof(KeyDeviceClass));
    if (KeyDevHandler == NULL)
    {
        goto exit;
    }

    KeyDevHandler->KeyReadSem = rkos_semaphore_create(1,1);

    if (KeyDevHandler->KeyReadSem == NULL)
    {
        goto KeyReadSem_err;
    }

    if(arg != NULL)
    {
        //KeyDevHandler->ADCHandler =((KEY_DEV_ARG *)arg)->ADCHandler;
        //KeyDevHandler->ADCHandler = arg;
        //KeyDevHandler->buf = ((KEY_DEV_ARG *)arg)->buf;
        //ADCDev_SetMainClk(KeyDevHandler->ADCHandler,1);
    }
    else
    {
        goto KeyArg_err;
    }

    KeyDevPublicHandler = (DEVICE_CLASS*)KeyDevHandler;
    KeyISRDevHandle = KeyDevHandler;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_KEY_DEV, MODULE_OVERLAY_CODE);
#endif

    if (KeyDevInit(KeyDevPublicHandler) != RK_SUCCESS)
    {
        rkos_semaphore_delete(KeyDevHandler->KeyReadSem);

        rkos_memory_free(KeyDevHandler);

        printf("init TimmerDev fail");
        return (HDC) RK_ERROR;
    }
    return KeyDevPublicHandler;


KeyArg_err:
    rkos_semaphore_delete(KeyDevHandler->KeyReadSem );
KeyReadSem_err:
    rkos_memory_free(KeyDevHandler);
exit:
    rkos_exit_critical();

    return (DEVICE_CLASS*)RK_ERROR;
}


/*
*********************************************************************************************************
*                                     KeyDevDeInit (void *p_arg)
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
_DRIVER_KEY_KEYDEVICE_INIT_
rk_err_t KeyDev_Delete(uint32 DevID, void * arg)
{
    #if 0
    KeyDeviceClass * KeyDevHandler;

    KeyISRDevHandle = NULL;

    KeyDevHandler = (KeyDeviceClass *)dev;
    KeyDevDeInit(dev);
    ADCDev_ClearMainClk(KeyDevHandler->ADCHandler);
    rkos_semaphore_delete(KeyDevHandler->KeyReadSem );

    rkos_memory_free(dev);

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_KEY_DEV);
#endif
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


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef _KEY_DEV_SHELL_
_DRIVER_KEY_KEYDEVICE_SHELL_
static char* ShellKeyName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "...",
    "\b",
};
/*******************************************************************************
** Name: KeyDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.24
** Time: 15:07:41
*******************************************************************************/
_DRIVER_KEY_KEYDEVICE_SHELL_
SHELL API rk_err_t KeyDev_Shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 Space;
    printf("KeyDev_Shell\n");
    StrCnt = ShellItemExtract(pstr, &pItem, &Space);
    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellKeyName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;
    switch (i)
    {
        case 0x00:
            //ret = ADCDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = KeyDevShellCreate(dev,pItem);
            break;

        case 0x02:
            //ret = ADCDevShellDel(dev,pItem);
            break;

        case 0x03:
            //ret = ADCDevShellTest(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}

/*******************************************************************************
** Name: KeyDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.24
** Time: 15:07:41
*******************************************************************************/
_DRIVER_KEY_KEYDEVICE_SHELL_
SHELL FUN rk_err_t KeyDevShellCreate(HDC dev, uint8 * pstr)
{
    HDC PhADC;
    rk_err_t ret;

    #if 0
    PhADC = RKDev_Open(DEV_CLASS_ADC, 1, NOT_CARE);
    if (PhADC == NULL)
    {
        printf("ADC OPEN FRIL\n");
    }
    else
    {
        printf("ADC OPEN SUCCESS\n");
    }
    #endif

    //stKeyArg->ADCHandler = gpstTimerDevISRHandler[0]->ADCHandle

    ret = RKDev_Create(DEV_CLASS_KEY, 0, &PhADC);//&stKeyArg
    if (ret != RK_SUCCESS)
    {
        printf("KEY CREATE FRIL\n");
    }
    else
    {
        printf("KEY CREATE SUCCESS\n");
    }

}

#endif
#endif


