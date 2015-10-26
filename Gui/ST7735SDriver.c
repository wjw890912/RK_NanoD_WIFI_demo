/*
********************************************************************************
*                   Copyright (c) 2015,Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* File Name£º   ST7735.c
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2015-1-15          1.0
*    desc:    ORG.
********************************************************************************
*/
#include "BspConfig.h"
#ifdef __GUI_ST7735SDRIVER_C__

//#define _IN_Lcd_

#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "ST7735SDriver.h"


#define _ATTR_LCDDRIVER_ST7735S_CODE_  __attribute__((section("ST7735SDriverCode")))
#define _ATTR_LCDDRIVER_ST7735S_DATA_  __attribute__((section("ST7735SDriverData")))
#define _ATTR_LCDDRIVER_ST7735S_BSS_   __attribute__((section("ST7735SDriverBss"), zero_init))


RK_LCD_DRIVER ST7735SDriver =
{
    2,
    LCD_WIDTH,//width
    LCD_HEIGHT,//height
    ST7735S_Init,
    ST7735S_SetWindow,
    ST7735S_WriteRAM_Prepare,
    ST7735S_WakeUp,
    ST7735S_Standby,
    ST7735S_Clear,
    ST7735S_Image,

    NULL,
};

/*
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
*/

_ATTR_LCDDRIVER_ST7735S_BSS_ static UINT16 DmaTranferCallback_t;

_ATTR_LCDDRIVER_ST7735S_CODE_
void DMATranferCallBack_S(void)
{
    DmaTranferCallback_t = 1;
}
_ATTR_LCDDRIVER_ST7735S_CODE_
void ST7735S_Standby(void)
{
    VopSetSplit(0, VOP_CON_SPLIT_ONE);

    VopSendCmd(0,0x28);
    VopSendCmd(0,0x10);
    DelayMs(120);

    VopSetSplit(0, VOP_CON_SPLIT_TWO);
}

/*
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
void ST7735S_WakeUp(void)
{
    ST7735S_Init();
}
/*
--------------------------------------------------------------------------------
  Function name :  void ST7735_WriteReg(UINT16 addr, UINT16 data)
  Author        : yangwenjie
  Description   : send command and data to LCD.

  Input         : data -- LCD data
                  addr LCD command register address.

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
 void ST7735S_WriteReg(UINT16 addr, UINT16 data)
 {
     VopSendCmd(0,addr);
     VopSendData (0, data);
 }
/*
--------------------------------------------------------------------------------
  Function name : void ST7735_Image(UINT8 x0,UINT8 y0,UINT8 x1,UINT8 y1,UINT32 *src)
  Author        : yangwenjie
  Description   : display the pictrue in specified

  Input         : x0,y0: the start coordinate of display pictrue.
                  x1,y1: the end coordinate of display pictrue.
                  pSrc£º the source address.

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
 void ST7735S_Image(UINT8 x0,UINT8 y0,UINT8 x1,UINT8 y1,UINT16 *pSrc)
 {
    int i,j;
    int Xpos,Ypos;
    //if(x0<0) x0 = 0;
    //if(y0<0) y0 = 0;
    if(x1>LCD_WIDTH-1) x1 = LCD_WIDTH-1;
    if(y1>LCD_HEIGHT-1) y1 = LCD_HEIGHT-1;

    Xpos = x0;
    Ypos = y0;

    ST7735S_SetWindow(x0,y0, x1,y1);

    VopSetSplit(0, VOP_CON_SPLIT_ONE);
    for(j = y0; j < y1+1; j++)
    {
        for(i = x0; i < x1+1; i++)
        {
            VopSendData (0, *pSrc++);
        }
        VopSetSplit(0, VOP_CON_SPLIT_TWO);
        Ypos++;
        ST7735S_SetWindow(Xpos, Ypos,x1,y1);
    }
}
 /*
--------------------------------------------------------------------------------
  Function name : void ST7735_Clear(UINT16 color)
  Author        : yangwenjie
  Description   : clear the screen

  Input         : color£ºclear lcd to the color.

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
 void ST7735S_Clear(UINT16 color)
 {
     int i,j,n;

      ST7735S_SetWindow(0,0,LCD_WIDTH -1, LCD_HEIGHT -1);

     for(i=0;i<LCD_WIDTH;i++)
     {
         n=0;
         for(j = 0; j <LCD_HEIGHT;j++)
         {
             VopSendData (0, color);

         }
     }
 }
/*
--------------------------------------------------------------------------------
  Function name : void ST7735_SetCursor(UINT16 x0,UINT16 y0,UINT16 x1,INT16 y1)
  Author        : yangwenjie
  Description   : set display area coordinate

  Input         : x0,y0: the start coordinate of display pictrue.
                  x1,y1: the end coordinate of display pictrue.
  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
 void ST7735S_SetWindow(UINT16 x0,UINT16 y0,UINT16 x1,INT16 y1)
 {
    //VopSetSplit(0,VOP_CON_SPLIT_ONE);
    VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x0<<3) | (0x0<<1));
    VopSendCmd(0,0x2a);

    #if(LCD_WIDTH == 160)
    VopSendData(0,0x00);
    VopSendData(0,x0);

    VopSendData(0,0x00);
    VopSendData(0,x1);
    #else
    VopSendData(0,0x00);
    VopSendData(0,x0);

    VopSendData(0,0x00);
    VopSendData(0,x1);
    #endif

    VopSendCmd(0,0x2b);

    #if(LCD_WIDTH == 160)
    VopSendData(0,0x00);
    VopSendData(0,y0);

    VopSendData(0,0x00);
    VopSendData(0,y1);
    #else
    VopSendData(0,0x00);
    VopSendData(0,y0);

    VopSendData(0,0x00);
    VopSendData(0,y1);
    #endif

    VopSendCmd(0,0x2c);

    //VopSetSplit(0,VOP_CON_SPLIT_TWO);
    VopSetMode(0, (0x0<<12) | (0<<11) | (0x0<<8) | (0x0<<3) | (0x1<<1));

 }
 /*
--------------------------------------------------------------------------------
  Function name : void ST7735_WriteRAM_Prepare(void)
  Author        : yangwenjie
  Description   : to enable LCD data transfer.

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
  void ST7735S_WriteRAM_Prepare(void)
 {
    VopSetSplit(0,VOP_CON_SPLIT_ONE);
    VopSendCmd(0,0x2c);
    VopSetSplit(0,VOP_CON_SPLIT_TWO);
 }
  /*
--------------------------------------------------------------------------------
  Function name :  void ST7735_Init(void)
  Author        : yangwenjie
  Description   : LCD initialization.

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-1¡ª15         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
_ATTR_LCDDRIVER_ST7735S_CODE_
 void ST7735S_Init(void)
 {
    printf("ST7735S_Init");
    //Lcd_SplitMode(LCD_SPLIT1);
    VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x1<<3) | (0x0<<1));
    VopSendCmd(0,0x11); //Sleep out
    DelayMs (120); //Delay 120ms
    //------------------------------------ST7735S Frame Rate-----------------------------------------//
    VopSendCmd(0,0xB1);
    VopSendData(0,0x05);
    VopSendData(0,0x3C);
    VopSendData(0,0x3C);

    VopSendCmd(0,0xB2);
    VopSendData(0,0x05);
    VopSendData(0,0x3C);
    VopSendData(0,0x3C);

    VopSendCmd(0,0xB3);
    VopSendData(0,0x05);
    VopSendData(0,0x3C);
    VopSendData(0,0x3C);
    VopSendData(0,0x05);
    VopSendData(0,0x3C);
    VopSendData(0,0x3C);
    //------------------------------------End ST7735S Frame Rate-----------------------------------------//
    VopSendCmd(0,0xB4); //Dot inversion
    VopSendData(0,0x03);
    //------------------------------------ST7735S Power Sequence-----------------------------------------//
    VopSendCmd(0,0xC0);
    VopSendData(0,0x28);
    VopSendData(0,0x08);
    VopSendData(0,0x04);

    VopSendCmd(0,0xC1);
    VopSendData(0,0XC0);

    VopSendCmd(0,0xC2);
    VopSendData(0,0x0D);
    VopSendData(0,0x00);

    VopSendCmd(0,0xC3);
    VopSendData(0,0x8D);
    VopSendData(0,0x2A);

    VopSendCmd(0,0xC4);
    VopSendData(0,0x8D);
    VopSendData(0,0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    VopSendCmd(0,0xC5); //VCOM
    VopSendData(0,0x0A);//ST7735_Data(0x1A);

    VopSendCmd(0,0x36); //MX, MY, RGB mode
    #if(LCD_WIDTH == 128)
    VopSendData(0,0xc8);
    #else
    VopSendData(0,0x68);  // for 160 * 128
    #endif

    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    VopSendCmd(0,0xE0);
    VopSendData(0,0x04);
    VopSendData(0,0x22);
    VopSendData(0,0x07);
    VopSendData(0,0x0A);
    VopSendData(0,0x2E);
    VopSendData(0,0x30);
    VopSendData(0,0x25);
    VopSendData(0,0x2A);
    VopSendData(0,0x28);
    VopSendData(0,0x26);
    VopSendData(0,0x2E);
    VopSendData(0,0x3A);
    VopSendData(0,0x00);
    VopSendData(0,0x01);
    VopSendData(0,0x03);
    VopSendData(0,0x13);

    VopSendCmd(0,0xE1);
    VopSendData(0,0x04);
    VopSendData(0,0x16);
    VopSendData(0,0x06);
    VopSendData(0,0x0D);
    VopSendData(0,0x2D);
    VopSendData(0,0x26);
    VopSendData(0,0x23);
    VopSendData(0,0x27);
    VopSendData(0,0x27);
    VopSendData(0,0x25);
    VopSendData(0,0x2D);
    VopSendData(0,0x3B);
    VopSendData(0,0x00);
    VopSendData(0,0x01);
    VopSendData(0,0x04);
    VopSendData(0,0x13);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    VopSendCmd(0,0x3A); //65k mode
    VopSendData(0,0x05);

    VopSendCmd(0,0x29); //Display on

    #if 1
    VopSetSplit(0,VOP_CON_SPLIT_TWO);   // 1
    #else
    VopSetMode(0, (0x0<<12) | (0<<11) | (0x1<<8) | (0x0<<3) | (0x1<<1));
    #endif

 }



/*
********************************************************************************
*
*                         End of Lcd.c
*
********************************************************************************
*/
#endif
