/*
********************************************************************************
*                   Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* File Name£º   ModuleInfoTab.h
*
* Description:  define module struct information
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2015-02-06         1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _MODULEINFO_H
#define _MODULEINFO_H

#undef  EXT
#ifdef _IN_MODULEINFO_
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                           Module Info
*
*-------------------------------------------------------------------------------
*/
//Module Section ID Define
typedef enum
{
    //SysCode
    MODULE_ID_SYS,
    MODULE_ID_INIT,
    MODULE_ID_INIT_PAD,
    MODULE_ID_SUSPEND,
    MODULE_ID_RESUME,
 
    MODULE_ID_BCORE_DEV,
    MODULE_ID_BCORE_INIT,
    MODULE_ID_MSG_DEV,
    MODULE_ID_MSG_INIT,
    MODULE_ID_MSG_SHELL,
    MODULE_ID_DIR_DEV,
    MODULE_ID_DIR_INIT,
    MODULE_ID_DIR_SHELL,
    MODULE_ID_STREAMCONTROL_TASK,
    MODULE_ID_STREAMCONTROL_INIT,
    MODULE_ID_AUDIOCONTROL_TASK,
    MODULE_ID_AUDIOCONTROL_INIT,
    MODULE_ID_ROCKCODEC_DEV,
    MODULE_ID_ROCKCODEC_INIT,
    MODULE_ID_ROCKCODEC_SHELL,
    MODULE_ID_I2S_DEV,
    MODULE_ID_I2S_INIT,
    MODULE_ID_I2S_SHELL,
    MODULE_ID_AUDIO_DEV,
    MODULE_ID_AUDIO_INIT,
    MODULE_ID_AUDIO_SHELL,
    MODULE_ID_AUDIOSUB_COMMON,
    MODULE_ID_AUDIOSUB_INIT,
    MODULE_ID_AUDIOMAIN_COMMON,
    MODULE_ID_AUDIOMAIN_INIT,
    MODULE_ID_USBMSC_COMMON,
    MODULE_ID_USBMSC_INIT,
    MODULE_ID_USBMSC_SHELL,
    MODULE_ID_USBMSC_COMMON,
    MODULE_ID_USBMSC_INIT,
    MODULE_ID_USBMSC_SHELL,
    MODULE_ID_MAILBOX_COMMON,
    MODULE_ID_MAILBOX_INIT,
    MODULE_ID_MAILBOX_SHELL,
    MODULE_ID_RK618_DEV,
    MODULE_ID_RK618_INIT,
    MODULE_ID_RK618_SHELL,
    MODULE_ID_SD_DEV,
    MODULE_ID_SD_INIT,
    MODULE_ID_SD_SHELL,
    MODULE_ID_I2C_DEV,
    MODULE_ID_I2C_INIT,
    MODULE_ID_I2C_SHELL,
    MODULE_ID_PWM_DEV,
    MODULE_ID_PWM_INIT,
    MODULE_ID_PWM_SHELL,
    MODULE_ID_USBOTG_DEV,
    MODULE_ID_USBOTG_INIT,
    MODULE_ID_USBOTG_SHELL,
    MODULE_ID_SDC_DEV,
    MODULE_ID_SDC_INIT,
    MODULE_ID_SDC_SHELL,
    MODULE_ID_SDIO_DEV,
    MODULE_ID_SDIO_INIT,
    MODULE_ID_SDIO_SHELL,
    MODULE_ID_RK903_DEV,
    MODULE_ID_RK903_INIT,
    MODULE_ID_RK903_SHELL,
    MODULE_ID_WLC_DEV,
    MODULE_ID_WLC_INIT,
    MODULE_ID_WLC_SHELL,
    MODULE_ID_WIFI_FIRMAVE_ONE,
    MODULE_ID_WIFI_FIRMAVE_TWO,
    MODULE_ID_BT_FIRMAVE,
    MAX_MODULE_NUM

} MODULE_ID;

//Code Info Table
typedef struct _MODULE_INFO_T
{
    UINT32 CodeLoadBase;
    UINT32 CodeImageBase;
    UINT32 CodeImageLength;

    UINT32 DataLoadBase;
    UINT32 DataImageBase;
    UINT32 DataImageLength;

    UINT32 BssImageBase;
    UINT32 BssImageLength;

} MODULE_INFO_T;

//Module Info
typedef struct _MODULE_INFO_TABLE
{
    UINT32                  ModuleNum;
    MODULE_INFO_T           Module[MAX_MODULE_NUM];

} MODULE_INFO_TABLE;


//System Default Para
typedef struct _SYSTEM_DEFAULT_PARA_T
{
    //system default setting
    UINT32 SDEnable;
    UINT32 FMEnable;
    UINT32 KeyNumIndex;

    UINT32 gLanguage;
    UINT32 Volume;

    UINT32 BLMode;
    UINT32 BLevel;
    UINT32 BLtime;

    UINT32 SettingPowerOffTime;
    UINT32 IdlePowerOffTime;

    //Music default setting
    UINT32 MusicRepMode;
    UINT32 MusicPlayOrder;
    UINT32 MusicEqSel;
    UINT32 MusicPlayFxSel;

    //Video default setting

    //Radio default setting
    UINT32 FMArea;
    UINT32 FMStereo;

    //Record default setting

    //Picture default setting
    UINT32 PicAutoPlayTime;

    //Text default setting
    UINT32 TextAutoPlayTime;

    //Image ID
    uint32 ImageIndex;

    //mutiple language
    uint32 MenuMaxItem;
    uint32 LanguageTotle;   //TOTAL_LANAUAGE_NUM,LANGUAGE_MAX_COUNT
    uint32 LanguagesEnableTbl[64];//support 256 languange

} SYSTEM_DEFAULT_PARA_T;

/*
*-------------------------------------------------------------------------------
*
*                           Module Info
*
*-------------------------------------------------------------------------------
*/
//Firmware Info
typedef struct _FIRMWARE_INFO_T
{
    UINT32                  LoadStartBase;
    MODULE_INFO_TABLE       ModuleInfo;

} FIRMWARE_INFO_T;

void SysBSSInit(void);
/*
********************************************************************************
*
*                         End of ModuleInfoTab.h
*
********************************************************************************
*/
#endif

