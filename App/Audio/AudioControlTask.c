/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\Audio\AudioControlTask.c
* Owner: aaron.sun
* Date: 2015.6.18
* Time: 10:19:27
* Version: 1.0
* Desc: Audio Control Task
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.18     10:19:27   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __APP_AUDIO_AUDIOCONTROLTASK_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"
#include "effect.h"
#include "audio_globals.h"
#include "SysInfoSave.h"
#include "Msg.h"
#include "AudioControlTask.h"
#include "audio_file_access.h"
#include "BspConfig.h"
#include "hw_mp3_imdct.h"
#include "hw_mp3_syn.h"
#include "mp3_accelerator_hal.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define _IN_AUDIO_CONTROL_

#ifdef _RK_SPECTRUM_
#define SPECTRUM_LINE_M      10     // 10跟频谱线
#define SpectrumLoopTime     3
#endif
/*
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
*/
typedef struct _TRACK_INFO
{
    unsigned long  TotalTime;
    unsigned long  CurrentTime;
    int            samplerate;
    int            bitrate;
    int            channels;
    int            total_length;
    int            cur_pos;
    int            RepeatMode;
    int            PlayOrder; //lyrics or order
    int            AudioFileDirOrAll;//directory files or all files.
    char           artist[TRACK_INFO_STRING_LEN];
    char           title[TRACK_INFO_STRING_LEN];
    char           albumb[TRACK_INFO_STRING_LEN];
    int            bps;
}TRACK_INFO;

typedef  struct _AUDIOCONTROL_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}AUDIOCONTROL_RESP_QUEUE;

typedef  struct _AUDIOCONTROL_ASK_QUEUE
{
    uint32 cmd;
    void * msg;
    uint32 mode;

}AUDIOCONTROL_ASK_QUEUE;

typedef struct _SYS_FILE_INFO
{
    uint32      TotalFiles;     //the total number of current direction/disk
    uint32      CurrentFileNum;
    uint32      TotalFileNum;
    uint32      PlayedFileNum;
    uint32      DiskTotalFiles; //he total number of current disk
    uint32      Range;//cycle direction or once direction
    uint32      PlayOrder; // random or order
    uint32      RepeatMode;
    uint32      AudioFileDirOrAll;//directory files or all files.
    uint8      *pExtStr;       //file type
    uint16      path[259];
    uint32      ucSelPlayType;  // add by phc

    uint32  ucCurDeep; // add by phc
    uint32  ulFullInfoSectorAddr;  // add by phc
    uint32  ulSortInfoSectorAddr;  // add by phc
    UINT16  uiCurId[8]; // add by phc
    UINT16 uiBaseSortId[4]; // add by phc

} SYS_FILE_INFO;

typedef  struct _AUDIOCONTROL_TASK_DATA_BLOCK
{
    pQueue      AudioControlAskQueue;
    pQueue      AudioControlRespQueue;
    uint32      TaskObjectID;
    HDC         hAudio;
    HDC         hDir;
    HDC         hFile[3];
    HDC         hFileDev;
    HDC         hDirDev;
    HDC         hBcore;
    HDC         hMsg;
    HDC         hFifo;
    uint32      TrackNo;

    int32       CurrentCodec;
    uint32      AudioPlayState;
    uint32      AudioPlayerState;
    uint32      AudioCodecOpenErr;
    uint32      AudioPtr;
    uint32      AudioNeedDecode;
    uint32      AudioLen;
    uint32      AudioErrorFrameNum;
    uint32      AudioEndFade;
    uint32      AudioFadeInStart;
    uint32      ABRequire;
    uint32      AudioABStart;
    uint32      AudioABEnd;
    uint32      PlayDirect;
    uint32      playVolume;
    uint32      StreamEnd;

#if 1
    uint32               AudioStopMode;   //normal stop or Force//Audio_Stop_Normal or Audio_Stop_Force
    uint32              MusicLongFileName[MAX_FILENAME_LEN];
    AudioInOut_Type     AudioIOBuf;
    AUDIOCONTROL        AudioPlayInfo;
    uint32              AudioPlayFileNum;
    uint32              AudioPlayFileNumSave;
    BOOLEAN             MusicUIState; //audio interface UI status.0 mean display audio interface.1 mean not.
    uint32              AudioErrorFileCount;//sen #20090803#1 all audio file is not support
    uint32              AudioDecodeCntBak;
    //the offset value of audio file seek.
    uint32              AudioFileSeekOffset;

    uint32              oldtick;  //used for musicserver
    uint32              gAudioPlayTime;
    uint32              IsEQNOR;//turn to NOR when user EQ's DB value turn to 0.

#endif
    TRACK_INFO     *pAudioRegKey;
    SYS_FILE_INFO  AudioFileInfo;
    RKEffect       UserEQ;
    int            IsEQUpdate;

#ifdef _RK_SPECTRUM_
    uint8 SpectrumOut[SPECTRUM_LINE_M];//存放转换过后的M根谱线的能量
    uint32 SpectrumLoop;
    uint32 SpectrumCnt;
    uint32 SpectrumEn;
#endif

}AUDIOCONTROL_TASK_DATA_BLOCK;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static AUDIOCONTROL_TASK_DATA_BLOCK * gpstAudioControlData;
static uint8 EqMode[8] = {EQ_HEAVY,EQ_POP,EQ_JAZZ,EQ_UNIQUE,EQ_USER,EQ_USER,EQ_NOR,EQ_BASS};
static short UseEqTable[CUSTOMEQ_LEVELNUM] = {-10, -6, -3, 0, 3, 6, 10};



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
uint32 gDecCmd;
uint32 gDecData;
pSemaphore osAudioDecodeOk;
HDC hAudioDma;
uint32 gError;
uint32 gACKDone;

volatile unsigned int coed_dma_channel;
MediaBlock    gpMediaBlock;
FILE_HANDLE_t gFileHandle;

unsigned char  * DecDataBuf[2];

unsigned char DecBufID;

//global variables
int CurrentCodec;

char        AudioFileExtString[]   = "MP1MP2MP3WMAWAVAPELACAACM4AOGGMP43GP";
char        RecordFileExtString[]  = "WAV";

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t GetFileInfoSetNewMusic(FILE_ATTR *stFileAttr, int32 fileNum, TRACK_INFO* tackInfo);
void AudioFREQDeInit(void);
void AudioFileClose(void);
void AudioWaitBBStop(void);
void AudioDeHWInit(void);
void CloseTrack(void);
void AudioVariableDeInit(void);
void AudioWaitBBStart(void);
int32 CheckID3V2Tag(uint8 *pucBuffer);
void AudioCheckStreamType(uint16 * path,  HDC hFile);
uint8 GetFileType(UINT16 *path, UINT8 *pStr);
void AudioCodec(UINT16 *pBuffer, UINT8 *pStr);
void AudioDecodeProc(AUDIO_CMD id, void * msg);
void AudioHoldonInit(void);
void AudioSetVolume(uint32 Volume);
void AudioFREQInit(void);
rk_err_t AudioCodecOpen(void);
void AudioHWInit(void);
rk_err_t AudioFileOpen(void);
void AudioVariableInit(void);
void SysFindFileInit(SYS_FILE_INFO *pSysFileInfo,UINT16 GlobalFileNum,UINT16 FindFileRange,UINT16 PlayMode, uint8 *pExtStr);
void RegMBoxDecodeSvc(void);
__irq void AudioDecodingGetOutBuffer(void);
//void Bit_Convertor_shift(short *ppsBuffer, long *plLength,int bps);
//void Bit_Convertor_DEC(short *ppsBuffer, long *plLength,int bps);
rk_err_t AudioControlTask_Resume(void);
rk_err_t AudioControlTask_Suspend(void);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioPlayer_GetTotalTime
** Input:uint32 * time
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.31
** Time: 16:20:42
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioPlayer_GetTotalTime(uint32 * time)
{
    *time = gpstAudioControlData->pAudioRegKey->TotalTime;
}

/*******************************************************************************
** Name: AudioPlayer_GetCurTime
** Input:uint32 * time
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.31
** Time: 16:19:51
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioPlayer_GetCurTime(uint32 * time)
{
    *time = gpstAudioControlData->pAudioRegKey->CurrentTime;
}

/*******************************************************************************
** Name: AudioPlayer_GetState
** Input:uint32 * state
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.31
** Time: 13:34:41
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioPlayer_GetState(uint32 * state)
{
    *state = gpstAudioControlData->AudioPlayState;
}

/*******************************************************************************
** Name: AudioFlieSour
** Input:(HDC dev, uint8 *purl)
** Return: rk_err_t
** Owner:cjh
** Date: 2015.8.12
** Time: 19:30:56
*******************************************************************************/
/*
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioFlieSour(HDC dev, uint8 *purl, void *fileAttr)
{
    HDC hFifo;
    uint32 TotalSize,i;
    rk_err_t ret;
    uint8 *url;
    uint16 LongName[64] = 0;
    FILE_ATTR stFileAttr;
    uint32 BufLen;
    uint8 Buf[128];

    &stFileAttr = fileAttr;
    if(dev = NULL)
    {
        printf("hfifo null\n");
        return RK_ERROR;
    }
    hFifo = dev;
    //ret = HttpTask_Get("http://192.168.0.125/%E6%9C%88%E4%BA%AE%E4%B9%8B%E4%B8%8A.APE");
    if(*purl != NULL)
    {
        url = purl;
        while (*purl == ' ')
        {
            purl++;    //skip spaces
        }

        //get proto "http://"
        if (StrCmpA(purl, "http://", 7) == 0)
        {
            purl += 7;
        }
        else
        {
            printf("url error\n");
            return RK_ERROR;
        }

        //get file:"ApiForM/TjMusicList.php?Apiadmin=xiaobuding"
        if (*purl == '/')
        {
            purl++; //skip "/"
            while (*purl && !isspace(*purl))
            {
                if (LongName)
                {
                    *LongName++ = *purl;
                    *LongName   = 0;
                }
                purl++;
            }
        }
        ret = HttpTask_Get(url);
        if(ret == RK_SUCCESS)
            printf("\nhttp ok\n");
        else
            printf("\nhttp error\n");

        //RKTaskCreate(TASK_ID_MEDIA, 0, NULL);

        //hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, NOT_CARE);

        TotalSize = 0;

        while(!TotalSize)
        {
            ret = fifoDev_GetTotalSize(hFifo, &TotalSize);
            rk_printf("TotalSize = %d", TotalSize);
        }
        RKDev_Close(hFifo);
        memcpy((uint8 *)gpstAudioControlData->AudioFileInfo.path, (uint8 *)LongName, StrLenW(LongName)*2);

#if 1
        {
            temp = GetFileType((UINT16 *)LongName, (UINT8 *)AudioFileExtString);
            switch (temp)
            {
                case 1:     //mp3
                case 2:     //mp2
                case 3:     //mp1
                    printf("mp1 2 3 type\n");
                    break;

                case 4:     //wma
                    printf("wma type\n");
                    break;
                case 5:     //wav
                    printf("wav type\n");
                    break;
                case 6:     //ape
                    printf("ape type\n");
                    break;

                case 7:     //flac
                    printf("flac type\n");
                    break;
                case 8:     //AAC
                    printf("AAC type\n");
                    break;
                case 9:     //M4A
                    printf("M4A type\n");
                    break;
                case 11:    //MP4
                    printf("MP4 type\n");
                    break;
                case 12:    // 3GP
                    printf("3GP type\n");
                    break;
                case 10:    //OGG
                    printf("OGG type\n");
                    break;
                default:
                    printf("unknown type\n");
                    break;
            }
        }
#endif
    return RK_SUCCESS;
    }
    else
    {
        #if 1
        //stFileAttr.Path = L"C:\\";
        //stFileAttr.FileName = NULL;//L"woshicjhcjhcjh";

        //gpstAudioControlData->hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);
        if ((gpstAudioControlData->hFileDev == NULL) || (gpstAudioControlData->hFileDev == (HDC)RK_ERROR) || (gpstAudioControlData->hFileDev == (HDC)RK_PARA_ERR))
        {
            rk_print_string("next music file open failure");
            return RK_SUCCESS;
        }

        //gpstAudioControlData->hDir = FileDev_OpenDir(gpstAudioControlData->hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
        if ((rk_err_t)gpstAudioControlData->hDir <= 0)
        {
            rk_print_string("init dir open failure");
            RKDev_Close(gpstAudioControlData->hFileDev);
            return RK_SUCCESS;
        }

        printf("\ntask init dir and file as Follow:");

        if(0 == gpstAudioControlData->AudioFileInfo.TotalFileNum)
        {
            BufLen = 0;
            while(1)
            {
                rk_print_string("\n");

                if (FileDev_NextDir(gpstAudioControlData->hDir, 0, &stFileAttr) != RK_SUCCESS)
                {
                    break;
                }

                if (FileDev_GetFileName(gpstAudioControlData->hDir, LongName) != RK_SUCCESS)
                {
                    rk_print_string("\ndir get long name failure");
                    break;
                }

                BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
                Buf[BufLen] = ' ';
                BufLen++;
                BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
                Buf[BufLen] = ' ';
                BufLen++;

                memcpy(Buf + BufLen, "<DIR>", 5);

                BufLen += 5;

                Buf[BufLen] = ' ';
                BufLen++;
                BufLen += Unicode2Ascii(Buf + BufLen, LongName, 128);
                printf("Buf =%s \n" ,Buf);

                //UartDev_Write(gpstAudioControlData->hFileDev, Buf, BufLen, SYNC_MODE, NULL);

                gpstAudioControlData->AudioFileInfo.TotalFileNum++;
                BufLen = 0;

            }
            FileDev_CloseDir(gpstAudioControlData->hDir);
        }
        //stFileAttr.Path = L"C:\\";
        //stFileAttr.FileName = NULL;

        gpstAudioControlData->hDir = FileDev_OpenDir(gpstAudioControlData->hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
        if ((rk_err_t)gpstAudioControlData->hDir <= 0)
        {
            rk_print_string("init2 dir open failure");
            RKDev_Close(gpstAudioControlData->hFileDev);
            return RK_ERROR;
        }
        gpstAudioControlData->AudioFileInfo.TotalFiles = 0;

        BufLen = 0;
        while(1)
        {
            if (FileDev_NextFile(gpstAudioControlData->hDir, 0, NULL, &stFileAttr) != RK_SUCCESS)
            {
                break;
            }

            if (FileDev_GetFileName(gpstAudioControlData->hDir, LongName) != RK_SUCCESS)
            {
                rk_print_string("file get long name failure");
                break;
            }

            BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;
            BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;

            memcpy(Buf + BufLen, "<FILE>", 6);

            BufLen += 6;

            Buf[BufLen] = ' ';
            BufLen++;

            BufLen += Unicode2Ascii(Buf + BufLen, LongName, 128);

            printf("Buf =%s \n" ,Buf);
            //UartDev_Write(gpstAudioControlData->hFileDev, Buf, BufLen, SYNC_MODE, NULL);

            rk_print_string("\r\n");

            gpstAudioControlData->AudioFileInfo.TotalFiles++;
            BufLen = 0;
        }
        gpstAudioControlData->AudioFileInfo.AudioFileDirOrAll = gpstAudioControlData->AudioFileInfo.TotalFileNum+gpstAudioControlData->AudioFileInfo.TotalFiles;
        BufLen = sprintf(Buf, "   total dir %d, totoal file %d", gpstAudioControlData->AudioFileInfo.TotalFileNum, gpstAudioControlData->AudioFileInfo.TotalFiles);
        //UartDev_Write(gpstAudioControlData->hFileDev, Buf, BufLen, SYNC_MODE, NULL);
        printf("total Buf =%s \n" ,Buf);

        FileDev_CloseDir(gpstAudioControlData->hDir);
        RKDev_Close(gpstAudioControlData->hFileDev);

        printf("\n$@@@path:%s FileName:%s CurrentFileNum=%d\n",stFileAttr.Path,stFileAttr.FileName,gpstAudioControlData->AudioFileInfo.CurrentFileNum);
        //gpstAudioControlData->AudioFileInfo.CurrentFileNum = 0;//first init 0

        stFileAttr.Path = (uint16 *)&Buf;
        stFileAttr.FileName = (uint16 *)&LongName;

        stFileAttr.Path = L"C:\\";
        debug_hex((char * )stFileAttr.Path, StrLenW(stFileAttr.Path), 16);
        printf("/n DDDDDDDStrLenW(stFileAttr.Path)= %d\n",StrLenW(stFileAttr.Path));
        stFileAttr.FileName = NULL;
        ret = 1;
        i = 0;
        while(ret != 0)
        {
            if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
            {
                printf("\n!!!!NO Find APE Type Music i=%d totalfiles=%d\n", i, gpstAudioControlData->AudioFileInfo.TotalFiles);
                i = 0;
                break;
            }
            ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
            printf("ret = %i \n",ret);
        }
        gpstAudioControlData->pAudioRegKey->RepeatMode = AUDIO_FOLDER_ONCE;

        return RK_SUCCESS;
#endif


    }
}
*/
/*******************************************************************************
** Name: AudioStart
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:17:59
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API void AudioStart(void)
{
    int ret = 0;
    uint32 timeout = 200;
    AUDIO_DEV_I2S_CODEC_CONFIG_ARG stAudioDevArg;
#ifdef _BEEP_
    BeepStop();
#endif

    DEBUG("\n Audio Start \n");

    FREQ_EnterModule(FREQ_AUDIO_INIT);

    AudioVariableInit();

    if (RK_ERROR == AudioFileOpen())
    {
        FREQ_ExitModule(FREQ_AUDIO_INIT);
        DEBUG("Audio File Open Error");
        return;
    }
    printf("CurrentCodec =%d \n",CurrentCodec);

    AudioHWInit();

    DEBUG("!!@!@AudioCodecOpen\n");
    if (RK_ERROR == AudioCodecOpen())
    {
        FREQ_ExitModule(FREQ_AUDIO_INIT);
        AudioStop(Audio_Stop_Force);
        DEBUG("Codec Open Error1");
        return;
    }

    DEBUG("\n audio codec open success.\n ");

    FREQ_ExitModule(FREQ_AUDIO_INIT);

    AudioFREQInit();

#ifdef 0//_RK_EQ_
    EffectInit();
    AudioDev_SetEQ(gpstAudioControlData->hAudio, EQ_HEAVY, 1);
    EQ_ClearBuff();
#endif
    //AudioHoldonInit();
    if (gpstAudioControlData->pAudioRegKey->bps == (I2S_DATA_WIDTH16+1))
    {
        printf("\n 16 bit len =%d\n ",gpstAudioControlData->AudioLen * 4);
        AudioDev_SetTrackLen(gpstAudioControlData->hAudio, gpstAudioControlData->AudioLen*4);//gpstAudioControlData->pAudioRegKey->channels
    }
    else if(gpstAudioControlData->pAudioRegKey->bps == (I2S_DATA_WIDTH24+1))
    {
        printf("\n 24 bit len =%d\n ",gpstAudioControlData->AudioLen * 6);
        AudioDev_SetTrackLen(gpstAudioControlData->hAudio, gpstAudioControlData->AudioLen*6);//gpstAudioControlData->pAudioRegKey->channels
    }
    AudioDev_SetChannel(gpstAudioControlData->hAudio, gpstAudioControlData->TrackNo, gpstAudioControlData->pAudioRegKey->channels);
    printf("\n\n\n >>> bit bps =%d\n\n\n ",gpstAudioControlData->pAudioRegKey->bps);
    AudioDev_SetBit(gpstAudioControlData->hAudio, gpstAudioControlData->TrackNo, gpstAudioControlData->pAudioRegKey->bps);
#if 1
    stAudioDevArg.stI2sDevArg.i2smode = I2S_SLAVE_MODE;//;I2S_SLAVE_MODE
    stAudioDevArg.stI2sDevArg.i2sCS = I2S_IN; //I2S_EXT
    stAudioDevArg.stI2sDevArg.I2S_FS = gpstAudioControlData->pAudioRegKey->samplerate;
    stAudioDevArg.stI2sDevArg.BUS_FORMAT = I2S_FORMAT;//I2S_FORMAT;
#ifdef CODEC_24BIT //24bit
    stAudioDevArg.stI2sDevArg.Data_width = I2S_DATA_WIDTH24;
#else
    stAudioDevArg.stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
#endif
    //stAudioDevArg.stI2sDevArg.Data_width = gpstAudioControlData->pAudioRegKey->bps;

    stAudioDevArg.stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    stAudioDevArg.stCodecArg.SampleRate = gpstAudioControlData->pAudioRegKey->samplerate;
    stAudioDevArg.stCodecArg.Codecmode = Codec_DACoutHP;
    AudioDev_I2S_Acodec_Mode(gpstAudioControlData->hAudio, &stAudioDevArg);
    printf("\n Audiostart set par. ok\n");

#endif
    AudioDev_SetSampleRate(gpstAudioControlData->hAudio, gpstAudioControlData->TrackNo, gpstAudioControlData->pAudioRegKey->samplerate);
    AudioSetVolume(25);

#ifndef A_CORE_DECODE
    gpstAudioControlData->AudioNeedDecode = 0;
#endif
    gpstAudioControlData->AudioPlayState = AUDIO_STATE_PLAY;

    if(gpstAudioControlData->AudioPlayerState == AUDIO_STATE_PLAY)
    {
        gpstAudioControlData->AudioPlayState = AUDIO_STATE_PLAY;
    }
    else
    {
        gpstAudioControlData->AudioPlayState = AUDIO_STATE_PAUSE;
    }

    printf("==##CurrentFileNum=%d totalNum = %d\n ",gpstAudioControlData->AudioFileInfo.CurrentFileNum,gpstAudioControlData->AudioFileInfo.TotalFiles);
}

/*******************************************************************************
** Name: AudioStop
** Input:UINT16 ReqType
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:01:28
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API void AudioStop(UINT16 ReqType)
{

    printf("\n ####Audio Stop,ReqType = %d\n",ReqType);

    if ((AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState) && ((Audio_Stop_Normal != ReqType)))
    {
#ifdef _FADE_PROCESS_
        FadeInit(0,gpstAudioControlData->pAudioRegKey->samplerate/32,FADE_OUT);
        FadeFinishedWait(1000/32);
#endif
    }

    gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;


    AudioSetVolume(0);
    printf("\n AudioSetVolume 0 \n");

    AudioVariableDeInit();

    CloseTrack();
    printf("\n CloseTrack \n");

    AudioDeHWInit();
    printf("\n AudioDeHWInit \n");

    AudioFileClose();
    printf("\n AudioFileClose \n");

    AudioFREQDeInit();
    printf("\n AudioFREQDeInit \n");

    if(gpstAudioControlData->AudioFileInfo.ucSelPlayType == SOURRE_FROM_DLNA)
    {
        rk_printf("audio stop from dlna.......!");
        fifoDev_SetTotalSize(gpstAudioControlData->hFifo, 0);
        MplayerCtrl_Player_Stop();
    }
    else if (Audio_Stop_Normal == ReqType)
    {
        gpstAudioControlData->AudioStopMode = Audio_Stop_Normal;//by zs 0512
        AudioGetNextMusic(1);

        //gpstAudioControlData->AudioStopMode = Audio_Stop_Normal;//by zs 0512
        //AudioGetNextMusic(1);
        //MsgDev_SendMsg(gpstAudioControlData->hMsg, MSG_AUDIO_NEXTFILE);
        //SendMsg(MSG_AUDIO_NEXTFILE);
    }

    DEBUG("AudioStop over \n");

}

/*******************************************************************************
** Name: AudioNext
** Input:uint32 msg
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:17:38
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioUp(void)
{
    AudioStop(Audio_Stop_Force);
    gpstAudioControlData->AudioStopMode = Audio_Stop_Force;
    AudioGetNextMusic(-1);
}
/*******************************************************************************
** Name: AudioNext
** Input:uint32 msg
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:17:38
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioNext(void)
{
    AudioStop(Audio_Stop_Force);
    gpstAudioControlData->AudioStopMode = Audio_Stop_Force;
    AudioGetNextMusic(1);
}
/*******************************************************************************
** Name: AudioFFDStop
** Input:VOID
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:16:29
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API void AudioFFDStop(void)
{
    if (gpstAudioControlData->pAudioRegKey->CurrentTime >= gpstAudioControlData->pAudioRegKey->TotalTime)
    {
        gpstAudioControlData->pAudioRegKey->CurrentTime = gpstAudioControlData->pAudioRegKey->TotalTime;
        //FileInfo[(uint32)pRawFileCache].Offset = FileInfo[(uint32)pRawFileCache].FileSize;
    }

    CodecSeek(gpstAudioControlData->pAudioRegKey->CurrentTime, 0);
}
/*******************************************************************************
** Name: AudioFFD
** Input:unsigned long StepLen
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:15:42
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioFFD(uint32 StepLen)
{
    uint32 timeout = 200;
    DEBUG("AudioFFD StepLen=%d",StepLen);
    if (AUDIO_STATE_STOP == gpstAudioControlData->AudioPlayerState)
    {
        return TRUE;
    }

    if (AUDIO_STATE_FFD != gpstAudioControlData->AudioPlayerState)  //wait for mute
    {
        if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState)
        {
#ifdef _FADE_PROCESS_
            FadeInit(0,gpstAudioControlData->pAudioRegKey->samplerate/4,FADE_OUT);
            FadeFinishedWait(1000/4);
#endif
        }

        gpstAudioControlData->AudioPlayerState = AUDIO_STATE_FFD; // audio player state
        gpstAudioControlData->AudioPlayState   = AUDIO_STATE_STOP; // play state

        //PAGE
        {
            while (DmaGetState(AUDIO_DMACHANNEL_IIS) == DMA_BUSY)
            {
                DelayMs(1);
                if (--timeout == 0)
                {
                    break;
                }
            }

            //gpstAudioControlData->AudioPlayInfo.VolumeCnt = 0;
            //AudioDev_SetVol(gpstAudioControlData->hAudio, gpstAudioControlData->AudioPlayInfo.VolumeCnt);
            //Codec_SetVolumet(gpstAudioControlData->AudioPlayInfo.VolumeCnt);
        }

    }
    else //if (AUDIO_STATE_FFD == AudioPlayState)   //FFD
    {
        gpstAudioControlData->pAudioRegKey->CurrentTime = (gpstAudioControlData->pAudioRegKey->CurrentTime < gpstAudioControlData->pAudioRegKey->TotalTime - StepLen)?\
                                                          (gpstAudioControlData->pAudioRegKey->CurrentTime + StepLen) : gpstAudioControlData->pAudioRegKey->TotalTime;

        gpstAudioControlData->gAudioPlayTime = gpstAudioControlData->pAudioRegKey->CurrentTime;
    }

    return TRUE;
}
/*******************************************************************************
** Name: AudioFFWStop
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.7.22
** Time: 16:15:00
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API void AudioFFWStop(void)
{
    CodecSeek(gpstAudioControlData->pAudioRegKey->CurrentTime, 0);
}
/*******************************************************************************
** Name: AudioFFW
** Input:unsigned long StepLen
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:14:17
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioFFW(uint32 StepLen)
{
    uint32 timeout = 200;

    DEBUG("AudioFFW StepLen=%d",StepLen);
    if (AUDIO_STATE_STOP == gpstAudioControlData->AudioPlayerState)
    {
        return TRUE;
    }

    if (AUDIO_STATE_FFW != gpstAudioControlData->AudioPlayerState)  //wait for mute
    {
        if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState)
        {
#ifdef _FADE_PROCESS_
            FadeInit(0,gpstAudioControlData->pAudioRegKey->samplerate/4,FADE_OUT);
            FadeFinishedWait(1000/4);
#endif
        }

        gpstAudioControlData->AudioPlayerState = AUDIO_STATE_FFW;
        gpstAudioControlData->AudioPlayState   = AUDIO_STATE_STOP;

        {
            while (DmaGetState(AUDIO_DMACHANNEL_IIS) == DMA_BUSY)
            {
                DelayMs(1);
                if (--timeout == 0)
                {
                    break;
                }
            }
            //gpstAudioControlData->AudioPlayInfo.VolumeCnt = 0;
            //AudioDev_SetVol(gpstAudioControlData->hAudio, gpstAudioControlData->AudioPlayInfo.VolumeCnt);
            //Codec_SetVolumet(gpstAudioControlData->AudioPlayInfo.VolumeCnt);
        }

        printf("!!!CurrentTime=%dm:%ds %d ms",gpstAudioControlData->pAudioRegKey->CurrentTime/60000, gpstAudioControlData->pAudioRegKey->CurrentTime/1000%60, gpstAudioControlData->pAudioRegKey->CurrentTime%1000);
    }
    else //if (AUDIO_STATE_FFW == AudioPlayState)   //FFW
    {
        gpstAudioControlData->pAudioRegKey->CurrentTime = (gpstAudioControlData->pAudioRegKey->CurrentTime > StepLen) ? (gpstAudioControlData->pAudioRegKey->CurrentTime - StepLen) : 0;
        gpstAudioControlData->gAudioPlayTime = gpstAudioControlData->pAudioRegKey->CurrentTime;
        printf("!@!CurrentTime=%dm:%ds %d ms",gpstAudioControlData->pAudioRegKey->CurrentTime/60000, gpstAudioControlData->pAudioRegKey->CurrentTime/1000%60, gpstAudioControlData->pAudioRegKey->CurrentTime%1000);
    }

    return TRUE;
}
/*******************************************************************************
** Name: AudioFFResume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:13:45
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioFFResume(void)
{
    CodecSeek(gpstAudioControlData->pAudioRegKey->CurrentTime, 0);


    if((gpstAudioControlData->AudioPlayerState == AUDIO_STATE_FFD) || (gpstAudioControlData->AudioPlayerState == AUDIO_STATE_FFW))
    {
        rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
    }

    //CodecDecode();

    //CodecGetCaptureBuffer((short*)&(gpstAudioControlData->AudioPtr),&(gpstAudioControlData->AudioLen));

    //DmaTransting = 1;
    //DmaStart(AUDIO_DMACHANNEL_IIS, (UINT32)AudioPtr, (uint32)(&(I2s_Reg->I2S_TXDR)), gpstAudioControlData->AudioLen, &AudioControlDmaCfg, AudioDmaIsrHandler);

    //AudioDev_Write(gpstAudioControlData->hAudio, 0, (uint8 *)gpstAudioControlData->AudioPtr);

    gpstAudioControlData->AudioPlayState = AUDIO_STATE_PLAY;
    gpstAudioControlData->AudioPlayerState = AUDIO_STATE_PLAY;
    //AudioDecodeing = 1;
    //AudioDev_SetVol(gpstAudioControlData->hAudio, gpstAudioControlData->playVolume);
    //Codec_SetVolumet(gpstAudioControlData->AudioPlayInfo.playVolume);

    //UserIsrRequest();

    //I2SStart(I2S_CH0,I2S_START_DMA_TX);
    return TRUE;
}
/*******************************************************************************
** Name: AudioResume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:13:21
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioResume(void)
{
    DEBUG("=== AudioResume in ===  ");
    if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayerState)
    {
        return RK_SUCCESS;
    }

    if (gpstAudioControlData->AudioPlayerState == AUDIO_STATE_PAUSE)
    {
        //AutoPowerOffDisable();
    }

    DelayMs(40);
    if (gpstAudioControlData->AudioCodecOpenErr)
    {
        gpstAudioControlData->AudioPlayerState = AUDIO_STATE_PLAY;
        gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;
        MsgDev_SendMsg(gpstAudioControlData->hMsg, MSG_AUDIO_FILE_ERROR);
        //SendMsg(MSG_AUDIO_FILE_ERROR);
        return TRUE;
    }

    gpstAudioControlData->AudioErrorFrameNum = 0;
    //MsgDev_SendMsg(gpstAudioControlData->hMsg, MSG_MUSIC_DISPFLAG_STATUS);
    //SendMsg(MSG_MUSIC_DISPFLAG_STATUS);//cw 2009-5-11

    //CodecGetCaptureBuffer((short*)&(gpstAudioControlData->AudioPtr),&(gpstAudioControlData->AudioLen));
    //memset((uint8*)(gpstAudioControlData->AudioPtr), 0, (gpstAudioControlData->AudioLen) * sizeof(int16) * 2);

    //DmaTransting = 1;
    //DmaStart(AUDIO_DMACHANNEL_IIS, (UINT32)gpstAudioControlData->AudioPtr, (uint32)(&(I2s_Reg->I2S_TXDR)),  gpstAudioControlData->AudioLen, &AudioControlDmaCfg, AudioDmaIsrHandler);

    //AudioDev_Write(gpstAudioControlData->hAudio, 0, (uint8 *)gpstAudioControlData->AudioPtr);

    gpstAudioControlData->AudioPlayerState = AUDIO_STATE_PLAY;
    gpstAudioControlData->AudioPlayState   = AUDIO_STATE_PLAY;
    //ClearMsg(MSG_MUSIC_FADE_OK);

    //AudioDecodeing = 1;
    //UserIsrRequest();

    //I2SStart(I2S_CH0,I2S_START_DMA_TX);
    DEBUG("=== AudioResume out ===");

    return TRUE;
}
/*******************************************************************************
** Name: AudioFFPause
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:12:26
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioFFPause(void)
{
    uint32 timeout = 20000;//20000;
    if (gpstAudioControlData->AudioPlayState != AUDIO_STATE_STOP)
    {
        gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;


        while (DmaGetState(AUDIO_DMACHANNEL_IIS) == DMA_BUSY)
        {
            DelayMs(1);
            if (--timeout == 0)
            {
                DEBUG("dma busy out");
                break;
            }
        }

        AudioDev_SetVol(gpstAudioControlData->hAudio, 0);
        //Codec_SetVolumet(0);

        //AudioDecodeing = 0;
        //DmaTransting = 0;

    }

    DelayMs(100);

    return TRUE;
}
/*******************************************************************************
** Name: AudioPause
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 16:09:19
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioPause(void)
{
    uint32 timeout = 20000;//20000;
    DEBUG("=== AudioPause in ===");

    if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayerState)
    {
#ifdef _FADE_PROCESS_
        FadeInit(0,gpstAudioControlData->pAudioRegKey->samplerate/32,FADE_OUT);
        FadeFinishedWait(1000/32);
#endif

        gpstAudioControlData->AudioPlayerState = AUDIO_STATE_PAUSE;
        gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;

        {
            while (DmaGetState(AUDIO_DMACHANNEL_IIS) == DMA_BUSY)
            {
                DelayMs(1);
                if (--timeout == 0)
                {
                    DEBUG("dma busy out");
                    break;
                }
            }
            //gpstAudioControlData->AudioPlayInfo.VolumeCnt = 0;
            //AudioDev_SetVol(gpstAudioControlData->hAudio, gpstAudioControlData->AudioPlayInfo.VolumeCnt);
            //Codec_SetVolumet(gpstAudioControlData->AudioPlayInfo.VolumeCnt);
        }


        //if (CheckMsg(MSG_MUSIC_FADE_IN))
        //{
            //ClearMsg(MSG_MUSIC_FADE_IN);
        //}

        //AutoPowerOffEnable();
    }

    DEBUG("=== AudioPause out ===");
    return TRUE;
}
/*******************************************************************************
** Name: AudioGetNextMusic
** Input:UINT32 msg
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.22
** Time: 14:55:51
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioGetNextMusic(int msg)
{
    int ret = 0;
    uint32  Playednumber;

    uint32 BufLen,i;
    uint16 LongName[64];
    uint8 Buf[128];
    FILE_ATTR stFileAttr;
    uint32 PathLen;
    uint16 *pPath;

    i = 0;

    PathLen = StrLenW(gpstAudioControlData->AudioFileInfo.path);
    pPath = gpstAudioControlData->AudioFileInfo.path;
    pPath += PathLen;
    printf("\n first pre Path PathLen =%d \n ",PathLen);

    while(*pPath != '\\')
    {
        PathLen--;
        pPath--;
    }
    //gpstAudioControlData->AudioFileInfo.path[PathLen] = 0;

    stFileAttr.Path = gpstAudioControlData->AudioFileInfo.path;

    gpstAudioControlData->AudioFileInfo.path[PathLen+1] = 0x0000;
    //stFileAttr.Path[PathLen+1] = 0x0000;
    stFileAttr.FileName = NULL;

#if 1
    switch (gpstAudioControlData->pAudioRegKey->RepeatMode)
    {
        case AUDIO_REPEAT://single repeat
        case AUDIO_REPEAT1:
            if (Audio_Stop_Force == gpstAudioControlData->AudioStopMode)
            {
                while(!ret)
                {
                    if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                    {
                        printf("NO Find APE Type Music\n ");
                        i = 0;
                        break;
                    }
                    ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
                }
            }
            break;

        case AUDIO_TRY: //ok lyrics
        case AUDIO_ALLONCE://only once all song play cycle.
            Playednumber = gpstAudioControlData->AudioFileInfo.CurrentFileNum;
            if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
            {
                Playednumber = gpstAudioControlData->AudioPlayFileNum;
            }

            if ((Playednumber >= gpstAudioControlData->AudioFileInfo.TotalFiles )&&(gpstAudioControlData->AudioStopMode != Audio_Stop_Force))
            {
                //SysFindFile(&gpstAudioControlData->AudioFileInfo,(INT16)msg);
                if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
                {
                    gpstAudioControlData->AudioPlayFileNum = 1;
                }
                AudioStart();

                //SCH
                CodecSeek(0, 0);
                if (gpstAudioControlData->pAudioRegKey->CurrentTime >= gpstAudioControlData->pAudioRegKey->TotalTime)
                {
                    gpstAudioControlData->pAudioRegKey->CurrentTime = gpstAudioControlData->pAudioRegKey->TotalTime;
                }

                AudioPause();

                return TRUE;
            }
            else
            {
                while(!ret)
                {
                    if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                    {
                        printf("NO Find APE Type Music\n ");
                        i = 0;
                        break;
                    }
                    ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
                }

                if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
                {
                    gpstAudioControlData->AudioPlayFileNum += msg;
                    if (gpstAudioControlData->AudioPlayFileNum < 1)
                    {
                        gpstAudioControlData->AudioPlayFileNum = gpstAudioControlData->AudioFileInfo.TotalFiles;
                    }
                }
            }
            break;

        case AUDIO_ALLREPEAT://all cycle play
            gpstAudioControlData->AudioFileInfo.Range = (INT16)FIND_FILE_RANGE_ALL;
            while(!ret)
            {
                if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                {
                    printf("NO Find APE Type Music\n ");
                    i = 0;
                    break;
                }
                ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
            }
            break;

        case AUDIO_ONCE: //single once.
            if (Audio_Stop_Force == gpstAudioControlData->AudioStopMode)
            {
                while(!ret)
                {
                    if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                    {
                        printf("NO Find APE Type Music\n ");
                        i = 0;
                        break;
                    }
                    ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
                }
                break;
            }
            else
            {
                AudioStart();
                //SCH
                CodecSeek(0, 0);
                if (gpstAudioControlData->pAudioRegKey->CurrentTime >= gpstAudioControlData->pAudioRegKey->TotalTime)
                {
                    gpstAudioControlData->pAudioRegKey->CurrentTime = gpstAudioControlData->pAudioRegKey->TotalTime;
                    //FileInfo[(uint32)pRawFileCache].Offset = FileInfo[(uint32)pRawFileCache].FileSize;
                }
                AudioPause();
                return TRUE;
            }
            break;

        case AUDIO_FOLDER_ONCE://directory once.
            gpstAudioControlData->AudioFileInfo.Range = (INT16)FIND_FILE_RANGE_DIR;

            Playednumber = gpstAudioControlData->AudioFileInfo.CurrentFileNum;
            if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
            {
                Playednumber = gpstAudioControlData->AudioPlayFileNum;
            }

            if ((Playednumber >= gpstAudioControlData->AudioFileInfo.TotalFiles)&&(gpstAudioControlData->AudioStopMode != Audio_Stop_Force))
            {
                ret = RK_ERROR;

                while(ret != RK_SUCCESS)
                {
                    if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                    {
                        printf("NO Find APE Type Music\n ");
                        i = 0;
                        break;
                    }
                    ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum + msg,  gpstAudioControlData->pAudioRegKey);
                }

                if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
                {
                    gpstAudioControlData->AudioPlayFileNum = 1;
                }

                AudioStart();
                //SCH
                CodecSeek(0, 0);
                if (gpstAudioControlData->pAudioRegKey->CurrentTime >= gpstAudioControlData->pAudioRegKey->TotalTime)
                {
                    gpstAudioControlData->pAudioRegKey->CurrentTime = gpstAudioControlData->pAudioRegKey->TotalTime;
                    //FileInfo[(uint32)pRawFileCache].Offset = FileInfo[(uint32)pRawFileCache].FileSize;
                }
                AudioPause();

                return TRUE;
            }
            else
            {
                rk_printf("next music");


                ret = RK_ERROR;

                while(ret != RK_SUCCESS)
                {
                    if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                    {
                        printf("NO Find APE Type Music\n ");
                        i = 0;
                        break;
                    }
                    ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum + msg, gpstAudioControlData->pAudioRegKey);
                }

                if (gpstAudioControlData->pAudioRegKey->PlayOrder == AUDIO_RAND)
                {
                    gpstAudioControlData->AudioPlayFileNum += msg;
                    if (gpstAudioControlData->AudioPlayFileNum < 1)
                    {
                        gpstAudioControlData->AudioPlayFileNum = gpstAudioControlData->AudioFileInfo.TotalFiles;
                    }
                }
            }
            break;

        case AUIDO_FOLDER_REPEAT://directory cycle.
            gpstAudioControlData->AudioFileInfo.Range = (INT16)FIND_FILE_RANGE_DIR;
            while(!ret)
            {
                if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
                {
                    printf("NO Find APE Type Music\n ");
                    i = 0;
                    break;
                }
                ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
            }
            break;

        default:
            ret = FALSE;
            break;
    }
#endif

    AudioStart();

    return ret;
}

/*******************************************************************************
** Name: AudioControlTask_SendCmd
** Input:uint32 Cmd
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.26
** Time: 16:19:05
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API rk_err_t AudioControlTask_SendCmd(uint32 Cmd, void * msg, uint32 Mode)
{
    AUDIOCONTROL_ASK_QUEUE AudioControlAskQueue;
    AUDIOCONTROL_RESP_QUEUE AudioControlRespQueue;

    while(RKTaskFind(TASK_ID_AUDIOCONTROL, 0) == NULL)
    {
        rkos_delay(10);
    }

    AudioControlAskQueue.cmd = Cmd;
    AudioControlAskQueue.msg = msg;
    AudioControlAskQueue.mode = Mode;

    rkos_queue_send(gpstAudioControlData->AudioControlAskQueue, &AudioControlAskQueue, MAX_DELAY);
    printf("\n cjh send Cmd=%d msg=%d\n", AudioControlAskQueue.cmd,AudioControlAskQueue.msg);

    if(Mode == SYNC_MODE)
    {
        rkos_queue_receive(gpstAudioControlData->AudioControlRespQueue, &AudioControlRespQueue, MAX_DELAY);
        if ((AudioControlRespQueue.cmd == Cmd) && AudioControlRespQueue.status == RK_SUCCESS)
        {
            printf("cmd == Cmd status == RK_SUCCESS\n");
            return RK_SUCCESS;
        }
        else
        {
            printf("cmd = %d  status ==%d\n",AudioControlRespQueue.cmd,AudioControlRespQueue.status);
            return RK_ERROR;
        }
    }
    else
    {
        return RK_SUCCESS;
    }

}



/*******************************************************************************
** Name: AudioControlTask_Enter
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 10:21:46
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON API void AudioControlTask_Enter(void)
{
    short *pBuffer1, * pBuffer2, * pBuffer3;
    int  i,j,k = 0;
    int32 DecodeErr;
    uint32 readlen, totalRead;
    int32 DataTemp;

    AUDIOCONTROL_ASK_QUEUE AudioControlAskQue;
    AUDIOCONTROL_RESP_QUEUE AudioControlResQue;

    rk_printf("audio control task enter...");

    gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;

    if(gpstAudioControlData->AudioFileInfo.ucSelPlayType != SOURRE_FROM_DLNA)
    {
        AudioControlResQue.cmd = AUDIO_CMD_DECSTART;
        AudioDecodeProc(AudioControlResQue.cmd , NULL);
    }

replay:
    while(1)
    {
        if (AUDIO_STATE_STOP == gpstAudioControlData->AudioPlayState)
        {
            rk_printf("task_enter state\n",gpstAudioControlData->AudioPlayState);
            if(rkos_queue_receive(gpstAudioControlData->AudioControlAskQueue, &AudioControlAskQue, MAX_DELAY) == RK_SUCCESS)
            {
                printf("#$#$cmd=%d \n",AudioControlAskQue.cmd);
                AudioDecodeProc(AudioControlAskQue.cmd, AudioControlAskQue.msg);
                AudioControlResQue.cmd = AudioControlAskQue.cmd;
                AudioControlResQue.status = RK_SUCCESS;
                printf("\n cmd=%d msg=%d \n",AudioControlAskQue.cmd,AudioControlAskQue.msg);
                if(AudioControlAskQue.mode == SYNC_MODE)
                {
                    rkos_queue_send(gpstAudioControlData->AudioControlRespQueue, &AudioControlResQue, MAX_DELAY);
                }
                printf("\n AudioPlayState =%d\n",gpstAudioControlData->AudioPlayState);

            }
        }
        else
        {
            if(rkos_queue_receive(gpstAudioControlData->AudioControlAskQueue, &AudioControlAskQue, 0) == RK_SUCCESS)
            {
                rk_printf("222rkos_queue_receive cmd %d\n",AudioControlAskQue.cmd);
                AudioDecodeProc(AudioControlAskQue.cmd, AudioControlAskQue.msg);
                AudioControlResQue.cmd = AudioControlAskQue.cmd;
                AudioControlResQue.status = RK_SUCCESS;
                printf("\n cmd=%d msg=%d \n",AudioControlAskQue.cmd,AudioControlAskQue.msg);
                if(AudioControlAskQue.mode == SYNC_MODE)
                {
                    rkos_queue_send(gpstAudioControlData->AudioControlRespQueue, &AudioControlResQue, MAX_DELAY);
                }
                printf("\n rkos_queue_send AudioPlayState =%d\n",gpstAudioControlData->AudioPlayState);

            }
        }

        if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState)
        {
            DecodeErr = CodecGetCaptureBuffer((short*)&gpstAudioControlData->AudioPtr, &gpstAudioControlData->AudioLen);

            if (0 == DecodeErr)   // Decoding end or error
            {
                if (gpstAudioControlData->StreamEnd == 0)
                {
                    printf("\n------1 file error------ ");
                    gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;
                    memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 4);
                    //SendMsg(MSG_AUDIO_FILE_ERROR);
                }
                else
                {
                    printf("\n------2 decode end------ ");
                    gpstAudioControlData->AudioPlayState = AUDIO_STATE_STOP;
                    memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 4);
                    //SendMsg(MSG_AUDIO_DECODE_END);
                }

                AudioDecodeProc(AUDIO_CMD_STOP, Audio_Stop_Normal);
                goto replay;
            }

            CodecGetTime(&gpstAudioControlData->pAudioRegKey->CurrentTime);
            //printf("\currenter timer = %d",gpstAudioControlData->pAudioRegKey->CurrentTime);

#ifdef DC_FILTER
            if ((gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_44100Hz)
                && (gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_22050Hz)
                && (gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_11025Hz))
            {
                gpstAudioControlData->AudioPtr = DC1_Filter((short*)gpstAudioControlData->AudioPtr, (int *)&gpstAudioControlData->AudioLen);
            }
#endif

            // fade in init
            if (gpstAudioControlData->AudioErrorFrameNum < 2)
            {
                if (++gpstAudioControlData->AudioErrorFrameNum >= 2)
                {
                    AudioDev_SetVol(gpstAudioControlData->hAudio, gpstAudioControlData->playVolume);
                    printf("here player=%d playstate=%d\n",gpstAudioControlData->AudioPlayerState,gpstAudioControlData->AudioPlayState);
            #ifdef _FADE_PROCESS_
                    FadeInit(0,gpstAudioControlData->samplerate/2,FADE_IN);
                    gpstAudioControlData->AudioFadeInStart = 1;
            #else
                    gpstAudioControlData->AudioFadeInStart = 1;
            #endif

                    if(gpstAudioControlData->hFifo != NULL)
                    {
                        MplayerCtrl_Player_Play();
                    }
                }
            }

            //Bit_Convertor_DEC((short*)gpstAudioControlData->AudioPtr, &gpstAudioControlData->AudioLen,  gpstAudioControlData->pAudioRegKey->bps);

            if(gpstAudioControlData->AudioFadeInStart == 0)
            {

                #ifdef CODEC_24BIT
                memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 8);
                #else
                memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 4);
                #endif
            }

            #ifdef _FADE_PROCESS_
            // last 500ms for Fadeout
            if ((gpstAudioControlData->pAudioRegKey->TotalTime - gpstAudioControlData->pAudioRegKey->CurrentTime) <= 500)
            {
                if (FadeIsFinished())
                {
                    if (gpstAudioControlData->AudioEndFade == 0)
                    {
                        FadeInit(0, gpstAudioControlData->pAudioRegKey->samplerate/4,FADE_OUT);
                        gpstAudioControlData->AudioEndFade = 1;
                    }
                }
            }

            if (gpstAudioControlData->AudioEndFade == 1)
            {
                if (FadeIsFinished())
                {
                    #ifdef CODEC_24BIT
                    memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 8);
                    #else
                    memset((uint8*)gpstAudioControlData->AudioPtr, 0x00, gpstAudioControlData->AudioLen * 4);
                    #endif
                }
            }
            #endif

            #ifdef _FADE_PROCESS_
            if (!FadeIsFinished())
            {
                #ifdef CODEC_24BIT
                FadeProcess((long*)gpstAudioControlData->AudioPtr,gpstAudioControlData->AudioLen);
                #else
                FadeProcess((short*)gpstAudioControlData->AudioPtr,gpstAudioControlData->AudioLen);
                #endif
            }
            #endif

            #ifdef _RK_SPECTRUM_
            if (gpstAudioControlData->SpectrumEn)
            {
                short Spectrum_data[128];

                if (gpstAudioControlData->SpectrumLoop == 0)
                {
                    if (gpstAudioControlData->SpectrumCnt == 0)
                        memset(gpstAudioControlData->SpectrumOut, 0, SPECTRUM_LINE_M*sizeof(char));


                    if ((AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState) && (gpstAudioControlData->SpectrumCnt < 3))
                    {
                     #ifdef CODEC_24BIT
                        {
                           int i = 0;
                           long *ptr = (long*)gpstAudioControlData->AudioPtr;
                           for(i=0;i<128;i++)
                            {
                              Spectrum_data[i] = (short)((*ptr) >>8);
                              ptr++;
                            }
                        }
                    #else
                         memcpy(Spectrum_data, (short*)gpstAudioControlData->AudioPtr, 256);
                    #endif
                        DoSpectrum((short*)Spectrum_data,&gpstAudioControlData->SpectrumOut[0]);

                        gpstAudioControlData->SpectrumCnt++;
                        if (gpstAudioControlData->SpectrumCnt >= 3)
                        {
                            gpstAudioControlData->SpectrumLoop = SpectrumLoopTime;
                            //SendMsg(MUSIC_UPDATESPECTRUM);
                            gpstAudioControlData->SpectrumCnt = 0;
                        }
                    }
                }
                if (gpstAudioControlData->SpectrumLoop > 0)
                    gpstAudioControlData->SpectrumLoop-- ;
            }
            #endif
            //printf("x");
            AudioDev_Write(gpstAudioControlData->hAudio, 0, (uint8 *)gpstAudioControlData->AudioPtr);
            //printf("y");
        }
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
** Name: GetFileInfo
** Input:uint16 *path, uint32 fileNum, TRACK_INFO* tackInfo, uint16* longName
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 8:16:13
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN rk_err_t GetFileInfoSetNewMusic(FILE_ATTR *fileAttr, int32 fileNum, TRACK_INFO* tackInfo)
{
    FILE_ATTR stFileAttr;
    uint16 LongName[64] = 0;
    uint8 Buf[128] = 0;
    uint32 BufLen;
    UINT8 temp;

    stFileAttr.Path = fileAttr->Path;
    stFileAttr.FileName = fileAttr->FileName;

    debug_hex((char *)stFileAttr.Path, 50, 6);

    //gpstAudioControlData->hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);
    if ((gpstAudioControlData->hFileDev == NULL) || (gpstAudioControlData->hFileDev == (HDC)RK_ERROR) || (gpstAudioControlData->hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("^^^next music file open failure");
        return RK_ERROR;
    }

    printf("^^^c FileDev_OpenDir\n");
    gpstAudioControlData->hDir = FileDev_OpenDir(gpstAudioControlData->hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
    if ((rk_err_t)gpstAudioControlData->hDir <= 0)
    {
        printf("^^^new dir open failure");
        RKDev_Close(gpstAudioControlData->hFileDev);
        return RK_ERROR;
    }
    printf("^^^new dir open success");

    BufLen = 0;
    {
        if(fileNum < 1) //1 --- gpstAudioControlData->AudioFileInfo.TotalFiles
        {
           fileNum = gpstAudioControlData->AudioFileInfo.TotalFiles;
           gpstAudioControlData->AudioFileInfo.CurrentFileNum= gpstAudioControlData->AudioFileInfo.TotalFiles;

           printf("<1 fileNum = %d\n",fileNum);
        }
        else if(fileNum > gpstAudioControlData->AudioFileInfo.TotalFiles)
        {
           fileNum = 1;
           gpstAudioControlData->AudioFileInfo.CurrentFileNum = 1;
           printf(">Total fileNum = %d\n",fileNum);
        }
        else
        {
           gpstAudioControlData->AudioFileInfo.CurrentFileNum = fileNum;
        }
        printf("###filenum = %d\n",fileNum);

        while(fileNum--)
        {
            rk_printf("filenum = %d", fileNum);

            if (FileDev_NextFile(gpstAudioControlData->hDir, 0, NULL, &stFileAttr) != RK_SUCCESS)
            {
                printf("Faile : FileDev_NextFile\n");
                break;
            }
        }
        printf("###FileDev_NextFile ok\n");
        if (FileDev_GetFileName(gpstAudioControlData->hDir, LongName) != RK_SUCCESS)
        {
            rk_print_string("file get long name failure\n");
        }
        printf("###FileDev_GetFileName ok\n");
        //set new music
        {
            temp = GetFileType((UINT16 *)LongName, (UINT8 *)AudioFileExtString);
            switch (temp)
            {
                case 1:     //mp3
                case 2:     //mp2
                case 3:     //mp1
                    printf("mp1 2 3 type\n");
                    break;

                case 4:     //wma
                    printf("wma type\n");
                    break;
                case 5:     //wav
                    printf("wav type\n");
                    break;
                case 6:     //ape
                    printf("ape type\n");
                    break;

                case 7:     //flac
                    printf("flac type\n");
                    break;
                case 8:     //AAC
                    printf("AAC type\n");
                    break;
                case 9:     //M4A
                    printf("M4A type\n");
                    break;
                case 11:    //MP4
                    printf("MP4 type\n");
                    break;
                case 12:    // 3GP
                    printf("3GP type\n");
                    break;
                case 10:    //OGG
                    printf("OGG type\n");
                    break;
                default:
                    FileDev_CloseDir(gpstAudioControlData->hDir);
                    RKDev_Close(gpstAudioControlData->hFileDev);
                    return RK_ERROR;
                    printf("unknown type\n");
                    break;
            }
        }
        printf("\n$#$#$#$# temp=%d\n", temp);
        if((temp < 10) || (temp >1))
        {

            BufLen = 0;
            memcpy(Buf, (uint8 *)stFileAttr.Path, StrLenW(stFileAttr.Path)*2);
            //printf("\n$#$#$#$# Path copy len:%d strlen(longName)=%d, StrLenW(buf)=%d\n",StrLenW(stFileAttr.Path)*2,StrLenW(LongName)*2,StrLenW((uint16 *)Buf)*2);
            //debug_hex((char *)Buf, StrLenW((uint16 *)Buf)*2, 6);

            BufLen += StrLenW(stFileAttr.Path)*2;
            //memcpy(Buf+BufLen , (uint8 *)LongName, 100);
            memcpy(Buf + BufLen, LongName, StrLenW(LongName)*2);
            //BufLen += Unicode2Ascii(Buf + BufLen, LongName, 128);

            BufLen += StrLenW(LongName)*2;
            //printf("\n$#$#$#$# LongName copy BufLen:%d\n",BufLen);
            //debug_hex((char *)Buf, StrLenW((uint16 *)Buf)*2, 6);
            //printf("\n$#$#$#$# LongName:\n");
            //debug_hex((char *)LongName, StrLenW(LongName)*2, 6);
            *(Buf + BufLen) = 0x00;
            BufLen++;
            *(Buf + BufLen) = 0x00;
            BufLen++;
            memcpy((uint8 *)gpstAudioControlData->AudioFileInfo.path, Buf, BufLen);

            //printf("path+name copy len:%d sizeof:%d\n",StrLenW(LongName)*2,StrLenW((uint16 *)Buf)*2);
            //debug_hex((char *)gpstAudioControlData->AudioFileInfo.path, StrLenW((uint16 *)Buf)*2, 6);

            memcpy((uint8 *)gpstAudioControlData->MusicLongFileName, (uint8 *)LongName, StrLenW(LongName)*2);

            //printf("\n ^^^ new song file path:\n");
            //debug_hex((char *)gpstAudioControlData->AudioFileInfo.path, 50, 6);


            //printf("\n$#$#$#$# LongName:\n");
            //debug_hex((char *)LongName, 50, 6);

            //printf("\n ^^^ new song file name:\n");
            //debug_hex((char *)gpstAudioControlData->MusicLongFileName, 50, 6);

            //memcpy(gpstAudioControlData->AudioFileInfo.path, L"C:\\月亮之上.APE", 100);
            //rkos_exit_critical();//1
            BufLen = 0;
            printf("change music over\n");
            FileDev_CloseDir(gpstAudioControlData->hDir);
            RKDev_Close(gpstAudioControlData->hFileDev);

            return RK_SUCCESS;
        }

        //rk_print_string("\r\n");
        BufLen = 0;
    }

    FileDev_CloseDir(gpstAudioControlData->hDir);
    RKDev_Close(gpstAudioControlData->hFileDev);

    return RK_ERROR;
}
/*******************************************************************************
** Name: AudioFREQDeInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:45:55
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioFREQDeInit(void)
{
    switch (CurrentCodec)
    {
#ifdef MP3_DEC_INCLUDE
        case (CODEC_MP3_DEC):
            {
                if ((gpstAudioControlData->pAudioRegKey->bitrate <= 128000)&&(gpstAudioControlData->pAudioRegKey->samplerate <= 44100))
                {
                    FREQ_ExitModule(FREQ_MP3);
                }
                else
                {
                    FREQ_ExitModule(FREQ_MP3H);
                }
                break;
            }
#endif

#ifdef WMA_DEC_INCLUDE
        case (CODEC_WMA_DEC):
            {
                if (gpstAudioControlData->pAudioRegKey->bitrate <= 129000)
                {
                    if ((gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_32KHz) &&(gpstAudioControlData->pAudioRegKey->bitrate/1000 == 22))
                    {
                        FREQ_ExitModule(FREQ_WMAH);
                    }
                    else
                        FREQ_ExitModule(FREQ_WMA);
                }
                else
                {
                    FREQ_ExitModule(FREQ_WMAH);
                }
                break;
            }
#endif

#ifdef AAC_DEC_INCLUDE
        case (CODEC_AAC_DEC):
            {
                FREQ_ExitModule(FREQ_AAC);
                break;
            }
#endif

#ifdef WAV_DEC_INCLUDE
        case (CODEC_WAV_DEC):
            {
                FREQ_ExitModule(FREQ_WAV);
                break;
            }
#endif

#ifdef APE_DEC_INCLUDE
        case (CODEC_APE_DEC):
            {
                if (gpstAudioControlData->pAudioRegKey->samplerate <= CodecFS_48KHz)
                    FREQ_ExitModule(FREQ_APE);
                else
                    FREQ_ExitModule(FREQ_HAPE);
                break;
            }
#endif

#ifdef FLAC_DEC_INCLUDE
        case (CODEC_FLAC_DEC):
            {
                FREQ_ExitModule(FREQ_FLAC);
                break;
            }
#endif
#ifdef OGG_DEC_INCLUDE
        case (CODEC_OGG_DEC):
            if (((gpstAudioControlData->pAudioRegKey->bitrate <= 240000)&&(gpstAudioControlData->pAudioRegKey->channels ==1)) ||
                    ((gpstAudioControlData->pAudioRegKey->bitrate > 54000)&&(gpstAudioControlData->pAudioRegKey->bitrate <= 172000)&&(gpstAudioControlData->pAudioRegKey->channels ==2)))
            {
                FREQ_ExitModule(FREQ_OGG);
                break;
            }
            else if ((gpstAudioControlData->pAudioRegKey->bitrate > 172000)&&(gpstAudioControlData->pAudioRegKey->bitrate < 256000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))
            {
                FREQ_ExitModule(FREQ_NOGG);
                break;
            }
            else if (( gpstAudioControlData->pAudioRegKey->bitrate >= 256000)&&( gpstAudioControlData->pAudioRegKey->bitrate <= 380000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))
            {
                FREQ_ExitModule(FREQ_HOGG);
                break;
            }
            else if (((380000 <gpstAudioControlData->pAudioRegKey->bitrate <= 500000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))||(gpstAudioControlData->pAudioRegKey->bitrate <= 54000))
            {
                FREQ_ExitModule(FREQ_EHOGG);
                break;
            }
#endif
#ifdef HIFI_APE_DECODE
        case (CODEC_HIFI_APE_DEC):
            {
                FREQ_ExitModule(FREQ_HAPE);
                break;
            }
#endif
#ifdef HIFI_FlAC_DECODE
        case (CODEC_HIFI_FLAC_DEC):
            {
                FREQ_ExitModule(FREQ_HFLAC);
                break;
            }
#endif
#ifdef HIFI_AlAC_DECODE
        case (CODEC_HIFI_ALAC_DEC):
            {
                FREQ_ExitModule(FREQ_HALAC);
                break;
            }
#endif
    }

#ifdef DC_FILTER
    if ((gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_44100Hz) && (gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_22050Hz)&&(gpstAudioControlData->pAudioRegKey->samplerate != CodecFS_11025Hz))
    {
        if ((CurrentCodec != CODEC_WAV_DEC)&&((gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_32KHz) || (gpstAudioControlData->pAudioRegKey->bitrate >= 256000)))
        {
            FREQ_ExitModule(FREQ_DC_FILTER_HIGH);
        }
        else if ((CurrentCodec == CODEC_MP3_DEC)&&(((MP3_FORMAT_FLAG == 0x11)&&(gpstAudioControlData->pAudioRegKey->bitrate == 128000))||
                 (((MP3_FORMAT_FLAG & 0xF0) == 0x20)&&(gpstAudioControlData->pAudioRegKey->bitrate <= 128000)&&(gpstAudioControlData->pAudioRegKey->bitrate >= 96000))))
        {
            FREQ_ExitModule(FREQ_DC_FILTER_HIGH);
        }
        else if (((CurrentCodec != CODEC_MP3_DEC)&&(gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_48KHz))
                 ||((CurrentCodec == CODEC_MP3_DEC)&&(gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_48KHz)&&(MP3_FORMAT_FLAG ==0x11) ))
        {
            FREQ_ExitModule(FREQ_DC_FILTER_HIGH);
        }
        else
        {
            FREQ_ExitModule(FREQ_DC_FILTER_LOW);
        }
    }
#endif

}
/*******************************************************************************
** Name: AudioFileClose
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:38:35
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioFileClose(void)
{
    FileDev_CloseFile(gpstAudioControlData->hFile[0]);
    gpstAudioControlData->hFile[0] = NULL;

#ifdef FLAC_DEC_INCLUDE
    if (CurrentCodec == CODEC_FLAC_DEC)
    {
        FileDev_CloseFile(gpstAudioControlData->hFile[1]);
        gpstAudioControlData->hFile[1] = NULL;
    }
#endif

#ifdef AAC_DEC_INCLUDE
    if (CurrentCodec == CODEC_AAC_DEC)
    {
        FileDev_CloseFile(gpstAudioControlData->hFile[1]);
        gpstAudioControlData->hFile[1] = NULL;

        FileDev_CloseFile(gpstAudioControlData->hFile[2]);
        gpstAudioControlData->hFile[2] = NULL;
    }
#endif

#ifdef HIFI_AlAC_DECODE
    if (CurrentCodec == CODEC_HIFI_ALAC_DEC)
    {
        FileDev_CloseFile(gpstAudioControlData->hFile[1]);
        gpstAudioControlData->hFile[1] = NULL;

        FileDev_CloseFile(gpstAudioControlData->hFile[2]);
        gpstAudioControlData->hFile[2] = NULL;
    }
#endif

}
/*******************************************************************************
** Name: AudioWaitBBStop
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:29:19
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioWaitBBStop(void)
{
    printf("\n zz \n");
    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_FILE_CLOSE_CMPL,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    MailBoxWriteA2BData(1,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
}
/*******************************************************************************
** Name: AudioDeHWInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:25:55
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioDeHWInit(void)
{
    switch (CurrentCodec)
    {
#ifdef MP3_DEC_INCLUDE
        case CODEC_MP3_DEC:
#ifdef A_CORE_DECODE
            {
#if(MP3_EQ_WAIT_SYNTH)
                {
                    mp3_wait_synth();
                }
#endif

                //Int Disable
                IntDisable(INT_ID_IMDCT);
                IntDisable(INT_ID_SYNTH);
                IntPendingClear(INT_ID_IMDCT);
                IntPendingClear(INT_ID_SYNTH);
                IntUnregister(INT_ID_IMDCT);
                IntUnregister(INT_ID_SYNTH);
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_MP3_DECODE_BIN);
                #endif
            }
#endif

            ScuSoftResetCtr(IMDCT_SRST, 1);
            ScuSoftResetCtr(SYNTH_SRST, 1);
            ScuClockGateCtr(HCLK_SYNTH_GATE,0);
            ScuClockGateCtr(HCLK_IMDCT_GATE,0);

            break;
#endif

#ifdef  WMA_DEC_INCLUDE
        case CODEC_WMA_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_WMA_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef  AAC_DEC_INCLUDE
        case CODEC_AAC_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_AAC_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef WAV_DEC_INCLUDE
        case CODEC_WAV_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_WAV_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef APE_DEC_INCLUDE
        case CODEC_APE_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_APE_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef FLAC_DEC_INCLUDE
        case CODEC_FLAC_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_FLAC_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef OGG_DEC_INCLUDE
        case CODEC_OGG_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_OGG_DECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef HIFI_APE_DECODE
        case CODEC_HIFI_APE_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                printf("\n Dd AudioWaitBBStop\n");
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_APE_HDECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef HIFI_FlAC_DECODE
        case CODEC_HIFI_FLAC_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_FLAC_HDECODE_BIN);
                #endif
            }
#endif
            break;
#endif

#ifdef HIFI_AlAC_DECODE
        case CODEC_HIFI_ALAC_DEC:
#ifdef A_CORE_DECODE
            {
                //...

                //Int Disable
                //...

                //clock gate&Reset Init
            }
#else
            {
                //...
                AudioWaitBBStop();
                BcoreDev_ShutOff(gpstAudioControlData->hBcore);
                #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
                FW_RemoveModule(MODULE_ID_ALAC_HDECODE_BIN);
                #endif
            }
#endif
            break;
#endif

        default:
            break;
    }

}
/*******************************************************************************
** Name: CloseTrack
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:23:17
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void CloseTrack(void)
{
    CodecClose();
}

/*******************************************************************************
** Name: AudioVariableDeInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 11:21:09
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioVariableDeInit(void)
{
    return;
}
/*******************************************************************************
** Name: AudioWaitBBStart
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.25
** Time: 17:57:12
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioWaitBBStart(void)
{
    gFileHandle.codecType = CurrentCodec;
    /*
    #if 1
    StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[0], 0);
    StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[1], 1);
    StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[2], 2);
    #else
    StreamControlTask_SendFileHandle(gpstAudioControlData->hFifo, 4);
    #endif
    */
    if(gpstAudioControlData->hFifo == NULL)
    {
        StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[0], 0);
        StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[1], 1);
        StreamControlTask_SendFileHandle(gpstAudioControlData->hFile[2], 2);
    }
    else
    {
        StreamControlTask_SendFileHandle(gpstAudioControlData->hFifo, 4);
    }

    MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_FILE_OPEN_CMPL,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    MailBoxWriteA2BData((int)&gFileHandle,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    printf("AudioWaitBBStart rkos_semaphore_take \n");
    rkos_semaphore_take(osAudioDecodeOk, MAX_DELAY);
    printf("AudioWaitBBStart over### \n");
}

/*******************************************************************************
** Name: AudioControlTask_AudioDecodeProc
** Input:MSG_ID id, void * msg
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:12:20
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioDecodeProc(AUDIO_CMD id, void * msg)
{
    BOOLEAN ret = TRUE;
    unsigned long  HoldOnTimeTemp;
    AUDIO_DEV_ARG stParg;

    switch (id)
    {
        case AUDIO_CMD_DECSTART:
            printf("#$#$AudioStart \n");
            AudioStart();
            break;

        case AUDIO_CMD_STOP:
            AudioStop((int)msg);
            break;
#if 1
        case AUDIO_CMD_PAUSE:
            AudioPause();
            break;

        case AUDIO_CMD_RESUME:
            AudioResume();
            break;

        case AUDIO_CMD_FFD:     //FFW
            AudioFFD((int)msg);
            break;

        case AUDIO_CMD_FFW:     //FFD
            AudioFFW((int)msg);
            break;
#if 0
        case AUDIO_CMD_ABPLAY:
            AudioSetAB();
            break;

        case AUDIO_CMD_ABSETA:
            AudioSetAB_A();
            break;

        case MSG_AUDIO_ABSTOP:
            AudioABStop();
            break;
#endif

        case AUDIO_CMD_VOLUMESET:
            AudioDev_Get_Par(&stParg);
            //AudioSetVolume(stParg.Vol);
            if (AUDIO_STATE_PLAY == gpstAudioControlData->AudioPlayState)
            {

                if(stParg.Vol <= 0)
                {
                    stParg.Vol = 0;
                }
                else if(stParg.Vol >=30)
                {
                    stParg.Vol = 30;
                }

                AudioSetVolume(stParg.Vol);
                AudioDev_SetVol(gpstAudioControlData->hAudio, stParg.Vol);
                //Codec_SetVolumet(AudioDev_SetEQ.playVolume);
            }

            {
                //AudioInOut_Type  *pAudio = &AudioIOBuf;
                //RKEffect   *pEffect = &pAudio->EffectCtl;
                //RKEffect   *pEffect = gpstAudioControlData->UserEQ;
                if ((gpstAudioControlData->playVolume > 27) && (gpstAudioControlData->UserEQ.Mode== EQ_BASS))
                {
#ifdef _RK_EQ_FAST_
                    MsgDev_SendMsg(gpstAudioControlData->hMsg, MSG_MUSIC_EQ_SET);
#endif
                    AudioDev_SetEQ(gpstAudioControlData->hAudio, EQ_BASS, gpstAudioControlData->playVolume);
                }
            }
            break;

        case AUDIO_CMD_EQSET:
            if ((int)msg)
            {
                //memcpy(AudioIOBuf->UserEQ, &gpstAudioControlData->UserEQ, sizeof(RKEffect));
            }
            AudioDev_SetEQ(gpstAudioControlData->hAudio, EQ_NOR, gpstAudioControlData->playVolume);
            break;

        case AUDIO_CMD_UNMUTE:
            {
                Codec_DACUnMute();
            }
            break;

        case AUDIO_CMD_NEXTFILE:
            AudioNext();
            break;

        case AUDIO_CMD_PREVFILE:
            AudioUp();
            break;

        case AUDIO_CMD_FF_PAUSE:
            AudioFFPause();
            break;

        case AUDIO_CMD_FF_RESUME:
            AudioFFResume();
            break;

        default:
            ret = FALSE;
            break;
#endif

    }
    return;

}


/*******************************************************************************
** Name: AudioSetVolume
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:46:40
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioSetVolume(uint32 Volume)
{
    gpstAudioControlData->playVolume = Volume;
}

/*******************************************************************************
** Name: AudioDecodingGetOutBuffer
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 14:53:22
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
__irq COMMON FUN void AudioDecodingGetOutBuffer(void)
{
    uint32 cmd;
    uint32 data;

    cmd  = MailBoxReadB2ACmd(MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    data = MailBoxReadB2AData(MAILBOX_ID_0, MAILBOX_CHANNEL_1);
    //printf("\n MailBoxReadB2ACmd\n");
    gDecCmd  = cmd;
    gDecData = data;

    MailBoxClearB2AInt(MAILBOX_ID_0, MAILBOX_INT_1);
    //printf("B2A Decode Service cmd = %d\n",cmd);

    switch (cmd)
    {
        case MEDIA_MSGBOX_CMD_FILE_OPEN_HANDSHK:
            printf("jszc \n");
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_FILE_CLOSE_HANDSHK:
            printf("\n cjh zs \n");
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

#if 0
        case MEDIA_MSGBOX_CMD_FILE_CLOSE:
            {
                //uint32 ret;
                //if(gpstAudioControlData->hFifo == NULL)
                //{
                //    ret = FileDev_CloseFile(gpstStreamControlData->hFile[gpstStreamControlData->data]);
                //}
                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_FILE_CLOSE_CMPL,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
                MailBoxWriteA2BData(1,MAILBOX_ID_0, MAILBOX_CHANNEL_1);
            }
            printf("MEDIA_MSGBOX_CMD_FILE_CLOSE rkos_semaphore_give_fromisr 2\n");
            //rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;
#endif


        case MEDIA_MSGBOX_CMD_DEC_OPEN_ERR:
            gError   = 1;
            //printf("AudioDecodingGetOutBuffer rkos_semaphore_give_fromisr 4\n");
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_DEC_OPEN_CMPL:
            memcpy(&gpMediaBlock,(MediaBlock *)data,sizeof(MediaBlock) - 4);
            gpMediaBlock.DecodeOver = 1;
            gError   = 0;
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_DECODE_CMPL:

            memcpy(&gpMediaBlock,(MediaBlock *)data,sizeof(MediaBlock) - 4);

            gpMediaBlock.DecodeOver = 1;
            gpMediaBlock.Decoding   = 0;

            //printf("gpMediaBlock.needDecode = %d", gpMediaBlock.needDecode);
            if(gpMediaBlock.needDecode)
            {
                gpMediaBlock.needDecode = 0;
                //printf("decode ok");
                rkos_semaphore_give_fromisr(osAudioDecodeOk);
            }
            break;

        case MEDIA_MSGBOX_CMD_DECODE_ERR:
            gpMediaBlock.DecodeOver = 1;
            gpMediaBlock.DecodeErr = 1;
            gpMediaBlock.Decoding   = 0;

            if(gpMediaBlock.needDecode)
            {
                gpMediaBlock.needDecode = 0;
                rkos_semaphore_give_fromisr(osAudioDecodeOk);
            }

            break;

        case MEDIA_MSGBOX_CMD_DECODE_SEEK_CMPL:
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;


        case MEDIA_MSGBOX_CMD_DECODE_GETTIME_CMPL:
            memcpy(&gpMediaBlock,(MediaBlock *)data,sizeof(MediaBlock) - 4);
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_DECODE_GETBUFFER_CMPL:
            memcpy(&gpMediaBlock,(MediaBlock *)data,sizeof(MediaBlock) - 4);
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_DECODE_CLOSE_CMPL:
            printf("MEDIA_MSGBOX_CMD_DECODE_CLOSE_CMPL\n");
            rkos_semaphore_give_fromisr(osAudioDecodeOk);
            break;

        case MEDIA_MSGBOX_CMD_FLAC_SEEKFAST:
            #if 0
            {
                FILE* fp;
                gpFlacSeekFastParm = (FLAC_SEEKFAST_OP_t*)data;
                fp = (FILE*)gpFlacSeekFastParm->fp;
                FileInfo[(int)fp].Offset = gpFlacSeekFastParm->offset;
                FileInfo[(int)fp].Clus   = gpFlacSeekFastParm->clus;

                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_FLAC_SEEKFAST_CMPL,MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                MailBoxWriteA2BData(0,MAILBOX_ID_0, MAILBOX_CHANNEL_2);
            }
            #endif
            break;


        case MEDIA_MSGBOX_CMD_FLAC_GETSEEK_INFO:
            #if 0
            {
                gpFlacSeekFastParm->clus = FileInfo[(int)data].Clus;
                gpFlacSeekFastParm->offset = FileInfo[(int)data].Offset;

                MailBoxWriteA2BCmd(MEDIA_MSGBOX_CMD_FLAC_SEEKFAST_INFO_CMPL,MAILBOX_ID_0, MAILBOX_CHANNEL_2);
                MailBoxWriteA2BData((uint32)gpFlacSeekFastParm,MAILBOX_ID_0, MAILBOX_CHANNEL_2);
            }
            #endif
            break;


        default:
            return;
    }
}

/*******************************************************************************
** Name: AudioControlTask_Resume
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 10:21:46
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN rk_err_t AudioControlTask_Resume(void)
{

}
/*******************************************************************************
** Name: AudioControlTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 10:21:46
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN rk_err_t AudioControlTask_Suspend(void)
{

}

/*******************************************************************************
** Name: CheckID3V2Tag
** Input:uint8 *buf
** Return: int32
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 16:27:47
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN int32 CheckID3V2Tag(uint8 *pucBuffer)
{
    // The first three bytes of the tag should be "ID3".
    if ((pucBuffer[0] !=    'I') || (pucBuffer[1] != 'D') || (pucBuffer[2] != '3'))
    {
        return(0);
    }

    // The next byte should be the value 3 (i.e. we support ID3v2.3.0).
    //if(pucBuffer[3]   != 3)
    if (pucBuffer[3] < 2 && pucBuffer[3] > 4)
    {
        return(0);
    }

    // The next byte should be less than 0xff.
    if (pucBuffer[4] == 0xff)
    {
        return(0);
    }

    // We don't care about the next byte.  The following four bytes should be
    // less than 0x80.
    if ((pucBuffer[6] >= 0x80) || (pucBuffer[7] >= 0x80)    ||
            (pucBuffer[8] >= 0x80) || (pucBuffer[9] >= 0x80))
    {
        return(0);
    }

    // Return the length of the ID3v2 tag.
    return((pucBuffer[6] << 21) | (pucBuffer[7] << 14) |
           (pucBuffer[8] <<  7) |  pucBuffer[9]);
}

/*******************************************************************************
** Name: AudioCheckStreamType
** Input:uint16 * path,  HDC hFile
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 16:21:48
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioCheckStreamType(uint16 * path,  HDC hFile)
{
    uint8 char_buf[512];
    uint8 *buf = char_buf;
    int ID3_Length ;
    int  Redundancy_len = 0;
    uint32 pathlen;

    FileDev_ReadFile(hFile,char_buf, 512);

    pathlen = StrLenW(path);

    pathlen -= 3;
    path += pathlen;


#if 0
    {
        uint32 i;

        for (i = 0; i < 512; i++)
        {
            if (i%16 == 0)
            {
                printf("\n");
            }
            printf("0x%2x ", buf[i]);
        }
    }
#endif

    ID3_Length = CheckID3V2Tag(buf);
    if (ID3_Length)
    {
        printf("\naudio auto anlayse == id3\n");
        ID3_Length += 10;
        if (ID3_Length < (512 -17))
        {
            buf += ID3_Length;
            Redundancy_len =  ID3_Length;
        }
        else
        {
            FileDev_FileSeek(hFile, 0, ID3_Length);
            FileDev_ReadFile(hFile, char_buf, 512);
        }
        printf("ID3 len = 0x%x \n",ID3_Length);
    }

    while (1)
    {
        if ((buf[0] == 0x30) && (buf[1] == 0x26) && (buf[2] == 0xB2))
        {
            printf("\naudio auto anlayse == wma\n");
            *path++ = 'W';
            *path++ = 'M';
            *path++ = 'A';
            break;
        }
        else if (((*(uint16 *)buf) & 0xe0ff) == 0xe0ff &&((buf[15] !='a') &&(buf[16] !='a') &&(buf[17] !='c')))
        {
            uint32 framelen, framesec, frameoffset;

            uint32 i;

            framelen = ((uint16)((buf[3] & 0x03) << 11)) | ((uint16)((buf[4])  << 3)) | ((uint16)(buf[5] >> 5));

            printf("\naudio auto anlayse == maybe no.id3.mp3, framesize = %d\n", framelen);

            if (framelen == 0)
            {
                printf("\naudio auto anlayse == no.id3.mp3\n");
                *path++ = 'M';
                *path++ = 'P';
                *path++ = '3';
                break;
            }
            else
            {
                framesec = framelen / 512;
                frameoffset = framelen % 512;

                if (framesec > 0)
                {
                    FileDev_FileSeek(hFile, SEEK_SET, framesec * 512+ID3_Length);
                    FileDev_ReadFile(hFile, char_buf, 512);

                    buf = char_buf;
#if 0
                    {
                        uint32 i;

                        for (i = 0; i < 512; i++)
                        {
                            if (i%16 == 0)
                            {
                                printf("\n");
                            }
                            printf("0x%2x ", buf[i]);
                        }
                    }
#endif
                }

                if (((buf[frameoffset] & 0xff) == 0xff)  && ((buf[frameoffset+1] & 0xf0) == 0xf0)&&((buf[frameoffset+3] & 0x03) !=0x03))
                {
                    printf("\naudio auto anlayse == AAC\n");
                    *path++ = 'A';
                    *path++ = 'A';
                    *path++ = 'C';
                    break;
                }
                else
                {
                    printf("\naudio auto anlayse == no.id3.mp3\n");

                    *path++ = 'M';
                    *path++ = 'P';
                    *path++ = '3';
                    break;
                }
            }


        }
        else if ((buf[4] == 'f') && (buf[5] == 't') && (buf[6] == 'y') && (buf[7] == 'p'))
        {
            printf("\naudio auto anlayse == M4A\n");
            *path++ = 'M';
            *path++ = '4';
            *path++ = 'A';
            break;
        }
        else if ((buf[9] == 0) && (buf[10] == 0) && (buf[11] == 'l') && (buf[12] == 'i') &&
                 (buf[13] == 'b') && (buf[14] == 'f') && (buf[15] == 'a') && (buf[16] == 'a') && (buf[17] == 'c'))
        {
            printf("\naudio auto anlayse == AAC\n");
            *path++ = 'A';
            *path++ = 'A';
            *path++ = 'C';
            break;
        }
        else if ((buf[0] == 'R') && (buf[1] == 'I') && (buf[2] == 'F') && (buf[3] == 'F'))
        {
            printf("\naudio auto anlayse == WAV\n");
            *path++ = 'W';
            *path++ = 'A';
            *path++ = 'V';
            break;
        }
        else if ((buf[0] == 'f') && (buf[1] == 'L') && (buf[2] == 'a') && (buf[3] == 'C'))
        {
            printf("\naudio auto anlayse == flac\n");
            *path++ = 'F';
            *path++ = 'L';
            *path++ = 'A';
            break;
        }
        else if ((buf[0] == 'M') && (buf[1] == 'A') && (buf[2] == 'C'))
        {
            printf("\naudio auto anlayse == ape\n");
            *path++ = 'A';
            *path++ = 'P';
            *path++ = 'E';
            break;
        }
        else if ((buf[0] == 'O') && (buf[1] == 'g') && (buf[2] == 'g') && (buf[3] == 'S'))
        {
            printf("\naudio auto anlayse == ogg\n");
            *path++ = 'O';
            *path++ = 'G';
            *path++ = 'G';
            break;
        }
        else
        {
            buf++;
            Redundancy_len++;
            if (Redundancy_len == 512)
            {
                int ret = FileDev_ReadFile(hFile, char_buf, 512);
                if (ret < 512)
                {
                    DEBUG("Don't know file type");
                    break;
                }
                buf = char_buf;
                Redundancy_len = 0;
            }
            else if (Redundancy_len == 1024)
            {
                DEBUG("Don't know file type");
                break;
            }

        }
    }

    FileDev_FileSeek(hFile, SEEK_SET, 0);

}

/*******************************************************************************
** Name: GetFileType
** Input:UINT16 *path, UINT8 *pStr
** Return: uint8
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 16:13:08
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN uint8 GetFileType(UINT16 *path, UINT8 *pStr)
{
    UINT8 Len;
    UINT8 Retval = 0xff;
    UINT8 i;
    UINT16 pathlen;

    uint8 * pBuffer;

    i = 0;
    Len = strlen((char*)pStr);
    printf("$$$GetFileType Len=%d----\n",Len);
    pathlen = StrLenW(path);
    printf("$$$GetFileType pathlen=%d----\n",pathlen);
    pathlen *= 2;

    pathlen -= 6;

    pBuffer = ((uint8 *)path) + pathlen;

    while (i <= Len)
    {
        i += 3;
        //printf("$$$string::%s----mva:%s\n",pBuffer,pStr);
        if (((*(pBuffer + 0) == *(pStr + 0)) || ((*(pBuffer + 0) + ('a' - 'A')) == *(pStr + 0)) || ((*(pBuffer + 0) - ('a' - 'A')) == *(pStr + 0)))
            && ((*(pBuffer + 2) == *(pStr + 1)) || ((*(pBuffer + 2) + ('a' - 'A')) == *(pStr + 1)) || ((*(pBuffer + 2) - ('a' - 'A')) == *(pStr + 1)))
            && ((*(pBuffer + 2) == *(pStr + 1)) || ((*(pBuffer + 2) + ('a' - 'A')) == *(pStr + 1)) || ((*(pBuffer + 2) - ('a' - 'A')) == *(pStr + 1)))
            && ((*(pBuffer + 2) == *(pStr + 1)) || ((*(pBuffer + 2) + ('a' - 'A')) == *(pStr + 1)) || ((*(pBuffer + 2) - ('a' - 'A')) == *(pStr + 1))))
        {
            break;
        }
        pStr += 3;
    }

    if (i <= Len)
    {
        Retval = i / 3;
    }
    return (Retval);
}

/*******************************************************************************
** Name: AudioCodec
** Input:UINT16 *pBuffer, UINT8 *pStr
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 16:11:33
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioCodec(UINT16 *path, UINT8 *pStr)
{
    UINT8 temp;
    temp = GetFileType(path, pStr);
    printf("AudioCodec temp =%d\n", temp);
    switch (temp)
    {
        case 1:     //mp3
        case 2:     //mp2
        case 3:     //mp1
#ifdef MP3_DEC_INCLUDE
            CurrentCodec = CODEC_MP3_DEC;
#endif
            break;

        case 4:     //wma
#ifdef  WMA_DEC_INCLUDE
            CurrentCodec = CODEC_WMA_DEC;
#endif
            break;

        case 5:     //wav
#ifdef WAV_DEC_INCLUDE
            CurrentCodec = CODEC_WAV_DEC;
#endif
            break;

        case 6:     //ape
#ifdef APE_DEC_INCLUDE
            CurrentCodec = CODEC_APE_DEC;
#endif

#ifdef HIFI_APE_DECODE
            CurrentCodec =CODEC_HIFI_APE_DEC;
#endif
            break;

        case 7:     //flac
#ifdef FLAC_DEC_INCLUDE
            CurrentCodec = CODEC_FLAC_DEC;
#endif
#ifdef HIFI_FlAC_DECODE
            CurrentCodec =CODEC_HIFI_FLAC_DEC;
#endif
            break;
        case 8:     //AAC
        case 9:     //M4A
        case 11:    //MP4
        case 12:    // 3GP
#ifdef AAC_DEC_INCLUDE
            CurrentCodec = CODEC_AAC_DEC;
#endif
#ifdef HIFI_AlAC_DECODE
            CurrentCodec =CODEC_HIFI_ALAC_DEC;
#endif
            break;

        case 10:    //OGG
#ifdef OGG_DEC_INCLUDE
            CurrentCodec = CODEC_OGG_DEC;
#endif
            break;


        default:
            CurrentCodec = 0xff;
            break;
    }
}
/*******************************************************************************
** Name: AudioHoldonInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:49:15
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioHoldonInit(void)
{

}
/*******************************************************************************
** Name: AudioFREQInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:46:06
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioFREQInit(void)
{
    switch (CurrentCodec)
    {
#ifdef MP3_DEC_INCLUDE
        case (CODEC_MP3_DEC):
            {
                if ((gpstAudioControlData->pAudioRegKey->bitrate <= 128000)&&(gpstAudioControlData->pAudioRegKey->samplerate <= 44100))
                {
                    FREQ_EnterModule(FREQ_MP3);
                }
                else
                {
                    FREQ_EnterModule(FREQ_MP3H);

                }
                break;
            }
#endif

#ifdef WMA_DEC_INCLUDE
        case (CODEC_WMA_DEC):
            {
                if (gpstAudioControlData->pAudioRegKey->bitrate < 128000)
                {
                    if ((gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_32KHz) &&(gpstAudioControlData->pAudioRegKey->bitrate/1000 == 22))
                    {
                        FREQ_EnterModule(FREQ_WMAH);
                    }
                    else if ((gpstAudioControlData->pAudioRegKey->samplerate == CodecFS_44100Hz) &&(gpstAudioControlData->pAudioRegKey->bitrate/1000 == 48))
                    {
                        FREQ_EnterModule(FREQ_WMAH);
                    }
                    else
                        FREQ_EnterModule(FREQ_WMA);
                }
                else
                {
                    FREQ_EnterModule(FREQ_WMAH);
                }
                break;
            }
#endif

#ifdef AAC_DEC_INCLUDE
        case (CODEC_AAC_DEC):
            {
                FREQ_EnterModule(FREQ_AAC);
                break;
            }
#endif

#ifdef WAV_DEC_INCLUDE
        case (CODEC_WAV_DEC):
            {
                FREQ_EnterModule(FREQ_WAV);
                break;
            }
#endif

#ifdef APE_DEC_INCLUDE
        case (CODEC_APE_DEC):
            {
                if (gpstAudioControlData->pAudioRegKey->samplerate <= CodecFS_48KHz)
                    FREQ_EnterModule(FREQ_APE);
                else
                    FREQ_EnterModule(FREQ_HAPE);
                break;
            }
#endif

#ifdef FLAC_DEC_INCLUDE
        case (CODEC_FLAC_DEC):
            {
                FREQ_EnterModule(FREQ_FLAC);
                break;
            }
#endif
#ifdef OGG_DEC_INCLUDE
        case (CODEC_OGG_DEC):
            {
                if (((gpstAudioControlData->pAudioRegKey->bitrate <= 240000)&&(gpstAudioControlData->pAudioRegKey->channels ==1)) ||
                        ((gpstAudioControlData->pAudioRegKey->bitrate > 54000)&&(gpstAudioControlData->pAudioRegKey->bitrate <= 172000)&&(gpstAudioControlData->pAudioRegKey->channels ==2)))
                {
                    DEBUG("ENTER FREQ_OGG");
                    FREQ_EnterModule(FREQ_OGG);
                    break;
                }
                else if ((gpstAudioControlData->pAudioRegKey->bitrate > 172000)&&(gpstAudioControlData->pAudioRegKey->bitrate < 256000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))
                {
                    DEBUG("ENTER FREQ_NOGG");
                    FREQ_EnterModule(FREQ_NOGG);
                    break;
                }
                else if ((gpstAudioControlData->pAudioRegKey->bitrate >= 256000)&&(gpstAudioControlData->pAudioRegKey->bitrate <= 380000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))
                {
                    DEBUG("ENTER FREQ_HOGG");
                    FREQ_EnterModule(FREQ_HOGG);
                    break;
                }
                else if (((gpstAudioControlData->pAudioRegKey->bitrate <= 500000)&&(gpstAudioControlData->pAudioRegKey->channels ==2))||(gpstAudioControlData->pAudioRegKey->bitrate <= 54000))
                {
                    FREQ_EnterModule(FREQ_EHOGG);
                    DEBUG("ENTER FREQ_EHOGG");
                    break;
                }
            }
#endif
#ifdef HIFI_APE_DECODE
        case (CODEC_HIFI_APE_DEC):
            {
                FREQ_EnterModule(FREQ_HAPE);
                break;
            }
#endif
#ifdef HIFI_FlAC_DECODE
        case (CODEC_HIFI_FLAC_DEC):
            {
                FREQ_EnterModule(FREQ_HFLAC);
                break;
            }
#endif
#ifdef HIFI_AlAC_DECODE
        case (CODEC_HIFI_ALAC_DEC):
            {
                FREQ_EnterModule(FREQ_HALAC);
                break;
            }
#endif
    }

#ifdef DC_FILTER
    if ((gpstAudioControlData->pAudioRegKey->samplerate != FS_44100Hz) && (gpstAudioControlData->pAudioRegKey->samplerate != FS_22050Hz)&&(gpstAudioControlData->pAudioRegKey->samplerate != FS_11025Hz))
    {
        if ((CurrentCodec != CODEC_WAV_DEC)&&((gpstAudioControlData->pAudioRegKey->samplerate == FS_32KHz) || (gpstAudioControlData->pAudioRegKey->bitrate >= 256000)))
        {
            FREQ_EnterModule(FREQ_DC_FILTER_HIGH);
        }
        else if ((CurrentCodec == CODEC_MP3_DEC)&&(((MP3_FORMAT_FLAG == 0x11)&&(gpstAudioControlData->pAudioRegKey->bitrate == 128000))||
                 (((MP3_FORMAT_FLAG & 0xF0) == 0x20)&&(gpstAudioControlData->pAudioRegKey->bitrate <= 128000)&&(gpstAudioControlData->pAudioRegKey->bitrate >= 96000))))
        {
            FREQ_EnterModule(FREQ_DC_FILTER_HIGH);
        }
        else if (((CurrentCodec != CODEC_MP3_DEC)&&(gpstAudioControlData->pAudioRegKey->samplerate == FS_48KHz))
                 ||((CurrentCodec == CODEC_MP3_DEC)&&(gpstAudioControlData->pAudioRegKey->samplerate == FS_48KHz)&&(MP3_FORMAT_FLAG ==0x11) ))
        {
            FREQ_EnterModule(FREQ_DC_FILTER_HIGH);
        }
        else
        {
            FREQ_EnterModule(FREQ_DC_FILTER_LOW);
        }
        ret = DC1_FilterInit(gpstAudioControlData->pAudioRegKey->samplerate);
#ifdef _BEEP_
        BeepSampleRate = ret;
#endif
        gpstAudioControlData->pAudioRegKey->samplerate = ret;
    }
#endif
}
/*******************************************************************************
** Name: AudioCodecOpen
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:44:41
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN rk_err_t AudioCodecOpen(void)
{
    if (1 != CodecOpen(0, CODEC_OPEN_DECODE))
    {
        rk_printfA("CodecOpen != 1\n");
        return ERROR;
    }
    rk_printfA("CodecGetLength\n");
    CodecGetLength(&gpstAudioControlData->pAudioRegKey->TotalTime);
    CodecGetSampleRate(&gpstAudioControlData->pAudioRegKey->samplerate);
    rk_printfA("CodecGetSampleRate\n");

#ifdef _BEEP_
    BeepSampleRate = gpstAudioControlData->pAudioRegKey->samplerate;
#endif
    rk_printfA("CodecGetBitrate\n");
    CodecGetBitrate(&gpstAudioControlData->pAudioRegKey->bitrate);
    CodecGetChannels(&gpstAudioControlData->pAudioRegKey->channels);
    CodecGetBps(&gpstAudioControlData->pAudioRegKey->bps);
    CodecGetFrameLen(&gpstAudioControlData->AudioLen);
    rk_printfA("channel=%d bps = %d fs = %d bitrate = %d len = %d, time=%dm:%ds %d ms",gpstAudioControlData->pAudioRegKey->channels,gpstAudioControlData->pAudioRegKey->bps,gpstAudioControlData->pAudioRegKey->samplerate,gpstAudioControlData->pAudioRegKey->bitrate,gpstAudioControlData->AudioLen,gpstAudioControlData->pAudioRegKey->TotalTime/60000,gpstAudioControlData->pAudioRegKey->TotalTime/1000%60,gpstAudioControlData->pAudioRegKey->TotalTime%1000);

    if ((gpstAudioControlData->pAudioRegKey->bitrate <= 0)||(gpstAudioControlData->pAudioRegKey->samplerate <= 0))
    {
        return ERROR;
    }


#ifdef A_CORE_DECODE
    AudioCodecGetBufferSize(CurrentCodec, gpstAudioControlData->pAudioRegKey->samplerate);

#ifdef MP3_DEC_INCLUDE
    if (CurrentCodec == CODEC_MP3_DEC)
    {
        mp3_wait_synth();
    }
#endif

#ifdef FLAC_DEC_INCLUDE
    if (CurrentCodec != CODEC_FLAC_DEC)
#endif
    {
#ifdef AAC_DEC_INCLUDE
        if (CurrentCodec != CODEC_AAC_DEC)
#endif
        {
            //AudioFileChangeBuf(pRawFileCache,CodecBufSize);
        }
    }

#ifdef  WMA_DEC_INCLUDE
    if (CODEC_WMA_DEC == CurrentCodec)
    {
    /*
        //ModuleOverlay(MODULE_ID_WMA_HIGH_LOW_COMMON, MODULE_OVERLAY_ALL);
        if (g_flagHighRate)
        {
            //ModuleOverlay(MODULE_ID_WMA_HIGH_RATE, MODULE_OVERLAY_ALL);
        }
        else
        {
            //ModuleOverlay(MODULE_ID_WMA_LOW_RATE, MODULE_OVERLAY_ALL);
        }

        if (WmaTableOverLay == 3)
        {
            ModuleOverlay(MODULE_ID_WMA_DATA_16Ob, MODULE_OVERLAY_ALL);
        }
        else if (WmaTableOverLay == 2)
        {
            ModuleOverlay(MODULE_ID_WMA_DATA_44Ob, MODULE_OVERLAY_ALL);
        }
        else if (WmaTableOverLay == 1)
        {
            ModuleOverlay(MODULE_ID_WMA_DATA_44Qb, MODULE_OVERLAY_ALL);
        }
        */
    }
#endif

#endif

    return OK;
}
/*******************************************************************************
** Name: AudioHWInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:43:55
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioHWInit(void)
{
    HDC hBcoreDev;
    switch (CurrentCodec)
    {
#ifdef MP3_DEC_INCLUDE
        case CODEC_MP3_DEC:

            //FW_ModuleOverlay(MODULE_ID_MP3_DECODE, MODULE_OVERLAY_ALL);

            //clock gate&Reset Init
            ScuClockGateCtr(HCLK_SYNTH_GATE,1);
            ScuClockGateCtr(HCLK_IMDCT_GATE,1);
            ScuSoftResetCtr(IMDCT_SRST, 0);
            ScuSoftResetCtr(SYNTH_SRST, 0);

#ifdef A_CORE_DECODE
            {
                //Int Init
                IntPendingClear(INT_ID_IMDCT);
                IntPendingClear(INT_ID_SYNTH);
                IntRegister(INT_ID_IMDCT, imdct36_handler);
                IntRegister(INT_ID_SYNTH, synth_handler);
                IntEnable(INT_ID_IMDCT);
                IntEnable(INT_ID_SYNTH);

                //Others
                AcceleratorHWInit();
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
               //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_MP3_DECODE_BIN);
#endif
                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;

                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef  WMA_DEC_INCLUDE
        case CODEC_WMA_DEC:
            //FW_ModuleOverlay(MODULE_ID_WMA_COMMON, MODULE_OVERLAY_ALL);
            //FW_ModuleOverlay(MODULE_ID_WMA_OPEN_CODEC, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_WMA_DECODE_BIN);
#endif

                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;

                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef  AAC_DEC_INCLUDE
        case CODEC_AAC_DEC:
            //FW_ModuleOverlay(MODULE_ID_AAC_DECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //

#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_AAC_DECODE_BIN);
#endif

                //Others
                if(gpstAudioControlData->hFifo == NULL)
                {
                    memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                    gFileHandle.handle1 = (unsigned char)0;
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);

                    gFileHandle.handle2 = (unsigned char)1;
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[1], &gFileHandle.curfileoffset[1]);

                    gFileHandle.handle3 = (unsigned char)2;
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[2], &gFileHandle.curfileoffset[2]);

                }
                else
                {
                    while(!gFileHandle.filesize)
                    {
                        fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioLen);
                        rk_printf("audiocontrol TotalSize = %d", gpstAudioControlData->AudioLen);
                    }
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioFileSeekOffset);
                }

                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef WAV_DEC_INCLUDE
        case CODEC_WAV_DEC:
            //FW_ModuleOverlay(MODULE_ID_WAV_DECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_WAV_DECODE_BIN);
#endif

                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;

                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef APE_DEC_INCLUDE
        case CODEC_APE_DEC:
            //FW_ModuleOverlay(MODULE_ID_APE_DECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {

#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_APE_DECODE_BIN);
#endif

                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;

                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef FLAC_DEC_INCLUDE
        case CODEC_FLAC_DEC:
            //FW_ModuleOverlay(MODULE_ID_FLAC_DECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //

#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_FLAC_DECODE_BIN);
#endif


                //Others
                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;
                if(gpstAudioControlData->hFifo == NULL)
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);


                    gFileHandle.handle2 = (unsigned char)1;
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[1], &gFileHandle.curfileoffset[1]);

                }
                else
                {
                    while(!gFileHandle.filesize)
                    {
                        fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioLen);
                        rk_printf("audiocontrol TotalSize = %d", gpstAudioControlData->AudioLen);
                    }
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioFileSeekOffset);
                }

                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef OGG_DEC_INCLUDE
        case CODEC_OGG_DEC:
            //FW_ModuleOverlay(MODULE_ID_OGG_DECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //

#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_OGG_DECODE_BIN);
#endif
                if(gpstAudioControlData->hFifo == NULL)
                {
                    //Others
                    memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                    gFileHandle.handle1 = (unsigned char)0;
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    while(!gFileHandle.filesize)
                    {
                        fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioLen);
                        rk_printf("audiocontrol TotalSize = %d", gpstAudioControlData->AudioLen);
                    }
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioFileSeekOffset);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef HIFI_APE_DECODE
        case CODEC_HIFI_APE_DEC:
            //FW_ModuleOverlay(MODULE_ID_APE_HDECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_APE_HDECODE_BIN);
#endif

                //Others
                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;
                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef HIFI_FlAC_DECODE
        case CODEC_HIFI_FLAC_DEC:
            //FW_ModuleOverlay(MODULE_ID_FLAC_HDECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_FLAC_HDECODE_BIN);
#endif
                memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                gFileHandle.handle1 = (unsigned char)0;

                if(gpstAudioControlData->hFifo)
                {
                    fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gFileHandle.filesize);
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gFileHandle.curfileoffset[0]);
                }
                else
                {
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);
                }
                AudioWaitBBStart();
            }
#endif
            break;
#endif

#ifdef HIFI_AlAC_DECODE
        case CODEC_HIFI_ALAC_DEC:
            //FW_ModuleOverlay(MODULE_ID_ALAC_HDECODE, MODULE_OVERLAY_ALL);
#ifdef A_CORE_DECODE
            {
                //clock gate&Reset Init
                //...

                //Int Init
                //...

                //Others
                //...
                //AudioCodecGetBufferSize(CurrentCodec, FS_44100Hz);
                        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
            }
#else
            {
                //
#ifdef BB_SYS_JTAG
                BcoreDev_Start(gpstAudioControlData->hBcore,MODULE_ID_BB_CODE);
#else
                BcoreDev_Start(gpstAudioControlData->hBcore, MODULE_ID_ALAC_HDECODE_BIN);
#endif
                if(gpstAudioControlData->hFifo == NULL)
                {
                    //Others
                    memset(&gFileHandle,0,sizeof(FILE_HANDLE_t));
                    gFileHandle.handle1 = (unsigned char)0;
                    FileDev_GetFileSize(gpstAudioControlData->hFile[0], &gFileHandle.filesize);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[0], &gFileHandle.curfileoffset[0]);

                    gFileHandle.handle2 = (unsigned char)1;
                    gFileHandle.handle3 = (unsigned char)2;
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[1], &gFileHandle.curfileoffset[1]);
                    FileDev_GetFileOffset(gpstAudioControlData->hFile[2], &gFileHandle.curfileoffset[2]);
                }
                else
                {
                    while(!gFileHandle.filesize)
                    {
                        fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioLen);
                        rk_printf("audiocontrol TotalSize = %d", gpstAudioControlData->AudioLen);
                    }
                    fifoDev_GetOffset(gpstAudioControlData->hFifo, &gpstAudioControlData->AudioFileSeekOffset);
                }

                AudioWaitBBStart();
            }
#endif
            break;
#endif

        default:
            break;
    }
}
/*******************************************************************************
** Name: AudioFileOpen
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:35:12
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN rk_err_t AudioFileOpen(void)
{
    FILE_ATTR stFileAttr;
    rk_err_t ret;
    uint16 PathLen;
    uint16 * pPath;
    uint32 TotalSize;

    int CurrentCodecSave;


    stFileAttr.ShortName[11] = 0;


    //memcpy(gpstAudioControlData->AudioFileInfo.path, L"C:\\月亮之上.APE", 100);
    //stFileAttr.Path = gpstAudioControlData->AudioFileInfo.path;
    //stFileAttr.FileName = NULL;
    //stFileAttr.Path = L"C:\\月亮之上.APE";
    //printf("check size len =%d strlen=%d\n",sizeof(stFileAttr.Path), strlen((const char *)stFileAttr.Path));
    //debug_hex((char *)stFileAttr.Path, StrLenW(stFileAttr.Path)*2, 6);
    //printf("\n check will open$#$#$#$# file path and name:\n");

/*
        memcpy(gpstAudioControlData->AudioFileInfo.path, L"C:\\喜欢你嗨飞.APE", 100);
        stFileAttr.Path = L"C:\\喜欢你嗨飞.APE";//L"C:\\";
        stFileAttr.FileName = L"喜欢你嗨飞.APE";
        printf("喜欢你嗨飞.APE\n");
*/
    if(gpstAudioControlData->hFifo == NULL)
    {
        stFileAttr.Path = gpstAudioControlData->AudioFileInfo.path;
        PathLen = StrLenW(gpstAudioControlData->AudioFileInfo.path);
        *(stFileAttr.Path + PathLen) = 0x0000;
        //(uint16 *)(&(gpstAudioControlData->MusicLongFileName));
        //printf("will size len =%d strlen=%d StrLenW=%d\n",sizeof(gpstAudioControlData->AudioFileInfo.path), strlen((const char *)gpstAudioControlData->AudioFileInfo.path), PathLen);
        //printf("\n will open$#$#$#$# file path and name:\n");
        //debug_hex((char *)gpstAudioControlData->AudioFileInfo.path, StrLenW(gpstAudioControlData->AudioFileInfo.path)*2, 6);
        //printf("\n will open$#$#$#$# old file path and name:\n");
        //debug_hex((char *)gpstAudioControlData->AudioFileInfo.path, 50, 6);
        stFileAttr.FileName = NULL;
        AudioCodec(gpstAudioControlData->AudioFileInfo.path, (UINT8 *)AudioFileExtString);
        CurrentCodecSave = CurrentCodec;
    }
    else
    {
        CurrentCodec = CODEC_MP3_DEC;
        CurrentCodecSave = CurrentCodec;

        TotalSize = 0;

        fifoDev_GetTotalSize(gpstAudioControlData->hFifo, &TotalSize);

        rk_printf("TotalSize = %d", TotalSize);
    }


    DEBUG("\n CurrentCodec = %d \n",CurrentCodec);

    //AudioCheckStreamType(&gpstAudioControlData->AudioFileInfo.path[0], gpstAudioControlData->hFile[0]);
    //AudioCodec(&gpstAudioControlData->AudioFileInfo.path[0],(UINT8 *)AudioFileExtString);

    if ((CurrentCodec == 0xff) && (CurrentCodecSave == 0xff))
    {
        printf("ERROR!!! CurrentCodec == 0x%02x \n",CurrentCodec);
        return ERROR;
    }

    /*
    //memcpy(gpstAudioControlData->AudioFileInfo.path, L"C:\\I love you.APE", 100);
    PathLen = StrLenW(gpstAudioControlData->AudioFileInfo.path);
    pPath = gpstAudioControlData->AudioFileInfo.path;
    pPath += PathLen;

    while(*pPath != '\\')
    {
        PathLen--;
        pPath--;
    }
    //gpstAudioControlData->AudioFileInfo.path[PathLen] = 0;
    stFileAttr.Path = gpstAudioControlData->AudioFileInfo.path;
    stFileAttr.FileName = gpstAudioControlData->AudioFileInfo.path + PathLen + 1;
    */


#ifdef MP3_DEC_INCLUDE
       if (CODEC_MP3_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                 gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                 if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                 {
                     rk_printf("FileDev_OpenFile faile return -1\n");
                     return RK_ERROR;
                 }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef WMA_DEC_INCLUDE
       if (CODEC_WMA_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                  rk_printf("FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef WAV_DEC_INCLUDE
       if (CODEC_WAV_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                  rk_printf("FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef APE_DEC_INCLUDE
       if (CODEC_APE_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                  rk_printf("FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef FLAC_DEC_INCLUDE
       if (CODEC_FLAC_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                  rk_printf("FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
                gpstAudioControlData->hFile[1] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[1] <= 0)
                {
                  rk_printf("\n 1FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef AAC_DEC_INCLUDE
       if (CODEC_AAC_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                  rk_printf("\n 0FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
                gpstAudioControlData->hFile[1] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[1] <= 0)
                {
                  rk_printf("\n 1FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
                gpstAudioControlData->hFile[2] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[2] <= 0)
                {
                  rk_printf("\n 2FileDev_OpenFile faile return -1\n");
                  return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }

       }
#endif

#ifdef HIFI_APE_DECODE
       printf("will CurrentCodec =%d\n",CurrentCodec);
       if (CODEC_HIFI_APE_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                rk_printf("CurrentCodec =%d\n",CurrentCodec);
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                    rk_printf("FileDev_OpenFile faile return -1\n");
                    return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }

       }
#endif

#ifdef HIFI_FlAC_DECODE
       if (CODEC_HIFI_FLAC_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                    rk_printf("FileDev_OpenFile faile return -1\n");
                    return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }

       }
#endif

#ifdef HIFI_AlAC_DECODE
       if (CODEC_HIFI_ALAC_DEC == CurrentCodec)
       {
            if(gpstAudioControlData->hFifo == NULL)
            {
                gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
                {
                    rk_printf("\n 0FileDev_OpenFile faile return -1\n");
                    return RK_ERROR;
                }
                gpstAudioControlData->hFile[1] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[1] <= 0)
                {
                    rk_printf("\n 1FileDev_OpenFile faile return -1\n");
                    return RK_ERROR;
                }
                gpstAudioControlData->hFile[2] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
                if ((rk_err_t)gpstAudioControlData->hFile[2] <= 0)
                {
                    rk_printf("\n 2FileDev_OpenFile faile return -1\n");
                    return RK_ERROR;
                }
            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
       }
#endif

#ifdef OGG_DEC_INCLUDE
        if (CODEC_OGG_DEC == CurrentCodec)
        {
            if(gpstAudioControlData->hFifo == NULL)
            {
               gpstAudioControlData->hFile[0] = FileDev_OpenFile(gpstAudioControlData->hFileDev, NULL, READ_ONLY, &stFileAttr);
               if ((rk_err_t)gpstAudioControlData->hFile[0] <= 0)
               {
                   rk_printf("FileDev_OpenFile faile return -1\n");
                   return RK_ERROR;
               }

            }
            else
            {
                //pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, READ_WRITE_CTRL);

            }
        }
#endif

    printf("\n");
    UartDev_Write(UartHDC, stFileAttr.ShortName, 11, SYNC_MODE, NULL);
    return OK;
}

/*******************************************************************************
** Name: AudioVariableInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.24
** Time: 11:19:53
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_COMMON_
COMMON FUN void AudioVariableInit(void)
{
    gpstAudioControlData->pAudioRegKey->CurrentTime = 0;
    gpstAudioControlData->pAudioRegKey->TotalTime = 1;
    gpstAudioControlData->IsEQUpdate = 0;

    //DecodingError      = 0;
    gpstAudioControlData->AudioErrorFrameNum = 0;

    gpstAudioControlData->AudioEndFade = 0;

    CurrentCodec = 0xff;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: AudioControlTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 10:21:46
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_INIT_
INIT API rk_err_t AudioControlTask_DeInit(void *pvParameters)
{
    uint8 i;

    if(gpstAudioControlData->AudioPlayerState != AUDIO_STATE_STOP)
    {
        AudioStop(Audio_Stop_Force);
    }

    rk_printf("AudioControlTask_DeInit\n");

    AudioDev_RealseMainTrack(gpstAudioControlData->hAudio);

    if (gpstAudioControlData->hAudio != NULL)
    {
        if (RKDev_Close(gpstAudioControlData->hAudio) != RK_SUCCESS)
        {
            printf("hAudio close failure\n");
            return RK_ERROR;
        }
    }
/*
    if (gpstAudioControlData->hDir != NULL)
    {
        printf("gpstAudioControlData->hDir != NULL\n");
        if (FileDev_CloseDir(gpstAudioControlData->hDir) != RK_SUCCESS)
        {
            printf("hDir close failure\n");
            return RK_ERROR;
        }
    }
*/
/*
    for(i=0;i<3;i++)
    {
        if (gpstAudioControlData->hFile[i] != NULL)
        {
            printf("gpstAudioControlData->hDir != NULL\n");
            if (FileDev_CloseFile(gpstAudioControlData->hFile[i]) != RK_SUCCESS)
            {
                printf("hFile[%d] close failure\n",i);
                return RK_ERROR;
            }
        }
    }
*/
    if (gpstAudioControlData->hFileDev != NULL)
    {
        if (RKDev_Close(gpstAudioControlData->hFileDev) != RK_SUCCESS)
        {
            printf("hFileDev close failure\n");
            return RK_ERROR;
        }
    }
    /*
    if (gpstAudioControlData->hDirDev != NULL)
    {
        if (RKDev_Close(gpstAudioControlData->hDirDev) != RK_SUCCESS)
        {
            printf("hAudio close failure\n");
            return RK_ERROR;
        }
    }
    */
    if (gpstAudioControlData->hBcore != NULL)
    {
        if (RKDev_Close(gpstAudioControlData->hBcore) != RK_SUCCESS)
        {
            printf("hBcore close failure\n");
            return RK_ERROR;
        }
    }
    /*
    if (gpstAudioControlData->hMsg != NULL)
    {
        if (RKDev_Close(gpstAudioControlData->hMsg) != RK_SUCCESS)
        {
            printf("hMsg close failure\n");
            return RK_ERROR;
        }
    }
    */
    if (gpstAudioControlData->hFifo != NULL)
    {
        rk_printf("a9 hfifo -1\n");
        if (RKDev_Close(gpstAudioControlData->hFifo) != RK_SUCCESS)
        {
            printf("hFifo close failure\n");
            return RK_ERROR;
        }
    }
    rk_printf("KKK DecDataBuf[0]=%d [1]=%d\n",DecDataBuf[0],DecDataBuf[1]);

    if(osAudioDecodeOk)
        rkos_semaphore_delete(osAudioDecodeOk);
    if(gpstAudioControlData->AudioControlAskQueue)
        rkos_queue_delete(gpstAudioControlData->AudioControlAskQueue);

    if(gpstAudioControlData->AudioControlRespQueue)
        rkos_queue_delete(gpstAudioControlData->AudioControlRespQueue);

    if(gpstAudioControlData)
        rkos_memory_free(gpstAudioControlData);
    gpstAudioControlData = NULL;

    if(DecDataBuf[0])
        rkos_memory_free(DecDataBuf[0]);
    if(DecDataBuf[1])
        rkos_memory_free(DecDataBuf[1]);
    DecDataBuf[0] = NULL;
    DecDataBuf[1] = NULL;
    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_AUDIOCONTROL_TASK);
    #endif

    rk_printf(" FW_RemoveModule 01\n");
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: AudioControlTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 10:21:46
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_INIT_
INIT API rk_err_t AudioControlTask_Init(void *pvParameters, void *arg)
{
    uint32 i;
    int32 ret=0;
    RK_TASK_CLASS*   pAudioControlTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_AUDIOCONTROL_ARG * pArg = (RK_TASK_AUDIOCONTROL_ARG *)arg;
    AUDIOCONTROL_TASK_DATA_BLOCK*  pAudioControlTaskData;

    FILE_ATTR stFileAttr;
    uint32 BufLen;
    uint16 LongName[64];
    uint8 Buf[128];
    UINT16 *pLongName;
    uint8 url;

    if (pAudioControlTask == NULL)
    {
        rk_printf("audio task para error");
        return RK_PARA_ERR;
    }
    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_AUDIOCONTROL_TASK, MODULE_OVERLAY_CODE);
    #endif

    pAudioControlTaskData = NULL;
    pAudioControlTaskData = rkos_memory_malloc(sizeof(AUDIOCONTROL_TASK_DATA_BLOCK));
    if(pAudioControlTaskData == NULL)
    {
        rk_printf("pAudioControlTaskData == null");
    }
    memset(pAudioControlTaskData, NULL, sizeof(AUDIOCONTROL_TASK_DATA_BLOCK));

    pAudioControlTaskData->AudioControlAskQueue= rkos_queue_create(1, sizeof(AUDIOCONTROL_ASK_QUEUE));
    pAudioControlTaskData->AudioControlRespQueue = rkos_queue_create(1, sizeof(AUDIOCONTROL_RESP_QUEUE));
    pAudioControlTaskData->TaskObjectID = pAudioControlTask->TaskObjectID;

    //FREQ_EnterModule(FREQ_MAX);
    rk_printf("FREQ_EnterModule \n");

    printf("\n\n\n >>> sour =0x%x FileNum=%d\n\n\n ",pArg->ucSelPlayType, pArg->FileNum);

    pAudioControlTaskData->AudioFileInfo.ucSelPlayType = pArg->ucSelPlayType;

    if ((pArg->ucSelPlayType == SOURRE_FROM_DLNA) || (pArg->ucSelPlayType == SOURRE_FROM_WIFI))
    {
        pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, NOT_CARE);
        rk_printf("0 hfifo +1\n");

        if ((pAudioControlTaskData->hFifo == NULL)
        || (pAudioControlTaskData->hFifo == (HDC)RK_ERROR)
        || (pAudioControlTaskData->hFifo == (HDC)RK_PARA_ERR))
        {
            rk_print_string("fifo device open failure");
            rk_printf("0 hfifo -1\n");
            RKDev_Close(pAudioControlTaskData->hFifo);
            while(1);
        }
        rk_print_string("hFifo device open success");

        if ((pArg->FileNum <= 0) || (pArg->FileNum >= 0xFFFF))
        {
            pAudioControlTaskData->AudioFileInfo.CurrentFileNum = 1;
        }
        else
        {
            pAudioControlTaskData->AudioFileInfo.CurrentFileNum = pArg->FileNum;
        }

    }
    else if(pArg->ucSelPlayType == SOURRE_FROM_BT)
    {
        pAudioControlTaskData->hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, NOT_CARE);
        rk_printf("1 hfifo +1\n");

        if ((pAudioControlTaskData->hFifo == NULL)
        || (pAudioControlTaskData->hFifo == (HDC)RK_ERROR)
        || (pAudioControlTaskData->hFifo == (HDC)RK_PARA_ERR))
        {
            rk_print_string("fifo device open failure");
            rk_printf("1 hfifo -1\n");
            RKDev_Close(pAudioControlTaskData->hFifo);
            while(1);
        }
        rk_print_string("hFifo device open success");

    }
    else
    {
        pAudioControlTaskData->hFifo = NULL;
        rk_print_string("hFifo NULL");
    }


    if ((pAudioControlTaskData->AudioFileInfo.CurrentFileNum == 0xFFFF)||(pAudioControlTaskData->AudioFileInfo.CurrentFileNum == 0))//by zs 05.13 fix bug
    {
        pAudioControlTaskData->AudioFileInfo.CurrentFileNum = 1;
        rk_printf("init CurrentFileNum =%d\n",pAudioControlTaskData->AudioFileInfo.CurrentFileNum);
    }
    rk_printf("CurrentFileNum =%d \n",pAudioControlTaskData->AudioFileInfo.CurrentFileNum);
    if (pAudioControlTaskData->AudioFileInfo.ucSelPlayType == SORT_TYPE_SEL_FOLDER)
    {
        pAudioControlTaskData->hDir = DirDev_BuildDirInfo(pAudioControlTaskData->hDirDev,(UINT8*)AudioFileExtString, MUSIC_DB, &pAudioControlTaskData->AudioFileInfo.TotalFiles);

        if (0 == pAudioControlTaskData->AudioFileInfo.TotalFiles)
        {
            MsgDev_SendMsg(pAudioControlTaskData->hMsg, MSG_AUDIO_NOFILE);
            goto MusicInit_ERR;
        }
    }
    else if (pAudioControlTaskData->AudioFileInfo.ucSelPlayType == SORT_TYPE_SEL_BROWSER)
    {
        pAudioControlTaskData->hDir = DirDev_BuildDirInfo(pAudioControlTaskData->hDirDev,(UINT8*)AudioFileExtString, FS_FAT, &pAudioControlTaskData->AudioFileInfo.TotalFiles);

        if (0 == pAudioControlTaskData->AudioFileInfo.TotalFiles)
        {
            MsgDev_SendMsg(pAudioControlTaskData->hMsg, MSG_AUDIO_NOFILE);
            goto MusicInit_ERR;
        }
    }
    /*
    else if (pAudioControlTaskData->AudioFileInfo.ucSelPlayType == SORT_TYPE_SEL_M3U_BROWSER)//sanshin norton
    {
        if (0 == (pAudioControlTaskData->AudioFileInfo.TotalFiles = gM3uPlayListInfoSize))
        {
            MsgDev_SendMsg(pAudioControlTaskData->hMsg,MSG_AUDIO_NOFILE);
            goto MusicInit_ERR;
        }

    }
    */
    rk_printf("AUDIO_STATE_PLAY  \n");

    pAudioControlTaskData->AudioPlayerState = AUDIO_STATE_PLAY;

    if (pAudioControlTaskData->AudioFileInfo.ucSelPlayType != SORT_TYPE_SEL_FOLDER)
    {
        if (pAudioControlTaskData->AudioFileInfo.ucSelPlayType  == SORT_TYPE_SEL_M3U_BROWSER)
        {
            /*
            if (gSysConfig.MusicConfig.RepeatMode == AUDIO_FOLDER_ONCE || gSysConfig.MusicConfig.RepeatMode == AUIDO_FOLDER_REPEAT)
            {
                pAudioControlTaskData->pAudioRegKey->RepeatMode    =  gSysConfig.MusicConfig.RepeatMode+ 3; //make sure it don't repeat in folder,just use globalnum
            }
            else
            */
            {
                pAudioControlTaskData->AudioFileInfo.RepeatMode    =  gSysConfig.MusicConfig.RepeatMode;
            }
        }
        else
        {
            pAudioControlTaskData->AudioFileInfo.RepeatMode    =  gSysConfig.MusicConfig.RepeatMode;
        }
    }
    else
    {
        pAudioControlTaskData->AudioFileInfo.RepeatMode    =  gSysConfig.MusicConfig.RepeatModeBak;
        if ( gSysConfig.MusicConfig.RepeatModeBak > AUDIO_REPEAT)
        {
            pAudioControlTaskData->AudioFileInfo.CurrentFileNum = 1;
        }
    }

    gSysConfig.MusicConfig.PlayOrder = AUDIO_INTURN;
    rk_printf("AUDIO_INTURN  \n");

    pAudioControlTaskData->AudioFileInfo.PlayOrder   =   gSysConfig.MusicConfig.PlayOrder;

#ifdef _RK_EQ_
    if (!gSysConfig.MusicConfig.BassBoost)  //BASS ON
    {
        pAudioControlTaskData->UserEQ.Mode = EqMode[7];  //BASS
    }
    else
    {
        pAudioControlTaskData->UserEQ.Mode   = EqMode[gSysConfig.MusicConfig.Eq.Mode];

        for (i = 0;i < 5;i++)
        {
            pAudioControlTaskData->UserEQ.RKCoef.dbGain[i] = UseEqTable[gSysConfig.MusicConfig.Eq.RKCoef.dbGain[i]];
        }
        //memcpy(&AudioIOBuf.EffectCtl, &pAudioControlTaskData->UserEQ, sizeof(RKEffect));
    }
#endif


    if ((pAudioControlTaskData->AudioFileInfo.RepeatMode == AUDIO_FOLDER_ONCE)||(pAudioControlTaskData->AudioFileInfo.RepeatMode == AUIDO_FOLDER_REPEAT))
    {
        pAudioControlTaskData->AudioFileInfo.AudioFileDirOrAll =  FIND_FILE_RANGE_DIR;//pAudioRegKey->RepeatMode;//by zs 06.01 解决目录一次的问题
    }


    if ((pAudioControlTaskData->AudioFileInfo.ucSelPlayType != SORT_TYPE_SEL_BROWSER)
        && (pAudioControlTaskData->AudioFileInfo.ucSelPlayType != SORT_TYPE_SEL_FOLDER)
        && (pAudioControlTaskData->AudioFileInfo.ucSelPlayType != SORT_TYPE_SEL_M3U_BROWSER))
    {
        //dgl audio AudioFileInfo.PlayOrder     = pAudioRegKey->AudioFileDirOrAll;
        pAudioControlTaskData->AudioFileInfo.PlayOrder     = gSysConfig.MusicConfig.PlayOrder;//debug the bug:the play type is order play type when power off then start machine again.
        pAudioControlTaskData->AudioFileInfo.pExtStr = RecordFileExtString;//SCH 11.04
    }
    else
    {
        SysFindFileInit(&pAudioControlTaskData->AudioFileInfo, pAudioControlTaskData->AudioFileInfo.CurrentFileNum,
            pAudioControlTaskData->AudioFileInfo.AudioFileDirOrAll, pAudioControlTaskData->AudioFileInfo.PlayOrder, (UINT8*)AudioFileExtString);
    }

#if 0

    if (pAudioControlTaskData->AudioFileInfo.TotalFiles > SORT_FILENUM_DEFINE)
    {
        pAudioControlTaskData->AudioFileInfo.TotalFiles = SORT_FILENUM_DEFINE;
        if (pAudioControlTaskData->AudioFileInfo.CurrentFileNum > pAudioControlTaskData->AudioFileInfo.TotalFiles)
            pAudioControlTaskData->AudioFileInfo.CurrentFileNum = pAudioControlTaskData->AudioFileInfo.TotalFiles - 1;
    }


    if (pAudioControlTaskData->pAudioRegKey->PlayOrder == AUDIO_RAND)
    {
        pAudioControlTaskData->AudioFileInfo.CurrentFileNum = 0;

        if (pAudioControlTaskData->AudioFileInfo.TotalFiles == 1)
        {
            server_ori_seed = 0;
        }
        else if (pAudioControlTaskData->AudioFileInfo.TotalFiles == 2)
        {
            if (pAudioControlTaskData->AudioFileInfo.CurrentFileNum == 1)
            {
                server_ori_seed = 1;
            }
            else
            {
                server_ori_seed = 0;
            }
        }
        else
        {
            server_ori_seed =(SysTickCounter % pAudioControlTaskData->AudioFileInfo.TotalFiles);

            i = 0;
            do
            {
                if ((server_ori_seed == gSysConfig.MusicConfig.ShuffleSeed)
                        || (server_ori_seed == (pAudioControlTaskData->AudioFileInfo.CurrentFileNum - 1)))
                {
                    //new seed
                    server_ori_seed =(SysTickCounter % pAudioControlTaskData->AudioFileInfo.TotalFiles);
                    DelayMs(10);
                }
                else
                {
                    break;
                }
            }
            while (i < 200);

        }
        CreateRandomList(pAudioControlTaskData->AudioFileInfo.TotalFiles, server_ori_seed, MaxShuffleAllCount);
        gSysConfig.MusicConfig.ShuffleSeed = server_ori_seed;
        rand_first_flag = 1;
    }
#endif
    pAudioControlTaskData->hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);
    if ((pAudioControlTaskData->hFileDev == NULL)
        || (pAudioControlTaskData->hFileDev == (HDC)RK_ERROR)
        || (pAudioControlTaskData->hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("au File device open failure");
        RKDev_Close(pAudioControlTaskData->hFileDev);
        while(1);
    }

    pAudioControlTaskData->hAudio = RKDev_Open(DEV_CLASS_AUDIO, 0, NOT_CARE);

    if ((pAudioControlTaskData->hAudio == NULL)
        || (pAudioControlTaskData->hAudio == (HDC)RK_ERROR)
        || (pAudioControlTaskData->hAudio == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Audio device open failure");
        while(1);
    }

    pAudioControlTaskData->TrackNo = AudioDev_GetMainTrack(pAudioControlTaskData->hAudio);

    pAudioControlTaskData->hBcore = RKDev_Open(DEV_CLASS_BCORE, 0, NOT_CARE);

    if ((pAudioControlTaskData->hBcore == NULL)
        || (pAudioControlTaskData->hBcore == (HDC)RK_ERROR)
        || (pAudioControlTaskData->hBcore == (HDC)RK_PARA_ERR))
    {
        rk_print_string("Bcore device open failure");
        while(1);
    }


    //FREQ_ExitModule(FREQ_MAX);
    rk_printf("FREQ_ExitModule  \n");

    gpstAudioControlData = pAudioControlTaskData;

#ifndef A_CORE_DECODE
    osAudioDecodeOk = rkos_semaphore_create(1, 0);
    RegMBoxDecodeSvc();
#endif
    rk_printf("RegMBoxDecodeSvc  \n");


    if((pArg->ucSelPlayType == SOURRE_FROM_DLNA) || (pArg->ucSelPlayType == SOURRE_FROM_WIFI))
    {
        DecDataBuf[0] = rkos_memory_malloc(1024 * 16);
        if(NULL == DecDataBuf[0])
        {
            rk_printf("ERROR: DecDataBuf[0] malloc faile\n");
            RK_ERROR;
        }

        DecDataBuf[1] = rkos_memory_malloc(1024 * 16);
        if(NULL == DecDataBuf[1])
        {
            rk_printf("ERROR: DecDataBuf[1] malloc faile\n");
            RK_ERROR;
        }
    }
    else
    {
        DecDataBuf[0] = rkos_memory_malloc(1024 * 36);
        if(NULL == DecDataBuf[0])
        {
            rk_printf("ERROR: DecDataBuf[0] malloc faile\n");
            RK_ERROR;
        }

        DecDataBuf[1] = rkos_memory_malloc(1024 * 36);
        if(NULL == DecDataBuf[1])
        {
            rk_printf("ERROR: DecDataBuf[1] malloc faile\n");
            RK_ERROR;
        }
    }

    rk_printf("DecDataBuf rkos_memory_malloc  \n");

    if(pAudioControlTaskData->AudioFileInfo.ucSelPlayType == SORT_TYPE_SEL_M3U_BROWSER)
    {
        //gM3uGlobalPointer = gM3uCurPageStartNum+gPlayListCurPointer;
    }

    //url = 0;
    //stFileAttr.Path = L"C:\\";
    //stFileAttr.FileName = NULL;
    //AudioFlieSour(gpstAudioControlData->hFifo, &url, stFileAttr);

#if 1
        stFileAttr.Path = L"C:\\";
        stFileAttr.FileName = NULL;//L"woshicjhcjhcjh";
        //debug_hex((char * )stFileAttr.Path, 8, 16);
        //printf("!@!@!@!@!@stFileAttr.FileName:%s\n", stFileAttr.FileName);
        printf("月亮之上\n");

        //gpstAudioControlData->hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);
        if ((gpstAudioControlData->hFileDev == NULL) || (gpstAudioControlData->hFileDev == (HDC)RK_ERROR) || (gpstAudioControlData->hFileDev == (HDC)RK_PARA_ERR))
        {
            rk_print_string("next music file open failure");
            return RK_SUCCESS;
        }

        gpstAudioControlData->hDir = FileDev_OpenDir(gpstAudioControlData->hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
        if ((rk_err_t)gpstAudioControlData->hDir <= 0)
        {
            rk_print_string("init dir open failure");
            RKDev_Close(gpstAudioControlData->hFileDev);
            return RK_SUCCESS;
        }

        printf("\ntask init dir and file as Follow:");

        gpstAudioControlData->AudioFileInfo.TotalFileNum = 0;
        BufLen = 0;
        while(1)
        {
            rk_print_string("\n");

            if (FileDev_NextDir(gpstAudioControlData->hDir, 0, &stFileAttr) != RK_SUCCESS)
            {
                break;
            }

            if (FileDev_GetFileName(gpstAudioControlData->hDir, LongName) != RK_SUCCESS)
            {
                rk_print_string("\ndir get long name failure");
                break;
            }

            BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;
            BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;

            memcpy(Buf + BufLen, "<DIR>", 5);

            BufLen += 5;

            Buf[BufLen] = ' ';
            BufLen++;
            BufLen += Unicode2Ascii(Buf + BufLen, LongName, 128);
            printf("Buf =%s \n" ,Buf);

            //UartDev_Write(gpstAudioControlData->hFileDev, Buf, BufLen, SYNC_MODE, NULL);

            gpstAudioControlData->AudioFileInfo.TotalFileNum++;
            BufLen = 0;

        }
        FileDev_CloseDir(gpstAudioControlData->hDir);

        stFileAttr.Path = L"C:\\";
        stFileAttr.FileName = NULL;

        gpstAudioControlData->hDir = FileDev_OpenDir(gpstAudioControlData->hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
        if ((rk_err_t)gpstAudioControlData->hDir <= 0)
        {
            rk_print_string("init2 dir open failure");
            RKDev_Close(gpstAudioControlData->hFileDev);
            return RK_ERROR;
        }
        gpstAudioControlData->AudioFileInfo.TotalFiles = 0;

        BufLen = 0;
        while(1)
        {
            if (FileDev_NextFile(gpstAudioControlData->hDir, 0, NULL, &stFileAttr) != RK_SUCCESS)
            {
                break;
            }

            if (FileDev_GetFileName(gpstAudioControlData->hDir, LongName) != RK_SUCCESS)
            {
                rk_print_string("file get long name failure");
                break;
            }

            BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;
            BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
            Buf[BufLen] = ' ';
            BufLen++;

            memcpy(Buf + BufLen, "<FILE>", 6);

            BufLen += 6;

            memcpy(Buf+ BufLen, stFileAttr.ShortName, 8);
            BufLen += 8;
            *(Buf + BufLen) = '.';
            BufLen += 1;
            memcpy(Buf+ BufLen, stFileAttr.ShortName + 8, 3);
            BufLen += 3;

            printf("\nBuf =%s" ,Buf);

            rk_print_string("\r\n");

            gpstAudioControlData->AudioFileInfo.TotalFiles++;
            BufLen = 0;
        }
        gpstAudioControlData->AudioFileInfo.AudioFileDirOrAll = gpstAudioControlData->AudioFileInfo.TotalFileNum+gpstAudioControlData->AudioFileInfo.TotalFiles;
        BufLen = sprintf(Buf, "   total dir %d, totoal file %d", gpstAudioControlData->AudioFileInfo.TotalFileNum, gpstAudioControlData->AudioFileInfo.TotalFiles);
        //UartDev_Write(gpstAudioControlData->hFileDev, Buf, BufLen, SYNC_MODE, NULL);
        rk_printf("total Buf =%s" ,Buf);

        FileDev_CloseDir(gpstAudioControlData->hDir);
        RKDev_Close(gpstAudioControlData->hFileDev);

        //rk_printf("\n$@@@path:%s FileName:%s CurrentFileNum=%d\n",stFileAttr.Path,stFileAttr.FileName,gpstAudioControlData->AudioFileInfo.CurrentFileNum);
        gpstAudioControlData->AudioFileInfo.CurrentFileNum = 0;//first init 0

        stFileAttr.Path = (uint16 *)&Buf;
        stFileAttr.FileName = (uint16 *)&LongName;

        stFileAttr.Path = L"C:\\";
        debug_hex((char * )stFileAttr.Path, StrLenW(stFileAttr.Path), 16);
        rk_printf("\n DDDDDDDStrLenW(stFileAttr.Path)= %d\n",StrLenW(stFileAttr.Path));
        stFileAttr.FileName = NULL;
        ret = 1;
        i = 0;
        while(ret != 0)
        {
            if(i++ > gpstAudioControlData->AudioFileInfo.TotalFiles)
            {
                rk_printf("\n!!!!NO Find APE Type Music i=%d totalfiles=%d\n", i, gpstAudioControlData->AudioFileInfo.TotalFiles);
                i = 0;
                break;
            }
            ret = GetFileInfoSetNewMusic(&stFileAttr, gpstAudioControlData->AudioFileInfo.CurrentFileNum+1, gpstAudioControlData->pAudioRegKey);
            rk_printf("ret = %i \n",ret);
        }
        gpstAudioControlData->pAudioRegKey->RepeatMode = AUDIO_FOLDER_ONCE;
        //printf("\n$#$#$#$# new song file path:\n");
        //debug_hex((char *)gpstAudioControlData->AudioFileInfo.path, 50, 6);

        //printf("\n$#$#$#$# new song file name:\n");
        //debug_hex((char *)gpstAudioControlData->MusicLongFileName, 50, 6);

        //printf("\n$#path:%s FileName:%s CurrentFileNum=%d\n",gpstAudioControlData->AudioFileInfo.path,stFileAttr.FileName,gpstAudioControlData->AudioFileInfo.CurrentFileNum);

        //printf("inti end path :%s\n",gpstAudioControlData->MusicLongFileName);

#endif

    return RK_SUCCESS;

MusicInit_ERR:

    //FREQ_ExitModule(FREQ_MAX);
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*******************************************************************************
** Name: RegMBoxDecodeSvc
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 14:55:01
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_INIT_
INIT FUN void RegMBoxDecodeSvc(void)
{
    IntRegister(INT_ID_MAILBOX1, (void*)AudioDecodingGetOutBuffer);
    IntPendingClear(INT_ID_MAILBOX1);
    IntEnable(INT_ID_MAILBOX1);
    MailBoxEnableB2AInt(MAILBOX_ID_0, MAILBOX_INT_1);
}

/*******************************************************************************
** Name: SysFindFileInit
** Input:SYS_FILE_INFO *pSysFileInfo,UINT16 GlobalFileNum,UINT16 FindFileRange,UINT16 PlayMode, uint8 *pExtStr
** Return: void
** Owner:aaron.sun
** Date: 2015.6.23
** Time: 16:18:20
*******************************************************************************/
_APP_AUDIO_AUDIOCONTROLTASK_INIT_
INIT FUN void SysFindFileInit(SYS_FILE_INFO *pSysFileInfo,UINT16 GlobalFileNum,UINT16 FindFileRange,UINT16 PlayMode, uint8 *pExtStr)
{
    UINT16 i,j;
    UINT16 tempFileNum;
    INT16  FindFileResult;
    INT16  RetVal = 0;
    FS_TYPE FsType;

    if(pSysFileInfo->ucSelPlayType == SORT_TYPE_SEL_FOLDER)
    {
        FsType = MUSIC_DB;
    }
        #ifdef _RECORD_
    else if(pSysFileInfo->ucSelPlayType == MUSIC_TYPE_SEL_FMFILE)
    {
        FsType = RECORD_DB;
    }
        #endif
    else
    {
        FsType = FS_FAT;
    }

    if (0 == pSysFileInfo->TotalFiles)
    {
        return;
    }

    pSysFileInfo->DiskTotalFiles = pSysFileInfo->TotalFiles;
    pSysFileInfo->CurrentFileNum = GlobalFileNum;
    pSysFileInfo->pExtStr        = pExtStr;
    pSysFileInfo->Range          = FindFileRange;
    pSysFileInfo->PlayOrder       = PlayMode;


    if (pSysFileInfo->CurrentFileNum > pSysFileInfo->TotalFiles)
    {
        pSysFileInfo->CurrentFileNum = 1;
    }

    DirDev_GetCurFileNum(gpstAudioControlData->hDir, pSysFileInfo->CurrentFileNum, &pSysFileInfo->CurrentFileNum);

    pSysFileInfo->PlayedFileNum = pSysFileInfo->CurrentFileNum;

    return;
}


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

