/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Bsp\EVK_V2.0\Scatter\ScatterLoader.c
* Owner: aaron.sun
* Date: 2015.6.10
* Time: 16:01:48
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.10     16:01:48   1.0
********************************************************************************************
*/

#define _IN_STARTLOAD_

#include "BspConfig.h"
#include "RKOS.h"
#include "BSP.h"
#include "ModuleInfoTab.h"

extern uint32 Load$$MODULE_INFO$$Base;

//SYS MODULE
extern uint32 Load$$AP_SYS_CODE$$Base;
extern uint32 Image$$AP_SYS_CODE$$Base;
extern uint32 Image$$AP_SYS_CODE$$Length;
extern uint32 Load$$AP_SYS_DATA$$Base;
extern uint32 Image$$AP_SYS_DATA$$RW$$Base;
extern uint32 Image$$AP_SYS_DATA$$RW$$Length;
extern uint32 Image$$AP_SYS_DATA$$ZI$$Base;
extern uint32 Image$$AP_SYS_DATA$$ZI$$Length;

//INIT MODULE
extern uint32 Load$$INIT_CODE$$Base;
extern uint32 Image$$INIT_CODE$$Base;
extern uint32 Image$$INIT_CODE$$Length;
extern uint32 Load$$INIT_DATA$$Base;
extern uint32 Image$$INIT_DATA$$RW$$Base;
extern uint32 Image$$INIT_DATA$$RW$$Length;
extern uint32 Image$$INIT_DATA$$ZI$$Base;
extern uint32 Image$$INIT_DATA$$ZI$$Length;

//PMU MODULE
extern uint32 Load$$PMU_CODE$$Base;
extern uint32 Image$$PMU_CODE$$Base;
extern uint32 Image$$PMU_CODE$$Length;
extern uint32 Load$$PMU_DATA$$Base;
extern uint32 Image$$PMU_DATA$$RW$$Base;
extern uint32 Image$$PMU_DATA$$RW$$Length;
extern uint32 Image$$PMU_DATA$$ZI$$Base;
extern uint32 Image$$PMU_DATA$$ZI$$Length;

//SYS SUSPEND MODULE
extern uint32 Load$$SYS_SUSPEND_CODE$$Base;
extern uint32 Image$$SYS_SUSPEND_CODE$$Base;
extern uint32 Image$$SYS_SUSPEND_CODE$$Length;
extern uint32 Load$$SYS_SUSPEND_DATA$$Base;
extern uint32 Image$$SYS_SUSPEND_DATA$$RW$$Base;
extern uint32 Image$$SYS_SUSPEND_DATA$$RW$$Length;
extern uint32 Image$$SYS_SUSPEND_DATA$$ZI$$Base;
extern uint32 Image$$SYS_SUSPEND_DATA$$ZI$$Length;


//SYS RESUME MODULE
extern uint32 Load$$SYS_RESUME_CODE$$Base;
extern uint32 Image$$SYS_RESUME_CODE$$Base;
extern uint32 Image$$SYS_RESUME_CODE$$Length;
extern uint32 Load$$SYS_RESUME_DATA$$Base;
extern uint32 Image$$SYS_RESUME_DATA$$RW$$Base;
extern uint32 Image$$SYS_RESUME_DATA$$RW$$Length;
extern uint32 Image$$SYS_RESUME_DATA$$ZI$$Base;
extern uint32 Image$$SYS_RESUME_DATA$$ZI$$Length;

//MAIN SEG MODULE

extern uint32 Load$$MAIN_TASK_CODE$$Base;
extern uint32 Image$$MAIN_TASK_CODE$$Base;
extern uint32 Image$$MAIN_TASK_CODE$$Length;

extern uint32 Load$$MAIN_INIT_CODE$$Base;
extern uint32 Image$$MAIN_INIT_CODE$$Base;
extern uint32 Image$$MAIN_INIT_CODE$$Length;

//MUSIC SEG MODULE

extern uint32 Load$$MUSIC_SHELL_CODE$$Base;
extern uint32 Image$$MUSIC_SHELL_CODE$$Base;
extern uint32 Image$$MUSIC_SHELL_CODE$$Length;

//EFFECT SEG MODULE
extern uint32 Load$$EFFECT_MODULE_CODE$$Base;
extern uint32 Image$$EFFECT_MODULE_CODE$$Base;
extern uint32 Image$$EFFECT_MODULE_CODE$$Length;

//TIMER SEG MODULE
extern uint32 Load$$TIMER_DEV_CODE$$Base;
extern uint32 Image$$TIMER_DEV_CODE$$Base;
extern uint32 Image$$TIMER_DEV_CODE$$Length;

extern uint32 Load$$TIMER_INIT_CODE$$Base;
extern uint32 Image$$TIMER_INIT_CODE$$Base;
extern uint32 Image$$TIMER_INIT_CODE$$Length;

extern uint32 Load$$TIMER_SHELL_CODE$$Base;
extern uint32 Image$$TIMER_SHELL_CODE$$Base;
extern uint32 Image$$TIMER_SHELL_CODE$$Length;

//ADC SEG MODULE
extern uint32 Load$$ADC_DEV_CODE$$Base;
extern uint32 Image$$ADC_DEV_CODE$$Base;
extern uint32 Image$$ADC_DEV_CODE$$Length;

extern uint32 Load$$ADC_INIT_CODE$$Base;
extern uint32 Image$$ADC_INIT_CODE$$Base;
extern uint32 Image$$ADC_INIT_CODE$$Length;

extern uint32 Load$$ADC_SHELL_CODE$$Base;
extern uint32 Image$$ADC_SHELL_CODE$$Base;
extern uint32 Image$$ADC_SHELL_CODE$$Length;

//KEY SEG MODULE
extern uint32 Load$$KEY_DEV_CODE$$Base;
extern uint32 Image$$KEY_DEV_CODE$$Base;
extern uint32 Image$$KEY_DEV_CODE$$Length;

extern uint32 Load$$KEY_INIT_CODE$$Base;
extern uint32 Image$$KEY_INIT_CODE$$Base;
extern uint32 Image$$KEY_INIT_CODE$$Length;

extern uint32 Load$$KEY_SHELL_CODE$$Base;
extern uint32 Image$$KEY_SHELL_CODE$$Base;
extern uint32 Image$$KEY_SHELL_CODE$$Length;

//PWM SEG MODULE
extern uint32 Load$$PWM_DEV_CODE$$Base;
extern uint32 Image$$PWM_DEV_CODE$$Base;
extern uint32 Image$$PWM_DEV_CODE$$Length;

extern uint32 Load$$PWM_INIT_CODE$$Base;
extern uint32 Image$$PWM_INIT_CODE$$Base;
extern uint32 Image$$PWM_INIT_CODE$$Length;

extern uint32 Load$$PWM_SHELL_CODE$$Base;
extern uint32 Image$$PWM_SHELL_CODE$$Base;
extern uint32 Image$$PWM_SHELL_CODE$$Length;

//DISPLAY SEG MODULE
extern uint32 Load$$DISPLAY_DEV_CODE$$Base;
extern uint32 Image$$DISPLAY_DEV_CODE$$Base;
extern uint32 Image$$DISPLAY_DEV_CODE$$Length;

extern uint32 Load$$DISPLAY_INIT_CODE$$Base;
extern uint32 Image$$DISPLAY_INIT_CODE$$Base;
extern uint32 Image$$DISPLAY_INIT_CODE$$Length;

extern uint32 Load$$DISPLAY_SHELL_CODE$$Base;
extern uint32 Image$$DISPLAY_SHELL_CODE$$Base;
extern uint32 Image$$DISPLAY_SHELL_CODE$$Length;

//GUI SEG MODULE
extern uint32 Load$$GUI_TASK_CODE$$Base;
extern uint32 Image$$GUI_TASK_CODE$$Base;
extern uint32 Image$$GUI_TASK_CODE$$Length;

extern uint32 Load$$GUI_INIT_CODE$$Base;
extern uint32 Image$$GUI_INIT_CODE$$Base;
extern uint32 Image$$GUI_INIT_CODE$$Length;

//BCORE SEG MODULE
extern uint32 Load$$BCORE_DEV_CODE$$Base;
extern uint32 Image$$BCORE_DEV_CODE$$Base;
extern uint32 Image$$BCORE_DEV_CODE$$Length;

extern uint32 Load$$BCORE_INIT_CODE$$Base;
extern uint32 Image$$BCORE_INIT_CODE$$Base;
extern uint32 Image$$BCORE_INIT_CODE$$Length;

extern uint32 Load$$BCORE_SHELL_CODE$$Base;
extern uint32 Image$$BCORE_SHELL_CODE$$Base;
extern uint32 Image$$BCORE_SHELL_CODE$$Length;

//MSG SEG MODULE
extern uint32 Load$$MSG_DEV_CODE$$Base;
extern uint32 Image$$MSG_DEV_CODE$$Base;
extern uint32 Image$$MSG_DEV_CODE$$Length;

extern uint32 Load$$MSG_INIT_CODE$$Base;
extern uint32 Image$$MSG_INIT_CODE$$Base;
extern uint32 Image$$MSG_INIT_CODE$$Length;

extern uint32 Load$$MSG_SHELL_CODE$$Base;
extern uint32 Image$$MSG_SHELL_CODE$$Base;
extern uint32 Image$$MSG_SHELL_CODE$$Length;


//FILESTREAM SEG MODULE
extern uint32 Load$$FILESTREAM_TASK_CODE$$Base;
extern uint32 Image$$FILESTREAM_TASK_CODE$$Base;
extern uint32 Image$$FILESTREAM_TASK_CODE$$Length;

extern uint32 Load$$FILESTREAM_INIT_CODE$$Base;
extern uint32 Image$$FILESTREAM_INIT_CODE$$Base;
extern uint32 Image$$FILESTREAM_INIT_CODE$$Length;

// HTTP SEG MODULE
extern uint32 Load$$HTTP_TASK_CODE$$Base;
extern uint32 Image$$HTTP_TASK_CODE$$Base;
extern uint32 Image$$HTTP_TASK_CODE$$Length;

extern uint32 Load$$HTTP_INIT_CODE$$Base;
extern uint32 Image$$HTTP_INIT_CODE$$Base;
extern uint32 Image$$HTTP_INIT_CODE$$Length;


//MEDIA SEG MODULE
extern uint32 Load$$MEDIA_TASK_CODE$$Base;
extern uint32 Image$$MEDIA_TASK_CODE$$Base;
extern uint32 Image$$MEDIA_TASK_CODE$$Length;

extern uint32 Load$$MEDIA_INIT_CODE$$Base;
extern uint32 Image$$MEDIA_INIT_CODE$$Base;
extern uint32 Image$$MEDIA_INIT_CODE$$Length;


//DIR SEG MODULE
extern uint32 Load$$DIR_DEV_CODE$$Base;
extern uint32 Image$$DIR_DEV_CODE$$Base;
extern uint32 Image$$DIR_DEV_CODE$$Length;

extern uint32 Load$$DIR_INIT_CODE$$Base;
extern uint32 Image$$DIR_INIT_CODE$$Base;
extern uint32 Image$$DIR_INIT_CODE$$Length;

extern uint32 Load$$DIR_SHELL_CODE$$Base;
extern uint32 Image$$DIR_SHELL_CODE$$Base;
extern uint32 Image$$DIR_SHELL_CODE$$Length;


//STREAMCONTROL SEG MODULE
extern uint32 Load$$STREAMCONTROL_TASK_CODE$$Base;
extern uint32 Image$$STREAMCONTROL_TASK_CODE$$Base;
extern uint32 Image$$STREAMCONTROL_TASK_CODE$$Length;

extern uint32 Load$$STREAMCONTROL_INIT_CODE$$Base;
extern uint32 Image$$STREAMCONTROL_INIT_CODE$$Base;
extern uint32 Image$$STREAMCONTROL_INIT_CODE$$Length;


//AUDIOCONTROL SEG MODULE
extern uint32 Load$$AUDIOCONTROL_TASK_CODE$$Base;
extern uint32 Image$$AUDIOCONTROL_TASK_CODE$$Base;
extern uint32 Image$$AUDIOCONTROL_TASK_CODE$$Length;

extern uint32 Load$$AUDIOCONTROL_INIT_CODE$$Base;
extern uint32 Image$$AUDIOCONTROL_INIT_CODE$$Base;
extern uint32 Image$$AUDIOCONTROL_INIT_CODE$$Length;

//I2S SEG MODULE
extern uint32 Load$$I2S_DEV_CODE$$Base;
extern uint32 Image$$I2S_DEV_CODE$$Base;
extern uint32 Image$$I2S_DEV_CODE$$Length;

extern uint32 Load$$I2S_INIT_CODE$$Base;
extern uint32 Image$$I2S_INIT_CODE$$Base;
extern uint32 Image$$I2S_INIT_CODE$$Length;

extern uint32 Load$$I2S_SHELL_CODE$$Base;
extern uint32 Image$$I2S_SHELL_CODE$$Base;
extern uint32 Image$$I2S_SHELL_CODE$$Length;

//ROCKCODEC SEG MODULE
extern uint32 Load$$ROCKCODEC_DEV_CODE$$Base;
extern uint32 Image$$ROCKCODEC_DEV_CODE$$Base;
extern uint32 Image$$ROCKCODEC_DEV_CODE$$Length;

extern uint32 Load$$ROCKCODEC_INIT_CODE$$Base;
extern uint32 Image$$ROCKCODEC_INIT_CODE$$Base;
extern uint32 Image$$ROCKCODEC_INIT_CODE$$Length;

extern uint32 Load$$ROCKCODEC_SHELL_CODE$$Base;
extern uint32 Image$$ROCKCODEC_SHELL_CODE$$Base;
extern uint32 Image$$ROCKCODEC_SHELL_CODE$$Length;

//AUDIO SEG MODULE
extern uint32 Load$$AUDIO_DEV_CODE$$Base;
extern uint32 Image$$AUDIO_DEV_CODE$$Base;
extern uint32 Image$$AUDIO_DEV_CODE$$Length;

extern uint32 Load$$AUDIO_INIT_CODE$$Base;
extern uint32 Image$$AUDIO_INIT_CODE$$Base;
extern uint32 Image$$AUDIO_INIT_CODE$$Length;

extern uint32 Load$$AUDIO_SHELL_CODE$$Base;
extern uint32 Image$$AUDIO_SHELL_CODE$$Base;
extern uint32 Image$$AUDIO_SHELL_CODE$$Length;

//AUDIOSUB SEG MODULE
extern uint32 Load$$AUDIOSUB_TASK_CODE$$Base;
extern uint32 Image$$AUDIOSUB_TASK_CODE$$Base;
extern uint32 Image$$AUDIOSUB_TASK_CODE$$Length;

extern uint32 Load$$AUDIOSUB_INIT_CODE$$Base;
extern uint32 Image$$AUDIOSUB_INIT_CODE$$Base;
extern uint32 Image$$AUDIOSUB_INIT_CODE$$Length;


//AUDIOMAIN SEG MODULE
extern uint32 Load$$AUDIOMAIN_TASK_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_TASK_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_TASK_CODE$$Length;

extern uint32 Load$$AUDIOMAIN_INIT_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_INIT_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_INIT_CODE$$Length;

//USBMSC SEG MODULE
extern uint32 Load$$USBMSC_DEV_CODE$$Base;
extern uint32 Image$$USBMSC_DEV_CODE$$Base;
extern uint32 Image$$USBMSC_DEV_CODE$$Length;

extern uint32 Load$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Length;

extern uint32 Load$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Length;

//MAILBOX SEG MODULE
extern uint32 Load$$MAILBOX_DEV_CODE$$Base;
extern uint32 Image$$MAILBOX_DEV_CODE$$Base;
extern uint32 Image$$MAILBOX_DEV_CODE$$Length;
extern uint32 Load$$MAILBOX_INIT_CODE$$Base;
extern uint32 Image$$MAILBOX_INIT_CODE$$Base;
extern uint32 Image$$MAILBOX_INIT_CODE$$Length;
extern uint32 Load$$MAILBOX_SHELL_CODE$$Base;
extern uint32 Image$$MAILBOX_SHELL_CODE$$Base;
extern uint32 Image$$MAILBOX_SHELL_CODE$$Length;

//RK618 SEG MODULE
extern uint32 Load$$RK618_DEV_CODE$$Base;
extern uint32 Image$$RK618_DEV_CODE$$Base;
extern uint32 Image$$RK618_DEV_CODE$$Length;

extern uint32 Load$$RK618_INIT_CODE$$Base;
extern uint32 Image$$RK618_INIT_CODE$$Base;
extern uint32 Image$$RK618_INIT_CODE$$Length;

extern uint32 Load$$RK618_SHELL_CODE$$Base;
extern uint32 Image$$RK618_SHELL_CODE$$Base;
extern uint32 Image$$RK618_SHELL_CODE$$Length;

//SD SEG MODULE
extern uint32 Load$$SD_DEV_CODE$$Base;
extern uint32 Image$$SD_DEV_CODE$$Base;
extern uint32 Image$$SD_DEV_CODE$$Length;

extern uint32 Load$$SD_INIT_CODE$$Base;
extern uint32 Image$$SD_INIT_CODE$$Base;
extern uint32 Image$$SD_INIT_CODE$$Length;

extern uint32 Load$$SD_SHELL_CODE$$Base;
extern uint32 Image$$SD_SHELL_CODE$$Base;
extern uint32 Image$$SD_SHELL_CODE$$Length;


//SD SEG MODULE
extern uint32 Load$$FIFO_DEV_CODE$$Base;
extern uint32 Image$$FIFO_DEV_CODE$$Base;
extern uint32 Image$$FIFO_DEV_CODE$$Length;

extern uint32 Load$$FIFO_INIT_CODE$$Base;
extern uint32 Image$$FIFO_INIT_CODE$$Base;
extern uint32 Image$$FIFO_INIT_CODE$$Length;

extern uint32 Load$$FIFO_SHELL_CODE$$Base;
extern uint32 Image$$FIFO_SHELL_CODE$$Base;
extern uint32 Image$$FIFO_SHELL_CODE$$Length;


//I2C SEG MODULE
extern uint32 Load$$I2C_DEV_CODE$$Base;
extern uint32 Image$$I2C_DEV_CODE$$Base;
extern uint32 Image$$I2C_DEV_CODE$$Length;

extern uint32 Load$$I2C_INIT_CODE$$Base;
extern uint32 Image$$I2C_INIT_CODE$$Base;
extern uint32 Image$$I2C_INIT_CODE$$Length;

extern uint32 Load$$I2C_SHELL_CODE$$Base;
extern uint32 Image$$I2C_SHELL_CODE$$Base;
extern uint32 Image$$I2C_SHELL_CODE$$Length;


//USBOTG SEG MODULE
extern uint32 Load$$USBOTG_DEV_CODE$$Base;
extern uint32 Image$$USBOTG_DEV_CODE$$Base;
extern uint32 Image$$USBOTG_DEV_CODE$$Length;

extern uint32 Load$$USBOTG_INIT_CODE$$Base;
extern uint32 Image$$USBOTG_INIT_CODE$$Base;
extern uint32 Image$$USBOTG_INIT_CODE$$Length;

extern uint32 Load$$USBOTG_SHELL_CODE$$Base;
extern uint32 Image$$USBOTG_SHELL_CODE$$Base;
extern uint32 Image$$USBOTG_SHELL_CODE$$Length;

//SDC SEG MODULE
extern uint32 Load$$SDC_DEV_CODE$$Base;
extern uint32 Image$$SDC_DEV_CODE$$Base;
extern uint32 Image$$SDC_DEV_CODE$$Length;

extern uint32 Load$$SDC_SHELL_CODE$$Base;
extern uint32 Image$$SDC_SHELL_CODE$$Base;
extern uint32 Image$$SDC_SHELL_CODE$$Length;

extern uint32 Load$$SDC_INIT_CODE$$Base;
extern uint32 Image$$SDC_INIT_CODE$$Base;
extern uint32 Image$$SDC_INIT_CODE$$Length;


//EMMC SEG MODULE
extern uint32 Load$$EMMC_DEV_CODE$$Base;
extern uint32 Image$$EMMC_DEV_CODE$$Base;
extern uint32 Image$$EMMC_DEV_CODE$$Length;

extern uint32 Load$$EMMC_SHELL_CODE$$Base;
extern uint32 Image$$EMMC_SHELL_CODE$$Base;
extern uint32 Image$$EMMC_SHELL_CODE$$Length;

extern uint32 Load$$EMMC_INIT_CODE$$Base;
extern uint32 Image$$EMMC_INIT_CODE$$Base;
extern uint32 Image$$EMMC_INIT_CODE$$Length;


//LUN SEG MODULE
extern uint32 Load$$LUN_DEV_CODE$$Base;
extern uint32 Image$$LUN_DEV_CODE$$Base;
extern uint32 Image$$LUN_DEV_CODE$$Length;

extern uint32 Load$$LUN_SHELL_CODE$$Base;
extern uint32 Image$$LUN_SHELL_CODE$$Base;
extern uint32 Image$$LUN_SHELL_CODE$$Length;

extern uint32 Load$$LUN_INIT_CODE$$Base;
extern uint32 Image$$LUN_INIT_CODE$$Base;
extern uint32 Image$$LUN_INIT_CODE$$Length;

//PAR SEG MODULE
extern uint32 Load$$PAR_DEV_CODE$$Base;
extern uint32 Image$$PAR_DEV_CODE$$Base;
extern uint32 Image$$PAR_DEV_CODE$$Length;

extern uint32 Load$$PAR_SHELL_CODE$$Base;
extern uint32 Image$$PAR_SHELL_CODE$$Base;
extern uint32 Image$$PAR_SHELL_CODE$$Length;

extern uint32 Load$$PAR_INIT_CODE$$Base;
extern uint32 Image$$PAR_INIT_CODE$$Base;
extern uint32 Image$$PAR_INIT_CODE$$Length;

//FAT SEG MODULE
extern uint32 Load$$FAT_DEV_CODE$$Base;
extern uint32 Image$$FAT_DEV_CODE$$Base;
extern uint32 Image$$FAT_DEV_CODE$$Length;

extern uint32 Load$$FAT_SHELL_CODE$$Base;
extern uint32 Image$$FAT_SHELL_CODE$$Base;
extern uint32 Image$$FAT_SHELL_CODE$$Length;

extern uint32 Load$$FAT_INIT_CODE$$Base;
extern uint32 Image$$FAT_INIT_CODE$$Base;
extern uint32 Image$$FAT_INIT_CODE$$Length;


//FILE SEG MODULE
extern uint32 Load$$FILE_DEV_CODE$$Base;
extern uint32 Image$$FILE_DEV_CODE$$Base;
extern uint32 Image$$FILE_DEV_CODE$$Length;

extern uint32 Load$$FILE_SHELL_CODE$$Base;
extern uint32 Image$$FILE_SHELL_CODE$$Base;
extern uint32 Image$$FILE_SHELL_CODE$$Length;

extern uint32 Load$$FILE_INIT_CODE$$Base;
extern uint32 Image$$FILE_INIT_CODE$$Base;
extern uint32 Image$$FILE_INIT_CODE$$Length;


//SDIO SEG MODULE
extern uint32 Load$$SDIO_DEV_CODE$$Base;
extern uint32 Image$$SDIO_DEV_CODE$$Base;
extern uint32 Image$$SDIO_DEV_CODE$$Length;

extern uint32 Load$$SDIO_SHELL_CODE$$Base;
extern uint32 Image$$SDIO_SHELL_CODE$$Base;
extern uint32 Image$$SDIO_SHELL_CODE$$Length;

extern uint32 Load$$SDIO_INIT_CODE$$Base;
extern uint32 Image$$SDIO_INIT_CODE$$Base;
extern uint32 Image$$SDIO_INIT_CODE$$Length;


//RK903 SEG MODULE
extern uint32 Load$$RK903_DEV_CODE$$Base;
extern uint32 Image$$RK903_DEV_CODE$$Base;
extern uint32 Image$$RK903_DEV_CODE$$Length;

extern uint32 Load$$RK903_SHELL_CODE$$Base;
extern uint32 Image$$RK903_SHELL_CODE$$Base;
extern uint32 Image$$RK903_SHELL_CODE$$Length;

extern uint32 Load$$RK903_INIT_CODE$$Base;
extern uint32 Image$$RK903_INIT_CODE$$Base;
extern uint32 Image$$RK903_INIT_CODE$$Length;


//WLC SEG MODULE
extern uint32 Load$$WLC_DEV_CODE$$Base;
extern uint32 Image$$WLC_DEV_CODE$$Base;
extern uint32 Image$$WLC_DEV_CODE$$Length;

extern uint32 Load$$WLC_SHELL_CODE$$Base;
extern uint32 Image$$WLC_SHELL_CODE$$Base;
extern uint32 Image$$WLC_SHELL_CODE$$Length;

extern uint32 Load$$WLC_INIT_CODE$$Base;
extern uint32 Image$$WLC_INIT_CODE$$Base;
extern uint32 Image$$WLC_INIT_CODE$$Length;
// innowifi
extern uint32 Load$$INNOWIFI_DEV_CODE$$Base;
extern uint32 Image$$INNOWIFI_DEV_CODE$$Base;
extern uint32 Image$$INNOWIFI_DEV_CODE$$Length;
//ap6181
extern uint32 Load$$AP6181WIFI_INIT_CODE$$Base;
extern uint32 Image$$AP6181WIFI_INIT_CODE$$Base;
extern uint32 Image$$AP6181WIFI_INIT_CODE$$Length;
extern uint32 Load$$AP6181WIFI_DEV_CODE$$Base;
extern uint32 Image$$AP6181WIFI_DEV_CODE$$Base;
extern uint32 Image$$AP6181WIFI_DEV_CODE$$Length;

//dlna
//extern uint32 Load$$DLNA_INIT_CODE$$Base;
//extern uint32 Image$$DLNA_INIT_CODE$$Base;
//extern uint32 Image$$DLNA_INIT_CODE$$Length;
extern uint32 Load$$DLNA_DEV_CODE$$Base;
extern uint32 Image$$DLNA_DEV_CODE$$Base;
extern uint32 Image$$DLNA_DEV_CODE$$Length;

//upnp
extern uint32 Load$$UPNP_DEV_CODE$$Base;
extern uint32 Image$$UPNP_DEV_CODE$$Base;
extern uint32 Image$$UPNP_DEV_CODE$$Length;

//lwip
extern uint32 Load$$LWIP_DEV_CODE$$Base;
extern uint32 Image$$LWIP_DEV_CODE$$Base;
extern uint32 Image$$LWIP_DEV_CODE$$Length;

//SPIFLASH SEG MODULE
extern uint32 Load$$SPIFLASH_DEV_CODE$$Base;
extern uint32 Image$$SPIFLASH_DEV_CODE$$Base;
extern uint32 Image$$SPIFLASH_DEV_CODE$$Length;

extern uint32 Load$$SPIFLASH_SHELL_CODE$$Base;
extern uint32 Image$$SPIFLASH_SHELL_CODE$$Base;
extern uint32 Image$$SPIFLASH_SHELL_CODE$$Length;

extern uint32 Load$$SPIFLASH_INIT_CODE$$Base;
extern uint32 Image$$SPIFLASH_INIT_CODE$$Base;
extern uint32 Image$$SPIFLASH_INIT_CODE$$Length;


//WIFI FIRMWARE MODULE
extern uint32 Load$$WIFI_FIRMWARE_ONE$$Base;
extern uint32 Image$$WIFI_FIRMWARE_ONE$$Length;
extern uint32 Load$$WIFI_FIRMWARE_TWO$$Base;
extern uint32 Image$$WIFI_FIRMWARE_TWO$$Length;

//BT FIRMWARE MODULE
extern uint32 Load$$BT_FIRMWARE$$Base;
extern uint32 Image$$BT_FIRMWARE$$Length;

//AUDIO EQTABLE
extern uint32 Load$$AUDIO_EQ_DATA$$Base;
extern uint32 Image$$AUDIO_EQ_DATA$$Length;
//xml DMR
extern uint32 Load$$DMR_DEVICE_DATA$$Base;
extern uint32 Image$$DMR_DEVICE_DATA$$Length;

extern uint32 Load$$DMR_CONNECTIONMANAGER_SERVICE_DATA$$Base;
extern uint32 Image$$DMR_CONNECTIONMANAGER_SERVICE_DATA$$Length;

extern uint32 Load$$DMR_RENDERINGCONTROL_SERVICE_DATA$$Base;
extern uint32 Image$$DMR_RENDERINGCONTROL_SERVICE_DATA$$Length;

extern uint32 Load$$DMR_AVTRANSPORT_SERVICE_DATA$$Base;
extern uint32 Image$$DMR_AVTRANSPORT_SERVICE_DATA$$Length;

//nametab

extern uint32 Load$$DMR_NAMINGBITMAP_DATA$$Base;
extern uint32 Image$$DMR_NAMINGBITMAP_DATA$$Length;

extern uint32 Load$$DMR_NMSTRTPAGES_DATA$$Base;
extern uint32 Image$$DMR_NMSTRTPAGES_DATA$$Length;

extern uint32 Load$$DMR_NAMEPAGES_DATA$$Base;
extern uint32 Image$$DMR_NAMEPAGES_DATA$$Length;

/****************BB 核*************************/
extern uint32  Load$$BB_SYS_CODE$$Base;
extern uint32 Image$$BB_SYS_CODE$$Base;
extern uint32 Image$$BB_SYS_CODE$$Length;
extern uint32  Load$$BB_SYS_DATA$$Base;
extern uint32 Image$$BB_SYS_DATA$$RW$$Base;
extern uint32 Image$$BB_SYS_DATA$$RW$$Length;
extern uint32 Image$$BB_SYS_DATA$$ZI$$Base;
extern uint32 Image$$BB_SYS_DATA$$ZI$$Length;

//MP3
extern uint32  Load$$MP3_DECODE_BIN_CODE$$Base;
extern uint32 Image$$MP3_DECODE_BIN_CODE$$Base;
extern uint32 Image$$MP3_DECODE_BIN_CODE$$Length;
extern uint32  Load$$MP3_DECODE_BIN_DATA$$Base;
extern uint32 Image$$MP3_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$MP3_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$MP3_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$MP3_DECODE_BIN_DATA$$ZI$$Length;

//WMA
extern uint32  Load$$WMA_DECODE_BIN_CODE$$Base;
extern uint32 Image$$WMA_DECODE_BIN_CODE$$Base;
extern uint32 Image$$WMA_DECODE_BIN_CODE$$Length;
extern uint32  Load$$WMA_DECODE_BIN_DATA$$Base;
extern uint32 Image$$WMA_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$WMA_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$WMA_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$WMA_DECODE_BIN_DATA$$ZI$$Length;


//WAV
extern uint32  Load$$WAV_DECODE_BIN_CODE$$Base;
extern uint32 Image$$WAV_DECODE_BIN_CODE$$Base;
extern uint32 Image$$WAV_DECODE_BIN_CODE$$Length;
extern uint32  Load$$WAV_DECODE_BIN_DATA$$Base;
extern uint32 Image$$WAV_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$WAV_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$WAV_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$WAV_DECODE_BIN_DATA$$ZI$$Length;


//FLAC
extern uint32  Load$$FLAC_DECODE_BIN_CODE$$Base;
extern uint32 Image$$FLAC_DECODE_BIN_CODE$$Base;
extern uint32 Image$$FLAC_DECODE_BIN_CODE$$Length;
extern uint32  Load$$FLAC_DECODE_BIN_DATA$$Base;
extern uint32 Image$$FLAC_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$FLAC_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$FLAC_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$FLAC_DECODE_BIN_DATA$$ZI$$Length;

//AAC
extern uint32  Load$$AAC_DECODE_BIN_CODE$$Base;
extern uint32 Image$$AAC_DECODE_BIN_CODE$$Base;
extern uint32 Image$$AAC_DECODE_BIN_CODE$$Length;
extern uint32  Load$$AAC_DECODE_BIN_DATA$$Base;
extern uint32 Image$$AAC_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$AAC_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$AAC_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$AAC_DECODE_BIN_DATA$$ZI$$Length;

//APE
extern uint32 Load$$APE_DECODE_BIN_CODE$$Base;
extern uint32 Image$$APE_DECODE_BIN_CODE$$Base;
extern uint32 Image$$APE_DECODE_BIN_CODE$$Length;
extern uint32 Load$$APE_DECODE_BIN_DATA$$Base;
extern uint32 Image$$APE_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$APE_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$APE_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$APE_DECODE_BIN_DATA$$ZI$$Length;


//OGG
extern uint32  Load$$OGG_DECODE_BIN_CODE$$Base;
extern uint32 Image$$OGG_DECODE_BIN_CODE$$Base;
extern uint32 Image$$OGG_DECODE_BIN_CODE$$Length;
extern uint32  Load$$OGG_DECODE_BIN_DATA$$Base;
extern uint32 Image$$OGG_DECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$OGG_DECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$OGG_DECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$OGG_DECODE_BIN_DATA$$ZI$$Length;


//HFLAC
extern uint32  Load$$FLAC_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$FLAC_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$FLAC_HDECODE_BIN_CODE$$Length;
extern uint32  Load$$FLAC_HDECODE_BIN_DATA$$Base;
extern uint32 Image$$FLAC_HDECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$FLAC_HDECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$FLAC_HDECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$FLAC_HDECODE_BIN_DATA$$ZI$$Length;

//HALAC
extern uint32  Load$$ALAC_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$ALAC_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$ALAC_HDECODE_BIN_CODE$$Length;
extern uint32  Load$$ALAC_HDECODE_BIN_DATA$$Base;
extern uint32 Image$$ALAC_HDECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$ALAC_HDECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$ALAC_HDECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$ALAC_HDECODE_BIN_DATA$$ZI$$Length;

//HAPE
extern uint32  Load$$APE_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$APE_HDECODE_BIN_CODE$$Base;
extern uint32 Image$$APE_HDECODE_BIN_CODE$$Length;
extern uint32  Load$$APE_HDECODE_BIN_DATA$$Base;
extern uint32 Image$$APE_HDECODE_BIN_DATA$$RW$$Base;
extern uint32 Image$$APE_HDECODE_BIN_DATA$$RW$$Length;
extern uint32 Image$$APE_HDECODE_BIN_DATA$$ZI$$Base;
extern uint32 Image$$APE_HDECODE_BIN_DATA$$ZI$$Length;



/*
--------------------------------------------------------------------------------
  Function name :
  Author        : ZHengYongzhi
  Description   : 模块信息表，生成固件模块头信息，用于代码调度

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
#ifndef _JTAG_DEBUG_
__attribute__((section("_ModuleInfo_")))
FIRMWARE_INFO_T const ModuleInfo  =
{

    (uint32)(&Load$$MODULE_INFO$$Base),

    //MODUE_INFO_TABLE
    {
        (uint32)(MAX_MODULE_NUM),
        {
            //SYS SEG
            {
                (uint32)(&Load$$AP_SYS_CODE$$Base),
                (uint32)(&Image$$AP_SYS_CODE$$Base),
                (uint32)(&Image$$AP_SYS_CODE$$Length),

                (uint32)(&Load$$AP_SYS_DATA$$Base),
                (uint32)(&Image$$AP_SYS_DATA$$RW$$Base),
                (uint32)(&Image$$AP_SYS_DATA$$RW$$Length),

                (uint32)(&Image$$AP_SYS_DATA$$ZI$$Base),
                (uint32)(&Image$$AP_SYS_DATA$$ZI$$Length),
            },

            //INIT SEG
            {
                (uint32)(&Load$$INIT_CODE$$Base),
                (uint32)(&Image$$INIT_CODE$$Base),
                (uint32)(&Image$$INIT_CODE$$Length),

                (uint32)(&Load$$INIT_DATA$$Base),
                (uint32)(&Image$$INIT_DATA$$RW$$Base),
                (uint32)(&Image$$INIT_DATA$$RW$$Length),

                (uint32)(&Image$$INIT_DATA$$ZI$$Base),
                (uint32)(&Image$$INIT_DATA$$ZI$$Length),
            },

            //PMU
            {
                (uint32)(&Load$$PMU_CODE$$Base),
                (uint32)(&Image$$PMU_CODE$$Base) + 0x03090000,
                (uint32)(&Image$$PMU_CODE$$Length),

                (uint32)(&Load$$PMU_DATA$$Base),
                (uint32)(&Image$$PMU_DATA$$RW$$Base) + 0x03090000,
                (uint32)(&Image$$PMU_DATA$$RW$$Length),

                (uint32)(&Image$$PMU_DATA$$ZI$$Base) + 0x03090000,
                (uint32)(&Image$$PMU_DATA$$ZI$$Length),
            },



            //SYS SUSPEND SEG
            {
                (uint32)(&Load$$SYS_SUSPEND_CODE$$Base),
                (uint32)(&Image$$SYS_SUSPEND_CODE$$Base),
                (uint32)(&Image$$SYS_SUSPEND_CODE$$Length),

                (uint32)(&Load$$SYS_SUSPEND_DATA$$Base),
                (uint32)(&Image$$SYS_SUSPEND_DATA$$RW$$Base),
                (uint32)(&Image$$SYS_SUSPEND_DATA$$RW$$Length),

                (uint32)(&Image$$SYS_SUSPEND_DATA$$ZI$$Base),
                (uint32)(&Image$$SYS_SUSPEND_DATA$$ZI$$Length),
            },


            //SYS RESUME SEG
            {
                (uint32)(&Load$$SYS_RESUME_CODE$$Base),
                (uint32)(&Image$$SYS_RESUME_CODE$$Base),
                (uint32)(&Image$$SYS_RESUME_CODE$$Length),

                (uint32)(&Load$$SYS_RESUME_DATA$$Base),
                (uint32)(&Image$$SYS_RESUME_DATA$$RW$$Base),
                (uint32)(&Image$$SYS_RESUME_DATA$$RW$$Length),

                (uint32)(&Image$$SYS_RESUME_DATA$$ZI$$Base),
                (uint32)(&Image$$SYS_RESUME_DATA$$ZI$$Length),
            },

			//MAIN TASK SEG
            {
                (uint32)(&Load$$MAIN_TASK_CODE$$Base),
                (uint32)(&Image$$MAIN_TASK_CODE$$Base),
                (uint32)(&Image$$MAIN_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MAIN INIT SEG
            {
                (uint32)(&Load$$MAIN_INIT_CODE$$Base),
                (uint32)(&Image$$MAIN_INIT_CODE$$Base),
                (uint32)(&Image$$MAIN_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //MUSIC SHELL SEG
            {
                (uint32)(&Load$$MUSIC_SHELL_CODE$$Base),
                (uint32)(&Image$$MUSIC_SHELL_CODE$$Base),
                (uint32)(&Image$$MUSIC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //EFFECT MODULE SEG
            {
                (uint32)(&Load$$EFFECT_MODULE_CODE$$Base),
                (uint32)(&Image$$EFFECT_MODULE_CODE$$Base),
                (uint32)(&Image$$EFFECT_MODULE_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //TIMER DEV SEG
            {
                (uint32)(&Load$$TIMER_DEV_CODE$$Base),
                (uint32)(&Image$$TIMER_DEV_CODE$$Base),
                (uint32)(&Image$$TIMER_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //TIMER INIT SEG
            {
                (uint32)(&Load$$TIMER_INIT_CODE$$Base),
                (uint32)(&Image$$TIMER_INIT_CODE$$Base),
                (uint32)(&Image$$TIMER_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //TIMER SHELL SEG
            {
                (uint32)(&Load$$TIMER_SHELL_CODE$$Base),
                (uint32)(&Image$$TIMER_SHELL_CODE$$Base),
                (uint32)(&Image$$TIMER_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ADC DEV SEG
            {
                (uint32)(&Load$$ADC_DEV_CODE$$Base),
                (uint32)(&Image$$ADC_DEV_CODE$$Base),
                (uint32)(&Image$$ADC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ADC INIT SEG
            {
                (uint32)(&Load$$ADC_INIT_CODE$$Base),
                (uint32)(&Image$$ADC_INIT_CODE$$Base),
                (uint32)(&Image$$ADC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ADC SHELL SEG
            {
                (uint32)(&Load$$ADC_SHELL_CODE$$Base),
                (uint32)(&Image$$ADC_SHELL_CODE$$Base),
                (uint32)(&Image$$ADC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //KEY DEV SEG
            {
                (uint32)(&Load$$KEY_DEV_CODE$$Base),
                (uint32)(&Image$$KEY_DEV_CODE$$Base),
                (uint32)(&Image$$KEY_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //KEY INIT SEG
            {
                (uint32)(&Load$$KEY_INIT_CODE$$Base),
                (uint32)(&Image$$KEY_INIT_CODE$$Base),
                (uint32)(&Image$$KEY_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //KEY SHELL SEG
            {
                (uint32)(&Load$$KEY_SHELL_CODE$$Base),
                (uint32)(&Image$$KEY_SHELL_CODE$$Base),
                (uint32)(&Image$$KEY_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //PWM DEV SEG
            {
                (uint32)(&Load$$PWM_DEV_CODE$$Base),
                (uint32)(&Image$$PWM_DEV_CODE$$Base),
                (uint32)(&Image$$PWM_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //PWM INIT SEG
            {
                (uint32)(&Load$$PWM_INIT_CODE$$Base),
                (uint32)(&Image$$PWM_INIT_CODE$$Base),
                (uint32)(&Image$$PWM_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //PWM SHELL SEG
            {
                (uint32)(&Load$$PWM_SHELL_CODE$$Base),
                (uint32)(&Image$$PWM_SHELL_CODE$$Base),
                (uint32)(&Image$$PWM_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DISPLAY DEV SEG
            {
                (uint32)(&Load$$DISPLAY_DEV_CODE$$Base),
                (uint32)(&Image$$DISPLAY_DEV_CODE$$Base),
                (uint32)(&Image$$DISPLAY_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DISPLAY INIT SEG
            {
                (uint32)(&Load$$DISPLAY_INIT_CODE$$Base),
                (uint32)(&Image$$DISPLAY_INIT_CODE$$Base),
                (uint32)(&Image$$DISPLAY_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DISPLAY SHELL SEG
            {
                (uint32)(&Load$$DISPLAY_SHELL_CODE$$Base),
                (uint32)(&Image$$DISPLAY_SHELL_CODE$$Base),
                (uint32)(&Image$$DISPLAY_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //GUI TASK SEG
            {
                (uint32)(&Load$$GUI_TASK_CODE$$Base),
                (uint32)(&Image$$GUI_TASK_CODE$$Base),
                (uint32)(&Image$$GUI_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //GUI INIT SEG
            {
                (uint32)(&Load$$GUI_INIT_CODE$$Base),
                (uint32)(&Image$$GUI_INIT_CODE$$Base),
                (uint32)(&Image$$GUI_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //BCORE DEV SEG
            {
                (uint32)(&Load$$BCORE_DEV_CODE$$Base),
                (uint32)(&Image$$BCORE_DEV_CODE$$Base),
                (uint32)(&Image$$BCORE_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //BCORE INIT SEG
            {
                (uint32)(&Load$$BCORE_INIT_CODE$$Base),
                (uint32)(&Image$$BCORE_INIT_CODE$$Base),
                (uint32)(&Image$$BCORE_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //BCORE SHELL SEG
            {
                (uint32)(&Load$$BCORE_SHELL_CODE$$Base),
                (uint32)(&Image$$BCORE_SHELL_CODE$$Base),
                (uint32)(&Image$$BCORE_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MSG DEV SEG
            {
                (uint32)(&Load$$MSG_DEV_CODE$$Base),
                (uint32)(&Image$$MSG_DEV_CODE$$Base),
                (uint32)(&Image$$MSG_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MSG INIT SEG
            {
                (uint32)(&Load$$MSG_INIT_CODE$$Base),
                (uint32)(&Image$$MSG_INIT_CODE$$Base),
                (uint32)(&Image$$MSG_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MSG SHELL SEG
            {
                (uint32)(&Load$$MSG_SHELL_CODE$$Base),
                (uint32)(&Image$$MSG_SHELL_CODE$$Base),
                (uint32)(&Image$$MSG_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DIR DEV SEG
            {
                (uint32)(&Load$$DIR_DEV_CODE$$Base),
                (uint32)(&Image$$DIR_DEV_CODE$$Base),
                (uint32)(&Image$$DIR_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DIR INIT SEG
            {
                (uint32)(&Load$$DIR_INIT_CODE$$Base),
                (uint32)(&Image$$DIR_INIT_CODE$$Base),
                (uint32)(&Image$$DIR_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //DIR SHELL SEG
            {
                (uint32)(&Load$$DIR_SHELL_CODE$$Base),
                (uint32)(&Image$$DIR_SHELL_CODE$$Base),
                (uint32)(&Image$$DIR_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FILESTREAM TASK SEG
            {
                (uint32)(&Load$$FILESTREAM_TASK_CODE$$Base),
                (uint32)(&Image$$FILESTREAM_TASK_CODE$$Base),
                (uint32)(&Image$$FILESTREAM_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //FILESTREAM INIT SEG
            {
                (uint32)(&Load$$FILESTREAM_INIT_CODE$$Base),
                (uint32)(&Image$$FILESTREAM_INIT_CODE$$Base),
                (uint32)(&Image$$FILESTREAM_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //HTTP TASK SEG
            {
                (uint32)(&Load$$HTTP_TASK_CODE$$Base),
                (uint32)(&Image$$HTTP_TASK_CODE$$Base),
                (uint32)(&Image$$HTTP_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //HTTP INIT SEG
            {
                (uint32)(&Load$$HTTP_INIT_CODE$$Base),
                (uint32)(&Image$$HTTP_INIT_CODE$$Base),
                (uint32)(&Image$$HTTP_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //MEDIA TASK SEG
            {
                (uint32)(&Load$$MEDIA_TASK_CODE$$Base),
                (uint32)(&Image$$MEDIA_TASK_CODE$$Base),
                (uint32)(&Image$$MEDIA_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MEDIA INIT SEG
            {
                (uint32)(&Load$$MEDIA_INIT_CODE$$Base),
                (uint32)(&Image$$MEDIA_INIT_CODE$$Base),
                (uint32)(&Image$$MEDIA_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //STREAMCONTROL TASK SEG
            {
                (uint32)(&Load$$STREAMCONTROL_TASK_CODE$$Base),
                (uint32)(&Image$$STREAMCONTROL_TASK_CODE$$Base),
                (uint32)(&Image$$STREAMCONTROL_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //STREAMCONTROL INIT SEG
            {
                (uint32)(&Load$$STREAMCONTROL_INIT_CODE$$Base),
                (uint32)(&Image$$STREAMCONTROL_INIT_CODE$$Base),
                (uint32)(&Image$$STREAMCONTROL_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //AUDIOCONTROL TASK SEG
            {
                (uint32)(&Load$$AUDIOCONTROL_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOCONTROL_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOCONTROL_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIOCONTROL INIT SEG
            {
                (uint32)(&Load$$AUDIOCONTROL_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOCONTROL_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOCONTROL_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2S DEV SEG
            {
                (uint32)(&Load$$I2S_DEV_CODE$$Base),
                (uint32)(&Image$$I2S_DEV_CODE$$Base),
                (uint32)(&Image$$I2S_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2S INIT SEG
            {
                (uint32)(&Load$$I2S_INIT_CODE$$Base),
                (uint32)(&Image$$I2S_INIT_CODE$$Base),
                (uint32)(&Image$$I2S_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2S SHELL SEG
            {
                (uint32)(&Load$$I2S_SHELL_CODE$$Base),
                (uint32)(&Image$$I2S_SHELL_CODE$$Base),
                (uint32)(&Image$$I2S_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ROCKCODEC DEV SEG
            {
                (uint32)(&Load$$ROCKCODEC_DEV_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_DEV_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ROCKCODEC INIT SEG
            {
                (uint32)(&Load$$ROCKCODEC_INIT_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_INIT_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //ROCKCODEC SHELL SEG
            {
                (uint32)(&Load$$ROCKCODEC_SHELL_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_SHELL_CODE$$Base),
                (uint32)(&Image$$ROCKCODEC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIO DEV SEG
            {
                (uint32)(&Load$$AUDIO_DEV_CODE$$Base),
                (uint32)(&Image$$AUDIO_DEV_CODE$$Base),
                (uint32)(&Image$$AUDIO_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIO INIT SEG
            {
                (uint32)(&Load$$AUDIO_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIO_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIO_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIO SHELL SEG
            {
                (uint32)(&Load$$AUDIO_SHELL_CODE$$Base),
                (uint32)(&Image$$AUDIO_SHELL_CODE$$Base),
                (uint32)(&Image$$AUDIO_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIOSUB TASK SEG
            {
                (uint32)(&Load$$AUDIOSUB_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIOSUB INIT SEG
            {
                (uint32)(&Load$$AUDIOSUB_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //AUDIOMAIN TASK SEG
            {
                (uint32)(&Load$$AUDIOMAIN_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_TASK_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_TASK_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //AUDIOMAIN INIT SEG
            {
                (uint32)(&Load$$AUDIOMAIN_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_INIT_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBMSC DEV SEG
            {
                (uint32)(&Load$$USBMSC_DEV_CODE$$Base),
                (uint32)(&Image$$USBMSC_DEV_CODE$$Base),
                (uint32)(&Image$$USBMSC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBMSC INIT SEG
            {
                (uint32)(&Load$$USBMSC_INIT_CODE$$Base),
                (uint32)(&Image$$USBMSC_INIT_CODE$$Base),
                (uint32)(&Image$$USBMSC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBMSC SHELL SEG
            {
                (uint32)(&Load$$USBMSC_SHELL_CODE$$Base),
                (uint32)(&Image$$USBMSC_SHELL_CODE$$Base),
                (uint32)(&Image$$USBMSC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MAILBOX DEV SEG
            {
                (uint32)(&Load$$MAILBOX_DEV_CODE$$Base),
                (uint32)(&Image$$MAILBOX_DEV_CODE$$Base),
                (uint32)(&Image$$MAILBOX_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MAILBOX INIT SEG
            {
                (uint32)(&Load$$MAILBOX_INIT_CODE$$Base),
                (uint32)(&Image$$MAILBOX_INIT_CODE$$Base),
                (uint32)(&Image$$MAILBOX_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //MAILBOX SHELL SEG
            {
                (uint32)(&Load$$MAILBOX_SHELL_CODE$$Base),
                (uint32)(&Image$$MAILBOX_SHELL_CODE$$Base),
                (uint32)(&Image$$MAILBOX_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //RK618 DEV SEG
            {
                (uint32)(&Load$$RK618_DEV_CODE$$Base),
                (uint32)(&Image$$RK618_DEV_CODE$$Base),
                (uint32)(&Image$$RK618_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //RK618 INIT SEG
            {
                (uint32)(&Load$$RK618_INIT_CODE$$Base),
                (uint32)(&Image$$RK618_INIT_CODE$$Base),
                (uint32)(&Image$$RK618_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //RK618 SHELL SEG
            {
                (uint32)(&Load$$RK618_SHELL_CODE$$Base),
                (uint32)(&Image$$RK618_SHELL_CODE$$Base),
                (uint32)(&Image$$RK618_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FIFO DEV SEG
            {
                (uint32)(&Load$$FIFO_DEV_CODE$$Base),
                (uint32)(&Image$$FIFO_DEV_CODE$$Base),
                (uint32)(&Image$$FIFO_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //FIFO INIT SEG
            {
                (uint32)(&Load$$FIFO_INIT_CODE$$Base),
                (uint32)(&Image$$FIFO_INIT_CODE$$Base),
                (uint32)(&Image$$FIFO_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //FIFO SHELL SEG
            {
                (uint32)(&Load$$FIFO_SHELL_CODE$$Base),
                (uint32)(&Image$$FIFO_SHELL_CODE$$Base),
                (uint32)(&Image$$FIFO_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SD DEV SEG
            {
                (uint32)(&Load$$SD_DEV_CODE$$Base),
                (uint32)(&Image$$SD_DEV_CODE$$Base),
                (uint32)(&Image$$SD_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SD INIT SEG
            {
                (uint32)(&Load$$SD_INIT_CODE$$Base),
                (uint32)(&Image$$SD_INIT_CODE$$Base),
                (uint32)(&Image$$SD_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SD SHELL SEG
            {
                (uint32)(&Load$$SD_SHELL_CODE$$Base),
                (uint32)(&Image$$SD_SHELL_CODE$$Base),
                (uint32)(&Image$$SD_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2C DEV SEG
            {
                (uint32)(&Load$$I2C_DEV_CODE$$Base),
                (uint32)(&Image$$I2C_DEV_CODE$$Base),
                (uint32)(&Image$$I2C_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2C INIT SEG
            {
                (uint32)(&Load$$I2C_INIT_CODE$$Base),
                (uint32)(&Image$$I2C_INIT_CODE$$Base),
                (uint32)(&Image$$I2C_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //I2C SHELL SEG
            {
                (uint32)(&Load$$I2C_SHELL_CODE$$Base),
                (uint32)(&Image$$I2C_SHELL_CODE$$Base),
                (uint32)(&Image$$I2C_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBOTG DEV SEG
            {
                (uint32)(&Load$$USBOTG_DEV_CODE$$Base),
                (uint32)(&Image$$USBOTG_DEV_CODE$$Base),
                (uint32)(&Image$$USBOTG_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBOTG INIT SEG
            {
                (uint32)(&Load$$USBOTG_INIT_CODE$$Base),
                (uint32)(&Image$$USBOTG_INIT_CODE$$Base),
                (uint32)(&Image$$USBOTG_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //USBOTG SHELL SEG
            {
                (uint32)(&Load$$USBOTG_SHELL_CODE$$Base),
                (uint32)(&Image$$USBOTG_SHELL_CODE$$Base),
                (uint32)(&Image$$USBOTG_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SDC DEV SEG
            {
                (uint32)(&Load$$SDC_DEV_CODE$$Base),
                (uint32)(&Image$$SDC_DEV_CODE$$Base),
                (uint32)(&Image$$SDC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //SDC INIT SEG
            {
                (uint32)(&Load$$SDC_INIT_CODE$$Base),
                (uint32)(&Image$$SDC_INIT_CODE$$Base),
                (uint32)(&Image$$SDC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //SDC SHELL SEG
            {
                (uint32)(&Load$$SDC_SHELL_CODE$$Base),
                (uint32)(&Image$$SDC_SHELL_CODE$$Base),
                (uint32)(&Image$$SDC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //EMMC DEV SEG
            {
                (uint32)(&Load$$EMMC_DEV_CODE$$Base),
                (uint32)(&Image$$EMMC_DEV_CODE$$Base),
                (uint32)(&Image$$EMMC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //EMMC INIT SEG
            {
                (uint32)(&Load$$EMMC_INIT_CODE$$Base),
                (uint32)(&Image$$EMMC_INIT_CODE$$Base),
                (uint32)(&Image$$EMMC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //EMMC SHELL SEG
            {
                (uint32)(&Load$$EMMC_SHELL_CODE$$Base),
                (uint32)(&Image$$EMMC_SHELL_CODE$$Base),
                (uint32)(&Image$$EMMC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //LUN DEV SEG
            {
                (uint32)(&Load$$LUN_DEV_CODE$$Base),
                (uint32)(&Image$$LUN_DEV_CODE$$Base),
                (uint32)(&Image$$LUN_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //LUN INIT SEG
            {
                (uint32)(&Load$$LUN_INIT_CODE$$Base),
                (uint32)(&Image$$LUN_INIT_CODE$$Base),
                (uint32)(&Image$$LUN_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //LUN SHELL SEG
            {
                (uint32)(&Load$$LUN_SHELL_CODE$$Base),
                (uint32)(&Image$$LUN_SHELL_CODE$$Base),
                (uint32)(&Image$$LUN_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //PAR DEV SEG
            {
                (uint32)(&Load$$PAR_DEV_CODE$$Base),
                (uint32)(&Image$$PAR_DEV_CODE$$Base),
                (uint32)(&Image$$PAR_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //PAR INIT SEG
            {
                (uint32)(&Load$$PAR_INIT_CODE$$Base),
                (uint32)(&Image$$PAR_INIT_CODE$$Base),
                (uint32)(&Image$$PAR_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //PAR SHELL SEG
            {
                (uint32)(&Load$$PAR_SHELL_CODE$$Base),
                (uint32)(&Image$$PAR_SHELL_CODE$$Base),
                (uint32)(&Image$$PAR_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FAT DEV SEG
            {
                (uint32)(&Load$$FAT_DEV_CODE$$Base),
                (uint32)(&Image$$FAT_DEV_CODE$$Base),
                (uint32)(&Image$$FAT_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FAT INIT SEG
            {
                (uint32)(&Load$$FAT_INIT_CODE$$Base),
                (uint32)(&Image$$FAT_INIT_CODE$$Base),
                (uint32)(&Image$$FAT_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FAT SHELL SEG
            {
                (uint32)(&Load$$FAT_SHELL_CODE$$Base),
                (uint32)(&Image$$FAT_SHELL_CODE$$Base),
                (uint32)(&Image$$FAT_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //FILE DEV SEG
            {
                (uint32)(&Load$$FILE_DEV_CODE$$Base),
                (uint32)(&Image$$FILE_DEV_CODE$$Base),
                (uint32)(&Image$$FILE_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FILE INIT SEG
            {
                (uint32)(&Load$$FILE_INIT_CODE$$Base),
                (uint32)(&Image$$FILE_INIT_CODE$$Base),
                (uint32)(&Image$$FILE_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //FILE SHELL SEG
            {
                (uint32)(&Load$$FILE_SHELL_CODE$$Base),
                (uint32)(&Image$$FILE_SHELL_CODE$$Base),
                (uint32)(&Image$$FILE_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SDIO DEV SEG
            {
                (uint32)(&Load$$SDIO_DEV_CODE$$Base),
                (uint32)(&Image$$SDIO_DEV_CODE$$Base),
                (uint32)(&Image$$SDIO_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //SDIO INIT SEG
            {
                (uint32)(&Load$$SDIO_INIT_CODE$$Base),
                (uint32)(&Image$$SDIO_INIT_CODE$$Base),
                (uint32)(&Image$$SDIO_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //SDIO SHELL SEG
            {
                (uint32)(&Load$$SDIO_SHELL_CODE$$Base),
                (uint32)(&Image$$SDIO_SHELL_CODE$$Base),
                (uint32)(&Image$$SDIO_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //RK903 DEV SEG
            {
                (uint32)(&Load$$RK903_DEV_CODE$$Base),
                (uint32)(&Image$$RK903_DEV_CODE$$Base),
                (uint32)(&Image$$RK903_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //RK903 INIT SEG
            {
                (uint32)(&Load$$RK903_INIT_CODE$$Base),
                (uint32)(&Image$$RK903_INIT_CODE$$Base),
                (uint32)(&Image$$RK903_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //RK903 SHELL SEG
            {
                (uint32)(&Load$$RK903_SHELL_CODE$$Base),
                (uint32)(&Image$$RK903_SHELL_CODE$$Base),
                (uint32)(&Image$$RK903_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //WLC DEV SEG
            {
                (uint32)(&Load$$WLC_DEV_CODE$$Base),
                (uint32)(&Image$$WLC_DEV_CODE$$Base),
                (uint32)(&Image$$WLC_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //WLC INIT SEG
            {
                (uint32)(&Load$$WLC_INIT_CODE$$Base),
                (uint32)(&Image$$WLC_INIT_CODE$$Base),
                (uint32)(&Image$$WLC_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },


            //WLC SHELL SEG
            {
                (uint32)(&Load$$WLC_SHELL_CODE$$Base),
                (uint32)(&Image$$WLC_SHELL_CODE$$Base),
                (uint32)(&Image$$WLC_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },
            // INNO wifi dev seg
            {
                (uint32)(&Load$$INNOWIFI_DEV_CODE$$Base),
                (uint32)(&Image$$INNOWIFI_DEV_CODE$$Base),
                (uint32)(&Image$$INNOWIFI_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },
                //ap6181
            {
                (uint32)(&Load$$AP6181WIFI_INIT_CODE$$Base),
                (uint32)(&Image$$AP6181WIFI_INIT_CODE$$Base),
                (uint32)(&Image$$AP6181WIFI_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            {
                (uint32)(&Load$$AP6181WIFI_DEV_CODE$$Base),
                (uint32)(&Image$$AP6181WIFI_DEV_CODE$$Base),
                (uint32)(&Image$$AP6181WIFI_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },
            //dlna
            {
                (uint32)(&Load$$DLNA_DEV_CODE$$Base),
                (uint32)(&Image$$DLNA_DEV_CODE$$Base),
                (uint32)(&Image$$DLNA_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },
            //upnp
            {
                (uint32)(&Load$$UPNP_DEV_CODE$$Base),
                (uint32)(&Image$$UPNP_DEV_CODE$$Base),
                (uint32)(&Image$$UPNP_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },

            // lwip seg
            {
                (uint32)(&Load$$LWIP_DEV_CODE$$Base),
                (uint32)(&Image$$LWIP_DEV_CODE$$Base),
                (uint32)(&Image$$LWIP_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            //SPIFLASH SEG MODULE

            {
                (uint32)(&Load$$SPIFLASH_INIT_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_INIT_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_INIT_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            {
                (uint32)(&Load$$SPIFLASH_DEV_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_DEV_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_DEV_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },

            {
                (uint32)(&Load$$SPIFLASH_SHELL_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_SHELL_CODE$$Base),
                (uint32)(&Image$$SPIFLASH_SHELL_CODE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
            },
            //WIFI FIRMWAVE ONE
            {
                (uint32)(&Load$$WIFI_FIRMWARE_ONE$$Base),
                NULL,
                (uint32)(&Image$$WIFI_FIRMWARE_ONE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },

            //WIFI FIRMWAVE TWO
            {
                (uint32)(&Load$$WIFI_FIRMWARE_TWO$$Base),
                NULL,
                (uint32)(&Image$$WIFI_FIRMWARE_TWO$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },

            //BT FIRMWAVE
            {
                (uint32)(&Load$$BT_FIRMWARE$$Base),
                NULL,
                (uint32)(&Image$$BT_FIRMWARE$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },

            //AUDIO EQTABLE
            {
                (uint32)(&Load$$AUDIO_EQ_DATA$$Base),
                 NULL,
                (uint32)(&Image$$AUDIO_EQ_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },

            //xml
            {
                (uint32)(&Load$$DMR_DEVICE_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_DEVICE_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },
            {
                (uint32)(&Load$$DMR_CONNECTIONMANAGER_SERVICE_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_CONNECTIONMANAGER_SERVICE_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },
            {
                (uint32)(&Load$$DMR_RENDERINGCONTROL_SERVICE_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_RENDERINGCONTROL_SERVICE_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },
            {
                (uint32)(&Load$$DMR_AVTRANSPORT_SERVICE_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_AVTRANSPORT_SERVICE_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,

            },
            //nametab
           {
                (uint32)(&Load$$DMR_NAMINGBITMAP_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_NAMINGBITMAP_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
           },
           {
                (uint32)(&Load$$DMR_NMSTRTPAGES_DATA$$Base),
                NULL,
                (uint32)(&Image$$DMR_NMSTRTPAGES_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
           },
            {
                (uint32)(&Load$$DMR_NAMEPAGES_DATA$$Base),
                 NULL,
                (uint32)(&Image$$DMR_NAMEPAGES_DATA$$Length),

                NULL,
                NULL,
                NULL,

                NULL,
                NULL,
             },

            //BB Module
            {
                (uint32)(&Load$$BB_SYS_CODE$$Base),
                (uint32)(&Image$$BB_SYS_CODE$$Base),
                (uint32)(&Image$$BB_SYS_CODE$$Length),
                (uint32)(&Load$$BB_SYS_DATA$$Base),
                (uint32)(&Image$$BB_SYS_DATA$$RW$$Base),
                (uint32)(&Image$$BB_SYS_DATA$$RW$$Length),
                (uint32)(&Image$$BB_SYS_DATA$$ZI$$Base),
                (uint32)(&Image$$BB_SYS_DATA$$ZI$$Length),
            },

            //MP3
            {
                (uint32)(&Load$$MP3_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$MP3_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$MP3_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$MP3_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$MP3_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$MP3_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$MP3_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$MP3_DECODE_BIN_DATA$$ZI$$Length),
            },

            //WMA
            {
                (uint32)(&Load$$WMA_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$WMA_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$WMA_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$WMA_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$WMA_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$WMA_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$WMA_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$WMA_DECODE_BIN_DATA$$ZI$$Length),
            },

            //WAV
            {
                (uint32)(&Load$$WAV_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$WAV_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$WAV_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$WAV_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$WAV_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$WAV_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$WAV_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$WAV_DECODE_BIN_DATA$$ZI$$Length),
            },


            //FLAC
            {
                (uint32)(&Load$$FLAC_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$FLAC_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$FLAC_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$FLAC_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$FLAC_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$FLAC_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$FLAC_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$FLAC_DECODE_BIN_DATA$$ZI$$Length),
            },


            //AAC
            {
                (uint32)(&Load$$AAC_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$AAC_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$AAC_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$AAC_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$AAC_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$AAC_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$AAC_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$AAC_DECODE_BIN_DATA$$ZI$$Length),
            },

            //APE
            {
                (uint32)(&Load$$APE_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$APE_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$APE_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$APE_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$APE_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$APE_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$APE_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$APE_DECODE_BIN_DATA$$ZI$$Length),
            },

            //OGG
            {
                (uint32)(&Load$$OGG_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$OGG_DECODE_BIN_CODE$$Base),
                (uint32)(&Image$$OGG_DECODE_BIN_CODE$$Length),
                (uint32)(&Load$$OGG_DECODE_BIN_DATA$$Base),
                (uint32)(&Image$$OGG_DECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$OGG_DECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$OGG_DECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$OGG_DECODE_BIN_DATA$$ZI$$Length),
            },



            //HIFI FLAC
            {
                (uint32)(&Load$$FLAC_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$FLAC_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$FLAC_HDECODE_BIN_CODE$$Length),
                (uint32)(&Load$$FLAC_HDECODE_BIN_DATA$$Base),
                (uint32)(&Image$$FLAC_HDECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$FLAC_HDECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$FLAC_HDECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$FLAC_HDECODE_BIN_DATA$$ZI$$Length),
            },

            //HIFI ALAC
            {
                (uint32)(&Load$$ALAC_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$ALAC_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$ALAC_HDECODE_BIN_CODE$$Length),
                (uint32)(&Load$$ALAC_HDECODE_BIN_DATA$$Base),
                (uint32)(&Image$$ALAC_HDECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$ALAC_HDECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$ALAC_HDECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$ALAC_HDECODE_BIN_DATA$$ZI$$Length),
            },

            //HIFI APE
            {
                (uint32)(&Load$$APE_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$APE_HDECODE_BIN_CODE$$Base),
                (uint32)(&Image$$APE_HDECODE_BIN_CODE$$Length),
                (uint32)(&Load$$APE_HDECODE_BIN_DATA$$Base),
                (uint32)(&Image$$APE_HDECODE_BIN_DATA$$RW$$Base),
                (uint32)(&Image$$APE_HDECODE_BIN_DATA$$RW$$Length),
                (uint32)(&Image$$APE_HDECODE_BIN_DATA$$ZI$$Base),
                (uint32)(&Image$$APE_HDECODE_BIN_DATA$$ZI$$Length),
            },
        }
    }
};

#endif

/*
--------------------------------------------------------------------------------
  Function name :
  Author        : ZHengYongzhi
  Description   : 模块信息表

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
__attribute__((section("AP_START_CODE")))
void ScatterLoader(void)
{
    uint32 i,len;
    uint8  *pDest;

    //clear sys bss data
    pDest = (uint8*)((uint32)(&Image$$AP_SYS_DATA$$ZI$$Base));
    len = (uint32)((uint32)(&Image$$AP_SYS_DATA$$ZI$$Length));
    for (i = 0; i < len; i++)
    {
        *pDest++ = 0;
    }
}

__attribute__((section("AP_START_CODE")))
void StartCASystem(void)
{
    uint32 *core2reset;
    uint32 i;
    core2reset = (uint*)0x401808F8;
    *core2reset = 0;

    while (i < 0xffff)
        i++;

    *core2reset |= 0x80;

#if 0
    while (1)
    {
        MailBoxWriteA2BCmd(0,0,0);
        MailBoxWriteA2BData(0,0,0);
        //__WFI();
    }
#endif

}
/*
********************************************************************************
*
*                         End of StartLoadTab.c
*
********************************************************************************
*/
