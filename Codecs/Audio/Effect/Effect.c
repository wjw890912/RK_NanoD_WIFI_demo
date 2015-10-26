/******************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
*  File name :     effect.c
*  Description:    音频效果处理
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*           Huweiguo     07/09/27      1.0
*
*******************************************************************************/
#include "BspConfig.h"
#ifdef _RK_EQ_
#include "RKOS.h"
#include "DeviceInclude.h"
#include "typedef.h"
#include "Effect.h"
#include "audio_globals.h"
//extern AudioInOut_Type       AudioIOBuf;
extern int CurrentCodec;

#ifdef _RK_EQ_5_
#define EQ_NUM  5
#endif
#ifdef _RK_EQ_31_
#define EQ_NUM  31
#endif

/**************************************************************************
* function description: set parameters of RockEQ
* input:  [pEft]   --
* output:
* return:    0:success; other:failure.
* note:
***************************************************************************/
#define EQADDVOL     6

#ifdef _RK_EQ_5_
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short PresetGain[4][EQ_NUM] =
{
    {15, 12, 9, 15, 12}, // HEAVY
    {12, 15, 15, 9, 12}, // POP
    {18, 12, 12, 9, 15}, // JAZZ
    {18, 6, 16, 13, 18}  // UNIQUE
                         //EQ_CLASS,
                         //EQ_BASS,
                         //EQ_ROCK,
};

_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short EqBassBoostGain[EQ_NUM] = {12, 12, 12, 12, 12};    // BASS BOOST
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short UseEQ[EQ_NUM];
#endif

#ifdef _RK_EQ_31_
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short PresetGain[4][EQ_NUM] =   //理论12   低音很好   修改一段 的0为 -17db 所用的很好的参数表
 {
 {12, 12, 12, 12, 12,
 12,  12, 15, 12, 12,   //3//
 12,  12, 12, 12, 12,
 12,  12, 9,  12, 12,
 12,  12, 15, 12, 12,
 12,  12, 12, 12, 12, 12
 },
{12, 12,  12, 12, 12,
 12,  12, 12, 12, 12,
 12,  12, 15, 12, 12,
 12,  12, 15, 12, 12,
 12,  12,  9, 12, 12,
 12,  12, 12, 12, 12, 12
 },
{12, 12,  12, 12, 12,
 12,  12, 18, 12, 12,
 12,  12, 12, 12, 12,
 12,  12, 12, 12, 12,
 12,  12,  9, 12, 12,
 12,  12, 15, 12, 12, 12
 },
 {12, 12, 12, 12, 12,
 12,  12, 18, 12, 12,
 12,  12, 6, 12, 12,
 12,  12, 16, 12, 12,
 12,  12, 13, 12, 12,
 12,  12, 18, 12, 12, 12
 }
/*
     {0, 1, 5, 5, 8,
 11,  14, 15, 14, 13,   //重低音效果最好，但频率不足
 15,  14, 13, 11, 10,
 11,  11, 12, 12, 13,
 13,  13, 13, 13, 13,
 12,  12, 12, 12, 12, 12
 }*/
 };

/*
     {0, 1, 5, 5, 8,
 11,  14, 15, 14, 13,   //重低音效果最好，但频率不足
 15,  14, 13, 11, 10,
 11,  11, 12, 12, 13,
 13,  13, 13, 13, 13,
 12,  12, 12, 12, 12, 12
 }*/
 //};     // BASS BOOST

 _CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short EqBassBoostGain[EQ_NUM] =
{    12,  12, 12, 12, 12,
     22,  12, 12, 17, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12, 12  // BASS BOOST
 };
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
short UseEQ[EQ_NUM] =
{    12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12,
     12,  12, 12, 12, 12, 12  // BASS BOOST
 };
#endif
/**************************************************************************
* function description: audio effect is end.
* input   :  null
* output  : null
* return  :    0:success; other:failure.
* note:
***************************************************************************/
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
void ReadEqData(uint8 *p , uint32 off ,uint32 size)
{
    int ret, i;
    MODULE_INFO_T stCodeInfo;
    uint32 page_addr;

    ret = FW_GetModuleInfo(MODULE_ID_AUDIO_EQ_DATA, &stCodeInfo);
    if (ret)
    {
        printf("FW_GetModuleInfo error\n");
    }

    page_addr = stCodeInfo.CodeLoadBase;

    //printf("FW_GetModuleInfo page_addr=%d off=%d size=%d\n",page_addr,off,size);
    if((page_addr + off)>0xffffffff)
    {
        printf("page_addr+offset overflow\n");
    }

    FW_ReadResourceData((page_addr + off), p, size);
/*
    {
        for(i = 0; i < 512; i++)
        {
            if(i % 16 == 0)
                printf("\n");

            printf("%02x ", p[i]);
        }
    }
*/
    //printf("over FW_ReadResourceData \n");

    //debug_hex(p, size, 16);
    //ReadModuleData(MODULE_ID_AUDIO_EQ_DATA , p , off , size );
}

/**************************************************************************
* function description: audio effect is end.
* input   :  null
* output  : null
* return  :    0:success; other:failure.
* note:
***************************************************************************/
/*
typedef struct _EQ_TABLE_BLOCK
{
    short PresetGain[4][EQ_NUM];
    short EqBassBoostGain[EQ_NUM];
    short UseEQ[EQ_NUM];
}EQ_TABLE_BLOCK;
*/
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
long RKEQAdjust(RKEffect *pEft ,uint32 sampleRate)
{
    int i;
    unsigned long SamplingRate = 44100;
/*
    EQ_TABLE_BLOCK* pTableBlock;
    pTableBlock = rkos_memory_malloc(sizeof(EQ_TABLE_BLOCK));
    if (pTableBlock == NULL)
    {
        printf("ERROR:pTableBlock malloc fail\n\n");
        return RK_ERROR;
    }
*/
    if(sampleRate)
    {
        SamplingRate = sampleRate;
    }
    //CodecGetSampleRate(&SamplingRate);
    //printf("#####RKEQAdjust CodecGetSampleRate =%d\n", SamplingRate);

    switch(SamplingRate)
        {
            case 8000:
            case 11025:
            case 12000 :
                SamplingRate = 11025;
                 break;
            case 16000:
                case 22050:
            case 24000:
                    SamplingRate = 22050;
                 break;
            case 32000:
             case 44100:
            case 48000:
            case 64000:
            case 88200:
            case 96000:
            case 128000:
            case 176400:
            case 192000:

                 SamplingRate = 44100;
                 break;
       }

    switch (pEft->Mode)
    {
        case EQ_ROCK:
        case EQ_POP:
        case EQ_CLASS:
        case EQ_JAZZ:
        case EQ_HEAVY:
        case EQ_UNIQUE:
            RockEQAdjust(SamplingRate, PresetGain[pEft->Mode], 1, pEft);
            break;

        case EQ_USER:
            #ifdef _RK_EQ_5_
            for (i = 0;i < 5;i++)
            {
                UseEQ[i] = pEft->RKCoef.dbGain[i] + 12;
            }
            #else
            for (i = 0;i < 5;i++)
            {
                UseEQ[7+i*5] = pEft->RKCoef.dbGain[i] + 12;
            }
            #endif
            RockEQAdjust(SamplingRate, UseEQ, 1, pEft);
            break;

        case EQ_BASS:
            RockEQAdjust(SamplingRate, EqBassBoostGain, 1, pEft);
            break;
        default:
            break;
    }
    //rkos_memory_free(pTableBlock);
    return 0;
}

/**************************************************************************
* function description: audio effect is processing.
* input   :[pBuffer] -- the buffer put pcm data.
           [PcmLen] --  the length of pcm data.
* output  : null
* return  : 0:success; other:failure.
* note    : the data that audio effect would take care need use data format LRLRLR.
***************************************************************************/
_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
long EffectProcess(EQ_TYPE *pBuffer, uint32 PcmLen, RKEffect *userEQ, int *isEQUpdate)
{
    //AudioInOut_Type  *pAudio = &AudioIOBuf;
    //RKEffect   *pEffect = &pAudio->EffectCtl;
    RKEffect   *pEffect = userEQ;
    if(*isEQUpdate)
    {
        printf("isEQUpdate = 1\n");
        return 0;
    }

    if (pBuffer == 0)
    {
        printf("pBuffer== 0\n");
        return 1;
    }

    if (PcmLen <= 0)
    {
        printf("pBuffer<= 0\n");
        return 0;
    }
//    printf("&*&EffectProcess\n");
    switch (pEffect->Mode)
    {
        case EQ_NOR:
            break;

        case EQ_ROCK:
        case EQ_POP:
        case EQ_CLASS:
        case EQ_BASS:
        case EQ_JAZZ:
        case EQ_HEAVY:
        case EQ_UNIQUE:
        case EQ_USER:
            if (!(PcmLen & (64-1))) //by Vincent
            {
                //printf("6464PcmLen = %d divisible by 64 userEQ=%d\n",PcmLen,userEQ->Mode);
                RockEQProcess(pBuffer, PcmLen, userEQ);
            }
            else
            {
                printf("PcmLen = %d can not divisible by 64\n",PcmLen);
            }
            break;

        default:
            break;
    }

    return 0; //success
}

/**************************************************************************
* description    : audio effect adjust.
* input           : null
* output          : null
* return          : 0:success; other:failure.
* note            :
***************************************************************************/
//_ATTR_SYS_BSS_ EXT  uint32  AudioLen;
static int EQ_FR_FLAG = 0;

_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
long EffectAdjust(RKEffect* userEQ, void* eqArg, uint32 sampleRate)
{
    //AudioInOut_Type  *pAudio = &AudioIOBuf;
    //RKEffect         *pEffect = &pAudio->EffectCtl;
    AUDIO_EQ_ARG* eqARG = (AUDIO_EQ_ARG *)eqArg;
    RKEffect *pEffect = userEQ;
    unsigned long SamplingRate = 44100;
    int i,j=0;
    int Mode;
    int EQ_AudioLen ;
    rk_err_t ret;

    if(sampleRate)
    {
        SamplingRate = sampleRate;
    }
    //CodecGetSampleRate(&SamplingRate);
    //printf("#####CodecGetSampleRate =%d\n", SamplingRate);

    //UserIsrDisable();
    //rkos_enter_critical();//0
    printf("\nEffectAdjust Mode = %d\n",pEffect->Mode);
    if (pEffect->Mode == EQ_USER)
    {
        for (i=0; i<EQ_NUM; i++)
        {
            if (pEffect->RKCoef.dbGain[i] == 0)
            {
                j++;
            }
        }

        if (j == EQ_NUM)
        {
            pEffect->Mode = EQ_NOR;
            //IsEQNOR = TRUE;
        }
    }
    switch (pEffect->Mode)
    {
        case EQ_NOR:
            #ifdef _RK_EQ_31_
            {
                switch(EQ_FR_FLAG)
                {
                  case 1:
                    FREQ_ExitModule(FREQ_EQ_L);
                    EQ_FR_FLAG = 0;
                    break;
                  case 2:
                    FREQ_ExitModule(FREQ_EQ);
                    EQ_FR_FLAG = 0;
                    break;
                  case 3:
                    FREQ_ExitModule(FREQ_EQ_H);
                    EQ_FR_FLAG = 0;
                    break;
                  case 0:
                    break;
                }
            }
            #else
                FREQ_ExitModule(FREQ_EQ);
            #endif
            //AudioSetVolume();
            break;

        case EQ_POP:
        case EQ_CLASS:
        case EQ_BASS:
        case EQ_JAZZ:
        case EQ_HEAVY:
        case EQ_UNIQUE:
        case EQ_USER:
            /* TODO : here load code */
//            FW_ModuleOverlay(MODULE_ID_AUDIO_RKEQ, MODULE_OVERLAY_ALL);

            #ifdef CODEC_24BIT
                EQ_AudioLen =  *(eqARG->audioLen) >>1;
                //printf("~!~!~~24 EQ_AudioLen = %d SamplingRate =%d\n", EQ_AudioLen, SamplingRate);
            #else
                EQ_AudioLen =  *(eqARG->audioLen);
                //printf("~!~!~~16 EQ_AudioLen = %d SamplingRate =%d\n", EQ_AudioLen, SamplingRate);
            #endif

            #ifdef _RK_EQ_31_
            if((EQ_AudioLen  <= 1152)&&( SamplingRate <=48000))
            {
                FREQ_EnterModule(FREQ_EQ_L);
                EQ_FR_FLAG = 1;
            }
            else if(((EQ_AudioLen  < 4096)&&( SamplingRate <=48000))||((EQ_AudioLen  <= 1152 )&&( (SamplingRate >48000)&&(SamplingRate <=88200))))
            {
                FREQ_EnterModule(FREQ_EQ);
                EQ_FR_FLAG = 2;
            }
            else
            {
                FREQ_EnterModule(FREQ_EQ_H);
                EQ_FR_FLAG = 3;
            }
            #else
                FREQ_EnterModule(FREQ_EQ);
            #endif
            RKEQAdjust(pEffect, sampleRate);
            AudioDev_SetVolume();
            //*(eqARG->playVolume) = gSysConfig.OutputVolume;
            //SendMsg(MSG_AUDIO_EQ_PRO);
            break;

        default:
            break;
    }

    //UserIsrEnable();

    *(eqARG->isEQUpdate) = 0;

    return 0; //success.
}


/**************************************************************************
* function description: audio effect is initialization.
* input   :  null
* output  : null
* return  :    0:success; other:failure.
* note:
***************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
long EffectInit(void)
{
    FW_ModuleOverlay(MODULE_ID_EFFECT_MODULE, MODULE_OVERLAY_CODE);//MODULE_OVERLAY_ALL
    rk_eq_init();
    return 0;
}


/*******************************************************************************
** Name: RockcodecDevDelete
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:41:18
*******************************************************************************/
_DRIVER_AUDIO_AUDIODEVICE_COMMON_
rk_err_t Effect_Delete()
{
    rk_eq_deinit();
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_EFFECT_MODULE);
#endif
    return RK_SUCCESS;
}

/**************************************************************************
* function description: audio effect is end.
* input   :  null
* output  : null
* return  :    0:success; other:failure.
* note:
***************************************************************************/
 _CODECS_AUDIO_EFFECT_EFFECT_COMMON_
long EffectEnd(RKEffect *userEQ)
{
    //AudioInOut_Type  *pAudio = &AudioIOBuf;
    //RKEffect   *pEffect = &pAudio->EffectCtl;
    RKEffect   *pEffect = userEQ;
    return 0;
}


_CODECS_AUDIO_EFFECT_EFFECT_COMMON_
void RockEQReduce9dB(EQ_TYPE *pwBuffer, long cwBuffer, long mode, RKEffect *userEQ)
{
    //AudioInOut_Type  *pAudio = &AudioIOBuf;
    //RKEffect   *pEffect = &pAudio->EffectCtl;
    RKEffect   *pEffect = userEQ;
    //printf("RockEQReduce9dB Mode =%d length=%d\n", pEffect->Mode,cwBuffer);
    switch (pEffect->Mode)
       {
           case EQ_NOR:
           case EQ_ROCK:
           case EQ_CLASS:
                 break;

           case EQ_HEAVY:
           case EQ_POP:
                 while(cwBuffer--)
                {
                    //printf("pwBuffer[%d]=0x%x  point= %d\n",cwBuffer,(*pwBuffer), pwBuffer);
                    *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer)*5)>>3);//20log(0.625)=-4
                    //printf("EQ_POP EQ_HEAVY:pwBuffer[%d]=0x%x \n",cwBuffer,(*pwBuffer));
                    pwBuffer += mode;
                }
                 break;
           case EQ_JAZZ:
           case EQ_UNIQUE:
                while(cwBuffer--)
                {
                    //printf("pwBuffer[%d]=0x%x  point= %d\n",cwBuffer,(*pwBuffer), pwBuffer);
                    *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer)*7)>>4);//20log(0.438)=-7.1
                    //printf("EQ_UNIQUE EQ_JAZZ:pwBuffer[%d]=0x%x point= %d\n",cwBuffer,(*pwBuffer), pwBuffer);
                    pwBuffer += mode;
                }
                 break;
           case EQ_USER:
                    while(cwBuffer--)
                {
                    //printf("pwBuffer[%d]=0x%x  point= %d\n",cwBuffer,(*pwBuffer), pwBuffer);
                    *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer))>>2);//20log(0.25)=-12
                    //printf("EQ_USER:pwBuffer[%d]=0x%x \n",cwBuffer,(*pwBuffer));
                    pwBuffer += mode;
                }
                    break;
           case EQ_BASS:
                //printf("effect.c EQ_BASS pEffect->max_DbGain=%d length=%d mode=%d\n",pEffect->max_DbGain, cwBuffer, mode);
                      while(cwBuffer--)
                {
                    if(pEffect->max_DbGain == 12)
                    {
                        *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer))>>2);
                    } //20log(0.25)=-12  //20log(3/8)=8.5
                    else if(pEffect->max_DbGain == 10)
                    {
                        *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer)*5)>>4);
                    }
                    else if(pEffect->max_DbGain == 8)
                    {
                        *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer)*3)>>3);
                    }
                    else if(pEffect->max_DbGain == 6)
                    {
                        //printf("pwBuffer[%d]=0x%x  point= %d\n",cwBuffer,(*pwBuffer), pwBuffer);
                        *pwBuffer = (EQ_TYPE)(((EQ_TYPE_LONG)(*pwBuffer))>>1);
                        //printf("EQ:pwBuffer[%d]=0x%x \n",cwBuffer,(*pwBuffer));

                    }

                       pwBuffer += mode;
                }

                  break;
        }
}

#endif  // _RK_EQ_
