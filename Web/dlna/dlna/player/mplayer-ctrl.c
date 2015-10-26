#include <rtos_config.h>

#if !defined(FREE_RTOS)
#define LOG_TAG "DlnaMplayer"
#include <utils/Log.h>
#include <linux/sem.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <VolumeClientProxy.h>
#endif
#include <player/controller.h>
#include "bspconfig.h"
#include "source_macro.h"
#include "rkos.h"

#ifdef __WIFI_DLNA_C__
ctrl_t mplayer_ctrl;
static HDC PAudio = NULL;


static pstate_t play_state;
static state_listener_t play_state_changed;


int MplayerCtrl_Init(void)
{
    RK_TASK_AUDIOCONTROL_ARG pArg;

    rk_printf("MplayerCtrl_Init\n");
    play_state = PLAYER_STOPPED;
    pArg.ucSelPlayType = SOURRE_FROM_DLNA;
    pArg.FileNum = 1;
    //memcpy(pArg.filepath, "http://192.168.1.100/%E6%89%93%E8%80%81%E8%99%8E.MP3", 200);

    RKTaskCreate(TASK_ID_AUDIOCONTROL, 0, &pArg);
    RKTaskCreate(TASK_ID_STREAMCONTROL, 0, NULL);
    PAudio = RKDev_Open(DEV_CLASS_AUDIO,0,NOT_CARE);
    if ((PAudio == NULL) || (PAudio == (HDC)RK_ERROR) || (PAudio == (HDC)RK_PARA_ERR))
    {
        printf("Audio device open failure\n");

        return RK_ERROR;
    }


    return 0;
}

int MplayerCtrl_Deinit(void)
{
    play_state = PLAYER_STOPPED;
    Http_Close();
    RKTaskDelete(TASK_ID_AUDIOCONTROL, 0, SYNC_MODE);
    RKTaskDelete(TASK_ID_STREAMCONTROL, 0, SYNC_MODE);
    RKDev_Close(PAudio);
    PAudio = NULL;
    return 0;
}

int MplayerCtrl_Play(char *url)
{
    int dwRet;
    uint32 state;
    RK_TASK_AUDIOCONTROL_ARG pArg;

    printf("\nMplayerCtrl_Play url = %s, play_state = %d\n", url, play_state);

    dwRet = HttpGet_Url(url);
    if(dwRet == 0)
    {
        printf("\nhttp ok\n");
        AudioControlTask_SendCmd(AUDIO_CMD_DECSTART, NULL, ASYNC_MODE);
        return 0;
    }
    else
    {
        printf("\nhttp error\n");
        MplayerCtrl_Set_state(PLAYER_STOPPED);
        return -1;
    }
}

int MplayerCtrl_Stop(void)
{
    rk_printf("MplayerCtrl_Stop\n");


    if (MplayerCtrl_Get_state() == PLAYER_STOPPED)
		return -1;


    Http_Close();

    AudioControlTask_SendCmd(AUDIO_CMD_STOP, (void *)Audio_Stop_Force, ASYNC_MODE);

    rkos_delay(1000);

    MplayerCtrl_Set_state(PLAYER_STOPPED);

    return 0;
}

int MplayerCtrl_Pause(void)
{
    //rk_printf("MplayerCtrl_Pause\n");

    AudioControlTask_SendCmd(AUDIO_CMD_PAUSE, NULL, SYNC_MODE);
   if (play_state == PLAYER_PLAYING){
		MplayerCtrl_Set_state(PLAYER_PAUSED);
		return 0;
	}
    return -1;
}

int MplayerCtrl_Resume(void)
{
    if (play_state == PLAYER_PAUSED) {
		//ALOGD("resume!");

		//mp_send_cmd("pause\n");
		AudioControlTask_SendCmd(AUDIO_CMD_RESUME, NULL, SYNC_MODE);

		MplayerCtrl_Set_state(PLAYER_PLAYING);
		//mp_send_get_pos_cmd();
		return 0;
	}

}

int MplayerCtrl_Seek(int ms)
{
    rk_printf("MplayerCtrl_Seek\n");

    //AudioControlTask_SendCmd(AUDIO_CMD_FFD, NULL, SYNC_MODE);

    return 0;
}

int MplayerCtrl_Current_time(char* buf, int len)
{
    int hour;
    int minute;
    int second;
    uint32 time;

    if(play_state == PLAYER_STOPPED)
    {
        time = 0;
    }
    else
    {
    //rk_printf("MplayerCtrl_Current_time\n");
        AudioPlayer_GetCurTime(&time);
    }
    //rk_printf("Current_time11 = %d\n", time);
    hour = time/(1000*3600);
    time = time%(1000*3600);
    minute = time/(1000*60);
    time = time%(1000*60);
    second = time/1000;

    sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
    //printf("current_time = %s\n",buf);

    return 0;
}

int MplayerCtrl_Get_duration(char* buf, int len)
{

    int hour;
    int minute;
    int second;
    uint32 time;

    //rk_printf("MplayerCtrl_Get_duration\n");
    if(play_state == PLAYER_STOPPED)
    {
         time = 0;
    }
    else
    {
        AudioPlayer_GetTotalTime(&time);
    }

   // rk_printf("totle_time11 = %d\n", time);
    hour = time/(1000*3600);
    time = time%(1000*3600);
    minute = time/(1000*60);
    time = time%(1000*60);
    second = time/1000;



    sprintf(buf, "%02d:%02d:%02d", hour, minute, second);
    //printf("duration = %s\n",buf);

    return 0;
}

int MplayerCtrl_Get_state(void)
{
    return (int)play_state;
}

int MplayerCtrl_Set_state(int state)
{
    rk_printf("MplayerCtrl_Set_state state = %d\n", state);

    if (play_state == state) {
		return -1;
	}

	play_state = state;

	if (play_state_changed)
		play_state_changed(play_state);

    return 0;
}

int MplayerCtrl_State_listener(state_listener_t listener)
{
    rk_printf("MplayerCtrl_State_listener\n");
    play_state_changed = listener;
    return 0;
}

int MplayerCtrl_Set_volume(int volume)
{
    rk_printf("MplayerCtrl_Set_volume\n");

    if(PAudio != NULL)
    {
        if(volume/3 > 32)
            volume = 32;
        else
            volume = volume/3;
        AudioSetVolume(volume);
        AudioDev_SetVol(PAudio, volume);
    }
    return 0;
}

int MplayerCtrl_Player_Stop()
{
    rk_printf("stop ****************************************");
    MplayerCtrl_Set_state(PLAYER_STOPPED);
}

int MplayerCtrl_Player_Play()
{
    rk_printf("play ****************************************");
    MplayerCtrl_Set_state(PLAYER_PLAYING);
}


int Mplayer_Init(void)
{
    memset(mplayer_ctrl.name, 0x00, 128);
    mplayer_ctrl.init = MplayerCtrl_Init;
    mplayer_ctrl.deinit = MplayerCtrl_Deinit;
    mplayer_ctrl.play =  MplayerCtrl_Play;
    mplayer_ctrl.stop = MplayerCtrl_Stop;
    mplayer_ctrl.pause = MplayerCtrl_Pause;
    mplayer_ctrl.seek =MplayerCtrl_Seek;
    mplayer_ctrl.resume = MplayerCtrl_Resume;
    mplayer_ctrl.current_time = MplayerCtrl_Current_time;
    mplayer_ctrl.get_duration = MplayerCtrl_Get_duration;
    mplayer_ctrl.get_state = MplayerCtrl_Get_state;
    mplayer_ctrl.set_state = MplayerCtrl_Set_state;
    mplayer_ctrl.state_listener = MplayerCtrl_State_listener;
    mplayer_ctrl.set_volume = MplayerCtrl_Set_volume;
}

#endif


