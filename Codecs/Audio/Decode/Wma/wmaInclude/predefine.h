//
//
// define the macro switch of WMA.
//
//

#ifndef __PREDEFINE_H__
#define __PREDEFINE_H__

//#include <stdio.h>
//#include "AudioConfig.h"
//#include "..\..\Include\audio_main.h"
//#define STRUCT_MEM_OFFSET_CALCULATE


//#define PRINTF  printf
//#define FPRINTF fprintf
//#define ASSERT  assert

//predefined macro,please refer the detailed description in document <wma9stddecoder_readme.doc>
#define BUILD_INTEGER
#define ENABLE_LPC
#define ENABLE_ALL_ENCOPT
#define BITSTREAM_CORRUPTION_ASSERTS
#define BUILD_WMASTD
//#define WMAAPI_NO_DRM           //switch of DRM9
#define WMAAPI_NO_DRM_STREAM    // ?
//#ifdef WMA_DEC_INCLUDE
//#define WMDRM_PORTABLE          //switch of DRM10


#define   WMAHIGHRATE //high bit rate decode code.
#define  WMAMIDRATELOWRATE //low bit rate decode code.
#define WMAINITIALIZE //the switch of ASF decoding and initialization.
#define WMAINITIALIZE_HIGH_MID_ALL_USED
//#endif

#define WMA_TABLE_ROOM_VERIFY//to use table rom
//#define WMA_TABLE_ROOM_GENERATE  //generate table rom

#define rk_nano_board
//#define PUT_ONE_OUTPUT_BUFFER_IN_IRAM
#define PUT_ONE_RECON_BUFFER_IN_IRAM
#define WMA_TAB_BASE_ADDR (0x02000000+0x6c50)
//#define USE_SYS_FILE_ACCESS//
#define TABLE_BUF_SIZE (13*1024)

//#define WMA_TARGET_ARM//start arm optimization
#define MAX_SAMPLES_OF_ONE_CHANNEL 4096
//#define SATURERATE_AFTER_FFT
//WMA,add by evan wu,2009-3-12
#define _ATTR_WMA_COMMON_CODE_            __attribute__((section("WmaCommonCode")))
#define _ATTR_WMA_DATA_                    __attribute__((section("WmaDecoderData")))
#define _ATTR_WMA_BSS_                    __attribute__((section("WmaDecoderBss")))

#define _ATTR_WMA_OPEN_CODEC_CODE_        __attribute__((section("WmaOpenCodecCode")))
#define _ATTR_WMA_OPEN_CODEC_DATA_        __attribute__((section("WmaOpenCodecData")))
#define _ATTR_WMA_OPEN_CODEC_BSS_        __attribute__((section("WmaOpenCodecBss")))

#define _ATTR_WMA_HIGH_LOW_COMMON_CODE_   __attribute__((section("WmaHighLowCommonCode")))
#define _ATTR_WMA_HIGH_RATE_CODE_         __attribute__((section("WmaHighRateCode")))
#define _ATTR_WMA_LOW_RATE_CODE_          __attribute__((section("WmaLowRateCode")))


#endif

