/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Gui\ST7735Driver.c
* Owner: zhuzhe
* Date: 2014.5.13
* Time: 16:51:46
* Desc:
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* zhuzhe     2014.5.13     16:51:46   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __GUI_ST7735DRIVER_C__
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "ST7735Driver.h"

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

#define _GUI_ST7735DRIVER_READ_  __attribute__((section("gui_st7735driver_read")))
#define _GUI_ST7735DRIVER_WRITE_ __attribute__((section("gui_st7735driver_write")))
#define _GUI_ST7735DRIVER_INIT_  __attribute__((section("gui_st7735driver_init")))

 _GUI_ST7735DRIVER_READ_
RK_LCD_DRIVER ST7735Driver =
{
    2,
    LCD_WIDTH,//width
    LCD_HEIGHT,//height
    ST7735_Init,
    ST7735_SetWindow,
    NULL,
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name:  ST7637_Standby
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 17:15:08
*******************************************************************************/
_GUI_ST7735DRIVER_READ_
void  ST7735_Standby(void)
{
    VopSetSplit(0, VOP_CON_SPLIT_ONE);

    VopSendCmd(0, 0x28);
    VopSendCmd(0, 0x10);
    DelayMs(120);

    VopSetSplit(0, VOP_CON_SPLIT_TWO);
}

/*******************************************************************************
** Name: ST7637_WakeUp
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 17:10:19
*******************************************************************************/
_GUI_ST7735DRIVER_READ_
void ST7735_WakeUp(void)
{
    ST7735_Init();
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name:  ST7735_WriteReg
** Input:UINT16 addr, UINT16 data
** Return: void
** Owner:zhuzhe
** Date: 2014.5.13
** Time: 10:55:23
*******************************************************************************/
_GUI_ST7735DRIVER_READ_
void  ST7735_WriteReg(UINT16 addr, UINT16 data)
{
     VopSendCmd(0,addr);
     VopSendData(0,data);
}




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: ST7637_Image
** Input:uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint16 *pSrc
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 17:41:27
*******************************************************************************/
_GUI_ST7735DRIVER_WRITE_
void ST7735_Image(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint16 *pSrc)
{
    int i,j;
    int Xpos,Ypos;
    //if(x0<0) x0 = 0;
    //if(y0<0) y0 = 0;
    if(x1>LCD_WIDTH-1) x1 = LCD_WIDTH-1;
    if(y1>LCD_HEIGHT-1) y1 = LCD_HEIGHT-1;

    Xpos = x0;
    Ypos = y0;

    ST7735_SetWindow(x0,y0, x1,y1);

    VopSetSplit(0, VOP_CON_SPLIT_ONE);
    for(j = y0; j < y1+1; j++)
    {
        for(i = x0; i < x1+1; i++)
        {
            VopSendData (0, *pSrc++);
        }
        VopSetSplit(0, VOP_CON_SPLIT_TWO);
        Ypos++;
        ST7735_SetWindow(Xpos, Ypos,x1,y1);
    }
}
/*******************************************************************************
** Name: ST7637_Clear
** Input:uint16 color
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 17:40:45
*******************************************************************************/
_GUI_ST7735DRIVER_WRITE_
void ST7735_Clear(uint16 color)
{
    int i,j,n;

 	ST7735_SetWindow(0,0,128-1,128-1);

	 for(i=0;i<128;i++)
     {
	     n=0;
         for(j = 0; j <128;j++)
         {
             VopSendData (0, color);

         }
     }
}
/*******************************************************************************
** Name: ST7637_SetWindow
** Input:uint16 x0,uint16 y0,uint16 x1,int16 y1
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 17:39:09
*******************************************************************************/
_GUI_ST7735DRIVER_WRITE_
#if LCD_WIDTH == 160
_GUI_ST7735DRIVER_WRITE_
void ST7735_SetWindow(uint16 x0,uint16 y0,uint16 x1,int16 y1)
{
    //VopSetSplit(0,VOP_CON_SPLIT_ONE);
    VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x0<<3) | (0x0<<1));
	VopSendCmd(0,0x2a);

    #if(LCD_WIDTH == 160)
	VopSendData(0,0x00);
	VopSendData(0,x0+1);

	VopSendData(0,0x00);
	VopSendData(0,x1+1);
    #else
    VopSendData(0,0x00);
	VopSendData(0,x0+2);

	VopSendData(0,0x00);
	VopSendData(0,x1+2);
    #endif

	VopSendCmd(0,0x2b);

    #if(LCD_WIDTH == 160)
	VopSendData(0,0x00);
	VopSendData(0,y0+2);

	VopSendData(0,0x00);
	VopSendData(0,y1+2);
    #else
    VopSendData(0,0x00);
	VopSendData(0,y0+1);

	VopSendData(0,0x00);
	VopSendData(0,y1+1);
    #endif

	VopSendCmd(0,0x2c);

    //VopSetSplit(0,VOP_CON_SPLIT_TWO);
   VopSetMode(0, (0x0<<12) | (0<<11) | (0x0<<8) | (0x0<<3) | (0x1<<1));

}
/*******************************************************************************
** Name:  ST7637_WriteRAM_Prepare
** Input:void
** Return: void'
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 16:55:37
*******************************************************************************/
_GUI_ST7735DRIVER_WRITE_
void  ST7735_WriteRAM_Prepare(void)
{
    VopSetSplit(0,VOP_CON_SPLIT_ONE);
    VopSendCmd(0,0x2c);
    VopSetSplit(0,VOP_CON_SPLIT_TWO);
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name:  ST7637_Init
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.4.28
** Time: 16:58:29
*******************************************************************************/
_GUI_ST7735DRIVER_INIT_
void  ST7735_Init(void)
{
   //    IoMuxSetLCD(LCD_MODE);
    //VopSetTiming(0,12,5,5);
    //VopSetWidthHeight(0,127,127);
    //VopSetMode(0,VOP_CON_SPLIT_ONE | VOP_CON_AUTO_CLK_DISABLE | VOP_CON_BYPASS_FIFO | VOP_CON_DATA_WIDTH_8);
    VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x0<<3) | (0x0<<1));
	VopSendCmd(0,0x11);       //sleep out
	DelayMs(120);               //-When IC is in Sleep Out or Display On mode, it is necessary to wait 120msec before
	                           //sending next command due to the download of default value of registers
                               // and the execution of self-diagnostic function.
	//Power Control//

    VopSendCmd(0,0xb6);
    VopSendData(0,0xb4);
	VopSendData(0,0xf0);

	VopSendCmd(0,0xb1);     //Set the frame frequency of the full colors normal mode.625/((RTNA x 2 + 40) x (LINE + FPA + BPA))
//	ST7735_Data(0x06);        //RTNA
    VopSendData(0,0x02);        //RTNA
	VopSendData(0,0x35);        //LINE
	VopSendData(0,0x36);        //BPA

    VopSendCmd(0,0xb2);
    VopSendData(0,0x02);        //RTNA
	VopSendData(0,0x35);        //LINE
	VopSendData(0,0x36);        //BPA

	VopSendCmd(0,0xb3);
    VopSendData(0,0x02);        //RTNA
	VopSendData(0,0x35);        //LINE
	VopSendData(0,0x36);        //BPA
	VopSendData(0,0x02);        //RTNA
	VopSendData(0,0x35);        //LINE
	VopSendData(0,0x36);        //BPA

    VopSendCmd(0,0xb4);
    VopSendData(0,0x07);

	VopSendCmd(0,0xc0);     // Power Control 1
    VopSendData(0,0xA2);        //AVDD 4.9
    VopSendData(0,0x02);        //GVDD 4.6
    VopSendData(0,0x84);        //AUTO

	VopSendCmd(0,0xc1);     // Power Control 2
    VopSendData(0,0xC5);

	VopSendCmd(0,0xc2);     // Power Control 3
//	ST7735_Data(0x01);
//	ST7735_Data(0x01);
    VopSendData(0,0x0A);
	VopSendData(0,0x00);

	VopSendCmd(0,0xc3);     // Power Control 4
//	ST7735_Data(0x02);
//	ST7735_Data(0x07);
    VopSendData(0,0x8A);
	VopSendData(0,0x2A);

	VopSendCmd(0,0xc4);     // Power Control 5
//	ST7735_Data(0x02);
//	ST7735_Data(0x04);
	VopSendData(0,0x8a);
	VopSendData(0,0xee);

	VopSendCmd(0,0xc5);
//	ST7735_Data(0x39);
//	ST7735_Data(0x42);
	VopSendData(0,0x06);

    VopSendCmd(0,0x36);
    #if(LCD_WIDTH == 128)
    VopSendData(0,0xc8);
    #else
    VopSendData(0,0x68);  // for 160 * 128
    #endif


	//Setup Gamma  Correction Characteristics Setting//
	VopSendCmd(0,0xe0);
	VopSendData(0,0x12);
	VopSendData(0,0x1c);
	VopSendData(0,0x10);
	VopSendData(0,0x18);
	VopSendData(0,0x33);
	VopSendData(0,0x2c);
	VopSendData(0,0x25);
	VopSendData(0,0x28);
	VopSendData(0,0x28);
	VopSendData(0,0x27);
	VopSendData(0,0x2f);
	VopSendData(0,0x3c);
	VopSendData(0,0x00);
	VopSendData(0,0x03);
	VopSendData(0,0x10);
	VopSendData(0,0x0e);

	VopSendCmd(0,0xe1);
	VopSendData(0,0x12);
	VopSendData(0,0x1c);
	VopSendData(0,0x10);
	VopSendData(0,0x18);
	VopSendData(0,0x2d);
	VopSendData(0,0x28);
	VopSendData(0,0x23);
	VopSendData(0,0x28);
	VopSendData(0,0x28);
	VopSendData(0,0x26);
	VopSendData(0,0x2f);
	VopSendData(0,0x3b);
	VopSendData(0,0x00);
	VopSendData(0,0x03);
	VopSendData(0,0x03);
	VopSendData(0,0x10);

    VopSendCmd(0,0x3a);
    VopSendData(0,0x05);

    VopSendCmd(0,0x2a);
    VopSendData(0,0x00);
    VopSendData(0,0x02);
    VopSendData(0,0x00);
    VopSendData(0,0x81);

    VopSendCmd(0,0x2b);
    VopSendData(0,0x00);
    VopSendData(0,0x03);
    VopSendData(0,0x00);
    VopSendData(0,0x82);

    VopSendCmd(0,0x29);
	DelayMs(120);

    VopSendCmd(0,0x2c);

    #if 0
    VopSetSplit(0,VOP_CON_SPLIT_TWO);   // 1
    #else
    VopSetMode(0, (0x0<<12) | (0<<11) | (0x0<<8) | (0x0<<3) | (0x1<<1));
    #endif
}
#endif
#if LCD_WIDTH == 240
_GUI_ST7735DRIVER_WRITE_
void ST7735_SetWindow(uint16 x0,uint16 y0,uint16 x1,int16 y1)
{
 uint8 xx1,xx2,yy1,yy2;
    VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x0<<3) | (0x0<<1));
   VopSendCmd(0,0x2a);
	xx1=x0/255;
	xx2=x0%255;
   	VopSendData(0,xx1);
	VopSendData(0,xx2);
	 xx1=x1/255;
	xx2=x1%255;
	VopSendData(0,xx1);
	VopSendData(0,xx2);
	VopSendCmd(0,0x2b);
	yy1=y0/255;
	yy2=y0%255;
    VopSendData(0,yy1);
	VopSendData(0,yy2);
	yy1=y1/255;
	yy2=y1%255;
	VopSendData(0,yy1);
	VopSendData(0,yy2);

	VopSendCmd(0,0x2c);
   VopSetMode(0, (0x0<<12) | (0<<11) | (0x0<<8) | (0x0<<3) | (0x1<<1));
 }
_GUI_ST7735DRIVER_INIT_
void  ST7735_Init(void)
{
    	uint16 i,j;
        VopSetMode(0,(0x0<<12) | (0x1<<11) | (0x0<<8) | (0x0<<3) | (0x0<<1));
	VopSendCmd(0,0x22);
	DelayMs(120);
	VopSendCmd(0,0xcf);VopSendData(0,0x00);VopSendData(0,0xa2);VopSendData(0,0xf0);VopSendData(0,0x00);
	DelayMs(120);
	VopSendCmd(0,0xed);VopSendData(0,0x67);VopSendData(0,0x03);VopSendData(0,0x12);VopSendData(0,0x81);
	DelayMs(120);
	VopSendCmd(0,0xe8);VopSendData(0,0x85);VopSendData(0,0x01);VopSendData(0,0x78);VopSendData(0,0x00);
	VopSendCmd(0,0xcb);VopSendData(0,0x39);VopSendData(0,0x2c);VopSendData(0,0x00);VopSendData(0,0x34);VopSendData(0,0x02);VopSendData(0,0x00);
	VopSendCmd(0,0xf7);VopSendData(0,0x20);VopSendData(0,0x00);
	VopSendCmd(0,0xea);VopSendData(0,0x00);VopSendData(0,0x00);
	VopSendCmd(0,0xc0);VopSendData(0,0x25);VopSendData(0,0x00);
	VopSendCmd(0,0xc1);VopSendData(0,0x10);VopSendData(0,0x00);
	VopSendCmd(0,0xc2);VopSendData(0,0x11);VopSendData(0,0x00);
	VopSendCmd(0,0xc5);VopSendData(0,0x30);VopSendData(0,0x3e);
	VopSendCmd(0,0xb1);VopSendData(0,0x00);VopSendData(0,0x16);
	VopSendCmd(0,0xb6);VopSendData(0,0x0a);VopSendData(0,0x82);
	VopSendCmd(0,0xc7);VopSendData(0,0xb5);VopSendData(0,0x00);
	VopSendCmd(0,0xf2);VopSendData(0,0x00);VopSendData(0,0x00);
	VopSendCmd(0,0xe0);
    VopSendData(0,0x1f);VopSendData(0,0x1a);VopSendData(0,0x18);VopSendData(0,0x0a);VopSendData(0,0x0f);VopSendData(0,0x06);VopSendData(0,0x45);VopSendData(0,0x87);VopSendData(0,0x32);VopSendData(0,0x0a);VopSendData(0,0x07);VopSendData(0,0x02);VopSendData(0,0x07);VopSendData(0,0x05);VopSendData(0,0x00);VopSendData(0,0x00);
	DelayMs(120);
	VopSendCmd(0,0xe1);
    VopSendData(0,0x00);VopSendData(0,0x25);VopSendData(0,0x27);VopSendData(0,0x05);VopSendData(0,0x10);VopSendData(0,0x09);VopSendData(0,0x3a);VopSendData(0,0x78);VopSendData(0,0x4d);VopSendData(0,0x05);VopSendData(0,0x18);VopSendData(0,0x0d);VopSendData(0,0x38);VopSendData(0,0x3a);VopSendData(0,0x1f);VopSendData(0,0x00);
	DelayMs(120);
	VopSendCmd(0,0x11);
	VopSendCmd(0,0x36);VopSendData(0,0x48);VopSendData(0,0x00);
	VopSendCmd(0,0x26);VopSendData(0,0x01);VopSendData(0,0x00);
	VopSendCmd(0,0x3a);VopSendData(0,0x55);VopSendData(0,0x00);
	VopSendCmd(0,0x2a);VopSendData(0,0x00);VopSendData(0,0x00);VopSendData(0,0x00);VopSendData(0,0xef);
	DelayMs(120);
	VopSendCmd(0,0x2b);VopSendData(0,0x00);VopSendData(0,0x00);VopSendData(0,0x01);VopSendData(0,0x3f);
	DelayMs(120);
	DelayMs(120);
	VopSendCmd(0,0x29);
	VopSendCmd(0,0x2c);
    VopSetSplit(0,VOP_CON_SPLIT_TWO);   // 1
}
#endif
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

