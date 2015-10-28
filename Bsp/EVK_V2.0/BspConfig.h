/*
********************************************************************************
*
*     Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
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

//#define BOARD_VERSION_RKNANOD_SDK_MAIN_V10_20150126HJH
//#define BOARD_VERSION_RKNANOD_SDK_MAIN_V21_20150515HJH
#define BOARD_VERSION_RKNANOD_SDK_MAIN_V22_20150727HJH

#define SYS_MIN_FREQ        1
#define SYS_DEFAULT_FREQ    24
#define SYS_MAX_FREQ        300
#define SYS_DATA_DISK_SIZE  40    //unit MB

//debug compile switch.
//#define _JTAG_DEBUG_
#define _LOG_DEBUG_
#define _MEMORY_LEAK_CHECH_
#ifdef _LOG_DEBUG_
#define _UART_DEBUG_
//#define _FILE_DEBUG_
#define BUG_LEVEL A
#define _USE_SHELL_
#endif

#ifdef _UART_DEBUG_
#define DEBUG_UART_PORT  1
//#define USB_SERIAL_DEBUG
#endif

#ifdef _LOG_DEBUG_
#define DEBUG(format,...)    rk_printf("FILE: %s, LINE: %d: "format, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUG2(format,...)    rk_printf2("FILE: %s, LINE: %d: "format, __FILE__, __LINE__, ##__VA_ARGS__)
#define USBDEBUG(format,...) printf("\nUSB:"format, ##__VA_ARGS__)
#define bb_printf1 rk_printf2
#else
#define DEBUG(format,...)
#define DEBUG2(format,...)
#define USBDEBUG(format,...)
#define printf(format,...)
#define bb_printf1(format,...)
#define rk_print_string(format,...)
#define rk_printf(format,...)
#define rk_printf_no_time(format,...)
#define rk_printf2(format,...)
#define rk_printf_no_time2(format,...)
#define rk_print_string2(format,...)
#endif


#ifdef _LOG_DEBUG_
#define rk_print_stringA rk_print_string
#define rk_print_stringB
#define rk_print_stringC
#define rk_print_stringD

#define rk_printfA rk_printf
#define rk_printfB
#define rk_printfC
#define rk_printfD

#define rk_printf_no_timeA rk_printf_no_time
#define rk_printf_no_timeB
#define rk_printf_no_timeC
#define rk_printf_no_timeD

#define printfA printf
#define printfB
#define printfC
#define printfD

#define DEBUGA(format,...)  rk_printf("FILE: %s, LINE: %d: "format, __FILE__, __LINE__, ##__VA_ARGS__)
#define DEBUGB(format,...)
#define DEBUGC(format,...)
#define DEBUGD(format,...)

#else
#define rk_print_stringA(format,...)
#define rk_print_stringB(format,...)
#define rk_print_stringC(format,...)
#define rk_print_stringD(format,...)

#define rk_printfA(format,...)
#define rk_printfB(format,...)
#define rk_printfC(format,...)
#define rk_printfD(format,...)

#define rk_printf_no_timeA(format,...)
#define rk_printf_no_timeB(format,...)
#define rk_printf_no_timeC(format,...)
#define rk_printf_no_timeD(format,...)

#define printfA(format,...)
#define printfB(format,...)
#define printfC(format,...)
#define printfD(format,...)

#define DEBUGA(format,...)
#define DEBUGB(format,...)
#define DEBUGC(format,...)
#define DEBUGD(format,...)
#endif


//boot switch
#define _SPI_BOOT_
//#define _EMMC_BOOT_
//#define _NAND_BOOT_
//#define _UART_BOOT_
//#define _USB_BOOT_

#ifdef _EMMC_BOOT_
#define EMMC_SDC_DEV_ID  0
#define SD_SDC_CHANNEL 0
#define EMMC_SDC_CHANNEL 1
#define EMMC_LUN0_SIZE 40 // unit MB
#define EMMC_LUN1_SIZE 40 // uint MB
#endif

//fun

//#define _DUAL_CORE_


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
#define _EBOOK_
#define _USB_
//#define DISK_VOLUME
//#define _MULT_DISK_                 //多磁盘选择
//#define ENABLE_MBR
//#define DC_FILTER
//#define _TEST_MUSIC_
#define _SUPPORT_BB_SYSTEM_
#define _MEDIA_MODULE_

#ifdef _MUSIC_

#define HIFI_MUSIC

#ifdef HIFI_MUSIC
#define CODEC_24BIT
#else
//#define A_CORE_DECODE
//#defind NANOC_DECODE
#endif


#define _MP3_DECODE_
#define _WMA_DECODE_
#define _WAV_DECODE_
//#define _OGG_DECODE_

///*
#ifdef HIFI_MUSIC
#define _HIFI_APE_DEC
#define _HIFI_FLAC_DEC
//#define _HIFI_ALAC_DEC
#else
//*/
#define _APE_DECODE_
#define _AAC_DECODE_
#define _FLAC_DECODE_
#endif

#ifdef CODEC_24BIT
//#define _RK_EQ_
//#define _RK_EQ_31_
//#define _RK_SPECTRUM_
//#define _FADE_PROCESS_
#else
#define _RK_EQ_
#define _RK_EQ_5_
//#define _RK_EQ_FAST_
//#define _RK_SPECTRUM_
//#define _FADE_PROCESS_
#endif

#define _RK_ID3_
#define AUDIOHOLDONPLAY             // 断点播放开关
#define FILE_ERROR_DIALOG           //文件格式不支持对话框//sen #20090803#1 all audio file is not support
#define SCROLL_LRC
//#define FFW_FFD_PLAY

#ifdef _MEDIA_MODULE_
#define BROSUB
#ifndef _JTAG_DEBUG_
#define FAVOSUB
#define MEDIA_UPDATE
#endif
#endif

#endif  //#ifdef _MUSIC_

//--------------------------------------------------------------------------------
#ifdef _VIDEO_
#define VIDEO_MP2_DECODE
#define VIDEO_AVI
#define VIDEO_HOLDON_PLAY
#endif

//--------------------------------------------------------------------------------
#ifdef _PICTURE_
#define _JPG_DECODE_
#define _BMP_DECODE_
//#define PIC_MEDIA
#endif

//--------------------------------------------------------------------------------
//FM module compile switch.
#ifdef _RADIO_
#endif

//--------------------------------------------------------------------------------
//record module compile switch.
#ifdef _RECORD_
#define _FM_RECORD_
#define _WAV_ENCODE_
//#define _MP3_ENCODE_
#endif

//--------------------------------------------------------------------------------
//book module compile switch.

//--------------------------------------------------------------------------------
//explorer module compile switch.
#ifdef _BROWSER_

#endif

//--------------------------------------------------------------------------------
//system setting module compile switch.
#ifdef _SYSSET_

#endif

/*
*-------------------------------------------------------------------------------
*
*                            Misc Option
*
*-------------------------------------------------------------------------------
*/
//------------------------------------------------------------------------------
//dual-core define
#ifdef _SUPPORT_BB_SYSTEM_
#define BB_SYS_INDEP_COMP
#ifdef BB_SYS_INDEP_COMP
//#define BB_SYS_JTAG
#endif  //BB_SYS_INDEP_COMP
#endif  //_SUPPORT_BB_SYSTEM_

#ifdef B_CORE_DECODE
#define A_CORE_DECODE
#endif

//------------------------------------------------------------------------------
//Audio define
//#define HP_DET_CONFIG
#define _Vol_Tab_General
//#define _Vol_Tab_Europe

//#define _BEEP_
#ifdef _BEEP_
#define _BEEP_NOT_MIXED_
#endif

//------------------------------------------------------------------------------
//Display define
//#define SUPPORT_YUV
#ifdef SUPPORT_YUV
#define USE_LLP
#endif
#define FILE_ERROR_DIALOG           //Support Dialog Display

/*
*-------------------------------------------------------------------------------
*
*                            gui Config
*
*-------------------------------------------------------------------------------
*/
#define _FRAME_BUFFER_

/*
*-------------------------------------------------------------------------------
*
*                            BOARD Config
*
*-------------------------------------------------------------------------------
*/
#ifdef BOARD_VERSION_RKNANOD_SDK_MAIN_V22_20150727HJH
#include "board_main_v22_20150727_config.h"
#endif

#ifdef BOARD_VERSION_RKNANOD_SDK_MAIN_V21_20150515HJH
#include "board_main_v21_20150515_config.h"
#endif

#ifdef BOARD_VERSION_RKNANOD_SDK_MAIN_V10_20150126HJH
#include "board_main_v10_20150126_config.h"
#endif
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

