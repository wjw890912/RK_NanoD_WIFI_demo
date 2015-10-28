/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   PMU.c
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/
#define _IN_PMU

#include <typedef.h>
#include <DriverInclude.h>


/*
--------------------------------------------------------------------------------

                        Variable Define

--------------------------------------------------------------------------------
*/
//typedef __int64 uint64 ;

//_ATTR_DRIVER_BSS_
SCU_CLK_INFO    g_chipClk;
//_ATTR_DRIVER_BSS_
uint64          g_APPList;
//_ATTR_DRIVER_DATA_
static int      is_codec_sp_mode = 0;

uint32 g_PmuCnt[PMU_APP_MAX];


#ifdef _LCD_SIZE_20_
#define PMU_BLON_FREQ   48
#else
#define PMU_BLON_FREQ   24
#endif

//_ATTR_DRIVER_DATA_
PMU_APP_TABLE g_scuAPPTabel[PMU_APP_MAX] =
{
    {PMU_MIN,         0,  0,  0},      //
    {PMU_INIT,        0,  0,  0},      //
    {PMU_BLON,        0,  24,  0},      //
    {PMU_AUDIO_INIT,  0,  24,  0},      //
    {PMU_MP3,         0,  12,  0},      //
    {PMU_MP3H,        0,  30,  0},      //
    {PMU_WMA,         0,  50,  0},      //
	{PMU_WMAH,		  0,  60,  0},		//
    {PMU_WAV,         0,  48,  0},
    {PMU_AAC,         0,  48,  0},
    {PMU_APE,         0,  80,  0},
    {PMU_FLAC,        0,  100, 0},
    {PMU_EQ,          0,  36,  0},      //
	{PMU_JPG,         0,  100, 0},
	{PMU_FM,          0,  13,  0},	    //24+13的倍频需要落在FM频段范围以外
    {PMU_RECORDADPCM, 0,  56,  0},
    {PMU_RECORDMP3,   0,  120, 0},      //
    {PMU_FMAUTOSEARCH, 0, 24,  0},
    {PMU_MEDIAUPDATA, 0,  140, 0},
    {PMU_USB,         0,  140, 0},      //
    {PMU_AVI,         0,  140, 0},      //{PMU_AVI,         0,  120, 0},

    //PMU Max
    #ifdef _EMMC_
    {PMU_MAX,         0,  120, 0},      //
    #else
    {PMU_MAX,         0,  140, 0},      //
    #endif
};

/*
--------------------------------------------------------------------------------
  Function name : void PMUSetCodecClockMode(int i)
  Author        :
  Description   :

  Input         :
  Return        :

  History:     <author>         <time>         <version>
                                2009-3-24         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void PMUSetCodecClockMode(int i)
{
    is_codec_sp_mode = i;
}

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
void PMUSetARMFreq(uint32 nMHz)
{
    #if 0
    if (nMHz > g_chipClk.armFreq)
    {
        if (nMHz > 24)
        {
            ScuDCoutSet(SCU_DCOUT_120);
        }
        else if (nMHz > 3)
        {
            ScuDCoutSet(SCU_DCOUT_100);
        }
        else
        {
            ScuDCoutSet(SCU_DCOUT_080);
        }
    }
    #endif

    SysFreqSet(nMHz);
    g_chipClk.armFreq = nMHz;

    #if 0
    if (nMHz < g_chipClk.armFreq)
    {
        if (g_chipClk.armFreq <= 3)
        {
            ScuDCoutSet(SCU_DCOUT_080);
        }
        else if (g_chipClk.armFreq <= 24)
        {
            ScuDCoutSet(SCU_DCOUT_100);
        }
        else
        {
            ScuDCoutSet(SCU_DCOUT_120);
        }
    }
    #endif

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
int32 PMUSetFreq(uint64 appList)
{
    uint32 armFreq = 0;
    //uint32 dspFreq = 0;
    uint32 i;

    if(!appList)
    {
        appList = PMU_MIN;
    }
    for(i = 0; i < PMU_APP_MAX; i++)
    {
        if(appList & ((uint64)0x01 << i))
        {
            armFreq += g_scuAPPTabel[i].armFreq;
            //dspFreq += g_scuAPPTabel[i].dummyFreq;
        }
    }

    if(armFreq < g_scuAPPTabel[PMU_MIN].armFreq)
    {
        armFreq = g_scuAPPTabel[PMU_MIN].armFreq;
    }

    if(armFreq > g_scuAPPTabel[PMU_MAX].armFreq)
    {
        armFreq = g_scuAPPTabel[PMU_MAX].armFreq;
    }

    PMUSetARMFreq(armFreq);
    //PMUSetDSPFreq(dspFreq);

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
int32 PMUStartAPP(ePMU_APP appId)
{
    if(g_APPList & ((uint64)0x01<< appId))//this application had enter.
    {
        return(0);
    }
    else
    {
        g_APPList |= ((uint64)0x01<< appId);
        PMUSetFreq(g_APPList);
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
int32 PMUStopAPP(ePMU_APP appId)
{
    if(appId >= PMU_APP_MAX)
    {
        return (-1);
    }
    if(g_APPList & ((uint64)0x01<< appId))
    {
        g_APPList &= ~((uint64)0x01<< appId);
        PMUSetFreq(g_APPList);
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
int32 PMU_EnterModule(ePMU_APP modulename)
{
    if(g_PmuCnt[modulename] < 0xffffffff)
        g_PmuCnt[modulename]++;

    if(g_PmuCnt[modulename] == 1)
        PMUStartAPP(modulename);

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
int32 PMU_ExitModule(ePMU_APP modulename)
{
    if(g_PmuCnt[modulename])
        g_PmuCnt[modulename]--;


    if(!g_PmuCnt[modulename])
        PMUStopAPP(modulename);

    return 0;
}

/*
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
*/
int32 PMU_Control_Init(void)
{
    uint32 i;

    for(i = 0; i < PMU_APP_MAX; i++)
    {
        g_PmuCnt[i] = 0;
    }
    return 0;
}
/*
********************************************************************************
*
*                         End of Pll.c
*
********************************************************************************
*/

