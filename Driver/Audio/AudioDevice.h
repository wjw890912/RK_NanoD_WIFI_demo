/*
********************************************************************************************
*
*                  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*
* FileName: ..\Driver\Audio\AudioDevice.h
* Owner: Aaron.sun
* Date: 2014.5.7
* Time: 17:48:32
* Desc: Audio Device Class
* History:
*     <author>     <date>       <time>     <version>       <Desc>
* Aaron.sun     2014.5.7     17:48:32   1.0
********************************************************************************************
*/

#ifndef __DRIVER_AUDIO_AUDIODEVICE_H__
#define __DRIVER_AUDIO_AUDIODEVICE_H__

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
#define _DRIVER_AUDIO_AUDIODEVICE_COMMON_  __attribute__((section("driver_audio_audiodevice_common")))
#define _DRIVER_AUDIO_AUDIODEVICE_INIT_  __attribute__((section("driver_audio_audiodevice_init")))
#define _DRIVER_AUDIO_AUDIODEVICE_SHELL_  __attribute__((section("driver_audio_audiodevice_shell")))

typedef enum _AUDIO_PROCESS
{
    AUDIO_PROCESS_CLOSE,
    AUDIO_PROCESS_EQ,
    AUDIO_PROCESS_FILTER,
    AUDIO_PROCESS_FADE_IN,
    AUDIO_PROCESS_FADE_OUT,
    AUDIO_PROCESS_CHANGE_SPEED,
    AUDIO_PROCESS_CHANGE_TONE,
    AUDIO_PROCESS_ECHO_CANCEL,
    AUDIO_PROCESS_MIX,
    AUDIO_PROCESS_MIX_0,
    AUDIO_PROCESS_MIX_1,
    AUDIO_PROCESS_MIX_2,
    AUDIO_PROCESS_MIX_3,
    AUDIO_PROCESS_MIX_4,
    AUDIO_PROCESS_MIX_5,
    AUDIO_PROCESS_MIX_6,
    AUDIO_PROCESS_MIX_7,
    AUDIO_PROCESS_MIX_8,
    AUDIO_PROCESS_MIX_9,
    AUDIO_PROCESS_MIX_10,
    AUDIO_PROCESS_MIX_11,
    AUDIO_PROCESS_MIX_12,
    AUDIO_PROCESS_MIX_13

}AUDIO_PROCESS;

typedef enum _TRACK_NO
{
    TRACK_NO_MAIN,
    TRACK_NO_SUB1,
    TRACK_NO_SUB2,
    TRACK_NO_SUB3,
    TRACK_NO_SUB4,
    TRACK_NO_NUM

}TRACK_NO;

typedef struct _AUDIO_DEV_ARG
{
    HDC hCodec;
    uint32 DevID;
    uint32 SampleRate;
    uint32 Channel;
    uint32 Bit;
    uint32 EQMode;
    uint32 Vol;
}AUDIO_DEV_ARG;

typedef struct _AUDIO_DEV_I2S_CODEC_CONFIG_ARG
{
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    ROCKCODEC_DEV_CONFIG_ARG  stCodecArg;
}AUDIO_DEV_I2S_CODEC_CONFIG_ARG;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/


//extern volatile track_info*         pAudioRegKey;
//extern volatile BOOL                IsEQNOR;//turn to NOR when user EQ's DB value turn to 0.

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t AudioDev_Get_Par(AUDIO_DEV_ARG *arg);
extern rk_err_t AudioDev_I2S_Acodec_Mode(HDC dev, void *arg);
extern void Bit_Convertor_shift(short *ppsBuffer, uint32 Length, int bps);
extern void Bit_Convertor_DEC(short *ppsBuffer, uint32 Length, int bps);
extern void AudioDev_SetVolume(void);
extern void AudioDev_SetEQ(HDC dev, uint32 userEQMod, int isEQUpdate);
extern rk_err_t AudioDev_SetVol(HDC dev, uint32 vol);
extern rk_err_t AudioDev_SetTrackProcess(HDC dev, uint32 TrackNo, uint8 subTrackProcess);
extern rk_err_t AudioDev_RealseMainTrack(HDC dev);
extern rk_err_t AudioDev_SetTrackLen(HDC dev, uint32 len);
extern rk_err_t AudioDev_GetSubTrack(HDC dev);
extern rk_err_t AudioDev_GetMainTrack(HDC dev);
extern rk_err_t AudioDev_Write(HDC dev , uint32 TrackNo, uint8 * buf);
extern rk_err_t AudioDev_SetMixScale(HDC dev ,uint32 TrackNo, uint32 MixMain, uint32 MixSub);
extern rk_err_t AudioDev_SetBit(HDC dev, uint32 TrackNo, uint32 Bit);
extern rk_err_t AudioDev_SetChannel(HDC dev, uint32 TrackNo, uint32 Channel);
extern rk_err_t AudioDev_SetSampleRate(HDC dev, uint32 TrackNo, uint32 SamleRate);
extern rk_err_t AudioDev_SetModel(HDC dev,  uint32 TrackNo, uint32 ProcessNo, uint32 ProcessPos);
extern rk_err_t AudioDev_Shell(HDC dev, uint8 * pstr);
extern HDC AudioDev_Create(uint32 DevID, void *arg);
extern rk_err_t AudioDev_Delete(uint32 DevID, void * arg);


#endif

