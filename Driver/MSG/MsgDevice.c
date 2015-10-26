/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\MSG\MsgDevice.c
* Owner: aaron.sun
* Date: 2015.6.23
* Time: 15:58:26
* Version: 1.0
* Desc: msg device class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.23     15:58:26   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __DRIVER_MSG_MSGDEVICE_C__

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
#define MSG_DEV_NUM 2

typedef  struct _MSG_DEVICE_CLASS
{
    DEVICE_CLASS stMsgDevice;
    pSemaphore osMsgReadReqSem;
    pSemaphore osMsgReadSem;
    pSemaphore osMsgWriteReqSem;
    pSemaphore osMsgWriteSem;
    pSemaphore osMsgOperReqSem;
    pSemaphore osMsgOperSem;
    uint32 Channel;

}MSG_DEVICE_CLASS;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static MSG_DEVICE_CLASS * gpstMsgDevISR[MSG_DEV_NUM] = {(MSG_DEVICE_CLASS *)NULL,(MSG_DEVICE_CLASS *)NULL};



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
rk_err_t MsgDevShellTest(HDC dev, uint8 * pstr);
rk_err_t MsgDevShellDel(HDC dev, uint8 * pstr);
rk_err_t MsgDevShellCreate(HDC dev, uint8 * pstr);
rk_err_t MsgDevShellPcb(HDC dev, uint8 * pstr);
void MsgDevIntIsr1(void);
void MsgDevIntIsr0(void);
void MsgDevIntIsr(uint32 DevID);
rk_err_t MsgDevDeInit(MSG_DEVICE_CLASS * pstMsgDev);
rk_err_t MsgDevInit(MSG_DEVICE_CLASS * pstMsgDev);
rk_err_t MsgDevResume(HDC dev);
rk_err_t MsgDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: MsgDev_CheckMsg
** Input:HDC dev,  uint32 MsgID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 16:03:03
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON API rk_err_t MsgDev_CheckMsg(HDC dev,  uint32 MsgID)
{

}

/*******************************************************************************
** Name: MsgDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON API rk_err_t MsgDev_SendMsg(HDC dev, uint32 MsgID)
{
    //MsgDev Write...
    MSG_DEVICE_CLASS * pstMsgDev =  (MSG_DEVICE_CLASS *)dev;
    if(pstMsgDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: MsgDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON API rk_err_t MsgDev_GetMsg(HDC dev, uint32 MsgID)
{
    //MsgDev Read...
    MSG_DEVICE_CLASS * pstMsgDev =  (MSG_DEVICE_CLASS *)dev;
    if(pstMsgDev == NULL)
    {
        return RK_ERROR;
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: MsgDevIntIsr1
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON FUN void MsgDevIntIsr1(void)
{
    //Call total int service...
    MsgDevIntIsr(1);

}
/*******************************************************************************
** Name: MsgDevIntIsr0
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON FUN void MsgDevIntIsr0(void)
{
    //Call total int service...
    MsgDevIntIsr(0);

}
/*******************************************************************************
** Name: MsgDevIntIsr
** Input:uint32 DevID
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON FUN void MsgDevIntIsr(uint32 DevID)
{
    uint32 MsgDevIntType;


    //Get MsgDev Int type...
    //MsgDevIntType = GetIntType();
    if (gpstMsgDevISR[DevID] != NULL)
    {
        //if (MsgDevIntType & INT_TYPE_MAP)
        {
            //write serice code...
        }

        //wirte other int service...
    }

}
/*******************************************************************************
** Name: MsgDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON FUN rk_err_t MsgDevResume(HDC dev)
{
    //MsgDev Resume...
    MSG_DEVICE_CLASS * pstMsgDev =  (MSG_DEVICE_CLASS *)dev;
    if(pstMsgDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: MsgDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_COMMON_
COMMON FUN rk_err_t MsgDevSuspend(HDC dev)
{
    //MsgDev Suspend...
    MSG_DEVICE_CLASS * pstMsgDev =  (MSG_DEVICE_CLASS *)dev;
    if(pstMsgDev == NULL)
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
** Name: MsgDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_INIT_
INIT API rk_err_t MsgDev_Delete(uint32 DevID, void * arg)
{
    //Check MsgDev is not exist...
    if(gpstMsgDevISR[DevID] == NULL)
    {
        return RK_ERROR;
    }

    //MsgDev deinit...
    MsgDevDeInit(gpstMsgDevISR[DevID]);

    //Free MsgDev memory...
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgOperReqSem);
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgOperSem);
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgReadReqSem);
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgReadSem);
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgWriteReqSem);
    rkos_semaphore_delete(gpstMsgDevISR[DevID]->osMsgWriteSem);
    rkos_memory_free(gpstMsgDevISR[DevID]);

    //Delete MsgDev...
    gpstMsgDevISR[DevID] = NULL;

    //Delete MsgDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_MSG_DEV);
#endif

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: MsgDev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_INIT_
INIT API HDC MsgDev_Create(uint32 DevID, void * arg)
{
    MSG_DEV_ARG * pstMsgDevArg;
    DEVICE_CLASS* pstDev;
    MSG_DEVICE_CLASS * pstMsgDev;

    if(arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstMsgDevArg = (MSG_DEV_ARG *)arg;
    pstMsgDev =  rkos_memory_malloc(sizeof(MSG_DEVICE_CLASS));
    memset(pstMsgDev, 0, sizeof(MSG_DEVICE_CLASS));
    if (pstMsgDev == NULL)
    {
        return NULL;
    }

    //init handler...
    pstMsgDev->osMsgOperReqSem  = rkos_semaphore_create(1,1);
    pstMsgDev->osMsgOperSem  = rkos_semaphore_create(1,0);

    pstMsgDev->osMsgReadReqSem  = rkos_semaphore_create(1,1);
    pstMsgDev->osMsgReadSem  = rkos_semaphore_create(1,0);

    pstMsgDev->osMsgWriteReqSem  = rkos_semaphore_create(1,1);
    pstMsgDev->osMsgWriteSem  = rkos_semaphore_create(1,0);
    if(((pstMsgDev->osMsgOperReqSem) == 0)
        || ((pstMsgDev->osMsgOperSem) == 0)
        || ((pstMsgDev->osMsgReadReqSem) == 0)
        || ((pstMsgDev->osMsgReadSem) == 0)
        || ((pstMsgDev->osMsgWriteReqSem) == 0)
        || ((pstMsgDev->osMsgWriteSem) == 0))
    {
        rkos_semaphore_delete(pstMsgDev->osMsgOperReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgOperSem);
        rkos_semaphore_delete(pstMsgDev->osMsgReadReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgReadSem);
        rkos_semaphore_delete(pstMsgDev->osMsgWriteReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgWriteSem);
        rkos_memory_free(pstMsgDev);
        return (HDC) RK_ERROR;
    }
    pstDev = (DEVICE_CLASS *)pstMsgDev;
    pstDev->suspend = MsgDevSuspend;
    pstDev->resume  = MsgDevResume;

    //init arg...
    pstMsgDev->Channel = pstMsgDevArg->Channel;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_MSG_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    gpstMsgDevISR[DevID] = NULL;
    //MsgHwInit(DevID, pstMsgDev->Channel);
    if(MsgDevInit(pstMsgDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstMsgDev->osMsgOperReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgOperSem);
        rkos_semaphore_delete(pstMsgDev->osMsgReadReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgReadSem);
        rkos_semaphore_delete(pstMsgDev->osMsgWriteReqSem);
        rkos_semaphore_delete(pstMsgDev->osMsgWriteSem);
        rkos_memory_free(pstMsgDev);
        return (HDC) RK_ERROR;
    }
    gpstMsgDevISR[DevID] = pstMsgDev;
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
** Name: MsgDevDeInit
** Input:MSG_DEVICE_CLASS * pstMsgDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_INIT_
INIT FUN rk_err_t MsgDevDeInit(MSG_DEVICE_CLASS * pstMsgDev)
{
    //If ip device first step unregister int
    //IntDisable(INT_ID17_UART);
    //IntPendingClear(INT_ID17_UART);
    //IntUnregister(INT_ID17_UART);

    //If ip device second step close clk
    //ScuClockGateCtr(CLOCK_GATE_UART,0);
    return RK_ERROR;

}
/*******************************************************************************
** Name: MsgDevInit
** Input:MSG_DEVICE_CLASS * pstMsgDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_INIT_
INIT FUN rk_err_t MsgDevInit(MSG_DEVICE_CLASS * pstMsgDev)
{
    //If ip device first step open ip clk...
    //ScuClockGateCtr(CLOCK_GATE_UART, 1);
    //SetUartFreq(24);

    //If ip device second step rst ip...
    //ScuSoftResetCtr(RST_UART, 1);
    //DelayMs(1);
    //ScuSoftResetCtr(RST_UART, 0);

    //If ip device three step call ip bsp function init ip...
    //UARTIntInit(pstUartDev->dwBitRate,pstUartDev->dwBitWidth,UART_ONE_STOPBIT,UART_PARITY_DISABLE);

    //If ip device four step register intreput service...
    //IntRegister(INT_ID17_UART ,UartIntIsr0);
    //IntPendingClear(INT_ID17_UART);
    //IntEnable(INT_ID17_UART);
    //some ip device,such as uart ip device, only a read fifo and a write fifo,
    //we can use fifo as buffer for save some data when shut off Ip int
    return RK_ERROR;

}


#ifdef _MSG_DEV_SHELL_
_DRIVER_MSG_MSGDEVICE_SHELL_
static char* ShellMsgName[] =
{
    "pcb",
    "create",
    "del",
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
** Name: MsgDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_SHELL_
SHELL API rk_err_t MsgDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellMsgName, pItem, StrCnt);
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
            ret = MsgDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = MsgDevShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = MsgDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = MsgDevShellTest(dev,pItem);
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
** Name: MsgDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_SHELL_
SHELL FUN rk_err_t MsgDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hMsgDev;
    uint32 DevID;

    //Get MsgDev ID...
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

    //Open MsgDev...
    hMsgDev = RKDev_Open(DEV_CLASS_MSG, 0, NOT_CARE);
    if((hMsgDev == NULL) || (hMsgDev == (HDC)RK_ERROR) || (hMsgDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("MsgDev open failure");
         return RK_SUCCESS;
    }

    //do test....

    //close MsgDev...
    RKDev_Close(hMsgDev);
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: MsgDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_SHELL_
SHELL FUN rk_err_t MsgDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;

    //Get MsgDev ID...
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
    if(RKDev_Delete(DEV_CLASS_MSG, DevID, NULL) != RK_SUCCESS)
    {
        rk_print_string("MSGDev delete failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: MsgDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_SHELL_
SHELL FUN rk_err_t MsgDevShellCreate(HDC dev, uint8 * pstr)
{
    MSG_DEV_ARG stMsgDevArg;
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

    //Init MsgDev arg...
    //stMsgDevArg.usbmode = USBOTG_MODE_DEVICE;

    //Create MsgDev...
    ret = RKDev_Create(DEV_CLASS_MSG, DevID, &stMsgDevArg);
    if(ret != RK_SUCCESS)
    {
        rk_print_string("MsgDev create failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: MsgDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:59:32
*******************************************************************************/
_DRIVER_MSG_MSGDEVICE_SHELL_
SHELL FUN rk_err_t MsgDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hMsgDev;
    uint32 DevID;

    //Get MsgDev ID...
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
    if(gpstMsgDevISR[DevID] != NULL)
    {
        rk_print_string("rn");
        rk_printf_no_time("MsgDev ID = %d Pcb list as follow:", DevID);
        rk_printf_no_time("    &stMsgDevice---0x%08x", &gpstMsgDevISR[DevID]->stMsgDevice);
        rk_printf_no_time("    osMsgOperReqSem---0x%08x", gpstMsgDevISR[DevID]->osMsgOperReqSem);
        rk_printf_no_time("    osMsgOperSem---0x%08x", gpstMsgDevISR[DevID]->osMsgOperSem);
        rk_printf_no_time("    osMsgReadReqSem---0x%08x", gpstMsgDevISR[DevID]->osMsgReadReqSem);
        rk_printf_no_time("    osMsgReadSem---0x%08x", gpstMsgDevISR[DevID]->osMsgReadSem);
        rk_printf_no_time("    osMsgWriteReqSem---0x%08x", gpstMsgDevISR[DevID]->osMsgWriteReqSem);
        rk_printf_no_time("    osMsgWriteSem---0x%08x", gpstMsgDevISR[DevID]->osMsgWriteSem);
        //rk_printf_no_time("    usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
        //rk_printf_no_time("    usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
    }
    else
    {
        rk_print_string("rn");
        rk_printf_no_time("MsgDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}


#endif

#endif
