/*
********************************************************************************
*                   Copyright (c) 2008,anzhiguo
*                         All rights reserved.
*
* File Name：  Scu.h
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24          1.0
*    desc:    ORG.
********************************************************************************
*/
#ifndef _SCU_H
#define _SCU_H

#undef EXT
#ifdef _IN_SCU_
#define EXT
#else
#define EXT extern
#endif

typedef enum _SCU_APP
{
    SCU_INIT = (uint32)0,

    SCU_I2C,
    SCU_I2S,
    SCU_ADC,
    SCU_SDMMC,
    SCU_PWM,                   //pwm
    SCU_SPI,
    SCU_UART,

    SCU_MP3,
    SCU_WMA,
    SCU_AAC,
    SCU_WAV,
    SCU_APE,
    SCU_FLAC,

    SCU_RADIO,
    SCU_WAV_ENC,
    SCU_MP3_ENC,
    SCU_USB,

    SCU_APP_MAX

}eSCU_APP;


extern uint32      DCDCLastLevel;
extern uint32      DCDCOutputVol;
extern uint32      LDOLastLevel;
extern uint32      LDOOutputVol;
extern uint32      EfuseReadData;

/*
--------------------------------------------------------------------------------

                        Funtion Declaration

--------------------------------------------------------------------------------
*/
void ScuClockGateCtr(eCLOCK_GATE clk_id, BOOL Enable);
void ScuSoftResetCtr(eSOFT_RST Module_id, BOOL Reset);
void ScuLDOEnable(void);
void ScuLDODisable(void);
uint32 ScuLDOSet(uint32 Level);
void ScuDCoutEnable(void);
void ScuDCoutDisable(void);
uint32 ScuDCoutSet(uint32 Level);
void Scu_Force_PWM_Mode_Set(BOOL Enable);
void Scu_Threshold_Set(Threshold_Value Value);
void Scu_Battery_Sns_Set(BOOL Enable);
void Scu_Charger_Enable_Set(BOOL Enable);
void Scu_Charger_Current_Set(Charge_Current_Value Value);
uint32 Scu_Charge_State_Get(void);
void Scu_USB_PowerPath_Enable(BOOL Enable);
uint32 Scu_DCout_Issue_State_Get(void);
uint32 Scu_PlayOn_Level_Get(void);
uint32 Scu_Play_Pause_Get(void);
uint32 Scu_Adapter_State_Get(void);
void Scu_OOL_Power_Set(BOOL Enable);
void Scu_mic_pga_powerup(BOOL Enable);
void Scu_mic_pga_volset(uint32 vol);
void Scu_mic_boosten_set(BOOL Enable);
void Scu_mic_ctriref_half_set(BOOL Enable);
void Scu_otgphy_suspend(BOOL Enable);
int32 ScuClockGateInit(void);
//uint32 SetUartFreq(UINT32 nMHz); //测试用，应该放在PLL.C中
uint32 Scu_Get_Cpu_ID(void);

/*
********************************************************************************
*
*                         End of Scu.h
*
********************************************************************************
*/

#endif

