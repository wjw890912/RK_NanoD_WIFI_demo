/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name£º   Pmu.h
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _PMU_H
#define _PMU_H

#undef  EXT
#ifdef _IN_PMU
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                           Macro define
*
*-------------------------------------------------------------------------------
*/
typedef enum _PMU_APP
{
    PMU_MIN = 0,
    PMU_INIT,
    PMU_BLON,
    PMU_AUDIO_INIT,
    PMU_MP3,
    PMU_MP3H,
    PMU_WMA,
    PMU_WMAH,
    PMU_AAC,
    PMU_WAV,
    PMU_APE,
    PMU_FLAC,
    PMU_EQ,
	PMU_JPG,
	PMU_FM,
    PMU_RECORDADPCM,
    PMU_RECORDMP3,
    PMU_FMAUTOSEARCH,
    PMU_MEDIAUPDATA,
    PMU_USB,
    PMU_AVI,
    PMU_MAX,

    PMU_APP_MAX

}ePMU_APP;

/*
*-------------------------------------------------------------------------------
*
*                           Struct define
*
*-------------------------------------------------------------------------------
*/
typedef struct tagSCU_CLK_INFO
{
    uint32 armFreq;		//ARM PLL FREQ
    //uint32 dummyFreq;	//dummy PLL FREQ
    //uint32 AuxFreq;		//AUX PLL FREQ
    //uint32 ahbDiv;
    //uint32 apbDiv;
    uint32 armFreqLast;

} SCU_CLK_INFO,*pSCU_CLK_INFO;

typedef struct tagPMU_APP_TABLE
{
    uint8  scuAppId;
    uint8  counter;
    uint16 armFreq;
    //uint16 dspFreq;
	uint16 dummyFreq;

}PMU_APP_TABLE,*pPMU_APP_TABLE;


/*
*-------------------------------------------------------------------------------
*
*                           Functon Declaration
*
*-------------------------------------------------------------------------------
*/
extern int32 PMUStartAPP(ePMU_APP appId);
extern int32 PMUStopAPP(ePMU_APP appId);

extern int32 PMUSetCodecFreq(uint32 freq);
extern void PMUSetLCDCFreq(uint32 freq);

/*
********************************************************************************
*
*                         End of Pmu.h
*
********************************************************************************
*/
#endif


