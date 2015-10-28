/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Bsp\EVK_V2.0\BSP.c
* Owner: aaron.sun
* Date: 2015.5.14
* Time: 15:23:47
* Version: 1.0
* Desc: EVK_V2.0 Bsp Init
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.5.14     15:23:47   1.0
********************************************************************************************
*/

#define __BSP_EVK_V2_0_BSP_C__
#include "BspConfig.h"
#ifdef __BSP_EVK_V2_0_BSP_C__

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

#include <stdarg.h>


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define DEBUG_TIME_LEN  8

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static uint32  SysTickCntSave;
static uint32  SysTickSave;

#ifdef _LOG_DEBUG_
uint32 BcoreRequestDebug;
uint32 BcoreDebug;
uint8 * pBBDebugBuf;
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
uint32 SysTickCounter;                            //在SYSTICK中进行增加
void * UartHDC;
void (*pIdleFun)(void);
void (*pIdleFunSave)(void);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
void CpuIdle(void);
void GpioInit(void);
void CPUInit(void);
void SysTickInit(void);
void SysVariableInit(void);
void SysInterruptInit(void);
void fputc_hook(char ch);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef _LOG_DEBUG_
/*******************************************************************************
** Name: BBReqDebug
** Input:uint8 * buf
** Return: void
** Owner:aaron.sun
** Date: 2015.6.13
** Time: 16:04:06
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void BBReqDebug(uint8 * buf)
{
      pBBDebugBuf = buf;
      BcoreDebug = 1;
}

/*******************************************************************************
** Name: BBDebug
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.13
** Time: 16:03:13
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void BBDebug(void)
{
    if (BcoreDebug)
    {
        UartDev_Write(UartHDC, pBBDebugBuf, StrLenA(pBBDebugBuf), SYNC_MODE, NULL);

        MailBoxWriteA2BCmd(MSGBOX_CMD_SYSTEM_PRINT_LOG_OK, MAILBOX_ID_0, MAILBOX_CHANNEL_0);
        MailBoxWriteA2BData(0, MAILBOX_ID_0, MAILBOX_CHANNEL_0);
        BcoreDebug = 0;
    }

}
#endif

/*******************************************************************************
** Name: vApplicationIdleHook
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:17:37
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void vApplicationIdleHook(void)
{
    while(1)
    {
        if (pIdleFun != NULL)
        {
            pIdleFun();
        }
    }
}

/*******************************************************************************
** Name: ShellItemExtract
** Input:uint8 * pstr, uint8 ** pItem, uint8 * Space
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:51:49
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 ShellItemExtract(uint8 * pstr, uint8 ** pItem, uint8 * Space)
{
    uint32 i = 0;
    uint16 ItemSize = 0;

    (*pItem) = (uint8 *)(pstr + i);

    while ((*(pstr + i) != ' ') && (*(pstr + i) != '.') && (*(pstr + i) != NULL))
    {
        ItemSize++;
        i++;
    }

    *Space = *(pstr + i);

    return ItemSize;

}
/*******************************************************************************
** Name: ShellCheckCmd
** Input:uint8 * CmdStr,  uint8 * Cmd,  uint32 len
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:36:37
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API rk_err_t ShellCheckCmd(uint8 * CmdStr[],  uint8 * Cmd,  uint32 len)
{

    int32 i = 0;

    while (1)   //compare the name
    {
        if (len != StrLenA(CmdStr[i]))
        {
            i++;
        }
        else
        {
            if (StrCmpA(CmdStr[i], Cmd, len) != 0)
            {
                i++;
            }
            else
            {
                break;
            }
        }

        if (*(CmdStr[i]) == '\b')
            return RK_ERROR;

    }

    return i;

}

/*******************************************************************************
** Name: Unicode2Ascii
** Input:uint8 * pbAscii, uint16 * pwUnicode, uint32 len
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:22:40
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 Unicode2Ascii(uint8 * pbAscii, uint16 * pwUnicode, uint32 len)
{
    uint32 i;
    uint8 * pbUnicode = (uint8 *)pwUnicode;

    for (i = 0; i < len;)
    {
        if ((*pbUnicode == 0) && (*(pbUnicode + 1) == 0))
        {
            pbAscii[i] = 0;
            break;
        }

        if (*pbUnicode != 0)
        {
            pbAscii[i] = *pbUnicode;
            i++;
        }

        if (*(pbUnicode + 1) != 0)
        {
            pbAscii[i] = *(pbUnicode + 1);
            i++;
        }
        pbUnicode += 2;
    }

    return i;
}
/*******************************************************************************
** Name: Ascii2Unicode
** Input:uint8 * pbAscii, uint16 * pwUnicode, uint32 len
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:21:35
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 Ascii2Unicode(uint8 * pbAscii, uint16 * pwUnicode, uint32 len)
{
    uint32 i;

    for (i = 0; i < len / 2; i++)
    {
        pwUnicode[i] = (uint16)pbAscii[i];
    }
    return i * 2;
}

/*******************************************************************************
** Name: String2Num
** Input:uint8 * pstr
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:08:07
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 String2Num(uint8 * pstr)
{
    uint32 num = 0;
    int32 i;

    /*ptr save the fisrt char of pstr*/
    const uint8* ptr = pstr;

    /*if pstr start with '-' or '+' sign,move to the next charactor.*/
    if (*pstr == '-' || *pstr == '+')
        pstr++;

    while (*pstr != 0)
    {
        /*if pstr is none digal number ,break and return*/
        if (*pstr < '0' || *pstr > '9')
            break;
        /*current is digal ,calculate the value*/
        num = num * 10 + (*pstr - '0');
        pstr++;
    }

    if ( *ptr == '-')
        num = -num;

    return num;
}
/*******************************************************************************
** Name: MemCpy
** Input:uint8 * trg, uint8 * src, uint32 len
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:07:14
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void MemCpy(uint8 * trg, uint8 * src, uint32 len)
{
    uint32 i;

    for (i = 0; i < len; i++)
    {
        *trg++ = *src++;
    }
}
/*******************************************************************************
** Name: StrCmpA
** Input:uint8 * pstr1, uint8 * pstr2, uint32 len
** Return: int32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:06:04
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API int32 StrCmpA(uint8 * pstr1, uint8 * pstr2, uint32 len)
{
    uint32 len1, len2;

    if (len == 0)
    {
        len1 = StrLenA(pstr1);
        len2 = StrLenA(pstr2);

        if (len1 > len2)
        {
            return 1;
        }
        else if (len1 < len2)
        {
            return -1;
        }
        else
        {
            len = len1;
        }
    }


    while (len--)
    {
        if ((*pstr1 == 0) && (*pstr2 == 0))
        {
            return 0;
        }
        else if (*pstr1 == 0)
        {
            return -1;
        }
        else if (*pstr2 == 0)
        {
            return 1;
        }
        if (*pstr1 > * pstr2)
        {
            return 1;
        }
        else if (*pstr1 < * pstr2)
        {
            return -1;
        }
        pstr1++;
        pstr2++;
    }

    return 0;

}
/*******************************************************************************
** Name: StrCmpW
** Input:uint16 * pstr1, uint16 * pstr2, uint32 len
** Return: int32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:04:53
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API int32 StrCmpW(uint16 * pstr1, uint16 * pstr2, uint32 len)
{
    uint32 len1, len2;

    if (len == 0)
    {
        len1 = StrLenW(pstr1);
        len2 = StrLenW(pstr2);

        if (len1 > len2)
        {
            return 1;
        }
        else if (len1 < len2)
        {
            return -1;
        }
        else
        {
            len = len1;
        }
    }

    while (len--)
    {
        if ((*pstr1 == 0) && (*pstr2 == 0))
        {
            return 0;
        }
        else if (*pstr1 == 0)
        {
            return -1;
        }
        else if (*pstr2 == 0)
        {
            return 1;
        }
        if (*pstr1 > * pstr2)
        {
            return 1;
        }
        else if (*pstr1 < * pstr2)
        {
            return -1;
        }
        pstr1++;
        pstr2++;
    }

    return 0;

}
/*******************************************************************************
** Name: StrLenA
** Input:uint8 * pstr
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:03:38
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 StrLenA(uint8 * pstr)
{
    uint32 i = 0;
    while (*pstr++ != 0)
        i++;

    return i;
}
/*******************************************************************************
** Name: StrLenW
** Input:uint16 * pstr
** Return: uint32
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:02:57
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API uint32 StrLenW(uint16 * pstr)
{
    uint32 i = 0;
    while (*pstr++ != 0)
        i++;

    return i;
}
/*******************************************************************************
** Name: rk_count_clk_end
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:59:48
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void rk_count_clk_end(void)
{
    uint32 SysTick, SysTickCnt;
    uint32 clk;

    SysTickCnt = SysTickCounter;
    SysTick = nvic->SysTick.Value;

    if (SysTickCntSave == SysTickCnt)
    {
        clk = SysTickSave - SysTick;
    }
    else
    {
        clk = SysTickSave + (SysTickCnt - SysTickCntSave - 1) * nvic->SysTick.Reload
              + (nvic->SysTick.Reload - SysTick);
    }

    printf("\ntotal clk = %u", clk);

}

/*******************************************************************************
** Name: rk_count_clk_start
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:58:24
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void rk_count_clk_start(void)
{
    SysTickCntSave = SysTickCounter;
    SysTickSave = nvic->SysTick.Value;
}

/*******************************************************************************
** Name: rk_print_string
** Input:const uint8 * pstr
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:57:38
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void rk_print_string(const uint8 * pstr)
{
    if (pstr[0] == '\n')
    {
        UartDev_Write(UartHDC, "\r\n[A]", 5, SYNC_MODE, NULL);
        UartDev_Write(UartHDC, pstr + 1, strlen(pstr) - 1, SYNC_MODE, NULL);

    }
    else
    {

        UartDev_Write(UartHDC, pstr, strlen(pstr), SYNC_MODE, NULL);
    }


}
/*******************************************************************************
** Name: rk_printf_no_time
** Input:const char * fmt,...
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:56:37
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void rk_printf_no_time(const char * fmt,...)
{
    uint8 buf[256];
    uint8 size;

    va_list arg_ptr;
    va_start(arg_ptr, fmt);

    UartDev_Write(UartHDC, "\r\n[A]", 5, SYNC_MODE, NULL);

    size =  vsnprintf((char*)buf, 255, (const char *)fmt, arg_ptr);
    UartDev_Write(UartHDC, buf, size,  SYNC_MODE, NULL);


}
/*******************************************************************************
** Name: debug_hex
** Input:char *buf, int BufSize, int radix
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:51:57
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void debug_hex(char *buf, int BufSize, int radix)
{

    int i;

    for (i=0; i<BufSize; i++)
    {
        if ((i>0)&&((i % 10) == 0))
            printf("\n");

        if (radix == 10)
            printf(" %d", buf[i]);
        else
            printf(" %02x", buf[i]);
    }
    printf("\n\n");

}

/*******************************************************************************
** Name: rk_printf
** Input:const char * fmt,...
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:45:24
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API void rk_printf(const char * fmt,...)
{

    uint8 buf[256];
    uint8 size;
    uint32 tmpcnt,i,j;
    uint8 dgbBuffer[DEBUG_TIME_LEN];
    va_list arg_ptr;

    tmpcnt = SysTickCounter;
    for (i = 0; i < DEBUG_TIME_LEN; i++)
    {
        dgbBuffer[i] = tmpcnt % 10;
        tmpcnt = tmpcnt / 10;
    }

    buf[0] = '\r';
    buf[1] = '\n';
    buf[2] = '[';

    buf[3] = 'A';

    buf[4] = ']';

    buf[5] = '[';

    for (i = 0 ,j = 0; i < DEBUG_TIME_LEN; i++)
    {
        buf[j + 6] = dgbBuffer[DEBUG_TIME_LEN - 1 - i] + 0x30;
        if (DEBUG_TIME_LEN - 1 - i == 2)
        {
            j++;
            buf[j + 6] = '.';
        }
        j++;
    }

    buf[j + 6] = ']';

    va_start(arg_ptr, fmt);

    size =  vsnprintf((char*)(buf + j + 7), 255, (const char *)fmt, arg_ptr);
    UartDev_Write(UartHDC, buf, size + 16,  SYNC_MODE, NULL);


}

/*******************************************************************************
** Name: fputc
** Input:int ch, FILE *f
** Return: int
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:39:24
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON API int fputc(int ch, FILE *f)
{
    uint8 dgbBuffer[DEBUG_TIME_LEN];
    uint32 tmpcnt, i;

    if (ch == '\n')
    {
        tmpcnt = SysTickCounter;

        for (i = 0; i < DEBUG_TIME_LEN; i++)
        {
            dgbBuffer[i] = tmpcnt % 10;
            tmpcnt = tmpcnt / 10;
        }

        fputc_hook('\r');
        fputc_hook('\n');
        fputc_hook('[');
        for (i = 0; i < DEBUG_TIME_LEN; i++)
        {
            fputc_hook(dgbBuffer[DEBUG_TIME_LEN - 1 -i]+0x30);
            if (DEBUG_TIME_LEN - 1 -i == 2)
            {
                fputc_hook('.');
            }
        }
        fputc_hook(']');

        return RK_SUCCESS;
    }

    fputc_hook(ch);

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
** Name: CpuIdle
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:18:49
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON FUN void CpuIdle(void)
{

    if (SysState == LEVEL_INIT)
    {
        SysWakeCnt = SysTickCounter;
        PrevSysWakeCnt = SysWakeCnt;
        PrevSysTickCounter = SysTickCounter;
        SysState = LEVEL_0;
    }
    else if (SysState != LEVEL_0)
    {
        SysResume(SysState);

        //printf("r\n");
        SysState = LEVEL_0;

        SysWakeCnt = SysTickCounter;
        PrevSysWakeCnt = SysWakeCnt;
        PrevSysTickCounter = SysTickCounter;
    }
    else
    {
        __WFI();

        SysWakeCnt++;

        if ((SysWakeCnt - PrevSysWakeCnt) == (SysTickCounter - PrevSysTickCounter))
        {
            if ((SysWakeCnt - PrevSysWakeCnt) >= 300) //wait 3s
            {
                //SysState = LEVEL_1;
                //if(FW_ModuleOverlay(MODULE_ID_RESUME, MODULE_OVERLAY_CODE) != RK_SUCCESS)
                {

                }
                //SysTickDisable();
                //printf("s");
            }
        }
        else
        {
            //printf("%d\n", SysTickCounter - PrevSysTickCounter);
            SysWakeCnt = SysTickCounter;
            PrevSysWakeCnt = SysWakeCnt;
            PrevSysTickCounter = SysTickCounter;
        }
    }


}
/*******************************************************************************
** Name: fputc_hook
** Input:char ch
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:43:49
*******************************************************************************/
_BSP_EVK_V20_BSP_COMMON_
COMMON FUN void fputc_hook(char ch)
{
    while (UARTWriteByte(UART_DEV1,(uint8*)&ch, 1) == 0);
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
/*******************************************************************************
** Name: DisplayDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:cjh
** Date: 2015.6.26
** Time: 13:38:16
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_INIT_
INIT API void DisplayDevHwInit(uint32 DevID, uint32 Channel)
{
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin0,IOMUX_GPIO0C0_LCD_D0);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1,IOMUX_GPIO0C1_LCD_D1);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2,IOMUX_GPIO0C2_LCD_D2);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3,IOMUX_GPIO0C3_LCD_D3);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4,IOMUX_GPIO0C4_LCD_D4);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5,IOMUX_GPIO0C5_LCD_D5);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6,IOMUX_GPIO0C6_LCD_D6);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin7,IOMUX_GPIO0C7_LCD_D7);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin0,IOMUX_GPIO0D0_LCD_WRN);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin1,IOMUX_GPIO0D1_LCD_RS);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin7,IOMUX_GPIO0B7_LCD_CSN);
    ScuClockGateCtr(HCLK_LCDC_GATE, 1);
    ScuSoftResetCtr(LCDC_SRST, 1);
    DelayMs(1);
    ScuSoftResetCtr(LCDC_SRST, 0);

    IntRegister(INT_ID_VOP ,DisplayDevIntIsr);
    IntPendingClear(INT_ID_VOP);
    IntEnable(INT_ID_VOP);
}

#endif

#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
/*******************************************************************************
** Name: TimerDevHwInit
** Input:HDC dev
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.2
** Time: 15:32:43
*******************************************************************************/
_DRIVER_TIMER_TIMERDEVICE_INIT_
INIT API void TimerDevHwInit(uint32 DevID, uint32 Channel)
{
    rk_printf ("Timer%d HardWare Init\n",DevID);
    if (DevID == 0)
    {
        //Timer0
        rk_printf ("Timer%d reset\n",DevID);
        ScuClockGateCtr( CLK_TIMER0_GATE, 1);
        ScuClockGateCtr( PCLK_TIMER_GATE, 1);

        ScuSoftResetCtr(TIMER0_SRST, 0);
        SetTimerFreq(TIMER0,XIN24M,24*1000*1000);
    }
    else if (DevID == 1)
    {
        //Timer1
        ScuClockGateCtr( CLK_TIMER0_GATE, 1);
        ScuClockGateCtr( PCLK_TIMER_GATE, 1);

        ScuSoftResetCtr(TIMER1_SRST, 0);
        SetTimerFreq(TIMER1,XIN24M,24*1000*1000);
    }

    if (DevID == 0)
    {
        printf("timer0 inttrupt init\n");
        IntUnregister(INT_ID_TIMER0);
        IntPendingClear(INT_ID_TIMER0);
        IntDisable(INT_ID_TIMER0);

        IntRegister(INT_ID_TIMER0, TimerIntIsr0);
        IntPendingClear(INT_ID_TIMER0);
        IntEnable(INT_ID_TIMER0);

        TimerStop(TIMER0);
        TimerInit(TIMER0,TIMER_FREE_RUNNING);
        TimerSetCount(TIMER0,(UINT64)24*1000*1000);
        TimerIntUnmask(TIMER0);
    }
    else if (DevID == 1)
    {
        IntUnregister(INT_ID_TIMER1);
        IntPendingClear(INT_ID_TIMER1);
        IntDisable(INT_ID_TIMER1);

        IntRegister(INT_ID_TIMER1, TimerIntIsr1);
        IntPendingClear(INT_ID_TIMER1);
        IntEnable(INT_ID_TIMER1);

        TimerStop(TIMER1);
        TimerInit(TIMER1,TIMER_FREE_RUNNING);
        TimerSetCount(TIMER1,(UINT64)24*1000*1000);
        TimerIntUnmask(TIMER1);
    }
}

#endif


#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
/*******************************************************************************
** Name: BBSystemInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.5
** Time: 10:27:18
*******************************************************************************/
_DRIVER_BCORE_BCOREDEVICE_INIT_
INIT API void BcoreHwInit(uint32 DevID, uint32 Channel)
{
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

}
#endif


#ifdef __DRIVER_SD_SDDEVICE_C__
/*******************************************************************************
** Name: SdDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.6.9
** Time: 16:40:42
*******************************************************************************/
_DRIVER_SD_SDDEVICE_INIT_
INIT API void SdDevHwInit(uint32 DevID, uint32 Channel)
{

}

#endif


#ifdef __DRIVER_EMMC_EMMCDEVICE_C__
/*******************************************************************************
** Name: EmmcDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:API
** Date: 2015.5.20
** Time: 19:16:20
*******************************************************************************/
_DRIVER_EMMC_EMMCDEVICE_INIT_
INIT API void EmmcDevHwInit(uint32 DevID, uint32 Channel)
{
     Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin0, IOMUX_GPIO0A0_IO);
     Gpio_SetPinDirection(GPIO_CH0, GPIOPortA_Pin0, GPIO_OUT);
     Gpio_SetPinLevel(GPIO_CH0, GPIOPortA_Pin0, GPIO_LOW);
     DelayMs(100);
     //Gpio_SetPinLevel(GPIO_CH0, GPIOPortA_Pin0, GPIO_HIGH);
     DelayMs(100);
}
#endif


#ifdef __DRIVER_I2S_I2SDEVICE_C__
/*
** Name: I2SDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:cjh
** Date: 2015.5.19
** Time: 17:36:58
*******************************************************************************/
_DRIVER_I2S_I2SDEVICE_INIT_
INIT API void I2SDevHwInit(uint32 DevID, uint32 Channel)
{
    switch (DevID)
    {
        case I2S_CH0:
            switch (Channel)
            {
                case I2S_SEL_ACODEC:
                    rk_printf("select I2S DevID 0, I2S_SEL_ACODEC\n");
                    Grf_I2S0_Sel(I2S_SEL_ACODEC);//Select NAND interior codec
                    break;
                case I2S_SEL_PAD:
                    rk_printf("select I2S DevID 0, I2S_SEL_PAD\n");
                    Grf_I2S0_Sel(I2S_SEL_PAD);//Select external codec
                    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin7, IOMUX_GPIO0B7_I2S0_CLK);
                    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin5, IOMUX_GPIO0B5_I2S0_SCLK);
                    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin3, IOMUX_GPIO0B3_I2S0_SDI);
                    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin6, IOMUX_GPIO0B6_I2S0_LRCK);
                    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin4, IOMUX_GPIO0B4_I2S0_SDO);
                    break;
                default:
                    break;
            }
            break;

        case I2S_CH1://i2s_ch1 only I2S mode
            rk_printf("select I2S DevID 1\n");
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin0, IOMUX_GPIO1A0_I2S1A_CLK);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin2, IOMUX_GPIO1A2_I2S1A_SCLK);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin4, IOMUX_GPIO1A4_I2S1A_SDI);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin1, IOMUX_GPIO1A1_I2S1A_LRCK);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin3, IOMUX_GPIO1A3_I2S1A_SDO);
            break;

        case I2S_CH1_PB://i2s_ch1 only I2S mode
            rk_printf("select I2S DevID 1_B\n");
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin0, IOMUX_GPIO0A0_I2S1B_CLK);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin2, IOMUX_GPIO0A2_I2S1B_SCLK);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin4, IOMUX_GPIO0A4_I2S1B_SDI);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin1, IOMUX_GPIO0A1_I2S1B_LRCK);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortA_Pin3, IOMUX_GPIO0A3_I2S1B_SDO);
            break;

        default:
            break;
     }
    //open I2S clk
    ScuClockGateCtr(CLK_I2S_GATE, 1);//CLOCK_GATE_I2S
    rk_printf("\n hhhhhh GateCtr befor  ScuSoftResetCtr\n");
    switch(DevID)
    {
        case I2S_CH0:
            //open rst I2S ip
            ScuSoftResetCtr(I2S0_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(I2S0_SRST, 0);

            IntRegister(INT_ID_I2S0 ,I2s0IntIsr);
            IntPendingClear(INT_ID_I2S0);
            IntEnable(INT_ID_I2S0);
            rk_printf("\n enable I2S_CH0\n");

            break;
        case I2S_CH1:
            //open rst I2S ip
            ScuSoftResetCtr(I2S1_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(I2S1_SRST, 0);

            IntRegister(INT_ID_I2S1 ,I2s1IntIsr);
            IntPendingClear(INT_ID_I2S1);
            IntEnable(INT_ID_I2S1);
            break;
        default:
            break;
    }

}
#endif

#ifdef __DRIVER_USB_USBOTGDEV_C__
/*******************************************************************************
** Name: UsbOtgDevHwInit
** Input:uitn32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.18
** Time: 11:29:13
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT API void UsbOtgDevHwInit(uint32 DevID, uint32 Channel)
{
    //CPU init
    ScuClockGateCtr(CLK_USBPHY_GATE, 1);
    ScuClockGateCtr(HCLK_USBC_GATE, 1);
    ScuSoftResetCtr(USBPHY_SRST, 0);
    ScuSoftResetCtr(USBGLB_SRST, 0);
    ScuSoftResetCtr(USBOTG_SRST, 0);
    DelayUs(5);
    Grf_otgphy_suspend(0);
    DelayUs(200);

    //interrupt init
    IntRegister(INT_ID_USBC, (void*)UsbOtgDevIntIsr0);

}
#endif

#ifdef __DRIVER_I2C_I2CDEVICE_C__
/*******************************************************************************
** Name: I2cDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:31:27
*******************************************************************************/
_DRIVER_I2C_I2CDEVICE_INIT_
INIT API void I2cDevHwInit(uint32 DevID, uint32 Channel)
{
    switch (DevID)
    {
        case I2C_CH0:
            Grf_I2C0_Sel(I2C_SEL_A);//I2C0_SEL Grf_I2C1_Sel(I2C_SEL_A);
            switch (Channel)
            {
                case GPIO_CH0:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin2,IOMUX_GPIO2B2_I2C0A_SCL);//I2C0  -- U6001 RTC_HYM8563
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin3,IOMUX_GPIO2B3_I2C0A_SDA);
                    //rk_printf("I2C0_IO RTC_HYM8563\n");
                    break;

                case GPIO_CH1:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin1,IOMUX_GPIO2C1_I2C0B_SCL);
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin0,IOMUX_GPIO2C0_I2C0B_SDA);
                    break;

                case GPIO_CH2:
                    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIO0A6_I2C0C_SCL);
                    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIO0A5_I2C0C_SDA);
                    break;

                default:
                    break;
            }
            break;
        case I2C_CH1:
            Grf_I2C0_Sel(I2C_SEL_B);//I2C1_SEL//FM5807
            switch (Channel)
            {
                case GPIO_CH0:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin1,IOMUX_GPIO2B1_I2C1A_SCL);
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin0,IOMUX_GPIO2B0_I2C1A_SDA);
                    break;

                case GPIO_CH1:
                    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin1,IOMUX_GPIO1B1_I2C1B_SCL);
                    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin2,IOMUX_GPIO1B2_I2C1B_SDA);
                    break;

                case GPIO_CH2:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin5,IOMUX_GPIO2B5_I2C1C_SCL);
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin4,IOMUX_GPIO2B4_I2C1C_SDA);
                    break;

                default:
                    break;
            }
            break;
        case I2C_CH2:
            Grf_I2C0_Sel(I2C_SEL_C);//I2C2_SEL
            switch (Channel)
            {
                case GPIO_CH0:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin1,IOMUX_GPIO2A1_I2C2A_SCL);
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin0,IOMUX_GPIO2A0_I2C2A_SDA);
                    break;

                case GPIO_CH1:
                    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin0,IOMUX_GPIO0D0_I2C2B_SCL);
                    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin1,IOMUX_GPIO0D1_I2C2B_SDA);
                    break;

                case GPIO_CH2:
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin5,IOMUX_GPIO2A5_I2C2C_SCL);
                    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin6,IOMUX_GPIO2A6_I2C2C_SDA);
                    break;

                default:
                    break;
            }
            break;
    }
    switch (DevID)
    {
        case I2C_CH0:
            //open I2C0 clk
            ScuClockGateCtr(PCLK_I2C0_GATE, 1);
            rk_printf("I2CDevInit I2C_CH0\n");
            ScuSoftResetCtr(I2C0_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(I2C0_SRST, 0);

            IntRegister(INT_ID_I2C0 ,I2CDevIntIsr0);//I2CDevIntIsr0
            IntPendingClear(INT_ID_I2C0);
            IntEnable(INT_ID_I2C0);
            break;

        case I2C_CH1:
            rk_printf("I2CDevInit I2C_CH1\n");
            //open I2C1 clk
            ScuClockGateCtr(PCLK_I2C1_GATE, 1);
            ScuSoftResetCtr(I2C1_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(I2C1_SRST, 0);

            IntRegister(INT_ID_I2C1 ,I2CDevIntIsr1);
            IntPendingClear(INT_ID_I2C1);
            IntEnable(INT_ID_I2C1);
            break;

        case I2C_CH2:
            //open I2C2 clk
            ScuClockGateCtr(PCLK_I2C2_GATE, 1);
            ScuSoftResetCtr(I2C2_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(I2C2_SRST, 0);

            IntRegister(INT_ID_I2C2 ,I2CDevIntIsr2);
            IntPendingClear(INT_ID_I2C2);
            IntEnable(INT_ID_I2C2);
            break;

        default:
            break;
    }
}
#endif

#ifdef __DRIVER_UART_UARTDEVICE_C__
/*******************************************************************************
** Name: UartDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:30:01
*******************************************************************************/
_DRIVER_UART_UARTDEVICE_INIT_
INIT API void UartDevHwInit(uint32 DevID, uint32 Channel)
{
    //gpio iomux
    switch (DevID)
    {
        case UART_DEV0:
            {

                switch (Channel)
                {
                    case UART_CHA:
                        Grf_UART0_Sel(UART_SEL_A);
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin5,IOMUX_GPIO2B5_UART0A_TX);
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin4,IOMUX_GPIO2B4_UART0A_RX);

                        break;
                }
                break;
            }


        case UART_DEV1:
            {
                switch (Channel)
                {
                    case UART_CHA:
                        Grf_UART1_Sel(UART_SEL_A);
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin0,IOMUX_GPIO2C0_UART1A_TX);
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin1,IOMUX_GPIO2C1_UART1A_RX);
                        // rts/cts
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin6,IOMUX_GPIO2B6_UART1A_RTS);
                        Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin7,IOMUX_GPIO2B7_UART1A_CTS);
                        break;
                }
                break;
            }

        case UART_DEV2:
            break;

        case UART_DEV3:
            break;

        case UART_DEV4:
            break;

        case UART_DEV5:
            break;
    }

    //clock gate & rst
    switch (DevID)
    {
        case UART_DEV0:
            ScuSoftResetCtr(UART0_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART0_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART0_GATE, 1);
            ScuClockGateCtr(PCLK_UART0_GATE, 1);
            SetUartFreq(UART_DEV0,XIN24M,24*1000*1000);
            break;

        case UART_DEV1:
            ScuSoftResetCtr(UART1_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART1_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART1_GATE, 1);
            ScuClockGateCtr(PCLK_UART1_GATE, 1);
            SetUartFreq(UART_DEV1,XIN24M,24*1000*1000);
            break;

        case UART_DEV2:
            ScuSoftResetCtr(UART2_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART2_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART2_GATE, 1);
            ScuClockGateCtr(PCLK_UART2_GATE, 1);
            SetUartFreq(UART_DEV2,XIN24M,24*1000*1000);
            break;

        case UART_DEV3:
            ScuSoftResetCtr(UART3_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART3_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART3_GATE, 1);
            ScuClockGateCtr(PCLK_UART3_GATE, 1);
            SetUartFreq(UART_DEV3,XIN24M,24*1000*1000);
            break;

        case UART_DEV4:
            ScuSoftResetCtr(UART4_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART4_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART4_GATE, 1);
            ScuClockGateCtr(PCLK_UART4_GATE, 1);
            SetUartFreq(UART_DEV4,XIN24M,24*1000*1000);
            break;

        case UART_DEV5:
            ScuSoftResetCtr(UART5_SRST, 1);
            DelayMs(1);
            ScuSoftResetCtr(UART5_SRST, 0);

            //open uart clk
            ScuClockGateCtr(CLK_UART5_GATE, 1);
            ScuClockGateCtr(PCLK_UART5_GATE, 1);
            SetUartFreq(UART_DEV5,XIN24M,24*1000*1000);
            break;
    }


    switch (DevID)
    {
        case UART_DEV0:
            IntRegister(INT_ID_UART0 ,UartIntIsr0);
            IntPendingClear(INT_ID_UART0);
            IntEnable(INT_ID_UART0);
            break;

        case UART_DEV1:
            IntRegister(INT_ID_UART1 ,UartIntIsr1);
            IntPendingClear(INT_ID_UART1);
            IntEnable(INT_ID_UART1);
            break;

        case UART_DEV2:
            IntRegister(INT_ID_UART2 ,UartIntIsr2);
            IntPendingClear(INT_ID_UART2);
            IntEnable(INT_ID_UART2);
            break;

        case UART_DEV3:
            IntRegister(INT_ID_UART3 ,UartIntIsr3);
            IntPendingClear(INT_ID_UART3);
            IntEnable(INT_ID_UART3);
            break;

        case UART_DEV4:
            IntRegister(INT_ID_UART4 ,UartIntIsr4);
            IntPendingClear(INT_ID_UART4);
            IntEnable(INT_ID_UART4);
            break;

        case UART_DEV5:
            IntRegister(INT_ID_UART5 ,UartIntIsr5);
            IntPendingClear(INT_ID_UART5);
            IntEnable(INT_ID_UART5);
            break;

        default:
            break;
    }


}
#endif

#ifdef __DRIVER_SDIO_SDIODEVICE_C__
/*******************************************************************************
** Name: SdioDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:28:17
*******************************************************************************/
_DRIVER_SDIO_SDIODEVICE_INIT_
INIT API void SdioDevHwInit(uint32 DevID, uint32 Channel)
{
    if (DevID == 0)
    {
        //GpioMuxSet(GPIO_CH0,SDMMC_POWER_EN,IOMUX_GPIOA0_SDMMC_PWREN);

        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1,IOMUX_GPIOC1_IO);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2,IOMUX_GPIOC2_IO);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3,IOMUX_GPIOC3_IO);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4,IOMUX_GPIOC4_IO);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5,IOMUX_GPIOC5_IO);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6,IOMUX_GPIOC6_IO);

        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin1,GPIO_OUT);
        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin2,GPIO_OUT);
        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin3,GPIO_OUT);
        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin4,GPIO_OUT);
        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin5,GPIO_OUT);
        //Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin6,GPIO_OUT);

        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin1,GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin2,GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin3,GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin4,GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin5,GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin6,GPIO_LOW);

        //WIFI Host wakeup: hold low level after powerup
        //GpioMuxSet(GPIO_CH0,WIFI_HOST_WAKE,IOMUX_GPIOC7_IO);
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_HOST_WAKE,GPIO_OUT);
        //Gpio_SetPinLevel(GPIO_CH0,WIFI_HOST_WAKE,GPIO_LOW);

        //WIFI RegOn: hold low level after powerup
        //GpioMuxSet(GPIO_CH0,WIFI_REG_ON,IOMUX_GPIOD2_IO);
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_REG_ON,GPIO_OUT);
        //Gpio_SetPinLevel(GPIO_CH0,WIFI_REG_ON,GPIO_LOW);

        //WIFI Power Down
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_POWER, GPIO_OUT);
        //Gpio_SetPinLevel(GPIO_CH0,WIFI_POWER, GPIO_LOW);

        DelayMs(250);


        //=========================================================
        //WIFI Power Initial: PowerDown, SDMMC Low Level
        //WIFI Power Down
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_POWER, GPIO_OUT);
        //Gpio_SetPinLevel(GPIO_CH0,WIFI_POWER, GPIO_HIGH);
        DelayMs(250);

        //WIFI RegOn: hold low level after powerup
        //GpioMuxSet(GPIO_CH0,WIFI_REG_ON,IOMUX_GPIOD2_IO);
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_REG_ON,GPIO_OUT);
        //Gpio_SetPinLevel(GPIO_CH0,WIFI_REG_ON,GPIO_HIGH);
        DelayMs(100);

        //WIFI Host wakeup: hold low level after powerup
        //GpioMuxSet(GPIO_CH0,WIFI_HOST_WAKE,IOMUX_GPIOC7_IO);
        //Gpio_SetPinDirection(GPIO_CH0,WIFI_HOST_WAKE,GPIO_IN);


        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1, IOMUX_GPIOC1_SDMMC_CMDP0);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2, IOMUX_GPIOC2_SDMMC_CLKP0);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3, IOMUX_GPIOC3_SDMMC_DATA0P0);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4, IOMUX_GPIOC4_SDMMC_DATA1P0);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5, IOMUX_GPIOC5_SDMMC_DATA2P0);
        //GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6, IOMUX_GPIOC6_SDMMC_DATA3P0);

    }
}
#endif


#ifdef __DRIVER_SDMMC_SDMMCDEVICE_C__
/*******************************************************************************
** Name: SdMmcDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:26:37
*******************************************************************************/
_DRIVER_SDMMC_SDMMCDEVICE_INIT_
INIT API void SdMmcDevHwInit(uint32 DevID, uint32 Channel)
{
    //io mux

    if (DevID == SDC0)
    {

        if (Channel == 0)
        {
            rk_printf("Sdmmc0 for TF card\n");
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIO0A6_EMMC_D3);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIO0A5_EMMC_D2);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin4,IOMUX_GPIO0A4_EMMC_D1);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3,IOMUX_GPIO0A3_EMMC_D0);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin2,IOMUX_GPIO0A2_EMMC_CMD);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin1,IOMUX_GPIO0A1_EMMC_CLK);
        }
        else if (Channel == 1)
        {
            rk_printf("Init emmc iomux\n");
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin7,IOMUX_GPIO0A7_EMMC_D4);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIO0A6_EMMC_D3);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIO0A5_EMMC_D2);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin4,IOMUX_GPIO0A4_EMMC_D1);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3,IOMUX_GPIO0A3_EMMC_D0);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin2,IOMUX_GPIO0A2_EMMC_CMD);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin1,IOMUX_GPIO0A1_EMMC_CLK);

            Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin2,IOMUX_GPIO0B2_EMMC_D7);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin1,IOMUX_GPIO0B1_EMMC_D6);
            Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin0,IOMUX_GPIO0B0_EMMC_D5);
        }
    }
    else if (DevID == SDC1)
    {
        Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin5,IOMUX_GPIO1A5_SDMMC_CMD);
        Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin6,IOMUX_GPIO1A6_SDMMC_CLK);
        Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin7,IOMUX_GPIO1A7_SDMMC_D0);

        Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin0,IOMUX_GPIO1B0_SDMMC_D1);
        Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin1,IOMUX_GPIO1B1_SDMMC_D2);
        Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin2,IOMUX_GPIO1B2_SDMMC_D3);
    }

    //clock gate
    if (DevID == SDC0)
    {
        SetSdmmcFreq(XIN24M,24*1000*1000);

        ScuClockGateCtr(HCLK_EMMC_GATE, 1);
        ScuClockGateCtr(CLK_EMMC_GATE, 1);

        ScuSoftResetCtr(SDMMC0_SRST, 1);
        DelayMs(1);
        ScuSoftResetCtr(SDMMC0_SRST, 0);

    }
    else if (DevID == SDC1)
    {
        SetSdmmcFreq(XIN24M,24*1000*1000);

        ScuClockGateCtr(HCLK_SDMMC_GATE, 1);
        ScuClockGateCtr(CLK_SDMMC_GATE, 1);

        //open rst sdmmc ip
        ScuSoftResetCtr(SDMMC1_SRST, 1);
        DelayMs(1);
        ScuSoftResetCtr(SDMMC1_SRST, 0);

    }

    // inter isr
    if (DevID == SDC0)
    {
        IntUnregister(INT_ID_EMMC);
        IntPendingClear(INT_ID_EMMC);
        IntDisable(INT_ID_EMMC);
        IntRegister(INT_ID_EMMC ,SdcIntIRQ0);
        IntPendingClear(INT_ID_EMMC);
        IntEnable(INT_ID_EMMC);
    }
    else if (DevID == SDC1)
    {
        IntUnregister(INT_ID_SDMMC);
        IntPendingClear(INT_ID_SDMMC);
        IntDisable(INT_ID_SDMMC);
        IntRegister(INT_ID_SDMMC ,SdcIntIRQ1);
        IntPendingClear(INT_ID_SDMMC);
        IntEnable(INT_ID_SDMMC);
    }

}
#endif

#ifdef __DRIVER_SPI_SPIDEVICE_C__
/*******************************************************************************
** Name: SpiDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:23:08
*******************************************************************************/
_DRIVER_SPI_SPIDEVICE_INIT_
INIT API void SpiDevHwInit(uint32 DevID, uint32 Channel)
{
    if(DevID == 0)
    {
        if(Channel == 0)
        {
            Grf_SPI0_Sel(SPI_SEL_A);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortC_Pin0, IOMUX_GPIO0C0_SPI0A_TX);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortC_Pin2, IOMUX_GPIO0C2_SPI0A_CLK);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortC_Pin3, IOMUX_GPIO0C3_SPI0A_CS);
            Grf_GpioMuxSet(GPIO_CH0, GPIOPortC_Pin1, IOMUX_GPIO0C1_SPI0A_RX);

        }
        else if(Channel == 1)
        {
            Grf_SPI0_Sel(SPI_SEL_B);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin2, IOMUX_GPIO2B2_SPI0B_TX);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin1, IOMUX_GPIO2B1_SPI0B_CLK);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin0, IOMUX_GPIO2B0_SPI0B_CS);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin3, IOMUX_GPIO2B3_SPI0B_RX);
        }
    }
    else if(DevID == 1)
    {
        if(Channel == 0)
        {
            Grf_SPI1_Sel(SPI_SEL_A);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortB_Pin0, IOMUX_GPIO1B0_SPI1A_TX);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin6, IOMUX_GPIO1A6_SPI1A_CLK);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin5, IOMUX_GPIO1A5_SPI1A_CS);
            Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin7, IOMUX_GPIO1A7_SPI1A_RX);

        }
        else if(Channel == 1)
        {
            Grf_SPI1_Sel(SPI_SEL_B);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortC_Pin1, IOMUX_GPIO2C1_SPI1B_TX);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortC_Pin0, IOMUX_GPIO2C0_SPI1B_CLK);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin6, IOMUX_GPIO2B6_SPI1B_CS);
            Grf_GpioMuxSet(GPIO_CH2, GPIOPortB_Pin7, IOMUX_GPIO2B7_SPI1B_RX);
        }
    }
}
#endif

#ifdef __DRIVER_AD_ADCDEVICE_C__
/*******************************************************************************
** Name: AdcDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:wrm
** Date: 2015.5.18
** Time: 15:43:37
*******************************************************************************/
void AdcDevHwInit (uint32 DevID, uint32 Channel)
{
    DelayMs(10);
    ScuClockGateCtr(CLK_SARADC_GATE, 1);
    ScuClockGateCtr(PCLK_SARADC_GATE, 1);

    ScuSoftResetCtr(SARADC_SRST, 0);

    SetSarAdcFreq(1000000);        // 1M

    Grf_Set_ADC_Vref_PowerOn(GRF_VREF_PWON);
    Grf_Verf_Trim_Sel(GRF_VREF_TRIM_2600);
    Grf_Verf_Vbg_Sel(GRF_VREF_TRIM_2425);

    IntUnregister(INT_ID_SRADC);
    IntPendingClear(INT_ID_SRADC);
    IntDisable(INT_ID_SRADC);

    IntRegister(INT_ID_SRADC, ADCDevISR);
    IntPendingClear(INT_ID_SRADC);
    IntEnable(INT_ID_SRADC);
}
#endif

#ifdef __DRIVER_PWM_PWMDEVICE_C__
/*******************************************************************************
** Name: PwmDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:wrm
** Date: 2015.5.19
** Time: 9:17:37
*******************************************************************************/
void PwmDevHwInit (uint32 DevID, uint32 channel)
{
    switch ( channel )
    {
        case PWM_CHN0:
            rk_printf ("Init Pwm Hardware\n");
            Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin4,IOMUX_GPIO2A4_PWM0_OUT);
            Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin4, GPIO_OUT);
            break;

        case PWM_CHN1:
            Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_PWM1_OUT);
            Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin3, GPIO_OUT);
            break;

        case PWM_CHN2:
            Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin2,IOMUX_GPIO2A2_PWM2_OUT);
            Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin2, GPIO_OUT);
            break;

        case PWM_CHN3:
            Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin1,IOMUX_GPIO2A1_PWM3_OUT);
            Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin1, GPIO_OUT);
            break;

        case PWM_CHN4:
            Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin0,IOMUX_GPIO2A0_PWM4_OUT);
            Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin0, GPIO_OUT);
            break;

        default:
            break;
    }

    if ( channel < PWM_CHN4)
    {
        ScuSoftResetCtr(PWM0_SRST, 1);
        DelayMs(1);
        ScuSoftResetCtr(PWM0_SRST, 0);

        ScuClockGateCtr(PCLK_PWM0_GATE, 1);
    }
    else
    {
        ScuSoftResetCtr(PWM1_SRST, 1);
        DelayMs(1);
        ScuSoftResetCtr(PWM1_SRST, 0);

        ScuClockGateCtr(PCLK_PWM1_GATE, 1);
    }
}
#endif


#ifdef __DRIVER_WATCHDOG_WATCHDOGDEVICE_C__
/*******************************************************************************
** Name: WdtDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:wrm
** Date: 2015.5.19
** Time: 10:34:37
*******************************************************************************/
void WdtDevHwInit (uint32 DevID, uint32 channel)
{

    rk_printf ("Init WDT Hardware\n");

    ScuClockGateCtr(PCLK_WDT_GATE, 1);

    ScuSoftResetCtr(WDT_SRST, 1);
    DelayMs(1);
    ScuSoftResetCtr(WDT_SRST, 0);

    IntUnregister(INT_ID_WDT);
    IntPendingClear(INT_ID_WDT);
    IntDisable(INT_ID_WDT);

    IntRegister(INT_ID_WDT, WDTDevIntIsr0);
    IntPendingClear(INT_ID_WDT);
    IntEnable(INT_ID_WDT);
}
#endif

#ifdef __DRIVER_MAILBOX_MAILBOXDEVICE_C__
/*******************************************************************************
** Name: MailBoxDevHwInit
** Input:uint32 DevID, uint32 Channel
** Return: void
** Owner:cjh
** Date: 2015.5.20
** Time: 19:28:55
*******************************************************************************/
_DRIVER_MAILBOX_MAILBOXDEVICE_INIT_
INIT API void MailBoxDevHwInit(uint32 DevID, uint32 IntTypeA2BB2A)
{
    switch (IntTypeA2BB2A)
    {
        case MAILBOX_A2B_TYPE:
            switch (DevID)
            {
                case MAILBOX_CHANNEL_0:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0, MailBoxDevB2AIsr0);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0);
                    MailBoxEnableA2BInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_0));
                    break;

                case MAILBOX_CHANNEL_1:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1, MailBoxDevB2AIsr1);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1);
                    MailBoxEnableA2BInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_1));
                    break;

                case MAILBOX_CHANNEL_2:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2, MailBoxDevB2AIsr2);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2);
                    MailBoxEnableA2BInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_2));
                    break;

                case MAILBOX_CHANNEL_3:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3, MailBoxDevB2AIsr3);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3);
                    MailBoxEnableA2BInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_3));
                    break;

                default:
                    break;
            }
            ScuSoftResetCtr(CAL_CORE_SRST, TRUE);
            DelayMs(1);
            ScuSoftResetCtr(CAL_CORE_SRST, FALSE);
                        break;
        case MAILBOX_B2A_TYPE:
            switch (DevID)
            {
                case MAILBOX_CHANNEL_0:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0, MailBoxDevA2BIsr0);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_0);
                    MailBoxEnableB2AInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_0));
                    break;

                case MAILBOX_CHANNEL_1:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1, MailBoxDevA2BIsr1);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_1);
                    MailBoxEnableB2AInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_1));
                    break;

                case MAILBOX_CHANNEL_2:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2, MailBoxDevA2BIsr2);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_2);
                    MailBoxEnableB2AInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_2));
                    break;

                case MAILBOX_CHANNEL_3:
                    IntRegister(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3, MailBoxDevA2BIsr3);
                    IntPendingClear(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3);
                    IntEnable(INT_ID_MAILBOX0 + MAILBOX_CHANNEL_3);
                    MailBoxEnableB2AInt(MAILBOX_ID_0, (int32)(1 << MAILBOX_CHANNEL_3));
                    break;

                default:
                    break;
            }
            ScuSoftResetCtr(CAL_CORE_SRST, TRUE);
            DelayMs(1);
            ScuSoftResetCtr(CAL_CORE_SRST, FALSE);
            break;
        default:
            break;
    }

}
#endif

#ifdef __DRIVER_SPIFLASH_SPIFLASHDEV_C__
INIT API void SfcDevHwInit(void)
{
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIO0A5_SFC_D3);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIO0A6_SFC_D2);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin7,IOMUX_GPIO0A7_SFC_D1);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin0,IOMUX_GPIO0B0_SFC_D0);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin1,IOMUX_GPIO0B1_SFC_CLK);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin2,IOMUX_GPIO0B2_SFC_CS);

    ScuClockGateCtr(CLK_SFC_GATE,1);
    ScuClockGateCtr(HCLK_SFC_GATE,1);
    ScuSoftResetCtr(SFC_SRST, 0);
}
#endif

/*******************************************************************************
** Name: BSP_Init
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 17:15:36
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT API void bsp_init(void)
{
    SysVariableInit();
    CPUInit();
    GpioInit();

    #ifdef BB_SYS_JTAG
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin4,IOMUX_GPIO2B4_JTG1_TMS);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin5,IOMUX_GPIO2B5_JTG1_TCK);
    #endif

    SysTickInit();
    SysInterruptInit();
}




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: GpioInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:56:53
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT FUN void GpioInit(void)
{
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin0,IOMUX_GPIO0A0_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin1,IOMUX_GPIO0A1_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin2,IOMUX_GPIO0A2_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3,IOMUX_GPIO0A3_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin4,IOMUX_GPIO0A4_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIO0A5_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIO0A6_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortA_Pin7,IOMUX_GPIO0A7_IO);

    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin0,IOMUX_GPIO0B0_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin1,IOMUX_GPIO0B1_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin2,IOMUX_GPIO0B2_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin3,IOMUX_GPIO0B3_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin4,IOMUX_GPIO0B4_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin5,IOMUX_GPIO0B5_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin6,IOMUX_GPIO0B6_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortB_Pin7,IOMUX_GPIO0B7_IO);


    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin0,IOMUX_GPIO0C0_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1,IOMUX_GPIO0C1_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2,IOMUX_GPIO0C2_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3,IOMUX_GPIO0C3_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4,IOMUX_GPIO0C4_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5,IOMUX_GPIO0C5_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6,IOMUX_GPIO0C6_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortC_Pin7,IOMUX_GPIO0C7_IO);

    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin0,IOMUX_GPIO0D0_IO);
    Grf_GpioMuxSet(GPIO_CH0,GPIOPortD_Pin1,IOMUX_GPIO0D1_IO);

    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin0,IOMUX_GPIO1A0_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin1,IOMUX_GPIO1A1_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin2,IOMUX_GPIO1A2_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin3,IOMUX_GPIO1A3_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin4,IOMUX_GPIO1A4_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin5,IOMUX_GPIO1A5_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin6,IOMUX_GPIO1A6_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortA_Pin7,IOMUX_GPIO1A7_IO);

    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin0,IOMUX_GPIO1B0_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin1,IOMUX_GPIO1B1_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin2,IOMUX_GPIO1B2_IO);
    Grf_GpioMuxSet(GPIO_CH1,GPIOPortB_Pin3,IOMUX_GPIO1B3_IO);


    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin0,IOMUX_GPIO2A0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin1,IOMUX_GPIO2A1_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin2,IOMUX_GPIO2A2_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin4,IOMUX_GPIO2A4_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin5,IOMUX_GPIO2A5_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin6,IOMUX_GPIO2A6_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin7,IOMUX_GPIO2A7_IO);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin0,IOMUX_GPIO2B0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin1,IOMUX_GPIO2B1_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin2,IOMUX_GPIO2B2_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin3,IOMUX_GPIO2B3_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin4,IOMUX_GPIO2B4_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin5,IOMUX_GPIO2B5_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin6,IOMUX_GPIO2B6_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin7,IOMUX_GPIO2B7_IO);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin0,IOMUX_GPIO2C0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin1,IOMUX_GPIO2C1_IO);

    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortA_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortB_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH0,GPIOPortD_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortD_Pin1,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortA_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH1,GPIOPortB_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortB_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH1,GPIOPortB_Pin2,GPIO_IN);


    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin1,GPIO_IN);


}
/*******************************************************************************
** Name: CPUInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:55:46
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT FUN void CPUInit(void)
{
    //Scu_OOL_Power_Set(1);    //power on

    //ScuLDOEnable();
    //ScuDCoutEnable();

    //ScuDCoutSet(SCU_DCOUT_120);
    //ScuLDOSet(SCU_LDO_30);

    //Scu_Force_PWM_Mode_Set(0);
    //Scu_Threshold_Set(PFM_26MA);
    //Scu_Battery_Sns_Set(1);

    FREQ_EnterModule(FREQ_BLON);

    //ScuIpCoreInit();
    //ScuClockGateInit();
    //Scu_Memory_Set_High_Speed(LDRAM_RESP_CYCLE);

}
/*******************************************************************************
** Name: SysTickInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:55:04
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT FUN void SysTickInit(void)
{
    SysTickDisable();

    SysTickClkSourceSet(NVIC_SYSTICKCTRL_CLKIN);
    SysTickPeriodSet(10);

    SysTickEnable();
}
/*******************************************************************************
** Name: SysVariableInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 16:54:12
*******************************************************************************/
_BSP_EVK_V20_BSP_INIT_
INIT FUN void SysVariableInit(void)
{
    SysTickCounter   = 0;

    chip_freq.armclk = 0;

    UartHDC = NULL;
    pIdleFun = CpuIdle;
    pIdleFunSave = CpuIdle;

    SysState = LEVEL_INIT;
    SysWakeCnt = 0;
}
/*******************************************************************************
** Name: SysInterruptInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.5.14
** Time: 15:29:42
*******************************************************************************/
extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );
extern void vPortSVCHandler( void );

_BSP_EVK_V20_BSP_INIT_
INIT FUN void SysInterruptInit(void)
{
    IntMasterEnable();

    //set priority.
    IrqPriorityInit();
    //......

    //register interrupt functions.
    IntRegister(FAULT_ID11_SVCALL, (void*)vPortSVCHandler);
    IntRegister(FAULT_ID15_SYSTICK, (void*)xPortSysTickHandler);
    IntRegister(FAULT_ID14_PENDSV, (void*)xPortPendSVHandler);;

    //interrupt enable.
    //IntEnable(FAULT_ID15_SYSTICK);
    //IntEnable(FAULT_ID14_PENDSV);
    //IntEnable(FAULT_ID11_SVCALL);

    IntDisable(FAULT_ID15_SYSTICK);

}
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
