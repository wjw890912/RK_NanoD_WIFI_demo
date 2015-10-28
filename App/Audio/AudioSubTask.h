/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioSubTask.h
* Owner: cjh
* Date: 2015.5.27
* Time: 10:50:57
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    cjh     2015.5.27     10:50:57   1.0
********************************************************************************************
*/


#ifndef __APP_AUDIO_AUDIOSUBTASK_H__
#define __APP_AUDIO_AUDIOSUBTASK_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

typedef struct _RK_TASK_AUDIOSUB_ARG
{
    uint32 xxx;
}RK_TASK_AUDIOSUB_ARG;

#define _APP_AUDIO_AUDIOSUBTASK_COMMON_  __attribute__((section("app_audio_audiosubtask_common")))
#define _APP_AUDIO_AUDIOSUBTASK_INIT_  __attribute__((section("app_audio_audiosubtask_init")))
#define _APP_AUDIO_AUDIOSUBTASK_SHELL_  __attribute__((section("app_audio_audiosubtask_shell")))
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern void AudioSubTask_Enter(void);
extern rk_err_t AudioSubTask_DeInit(void *pvParameters);
extern rk_err_t AudioSubTask_Init(void *pvParameters, void *arg);



#endif
