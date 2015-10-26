/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Bsp\EVK_V1.0\source_macro.h
* Owner: Aaron
* Date: 2014.2.17
* Time: 14:09:36
* Desc: this file define some macro using control source code whether be builied
* History:
*	 <author>	 <date> 	  <time>	 <version>	   <Desc>
* Aaron     2014.2.17     14:09:36   1.0
********************************************************************************************
*/

#ifndef __BSP_EVK_V1_0_SOURCE_MACRO_H__
#define __BSP_EVK_V1_0_SOURCE_MACRO_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//#define __DRIVER_RK61X_RK618DEVICE_C__
#define __DRIVER_SD_SDDEVICE_C__
//#define __DRIVER_PWM_PWMDEVICE_C__
//#define __DRIVER_USB_USBOTGDEV_C__
//#define __WEB_TASK_WEBTASK_C__
#define ___COMMON_SYSTEM_SYSSERVICE_SYSRESUME_C__
//#define __DRIVER_WLC_WLCDEVICE_C__
//#define __DRIVER_RK903_RK903DEVICE_C__
//#define __DRIVER_SDIO_SDIODEVICE_C__
//#define __DRIVER_SPINOR_SPINORDEVICE_C__
//#define __DRIVER_SPI_SPIDEVICE_C__
#define __DRIVER_FIFO_FIFODEVICE_C__
#define __APP_MEDIA_MEDIATASK_C__
#define __APP_FILESTREAM_FILESTREAMTASK_C__
//#define __SYSTEM_EXAMPLE_MODULEFORMAT_C__
//#define __APP_EXAMPLE_TASKFORMAT_C__
//#define __DRIVER_EXAMPLE_DEVICEFORMAT_C__
#define __DRIVER_UART_UARTDEVICE_C__

//zhuzhe usb struct
//#define __DRIVER_USB_USBDEVCLASS_C__
//#define __DRIVER_USB_USBDEVISR_C__
//#define __DRIVER_USB_USBOTGDEVICE_C__
//#define __DRIVER_USB_USBDEVICE_C__
//#define __DRIVER_MSC_MSCDEVICE_C__
//#define __DRIVER_USB_MSC_C_
//#define __CPU_NANOC_LIB_USBOTG_C__

#define __DRIVER_SDMMC_SDMMCDEVICE_C__
//#define __DRIVER_EMMC_EMMCDEVICE_C__
//#define __DRIVER_KEY_KEYDEVICE_C__

#define __DRIVER_LUN_LUNDEVICE_C__
#define __DRIVER_VOLUME_FATDEVICE_C__
#define __DRIVER_PARTION_PARTIONDEVICE_C__
#define __DRIVER_FILE_FILEDEVICE_C__
#define __DRIVER_I2C_I2CDEVICE_C__
#define __DRIVER_I2S_I2SDEVICE_C__
#define __DRIVER_RockCodec_RockCodecDEVICE_C__
//#define __OS_PLUGIN_DEVICEPLUGIN_C__
#define __DRIVER_ALC5633_ALC5633DEVICE_C__
//#define __GUI_LCDDRIVER_C__
#define __DRIVER_DMA_DMADEVICE_C__
#define __DRIVER_AUDIO_AUDIODEVICE_C__
#define __DRIVER_AD_ADCDEVICE_C__
//#define __DRIVER_TIMER_TIMERDEVICE_C__
#define __DRIVER_WM8987_WM8987DEVICE_C__
#define __DRIVER_WATCHDOG_WATCHDOGDEVICE_C__

//#define __GUI_GUITASK_C__
//#define __GUI_GUILABEL_C__
//#define __GUI_ST7735DRIVER_C__
//#define __SYSTEM_FWANALYSIS_FWANALYSIS_C__
//#define __CPU_NANOC_LIB_HW_MP3_IMDCT_C__
//#define __CPU_NANOC_LIB_HW_MP3_SYN_C__
//#define __CPU_NANOC_LIB_HW_YUV2RGB_C__
//#define __CPU_NANOC_LIB_MAILBOX_C__
//#define __CPU_NANOC_LIB_VOP_C__

//#define __BBSYSTEM_HIFI_FILE_ACCESS_C__
#define __BBSYSTEM_PCODECS_C__
#define __BBSYSTEM_CODECS_AUDIO_DECODE_APE_PAPE_C__

#ifdef  _SIMULATOR_
#define __SYSTEM_VIRTUALFILE_VIRTUALFILE_C__
#endif


#ifdef _USE_SHELL_
    #define _WM8987_DEV_SHELL_
    #define _RK618_DEV_SHELL_
    #define _SD_DEV_SHELL_
    #define _I2C_DEV_SHELL_
    #define _I2S_DEV_SHELL_
    #define _PWM_DEV_SHELL_
    #define _USBOTG_DEV_SHELL_
	#define _WLC_DEV_SHELL_
    #define _RK903_DEV_SHELL_
    #define _SDIO_DEV_SHELL_
    #define _SPINOR_DEV_SHELL_
    #define _SPI_DEV_SHELL_
    #define _FIFO_DEV_SHELL_
    #define _MEDIA_TASK_SHELL_
    #define _FILESTREAM_TASK_SHELL_
    #define _TEST_SHELL_
    #define _TEST_TASK_SHELL_
    #define _FAT_SHELL_
    #define _FILE_SHELL_
    #define _SDEMMC_SHELL_
    #define _DMA_SHELL_
    #define _AUDIO_SHELL_
    #define _UART_DEV_SHELL_
    //#define _FW_ANLYSIS_SHELL_
    #define _SYS_BOOT_SHELL_

    #define SHELL_CPU_TEST
    #define SHELL_SYS_PAR
    #define SHELL_CMD_OPER
    #define SHELL_TASK_PAR
    #define SHELL_HELP
    //#define SHELL_BSP
#endif

#define RK903    (1)
#define RK901    (2)
#define AP6181   (3)

#define WIFI_MODULE  RK903


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

