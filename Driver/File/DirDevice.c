/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\File\DirDevice.c
* Owner: aaron.sun
* Date: 2015.6.23
* Time: 13:54:25
* Version: 1.0
* Desc: dir device class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.23     13:54:25   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __DRIVER_FILE_DIRDEVICE_C__

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
#define DIR_DEV_NUM 2

typedef  struct _DIR_DEVICE_CLASS
{
    DEVICE_CLASS stDirDevice;
    pSemaphore osDirReadReqSem;
    pSemaphore osDirReadSem;
    pSemaphore osDirWriteReqSem;
    pSemaphore osDirWriteSem;
    pSemaphore osDirOperReqSem;
    pSemaphore osDirOperSem;
    uint32 Channel;

}DIR_DEVICE_CLASS;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static DIR_DEVICE_CLASS * gpstDirDevISR[DIR_DEV_NUM] = {(DIR_DEVICE_CLASS *)NULL,(DIR_DEVICE_CLASS *)NULL};



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
rk_err_t DirDev_ChangeDir(HDC dev);
rk_err_t DirDevShellTest(HDC dev, uint8 * pstr);
rk_err_t DirDevShellDel(HDC dev, uint8 * pstr);
rk_err_t DirDevShellCreate(HDC dev, uint8 * pstr);
rk_err_t DirDevShellPcb(HDC dev, uint8 * pstr);
void DirDevIntIsr1(void);
void DirDevIntIsr0(void);
void DirDevIntIsr(uint32 DevID);
rk_err_t DirDevDeInit(DIR_DEVICE_CLASS * pstDirDev);
rk_err_t DirDevInit(DIR_DEVICE_CLASS * pstDirDev);
rk_err_t DirDevResume(HDC dev);
rk_err_t DirDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DirDev_GetCurFilePath
** Input:HDC dev, uint16 * path
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 10:08:31
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_GetCurFilePath(HDC dev, uint16 * path)
{

}

/*******************************************************************************
** Name: DirDev_GotoNextDir
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:18:36
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_GotoNextDir(HDC dev)
{

}

/*******************************************************************************
** Name: DirDev_GotoRootDir
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:16:14
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_GotoRootDir(HDC dev)
{

}

/*******************************************************************************
** Name: DirDev_GetCurFileNum
** Input:uint32 FileNum
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:08:41
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_GetCurFileNum(HDC dev, uint32 FileNum, uint32 * CurFileNum)
{

}

/*******************************************************************************
** Name: DirDev_BuildDirInfo
** Input:uint8* ExtName, FS_TYPE FsType, uint32 * TotalNum
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:00:08
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API HDC DirDev_BuildDirInfo(HDC dev, uint8* ExtName, FS_TYPE FsType, uint32 * TotalNum)
{

}

/*******************************************************************************
** Name: DirDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_Write(HDC dev)
{
    //DirDev Write...
    DIR_DEVICE_CLASS * pstDirDev =  (DIR_DEVICE_CLASS *)dev;
    if(pstDirDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: DirDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON API rk_err_t DirDev_Read(HDC dev)
{
    //DirDev Read...
    DIR_DEVICE_CLASS * pstDirDev =  (DIR_DEVICE_CLASS *)dev;
    if(pstDirDev == NULL)
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
** Name: DirDev_ChangeDir
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 15:22:32
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN rk_err_t DirDev_ChangeDir(HDC dev)
{

}

/*******************************************************************************
** Name: DirDevIntIsr1
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN void DirDevIntIsr1(void)
{
    //Call total int service...
    DirDevIntIsr(1);

}
/*******************************************************************************
** Name: DirDevIntIsr0
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN void DirDevIntIsr0(void)
{
    //Call total int service...
    DirDevIntIsr(0);

}
/*******************************************************************************
** Name: DirDevIntIsr
** Input:uint32 DevID
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN void DirDevIntIsr(uint32 DevID)
{
    uint32 DirDevIntType;


    //Get DirDev Int type...
    //DirDevIntType = GetIntType();
    if (gpstDirDevISR[DevID] != NULL)
    {
        //if (DirDevIntType & INT_TYPE_MAP)
        {
            //write serice code...
        }

        //wirte other int service...
    }

}
/*******************************************************************************
** Name: DirDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:26
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN rk_err_t DirDevResume(HDC dev)
{
    //DirDev Resume...
    DIR_DEVICE_CLASS * pstDirDev =  (DIR_DEVICE_CLASS *)dev;
    if(pstDirDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: DirDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:26
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_COMMON_
COMMON FUN rk_err_t DirDevSuspend(HDC dev)
{
    //DirDev Suspend...
    DIR_DEVICE_CLASS * pstDirDev =  (DIR_DEVICE_CLASS *)dev;
    if(pstDirDev == NULL)
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
** Name: DirDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_INIT_
INIT API rk_err_t DirDev_Delete(uint32 DevID, void *arg)
{
    //Check DirDev is not exist...
    if(gpstDirDevISR[DevID] == NULL)
    {
        return RK_ERROR;
    }

    //DirDev deinit...
    DirDevDeInit(gpstDirDevISR[DevID]);

    //Free DirDev memory...
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirOperReqSem);
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirOperSem);
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirReadReqSem);
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirReadSem);
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirWriteReqSem);
    rkos_semaphore_delete(gpstDirDevISR[DevID]->osDirWriteSem);
    rkos_memory_free(gpstDirDevISR[DevID]);

    //Delete DirDev...
    gpstDirDevISR[DevID] = NULL;

    //Delete DirDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_DIR_DEV);
#endif

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DirDev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:26
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_INIT_
INIT API HDC DirDev_Create(uint32 DevID, void * arg)
{
    DIR_DEV_ARG * pstDirDevArg;
    DEVICE_CLASS* pstDev;
    DIR_DEVICE_CLASS * pstDirDev;

    if(arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstDirDevArg = (DIR_DEV_ARG *)arg;
    pstDirDev =  rkos_memory_malloc(sizeof(DIR_DEVICE_CLASS));
    memset(pstDirDev, 0, sizeof(DIR_DEVICE_CLASS));
    if (pstDirDev == NULL)
    {
        return NULL;
    }

    //init handler...
    pstDirDev->osDirOperReqSem  = rkos_semaphore_create(1,1);
    pstDirDev->osDirOperSem  = rkos_semaphore_create(1,0);

    pstDirDev->osDirReadReqSem  = rkos_semaphore_create(1,1);
    pstDirDev->osDirReadSem  = rkos_semaphore_create(1,0);

    pstDirDev->osDirWriteReqSem  = rkos_semaphore_create(1,1);
    pstDirDev->osDirWriteSem  = rkos_semaphore_create(1,0);
    if(((pstDirDev->osDirOperReqSem) == 0)
        || ((pstDirDev->osDirOperSem) == 0)
        || ((pstDirDev->osDirReadReqSem) == 0)
        || ((pstDirDev->osDirReadSem) == 0)
        || ((pstDirDev->osDirWriteReqSem) == 0)
        || ((pstDirDev->osDirWriteSem) == 0))
    {
        rkos_semaphore_delete(pstDirDev->osDirOperReqSem);
        rkos_semaphore_delete(pstDirDev->osDirOperSem);
        rkos_semaphore_delete(pstDirDev->osDirReadReqSem);
        rkos_semaphore_delete(pstDirDev->osDirReadSem);
        rkos_semaphore_delete(pstDirDev->osDirWriteReqSem);
        rkos_semaphore_delete(pstDirDev->osDirWriteSem);
        rkos_memory_free(pstDirDev);
        return (HDC) RK_ERROR;
    }
    pstDev = (DEVICE_CLASS *)pstDirDev;
    pstDev->suspend = DirDevSuspend;
    pstDev->resume  = DirDevResume;

    //init arg...
    pstDirDev->Channel = pstDirDevArg->Channel;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_DIR_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    gpstDirDevISR[DevID] = NULL;
    //DirHwInit(DevID, pstDirDev->Channel);
    if(DirDevInit(pstDirDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstDirDev->osDirOperReqSem);
        rkos_semaphore_delete(pstDirDev->osDirOperSem);
        rkos_semaphore_delete(pstDirDev->osDirReadReqSem);
        rkos_semaphore_delete(pstDirDev->osDirReadSem);
        rkos_semaphore_delete(pstDirDev->osDirWriteReqSem);
        rkos_semaphore_delete(pstDirDev->osDirWriteSem);
        rkos_memory_free(pstDirDev);
        return (HDC) RK_ERROR;
    }
    gpstDirDevISR[DevID] = pstDirDev;
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
** Name: DirDevDeInit
** Input:DIR_DEVICE_CLASS * pstDirDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_INIT_
INIT FUN rk_err_t DirDevDeInit(DIR_DEVICE_CLASS * pstDirDev)
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
** Name: DirDevInit
** Input:DIR_DEVICE_CLASS * pstDirDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:26
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_INIT_
INIT FUN rk_err_t DirDevInit(DIR_DEVICE_CLASS * pstDirDev)
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


#ifdef _DIR_DEV_SHELL_
_DRIVER_FILE_DIRDEVICE_SHELL_
static char* ShellDirName[] =
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
** Name: DirDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_SHELL_
SHELL API rk_err_t DirDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellDirName, pItem, StrCnt);
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
            ret = DirDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = DirDevShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = DirDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = DirDevShellTest(dev,pItem);
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
** Name: DirDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_SHELL_
SHELL FUN rk_err_t DirDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hDirDev;
    uint32 DevID;

    //Get DirDev ID...
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

    //Open DirDev...
    hDirDev = RKDev_Open(DEV_CLASS_DIR, 0, NOT_CARE);
    if((hDirDev == NULL) || (hDirDev == (HDC)RK_ERROR) || (hDirDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("DirDev open failure");
         return RK_SUCCESS;
    }

    //do test....

    //close DirDev...
    RKDev_Close(hDirDev);
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DirDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_SHELL_
SHELL FUN rk_err_t DirDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;

    //Get DirDev ID...
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
    if(RKDev_Delete(DEV_CLASS_DIR, DevID, NULL) != RK_SUCCESS)
    {
        rk_print_string("DIRDev delete failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DirDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_SHELL_
SHELL FUN rk_err_t DirDevShellCreate(HDC dev, uint8 * pstr)
{
    DIR_DEV_ARG stDirDevArg;
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

    //Init DirDev arg...
    //stDirDevArg.usbmode = USBOTG_MODE_DEVICE;

    //Create DirDev...
    ret = RKDev_Create(DEV_CLASS_DIR, DevID, &stDirDevArg);
    if(ret != RK_SUCCESS)
    {
        rk_print_string("DirDev create failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DirDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 13:58:27
*******************************************************************************/
_DRIVER_FILE_DIRDEVICE_SHELL_
SHELL FUN rk_err_t DirDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hDirDev;
    uint32 DevID;

    //Get DirDev ID...
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
    if(gpstDirDevISR[DevID] != NULL)
    {
        rk_print_string("rn");
        rk_printf_no_time("DirDev ID = %d Pcb list as follow:", DevID);
        rk_printf_no_time("    &stDirDevice---0x%08x", &gpstDirDevISR[DevID]->stDirDevice);
        rk_printf_no_time("    osDirOperReqSem---0x%08x", gpstDirDevISR[DevID]->osDirOperReqSem);
        rk_printf_no_time("    osDirOperSem---0x%08x", gpstDirDevISR[DevID]->osDirOperSem);
        rk_printf_no_time("    osDirReadReqSem---0x%08x", gpstDirDevISR[DevID]->osDirReadReqSem);
        rk_printf_no_time("    osDirReadSem---0x%08x", gpstDirDevISR[DevID]->osDirReadSem);
        rk_printf_no_time("    osDirWriteReqSem---0x%08x", gpstDirDevISR[DevID]->osDirWriteReqSem);
        rk_printf_no_time("    osDirWriteSem---0x%08x", gpstDirDevISR[DevID]->osDirWriteSem);
        //rk_printf_no_time("    usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
        //rk_printf_no_time("    usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
    }
    else
    {
        rk_print_string("rn");
        rk_printf_no_time("DirDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}


#endif

#endif
