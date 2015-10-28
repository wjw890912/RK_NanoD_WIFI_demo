#include "RKOS.h"
#include "audio_main.h"

#ifndef _JTAG_DEBUG_
#ifdef BB_SYS_JTAG
_ATTR_BB_SYS_CODE_
const uint8 bbcore[] =
{
    #include "bb_core_jtag_code.bin"
};


_ATTR_BB_SYS_DATA_
uint8 bbdata[] =
{
    #include "bb_core_jtag_data.bin"
};
#else

_ATTR_BB_SYS_CODE_
const uint8 bbcore[] =
{
    #include "bb_core_code.bin"
};


_ATTR_BB_SYS_DATA_
uint8 bbdata[] =
{
    #include "bb_core_data.bin"
};


#ifndef A_CORE_DECODE

#ifdef MP3_DEC_INCLUDE
_ATTR_MP3DEC_BIN_TEXT_
const uint8 mp3_code_bin[] =
{
    #include "mp3_code.bin"
};


_ATTR_MP3DEC_BIN_DATA_
uint8 mp3_data_bin[] =
{
    #include "mp3_data.bin"
};
#endif

#ifdef WAV_DEC_INCLUDE
_ATTR_WAVDEC_BIN_TEXT_
const uint8 wav_code_bin[] =
{
    #include "wav_code.bin"
};


_ATTR_WAVDEC_BIN_DATA_
uint8 wav_data_bin[] =
{
    #include "wav_data.bin"
};
#endif

#ifdef APE_DEC_INCLUDE
_ATTR_APEDEC_BIN_TEXT_
const uint8 ape_code_bin[] =
{
    #include "ape_code.bin"
};


_ATTR_APEDEC_BIN_DATA_
uint8 ape_data_bin[] =
{
    #include "ape_data.bin"
};
#endif


#ifdef AAC_DEC_INCLUDE
_ATTR_AACDEC_BIN_TEXT_
const uint8 aac_code_bin[] =
{
    #include "aac_code.bin"
};


_ATTR_AACDEC_BIN_DATA_
uint8 aac_data_bin[] =
{
    #include "aac_data.bin"
};
#endif

#ifdef FLAC_DEC_INCLUDE
_ATTR_FLACDEC_BIN_TEXT_
const uint8 flac_code_bin[] =
{
    #include "flac_code.bin"
};


_ATTR_FLACDEC_BIN_DATA_
uint8 flac_data_bin[] =
{
    #include "flac_data.bin"
};
#endif

#ifdef WMA_DEC_INCLUDE
_ATTR_WMADEC_BIN_TEXT_
const uint8 wma_code_bin[] =
{
    #include "wma_code.bin"
};


_ATTR_WMADEC_BIN_DATA_
uint8 wma_data_bin[] =
{
    #include "wma_data.bin"
};
#endif

#ifdef OGG_DEC_INCLUDE
_ATTR_OGGDEC_BIN_TEXT_
const uint8 ogg_code_bin[] =
{
    #include "ogg_code.bin"
};


_ATTR_OGGDEC_BIN_DATA_
uint8 ogg_data_bin[] =
{
    #include "ogg_data.bin"
};

#endif

#ifdef _HIFI_APE_DEC
_ATTR_HIFI_APEDEC_BIN_TEXT_
const uint8 hifi_ape_code_bin[] =
{
    #include "hifi_ape_code.bin"
};


_ATTR_HIFI_APEDEC_BIN_DATA_
uint8 hifi_ape_data_bin[] =
{
    #include "hifi_ape_data.bin"
};

#endif

#ifdef _HIFI_FLAC_DEC
_ATTR_HIFI_FLACDEC_BIN_TEXT_
const uint8 hifi_flac_code_bin[] =
{
    #include "hifi_flac_code.bin"
};


_ATTR_HIFI_FLACDEC_BIN_DATA_
uint8 hifi_flac_data_bin[] =
{
    #include "hifi_flac_data.bin"
};

#endif

#ifdef _HIFI_ALAC_DEC
_ATTR_HIFI_ALACDEC_BIN_TEXT_
const uint8 hifi_alac_code_bin[] =
{
    #include "hifi_alac_code.bin"
};


_ATTR_HIFI_ALACDEC_BIN_DATA_
uint8 hifi_alac_data_bin[] =
{
    #include "hifi_alac_data.bin"
};

#endif



#endif
#endif
#endif
