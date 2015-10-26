/*
********************************************************************************
*                   Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* File Name：   StartLoadTab.c
*
* Description:  定义模块信息，在模块调度时需要使用
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2015-02-06         1.0
*    desc:    ORG.
********************************************************************************
*/
#define _IN_STARTLOAD_

#include "BspCOnfig.h"
#include "RKOS.h"
#include "Bsp.h"
#include "moduleInfoTab.h"

extern uint32 Load$$MODULE_INFO$$Base;

//SYS MODULE
extern uint32 Load$$SYS_CODE$$Base;
extern uint32 Image$$SYS_CODE$$Base;
extern uint32 Image$$SYS_CODE$$Length;
extern uint32 Load$$SYS_DATA$$Base;
extern uint32 Image$$SYS_DATA$$RW$$Base;
extern uint32 Image$$SYS_DATA$$RW$$Length;
extern uint32 Image$$SYS_DATA$$ZI$$Base;
extern uint32 Image$$SYS_DATA$$ZI$$Length;

//INIT MODULE
extern uint32 Load$$INIT_CODE$$Base;
extern uint32 Image$$INIT_CODE$$Base;
extern uint32 Image$$INIT_CODE$$Length;
extern uint32 Load$$INIT_DATA$$Base;
extern uint32 Image$$INIT_DATA$$RW$$Base;
extern uint32 Image$$INIT_DATA$$RW$$Length;
extern uint32 Image$$INIT_DATA$$ZI$$Base;
extern uint32 Image$$INIT_DATA$$ZI$$Length;

//INIT PAD MODULE
extern uint32 Load$$INIT_PAD_CODE$$Base;
extern uint32 Image$$INIT_PAD_CODE$$Base;
extern uint32 Image$$INIT_PAD_CODE$$Length;
extern uint32 Load$$INIT_PAD_DATA$$Base;
extern uint32 Image$$INIT_PAD_DATA$$RW$$Base;
extern uint32 Image$$INIT_PAD_DATA$$RW$$Length;
extern uint32 Image$$INIT_PAD_DATA$$ZI$$Base;
extern uint32 Image$$INIT_PAD_DATA$$ZI$$Length;


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
extern uint32 Load$$AUDIOSUB_COMMON_CODE$$Base;
extern uint32 Image$$AUDIOSUB_COMMON_CODE$$Base;
extern uint32 Image$$AUDIOSUB_COMMON_CODE$$Length;

extern uint32 Load$$AUDIOSUB_INIT_CODE$$Base;
extern uint32 Image$$AUDIOSUB_INIT_CODE$$Base;
extern uint32 Image$$AUDIOSUB_INIT_CODE$$Length;


//AUDIOMAIN SEG MODULE
extern uint32 Load$$AUDIOMAIN_COMMON_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_COMMON_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_COMMON_CODE$$Length;

extern uint32 Load$$AUDIOMAIN_INIT_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_INIT_CODE$$Base;
extern uint32 Image$$AUDIOMAIN_INIT_CODE$$Length;
//USBMSC SEG MODULE
extern uint32 Load$$USBMSC_COMMON_CODE$$Base;
extern uint32 Image$$USBMSC_COMMON_CODE$$Base;
extern uint32 Image$$USBMSC_COMMON_CODE$$Length;

extern uint32 Load$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Length;

extern uint32 Load$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Length;

//USBMSC SEG MODULE
extern uint32 Load$$USBMSC_COMMON_CODE$$Base;
extern uint32 Image$$USBMSC_COMMON_CODE$$Base;
extern uint32 Image$$USBMSC_COMMON_CODE$$Length;

extern uint32 Load$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Base;
extern uint32 Image$$USBMSC_INIT_CODE$$Length;

extern uint32 Load$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Base;
extern uint32 Image$$USBMSC_SHELL_CODE$$Length;

//MAILBOX SEG MODULE
extern uint32 Load$$MAILBOX_COMMON_CODE$$Base;
extern uint32 Image$$MAILBOX_COMMON_CODE$$Base;
extern uint32 Image$$MAILBOX_COMMON_CODE$$Length;

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


//WIFI FIRMWARE MODULE
extern uint32 Load$$WIFI_FIRMWARE_ONE$$Base;
extern uint32 Image$$WIFI_FIRMWARE_ONE$$Length;
extern uint32 Load$$WIFI_FIRMWARE_TWO$$Base;
extern uint32 Image$$WIFI_FIRMWARE_TWO$$Length;

//BT FIRMWARE MODULE
extern uint32 Load$$BT_FIRMWARE$$Base;
extern uint32 Image$$BT_FIRMWARE$$Length;


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
__attribute__((section("ModuleInfo")))
FIRMWARE_INFO_T const ModuleInfo  =
{

    (uint32)(&Load$$MODULE_INFO$$Base),

    //MODUE_INFO_TABLE
    {
        (uint32)(MAX_MODULE_NUM),
        {
            //SYS SEG
            {
                (uint32)(&Load$$SYS_CODE$$Base),
                (uint32)(&Image$$SYS_CODE$$Base),
                (uint32)(&Image$$SYS_CODE$$Length),

                (uint32)(&Load$$SYS_DATA$$Base),
                (uint32)(&Image$$SYS_DATA$$RW$$Base),
                (uint32)(&Image$$SYS_DATA$$RW$$Length),

                (uint32)(&Image$$SYS_DATA$$ZI$$Base),
                (uint32)(&Image$$SYS_DATA$$ZI$$Length),
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

            //INIT PAD SEG
            {
                (uint32)(&Load$$INIT_PAD_CODE$$Base),
                (uint32)(&Image$$INIT_PAD_CODE$$Base),
                (uint32)(&Image$$INIT_PAD_CODE$$Length),

                (uint32)(&Load$$INIT_PAD_DATA$$Base),
                (uint32)(&Image$$INIT_PAD_DATA$$RW$$Base),
                (uint32)(&Image$$INIT_PAD_DATA$$RW$$Length),

                (uint32)(&Image$$INIT_PAD_DATA$$ZI$$Base),
                (uint32)(&Image$$INIT_PAD_DATA$$ZI$$Length),
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
                (uint32)(&Load$$AUDIOSUB_COMMON_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_COMMON_CODE$$Base),
                (uint32)(&Image$$AUDIOSUB_COMMON_CODE$$Length),

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
                (uint32)(&Load$$AUDIOMAIN_COMMON_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_COMMON_CODE$$Base),
                (uint32)(&Image$$AUDIOMAIN_COMMON_CODE$$Length),

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
                (uint32)(&Load$$USBMSC_COMMON_CODE$$Base),
                (uint32)(&Image$$USBMSC_COMMON_CODE$$Base),
                (uint32)(&Image$$USBMSC_COMMON_CODE$$Length),

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

            //USBMSC DEV SEG
            {
                (uint32)(&Load$$USBMSC_COMMON_CODE$$Base),
                (uint32)(&Image$$USBMSC_COMMON_CODE$$Base),
                (uint32)(&Image$$USBMSC_COMMON_CODE$$Length),

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
                (uint32)(&Load$$MAILBOX_COMMON_CODE$$Base),
                (uint32)(&Image$$MAILBOX_COMMON_CODE$$Base),
                (uint32)(&Image$$MAILBOX_COMMON_CODE$$Length),

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
__attribute__((section("START_CODE")))
void ScatterLoader(void)
{
    uint32 i,len;
    uint8  *pDest;

    //clear sys bss data
    pDest = (uint8*)((uint32)(&Image$$SYS_DATA$$ZI$$Base));
    len = (uint32)((uint32)(&Image$$SYS_DATA$$ZI$$Length));
    for (i = 0; i < len; i++)
    {
        *pDest++ = 0;
    }

}

/*
********************************************************************************
*
*                         End of StartLoadTab.c
*
********************************************************************************
*/
