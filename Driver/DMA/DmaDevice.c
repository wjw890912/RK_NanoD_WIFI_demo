/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                     All rights reserved.
*
* FileName: ..\Driver\DMA\DmaDevice.c
* Owner: Aaron.sun
* Date: 2014.4.24
* Time: 10:56:35
* Desc: Dma Device Class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.4.24     10:56:35   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_DMA_DMADEVICE_C__

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
typedef  struct _DMA_DEVICE_CLASS
{
    DEVICE_CLASS stDmaDevice;

    pSemaphore osDmaOperReqSem;
    pFunc DmaIsrCallBack[DMA_CHN_MAX];
    uint32 DmaTransSizeBack[DMA_CHN_MAX];
    DMA_LLP g_llpListn[DMA_CHN_MAX][LLP_MAX_NUM];
    uint32 DmaChStatus;

}DMA_DEVICE_CLASS;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static DMA_DEVICE_CLASS * gpstDmaDevISR;



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
rk_err_t ShellDmaBsp(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestChp(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestMulDma(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestLlp(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestInt(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestStartStop(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestBlock(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspTestDma(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspStop(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspStart(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetChP(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetTargetInc(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetSrcInc(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetBlock(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetTarget(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspSetSrc(HDC dev, uint8 * pstr);
rk_err_t ShellDmaBspHelp(HDC dev, uint8 * pstr);
rk_err_t DmaShellHelp(HDC dev, uint8 * pstr);
rk_err_t DmaShellCreate(HDC dev, uint8 * pstr);
rk_err_t DmaShellPcb(HDC dev,  uint8 * pstr);
rk_err_t DmaDevResume(HDC dev);
rk_err_t DmaDevSuspend(HDC dev);

rk_err_t DmaDevDelete(HDC dev);

rk_err_t DmaDevDeInit(DMA_DEVICE_CLASS * pstDmaDev);
rk_err_t DmaDevInit(DMA_DEVICE_CLASS * pstDmaDev);

void DmaIntIsr(void);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DmaDev_RealseChannel
** Input:HDC dev ,uint32 ch
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 15:16:19
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_COMMON_
COMMON API rk_err_t DmaDev_RealseChannel(HDC dev ,uint32 ch)
{
    DMA_DEVICE_CLASS * pstDmaDev = (DMA_DEVICE_CLASS *)dev;

    if (pstDmaDev == NULL)
    {
        return RK_ERROR;
    }

    pstDmaDev->DmaChStatus &= ~((uint32)0x01 << ch);
    DmaStop(ch);

    rkos_semaphore_give(pstDmaDev->osDmaOperReqSem);

}

/*******************************************************************************
** Name: DmaDev_GetChannel
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 14:48:54
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_COMMON_
COMMON API rk_err_t DmaDev_GetChannel(HDC dev)
{
    DMA_DEVICE_CLASS * pstDmaDev = (DMA_DEVICE_CLASS *)dev;

    uint32 ch;

    if (pstDmaDev == NULL)
    {
        return RK_ERROR;
    }

    rkos_semaphore_take(pstDmaDev->osDmaOperReqSem, MAX_DELAY);

    for (ch = 0; ch < DMA_CHN_MAX; ch++)
    {
        if ((pstDmaDev->DmaChStatus & ((uint32)0x01 << ch)) == 0)
        {
            if (DmaGetState(ch) == DMA_IDLE)
            {
                pstDmaDev->DmaChStatus |= ((uint32)0x01 << ch);
                return (rk_err_t)ch;
            }
        }
    }

    return RK_ERROR;

}

/*******************************************************************************
** Name: DmaDev_DmaReStart
** Input:HDC dev, uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 13:41:23
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_COMMON_
COMMON API rk_err_t DmaDev_DmaReStart(HDC dev, uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack)
{
    //dma channel configer

    DMA_DEVICE_CLASS * pstDmaDev = (DMA_DEVICE_CLASS *)dev;

    if (pstDmaDev == NULL)
    {
        return RK_ERROR;
    }

    rkos_enter_critical();

    if (pstDmaDev->DmaTransSizeBack[ch] == size)
    {
        pstDmaDev->DmaIsrCallBack[ch] = (pFunc)CallBack;
        DmaReConfig(ch, srcAddr, dstAddr, size, pstDmaDev->g_llpListn[ch]);
    }
    else
    {
        pstDmaDev->DmaTransSizeBack[ch] = size;
        pstDmaDev->DmaIsrCallBack[ch] = CallBack;

        DmaConfig(ch, srcAddr, dstAddr, size, g_dmaPar, pstDmaDev->g_llpListn[ch]);
    }

    rkos_exit_critical();

}

/*******************************************************************************
** Name: DmaDev_DmaStart
** Input:uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 11:39:48
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_COMMON_
COMMON API rk_err_t DmaDev_DmaStart(HDC dev, uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack)
{
    DMA_DEVICE_CLASS * pstDmaDev = (DMA_DEVICE_CLASS *)dev;

    uint32   llpNum;
    uint32   temp_ch;
    rk_err_t ret = RK_ERROR;

    if (pstDmaDev == NULL)
    {
        return RK_ERROR;
    }

    rkos_enter_critical();

    temp_ch = ch;
    if (temp_ch >= DMA_CHN_MAX)
    {
        //rk_printf("Dma Channel error: ch = %d!\n", ch);
        ret = RK_ERROR;
        goto out;
    }

    if (DmaGetState(ch) == DMA_BUSY)
    {
        ret = RK_ERROR;
        goto out;
    }

    if (size > DMA_MAX_BLOCK_SIZE)
    {
        llpNum  = (size + (LLP_BLOCK_SIZE - 1))/LLP_BLOCK_SIZE;

        if (llpNum > LLP_MAX_NUM)
        {
            //rk_printf("Dma Ch%d size so large!\n", temp_ch);
            ret = RK_ERROR;
            goto out;
        }
    }

    pstDmaDev->DmaTransSizeBack[temp_ch] = size;
    pstDmaDev->DmaIsrCallBack[temp_ch] = CallBack;
    DmaEnableInt(temp_ch);
    DmaConfig(temp_ch, srcAddr, dstAddr, size, g_dmaPar, pstDmaDev->g_llpListn[temp_ch]);

out:

    rkos_exit_critical();

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
** Name: DmaIntIsr
** Input:void
** Return: void
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_COMMON_
COMMON FUN void DmaIntIsr(void)
{
    uint32 channel;
    uint32 rawStatus;
    pFunc CallBack;

    rawStatus = DmaGetIntType();

    for (channel = 0; channel < DMA_CHN_MAX; channel++)
    {
        if (((uint32)(rawStatus)) & ((uint32)(0x01) << channel))
        {
            CallBack = gpstDmaDevISR->DmaIsrCallBack[channel];
            if (CallBack)
            {
                gpstDmaDevISR->DmaIsrCallBack[channel] = 0;
                CallBack();
            }

            DmaDisableInt(channel);
        }
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
** Name: DmaDevCreate
** Input:void * Args
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT API HDC DmaDev_Create(uint32 DevID, void * arg)
{

    DEVICE_CLASS* pstDev;
    DMA_DEVICE_CLASS * pstDmaDev;

    rkos_enter_critical();

    pstDmaDev =  rkos_memory_malloc(sizeof(DMA_DEVICE_CLASS));
    if (pstDmaDev == NULL)
    {
        return pstDmaDev;
    }

    memset((uint8 *)pstDmaDev, 0, sizeof(DMA_DEVICE_CLASS));

    pstDmaDev->osDmaOperReqSem     = rkos_semaphore_create(DMA_CHN_MAX, DMA_CHN_MAX);

    if ((pstDmaDev->osDmaOperReqSem) == 0)
    {

        rkos_semaphore_delete(pstDmaDev->osDmaOperReqSem);

        rkos_memory_free(pstDmaDev);
        rkos_exit_critical();
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstDmaDev;

    pstDev->suspend = DmaDevResume;
    pstDev->resume  = DmaDevResume;

    gpstDmaDevISR = pstDmaDev;

    rkos_exit_critical();

    DmaDevInit(pstDmaDev);

    return pstDev;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DmaDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT FUN rk_err_t DmaDevResume(HDC dev)
{

}
/*******************************************************************************
** Name: DmaDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT FUN rk_err_t DmaDevSuspend(HDC dev)
{

}
/*******************************************************************************
** Name: DmaDevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT FUN rk_err_t DmaDevDelete(HDC dev)
{

}
/*******************************************************************************
** Name: DmaDevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT FUN rk_err_t DmaDevDeInit(DMA_DEVICE_CLASS * pstDmaDev)
{
    ScuClockGateCtr(HCLK_DMA_GATE, 0);

    ScuSoftResetCtr(SYSDMA_SRST, 1);
    DelayMs(1);
    ScuSoftResetCtr(SYSDMA_SRST, 0);
}
/*******************************************************************************
** Name: DmaDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_INIT_
INIT FUN rk_err_t DmaDevInit(DMA_DEVICE_CLASS * pstDmaDev)
{
    //open dma1 clk
    ScuClockGateCtr(HCLK_DMA_GATE, 1);
    //open rst dma1 ip
    ScuSoftResetCtr(SYSDMA_SRST, 1);
    DelayMs(1);
    ScuSoftResetCtr(SYSDMA_SRST, 0);

    //open dma2 clk
    ScuClockGateCtr(HCLK_HIGH_DMA_GATE, 1);
    //open rst dma2 ip
    ScuSoftResetCtr(DMA2_SRST, 1);
    DelayMs(1);
    ScuSoftResetCtr(DMA2_SRST, 0);

    //IntUnregister(INT_ID_DMA);
    //IntPendingClear(INT_ID_DMA);
    //IntDisable(INT_ID_DMA);
    //dma1 int
    IntRegister(INT_ID_DMA ,DmaIntIsr);
    IntPendingClear(INT_ID_DMA);
    IntEnable(INT_ID_DMA);
}


#ifdef _DMA_SHELL_
_DRIVER_DMA_DMADEVICE_SHELL_
static uint8* ShellDmaName[] =
{
    "pcb",
    "create",
    "help",
    "bsp",
    "\b",
};

#ifdef SHELL_BSP
_DRIVER_DMA_DMADEVICE_SHELL_
static uint8* ShellDmaBspName[] =
{
    "help",
    "setsrc",
    "settarget",
    "setblock",
    "setsrcinc",
    "settargetinc",
    "setchp",
    "start",
    "stop",
    "testdma",
    "testblock",
    "teststartstop",
    "testint",
    "testllp",
    "testmuldma",
    "testchp",
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
** Name: DmaShell
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL API rk_err_t DmaDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellDmaName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                 //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = DmaShellPcb(dev,pItem);
            break;

        case 0x01:  //Create
            ret = DmaShellCreate(dev,pItem);
            break;

        case 0x02:  //help
#ifdef SHELL_HELP
            ret = DmaShellHelp(dev,pItem);
#endif
            break;

        case 0x03:
#ifdef SHELL_BSP
            ret = ShellDmaBsp(dev, pItem);
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
** Name: ShellDmaBsp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 14:05:25
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBsp(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellDmaBspName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                 //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //help
#ifdef SHELL_HELP
            ret = ShellDmaBspHelp(dev,pItem);
#endif
            break;

        case 0x01:  //setsrc
            ret = ShellDmaBspSetSrc(dev,pItem);
            break;

        case 0x02:  //settarget
            ret = ShellDmaBspSetTarget(dev,pItem);
            break;

        case 0x03: //setblock
            ret = ShellDmaBspSetBlock(dev, pItem);
            break;

        case 0x04: //setsrcinc
            ret = ShellDmaBspSetSrcInc(dev, pItem);
            break;

        case 0x05: //settargetinc
            ret = ShellDmaBspSetTargetInc(dev, pItem);
            break;

        case 0x06: //setchp
            ret = ShellDmaBspSetChP(dev, pItem);
            break;

        case 0x07:  //start
            ret = ShellDmaBspStart(dev,pItem);
            break;

        case 0x08:  //stop
            ret = ShellDmaBspStop(dev,pItem);
            break;

        case 0x09:  //testdma
            ret = ShellDmaBspTestDma(dev,pItem);
            break;

        case 0x0a: //testblock
            ret = ShellDmaBspTestBlock(dev, pItem);
            break;

        case 0x0b: //teststartstop
            ret = ShellDmaBspTestStartStop(dev, pItem);
            break;

        case 0x0c: //testint
            ret = ShellDmaBspTestInt(dev, pItem);
            break;

        case 0x0d: //testllp
            ret = ShellDmaBspTestLlp(dev, pItem);
            break;

        case 0x0e: //testmuldma
            ret = ShellDmaBspTestMulDma(dev, pItem);
            break;

        case 0x0f: //testchp
            ret = ShellDmaBspTestChp(dev, pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}

/*******************************************************************************
** Name: ShellDmaBspTestChp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:59:48
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestChp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspTestMulDma
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:59:10
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestMulDma(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspTestLlp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:58:30
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestLlp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspTestInt
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:58:10
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestInt(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspTestStartStop
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:57:03
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestStartStop(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspTestBlock
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:56:35
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestBlock(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

_DRIVER_DMA_DMADEVICE_SHELL_ uint32 DmaFinish;
_DRIVER_DMA_DMADEVICE_SHELL_ DMA_LLP llpListn[DMA_CHN_MAX][LLP_MAX_NUM];
_DRIVER_DMA_DMADEVICE_SHELL_    eDMA_CHN dma_test_channel;

_DRIVER_DMA_DMADEVICE_SHELL_
void DmaIsr()
{
    DmaFinish = 1;
    DmaDisableInt(dma_test_channel);
}

/*******************************************************************************
** Name: ShellDmaBspTestDma
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:56:02
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspTestDma(HDC dev, uint8 * pstr)
{
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        dma_test_channel = 0;
        rk_print_string("\r\nShellDmaBspTestDma start 0\n");
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        dma_test_channel = 1;
        rk_print_string("\r\nShellDmaBspTestDma start 1\n");
    }
    else if (StrCmpA(pstr, "2", 1) == 0)
    {
        dma_test_channel = 2;
        rk_print_string("\r\nShellDmaBspTestDma start 2\n");
    }
    else if (StrCmpA(pstr, "3", 1) == 0)
    {
        dma_test_channel = 3;
        rk_print_string("\r\nShellDmaBspTestDma start 3\n");
    }
    else if (StrCmpA(pstr, "4", 1) == 0)
    {
        dma_test_channel = 4;
        rk_print_string("\r\nShellDmaBspTestDma start 4\n");
    }
    else if (StrCmpA(pstr, "5", 1) == 0)
    {
        dma_test_channel = 5;
        rk_print_string("\r\nShellDmaBspTestDma start 5\n");
    }
    //open uart clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 1);

    //open rst uart ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);

    IntRegister(INT_ID_DMA ,DmaIsr);

    IntPendingClear(INT_ID_DMA);
    IntEnable(INT_ID_DMA);

    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

    // test signle
    DmaEnableInt(dma_test_channel);
    DmaFinish = 0;

    *(uint32 *)0x01040000 = 0x55aaaa55;

    *(uint32 *)0x01043FF8 = 0xaa5555aa;

    *(uint32 *)0x03040000 = 0;

    *(uint32 *)0x03043FF8 = 0;


    DmaConfig(dma_test_channel, 0x01040000, 0x03040000, 0xFFF, &DmaCfg, NULL);
    while (!DmaFinish);

    DmaDisableInt(dma_test_channel);

    if ((*(uint32 *)0x03040000 != 0x55aaaa55) || (*(uint32 *)0x03043FF8!= 0xaa5555aa))
    {
        return RK_SUCCESS;
    }
    rk_print_string("\r\ntest signle over\n");


    //test auto llp
    DmaEnableInt(dma_test_channel);
    DmaFinish = 0;

    *(uint32 *)0x01040000 = 0x55aaaa55;

    *(uint32 *)0x01047ffc = 0xaa5555aa;

    *(uint32 *)0x03040000 = 0;

    *(uint32 *)0x03047ffc = 0;



    DmaConfig(dma_test_channel, 0x01040000, 0x03040000, 0x2000, &DmaCfg, llpListn[dma_test_channel]);
    while (!DmaFinish);

    DmaDisableInt(dma_test_channel);

    if ((*(uint32 *)0x03040000 != 0x55aaaa55) || (*(uint32 *)0x03047ffc != 0xaa5555aa))
    {
        return RK_SUCCESS;
    }
    rk_print_string("\r\ntest auto llp over\n");


    //test define llp
    DmaEnableInt(dma_test_channel);
    DmaFinish = 0;

    *(uint32 *)0x01040000 = 0x55aaaa55;
    *(uint32 *)0x010407fc = 0xaa5555aa;

    *(uint32 *)0x03040000 = 0;
    *(uint32 *)0x030407fc = 0;

    *(uint32 *)0x01043000 = 0x55aaaa55;
    *(uint32 *)0x010437fc = 0xaa5555aa;

    *(uint32 *)0x03043000 = 0;
    *(uint32 *)0x030437fc = 0;

    llpListn[0][0].SAR = 0x01040000;
    llpListn[0][0].DAR = 0x03040000;
    llpListn[0][0].SIZE = 0x200;

    llpListn[0][1].SAR = 0x01043000;
    llpListn[0][1].DAR = 0x03043000;
    llpListn[0][1].SIZE = 0x200;

    DmaConfig_for_LLP(dma_test_channel, 0x200, 2,&DmaCfg, llpListn[0]);
    while (!DmaFinish);

    DmaDisableInt(dma_test_channel);

    if ((*(uint32 *)0x03040000 != 0x55aaaa55) || (*(uint32 *)0x030407fc != 0xaa5555aa) ||
            (*(uint32 *)0x03043000 != 0x55aaaa55) || (*(uint32 *)0x030437fc != 0xaa5555aa) )
    {
        return RK_SUCCESS;
    }

    rk_print_string("\r\ntest define llp\n");


    //close dma clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 0);

    //rst dma ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);

    IntUnregister(INT_ID_DMA);


    rk_print_string("dma test over");


    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspStop
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:55:19
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspStop(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspStart
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:54:57
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspStart(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetChP
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:54:20
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetChP(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetTargetInc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:53:51
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetTargetInc(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetSrcInc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:53:20
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetSrcInc(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetBlock
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:52:57
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetBlock(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetTarget
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:52:34
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetTarget(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellDmaBspSetSrc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:51:58
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspSetSrc(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

#ifdef SHELL_HELP
/*******************************************************************************
** Name: ShellDmaBspHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:51:29
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t ShellDmaBspHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
#endif

#endif


/*******************************************************************************
** Name: DmaShellHelp
** Input:HDC dev,const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 10:46:25
*******************************************************************************/
#ifdef SHELL_HELP
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t DmaShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if ( StrLenA( (uint8 *)pstr) != 0)
        return RK_ERROR;

    rk_print_string("dma命令集提供了一系列的命令对dma进行操作\r\n");
    rk_print_string("dma包含的子命令如下:              \r\n");
    rk_print_string("pcb       显示pcb信息             \r\n");
    rk_print_string("open      打开dma                 \r\n");
    rk_print_string("help      显示dma命令帮助信息     \r\n");

    return RK_SUCCESS;

}
#endif

/*******************************************************************************
** Name: DmaShellOpen
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t DmaShellCreate(HDC dev,  uint8 * pstr)
{
    rk_err_t ret;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("dma.open : open dma. \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("dma device create failure");
    }

    return ret;

}

/*******************************************************************************
** Name: DmaShellPcb
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.24
** Time: 10:58:36
*******************************************************************************/
_DRIVER_DMA_DMADEVICE_SHELL_
SHELL FUN rk_err_t DmaShellPcb(HDC dev,  uint8 * pstr)
{

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("dma.pcb : dma pcb info. \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return  RK_SUCCESS;
}



#endif
#endif

