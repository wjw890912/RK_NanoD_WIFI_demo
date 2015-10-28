/*
********************************************************************************
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* File Name：   PowerManager.c
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/

#include "bspconfig.h"
#include "RKOS.h"
#include "DriverInclude.h"
#include "interrupt.h"
#include "PowerManager.h"

/*
--------------------------------------------------------------------------------

                        Variable Define

--------------------------------------------------------------------------------
*/
//typedef __int64 uint64 ;

typedef void(*ExecFunPtr)(void) __irq;
extern UINT32 Image$$IDLE_STACK$$ZI$$Limit;
void PmuResume();

CRU_CLK_INFO    g_chipClk;
uint64          g_APPList;
uint64          g_APPDisableList;
uint8           FreqDisableFlag = 0;
uint8           FreqDisableCnt = 0;

#define _ATTR_VECTT_PMU_             __attribute__((section("pmu_vect")))
#define _ATTR_PMU_CODE_              __attribute__((section("pmucode")))
#define _ATTR_PMU_DATA_              __attribute__((section("pmudata")))
#define _ATTR_PMU_BSS_              __attribute__((section("pmudata"),zero_init))

_ATTR_VECTT_PMU_
ExecFunPtr pmusram_table[NUM_INTERRUPTS] = {

    /* Core Fixed interrupts start here...*/

    (ExecFunPtr)&Image$$IDLE_STACK$$ZI$$Limit,
    (ExecFunPtr)PmuResume,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,

    //RKNano Process Intterupt
    IntDefaultHandler,                                          //00 int_dma
    IntDefaultHandler,                                          //01 uart
    IntDefaultHandler,                                          //02 sd mmc
    IntDefaultHandler,                                          //03 pwm1
    IntDefaultHandler,                                          //04 pwm2
    IntDefaultHandler,                                          //05 imdct36
    IntDefaultHandler,                                          //06 synthesize
    IntDefaultHandler,                                          //07 usb
    IntDefaultHandler,                                          //08 i2c
    IntDefaultHandler,                                          //09 i2s
    IntDefaultHandler,                                          //10 gpio
    IntDefaultHandler,                                          //11 spi
    IntDefaultHandler,                                          //12 pwm0
    IntDefaultHandler,                                          //13 timer
    IntDefaultHandler,                                          //14 sar-adc
    IntDefaultHandler,                                          //15 reserved
    IntDefaultHandler,                                          //16 reserved
    IntDefaultHandler,                                          //17 reserved
    IntDefaultHandler,                                          //18 rtc
    IntDefaultHandler,                                          //19 reserved
    IntDefaultHandler,                                          //20 ool_bat_snsen
    IntDefaultHandler,                                          //21 reserved
    IntDefaultHandler,                                          //22 ool_pause
    IntDefaultHandler,                                          //23 ool_PlayOn
    IntDefaultHandler,                                          //24 pwr_5v_ready     Adapter supply detected
    IntDefaultHandler,                                          //25 pwr_uvlo_vdd      Vbat under 3.2V indicator
    IntDefaultHandler,                                          //26 pwr_uvp              DC-DC under voltage alarm
    IntDefaultHandler,                                          //27 pwr_ovp              DC-DC over voltage alarm
    IntDefaultHandler,                                          //28 pwr_ot                DC-DC over temperature alarm
    IntDefaultHandler,                                          //29 pwr_oc                DC-DC over current alarm
    IntDefaultHandler,                                          //30 pwr_charge         Charge state changing indicator
    IntDefaultHandler,                                          //31 reserved
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler
};


FREQ_APP_TABLE g_CruAPPTabel[FREQ_APP_MAX] =
#ifdef A_CORE_DECODE
{
//          ID,      counter,       pll,  sys_hclk,  sys_stclk, sys_pclk,  cal_hclk,  cal_stclk
    {FREQ_IDLE,            0,         0,         0,         0,         0,         0,         0},
    {FREQ_MIN,             0,  24000000,  24000000,  24000000,  24000000,         0,         0},

    {FREQ_INIT,            0,  96000000,  96000000,  96000000,  75000000,         0,         0},
    {FREQ_BLON,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_AUDIO_INIT,      0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MP3,             0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MP3H,            0,  30000000,  30000000,  30000000,  30000000,         0,         0},
    {FREQ_WMA,             0,  60000000,  60000000,  60000000,  60000000,         0,         0},
    {FREQ_WMAH,               0,  96000000,  96000000,  96000000,  96000000,         0,         0},
    {FREQ_WAV,             0,  60000000,  60000000,  60000000,  60000000,         0,         0},
    {FREQ_AAC,             0,  48000000,  48000000,  48000000,  48000000,         0,         0},
    {FREQ_APE,             0,  80000000,  80000000,  80000000,  75000000,         0,         0},
    {FREQ_FLAC,            0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_OGG,             0, 140000000, 140000000, 140000000,  75000000,         0,         0},
    {FREQ_NOGG,            0, 140000000, 140000000, 140000000,  75000000,         0,         0},
    {FREQ_HOGG,            0, 180000000, 180000000, 180000000,  75000000,         0,         0},
    {FREQ_EHOGG,           0, 260000000, 260000000, 260000000,  75000000,         0,         0},
    {FREQ_HAPE,            0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_HFLAC,           0,  80000000,  80000000,  80000000,  75000000,         0,         0},
    {FREQ_HALAC,           0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    #ifdef _RK_EQ_5_
    {FREQ_EQ,              0,  36000000,  36000000,  36000000,  36000000,         0,         0},
    #else
    {FREQ_EQ_L,            0,  36000000,  36000000,  36000000,  36000000,  36000000,  36000000},
    {FREQ_EQ,              0,  72000000,  72000000,  72000000,  72000000,  72000000,  72000000},
    {FREQ_EQ_H,            0, 140000000, 140000000, 140000000, 140000000, 140000000, 140000000},
    #endif
    {FREQ_JPG,             0, 250000000, 250000000, 250000000,  75000000,         0,         0},
    {FREQ_FM,              0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_RECORDADPCM,     0,  56000000,  56000000,  56000000,  56000000,         0,         0},
    {FREQ_FMAUTOSEARCH,    0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIAUPDATA,     0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_USB,             0, 500000000, 250000000, 250000000,  75000000, 500000000, 500000000}, //USB 会锁频
    {FREQ_BEEP,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_LOW,   0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_HIGH,  0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIA_INIT,      0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_AVI,             0, 100000000, 100000000, 100000000,  75000000,         0,         0},

    {FREQ_MAX,             0, 500000000, 250000000, 250000000,  75000000, 300000000, 300000000},
};
#else
{

#ifdef CODEC_24BIT
//          ID,      counter,       pll,  sys_hclk,  sys_stclk, sys_pclk,  cal_hclk,  cal_stclk
    {FREQ_IDLE,            0,         0,         0,         0,         0,         0,         0},
    {FREQ_MIN,             0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_INIT,            0,  96000000,  96000000,  96000000,  96000000,         0,         0},
    {FREQ_BLON,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_AUDIO_INIT,      0,  48000000,  48000000,  48000000,  48000000,  48000000,  48000000},
    {FREQ_MP3,             0,  24000000,  24000000,  24000000,  24000000,  24000000,  24000000},
    {FREQ_MP3H,            0,  36000000,  36000000,  36000000,  36000000,  36000000,  36000000},
    {FREQ_WMA,             0,  60000000,  60000000,  60000000,  60000000,  60000000,  60000000},
    {FREQ_WMAH,               0,  100000000, 100000000, 100000000, 100000000, 100000000, 100000000},
    {FREQ_WAV,             0,  88000000,  88000000,  88000000,  88000000,  88000000,  88000000},
    {FREQ_AAC,             0,  80000000,  80000000,  80000000,  80000000,  80000000,  80000000},
    {FREQ_APE,             0, 150000000, 150000000, 150000000,  75000000, 150000000, 150000000},
    {FREQ_FLAC,            0, 150000000, 150000000, 150000000,  75000000, 150000000, 150000000},
    {FREQ_OGG,             0,  80000000,  80000000,  80000000,  80000000, 120000000, 120000000},
    {FREQ_NOGG,            0, 120000000,  80000000,  80000000,  80000000, 120000000, 120000000},
    {FREQ_HOGG,            0, 180000000,  80000000,  80000000,  80000000, 180000000, 180000000},
    {FREQ_EHOGG,           0, 260000000,  80000000,  80000000,  80000000, 260000000, 260000000},
    {FREQ_HAPE,            0, 220000000,  80000000,  80000000,  80000000, 440000000, 440000000},
    {FREQ_HFLAC,           0, 220000000,  80000000,  80000000,  80000000, 220000000, 220000000},
    {FREQ_HALAC,           0, 220000000,  80000000,  80000000,  80000000, 440000000, 440000000},
    #ifdef _RK_EQ_5_
    {FREQ_EQ,              0,  36000000,  36000000,  36000000,  36000000,  36000000,  36000000},
    #else
    {FREQ_EQ_L,            0,  36000000,  36000000,  36000000,  36000000,  36000000,  36000000},
    {FREQ_EQ,              0,  72000000,  72000000,  72000000,  72000000,  72000000,  72000000},
    {FREQ_EQ_H,            0, 140000000, 140000000, 140000000, 140000000, 140000000, 140000000},
    #endif
    {FREQ_JPG,             0, 250000000, 250000000, 250000000,  75000000,         0,         0},
    {FREQ_FM,              0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_RECORDADPCM,     0,  56000000,  56000000,  56000000,  56000000,         0,         0},
    {FREQ_FMAUTOSEARCH,    0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIAUPDATA,     0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_USB,             0, 500000000, 250000000, 250000000,  75000000, 500000000, 500000000}, //USB 会锁频
    {FREQ_BEEP,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_LOW,   0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_HIGH,  0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIA_INIT,      0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_AVI,             0, 100000000, 100000000, 100000000,  75000000,         0,         0},

    {FREQ_MAX,             0, 500000000, 200000000, 200000000,  75000000, 200000000, 200000000},


#else


    //          ID,      counter,       pll,  sys_hclk,  sys_stclk, sys_pclk,  cal_hclk,  cal_stclk
    {FREQ_IDLE,            0,         0,         0,         0,         0,         0,         0},
    {FREQ_MIN,             0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_INIT,            0,  96000000,  96000000,  96000000,  75000000,         0,         0},
    {FREQ_BLON,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_AUDIO_INIT,      0,  48000000,  48000000,  48000000,  48000000,  48000000,  48000000},
    {FREQ_MP3,             0,  12000000,  12000000,  12000000,  12000000,  24000000,  24000000},
    {FREQ_MP3H,            0,  30000000,  30000000,  30000000,  30000000,  30000000,  30000000},
    {FREQ_WMA,             0,  60000000,  60000000,  60000000,  60000000,  60000000,  60000000},
    {FREQ_WMAH,               0,  96000000,  96000000,  96000000,  96000000,  96000000,  96000000},
    {FREQ_WAV,             0,  88000000,  88000000,  88000000,  88000000,  88000000,  88000000},
    {FREQ_AAC,             0,  80000000,  80000000,  80000000,  75000000,  80000000,  80000000},
    {FREQ_APE,             0, 100000000, 100000000, 100000000,  75000000, 120000000, 120000000},
    {FREQ_FLAC,            0, 100000000, 100000000, 100000000,  75000000, 100000000, 100000000},
    {FREQ_OGG,             0,  80000000,  80000000,  80000000,  75000000,  80000000,  80000000},
    {FREQ_NOGG,            0, 120000000, 120000000, 120000000,  75000000, 120000000, 120000000},
    {FREQ_HOGG,            0, 180000000, 180000000, 180000000,  75000000, 180000000, 180000000},
    {FREQ_EHOGG,           0, 260000000,  80000000,  80000000,  75000000, 260000000, 260000000},
    {FREQ_HAPE,            0, 100000000, 100000000, 100000000,  75000000, 100000000, 100000000},
    {FREQ_HFLAC,           0,  80000000,  80000000,  80000000,  75000000,  80000000,  80000000},
    {FREQ_HALAC,           0, 100000000, 100000000, 100000000,  75000000, 100000000, 100000000},
    #ifdef _RK_EQ_5_
    {FREQ_EQ,              0,  36000000,  36000000,  36000000,  36000000,         0,         0},
    #else
    {FREQ_EQ_L,            0,  36000000,  36000000,  36000000,  36000000,  36000000,  36000000},
    {FREQ_EQ,              0,  72000000,  72000000,  72000000,  72000000,  72000000,  72000000},
    {FREQ_EQ_H,            0, 140000000, 140000000, 140000000, 140000000, 140000000, 140000000},
    #endif
    {FREQ_JPG,             0, 250000000, 250000000, 250000000,  75000000,         0,         0},
    {FREQ_FM,              0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    //{FREQ_RECORDADPCM,     0,  56000000,  56000000,  56000000,  56000000,  56000000,  56000000},
    {FREQ_RECORDADPCM,     0, 120000000, 120000000, 120000000, 120000000, 200000000, 200000000},
    {FREQ_FMAUTOSEARCH,    0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIAUPDATA,     0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_USB,             0, 500000000, 250000000, 250000000,  75000000, 500000000, 500000000}, //USB 会锁频
    {FREQ_BEEP,            0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_LOW,   0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_DC_FILTER_HIGH,  0,  24000000,  24000000,  24000000,  24000000,         0,         0},
    {FREQ_MEDIA_INIT,      0, 100000000, 100000000, 100000000,  75000000,         0,         0},
    {FREQ_AVI,             0, 100000000, 100000000, 100000000,  75000000,         0,         0},

    {FREQ_MAX,             0, 500000000, 250000000, 250000000,  75000000, 300000000, 300000000},

    #endif
};
#endif


/*
--------------------------------------------------------------------------------
  Function name : void PMUSetARMFreq(uint32 nMHz)
  Author        :
  Description   : set arm main frequency,HCLK,PCLK.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:         ARM, HCLK, PCLK's frequency should in valid range
--------------------------------------------------------------------------------
*/
void FREQSetARMFreq(FREQ_APP_TABLE *FreqTab, chip_freq_t *pChipFreq)
{
    uint32 i,j,k;
    uint32 pllclk;
    uint32 syshclk_div;
    uint32 sysstclk_div;
    uint32 pclk_dkv;
    uint32 calhclk_div;
    uint32 calstclk_div;
    uint32 osc_24m_div;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    Grf_Memory_Resp_Enable(PMU_RAM | LOW_IRAM | LOW_DRAM, 1);
    Grf_Memory_Resp_Enable(HIGH_IRAM | HIGH_DRAM, 1);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    if ((FreqTab->syshclk <= 24000000) && (FreqTab->calhclk <= 24000000))
    {
        pllclk = 24000000;
        if (FreqTab->syshclk == 0)
        {
            FreqTab->syshclk = 24000000 / 64;
            FreqTab->sysstclk = FreqTab->syshclk;
            FreqTab->syspclk  = FreqTab->syshclk;
        }
        if (FreqTab->calhclk == 0)
        {
            FreqTab->calhclk = 24000000 / 64;
            FreqTab->calstclk = FreqTab->calhclk;
        }
        if (FreqTab->syshclk >= FreqTab->calhclk)
        {
            osc_24m_div = 24000000 / FreqTab->syshclk;
            pllclk = 24000000 / osc_24m_div;
        }
        else if (FreqTab->syshclk < FreqTab->calhclk)
        {
            osc_24m_div = 24000000 / FreqTab->calhclk;
            pllclk = 24000000 / osc_24m_div;
        }

        syshclk_div = pllclk / FreqTab->syshclk;
        calhclk_div = pllclk / FreqTab->calhclk;
    }
    else
    {
        if (FreqTab->syshclk == 0)
        {
            FreqTab->syshclk = FreqTab->calhclk;
        }
        else if (FreqTab->calhclk == 0)
        {
            FreqTab->calhclk = FreqTab->syshclk;
        }

        if (((FreqTab->PllFreq % FreqTab->syshclk) == 0) && ((FreqTab->PllFreq % FreqTab->calhclk) == 0))
        {
            pllclk       = FreqTab->PllFreq;
            syshclk_div  = FreqTab->PllFreq / FreqTab->syshclk;
            calhclk_div  = FreqTab->PllFreq / FreqTab->calhclk;
        }
        else if (FreqTab->syshclk > FreqTab->calhclk)
        {
            for (i = 1; i < 9; i++)
            {
                pllclk = FreqTab->calhclk * i;
                if (pllclk > 1600000000)
                {
                    pllclk = FreqTab->calhclk * (i - 1);
                    if (pllclk < FreqTab->syshclk)
                    {
                        pllclk = FreqTab->syshclk;
                    }
                    syshclk_div = pllclk / FreqTab->syshclk;
                    pllclk      = FreqTab->syshclk * syshclk_div;     //PLL 以系统频率对齐设定
                    calhclk_div = pllclk / FreqTab->calhclk;
                    if (calhclk_div > 8)
                    {
                        calhclk_div = 8;
                    }
                    break;
                }
                if ((pllclk % FreqTab->syshclk) == 0)
                {
                    calhclk_div = i;
                    syshclk_div = pllclk / FreqTab->syshclk;
                    break;
                }
            }

            if (i >= 8)
            {
                pllclk      = FreqTab->calhclk * 8;
                if (pllclk < FreqTab->syshclk)
                {
                    pllclk = FreqTab->syshclk;
                }
                syshclk_div = pllclk / FreqTab->syshclk;
                pllclk      = FreqTab->syshclk * syshclk_div;     //PLL 以系统频率对齐设定
                calhclk_div = pllclk / FreqTab->calhclk;
                if (calhclk_div > 8)
                {
                    calhclk_div = 8;
                }
            }
        }
        else // if (FreqTab->syshclk < FreqTab->calhclk)
        {
            for (i = 1; i < 9; i++)
            {
                pllclk = FreqTab->syshclk * i;
                if (pllclk > 1600000000)
                {
                    pllclk      = FreqTab->syshclk * (i - 1);
                    if (pllclk < FreqTab->calhclk)
                    {
                        pllclk = FreqTab->calhclk;
                    }
                    calhclk_div = pllclk / FreqTab->calhclk;
                    pllclk      = FreqTab->calhclk * calhclk_div;
                    syshclk_div = pllclk / FreqTab->syshclk;
                    if (pllclk / syshclk_div > 300000000)
                    {
                        syshclk_div++;
                    }
                    break;
                }
                if ((pllclk % FreqTab->calhclk) == 0)
                {
                    syshclk_div = i;
                    calhclk_div = pllclk / FreqTab->calhclk;
                    if (pllclk / syshclk_div > 300000000)
                    {
                        syshclk_div++;
                    }
                    if (syshclk_div > 8)
                    {
                        syshclk_div = 8;
                    }
                    break;
                }
            }
            if (i >= 8)
            {
                pllclk      = FreqTab->syshclk * 8;
                if (pllclk < FreqTab->calhclk)
                {
                    pllclk = FreqTab->calhclk;
                }
                calhclk_div = pllclk / FreqTab->calhclk;
                pllclk      = FreqTab->calhclk * calhclk_div;
                syshclk_div = pllclk / FreqTab->syshclk;
                if (pllclk / syshclk_div > 300000000)
                {
                    syshclk_div++;
                }
                if (syshclk_div > 8)
                {
                    syshclk_div = 8;
                }
            }
        }
    }

    if(pllclk != pChipFreq->pll)
    {
        pChipFreq->pll = pllclk;
        pChipFreq->armclk = pllclk;
        pChipFreq->armclk2 = pllclk;

        if (pChipFreq->pll > 24000000)
        {
            Get_PLL_Post_Div(pChipFreq->pll, &PllArg);
            Get_PLL_Pre_Div(PllArg.VCO, &PllArg);
        }
        else
        {
            memset(&PllArg, 0, sizeof(PLL_ARG_t));
            PllArg.div_con_24m = osc_24m_div;
        }

        //sys core hclk/fclk
        PllArg.sys_core_div = syshclk_div;
        pChipFreq->hclk_sys_core = pChipFreq->pll / syshclk_div;
        pChipFreq->fclk_sys_core  = pChipFreq->hclk_sys_core;
        //sys core stclk
        PllArg.sys_stclk_div = pChipFreq->hclk_sys_core / FreqTab->sysstclk;
        pChipFreq->stclk_sys_core = pChipFreq->hclk_sys_core / PllArg.sys_stclk_div;
        //sys core pclk
        for (i = 0; i < 4; i++)
        {
            j = (0x0001 << i);
            if ((FreqTab->syspclk * j) > pChipFreq->hclk_sys_core)
            {
                if (i == 0)
                {
                    PllArg.pclk_logic_div = 1;
                }
                else
                {
                    PllArg.pclk_logic_div = i;
                }
                pChipFreq->pclk_logic_pre = pChipFreq->hclk_sys_core / (j >> 1);
                break;
            }
        }

        //cal core hclk/fclk
        PllArg.cal_core_div = calhclk_div;
        pChipFreq->hclk_cal_core = pChipFreq->pll / calhclk_div;
        pChipFreq->fclk_cal_core = pChipFreq->hclk_cal_core;
        //cal core stclk
        PllArg.cal_stclk_div = pChipFreq->hclk_cal_core / FreqTab->calstclk;
        pChipFreq->stclk_cal_core = pChipFreq->hclk_cal_core / PllArg.cal_stclk_div;

        SetPllFreq(pllclk, &PllArg);

        SetHclkLogicPreFreq();
        SetPclkPmuPreFreq();
        SetHclkPmuPreFreq();

        SysTickPeriodSet(10);
    }
    else
    {
        if(FreqTab->syshclk != pChipFreq->hclk_sys_core)
        {
            pChipFreq->hclk_sys_core  = SetHclkSysCoreFreq(PLL_MUX_CLK, FreqTab->syshclk);
            pChipFreq->fclk_sys_core  = pChipFreq->hclk_sys_core;
            pChipFreq->stclk_sys_core = SetStclkSysCoreFreq(FreqTab->sysstclk);
            pChipFreq->pclk_logic_pre = SetPclkLogicPreFreq(FreqTab->syspclk);
            SysTickPeriodSet(10);
        }

        if(FreqTab->sysstclk != pChipFreq->stclk_sys_core)
        {
            pChipFreq->stclk_sys_core = SetStclkSysCoreFreq(FreqTab->sysstclk);
        }

        if(FreqTab->syspclk != pChipFreq->pclk_logic_pre)
        {
            pChipFreq->pclk_logic_pre = SetPclkLogicPreFreq(FreqTab->syspclk);
        }

        if(FreqTab->calhclk != pChipFreq->hclk_cal_core)
        {
            pChipFreq->hclk_cal_core  = SetHclkCalCoreFreq(PLL_MUX_CLK, FreqTab->calhclk);
            pChipFreq->fclk_cal_core  = pChipFreq->hclk_cal_core;
            pChipFreq->stclk_cal_core = SetStclkCalCoreFreq(FreqTab->calstclk);
        }

        if(FreqTab->calstclk != pChipFreq->stclk_cal_core)
        {
            pChipFreq->stclk_cal_core = SetStclkCalCoreFreq(FreqTab->calstclk);
        }
    }

    SetHclkLogicPreFreq();
    SetPclkPmuPreFreq();
    SetHclkPmuPreFreq();

    {
        uint32 temp = pChipFreq->hclk_sys_core/24000000;
        uint32 csrw = temp / 2;
        uint32 rwpw = temp / 1;
        uint32 rwcs = temp / 2;
        //VopSetTiming(0, csrw, rwpw, rwcs );     //LCD timing
        //BL_SetLevel(gSysConfig.BLevel);         //backlight
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //after frequency stable, the VDD voltage and memory can be set for low power
    if (pChipFreq->hclk_sys_core <= 180000000)
    {
        Grf_Memory_Resp_Enable(PMU_RAM | LOW_IRAM | LOW_DRAM, 0);
    }

    if (pChipFreq->hclk_cal_core <= 300000000)
    {
        Grf_Memory_Resp_Enable(HIGH_IRAM | HIGH_DRAM, 0);
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    printf("\nVCO=%d, pll=%d, shclk=%d, stck=%d, splk=%d, hhclk=%d, htck=%d\n",
          PllArg.VCO, pChipFreq->pll, pChipFreq->hclk_sys_core, pChipFreq->stclk_sys_core, pChipFreq->pclk_logic_pre,
          pChipFreq->hclk_cal_core, pChipFreq->stclk_cal_core);

    //printf("\n");
}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMUSetCodecFreq(uint32 nKHz)
  Author        :
  Description   : set CODEC frequency

  Input         : freq:codec frequency
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
int32 FREQSetCalCoreFreq(uint32 nMhz)
{

}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMUSetFreq(uint64 appList)
  Author        :
  Description   : set each PLL output frequency and bus frequency.

  Input         : appList:application list.
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
int32 FREQSetFreq(uint64 appList)
{
    uint32 i;
    uint32 FreqIdleDisable = 0;
    FREQ_APP_TABLE FreqAppTabl;

    memset(&FreqAppTabl, 0,sizeof(FREQ_APP_TABLE));

    if(appList & (((uint64)0x01) << FREQ_IDLE))
    {
        appList = ((uint64)0x01) << FREQ_IDLE;
        FreqIdleDisable = 1;

    }
    else if(appList == 0)
    {
        appList = ((uint64)0x01) << FREQ_MIN;
        FreqIdleDisable = 0;
    }

    for(i = 0; i < FREQ_APP_MAX; i++)
    {
        if(appList & ((uint64)0x01 << i))
        {
            FreqAppTabl.PllFreq  += g_CruAPPTabel[i].PllFreq;

            FreqAppTabl.syshclk  += g_CruAPPTabel[i].syshclk;
            FreqAppTabl.sysstclk += g_CruAPPTabel[i].sysstclk;
            FreqAppTabl.syspclk  += g_CruAPPTabel[i].syspclk;

            FreqAppTabl.calhclk  += g_CruAPPTabel[i].calhclk;
            FreqAppTabl.calstclk += g_CruAPPTabel[i].calstclk;
        }
    }

    //pll min:mix
    if((FreqAppTabl.PllFreq < g_CruAPPTabel[FREQ_MIN].PllFreq) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.PllFreq = g_CruAPPTabel[FREQ_MIN].PllFreq;
    }
    if(FreqAppTabl.PllFreq > g_CruAPPTabel[FREQ_MAX].PllFreq)
    {
        FreqAppTabl.PllFreq = g_CruAPPTabel[FREQ_MAX].PllFreq;
    }

    //syshclk min:mix
    if((FreqAppTabl.syshclk < g_CruAPPTabel[FREQ_MIN].syshclk) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.syshclk = g_CruAPPTabel[FREQ_MIN].syshclk;
    }
    if(FreqAppTabl.syshclk > g_CruAPPTabel[FREQ_MAX].syshclk)
    {
        FreqAppTabl.syshclk = g_CruAPPTabel[FREQ_MAX].syshclk;
    }

    //sysstclk min:mix
    if((FreqAppTabl.sysstclk < g_CruAPPTabel[FREQ_MIN].sysstclk) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.sysstclk = g_CruAPPTabel[FREQ_MIN].sysstclk;
    }
    if(FreqAppTabl.sysstclk > g_CruAPPTabel[FREQ_MAX].sysstclk)
    {
        FreqAppTabl.sysstclk = g_CruAPPTabel[FREQ_MAX].sysstclk;
    }

    //syspclk min:mix
    if((FreqAppTabl.syspclk < g_CruAPPTabel[FREQ_MIN].syspclk) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.syspclk = g_CruAPPTabel[FREQ_MIN].syspclk;
    }
    if(FreqAppTabl.syspclk > g_CruAPPTabel[FREQ_MAX].syspclk)
    {
        FreqAppTabl.syspclk = g_CruAPPTabel[FREQ_MAX].syspclk;
    }

    //calhclk min:mix
    if((FreqAppTabl.calhclk < g_CruAPPTabel[FREQ_MIN].calhclk) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.calhclk = g_CruAPPTabel[FREQ_MIN].calhclk;
    }
    if(FreqAppTabl.calhclk > g_CruAPPTabel[FREQ_MAX].calhclk)
    {
        FreqAppTabl.calhclk = g_CruAPPTabel[FREQ_MAX].calhclk;
    }

    //pll min:mix
    if((FreqAppTabl.calstclk < g_CruAPPTabel[FREQ_MIN].calstclk) && (FreqIdleDisable == 0))
    {
        FreqAppTabl.calstclk = g_CruAPPTabel[FREQ_MIN].calstclk;
    }
    if(FreqAppTabl.calstclk > g_CruAPPTabel[FREQ_MAX].calstclk)
    {
        FreqAppTabl.calstclk = g_CruAPPTabel[FREQ_MAX].calstclk;
    }

    FREQSetARMFreq(&FreqAppTabl, &chip_freq);

    return(0);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMUStartAPP(ePMU_APP appId)
  Author        :
  Description   : start one application

  Input         : appId: application serial number.
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
int32 FREQStartAPP(eFREQ_APP appId)
{
    if(g_APPList & ((uint64)0x01<< appId))//this application had enter.
    {
        return(0);
    }
    else
    {
        g_APPList |= ((uint64)0x01<< appId);
        FREQSetFreq(g_APPList);
        return(0);
    }

}

/*
--------------------------------------------------------------------------------
  Function name : PMUStopAPP
  Author        :
  Description   : stop one application

  Input         : appId:application serial number.
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
int32 FREQStopAPP(eFREQ_APP appId)
{
    if(appId >= FREQ_APP_MAX)
    {
        return (-1);
    }
    if(g_APPList & ((uint64)0x01<< appId))
    {
        g_APPList &= ~((uint64)0x01<< appId);
        FREQSetFreq(g_APPList);
    }
    else//this application had exit.
    {
        return(-1);
    }
    return(0);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMU_EnterModule(ePMU_APP modulename)
  Author        :
  Description   : set pll frequency.(arm freq)

  Input         :  nMHz  PLL 's frequency

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 FREQ_EnterModule(eFREQ_APP modulename)
{
    if (FreqDisableFlag == 0)
    {
        FREQStartAPP(modulename);
    }

    return(0);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMU_ExitModule(ePMU_APP modulename)
  Author        :
  Description   :

  Input         :  nMHz  PLL 's frequency

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 FREQ_ExitModule(eFREQ_APP modulename)
{
    if (FreqDisableFlag == 0)
    {
        FREQStopAPP(modulename);
    }
    else
    {
         g_APPDisableList |= ((uint64)0x01<< modulename);
    }

    return 0;
}

/*
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
*/
int32 FREQ_Control_Init(void)
{
    FreqDisableFlag = 0;
    FreqDisableCnt = 0;
}



int32 FREQ_Disable(void)
{
    FreqDisableFlag = 1;
    FreqDisableCnt++;
}

/*
--------------------------------------------------------------------------------
  Function name : int32 PMU_ExitModule(ePMU_APP modulename)
  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 FREQ_Enable(void)
{
    uint32 i;

    if(FreqDisableCnt >= 1)
    {
        FreqDisableCnt--;
    }

    if(FreqDisableCnt == 0)
    {
        FreqDisableFlag = 0;
        for (i = 0; i < 64; i++)
        {
            if(g_APPDisableList & ((uint64)0x01<< i))
            {
                g_APPDisableList &= ~((uint64)0x01 << i);
                FREQ_ExitModule(i);
            }
        }
    }
}

void SetSysFreq(uint32 nMhz)
{
     uint32 i;

     memset(&PllArg, 0, sizeof(PLL_ARG_t));

     if(nMhz > 24)
     {
         Get_PLL_Post_Div(nMhz * 1000 * 1000, &PllArg);
         Get_PLL_Pre_Div(PllArg.VCO, &PllArg);
     }
     else
     {
        if(24 % nMhz)
        {
            for(i = 0; i < 24; i++)
            {
                if((24 % i) == 0)
                {
                    if(i > nMhz)
                    {
                        nMhz = i;
                        break;
                    }
                }
            }
        }

     }

     PllArg.cal_core_div = 1;
     PllArg.cal_stclk_div = 1;
     PllArg.sys_core_div = 1;
     PllArg.sys_stclk_div = 1;
     PllArg.pclk_logic_div = 4;
     PllArg.div_con_24m = 24 / nMhz;

     chip_freq.armclk = nMhz * 1000 * 1000;
     chip_freq.hclk_sys_core = nMhz * 1000 * 1000;
     chip_freq.pclk_logic_pre = chip_freq.hclk_sys_core / PllArg.pclk_logic_div;

     SetPllFreq(nMhz * 1000 * 1000, &PllArg);

     SysTickPeriodSet(10);


}


#if 0
void SysSuspend()
{
    //Grf_NOC_Remap_Sel(NOC_PMU_SRAM);


    //PmuPdLogicPowerDown(1);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin0,IOMUX_GPIO2A0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin1,IOMUX_GPIO2A1_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin2,IOMUX_GPIO2A2_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin4,IOMUX_GPIO2A4_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin5,IOMUX_GPIO2A5_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin6,IOMUX_GPIO2A6_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin7,IOMUX_GPIO2A7_IO);

    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin0,IOMUX_GPIO2B0_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin1,IOMUX_GPIO2B1_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin2,IOMUX_GPIO2B2_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin3,IOMUX_GPIO2B3_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin4,IOMUX_GPIO2B4_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin5,IOMUX_GPIO2B5_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin6,IOMUX_GPIO2B6_IO);
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortB_Pin7,IOMUX_GPIO2B7_IO);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin0,IOMUX_GPIO2C0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin1,IOMUX_GPIO2C1_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin2,IOMUX_GPIO2C2_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin3,IOMUX_GPIO2C3_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin4,IOMUX_GPIO2C4_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin5,IOMUX_GPIO2C5_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin6,IOMUX_GPIO2C6_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortC_Pin7,IOMUX_GPIO2C7_IO);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin0,IOMUX_GPIO2D0_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin1,IOMUX_GPIO2D1_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin2,IOMUX_GPIO2D2_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin3,IOMUX_GPIO2D3_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin4,IOMUX_GPIO2D4_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin5,IOMUX_GPIO2D5_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin6,IOMUX_GPIO2D6_IO);
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortD_Pin7,IOMUX_GPIO2D7_IO);

    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin6,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin7,GPIO_IN);

    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin0,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin1,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin2,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin3,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin4,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin5,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin6,GPIO_IN);
    //Gpio_SetPinDirection(GPIO_CH2,GPIOPortB_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortC_Pin7,GPIO_IN);

    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin0,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin1,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin2,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin3,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin4,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin5,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin6,GPIO_IN);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortD_Pin7,GPIO_IN);

    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin0,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin1,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin2,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin3,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin4,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin5,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin6,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortA_Pin7,DISABLE);

    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin0,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin1,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin2,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin3,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin4,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin5,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin6,DISABLE);
    //Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortB_Pin7,DISABLE);

    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin0,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin1,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin2,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin3,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin4,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin5,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin6,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortC_Pin7,DISABLE);

    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin0,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin1,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin2,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin3,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin4,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin5,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin6,DISABLE);
    Grf_GPIO_SetPinPull(GPIO_CH2,GPIOPortD_Pin7,DISABLE);


   // Grf_PVTM_Set_Cal_Cnt(1000);
   // Grf_PVTM_OSC_Enable(1);
   // Grf_PVTM_Fun_Start(1);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_CLK_OBS);
    OBS_output_Source_sel(obs_clk_pvtm);

   // Grf_PVTM_Set_Divider(127, 1);


    PmuGpioPosedegePlusWakeUpEnable(GPIOPortA_Pin5);

    PmuSelLowFreq(PMU_LOW_FREQ_CLK_SEL_PVTM);

    PmuSetPowerMode(PMU_POWER_MODE_POWER_DOWN_VD_LOGIC
                    | PMU_POWER_MODE_USE_LOW_FREQ_EN
                    | PMU_POWER_MODE_OSC_POWER_DOWN_EN
                    | PMU_POWER_MODE_PLL_POWER_DOWN_EN
                    | PMU_POWER_MODE_EN);

    DelayMs(100);

   __WFI();
}
#endif

_ATTR_PMU_CODE_
void PmuResume()
{
#if 0
    UINT32 KeyTemp;

    //initialization for global variable.
    SysVariableInit();

    #ifdef _FILE_DEBUG_
    SysDebugHookInitMem();
    #endif

    //PLL setting,disable interrupt and PWM.
    SysCpuInit();
    IntMasterDisable(); /*disable int for sysinit*/

    //ADC initializtion, low power check,LCD initialization, Codec initialization.
    BoardInit();

    //interruption initialization.
    SysIntrruptInit();

    //system tick initialization.
    SysTickInit();

    //Flash initial,get system parameter, file system initial
    MediaInit();

    //文件系统初始化之后才能初始化调试文件
    #ifdef _FILE_DEBUG_
    SysDebugHookInit();
    #endif

    //module initialization.
    ModuleInit();

    SysServiceInit();

    while(1);
    #endif
}

#if 0
//#define _ATTR_PMU_CODE_              __attribute__((section("pmucode")))
_ATTR_PMU_CODE_
void PowerTest(void)
{

    FREQ_EnterModule(FREQ_INIT);

    //jtag disable
    Grf_Force_Jtag_Set(0);

    //GPIO IDLE
    Grf_GPIO_PortMux_Set(GPIO_CH0, GPIO_PORTA, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH0, GPIO_PORTB, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH0, GPIO_PORTC, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH0, GPIO_PORTD, 0x0000);

    Grf_GPIO_PortMux_Set(GPIO_CH1, GPIO_PORTA, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH1, GPIO_PORTB, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH1, GPIO_PORTC, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH1, GPIO_PORTD, 0x0000);

    Grf_GPIO_PortMux_Set(GPIO_CH2, GPIO_PORTA, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH2, GPIO_PORTB, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH2, GPIO_PORTC, 0x0000);
    Grf_GPIO_PortMux_Set(GPIO_CH2, GPIO_PORTD, 0x0000);

    Gpio_SetPortDirec(GPIO_CH0,0x00000000);
    Gpio_SetPortDirec(GPIO_CH1,0x00000000);
    Gpio_SetPortDirec(GPIO_CH2,0x00000000);

    Grf_GPIO_PortPull_Set(GPIO_CH0, GPIO_PORTA, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH0, GPIO_PORTB, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH0, GPIO_PORTC, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH0, GPIO_PORTD, 0xff);

    Grf_GPIO_PortPull_Set(GPIO_CH1, GPIO_PORTA, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH1, GPIO_PORTB, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH1, GPIO_PORTC, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH1, GPIO_PORTD, 0xff);

    Grf_GPIO_PortPull_Set(GPIO_CH2, GPIO_PORTA, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH2, GPIO_PORTB, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH2, GPIO_PORTC, 0xff);
    Grf_GPIO_PortPull_Set(GPIO_CH2, GPIO_PORTD, 0xff);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin6,IOMUX_GPIO2A6_IO);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin6,GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH2,GPIOPortA_Pin6,GPIO_HIGH);

    //USB Init:disable
    UsbIntControlInit();
    Grf_otgphy_suspend(1);

    #if 0
    //ACODEC Power Off
    Codec_PowerOnInitial();
    #endif

    #if 1
    //ADC Vref powerOff
    Grf_Set_ADC_Vref_PowerOn(GRF_VREF_PWOFF);
    AdcPowerDown();
    #else
    Grf_Set_ADC_Vref_PowerOn(GRF_VREF_PWON);
    AdcPowerUp();
    #endif

    //High Core init:disable
    PmuPdLogicPowerDown(1);

    //softreset init
    CpuSoftResetInit();

    //clockgate init
    CpuClockGateInit();

    #if 0
    //OSC Stop
    Pmu_Reg->PMU_SOFTRST_CON = 0x00010001;
    Pmu_Reg->PMU_SOFT_CON = 0x00010001;
    Pmu_Reg->PMU_SOFT_CON |= PMU_OSC_POWER_DOWN_EN;
    #endif

    #if 0
    //OSC off, PVTM On
    Grf_PVTM_Set_Cal_Cnt(1000);
    Grf_PVTM_OSC_Enable(1);
    Grf_PVTM_Fun_Start(1);
    Grf_PVTM_Set_Divider(127, 1);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin7,IOMUX_GPIO2A7_PMU_IDEL);
    PmuSelLowFreq(PMU_LOW_FREQ_CLK_SEL_PVTM);
    PmuSetPowerMode(PMU_POWER_MODE_POWER_DOWN_VD_LOGIC |
                    PMU_POWER_MODE_USE_LOW_FREQ_EN   |
                    PMU_POWER_MODE_OSC_POWER_DOWN_EN |
                    PMU_POWER_MODE_PLL_POWER_DOWN_EN |
                    PMU_POWER_MODE_EN);
    #endif

    DelayMs(1000);
    __WFI();
    while(1);


}

void PowerTest(void)
{
    Grf->GRF_INTER_CON0 = FORCE_JTAG_MASK << 16 | FORCE_JTAG_DIS ;
    Grf->GPIO_IO0MUX[0] = 0xffff0000;
    Grf->GPIO_IO0MUX[1] = 0xffff0000;
    Grf->GPIO_IO0MUX[2] = 0xffff0000;
    Grf->GPIO_IO0MUX[3] = 0xffff0000;

    Grf->GPIO_IO1MUX[0] = 0xffff0000;
    Grf->GPIO_IO1MUX[1] = 0xffff0000;
    Grf->GPIO_IO1MUX[2] = 0xffff0000;
    Grf->GPIO_IO1MUX[3] = 0xffff0000;

    Grf->GPIO_IO2MUX[0] = 0xffff0000;
    Grf->GPIO_IO2MUX[1] = 0xffff0000;
    Grf->GPIO_IO2MUX[2] = 0xffff0000;
    Grf->GPIO_IO2MUX[3] = 0xffff0000;


    Gpio_SetPortDirec(GPIO_CH0,0x00000000);
    Gpio_SetPortDirec(GPIO_CH1,0x00000000);
    Gpio_SetPortDirec(GPIO_CH2,0x00000000);

    {
        //pull disable
        Grf->GPIO_IO0PULL[0] = 0xffffffff;
        Grf->GPIO_IO0PULL[1] = 0xffffffff;
        Grf->GPIO_IO0PULL[2] = 0xffffffff;
        Grf->GPIO_IO0PULL[3] = 0xffffffff;

        Grf->GPIO_IO1PULL[0] = 0xffffffff;
        Grf->GPIO_IO1PULL[1] = 0xffffffff;
        Grf->GPIO_IO1PULL[2] = 0xffffffff;
        Grf->GPIO_IO1PULL[3] = 0xffffffff;

        Grf->GPIO_IO2PULL[0] = 0xffffffff;
        Grf->GPIO_IO2PULL[1] = 0xffffffff;
        Grf->GPIO_IO2PULL[2] = 0xffffffff;
        Grf->GPIO_IO2PULL[3] = 0xffffffff;
    }

    //Power On
    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin6,IOMUX_GPIO2A6_IO);
    Gpio_SetPinDirection(GPIO_CH2,GPIOPortA_Pin6,GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH2,GPIOPortA_Pin6,GPIO_HIGH);

    Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin7,IOMUX_GPIO2A7_PMU_IDEL);

    Grf->GRF_UOC_CON1 = 0x000f0005;
    Grf->GRF_UOC_CON2 = 0x00010001;
    Grf->GRF_USBPHY_CON6 = 0x00040000;
    Grf->GRF_USBPHY_CON7 = 0x80008000;
    ScuClockGateCtr(CLK_USBPHY_GATE, 0);
    ScuClockGateCtr(HCLK_USBC_GATE, 0);

    ScuClockGateCtr(CLK_ACODEC_GATE, 0);      //ACODEC gating open
    ScuClockGateCtr(PCLK_ACODEC_GATE, 0);     //PCLK ACODEC gating open

    Grf_Set_ADC_Vref_PowerOn(GRF_VREF_PWOFF);
    AdcPowerDown();

    PmuPdLogicPowerDown(1);

    #if 0
    Grf_PVTM_Set_Cal_Cnt(1000);
    Grf_PVTM_OSC_Enable(1);
    Grf_PVTM_Fun_Start(1);
    Grf_PVTM_Set_Divider(127, 1);

    PmuSelLowFreq(PMU_LOW_FREQ_CLK_SEL_PVTM);
    Pmu_Reg->PMU_SOFTRST_CON = 0x00010001;
    Pmu_Reg->PMU_SOFT_CON = 0x00010001;
    Pmu_Reg->PMU_SOFT_CON |= PMU_OSC_POWER_DOWN_EN;

    #else

    Grf_PVTM_Set_Cal_Cnt(1000);
    Grf_PVTM_OSC_Enable(1);
    Grf_PVTM_Fun_Start(1);
    Grf_PVTM_Set_Divider(127, 1);

    PmuGpioPosedegePlusWakeUpEnable(GPIOPortA_Pin5);

    PmuSelLowFreq(PMU_LOW_FREQ_CLK_SEL_PVTM);
    PmuSetPowerMode(PMU_POWER_MODE_POWER_DOWN_VD_LOGIC
                    | PMU_POWER_MODE_USE_LOW_FREQ_EN
                    | PMU_POWER_MODE_OSC_POWER_DOWN_EN
                    | PMU_POWER_MODE_PLL_POWER_DOWN_EN
                    | PMU_POWER_MODE_EN);
    #endif

    DelayMs(1000);
    __WFI();
    while(1);

}
#endif

/*
********************************************************************************
*
*                         End of Pll.c
*
********************************************************************************
*/

