/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: System\Shell\ShellTask.c
* Owner: aaron.sun
* Date: 2015.6.10
* Time: 16:03:21
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.10     16:03:21   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#include "platform.h"
#ifdef _USE_SHELL_
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "AppInclude.h"
#include "ModuleFormat.h"
#include "LCDDriver.h"
#include "FwAnalysis.h"
#include "GUITask.h"
#include "music.h"
#include "ap6181wifi.h"
#ifdef SHELL_DEV_PAR
#include "ShellDevCmd.h"
#endif
/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/


#define EFFECT_TEST_NUM 0X000000ff
#define EFFECT_TEST_BASE_NUM 3000

#define SHELL_RX_BUF_MAX_SIZE                128
#define SHELL_CMD_MAX_ITEM                   1

typedef rk_err_t(* SHELL_PARASE_FUN)(HDC dev, uint8 *pStrBuff);

typedef struct _SHELL_CMD_ITEM
{
    uint32 useflag;
    uint32 CmdID;
    uint8 * ShellCmdName;
    SHELL_PARASE_FUN ShellCmdParaseFun;

}SHELL_CMD_ITEM;


typedef struct SHELL_TASK_DATA_BLOCK
{
    uint8 ShellRxBuffer[SHELL_RX_BUF_MAX_SIZE];
    uint32 ShellRxStart;
    HDC hUart;
    SHELL_CMD_ITEM CmdHeadItem[SHELL_CMD_MAX_ITEM];

#ifdef SHELL_CPU_TEST
    uint32 IdleTime;
    uint32 IdleCnt;
    uint32 SysTick;
    uint32 temp;
#endif

}SHELL_TASK_DATA_BLOCK;

typedef struct _SHELL_CMD_INFO
{
    uint8 * ShellCmdName;
    uint32 ModuleID;
    SHELL_PARASE_FUN ShellCmdParaseFun;

}SHELL_CMD_INFO;

#define _SYSTEM_SHELL_SHELLTASK_READ_  __attribute__((section("system_shell_shelltask_read")))
#define _SYSTEM_SHELL_SHELLTASK_WRITE_ __attribute__((section("system_shell_shelltask_write")))
#define _SYSTEM_SHELL_SHELLTASK_INIT_  __attribute__((section("system_shell_shelltask_init")))
#define _SYSTEM_SHELL_SHELLTASK_SHELL_  __attribute__((section("system_shell_shelltask_shell")))

/*
*********************************************************************************************************
*                                      Variable Define
*********************************************************************************************************
*/

_SYSTEM_SHELL_SHELLTASK_READ_
static char* ShellRootName[] =
{
    "task",
    "event",
    "device",
    "system",
    "custom",
    "connect",
    "remove",
    "cpu",
    "rkos",
    "help",
    "bsp",
    //"wifi",
   // "dlna",
    //"music",
    "\b"                          // the end

};

_SYSTEM_SHELL_SHELLTASK_READ_
static char* ShellSystemName[] =
{
    "memory",
#ifdef _MEMORY_LEAK_CHECH_
    "mleakstart",
    "mleakwatch",
    "mleakstop",
#endif
    "\b"
};


_SYSTEM_SHELL_SHELLTASK_WRITE_
static SHELL_CMD_INFO ShellRegisterName[] =
{
    //add new shell cmd here
    //...
#ifdef __DRIVER_PWM_PWMDEVICE_C__
    "pwm",MODULE_ID_PWM_SHELL,PwmDev_Shell,
#endif
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
    "display",MODULE_ID_DISPLAY_SHELL,DisplayDev_Shell,
#endif
#ifdef __DRIVER_KEY_KEYDEVICE_C__
    "key",MODULE_ID_KEY_SHELL,KeyDev_Shell,
#endif
#ifdef __DRIVER_AD_ADCDEVICE_C__
    "adc",MODULE_ID_ADC_SHELL,ADCDev_Shell,
#endif
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    "timer",MODULE_ID_TIMER_SHELL,TimerDev_Shell,
#endif
#ifdef _SPIFLASH_DEV_SHELL_
    "spiflash",MODULE_ID_SPIFLASH_SHELL,SpiFlashDev_Shell,
#endif

#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    "bcore",MODULE_ID_BCORE_SHELL,BcoreDev_Shell,
#endif
#ifdef __DRIVER_MSG_MSGDEVICE_C__
    "msg",MODULE_ID_MSG_SHELL,MsgDev_Shell,
#endif
#ifdef __DRIVER_FILE_DIRDEVICE_C__
    "dir",MODULE_ID_DIR_SHELL,DirDev_Shell,
#endif
#ifdef __DRIVER_ROCKCODEC_ROCKCODECDEVICE_C__
    "rockcodec",MODULE_ID_ROCKCODEC_SHELL,RockCodecDev_Shell,
#endif
#ifdef __DRIVER_I2S_I2SDEVICE_C__
    "i2s",MODULE_ID_I2S_SHELL,I2SDev_Shell,
#endif
#ifdef __DRIVER_AUDIO_AUDIODEVICE_C__
    "audio",MODULE_ID_AUDIO_SHELL,AudioDev_Shell,
#endif
#ifdef __APP_AUDIO_AUDIOMAINTASK_C__
    "audiomix",NULL,AudioTask_Shell,
#endif
#ifdef __DRIVER_USBHUB_USBHUBDEVICE_C__
    "usbhub",MODULE_ID_USBHUB_SHELL,USBHUBDev_Shell,
#endif
#ifdef __DRIVER_USBETH_USBETHDEVICE_C__
    "usbeth",MODULE_ID_USBETH_SHELL,USBETHDev_Shell,
#endif

#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    "usbmsc",MODULE_ID_USBMSC_SHELL,USBMSCDev_Shell,
#endif
#ifdef __DRIVER_MAILBOX_MAILBOXDEVICE_C__
    "mailbox",MODULE_ID_MAILBOX_SHELL,MailBoxDev_Shell,
#endif
#ifdef __DRIVER_WATCHDOG_WATCHDOGDEVICE_C__
    "wdt", NULL, WDTDev_Shell,
#endif

#ifdef __DRIVER_WM8987_WM8987DEVICE_C__
    "8987",NULL,WM8987Dev_Shell,
#endif

#ifdef __DRIVER_RK61X_RK618DEVICE_C__
    "rk618",MODULE_ID_RK618_SHELL,RK618Dev_Shell,
#endif
#ifdef __DRIVER_SD_SDDEVICE_C__
    "sd",MODULE_ID_SD_SHELL,SdDev_Shell,
#endif

#ifdef __DRIVER_I2C_I2CDEVICE_C__
    "i2c",MODULE_ID_I2C_SHELL,I2CDev_Shell,
#endif
#ifdef __DRIVER_USB_USBOTGDEV_C__
    "usbotg",MODULE_ID_USBOTG_SHELL,UsbOtgDev_Shell,
#endif
#ifdef __DRIVER_WLC_WLCDEVICE_C__
    "wlc",MODULE_ID_WLC_SHELL,WlcDev_Shell,
#endif
#ifdef __DRIVER_RK903_RK903DEVICE_C__
    "rk903",MODULE_ID_RK903_SHELL,Rk903Dev_Shell,
#endif
#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    "sdio",MODULE_ID_SDIO_SHELL,SdioDev_Shell,
#endif
#ifdef __DRIVER_SPINOR_SPINORDEVICE_C__
    "spinor",NULL,SpiNorDev_Shell,
#endif

#ifdef __DRIVER_SPI_SPIDEVICE_C__
     "spi",NULL,SpiDev_Shell,
#endif

#ifdef __DRIVER_FIFO_FIFODEVICE_C__
    "fifo",MODULE_ID_FIFO_SHELL,fifoDev_Shell,
#endif

#ifdef __SYSTEM_EXAMPLE_MODULEFORMAT_C__
    "test",NULL,Test_Shell,
#endif

#ifdef __APP_EXAMPLE_TASKFORMAT_C__
    "test",NULL,TestTask_Shell,
#endif
#ifdef __DRIVER_EXAMPLE_DEVICEFORMAT_C__
    "test",NULL,TestDev_Shell,
#endif

#ifdef __DRIVER_USB_USBOTGDEVICE_C__
    "otg", NULL, USBOtgShell,
#endif

#ifdef __DRIVER_MSC_MSCDEVICE_C__
    "msc", NULL, MscShell,
#endif

#ifdef __DRIVER_SDMMC_SDMMCDEVICE_C__
    "sdmmc",MODULE_ID_SDC_SHELL,SdcDev_Shell,
#endif

#ifdef __DRIVER_EMMC_EMMCDEVICE_C__
    "emmc", MODULE_ID_EMMC_SHELL, EmmcDev_Shell,
#endif

#ifdef __DRIVER_LUN_LUNDEVICE_C__
    "lun", MODULE_ID_LUN_SHELL, LunDev_Shell,
#endif

#ifdef __DRIVER_PARTION_PARTIONDEVICE_C__
    "par", MODULE_ID_PAR_SHELL, ParDev_Shell,
#endif

#ifdef __DRIVER_VOLUME_FATDEVICE_C__
    "fat", MODULE_ID_FAT_SHELL, FatDev_Shell,
#endif

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    "file", MODULE_ID_FILE_SHELL, FileDev_Shell,
#endif


#ifdef __DRIVER_ALC5633_ALC5633DEVICE_C__
    "5633", NULL,  ALC5633Shell,
#endif

#ifdef __DRIVER_DMA_DMADEVICE_C__
    "dma", NULL, DmaDev_Shell,
#endif

#ifdef __GUI_LCDDRIVER_C__
    "lcd", NULL, LcdShell,
#endif

#ifdef __SYSTEM_FWANALYSIS_C__
    "fw", NULL, FWShell,
#endif

#ifdef __DRIVER_UART_UARTDEVICE_C__
    "uart", NULL, UartDev_Shell,
#endif

#ifdef __CPU_NANOC_LIB_HW_MP3_IMDCT_C__
    "imdct", NULL, hw_imdct_shell,
#endif
#ifdef __CPU_NANOC_LIB_HW_MP3_SYN_C__
    "syn", NULL, hw_syn_shell,
#endif

#ifdef __CPU_NANOC_LIB_HW_YUV2RGB_C__
    "yuv", NULL, hw_yuv2rgb_shell,
#endif
#ifdef __CPU_NANOD_LIB_HIFI_APE_C__
    "ape", NULL,  hifi_ape_shell,
#endif

#ifdef __CPU_NANOD_LIB_HIFI_MAC_C__
    "mac", NULL, hifi_mac_shell,
#endif

#ifdef __CPU_NANOD_LIB_HIFI_C__
    "hifi", NULL, HIFI_SHELL,
#endif

#ifdef __CPU_NANOD_LIB_HIFI_FFT_C__
    "fft", NULL,  hifi_fft_shell,
#endif

#ifdef __CPU_NANOD_LIB_HIFI_ALAC_C__
    "alac", NULL,  hifi_alac_shell,
#endif

#ifdef __CPU_NANOD_LIB_HIFI_FLAC_C__
    "flac", NULL,  hifi_flac_shell,
#endif

#ifdef __CPU_NANOD_LIB_GPIO_C__
    "gpio", NULL, GPIOShell,
#endif

#ifdef __APP_AUDIO_MUSIC_C__
    "music", MODULE_ID_MUSIC_SHELL, music_shell,
#endif
#ifdef _DRIVER_AP6181_WIFI_C__
    "wifi",NULL,wifi_shell,
#endif

    "\b", NULL, NULL
};

static SHELL_TASK_DATA_BLOCK*   gpstShellTaskDataBlock;


#ifdef SHELL_CPU_TEST

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
void RkosEffectTestReport (void);

int sCoefs[32] =
{
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,0 , 0 , 0,
    0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,0 , 0 , 0

};

int tempbuf[EFFECT_TEST_BASE_NUM];


void Filt(int* in, int len)
{
    int iLoop;
    int *inptr;
    long long temp;
    int *coeff;
    int * out;

    out = in + len;

    while (len--)
    {
        coeff = sCoefs;
        inptr = in;
        in++;
        temp = 0;

        iLoop = 32;
        while (iLoop--)
        {
            int scoeff;
            scoeff = *coeff;
            temp += scoeff * (*inptr--);
            coeff++;
        }
        *out = (int)temp;
        out++;
    }

}


void RkosEffectTest3()
{
    if (gpstShellTaskDataBlock->IdleCnt < EFFECT_TEST_NUM)
    {
        Filt(tempbuf, EFFECT_TEST_BASE_NUM);
        gpstShellTaskDataBlock->IdleCnt++;
    }
    else if (gpstShellTaskDataBlock->IdleCnt == EFFECT_TEST_NUM)
    {
        if (gpstShellTaskDataBlock->IdleTime != NULL)
        {
            pIdleFun = pIdleFunSave;
            gpstShellTaskDataBlock->IdleCnt = 0;
            gpstShellTaskDataBlock->IdleTime = NULL;
        }
        else
        {
            pIdleFun = NULL;
            gpstShellTaskDataBlock->IdleCnt = 0;
        }
    }

}

void MemoryReadWriteTest()
{
    //SCAN H_IRAM
    uint8 * pbuf = rkos_memory_malloc(512);
    uint8 * pbuf1;
    uint8 * pbuf2;
    uint32 i, j;
    uint8 temp;

    for (i = 0; i < EFFECT_TEST_BASE_NUM; i++)
    {
        pbuf1 = pbuf;
        pbuf2 = pbuf;

        for (j = 0; j < 512; j++)
        {
            *pbuf1++ = temp;
            temp = *pbuf2++;
        }
    }

    rkos_memory_free(pbuf);

}


void RkosEffectTest2()
{
    if (gpstShellTaskDataBlock->IdleCnt < EFFECT_TEST_NUM)
    {
        MemoryReadWriteTest();
        gpstShellTaskDataBlock->IdleCnt++;
    }
    else if (gpstShellTaskDataBlock->IdleCnt == EFFECT_TEST_NUM)
    {
        if (gpstShellTaskDataBlock->IdleTime != NULL)
        {
            pIdleFun = pIdleFunSave;
            gpstShellTaskDataBlock->IdleCnt = 0;
            gpstShellTaskDataBlock->IdleTime = NULL;

        }
        else
        {
            pIdleFun = NULL;
            gpstShellTaskDataBlock->IdleCnt = 0;
        }
    }

}



void MathCapacity()
{
    uint32 i, j;
    for (i = 0; i < EFFECT_TEST_BASE_NUM; i++)
        gpstShellTaskDataBlock->temp++;
}

void RkosEffectTest1()
{
    if (gpstShellTaskDataBlock->IdleCnt < EFFECT_TEST_NUM)
    {
        MathCapacity();
        gpstShellTaskDataBlock->IdleCnt++;
    }
    else if (gpstShellTaskDataBlock->IdleCnt == EFFECT_TEST_NUM)
    {
        if (gpstShellTaskDataBlock->IdleTime != NULL)
        {
            RkosEffectTestReport();
            pIdleFun = pIdleFunSave;
            gpstShellTaskDataBlock->IdleTime = 0;
            gpstShellTaskDataBlock->IdleCnt = 0;
        }
        else
        {
            pIdleFun = NULL;
            gpstShellTaskDataBlock->IdleCnt = 0;
        }
    }

}


/*
*********************************************************************************************************
*                                     void vApplicationIdleTime(void)
*
* Description:  This function timer hook for display idle time
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void RkosEffectTestReport (void)
{
    uint32 time, tick;

    tick = SysTickCounter -  gpstShellTaskDataBlock->SysTick - 1;
    time = (EFFECT_TEST_NUM * 3000)/ tick;

    printf("\r\n  rkos executive capacity is %d --- %d", time, tick);

}


/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellCpuTest(HDC dev,  char * pstr)
{
    uint32 Time, Delay, Tick;

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_CLK_OBS);
    OBS_output_Source_sel(obs_clk_sys_core);

    rk_print_string("  cpu chage freq test...\r\n");

    for (Time = SYS_MIN_FREQ; Time <= SYS_MAX_FREQ; Time++)
    {
        SetSysFreq(Time);

        rk_printf("armclk = %dMHz, \tpclk = %dMHz", chip_freq.armclk, chip_freq.pclk_logic_pre);

        Delay = 0x7ffff;
        while (Delay--);
    }

    rk_print_string("\r\n  cpu executive capacity testing...\r\n");

    vTaskSuspendAll();


    pIdleFunSave = pIdleFun;
    if (StrCmpA(pstr, "1", 1) == 0)
    {
        pIdleFun = RkosEffectTest2;
    }
    else if (StrCmpA(pstr, "2", 1) == 0)
    {
        pIdleFun = RkosEffectTest3;
    }
    else
    {
        pIdleFun = RkosEffectTest1;
    }

    gpstShellTaskDataBlock->IdleCnt = 0;
    gpstShellTaskDataBlock->SysTick = SysTickCounter;

    while (pIdleFun != NULL)
    {
        pIdleFun();
    }

    pIdleFun = pIdleFunSave;

    Tick = SysTickCounter -  gpstShellTaskDataBlock->SysTick;
    Time = (EFFECT_TEST_NUM * EFFECT_TEST_BASE_NUM)/ Tick;

    xTaskResumeAll();

    rk_printf_no_time("  cpu executive capacity is %d --- %d\r\n", Time, Tick);

    rk_print_string("\r\ncpu test over");

    return RK_SUCCESS;

}



rk_err_t ShellRkosIdleTest(HDC dev, const char * pItemName)
{
    gpstShellTaskDataBlock->IdleTime = 1;

    pIdleFunSave = pIdleFun;
    pIdleFun = RkosEffectTest1;

    rk_print_string("  rkos executive capacity testing...\r\n");

    gpstShellTaskDataBlock->SysTick = SysTickCounter;
    gpstShellTaskDataBlock->IdleCnt = 0;

    return RK_SUCCESS;
}

#endif


#ifdef SHELL_CMD_OPER

rk_err_t ShellCmdRemove(HDC dev, uint8 * pItemName)
{
    SHELL_CMD_ITEM * pCmdItem;

    uint32 i = 0, j = 0;

    pCmdItem = gpstShellTaskDataBlock->CmdHeadItem;
    for (j = 0; j < SHELL_CMD_MAX_ITEM; j++)
    {
        if (pCmdItem->useflag == 1)
        {
            if (StrCmpA((uint8*)pCmdItem->ShellCmdName, pItemName, 0) == 0)
            {
                // already register
                pCmdItem->useflag = 0;
                rk_print_string("device remove success");

                if (ShellRegisterName[pCmdItem->CmdID].ModuleID)
                {
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                    FW_RemoveModule(ShellRegisterName[pCmdItem->CmdID].ModuleID);
#endif
                }

                return RK_SUCCESS;
            }
        }
        pCmdItem++;
    }

    rk_print_string("not find cmd");

    return RK_SUCCESS;
}

rk_err_t ShellCmdRegister(HDC dev, uint8 * pItemName)
{
    SHELL_CMD_ITEM * pCmdItem;

    uint32 i = 0, j = 0;

    pCmdItem = gpstShellTaskDataBlock->CmdHeadItem;

    for (j = 0; j < SHELL_CMD_MAX_ITEM; j++)
    {
        if (pCmdItem->useflag == 1)
        {
            if (StrCmpA((uint8*)pCmdItem->ShellCmdName, pItemName, 0) == 0)
            {
                // already register
                rk_print_string("\r\ndevice already connected");
                return RK_SUCCESS;
            }
        }
        pCmdItem++;
    }

    while (StrCmpA((uint8*)ShellRegisterName[i].ShellCmdName,pItemName, 0) != 0)
    {
        i++;
        if (*((uint8*)ShellRegisterName[i].ShellCmdName) == '\b')
        {
            rk_print_string("\r\ndevice not exist");
            return RK_SUCCESS;
        }
    }


    pCmdItem = gpstShellTaskDataBlock->CmdHeadItem;
    for (j = 0; j < SHELL_CMD_MAX_ITEM; j++)
    {
        if (pCmdItem->useflag == 0)
        {
            if (ShellRegisterName[i].ModuleID != 0)
            {
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                if (FW_ModuleOverlay(ShellRegisterName[i].ModuleID, MODULE_OVERLAY_CODE) != RK_SUCCESS)
                {
                    rk_print_string("\r\ndevice connect err");
                    return RK_SUCCESS;
                }
#endif
            }
            pCmdItem->ShellCmdParaseFun  = ShellRegisterName[i].ShellCmdParaseFun;
            pCmdItem->ShellCmdName = ShellRegisterName[i].ShellCmdName;
            pCmdItem->useflag = 1;
            pCmdItem->CmdID = i;

            rk_print_string("\r\ndevice connect success");

            return RK_SUCCESS;
        }
        pCmdItem++;
    }

    rk_print_string("\r\ncmd full");

    return RK_SUCCESS;
}

#endif

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
#ifdef SHELL_SYS_PAR
rk_err_t ShellMemory(HDC dev)
{
    uint8 TxBuffer[128];
    uint32 MemorySize;
    uint16 BuffSize = 0;

    if (dev == NULL)
        return RK_ERROR;

    MemorySize = RKTaskHeapFree();
    BuffSize = sprintf(TxBuffer,"Total       Used      Remaining\r\n");
    UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

    BuffSize = sprintf(TxBuffer,"-------------------------------\r\n");
    UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);


    BuffSize = sprintf(TxBuffer,"0x%x      ",RKTaskHeapTotal());
    UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

    BuffSize = sprintf(TxBuffer,"0x%x      ",(RKTaskHeapTotal() - RKTaskHeapFree()));
    UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);

    BuffSize = sprintf(TxBuffer,"0x%x    ",MemorySize);
    UartDev_Write(dev,TxBuffer,BuffSize,SYNC_MODE,NULL);
#ifdef _MEMORY_LEAK_CHECH_
    memory_leak_check_start();
#endif
    return RK_SUCCESS;
}
#ifdef _MEMORY_LEAK_CHECH_
rk_err_t ShellMemoryLeakstart(HDC dev)
{
    uint8 TxBuffer[128];
    uint32 MemorySize;
    uint16 BuffSize = 0;

    if (dev == NULL)
        return RK_ERROR;

    printf("Remaining = %d, 0x%04x\n",RKTaskHeapFree(), RKTaskHeapFree());
    memory_leak_check_start();

    return RK_SUCCESS;
}


rk_err_t ShellMemoryLeakWatch(HDC dev)
{
    uint8 TxBuffer[128];
    uint32 MemorySize;
    uint16 BuffSize = 0;

    if (dev == NULL)
        return RK_ERROR;

    printf("Remaining = %d, 0x%04x\n",RKTaskHeapFree(), RKTaskHeapFree());
    memory_leak_check_watch();

    return RK_SUCCESS;
}


rk_err_t ShellMemoryLeakStop(HDC dev)
{
    uint8 TxBuffer[128];
    uint32 MemorySize;
    uint16 BuffSize = 0;

    if (dev == NULL)
        return RK_ERROR;

    printf("Remaining = %d, 0x%04x\n",RKTaskHeapFree(), RKTaskHeapFree());
    memory_leak_check_stop();

    return RK_SUCCESS;
}
#endif
/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellSystemParsing(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
        return RK_ERROR;

    ret = ShellCheckCmd(ShellSystemName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    switch (i)
    {
        case 0x00:
            ret = ShellMemory(dev);
            break;
#ifdef _MEMORY_LEAK_CHECH_
        case 0x01:
        ret = ShellMemoryLeakstart(dev);
        break;
        case 0x02:
            ret = ShellMemoryLeakWatch(dev);
            break;
        case 0x03:
            ret = ShellMemoryLeakStop(dev);
            break;
#endif
        default:
            break;
    }

    return ret;
}

#endif

#ifdef SHELL_HELP
/*
*********************************************************************************************************
*                                      ShellHelpParsing(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellHelpParsing(HDC dev, uint8 * pstr)
{
    pstr--;

    if (StrLenA(pstr) != 0)
        return RK_ERROR;

    rk_print_string("有关某个命令的详细信息，请键入 命令名.help\r\n");
    rk_print_string("RKOS SHELL 命令支持多级命令，不同级别之间的命令之间用.号隔开，比如usbotg.mc\r\n");
    rk_print_string("下面是顶层命令列表:\r\n");
    rk_print_string("task             用于查看RKOS中所有已创TASK的信息\r\n");
    rk_print_string("event            用于查看RKOS中所有已创event的信息，主要是消息队列和信号量\r\n");
    rk_print_string("device           用于查看RKOS中所有已创device, device list的信息\r\n");
    rk_print_string("system           用于查看RKOS整个系统中的资源使用情况\r\n");
    rk_print_string("custom           用户自定义命令集合\r\n");
    rk_print_string("connect          用于连接具体的对象\r\n");
    rk_print_string("remove           用于移除具体的对象\r\n");
    rk_print_string("cpu              用于测试CPU的总执行能力\r\n");
    rk_print_string("rkos             用于测试RKOS的剩余执行能力\r\n");
    rk_print_string("help             RKOS SHELL帮助平台 \r\n");
    rk_print_string("bsp              RKOS 板级驱动包测试平台\r\n");
    rk_print_string("usbotg           USBOTG设备测试总入口\r\n");
    rk_print_string("wlc              WLC设备测试总入口\r\n");
    rk_print_string("rk903            RK903设备测试总入口\r\n");
    rk_print_string("sdio             SDIO设备测试总入口\r\n");
    rk_print_string("spi              SPI设备测试总入口\r\n");
    rk_print_string("spinor           SPINOR设备测试总入口\r\n");
    rk_print_string("fifo             FIFO设备测试总入口\r\n");
    rk_print_string("media            MEDIA任务测试总入口\r\n");
    rk_print_string("filestream       FILESTREAM任务测试总入口\r\n");
    rk_print_string("sdmmc            SDMMC设备测试总入口\r\n");
    rk_print_string("emmc             EMMC设备测试总入口\r\n");
    rk_print_string("lun              LUN设备测试总入口\r\n");
    rk_print_string("par              PARTION设备测试总入口\r\n");
    rk_print_string("fat              FAT文件系统设备测试总入口\r\n");
    rk_print_string("file             FILE文件操作设备测试总入口\r\n");
    rk_print_string("i2c              I2C总线设备测试总入口\r\n");
    rk_print_string("5633             5633芯片驱动测试总入口\r\n");
    rk_print_string("8987             8987芯片驱动测试总入口\r\n");
    rk_print_string("rockcodec        CODEC设备测试总入口\r\n");
    rk_print_string("i2s              I2S总线设备测试总入口\r\n");
    rk_print_string("dma              DMA设备测试总入口\r\n");
    rk_print_string("audio            AUDIO设备测试总入口\r\n");
    rk_print_string("gui              GUI任务测试总入口\r\n");
    rk_print_string("lcd              LCD驱动测试总入口\r\n");
    rk_print_string("fw               固件测试总入口\r\n");
    rk_print_string("pwm              PWM设备测试总入口\r\n");
    rk_print_string("timer            Timer设备测试总入口\r\n");
    rk_print_string("uart             uart设备测试总入口\r\n");
    rk_print_string("adc              adc设备测试总入口\r\n");
    rk_print_string("wdt              watchdog设备测试总入口\r\n");
    rk_print_string("mailbox          mailbox设备测试总入口\r\n");
    rk_print_string("usbmsc           usbmsc设备测试总入口\r\n");
    rk_print_string("audiomix         audio设备混音测试总入口\r\n");
    rk_print_string("music            music控制总入口\r\n");

    return RK_SUCCESS;
}

#endif

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description: 根目录元素提取
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellRootParsing(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 temp;
    SHELL_CMD_ITEM * pTempCmdItem = NULL;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        rk_print_string("error cmd\r\n");
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellRootName, pItem, StrCnt);

    if (ret < 0)
    {
#ifdef SHELL_CMD_OPER

        pTempCmdItem = gpstShellTaskDataBlock->CmdHeadItem;

        temp = pstr[StrCnt];
        pstr[StrCnt] = 0;

        for (i = 0; i < SHELL_CMD_MAX_ITEM; i++)
        {
            if (pTempCmdItem->useflag == 1)
            {
                if (StrCmpA(pTempCmdItem->ShellCmdName, pstr, 0) == 0)
                {
                    if (pTempCmdItem->ShellCmdParaseFun != NULL)
                    {
                        pItem += StrCnt;
                        pItem++;

                        ret = pTempCmdItem->ShellCmdParaseFun(dev,pItem);
                        if (ret == RK_ERROR)
                        {
                            rk_print_string("error cmd\r\n");
                        }
                        pstr[StrCnt] = temp;
                        return ret;
                    }
                }
            }
            pTempCmdItem++;
        }

#endif
        pstr[StrCnt] = temp;
        rk_print_string("error cmd\r\n");
        return RK_SUCCESS;

    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:
#ifdef SHELL_TASK_PAR
            ret = ShellTaskParsing(dev,pItem);
#endif
            break;

        case 0x01:
#ifdef SHELL_EVENT_PAR
            ret = ShellEventParsing(dev, pItem);
#endif
            break;

        case 0x02:
#ifdef SHELL_DEV_PAR
            ret = ShellDeviceParsing(dev,pItem);
#endif
            break;

        case 0x03:
#ifdef SHELL_SYS_PAR
            ret = ShellSystemParsing(dev, pItem);
#endif
            break;

        case 0x04:
#if SHELL_CUSTOM_PAR
            ret = ShellCustomParsing(dev,pItem);
#endif
            break;

        case 0x05:
#ifdef SHELL_CMD_OPER
            ret = ShellCmdRegister(dev, pItem);
#endif
            break;

        case 0x06:
#ifdef SHELL_CMD_OPER
            ret = ShellCmdRemove(dev, pItem);
#endif
            break;

        case 0x07:
#ifdef SHELL_CPU_TEST
            ret = ShellCpuTest(dev, pItem);
#endif
            break;

        case 0x08:
#ifdef SHELL_CPU_TEST
            ret = ShellRkosIdleTest(dev, pItem);
#endif
            break;

        case 0x09:
#ifdef SHELL_HELP
            ret = ShellHelpParsing(dev, pItem);
#endif
            break;

        case 0x0A:
#ifdef SHELL_BSP
            ret = ShellBsp(dev, pItem);
#endif
            break;
 //       case 0x0B:
//#ifdef _DRIVER_AP6181_WIFI_C__
        //RKTaskCreate(TASK_ID_MAIN_TASK,0, NULL);
    //    wifi_shell_test();
//#endif
  //      break;
//        case 0x0C:
//#ifdef __APP_AUDIO_MUSIC_C__
//        printf("shell task music_shell pItem=%s \n",pItem);

//        music_shell(pItem);
//        printf("shell task music over \n");
//#endif
        break;
        default:
            ret = RK_ERROR;
            break;

    }

    if (ret == RK_ERROR)
    {
        rk_print_string("error cmd\r\n");
    }

    return ret;
}

/*
*********************************************************************************************************
*                                      void ShellTaskInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_SYSTEM_SHELL_SHELLTASK_INIT_
rk_err_t ShellTaskInit(void *pvParameters, void * arg)
{
    RK_TASK_CLASS*   pShellTask = (RK_TASK_CLASS*)pvParameters;
    SHELL_TASK_DATA_BLOCK*  pShellTaskDataBlock;
    SHELL_CMD_ITEM * pShellCmdItem;

    uint32 i;

    if (pShellTask == NULL)
        return RK_PARA_ERR;

    pShellTaskDataBlock = rkos_memory_malloc(sizeof(SHELL_TASK_DATA_BLOCK));
    memset(pShellTaskDataBlock,NULL,sizeof(SHELL_TASK_DATA_BLOCK));

    pShellCmdItem = (void *)pShellTaskDataBlock->CmdHeadItem;
    for (i = 0; i < SHELL_CMD_MAX_ITEM; i++)
    {
        pShellCmdItem->useflag = 0;
        pShellCmdItem++;
    }

    pShellTaskDataBlock->hUart = RKDev_Open(DEV_CLASS_UART, UART_DEV1, NOT_CARE);
    if (pShellTaskDataBlock->hUart ==  (HDC)RK_ERROR)
        goto exit;

    gpstShellTaskDataBlock = pShellTaskDataBlock;

    Grf_GpioMuxSet(SYSTEM_START_LED_GPIO_CH, SYSTEM_START_LED_GPIO_PIN, Type_Gpio);
    Gpio_SetPinDirection(SYSTEM_START_LED_GPIO_CH, SYSTEM_START_LED_GPIO_PIN, GPIO_OUT);

    Gpio_SetPinLevel(SYSTEM_START_LED_GPIO_CH, SYSTEM_START_LED_GPIO_PIN, SYSTEM_START_LED_ON_STATE);

    Grf_GpioMuxSet(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, Type_Gpio);
    Gpio_SetPinDirection(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, GPIO_OUT);
    return RK_SUCCESS;

exit:

    return RK_ERROR;
}


/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
_SYSTEM_SHELL_SHELLTASK_INIT_
rk_err_t ShellTaskDeInit(void *pvParameters)
{
    RK_TASK_CLASS*     TaskHandler = (RK_TASK_CLASS*)pvParameters;

    if (RKDev_Close(gpstShellTaskDataBlock->hUart) == RK_ERROR)
        goto exit;

    rkos_memory_free(gpstShellTaskDataBlock);

    return RK_SUCCESS;

exit:
    return RK_ERROR;

}

/*
*********************************************************************************************************
*                                      void ShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
void ShellTask(void)
{
    uint32 i,j;
    uint8* pstr;
    HDC pstTimerDevHandler;
    pstr = gpstShellTaskDataBlock->ShellRxBuffer;
    memset(pstr,0,SHELL_RX_BUF_MAX_SIZE);
	rkos_delay(300);

    rk_print_string("\r\nWelcome to Use RKOS\r\n");
    rk_print_string("RKOS V1.0.0 - Copyright (C) 2018 Fuzhou Rockchips Electronics CO.,Ltd\r\n");
    rk_print_string("Please visit http://www.rock-chips.com to ensure you are using the last verison\r\n");
    rk_print_string("***************************************************************************\r\n");
    rk_print_string(" *                                                                       *\r\n");
    rk_print_string(" *    RKOS SHELL V1.0.0                                                  *\r\n");
    rk_print_string(" *    Copyright (C) 2018 Fuzhou Rockchips Electronics CO.,Ltd            *\r\n");
    rk_print_string(" *                                                                       *\r\n");
    rk_print_string(" *    >>> See the program guide of project for details. <<<              *\r\n");
    rk_print_string(" *    If you do not how to use RKOS, please input help cmd.              *\r\n");
    rk_print_string(" *    also send message to aaron.sun@rock-chips.com                      *\r\n");
    rk_print_string(" *    Thank you for using RKOS, and thank you for your support!          *\r\n");
    rk_print_string(" *                                                                       *\r\n");
    rk_print_string("***************************************************************************\r\n");

    if (Grf_Get_Cpu_ID())
    {
        rk_print_string("cur system is BB system\r\n");
    }
    else
    {
        rk_print_string("cur system is AP system\r\n");
    }

    pstr[0] = 'r';
    pstr[1] = 'k';
    pstr[2] = 'o';
    pstr[3] = 's';
    pstr[4] = ':';
    pstr[5] = '/';
    pstr[6] = '/';

    gpstShellTaskDataBlock->ShellRxStart = 7;

    FREQ_EnterModule(FREQ_MAX);

#if 0

    wifi_shell_test();

#ifdef __WIFI_DLNA_C__
#ifndef _JTAG_DEBUG__
    FW_ModuleOverlay(MODULE_ID_DLNA,MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_UPNP,MODULE_OVERLAY_CODE);
#endif
    RKTaskCreate(TASK_ID_DLNA,0, NULL);
#endif

#endif


    while (1)
    {
        rk_print_string("\r\n");
        rk_print_string(pstr);

        i = gpstShellTaskDataBlock->ShellRxStart;

        while (i <= SHELL_RX_BUF_MAX_SIZE)
        {

            UartDev_Read(gpstShellTaskDataBlock->hUart, pstr + i, 1);

            if (pstr[i] == 0x0d)
            {
                UartDev_Write(gpstShellTaskDataBlock->hUart,"\r\n",2,SYNC_MODE,NULL);
                break;
            }
            else if ((pstr[i] < 32) && (pstr[i] != '\b'))
            {
                continue;
            }
            else if ((pstr[i] >= 127))
            {
                continue;
            }

            UartDev_Write(gpstShellTaskDataBlock->hUart,pstr+i,1,SYNC_MODE,NULL);

            if (pstr[i] == '\b')
            {
                if (i == gpstShellTaskDataBlock->ShellRxStart)
                {
                    if (gpstShellTaskDataBlock->ShellRxStart == 7)
                    {
                        UartDev_Write(gpstShellTaskDataBlock->hUart,"/",1,SYNC_MODE,NULL);
                    }
                    else
                    {
                        UartDev_Write(gpstShellTaskDataBlock->hUart,".",1,SYNC_MODE,NULL);
                    }
                }
                else
                {
                    UartDev_Write(gpstShellTaskDataBlock->hUart," \b", 2, SYNC_MODE, NULL);
                    i--;
                }
            }
            else
            {
                i++;
            }
        }

        if (pstr[i] == 0x0d)
        {
            if (i == gpstShellTaskDataBlock->ShellRxStart)
            {
                //RKwrite(gpstShellTaskDataBlock->hUart,0,"rkos://",7,SYNC_MODE,NULL);
            }
            else if ((pstr[gpstShellTaskDataBlock->ShellRxStart] == 'c')
                     && (pstr[gpstShellTaskDataBlock->ShellRxStart + 1] == 'd')
                     && (pstr[gpstShellTaskDataBlock->ShellRxStart + 2] == 0x0d))
            {
                pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
            }
            else if ((pstr[gpstShellTaskDataBlock->ShellRxStart] == 'c')
                     && (pstr[gpstShellTaskDataBlock->ShellRxStart + 1] == 'd')
                     && (pstr[gpstShellTaskDataBlock->ShellRxStart + 2] == ' '))
            {
                if (pstr[gpstShellTaskDataBlock->ShellRxStart + 3] == 0x0d)
                {
                    pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
                }
                else if ((pstr[gpstShellTaskDataBlock->ShellRxStart + 3] == '.')
                         && (pstr[gpstShellTaskDataBlock->ShellRxStart + 4] == '.')
                         &&(pstr[gpstShellTaskDataBlock->ShellRxStart + 5] == 0x0d))
                {
                    if (gpstShellTaskDataBlock->ShellRxStart > 8)
                    {
                        for (j = gpstShellTaskDataBlock->ShellRxStart - 2; j >= 7; j--)
                        {
                            if (pstr[j] == '.')
                            {
                                break;
                            }
                        }
                        gpstShellTaskDataBlock->ShellRxStart = j + 1;
                    }
                    pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
                }
                else if ((pstr[gpstShellTaskDataBlock->ShellRxStart + 3] == '.')
                         && (pstr[gpstShellTaskDataBlock->ShellRxStart + 4] == 0x0d))
                {
                    pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
                }
                else if (memcmp(pstr + gpstShellTaskDataBlock->ShellRxStart + 3, "rkos://", 7) == 0)
                {
                    pstr[i] = 0;
                    for (j = 0; j < strlen(pstr + gpstShellTaskDataBlock->ShellRxStart + 3); j++)
                    {
                        pstr[j] = pstr[gpstShellTaskDataBlock->ShellRxStart + 3 + j];
                    }

                    if (j > 7)
                    {
                        pstr[j++] = '.';
                        gpstShellTaskDataBlock->ShellRxStart = strlen(pstr + gpstShellTaskDataBlock->ShellRxStart + 3) + 1;
                    }
                    else
                    {
                        gpstShellTaskDataBlock->ShellRxStart = strlen(pstr + gpstShellTaskDataBlock->ShellRxStart + 3);
                    }
                    pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
                }
                else
                {
                    pstr[i] = 0;
                    for (j = gpstShellTaskDataBlock->ShellRxStart; j <(gpstShellTaskDataBlock->ShellRxStart + strlen(pstr + gpstShellTaskDataBlock->ShellRxStart + 3)); j++)
                    {
                        pstr[j] = pstr[3 + j];
                    }
                    pstr[j++] = '.';

                    gpstShellTaskDataBlock->ShellRxStart += strlen(pstr + gpstShellTaskDataBlock->ShellRxStart + 3) + 1;
                    pstr[gpstShellTaskDataBlock->ShellRxStart] = 0;
                }
            }
            else
            {
                pstr[i] = 0;
                ShellRootParsing(gpstShellTaskDataBlock->hUart, pstr + 7);
            }
        }

        memset(pstr + gpstShellTaskDataBlock->ShellRxStart,0,(SHELL_RX_BUF_MAX_SIZE - gpstShellTaskDataBlock->ShellRxStart));
    }
}

#endif

