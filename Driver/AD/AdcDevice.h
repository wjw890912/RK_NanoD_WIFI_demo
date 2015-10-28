/*
********************************************************************************************
*
*       Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\AD\AdcDevice.h
* Owner: zhuzhe
* Date: 2014.5.26
* Time: 10:43:06
* Desc:
* History:
*    <author>     <date>       <time>     <version>       <Desc>
*    zhuzhe     2014.5.26     10:43:06   1.0
********************************************************************************************
*/

#ifndef __DRIVER_AD_ADCDEVICE_H__
#define __DRIVER_AD_ADCDEVICE_H__


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

typedef rk_err_t (*pADCDevRx_indicate)(HDC dev);

//typedef struct _ADC_DEV_ARG
//{
//    HDC hTimmer;
//    uint32 channel;
//}ADC_DEV_ARG;

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
extern uint32  ADCDev_GetAdcData(uint32 Channel);
extern uint16   ADCDev_GetMainClk(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_ClearMainClk(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_SetMainClk(DEVICE_CLASS *dev,uint16 clk);
extern rk_err_t ADCDev_Init(HDC dev);
extern rk_err_t ADCDev_DeInit(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_Delete(uint32 DevID, void * arg);
extern rk_err_t ADCDev_Resume(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_Suspend(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_Read(HDC dev,uint16 channel,uint16 size,uint16 clk,uint16* buf, pADCDevRx_indicate RxCallBack);
extern HDC      ADCDev_Create(uint32 DevID, void * arg);
extern rk_err_t ADCDev_Stop(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_Start(DEVICE_CLASS *dev);
extern rk_err_t ADCDev_UnRegister(DEVICE_CLASS *dev,uint16 channel);
extern rk_err_t ADCDev_Register(HDC dev,uint16 channel,uint16 size,uint16 clk,uint16* buf, pADCDevRx_indicate RxCallBack);
extern void ADCDevISR(void);

extern rk_err_t ADCDev_Shell(HDC dev, uint8 * pstr);

#endif

