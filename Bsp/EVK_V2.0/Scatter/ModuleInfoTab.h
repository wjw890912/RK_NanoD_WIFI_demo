/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Bsp\EVK_V2.0\Scatter\ModuleInfoTab.h
* Owner: aaron.sun
* Date: 2015.6.10
* Time: 16:00:57
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.10     16:00:57   1.0
********************************************************************************************
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
    MODULE_ID_PMU,
    MODULE_ID_SUSPEND,
    MODULE_ID_RESUME,
	MODULE_ID_MAIN_TASK,
    MODULE_ID_MAIN_INIT,
    MODULE_ID_MUSIC_SHELL,
    MODULE_ID_EFFECT_MODULE,
    MODULE_ID_TIMER_DEV,
    MODULE_ID_TIMER_INIT,
    MODULE_ID_TIMER_SHELL,
    MODULE_ID_ADC_DEV,
    MODULE_ID_ADC_INIT,
    MODULE_ID_ADC_SHELL,
    MODULE_ID_KEY_DEV,
    MODULE_ID_KEY_INIT,
    MODULE_ID_KEY_SHELL,
    MODULE_ID_PWM_DEV,
    MODULE_ID_PWM_INIT,
    MODULE_ID_PWM_SHELL,
    MODULE_ID_DISPLAY_DEV,
    MODULE_ID_DISPLAY_INIT,
    MODULE_ID_DISPLAY_SHELL,
    MODULE_ID_GUI_TASK,
    MODULE_ID_GUI_INIT,
    MODULE_ID_BCORE_DEV,
    MODULE_ID_BCORE_INIT,
    MODULE_ID_BCORE_SHELL,
    MODULE_ID_MSG_DEV,
    MODULE_ID_MSG_INIT,
    MODULE_ID_MSG_SHELL,
    MODULE_ID_DIR_DEV,
    MODULE_ID_DIR_INIT,
    MODULE_ID_DIR_SHELL,
    MODULE_ID_FILESTREAM_TASK,
    MODULE_ID_FILESTREAM_INIT,
    MODULE_ID_HTTP_TASK,
    MODULE_ID_HTTP_INIT,
    MODULE_ID_MEDIA_TASK,
    MODULE_ID_MEDIA_INIT,
    MODULE_ID_STREAMCONTROL_TASK,
    MODULE_ID_STREAMCONTROL_INIT,
    MODULE_ID_AUDIOCONTROL_TASK,
    MODULE_ID_AUDIOCONTROL_INIT,
    MODULE_ID_I2S_DEV,
    MODULE_ID_I2S_INIT,
    MODULE_ID_I2S_SHELL,
    MODULE_ID_ROCKCODEC_DEV,
    MODULE_ID_ROCKCODEC_INIT,
    MODULE_ID_ROCKCODEC_SHELL,
    MODULE_ID_AUDIO_DEV,
    MODULE_ID_AUDIO_INIT,
    MODULE_ID_AUDIO_SHELL,
    MODULE_ID_AUDIOSUB_TASK,
    MODULE_ID_AUDIOSUB_INIT,
    MODULE_ID_AUDIOMAIN_TASK,
    MODULE_ID_AUDIOMAIN_INIT,
    MODULE_ID_USBHUB_DEV,//add jdz 
    MODULE_ID_USBHUB_INIT,
    MODULE_ID_USBHUB_SHELL,
	MODULE_ID_USBETH_DEV,//add jdz 
	MODULE_ID_USBETH_INIT,
	MODULE_ID_USBETH_SHELL,
	MODULE_ID_USBMSC_DEV,
	MODULE_ID_USBMSC_INIT,
	MODULE_ID_USBMSC_SHELL,
    MODULE_ID_MAILBOX_DEV,
    MODULE_ID_MAILBOX_INIT,
    MODULE_ID_MAILBOX_SHELL,
    MODULE_ID_RK618_DEV,
    MODULE_ID_RK618_INIT,
    MODULE_ID_RK618_SHELL,
    MODULE_ID_FIFO_DEV,
    MODULE_ID_FIFO_INIT,
    MODULE_ID_FIFO_SHELL,
    MODULE_ID_SD_DEV,
    MODULE_ID_SD_INIT,
    MODULE_ID_SD_SHELL,
    MODULE_ID_I2C_DEV,
    MODULE_ID_I2C_INIT,
    MODULE_ID_I2C_SHELL,
    MODULE_ID_USBOTG_DEV,
    MODULE_ID_USBOTG_INIT,
    MODULE_ID_USBOTG_SHELL,
    MODULE_ID_SDC_DEV,
    MODULE_ID_SDC_INIT,
    MODULE_ID_SDC_SHELL,
    MODULE_ID_EMMC_DEV,
    MODULE_ID_EMMC_INIT,
    MODULE_ID_EMMC_SHELL,
    MODULE_ID_LUN_DEV,
    MODULE_ID_LUN_INIT,
    MODULE_ID_LUN_SHELL,
    MODULE_ID_PAR_DEV,
    MODULE_ID_PAR_INIT,
    MODULE_ID_PAR_SHELL,
    MODULE_ID_FAT_DEV,
    MODULE_ID_FAT_INIT,
    MODULE_ID_FAT_SHELL,
    MODULE_ID_FILE_DEV,
    MODULE_ID_FILE_INIT,
    MODULE_ID_FILE_SHELL,
    MODULE_ID_SDIO_DEV,
    MODULE_ID_SDIO_INIT,
    MODULE_ID_SDIO_SHELL,
    MODULE_ID_RK903_DEV,
    MODULE_ID_RK903_INIT,
    MODULE_ID_RK903_SHELL,
    MODULE_ID_WLC_DEV,
    MODULE_ID_WLC_INIT,
    MODULE_ID_WLC_SHELL,
    MODULE_ID_INNO_WIFI,
    MODULE_ID_AP6181_INIT,  //101
    MODULE_ID_AP6181_WIFI,  //102
    MODULE_ID_DLNA,
    MODULE_ID_UPNP,
    MODULE_ID_LWIP, //103
    MODULE_ID_SPIFLASH_INIT,
    MODULE_ID_SPIFLASH_DEV,
    MODULE_ID_SPIFLASH_SHELL,
    MODULE_ID_WIFI_FIRMAVE_ONE,
    MODULE_ID_WIFI_FIRMAVE_TWO,
    MODULE_ID_BT_FIRMAVE,
    MODULE_ID_AUDIO_EQ_DATA,
    MODULE_ID_DMR_DEVICE_DATA,
    MODULE_ID_DMR_CONNECTIONMANAGER_SERVICE_DATA,
    MODULE_ID_DMR_RENDERINGCONTROL_SERVICE_DATA,
    MODULE_ID_DMR_AVTRANSPORT_SERVICE_DATA,
    MODULE_ID_DMR_NAMINGBITMAP_DATA,
    MODULE_ID_DMR_NMSTRTPAGES_DATA,
    MODULE_ID_DMR_NAMEPAGES_DATA,
    MODULE_ID_BB_CODE,
    MODULE_ID_MP3_DECODE_BIN,
    MODULE_ID_WMA_DECODE_BIN,
    MODULE_ID_WAV_DECODE_BIN,
    MODULE_ID_FLAC_DECODE_BIN,
    MODULE_ID_AAC_DECODE_BIN,
    MODULE_ID_APE_DECODE_BIN,
    MODULE_ID_OGG_DECODE_BIN,
    MODULE_ID_FLAC_HDECODE_BIN,
    MODULE_ID_ALAC_HDECODE_BIN,
    MODULE_ID_APE_HDECODE_BIN,

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

