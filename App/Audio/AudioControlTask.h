/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioControlTask.h
* Owner: aaron.sun
* Date: 2015.6.18
* Time: 10:19:58
* Version: 1.0
* Desc: Audio Control Task
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.18     10:19:58   1.0
********************************************************************************************
*/


#ifndef __APP_AUDIO_AUDIOCONTROLTASK_H__
#define __APP_AUDIO_AUDIOCONTROLTASK_H__

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

//播放顺序定义
typedef enum
{
    AUDIO_INTURN,
    AUDIO_RAND

}AUDIOPLAYMODE;

typedef struct
{
    uint32 ABRequire;
    uint32 AudioABStart;
    uint32 AudioABEnd;
    uint32 PlayDirect;
    uint8  playVolume;
    uint8  VolumeCnt;

}AUDIOCONTROL;

typedef enum _AUDIO_CMD
{
    AUDIO_CMD_DECSTART,
    AUDIO_CMD_PAUSE,
    AUDIO_CMD_RESUME,
    AUDIO_CMD_STOP,
    AUDIO_CMD_FFD,
    AUDIO_CMD_FFW,
    AUDIO_CMD_VOLUMESET,
    AUDIO_CMD_EQSET,
    AUDIO_CMD_UNMUTE,
    AUDIO_CMD_NEXTFILE,
    AUDIO_CMD_PREVFILE,
    AUDIO_CMD_FF_PAUSE,
    AUDIO_CMD_FF_RESUME

}AUDIO_CMD;

typedef enum _AUDIO_STATE
{
    AUDIO_STATE_PLAY,
    AUDIO_STATE_FFD,
    AUDIO_STATE_FFW,
    AUDIO_STATE_PAUSE,
    AUDIO_STATE_STOP,
    AUDIO_STATE_PLAY_FFD,
    AUDIO_STATE_PLAY_FFW,
    AUDIO_STATE_PAUSE_FFD,
    AUDIO_STATE_PAUSE_FFW,
    AUDIO_STATE_FFD_PLAY,
    AUDIO_STATE_FFW_PLAY

}AUDIO_STATE;

//reapeat mode
typedef enum _AUDIO_REPEAT_MODE
{
    AUDIO_FOLDER_ONCE,    //once directory.
    AUIDO_FOLDER_REPEAT,  //directory cycle
    AUDIO_REPEAT,         //repeat one song
    AUDIO_ALLONCE,        //repeat once all song.
    AUDIO_ALLREPEAT,      //cycle play all song.
    AUDIO_REPEAT1,
    AUDIO_ONCE,           //repeat once one song.
    AUDIO_TRY              //lyrics

}AUDIO_REPEAT_MODE;

//audio effect define of user.
typedef enum
{
    EQ_N12DB,
    EQ_N8DB,
    EQ_N4DB,
    EQ_0DB,
    EQ_4DB,
    EQ_8DB,
    EQ_12DB

}USER_EQ;

typedef enum
{
    SOURRE_FROM_WIFI=0x55,
    SOURRE_FROM_DLNA,
    SOURRE_FROM_BT,
    SOURRE_FROM_EMMC,
    SOURRE_MAX

}AUDIO_SOURCE;

//Play Range define
#define     FIND_FILE_RANGE_DIR            2// once direction
#define     FIND_FILE_RANGE_ALL            3// cycle in direction



#define _APP_AUDIO_AUDIOCONTROLTASK_COMMON_  __attribute__((section("app_audio_audiocontroltask_common")))
#define _APP_AUDIO_AUDIOCONTROLTASK_INIT_  __attribute__((section("app_audio_audiocontroltask_init")))
#define _APP_AUDIO_AUDIOCONTROLTASK_SHELL_  __attribute__((section("app_audio_audiocontroltask_shell")))

#define AUDIO_DMACHANNEL_IIS    (DMA_CHN_MAX - 1)

#define        SORT_TYPE_SEL_NOW_PLAY        0
#define     SORT_TYPE_SEL_ID3TITLE      1
#define     SORT_TYPE_SEL_ID3ALBUM      3
#define     SORT_TYPE_SEL_ID3SINGER     2

#define     SORT_TYPE_SEL_GENRE         4//流派
#define     SORT_TYPE_PLAY_LIST         5
#define     SORT_TYPE_SEL_FOLDER        6//文件夹浏览

#define     MUSIC_TYPE_SEL_RECORDFILE   7
#define     MUSIC_TYPE_SEL_FMFILE       8
#define     MUSIC_TYPE_SEL_MYFAVORITE   9
#define        MEDIA_SORT_UPDATE_SEL        10
#define        MEDIA_MUSIC_BREAKPOINT        11
#define     MUSIC_TYPE_SEL_RECORDFILE_DEL 12

#define SORT_TYPE_SEL_M3U_BROWSER 98
#define SORT_TYPE_SEL_BROWSER  99
#define SORT_TYPE_SEL_FILENAME 100


#define Audio_Stop_Normal       0
#define Audio_Stop_Force        1

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern char        AudioFileExtString[];
extern char        RecordFileExtString[];

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t AudioPlayer_GetTotalTime(uint32 * time);
extern rk_err_t AudioPlayer_GetCurTime(uint32 * time);
extern rk_err_t AudioPlayer_GetState(uint32 * state);
extern void AudioStart(void);
extern void AudioStop(UINT16 ReqType);
extern rk_err_t AudioUp();
extern rk_err_t AudioNext();
extern void AudioFFDStop(void);
extern rk_err_t AudioFFD(uint32 StepLen);
extern void AudioFFWStop(void);
extern rk_err_t AudioFFW(uint32 StepLen);
extern rk_err_t AudioFFResume(void);
extern rk_err_t AudioResume(void);
extern rk_err_t AudioFFPause(void);
extern rk_err_t AudioPause(void);
extern rk_err_t AudioGetNextMusic(int msg);
extern rk_err_t AudioControlTask_SendCmd(uint32 Cmd, void * msg, uint32 Mode);
extern void AudioControlTask_Enter(void);
extern rk_err_t AudioControlTask_DeInit(void *pvParameters);
extern rk_err_t AudioControlTask_Init(void *pvParameters, void *arg);



#endif
