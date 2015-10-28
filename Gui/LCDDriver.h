/*
********************************************************************************************
*
*                  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*
* FileName: ..\Gui\LCDDriver.h
* Owner: zhuzhe
* Date: 2014.4.28
* Time: 16:01:13
* Desc:
* History:
*     <author>     <date>       <time>     <version>       <Desc>
* zhuzhe     2014.4.28     16:01:13   1.0
********************************************************************************************
*/

#ifndef __GUI_LCDDRIVER_H__
#define __GUI_LCDDRIVER_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define _ATTR_LCD_CODE_          __attribute__((section("SysCode")))
#define _ATTR_LCD_DATA_          __attribute__((section("SysData")))
#define _ATTR_LCD_BSS_           __attribute__((section("SysBss"), zero_init))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define LCD_WIDTH                       160//240
#define LCD_HEIGHT                       128//320

//every lcd driver must be packed to these struct
/*Because the number of bytes occupied pixels undecided, so the incoming pointer of type void * */
typedef __packed struct _RK_LCD_DRIVER
{
    ///* Drvier Name */
    //char* name;

    /* every pixel byte */
    uint8 pixel_byte;

    /* width and height*/
    uint16 width;
    uint16 height;

    void (*LcdInit)(void);

    void (*LcdSetWindow)(uint16,uint16,uint16,int16);
    void (*LcdWriteRAMPrepare)(void);
    void (*LcdWakeUp)(void);
    void (*LcdStandby)(void);
    void (*LcdClear)(uint16);
    void (*LcdImage)(uint8, uint8, uint8, uint8, uint16*);

    /* point the next driver*/
    void *next;

}RK_LCD_DRIVER;


typedef enum
{
    DIRECTION_UP = (uint16)0,
    DIRECTION_DOWN,
    DIRECTION_LEFT,
    DIRECTION_RIGHT,
    DIRECTION_MAX
} BUFFER_SHIFT_DIRECTION;

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
#ifdef _FRAME_BUFFER_
extern void LcdGetParameter(uint32 *pParameter);
extern void LCDClrRect(int x0, int y0, int x1, int y1, uint16 color);
extern void LcdClear(uint16 color);
extern void LcdSetPixel(UINT16 x, UINT16 y, UINT16 data);
extern void LcdImage(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint16 *pSrc);
extern uint32 LcdIsr(void);
extern rk_err_t LcdSetWindow(uint32 x0, uint32 y0,uint32 x1, uint32 y1);
extern void  LCD_ScreenUpdateDma(void* PanelBuf,HDC DmaDev,pFunc LCDCallBack,uint32 channel);
extern void LCD_ScreenUpdate(uint16* PanelBuf);
extern void LcdDmaCallBack(void);
extern void LcdControlInit(void);
extern void LcdDmaIsr(void);
#endif

#ifdef _USE_SHELL_
extern rk_err_t LcdShellPcb(HDC dev,  uint8 * pstr);
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
extern rk_err_t LcdShellOpen(HDC dev,  uint8 * pstr);
#endif
extern rk_err_t LcdShellTest(HDC dev,  uint8 * pstr);
extern rk_err_t LcdShell(HDC dev, uint8 * pstr);
#endif

#endif


