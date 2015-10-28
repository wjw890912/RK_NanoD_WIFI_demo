/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\Timer\TimerDevice.c
* Owner: zhuzhe
* Date: 2014.5.19
* Time: 11:00:06
* Desc:
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* zhuzhe     2014.5.19     11:00:06   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "BspConfig.h"
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
typedef  struct _TIMER_DEVICE_CLASS
{
    DEVICE_CLASS stTimerDevice;
    pSemaphore osTimerReqSem;
    pFunc TimerCallBack;
    uint32 Clk;

    HDC    TimerHandle;
    HDC    ADCHandle;
    HDC    KeyHandle;

    uint32 KeyVal;
}TIMER_DEVICE_CLASS;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static volatile TIMER_DEVICE_CLASS * gpstTimerDevISRHandler[TIMER_MAX];



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
void Timer1_Test_ISR(void);
void Timer0_Test_ISR(void);
rk_err_t TimeDevShellBspDeinit(HDC dev, uint8 * pstr);
rk_err_t TimeDevShellBspTime_Test(HDC dev, uint8 * pstr);
rk_err_t TimeDevShellBsp(HDC dev, uint8 * pstr);
rk_err_t TimeShellHelp(HDC dev, uint8 * pstr);
rk_err_t TimeDevShellBspHelp(HDC dev, uint8 * pstr);

rk_err_t TimerDevDeInit(HDC dev);
rk_err_t TimerDevInit(HDC dev);
void TimerIntIsr(uint32 timerNum);
void TimerDev_CallBack(void);
//void TimerIntIsr0(void);
//void TimerIntIsr1(void);




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(Common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: TimerDev_Task_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.17
** Time: 20:15:34
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
COMMON API rk_err_t TimerDev_Task_Init(void *pvParameters)
{
    printf("enter TimerDev_Task_Init\n");
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerDev_Task_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.17
** Time: 20:15:34
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
COMMON API rk_err_t TimerDev_Task_DeInit(void *pvParameters)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerDev_Connect
** Input:HDC dev, uint32 Period
** Return: rk_err_t
** Owner:cjh
** Date: 2014.10.10
** Time: 14:28:54
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
COMMON API rk_err_t TimerDev_Connect(HDC dev, uint32 tmerNum, uint32 TimerClk, uint64 usTick)
{
    TIMER_DEVICE_CLASS * pstTimergDev = (TIMER_DEVICE_CLASS *)dev;

    //timer interupt Period
    //printf("Timer task Connect\n");
    TimerDev_PeriodSet(tmerNum, usTick, TimerClk);
    //printf("Timer task TimerDev_PeriodSet over\n");
    RKTaskCreate(TASK_ID_TIMER, 0, NULL);

    //printf("Timer task Connect success\n");
    return RK_SUCCESS;
}


/*******************************************************************************
** Name: TimerDev_Task_Enter
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.17
** Time: 20:15:34
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
COMMON API void TimerDev_Task_Enter(void)
{
    while (1)
    {
        //printf ("~!~!~!osTimerReqSem_take~!~!~!\n");
        rkos_semaphore_take(gpstTimerDevISRHandler[TIMER0]->osTimerReqSem,MAX_DELAY);
#ifdef __DRIVER_KEY_KEYDEVICE_C__
        //printf ("~!~!~!KeyDevRead~!~!~!\n");
        KeyDevRead(gpstTimerDevISRHandler[TIMER0]->KeyHandle,&(gpstTimerDevISRHandler[TIMER0]->KeyVal), GUIKeyCallBack);//GUIKeyCallBack
        //TimerStart(TIMER0);
#endif
        TimerClrInt(TIMER0);
        IntEnable(INT_ID_TIMER0);
        //printf("timer task serice...\n");
    }
}

/*******************************************************************************
** Name: TimerDev_PeriodSet
** Input:HDC dev,pFunc TimerCallBack
** Return: rk_err_t
** Owner:cjh
** Date: 2014.5.21
** Time: 11:25:54
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_PeriodSet(eTIMER_NUM timerNum,UINT64 usTick, uint32 timerClk)
{
    UINT64 count,Timeclk,count_check;
    BOOL Retval = TRUE;
    TIMER_DEVICE_CLASS pstTimerDevHandler;
    HDC dev;

    //SetTimerFreq(timerNum,XIN24M,24*1000*1000);
    SetTimerFreq(timerNum,XIN24M,timerClk);

    Timeclk  = GetTimerFreq(timerNum);
    //1s
    count =  Timeclk * usTick/1000/1000;

    //printf("period = %d us\n",usTick);
    pstTimerDevHandler.stTimerDevice.DevID = timerNum;
    dev = (HDC) (&pstTimerDevHandler);
   /* Disable the timer ,usermode,mask*/
    //TimerDev_Stop(dev);
    TimerStop (timerNum);

    TimerInit(timerNum,TIMER_FREE_RUNNING);

   /* free mode and enable the timer  ，not mask*/
    TimerSetCount(timerNum,count);

    TimerIntUnmask(timerNum);

    TimerDev_Start(dev);

    return Retval;
}

/*******************************************************************************
** Name: TimerDev_UnRegister
** Input:HDC dev,pFunc TimerCallBack
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.21
** Time: 11:25:54
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_UnRegister(HDC dev,pFunc TimerCallBack)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;
    printf("TimerDev_UnRegister\n");
    if(TimerCallBack == pstTimerDevHandler->TimerCallBack)
    {
        printf("TimerDev_UnRegister disable timer int\n");
        rkos_enter_critical();
        pstTimerDevHandler->TimerCallBack = NULL;
//        rkos_semaphore_give(pstTimerDevHandler->osTimerReqSem);
        TimerDev_Stop(dev);
        rkos_exit_critical();
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;

    }
}

/*******************************************************************************
** Name: TimerDev_Register
** Input:HDC dev,uint32 clk,pFuncTimerCallBack
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.21
** Time: 11:24:35
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_Register(HDC dev,uint32 clk,pFunc TimerCallBack)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    //rkos_semaphore_take(pstTimerDevHandler->osTimerReqSem,MAX_DELAY);

    pstTimerDevHandler->TimerCallBack = TimerCallBack;
    pstTimerDevHandler->Clk = clk;

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerDev_Clear
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.21
** Time: 11:24:00
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_Clear(HDC dev)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;

    rkos_enter_critical();
    IntPendingClear(pstTimerPublicHandler->DevID);
    rkos_exit_critical();
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
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t  TimerDev_Stop(HDC dev)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = &(pstTimerDevHandler->stTimerDevice);
    printf("!!Stop timerdev=%d\n",pstTimerPublicHandler->DevID);
    //TimerStop(pstTimerPublicHandler->DevID);
    rkos_enter_critical();
    if(pstTimerPublicHandler->DevID == TIMER0)
        IntDisable(INT_ID_TIMER0);
    else
        IntDisable(INT_ID_TIMER1);
    rkos_exit_critical();

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerDev_Start
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.21
** Time: 11:22:48
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t  TimerDev_Start(HDC dev)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS *)dev;
    pstTimerPublicHandler = &(pstTimerDevHandler->stTimerDevice);
    //printf("!!intenable,,,,,start timerdev=%d\n",pstTimerPublicHandler->DevID);
    TimerStart(pstTimerPublicHandler->DevID);
/*
    rkos_enter_critical();
    if(pstTimerPublicHandler->DevID == TIMER0)
        IntEnable(INT_ID_TIMER0);
    else
        IntEnable(INT_ID_TIMER1);
    rkos_exit_critical();
*/
    return RK_SUCCESS;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: TimerDev_CallBack
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.19
** Time: 10:13:39
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
COMMON FUN void TimerDev_CallBack(void)
{
    printf("~TimerDev_CallBack\n");
}


/*******************************************************************************
** Name: TimerDevClose
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_Close(HDC dev)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;

    if ( pstTimerPublicHandler->Cnt == 0)
    {
        TimerDevDeInit(dev);
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: TimerDevOpen
** Input:HDC dev, uint16 oflag
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
rk_err_t TimerDev_Open(HDC dev, uint16 oflag)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;

    if ( pstTimerPublicHandler->Cnt == 0)
    {
        TimerDevInit(dev);
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: TimerIntIsr0
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
void TimerIntIsr0(void)
{
    TimerIntIsr(TIMER0);
   /*
    if(gpstTimerDevISRHandler[TIMER0] != NULL)
    {
        //IntDisable(INT_ID_TIMER0);
        gpstTimerDevISRHandler[TIMER0]->Clk = gpstTimerDevISRHandler[TIMER0]->Clk + 1;
        if(gpstTimerDevISRHandler[TIMER0]->TimerCallBack != NULL)
            gpstTimerDevISRHandler[TIMER0]->TimerCallBack();
    }
    */
    //printf ("IntIs0:%d\n",gpstTimerDevISRHandler[TIMER0]->Clk);
}

/*******************************************************************************
** Name: TimerIntIsr1
** Input:void
** Return: void
** Owner:wrm
** Date: 2015.5.12
** Time: 14:57:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
void TimerIntIsr1(void)
{
    TimerIntIsr(TIMER1);
}

/*******************************************************************************
** Name: TimerIntIsr
** Input:uint32 timerNum
** Return: void
** Owner:cjh
** Date: 2015.6.19
** Time: 9:57:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_COMMON_
void TimerIntIsr(uint32 timerNum)
{
    pFunc CallBack;

    if(timerNum)
    {
        IntDisable(INT_ID_TIMER1);
//        printf ("*****~!~!~!IntIs1 & give osTimerReqSem\n");
        TimerClrInt(TIMER1);
        CallBack = gpstTimerDevISRHandler[TIMER1]->TimerCallBack;
        //gpstTimerDevISRHandler[TIMER1]->TimerCallBack = 0;
        rkos_semaphore_give_fromisr(gpstTimerDevISRHandler[TIMER1]->osTimerReqSem);
    }
    else
    {
        IntDisable(INT_ID_TIMER0);
        //printf ("****~!~!~!IntDisable0 & give osTimerReqSem\n");
        TimerClrInt(TIMER0);
        CallBack = gpstTimerDevISRHandler[TIMER0]->TimerCallBack;
        //gpstTimerDevISRHandler[TIMER0]->TimerCallBack = 0;
        rkos_semaphore_give_fromisr(gpstTimerDevISRHandler[TIMER0]->osTimerReqSem);
    }
    //printf ("TimerIntIsr CallBack\n");

    if (CallBack)
    {
        CallBack();
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
** Name: TimerDevCreate
** Input:void
** Return: HDC
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
HDC TimerDev_Create(uint32 DevID, void * arg)
{
    //DEVICE_CLASS* pstTimerPublicHandler;
    DEVICE_CLASS* pstDev;
    TIMER_DEVICE_CLASS * pstTimerDevHandler;

   // if (pstTimerDevHandler == NULL)
   // {
   //     return (HDC) RK_ERROR;
   // }
    pstTimerDevHandler =  rkos_memory_malloc(sizeof(TIMER_DEVICE_CLASS));
    memset(pstTimerDevHandler, 0, sizeof(TIMER_DEVICE_CLASS));
    if (pstTimerDevHandler == NULL)
    {
        return NULL;
    }
    //init handler...
    pstTimerDevHandler->osTimerReqSem = rkos_semaphore_create(1,1);
    //pstTimerDevHandler->osTimerReqSem = rkos_semaphore_create(1,1);

    if ((pstTimerDevHandler->osTimerReqSem) == 0)//|| ((pstTimerDevHandler->osTimerReqSem) == 0)
    {
        rkos_semaphore_delete(pstTimerDevHandler->osTimerReqSem);
        //rkos_semaphore_delete(pstTimerDevHandler->osTimerReqSem);

        rkos_memory_free(pstTimerDevHandler);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstTimerDevHandler;
    pstDev->suspend = TimerDev_Suspend;
    pstDev->resume  = TimerDev_Resume;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_TIMER_DEV, MODULE_OVERLAY_CODE);
#endif

    gpstTimerDevISRHandler[DevID] = NULL;

    TimerDevHwInit(DevID, 0);

    if (TimerDevInit(pstTimerDevHandler) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstTimerDevHandler->osTimerReqSem);
        //rkos_semaphore_delete(pstTimerDevHandler->osTimerReqSem);
        rkos_memory_free(pstTimerDevHandler);

        printf("init TimmerDev fail");
        return (HDC) RK_ERROR;
    }
    if (DevID == 0)
        gpstTimerDevISRHandler[TIMER0] = pstTimerDevHandler;
    else
        gpstTimerDevISRHandler[TIMER1] = pstTimerDevHandler;

    return (HDC)pstTimerDevHandler;
}

/*******************************************************************************
** Name: TimerDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.17
** Time: 20:15:34
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
INIT API rk_err_t TimerDev_Delete(uint32 DevID, void * arg)
{
#if 0
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;
    HDC hTimer;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;

    //Check TimerDev is not exist...
    if(gpstTimerDevISRHandler[pstTimerDevHandler->stTimerDevice.DevID] == NULL)
    {
        return RK_ERROR;
    }

    hTimer = (HDC)gpstTimerDevISRHandler[pstTimerDevHandler->stTimerDevice.DevID];
    //TimerDev deinit...
    TimerDevDeInit(hTimer);

    rkos_memory_free( pstTimerDevHandler);
    gpstTimerDevISRHandler[pstTimerDevHandler->stTimerDevice.DevID] = NULL;

    //Delete TimerDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_TIMER_DEV);
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
/*******************************************************************************
** Name: TimerDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
rk_err_t TimerDev_Resume(HDC dev)
{
    //TimerDev Resume...
    TIMER_DEVICE_CLASS * TimerDev =  (TIMER_DEVICE_CLASS *)dev;
    if (TimerDev == NULL)
    {
        return RK_ERROR;
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: TimerDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
rk_err_t TimerDev_Suspend(HDC dev)
{
    //TimerDev Suspend...
    TIMER_DEVICE_CLASS * TimerDev =  (TIMER_DEVICE_CLASS *)dev;
    if (TimerDev == NULL)
    {
        return RK_ERROR;
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: TimerDevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
rk_err_t TimerDevDeInit(HDC dev)
{
    TIMER_DEVICE_CLASS * pstTimerDevHandler;
    DEVICE_CLASS* pstTimerPublicHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS*)dev;
    pstTimerPublicHandler = (DEVICE_CLASS*)dev;

    IntPendingClear(FAULT_ID15_SYSTICK);
    IntUnregister(FAULT_ID15_SYSTICK);
    //TimerPeriodSet(0);
    TimerDev_PeriodSet(pstTimerPublicHandler->DevID, 0, 0);
    IntDisable(FAULT_ID15_SYSTICK);
    ScuClockGateCtr( PCLK_TIMER_GATE, 0);

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: TimerDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.5.19
** Time: 11:08:25
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
rk_err_t TimerDevInit(HDC dev)
{
    DEVICE_CLASS* pstTimerPublicHandler;
    TIMER_DEVICE_CLASS * pstTimerDevHandler;

    pstTimerDevHandler = (TIMER_DEVICE_CLASS *)dev;
    pstTimerPublicHandler = &(pstTimerDevHandler->stTimerDevice);

    return RK_SUCCESS;
}

#ifdef _TIMER_DEV_SHELL_

_DRIVER_TIMER_TIMERDEVICE_SHELL_
static uint8* ShellTimeName[] =
{
    //"pcb",
    "bsp",
    "help",
    "create",
    "create1",
    "test",
    "test0_10",
    "test1_5",
    "test1_10",
    "\b",                          // the end
};

#ifdef SHELL_BSP
static uint8* ShellTimeBspName[] =
{
    "help",
    "init",
    "deinit",
    "\b",
};
#endif
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: TimerDev_Shell
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 16:40:07
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL API rk_err_t TimerDev_Shell(HDC dev,  uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t  ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }
    while (memcmp((uint8*)ShellTimeName[i],pItem,strlen(ShellTimeName[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellTimeName[i]) == '\b')
            return RK_ERROR;
    }

    pItem += StrCnt;
    pItem++;                  //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //bsp
            #ifdef SHELL_BSP
            ret = TimeDevShellBsp(dev,pItem);
            #endif
            break;
        case 0x01:  //help
            ret = TimeShellHelp(dev,pItem);
            break;
        case 0x02: //create
            *pItem = 0;
            ret = TimerShellCreate(dev,pItem);
            break;
        case 0x03:
            *pItem = 1;
            ret = TimerShellCreate(dev,pItem);
            break;
        case 0x04: //test_timer0 5 second
            *pItem = 0;
            ret = TimerShellTest(dev,pItem,5);
            break;
        case 0x05: //test_timer0 10 second
            *pItem = 0;
            ret = TimerShellTest(dev,pItem,10);
            break;
        case 0x06: //test_timer1 5 second
            *pItem = 1;
            ret = TimerShellTest(dev,pItem,5);
            break;
        case 0x07: //test_timer1 10 second
            *pItem = 1;
            ret = TimerShellTest(dev,pItem,10);
            break;
        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
uint32 TIME_intstatus;

/*******************************************************************************
** Name: Timer1_Test_ISR
** Input:void
** Return: void
** Owner:hj
** Date: 2014.12.3
** Time: 15:55:11
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN void Timer1_Test_ISR(void)
{
    pFunc callback;
    pTIMER_REG timerReg;
    TIME_intstatus = 0;
    printf("\r\n ------------Timer11111_Test_ISR------------\n");
    TimerClrInt(TIMER1);
    //TimerStop(TIMER1);
    //callback = g_timerIRQ[0];
    //if (callback)
    {
        //callback();
    }
}
/*******************************************************************************
** Name: Timer0_Test_ISR
** Input:void
** Return: void
** Owner:hj
** Date: 2014.12.3
** Time: 15:52:18
*******************************************************************************/

_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN void Timer0_Test_ISR(void)
{
    pFunc callback;
    pTIMER_REG timerReg;
    TIME_intstatus = 0;
    printf("\r\n ------------Timer00000_Test_ISR------------\n");
    TimerClrInt(TIMER0);
    //TimerStop(TIMER0);
    //callback = g_timerIRQ[0];
    //if (callback)
    {
        //callback();
    }
}
/*******************************************************************************
** Name: TimeDevShellBspDeinit
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 17:56:41
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimeDevShellBspDeinit(HDC dev, uint8 * pstr)
{

}

/*******************************************************************************
** Name: TimeDevShellBspTime_Test
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 17:55:58
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimeDevShellBspTime_Test(HDC dev, uint8 * pstr)
{
    uint64 loadcount = 0;
    uint32 loadCounthigh = 0,loadCountlow = 0;

    uint32 time_reg_num;
    if(StrCmpA(pstr, "0", 1) == 0)
    {
        time_reg_num = 0;
        loadcount = 5*1000000;      //5s
        printf("\r\n Time0 5s start 0\n");
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        time_reg_num = 0;
        loadcount = 10*1000000;      //10s
        printf("\r\n Time0 10s start 0\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        time_reg_num = 0;
        loadcount = 15*1000000;      //15s
        printf("\r\n Time0 15s start 0\n");
    }
    else if(StrCmpA(pstr, "3", 1) == 0)
    {
        time_reg_num = 0;
        loadcount = 20*1000000;      //20s
        printf("\r\n Time0 20s start 0\n");
    }
    else if(StrCmpA(pstr, "4", 1) == 0)
    {
        time_reg_num = 0;
        loadcount = 5400000000;      //5400s
        printf("\r\n Time0 5400s start 0\n");
    }
    //---------------------------------------------------------------//
    else if(StrCmpA(pstr, "5", 1) == 0)
    {
        time_reg_num = 1;
        loadcount = 5*1000000;      //5s
        printf("\r\n Time1 5s start 0\n");
    }
    else if(StrCmpA(pstr, "6", 1) == 0)
    {
        time_reg_num = 1;
        loadcount = 10*1000000;      //10s
        printf("\r\n Time1 10s start 0\n");
    }
    else if(StrCmpA(pstr, "7", 1) == 0)
    {
        time_reg_num = 1;
        loadcount = 15*1000000;      //15s
        printf("\r\n Time1 15s start 0\n");
    }
    else if(StrCmpA(pstr, "8", 1) == 0)
    {
        time_reg_num = 1;
        loadcount = 20*1000000;      //20s
        printf("\r\n Time1 20s start 0\n");
    }
    else if(StrCmpA(pstr, "9", 1) == 0)
    {
        time_reg_num = 1;
        loadcount = 5400000000;      //5400s
        printf("\r\n Time1 5400s start 0\n");
    }

    TIME_intstatus = 1;

    if(0 == time_reg_num)
    {
        IntUnregister(INT_ID_TIMER0);
        IntPendingClear(INT_ID_TIMER0);
        IntDisable(INT_ID_TIMER0);

        TimerStop(TIMER0);
        TimerInit(TIMER0,TIMER_FREE_RUNNING);

        IntRegister(INT_ID_TIMER0 ,Timer0_Test_ISR);
        IntPendingClear(INT_ID_TIMER0);
        IntEnable(INT_ID_TIMER0);

        TimerSetCount(TIMER0,loadcount);

        TimerIntUnmask(TIMER0);

        TimerStart(TIMER0);
    }
    else if(1 == time_reg_num)
    {
        IntUnregister(INT_ID_TIMER1);
        IntPendingClear(INT_ID_TIMER1);
        IntDisable(INT_ID_TIMER1);

        TimerStop(TIMER1);
        TimerInit(TIMER1,TIMER_FREE_RUNNING);

        IntRegister(INT_ID_TIMER1 ,Timer1_Test_ISR);
        IntPendingClear(INT_ID_TIMER1);
        IntEnable(INT_ID_TIMER1);

        TimerSetCount(TIMER1,loadcount);

        TimerIntUnmask(TIMER1);

        TimerStart(TIMER1);
    }
    while(TIME_intstatus);

    printf("\r\ntime init over");

}


#ifdef SHELL_HELP
/*******************************************************************************
** Name: TimeShellHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 17:52:36
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimeShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA( pstr) != 0)
        return RK_ERROR;

    printf("Time命令集提供了一系列的命令对Time进行操作\r\n");
    printf("Time包含的子命令如下:           \r\n");
    printf("help       显示help信息           \r\n");
    printf("bsp        板级支持包命令       \r\n");
    printf("create0    打开timer0             \r\n");
    printf("create1    打开timer1             \r\n");
    printf("test0_5    测试timer0 5秒            \r\n");
    printf("test0_10   测试timer0 10秒            \r\n");
    printf("test1_5    测试timer1 5秒            \r\n");
    printf("test1_10   测试timer1 10秒            \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: TimerShellOpen
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.12
** Time: 14:30:36
*******************************************************************************/
/*
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimerShellCreate (HDC dev, uint8* pstr)
{
    rk_err_t ret;
    int timer_num = *pstr;

    ret = RKDev_Create(DEV_CLASS_TIMER, timer_num, NULL);
    if(ret != RK_SUCCESS)
    {
        printf("Timer%d create failure",timer_num);
        return RK_ERROR;
    }
    printf("Timer%d create successful\n",timer_num);
    return ret;
}
*/
/*******************************************************************************
** Name: TimerShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.11
** Time: 20:08:42
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimerShellCreate(HDC dev, uint8 * pstr)
{
    rk_err_t ret;
    //KEY_DEV_ARG stKeyArg;
    ret = RKDev_Create(DEV_CLASS_TIMER,0,NULL);
    if (ret != RK_SUCCESS)
    {
        printf("TIMER CREATE FRIL\n");
    }
    else
    {
        printf("TIMER CREATE SUCCESS\n");
    }

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: TimerShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:wrm
** Date: 2015.5.12
** Time: 14:30:36
*******************************************************************************/

SHELL FUN rk_err_t TimerShellTest (HDC dev, uint8* pstr, uint8 sec)
{
#ifdef __GUI_GUITASK_C__
    HDC hTimerDev;
    rk_err_t ret;
    //printf("GUITaskShellTest\n");
    hTimerDev =  RKDev_Open(DEV_CLASS_TIMER,0,NOT_CARE);
    if(hTimerDev != NULL)
    {
        printf("TimerDev open success\n");
    }

    printf ("GUITask_Create \n");
    //must create GUItask befor timertask for COMMON callback FUN overlay
    RKTaskCreate(TASK_ID_GUI, 0, NULL);
//    printf ("GUITask_Create TIMER0\n");
    ret = TimerDev_Connect(hTimerDev, TIMER0, 24*1000*1000, 22*1000);
    if (ret != RK_SUCCESS)
    {
        printf ("TIMER0 connect faile\n");
        return RK_ERROR;
    }
    printf ("TIMER0 connect SUCCESS\n");

/*    ret = GUITask_Connect(hTimerDev, TIMER0, 24*1000*1000, 20*1000);
    if(ret == RK_SUCCESS)
    {
       printf("GUITask Start & Timer task start\n");
    }
    */
#endif
    return RK_SUCCESS;
}
//{
/*
    int timer_num = * pstr;//TIMER0
    HDC pDev;
    //DEVICE_CLASS* pstTimerPublicHandler;
    TIMER_DEVICE_CLASS * pstTimerDevHandler;

    pDev = RKDev_Open(DEV_CLASS_TIMER, timer_num, NOT_CARE);
    if(pDev != NULL)
    {
        printf("Timer0Dev open success\n");
    }
    pstTimerDevHandler = (TIMER_DEVICE_CLASS *)pDev;
    pstTimerDevHandler->stTimerDevice.DevID = timer_num;

    TimerDev_Register((HDC)pstTimerDevHandler, 1*1000*1000 ,TimerDev_CallBack);

    //pstTimerPublicHandler = &(pstTimerDevHandler->stTimerDevice);
    //pstTimerPublicHandler->DevID = timer_num;
    //TimerStart(time_type);

    //TimerDev_Start(pstTimerDevHandler);
    //循环的秒数
    TimerDev_PeriodSet(TIMER0, 100*1000, 1*1000*1000);
    //while (gpstTimerDevISRHandler[timer_num]->Clk != sec);
*/
/*
    //TimerStop (time_type);
    TimerDev_Stop(pstTimerDevHandler);
//    TimerInit(timer_num,TIMER_FREE_RUNNING);
    //一个周期为1秒
    TimerSetCount(timer_num,(UINT64)24*1000*1000);
    TimerIntUnmask(timer_num);

    gpstTimerDevISRHandler[timer_num]->Clk = 0;

    rk_printf ("Timer%d test %d second end\n",timer_num,sec);
 */
//    return RK_SUCCESS;
//}


/*******************************************************************************
** Name: TimeDevShellBspHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 17:51:19
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimeDevShellBspHelp(HDC dev, uint8 * pstr)
{
        pstr--;

    if(StrLenA(pstr) != 0)
        return RK_ERROR;

    printf("Time.Bsp命令集提供了一系列的命令对Time板级驱动接口进行测试\r\n");
    printf("测试命令如下:\r\n");
    printf("init      初始化 Time          \r\n");
    printf("deinit    反初始化 Time        \r\n");
    //printf("start     测试I2S             \r\n");
    //printf("stop      关闭I2S             \r\n");
    return RK_SUCCESS;
}

#endif
#ifdef SHELL_BSP

/*******************************************************************************
** Name: TimeDevShellBsp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.11
** Time: 17:53:20
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_SHELL_
SHELL FUN rk_err_t TimeDevShellBsp(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellTimeBspName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                                 //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //bsp help
        #ifdef SHELL_HELP
            ret = TimeDevShellBspHelp(dev,pItem);
        #endif
            break;

        case 0x01:  //test
            ret = TimeDevShellBspTime_Test(dev,pItem);
            break;

        case 0x02:  //deinit
            ret = TimeDevShellBspDeinit(dev,pItem);
            break;
        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}
#endif



#endif

#endif



