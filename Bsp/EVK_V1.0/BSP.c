/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                                    (c) Copyright 2013, RockChip.Ltd
*                                          All Rights Reserved
*File    : BSP.C
* By     : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/
#include "BspCOnfig.h"
#include "FreeRTOS.h"
#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"
#include "sysresume.h"


/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/


#define _BSP_EVK_V10_BSP_READ_  __attribute__((section("bsp_evk_v10_bsp_read")))
#define _BSP_EVK_V10_BSP_WRITE_ __attribute__((section("bsp_evk_v10_bsp_write")))
#define _BSP_EVK_V10_BSP_INIT_  __attribute__((section("bsp_evk_v10_bsp_init")))
#define _BSP_EVK_V10_BSP_SHELL_  __attribute__((section("bsp_evk_v10_bsp_shell")))


/*
*********************************************************************************************************
*                                        Variable Define
*********************************************************************************************************
*/
uint32 SysTickCounter;                            //在SYSTICK中进行增加
void * UartHDC;
void (*pIdleFun)(void);
void (*pIdleFunSave)(void);


extern void xPortPendSVHandler( void );
extern void xPortSysTickHandler( void );
extern void vPortSVCHandler( void );
extern void SysTickDisable(void);
extern void SysTickClkSourceSet(UINT32 Source);
extern void SysTickPeriodSet(UINT32 mstick );
extern void SysTickEnable(void);
extern int32 ScuIpCoreInit(void);
extern int32 PMU_EnterModule(ePMU_APP modulename);
extern void DelayMs(UINT32 ms);
/*
*********************************************************************************************************
*                                     int fputc(int ch, FILE *f)
* Description:  This function is for the rk_printf from uart debug.
*
* Argument(s) : int ch, FILE *f
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/
#define DEBUG_TIME_LEN  8

#include <stdarg.h>

void fputc_hook(char ch)
{
     while(UARTWriteByte(UART_CH0,(uint8*)&ch, 1) == 0);
}

int fputc(int ch, FILE *f)
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


void rk_printf(const char * fmt,...)
{
    uint8 buf[128];
    uint8 size;
    uint32 tmpcnt,i,j;
    uint8 dgbBuffer[DEBUG_TIME_LEN];

    if (UartHDC == NULL)
    {
        return;
    }

    tmpcnt = SysTickCounter;
    for (i = 0; i < DEBUG_TIME_LEN; i++)
    {
        dgbBuffer[i] = tmpcnt % 10;
        tmpcnt = tmpcnt / 10;
    }

    buf[0] = '\r';
    buf[1] = '\n';
    buf[2] = '[';

    for (i = 0 ,j = 0; i < DEBUG_TIME_LEN; i++)
    {
        buf[j + 3] = dgbBuffer[DEBUG_TIME_LEN - 1 - i] + 0x30;
        if (DEBUG_TIME_LEN - 1 - i == 2)
        {
            j++;
            buf[j + 3] = '.';
        }
        j++;
    }

    buf[j + 3] = ']';


    va_list arg_ptr;
    va_start(arg_ptr, fmt);

#ifndef _SIMULATOR_
    size =  vsprintf((char*)(buf + j + 4),(const char *)fmt, arg_ptr);

    UartDev_Write(UartHDC,buf, size + 13,  SYNC_MODE, NULL);
#else
    //printf((const char *)fmt, arg_ptr);
#endif

}

void rk_printf_no_time(const char * fmt,...)
{
    uint8 buf[128];
    uint8 size;

    va_list arg_ptr;
    va_start(arg_ptr, fmt);

#ifndef _SIMULATOR_
    size =  vsprintf((char*)buf,(const char *)fmt, arg_ptr);
    buf[size] = '\r';
    buf[size + 1] = '\n';

    UartDev_Write(UartHDC, buf, size + 2,  SYNC_MODE, NULL);
#else
    //printf((const char *)fmt, arg_ptr);
#endif
}


void rk_print_string(const uint8 * pstr)
{
#ifndef _SIMULATOR_
    UartDev_Write(UartHDC, pstr, strlen(pstr), SYNC_MODE, NULL);
#else
    //printf((const char *)pstr);
#endif
}

uint32  SysTickCntSave;
uint32  SysTickSave;

void rk_count_clk_start()
{
    SysTickCntSave = SysTickCounter;
    SysTickSave = nvic->SysTick.Value;
}

void rk_count_clk_end()
{
    uint32 SysTick, SysTickCnt;
    uint32 clk;

    SysTickCnt = SysTickCounter;
    SysTick = nvic->SysTick.Value;

    if(SysTickCntSave == SysTickCnt)
    {
        clk = SysTickSave - SysTick;
    }
    else
    {
        clk = SysTickSave + (SysTickCnt - SysTickCntSave - 1) * nvic->SysTick.Reload
            + (nvic->SysTick.Reload - SysTick);
    }

    rk_printf("total clk = %u", clk);

}


uint32 StrLenW(uint16 * pstr)
{
    uint32 i = 0;
    while (*pstr++ != 0)
        i++;

    return i;
}

uint32 StrLenA(uint8 * pstr)
{
    uint32 i = 0;
    while (*pstr++ != 0)
        i++;

    return i;
}

int32 StrCmpW(uint16 * pstr1, uint16 * pstr2, uint32 len)
{
    uint32 len1, len2;

    if(len == 0)
    {
        len1 = StrLenW(pstr1);
        len2 = StrLenW(pstr2);

        if(len1 > len2)
        {
            return 1;
        }
        else if(len1 < len2)
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

void MemCpy(uint8 * trg, uint8 * src, uint32 len)
{
    uint32 i;

    for(i = 0; i < len; i++)
    {
        *trg++ = *src++;
    }
}

int32 StrCmpA(uint8 * pstr1, uint8 * pstr2, uint32 len)
{
    uint32 len1, len2;

    if(len == 0)
    {
        len1 = StrLenA(pstr1);
        len2 = StrLenA(pstr2);

        if(len1 > len2)
        {
            return 1;
        }
        else if(len1 < len2)
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

uint32 StringtoNum(uint8 * pstr)
{
    uint32 num = 0;
    int32 i;

    /*ptr save the fisrt char of pstr*/
    const uint8* ptr = pstr;

    /*if pstr start with '-' or '+' sign,move to the next charactor.*/
    if(*pstr == '-' || *pstr == '+')
        pstr++;

     while(*pstr != 0)
     {
        /*if pstr is none digal number ,break and return*/
        if(*pstr < '0' || *pstr > '9')
            break;
        /*current is digal ,calculate the value*/
        num = num * 10 + (*pstr - '0');
        pstr++;
     }

     if( *ptr == '-')
        num = -num;

    return num;
}

rk_size_t Ascii2Unicode(uint8 * pbAscii, uint16 * pwUnicode, uint32 len)
{
    uint32 i;

    for (i = 0; i < len / 2; i++)
    {
        pwUnicode[i] = (uint16)pbAscii[i];
    }
    return i * 2;
}

rk_size_t Unicode2Ascii(uint8 * pbAscii, uint16 * pwUnicode, uint32 len)
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


rk_err_t ShellCheckCmd(uint8 * CmdStr[],  uint8 * Cmd,  uint32 len)
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

uint32 ShellItemExtract(uint8 * pstr, uint8 ** pItem, uint8 * Space)
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


/*
*********************************************************************************************************
*                               void SysVariableInit(void)
* Description:  This function is to init the system value.
*
* Argument(s) : none
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_BSP_EVK_V10_BSP_INIT_
void SysVariableInit(void)
{
    SysTickCounter   = 0;

#ifndef _SIMULATOR_
    chip_freq.armclk = 0;
#else
    chip_freq.armclk = SYS_MAX_FREQ;
#endif

    UartHDC = NULL;
    pIdleFun = CpuIdle;
    pIdleFunSave = CpuIdle;

    SysState = LEVEL_INIT;
    SysWakeCnt = 0;
}

/*
*********************************************************************************************************
*                                     void SysInterruptInit(void)
* Description:  This function is to init the interrut .
*
* Argument(s) : none
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_BSP_EVK_V10_BSP_INIT_
void SysInterruptInit(void)
{
#ifndef _SIMULATOR_
    IntMasterEnable();
#endif

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
*********************************************************************************************************
*                                     void SysTickInit(void)
* Description:  This function is to init the systick.
*
* Argument(s) : none
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_BSP_EVK_V10_BSP_INIT_
void SysTickInit(void)
{
    SysTickDisable();

    SysTickClkSourceSet(NVIC_SYSTICKCTRL_CLKIN);
    SysTickPeriodSet(10);

    SysTickEnable();
}
/*
*********************************************************************************************************
*                                     void CPUInit(void))
* Description:  This function is to init the CPU.
*
* Argument(s) : none
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_BSP_EVK_V10_BSP_INIT_
void CPUInit(void)
{
    Scu_OOL_Power_Set(1);    //power on

    ScuLDOEnable();
    ScuDCoutEnable();

    ScuDCoutSet(SCU_DCOUT_120);
    ScuLDOSet(SCU_LDO_30);

    Scu_Force_PWM_Mode_Set(0);
    Scu_Threshold_Set(PFM_26MA);
    Scu_Battery_Sns_Set(1);

    SysFreqSet(SYS_DEFAULT_FREQ);

    ScuIpCoreInit();
    ScuClockGateInit();

}

_BSP_EVK_V10_BSP_INIT_
void GpioInit(void)
{
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin0,IOMUX_GPIOA0_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin1,IOMUX_GPIOA1_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin2,IOMUX_GPIOA2_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3,IOMUX_GPIOA3_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin4,IOMUX_GPIOA4_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIOA5_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIOA6_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortA_Pin7,IOMUX_GPIOA7_IO);

    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin0,IOMUX_GPIOB0_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin1,IOMUX_GPIOB1_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin2,IOMUX_GPIOB2_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin3,IOMUX_GPIOB3_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin4,IOMUX_GPIOB4_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin5,IOMUX_GPIOB5_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin6,IOMUX_GPIOB6_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin7,IOMUX_GPIOB7_IO);

    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin0,IOMUX_GPIOC0_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1,IOMUX_GPIOC1_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2,IOMUX_GPIOC2_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3,IOMUX_GPIOC3_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4,IOMUX_GPIOC4_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5,IOMUX_GPIOC5_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6,IOMUX_GPIOC6_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortC_Pin7,IOMUX_GPIOC7_IO);

    GpioMuxSet(GPIO_CH0,GPIOPortD_Pin0,IOMUX_GPIOD0_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortD_Pin1,IOMUX_GPIOD1_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortD_Pin2,IOMUX_GPIOD2_IO);
    GpioMuxSet(GPIO_CH0,GPIOPortD_Pin3,IOMUX_GPIOD3_IO);

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
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortD_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH0,GPIOPortD_Pin3,GPIO_IN);

}

_BSP_EVK_V10_BSP_INIT_
void BSP_Init(void)
{
    SysVariableInit();

#ifndef _SIMULATOR_
    CPUInit();
    GpioInit();
#endif
    SysTickInit();
    SysInterruptInit();
}



/*
*********************************************************************************************************
*                                     void vApplicationIdleHook (void)
*
* Description:  This function idle hook.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void vApplicationIdleHook (void)
{
    while (1)
    {
        if (pIdleFun != NULL)
        {
            pIdleFun();
        }
    }
}

void CpuIdle(void)
{
    if(SysState == LEVEL_INIT)
    {
        SysWakeCnt = SysTickCounter;
        PrevSysWakeCnt = SysWakeCnt;
        PrevSysTickCounter = SysTickCounter;
        SysState = LEVEL_0;
    }
    else if(SysState != LEVEL_0)
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

        if((SysWakeCnt - PrevSysWakeCnt) == (SysTickCounter - PrevSysTickCounter))
        {
            if((SysWakeCnt - PrevSysWakeCnt) >= 300) //wait 3s
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


/*
*********************************************************************************************************
*                                     void GpioInit(void)
* Description:  This function is to init the Gpio.
*
* Argument(s) : none
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_BSP_EVK_V10_BSP_INIT_
void SpiDevGpioInit(uint32 DevID, uint32 Channel)
{
    if(Channel == 1)
    {
        GpioMuxSet(GPIO_CH0,GPIOPortB_Pin1,IOMUX_GPIOB1_SPI_CSN1);
        GpioMuxSet(GPIO_CH0,GPIOPortB_Pin2,IOMUX_GPIOB2_SPI_CLKP1);
        GpioMuxSet(GPIO_CH0,GPIOPortB_Pin3,IOMUX_GPIOB3_SPI_TXDP1);
        GpioMuxSet(GPIO_CH0,GPIOPortB_Pin4,IOMUX_GPIOB4_SPI_RXDP1);

    }
    else if(Channel == 0)
    {
        GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3,IOMUX_GPIOA3_SPI_RXDP0);
        GpioMuxSet(GPIO_CH0,GPIOPortA_Pin4,IOMUX_GPIOA4_SPI_TXDP0);
        GpioMuxSet(GPIO_CH0,GPIOPortA_Pin5,IOMUX_GPIOA5_SPI_CLKP0);
        GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6,IOMUX_GPIOA6_SPI_CSN0);
    }
}


_BSP_EVK_V10_BSP_INIT_
void SdMmcDevGpioInit(uint32 DevID, uint32 Channel)
{
    if(DevID == 0)
    {
        if(Channel == 0)
        {
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1, IOMUX_GPIOC1_SDMMC_CMDP0);
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2, IOMUX_GPIOC2_SDMMC_CLKP0);
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3, IOMUX_GPIOC3_SDMMC_DATA0P0);
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4, IOMUX_GPIOC4_SDMMC_DATA1P0);
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5, IOMUX_GPIOC5_SDMMC_DATA2P0);
            GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6, IOMUX_GPIOC6_SDMMC_DATA3P0);
        }
        else
        {
            GpioMuxSet(GPIO_CH0,GPIOPortA_Pin3, IOMUX_GPIOA3_SDMMC_CCLKP1);
            GpioMuxSet(GPIO_CH0,GPIOPortA_Pin6, IOMUX_GPIOA6_SDMMC_CMDP1);
            DataPortMuxSet(IO_SDMMC);
        }
    }
}

//WIFI Control IO Define
#define WIFI_POWER          GPIOPortD_Pin7
#define WIFI_REG_ON         GPIOPortD_Pin2
#define WIFI_HOST_WAKE      GPIOPortC_Pin7
#define SDMMC_POWER_EN      GPIOPortA_Pin0


_DRIVER_SDIO_SDIODEVICE_INIT_
void SdioDevGpioInit(uint32 DevID)
{
    if(DevID == 0)
    {
        GpioMuxSet(GPIO_CH0,SDMMC_POWER_EN,IOMUX_GPIOA0_SDMMC_PWREN);

        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1,IOMUX_GPIOC1_IO);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2,IOMUX_GPIOC2_IO);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3,IOMUX_GPIOC3_IO);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4,IOMUX_GPIOC4_IO);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5,IOMUX_GPIOC5_IO);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6,IOMUX_GPIOC6_IO);

        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin1,GPIO_OUT);
        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin2,GPIO_OUT);
        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin3,GPIO_OUT);
        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin4,GPIO_OUT);
        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin5,GPIO_OUT);
        Gpio_SetPinDirection(GPIO_CH0,GPIOPortC_Pin6,GPIO_OUT);

        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin1,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin2,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin3,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin4,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin5,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,GPIOPortC_Pin6,GPIO_LOW);

        //WIFI Host wakeup: hold low level after powerup
        GpioMuxSet(GPIO_CH0,WIFI_HOST_WAKE,IOMUX_GPIOC7_IO);
        Gpio_SetPinDirection(GPIO_CH0,WIFI_HOST_WAKE,GPIO_OUT);
        Gpio_SetPinLevel(GPIO_CH0,WIFI_HOST_WAKE,GPIO_LOW);

        //WIFI RegOn: hold low level after powerup
        GpioMuxSet(GPIO_CH0,WIFI_REG_ON,IOMUX_GPIOD2_IO);
        Gpio_SetPinDirection(GPIO_CH0,WIFI_REG_ON,GPIO_OUT);
        Gpio_SetPinLevel(GPIO_CH0,WIFI_REG_ON,GPIO_LOW);

        //WIFI Power Down
        Gpio_SetPinDirection(GPIO_CH0,WIFI_POWER, GPIO_OUT);
        Gpio_SetPinLevel(GPIO_CH0,WIFI_POWER, GPIO_LOW);

        DelayMs(250);


        //=========================================================
        //WIFI Power Initial: PowerDown, SDMMC Low Level
        //WIFI Power Down
        Gpio_SetPinDirection(GPIO_CH0,WIFI_POWER, GPIO_OUT);
        Gpio_SetPinLevel(GPIO_CH0,WIFI_POWER, GPIO_HIGH);
        DelayMs(250);

        //WIFI RegOn: hold low level after powerup
        GpioMuxSet(GPIO_CH0,WIFI_REG_ON,IOMUX_GPIOD2_IO);
        Gpio_SetPinDirection(GPIO_CH0,WIFI_REG_ON,GPIO_OUT);
        Gpio_SetPinLevel(GPIO_CH0,WIFI_REG_ON,GPIO_HIGH);
        DelayMs(100);

        //WIFI Host wakeup: hold low level after powerup
        GpioMuxSet(GPIO_CH0,WIFI_HOST_WAKE,IOMUX_GPIOC7_IO);
        Gpio_SetPinDirection(GPIO_CH0,WIFI_HOST_WAKE,GPIO_IN);


        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin1, IOMUX_GPIOC1_SDMMC_CMDP0);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin2, IOMUX_GPIOC2_SDMMC_CLKP0);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin3, IOMUX_GPIOC3_SDMMC_DATA0P0);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin4, IOMUX_GPIOC4_SDMMC_DATA1P0);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin5, IOMUX_GPIOC5_SDMMC_DATA2P0);
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin6, IOMUX_GPIOC6_SDMMC_DATA3P0);

    }
}

_BSP_EVK_V10_BSP_INIT_
void UartDevGpioInit(uint32 DevID)
{
    uint32 *uartCh;
    uartCh = (uint32*)UART_CONF;

    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin5,IOMUX_GPIOB5_UART_TXD);
    GpioMuxSet(GPIO_CH0,GPIOPortB_Pin6,IOMUX_GPIOB6_UART_RXD);

    switch(DevID)
    {
        case UART_CH0:
            //*uartCh = UART0_EN;
            break;

        case UART_CH1:
            *uartCh = UART1_EN;
            break;

        case UART_CH2:
            *uartCh = UART2_EN;
            break;

        case UART_CH3:
            *uartCh = UART3_EN;
            break;

        case UART_CH4:
            *uartCh = UART4_EN;
            break;

        case UART_CH5:
            *uartCh = UART5_EN;
            break;
    }
}

_BSP_EVK_V10_BSP_INIT_
void I2CDevGpioInit(uint32 DevID)
{
    if(DevID == I2C_CH0)
    {
        GpioMuxSet(GPIO_CH0,GPIOPortC_Pin7,IOMUX_GPIOC7_I2C_SCL);
        GpioMuxSet(GPIO_CH0,GPIOPortD_Pin0,IOMUX_GPIOD0_I2C_SDA);
    }
    else if(DevID == I2C_CH1)
    {
        // TODO:

    }
    else if(DevID == I2C_CH2)
    {
        // TODO:

    }
}

