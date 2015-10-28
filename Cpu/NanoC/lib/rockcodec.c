/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name£º   Rockcodec.c
*
* Description:
*
* History:      <author>          <time>        <version>
*               anzhiguo          2009-3-24         1.0
*    desc:    ORG.
********************************************************************************
*/
#include "typedef.h"
#include "DriverInclude.h"

#if (CODEC_CONFIG == CODEC_ROCKC)

#include "rockcodec.h"

#define  MAX_OUTLEVEL            10






/*
--------------------------------------------------------------------------------
  Function name : void Codec_PowerOnInitial(void)
  Author        : yangwenjie
  Description   : Codec power on initial

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
void Codec_PowerOnInitial(void)
{
    //Disable Force to VDD1.2 or VDDIO
    Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_1200MV_MASK) << 16) | MPMU_HP_FORCE_1200MV_INACTIVE;
    Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_VDDIO_MASK) << 16) | MPMU_HP_FORCE_VDDIO_INACTIVE;

    //Mute
    Grf->MPMU_HP_CON  = (MPMU_HP_MIX_MASK << 16) | MPMU_HP_MIX_NO_INPUT;
    Grf->MPMU_DAC_CON = (MPMU_DAC_MUTE_MASK << 16) | MPMU_DAC_MUTE;

    //Power Down
    Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_DOWM;
    Grf->MPMU_HP_CON  = (MPMU_HP_POWER_MASK << 16) | MPMU_HP_POWER_DOWM;
}

/*
--------------------------------------------------------------------------------
  Function name : void Codec_DeIitial(void)
  Author        : yangwenjie
  Description   : close Codec

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
void Codec_DeInitial(void)
{
    //Mute
    Grf->MPMU_HP_CON  = (MPMU_HP_MIX_MASK << 16) | MPMU_HP_MIX_NO_INPUT;
    Grf->MPMU_DAC_CON = (MPMU_DAC_MUTE_MASK << 16) | MPMU_DAC_MUTE;

    //Power Down
    Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_DOWM;
    Grf->MPMU_HP_CON  = (MPMU_HP_POWER_MASK << 16) | MPMU_HP_POWER_DOWM;
}

/*
--------------------------------------------------------------------------------
  Function name : void Codec_SetMode(CodecMode_en_t Codecmode)
  Author        : yangwenjie
  Description   :

  Input         : Codecmode£º

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:      if exit from application, like FM or MIC , please set codec to standby mode
--------------------------------------------------------------------------------
*/
void Codec_SetMode(CodecMode_en_t Codecmode)
{
    Grf->MPMU_HP_CON  = (MPMU_HP_POWER_MASK << 16) | MPMU_HP_POWER_UP;
    switch (Codecmode)
    {
    case Codec_DACout:
        Codec_SetVolumet(0,255);
        Grf->MPMU_HP_CON  = ((MPMU_HP_MIX_MASK) << 16) | (MPMU_HP_MIX_DAC_IN);
        DelayMs(20);
        Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_UP;
        Codec_DACUnMute();
        break;

    case Codec_FMin:
        Grf->MPMU_HP_CON = ((MPMU_HP_MIX_MASK) << 16) | (MPMU_HP_MIX_LINE_IN);
        DelayMs(20);
        Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_DOWM;
        break;

    default:
        Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_DOWM;
        //Grf->MPMU_HP_CON  = ((MPMU_HP_MIX_MASK | MPMU_HP_POWER_MASK) << 16) | (MPMU_HP_MIX_NO_INPUT | MPMU_HP_POWER_DOWM);
        break;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Codec_SetMode(CodecMode_en_t Codecmode)
  Author        : yangwenjie
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  note:      if exit from application, like FM or MIC , please set codec to standby mode
--------------------------------------------------------------------------------
*/
void Codec_ExitMode(void)
{
    Codec_DACMute();
    DelayMs(20);

    //no input
    Grf->MPMU_HP_CON = ((MPMU_HP_MIX_MASK) << 16) | (MPMU_HP_MIX_NO_INPUT);
    DelayMs(20);

    //Power Down
    Grf->MPMU_DAC_CON = (MPMU_DAC_POWER_MASK << 16) | MPMU_DAC_POWER_DOWM;
    Grf->MPMU_HP_CON  = (MPMU_HP_POWER_MASK << 16) | MPMU_HP_POWER_DOWM;
}
/*
--------------------------------------------------------------------------------
  Function name : void Codec_SetSampleRate(CodecFS_en_t CodecFS)
  Author        : yangwenjie
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
void Codec_SetSampleRate(CodecFS_en_t CodecFS)
{
    switch (CodecFS)
    {
        case CodecFS_8000Hz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ8kHz;
            break;

        case CodecFS_11025Hz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ11025Hz;
            break;

        case CodecFS_12KHz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ12kHz;
            break;

        case CodecFS_16KHz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ16kHz;
            break;

        case CodecFS_22050Hz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ22050Hz;
            break;

        case CodecFS_24KHz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ24kHz;
            break;

        case CodecFS_32KHz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ32kHz;
            break;

        case CodecFS_48KHz:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ48kHz;
            break;

        case CodecFS_44100Hz:
        default:
            Grf->MPMU_HP_CON = (MPMU_FREQ_MASK << 16) | MPMU_FREQ441kHz;
            break;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void Codec_SetVolumet(unsigned int Volume)
  Author        : yangwenjie
  Description   : codec control volume

  Input         : Volume

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:      volume = 0 mean mute,
--------------------------------------------------------------------------------
*/
void Codec_SetVolumet(UINT8 DacAnaVol,UINT8 DacDigVol)
{
    Grf->MPMU_DAC_CON = (MPMU_DAC_VOL_UPD_MASK << 16) | MPMU_DAC_VOL_UPD_INACTIVE;

    //Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_VDDIO_MASK) << 16) | MPMU_HP_FORCE_VDDIO_ACTIVE;
    //Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_1200MV_MASK) << 16) | MPMU_HP_FORCE_1200MV_ACTIVE;

    if (DacDigVol >= 254)
    {
         Grf->MPMU_DAC_CON = (((MPMU_DAC_DIG_VOL_MASK ) << 16) | (DacDigVol | DacAnaVol));
        //Grf->MPMU_DAC_CON = ((MPMU_DAC_MUTE_MASK << 16) | MPMU_DAC_MUTE);
    }
    else
    {
        //Volume += OutputVolOffset;
                    Grf->MPMU_DAC_CON = (((MPMU_DAC_DIG_VOL_MASK | MPMU_DAC_ANA_VOL_MASK) << 16) |
                        DacDigVol |
                        DacAnaVol);
    }

    Grf->MPMU_DAC_CON = (MPMU_DAC_VOL_UPD_MASK << 16) | MPMU_DAC_VOL_UPD_ACTIVE;

}
/*
--------------------------------------------------------------------------------
  Function name : void Codec_DACMute(void)
  Author        : yangwenjie
  Description   : set codec mute

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  Note:      this function only used when DAC working
--------------------------------------------------------------------------------
*/
void Codec_DACMute(void)
{
    Grf->MPMU_DAC_CON = ((MPMU_DAC_MUTE_MASK) << 16) | (MPMU_DAC_MUTE);
    //Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_1200MV_MASK) << 16) | MPMU_HP_FORCE_1200MV_ACTIVE;
}

/*
--------------------------------------------------------------------------------
  Function name : void Codec_DACUnMute(void)
  Author        : yangwenjie
  Description   : set codec exit from mute.

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  note:      this function only used when DAC working
--------------------------------------------------------------------------------
*/
void Codec_DACUnMute(void)
{
    Grf->MPMU_DAC_CON = ((MPMU_DAC_MUTE_MASK) << 16) | (MPMU_DAC_UNMUTE);
    //Codec_SetVolumet(gSysConfig.OutputVolume);
}

/*
********************************************************************************
*
*                         End of Rockcodec.c
*
********************************************************************************
*/
#endif

