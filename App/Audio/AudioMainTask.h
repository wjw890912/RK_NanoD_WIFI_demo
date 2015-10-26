/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioMainTask.h
* Owner: cjh
* Date: 2015.5.27
* Time: 10:50:49
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    cjh     2015.5.27     10:50:49   1.0
********************************************************************************************
*/


#ifndef __APP_AUDIO_AUDIOMAINTASK_H__
#define __APP_AUDIO_AUDIOMAINTASK_H__

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

typedef struct _RK_TASK_AUDIOMAIN_ARG
{
    uint32 ARG;
}RK_TASK_AUDIOMAIN_ARG;

#define _APP_AUDIO_AUDIOMAINTASK_COMMON_  __attribute__((section("app_audio_audiomaintask_common")))
#define _APP_AUDIO_AUDIOMAINTASK_INIT_  __attribute__((section("app_audio_audiomaintask_init")))
#define _APP_AUDIO_AUDIOMAINTASK_SHELL_  __attribute__((section("app_audio_audiomaintask_shell")))
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
extern rk_err_t AudioTask_Shell(HDC dev, uint8 * pstr);
extern void AudioMainTask_Enter(void);
extern rk_err_t AudioMainTask_DeInit(void *pvParameters);
extern rk_err_t AudioMainTask_Init(void *pvParameters, void *arg);



#endif
