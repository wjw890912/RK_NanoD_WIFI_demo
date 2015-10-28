 /*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: ..\Driver\Audio\AudioDevice.c
* Owner: Aaron.sun
* Date: 2014.5.7
* Time: 17:45:29
* Desc: Audio Device Class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.5.7     17:45:29   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_AUDIO_AUDIODEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "audio_globals.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "effect.h"
#include "sysinfosave.h"
#include "rockcodec.h"
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define AUDIO_DEV_NUM 1

typedef enum _TRACK_STATUS
{
    TRACK_UNUSED,
    TRACK_IDLE,
    TRACK_READIED,
    TRACK_NUM

}TRACK_STATUS;

typedef struct _AUDIO_TRACK
{
    uint8 * Track;
    uint32 TrackLen;
    uint32 Status;
    uint32 SampleRate;
    uint32 Bit;
    uint32 Channel;
    uint32 MixMain;
    uint32 MixSub;
    uint8 TrackProcess[AUDIO_PROCESS_MIX];

}AUDIO_TRACK;

typedef struct _TRACK_INFO
{
    unsigned long  TotalTime;
    unsigned long  CurrentTime;
    int            samplerate;
    int            bitrate;
    int            channels;
    int            total_length;
    int            bps;

}TRACK_INFO;

typedef struct _AUDIO_DEVICE_CLASS
{
    DEVICE_CLASS stAudioDevice;
    pSemaphore osAudioWriteSem;
    pSemaphore osAudioControlReqSem;
    HDC hCodec;

    uint32 TrackLen;
    uint32 SampleRate;
    uint32 Channel;
    uint32 Bit;
    uint8 * Track;
    uint32 Status;
    uint32 TotalCnt;
    uint32 SuspendCnt;
    uint8 TrackProcess[AUDIO_PROCESS_MIX + TRACK_NO_NUM - 1];
    //track_info * pAudioRegKey;
    //BOOL         IsEQNOR;//turn to NOR when user EQ's DB value turn to 0.

    AUDIO_TRACK stSubTrack[TRACK_NO_NUM - 1];

//    int32   CurrentCodec;
//    uint32  AudioCodecOpenErr;
//    uint32  AudioPtr;
//    uint32  AudioNeedDecode;
//    uint32  AudioLen;
    uint32  AudioErrorFrameNum;
    uint32  AudioEndFade;
    uint32  AudioFadeInStart;
    uint32  ABRequire;
    uint32  AudioABStart;
    uint32  AudioABEnd;
    uint32  PlayDirect;
    uint32  playVolume;
    uint32  VolumeCnt;

    TRACK_INFO  * pAudioRegKey;
    long        flag;
    RKEffect    UserEQ;
    int         IsEQUpdate;


}AUDIO_DEVICE_CLASS;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

static AUDIO_DEVICE_CLASS * gpstAudioDevISRHandler;
//static AUDIO_DEVICE_DATA_BLOCK * gpstAudioDeviceData;
static uint8 EqMode[8] = {EQ_HEAVY,EQ_POP,EQ_JAZZ,EQ_UNIQUE,EQ_USER,EQ_USER,EQ_NOR,EQ_BASS};


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

//#define _AudioTest_

#ifdef _AudioTest_

#ifdef CODEC_24BIT //24bit

#if 1 //16bit
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint8 Audiooutptr[1][16*(48*4+48*4)] =
{
    //0
    {
        #include "test01_1K0_48_16bit_16.data"
    },
};

_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint32 Audiolength = 16*(48*4);//176 * 32/4; 32bit

#else

_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint8 __align(4) Audiooutptr[1][16*(48*6+48*6/3)] =
{
    //0
    {
        #include "test01_1K0_48_24bit_16.data"
    },
};
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint32 Audiolength = 16*(48*6+48*6/3);
#endif

#else //16bit
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint8 Audiooutptr[1][16*(48*4)] =
{
    //0
    {
        #include "test01_1K0_48_16bit_16.data"
    },
};

_DRIVER_AUDIO_AUDIODEVICE_COMMON_
uint32 Audiolength = 16*(48*4);//176 * 32/4; 32bit

#endif

#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t AudioShellDelete(HDC dev,  uint8 * pstr);
rk_err_t AudioShellTest(HDC dev, uint8 * pstr);
rk_err_t AudioShellHelp(HDC dev, uint8 * pstr);
rk_err_t AudioShellPlay(HDC dev,  uint8 * pstr);
rk_err_t AudioShellCreate(HDC dev,  uint8 * pstr);
rk_err_t AudioShellOpen(HDC dev,  uint8 * pstr);
rk_err_t AudioShellPcb(HDC dev,  uint8 * pstr);
rk_err_t AudioDevDeInit(AUDIO_DEVICE_CLASS * pstAudioDev);
rk_err_t AudioDevInit(AUDIO_DEVICE_CLASS * pstAudioDev);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioDev_Get_Par
** Input:AUDIO_DEV_ARG *arg
** Return: rk_err_t
** Owner:cjh
** Date: 2014.5.13
** Time: 17:49:25
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_Get_Par(AUDIO_DEV_ARG *arg)
{
    AUDIO_DEV_ARG *stParg = (AUDIO_DEV_ARG *)arg;
    stParg->EQMode = gpstAudioDevISRHandler->UserEQ.Mode;
    stParg->Vol = gpstAudioDevISRHandler->playVolume;
    stParg->EQMode = gpstAudioDevISRHandler->UserEQ.Mode;
    stParg->Bit = gpstAudioDevISRHandler->Bit;
    stParg->Channel = gpstAudioDevISRHandler->Channel;
    stParg->SampleRate = gpstAudioDevISRHandler->SampleRate;

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioDev_SetTrackLen
** Input:HDC dev, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 17:49:25
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_I2S_Acodec_Mode(HDC dev, void *arg)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    AUDIO_DEV_I2S_CODEC_CONFIG_ARG *stAudioDevArg = (AUDIO_DEV_I2S_CODEC_CONFIG_ARG *)arg;

    ACodec_Set_I2S_Mode(TFS_TX_I2S_MODE,stAudioDevArg->stI2sDevArg.Data_width,IBM_TX_BUS_MODE_NORMAL,I2S_MST_MASTER);
    RockcodecDevConfigI2S(pstAudioDev->hCodec,&(stAudioDevArg->stI2sDevArg));


    //set codec samplerate
    //RockcodecDev_SetRate(pstAudioDev->hCodec, stAudioDevArg->stCodecArg.SampleRate);//codec SampleRate
    //RockcodecDev_SetMode(pstAudioDev->hCodec, stAudioDevArg->stCodecArg.Codecmode, stAudioDevArg->stCodecArg.SampleRate);//CodecMode_en_t

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: Bit_Convertor_shift
** Input:short *ppsBuffer, long *plLength,int bps
** Return: void
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 13:47:42
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API void Bit_Convertor_shift(short *ppsBuffer, uint32 Length, int bps)
{
    int i = 0;
    int offset = 0 ;
#ifdef CODEC_24BIT
    if (bps == 16)
    {
        long *pOut =(long *) ppsBuffer;
        offset = Length*2;
        for (i=offset-1; i >= 0 ; i--)
        {
            pOut[i] = pOut[i] << 8;
        }
    }
    else if ((bps == 24)||(bps == 32))
    {
        long *pOut =(long *) ppsBuffer;
        offset = Length*2;
        for (i=offset-1; i >= 0 ; i--)
        {
            pOut[i] = pOut[i] << 8;
        }
    }
    //*plLength = *plLength*2;
#endif

}

/*******************************************************************************
** Name: Bit_Convertor_DEC
** Input:short *ppsBuffer, long *plLength,int bps
** Return: void
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 13:42:16
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API void Bit_Convertor_DEC(short *ppsBuffer, uint32 Length, int bps)
{
    int i = 0;
    int offset = 0 ;
    //printf("in Bit_Convertor_DEC\n");
#ifdef CODEC_24BIT
    if (bps == 16)
    {
        short *pOut = ppsBuffer;
        long *pOut32 =(long *) ppsBuffer;
        offset = Length*2;
        for (i=offset-1; i >= 0 ; i--)
        {
            pOut32[i] = (pOut[i]<<16)>>8;
        }
    }
    else if (bps == 24)
    {
        char *pOut =(char *) ppsBuffer;
        long *pOut32 =(long *) ppsBuffer;
        offset = (Length)*2;
        for (i=offset; i > 0 ; i--)
        {
            pOut32[i-1]  =( pOut[3*i-1]<<24 | pOut[3*i-2]<<16| pOut[3*i-3]<<8)>>8;
        }
        //printf("bps == 24!!!! \n");

    }
    else if (bps == 32)
    {
        long *pOut =(long *) ppsBuffer;
        offset = Length*2;
        for (i=offset-1; i >= 0 ; i--)
        {
            pOut[i] = pOut[i] >> 8;
        }
    }
#else
    if (bps == 16)
    {
    }
    else if (bps == 24)
    {
        char *pOut = (char *)ppsBuffer;
        offset = Length*2;
        for (i=0; i < offset ; i++)
        {
            pOut[2*i]= pOut[3*i+1];
            pOut[2*i+1]= pOut[3*i+2];
        }
    }
    else if (bps == 32)
    {
        char *pOut = (char *)ppsBuffer;
        offset = Length*2;
        for (i=0; i < offset ; i--)
        {
            pOut[2*i]= pOut[4*i+2];
            pOut[2*i+1]= pOut[4*i+3];
        }
    }
#endif
}

/*******************************************************************************
** Name: AudioDev_SetVol
** Input:HDC dev,  uint32 vol
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
rk_err_t AudioDev_SetVol(HDC dev, uint32 vol)
{
    AUDIO_DEVICE_CLASS *  hAudio = (AUDIO_DEVICE_CLASS * )dev;
    if(hAudio == NULL)
    {
        printf("\n AudioDev_SetVol HDC dev == NULL \n");
        return RK_ERROR;
    }
    printf("\n AudioDev vol=%d \n", vol);
    hAudio->playVolume = vol;
    RockcodecDev_SetVol(hAudio->hCodec, hAudio->UserEQ.Mode, vol);
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioDev_RealseMainTrack
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.14
** Time: 10:34:20
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_RealseMainTrack(HDC dev)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    pstAudioDev->Status = TRACK_UNUSED;
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioDev_SetTrackLen
** Input:HDC dev, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 17:49:25
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetTrackLen(HDC dev, uint32 len)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    //printf("@@@@@@@@@audiodev sour 16bit len=%d\n",pstAudioDev->TrackLen);
    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }
    pstAudioDev->TrackLen = len;
    //printf("audiodev sour 16bit len=%d\n",pstAudioDev->TrackLen);

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: AudioDev_SetSubTrackLen
** Input:HDC dev, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 17:49:25
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetSubTrackLen(HDC dev,uint32 TrackNo, uint32 len)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    pstAudioDev->stSubTrack[TrackNo].TrackLen = len;

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: AudioDev_SetTrackProcess
** Input:HDC dev, uint8 mainTrackProcess, uint8 subTrackProcess
** Return: rk_err_t
** Owner:Aron.chen
** Date: 2015.5.29
** Time: 09:49:25
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetTrackProcess(HDC dev, uint32 TrackNo, uint8 subTrackProcess)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    pstAudioDev->stSubTrack[TrackNo].TrackProcess[AUDIO_PROCESS_MIX-1] = subTrackProcess;

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: AudioDev_GetSubTrack
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 15:56:10
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_GetSubTrack(HDC dev)
{

    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    uint32 i;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    //pstAudioDev->stSubTrack[0].Status = TRACK_IDLE;
    //pstAudioDev->stSubTrack[0].TrackProcess[AUDIO_PROCESS_MIX] = AUDIO_PROCESS_MIX;
    for (i = 0; i < TRACK_NO_NUM; i++)
    {
        if (pstAudioDev->stSubTrack[i].Status == TRACK_UNUSED)
        {
            pstAudioDev->stSubTrack[i].Status = TRACK_IDLE;

            return i;
        }
    }

    return RK_ERROR;

}

/*******************************************************************************
** Name: AudioDev_GetMainTrack
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 15:55:01
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_GetMainTrack(HDC dev)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    printf("!! enter AudioDev_GetMainTrack \n");
    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if (pstAudioDev->Status == TRACK_UNUSED)
    {
        pstAudioDev->Status = TRACK_IDLE;

        return RK_SUCCESS;
    }
//    if (pstAudioDev->stSubTrack[0].Status == TRACK_UNUSED)
//    {
//        pstAudioDev->stSubTrack[0].Status = TRACK_IDLE;
//        return RK_SUCCESS;
//    }
    printf("!! AudioDev_GetMainTrack ERROR\n");
    return RK_ERROR;
}

/*******************************************************************************
** Name: AudioDev_SetMixScale
** Input:HDC dev ,uint32 TrackNo, uint32 MixMain, uint32 MixSub
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:52:05
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetMixScale(HDC dev ,uint32 TrackNo, uint32 MixMain, uint32 MixSub)
{
    uint64 temp;
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if ((TrackNo == 0) || (TrackNo >= TRACK_NO_NUM))
    {
        return RK_ERROR;
    }
    temp = (uint64 )(MixMain & 0xffff) + (uint64 )(MixSub & 0xffff);

    if ((temp > 0xffff) || (temp < 1))
    {
        return RK_ERROR;
    }

    MixMain =(uint32 )(((MixMain << 8) + (temp/2)) / temp);
    MixSub = (uint32 )(((MixSub << 8) + (temp/2)) / temp);

    pstAudioDev->stSubTrack[TrackNo].MixMain = MixMain;// - 1?
    pstAudioDev->stSubTrack[TrackNo].MixSub = MixSub;// - 1?

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: AudioDev_SetBit
** Input:HDC dev, uint32 Bit
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:47:28
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetBit(HDC dev, uint32 TrackNo, uint32 Bit)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    AUDIO_DEV_I2S_CODEC_CONFIG_ARG stAudioDevArg;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if (TrackNo >= TRACK_NO_NUM)
    {
        return RK_ERROR;
    }

    if (TrackNo > 0)
    {
        pstAudioDev->stSubTrack[TrackNo - 1].Bit = Bit;
    }
    else
    {
        pstAudioDev->Bit = Bit;
    }

#if 0
    stAudioDevArg.stI2sDevArg.i2smode = I2S_SLAVE_MODE;//;I2S_SLAVE_MODE
    stAudioDevArg.stI2sDevArg.i2sCS = I2S_IN; //I2S_EXT
    stAudioDevArg.stI2sDevArg.I2S_FS = pstAudioDev->SampleRate;
    stAudioDevArg.stI2sDevArg.BUS_FORMAT = I2S_FORMAT;//I2S_FORMAT;

    stAudioDevArg.stI2sDevArg.Data_width = Bit;

    stAudioDevArg.stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    stAudioDevArg.stCodecArg.SampleRate = pstAudioDev->SampleRate;
    stAudioDevArg.stCodecArg.Codecmode = Codec_DACoutHP;
    AudioDev_I2S_Acodec_Mode(pstAudioDev, &stAudioDevArg);
#endif

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioDev_SetChannel
** Input:HDC dev, uint32 bit
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:46:10
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetChannel(HDC dev, uint32 TrackNo, uint32 Channel)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;
    AUDIO_DEV_I2S_CODEC_CONFIG_ARG stAudioDevArg;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if (TrackNo >= TRACK_NO_NUM)
    {
        return RK_ERROR;
    }

    if (TrackNo > 0)
    {
        pstAudioDev->stSubTrack[TrackNo - 1].Channel = Channel;
    }
    else
    {
        pstAudioDev->Channel = Channel;
        printf("channel LR=%d\n",pstAudioDev->Channel);
    }

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioDev_SetSampleRate
** Input:HDC dev, uint32 SamleRate
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:43:09
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetSampleRate(HDC dev, uint32 TrackNo, uint32 SamleRate)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if (TrackNo >= TRACK_NO_NUM)
    {
        return RK_ERROR;
    }

    if (TrackNo > 0)
    {
        pstAudioDev->stSubTrack[TrackNo - 1].SampleRate = SamleRate;
    }
    else
    {
        pstAudioDev->SampleRate = SamleRate;
    }

    RockcodecDev_SetRate(pstAudioDev->hCodec, pstAudioDev->SampleRate);//codec SampleRate
    RockcodecDev_SetMode(pstAudioDev->hCodec, Codec_DACoutHP, pstAudioDev->SampleRate);//CodecMode_en_t

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: AudioDev_SetModel
** Input:HDC dev,  uint32 TrackNo, uint32 ProcessNo, uint32 ProcessPos
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:38:19
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_SetModel(HDC dev, uint32 TrackNo, uint32 ProcessNo, uint32 ProcessPos)
{
    AUDIO_DEVICE_CLASS * pstAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    if (pstAudioDev == NULL)
    {
        return RK_PARA_ERR;
    }

    if (TrackNo >= TRACK_NO_NUM)
    {
        return RK_ERROR;
    }

    if (TrackNo > 0)
    {
        pstAudioDev->stSubTrack[TrackNo - 1].TrackProcess[ProcessNo] = ProcessPos;
    }
    else
    {
        pstAudioDev->TrackProcess[ProcessNo] = ProcessPos;
    }

    return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*******************************************************************************
** Name: AudioSetVolume
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:46:40
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON FUN void AudioDev_SetVolume(void)
{
    gpstAudioDevISRHandler->playVolume = gSysConfig.OutputVolume;
}

/*******************************************************************************
** Name: AudioDev_SetEQ
** Input:HDC dev, uint32 userEQMod, uint32 vol
** Return: void
** Owner:cjh
** Date: 2015.1.1
** Time: 9:53:09
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API void AudioDev_SetEQ(HDC dev, uint32 userEQMod, int isEQUpdate)
{
    AUDIO_EQ_ARG eqARG;
    AUDIO_DEVICE_CLASS *  hAudio = (AUDIO_DEVICE_CLASS * )dev;
    RKEffect *pEffect;
    if(hAudio == NULL)
    {
        printf("ERROR:##hAudio NULL\n");
    }
    pEffect = (RKEffect *)(&(hAudio->UserEQ));

    hAudio->UserEQ.Mode = userEQMod;

    //hAudio->AudioLen = audioLen ;
    //hAudio->playVolume = vol;
    hAudio->IsEQUpdate = isEQUpdate;
    //printf("#########set eqARG\n");

    eqARG.audioLen = &hAudio->TrackLen;
    eqARG.playVolume = &hAudio->playVolume;
    eqARG.isEQUpdate = &hAudio->IsEQUpdate;
    //printf("#########enter AudioDev_SetEQ\n");

 //   RockcodecDev_SetEQ(hAudio->hCodec, (void *)(&(hAudio->UserEQ)), (void *)eqARG);
#ifdef  _RK_EQ_
    if (pEffect->Mode == EQ_BASS)
    {
        if (*(eqARG.playVolume) <= 27 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 12;
            else
                pEffect->max_DbGain = 12;
        }
        else if (*(eqARG.playVolume) == 28 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 10;
            else
                pEffect->max_DbGain = 10;
        }
        else if (*(eqARG.playVolume) == 29 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 8;
            else
                pEffect->max_DbGain = 8;
        }
        else if (*(eqARG.playVolume) == 30 )
        {
            //printf("#########AudioPlayInfo.playVolume = %d\n",(*(eqARG->playVolume)));
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 6;
            else
                pEffect->max_DbGain = 6;
        }

    }
    printf("max_DbGain = %d userEQMod=%d playVolume=%d\n",pEffect->max_DbGain,userEQMod, *(eqARG.playVolume));
    EffectAdjust(pEffect, &eqARG ,hAudio->SampleRate);
    //printf("EQ set EffectAdjust over\n");
    AudioDev_SetVolume();
    //printf("EQ set volume over\n");
#endif
}


/*******************************************************************************
** Name: AudioDev_Write
** Input:HDC dev , uint32 TrackNo, uint8 * buf, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.9
** Time: 13:54:20
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
COMMON API rk_err_t AudioDev_Write(HDC dev , uint32 TrackNo, uint8 * buf)
{
    AUDIO_DEVICE_CLASS * pstWriteAudioDev = (AUDIO_DEVICE_CLASS *)dev;

    uint32 i, j, mixLen;
    uint16 sacleMain, scaleSub;
    uint64 temp;
    uint8 *pMainTrack ,*pSubTrack;
    uint32 *p32MainTrack;

    if (pstWriteAudioDev == NULL)
    {
        printf("pstWriteAudioDev == NULL\n");
        return RK_PARA_ERR;
    }

    if (TrackNo == TRACK_NO_MAIN)
    {
        if (pstWriteAudioDev->Status == TRACK_UNUSED)
        {
            return RK_ERROR;
        }
    }
    else if ((TrackNo >= 0) && (TrackNo < TRACK_NO_NUM))
    {
        if (pstWriteAudioDev->stSubTrack[TrackNo].Status == TRACK_UNUSED)// - 1?
        {
            return RK_ERROR;
        }
    }
    else
    {
        return RK_PARA_ERR;
    }
    if (TrackNo == TRACK_NO_MAIN)
    {
        //call hCode and suspend
        pstWriteAudioDev->Track = buf;
#if 0
        pMainTrack = pstWriteAudioDev->Track;
        for (i = pstWriteAudioDev->TrackLen/3*2; i < pstWriteAudioDev->TrackLen; i++)//pstWriteAudioDev->TrackLen
        {
            rk_printf("##send for printf Track NO.%d = 0x%x \n", i, *pMainTrack);
            pMainTrack++;
        }

#endif

#if 0
        pMainTrack = pstWriteAudioDev->Track;
        for (i = 0; i < 500; i++)//pstWriteAudioDev->TrackLen
        {
            rk_printf("!#!#send for printf Track NO.%d = 0x%x \n", i, *pMainTrack);
            pMainTrack++;
        }

#endif

#ifndef _AudioTest_
        //AudioDev_SetEQ(pstWriteAudioDev, EQ_HEAVY, 1); //SET EQ befor set vol
        //AudioDev_SetVol(pstWriteAudioDev, 25);
        //printf("16bit or 24bit Convertor\n");
        if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
        {
            //printf("\nWIDTH24 Convertor_DEC len=%d len/6=%d\n",pstWriteAudioDev->TrackLen,pstWriteAudioDev->TrackLen/(3*2));
            Bit_Convertor_DEC((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/6, 24);//pstWriteAudioDev->Channel
        }
        else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
        {
            Bit_Convertor_DEC((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2), 16);//pstWriteAudioDev->Channel
        }
        else
        {
            //printf("ERROR:sour music no 16bit or 24bit \n");
        }
        //printf("~~~len= %d \n",gpstAudioDevISRHandler->TrackLen);
#ifdef _RK_EQ_
        if(EQ_NOR != pstWriteAudioDev->UserEQ.Mode)
        {
            /*audio effect process.*/
            #ifdef CODEC_24BIT
            //if(gpstAudioDevISRHandler->SampleRate <= 96000)
            if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
            {
                EffectProcess((long*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(3*2),\
                              &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
            }
            else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
            {
                //printf("16BIT TrackLen=%d\n",gpstAudioDevISRHandler->TrackLen);
                EffectProcess((long*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2),\
                              &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
            }

            #else
                EffectProcess((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2),\
                              &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
            #endif
        }
#endif //end #ifdef _RK_EQ_

        if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
        {
            //printf("\nWIDTH24 Convertor_shift\n");
            Bit_Convertor_shift((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(3*2), 24);
        }
        else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
        {
            //printf("@@@@convertor 16bit len=%d\n",pstWriteAudioDev->TrackLen/(2*2));
            Bit_Convertor_shift((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2), 16);
        }
        else
        {
            printf("ERROR2:sour music no 16bit or 24bit \n");
        }
        //AudioDev_SetTrackLen(pstWriteAudioDev, pstWriteAudioDev->TrackLen*4);
        //printf("@@@@3sour 16bit len=%d\n",pstWriteAudioDev->TrackLen);
#endif //end #ifndef AudioTest

#if 0
        p32MainTrack = (uint32 *)pstWriteAudioDev->Track;
        for (i = 16300/4 ; i < pstWriteAudioDev->TrackLen/4; i++)
        {
            rk_printf("******send for printf Track NO.%d = 0x%x \n", i, *p32MainTrack);
            p32MainTrack++;
        }

#endif

        for (j=1; j<TRACK_NO_NUM; j++)
        {
            if ((pstWriteAudioDev->stSubTrack[j].Status == TRACK_READIED) && (pstWriteAudioDev->Bit == pstWriteAudioDev->stSubTrack[j].Bit))
            {

                // first / or *
                sacleMain = pstWriteAudioDev->stSubTrack[j].MixMain  ;
                scaleSub = pstWriteAudioDev->stSubTrack[j].MixSub  ;

                // To prevent the Sub pointer overflow
                mixLen = (pstWriteAudioDev->TrackLen < pstWriteAudioDev->stSubTrack[j].TrackLen)\
                         ? pstWriteAudioDev->TrackLen : pstWriteAudioDev->stSubTrack[j].TrackLen;

                // ++
                pMainTrack = pstWriteAudioDev->Track;
                pSubTrack = pstWriteAudioDev->stSubTrack[j].Track;
                for (i = 0; i < mixLen; i++)
                {
#if 0
                    rk_printf("**************\n");
                    rk_printf("++++++pMainTrack = 0x%x \n",*pMainTrack);
                    rk_printf("++++++pSubTrack = 0x%x \n",*pSubTrack);
                    rk_printf("==sumMainTcak = 0x%x \n",(((*pMainTrack) * sacleMain)>>8));
                    rk_printf("==sumSubTcak = 0x%x \n",(((*pSubTrack) * scaleSub) >> 8));
                    rk_printf("**************\n");
#endif
                    *pMainTrack =(uint8)((((*pMainTrack) * sacleMain)>>8) + (((*pSubTrack) * scaleSub >> 8)));
                    //rk_printf("~~~Sum Track NO.%d = 0x%x \n", i, *pMainTrack);

                    i++;
                    if (i >= pstWriteAudioDev->TrackLen)
                        break;
                    pMainTrack++;
                    pSubTrack++;
                    *pMainTrack =(uint8)((((*pMainTrack) * sacleMain)>>8) + (((*pSubTrack) * scaleSub >> 8)));
                    if (i >= pstWriteAudioDev->TrackLen)
                        break;
                    pMainTrack++;
                    pSubTrack++;

                }

                pstWriteAudioDev->stSubTrack[j].Status = TRACK_IDLE; //TRACK_UNUSED for next select
            }

        }

#if 0
        pMainTrack = pstWriteAudioDev->Track;
        for (i = 0; i < pstWriteAudioDev->TrackLen; i++)
        {
            rk_printf("~~~send for printf Track NO.%d = 0x%x \n", i, *pMainTrack);
            pMainTrack++;
        }

#endif
#ifdef CODEC_24BIT
        if (gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH24+1))
        {
            //printf("24bit_Write len=%d\n",pstWriteAudioDev->TrackLen*4/3);
            RockcodecDev_Write(pstWriteAudioDev->hCodec, (uint8 *)pstWriteAudioDev->Track, (pstWriteAudioDev->TrackLen*4/3), ASYNC_MODE);
        }
        else if(gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH16+1))
        {
            //printf("16bit_Write len=%d\n",pstWriteAudioDev->TrackLen*2);
            RockcodecDev_Write(pstWriteAudioDev->hCodec, (uint8 *)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen*2, ASYNC_MODE);
        }
        else
        {
            printf("ERROR2:sour music no 16bit or 24bit \n");
        }
#else
        RockcodecDev_Write(pstWriteAudioDev->hCodec, (uint8 *)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen, ASYNC_MODE);
#endif
        //printf(" RockcodecDev_Write OK\n");

        //WM8987Dev_Write(pstAudioDev->hCodec, pstAudioDev->Track, pstAudioDev->TrackLen, ASYNC_MODE);
        for (i = 0; i < pstWriteAudioDev->SuspendCnt;)
        {
            rkos_semaphore_give(pstWriteAudioDev->osAudioWriteSem);
            pstWriteAudioDev->SuspendCnt--;
        }
    }
    else
    {

        pstWriteAudioDev->stSubTrack[TrackNo].Status = TRACK_READIED;// - 1?

        for (i = 0; i < AUDIO_PROCESS_MIX; i++)
        {

            switch (pstWriteAudioDev->stSubTrack[TrackNo].TrackProcess[i])// -1?
            {
                case AUDIO_PROCESS_CLOSE:
                    break;

                case AUDIO_PROCESS_EQ:
                    break;

                case AUDIO_PROCESS_FILTER:
                    break;

                case AUDIO_PROCESS_FADE_IN:
                    break;

                case AUDIO_PROCESS_FADE_OUT:
                    break;

                case AUDIO_PROCESS_CHANGE_SPEED:
                    break;

                case AUDIO_PROCESS_CHANGE_TONE:
                    break;

                case AUDIO_PROCESS_ECHO_CANCEL:
                    break;

                case AUDIO_PROCESS_MIX:
                    goto sub_over;

            }

        }

sub_over:

        rkos_enter_critical();

        pstWriteAudioDev->SuspendCnt++;

        if (pstWriteAudioDev->SuspendCnt >= pstWriteAudioDev->TotalCnt)
        {
            //mix sound
            for (i = 0 ; i < (AUDIO_PROCESS_MIX + TRACK_NO_NUM - 1); i++)
            {
                switch (pstWriteAudioDev->stSubTrack[TrackNo].TrackProcess[i])// -1?
                {
                    case AUDIO_PROCESS_CLOSE:
                        break;

                    case AUDIO_PROCESS_EQ:
                        break;

                    case AUDIO_PROCESS_FILTER:
                        break;

                    case AUDIO_PROCESS_FADE_IN:
                        break;

                    case AUDIO_PROCESS_FADE_OUT:
                        break;

                    case AUDIO_PROCESS_CHANGE_SPEED:
                        break;

                    case AUDIO_PROCESS_CHANGE_TONE:
                        break;

                    case AUDIO_PROCESS_ECHO_CANCEL:
                        break;

                    case AUDIO_PROCESS_MIX:

                        pstWriteAudioDev->stSubTrack[TrackNo].Track = buf;
#if 0//ndef _AudioTest_

                    //printf("16bit or 24bit Convertor\n");
                    if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
                    {
                        //AudioDev_SetTrackLen(pstWriteAudioDev, pstWriteAudioDev->TrackLen/(3*2));//(3*2)
                        printf("sour 24bit len=%d\n",pstWriteAudioDev->TrackLen);
                        Bit_Convertor_DEC((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(3*2), 24);
                    }
                    else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
                    {
                        printf("@@@@1sour 16bit len=%d\n",pstWriteAudioDev->TrackLen);
                        //AudioDev_SetTrackLen(pstWriteAudioDev, pstWriteAudioDev->TrackLen/(2*2));//(2*2)
                        //printf("@@@@2sour 16bit len=%d\n",pstWriteAudioDev->TrackLen);
                        Bit_Convertor_DEC((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2), 16);
                        //printf("Convertor over\n");
                    }
                    else
                    {
                        printf("ERROR:sour music no 16bit or 24bit \n");
                    }
                    //printf("~~~len= %d \n",gpstAudioDevISRHandler->TrackLen);
#ifdef _RK_EQ_
                    if(EQ_NOR != pstWriteAudioDev->UserEQ.Mode)
                    {
                        /*audio effect process.*/
                        #ifdef CODEC_24BIT
                        //if(gpstAudioDevISRHandler->SampleRate <= 96000)
                        printf("16BIT TrackLen=%d\n",gpstAudioDevISRHandler->TrackLen);
                        if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
                        {
                            EffectProcess((long*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(3*2),\
                                          &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
                        }
                        else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
                        {
                            EffectProcess((long*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2),\
                                          &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
                        }

                        #else
                            EffectProcess((long*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2),\
                                          &(pstWriteAudioDev->UserEQ), &(pstWriteAudioDev->IsEQUpdate));
                        #endif
                    }
#endif //end #ifdef _RK_EQ_

                    if (pstWriteAudioDev->Bit == (I2S_DATA_WIDTH24+1))
                    {
                        Bit_Convertor_shift((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(3*2), 24);
                    }
                    else if(pstWriteAudioDev->Bit == (I2S_DATA_WIDTH16+1))
                    {
                        printf("@@@@Convertor_shift len=%d\n",pstWriteAudioDev->TrackLen);
                        Bit_Convertor_shift((short*)pstWriteAudioDev->Track, pstWriteAudioDev->TrackLen/(2*2), 16);
                    }
                    else
                    {
                        printf("ERROR2:sour music no 16bit or 24bit \n");
                    }

                    //AudioDev_SetTrackLen(pstWriteAudioDev, pstWriteAudioDev->TrackLen*4);
                    //printf("@@@@3sour 16bit len=%d\n",pstWriteAudioDev->TrackLen);
#endif //end #ifndef AudioTest

                        //pstWriteAudioDev->stSubTrack[TrackNo].Status = TRACK_IDLE;//TRACK_READIED;

                        break;

                }

            }

        }

        rkos_exit_critical();

        //suspend
        rkos_semaphore_take(pstWriteAudioDev->osAudioWriteSem, MAX_DELAY);

    }


}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioTxCompleteIsr
** Input:HDC dev, uint32 * buf
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioDev_Create
** Input:void * Args
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_INIT_
INIT API HDC AudioDev_Create(uint32 DevID, void *arg)
{
    AUDIO_DEV_ARG * pstAudioArg = (AUDIO_DEV_ARG * )arg;
    DEVICE_CLASS * pstDev;
    AUDIO_DEVICE_CLASS * pstAudioDev;

    if (pstAudioArg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstAudioDev =  rkos_memory_malloc(sizeof(AUDIO_DEVICE_CLASS));
    if (pstAudioDev == NULL)
    {
        return pstAudioDev;
    }

    memset(pstAudioDev, 0, sizeof(AUDIO_DEVICE_CLASS));

    pstAudioDev->osAudioWriteSem = rkos_semaphore_create(1,0);
    pstAudioDev->osAudioControlReqSem = rkos_semaphore_create(1,1);

    if ((      pstAudioDev->osAudioWriteSem
               && pstAudioDev->osAudioControlReqSem) == 0)
    {
        rkos_semaphore_delete(pstAudioDev->osAudioWriteSem);
        rkos_semaphore_delete(pstAudioDev->osAudioControlReqSem);

        rkos_memory_free(pstAudioDev);
        return (HDC) RK_ERROR;
    }


    pstDev = (DEVICE_CLASS *)pstAudioDev;

    pstDev->suspend = NULL;
    pstDev->resume  = NULL;

    pstAudioDev->hCodec = pstAudioArg->hCodec;
    pstAudioDev->SampleRate = pstAudioArg->SampleRate;
    pstAudioDev->Channel = pstAudioArg->Channel;
    pstAudioDev->Bit = pstAudioArg->Bit;
    pstAudioDev->playVolume = pstAudioArg->Vol;
#ifdef _RK_EQ_
    pstAudioDev->UserEQ.Mode = pstAudioArg->EQMode;
#endif

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_AUDIO_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    if (AudioDevInit(pstAudioDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstAudioDev->osAudioWriteSem);
        rkos_semaphore_delete(pstAudioDev->osAudioControlReqSem);

        rkos_memory_free(pstAudioDev);
        return (HDC) RK_ERROR;
    }
    gpstAudioDevISRHandler = pstAudioDev;

    return (HDC)pstDev;
}
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioDev_Delete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_INIT_
INIT API rk_err_t AudioDev_Delete(uint32 DevID, void * arg)
{
    AUDIO_DEV_ARG *stParg = (AUDIO_DEV_ARG *)arg;
#if 1
    stParg->hCodec = gpstAudioDevISRHandler->hCodec;
    AudioDevDeInit(gpstAudioDevISRHandler);

    //Free AudioDev memory...
    rkos_semaphore_delete(gpstAudioDevISRHandler->osAudioWriteSem);
    rkos_semaphore_delete(gpstAudioDevISRHandler->osAudioControlReqSem);

    rkos_memory_free(gpstAudioDevISRHandler);
    gpstAudioDevISRHandler = NULL;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_AUDIO_DEV);
#endif

#ifdef _RK_EQ_
    Effect_Delete();
#endif

#endif

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: AudioDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_INIT_
INIT FUN rk_err_t AudioDevInit(AUDIO_DEVICE_CLASS * pstAudioDev)
{
    //create audio driver model
    uint32 i;
    //I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    AUDIO_DEV_I2S_CODEC_CONFIG_ARG stAudioDevArg;
    //WM8987_DEVICE_CONFIG_REQ_ARG stWM8987DevArg;

    //stWM8987DevArg.Codecmode = Codec_DACoutHP;
    //stWM8987DevArg.CodecFS = pstAudioDev->SampleRate;

    for (i = 0; i < TRACK_NUM - 1; i++)
    {
        pstAudioDev->stSubTrack[i].TrackProcess[0] = AUDIO_PROCESS_MIX_0;
    }
    pstAudioDev->UserEQ.max_DbGain = 12;

#if 1
    stAudioDevArg.stI2sDevArg.i2smode = I2S_SLAVE_MODE;//;I2S_SLAVE_MODE
    stAudioDevArg.stI2sDevArg.i2sCS = I2S_IN; //I2S_EXT
    stAudioDevArg.stI2sDevArg.I2S_FS = I2S_FS_44100Hz;
    stAudioDevArg.stI2sDevArg.BUS_FORMAT = I2S_FORMAT;//I2S_FORMAT;
#ifdef CODEC_24BIT //24bit
    stAudioDevArg.stI2sDevArg.Data_width = I2S_DATA_WIDTH24;
#else
    stAudioDevArg.stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
#endif
    stAudioDevArg.stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    stAudioDevArg.stCodecArg.SampleRate = pstAudioDev->SampleRate;
    stAudioDevArg.stCodecArg.Codecmode = Codec_DACoutHP;
    AudioDev_I2S_Acodec_Mode(pstAudioDev, &stAudioDevArg);
    RockcodecDev_SetRate(pstAudioDev->hCodec, pstAudioDev->SampleRate);
    RockcodecDev_SetMode(pstAudioDev->hCodec, Codec_DACoutHP, pstAudioDev->SampleRate);//CodecMode_en_t
    printf("\n AudioDevInit set par. ok\n");
#else
    //set codec samplerate
    RockcodecDev_SetRate(pstAudioDev->hCodec, pstAudioDev->SampleRate);
    RockcodecDev_SetMode(pstAudioDev->hCodec, Codec_DACoutHP, pstAudioDev->SampleRate);//CodecMode_en_t
#endif

#ifdef _RK_EQ_
    EffectInit();
    EQ_ClearBuff();

    printf("SetEQ EQ_POP\n");
    AudioDev_SetEQ(pstAudioDev, EQ_HEAVY, 1); //SET EQ befor set vol
#endif
    printf("AudioDevInit set vol\n");
    AudioDev_SetVol(pstAudioDev, pstAudioDev->playVolume);

    //RockcodecDev_SetVol(hAudio->hCodec, hAudio->UserEQ.Mode, 30);
    //WM8987Dev_Control(pstAudioDev->hCodec, WM8987_DEVICE_CONFIG_I2S_CMD, &stI2sDevArg);

    //WM8987Dev_Control(pstAudioDev->hCodec, WM8987_DEVICE_SET_MODE_CMD, &stWM8987DevArg);
    //WM8987Dev_Control(pstAudioDev->hCodec, WM8987_DEVICE_SET_SAMPLERATE_CMD, &stWM8987DevArg);
    //WM8987Dev_Control(pstAudioDev->hCodec, WM8987_DEVICE_SET_VOLUME_CMD, NULL);
#ifdef _RK_EQ_
    printf("AudioDevInitFREQ_EnterModule\n");
    FREQ_EnterModule(FREQ_AUDIO_INIT);
    //EffectInit();
#endif
    return RK_SUCCESS;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: AudioDevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_INIT_
INIT FUN rk_err_t AudioDevDeInit(AUDIO_DEVICE_CLASS * pstAudioDev)
{
    return RK_SUCCESS;
}


#ifdef _AUDIO_SHELL_
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
static uint8* ShellAudioName[] =
{
    "pcb",
    "create",
    "delete",
    "open",
    "play",
    "help",
    "test",
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
** Name: AudioDev_Shell
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL API rk_err_t AudioDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellAudioName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                   //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:
            ret = AudioShellPcb(dev, pItem);
            break;

        case 0x01:
            ret = AudioShellCreate(dev, pItem);
            break;

        case 0x02:
            ret = AudioShellDelete(dev, pItem);
            break;

        case 0x03:
            ret = AudioShellOpen(dev, pItem);

        case 0x04:
            ret = AudioShellPlay(dev, pItem);
            break;

        case 0x05:
            ret = AudioShellHelp(dev, pItem);
            break;

        case 0x06:
            ret = AudioShellTest(dev, pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
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
** Name: AudioShellDelete
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.18
** Time: 11:48:55
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellDelete(HDC dev,  uint8 * pstr)
{
    uint32 DevID;
    AUDIO_DEV_ARG stParg;
    //Get audioDevice ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    if (RKDev_Delete(DEV_CLASS_AUDIO, DevID, &stParg) != RK_SUCCESS)
    {
        printf("RockCodecDev delete failure DevID %d\n",DevID);
        return RK_ERROR;
    }
    if (stParg.hCodec != NULL)
    {
        if (RKDev_Close(stParg.hCodec) != RK_SUCCESS)
        {
            printf("audioDevice hCodec failure\n");
            return RK_ERROR;
        }
    }

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 10:11:42
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellTest(HDC dev, uint8 * pstr)
{
    HDC hAudio;
    rk_err_t ret;
    uint32 vol,time = 0,i;
    AUDIO_DEVICE_CLASS * pstAudioDev;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.test : test RockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif
#ifdef _AudioTest_
    hAudio = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((hAudio == NULL) || (hAudio == (HDC)RK_ERROR) || (hAudio == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Audio device open failure\n");

        return RK_ERROR;
    }
    rk_print_string("Audio device open success\n");

    pstAudioDev = (AUDIO_DEVICE_CLASS *)hAudio;
    FREQ_EnterModule(FREQ_AUDIO_INIT);
    //SetI2SFreq(I2S_CH0,I2S_XIN12M,0);

    //Acodec_pll_Source_sel(CLK_ACODEC_PLL);//
    //Grf_GpioMuxSet(GPIO_CH2,GPIOPortA_Pin3,IOMUX_GPIO2A3_CLK_OBS);
    //SetI2SFreq(I2S_CH0,CLK_ACODEC_PLL,0);
    //OBS_output_Source_sel(obs_clk_i2s0);
    AudioDev_SetTrackLen(gpstAudioDevISRHandler, Audiolength);
    AudioDev_SetBit(pstAudioDev, 0, 16);
    printf("$#Bit =%d\n",gpstAudioDevISRHandler->Bit);
    AudioDev_GetMainTrack(hAudio);
    gpstAudioDevISRHandler->Track = (uint8 *)Audiooutptr;
    RockcodecDev_SetRate(pstAudioDev->hCodec,CodecFS_48KHz);//Smaple rate PLL  CodecFS_44100Hz

#if 0
     for(i=0;i < gpstAudioDevISRHandler->TrackLen;i++)
     printf("Old Track[%d]=0x%x\n",i,gpstAudioDevISRHandler->Track[i]);
#endif
    //Bit_Convertor_DEC
    if (gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH24+1))
    {
        printf("DEC test sour 24bit len=%d\n",gpstAudioDevISRHandler->TrackLen/(3*gpstAudioDevISRHandler->Channel));
        Bit_Convertor_DEC((short*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(3*gpstAudioDevISRHandler->Channel), 24);
    }
    else if(gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH16+1))
    {
        printf("DEC test 2sour 16bit len=%d\n",gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel));
        Bit_Convertor_DEC((short*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel), 16);
    }
    else
    {
        printf("DEC test ERROR:sour music no 16bit or 24bit \n");
    }

#ifdef _RK_EQ_
    //set EQ mode
    printf("cjh$#$test EQmode = %d\n", EQ_HEAVY);
    AudioDev_SetEQ(hAudio, EQ_HEAVY, 1); //SET EQ befor set vol
    AudioDev_SetVol(pstAudioDev, 25);
    printf("gpstAudioDevISRHandler->TrackLen = %d IsEQUpdate=%d\n",gpstAudioDevISRHandler->TrackLen,gpstAudioDevISRHandler->IsEQUpdate);
    //AudioDev_SetVol(pstAudioDev, 25);
    if(EQ_NOR != gpstAudioDevISRHandler->UserEQ.Mode)
    {
        /*audio effect process.*/
#ifdef CODEC_24BIT
        //if(gpstAudioDevISRHandler->SampleRate <= 96000)
        if (gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH24+1))
        {
            EffectProcess((long*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(3*gpstAudioDevISRHandler->Channel),\
                          &(gpstAudioDevISRHandler->UserEQ), &(gpstAudioDevISRHandler->IsEQUpdate));
        }
        else if(gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH16+1))
        {
            printf("16BIT EffectProcess TrackLen=%d\n",gpstAudioDevISRHandler->TrackLen);
            EffectProcess((long*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel),\
                          &(gpstAudioDevISRHandler->UserEQ), &(gpstAudioDevISRHandler->IsEQUpdate));
        }

#else
        printf("2_16BIT EffectProcess TrackLen=%d\n",gpstAudioDevISRHandler->TrackLen);
        EffectProcess((short*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel),\
                      &(gpstAudioDevISRHandler->UserEQ), &(gpstAudioDevISRHandler->IsEQUpdate));
#endif
    }
#endif //end #ifdef _RK_EQ_
    if (gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH24+1))
    {
        Bit_Convertor_shift((short*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(3*gpstAudioDevISRHandler->Channel), 24);
    }
    else if(gpstAudioDevISRHandler->Bit == (I2S_DATA_WIDTH16+1))
    {
        printf("!@@@@convertor shift 16bit len=%d\n",gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel));
        Bit_Convertor_shift((short*)gpstAudioDevISRHandler->Track, gpstAudioDevISRHandler->TrackLen/(2*gpstAudioDevISRHandler->Channel), 16);
    }
    else
    {
        printf("!ERROR2:sour music no 16bit or 24bit \n");
    }
#if 0
     for(i=0;i < gpstAudioDevISRHandler->TrackLen;i++)
     printf("Track[%d]=0x%x\n",i,gpstAudioDevISRHandler->Track[i]);
#endif
    //printf("^&&^%%& will write\n");

    while (1)
    {
        AudioDev_Write(pstAudioDev, TRACK_NO_MAIN, (uint8 *)gpstAudioDevISRHandler->Track);
//#ifdef AudioTest
//        RockcodecDev_Write(pstAudioDev->hCodec, (uint8 *)Audiooutptr, Audiolength, ASYNC_MODE);
//#endif
        time++;
        if (time == 8000)
        {
            printf("test over\n");
            break;
        }
        //ret = WM8987Dev_Write(pstAudioDev->hCodec, Audiooutptr, Audiolength, ASYNC_MODE);
    }
#endif
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: AudioShellHelp
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 10:11:42
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if ( StrLenA( pstr) != 0)
        return RK_ERROR;

    rk_print_string("audio命令集提供了一系列的命令对audio进行操作\r\n");
    rk_print_string("audio包含的子命令如下:            \r\n");
    rk_print_string("pcb       显示pcb信息             \r\n");
    rk_print_string("open      open audio              \r\n");
    rk_print_string("play      play audio              \r\n");
    rk_print_string("help      显示audio命令帮助信息   \r\n");

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: AudioShellPlay
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 9:08:47
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellPlay(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("audio.play  : play audio.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: AudioShellCreate
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellCreate(HDC dev,  uint8 * pstr)
{
    HDC hCodec;
    AUDIO_DEV_ARG stAudioArg;
    rk_err_t ret;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    I2S_DEV_ARG stI2Sarg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("audio.open  : open audio.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
#ifdef __DRIVER_AUDIO_AUDIODEVICE_C__

    //SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//测试默认12M

    hCodec = RKDev_Open(DEV_CLASS_ROCKCODEC,0,NOT_CARE);

    //hCodec = RKDev_Open(DEV_CLASS_WM8987, 0, NOT_CARE);

    if ((hCodec == NULL) || (hCodec == (HDC)RK_ERROR) || (hCodec == (HDC)RK_PARA_ERR))
    {
        rk_print_string("codec device open failure");

        return RK_SUCCESS;
    }

    stAudioArg.Bit = I2S_DATA_WIDTH24;
    stAudioArg.Channel = 2; //L/R
    stAudioArg.DevID = 0;
    stAudioArg.hCodec = hCodec;
    stAudioArg.SampleRate = CodecFS_48KHz;//CodecFS_44100Hz;//CodecFS_16KHz;
    stAudioArg.Vol = 25;
#ifdef _RK_EQ_
    stAudioArg.EQMode = EQ_NOR;
#endif
    ret = RKDev_Create(DEV_CLASS_AUDIO, 0, &stAudioArg);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("audio device create failure");
    }
#endif
    return ret;
}


/*******************************************************************************
** Name: AudioShellOpen
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellOpen(HDC dev,  uint8 * pstr)
{
    HDC hCodec;
    AUDIO_DEV_ARG stAudioArg;
    rk_err_t ret;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    I2S_DEV_ARG stI2Sarg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("audio.open  : open audio.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//测试默认12M
    //uint32 SetI2SFreq(UINT32 I2sId,Clock_Source_Sel Clk_Source,UINT32 TargetFreqHz)

//    ret=RKDev_Create(DEV_CLASS_DMA, DMA_CHN1, NULL);
//    if (ret != RK_SUCCESS)
//    {
//        UartDev_Write(dev, "DMA RKDev_Create failure\n",25,SYNC_MODE,NULL);
//    }
    /*
        stI2Sarg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN0, NOT_CARE);
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
    */
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

    return ret;
}


/*******************************************************************************
** Name: AudioShellPcb
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.8
** Time: 10:52:45
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_SHELL_
SHELL FUN rk_err_t AudioShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("audio.pcb : pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...
    return RK_SUCCESS;
}


#else
rk_err_t AudioDev_Shell(HDC dev, uint8 * pstr)
{

}
#endif
#endif

