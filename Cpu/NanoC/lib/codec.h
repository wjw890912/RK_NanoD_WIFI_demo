/*
********************************************************************************
*                   Copyright (c) 2008,ZhengYongzhi
*                         All rights reserved.
*
* File Name£º   Hw_codec.h
*
* Description:
*
* History:      <author>          <time>        <version>
*                               2008-8-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_CODEC_H
#define _HW_CODEC_H

#define CODEC_ROCKC     0
#define CODEC_WM8987    1
//page
#define VOL_General     0  //for suit FrenchRegionVal, set VOL_General = 0,  RK original = 1
#define VOL_Europe      1  //for suit FrenchRegionVal, set VOL_General = 1,  RK original = 0

#define VOLTAB_CONFIG    gSysConfig.VolTableIndex

#define CODEC_CONFIG    CODEC_ROCKC

#define MAX_VOLUME      30

typedef enum CodecMode
{
    Codec_DACout,  // for MP3, MP4 playback
    Codec_FMin,   // for FM playback
    Codec_FMADC,  // for FM recording, output remain FM singal
    Codec_LineADC,  // for Line in recording
    Codec_MICAdc,  // for MIC recording
    Codec_Standby  // for player no application work, like main menu, or exit from FM
}CodecMode_en_t;

typedef enum CodecFS
{
    CodecFS_8000Hz   = 8000,
    CodecFS_11025Hz  = 11025,
    CodecFS_12KHz    = 12000,
    CodecFS_16KHz    = 16000,
    CodecFS_22050Hz  = 22050,
    CodecFS_24KHz    = 24000,
    CodecFS_32KHz    = 32000,
    CodecFS_44100Hz  = 44100,
    CodecFS_48KHz    = 48000,
    CodecFSSTOP      = 96000
} CodecFS_en_t;


void Codec_PowerOnInitial(void);
void Codec_SetMode(CodecMode_en_t Codecmode);
void Codec_SetSampleRate(CodecFS_en_t CodecFS);
void Codec_SetVolumet(UINT8 DacAnaVol,UINT8 DacDigVol);
void Codec_DACMute(void);
void Codec_DACUnMute(void);
void Codec_DeInitial(void);
void Codec_ExitMode(void);

/*
********************************************************************************
*
*                         End of Codec.h
*
********************************************************************************
*/

#endif

