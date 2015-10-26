/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: System\Shell\ShellBsp.c
* Owner: aaron.sun
* Date: 2014.10.11
* Time: 15:03:09
* Desc: Shell Bsp Test
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2014.10.11     15:03:09   1.0
********************************************************************************************
*/
#define __SYSTEM_SHELL_SHELLBSP_C__
#include "BspConfig.h"
#ifdef __SYSTEM_SHELL_SHELLBSP_C__
#ifdef SHELL_BSP
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _SYSTEM_SHELL_SHELLBSP_READ_  __attribute__((section("system_shell_shellbsp_read")))
#define _SYSTEM_SHELL_SHELLBSP_WRITE_ __attribute__((section("system_shell_shellbsp_write")))
#define _SYSTEM_SHELL_SHELLBSP_INIT_  __attribute__((section("system_shell_shellbsp_init")))
#define _SYSTEM_SHELL_SHELLBSP_SHELL_  __attribute__((section("system_shell_shellbsp_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspName[] =
{
    "help",
    "memory",
    "gpio",
    "nvic",
    "mailbox",
    "...",
    "\b"
};

_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspNvicName[] =
{
    "help",
    "setint",
    "setpriority",
    "setmask",
    "testint",
    "testpriority",
    "resumem3",
    "resumenanod",
    "...",
    "\b"
};

_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspMemoryName[] =
{
    "help",
    "testpower",
    "testclk",
    "testrw",
    "testconsumption",
    "testeffect",
    "...",
    "\b"
};

_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspMemoryRAMName[] =
{
    "help",
    "iram",
    "dram",
    "hiram",
    "hdram",
    "pmusarm",
    "dma",
    "dmallp",
    "...",
    "\b"
};

_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspGpioName[] =
{
    "help",
    "setin",
    "setout",
    "sethigh",
    "setlow",
    "setpull",
    "testinout",
    "testpull",
    "testint",
    "...",
    "\b"
};

_SYSTEM_SHELL_SHELLBSP_READ_
static char* ShellBspMailBoxName[] =
{
    "help",
    "writedata",
    "writecmd",
    "readdata",
    "readcmd",
    "testa2b",
    "testb2a",
    "...",
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
rk_err_t ShellBspMemoryTest_DMA_LLP_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTest_DMA_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestPMUSRAM_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestHIRAM_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestHDRAM_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestDRAM_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestIRAM_RW(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxTestB2A(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxTestA2B(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxReadCmd(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxReadData(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxWriteCmd(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxWriteData(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioTestInt(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioTestPull(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioTestInOut(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioSetPull(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioSetLow(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioSetOut(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioSetHigh(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioSetIn(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestEffect(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestConsumption(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestReadWrite(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestClk(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryTestPower(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicResumeNanoD(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicResumeM3(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicTestPriority(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicTestInt(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicSetMask(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicSetPriority(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicSetInt(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBoxHelp(HDC dev, uint8 * pstr);
rk_err_t ShellBspGpioHelp(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemoryHelp(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvicHelp(HDC dev, uint8 * pstr);
rk_err_t ShellBspHelp(HDC dev, uint8 * pstr);
rk_err_t ShellBspMailBox(HDC dev, uint8 * pstr);
rk_err_t ShellBspNvic(HDC dev, uint8 * pstr);
rk_err_t ShellBspGPIO(HDC dev, uint8 * pstr);
rk_err_t ShellBspMemory(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ShellBsp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.10.13
** Time: 16:35:58
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON API rk_err_t ShellBsp(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if(StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellBspName, pItem, StrCnt);
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
            #ifdef SHELL_HELP
            ret = ShellBspHelp(dev, pItem);
            #endif
            break;

        case 0x01:
            ret = ShellBspMemory(dev, pItem);
            break;

        case 0x02:
            ret = ShellBspGPIO(dev, pItem);
            break;

        case 0x03:
            ret = ShellBspNvic(dev, pItem);
            break;

        case 0x04:
            ret = ShellBspMailBox(dev, pItem);
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
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/





uint32 MailBoxCmd;
uint32 MailBoxID;
uint32 Port;
uint32 MailBoxData;

void MailBoxA2BService()
{
    MailBoxCmd = MailBoxReadA2BCmd(MailBoxID, Port);
    MailBoxData = MailBoxReadA2BData(MailBoxID, Port);
    MailBoxClearA2BInt(MailBoxID, (uint32)(1 << Port));
}

void MailBoxB2AService()
{
    MailBoxCmd = MailBoxReadB2ACmd(MailBoxID, Port);
    MailBoxData = MailBoxReadB2AData(MailBoxID, Port);
    MailBoxClearB2AInt(MailBoxID, (uint32)(1 << Port));
}

/*******************************************************************************
** Name: ShellBspMailBoxTestCmd
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:43:11
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxTestB2A(HDC dev, uint8 * pstr)
{
    uint32 i;
    uint32 Cmd, Data;

    for(i = 0; i < 4; i++)
    {
        IntRegister(INT_ID_MAILBOX0 + i, MailBoxB2AService);
        IntPendingClear(INT_ID_MAILBOX0 + i);
        IntEnable(INT_ID_MAILBOX0 + i);
        MailBoxEnableB2AInt(MailBoxID, (int32)(1 << i));
        Cmd = 0x55 + i;
        Data = 0xaa + i;
        MailBoxID = 0;
        Port = i;
        MailBoxCmd = 0;
		MailBoxData = 0;

        MailBoxWriteB2ACmd(Cmd, MailBoxID, Port);
        MailBoxWriteB2AData(Data, MailBoxID, Port);

        while(1)
        {
            if((MailBoxCmd == Cmd) && (MailBoxData == Data))
            {
                rk_printf_no_time1("MailBox B2A Port = %d test ok", i);
                break;
            }
        }

        IntUnregister(INT_ID_MAILBOX0 + i);
        IntPendingClear(INT_ID_MAILBOX0 + i);
        IntDisable(INT_ID_MAILBOX0 + i);
        MailBoxDisableB2AInt(MailBoxID, (int32)(1 << i));

    }

    rk_print_string("mailbox test over\r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMailBoxTestData
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:42:30
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxTestA2B(HDC dev, uint8 * pstr)
{
    uint32 i;
    uint32 Cmd, Data;

    for(i = 0; i < 4; i++)
    {
        IntRegister(INT_ID_MAILBOX0 + i, MailBoxA2BService);
        IntPendingClear(INT_ID_MAILBOX0 + i);
        IntEnable(INT_ID_MAILBOX0 + i);
        MailBoxEnableA2BInt(MailBoxID, (int32)(1 << i));
        Cmd = 0x55 + i;
        Data = 0xaa + i;
        MailBoxID = 0;
        Port = i;
        MailBoxCmd = 0;
		MailBoxData = 0;

        MailBoxWriteA2BCmd(Cmd, MailBoxID, Port);
        MailBoxWriteA2BData(Data, MailBoxID, Port);

        while(1)
        {
            if((MailBoxCmd == Cmd) && (MailBoxData == Data))
            {
                rk_printf_no_time1("MailBox A2B Port = %d test ok", i);
                break;
            }
        }

        IntUnregister(INT_ID_MAILBOX0 + i);
        IntPendingClear(INT_ID_MAILBOX0 + i);
        IntDisable(INT_ID_MAILBOX0 + i);
        MailBoxDisableA2BInt(MailBoxID, (int32)(1 << i));

    }

    rk_print_string("mailbox test over\r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMailBoxReadCmd
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:41:41
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxReadCmd(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMailBoxReadData
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:41:14
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxReadData(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMailBoxWriteCmd
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:18:04
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxWriteCmd(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMailBoxWriteData
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:17:32
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxWriteData(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioTestInt
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:17:03
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioTestInt(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioTestPull
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:16:05
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioTestPull(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioTestInOut
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:14:24
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioTestInOut(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioSetPull
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:13:27
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioSetPull(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioSetLow
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:02:29
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioSetLow(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioSetOut
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:02:08
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioSetOut(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioSetHigh
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:01:27
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioSetHigh(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspGpioSetIn
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:00:56
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioSetIn(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}


static uint32 DmaFinish;
static void DmaIsr()
{
     DmaFinish = 1;
     DmaDisableInt(0);
}

_ATTR_AP_SYS_BSS_ __align(4) uint32 buf[512];

/*******************************************************************************
** Name: ShellBspMemoryTestEffect
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 11:00:04
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestEffect(HDC dev, uint8 * pstr)
{
    uint32 * pstr1, * pstr2;
    uint32 savesystick,i;

    pstr1 = rkos_memory_malloc(2048);
    //pstr1 = buf;
    pstr2 = rkos_memory_malloc(2048);

    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

     //open uart clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 1);

    //open rst uart ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);

    IntRegister(INT_ID_DMA ,DmaIsr);

    IntPendingClear(INT_ID_DMA);
    IntEnable(INT_ID_DMA);

    savesystick = SysTickCounter;

    for(i = 0; i < 3000; i++)
    {
        DmaEnableInt(0);
        DmaFinish = 0;

        DmaConfig(0, (uint32)pstr1, (uint32)pstr2, 512, &DmaCfg, NULL);
    	while(!DmaFinish)
        {
            __WFI();
        }

    }

    rk_printf("dma1 acess memory speed = %d M/S", (3 * 2048)/ ((SysTickCounter - savesystick) * 10));


    //close dma clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 0);

    //rst dma ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);


    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ShellBspMemoryTestConsumption
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:59:28
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestConsumption(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
uint32 IDRAM_DMA_CH;
uint32 IDRAM_DmaFinish;
_SYSTEM_SHELL_SHELLBSP_READ_
void IDRAM_DmaIsr()
{
     IDRAM_DmaFinish = 1;
     rk_print_string("\r\n IDRAM_DmaIsr\n");
     DmaDisableInt(IDRAM_DMA_CH);
}
_SYSTEM_SHELL_SHELLBSP_READ_ DMA_LLP TEST_llpListn[DMA_CHN_MAX][50];

/*******************************************************************************
** Name: ShellBspMemoryTest_DMA_LLP_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.27
** Time: 9:11:16
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTest_DMA_LLP_RW(HDC dev, uint8 * pstr)
{
    uint32 *pbuf_LLP_0,*pbuf_LLP_1,*pbuf_LLP_2,
           *pbuf_LLP_3,*pbuf_LLP_4,*pbuf_LLP_5,
           *pbuf_LLP_6,*pbuf_LLP_7,*pbuf_LLP_8,
           *pbuf_LLP_9;
    uint32 *pbuf1_LLP_0,*pbuf1_LLP_1,*pbuf1_LLP_2,
           *pbuf1_LLP_3,*pbuf1_LLP_4,*pbuf1_LLP_5,
           *pbuf1_LLP_6,*pbuf1_LLP_7,*pbuf1_LLP_8,
           *pbuf1_LLP_9;
    uint32 i = 0;
    uint32 f1,f2,f3,f4;

    #define H_IRAM_ADDR 0x01040000
    #define H_IRAM_SIZE 0x40000

    #define H_DRAM_ADDR 0x03040000
    #define H_DRAM_SIZE 0x30000

    if(StrCmpA(pstr, "0", 1) == 0)
    {
        IDRAM_DMA_CH = 0;
        rk_print_string("\r\n IDRAM_DMA_CH start 0\n");
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        IDRAM_DMA_CH = 1;
        rk_print_string("\r\n IDRAM_DMA_CH start 1\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        IDRAM_DMA_CH = 2;
        rk_print_string("\r\n IDRAM_DMA_CH start 2\n");
    }
    else if(StrCmpA(pstr, "3", 1) == 0)
    {
        IDRAM_DMA_CH = 3;
        rk_print_string("\r\n IDRAM_DMA_CH start 3\n");
    }
    else if(StrCmpA(pstr, "4", 1) == 0)
    {
        IDRAM_DMA_CH = 4;
        rk_print_string("\r\n IDRAM_DMA_CH start 4\n");
    }
    else if(StrCmpA(pstr, "5", 1) == 0)
    {
        IDRAM_DMA_CH = 5;
        rk_print_string("\r\n IDRAM_DMA_CH start 5\n");
    }

    pbuf_LLP_0 = (uint32 *)(0x01040000);
    pbuf_LLP_1 = (uint32 *)(0x01040500);
    pbuf_LLP_2 = (uint32 *)(0x01041000);
    pbuf_LLP_3 = (uint32 *)(0x01041500);
    pbuf_LLP_4 = (uint32 *)(0x01042000);
    pbuf_LLP_5 = (uint32 *)(0x01042500);
    pbuf_LLP_6 = (uint32 *)(0x01043000);
    pbuf_LLP_7 = (uint32 *)(0x01043500);
    pbuf_LLP_8 = (uint32 *)(0x01044000);
    pbuf_LLP_9 = (uint32 *)(0x01044500);

    pbuf1_LLP_0 = (uint32 *)0x03040000;
    pbuf1_LLP_1 = (uint32 *)0x03040500;
    pbuf1_LLP_2 = (uint32 *)0x03041000;
    pbuf1_LLP_3 = (uint32 *)0x03041500;
    pbuf1_LLP_4 = (uint32 *)0x03042000;
    pbuf1_LLP_5 = (uint32 *)0x03042500;
    pbuf1_LLP_6 = (uint32 *)0x03043000;
    pbuf1_LLP_7 = (uint32 *)0x03043500;
    pbuf1_LLP_8 = (uint32 *)0x03044000;
    pbuf1_LLP_9 = (uint32 *)0x03044500;


    //open uart clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 1);

    //open rst uart ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);

    IntRegister(INT_ID_DMA ,IDRAM_DmaIsr);

    IntPendingClear(INT_ID_DMA);
    IntEnable(INT_ID_DMA);

    DmaEnableInt(IDRAM_DMA_CH);
    IDRAM_DmaFinish = 0;

    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

    f1 = f3 = 0x55aa55aa;
    f2 = f4 = 0xaa55aa55;

    for(i = 0; i < 0x100; i++)
    {
        *pbuf_LLP_0++ = f1;
        *pbuf_LLP_1++ = f1;
        *pbuf_LLP_2++ = f1;
        *pbuf_LLP_3++ = f1;
        *pbuf_LLP_4++ = f1;
        *pbuf_LLP_5++ = f1;
        *pbuf_LLP_6++ = f1;
        *pbuf_LLP_7++ = f1;
        *pbuf_LLP_8++ = f1;
        *pbuf_LLP_9++ = f1;

        /*
        *pbuf1_LLP_0++ = 0;
        *pbuf1_LLP_1++ = 0;
        *pbuf1_LLP_2++ = 0;
        *pbuf1_LLP_3++ = 0;
        *pbuf1_LLP_4++ = 0;
        *pbuf1_LLP_5++ = 0;
        *pbuf1_LLP_6++ = 0;
        *pbuf1_LLP_7++ = 0;
        *pbuf1_LLP_8++ = 0;
        *pbuf1_LLP_9++ = 0;
        */
    }

    TEST_llpListn[IDRAM_DMA_CH][0].SAR = 0x01040000;
    TEST_llpListn[IDRAM_DMA_CH][0].DAR = 0x03040000;
    TEST_llpListn[IDRAM_DMA_CH][0].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][1].SAR = 0x01040500;
    TEST_llpListn[IDRAM_DMA_CH][1].DAR = 0x03040500;
    TEST_llpListn[IDRAM_DMA_CH][1].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][2].SAR = 0x01041000;
    TEST_llpListn[IDRAM_DMA_CH][2].DAR = 0x03041000;
    TEST_llpListn[IDRAM_DMA_CH][2].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][3].SAR = 0x01041500;
    TEST_llpListn[IDRAM_DMA_CH][3].DAR = 0x03041500;
    TEST_llpListn[IDRAM_DMA_CH][3].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][4].SAR = 0x01042000;
    TEST_llpListn[IDRAM_DMA_CH][4].DAR = 0x03042000;
    TEST_llpListn[IDRAM_DMA_CH][4].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][5].SAR = 0x01042500;
    TEST_llpListn[IDRAM_DMA_CH][5].DAR = 0x03042500;
    TEST_llpListn[IDRAM_DMA_CH][5].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][6].SAR = 0x01043000;
    TEST_llpListn[IDRAM_DMA_CH][6].DAR = 0x03043000;
    TEST_llpListn[IDRAM_DMA_CH][6].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][7].SAR = 0x01043500;
    TEST_llpListn[IDRAM_DMA_CH][7].DAR = 0x03043500;
    TEST_llpListn[IDRAM_DMA_CH][7].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][8].SAR = 0x01044000;
    TEST_llpListn[IDRAM_DMA_CH][8].DAR = 0x03044000;
    TEST_llpListn[IDRAM_DMA_CH][8].SIZE = 0x100;

    TEST_llpListn[IDRAM_DMA_CH][9].SAR = 0x01044500;
    TEST_llpListn[IDRAM_DMA_CH][9].DAR = 0x03044500;
    TEST_llpListn[IDRAM_DMA_CH][9].SIZE = 0x100;

    DmaConfig_for_LLP(IDRAM_DMA_CH, 0x100, 10,&DmaCfg, TEST_llpListn[IDRAM_DMA_CH]);
		while(!IDRAM_DmaFinish);


    for(i = 0; i < 0x100; i++)
    {
        f1 = *pbuf1_LLP_0++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_0 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_1++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_1 = 0x%x", f1);
        }


        f1 = *pbuf1_LLP_2++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_2 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_3++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_3 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_4++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_4 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_5++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_5 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_6++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_6 = 0x%x", f1);
        }
#if 1
        f1 = *pbuf1_LLP_7++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_7 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_8++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_8 = 0x%x", f1);
        }

        f1 = *pbuf1_LLP_9++;
        if((f1 & f3) != f3)
        {
            rk_printf("pbuf1_LLP_9 = 0x%x", f1);
        }
#endif

    }

    DmaDisableInt(IDRAM_DMA_CH);

    rk_print_string("\r\ndmallp test over");

    #undef H_IRAM_ADDR
    #undef H_IRAM_SIZE
    #undef H_DRAM_ADDR
    #undef H_DRAM_SIZE
}
/*******************************************************************************
** Name: ShellBspMemoryTest_DMA_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.27
** Time: 9:10:43
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTest_DMA_RW(HDC dev, uint8 * pstr)
{
    uint32 * pbuf, *pbuf1;
    uint32 i = 0;
    uint32 f1,f2,f3,f4;

    #define H_IRAM_ADDR 0x01040000
    #define H_IRAM_SIZE 0x40000

    #define H_DRAM_ADDR 0x03040000
    #define H_DRAM_SIZE 0x30000

    if(StrCmpA(pstr, "0", 1) == 0)
    {
        IDRAM_DMA_CH = 0;
        rk_print_string("\r\n IDRAM_DMA_CH start 0\n");
    }
    else if(StrCmpA(pstr, "1", 1) == 0)
    {
        IDRAM_DMA_CH = 1;
        rk_print_string("\r\n IDRAM_DMA_CH start 1\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        IDRAM_DMA_CH = 2;
        rk_print_string("\r\n IDRAM_DMA_CH start 2\n");
    }
    else if(StrCmpA(pstr, "3", 1) == 0)
    {
        IDRAM_DMA_CH = 3;
        rk_print_string("\r\n IDRAM_DMA_CH start 3\n");
    }
    else if(StrCmpA(pstr, "4", 1) == 0)
    {
        IDRAM_DMA_CH = 4;
        rk_print_string("\r\n IDRAM_DMA_CH start 4\n");
    }
    else if(StrCmpA(pstr, "5", 1) == 0)
    {
        IDRAM_DMA_CH = 5;
        rk_print_string("\r\n IDRAM_DMA_CH start 5\n");
    }
    //open uart clk
    ScuClockGateCtr(CLOCK_GATE_DMA, 1);

    //open rst uart ip
    ScuSoftResetCtr(RST_DMA, 1);
    DelayMs(1);
    ScuSoftResetCtr(RST_DMA, 0);

    IntRegister(INT_ID_DMA ,IDRAM_DmaIsr);

    IntPendingClear(INT_ID_DMA);
    IntEnable(INT_ID_DMA);

    DmaEnableInt(IDRAM_DMA_CH);
    IDRAM_DmaFinish = 0;

    DMA_CFGX DmaCfg = {DMA_CTLL_M2M_WORD, DMA_CFGL_M2M_WORD, DMA_CFGH_M2M_WORD, 0};

    f1 = f3 = 0x55aa55aa;
    f2 = f4 = 0xaa55aa55;

//------------------------------------------------------------//
    pbuf = (uint32 *)H_IRAM_ADDR;

    for(i = 0; i < H_DRAM_SIZE/4; i++)
    {
        *pbuf++ = f1;
    }

    DmaConfig(IDRAM_DMA_CH, H_IRAM_ADDR, H_DRAM_ADDR, H_DRAM_SIZE/4, &DmaCfg, TEST_llpListn[IDRAM_DMA_CH]);
		while(!IDRAM_DmaFinish);

    DmaDisableInt(IDRAM_DMA_CH);

    pbuf1 = (uint32 *)H_DRAM_ADDR;

    f1 = 0;

    for(i = 0; i < H_DRAM_SIZE/4; i++)
    {
        f1 = *pbuf1++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = 0x%x", f1);
            rk_printf("err address = 0x%x", pbuf1);
        }

        if((i%500) == 0)
        {
           printf("0x%x,", f1);
        }
    }
    rk_print_string("\r\n 55aa test over\n");
//-------------------------------------------------------------------//
    pbuf = (uint32 *)H_IRAM_ADDR;

    for(i = 0; i < H_DRAM_SIZE/4; i++)
    {
        *pbuf++ = f2;
    }

    DmaEnableInt(IDRAM_DMA_CH);
    IDRAM_DmaFinish = 0;

    DmaConfig(IDRAM_DMA_CH, H_IRAM_ADDR, H_DRAM_ADDR, H_DRAM_SIZE/4, &DmaCfg, TEST_llpListn[IDRAM_DMA_CH]);
		while(!IDRAM_DmaFinish);

    DmaDisableInt(IDRAM_DMA_CH);

    pbuf1 = (uint32 *)H_DRAM_ADDR;

    f2 = 0;

    for(i = 0; i < H_DRAM_SIZE/4; i++)
    {
        f2 = *pbuf1++;
        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = 0x%x", f2);
            rk_printf("err address = 0x%x", pbuf1);
        }

        if((i%500) == 0)
        {
           printf("0x%x,", f2);
        }
    }
    rk_print_string("\r\n aa55 test over\n");
    #undef H_IRAM_ADDR
    #undef H_IRAM_SIZE
    #undef H_DRAM_ADDR
    #undef H_DRAM_SIZE
    rk_print_string("\r\ndma test over");

}

/*******************************************************************************
** Name: ShellBspMemoryTestPMUSRAM_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.24
** Time: 19:53:07
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestPMUSRAM_RW(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;
    uint32 i = 0;
    uint32 f1,f2,f3,f4;
    #define PMU_SRAM_ADDR0 0x00000000
    #define PMU_SRAM_ADDR1 0x00ff0000
    #define PMU_SRAM_SIZE  0x10000
    //test pmu sram

    pbuf = (uint8 *)PMU_SRAM_ADDR1;

    if(StrCmpA(pstr, "1", 1) == 0)
    {
        f1 = f3 = 0x55;
        f2 = f4 = 0xaa;
        rk_print_string("\r\npmu sram read write test start 0x55aa\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        f1 = f3 = 0xaa;
        f2 = f4 = 0x55;
        rk_print_string("\r\npmu sram read write test start 0xaa55\n");
    }



    for(i = 0; i < PMU_SRAM_SIZE/2; i++)
    {
        *pbuf++ = f1;
        *pbuf++ = f2;
    }

    pbuf = (uint8 *)PMU_SRAM_ADDR1;

    for(i = 0; i < PMU_SRAM_SIZE/2; i++)
    {
        f1 = *pbuf++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = %x", f1);
            rk_printf("err address = %x", pbuf);
        }

        f2 = *pbuf++;

        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = %x", f2);
            rk_printf("err address = %x", pbuf);
        }

        if((i%500) == 0)
        {
           printf("%x,%x,", f1,f2);
        }
    }

    rk_print_string("\r\npmu sram read write test over");
    #undef PMU_SRAM_ADDR0
    #undef PMU_SRAM_ADDR1
    #undef PMU_SRAM_SIZE
}
/*******************************************************************************
** Name: ShellBspMemoryTestHIRAM_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.24
** Time: 19:51:37
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestHIRAM_RW(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;
    uint32 i = 0;
    uint8 f1,f2,f3,f4;

    #define H_IRAM_ADDR 0x01040000
    #define H_IRAM_SIZE 0x40000

    //test H iram
    pbuf = (uint8 *)H_IRAM_ADDR;

    if(StrCmpA(pstr, "1", 1) == 0)
    {
        f1 = f3 = 0x55;
        f2 = f4 = 0xaa;
        rk_print_string("\r\nHiram read write test start 0x55aa\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        f1 = f3 = 0xaa;
        f2 = f4 = 0x55;
        rk_print_string("\r\nHiram read write test start 0xaa55\n");
    }

    for(i = 0; i < H_IRAM_SIZE/2; i++)
    {
        *pbuf++ = f1;
        *pbuf++ = f2;
    }

    pbuf = (uint8 *)H_IRAM_ADDR;

    for(i = 0; i < H_IRAM_SIZE/2; i++)
    {
        f1 = *pbuf++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = %x", f1);
            rk_printf("err address = %x", pbuf);
        }

        f2 = *pbuf++;

        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = %x", f2);
            rk_printf("err address = %x", pbuf);
        }

        if((i%500) == 0)
        {
           printf("%x,%x,", f1,f2);
        }
    }

    rk_print_string("\r\nhiram read write test over");
    #undef H_IRAM_ADDR
    #undef H_IRAM_SIZE
}
/*******************************************************************************
** Name: ShellBspMemoryTestHDRAM_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.24
** Time: 19:49:32
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestHDRAM_RW(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;
    uint32 i = 0;
    uint8 f1,f2,f3,f4;

    #define H_DRAM_ADDR 0x03040000
    #define H_DRAM_SIZE 0x30000

    //test H dram
    pbuf = (uint8 *)H_DRAM_ADDR;

    if(StrCmpA(pstr, "1", 1) == 0)
    {
        f1 = f3 = 0x55;
        f2 = f4 = 0xaa;
        rk_print_string("\r\nHdram read write test start 0x55aa\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        f1 = f3 = 0xaa;
        f2 = f4 = 0x55;
        rk_print_string("\r\nHdram read write test start 0xaa55\n");
    }

    for(i = 0; i < H_DRAM_SIZE/2; i++)
    {
        *pbuf++ = f1;
        *pbuf++ = f2;
    }

    pbuf = (uint8 *)H_DRAM_ADDR;

    for(i = 0; i < H_DRAM_SIZE/2; i++)
    {
        f1 = *pbuf++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = %x", f1);
            rk_printf("err address = %x", pbuf);
        }

        f2 = *pbuf++;

        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = %x", f2);
            rk_printf("err address = %x", pbuf);
        }

        if((i%500) == 0)
        {
           printf("%x,%x,", f1,f2);
        }
    }

    rk_print_string("\r\nhdram read write test over");
    #undef H_DRAM_ADDR
    #undef H_DRAM_SIZE
}
/*******************************************************************************
** Name: ShellBspMemoryTestDRAM_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.24
** Time: 19:45:16
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestDRAM_RW(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;
    uint32 i = 0;
    uint8 f1,f2,f3,f4;
    #define DRAM_ADDR 0x03000000
    #define DRAM_SIZE 0x40000


    //test H dram
    pbuf = (uint8 *)DRAM_ADDR;

    if(StrCmpA(pstr, "1", 1) == 0)
    {
        f1 = f3 = 0x55;
        f2 = f4 = 0xaa;
        rk_print_string("\r\ndram read write test start 0x55aa\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        f1 = f3 = 0xaa;
        f2 = f4 = 0x55;
        rk_print_string("\r\ndram read write test start 0xaa55\n");
    }


    for(i = 0; i < DRAM_SIZE/2; i++)
    {
        *pbuf++ = f1;
        *pbuf++ = f2;
    }

    pbuf = (uint8 *)DRAM_ADDR;

    for(i = 0; i < DRAM_SIZE/2; i++)
    {
        f1 = *pbuf++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = %x", f1);
            rk_printf("err address = %x", pbuf);
        }

        f2 = *pbuf++;

        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = %x", f2);
            rk_printf("err address = %x", pbuf);
        }

        if((i%500) == 0)
        {
           printf("%x,%x,", f1,f2);
        }
    }

    rk_print_string("\r\ndram read write test over");

    #undef DRAM_ADDR
    #undef DRAM_SIZE
}
/*******************************************************************************
** Name: ShellBspMemoryTestIRAM_RW
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:hj
** Date: 2014.11.24
** Time: 19:42:02
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestIRAM_RW(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;
    uint32 i = 0;
    uint8 f1,f2,f3,f4;

    #define IRAM_ADDR 0x01000000
    #define IRAM_SIZE 0x40000

    //test H dram
    pbuf = (uint8 *)IRAM_ADDR;

    if(StrCmpA(pstr, "1", 1) == 0)
    {
        f1 = f3 = 0x55;
        f2 = f4 = 0xaa;
        rk_print_string("\r\niram read write test start 0x55aa\n");
    }
    else if(StrCmpA(pstr, "2", 1) == 0)
    {
        f1 = f3 = 0xaa;
        f2 = f4 = 0x55;
        rk_print_string("\r\niram read write test start 0xaa55\n");
    }

    for(i = 0; i < IRAM_SIZE/2; i++)
    {
        *pbuf++ = f1;
        *pbuf++ = f2;
    }

    pbuf = (uint8 *)IRAM_ADDR;

    for(i = 0; i < IRAM_SIZE/2; i++)
    {
        f1 = *pbuf++;
        if((f1 & f3) != f3)
        {
            rk_printf("err data f1 = %x", f1);
            rk_printf("err address = %x", pbuf);
        }

        f2 = *pbuf++;

        if((f2 & f4) != f4)
        {
            rk_printf("err data f2 = %x", f2);
            rk_printf("err address = %x", pbuf);
        }

        if((i%500) == 0)
        {
           printf("%x,%x,", f1,f2);
        }
    }

    rk_print_string("\r\niram read write test over");
    #undef IRAM_ADDR
    #undef IRAM_SIZE


}

/*******************************************************************************
** Name: ShellBspMemoryTestReadWrite
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:59:02
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestReadWrite(HDC dev, uint8 * pstr)
{
    uint8 * pbuf, *pbuf1;

    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);
    if(StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellBspMemoryRAMName, pItem, StrCnt);


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
            #ifdef SHELL_HELP
            ret = ShellBspMemoryHelp(dev, pItem);
            #endif
            break;

        case 0x01:
            ret = ShellBspMemoryTestIRAM_RW(dev, pItem);
            break;

        case 0x02:
            ret = ShellBspMemoryTestDRAM_RW(dev, pItem);
            break;

        case 0x03:
            ret = ShellBspMemoryTestHIRAM_RW(dev, pItem);
            break;

        case 0x04:
            ret = ShellBspMemoryTestHDRAM_RW(dev, pItem);
            break;

        case 0x05:
            ret = ShellBspMemoryTestPMUSRAM_RW(dev, pItem);
            break;
        case 0x06:
            ret = ShellBspMemoryTest_DMA_RW(dev, pItem);
            break;
        case 0x07:
            ret = ShellBspMemoryTest_DMA_LLP_RW(dev, pItem);
            break;
        default:
            ret = RK_ERROR;
            break;
    }
    //rk_print_string("\r\nmemory test over");

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: ShellBspMemoryTestClk
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:56:10
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestClk(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspMemoryTestPower
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:55:37
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryTestPower(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicResumeNanoD
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:54:40
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicResumeNanoD(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicResumeM3
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:54:12
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicResumeM3(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicTestPriority
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:53:11
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicTestPriority(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

uint32 IntCnt;

void TestInt(void)
{
    IntCnt++;
}

/*******************************************************************************
** Name: ShellBspNvicTestInt
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:52:30
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicTestInt(HDC dev, uint8 * pstr)
{
    uint32 i;
    IntCnt = 15;

    for(i = 16; i < NUM_INTERRUPTS; i++)
    {
        if(i == INT_ID_UART0)
        {
			IntCnt++;
			continue;
        }

        IntRegister(i ,TestInt);
        IntPendingClear(i);
        IntEnable(i);

        IntPendingSet(i);

        while(1)
        {
            if(IntCnt == i)
            {
                rk_printf_no_time("Int = %d test ok", i);
                break;
            }
        }

        IntDisable(i);

        IntPendingClear(i);

        IntUnregister(i);

    }

    rk_print_string("\r\nnvic int test over");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicSetMask
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:51:36
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicSetMask(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicSetPriority
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:50:56
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicSetPriority(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicSetInt
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:50:14
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicSetInt(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

#ifdef SHELL_HELP

/*******************************************************************************
** Name: ShellBspMailBoxHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:48:57
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBoxHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ShellBspGpioHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:48:23
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGpioHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ShellBspMemoryHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:47:31
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemoryHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspNvicHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:41:28
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvicHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: ShellBspHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 10:40:03
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspHelp(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

#endif

/*******************************************************************************
** Name: ShellBspMailBox
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 9:16:04
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMailBox(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if(StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellBspMailBoxName, pItem, StrCnt);
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
            #ifdef SHELL_HELP
            ret = ShellBspMailBoxHelp(dev, pItem);
            #endif
            break;

        case 0x01:
            ret = ShellBspMailBoxWriteData(dev, pItem);
            break;

        case 0x02:
            ret = ShellBspMailBoxWriteCmd(dev, pItem);
            break;

        case 0x03:
            ret = ShellBspMailBoxReadData(dev, pstr);
            break;

        case 0x04:
            ret = ShellBspMailBoxReadCmd(dev, pstr);
            break;

        case 0x05:
            ret = ShellBspMailBoxTestA2B(dev, pstr);
            break;

        case 0x06:
            ret = ShellBspMailBoxTestB2A(dev, pstr);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}


/*******************************************************************************
** Name: ShellBspNvic
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 9:14:32
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspNvic(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if(StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellBspNvicName, pItem, StrCnt);
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
            #ifdef SHELL_HELP
            ret = ShellBspNvicHelp(dev, pItem);
            #endif
            break;

        case 0x01:
            ret = ShellBspNvicSetInt(dev, pItem);
            break;

        case 0x02:
            ret = ShellBspNvicSetPriority(dev, pItem);
            break;

        case 0x03:
            ret = ShellBspNvicSetMask(dev, pstr);
            break;

        case 0x04:
            ret = ShellBspNvicTestInt(dev, pstr);
            break;

        case 0x05:
            ret = ShellBspNvicTestPriority(dev, pstr);
            break;

        case 0x06:
            ret = ShellBspNvicResumeM3(dev, pstr);
            break;

        case 0x07:
            ret = ShellBspNvicResumeNanoD(dev, pstr);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}

/*******************************************************************************
** Name: ShellBspGPIO
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 9:14:03
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspGPIO(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ShellBspMemory
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.11
** Time: 9:13:00
*******************************************************************************/
_SYSTEM_SHELL_SHELLBSP_READ_
COMMON FUN rk_err_t ShellBspMemory(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if(StrCnt == 0)
        return RK_ERROR;


    ret = ShellCheckCmd(ShellBspMemoryName, pItem, StrCnt);
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
            #ifdef SHELL_HELP
            ret = ShellBspMemoryHelp(dev, pItem);
            #endif
            break;

        case 0x01:
            ret = ShellBspMemoryTestPower(dev, pItem);
            break;

        case 0x02:
            ret = ShellBspMemoryTestClk(dev, pItem);
            break;

        case 0x03:
            ret = ShellBspMemoryTestReadWrite(dev, pItem);
            break;

        case 0x04:
            ret = ShellBspMemoryTestConsumption(dev, pstr);
            break;

        case 0x05:
            ret = ShellBspMemoryTestEffect(dev, pstr);
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

