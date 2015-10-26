/*
********************************************************************************************
*
*      Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\AD\AdcDevice.c
* Owner: zhuzhe
* Date: 2014.5.26
* Time: 10:42:35
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.5.26     10:42:35   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_AD_ADCDEVICE_C__
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
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

#define _DRIVER_AD_ADCDEVICE_COMMON_  __attribute__((section("driver_ad_adcdevice_common")))
//#define _DRIVER_AD_ADCDEVICE_COMMON_ __attribute__((section("driver_ad_adcdevice_write")))
#define _DRIVER_AD_ADCDEVICE_INIT_  __attribute__((section("driver_ad_adcdevice_init")))
#define _DRIVER_AD_ADCDEVICE_SHELL_  __attribute__((section("driver_ad_adcdevice_shell")))


#define ADCQueueLenth 16
#define ADCQueueSize  2
#define ADCMAXCHANNELNUM 5

typedef struct ADCReadItem
{
    uint16 channel;
    uint16* buf;
    uint32 size;
    uint16 clk;
    uint16 multiple;
    pADCDevRx_indicate RxCallBack;

}ADCReadItem;


typedef struct ADCDeviceClass
{
    DEVICE_CLASS ADCDevice;            //device father class
    HDC          pTimerDevice;

    pSemaphore ADCReadSem[ADCMAXCHANNELNUM];
    pSemaphore ADCMainClkSem;
    uint16       MainChannel;
    uint16       MainClk;
    ADCReadItem  ChannelItem[ADCMAXCHANNELNUM];
}ADCDeviceClass;

static ADCDeviceClass *  ADCDevISRHandler;

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
void ADCISRCallBack(HDC dev);
//void ADCISRCallBack(DEVICE_CLASS *dev);
void ADCDevISR(void);
void ADCTimerCallBack(void);

rk_err_t ADCDevShellHelp(HDC dev,  uint8 * pstr);
rk_err_t ADCDevShellCreate(HDC dev, uint8 * pstr);
rk_err_t ADCDevShellTest(HDC dev, uint8 * pstr);
rk_err_t ADCDevShellDel(HDC dev, uint8 * pstr);
rk_err_t ADCDevShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ADCDev_ScrSelect
** Input:eADC_CHN Channel
** Return: void
** Owner:cjh
** Date: 2015.6.17
** Time: 8:59:48
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API uint32  ADCDev_GetAdcData(uint32 Channel)
{
    uint32 timeOut = 60000;
    do
    {
        DelayUs(200);
        timeOut--;
        if(!timeOut)
            break;

    }while(CheckAdcState(Channel) & 0x01);

    if( timeOut )
    {
        return GetAdcData(Channel);
    }
    else
    {
        return timeOut;
    }

}

/*******************************************************************************
** Name: ADCDev_GetMainClk
** Input:DEVICE_CLASS *dev
** Return: uint16
** Owner:zhuzhe
** Date: 2014.5.29
** Time: 17:12:44
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API uint16 ADCDev_GetMainClk(DEVICE_CLASS *dev)
{
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;

    return  (ADCDevHandler->MainClk);
}
/*******************************************************************************
** Name: ADCDev_ClearMainClk
** Input:DEVICE_CLASS *dev,uint16 clk
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.29
** Time: 17:04:40
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_ClearMainClk(DEVICE_CLASS *dev)
{
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;

    rkos_semaphore_give(ADCDevHandler->ADCMainClkSem);

    ADCDevHandler->MainClk = 0;

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ADCDev_SetMainClk
** Input:uint16 clk
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.29
** Time: 16:21:48
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_SetMainClk(DEVICE_CLASS *dev,uint16 clk)
{
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;
    printf("tack ADCMainClkSem \n");
    rkos_semaphore_take(ADCDevHandler->ADCMainClkSem,MAX_DELAY);
    if (clk  != NULL)
    {
        ADCDevHandler->MainClk = clk;
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: ADCDev_Resu me
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 16:53:27
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Resume(DEVICE_CLASS *dev)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ADCDev_Suspend
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 16:52:13
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Suspend(DEVICE_CLASS *dev)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ADCDev_Read
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 16:51:02
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Read(HDC dev,uint16 channel,uint16 size,uint16 clk,uint16* buf, pADCDevRx_indicate RxCallBack)
{
    rk_err_t ret;
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;
    ADCReadItem tempReadItem;

    rkos_enter_critical();

    //ADCDev_SetMainClk((DEVICE_CLASS *)ADCDevHandler,clk);
    ADCDevHandler->ChannelItem[channel].size = size;
    ret = ADCDev_Register(dev,channel,size,clk, buf, RxCallBack);

#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    TimerDev_Register(ADCDevHandler->pTimerDevice,clk,ADCTimerCallBack);
#endif
    rkos_exit_critical();

    //TimerDev_PeriodSet(TIMER0, 50*1000, 24*1000*1000);

    ADCDev_Start((DEVICE_CLASS *)ADCDevHandler);
    //*buf = ADCDev_GetAdcData(channel);

    return ret;
}
/*******************************************************************************
** Name: ADCDev_Stop
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 11:38:22
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Stop(DEVICE_CLASS *dev)
{
    rk_err_t ret;
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) dev;
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    ret = TimerDev_Stop(ADCDevHandler->pTimerDevice);
#endif
    return ret;
}
/*******************************************************************************
** Name: ADCDev_Start
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 11:37:45
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Start(DEVICE_CLASS *dev)
{
    rk_err_t ret;
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) dev;
    //ADC power up and reset
    //AdcPowerDown();
    AdcStart(ADCDevHandler->ADCDevice.DevID);
    AdcIntEnable();
    AdcPowerUp();

#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    if(ADCDevHandler->pTimerDevice)
    {
        ret = TimerDev_Start(ADCDevHandler->pTimerDevice);
    }
    else
    {
        return RK_SUCCESS;
    }
#endif

    return ret;
}
/*******************************************************************************
** Name: ADCDev_UnRegister
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 11:35:19
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_UnRegister(DEVICE_CLASS *dev,uint16 channel)
{
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) dev;
    ADCReadItem* pReadItem;
    //portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    printf("ADCDev_UnRegister give ADCReadSem\n");
    if (dev != NULL)
    {
        pReadItem = ADCDevHandler->ChannelItem;
        pReadItem+= channel;
        rkos_enter_critical();
        pReadItem->clk = 0;
        pReadItem->buf = NULL;
        pReadItem->channel = 0;
        pReadItem->multiple = 0;
        rkos_exit_critical();

        //rkos_semaphore_give_fromisr(ADCDevHandler->ADCReadSem[channel]);

        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;

    }
}
/*******************************************************************************
** Name: ADCDev_Register
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 10:54:28
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON API rk_err_t ADCDev_Register(HDC dev,uint16 channel,uint16 size,uint16 clk,uint16* buf, pADCDevRx_indicate RxCallBack)
{
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) dev;
    ADCReadItem* pReadItem;

    if (dev != NULL)
    {
        pReadItem = ADCDevHandler->ChannelItem;
        pReadItem+= channel;
        //printf("Register take ADCReadSem\n");
        //rkos_semaphore_take(ADCDevHandler->ADCReadSem[channel],MAX_DELAY);
        if ( pReadItem->clk == 0)
        {
            if ((clk % ADCDevHandler->MainClk )!= 0)
            {
                return RK_ERROR;
            }

            pReadItem->channel = channel;
            pReadItem->clk = clk;
            pReadItem->size = size;
            pReadItem->multiple = clk/(ADCDevHandler->MainClk);
            pReadItem->buf = buf;
            pReadItem->RxCallBack = RxCallBack;
            return RK_SUCCESS;
        }
        else
        {
            return RK_ERROR;

        }
    }
    else
    {
        return RK_ERROR;

    }

}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ADCTimerCallBack
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.29
** Time: 14:08:39
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON FUN void ADCTimerCallBack(void)
{
    ADCReadItem tempReadItem;
    uint32 xHigherPriorityTaskWoken;
    uint16 i,j;
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) ADCDevISRHandler;
    ADCReadItem* pReadItem;
    uint16 Val;
    uint32 CheckNum = 0;
    uint32 adcValueTemp;
    //printf("~~~ADCTimerCallBack~~~\n");
    if (ADCDevHandler != NULL)
    {
        pReadItem = ADCDevHandler->ChannelItem;

        for (i = 0;i<ADCMAXCHANNELNUM;i++)
        {
            if ( pReadItem->clk != NULL)
            {
                pReadItem->multiple --;

                if (pReadItem->multiple == NULL)
                {
                    pReadItem->size--;
                    //AdcStart((uint8)pReadItem->channel);
                    //AdcIntEnable();
                    //AdcPowerUp();
                    Adc->ADC_CTRL = pReadItem->channel | ADC_START | ADC_POWERUP | ADC_INT_ENBALE;//ADCDevISR
                    //printf("ADCTimerCallback adc star\n");
                    while (1)
                    {
                        if (CheckAdcState((uint8)pReadItem->channel) == NULL)
                            break;
                        CheckNum++;
                        if (CheckNum > 1024)
                        {
                           // AdcPowerDown();
                           // AdcPowerUp();
                           // AdcStart((uint8)pReadItem->channel);
                           // AdcIntEnable();
                            Adc->ADC_CTRL = pReadItem->channel | ADC_START | ADC_POWERUP | ADC_INT_ENBALE;//ADCDevISR
                            CheckNum = 0;
                        }
                    }

                    //*(pReadItem->buf) = GetAdcData((uint8)pReadItem->channel);
                    adcValueTemp = GetAdcData((uint8)pReadItem->channel);
                    *(pReadItem->buf) = adcValueTemp;
                    //printf("*&*&*&*&*&ADCTimerCallback ADCvalue= %d\n",adcValueTemp);
                    //pReadItem->multiple = pReadItem->clk;    // reload the clk
                    pReadItem->multiple = (pReadItem->clk)/(ADCDevHandler->MainClk);

                    if (pReadItem->size == 0)
                    {
                        pReadItem->clk = 0;
                    }
                    else
                    {
                        pReadItem->buf++;
                    }
                }
            }
            pReadItem++;
        }

        pReadItem = ADCDevHandler->ChannelItem;
        for (i = 0;i<ADCMAXCHANNELNUM;i++)
        {
            if (pReadItem->size == NULL)
            {
                j++;
            }
        }
        if (j =  ADCMAXCHANNELNUM )
        {
            //printf("&&&&&TimerDev_UnRegister\n");
            //TimerDev_UnRegister(ADCDevHandler->pTimerDevice, ADCTimerCallBack);
        }

        for (i = 0;i<ADCMAXCHANNELNUM;i++)
        {
            if (pReadItem->size == NULL)
            {
                if (pReadItem->RxCallBack!= NULL)
                {
                    //printf("&&&&&ADCDev_UnRegister&pReadItem->RxCallBack:keyscance\n");
                    //ADCDev_UnRegister((HDC)ADCDevHandler,pReadItem->channel);
                    pReadItem->RxCallBack((HDC)ADCDevHandler);  // must be the last

                }
            }
            pReadItem++;
        }
    }
}
/*******************************************************************************
** Name: ADCISRCallBack
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 17:19:58
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON FUN void ADCISRCallBack(HDC dev)//DEVICE_CLASS *dev
{
#if 0
    static long xHigherPriorityTaskWoken;
    //ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) dev;
    ADCDeviceClass* ADCDevHandler = ADCDevISRHandler;
    ADCReadItem* pReadItem;
    uint32 i;

    pReadItem = ADCDevHandler->ChannelItem;
    pReadItem += ADCDevHandler->MainChannel;

    printf("~!~!~!ADCISRCallBack\n");
    *(pReadItem->buf) = GetAdcData(ADCDevHandler->MainChannel);
    pReadItem->buf++;

    if (pReadItem->size == 0)
    {
        pReadItem->RxCallBack(ADCDevISRHandler);
        ADCDev_UnRegister((DEVICE_CLASS *)ADCDevHandler,pReadItem->channel);

    }

    for (i = 0;i<ADCMAXCHANNELNUM;i++)
    {
        if (pReadItem->clk ==0)
            pReadItem++;
        else
            break;
    }

    if (i != ADCMAXCHANNELNUM )
    {
        //reload the same period
        pReadItem = ADCDevHandler->ChannelItem;
        for (i = 0;i<ADCMAXCHANNELNUM;i++)
        {
            if ((pReadItem->multiple == NULL) && (pReadItem->clk != NULL))
            {
                pReadItem->size--;
                AdcStart(pReadItem->channel);
                pReadItem->multiple = pReadItem->clk;    // reload the clk
                ADCDevHandler->MainChannel = pReadItem->channel;
                if (pReadItem->size == 0)
                {
                    pReadItem->clk = 0;
                }
                break;
            }
            pReadItem++;
        }
    }
    else
    {
        //没有ADC请求
        TimerDev_Stop(ADCDevHandler->pTimerDevice);
        TimerDev_UnRegister(ADCDevHandler->pTimerDevice,ADCTimerCallBack);
    }
#endif
}

/*******************************************************************************
** Name: ADCDevISR
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 17:12:44
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_COMMON_
COMMON FUN void ADCDevISR(void)
{
    ADCDeviceClass* ADCDevHandler = (ADCDeviceClass*) ADCDevISRHandler;
    ADCReadItem* pReadItem;
    pFunc CallBack;

    AdcIntClr();

    if (ADCDevHandler != NULL)
    {
        pReadItem = &(ADCDevHandler->ChannelItem[ADCDevHandler->ADCDevice.DevID]);
        CallBack = (pFunc) pReadItem->RxCallBack;
        if (CallBack)
        {
            ADCDevHandler->ChannelItem[ADCDevHandler->ADCDevice.DevID].RxCallBack = 0;
            CallBack();
        }
    }

    if (ADCDevISRHandler != NULL)
    {

        ADCISRCallBack((DEVICE_CLASS *)ADCDevHandler);
    }

}



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

/*******************************************************************************
** Name: ADCDev_Create
** Input:void
** Return: HDC
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 15:38:01
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_INIT_
INIT API HDC ADCDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS* ADCDevPublicHandler;
    ADCDeviceClass* ADCDevHandler;
    //ADC_DEV_ARG* stADCarg = (ADC_DEV_ARG *)arg;
    int channel;
    uint i;

    ADCDevHandler  = (ADCDeviceClass *)rkos_memory_malloc(sizeof(ADCDeviceClass));
    memset(ADCDevHandler, 0, sizeof(ADCDeviceClass));
    memset((void *)(ADCDevHandler->ChannelItem),NULL,sizeof(ADCReadItem)*ADCMAXCHANNELNUM);

    if (ADCDevHandler == NULL)
    {
        goto exit;
    }

    ADCDevHandler->pTimerDevice = arg;

    ADCDevHandler->ADCMainClkSem = rkos_semaphore_create(1,1);

    for (i = 0;i< ADCMAXCHANNELNUM;i++)
    {
        ADCDevHandler->ADCReadSem[i] = rkos_semaphore_create(1,1);
        if (ADCDevHandler->ADCReadSem[i] == NULL)
        {
            break;
        }
    }
    if (i != NULL && arg == NULL)
    {
        goto ADCReadSemError;
    }

    ADCDevPublicHandler = &(ADCDevHandler->ADCDevice);
    ADCDevPublicHandler->DevID = DevID;

    AdcDevHwInit (ADCDevPublicHandler->DevID,channel);

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_ADC_DEV, MODULE_OVERLAY_CODE);
#endif
    if (ADCDev_Init(ADCDevHandler) != RK_SUCCESS)
    {
        rkos_semaphore_delete(ADCDevHandler->ADCReadSem);

        rkos_memory_free(ADCDevHandler);

        printf("init ADCDev fail");
        return (HDC) RK_ERROR;
    }
    ADCDevISRHandler =  ADCDevHandler;
    return ADCDevHandler;

ADCControlSemError:
    rkos_semaphore_delete(ADCDevHandler->ADCReadSem);
ADCReadSemError:
    rkos_memory_free(ADCDevHandler);
exit:
    return (DEVICE_CLASS*) RK_ERROR;
}

/*******************************************************************************
** Name: ADCDevInit
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 17:29:17
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_INIT_
COMMON FUN rk_err_t ADCDev_Init(HDC dev)
{
    ADCDeviceClass* ADCDevHandler =(ADCDeviceClass *)dev;
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ADCDevDeInit
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 17:38:05
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_INIT_
INIT API rk_err_t ADCDev_DeInit(DEVICE_CLASS *dev)
{
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;

    IntDisable(INT_ID_SRADC);
    IntUnregister(INT_ID_SRADC);

    AdcPowerDown();
    ScuClockGateCtr(CLK_SARADC_GATE, 0);
    ScuClockGateCtr(PCLK_SARADC_GATE, 0);
    ADCDev_Stop(dev);

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ADCDev_Delete
** Input:void
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.26
** Time: 17:14:30
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_INIT_
INIT API rk_err_t ADCDev_Delete(uint32 DevID, void * arg)
{

#if 0
    ADCDeviceClass * ADCDevHandler;
    ADCDevHandler =  (ADCDeviceClass *)dev;

    ADCDevISRHandler  = NULL;

    ADCDev_DeInit(dev);
    rkos_semaphore_delete(ADCDevHandler->ADCReadSem);

    rkos_memory_free(ADCDevHandler);

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_ADC_DEV);
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


#ifdef _ADC_DEV_SHELL_

_DRIVER_AD_ADCDEVICE_SHELL_
static char* ShellADCName[] =
{
    "help",
    "create",
    "del",
    "test",
    "pcb",
    "\b",
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AdcDev_Shell
** Input:HDC dev
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.18
** Time: 16:50:17
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_SHELL_
rk_err_t ADCDev_Shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 Space;
    StrCnt = ShellItemExtract(pstr, &pItem, &Space);
    if (StrCnt == 0)
    {
        return RK_ERROR;
    }
    ret = ShellCheckCmd(ShellADCName, pItem, StrCnt);
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
            ret = ADCDevShellHelp(dev,pItem);
            break;

        case 0x01:
            ret = ADCDevShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = ADCDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = ADCDevShellTest(dev,pItem);
            break;

        case 0x04:
            ret = ADCDevShellPcb(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
    return RK_SUCCESS;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
_DRIVER_AD_ADCDEVICE_SHELL_
SHELL FUN rk_err_t ADCDevShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if ( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    printf("        Adc命令集提供了一系列的命令对Adc进行操作\r\n");
    printf("Adc包含的子命令如下:           \r\n");
    printf("help      显示help信息           \r\n");
    printf("create    创建adc              \r\n");
    printf("test      测试adc命令          \r\n");
    printf("del       删除adc              \r\n");
    printf("pcb       显示pcb信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: WRMDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.13
** Time: 14:03:40
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_SHELL_
SHELL FUN rk_err_t ADCDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hADCDevHandle;
    DEVICE_CLASS* pstADCPublicHandler;
    ADCDeviceClass * ADCDevHandle;
    uint32 adcChannel = ADC_CHANEL_KEY;
    uint32 adcVal = 0;
    uint32 temp = 0x01;

    hADCDevHandle = RKDev_Open(DEV_CLASS_ADC,ADC_CHANEL_KEY,NOT_CARE);
    if (hADCDevHandle == NULL)
    {
        printf("ADC OPEN FRIL\n");
    }
    else
    {
        printf("ADC%d OPEN SUCCESS\n",ADC_CHANEL_KEY);
    }
    //do test....
    ADCDevHandle = (ADCDeviceClass *)hADCDevHandle;
    pstADCPublicHandler = (DEVICE_CLASS *)ADCDevHandle;

//    Adc->ADC_CTRL = adcChannel    | ADC_START | ADC_POWERUP | ADC_INT_ENBALE;//ADCDevISR
    ADCDev_Read(ADCDevHandle,adcChannel,1,1, (uint16 *)(&adcVal), ADCISRCallBack);//((DEVICE_CLASS *)ADCDevHandle)
   /* while (1)
    {
        DelayMs(10);
        if ((CheckAdcState(adcChannel) & 0x01) == 0)
        {
            break;
        }

        temp = Adc->ADC_STAS;
        if ((temp & 0x01) == 0)
        {
            break;
        }

    } */
    //ADCDev_Stop(pstADCPublicHandler);
    //adcVal = (UINT32)(Adc->ADC_DATA);
    printf("Adc channel %d read value = %d\n", adcChannel, adcVal);

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: WRMDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.13
** Time: 14:03:40
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_SHELL_
SHELL FUN rk_err_t ADCDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;

    //Get WRMDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }
    if (RKDev_Delete(DEV_CLASS_ADC, DevID, NULL) != RK_SUCCESS)
    {
        rk_print_string("WRMDev delete failure");
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: WRMDevShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.13
** Time: 14:03:40
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_SHELL_
SHELL FUN rk_err_t ADCDevShellCreate(HDC dev, uint8 * pstr)
{
    //ADC_DEV_ARG stADCArg;
    //TIMER_DEV_ARG stTimerArg;
    HDC PhTimmer;
    rk_err_t ret;
    printf("ADCDevShellCreate\n");
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__

    //stADCArg.hTimmer = RKDev_Open(DEV_CLASS_TIMER,TIMER0,NOT_CARE);
    PhTimmer = RKDev_Open(DEV_CLASS_TIMER,TIMER0,NOT_CARE);
    //stADCArg.channel = adcChannel;

    if (PhTimmer == NULL)
    {
        printf("TIMER0 OPEN FRIL\n");
    }
    else
    {
        printf("TIMER0 OPEN SUCCESS\n");
    }

    ret = RKDev_Create(DEV_CLASS_ADC,ADC_CHANEL_KEY, PhTimmer);
#else
    ret = RKDev_Create(DEV_CLASS_ADC, ADC_CHANEL_KEY, NULL);
#endif

    if (ret != RK_SUCCESS)
    {
        printf("ADC CREATE FRIL\n");
    }
    else
    {
        printf("ADC CREATE SUCCESS\n");
    }

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: WRMDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.13
** Time: 14:03:40
*******************************************************************************/
_DRIVER_AD_ADCDEVICE_SHELL_
SHELL FUN rk_err_t ADCDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hWRMDev;
    uint32 DevID;

    //Get WRMDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    //Display pcb...
#if 0
    if (ADCDevISRHandler[DevID] != NULL)
    {
        rk_print_string("rn");
        rk_printf_no_time("WRMDev ID = %d Pcb list as follow:", DevID);
        rk_printf_no_time("    &stWRMDevice---0x%08x", &gpstWRMDevISR[DevID]->stWRMDevice);
        rk_printf_no_time("    osWRMOperReqSem---0x%08x", gpstWRMDevISR[DevID]->osWRMOperReqSem);
        rk_printf_no_time("    osWRMOperSem---0x%08x", gpstWRMDevISR[DevID]->osWRMOperSem);
        rk_printf_no_time("    osWRMReadReqSem---0x%08x", gpstWRMDevISR[DevID]->osWRMReadReqSem);
        rk_printf_no_time("    osWRMReadSem---0x%08x", gpstWRMDevISR[DevID]->osWRMReadSem);
        rk_printf_no_time("    osWRMWriteReqSem---0x%08x", gpstWRMDevISR[DevID]->osWRMWriteReqSem);
        rk_printf_no_time("    osWRMWriteSem---0x%08x", gpstWRMDevISR[DevID]->osWRMWriteSem);
        //rk_printf_no_time("    usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
        //rk_printf_no_time("    usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
    }
    else
    {
        rk_print_string("rn");
        rk_printf_no_time("WRMDev ID = %d not exit", DevID);
    }
#endif
    return RK_SUCCESS;
}
#endif

#endif



