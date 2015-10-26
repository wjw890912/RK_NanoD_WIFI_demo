/*
********************************************************************************
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* File Name£º   PowerManager.h
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo         2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _POWER_H
#define _POWER_H

#undef  EXT
#ifdef _IN_POWER
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
typedef enum _FREQ_APP
{
    FREQ_IDLE = 0,
    FREQ_MIN,
    FREQ_INIT,
    FREQ_BLON,
    FREQ_AUDIO_INIT,
    FREQ_MP3,
    FREQ_MP3H,
    FREQ_WMA,
    FREQ_WMAH,
    FREQ_AAC,
    FREQ_WAV,
    FREQ_APE,
    FREQ_FLAC,
    FREQ_OGG,
    FREQ_NOGG,
    FREQ_HOGG,
    FREQ_EHOGG,
    FREQ_HAPE,
    FREQ_HFLAC,
    FREQ_HALAC,
    #ifdef _RK_EQ_5_
    FREQ_EQ,        //20
    #else
    FREQ_EQ_L,
    FREQ_EQ,
    FREQ_EQ_H,
    #endif
    FREQ_JPG,
    FREQ_FM,
    FREQ_RECORDADPCM,
    FREQ_FMAUTOSEARCH,
    FREQ_MEDIAUPDATA,
    FREQ_USB,
    FREQ_BEEP,
    FREQ_DC_FILTER_LOW,
    FREQ_DC_FILTER_HIGH,
    FREQ_MEDIA_INIT,
    FREQ_AVI,
    FREQ_MAX,

    FREQ_APP_MAX

}eFREQ_APP;

/*
*-------------------------------------------------------------------------------
*
*                           Struct define
*
*-------------------------------------------------------------------------------
*/
typedef struct tagCRU_CLK_INFO
{
    uint32 armFreq;        //ARM PLL FREQ
    uint32 armFreqLast;

} CRU_CLK_INFO,*pCRU_CLK_INFO;

typedef struct tagFREQ_APP_TABLE
{
    uint8  scuAppId;
    uint8  counter;

    uint32 PllFreq;
    uint32 syshclk; //sysfclk
    uint32 sysstclk;
    uint32 syspclk;

    uint32 calhclk; //calfclk
    uint32 calstclk;

}FREQ_APP_TABLE,*pFREQ_APP_TABLE;


/*
*-------------------------------------------------------------------------------
*
*                           Functon Declaration
*
*-------------------------------------------------------------------------------
*/
extern int32 FREQ_Enable(void);
extern int32 FREQ_Disable(void);

extern int32 FREQ_EnterModule(eFREQ_APP modulename);
extern int32 FREQ_ExitModule(eFREQ_APP modulename);
extern void SetSysFreq(uint32 nMhz);

/*
********************************************************************************
*
*                         End of Pmu.h
*
********************************************************************************
*/
#endif


