/*
********************************************************************************************
*
*   Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\Timer\TimerDevice.h
* Owner: zhuzhe
* Date: 2014.5.19
* Time: 11:00:17
* Desc:
* History:
*     <author>     <date>       <time>     <version>       <Desc>
* zhuzhe     2014.5.19     11:00:17   1.0
********************************************************************************************
*/

#ifndef __DRIVER_TIMER_TIMERDEVICE_H__
#define __DRIVER_TIMER_TIMERDEVICE_H__

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
#define _DRIVER_TIMER_TIMERDEVICE_COMMON_  __attribute__((section("driver_timer_timerdevice_common")))
#define _DRIVER_TIMER_TIMERDEVICE_INIT_  __attribute__((section("driver_timer_timerdevice_init")))
#define _DRIVER_TIMER_TIMERDEVICE_SHELL_  __attribute__((section("driver_timer_timerdevice_shell")))
/*
typedef  struct _TIMER_DEV_ARG
{
    uint32 timerNum;
}TIMER_DEV_ARG;
*/

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
extern rk_err_t TimerDev_Connect(HDC dev, uint32 tmerNum, uint32 TimerClk, uint64 usTick);
extern rk_err_t TimerDev_PeriodSet(eTIMER_NUM timerNum,UINT64 usTick, uint32 timerClk);
extern void TimerIntIsr0(void);
extern void TimerIntIsr1(void);
extern rk_err_t TimerDev_UnRegister(HDC dev,pFunc TimerCallBack);
extern rk_err_t TimerDev_Register(HDC dev,uint32 clk,pFunc TimerCallBack);
extern rk_err_t TimerDev_Clear(HDC dev);
extern rk_err_t  TimerDev_Stop(HDC dev);
extern rk_err_t  TimerDev_Start(HDC dev);
extern rk_err_t TimerDev_Close(HDC dev);
extern rk_err_t TimerDev_Open(HDC dev, uint16 oflag);
extern rk_err_t TimerDev_Delete(uint32 DevID, void * arg);
extern rk_err_t TimerDev_Resume(HDC dev);
extern rk_err_t TimerDev_Suspend(HDC dev);
extern HDC TimerDev_Create(uint32 DevID, void * arg);

extern rk_err_t TimerDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t TimerDev_Task_DeInit(void *pvParameters);
extern rk_err_t TimerDev_Task_Init(void *pvParameters);
extern void TimerDev_Task_Enter(void);

#endif

