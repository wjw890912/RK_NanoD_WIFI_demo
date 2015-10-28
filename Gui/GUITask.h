/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                        All rights reserved.
*
* FileName: ..\Gui\GUITask.h
* Owner: zhuzhe
* Date: 2014.4.28
* Time: 16:01:47
* Desc:
* History:
*     <author>     <date>       <time>     <version>       <Desc>
* zhuzhe     2014.4.28     16:01:47   1.0
********************************************************************************************
*/

#ifndef __GUI_GUITASK_H__
#define __GUI_GUITASK_H__

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
#define _GUI_GUITASK_COMMON_ __attribute__((section("gui_guitask_common")))
#define _GUI_GUITASK_INIT_  __attribute__((section("gui_guitask_init")))

#ifndef GUI_USE_MULTI_FONT
    #define GUI_USE_SINGLE_FONT
#endif

#ifndef GUI_USE_ANSI_FONT
    #define GUI_USE_UNICODE_FONT
#endif

typedef void * HGC;

typedef struct _RKGUI_WIDGET_CLASS
{
    //Location information
    uint16 x1;
    uint16 x2;
    uint16 y1;
    uint16 y2;

    uint8  Layer;
    HGC    Next;
}RKGUI_WIDGET_CLASS;

typedef struct _RKGUI_LABEL_CLASS
{
    //Inherit the base class
    RKGUI_WIDGET_CLASS Widget;

    uint16  BackgroundColor;
    uint16  ForegroundColor;

    uint8   FocusFlag;  //mark is the focus
    uint32  PictureID;

#ifdef GUI_USE_ANSI_FONT
    uint8*  TextBuffer;
#else
    uint16* TextBuffer;
#endif

#ifdef GUI_USE_MULTI_FONT
    UINT16  FontID;
#endif
}RKGUI_LABEL_CLASS;

typedef struct _RKGUI_BUTTON_CLASS
{
    //Inherit the label class
    RKGUI_LABEL_CLASS Label;

    rk_err_t (*ButtonIndicate)(HGC widget,uint16 event);
}RKGUI_BUTTON_CLASS;

typedef struct _RK_TASK_GUI_ARG
{
    uint32 xxx;
}RK_TASK_GUI_ARG;


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
extern rk_err_t GUITask_Connect(HDC dev, uint32 tmerNum, uint32 TimerClk, uint64 usTick);
extern rk_err_t GUITask_Shell(HDC dev,uint8 * pstr);
extern void GUITask_Enter(void);
extern rk_err_t GUITask_DeInit(void *pvParameters);
extern rk_err_t GUITask_Init(void *pvParameters, void *arg);
extern void GUITaskTask_Enter(void);
extern rk_err_t GUITaskTask_DevInit(void *pvParameters);
extern rk_err_t GUITaskTask_Init(void *pvParameters);
extern rk_err_t GUITaskDeinit(void *pvParameters);
extern rk_err_t GUITaskInit(void *pvParameters);
extern void GUITask(void);
extern void GUIKeyCallBack(uint32 para);


#endif

