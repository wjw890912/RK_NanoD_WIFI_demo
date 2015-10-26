/*
********************************************************************************************
*
*                  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*
* FileName: ..\Gui\LCDDriver.c
* Owner: zhuzhe
* Date: 2014.4.28
* Time: 16:01:06
* Desc:
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* zhuzhe     2014.4.28     16:01:06   1.0
********************************************************************************************
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "BspConfig.h"
#ifdef __GUI_LCDDRIVER_C__
#include <typedef.h>
#include <DriverInclude.h>
#include <device.h>
#include <LCDDriver.h>
#include "UartDevice.h"
#include <DmaDevice.h>
#include <ST7735Driver.h>
#include <ST7735SDriver.h>
#include <FreeRTOS.h>
#include <RKOS.h>

//uint16 PanelBuf[128][160];      //uint16 PanelBuf[64][128];


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define _GUI_LCDDRIVER_COMMON_  __attribute__((section("gui_lcddriver_common")))
#define _GUI_LCDDRIVER_READ_  __attribute__((section("gui_lcddriver_read")))
#define _GUI_LCDDRIVER_WRITE_ __attribute__((section("gui_lcddriver_write")))
#define _GUI_LCDDRIVER_INIT_  __attribute__((section("gui_lcddriver_init")))

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

#ifdef __GUI_ST7735SDRIVER_C__
RK_LCD_DRIVER *pLCDriver = &ST7735SDriver;
#endif
#ifdef __GUI_ST7735DRIVER_C__
RK_LCD_DRIVER *pLCDriver = &ST7735Driver;
#endif

pSemaphore  LCDDmaSem;   //only for these ,one task use it,test


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
uint32 LcdIsr(void);
rk_err_t LcdShellBspReadSize(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspSetSize(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspReadTiming(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspSetTiming(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspReadStatus(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspReadVersion(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspReadMode(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspSetMode(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspReadData(HDC dev, uint8 * pstr);
rk_err_t LcdShellBspTestSpeed(HDC dev,  uint8 * pstr);
rk_err_t LcdShellBspSendData(HDC dev,  uint8 * pstr);
rk_err_t LcdShellBspSendCmd(HDC dev,  uint8 * pstr);
rk_err_t LcdShellBspSetSpeed(HDC dev,  uint8 * pstr);
rk_err_t LcdShellBspHelp(HDC dev,  uint8 * pstr);
rk_err_t LcdShellBsp(HDC dev,  uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: LcdSetWindow
** Input:uint32 x0, uint32 y0,uint32 x1, uint32 y1
** Return: rk_err_t
** Owner:wjr
** Date: 2014.11.25
** Time: 11:11:26
*******************************************************************************/
_GUI_LCDDRIVER_READ_
COMMON API rk_err_t LcdSetWindow(uint32 x0, uint32 y0,uint32 x1, uint32 y1)
{
    if(pLCDriver != NULL)
    {
        pLCDriver->LcdSetWindow(x0,y0,x1,y1);
    }
}
/*******************************************************************************
** Name: LcdInit
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.14
** Time: 15:27:57
*******************************************************************************/
_GUI_LCDDRIVER_READ_
void LcdInit(void)
{
    if(pLCDriver != NULL)
    pLCDriver->LcdInit();
}
/*******************************************************************************
** Name: LcdDmaCallBack
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.13
** Time: 16:18:05
*******************************************************************************/
_GUI_LCDDRIVER_READ_
void LcdDmaCallBack(void)
{
    //only for test
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    if(LCDDmaSem != NULL)
        rkos_semaphore_give_fromisr(LCDDmaSem);
}

/*******************************************************************************
** Name: LcdIsr
** Input:void
** Return: void
** Owner:WJR
** Date: 2014.11.25
** Time: 19:50:02
*******************************************************************************/
_GUI_LCDDRIVER_READ_
COMMON API uint32 LcdIsr(void)
{
    return VopGetInt(0);
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: LcdGetInfo
** Input:uint32 *pinfo
** Return: void
** Owner:cjh
** Date: 2015.6.9
** Time: 20:01:07
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LcdGetParameter(uint32 *pParameter)
{
    VopReadData(0, pParameter);
}

/*******************************************************************************
** Name: Lcd_WriteRAM_Prepare
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 14:38:37
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void Lcd_WriteRAM_Prepare(void)
{
    pLCDriver->LcdWriteRAMPrepare();
}

/*******************************************************************************
** Name: LCDClrRect
** Input:int x0, int y0, int x1, int y1
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:23:53
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LCDClrRect(int x0, int y0, int x1, int y1, uint16 color)
{
    int i,j;

    LcdSetWindow(x0, y0, x1, y1);

    for (i = y0; i <= y1; i++)
    {
        for (j = x0; j <= x1; j++)
        {
            VopSendData(0, color);
        }
    }
}

/*******************************************************************************
** Name: LcdClear
** Input:uint16 color
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 14:55:59
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LcdClear(uint16 color)
{
    pLCDriver->LcdClear(color);
}

/*******************************************************************************
** Name: LcdSetPixel
** Input:UINT16 x, UINT16 y, UINT16 data
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:02:21
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LcdSetPixel(UINT16 x, UINT16 y, UINT16 data)
{

    LcdSetWindow(x, y, x, y);
    VopSendData(0, data);
}

/*******************************************************************************
** Name: LcdImage
** Input:uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint16 *pSrc
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 14:58:13
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LcdImage(uint8 x0,uint8 y0,uint8 x1,uint8 y1,uint16 *pSrc)
{
    pLCDriver->LcdImage(x0, y0, x1, y1, pSrc);
}

/*******************************************************************************
** Name: LcdDmaIsr
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.5
** Time: 14:15:10
*******************************************************************************/
_GUI_LCDDRIVER_COMMON_
COMMON API void LcdDmaIsr(void)
{
    Lcd_SplitMode(LCD_SPLIT2);
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name:  LCD_ScreenUpdateDma
** Input:void* PanelBuf,pFunc LCDCallBack
** Return: void
** Owner:zhuzhe
** Date: 2014.5.13
** Time: 17:55:13
*******************************************************************************/
_GUI_LCDDRIVER_WRITE_
void  LCD_ScreenUpdateDma(void* PanelBuf,HDC DmaDev,pFunc LCDCallBack,uint32 channel)
{
    //if(pLCDriver != NULL)
    //    pLCDriver->LcdScreenUpdateDma(PanelBuf,DmaDev,LCDCallBack,channel);
    //ST7735_ScreenUpdateDma(PanelBuf, DmaDev, LCDCallBack, channel);
}
/*******************************************************************************
** Name: LCDScreenUpdate
** Input:void* PanelBuf
** Return: void
** Owner:zhuzhe
** Date: 2014.5.13
** Time: 17:28:42
*******************************************************************************/
_GUI_LCDDRIVER_WRITE_
void LCD_ScreenUpdate(uint16 * PanelBuf)
{
    uint32 i, j;

    if (pLCDriver != NULL)
    {
        LcdSetWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
        //ST7735S_SetWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
        for (i = 0; i < LCD_HEIGHT; i++)
        {
            for(j = 0; j <(LCD_WIDTH);j++)
              {
                    VopSendData(0, PanelBuf[i * LCD_WIDTH + j]);
              }
        }
    }
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
** Name: LcdControlInit
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.5.13
** Time: 15:30:05
*******************************************************************************/
_GUI_LCDDRIVER_INIT_
void LcdControlInit(void)
{
    LcdInit();
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#ifdef _LCD_SHELL_
static uint8* ShellLcdName[] =
{
    "pcb",
    "open",
    "test",
    "help",
    "bsp",
    "\b",                          // the end
};

#ifdef SHELL_BSP
static uint8* ShellLcdBspName[] =
{
    "help",
    "setspeed",
    "sendcmd",
    "senddata",
    "readdata"
    //"testspeed",
    "setmode",
    "readmode",
    "readvsion",
    "readstatus",
    "settiming",
    "readtiming",
    "setlcdsize",
    "getlcdsize",
    "\b",
};
#endif
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#ifdef SHELL_BSP
/*******************************************************************************
** Name: LcdShellBspReadSize
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:32:13
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadSize(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readtimg : 获取LCD显示的宽高大小。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: LcdShellBspSetSize
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:31:55
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSetSize(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.setsize : 设置LCD显示的宽高大小。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspReadTiming
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:31:37
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadTiming(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readtimg : 读取VOP 时序寄存器值。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: LcdShellBspSetTiming
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:31:21
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSetTiming(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.settiming : 配置VOP 时序寄存器。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspReadStatus
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:31:02
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadStatus(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readstatus : 获取VOP工作状态。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspReadVersion
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:30:20
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadVersion(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readversion : 获取VOP(video output process)版本信息。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspReadMode
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:30:04
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadMode(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readmode : 读取LCDC 工作模式。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspSetMode
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:29:38
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSetMode(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.setmode : 配置LCDC 工作模式。\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: LcdShellBspReadData
** Input:HDC dev, uint8 * pstr
** Return: rk
** Owner:chad.Ma
** Date: 2014.11.11
** Time: 16:28:20
*******************************************************************************/
//_GUI_LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspReadData(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.readdata : 读取数据.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspTestSpeed
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:39:21
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspTestSpeed(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.testspeed : 测试最大传输频率.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: LcdShellBspSendData
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:38:42
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSendData(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.senddata : 想lcd发送数据.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellBspSendCmd
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:37:48
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSendCmd(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.sendcmd c: 发送命令，c为发送的命令参数.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: LcdShellBspSetSpeed
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:37:04
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspSetSpeed(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.bsp.setspeed : 设置传输频率.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: LcdShellBspHelp
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_T
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:31:01
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBspHelp(HDC dev,  uint8 * pstr)
{
     pstr--;

    if(StrLenA(pstr) != 0)
        return RK_ERROR;

    printf("lcd.bsp命令集提供了一系列的命令对lcd板级驱动接口进行测试\r\n");
    printf("测试命令如下:\r\n");
    printf("setspeed s       设置LCD的时钟频率 s，s为设置的时钟频率\r\n");
    printf("sendcmd c        发送命令 c ，c为命令\r\n");
    printf("senddata d       发送数据 d ，d为数据\r\n");
    printf("testspeed        设置LCD的时钟频率，在数据不出错的情况下，验证可达到多少\r\n");

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: LcdShellBsp
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.10
** Time: 15:23:55
*******************************************************************************/
//__LCDDRIVER_SHELL_
SHELL FUN rk_err_t LcdShellBsp(HDC dev,  uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr, &pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellLcdBspName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                          //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //bsp help
            #ifdef SHELL_HELP
            ret = LcdShellBspHelp(dev,pItem);
            #endif

            break;

        case 0x01:  //setspeed
            ret = LcdShellBspSetSpeed(dev,pItem);
            break;

        case 0x02:  //send command
            ret = LcdShellBspSendCmd(dev,pItem);
            break;

        case 0x03:  //send data
            ret = LcdShellBspSendData(dev,pItem);
            break;

        case 0x04:  //read data
            ret = LcdShellBspReadData(dev,pItem);
            break;

        case 0x05:  //set mode
            ret = LcdShellBspSetMode(dev,pItem);
            break;

        case 0x06:  //read mode
            ret = LcdShellBspReadMode(dev,pItem);
            break;

        case 0x07:  //read version
            ret = LcdShellBspReadVersion(dev,pItem);
            break;

        case 0x08:  //read status
            ret = LcdShellBspReadStatus(dev,pItem);
            break;

        case 0x09:  //set timing
            ret = LcdShellBspSetTiming(dev,pItem);
            break;

        case 0x0a:  //read timing
            ret = LcdShellBspReadTiming(dev,pItem);
            break;

        case 0x0b:  //set lcd size
            ret = LcdShellBspSetSize(dev,pItem);
            break;

        case 0x0c:  //read lcd size
            ret = LcdShellBspReadSize(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}
#endif

/*******************************************************************************
** Name: EmmcShellPcb
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t LcdShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.pcb : lcd pcb info cmd .  \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}


extern char pSrc[];

/*******************************************************************************
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t LcdShellOpen(HDC dev,  uint8 * pstr)
{
    uint16 i,j,n;
    uint16 color =0x0000;
    uint32 channel = 0;
    uint32 ret;
    HDC DmaDev;
    pFunc pCallBack;
    uint16 frm_cnt = 0;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.open : lcd open cmd   \r\n");
            return RK_SUCCESS;
        }
    }
#endif
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
    DisplayDevHwInit(0,0);
#endif
    //pLCDriver = &ST7735Driver;

    LcdControlInit();

    //VopSetTiming(0,5,5,5);
    VopSetStart(0, 0);
    for (j = 0;j<LCD_HEIGHT;j++)
    {
        for (i =0;i<LCD_WIDTH;i++)
        {
            //PanelBuf[j][i] = i + j;//((uint16 *)pSrc)[j * 160 + i];
        }
    }

    //LCD_ScreenUpdate((uint16 *)PanelBuf);

    VopIntClear(0);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t LcdShellTest(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("lcd.test : lcd test cmd   \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShellHelp
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t LcdShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if ( StrLenA( pstr) != 0)
        return RK_ERROR;

    printf("lcd命令集提供了一系列的命令对lcd进行操作\r\n");
    printf("lcd包含的子命令如下:           \r\n");
    printf("pcb       显示pcb信息         \r\n");
    printf("open      打开firmware        \r\n");
    printf("test      测试firmware命令    \r\n");
    printf("help      显示lcd命令帮助信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: LcdShell
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t LcdShell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellLcdName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item                     //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = LcdShellPcb(dev,pItem);
            break;

        case 0x01:  //open
            ret = LcdShellOpen(dev,pItem);
            break;

        case 0x02:  //test
            ret = LcdShellTest(dev,pItem);
            break;

        case 0x03:  //help
            ret = LcdShellHelp(dev,pItem);
            break;

        case 0x04:  //bsp
#ifdef SHELL_BSP
            ret = LcdShellBsp(dev,pItem);
#endif
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}
#endif

#endif


