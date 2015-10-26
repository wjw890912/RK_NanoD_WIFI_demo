/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name£º   Scu.c
*
* Description:  each module open,close and reset and some related usb PHY registers setting.
*
* History:      <author>          <time>        <version>
*               anzhiguo          2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/
#include "BspConfig.h"
#include <typedef.h>
#include <DriverInclude.h>
#include "SysInfoSave.h"
#include "delay.h"

uint32      DCDCLastLevel;
uint32      DCDCOutputVol;
uint32      LDOLastLevel;
uint32      LDOOutputVol;
uint32      EfuseReadData;

/*
--------------------------------------------------------------------------------
  Function name : ScuClockSet()
  Author        : anzhiguo
  Description   : selet open/close module by its id.

  Input         : clk_id module id
                  Enable :open flag: 1--open this id module.0--close this id module

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo       2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void ScuClockGateCtr(eCLOCK_GATE clk_id, BOOL Enable)
{
    UINT8  group;
    UINT8  num;

    group  = clk_id / 16 ;
    num    = clk_id % 16;

    if (Enable)
    {
        Scu->CLKGATE[group] = (uint32)(0x00010000) << (num);
    }
    else
    {
        Scu->CLKGATE[group] = (uint32)(0x00010001) << (num);
    }
    Delay10cyc(5);
}

/*
--------------------------------------------------------------------------------
  Function name : ScuModuleReset()
  Author        : anzhiguo
  Description   : reset module by id.

  Input         : clk_id module id
                  Reset:   1:  reset
                           0:  release reset
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void ScuSoftResetCtr(eSOFT_RST Module_id, BOOL Reset)
{
    UINT8  group;
    UINT8  num;

    group  = Module_id / 16 ;
    num    = Module_id % 16;

    if (Reset)
    {
        Scu->SOFTRST[group] = (uint32)(0x00010001) << (num);
    }
    else
    {
        Scu->SOFTRST[group] = (uint32)(0x00010000) << (num);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDOEnable(void)
  Author        : zyz
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                     zyz        2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint8 LDODisable = 0;
uint8 LDORequestCnt = 0;

void ScuLDOControlInit(void)
{
    LDODisable = 0;
    LDORequestCnt = 0;
}


void ScuLDOEnable(void)
{
    if(LDORequestCnt >= 1)
    {
        LDORequestCnt--;
    }

    if(LDORequestCnt == 0)
        LDODisable = 0;
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDODisable(void)
  Author        : zyz
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                     zyz        2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void ScuLDODisable(void)
{
    LDODisable = 1;
    LDORequestCnt++;
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDOSet(uint32 Level)
  Author        : anzhiguo

  Input         : clk_id
                  Reset:   1:  reset
                           0:  release reset
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 ScuLDOEfuseTable[16]=
//{0,440,852,1238,1623,2008,2365,2778,-440,-880,-1320,-1760,-2228,-2723,-3218,-3795};
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int32 ScuLDOLevelTable[8]=
{34000,33000,32000,31000,30000,29000,28000,27000};

uint32 ScuLDOSet(uint32 Level)
{
    uint32 RetVal = LDOLastLevel;
    int32  delta;
    int32  SetVol;
    int32  eFuseRead   = (int32)(EfuseReadData & 0x0f);
    int32  eFuseDefaut = (int32)(gSysConfig.eFuseSaveData & 0x0f);

    if (LDODisable == 1)
    {
        return LDOLastLevel;
    }

    if (Level == LDOLastLevel)
    {
        return Level;
    }

    LDOLastLevel = Level;
    LDOOutputVol = (ScuLDOLevelTable[Level])/10;

    delta  = (ScuLDOEfuseTable[eFuseRead] - ScuLDOEfuseTable[eFuseDefaut]);
    SetVol = (33000 - delta) * ScuLDOLevelTable[Level]/33000;

    if (SetVol >= ScuLDOLevelTable[SCU_LDO_34])
    {
        Level = SCU_LDO_34;
    }
    else if (SetVol <= ScuLDOLevelTable[SCU_LDO_27])
    {
        Level = SCU_LDO_27;
    }
    else
    {
        for (Level = 0; Level < 7; Level++)
        {
            if ((ScuLDOLevelTable[Level + 1] < SetVol) && (SetVol <= ScuLDOLevelTable[Level]))
            {
                break;
            }
        }
    }

    LDOOutputVol = ((33000 + delta) * ScuLDOLevelTable[Level]/33000)/10;

    Grf->MPMU_PWR_CON = (MPMU_PWR_VID_LDO_MASK << 16) | (Level << 0);
    DEBUG1("LDOSetVol = %d, LDORealVol = %d", ScuLDOLevelTable[Level], LDOOutputVol);

    return(RetVal);
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDOEnable(void)
  Author        : zyz
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                     zyz        2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint8 DCoutDisable = 0;
uint8 DCoutRequestCnt = 0;

void ScuDCoutControlInit(void)
{
    DCoutDisable = 0;
    DCoutRequestCnt = 0;
}

void ScuDCoutEnable(void)
{
    if(DCoutRequestCnt >= 1)
    {
        DCoutRequestCnt--;
    }

    if(DCoutRequestCnt == 0)
        DCoutDisable = 0;
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDODisable(void)
  Author        : zyz
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                     zyz        2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void ScuDCoutDisable(void)
{
    DCoutDisable = 1;
    DCoutRequestCnt++;
}

/*
--------------------------------------------------------------------------------
  Function name : void ScuLDOSet(uint32 Level)
  Author        : anzhiguo

  Input         : clk_id
                  Reset:   1:  reset
                           0:  release reset
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 ScuDCDCLevelTable[16]=
{14000,14000,14000,14000,13500,13000,12500,12000,11500,11000,10500,10000,9500,9000,8500,8000};

uint32 ScuDCoutSet(uint32 Level)
{
    uint32 RetVal = DCDCLastLevel;
    int32  delta;
    int32  SetVol;
    uint32 eFuseRead   = (EfuseReadData & 0x0f) ;
    uint32 eFuseDefaut = (gSysConfig.eFuseSaveData & 0x0f);

    if(DCoutDisable == 1)
    {
        return DCDCLastLevel;
    }

    if (Level == DCDCLastLevel)
    {
        return Level;
    }

    DCDCOutputVol = ScuDCDCLevelTable[Level]/10;
    DCDCLastLevel = Level;

    delta  = (ScuLDOEfuseTable[eFuseRead] - ScuLDOEfuseTable[eFuseDefaut]);
    delta  = delta * 12000 / 33000;
    SetVol = (12000 - delta) * ScuDCDCLevelTable[Level]/12000;

    if (SetVol >= ScuDCDCLevelTable[SCU_DCOUT_140])
    {
        Level = SCU_DCOUT_140;
    }
    else if (SetVol <= ScuDCDCLevelTable[SCU_DCOUT_080])
    {
        Level = SCU_DCOUT_080;
    }
    else
    {
        for (Level = SCU_DCOUT_135 - 1; Level < SCU_DCOUT_080; Level++)
        {
            if ((ScuDCDCLevelTable[Level + 1] < SetVol) && (SetVol <= ScuDCDCLevelTable[Level]))
            {
                break;
            }
        }
    }
    if (Level < SCU_DCOUT_135) Level = SCU_DCOUT_140;

    DCDCOutputVol = ((12000 + delta) * ScuDCDCLevelTable[Level]/12000)/10;

    {
        Grf->MPMU_PWR_CON = (MPMU_PWR_VID_BUCK_MASK << 16) | (Level << 6);
    }

    DEBUG1("DCDCSetVol = %d, DCDCRealVol = %d", ScuDCDCLevelTable[Level], DCDCOutputVol);

    return(RetVal);
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Force_PWM_Mode_Set(BOOL Enable)
  Author           :
  Description     :  SW can charge PFM and PWM mode current threshold
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_Force_PWM_Mode_Set(BOOL Enable)
{
    if (Enable)
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_FORCE_PWM_MASK << 16) | MPMU_PWR_FORCE_PWM);
    }
    else
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_FORCE_PWM_MASK << 16) | MPMU_PWR_UNFORCE_PWM);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Threshold_Set(Threshold_Value Value)
  Author           :
  Description     :  SW can charge PFM and PWM mode current threshold
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_Threshold_Set(Threshold_Value Value)
{
    Grf->MPMU_PWR_CON = ((MPMU_PFM_THRESHOLD_MASK << 16) | (Value));
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Battery_Sns_Set(BOOL Enable)
  Author           :
  Description     :  Enable Batter Sences Sample
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_Battery_Sns_Set(BOOL Enable)
{
    if(Enable)
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_BAT_SNS_MASK << 16) | MPMU_PWR_BAT_SNS_ENABLE);
    }
    else
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_BAT_SNS_MASK << 16) | MPMU_PWR_BAT_SNS_DISABLE);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Charger_Enable_Set(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_Charger_Enable_Set(BOOL Enable)
{
    if (Enable)
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_CHARGE_ENABLE_MASK << 16) | MPMU_PWR_CHARGE_ENABLE);
    }
    else
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_CHARGE_ENABLE_MASK << 16) | MPMU_PWR_CHARGE_DISABLE);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Charger_Current_Set(Charge_Current_Value Value)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_Charger_Current_Set(Charge_Current_Value Value)
{
    Grf->MPMU_PWR_CON = ((MPMU_PWR_CHARGE_CURRENT_MASK << 16) | Value);
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_Charger_Current_Get(Charge_Current_Value Value)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
Charge_Current_Value Scu_Charger_Current_Get(void)
{
    return (Grf->MPMU_PWR_CON & MPMU_PWR_CHARGE_CURRENT_MASK);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 Scu_Charge_State_Get(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_Charge_State_Get(void)
{
    return ((Grf->MPMU_STATE  & MPMU_PWR_CHARGE_STATE_MASK) >> 10);
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_USB_PowerPath_Enable(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/

void Scu_USB_PowerPath_Enable(BOOL Enable)
{
    if (Enable)
    {
        RTCReg->RTC_CTRL &= ~(0x01ul);
    }
    else
    {
        RTCReg->RTC_CTRL |= 0x01ul;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_USB_PowerPath_Enable(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/

uint32 Scu_USB_PowerPath_State_Get(void)
{
    return (RTCReg->RTC_CTRL & 0x01ul);
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_USB_PowerPath_Enable(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/

void Scu_Obs_Select(uint32 sel)
{
    Grf->IOMUX_CON1 = ((OBS_SEL_MASK << 16) | sel);
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_USB_PowerPath_Enable(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/

uint32 Scu_Efuse_Read(uint32 Delay)
{
    uint32 temp;

    //VP HIGH
    //eFuse->CTRL = EFUSE_VP_EN(1) | EFUSE_WR_MODE(EFULE_INACTIVE);
    ASMDelay(Delay * 5);

    //VP LOW
    eFuse->CTRL = EFUSE_VP_EN(0) | EFUSE_WR_MODE(EFULE_INACTIVE);
    ASMDelay(Delay);

    //READ LOW
    eFuse->CTRL = EFUSE_VP_EN(0) | EFUSE_WR_MODE(EFUSE_READ);
    ASMDelay(Delay);

    //READ HIGH
    eFuse->CTRL = EFUSE_VP_EN(0) | EFUSE_WR_MODE(EFULE_INACTIVE);
    ASMDelay(Delay/2);

    //get read data
    temp = eFuse->DOUT;
    ASMDelay(Delay/2);

    //VP HIGH
    //eFuse->CTRL = EFUSE_VP_EN(1) | EFUSE_WR_MODE(EFULE_INACTIVE);
    ASMDelay(Delay * 5);

    return temp;

}

/*
--------------------------------------------------------------------------------
  Function name : BOOL Scu_DCout_Issue_State_Get(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_DCout_Issue_State_Get(void)
{
    return ((Grf->MPMU_STATE & MPMU_PWR_DCOUT_ALRAM_MASK) >> 5);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 Scu_PlayOn_State_Get(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_PlayOn_Level_Get(void)
{
    if (Grf->MPMU_STATE & MPMU_PWR_PLAY_LEVEL_MASK)
    {
        return 1;
    }
    return 0;
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 Scu_Play_Pause_Get(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_Play_Pause_Get(void)
{
    if (Grf->MPMU_STATE & MPMU_PWR_PLAY_PAUSE_MASK)
    {
        return 1;
    }
    return 0;
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 Scu_Adapter_State_Get(void)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_Adapter_State_Get(void)
{
    if (Grf->MPMU_STATE & MPMU_PWR_5V_READY)
    {
        return 1;
    }
    return 0;
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_OOL_Power_Set(BOOL Enable)
  Author           :
  Description     :  System PowerDown Or PowerUp
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_OOL_Power_Set(BOOL Enable)
{
    if(Enable)
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_BAT_POWER_MASK << 16) | MPMU_PWR_OOL_POWER_UP);
    }
    else
    {
        Grf->MPMU_PWR_CON = ((MPMU_PWR_BAT_POWER_MASK << 16) | MPMU_PWR_OOL_POWER_DOWN);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_saradc_mic_powerup(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_mic_pga_powerup(BOOL Enable)
{
    if(Enable)
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_POWER_MASK << 16) | MPMU_SARADC_MIC_POWERUP);
    }
    else
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_POWER_MASK << 16) | MPMU_SARADC_MIC_POWERDOWN);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_mic_pga_volset(uint32 vol)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_mic_pga_volset(uint32 vol)
{
    Grf->SARADC_CON = ((MPMU_SARADC_MIC_VOL_MASK << 16) | ((vol & 0x0f) << 1));
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_mic_boosten_set(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_mic_boosten_set(BOOL Enable)
{
    if(Enable)
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_BOOST_MASK << 16) | MPMU_SARADC_MIC_BOOSTEN);
    }
    else
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_BOOST_MASK << 16) | MPMU_SARADC_MIC_BOOSTDIS);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Scu_mic_boosten_set(BOOL Enable)
  Author           :
  Description     :
  Input             :
  Return           :

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_mic_ctriref_half_set(BOOL Enable)
{
    if(Enable)
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_CTRIREF_MASK << 16) | MPMU_SARADC_MIC_CTRIREFEN);
    }
    else
    {
        Grf->SARADC_CON = ((MPMU_SARADC_MIC_CTRIREF_MASK << 16) | MPMU_SARADC_MIC_CTRIREFDIS);
    }
}

/*
--------------------------------------------------------------------------------
  Function name : ScuUsbPHYSet()
  Author        : anzhiguo
  Description   : usb PHY control registers setting.



  Input         :

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo       2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void Scu_otgphy_suspend(BOOL Enable)
{
    if (Enable)
    {
        //Grf->OTGPHY_CON0 = (((OTGPHY_SUSPEND_MASK | OTGPHY_SOFT_CON_MASK) << 16) | (OTGPHY_SUSPEND_EN | OTGPHY_SOFT_CON_EN));
        Grf->OTGPHY_CON0 = (((OTGPHY_SUSPEND_MASK | OTGPHY_SOFT_CON_MASK) << 16) | (OTGPHY_SUSPEND_DIS | OTGPHY_SOFT_CON_EN));
    }
    else
    {
        Grf->OTGPHY_CON0 = (((OTGPHY_SUSPEND_MASK | OTGPHY_SOFT_CON_MASK) << 16) | (OTGPHY_SUSPEND_DIS | OTGPHY_SOFT_CON_DIS));
    }
    Delay100cyc(5);     //zyz: need delay
}

/*
--------------------------------------------------------------------------------
  Function name : int32 SCU_Init(void)
  Author        :
  Description   :

  Input         :
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 ScuClockGateInit(void)
{
    //Clock Gate Init
    Scu->CLKGATE[0] = (((uint32)(0x00010000) << (CLOCK_GATE_DRAM  - 0)) |
                       ((uint32)(0x00010000) << (CLOCK_GATE_IRAM  - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_TROM1 - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_TROM0 - 0)) |
                       ((uint32)(0x00010000) << (CLOCK_GATE_BTROM - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_LCDC  - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_BCH   - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_NANDC - 0)) |

                       ((uint32)(0x00010000) << (CLOCK_GATE_GPIO  - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_I2C   - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_I2S   - 0)) |
                       ((uint32)(0x00010000) << (CLOCK_GATE_PWM   - 0)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_ADC   - 0)) |
                       ((uint32)(0         )                          )
                      );

    Scu->CLKGATE[1] = (((uint32)(0x00010001) << (CLOCK_GATE_TIMER  - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_SPI    - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_SDMMC  - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_DMA    - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_SYNTH  - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_IMDCT  - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_USBPHY - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_USBBUS - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_UART   - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_EFUSE  - 16)) |
                       ((uint32)(0x00010001) << (CLOCK_GATE_RTC    - 16)) |
                       ((uint32)(0x00010000) << (CLOCK_GATE_GRF    - 16)) |
                       ((uint32)(0         )                            )
                      );
    Delay100cyc(50);
    return(0);
}

int32 ScuIpCoreInit(void)
{
    Scu->SOFTRST[0] = (uint32)(0xFFFFFFFF);
    Scu->SOFTRST[1] = (uint32)(0x001FFFFF);
    Scu->CLKSEL_CODEC |= 0X01;
    Scu->CLKSEL_MMC |= 0X01;
    Scu->CLKSEL_SPI |= 0x01;
    Scu->CLKSEL_UART |= 0x01;
    Scu->CLKSEL_SARADC |= 0x01;


    DelayMs(1);

    Scu->SOFTRST[0] = (uint32)(0xFFFF0000);
    Scu->SOFTRST[1] = (uint32)(0x001F0000);

	return 0;
}


/*
--------------------------------------------------------------------------------
  Function name : Scu_Memory_Set_Low_Speed(uint32 Memory)
  Author        : aaron.sun
  Description   : usb PHY control registers setting.



  Input         :

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo       2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
uint32 Scu_Get_Cpu_ID(void)
{
   return 0;
}

/*
********************************************************************************
*
*                         End of Scu.c
*
********************************************************************************
*/


