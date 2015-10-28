/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\Display\DisplayDevice.c
* Owner: aaron.sun
* Date: 2015.5.29
* Time: 10:20:23
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.5.29     10:20:23   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define DISPLAY_DEV_NUM  3

#ifdef _FRAME_BUFFER_
typedef struct RectInfoStruct
{
    UINT16  x0;
    UINT16  y0;
    UINT16  x1;
    UINT16  y1;
    UINT16  x;
    UINT16  y;
} RECT_INFO_STRUCT;

typedef struct _FrameParameter
{
    //uint32 FrameIndex;
    UINT16 frameBuffer[LCD_HEIGHT][LCD_WIDTH];
} FrameParameter;

typedef  struct _DISPLAY_DEVICE_CLASS
{
    DEVICE_CLASS stDisplayDevice;
    xSemaphoreHandle osDisplayReadReqSem;
    xSemaphoreHandle osDisplayReadSem;
    xSemaphoreHandle osDisplayWriteReqSem;
    xSemaphoreHandle osDisplayWriteSem;
    xSemaphoreHandle osDisplayOperReqSem;
    xSemaphoreHandle osDisplayOperSem;
    RECT_INFO_STRUCT stRectInfo;
    FrameParameter *FrameData;
    uint32 dmaCh;
    HDC hDma;
    uint32 FrameIndex;

}DISPLAY_DEVICE_CLASS;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static DISPLAY_DEVICE_CLASS * gpstDisplayDevISR[DISPLAY_DEV_NUM] = {(DISPLAY_DEVICE_CLASS *)NULL, (DISPLAY_DEVICE_CLASS *)NULL, (DISPLAY_DEVICE_CLASS *)NULL};


_ATTR_LCD_DATA_
__align(4)
//static UINT16 gpstFrameBuffer[LCD_MAX_YSIZE][LCD_MAX_XSIZE];
/*static*/
//_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
//UINT16 gpstFrameBuffer[BUFFER_MAX_NUM][LCD_HEIGHT][LCD_WIDTH];

//bin: 1111 0111 1101 1110 ; RGB fmt: 11110 111110 11110
#define RGB_GRAY_SCALE_MASK_LEV1 0xF7DE

//bin: 1110 0111 1001 1100 ; RGB fmt: 11100 111100 11100
#define RGB_GRAY_SCALE_MASK_LEV2 0xE79C

//bin: 1100 0111 0001 1000 ; RGB fmt: 11000 111000 11000
#define RGB_GRAY_SCALE_MASK_LEV3 0xC718

//bin: 1000 0110 0001 0000 ; RGB fmt: 10000 110000 10000
#define RGB_GRAY_SCALE_MASK_LEV4 0x8610

//bin: 0000 0100 0000 0000 ; RGB fmt: 00000 100000 00000
#define RGB_GRAY_SCALE_MASK_LEV5 0x0400

_ATTR_LCD_BSS_ /*static*/ UINT16 CurrentFrameIndex;
_ATTR_LCD_BSS_ static UINT16 Ymin;
_ATTR_LCD_BSS_ static UINT16 Ymax;
_ATTR_LCD_BSS_ static UINT32 CrossBorder;
_ATTR_LCD_BSS_ static UINT32 Refresh;
_ATTR_LCD_BSS_ static UINT32 RefreshCnt;
_ATTR_LCD_BSS_ static UINT32 FrameMode;
_ATTR_LCD_BSS_ static RECT_INFO_STRUCT RECT;

#endif


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//Instructions can start next frame transmission
int VopIntTypeFrameDoneFlag = 0;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t DisplayDevShellTest(HDC dev, uint8 * pstr);
rk_err_t DisplayDevShellDel(HDC dev, uint8 * pstr);
rk_err_t DisplayDevShellCreate(HDC dev, uint8 * pstr);
rk_err_t DisplayDevShellPcb(HDC dev, uint8 * pstr);
rk_err_t DisplayDevDeInit(DISPLAY_DEVICE_CLASS * pstDisplayDev);
rk_err_t DisplayDevInit(DISPLAY_DEVICE_CLASS * pstDisplayDev);
rk_err_t DisplayDevResume(HDC dev);
rk_err_t DisplayDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DisplayDev_SetVopMode
** Input:uint32 RawFormat, uint32 Version
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.11
** Time: 11:03:23
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_SetVopMode(uint32 RawFormat, uint32 Version)
{
    VopSetMode(0, RawFormat|VOP_CON_SPLIT_TWO|VOP_CON_NOT_BYPASS_FIFO\
               |VOP_CON_IDLE_INPUT|VOP_CON_AUTO_CLK_DISABLE|Version);
}
/*******************************************************************************
** Name: DisplayDevGetParameter
** Input:uint32 *pParameter
** Return: void
** Owner:cjh
** Date: 2015.6.9
** Time: 20:37:38
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDevGetParameter(uint32 *pParameter)
{
    int32 lcdWidth, lcdHeight;
    VOP * pVop;

    LcdGetParameter(pParameter);
    pVop = (VOP *)pParameter;

    pVop->VopMcuLcdSize = (pVop->VopMcuLcdSize & (~0x1FF)) & (~0xFF800);

    pVop->VopMcuLcdSize |= (LCD_WIDTH << 0);
    pVop->VopMcuLcdSize |= (LCD_HEIGHT << 11);

    lcdWidth = ((pVop->VopMcuLcdSize & 0x1FF) >> 0);
    lcdHeight = ((pVop->VopMcuLcdSize & 0xFF800) >> 11);
    printf("lcdWidth= %d\n lcdHeight= %d\n",lcdWidth, lcdHeight);

}
/*******************************************************************************
** Name: DisplayDevIntIsr
** Input:uint32 DevID
** Return: void
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDevIntIsr(void)
{
    //Get DisplayDev Int type...
    //DisplayDevIntType = GetIntType();

    if (gpstDisplayDevISR[CurrentFrameIndex] != NULL)
    {
        VopIntTypeFrameDoneFlag = VopGetInt(0);
        //wirte other int service...
    }
}

/*******************************************************************************
** Name: DisplayDevDmaIntIsr
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.9
** Time: 17:08:22
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDevDmaIntIsr(void)
{
    if (gpstDisplayDevISR[CurrentFrameIndex] != NULL)
    {
        LcdDmaIsr();
        DmaDev_RealseChannel(gpstDisplayDevISR[CurrentFrameIndex]->hDma, gpstDisplayDevISR[CurrentFrameIndex]->dmaCh);
        //wirte other int service...
    }

}
/*******************************************************************************
** Name: DisplayDev_SetFrameMode
** Input:uint32 mode
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 15:53:30
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_SetFrameMode(uint16 mode)
{
    FrameMode = mode;
    CrossBorder = 1;
}
/*******************************************************************************
** Name: DisplayDev_WriteRAM_Prepare
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 15:51:22
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_WriteRAM_Prepare(void)
{
    Lcd_WriteRAM_Prepare();
}

/*******************************************************************************
** Name: DisplayDev_ClrSrc
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 14:21:42
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_ClrSrc(void)
{
    LcdClear(0x00);
}

/*******************************************************************************
** Name: DisplayDev_SetCurrentBufferFrameIndex
** Input:UINT16 index
** Return: int16
** Owner:cjh
** Date: 2015.6.4
** Time: 15:44:24
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_SetCurrentBufferFrameIndex(UINT16 index)
{

    if (index >=0 && index <=BUFFER_MAX_NUM-1)
    {

        CurrentFrameIndex=index;

        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}

/*******************************************************************************
** Name: DisplayDev_GetCurrentIndex
** Input:void
** Return: UINT16
** Owner:cjh
** Date: 2015.6.4
** Time: 19:18:14
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API UINT16 DisplayDev_GetCurrentIndex(void)
{
    return CurrentFrameIndex;
}

/*******************************************************************************
** Name: DisplayDev_ClrRect
** Input:int x0, int y0, int x1, int y1, uint16 color
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 14:26:11
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_ClrRect(uint16 color)
{
    DisplayDev_SetWindow(gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0, gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0,\
                         gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1, gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1);
    LCDClrRect(gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0, gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0,\
               gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1, gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1, color);
}

/*******************************************************************************
** Name: DisplayDev_SetWindow
** Input:uint16 x0,uint16 y0,uint16 x1,int16 y1
** Return: void
** Owner:cjh
** Date: 2015.6.8
** Time: 14:15:57
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_SetWindow(uint16 x0, uint16 y0, uint16 x1, int16 y1)
{

#if 0
    if (x0 < 0 || x0 > LCD_MAX_XSIZE)
    {
        printf("\ncross-border, (%d, %d), (%d, %d)\n", x0, y0, x1, y1);
        CrossBorder = 1;
        return;
    }
    else if (x1 < 0 || x1 > LCD_MAX_XSIZE)
    {
        printf("\ncross-border, (%d, %d), (%d, %d)\n", x0, y0, x1, y1);
        CrossBorder = 1;
        return;
    }
    else if (y0 < 0 || y0 > LCD_MAX_YSIZE)
    {
        printf("\ncross-border, (%d, %d), (%d, %d)\n", x0, y0, x1, y1);
        CrossBorder = 1;
        return;
    }
    else if (y1 < 0 || y1 > LCD_MAX_YSIZE)
    {
        printf("\nCross Border, (%d, %d), (%d, %d)\n", x0, y0, x1, y1);
        CrossBorder = 1;
        return;
    }
    else
    {
        CrossBorder = 0;
    }
#endif

    if (FrameMode == 0)
    {
        RECT.x0 = x0;
        RECT.x1 = (x1 > (LCD_WIDTH - 1))? (LCD_HEIGHT - 1):x1;
        RECT.y0 = y0;
        RECT.y1 = (y1 > (LCD_WIDTH - 1))? (LCD_HEIGHT - 1):y1;
        RECT.x  = RECT.x0;
        RECT.y  = RECT.y0;


    }
    else
    {
        RECT.x0 = (y1 > (LCD_WIDTH - 1)) ? 0 : (LCD_WIDTH - y1 - 1);
        RECT.x1 = (y0 > (LCD_WIDTH - 1)) ? 0 : (LCD_WIDTH - y0 - 1);

        RECT.y0 = x0;
        RECT.y1 = (x1 > LCD_HEIGHT)? (LCD_HEIGHT - 1) : x1;
        RECT.x  = RECT.x1;
        RECT.y  = RECT.y0;
    }


    if (RECT.y1 > Ymax)
    {
        Ymax = RECT.y1;
    }

    if (RECT.y0 < Ymin)
    {
        Ymin = RECT.y0;
    }

    CrossBorder = 0;
}

/*******************************************************************************
** Name: DisplayDev_SendData
** Input:HDC dev
** Return: void
** Owner:cjh
** Date: 2015.6.5
** Time: 17:02:10
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_SendData( UINT16 data)
{
    if (CrossBorder == 1)
        return;

    Refresh = 1;

    if (FrameMode == 0)
    {
//        if (RECT.x % 2) /*奇数存偶数位*/
//             gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x - 1] = data;
//        else    /*偶数存奇数位*/
//             gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x + 1] = data;

        gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x] = data; //+ 1

        RECT.x++;

        if (RECT.x > RECT.x1)
        {
            RECT.x = RECT.x0;
            RECT.y++;
            if (RECT.y > RECT.y1)
            {
                RECT.x = RECT.x0;
                RECT.y = RECT.y0;
            }
        }
    }
    else
    {
//        if (RECT.x % 2) /*奇数存偶数位*/
//             gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x - 1] = data;
//        else    /*偶数存奇数位*/
//             gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x + 1] = data;
        gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[RECT.y][RECT.x] = data; //+1

        RECT.y++;

        if (RECT.y > RECT.y1)
        {
            RECT.y = RECT.y0;

            if (RECT.x <= RECT.x0)
            {
                RECT.x = RECT.x1;
                RECT.y = RECT.y0;
            }
            else
            {
                RECT.x--;
            }
        }
    }
}

/*******************************************************************************
** Name: DisplayDev_DMATranfer
** Input:UINT8 x0, UINT8 y0, UINT8 x1, UINT8 y1, UINT16 *pSrc
** Return: UINT16
** Owner:cjh
** Date: 2015.6.5
** Time: 14:03:03
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_DMATranfer(HDC dev, uint16 *pSrc)
{
    UINT32 ret, Size;
    eDMA_CHN channel;
    DISPLAY_DEVICE_CLASS * pstDisplayDev = (DISPLAY_DEVICE_CLASS *)dev;
    VOP * pVop = VopPort(0);
    uint32 rgbDstAddr =(uint32)&pVop->VopMcuData ;

    //DMA_CFGX DmaCfg = {DMA_CTLL_M2LCD_HALF, DMA_CFGL_M2LCD,DMA_CFGH_M2LCD,0};
    DMA_CFGX DmaCfg = {DMA_CTLL_VOP_WR, DMA_CFGL_VOP_WR, DMA_CFGH_VOP_WR, 0};
    //rkos_semaphore_take(pstDisplayDev->osDisplayOperReqSem, MAX_DELAY);

    LcdSetWindow(gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0,\
                 gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0,\
                 gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1,\
                 gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1);

    channel = DmaDev_GetChannel(dev);

    if (channel != RK_ERROR)
    {
        Size = (((gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1 + 1)\
                 - gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0)\
                * ((gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1+1)\
                   - gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0)) >> 1;

        //rkos_enter_critical();
        ret = DmaDev_DmaStart(pstDisplayDev->hDma, channel,\
                              (UINT32)(&gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer\
                              [gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0][gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0]),\
                              rgbDstAddr, Size / 4, &DmaCfg, DisplayDevDmaIntIsr);//(uint32)(&(Lcd->data))

        if (ret != RK_SUCCESS)
        {
            return (rk_size_t)RK_ERROR;
        }
        //rkos_exit_critical();
        //rkos_semaphore_give(pstDisplayDev->osDisplayOperReqSem);
        return channel;
    }
    return RK_ERROR;
}


/*******************************************************************************
** Name: DisplayDev_BuferTranfer
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:10:00
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_BuferTranfer(void)
{
    int i, j;
    int ret = 0;
    HDC dev;
#if 0
    if (Ymin < 0 || Ymin > LCD_MAX_YSIZE)
    {
        if (Ymin != 255)
            printf("\nYmin cross-border: %d\n", Ymin);
        goto skip;
    }

    if ((Ymax - 1) < 0 || Ymax > LCD_MAX_YSIZE)
    {
        printf("\nYmax cross-border: %d\n", Ymax);
        goto skip;
    }

    if (Ymin >= Ymax)
    {
        printf("\nYmin >= Ymax: %d, %d\n", Ymin, Ymax);
        goto skip;
    }
#endif

    if (Refresh && (RefreshCnt == 0))
    {
        //printf("Lcd_BuferTranfer, Ymin: %d, Ymax: %d\n", Ymin, Ymax);
        //Buffer synchronization

#if 0   //单buffer
        ret = DisplayDev_DMATranfer(0, Ymin, LCD_MAX_XSIZE - 1, Ymax - 1,
                                    &gpstFrameBuffer[Ymin][0]);
#else  //three buffer
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0 = 0;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0 = Ymin;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1 = LCD_WIDTH - 1;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1 = Ymax;
        //printf("CurrentFrameIndex=%d",CurrentFrameIndex);
        dev = (HDC)gpstDisplayDevISR[CurrentFrameIndex];
        ret = DisplayDev_DMATranfer(dev, (uint16 *)&gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[Ymin][0]);
#endif

#if 0  //rk ref code
#if 1
        memcpy(&gpstFrameBuffer[FrameIndex^1][0][0],
               &gpstFrameBuffer[FrameIndex][0][0], LCD_MAX_XSIZE*LCD_MAX_YSIZE*2);
#else
        for (i = 0; i < LCD_MAX_YSIZE; i++)
            for (j = 0; j < LCD_MAX_XSIZE; j++)
                gpstFrameBuffer[FrameIndex^1][i][j] = gpstFrameBuffer[FrameIndex][i][j];
#endif

        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x0 = 0;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0 = Ymin;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.x1 = LCD_WIDTH - 1;
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1 = Ymax;
        dev = (HDC)gpstDisplayDevISR[CurrentFrameIndex];
        ret = DisplayDev_DMATranfer(dev, &gpstFrameBuffer[CurrentFrameIndex][Ymin][0]);

        FrameIndex ^= 1;
#endif
skip:
        //memset(&RECT, 0 , sizeof(RECT_INFO_STRUCT));
        Refresh = 0;
        Ymin = 0;
        Ymax = 0;

        return;
    }

    return;

}

/*******************************************************************************
** Name: DisplayDev_FreshDisable
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 19:22:00
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_FreshDisable(void)
{
    RefreshCnt = 0;
}
/*******************************************************************************
** Name: DisplayDev_FreshEn
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 19:21:30
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_FreshEn(void)
{
    RefreshCnt = 1;
}

/*******************************************************************************
** Name: DisplayDev_BackToUnderLayer
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 18:08:43
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_BackToUnderLayer(HDC dev)
{

    if (CurrentFrameIndex >= DISPLAY_DEV_NUM || CurrentFrameIndex <=0 )
    {
        return;
    }
    DisplayDev_SetCurrentBufferFrameIndex(CurrentFrameIndex-1);
    DisplayDev_BufferSectionRefresh();
}
/*******************************************************************************
** Name: DisplayDev_CreateUpperLayer
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:56:48
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_CreateUpperLayer(void)
{
    FrameParameter *frameData = NULL;
    frameData = rkos_memory_malloc(sizeof(FrameParameter));
    if (frameData == NULL)
    {
        printf ("\n malloc frameData memory failure\n");
    }
    gpstDisplayDevISR[CurrentFrameIndex + 1]->FrameData = frameData;
    memset(gpstDisplayDevISR[CurrentFrameIndex + 1]->FrameData, 0 ,\
           sizeof(gpstDisplayDevISR[CurrentFrameIndex + 1]->FrameData)/(sizeof(uint32) * 8));


    if ((CurrentFrameIndex >= DISPLAY_DEV_NUM) || (CurrentFrameIndex < 0))
    {
        return;
    }

    DisplayDev_CopyFrameBuffer(CurrentFrameIndex,CurrentFrameIndex+1);
    DisplayDev_SetCurrentBufferFrameIndex(CurrentFrameIndex+1);
    DisplayDev_BufferSectionRefresh();

}
/*******************************************************************************
** Name: DisplayDev_BufferSectionRefresh
** Input:UINT16 pYim,UINT16 pYmax
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:54:48
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_BufferSectionRefresh(void)
{
    if (gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1 > LCD_HEIGHT)
    {
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1 = LCD_HEIGHT;
    }

    if (gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0 < 0)
    {
        gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1 = 0;
    }

    Refresh = 1;
    Ymin = gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0;
    Ymax = gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1;
}

/*******************************************************************************
** Name: DisplayDev_CopyFrameBuffer
** Input:UINT16 src_index,UINT16 dest_index
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.4
** Time: 15:46:56
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_CopyFrameBuffer(UINT16 src_index,UINT16 dest_index)
{

    //must be different index
    if (src_index==dest_index)
    {
        return -1;
    }

    //every pixel corresponding 2 Bytes;  you can use LastFrameIndex as src index
    memcpy(&gpstDisplayDevISR[dest_index]->FrameData,
           &gpstDisplayDevISR[src_index]->FrameData, LCD_WIDTH*LCD_HEIGHT*2);


}

/*******************************************************************************
** Name: DisplayDev_ShiftWindow
** Input:frameIndex,direction,distance,0,0,LCD_WIDTH-1,LCD_HEIGHT-1
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:39:50
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_ShiftWindow(UINT16 frameIndex, UINT16 direction, UINT16 distance)
{
    int x,y;
    int lcd_width;
    int Xmin, Xmax;

    //get Ymax and Ymin
    LcdSetWindow(gpstDisplayDevISR[frameIndex]->stRectInfo.x0, gpstDisplayDevISR[frameIndex]->stRectInfo.y0,\
                 gpstDisplayDevISR[frameIndex]->stRectInfo.x1, gpstDisplayDevISR[frameIndex]->stRectInfo.y1);

    if (gpstDisplayDevISR[frameIndex]->stRectInfo.x1 > gpstDisplayDevISR[frameIndex]->stRectInfo.x0)
    {
        Xmax = gpstDisplayDevISR[frameIndex]->stRectInfo.x1;
        Xmin = gpstDisplayDevISR[frameIndex]->stRectInfo.x0;
    }
    else
    {

        Xmax = gpstDisplayDevISR[frameIndex]->stRectInfo.x0;
        Xmin = gpstDisplayDevISR[frameIndex]->stRectInfo.x1;
    }

    lcd_width = Xmax-Xmin;

    //printf("frameindex = %d ",frameIndex);
    //param distance hasn't been designed
    switch (direction)
    {
            //all of these for case should include the Ymax and Ymin Line
        case DIRECTION_DOWN:
            for (y = gpstDisplayDevISR[frameIndex]->stRectInfo.y1; y >= (distance + gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0) ; y--)
            {
                memcpy(&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][Xmin],&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y-distance][Xmin],lcd_width*2);
            }

            break;

        case DIRECTION_UP:
            for (y=(distance + gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y0); y <= gpstDisplayDevISR[CurrentFrameIndex]->stRectInfo.y1; y++)
            {
                memcpy(&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y-distance][Xmin],&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][Xmin],lcd_width*2);
            }

            break;

        case DIRECTION_LEFT:
            printf("DIRECTION_LEFT");
            for (y=0; y <= Ymax;y++)
            {
                //since x coordinate will exchange every 2 bit, I try shift 2 pixel as the minimum unit
                memcpy(&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][Xmin],&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][distance*2+Xmin],lcd_width*2-distance*2);
            }
            break;

        case DIRECTION_RIGHT:
            printf("DIRECTION_RIGHT");
            for (y=0; y <= Ymax;y++)
            {
                //since x coordinate will exchange every 2 bit, I try shift 2 pixel as the minimum unit
                memcpy(&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][distance*2+Xmin],&gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[y][Xmin],lcd_width*2-distance*2);
            }
            break;

        default:
            break;
    }
}


/*******************************************************************************
** Name: DisplayDev_ShiftBufferFrame
** Input:UINT16 frameIndex,BUFFER_SHIFT_DIRECTION  direction, UINT16 distance
** Return: void
** Owner:cjh
** Date: 2015.6.4
** Time: 15:36:36
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API void DisplayDev_ShiftBufferFrame(UINT16 frameIndex, UINT16 direction, UINT16 distance)
{
    DisplayDev_ShiftWindow(frameIndex,direction,distance);
}
/*******************************************************************************
** Name: DisplayDev_BufferGrayScale
** Input:UINT16 frameIndex,UINT16 level
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.4
** Time: 15:34:33
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_BufferGrayScale(UINT16 frameIndex,UINT16 level)
{

    int16 i, j;
    int32 rgbGrayScaleMask;

    switch (level)
    {
        case 1:
            rgbGrayScaleMask = RGB_GRAY_SCALE_MASK_LEV1;
            break;

        case 2:
            rgbGrayScaleMask = RGB_GRAY_SCALE_MASK_LEV2;
            break;

        case 3:
            rgbGrayScaleMask = RGB_GRAY_SCALE_MASK_LEV3;
            break;

        case 4:
            rgbGrayScaleMask = RGB_GRAY_SCALE_MASK_LEV4;
            break;

        case 5:
            rgbGrayScaleMask = RGB_GRAY_SCALE_MASK_LEV5;
            break;

        default:
            //not in the range
            return RK_ERROR;

    }

    // every pixel
    for (i=0;i<LCD_HEIGHT;i++)
    {
        for (j=0;j<LCD_WIDTH;j++)
        {
            //first use RGB_Gray_Scale_Mask to execute bit AND
            gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[i][j] &= rgbGrayScaleMask;
            //the shift right 1 bit
            gpstDisplayDevISR[frameIndex]->FrameData->frameBuffer[i][j] >>= level;
        }
    }

    return RK_SUCCESS;

    //then shift one bit
}

/*******************************************************************************
** Name: DisplayDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_Write(HDC dev)
{
    uint32 size, dmaCh;
    //DisplayDev Write...
    DISPLAY_DEVICE_CLASS * pstDisplayDev =  (DISPLAY_DEVICE_CLASS *)dev;
    if (pstDisplayDev == NULL)
    {
        return RK_ERROR;
    }

    rkos_semaphore_take(pstDisplayDev->osDisplayWriteSem, MAX_DELAY);
    rkos_enter_critical();

#if 0
    size = LCD_HEIGHT * LCD_WIDTH;

    if ((((uint32)(&gpstFrameBuffer[CurrentFrameIndex][0][0]) % 4) == 0)
            && ((size % 4) == 0))
    {
        DisplayDev_DMATranfer(pstDisplayDev, &gpstFrameBuffer[CurrentFrameIndex][0][0]);
    }
    else
    {
        LCD_ScreenUpdate(&gpstFrameBuffer[CurrentFrameIndex][0][0]);
    }
#endif
    LCD_ScreenUpdate((uint16 *)&gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[0][0]);
    rkos_exit_critical();

    rkos_semaphore_give(pstDisplayDev->osDisplayWriteSem);

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: DisplayDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON API rk_err_t DisplayDev_Read(HDC dev)
{
    //DisplayDev Read...
    DISPLAY_DEVICE_CLASS * pstDisplayDev =  (DISPLAY_DEVICE_CLASS *)dev;
    if (pstDisplayDev == NULL)
    {
        return RK_ERROR;
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*******************************************************************************
** Name: DisplayDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON FUN rk_err_t DisplayDevResume(HDC dev)
{
    //DisplayDev Resume...
    DISPLAY_DEVICE_CLASS * pstDisplayDev =  (DISPLAY_DEVICE_CLASS *)dev;
    if (pstDisplayDev == NULL)
    {
        return RK_ERROR;
    }

}
/*******************************************************************************
** Name: DisplayDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_COMMON_
COMMON FUN rk_err_t DisplayDevSuspend(HDC dev)
{
    //DisplayDev Suspend...
    DISPLAY_DEVICE_CLASS * pstDisplayDev =  (DISPLAY_DEVICE_CLASS *)dev;
    if (pstDisplayDev == NULL)
    {
        return RK_ERROR;
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DisplayDev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_INIT_
INIT API rk_err_t DisplayDev_Delete(uint32 DevID, void * arg)
{
    //Check DisplayDev is not exist...
    if (gpstDisplayDevISR[DevID] == NULL)
    {
        return RK_ERROR;
    }

    //DisplayDev deinit...
    DisplayDevDeInit(gpstDisplayDevISR[DevID]);

    //Free DisplayDev memory...
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayOperReqSem);
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayOperSem);
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayReadReqSem);
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayReadSem);
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayWriteReqSem);
    rkos_semaphore_delete(gpstDisplayDevISR[DevID]->osDisplayWriteSem);
    rkos_memory_free(gpstDisplayDevISR[DevID]->FrameData);
    rkos_memory_free(gpstDisplayDevISR[DevID]);


    //Delete DisplayDev...
    gpstDisplayDevISR[DevID] = NULL;

    //Delete DisplayDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_DISPLAY_DEV);
#endif

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DisplayDev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_INIT_
INIT API HDC DisplayDev_Create(uint32 DevID, void * arg)
{
    DISPLAY_DEV_ARG * pstDisplayDevArg;
    DEVICE_CLASS* pstDev;
    DISPLAY_DEVICE_CLASS * pstDisplayDev;

    if (arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstDisplayDevArg = (DISPLAY_DEV_ARG *)arg;
    pstDisplayDev =  rkos_memory_malloc(sizeof(DISPLAY_DEVICE_CLASS));
    memset(pstDisplayDev, 0, sizeof(DISPLAY_DEVICE_CLASS));
    if (pstDisplayDev == NULL)
    {
        return NULL;
    }

    //init handler...
    pstDisplayDev->osDisplayOperReqSem  = rkos_semaphore_create(1,1);
    pstDisplayDev->osDisplayOperSem  = rkos_semaphore_create(1,0);

    pstDisplayDev->osDisplayReadReqSem  = rkos_semaphore_create(1,1);
    pstDisplayDev->osDisplayReadSem  = rkos_semaphore_create(1,1);

    pstDisplayDev->osDisplayWriteReqSem  = rkos_semaphore_create(1,1);
    pstDisplayDev->osDisplayWriteSem  = rkos_semaphore_create(1,1);
    if (((pstDisplayDev->osDisplayOperReqSem) == 0)
            || ((pstDisplayDev->osDisplayOperSem) == 0)
            || ((pstDisplayDev->osDisplayReadReqSem) == 0)
            || ((pstDisplayDev->osDisplayReadSem) == 0)
            || ((pstDisplayDev->osDisplayWriteReqSem) == 0)
            || ((pstDisplayDev->osDisplayWriteSem) == 0))
    {
        rkos_semaphore_delete(pstDisplayDev->osDisplayOperReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayOperSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayReadReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayReadSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayWriteReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayWriteSem);
        rkos_memory_free(pstDisplayDev);
        return (HDC) RK_ERROR;
    }
    pstDev = (DEVICE_CLASS *)pstDisplayDev;
    pstDev->suspend = DisplayDevSuspend;
    pstDev->resume  = DisplayDevResume;

    pstDisplayDev->hDma = pstDisplayDevArg->hDma;
    pstDisplayDev->FrameIndex = pstDisplayDevArg->FrameIndex;
    //init arg...
    //pstDisplayDev->usbmode = pstDisplayDevArg->usbmode;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_DISPLAY_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    gpstDisplayDevISR[DevID] = NULL;

    DisplayDevHwInit(DevID, 0);

    DisplayDevInit(pstDisplayDev);

    gpstDisplayDevISR[DevID] = pstDisplayDev;

    if (DisplayDevInit(pstDisplayDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstDisplayDev->osDisplayOperReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayOperSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayReadReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayReadSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayWriteReqSem);
        rkos_semaphore_delete(pstDisplayDev->osDisplayWriteSem);
        rkos_memory_free(pstDisplayDev);
        return (HDC) RK_ERROR;
    }

    return (HDC)pstDev;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DisplayDevDeInit
** Input:DISPLAY_DEVICE_CLASS * pstDisplayDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_INIT_
INIT FUN rk_err_t DisplayDevDeInit(DISPLAY_DEVICE_CLASS * pstDisplayDev)
{
    //If ip device first step unregister int
    //IntDisable(INT_ID17_UART);
    //IntPendingClear(INT_ID17_UART);
    //IntUnregister(INT_ID17_UART);

    IntUnregister(INT_ID_VOP);
    IntPendingClear(INT_ID_VOP);
    IntDisable(INT_ID_VOP);
    ScuClockGateCtr( HCLK_LCDC_GATE, 1);

    //If ip device second step close clk
    //ScuClockGateCtr(CLOCK_GATE_UART,0);
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DisplayDevInit
** Input:DISPLAY_DEVICE_CLASS * pstDisplayDev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_INIT_
INIT FUN rk_err_t DisplayDevInit(DISPLAY_DEVICE_CLASS * pstDisplayDev)
{
    //If ip device three step call ip bsp function init ip...
    //UARTIntInit(pstUartDev->dwBitRate,pstUartDev->dwBitWidth,UART_ONE_STOPBIT,UART_PARITY_DISABLE);
    FrameParameter *frameData = NULL;

    LcdControlInit();
    //VopSetTiming(0,5,5,5);
    //VopSetStart(0, 1);
    VopIntClear(0);

#ifdef _FRAME_BUFFER_
    //memset(frame_buffer, 0 , sizeof(frame_buffer));
    //init 32bit * 8 to zero every for{} ,for effeciency;frame_buffer is arrange by __align(4) , as 32bit per pixel
    frameData = rkos_memory_malloc(sizeof(FrameParameter));
    if (frameData != NULL)
    {
        printf ("\n malloc frameData memory success FrameNO=%d\n",pstDisplayDev->FrameIndex);
    }
    CurrentFrameIndex = pstDisplayDev->FrameIndex;

    gpstDisplayDevISR[CurrentFrameIndex]->FrameData = frameData;
    memset(gpstDisplayDevISR[CurrentFrameIndex]->FrameData, 0 , sizeof(gpstDisplayDevISR[CurrentFrameIndex]->FrameData)/(sizeof(uint32) * 8));

    Ymin = 0;
    Ymax = 0;
    Refresh = 0;
    RefreshCnt = 0;
    CrossBorder = 1;
    memset(&RECT, 0 , sizeof(RECT_INFO_STRUCT));
#endif
    //If ip device four step register intreput service...
    //IntRegister(INT_ID17_UART ,UartIntIsr0);
    //IntPendingClear(INT_ID17_UART);
    //IntEnable(INT_ID17_UART);
    //some ip device,such as uart ip device, only a read fifo and a write fifo,
    //we can use fifo as buffer for save some data when shut off Ip int
    return RK_SUCCESS;

}


#ifdef _DISPLAY_DEV_SHELL_
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
static char* ShellDisplayName[] =
{
    "pcb",
    "create",
    "del",
    "test",
    "...",
    "\b",
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DisplayDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
SHELL API rk_err_t DisplayDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellDisplayName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;
    switch (i)
    {
        case 0x00:
            ret = DisplayDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = DisplayDevShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = DisplayDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = DisplayDevShellTest(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DisplayDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
SHELL FUN rk_err_t DisplayDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hDisplayDev;
    DISPLAY_DEVICE_CLASS *pDev;
    uint32 DevID;
    uint16 i, j;
    VOP  stVop;
    //DisplayDevGetParameter(&stVop.VopMcuCmd);
    //Get DisplayDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }
    //Open DisplayDev...
    hDisplayDev = RKDev_Open(DEV_CLASS_DISPLAY, 0, NOT_CARE);
    if ((hDisplayDev == NULL) || (hDisplayDev == (HDC)RK_ERROR) || (hDisplayDev == (HDC)RK_PARA_ERR))
    {
        printf("DisplayDev open failure\n");
        return RK_SUCCESS;
    }
    pDev = (DISPLAY_DEVICE_CLASS *)hDisplayDev;

    VopSetWidthHeight(0, LCD_WIDTH, LCD_HEIGHT);
    DisplayDev_ClrSrc();
    VopEnableInt(0, VOP_INT_TYPE_FRAME_DONE);
    VopIntClear(0);
    VopSetTiming(0,5,5,5);

    printf("clr sc\n");
    DisplayDev_ClrSrc();
    DelayMs(2000);
    //do test....
    DisplayDev_SetCurrentBufferFrameIndex(0);
    printf("red rec:50,0,100,50\n");
    DisplayDev_SetWindow(50,0,100,50);
    VopSetStart(0,1);
    DisplayDev_SetVopMode(VOP_CON_FORMAT_RGB565, YUV_VERSION_0);
    for (j = 50; j < 101; j++)
    {
        for (i = 0; i < 51; i++)
        {
            DisplayDev_SendData(0xF800);
            //gpstFrameBuffer[CurrentFrameIndex][j][i] = i + j;
        }
    }
    //DisplayDev_BufferGrayScale(CurrentFrameIndex, 1);
    //LCD_ScreenUpdate(&gpstFrameBuffer[0][0][0]);
    VopIntClear(0);
    VopIntTypeFrameDoneFlag = 0;
    DisplayDev_Write(hDisplayDev);
    while ( VopIntTypeFrameDoneFlag == 0);
    VopIntTypeFrameDoneFlag = 0;
    DelayMs(2000);
    printf("clr sc\n");
    DisplayDev_ClrSrc();
    //  while(DisplayDevIntIsr() == 0);
    DelayMs(2000);
    printf("add purple line:y=50\n");
    DisplayDev_SetWindow(0,50,LCD_WIDTH-1,50);
    for (j = 50; j < 51; j++)
    {
        for (i = 0; i < 160; i++)
        {
            DisplayDev_SendData(0x8010);
            //gpstFrameBuffer[CurrentFrameIndex][j][i] = i + j;
        }
    }
    //DisplayDev_BufferGrayScale(CurrentFrameIndex, 1);
    //LCD_ScreenUpdate(&gpstFrameBuffer[0][0][0]);
    VopIntClear(0);
    VopIntTypeFrameDoneFlag = 0;
    DisplayDev_Write(hDisplayDev);
    while ( VopIntTypeFrameDoneFlag == 0);
    VopIntTypeFrameDoneFlag = 0;
#if 1
    DelayMs(2000);
    printf("green full scr FrameNO=%d \n",CurrentFrameIndex);
    DisplayDev_SetWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    for (j = 0;j < LCD_HEIGHT; j++)
    {
        for (i = 0;i < LCD_WIDTH; i++)
        {
            gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[j][i] = 0x07E0;
        }
    }
    //DisplayDev_BufferGrayScale(CurrentFrameIndex, 1);
    //LCD_ScreenUpdate(&gpstFrameBuffer[0][0][0]);
    VopIntClear(0);
    VopIntTypeFrameDoneFlag = 0;
    DisplayDev_Write(hDisplayDev);
    while ( VopIntTypeFrameDoneFlag == 0);
    VopIntTypeFrameDoneFlag = 0;
    DelayMs(2000);
    printf("yellow full scr\n");
    DisplayDev_SetWindow(0,0,LCD_WIDTH-1,LCD_HEIGHT-1);
    for (j = 0;j < LCD_HEIGHT; j++)
    {
        for (i = 0;i < LCD_WIDTH; i++)
        {
            gpstDisplayDevISR[CurrentFrameIndex]->FrameData->frameBuffer[j][i] = 0xFFE0;
        }
    }
    //DisplayDev_BufferGrayScale(CurrentFrameIndex, 5);
    //LCD_ScreenUpdate(&gpstFrameBuffer[0][0][0]);
    VopIntClear(0);
     VopIntTypeFrameDoneFlag = 0;
    DisplayDev_Write(hDisplayDev);
    while ( VopIntTypeFrameDoneFlag == 0);
    VopIntTypeFrameDoneFlag = 0;
#endif
    //close DisplayDev...
    RKDev_Close(hDisplayDev);
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DisplayDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
SHELL FUN rk_err_t DisplayDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;

    //Get DisplayDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }
    if (RKDev_Delete(DEV_CLASS_DISPLAY, DevID, NULL) != RK_SUCCESS)
    {
        rk_print_string("DISPLAYDev delete failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DisplayDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
SHELL FUN rk_err_t DisplayDevShellCreate(HDC dev, uint8 * pstr)
{
    DISPLAY_DEV_ARG stDisplayDevArg;
    rk_err_t ret;
    uint32 DevID;

    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }
#ifdef _JTAG_DEBUG_
    ret=RKDev_Create(DEV_CLASS_DMA, DMA_CHN0, NULL);
    if (ret != RK_SUCCESS)
    {
        printf( "DMA RKDev_Create failure\n");
    }
    printf( "DMA Create success\n");
#endif
    stDisplayDevArg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN0, NOT_CARE);
    stDisplayDevArg.FrameIndex = 0;
    //Create DisplayDev...
    if (stDisplayDevArg.hDma != NULL)
    {
        printf( "DMA OPEN success\n");
    }

    ret = RKDev_Create(DEV_CLASS_DISPLAY, DevID, &stDisplayDevArg);
    if (ret != RK_SUCCESS)
    {
        printf("DisplayDev create failure\n");
    }
    printf("\n DisplayDev create success\n");
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: DisplayDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.5.29
** Time: 10:23:49
*******************************************************************************/
_DRIVER_DISPLAY_DISPLAYDEVICE_SHELL_
SHELL FUN rk_err_t DisplayDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hDisplayDev;
    uint32 DevID;

    //Get DisplayDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    //Display pcb...
    if (gpstDisplayDevISR[DevID] != NULL)
    {
        rk_print_string("rn");
        rk_printf_no_time("DisplayDev ID = %d Pcb list as follow:", DevID);
        rk_printf_no_time("	&stDisplayDevice---0x%08x", &gpstDisplayDevISR[DevID]->stDisplayDevice);
        rk_printf_no_time("	osDisplayOperReqSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayOperReqSem);
        rk_printf_no_time("	osDisplayOperSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayOperSem);
        rk_printf_no_time("	osDisplayReadReqSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayReadReqSem);
        rk_printf_no_time("	osDisplayReadSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayReadSem);
        rk_printf_no_time("	osDisplayWriteReqSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayWriteReqSem);
        rk_printf_no_time("	osDisplayWriteSem---0x%08x", gpstDisplayDevISR[DevID]->osDisplayWriteSem);
        //rk_printf_no_time("	usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
        //rk_printf_no_time("	usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
    }
    else
    {
        rk_print_string("rn");
        rk_printf_no_time("DisplayDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}


#endif

#endif



