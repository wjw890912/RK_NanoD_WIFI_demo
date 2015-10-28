/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: rk-iot\App\Audio\music.c
* Owner: cjh
* Date: 2015.7.17
* Time: 16:38:34
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    cjh     2015.7.17     16:38:34   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __APP_AUDIO_MUSIC_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "DeviceInclude.h"
#include "music.h"
#include "effect.h"
#include "BSP.h"
#include "http.h"
#include "wiced_management.h"
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*
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


typedef struct _MUSIC_CLASS
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

    AUDIO_TRACK stSubTrack[TRACK_NO_NUM - 1];

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


}MUSIC_CLASS;
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static HDC PAudio = NULL;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t music_shell_ff_resume(uint8 * pstr);
rk_err_t music_shell_mute(uint8 * pstr);
rk_err_t music_shell_unmute(uint8 * pstr);
rk_err_t music_shell_resume(uint8 * pstr);
rk_err_t music_shell_pause(uint8 * pstr);
rk_err_t music_shell_play(uint8 * pstr);
rk_err_t music_shell_stop(uint8 * pstr);
rk_err_t music_shell_up(uint8 * pstr);
rk_err_t music_shell_next(uint8 * pstr);
rk_err_t music_shell_ffw(uint8 * pstr);
rk_err_t music_shell_ffw_stop(uint8 * pstr);
rk_err_t music_shell_ffd(uint8 * pstr);
rk_err_t music_shell_ffd_stop( uint8 * pstr);
rk_err_t music_shell_set_eq(uint8 * pstr);
rk_err_t music_shell_set_vol_up(uint8 * pstr);
rk_err_t music_shell_set_vol_down(uint8 * pstr);
rk_err_t music_shell_create(uint8 * pstr);
rk_err_t music_shell_del(uint8 * pstr);
rk_err_t music_shell_help(void);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



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
_APP_AUDIO_MUSIC_SHELL_
static uint8* ShellMusicName[] =
{
    "play",
    "stop",
    "prev",
    "next",
    "ffw",
    "ffw_stop",
    "ffd",
    "ffd_stop",
    "set_eq",
    "up",
    "down",
    "create",
    "resume",
    "pause",
    "unmute",
    "ff_resume",
    "delete",
    "help",
    "\b",                // the end
};

/*******************************************************************************
** Name: music_shell
** Input:(HDC dev,  uint8 * pstr)
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL API rk_err_t music_shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 Space;

    printf("\n music_shell cmd:%s\n",pstr);
    StrCnt = ShellItemExtract(pstr,&pItem, &Space);
    if (StrCnt == 0)
    {
        printf("\n StrCnt = 0 \n");
        return RK_ERROR;
    }
    ret = ShellCheckCmd(ShellMusicName, pItem, StrCnt);
    if (ret < 0)
    {
        printf("\n ret < 0 \n");
        return RK_ERROR;
    }
    i = (uint32)ret;
    pItem += StrCnt;
    pItem++;         //remove '.',the point is the useful item
    printf("\n music_shell i = %d \n",i);
    switch (i)
    {
        case 0x00:
            ret = music_shell_play(pItem);
            break;

        case 0x01:
            ret = music_shell_stop(pItem);
            break;

        case 0x02:
            ret = music_shell_up(pItem);
            break;

        case 0x03:
            ret = music_shell_next(pItem);
            break;

        case 0x04:
            ret = music_shell_ffw(pItem);
            break;

        case 0x05:
            ret = music_shell_ffw_stop(pItem);
            break;

        case 0x06:
            ret = music_shell_ffd(pItem);
            break;

        case 0x07:
            ret = music_shell_ffd_stop(pItem);
            break;

        case 0x08:
            ret = music_shell_set_eq(pItem);
            break;

        case 0x09:
            ret = music_shell_set_vol_up(pItem);
            break;

        case 0x0a:
            ret = music_shell_set_vol_down(pItem);
            break;

        case 0x0b:
            ret = music_shell_create(pItem);
            break;

        case 0x0c:
            ret = music_shell_resume(pItem);
            break;

        case 0x0d:
            ret = music_shell_pause(pItem);
            break;

        case 0x0e:
            ret = music_shell_unmute(pItem);
            break;

        case 0x0f:
            ret = music_shell_ff_resume(pItem);
            break;

        case 0x10:
            ret = music_shell_del(pItem);
            break;

        case 0x11:
            ret = music_shell_help();
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
** Name: music_shell_ff_resume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:52:44
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL FUN rk_err_t music_shell_ff_resume(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.ff_resume : mute \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! AudioFFResume \r\n");
    //AudioFFResume();
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_FF_RESUME, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_unmute
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:52:44
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL FUN rk_err_t music_shell_mute(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.mute : mute \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! mute \r\n");
    //AudioResume();
    msg = 1;
    //AudioControlTask_SendCmd(AUDIO_CMD_MUTE,&msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_unmute
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:52:44
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL FUN rk_err_t music_shell_unmute(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.unmute : unmute \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! unmute \r\n");
    //AudioResume();
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_UNMUTE, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_resume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:52:44
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL FUN rk_err_t music_shell_resume(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.resume : resume play music \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! resume play music \r\n");
    //AudioResume();
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_RESUME, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}
/*******************************************************************************
** Name: music_shell_pause
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:51:20
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL FUN rk_err_t music_shell_pause(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.pause : pause play music \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    //AudioPause();
    printf("!!! pause play music \r\n");
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_PAUSE, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_play
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_play(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.play : play music \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    printf("!!! play music \r\n");
    msg = 1;
    //AudioStart();
    AudioControlTask_SendCmd(AUDIO_CMD_RESUME, &msg, SYNC_MODE);

    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_stop
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_stop(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.stop : stop music \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! stop music \r\n");
    //AudioStop(0);
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_STOP, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_up
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_up(uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.up : pre song\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! next song\r\n");

    //AudioUp((uint32)1);

    AudioControlTask_SendCmd(AUDIO_CMD_PREVFILE, NULL, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_next
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_next(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.next : next song\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! next song\r\n");

    //AudioNext((uint32)1);
    msg = 1;
    AudioControlTask_SendCmd(AUDIO_CMD_NEXTFILE, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_ffw
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_ffw(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.ffw : ffw\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! ffw\r\n");
    //AudioFFW(5);
    msg = 2000;
    AudioControlTask_SendCmd(AUDIO_CMD_FFW, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_ffw_stop
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_ffw_stop(uint8 * pstr)
{
    //uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.ffw_stop : ffw stop\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! ffw stop\r\n");
    AudioFFWStop();
    //msg = 1;
    //AudioControlTask_SendCmd(AUDIO_CMD_RESUME, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_ffd
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_ffd(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.ffd : ffd \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! ffd \r\n");
    //AudioFFD(5);
    msg = 2000;
    AudioControlTask_SendCmd(AUDIO_CMD_FFD, &msg, SYNC_MODE);
    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_ffd_stop
** Input:(HDC dev,  uint8 * pstr)
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_ffd_stop( uint8 * pstr)
{
    //uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.ffd_stop : ffd stop\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("!!! ffd stop\r\n");
    AudioFFDStop();
    //msg = 1;
    //AudioControlTask_SendCmd(AUDIO_CMD_RESUME,&msg, SYNC_MODE);
    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_set_eq
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_set_eq(uint8 * pstr)
{
    //MUSIC_CLASS * pstMusic;
    AUDIO_DEV_ARG stParg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.set_eq.heavy : set eq heavy mode \r\n");
            printf("music.set_eq.pop : set eq heavy mode \r\n");
            printf("music.set_eq.jazz : set eq heavy mode \r\n");
            printf("music.set_eq.unique : set eq heavy mode \r\n");
            printf("music.set_eq.class : set eq heavy mode \r\n");
            printf("music.set_eq.bass : set eq heavy mode \r\n");
            printf("music.set_eq.rock : set eq heavy mode \r\n");
            printf("music.set_eq.user : set eq heavy mode \r\n");
            printf("music.set_eq.nor : set eq heavy mode \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    AudioDev_Get_Par(&stParg);

    if(PAudio == NULL)
    {
        printf("please run music.open first\n");
        return RK_ERROR;
    }

    //pstMusic = (MUSIC_CLASS *)PAudio;

    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "heavy", 5) == 0)
        {
            printf("set eq EQ_HEAVY mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_HEAVY, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "pop", 3) == 0)
        {
            printf("set eq EQ_POP mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_POP, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "jazz", 4) == 0)
        {
            printf("set eq EQ_JAZZ mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_JAZZ, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "unique", 6) == 0)
        {
            printf("set eq EQ_UNIQUE mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_UNIQUE, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "class", 5) == 0)
        {
            printf("set eq EQ_CLASS mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_CLASS, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "bass", 4) == 0)
        {
            printf("set eq EQ_BASS mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_BASS, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "rock", 4) == 0)
        {
            printf("set eq EQ_ROCK mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_ROCK, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "user", 4) == 0)
        {
            printf("set eq EQ_USER mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_USER, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
        else if(StrCmpA(pstr, "nor", 3) == 0)
        {
            printf("set eq EQ_NOR mod vol=%d\n",stParg.Vol);
            AudioDev_SetEQ(PAudio, EQ_NOR, 1);
            AudioDev_SetVol(PAudio, stParg.Vol);
            return RK_SUCCESS;
        }
    }

    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_set_vol_up
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_set_vol_up(uint8 * pstr)
{
    //MUSIC_CLASS * pstMusic;
    AUDIO_DEV_ARG stParg;
    uint8 vol;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.set_vol_up : set vol + 1 \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    AudioDev_Get_Par(&stParg);

    if(PAudio == NULL)
    {
        printf("please run music.open first\n");
        return RK_ERROR;
    }

    //pstMusic = (MUSIC_CLASS *)PAudio;

    if(stParg.Vol+1 <= 0)
    {
        stParg.Vol = 0;
        vol = 0;
    }
    else if(stParg.Vol >=30)
    {
        stParg.Vol = 30;
        vol = 30;
    }
    else
    {
        vol = stParg.Vol + 1;
    }

    AudioSetVolume(vol);
    AudioDev_SetVol(PAudio, vol);

    return RK_SUCCESS;;
}


/*******************************************************************************
** Name: music_shell_set_vol_down
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_set_vol_down(uint8 * pstr)
{
    //MUSIC_CLASS * pstMusic;
    AUDIO_DEV_ARG stParg;
    uint32 vol;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.set_vol_up : set vol - 1 \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    AudioDev_Get_Par(&stParg);

    if(PAudio == NULL)
    {
        printf("please run music.open first\n");
        return RK_ERROR;
    }

    //pstMusic = (MUSIC_CLASS *)PAudio;

    if(stParg.Vol <= 0)
    {
        stParg.Vol = 0;
        vol = 0;
    }
    else if(stParg.Vol-1 >=30)
    {
        stParg.Vol = 30;
        vol = 30;
    }
    else
    {
        vol = stParg.Vol - 1;
    }

    AudioSetVolume(vol);
    AudioDev_SetVol(PAudio, vol);

    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_create
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_create(uint8 * pstr)
{

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("music.open : open music    \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    printf("music_shell_create\n");
    PAudio = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((PAudio == NULL) || (PAudio == (HDC)RK_ERROR) || (PAudio == (HDC)RK_PARA_ERR))
    {
        printf("Audio device open failure\n");

        return RK_ERROR;
    }
    printf("Audio device open success\n");

    //FREQ_EnterModule(FREQ_MAX);
    //printf("AUDIOCONTROL create\n");
    //RKTaskCreate(TASK_ID_MEDIA, 0, NULL);
    //RKTaskCreate(TASK_ID_FILESTREAM, 0, NULL);
    RKTaskCreate(TASK_ID_AUDIOCONTROL, 0, NULL);
    RKTaskCreate(TASK_ID_STREAMCONTROL, 0, NULL);

    #if 0
    ret = HttpTask_Get("http://192.168.1.101/I%20love%20you.pcm");
    if(ret == RK_SUCCESS)
        printf("\nhttp ok\n");
    else
        printf("\nhttp error\n");

    RKTaskCreate(TASK_ID_MEDIA, 0, NULL);
    #endif

    rk_printf("will return\n");

    return RK_SUCCESS;;
}

/*******************************************************************************
** Name: music_shell_del
** Input:uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_del(uint8 * pstr)
{
    uint32 msg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("music.open : open music    \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    RKTaskDelete(TASK_ID_AUDIOCONTROL, 0, SYNC_MODE);
    printf("Delete audiocontrol task OK\n");
    RKTaskDelete(TASK_ID_STREAMCONTROL, 0, SYNC_MODE);
    printf("Delete streamcontol task OK\n");
    #if 0
    ret = HttpTask_Get("http://192.168.1.101/I%20love%20you.pcm");
    if(ret == RK_SUCCESS)
        printf("\nhttp ok\n");
    else
        printf("\nhttp error\n");

    RKTaskCreate(TASK_ID_MEDIA, 0, NULL);
    #endif

    printf("3 del success\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: music_shell_help
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.17
** Time: 16:48:43
*******************************************************************************/
_APP_AUDIO_MUSIC_SHELL_
SHELL COMMON rk_err_t music_shell_help(void)
{
    printf("music命令集提供了一系列的命令对music进行操作\r\n");
    printf("music包含的子命令如下:            \r\n");

    printf("music.open     : 打开音乐播放\r\n");

    printf("music.play     : 播放 \r\n");
    printf("music.stop     : 暂停 \r\n");
    printf("music.up       : 上一首\r\n");
    printf("music.next     : 下一首\r\n");
    printf("music.ffw      : 快进\r\n");
    printf("music.ffw_stop : 停止快进\r\n");
    printf("music.ffd      : 快退 \r\n");
    printf("music.ffd_stop : 停止快退\r\n");
    printf("music.set_eq    : eq设置入口 \r\n");
    printf("music.vol_up   : 音量加1 \r\n");
    printf("music.vol_down : 音量加1 \r\n");

    return RK_SUCCESS;;
}

#endif
