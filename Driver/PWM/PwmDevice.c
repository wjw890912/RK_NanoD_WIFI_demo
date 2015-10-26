/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\PWM\PwmDevice.c
* Owner: chad.Ma
* Date: 2014.11.10
* Time: 17:45:27
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    chad.Ma     2014.11.10     17:45:27   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_PWM_PWMDEVICE_C__

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



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _PWM_DEVICE_CLASS
{
    DEVICE_CLASS stPwmDevice;
    uint32 channel;
    pSemaphore osPwmReadReqSem;
    pSemaphore osPwmReadSem;
    pSemaphore osPwmWriteReqSem;
    pSemaphore osPwmWriteSem;
    pSemaphore osPwmControlReqSem;

}PWM_DEVICE_CLASS;


#define PWM_FREQ        10000     //can not exceed 10KHZ
#define BL_PWM_CH       1

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static PWM_DEVICE_CLASS * gpstPwmDevISR;



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
void PwmDevShellIsrServer4(void);
void PwmDevShellIsrServer0(void);
void PwmDevShellIsrHandle(ePWM_CHN pwmChannel);
rk_err_t PwmDevShellBspStart(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBspStop(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBspSetRate(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBspSetFreq(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBspIrqTest(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBspHelp(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellHelp(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellBsp(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellTest(HDC dev, uint8 * pstr, uint8 ch);
rk_err_t PwmDevShellDel(HDC dev, uint8 * pstr);
rk_err_t PwmDevShellCreate(HDC dev, uint8 * pstr, uint8 ch);
rk_err_t PwmDevShellPcb(HDC dev, uint8 * pstr);
void PwmDevIntIsr(void);
rk_err_t PwmDevDeInit(PWM_DEVICE_CLASS * pstPwm);
rk_err_t PwmDevInit(PWM_DEVICE_CLASS * pstPwm);
rk_err_t PwmDevResume(HDC dev);
rk_err_t PwmDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: PwmDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_COMMON_
COMMON API rk_err_t PwmDev_Read(HDC dev)
{
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
** Name: PwmDevIntIsr
** Input:void
** Return: void
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_COMMON_
COMMON FUN void PwmDevIntIsr(void)
{
}

/*******************************************************************************
** Name: PwmDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_COMMON_
COMMON FUN rk_err_t PwmDevResume(HDC dev)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_COMMON_
COMMON FUN rk_err_t PwmDevSuspend(HDC dev)
{
    return RK_SUCCESS;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: PwmDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_COMMON_
COMMON API rk_err_t PwmDev_Write(HDC dev)
{
    return RK_SUCCESS;
}



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
** Name: PwmDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_INIT_
INIT API rk_err_t PwmDev_Delete(uint32 DevID, void * arg)
{
#if 0
    PWM_DEVICE_CLASS* PwmDev = (PWM_DEVICE_CLASS *)dev;;
    gpstPwmDevISR = NULL;
    rkos_semaphore_delete(PwmDev->osPwmReadSem);
    rkos_semaphore_delete(PwmDev->osPwmWriteReqSem);
    rkos_semaphore_delete(PwmDev->osPwmWriteSem);
    rkos_semaphore_delete(PwmDev->osPwmControlReqSem);

    rkos_memory_free(PwmDev);

    printf("PwmDev_Delete\n");
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_PWM_DEV);
#endif
#endif
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_INIT_
INIT API HDC PwmDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS*     DevClass;
    PWM_DEVICE_CLASS* PwmDev;
    uint32 channel;

    PwmDev  = (PWM_DEVICE_CLASS*)rkos_memory_malloc(sizeof(PWM_DEVICE_CLASS));
    if (PwmDev == NULL)
    {
        return PwmDev;
    }
    PwmDev->osPwmReadReqSem    = rkos_semaphore_create(1,1);
    PwmDev->osPwmReadSem       = rkos_semaphore_create(1,1);
    PwmDev->osPwmWriteReqSem   = rkos_semaphore_create(1,1);
    PwmDev->osPwmWriteSem      = rkos_semaphore_create(1,1);
    PwmDev->osPwmControlReqSem = rkos_semaphore_create(1,0);

    if ((      PwmDev->osPwmReadReqSem
               && PwmDev->osPwmReadSem
               && PwmDev->osPwmWriteReqSem
               && PwmDev->osPwmWriteSem
               && PwmDev->osPwmControlReqSem) == 0)
    {
        rkos_semaphore_delete(PwmDev->osPwmReadReqSem);
        rkos_semaphore_delete(PwmDev->osPwmReadSem);
        rkos_semaphore_delete(PwmDev->osPwmWriteReqSem);
        rkos_semaphore_delete(PwmDev->osPwmWriteSem);
        rkos_semaphore_delete(PwmDev->osPwmControlReqSem);

        rkos_memory_free(PwmDev);
        printf("create PwmDev fail");
        return (HDC) RK_ERROR;
    }

    *PwmDev = *((PWM_DEVICE_CLASS*)arg);

    channel = PwmDev->channel;

    rk_printf ("channel=%d\n",channel);
    PwmDevHwInit (DevID,channel);
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_PWM_DEV, MODULE_OVERLAY_CODE);
#endif
    rk_printf ("PwmDev Create success\n");
    if (PwmDevDeInit(PwmDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(PwmDev->osPwmReadReqSem);
        rkos_semaphore_delete(PwmDev->osPwmReadSem);
        rkos_semaphore_delete(PwmDev->osPwmWriteReqSem);
        rkos_semaphore_delete(PwmDev->osPwmWriteSem);
        rkos_semaphore_delete(PwmDev->osPwmControlReqSem);

        rkos_memory_free(PwmDev);

        printf("create PwmDev fail");
        return (HDC) RK_ERROR;
    }

    gpstPwmDevISR = PwmDev;

    return &(PwmDev->stPwmDevice);
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: PwmDevDeInit
** Input:PWM_DEVICE_CLASS * pstPwm
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_INIT_
INIT FUN rk_err_t PwmDevDeInit(PWM_DEVICE_CLASS * pstPwm)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevInit
** Input:PWM_DEVICE_CLASS * pstPwm
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_INIT_
INIT FUN rk_err_t PwmDevInit(PWM_DEVICE_CLASS * pstPwm)
{
    return RK_SUCCESS;
}


#ifdef _PWM_DEV_SHELL_
_DRIVER_PWM_PWMDEVICE_SHELL_
static char* ShellPwmName[] =
{
    "pcb",
    "create",
    "del",
    "test",
    "bsp",
    "help",
    "\b",
};

#ifdef SHELL_BSP
static uint8 * ShellPwmBspName[] =
{
    "help",
    "start",
    "stop",
    "setrate",
    "setfreq",
    "irqtest",
    "\b",

};


uint8       gPWMChannel;
uint8       gPWMInt_flag;
uint32      gPWMRate;       //reference/capture
uint32      gPWMFreq;       //freq
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: PwmDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL API rk_err_t PwmDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellPwmName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                      //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = PwmDevShellPcb(dev,pItem);
            break;

        case 0x01:  //mc pwm0
            *pItem = 0;
            ret = PwmDevShellCreate(dev,pItem,0);
            break;

        case 0x02:  //del
            ret = PwmDevShellDel(dev,pItem);
            break;

        case 0x03:  //test pwm0
            ret = PwmDevShellTest(dev,pItem,0);
            break;

        case 0x04:  //bsp
            #ifdef SHELL_BSP
            ret = PwmDevShellBsp(dev,pItem);
            #endif
            break;
         case 0x05: //help
           #ifdef SHELL_HELP
           ret = PwmDevShellHelp(dev,pItem);
           #endif
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
#ifdef SHELL_BSP
/*******************************************************************************
** Name: PwmDevShellIsrServer1
** Input:void
** Return: vois
** Owner:chad.Ma
** Date: 2014.12.1
** Time: 17:31:50
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN void PwmDevShellIsrServer4(void)
{
    PwmDevShellIsrHandle(PWM_CHN4);
}

/*******************************************************************************
** Name: PwmDevShellIsrServer
** Input:void
** Return: void
** Owner:chad.Ma
** Date: 2014.12.1
** Time: 17:30:00
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN void PwmDevShellIsrServer0(void)
{
    PwmDevShellIsrHandle(PWM_CHN0);
}

/*******************************************************************************
** Name: PwmDevShellIsrHandle
** Input:void
** Return: void
** Owner:chad.Ma
** Date: 2014.11.24
** Time: 16:44:21
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN void PwmDevShellIsrHandle(ePWM_CHN pwmChannel )
{
    gPWMInt_flag = 1;
    PWM_SetIntDisable( pwmChannel );
    PWM_ClrInt( pwmChannel );
//    printf( "pwm channel[%d] interrupt handle out~ \r\n",pwmChannel);
}

/*******************************************************************************
** Name: PwmDevShellBspHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:06:11
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspHelp(HDC dev, uint8 * pstr)
{
     pstr--;

    if(StrLenA(pstr) != 0)
        return RK_ERROR;

    printf("pwm.bsp?????????????pwm??????????\r\n");
    printf("??????:\r\n");
    printf("help :        pwm.bsp ??????? \r\n");
    printf("start :       ??pwm\r\n");
    printf("stop  :       ??pwm\r\n");
    printf("setrate :     ??pwm???\r\n");
    printf("setfreq :     ???? \r\n");
    printf("irqtest :     ??pwm??????r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellBspStart
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:08:26
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspStart(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.bsp.start ??pwm\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    gPWMChannel = PWM_CHN0;
    PWM_Start(gPWMChannel);

    //open rst uart ip
    ScuSoftResetCtr(RST_PWM, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_PWM, 0);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmShellBspStop
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:08:01
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspStop(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.bsp.stop ??pwm\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:

    gPWMChannel = PWM_CHN0;
    PWM_Start(gPWMChannel);

    //open rst uart ip
    ScuSoftResetCtr(RST_PWM, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_PWM, 0);

    PWM_Stop( gPWMChannel );

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmShellBspSetRate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:07:37
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspSetRate(HDC dev, uint8 * pstr)
{
    UINT32 rate;    //reference / capture
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.bsp.setrate  ??pwm???\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    while(pstr[0] == ' ')
        pstr++;

    rate = StringtoNum(pstr);
    if( rate > 100 )
    {
        printf("???0-100??????,????100. \r\n");
        return RK_PARA_ERR;
    }

    gPWMChannel = PWM_CHN0;
    PWM_Start(gPWMChannel);

    //open rst uart ip
    ScuSoftResetCtr(RST_PWM, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_PWM, 0);

    PwmRegReset(gPWMChannel);
    gPWMFreq = 10000;
    PwmRateSet(gPWMChannel , rate, gPWMFreq);
//    PWM_Stop( gPWMChannel );

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmShellBspSetFreq
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:07:18
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspSetFreq(HDC dev, uint8 * pstr)
{
    uint32 freq;
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.bsp.setfreq ??pwm??\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    while(pstr[0] == ' ')
        pstr++;

    freq = StringtoNum(pstr);
    if(freq > 10000)
    {
        printf("warinning :pwm??????10khz,please try again.\r\n");
        return RK_PARA_ERR;
    }

    gPWMChannel = PWM_CHN0;
    PWM_Start(gPWMChannel);

    //open rst uart ip
    ScuSoftResetCtr(RST_PWM, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_PWM, 0);

    PwmRegReset(gPWMChannel);
    gPWMRate = 50;
    PwmRateSet(gPWMChannel,gPWMRate,freq);
//    PWM_Stop( gPWMChannel );
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmShellBspIrqTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 18:06:59
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBspIrqTest(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.bsp.irqtest pwm????\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    gPWMChannel = PWM_CHN0;
    gPWMInt_flag = 0;

    PWM_Start(gPWMChannel);

    //open rst uart ip
    ScuSoftResetCtr(RST_PWM, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_PWM, 0);

    switch(gPWMChannel)
    {
        case PWM_CHN0:
        case PWM_CHN1:
        case PWM_CHN2:
        case PWM_CHN3:
            IntRegister(INT_ID_PWM0 ,PwmDevShellIsrServer0);
            IntPendingClear(INT_ID_PWM0);
            IntEnable(INT_ID_PWM0);
            break;

        case PWM_CHN4:
            IntRegister(INT_ID_PWM1 ,PwmDevShellIsrServer4);
            IntPendingClear(INT_ID_PWM1);
            IntEnable(INT_ID_PWM1);
            break;

         default:
            break;
    }

    gPWMRate = 50;
    gPWMFreq = 10000;

    PwmRegReset(gPWMChannel);
    PWM_SetIntEnable(gPWMChannel);
    PwmRateSet(gPWMChannel , gPWMRate, gPWMFreq);

    while(!gPWMInt_flag);

    if(gPWMInt_flag)
    {
        gPWMInt_flag = 0;
    }

    rk_print_string("pwm IRQ test over.\r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellBsp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:54:08
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellBsp(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellPwmBspName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                        //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //bsp help
            #ifdef SHELL_HELP
            ret = PwmDevShellBspHelp(dev,pItem);
            #endif

            break;

        case 0x01:  //start
            ret = PwmDevShellBspStart(dev,pItem);
            break;

        case 0x02:  //stop
            ret = PwmDevShellBspStop(dev,pItem);
            break;

        case 0x03:  //set rate
            ret = PwmDevShellBspSetRate(dev,pItem);
            break;

        case 0x04:  //set freq
            ret = PwmDevShellBspSetFreq(dev,pItem);
            break;

        case 0x05:  //irq test
            ret = PwmDevShellBspIrqTest(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}
#endif
/*******************************************************************************
** Name: PwmDevShellHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:54:37
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA( pstr) != 0)
        return RK_ERROR;

    printf("pwm?????????????pwm????\r\n");
    printf("????:\r\n");
    printf("pcb       ??pcb??         \r\n");
    printf("mc        mc??              \r\n");
    printf("del       ??pwm device?? \r\n");
    printf("test      ????            \r\n");
    printf("bsp       ???????      \r\n");
    printf("help      ??pwm??????\r\n");
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellTest(HDC dev, uint8 * pstr, uint8 ch)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.test  : test pwm device.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...
    rk_printf ("Enrty Pwm test\n");
    PwmRateSet(ch,20,PWM_FREQ);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellDel(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.del  : ??pwm device.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellCreate(HDC dev, uint8 * pstr, uint8 ch)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.mc  : ??pwm device.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...
    PWM_DEVICE_CLASS  PwmDev;
    DEVICE_CLASS      DevClass;
    uint32 DevID;
    uint32 channel;
    rk_err_t ret;

    DevID = *pstr;
    channel = ch;

    PwmDev.stPwmDevice.DevID = DevID;
    PwmDev.channel = channel;

    //Create PWMDev...
    ret = RKDev_Create(DEV_CLASS_PWM, DevID, &PwmDev);
    if(ret != RK_SUCCESS)
    {
        rk_print_string("PWMDev create failure\n");
    }
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: PwmDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 17:45:44
*******************************************************************************/
_DRIVER_PWM_PWMDEVICE_SHELL_
SHELL FUN rk_err_t PwmDevShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("pwm.pcb : pwm pcb info.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

#endif
#endif

