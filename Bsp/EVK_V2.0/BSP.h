/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Bsp\EVK_V2.0\BSP.h
* Owner: aaron.sun
* Date: 2015.5.14
* Time: 15:25:07
* Version: 1.0
* Desc: EVK2.0 BSP init
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.5.14     15:25:07   1.0
********************************************************************************************
*/


#ifndef __BSP_EVK_V2_0_BSP_H__
#define __BSP_EVK_V2_0_BSP_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*---------------------------------------------------------------------------------------------------------------------
*/
#include "global.h"
#include "Delay.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _BSP_EVK_V20_BSP_COMMON_  __attribute__((section("bsp_evk_v20_bsp_common")))
#define _BSP_EVK_V20_BSP_INIT_  __attribute__((section("bsp_evk_v20_bsp_init")))
#define _BSP_EVK_V20_BSP_SHELL_  __attribute__((section("bsp_evk_v20_bsp_shell")))

#define min(x, y) (x < y ? x : y)
#define max(x, y) (x < y ? y : x)

//Emmc0 Config
#define EMMC_DEV0_BUS_WIDTH  BUS_WIDTH_8_BIT
#define EMMC_DEV0_SDC_ID     0
#define EMMC_DEV0_DMA_ID     0

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern void * UartHDC;
extern uint32 SysTickCounter;
extern void (*pIdleFun)(void);
extern void (*pIdleFunSave)(void);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern void DisplayDevHwInit(uint32 DevID, uint32 Channel);
extern void TimerDevHwInit(uint32 DevID, uint32 Channel);
extern void BBReqDebug(uint8 * buf);
extern void BBDebug(void);
extern void SdDevHwInit(uint32 DevID, uint32 Channel);
extern void EmmcDevHwInit(uint32 DevID, uint32 Channel);
extern void UsbOtgDevHwInit(uint32 DevID, uint32 Channel);
extern void I2cDevHwInit(uint32 DevID, uint32 Channel);
extern void UartDevHwInit(uint32 DevID, uint32 Channel);
extern void SdioDevHwInit(uint32 DevID, uint32 Channel);
extern void SdMmcDevHwInit(uint32 DevID, uint32 Channel);
extern void SpiDevHwInit(uint32 DevID, uint32 Channel);
extern void AdcDevHwInit (uint32 DevID, uint32 Channel);
extern void PwmDevHwInit (uint32 DevID, uint32 channel);
extern void WdtDevHwInit (uint32 DevID, uint32 channel);


extern void vApplicationIdleHook(void);
extern void bsp_init(void);
extern uint32 ShellItemExtract(uint8 * pstr, uint8 ** pItem, uint8 * Space);
extern rk_err_t ShellCheckCmd(uint8 * CmdStr[],  uint8 * Cmd,  uint32 len);
extern uint32 Unicode2Ascii(uint8 * pbAscii, uint16 * pwUnicode, uint32 len);
extern uint32 Ascii2Unicode(uint8 * pbAscii, uint16 * pwUnicode, uint32 len);
extern uint32 String2Num(uint8 * pstr);
extern void MemCpy(uint8 * trg, uint8 * src, uint32 len);
extern int32 StrCmpA(uint8 * pstr1, uint8 * pstr2, uint32 len);
extern int32 StrCmpW(uint16 * pstr1, uint16 * pstr2, uint32 len);
extern uint32 StrLenA(uint8 * pstr);
extern uint32 StrLenW(uint16 * pstr);
extern void rk_count_clk_end(void);
extern void rk_count_clk_start(void);
extern void rk_print_string(const uint8 * pstr);
extern void rk_printf_no_time(const char * fmt,...);
extern void debug_hex(char *buf, int BufSize, int radix);
extern void rk_printf(const char * fmt,...);
extern int fputc(int ch, FILE *f);
#endif
