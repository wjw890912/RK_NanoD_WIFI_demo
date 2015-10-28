/******************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
*  File name :     effect.h
*  Description:   audio effect process.
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*           Huweiguo     07/09/27      1.0
*
*******************************************************************************/

#ifndef __EFFECT_H__
#define __EFFECT_H__


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _CODECS_AUDIO_EFFECT_EFFECT_COMMON_  __attribute__((section("codecs_audio_effect_effect_common")))
//#define _CODECS_AUDIO_EFFECT_EFFECT_INIT_  __attribute__((section("codecs_audio_effect_effect_init")))
//#define _CODECS_AUDIO_EFFECT_EFFECT_SHELL_  __attribute__((section("codecs_audio_effect_effect_shell")))

/* EQ mode */
typedef enum
{
    EQ_HEAVY,
    EQ_POP,
    EQ_JAZZ,
    EQ_UNIQUE,
    EQ_CLASS,
    EQ_BASS,
    EQ_ROCK,
    EQ_USER,
    EQ_NOR

} eEQMode;

#ifdef CODEC_24BIT
typedef long    EQ_TYPE;
typedef long long   EQ_TYPE_LONG;
#else
typedef short    EQ_TYPE;
typedef long    EQ_TYPE_LONG;

#endif
/*parameter structure of RK audio effect.*/
typedef __packed struct
{
    short    dbGain[5]; //5 band EQµÄÔöÒæ
} tRKEQCoef;


typedef __packed struct
{
    eEQMode Mode;      // EQ mode
    unsigned short max_DbGain;
   //#ifdef _RK_EQ_
    tRKEQCoef  RKCoef;
   // #endif

    //tPFEQCoef  PFCoef;
} RKEffect;

#define CUSTOMEQ_LEVELNUM        7

/* API interface function. */

rk_err_t Effect_Delete();
long EffectInit(void);       // initialization.
long EffectEnd(RKEffect *userEQ);      //handle over.
long EffectProcess(EQ_TYPE *pBuffer, uint32 PcmLen, RKEffect *userEQ, int *isEQUpdate);   //audio effect process function,call it every frame.

//this function is for adjust audio effect.
long EffectAdjust(RKEffect *userEQ, void* eqARG, uint32 sampleRate);
long RKEQAdjust(RKEffect *pEft, uint32 sampleRate);

#endif

