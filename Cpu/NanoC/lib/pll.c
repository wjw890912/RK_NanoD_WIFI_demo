/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   scu.c
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/
#define _PLL_IN_

#include <typedef.h>
#include <BspConfig.h>
#include <DriverInclude.h>
#include <FreeRTOS.h>


#define MAX_SDMMC_CLK  48

#if (defined(_SDCARD_) || defined(_EMMC_))
extern void SDC_UpdateClkHook(uint32 SDCPort);
#endif

/*
--------------------------------------------------------------------------------
  Function name : Pll_SetFreq()
  Author        : anzhiguo
  Description   :

  Input         :  nMHz: pll out clock (arm clock, ahb clock)

  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
  note:    CLK_OUT=24*M/(N*OD*2);M>=2;N>=2,OD>=1; 100MHz<+CLK_OUTxOD<=300MHz

--------------------------------------------------------------------------------
*/
void FreqDiv24Set(uint32 div)
{
    Scu->CLKSEL_CPU = ((X24MIN_CLKDIV_MASK << 16) | div) << X24MIN_CLKDIV_SHIFT;
}

/*
--------------------------------------------------------------------------------
  Function name : Pll_SetFreq()
  Author        : anzhiguo
  Description   :

  Input         :  nMHz: pll out clock (arm clock, ahb clock)

  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
  note:    CLK_OUT=24*M/(N*OD*2);M>=2;N>=2,OD>=1; 100MHz<+CLK_OUTxOD<=300MHz

--------------------------------------------------------------------------------
*/
uint32 PllSetFreq(UINT32 nMHz)
{
    UINT32  temp, div_num;

    if( nMHz <= 24 )
    {
        Scu->CLKSEL_CPU = ((ARMCLK_SEL_MASK << 16) | ARMCLK_SEL_24M) << ARMCLK_SEL_SHIFT;

        if (nMHz < 3) nMHz = 3;

        div_num = (24 / nMHz) - 1;
        Scu->CLKSEL_CPU = ((AHBCLK_CLKDIV_MASK << 16) | div_num) << AHBCLK_CLKDIV_SHIFT;
        nMHz = 24 / (div_num + 1);

        Scu->PLL_CON |= PLL_RESET;
        Scu->PLL_CON |= PLL_POWER_DOWN;
    }
    else
    {
        Scu->CLKSEL_CPU = ((ARMCLK_SEL_MASK << 16) | ARMCLK_SEL_24M) << ARMCLK_SEL_SHIFT;
        Scu->CLKSEL_CPU = ((AHBCLK_CLKDIV_MASK << 16) | 0) << AHBCLK_CLKDIV_SHIFT;

        Scu->PLL_CON &= ~PLL_POWER_DOWN;
        Scu->PLL_CON |= PLL_RESET;
        Scu->PLL_CON &= ~0X01; // our pll reset , this  value is not 0
        if (nMHz < 40)
        {
            temp = (nMHz << PLL_CLKM_SHIFT) | (2 << PLL_CLKN_SHIFT) | (6 << PLL_CLKOD_SHIFT);
        }
        else if (nMHz < 60)
        {
            temp = (nMHz << PLL_CLKM_SHIFT) | (3 << PLL_CLKN_SHIFT) | (4 << PLL_CLKOD_SHIFT);
        }
        else if (nMHz < 130)
        {
            temp = (nMHz << PLL_CLKM_SHIFT) | (6 << PLL_CLKN_SHIFT) | (2 << PLL_CLKOD_SHIFT);
        }
        else
        {
            temp = (nMHz << PLL_CLKM_SHIFT) | (12 << PLL_CLKN_SHIFT) | (1 << PLL_CLKOD_SHIFT);
        }

        Scu->PLL_CON = (Scu->PLL_CON & 0xfffe0001) | temp;
        Scu->PLL_CON &= ~PLL_RESET;

        temp = 200000;
        while(--temp > 0)
        {
            Delay10cyc(10);
            if( Scu->PLL_CON & PLL_POWER_LOCK)
            {
                break;
            }
        }

        Scu->CLKSEL_CPU = ((ARMCLK_SEL_MASK << 16) | ARMCLK_SEL_PLL) << ARMCLK_SEL_SHIFT;

    }

    return(nMHz);
}

/*
--------------------------------------------------------------------------------
  Function name : SetPclkFreq()
  Author        : anzhiguo
  Description   : set apb clock para

  Input         : armclk

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         pclk < 50M
--------------------------------------------------------------------------------
*/
uint32 SetPclkFreq(UINT32 nMHz)
{
    UINT32 temp, div_num;

    div_num = (nMHz / 50);
    Scu->CLKSEL_CPU = ((APBCLK_CLKDIV_MASK << 16) | div_num) << APBCLK_CLKDIV_SHIFT;

    chip_freq.pclk = nMHz/(div_num + 1);

    return(chip_freq.pclk);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetPclkFreq(void)
  Author        : anzhiguo
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:        get apb clock
--------------------------------------------------------------------------------
*/
UINT32 GetPclkFreq(void)
{
    return (chip_freq.pclk);
}




/*
--------------------------------------------------------------------------------
  Function name : SetSysTickFreq()
  Author        : anzhiguo
  Description   : set systick clock para

  Input         : armclk

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         systick = amclk/4
--------------------------------------------------------------------------------
*/
uint32 SetSystickFreq(UINT32 nMHz)
{
    UINT32 div_num;

    div_num = 3;
    Scu->CLKSEL_CPU = ((SYSTICK_CLKDIV_MASK << 16) | div_num) << SYSTICK_CLKDIV_SHIFT;

    chip_freq.systickclk = nMHz / (div_num + 1);

    return(chip_freq.systickclk);
}


/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetStclkFreq(void)
  Author        : anzhiguo
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
               anzhiguo        2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
UINT32 GetStclkFreq(void)
{
    return (chip_freq.systickclk);
}


/*
--------------------------------------------------------------------------------
  Function name : void SysFreqSet(uint32 nMhz)
  Author        :
  Description   : set system clock

  Input         :  nMHz

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         if system clock update, systick,apb,spi,sdmmc should update
--------------------------------------------------------------------------------
*/
void SysFreqSet(uint32 nMhz)
{
    if(nMhz == chip_freq.armclk)
    {
        return;
    }

    if (nMhz < 3)
    {
        PllSetFreq(25);             //切换至PLL输出
        if (nMhz == 2)
        {
            chip_freq.armclk = 2;
            FreqDiv24Set(2);        //设置分频参数: 1.5MHz
        }
        else
        {
            chip_freq.armclk = 1;
            FreqDiv24Set(3);        //设置分频参数: 0.75MHz
        }
        PllSetFreq(3);              //切换至24MHz分频输出
    }
    else
    {
        if (chip_freq.armclk < 3)
        {
            PllSetFreq(25);         //切换至 PLL 输出
            FreqDiv24Set(0);        //清除分频参数
        }
        chip_freq.armclk = PllSetFreq(nMhz);
    }


    //chip_freq.systickclk = SetSystickFreq(chip_freq.armclk);
    chip_freq.pclk       = SetPclkFreq(chip_freq.armclk);
    //chip_freq.spiclk     = SetSpiFreq(chip_freq.armclk);

    //FlashWaitSet(chip_freq.armclk);

    SysTickPeriodSet(10);
	//TimerPeriodSet(TimerTickBak);

    #if 0
    if (chip_freq.armclk > 24)
    {
        Scu_Force_PWM_Mode_Set(1);
    }
    else
    {
        Scu_Force_PWM_Mode_Set(0);
    }
    #endif

    //rk_printf("armclk = %dMHz, pclk = %dMHz\n", chip_freq.armclk, chip_freq.pclk);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetPllSysFreq(void)
  Author        : anzhiguo
  Description   : get system clock

  Input         :
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
UINT32 GetPllSysFreq(void)
{
    return (chip_freq.armclk);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 SetAdcFreq(UINT32 nMHz)
  Author        : anzhiguo
  Description   : set adc out clock

  Input         :

  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         adc input clock fix to 24M
--------------------------------------------------------------------------------
*/
uint32 SetAdcFreq(UINT32 nMHz)
{
    UINT32 div_num;

    div_num = (24 / nMHz) - 1;
    Scu->CLKSEL_SARADC = ((ADC_CLK_DIV_MASK << 16) | div_num) << ADC_CLK_DIV_SHIFT;

    chip_freq.adcclk = 24 / (div_num + 1);

    return(1);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetAdcClkFreq(void)
  Author        : anzhiguo
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
UINT32 GetAdcClkFreq(void)
{
    return (chip_freq.adcclk);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 SetI2sFreq(UINT32 nMHz)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         I2s Mclk fix to 12M
--------------------------------------------------------------------------------
*/
uint32 SetI2sFreq(UINT32 nMHz)
{
    UINT32 div_num;

    Scu->CLKSEL_CODEC = ((I2S_CLK_SEL_MASK << 16) | I2S_CLK_SEL_12M);

    return(1);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 SetUartFreq(UINT32 nMHz)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:        uart clk fix to 24M
--------------------------------------------------------------------------------
*/
uint32 SetUartFreq(UINT32 nMHz)
{
    UINT32 div_num;

    Scu->CLKSEL_UART = ((UART_CLK_SEL_MASK << 16) | UART_CLK_SEL_24M);

    return(1);
}



/*
--------------------------------------------------------------------------------
  Function name : void FlashWaitSet(UINT32 nMHz)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void FlashWaitSet(UINT32 nMHz)
{
    if(nMHz > 100)
    {
        NandCtl->FMWait = 0x1081;
    }
    else if(nMHz > 50)
    {
        NandCtl->FMWait = 0x0061;
    }
    else
    {
        NandCtl->FMWait = 0x0040;
    }
}


/*
--------------------------------------------------------------------------------
  Function name : uint32 SetSdmmcFreq(UINT32 nMHz)
  Author        : anzhiguo
  Description   : set sdmmc clock para

  Input         : armclk
  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         sdmmc < 50M
--------------------------------------------------------------------------------
*/
uint32 SetSdmmcFreq(UINT32 nMHz)
{
    UINT32 temp, div_num;

    #if 0
    if (Scu->PLL_CON & PLL_POWER_DOWN)
    {
        div_num = 0;
        chip_freq.mmcclk = 24;
    }
    else
    {
        div_num = ((nMHz + MAX_SDMMC_CLK-1) / MAX_SDMMC_CLK) - 1;
        chip_freq.mmcclk = nMHz/(div_num + 1);
    }
    Scu->CLKSEL_MMC = ((Scu->PLL_CON & PLL_POWER_DOWN)? 1:0)|(div_num << MMC_CLK_DIV_SHIFT)|(MMC_CLK_SEL_MASK << 16)|(MMC_CLK_DIV_MASK<<17);
    #else
    div_num = 0;
    chip_freq.mmcclk = 24;
    Scu->CLKSEL_MMC = 1 |(div_num << MMC_CLK_DIV_SHIFT)|(MMC_CLK_SEL_MASK << 16)|(MMC_CLK_DIV_MASK<<17);
    #endif

    #if (defined(_SDCARD_) || defined(_EMMC_))
    SDC_UpdateClkHook(0);
    #endif

    return(chip_freq.mmcclk);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetSdmmcFreq(void)
  Author        : anzhiguo
  Description   :

  Input         :
  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
UINT32 GetSdmmcFreq(void)
{
    return (chip_freq.mmcclk);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 SetSpiFreq(UINT32 nMHz)
  Author        : anzhiguo
  Description   : set spi clock para

  Input         : arm clk

  Return        : null

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:         spi clock < 60M
--------------------------------------------------------------------------------
*/
uint32 SetSpiFreq(UINT32 nMHz)
{
    UINT32 div_num;

    if((nMHz > 24) && (chip_freq.armclk > 24))
    {
        div_num = chip_freq.armclk / nMHz;
        Scu->CLKSEL_SPI = ((SPI_CLK_DIV_MASK << 16) | div_num) << SPI_CLK_DIV_SHIFT;
        chip_freq.spiclk = chip_freq.armclk / (div_num + 1);
    }
    else
    {
        Scu->CLKSEL_SPI = ((SPI_CLK_SEL_MASK << 16) | SPI_CLK_SEL_24M);
        chip_freq.spiclk = 24;
    }

    return(chip_freq.spiclk);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 GetSpiFreq(void)
  Author        : anzhiguo
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
               anzhiguo     2009-3-24         Ver1.0
  desc:
--------------------------------------------------------------------------------
*/
UINT32 GetSpiFreq(void)
{
    return (chip_freq.spiclk);
}


/*
********************************************************************************
*
*                         End of Pll.c
*
********************************************************************************
*/


