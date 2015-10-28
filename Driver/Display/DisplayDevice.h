/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\Display\DisplayDevice.h
* Owner: aaron.sun
* Date: 2015.5.29
* Time: 10:20:33
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.5.29     10:20:33   1.0
********************************************************************************************
*/


#ifndef __DRIVER_DISPLAY_DISPLAYDEVICE_H__
#define __DRIVER_DISPLAY_DISPLAYDEVICE_H__

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
typedef  struct _DISPLAY_DEV_ARG
{
    HDC hDma;
    uint32 FrameIndex;

}DISPLAY_DEV_ARG;

#define BUFFER_MAX_NUM 1
#define H_SCR_MOD 0
#define _DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_  __attribute__((section("driver_display_displaydevice_common")))
#define _DRIVER_DISPLAY_DISPLAYDEVICE_INIT_  __attribute__((section("driver_display_displaydevice_init")))
#define _DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_  __attribute__((section("driver_display_displaydevice_shell")))
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
extern rk_err_t DisplayDev_SetVopMode(uint32 RawFormat, uint32 Version);
extern void DisplayDevGetParameter(uint32 *pParameter);
extern void DisplayDevDmaIntIsr(void);
extern void DisplayDev_SetFrameMode(uint16 mode);
extern void DisplayDev_WriteRAM_Prepare(void);
extern void DisplayDev_ClrSrc(void);
extern rk_err_t DisplayDev_SetCurrentBufferFrameIndex(UINT16 index);
extern UINT16 DisplayDev_GetCurrentIndex(void);
extern void DisplayDev_ClrRect(uint16 color);
extern void DisplayDev_SetWindow(uint16 x0, uint16 y0, uint16 x1, int16 y1);
extern void DisplayDev_SendData( uint16 data);
extern rk_err_t DisplayDev_DMATranfer(HDC dev, uint16 *pSrc);
extern void DisplayDev_BuferTranfer(void);
extern void DisplayDev_FreshDisable(void);
extern void DisplayDev_FreshEn(void);
extern void DisplayDev_BackToUnderLayer(HDC dev);
extern void DisplayDev_CreateUpperLayer(void);
extern void DisplayDev_BufferSectionRefresh(void);
extern rk_err_t DisplayDev_CopyFrameBuffer(UINT16 src_index,UINT16 dest_index);
extern rk_err_t DisplayDev_CopyFrameBuffer(UINT16 src_index,UINT16 dest_index);
extern void DisplayDev_ShiftWindow(UINT16 frameIndex, UINT16 direction, UINT16 distance);
extern void DisplayDev_ShiftBufferFrame(UINT16 frameIndex, UINT16 direction, UINT16 distance);
extern rk_err_t DisplayDev_BufferGrayScale(UINT16 frameIndex,UINT16 level);


extern rk_err_t DisplayDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t DisplayDev_Write(HDC dev);
extern rk_err_t DisplayDev_Read(HDC dev);
extern rk_err_t DisplayDev_Delete(uint32 DevID, void * arg);
extern HDC DisplayDev_Create(uint32 DevID, void * arg);
extern void DisplayDevIntIsr(void);

#endif

