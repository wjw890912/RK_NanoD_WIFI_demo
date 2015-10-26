/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\Bcore\BcoreDevice.c
* Owner: aaron.sun
* Date: 2015.6.26
* Time: 10:18:52
* Version: 1.0
* Desc: bb system driver
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.26     10:18:52   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __DRIVER_BCORE_BCOREDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define BCORE_DEV_NUM 2

typedef  struct _BCORE_DEVICE_CLASS
{
    DEVICE_CLASS stBcoreDevice;
    pSemaphore osBcoreDebugSem;
    pSemaphore osBcoreOkSem;

    uint32 Channel;

}BCORE_DEVICE_CLASS;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static BCORE_DEVICE_CLASS * gpstBcoreDevISR[2];



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
rk_err_t BcoreDevShellTest(HDC dev, uint8 * pstr);
rk_err_t BcoreDevShellDel(HDC dev, uint8 * pstr);
rk_err_t BcoreDevShellCreate(HDC dev, uint8 * pstr);
rk_err_t BcoreDevShellPcb(HDC dev, uint8 * pstr);
void BcoreDevIntIsr1(void);
void BcoreDevIntIsr0(void);
void BcoreDevIntIsr(uint32 DevID);
rk_err_t BcoreDevDeInit(BCORE_DEVICE_CLASS * pstBcoreDev);
rk_err_t BcoreDevInit(BCORE_DEVICE_CLASS * pstBcoreDev);
rk_err_t BcoreDevResume(HDC dev);
rk_err_t BcoreDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: BcoreDev_ShutOff
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:54:36
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON API rk_err_t BcoreDev_ShutOff(HDC dev)
{
   BCORE_DEVICE_CLASS * pstBcoreDev =  (BCORE_DEVICE_CLASS *)dev;
   ScuSoftResetCtr(CAL_CORE_SRST, TRUE);
   ScuClockGateCtr(HCLK_CAL_CORE_GATE,0);
   DEBUG("shut off bb system OK");
}

/*******************************************************************************
** Name: BcoreDev_Task_Enter
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON API void BcoreDev_Task_Enter(void)
{
    //rk_printf("b core enter....");
    while(1)
    {
        rkos_semaphore_take(gpstBcoreDevISR[0]->osBcoreDebugSem,MAX_DELAY);
        BBDebug();
    }
}

/*******************************************************************************
** Name: BcoreDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON API rk_err_t BcoreDev_Write(HDC dev, uint32 Cmd, uint32 Data, uint32 port)
{
    //BcoreDev Write...
    BCORE_DEVICE_CLASS * pstBcoreDev =  (BCORE_DEVICE_CLASS *)dev;
    if(pstBcoreDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: BcoreDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON API rk_err_t BcoreDev_Start(HDC dev, uint32 ModuleId)
{
    BCORE_DEVICE_CLASS * pstBcoreDev =  (BCORE_DEVICE_CLASS *)dev;


    ScuClockGateCtr(HCLK_CAL_CORE_GATE,1);

    ScuSoftResetCtr(CAL_CORE_SRST, TRUE);

    FW_ModuleOverlay(ModuleId, MODULE_OVERLAY_ALL);

    memcpy((void *)0x01020000, &chip_freq, sizeof(chip_freq_t));

    ScuSoftResetCtr(CAL_CORE_SRST, FALSE);

    DEBUG("start bb system...");

    rkos_semaphore_take(pstBcoreDev->osBcoreOkSem, MAX_DELAY);

    #ifdef BB_SYS_JTAG
    memcpy((void *)0x01020000, &chip_freq, sizeof(chip_freq_t));

    rkos_semaphore_take(pstBcoreDev->osBcoreOkSem, MAX_DELAY);

    #endif

    #ifdef _LOG_DEBUG_
    DEBUG("start bb system OK");
    #endif

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: BcoreDevIntIsr1
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON FUN void BcoreDevIntIsr1(void)
{
    //Call total int service...
    BcoreDevIntIsr(1);

}
/*******************************************************************************
** Name: BcoreDevIntIsr0
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON FUN void BcoreDevIntIsr0(void)
{
    //Call total int service...
    BcoreDevIntIsr(0);

}
/*******************************************************************************
** Name: BcoreDevIntIsr
** Input:uint32 DevID
** Return: void
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON FUN void BcoreDevIntIsr(uint32 DevID)
{
    uint32 BcoreDevIntType;


    //Get BcoreDev Int type...
    //BcoreDevIntType = GetIntType();
    if (gpstBcoreDevISR[DevID] != NULL)
    {
        uint32 Cmd;
        uint32 Data;

        MailBoxClearB2AInt(MAILBOX_ID_0,  MAILBOX_INT_0);
        Cmd = MailBoxReadB2ACmd(MAILBOX_ID_0, MAILBOX_CHANNEL_0);

        switch(Cmd)
        {
            case MSGBOX_CMD_SYSTEM_PRINT_LOG:
                #ifdef _LOG_DEBUG_
                BBReqDebug((uint8 *)MailBoxReadB2AData(MAILBOX_ID_0,MAILBOX_CHANNEL_0));
                rkos_semaphore_give_fromisr(gpstBcoreDevISR[DevID]->osBcoreDebugSem);
                #endif

                break;

            case MSGBOX_CMD_SYSTEM_START_OK:
                rkos_semaphore_give_fromisr(gpstBcoreDevISR[DevID]->osBcoreOkSem);
                break;
        }
    }

}
/*******************************************************************************
** Name: BcoreDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON FUN rk_err_t BcoreDevResume(HDC dev)
{
    //BcoreDev Resume...
    BCORE_DEVICE_CLASS * pstBcoreDev =  (BCORE_DEVICE_CLASS *)dev;
    if(pstBcoreDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: BcoreDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_COMMON_
COMMON FUN rk_err_t BcoreDevSuspend(HDC dev)
{
    //BcoreDev Suspend...
    BCORE_DEVICE_CLASS * pstBcoreDev =  (BCORE_DEVICE_CLASS *)dev;
    if(pstBcoreDev == NULL)
    {
        return RK_ERROR;
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: BcoreDev_Task_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT API rk_err_t BcoreDev_Task_DeInit(void *pvParameters)
{
    RK_TASK_CLASS* pBcoreDevTask = (RK_TASK_CLASS*)pvParameters;
    //uint8 i;
    //AudioStop(Audio_Stop_Force);
    rk_printf("BcoreDev_Task_DeInit id=%d\n",pBcoreDevTask->TaskObjectID);
/*
    if (gpstBcoreDevISR[0] == NULL)
    {
        return RK_ERROR;
    }

    if(gpstBcoreDevISR[0]->osBcoreDebugSem)
        rkos_semaphore_delete(gpstBcoreDevISR[0]->osBcoreDebugSem);
    if(gpstBcoreDevISR[0]->osBcoreOkSem)
        rkos_semaphore_delete(gpstBcoreDevISR[0]->osBcoreOkSem);

    gpstBcoreDevISR[0]->osBcoreDebugSem = NULL;
    gpstBcoreDevISR[0]->osBcoreOkSem = NULL;

    if(gpstBcoreDevISR[0])
        rkos_memory_free(gpstBcoreDevISR[0]);
    gpstBcoreDevISR[0] = NULL;
*/
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: BcoreDev_Task_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT API rk_err_t BcoreDev_Task_Init(void *pvParameters)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: BcoreDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT API rk_err_t BcoreDev_Delete(uint32 DevID, void * arg)
{
    //Check BcoreDev is not exist...
    if(gpstBcoreDevISR[DevID] == NULL)
    {
        return RK_ERROR;
    }

    //BcoreDev deinit...
    BcoreDevDeInit(gpstBcoreDevISR[DevID]);

    //Free BcoreDev memory...

    rkos_memory_free(gpstBcoreDevISR[DevID]);

    //Delete BcoreDev...
    gpstBcoreDevISR[DevID] = NULL;

    //Delete BcoreDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_BCORE_DEV);
#endif

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: BcoreDev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT API HDC BcoreDev_Create(uint32 DevID, void * arg)
{
    BCORE_DEV_ARG * pstBcoreDevArg;
    DEVICE_CLASS* pstDev;
    BCORE_DEVICE_CLASS * pstBcoreDev;

    if(arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstBcoreDevArg = (BCORE_DEV_ARG *)arg;
    pstBcoreDev =  rkos_memory_malloc(sizeof(BCORE_DEVICE_CLASS));
    memset(pstBcoreDev, 0, sizeof(BCORE_DEVICE_CLASS));
    if (pstBcoreDev == NULL)
    {
        return NULL;
    }

    //init handler...
    pstBcoreDev->osBcoreDebugSem  = rkos_semaphore_create(1,0);
    pstBcoreDev->osBcoreOkSem  = rkos_semaphore_create(1,0);

    if(((pstBcoreDev->osBcoreDebugSem) == 0)
        || ((pstBcoreDev->osBcoreOkSem) == 0))
    {
        rkos_semaphore_delete(pstBcoreDev->osBcoreDebugSem);
        rkos_semaphore_delete(pstBcoreDev->osBcoreOkSem);
        rkos_memory_free(pstBcoreDev);
        return (HDC) RK_ERROR;
    }
    pstDev = (DEVICE_CLASS *)pstBcoreDev;
    pstDev->suspend = BcoreDevSuspend;
    pstDev->resume  = BcoreDevResume;

    //init arg...
    pstBcoreDev->Channel = pstBcoreDevArg->Channel;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_BCORE_DEV, MODULE_OVERLAY_CODE);
#endif

    //device init...
    gpstBcoreDevISR[DevID] = NULL;
    BcoreHwInit(DevID, pstBcoreDev->Channel);
    if(BcoreDevInit(pstBcoreDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstBcoreDev->osBcoreDebugSem);
        rkos_semaphore_delete(pstBcoreDev->osBcoreOkSem);
        rkos_memory_free(pstBcoreDev);
        return (HDC) RK_ERROR;
    }
    gpstBcoreDevISR[DevID] = pstBcoreDev;
    return (HDC)pstDev;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: BcoreDevDeInit
** Input:BCORE_DEVICE_CLASS * pstBcoreDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT FUN rk_err_t BcoreDevDeInit(BCORE_DEVICE_CLASS * pstBcoreDev)
{
    IntDisable(INT_ID_MAILBOX0);

    MailBoxDisableB2AInt(MAILBOX_ID_0, MAILBOX_INT_0);

    IntPendingClear(MAILBOX_ID_0);

    IntUnregister(INT_ID_MAILBOX0);

    PmuPdLogicPowerDown(0);

    ScuClockGateCtr(PCLK_MAILBOX_GATE,1);
    ScuClockGateCtr(HCLK_ASYNC_BRG_GATE,1);
    ScuClockGateCtr(HDRAM0_GATE,1);
    ScuClockGateCtr(HDRAM1_GATE,1);
    ScuClockGateCtr(HDRAM2_GATE,1);
    ScuClockGateCtr(HDRAM3_GATE,1);
    ScuClockGateCtr(HDRAM4_GATE,1);
    ScuClockGateCtr(HDRAM5_GATE,1);
    ScuClockGateCtr(HDRAM6_GATE,1);
    ScuClockGateCtr(HDRAM7_GATE,1);
    ScuClockGateCtr(HDRAM_INTERFACE_GATE,1);
    ScuClockGateCtr(HIRAM0_GATE,1);
    ScuClockGateCtr(HIRAM1_GATE,1);
    ScuClockGateCtr(HIRAM2_GATE,1);
    ScuClockGateCtr(HIRAM3_GATE,1);
    ScuClockGateCtr(HIRAM_INTERFACE_GATE,1);
    ScuClockGateCtr(HCLK_HIGH_GATE,1);
    ScuClockGateCtr(HCLK_HIGH_MATRIX_GATE,1);
    ScuClockGateCtr(HCLK_ASYNC_BRG_GATE,1);
    ScuClockGateCtr(HCLK_CAL_CORE_GATE,1);

    ScuSoftResetCtr(MAILBOX_SRST, FALSE);
    ScuSoftResetCtr(ASYNC_BRG_SRST, FALSE);

    ScuSoftResetCtr(HIGH_MATRIX_SRST, FALSE);
    ScuSoftResetCtr(HIGHRAM1_SRST, FALSE);
    ScuSoftResetCtr(HIGHRAM0_SRST, FALSE);

    rk_printf("enter BcoreDev_Task_DeInit \n");

    if(gpstBcoreDevISR[0]->osBcoreDebugSem)
        rkos_semaphore_delete(gpstBcoreDevISR[0]->osBcoreDebugSem);
    if(gpstBcoreDevISR[0]->osBcoreOkSem)
        rkos_semaphore_delete(gpstBcoreDevISR[0]->osBcoreOkSem);

    gpstBcoreDevISR[0]->osBcoreDebugSem = NULL;
    gpstBcoreDevISR[0]->osBcoreOkSem = NULL;


    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    //FW_RemoveModule(MODULE_ID_AUDIOCONTROL_TASK);
    #endif

    rk_printf("### BcoreDev_Task_DeInit 01\n");

    RKTaskDelete(TASK_ID_BCORE, 0, SYNC_MODE);

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: BcoreDevInit
** Input:BCORE_DEVICE_CLASS * pstBcoreDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT FUN rk_err_t BcoreDevInit(BCORE_DEVICE_CLASS * pstBcoreDev)
{
    MailBoxEnableB2AInt(MAILBOX_ID_0, MAILBOX_INT_0);

    IntRegister(INT_ID_MAILBOX0, (void*)BcoreDevIntIsr0);

    IntEnable(INT_ID_MAILBOX0);

    return RK_SUCCESS;
}


#ifdef _BCORE_DEV_SHELL_
_DRIVER_BCORE_BCOREDEVICE_SHELL_
static char* ShellBcoreName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "...",
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
** Name: BcoreDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_SHELL_
SHELL API rk_err_t BcoreDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellBcoreName, pItem, StrCnt);
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
            ret = BcoreDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = BcoreDevShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = BcoreDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = BcoreDevShellTest(dev,pItem);
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
/*******************************************************************************
** Name: BcoreDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_SHELL_
SHELL FUN rk_err_t BcoreDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hBcoreDev;
    uint32 DevID;

    //Get BcoreDev ID...
    if(StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    //Open BcoreDev...
    hBcoreDev = RKDev_Open(DEV_CLASS_BCORE, 0, NOT_CARE);
    if((hBcoreDev == NULL) || (hBcoreDev == (HDC)RK_ERROR) || (hBcoreDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("BcoreDev open failure");
         return RK_SUCCESS;
    }

    //do test....
    BcoreDev_Start(hBcoreDev, MODULE_ID_BB_CODE);

    //close BcoreDev...
    RKDev_Close(hBcoreDev);
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: BcoreDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_SHELL_
SHELL FUN rk_err_t BcoreDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;

    //Get BcoreDev ID...
    if(StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    rk_print_string("TASK_ID_BCORE success\n");

    if(RKDev_Delete(DEV_CLASS_BCORE, 0, NULL) != RK_SUCCESS)
    {
        rk_print_string("BCOREDev delete failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: BcoreDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_SHELL_
SHELL FUN rk_err_t BcoreDevShellCreate(HDC dev, uint8 * pstr)
{
    BCORE_DEV_ARG stBcoreDevArg;
    rk_err_t ret;
    uint32 DevID;

    if(StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    //Init BcoreDev arg...
    //stBcoreDevArg.usbmode = USBOTG_MODE_DEVICE;

    //Create BcoreDev...
    ret = RKDev_Create(DEV_CLASS_BCORE, DevID, &stBcoreDevArg);
    if(ret != RK_SUCCESS)
    {
        rk_print_string("BcoreDev create failure");
    }

    RKTaskCreate(TASK_ID_BCORE, 0, NULL);

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: BcoreDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 10:19:35
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_SHELL_
SHELL FUN rk_err_t BcoreDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hBcoreDev;
    uint32 DevID;

    //Get BcoreDev ID...
    if(StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    //Display pcb...
    if(gpstBcoreDevISR[DevID] != NULL)
    {
        rk_print_string("rn");
        rk_printf_no_time("BcoreDev ID = %d Pcb list as follow:", DevID);
        rk_printf_no_time("    osBcoreDebugSem---0x%08x", gpstBcoreDevISR[DevID]->osBcoreDebugSem);
        rk_printf_no_time("    osBcoreOkSem---0x%08x", gpstBcoreDevISR[DevID]->osBcoreOkSem);
        //rk_printf_no_time("    usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
        //rk_printf_no_time("    usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
    }
    else
    {
        rk_print_string("rn");
        rk_printf_no_time("BcoreDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}


#endif

#endif
