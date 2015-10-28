/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name£º   Pll.h
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _PLL_H
#define _PLL_H

#undef EXT
#ifdef _PLL_IN_
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                         Macro Define
*
*-------------------------------------------------------------------------------
*/

/*
*-------------------------------------------------------------------------------
*
*                         struct Define
*
*-------------------------------------------------------------------------------
*/

typedef  struct chip_freq_st
{
    UINT32  armclk;
    UINT32  pclk;
    UINT32  spiclk;
    UINT32  adcclk;
    UINT32  systickclk;
    UINT32  mmcclk;

}chip_freq_t;
EXT chip_freq_t chip_freq;

/*
*-------------------------------------------------------------------------------
*
*                         function declaration
*
*-------------------------------------------------------------------------------
*/
void FreqDiv24Set(uint32 div);
UINT32 GetPclkFreq(void);
UINT32 GetSdmmcFreq(void);
UINT32 GetStclkFreq(void);
UINT32 GetSpiFreq(void);
void SysFreqSet(uint32 nMhz);
UINT32 GetPllSysFreq(void);
uint32 SetAdcFreq(UINT32 nMHz);
UINT32 GetAdcClkFreq(void);
uint32 SetI2sFreq(UINT32 nMHz);
uint32 SetUartFreq(UINT32 nMHz);
void LcdWaitSet(UINT32 nMHz);
void FlashWaitSet(UINT32 nMHz);

/*
********************************************************************************
*
*                         End of Pll.h
*
********************************************************************************
*/
#endif

