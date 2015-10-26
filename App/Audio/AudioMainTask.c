/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioMainTask.c
* Owner: cjh
* Date: 2015.5.27
* Time: 10:50:46
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    cjh     2015.5.27     10:50:46   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __APP_AUDIO_AUDIOMAINTASK_C__

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
typedef  struct _AUDIOMAIN_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}AUDIOMAIN_RESP_QUEUE;
typedef  struct _AUDIOMAIN_ASK_QUEUE
{
    uint32 cmd;

}AUDIOMAIN_ASK_QUEUE;
typedef  struct _AUDIOMAIN_TASK_DATA_BLOCK
{
    xQueueHandle  AudioMainAskQueue;
    xQueueHandle  AudioMainRespQueue;

}AUDIOMAIN_TASK_DATA_BLOCK;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static AUDIOMAIN_TASK_DATA_BLOCK * gpstAudioMainData;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//#define AudioMix
#ifdef AudioMix
uint8 AudioMainoutptr[32][176] =
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
const uint32 AudioMainlength = 176 * 32 / 4;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t MainAudioWriteData(HDC dev, uint8 * pstr);
rk_err_t AudioMixTaskShell(HDC dev, uint8 * pstr);
rk_err_t AudioMainTask_Resume(void);
rk_err_t AudioMainTask_Suspend(void);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioMainTask_Enter
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:28
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_COMMON_
COMMON API void AudioMainTask_Enter(void)
{
    HDC hAudio;
    uint32 subno;
    //AUDIO_DEVICE_CLASS * pstAudioDev;

    SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//²âÊÔÄ¬ÈÏ12M

    hAudio = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((hAudio == NULL) || (hAudio == (HDC)RK_ERROR) || (hAudio == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Audio device open failure");
    }

    AudioDev_GetMainTrack(hAudio);
    subno=AudioDev_GetSubTrack(hAudio);
    AudioDev_SetTrackLen(hAudio, AudioMainlength);

    //AudioDev_SetMixScale(hRockCodec, subno, 0, 256);

    //pstAudioDev = (AUDIO_DEVICE_CLASS *)hRockCodec;
    //pstAudioDev->osAudioWriteSem = pstAudioDev->osAudioWriteSem;

    //hRockCodec = (HDC)pstAudioDev;

    while (1)
    {
#ifdef AudioMix
        MainAudioWriteData(hAudio,(uint8 *)AudioMainoutptr);
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
** Name: AudioMainTask_Resume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:28
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_COMMON_
COMMON FUN rk_err_t AudioMainTask_Resume(void)
{
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: AudioMainTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:28
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_COMMON_
COMMON FUN rk_err_t AudioMainTask_Suspend(void)
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
** Name: AudioMainTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:28
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_INIT_
INIT API rk_err_t AudioMainTask_DeInit(void *pvParameters)
{
    rkos_queue_delete(gpstAudioMainData->AudioMainAskQueue);
    rkos_queue_delete(gpstAudioMainData->AudioMainRespQueue);
    return RK_SUCCESS;
    //rkos_memory_free(pAudioMainTaskData);
}
/*******************************************************************************
** Name: AudioMainTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 10:51:28
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_INIT_
INIT API rk_err_t AudioMainTask_Init(void *pvParameters, void *arg)
{
    RK_TASK_CLASS*   pAudioMainTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_AUDIOMAIN_ARG * pArg = (RK_TASK_AUDIOMAIN_ARG *)arg;
    AUDIOMAIN_TASK_DATA_BLOCK*  pAudioMainTaskData;
    if (pAudioMainTask == NULL)
        return RK_PARA_ERR;

    pAudioMainTaskData->AudioMainAskQueue = rkos_semaphore_create(1,1);
    pAudioMainTaskData->AudioMainRespQueue = rkos_semaphore_create(1,1);

    if ((      pAudioMainTaskData->AudioMainAskQueue
               && pAudioMainTaskData->AudioMainAskQueue) == 0)
    {

        rkos_semaphore_delete(pAudioMainTaskData->AudioMainAskQueue );
        rkos_semaphore_delete(pAudioMainTaskData->AudioMainAskQueue);

        rkos_memory_free(pAudioMainTaskData);

        return RK_ERROR;
    }

    pAudioMainTaskData = rkos_memory_malloc(sizeof(AUDIOMAIN_TASK_DATA_BLOCK));
    memset(pAudioMainTaskData, NULL, sizeof(AUDIOMAIN_TASK_DATA_BLOCK));
    return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef _AUDIO_TASK_SHELL_
_APP_AUDIO_AUDIOMAINTASK_SHELL_
static uint8* ShellMediaName[] =
{
    //"pcb",
    //"mc",
    //"del",
    "test",
    //"help",
    "...",
    "\b",
};


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioTask_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 12:00:07
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_SHELL_
SHELL API rk_err_t AudioTask_Shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);
    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellMediaName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                       //remove '.',the point is the useful item

    ret = AudioMixTaskShell(dev, pItem);
    /*   switch (i)
       {
           case 0x00: //pcb
               ret = MediaTaskShellPcb(dev,pItem);
               break;

           case 0x01: //mc
               ret = MediaTaskShellMc(dev,pItem);
               break;

           case 0x02://del
               ret = MediaTaskShellDel(dev, pItem);
               break;

           case 0x03://test
               ret = MediaTaskShellTest(dev, pItem);
               break;

           case 0x04://help
               ret = MediaTaskShellHelp(dev, pItem);
               break;
           default:
               ret = RK_ERROR;
               break;
       }
    */
    return ret;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: MainAudioWriteData
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 11:27:37
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_SHELL_
SHELL FUN rk_err_t MainAudioWriteData(HDC dev, uint8 * buf)
{
    AudioDev_Write(dev,TRACK_NO_MAIN,buf);
    return RK_SUCCESS;
    //AudioDev_Write(dev,TRACK_NO_MAIN,AudioMainoutptr);
}
/*******************************************************************************
** Name: AudioMixTaskShell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.5.27
** Time: 11:13:53
*******************************************************************************/
_APP_AUDIO_AUDIOMAINTASK_SHELL_
SHELL FUN rk_err_t AudioMixTaskShell(HDC dev, uint8 * pstr)
{
    HDC hCodec;
    AUDIO_DEV_ARG stAudioArg;
    rk_err_t ret;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    HDC hAudio;
    uint32 subno,vol,time = 0;
    I2S_DEV_ARG stI2Sarg;
    // AUDIO_DEVICE_CLASS * pstAudioDev;

    SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//²âÊÔÄ¬ÈÏ12M
    //uint32 SetI2SFreq(UINT32 I2sId,Clock_Source_Sel Clk_Source,UINT32 TargetFreqHz)

    ret=RKDev_Create(DEV_CLASS_DMA, DMA_CHN1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "DMA RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }
    stI2Sarg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN1, NOT_CARE);
    stI2Sarg.i2s_SelectMod = I2S_SEL_ACODEC;

    ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH0, &stI2Sarg);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2S RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }

    stRockCodecDevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, 0, NOT_CARE);

    ret = RKDev_Create(DEV_CLASS_ROCKCODEC, 0,&stRockCodecDevArg);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "open failure",17,SYNC_MODE,NULL);
    }

    hCodec = RKDev_Open(DEV_CLASS_ROCKCODEC,0,NOT_CARE);

    //hCodec = RKDev_Open(DEV_CLASS_WM8987, 0, NOT_CARE);

    if ((hCodec == NULL) || (hCodec == (HDC)RK_ERROR) || (hCodec == (HDC)RK_PARA_ERR))
    {
        rk_print_string("codec device open failure");

        return RK_SUCCESS;
    }

    stAudioArg.Bit = 16;
    stAudioArg.Channel = 2;
    stAudioArg.DevID = 0;
    stAudioArg.hCodec = hCodec;
    stAudioArg.SampleRate = CodecFS_44100Hz;//CodecFS_16KHz;

    ret = RKDev_Create(DEV_CLASS_AUDIO, 0, &stAudioArg);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("audio device create failure");
    }

#if 0
    hAudio = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((hAudio == NULL) || (hAudio == (HDC)RK_ERROR) || (hAudio == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Audio device open failure");
    }

    AudioDev_GetMainTrack(hAudio);
    subno=AudioDev_GetSubTrack(hAudio);//MainTrack

    AudioDev_GetMainTrack(hAudio);
    subno=AudioDev_GetSubTrack(hAudio);

    AudioDev_SetTrackLen(hAudio, AudioMainlength);

    AudioDev_SetMixScale(hAudio, subno, 10, 10);


    while (1)
    {
        SubAudioWriteData(hAudio, subno, (uint8 *)AudioSuboutptr);

    }

#else
    RKTaskCreate(TASK_ID_AUDIOMAIN, 0, NULL);
    RKTaskCreate(TASK_ID_AUDIOSUB, 0, NULL);
#endif

    return RK_SUCCESS;
}



#endif
#endif
