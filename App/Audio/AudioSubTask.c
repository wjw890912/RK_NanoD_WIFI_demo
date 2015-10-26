/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioSubTask.c
* Owner: cjh
* Date: 2015.5.27
* Time: 10:50:53
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    cjh     2015.5.27     10:50:53   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __APP_AUDIO_AUDIOSUBTASK_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _AUDIOSUB_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}AUDIOSUB_RESP_QUEUE;
typedef  struct _AUDIOSUB_ASK_QUEUE
{
    uint32 cmd;

}AUDIOSUB_ASK_QUEUE;
typedef  struct _AUDIOSUB_TASK_DATA_BLOCK
{
    xQueueHandle  AudioSubAskQueue;
    xQueueHandle  AudioSubRespQueue;

}AUDIOSUB_TASK_DATA_BLOCK;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static AUDIOSUB_TASK_DATA_BLOCK * gpstAudioSubData;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//#define AudioMix
#ifdef AudioMix
uint8 Audiosuboutptr[32][176] =
{
    //0
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //8
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //16
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //24
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //32
};
#endif
const uint32 AudioSublength = 176 * 32 / 4;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t SubAudioWriteData(HDC dev, uint32 TrackNo, uint8 * buf);
rk_err_t AudioSubTask_Resume(void);
rk_err_t AudioSubTask_Suspend(void);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioSubTask_Enter
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:43
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_COMMON_
COMMON API void AudioSubTask_Enter(void)
{
    HDC hRockCodec;
    uint32 subno;

    hRockCodec = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((hRockCodec == NULL) || (hRockCodec == (HDC)RK_ERROR) || (hRockCodec == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Audio device open failure");
    }

    AudioDev_GetMainTrack(hRockCodec);
    subno=AudioDev_GetSubTrack(hRockCodec);

    AudioDev_SetTrackProcess(hRockCodec, subno, AUDIO_PROCESS_MIX);

    AudioDev_SetSubTrackLen(hRockCodec, subno, AudioSublength);

    AudioDev_SetMixScale(hRockCodec, subno, 50, 5);

    while (1)
    {
#ifdef AudioMix
        SubAudioWriteData(hRockCodec, subno, (uint8 *)Audiosuboutptr);
#endif
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SubAudioWriteData
** Input:HDC dev, uint8 * buf
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 11:41:52
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_COMMON_
COMMON FUN rk_err_t SubAudioWriteData(HDC dev, uint32 TrackNo, uint8 * buf)
{

    AudioDev_Write(dev,TrackNo,buf);
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: AudioSubTask_Resume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:43
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_COMMON_
COMMON FUN rk_err_t AudioSubTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: AudioSubTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:43
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_COMMON_
COMMON FUN rk_err_t AudioSubTask_Suspend(void)
{
    return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioSubTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:43
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_INIT_
INIT API rk_err_t AudioSubTask_DeInit(void *pvParameters)
{
    rkos_queue_delete(gpstAudioSubData->AudioSubAskQueue);
    rkos_queue_delete(gpstAudioSubData->AudioSubRespQueue);

    return RK_SUCCESS;
    //rkos_memory_free(pAudioSubTaskData);
}
/*******************************************************************************
** Name: AudioSubTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:43
*******************************************************************************/
_APP_AUDIO_AUDIOSUBTASK_INIT_
INIT API rk_err_t AudioSubTask_Init(void *pvParameters, void *arg)
{

    RK_TASK_CLASS*   pAudioSubTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_AUDIOSUB_ARG * pArg = (RK_TASK_AUDIOSUB_ARG *)arg;
    AUDIOSUB_TASK_DATA_BLOCK*  pAudioSubTaskData;
    if (pAudioSubTask == NULL)
        return RK_PARA_ERR;

    pAudioSubTaskData->AudioSubAskQueue       = rkos_semaphore_create(1,1);
    pAudioSubTaskData->AudioSubRespQueue     = rkos_semaphore_create(1,1);

    if ((      pAudioSubTaskData->AudioSubAskQueue
               && pAudioSubTaskData->AudioSubAskQueue) == 0)
    {

        rkos_semaphore_delete(pAudioSubTaskData->AudioSubAskQueue );
        rkos_semaphore_delete(pAudioSubTaskData->AudioSubAskQueue);

        rkos_memory_free(pAudioSubTaskData);

        return RK_ERROR;
    }

    pAudioSubTaskData = rkos_memory_malloc(sizeof(AUDIOSUB_TASK_DATA_BLOCK));
    memset(pAudioSubTaskData, NULL, sizeof(AUDIOSUB_TASK_DATA_BLOCK));
    return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif
