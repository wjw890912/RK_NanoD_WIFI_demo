/*
********************************************************************************
*                   Copyright (c) 2008,anzhiguo
*                         All rights reserved.
*
* File Name：   Hw_scu.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             anzhiguo          2009-3-24          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HWAPI_SCU_H
#define _HWAPI_SCU_H


typedef volatile struct SCUReg
{
    UINT32 PLL_CON;
    UINT32 MODE_CON;
    UINT32 CLKSEL_CPU;
    UINT32 CLKSEL_CODEC;            //CLKSEL_CODEC_CON
    UINT32 CLKSEL_MMC;              //CLKSEL_MMC_CON
    UINT32 CLKSEL_SPI;              //CLKSEL_SPI_CON
    UINT32 CLKSEL_UART;             //CLKSEL_UART_CON
    UINT32 CLKSEL_SARADC;           //CLKSEL_SARADC_CON
    UINT32 CLKGATE[2];
   	UINT32 SOFTRST[2];
    UINT32 STCLK;
    UINT32 STATUS;

}SCUReg_t, *pSCUReg_t; //将IO mux 放在别处处理

#define Scu  ((SCUReg_t *) CRU_BASE)

typedef volatile struct GRFReg
{
    UINT32 GPIO_IOMUX[4];

    UINT32 GPIO_PULL[4];

    UINT32 INTER_CON0;
    UINT32 INTER_CON1;

    UINT32 IOMUX_CON0;
    UINT32 IOMUX_CON1;


    UINT32 OTGPHY_CON0;
    UINT32 OTGPHY_CON1;

    UINT32 SARADC_CON;

    UINT32 PADREN_CON;

    UINT32 MPMU_DAC_CON;
    UINT32 MPMU_HP_CON;
    UINT32 MPMU_PWR_CON;
    UINT32 MPMU_STATE;

    UINT32 RESERVED[(0x60-0x50)/4];
    UINT32 SOFT_DBG0;
    UINT32 SOFT_DBG1;
    UINT32 PRJ_ID;
    UINT32 PRJ_VER;
}GRFReg_t, *pGRFReg_t; //将IO mux 放在别处处理

#define Grf     ((GRFReg_t *) GRF_BASE)

typedef volatile struct EfuseReg
{
    UINT32 CTRL;
    UINT32 DOUT;

}EFUSEReg_t, *pEFUSEReg_t;

#define eFuse   ((EFUSEReg_t *) EFUSE_BASE)

typedef enum CLOCK_GATE
{
    CLOCK_GATE_REV0,
    CLOCK_GATE_DRAM,
    CLOCK_GATE_REV2,
    CLOCK_GATE_REV3,
    CLOCK_GATE_IRAM,
    CLOCK_GATE_TROM1,
    CLOCK_GATE_TROM0,
    CLOCK_GATE_BTROM,
    CLOCK_GATE_LCDC,
    CLOCK_GATE_BCH,
    CLOCK_GATE_NANDC,
    CLOCK_GATE_GPIO,
    CLOCK_GATE_I2C,
    CLOCK_GATE_I2S,
    CLOCK_GATE_PWM,
    CLOCK_GATE_ADC,

    CLOCK_GATE_TIMER,
    CLOCK_GATE_SPI,
    CLOCK_GATE_SDMMC,
    CLOCK_GATE_DMA,
    CLOCK_GATE_SYNTH,
    CLOCK_GATE_IMDCT,
    CLOCK_GATE_USBPHY,
    CLOCK_GATE_USBBUS,
    CLOCK_GATE_UART,
    CLOCK_GATE_EFUSE,
    CLOCK_GATE_RTC,
    CLOCK_GATE_GRF,

}eCLOCK_GATE;

typedef enum SOFT_RST
{
    RST_ARM,
    RST_I2C,
    RST_I2S,
    RST_PWM,
    RST_ADC,
    RST_GPIO,
    RST_UART,
    RST_EFUSE,
    RST_RTC,
    RST_GRF,
    RST_TIMER,
    RST_SPI,
    RST_SDMMC,
    RST_DMA,
    RST_SYNTH,
    RST_IMDCT,

    RST_LCDC,
    RST_USBPHY,
    RST_USBLGC,
    RST_USBOTG,
    RST_NANDC,

}eSOFT_RST;

/*
*-------------------------------------------------------------------------------
*
*                         I2S memap register define
*
*-------------------------------------------------------------------------------
*/
//CRU_PLL_CON
#define PLL_BYPASS          ((UINT32)(1) << 0)
#define PLL_CLKOD_SHIFT     ((UINT32)(1) << 0)
#define PLL_CLKM_SHIFT      ((UINT32)(5) << 0)
#define PLL_CLKN_SHIFT      ((UINT32)(13)<< 0)
#define PLL_RESET           ((UINT32)(1) << 17)
#define PLL_POWER_DOWN      ((UINT32)(1) << 18)
#define PLL_POWER_LOCK      ((UINT32)(1) << 19)


//CRU_CLKSEL_CPU_CON
#define X24MIN_CLKDIV_SHIFT     ((UINT32)(9))
#define X24MIN_CLKDIV_MASK      ((UINT32)(0x03))
#define SYSTICK_CLKDIV_SHIFT    ((UINT32)(0))
#define SYSTICK_CLKDIV_MASK     ((UINT32)(0x07))
#define AHBCLK_CLKDIV_SHIFT     ((UINT32)(3))
#define AHBCLK_CLKDIV_MASK      ((UINT32)(0x07))
#define APBCLK_CLKDIV_SHIFT     ((UINT32)(7))
#define APBCLK_CLKDIV_MASK      ((UINT32)(0x03))
#define ARMCLK_SEL_SHIFT        ((UINT32)(6))
#define ARMCLK_SEL_24M          ((UINT32)(0))
#define ARMCLK_SEL_PLL          ((UINT32)(1))
#define ARMCLK_SEL_MASK         ((UINT32)(1))

//CRU_CLKSEL_CODEC_CON
#define I2S_CLK_SEL_PLL         ((UINT32)(0))
#define I2S_CLK_SEL_12M         ((UINT32)(1))
#define I2S_CLK_SEL_MASK        ((UINT32)(1))
#define I2S_CLK_DIV_SHIFT       ((UINT32)(1))
#define I2S_CLK_DIV_MASK        ((UINT32)(0xff))

//CRU_CLKSEL_MMC_CON
#define MMC_CLK_SEL_PLL         ((UINT32)(0))
#define MMC_CLK_SEL_24M         ((UINT32)(1))
#define MMC_CLK_SEL_MASK        ((UINT32)(1))
#define MMC_CLK_DIV_SHIFT       ((UINT32)(1))
#define MMC_CLK_DIV_MASK        ((UINT32)(0x1f))

//CRU_CLKSEL_SPI_CON
#define SPI_CLK_SEL_PLL         ((UINT32)(0))
#define SPI_CLK_SEL_24M         ((UINT32)(1))
#define SPI_CLK_SEL_MASK        ((UINT32)(1))
#define SPI_CLK_DIV_SHIFT       ((UINT32)(1))
#define SPI_CLK_DIV_MASK        ((UINT32)(0x3f))

//CRU_CLKSEL_UART_CON
#define UART_CLK_SEL_PLL        ((UINT32)(0))
#define UART_CLK_SEL_24M        ((UINT32)(1))
#define UART_CLK_SEL_MASK       ((UINT32)(1))
#define UART_CLK_DIV_SHIFT      ((UINT32)(1))
#define UART_CLK_DIV_MASK       ((UINT32)(0x3f))

//CRU_CLKSEL_SARADC_CON (24M sel Only)
#define ADC_CLK_DIV_SHIFT       ((UINT32)(0))
#define ADC_CLK_DIV_MASK        ((UINT32)(0x3f))

//GRF OTGPHY CON
#define OTGPHY_SUSPEND_DIS      ((UINT32)0 << 6)
#define OTGPHY_SUSPEND_EN       ((UINT32)1 << 6)
#define OTGPHY_SUSPEND_MASK     ((UINT32)1 << 6)

#define OTGPHY_SOFT_CON_DIS     ((UINT32)0 << 5)
#define OTGPHY_SOFT_CON_EN      ((UINT32)1 << 5)
#define OTGPHY_SOFT_CON_MASK    ((UINT32)1 << 5)

//MPMU_DAC_CON
#define MPMU_DAC_POWER_DOWM         ((UINT32)1 << 0)
#define MPMU_DAC_POWER_UP           ((UINT32)0 << 0)
#define MPMU_DAC_POWER_MASK         ((UINT32)1 << 0)

#define MPMU_DAC_VOL_UPD_ACTIVE     ((UINT32)1 << 1)
#define MPMU_DAC_VOL_UPD_INACTIVE   ((UINT32)0 << 1)
#define MPMU_DAC_VOL_UPD_MASK       ((UINT32)1 << 1)

#define MPMU_DAC_MUTE               ((UINT32)1 << 2)
#define MPMU_DAC_UNMUTE             ((UINT32)0 << 2)
#define MPMU_DAC_MUTE_MASK          ((UINT32)1 << 2)

#define MPMU_DAC_DIG_VOL_MASK       ((UINT32)0xff << 3)
#define MPMU_DAC_ANA_VOL_MASK       ((UINT32)0x0f << 11)

#define MPMU_DAC_DIG_VOL(DigVol)    ((UINT32)(DigVol) << 3)
#define MPMU_DAC_ANA_VOL(AnaVol)    ((UINT32)(AnaVol) << 11)

//MPMU_HP_CON
#define MPMU_HP_FORCE_VDDIO_ACTIVE   ((UINT32)1 << 0)
#define MPMU_HP_FORCE_VDDIO_INACTIVE ((UINT32)0 << 0)
#define MPMU_HP_FORCE_VDDIO_MASK     ((UINT32)1 << 0)

#define MPMU_HP_FORCE_1200MV_ACTIVE   ((UINT32)1 << 8)
#define MPMU_HP_FORCE_1200MV_INACTIVE ((UINT32)0 << 8)
#define MPMU_HP_FORCE_1200MV_MASK     ((UINT32)1 << 8)

#define MPMU_HP_POWER_DOWM          ((UINT32)1 << 1)
#define MPMU_HP_POWER_UP            ((UINT32)0 << 1)
#define MPMU_HP_POWER_MASK          ((UINT32)1 << 1)

#define MPMU_HP_MIX_NO_INPUT        ((UINT32)0 << 2)
#define MPMU_HP_MIX_LINE_IN         ((UINT32)1 << 2)
#define MPMU_HP_MIX_DAC_IN          ((UINT32)2 << 2)
#define MPMU_HP_MIX_MIXED           ((UINT32)3 << 2)
#define MPMU_HP_MIX_MASK            ((UINT32)3 << 2)

#define MPMU_HP_WRITE_ENABLE        ((UINT32)1 << 16)
#define MPMU_HP_WRITE_DISABLE       ((UINT32)0 << 16)

#define MPMU_HP_LINE_IN_GAIN_MASK   ((UINT32)3 << 9)
#define MPMU_HP_LINE_IN_GAIN(Gain)  ((UINT32)(Gain) << 9)

#define  MPMU_FREQ48kHz             ((UINT32)0x08<<4)
#define  MPMU_FREQ441kHz            ((UINT32)0x07<<4)
#define  MPMU_FREQ32kHz             ((UINT32)0x06<<4)
#define  MPMU_FREQ24kHz             ((UINT32)0x05<<4)
#define  MPMU_FREQ22050Hz           ((UINT32)0x04<<4)
#define  MPMU_FREQ16kHz             ((UINT32)0x03<<4)
#define  MPMU_FREQ12kHz             ((UINT32)0x02<<4)
#define  MPMU_FREQ11025Hz           ((UINT32)0x01<<4)
#define  MPMU_FREQ8kHz              ((UINT32)0x00<<4)
#define  MPMU_FREQ_MASK             ((UINT32)0x0F<<4)

// MPMU_PWR_SCU
#define MPMU_PWR_BAT_SNS_DISABLE        ((UINT32)(0) << 14)
#define MPMU_PWR_BAT_SNS_ENABLE         ((UINT32)(1) << 14)
#define MPMU_PWR_BAT_SNS_MASK           ((UINT32)(1) << 14)

#define MPMU_PWR_OOL_POWER_UP           ((UINT32)(0) << 13)
#define MPMU_PWR_OOL_POWER_DOWN         ((UINT32)(1) << 13)
#define MPMU_PWR_BAT_POWER_MASK         ((UINT32)(1) << 13)

#define MPMU_PWR_CHARGE_CURRENT60       ((UINT32)(0) << 11)
#define MPMU_PWR_CHARGE_CURRENT150      ((UINT32)(1) << 11)
#define MPMU_PWR_CHARGE_CURRENT280      ((UINT32)(2) << 11)
#define MPMU_PWR_CHARGE_CURRENT30       ((UINT32)(3) << 11)
#define MPMU_PWR_CHARGE_CURRENT_MASK    ((UINT32)(3) << 11)

//GRF_PWR_CHARGE_SET
#define MPMU_PWR_CHARGE_DISABLE         ((UINT32)(0) << 10)
#define MPMU_PWR_CHARGE_ENABLE          ((UINT32)(1) << 10)
#define MPMU_PWR_CHARGE_ENABLE_MASK     ((UINT32)(1) << 10)

#define MPMU_PWR_VID_BUCK_MASK          ((UINT32)(0x0F) << 6)

#define MPMU_PFM_THRESHOLD_SHIFT        (4)
#define MPMU_PFM_THRESHOLD_26MA         ((UINT32)(0x0) << 4)
#define MPMU_PFM_THRESHOLD_34MA         ((UINT32)(0x1) << 4)
#define MPMU_PFM_THRESHOLD_10MA         ((UINT32)(0x2) << 4)
#define MPMU_PFM_THRESHOLD_18MA         ((UINT32)(0x3) << 4)
#define MPMU_PFM_THRESHOLD_MASK         ((UINT32)(0x3) << 4)

#define MPMU_PWR_UNFORCE_PWM            ((UINT32)(0x0) << 3)
#define MPMU_PWR_FORCE_PWM              ((UINT32)(0x1) << 3)
#define MPMU_PWR_FORCE_PWM_MASK         ((UINT32)(0x1) << 3)

#define MPMU_PWR_VID_LDO_MASK           ((UINT32)(0x07) << 0)

//MPMU_STATE
//GRF_PWR_CHARGE_STATE
#define MPMU_PWR_CHARGE_IS_OVER         ((UINT32)0)
#define MPMU_PWR_CHARGE_IS_WORKING      ((UINT32)1)
#define MPMU_PWR_CHARGE_IS_ERROR        ((UINT32)2)
#define MPMU_PWR_CHARGE_STATE_MASK      ((UINT32)3 <<10)

//GRF_PWR_DCOUT_ALARM   OUTPUT
#define MPMU_PWR_DCOUT_OVER_CURRENT     ((UINT32)(1) << 9)  //current too high
#define MPMU_PWR_DCOUT_OVER_TEMPRATURE  ((UINT32)(1) << 8)  //temperature too high
#define MPMU_PWR_DCOUT_OVER_VOLTAGE     ((UINT32)(1) << 7)  //woltage too high.
#define MPMU_PWR_DCDC_UNDER_VOLTAGE     ((UINT32)(1) << 6)  //DCDC under.
#define MPMU_PWR_DCOUT_UNDER_VOLTAGE    ((UINT32)(1) << 5)  //voltage too low
#define MPMU_PWR_DCOUT_ALRAM_MASK       ((UINT32)(0x1f) << 5)  //voltage too low

#define MPMU_PWR_5V_READY               ((UINT32)(1) << 4)

#define MPMU_PWR_OOL_PAUSE              ((UINT32)(0) << 2)
#define MPMU_PWR_PLAY_PAUSE_MASK        ((UINT32)(1) << 2)

#define MPMU_PWR_PLAY_LEVEL_MASK        ((UINT32)(1) << 3)

//mic pga power
#define MPMU_SARADC_MIC_POWERUP         ((UINT32)(0) << 0)
#define MPMU_SARADC_MIC_POWERDOWN       ((UINT32)(1) << 0)
#define MPMU_SARADC_MIC_POWER_MASK      ((UINT32)(1) << 0)

//mic vol
#define MPMU_SARADC_MIC_VOL_MASK        ((UINT32)(0x0f) << 1)

//mic boosten
#define MPMU_SARADC_MIC_BOOSTDIS        ((UINT32)(0) << 5)
#define MPMU_SARADC_MIC_BOOSTEN         ((UINT32)(1) << 5)
#define MPMU_SARADC_MIC_BOOST_MASK      ((UINT32)(1) << 5)

//mic ctriref
#define MPMU_SARADC_MIC_CTRIREFDIS      ((UINT32)(0) << 6)
#define MPMU_SARADC_MIC_CTRIREFEN       ((UINT32)(1) << 6)
#define MPMU_SARADC_MIC_CTRIREF_MASK    ((UINT32)(1) << 6)

//LDO VID
#define SCU_LDO_27                      ((uint32)7)
#define SCU_LDO_28                      ((uint32)6)
#define SCU_LDO_29                      ((uint32)5)
#define SCU_LDO_30                      ((uint32)4)
#define SCU_LDO_31                      ((uint32)3)
#define SCU_LDO_32                      ((uint32)2)
#define SCU_LDO_33                      ((uint32)1)
#define SCU_LDO_34                      ((uint32)0)

//DCDC VID
#define SCU_DCOUT_080                   ((uint32)0x0f)
#define SCU_DCOUT_085                   ((uint32)0x0e)
#define SCU_DCOUT_090                   ((uint32)0x0d)
#define SCU_DCOUT_095                   ((uint32)0x0c)
#define SCU_DCOUT_100                   ((uint32)0x0b)
#define SCU_DCOUT_105                   ((uint32)0x0a)
#define SCU_DCOUT_110                   ((uint32)0x09)
#define SCU_DCOUT_115                   ((uint32)0x08)
#define SCU_DCOUT_120                   ((uint32)0x07)
#define SCU_DCOUT_125                   ((uint32)0x06)
#define SCU_DCOUT_130                   ((uint32)0x05)
#define SCU_DCOUT_135                   ((uint32)0x04)
#define SCU_DCOUT_140                   ((uint32)0x00)

typedef enum
{
    PFM_26MA = MPMU_PFM_THRESHOLD_26MA,
    PFM_34MA = MPMU_PFM_THRESHOLD_34MA,
    PFM_10MA = MPMU_PFM_THRESHOLD_10MA,
    PFM_18MA = MPMU_PFM_THRESHOLD_18MA

}Threshold_Value;

typedef enum
{
    CHARGE_30mA  = MPMU_PWR_CHARGE_CURRENT30,
    CHARGE_60mA  = MPMU_PWR_CHARGE_CURRENT60,
    CHARGE_150mA = MPMU_PWR_CHARGE_CURRENT150,
    CHARGE_280mA = MPMU_PWR_CHARGE_CURRENT280,

}Charge_Current_Value;

//eFuse define
#define EFUSE_PROGRAM           1
#define EFUSE_READ              2
#define EFULE_INACTIVE          3

#define EFUSE_VP_EN(en)         ((UINT32)(en) << 6)
#define EFULE_ADDR(addr)        ((UINT32)(addr) << 3)
#define EFUSE_ADDR_MASK         ((UINT32)(0x07) << 3)
#define EFUSE_ENABLE(en)        ((UINT32)(en) << 2)
#define EFUSE_WR_SEL(wr)        ((UINT32)(wr) << 1)
#define EFUSE_WR_MODE(mode)     ((UINT32)(mode) << 1)

//obs sel
#define OBS_SEL_HCLK            ((uint32)(0x01) << 3)
#define OBS_SEL_PCLK            ((uint32)(0x02) << 3)
#define OBS_SEL_ARM_HCLK        ((uint32)(0x03) << 3)
#define OBS_SEL_ARM_FCLK        ((uint32)(0x04) << 3)
#define OBS_SEL_ARM_STCLK       ((uint32)(0x05) << 3)
#define OBS_SEL_SDMMC_CLK       ((uint32)(0x06) << 3)
#define OBS_SEL_SARADC_CLK      ((uint32)(0x07) << 3)
#define OBS_SEL_OTG_CLK         ((uint32)(0x08) << 3)
#define OBS_SEL_SPI_CLK         ((uint32)(0x09) << 3)
#define OBS_SEL_UART_CLK        ((uint32)(0x0A) << 3)
#define OBS_SEL_I2S_CLK         ((uint32)(0x0B) << 3)
#define OBS_SEL_MASK            ((uint32)(0x0F) << 3)

/*
********************************************************************************
*
*                         End of Hw_scu.h
*
********************************************************************************
*/

#endif

