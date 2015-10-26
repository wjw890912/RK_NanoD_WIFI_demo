/*
********************************************************************************
*                   Copyright (c) 2008,ZhengYongzhi
*                         All rights reserved.
*
* File Name  BspConfig.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-9-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__
/*
*-------------------------------------------------------------------------------
*
*                            Module Config
*
*-------------------------------------------------------------------------------
*/
#define SYS_MIN_FREQ        1
#define SYS_DEFAULT_FREQ    24
#define SYS_MAX_FREQ        24
#define SYS_DATA_DISK_SIZE	40    //unit MB

//debug compile switch.
#define _JTAG_DEBUG_
#define _USE_SHELL_
//#define _SIMULATOR_

//boot switch
//#define _SPI_BOOT_
#define _EMMC_BOOT_
//#define _NAND_BOOT_
//#define _UART_BOOT_
//#define _USB_BOOT_


#ifdef _EMMC_BOOT_
#define EMMC_SDC_CHANNEL 1
#define EMMC_LUN0_SIZE 40 // unit MB
#endif

#define rk_print_string1 rk_print_string
#define rk_print_string2
#define rk_print_string3
#define rk_print_string4

#define rk_printf1 rk_printf
#define rk_printf2
#define rk_printf3
#define rk_printf4

#define rk_printf_no_time1 rk_printf_no_time
#define rk_printf_no_time2
#define rk_printf_no_time3
#define rk_printf_no_time4

#define printf1 printf
#define printf2
#define printf3
#define printf4

#define DEBUG1(format,...)  rk_printf("FILE: %s, LINE: %d: "format, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG2(format,...)
#define DEBUG3(format,...)
#define DEBUG4(format,...)


/*
*-------------------------------------------------------------------------------
*
*                            Application Modules Option
*
*-------------------------------------------------------------------------------*/
#define _MUSIC_
#define _RADIO_
#define _RECORD_
#define _BROWSER_
#define _SYSSET_
#define _VIDEO_
#define _PICTURE_
//#define _SDCARD_
//#define DISK_VOLUME
#define _USB_
//#define _MULT_DISK_                 //多磁盘选择
//#define _BEEP_
//#define _TEST_MUSIC_
//#define ENABLE_MBR
//#define DC_FILTER
//#define _DRIVER_TEST_
#define _SUPPORT_BB_SYSTEM_
#define _MEDIA_MODULE_
#define _FRAME_BUFFER_
//#define SUPPORT_YUV

//音乐播放子模块编译开关

#ifdef _MUSIC_
#define _MP3_DECODE_
#define _WMA_DECODE_
#define _APE_DECODE_
#define _AAC_DECODE_
#define _FLAC_DECODE_
#define _WAV_DECODE_
#define _OGG_DECODE_
//#define _HIFI_APE_DEC
//#define _HIFI_FLAC_DEC
//#define _HIFI_ALAC_DEC

#define _RK_EQ_
#define _FADE_PROCESS_
#define _RK_ID3_
#define _RK_SPECTRUM_
#define AUDIOHOLDONPLAY  // 断点播放开关
#define FILE_ERROR_DIALOG           //文件格式不支持对话框//sen #20090803#1 all audio file is not support
#define SCROLL_LRC

#define A_CORE_DECODE
//#definde NANOC_DECODE
#endif

#ifdef _MEDIA_MODULE_
#define BROSUB
#ifndef _JTAG_DEBUG_
#define FAVOSUB
#define MEDIA_UPDATE
#endif
#endif

#ifdef _VIDEO_
#define VIDEO_MP2_DECODE
#define VIDEO_AVI
#define VIDEO_HOLDON_PLAY
#endif

#ifdef _PICTURE_
#define _JPG_DECODE_
#define _BMP_DECODE_
#endif

//FM module compile switch.
#ifdef _RADIO_
#endif

//record module compile switch.
#ifdef _RECORD_
#define _FM_RECORD_
#define _WAV_ENCODE_
//#define _MP3_ENCODE_
#endif

#ifdef _DRIVER_TEST_
#define _HIFI_TEST_
#define _MP3_TEST_
#define _VOP_TEST_
#define _ACODEC_TEST_
#define _ADC_TEST_
#define _EMMC_TEST_
#define _GPIO_TEST_
#define _I2C_TEST_
#define _I2S_TEST_
#define _M2M_TEST_
#define _MAILBOX_TEST_
#define _MAIN2_TEST_
#define _PVTM_TEST_
#define _PWM_TEST_
#define _SD_TEST_
#define _SPI_TEST_
#define _SYSTICK_TEST_
#define _TIME_TEST_
#define _UART_TEST_
#define _WATCHDOG_TEST_
#endif

//book module compile switch.

//explorer module compile switch.
#ifdef _BROWSER_

#endif

//system setting module compile switch.
#ifdef _SYSSET_

#endif

#ifdef _BEEP_
#define _BEEP_NOT_MIXED_
#endif

#define _Vol_Tab_General
//#define _Vol_Tab_Europe


#ifdef _SUPPORT_BB_SYSTEM_

#define BB_SYS_INDEP_COMP

#ifdef BB_SYS_INDEP_COMP
//#define BB_SYS_JTAG
#endif

#endif


#ifdef SUPPORT_YUV
#define USE_LLP
#endif

#ifdef B_CORE_DECODE
#define A_CORE_DECODE
#endif


//fun

//#define _DUAL_CORE_

/*
*-------------------------------------------------------------------------------
*
*                            Key Driver Configuration
*
*-------------------------------------------------------------------------------
*/
//Key Num Define
#define KEY_NUM_4		    4
#define KEY_NUM_5		    5
#define KEY_NUM_6		    6
#define KEY_NUM_7		    7
#define KEY_NUM_8		    8

//Config KeyDriver
#define KEY_NUM  	        KEY_NUM_7

//Key Val Define
#define KEY_VAL_NONE        ((UINT32)0x0000)
#define KEY_VAL_PLAY        ((UINT32)0x0001 << 0)
#define KEY_VAL_MENU        ((UINT32)0x0001 << 1)
#define KEY_VAL_FFD         ((UINT32)0x0001 << 2)
#define KEY_VAL_FFW         ((UINT32)0x0001 << 3)
#define KEY_VAL_UP          ((UINT32)0x0001 << 4)
#define KEY_VAL_DOWN        ((UINT32)0x0001 << 5)
#define KEY_VAL_ESC         ((UINT32)0x0001 << 6)
//#define KEY_VAL_VOL         ((UINT32)0x0001 << 7)

#define KEY_VAL_UPGRADE     KEY_VAL_MENU
#define KEY_VAL_POWER       KEY_VAL_PLAY
#define KEY_VAL_HOLD        (KEY_VAL_MENU | KEY_VAL_PLAY)
#define KEY_VAL_VOL         KEY_VAL_ESC

#define KEY_VAL_MASK        ((UINT32)0x0fffffff)
#define KEY_VAL_UNMASK      ((UINT32)0xf0000000)

//Bit position define for AD keys.
#define KEY_VAL_ADKEY2		KEY_VAL_MENU
#define KEY_VAL_ADKEY3		KEY_VAL_UP
#define KEY_VAL_ADKEY4      KEY_VAL_FFD
#define KEY_VAL_ADKEY5      KEY_VAL_FFW
#define KEY_VAL_ADKEY6      KEY_VAL_DOWN
#define KEY_VAL_ADKEY7      KEY_VAL_ESC

//AdKey Reference Voltage define
//ADC ref = LDO Output
#define ADKEY2_MIN          ((0   +   0) / 2)
#define ADKEY2_MAX          ((0   + 113) / 2)

#define ADKEY3_MIN          ((113 +   0) / 2)
#define ADKEY3_MAX          ((113 + 250) / 2)

#define ADKEY4_MIN          ((250 + 113) / 2)
#define ADKEY4_MAX          ((250 + 395) / 2)

#define ADKEY5_MIN          ((395 + 250) / 2)
#define ADKEY5_MAX          ((395 + 590) / 2)

#define ADKEY6_MIN          ((590 + 395) / 2)
#define ADKEY6_MAX          ((590 + 725) / 2)

#define ADKEY7_MIN          ((725 + 590) / 2)
#define ADKEY7_MAX          ((725 + 1023) / 2)
/*
*-------------------------------------------------------------------------------
*
*                            GPIO Config
*
*-------------------------------------------------------------------------------
*/
#define SD_DET              GPIOPortD_Pin2


/*
*-----------------------------------------------------------------------------
*       include source define macro
*------------------------------------------------------------------------------
*/
#include "source_macro.h"


/*
********************************************************************************
*
*                         End of BspConfig.h
*
********************************************************************************
*/

#endif

