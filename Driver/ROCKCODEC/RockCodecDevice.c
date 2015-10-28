/*
********************************************************************************************
*
*                  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*
* FileName: ..\Driver\RockCodec\RockCodecDevice.c
* Owner: HJ
* Date: 2014.3.10
* Time: 13:51:36
* Desc: Uart Device Class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
*      hj     2014.3.10     13:51:36   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_ROCKCODEC_ROCKCODECDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "rockcodec.h"
#include "effect.h"
#include "audio_globals.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*
typedef struct _SRUCT_CODEC_CONFIG
{
    UINT8 DacAnaVol;
    UINT8 DacDigVol;
}SRUCT_CODEC_CONFIG;
*/
typedef struct _SRUCT_CODEC_CONFIG
{
    UINT8 HP_AMPVol;
    UINT8 Dac_DigVol;


    UINT8 HP_ANTIPOPVol;
    UINT8 BeepFadeDeep;
    UINT8 EQ_POP_HEAVY_HP_AMPVol;
    UINT8 EQ_POP_HEAVY_DacDigVol;
    UINT8 EQ_JAZZ_UNIQUE_HP_AMPVol;
    UINT8 EQ_JAZZ_UNIQUE_DacDigVol;
    UINT8 EQ_USER_HP_AMPVol;
    UINT8 EQ_USER_DacDigVol;
    UINT8 EQ_BASS_HP_AMPVol;
    UINT8 EQ_BASS_DacDigVol;

}SRUCT_CODEC_CONFIG;

typedef struct _SRUCT_ACODEC_CONFIG
{
    UINT8 HP_AMPVol;
    UINT8 Dac_DigVol;

}SRUCT_ACODEC_CONFIG;

#if 1
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_CODEC_CONFIG ACodec_LineOutVol_General[MAX_VOLUME + 1] =
{
//+0dB            //+4dB     //+7db       //+12dB   //BASS
 0, 254, 15, 1,  0, 254,   0, 254,      0, 254,   0, 254, //  0
 0, 140, 15, 1,  0, 165,   0, 157,      0, 124,   0, 124, //  1
 0, 135, 15, 1,  0, 153,   0, 145,      0, 132,   0, 132, //  2
 0, 123, 15, 1,  0, 148,   0, 140,      0, 127,   0, 127, //  3
 0, 123, 11, 1,  0, 145,   0, 137,      0, 124,   0, 124, //  4
 0, 123, 7,  1,  0, 143,   0, 135,      0, 122,   0, 122, //  5
 0, 123, 4,  1,  0, 144,   0, 136,      0, 123,   0, 123, //  6
 0, 123, 2,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  7
 0, 123, 0,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  8
 0, 117, 0,  1,  0, 134,   0, 126,      0, 113,   0, 113, //  9
 0, 111, 0,  1,  0, 127,   0, 119,      0, 106,   0, 106, //  10
 0, 107, 0,  1,  0, 122,   0, 114,      0, 101,   0, 101, //  11
 0, 101, 0,  1,  0, 116,   0, 108,      0, 95,    0, 95,  //  12
 0, 96,  0,  1,  0, 110,   0, 102,      0, 89,    0, 89,  //  13
 0, 91,  0,  1,  0, 103,   0, 95,       0, 82,    0, 82,  //  14
 0, 87,  0,  1,  0, 98,    0, 90,       0, 77,    0, 77,  //  15
 0, 81,  0,  1,  0, 93,    0, 85,       0, 72,    0, 72,  //  16
 0, 77,  0,  1,  0, 84,    0, 76,       0, 63,    0, 63,  //  17
 0, 72,  0,  1,  0, 80,    0, 72,       0, 59,    0, 59,  //  18
 0, 67,  0,  1,  0, 77,    0, 69,       0, 56,    0, 56,  //  19
 0, 62,  0,  1,  0, 69,    0, 61,       0, 48,    0, 48,  //  20
 0, 57,  0,  1,  0, 61,    0, 53,       0, 40,    0, 40,  //  21
 0, 52,  0,  1,  0, 56,    0, 48,       0, 35,    0, 35,  //  22
 0, 47,  0,  1,  0, 50,    0, 42,       0, 29,    0, 29,  //  23
 0, 43,  0,  1,  0, 45,    0, 37,       0, 24,    0, 24,  //  24
 0, 38,  0,  1,  0, 40,    0, 32,       0, 19,    0, 19,  //  25
 0, 34,  0,  1,  0, 33,    0, 25,       0, 12,    0, 12,  //  26
 0, 28,  0,  1,  0, 28,    0, 20,       0, 7,     0, 7,   //  27
 0, 24,  0,  1,  0, 22,    0, 14,       0, 1,     0, 1,    //  28
 0, 19,  0,  1,  0, 17,    0, 9,        1, 4,     1, 4,    //  29
 0, 14,  0,  1,  0, 14,    0, 6,        1, 1,     1, 1,    //  30
 0, 9,   0,  1,  0, 17,    0, 9,        1, 4,     1, 4,    //  31
 0, 4,   0,  1,  0, 14,    0, 6,        1, 1,     1, 1,    //  32
};

// HP_AMPVol;EQ_POP_HEAVY_HP_AMPVol;EQ_JAZZ_UNIQUE_HP_AMPVol;EQ_USER_HP_AMPVol;EQ_BASS_HP_AMPVol
// Description : increase the HP amplitude from 3dB to 9dB
//               0: 0 dB;1: 3 dB;2: 6 dB;3: 9 dB

// Dac_DigVol;EQ_POP_HEAVY_Dac_DigVol;EQ_JAZZ_UNIQUE_Dac_DigVol;EQ_USER_Dac_DigVol;EQ_BASS_Dac_DigVol
// Description : digital volume of DAC channel
//               0.375db/step: 0 dB - (-95) dB
//               0x00: 0dB; 0xff : -95dB

// HP_ANTIPOPVol
// Description : decrease the HP amplitude from 0dB to -15dB
//               1 db/step: 0 dB - (-15) dB
//               0x00: 0dB; 0xf : -15dB

_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_CODEC_CONFIG ACodec_HPoutVol_General[MAX_VOLUME + 1] =
{
#if 1
//+0dB            //+4dB     //+7db       //+12dB   //BASS
 0, 254, 15, 1,  0, 254,   0, 254,      0, 254,   0, 254, //  0
 0, 144, 15, 1,  0, 165,   0, 157,      0, 124,   0, 124, //  1
 0, 140, 15, 1,  0, 153,   0, 145,      0, 132,   0, 132, //  2
 0, 137, 15, 1,  0, 148,   0, 140,      0, 127,   0, 127, //  3
 0, 135, 12, 1,  0, 145,   0, 137,      0, 124,   0, 124, //  4
 0, 135, 9,  1,  0, 143,   0, 135,      0, 122,   0, 122, //  5
 0, 135, 6,  1,  0, 144,   0, 136,      0, 123,   0, 123, //  6
 0, 135, 3,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  7
 0, 135, 0,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  8
 0, 132, 0,  1,  0, 134,   0, 126,      0, 113,   0, 113, //  9
 0, 129, 0,  1,  0, 127,   0, 119,      0, 106,   0, 106, //  10
 0, 122, 0,  1,  0, 122,   0, 114,      0, 101,   0, 101, //  11
 0, 115, 0,  1,  0, 116,   0, 108,      0, 95,    0, 95,  //  12
 0, 110, 0,  1,  0, 110,   0, 102,      0, 89,    0, 89,  //  13
 0, 104, 0,  1,  0, 103,   0, 95,       0, 82,    0, 82,  //  14
 0, 98,  0,  1,  0, 98,    0, 90,       0, 77,    0, 77,  //  15
 0, 92,  0,  1,  0, 93,    0, 85,       0, 72,    0, 72,  //  16
 0, 87,  0,  1,  0, 84,    0, 76,       0, 63,    0, 63,  //  17
 0, 81,  0,  1,  0, 80,    0, 72,       0, 59,    0, 59,  //  18
 0, 75,  0,  1,  0, 77,    0, 69,       0, 56,    0, 56,  //  19
 0, 70,  0,  1,  0, 69,    0, 61,       0, 48,    0, 48,  //  20
 0, 64,  0,  1,  0, 61,    0, 53,       0, 40,    0, 40,  //  21
 0, 58,  0,  1,  0, 56,    0, 48,       0, 35,    0, 35,  //  22
 0, 52,  0,  1,  0, 50,    0, 42,       0, 29,    0, 29,  //  23
 0, 47,  0,  1,  0, 45,    0, 37,       0, 24,    0, 24,  //  24
 0, 41,  0,  1,  0, 40,    0, 32,       0, 19,    0, 19,  //  25
 0, 35,  0,  1,  0, 33,    0, 25,       0, 12,    0, 12,  //  26
 0, 30,  0,  1,  0, 28,    0, 20,       0, 7,     0, 7,   //  27
 0, 24,  0,  1,  0, 22,    0, 14,       0, 7,     0, 7,    //  28
 0, 19,  0,  1,  0, 17,    0, 9,        0, 7,     0, 7,    //  29
 0, 16,  0,  1,  0, 14,    0, 6,        0, 7,     0, 7,    //  30
 0, 14,  0,  1,  0, 17,    0, 9,        0, 7,     0, 7,    //  31
 0, 13,  0,  1,  0, 14,    0, 6,        0, 7,     0, 7,    //  32
#else
//+0dB            //+4dB     //+7db       //+12dB   //BASS
 0, 254, 15, 1,  0, 254,   0, 254,      0, 254,   0, 254,//  0
 0, 176, 15, 1,  0, 166,   0, 157,      0, 124,   0, 124,//  1
 0, 164, 15, 1,  0, 154,   0, 145,      0, 132,   0, 132,//  2
 0, 159, 15, 1,  0, 149,   0, 140,      0, 127,   0, 127,//  3
 0, 156, 11, 1,  0, 146,   0, 137,      0, 124,   0, 124,//  4
 0, 154, 9,  1,  0, 144,   0, 135,      0, 122,   0, 122,//  5
 0, 155, 5,  1,  0, 143,   0, 136,      0, 123,   0, 123,//  6
 0, 153, 3,  1,  0, 143,   0, 134,      0, 121,   0, 121,//  7
 0, 153, 0,  1,  0, 143,   0, 134,      0, 121,   0, 121,//  8
 0, 145, 0,  1,  0, 135,   0, 126,      0, 113,   0, 113,//  9
 0, 138, 0,  1,  0, 128,   0, 119,      0, 106,   0, 106,//  10
 0, 133, 0,  1,  0, 123,   0, 114,      0, 101,   0, 101,//  11
 0, 127, 0,  1,  0, 117,   0, 108,      0, 95,    0, 95,  //  12
 0, 121, 0,  1,  0, 111,   0, 102,      0, 89,    0, 89,  //  13
 0, 114, 0,  1,  0, 104,   0, 95,       0, 82,    0, 82,  //  14
 0, 109, 0,  1,  0, 99,    0, 90,       0, 77,    0, 77,  //  15
 0, 104, 0,  1,  0, 94,    0, 85,       0, 72,    0, 72,  //  16
 0, 95,  0,  1,  0, 85,    0, 76,       0, 63,    0, 63,  //  17
 0, 91,  0,  1,  0, 81,    0, 72,       0, 59,    0, 59,  //  18
 0, 88,  0,  1,  0, 78,    0, 69,       0, 56,    0, 56,  //  19
 0, 80,  0,  1,  0, 70,    0, 61,       0, 48,    0, 48,  //  20
 0, 72,  0,  1,  0, 62,    0, 53,       0, 40,    0, 40,  //  21
 0, 67,  0,  1,  0, 57,    0, 48,       0, 35,    0, 35,  //  22
 0, 61,  0,  1,  0, 51,    0, 42,       0, 29,    0, 29,  //  23
 0, 56,  0,  1,  0, 46,    0, 37,       0, 24,    0, 24,  //  24
 0, 51,  0,  1,  0, 41,    0, 32,       0, 19,    0, 19,  //  25
 0, 44,  0,  1,  0, 34,    0, 25,       0, 12,    0, 12,  //  26
 0, 39,  0,  1,  0, 29,    0, 20,       0, 7,     0, 7,   //  27
 0, 33,  0,  1,  0, 23,    0, 14,       0, 7,     0, 7,    //  28
 0, 28,  0,  1,  0, 18,    0, 9,        0, 7,     0, 7,    //  29
 0, 25,  0,  1,  0, 15,    0, 7,        0, 7,     0, 7,    //  30
 0, 28,  0,  1,  0, 18,    0, 9,        0, 7,     0, 7,    //  31
 0, 25,  0,  1,  0, 15,    0, 7,        0, 7,     0, 7,    //  32
#endif

};

// HP_AMPVol;EQ_POP_HEAVY_HP_AMPVol;EQ_JAZZ_UNIQUE_HP_AMPVol;EQ_USER_HP_AMPVol;EQ_BASS_HP_AMPVol
// Description : increase the HP amplitude from 3dB to 9dB
//               0: 0 dB;1: 3 dB;2: 6 dB;3: 9 dB

// Dac_DigVol;EQ_POP_HEAVY_Dac_DigVol;EQ_JAZZ_UNIQUE_Dac_DigVol;EQ_USER_Dac_DigVol;EQ_BASS_Dac_DigVol
// Description : digital volume of DAC channel
//               0.375db/step: 0 dB - (-95) dB
//               0x00: 0dB; 0xff : -95dB

// HP_ANTIPOPVol
// Description : decrease the HP amplitude from 0dB to -15dB
//               1 db/step: 0 dB - (-15) dB
//               0x00: 0dB; 0xf : -15dB

_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_CODEC_CONFIG CodecConfig_Europe[MAX_VOLUME + 1] =
{
//+0dB            //+4dB     //+7db       //+12dB   //BASS
 0, 254, 15, 1,  0, 254,   0, 254,      0, 254,   0, 254, //  0
 0, 144, 15, 1,  0, 165,   0, 157,      0, 124,   0, 124, //  1
 0, 140, 15, 1,  0, 153,   0, 145,      0, 132,   0, 132, //  2
 0, 137, 15, 1,  0, 148,   0, 140,      0, 127,   0, 127, //  3
 0, 135, 12, 1,  0, 145,   0, 137,      0, 124,   0, 124, //  4
 0, 135, 9,  1,  0, 143,   0, 135,      0, 122,   0, 122, //  5
 0, 135, 6,  1,  0, 144,   0, 136,      0, 123,   0, 123, //  6
 0, 135, 3,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  7
 0, 135, 0,  1,  0, 142,   0, 134,      0, 121,   0, 121, //  8
 0, 132, 0,  1,  0, 134,   0, 126,      0, 113,   0, 113, //  9
 0, 129, 0,  1,  0, 127,   0, 119,      0, 106,   0, 106, //  10
 0, 122, 0,  1,  0, 122,   0, 114,      0, 101,   0, 101, //  11
 0, 115, 0,  1,  0, 116,   0, 108,      0, 95,    0, 95,  //  12
 0, 110, 0,  1,  0, 110,   0, 102,      0, 89,    0, 89,  //  13
 0, 104, 0,  1,  0, 103,   0, 95,       0, 82,    0, 82,  //  14
 0, 98,  0,  1,  0, 98,    0, 90,       0, 77,    0, 77,  //  15
 0, 92,  0,  1,  0, 93,    0, 85,       0, 72,    0, 72,  //  16
 0, 87,  0,  1,  0, 84,    0, 76,       0, 63,    0, 63,  //  17
 0, 81,  0,  1,  0, 80,    0, 72,       0, 59,    0, 59,  //  18
 0, 75,  0,  1,  0, 77,    0, 69,       0, 56,    0, 56,  //  19
 0, 70,  0,  1,  0, 69,    0, 61,       0, 48,    0, 48,  //  20
 0, 64,  0,  1,  0, 61,    0, 53,       0, 40,    0, 40,  //  21
 0, 58,  0,  1,  0, 56,    0, 48,       0, 35,    0, 35,  //  22
 0, 52,  0,  1,  0, 50,    0, 42,       0, 29,    0, 29,  //  23
 0, 47,  0,  1,  0, 45,    0, 37,       0, 24,    0, 24,  //  24
 0, 41,  0,  1,  0, 40,    0, 32,       0, 19,    0, 19,  //  25
 0, 35,  0,  1,  0, 33,    0, 25,       0, 12,    0, 12,  //  26
 0, 30,  0,  1,  0, 28,    0, 20,       0, 7,     0, 7,   //  27
 0, 24,  0,  1,  0, 22,    0, 14,       0, 7,     0, 7,    //  28
 0, 19,  0,  1,  0, 17,    0, 9,        0, 7,     0, 7,    //  29
 0, 16,  0,  1,  0, 14,    0, 6,        0, 7,     0, 7,    //  30
 0, 14,  0,  1,  0, 17,    0, 9,        0, 7,     0, 7,    //  31
 0, 13,  0,  1,  0, 14,    0, 6,        0, 7,     0, 7,    //  32

};
#endif

#if 0
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_ACODEC_CONFIG ACodec_LineOutVol[MAX_VOLUME + 1] =
{
    0, 254,    //  0
    0, 184,    //  1
    0, 178,   //  2
    0, 163,   //  3
    0, 153,   //  4
    0, 142,   //  5
    0, 134,   //  6
    0, 128,   //  7
    0, 124,   //  8
    0, 120,    //  9
    0, 114,    //  10
    0, 110,    //  11
    0, 104,    //  12
    0, 99,    //  13
    0, 94,    //  14
    0, 90,    //  15
    0, 84,    //  16
    0, 80,    //  17
    0, 75,    //  18
    0, 70,    //  19
    0, 65,    //  20
    0, 60,    //  21
    0, 55,   //  22
    0, 50,   //  23
    0, 46,   //  24
    0, 41,     //  25
    0, 37,     //  26
    0, 31,     //  27
    0, 27,     //  28
    0, 22,     //  29
    0, 17,     //  30
//0, 12,     //  31
//0, 7,     //  32
};
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_ACODEC_CONFIG ACodec_HPoutVol[MAX_VOLUME + 1] =
{
    0, 254,    //  0
    0, 188,    //  1
    0, 187,   //  2
    0, 184,   //  3
    0, 174,   //  4
    0, 166,   //  5
    0, 158,   //  6
    0, 150,   //  7
    0, 144,   //  8
    0, 138,    //  9
    0, 131,    //  10
    0, 125,    //  11
    0, 118,    //  12
    0, 113,    //  13
    0, 107,    //  14
    0, 101,    //  15
    0, 95,    //  16
    0, 90,    //  17
    0, 84,    //  18
    0, 78,    //  19
    0, 73,    //  20
    0, 67,    //  21
    0, 61,   //  22
    0, 55,   //  23
    0, 50,   //  24
    0, 44,     //  25
    0, 38,     //  26
    0, 33,     //  27
    0, 27,     //  28
    0, 22,     //  29
    0, 19,     //  30
// 0, 17,     //  31
// 0, 15,     //  30
};
/*
#if 1
SRUCT_CODEC_CONFIG CodecConfig_Europe[MAX_VOLUME + 1] =
{  //+0dB              //+4dB           //+7db          //+12dB
  0, 254,  1,       0, 254,       0, 254,       0, 254,// 0
  0, 129,  1,       0, 118,       0, 110,       0, 97,// 1
  0, 120,  1,       0, 109,       0, 101,       0, 88,// 2
  0, 110,  1,       0, 99,        0, 91,        0, 78,// 3
  0, 105,  1,       0, 94,        0, 86,        0, 73,// 4
  0, 100,  1,       0, 89,        0, 81,        0, 68,// 5
  0, 95,   1,       0, 84,        0, 76,        0, 63,// 6
  0, 90,   1,       0, 79,        0, 71,        0, 58,// 7
  0, 85,   1,       0, 74,        0, 66,        0, 53,// 8
  0, 81,   1,       0, 70,        0, 62,        0, 49,// 9
  0, 77,   1,       0, 66,        0, 58,        0, 45,// 10
  0, 73,   1,       0, 62,        0, 54,        0, 41,// 11
  0, 69,   1,       0, 58,        0, 50,        0, 37,// 12
  0, 65,   1,       0, 54,        0, 46,        0, 33,// 13
  1, 64,   1,       1, 53,        1, 45,        1, 32,// 14
  2, 63,   1,       2, 52,        2, 44,        2, 33,// 15
  0, 54,   1,       0, 43,        0, 35,        0, 24,// 16
  1, 53,   1,       1, 42,        1, 34,        1, 23,// 17
  2, 52,   1,       2, 41,        2, 33,        2, 22,// 18
  1, 46,   1,       0, 32,        0, 24,        0, 11,// 19
  1, 42,   1,       1, 31,        1, 23,        1, 10,// 20
  0, 36,   1,       0, 25,        0, 17,        0, 4, // 21
  1, 35,   1,       1, 24,        1, 16,        1, 3, // 22
  0, 29,   1,       0, 18,        0, 10,        1, 0, // 23
  1, 28,   1,       1, 17,        1, 9,         3, 1, // 24
  2, 27,   1,       2, 16,        2, 8,         4, 0, // 25
  1, 21,   1,       1, 10,        1, 2,         6, 3,// 26
  2, 20,   1,       2, 9,         2, 0,         6, 2, // 27
  1, 14,   1,       1, 3,         3, 1,         6, 1, // 28
  0, 8,    1,       2, 2,         6, 4,         6, 0, // 29
  12, 36,  1,       12, 25,       12, 17,      12,15,// 30

};

SRUCT_CODEC_CONFIG CodecConfig_General[MAX_VOLUME + 1] =
{//+0dB          //+4dB           //+7db        //+12dB
 0, 254,   1,  0, 254,      0, 254,   0, 254,   //  0
 0, 142,   1,  0, 131,      0, 123,   0, 110,   //  1
 0, 137,   1,  0, 126,      0, 118,   0, 105,  //  2
 0, 132,   1,  0, 121,      0, 113,   0, 100 , //  3
 0, 126,   1,  0, 115,      0, 107,   0, 96,  //  4
 0, 120,   1,  0, 109,      0, 101,   0, 88,  //  5
 0, 114,   1,  0, 103,      0, 95,    0, 82,  //  6
 0, 109,   1,  0, 98,       0, 90,    0, 77,  //  7
 0, 103,   1,  0, 92,       0, 84,    0, 71,  //  8
 0, 97,    1,  0, 86,       0, 78,    0, 65,  //  9
 0, 91,    1,  0, 80,       0, 72,    0, 59,  //  10
 0, 87,    1,  0, 76,       0, 68,    0, 55,  //  11
 0, 81,    1,  0, 70,       0, 62,    0, 49,  //  12
 0, 75,    1,  0, 64,       0, 56,    0, 43,  //  13
 0, 69,    1,  0, 58,       0, 50,    0, 37,  //  14
 0, 64,    1,  0, 53,       0, 45,    0, 31,  //  15
 0, 59,    1,  0, 48,       0, 40,    0, 27,  //  16
 0, 51,    1,  0, 40,       0, 32,    0, 19,  //  17
 0, 47,    1,  0, 36,       0, 28,    0, 15,  //  18
 0, 42,    1,  0, 31,       0, 23,    0, 10,  //  19
 0, 36,    1,  0, 25,       0, 17,    0, 4,   //  20
 0, 28,    1,  0, 17,       0, 9,     2, 3,   //  21
 0, 23,    1,  0, 12,       0, 4,     4, 2,  //  22
 0, 17,    1,  0, 6,        1, 0,     6, 1,  //  23
 0, 12,    1,  0, 1,        3, 0,     6, 0,  //  24
 0, 7,     1,  2, 0,        5, 0,     6, 0,   //  25
 0, 0,     1,  4, 0,        7, 0,     6, 0,   //  26
 1, 3,     1,  6, 0,        8, 2,     6, 0,   //  27
 2, 5,     1,  8, 1,        9, 3,     6, 0,   //  28
 2, 0,     1,  8, 1,        9, 3,     6, 0,   //  29
 3, 4,     1,  8, 1,        9, 3,     6, 0,   //  30

};
#else
_ATTR_DRIVER_CODE_ SRUCT_CODEC_CONFIG CodecConfig_Europe[MAX_VOLUME + 1] =
{  //+0dB                           //+4dB                //+7db                //+12dB ,   //BASS
  0, 254,  1,       0, 254,       0, 254,       0, 254,  0,254,// 0
  0, 129,  1,       0, 118,       0, 110,       0, 97,   0,97,// 1
  0, 120,  1,       0, 109,       0, 101,       0, 88,   0,88,// 2
  0, 110,  1,       0, 99,        0, 91,        0, 78,   0,78,// 3
  0, 105,  1,       0, 94,        0, 86,        0, 73,   0,73,// 4
  0, 100,  1,       0, 89,        0, 81,        0, 68,   0,68,// 5
  0, 95,   1,       0, 84,        0, 76,        0, 63,   0,63,// 6
  0, 90,   1,       0, 79,        0, 71,        0, 58,   0,58,// 7
  0, 85,   1,       0, 74,        0, 66,        0, 53,   0,53,// 8
  0, 81,   1,       0, 70,        0, 62,        0, 49,   0,49,// 9
  0, 77,   1,       0, 66,        0, 58,        0, 45,   0,45,// 10
  0, 73,   1,       0, 62,        0, 54,        0, 41,   0,41,// 11
  0, 69,   1,       0, 58,        0, 50,        0, 37,   0,37,// 12
  0, 65,   1,       0, 54,        0, 46,        0, 33,   0,33,// 13
  1, 64,   1,       1, 53,        1, 45,        1, 32,   1,32,// 14
  2, 63,   1,       2, 52,        2, 44,        2, 33,   2,31,// 15
  0, 54,   1,       0, 43,        0, 35,        0, 24,   0,22,// 16
  1, 53,   1,       1, 42,        1, 34,        1, 23,   1,21,// 17
  2, 52,   1,       2, 41,        2, 33,        2, 22,   2,20,// 18
  1, 46,   1,       0, 32,        0, 24,        0, 11,   0,11,// 19
  1, 42,   1,       1, 31,        1, 23,        1, 10,   1,10, // 20
  0, 36,   1,       0, 25,        0, 17,        0, 4,    0,4, // 21
  1, 35,   1,       1, 24,        1, 16,        1, 3,    1,6, // 22
  0, 29,   1,       0, 18,        0, 10,        1, 0,    0,3, // 23
  1, 28,   1,       1, 17,        1, 9,         3, 1,    0,3, // 24
  2, 27,   1,       2, 16,        2, 8,         4, 0,    2,7, // 25
  1, 21,   1,       1, 10,        1, 2,         6, 3,    2,7, // 26
  2, 20,   1,       2, 9,         2, 0,         6, 2,    0,3, // 27
  1, 14,   1,       1, 3,         3, 1,         6, 1,    2,8, //28
  0, 8,    1,       2, 2,         6, 4,         6, 0,    4,14, // 29
  12, 36,  1,       12, 25,       12, 17,      12,15,    12,36,// 30

};

_ATTR_DRIVER_CODE_ SRUCT_CODEC_CONFIG CodecConfig_General[MAX_VOLUME + 1] =
{  //+0dB                //+4dB               //+7db        //+12dB      //BASS
 0, 254,   1,  0, 254,      0, 254,   0, 254,  0,254,//  0
 0, 142,   1,  0, 131,      0, 123,   0, 110,  0,110,   //  1
 0, 137,   1,  0, 126,      0, 118,   0, 105,  0,105,//  2
 0, 132,   1,  0, 121,      0, 113,   0, 100,  0,100,//  3
 0, 126,   1,  0, 115,      0, 107,   0, 96,   0,94,//  4
 0, 120,   1,  0, 109,      0, 101,   0, 88,   0,88,//  5
 0, 114,   1,  0, 103,      0, 95,    0, 82,   0,82,//  6
 0, 109,   1,  0, 98,       0, 90,    0, 77,   0,77,//  7
 0, 103,   1,  0, 92,       0, 84,    0, 71,   0,71,//  8
 0, 97,    1,  0, 86,       0, 78,    0, 65,   0,65,//  9
 0, 91,    1,  0, 80,       0, 72,    0, 59,   0,59,//  10
 0, 87,    1,  0, 76,       0, 68,    0, 55,   0,55,//  11
 0, 81,    1,  0, 70,       0, 62,    0, 49,   0,49,//  12
 0, 75,    1,  0, 64,       0, 56,    0, 43,   0,43,//  13
 0, 69,    1,  0, 58,       0, 50,    0, 37,   0,37,//  14
 0, 64,    1,  0, 53,       0, 45,    0, 31,   0,32,//  15
 0, 59,    1,  0, 48,       0, 40,    0, 27,   0,27,//  16
 0, 51,    1,  0, 40,       0, 32,    0, 19,   0,19,//  17
 0, 47,    1,  0, 36,       0, 28,    0, 15,   0,15,//  18
 0, 42,    1,  0, 31,       0, 23,    0, 10,   0,10,//  19
 0, 36,    1,  0, 25,       0, 17,    0, 4,    0,4,//  20
 0, 28,    1,  0, 17,       0, 9,     2, 3,    2,1,//  21
 0, 23,    1,  0, 12,       0, 4,     4, 2,    4,2,//  22
 0, 17,    1,  0, 6,        1, 0,     6, 1,    6,1,//  23
 0, 12,    1,  0, 1,        3, 0,     6, 0,    8,1,//  24
 0, 7,     1,  2, 0,        5, 0,     6, 0,    8,1,//  25
 0, 0,     1,  4, 0,        7, 0,     6, 0,    9,1,//  26
 2, 0,     1,  6, 0,        8, 2,     6, 0,    8,0,//  27
 4, 0,     1,  8, 1,        9, 3,     6, 0,    8,0,//  28
 6, 0,     1,  8, 1,        9, 3,     6, 0,    10,4,//  29
 8, 2,     1,  8, 1,        9, 3,     6, 0,    8,2,//  30

};
#endif*/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_CODEC_CONFIG CodecConfig_Europe[30 + 1] =
{
    //+0dB                           //+4dB                //+7db                //+12dB ,   //BASS
    0, 254,  // 0
    0, 129, // 1
    0, 120,  // 2
    0, 110,  // 3
    0, 105,  // 4
    0, 100,  // 5
    0, 95,  // 6
    0, 90,  // 7
    0, 85,   // 8
    0, 81,  // 9
    0, 77,
    0, 73,
    0, 69,
    0, 65,
    1, 64,
    2, 63,
    0, 54,
    1, 53,
    2, 52,
    1, 46,
    1, 42,
    0, 36,
    1, 35,
    0, 29,
    1, 28,
    2, 27,
    1, 21,
    2, 20,
    1, 14,
    0, 8,
    12, 36

};
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
SRUCT_CODEC_CONFIG CodecConfig_General[30 + 1] =
{
    //+0dB                //+4dB               //+7db        //+12dB      //BASS
    0, 254,   //  0
    0, 142,     //  1
    0, 137,   //  2
    0, 132,   //  3
    0, 126,   //  4
    0, 120,   //  5
    0, 114,  //  6
    0, 109,  //  7
    0, 103,  //  8
    0, 97,   //  9
    0, 91,  //  10
    0, 87,   //  11
    0, 81,   //  12
    0, 75,  //  13
    0, 69,    //  14
    0, 64,   //  15
    0, 59,  //  16
    0, 51,  //  17
    0, 47,   //  18
    0, 42,   //  19
    0, 36,    //  20
    0, 28,    //  21
    0, 23,    //  22
    0, 17,    //  23
    0, 12,    //  24
    0, 7,     //  25
    0, 0,     //  26
    2, 0,     //  27
    4, 0,     //  28
    6, 0,     //  29
    8, 2,    //  30

};
#endif

typedef struct _RockCodec_DEVICE_CONFIG_REQ_ARG
{
    CodecMode_en_t Codecmode;
    CodecFS_en_t CodecFS;
}RockCodec_DEVICE_CONFIG_REQ_ARG;

typedef  struct _RockCodec_DEVICE_CLASS
{
    DEVICE_CLASS    stRockCodecDevice;            //device father class


    pSemaphore osRockCodecWriteSem;
    pSemaphore osRockCodecControlReqSem;

//    AudioInOut_Type AudioInOutType;
    SRUCT_CODEC_CONFIG *CodecVolumeTable;
    HDC        hI2S;

}RockCodec_DEVICE_CLASS;

/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
RockCodec_DEVICE_CLASS * gpstRockcodecDevISRHandler;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//volatile AudioInOut_Type AudioIOBuf;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t RockCodecShellCreate(HDC dev, uint8 * pstr);
rk_err_t RockCodecShellDel(HDC dev, uint8 * pstr);
rk_err_t RockCodecShellHelp(HDC dev, uint8 * pstr);
#if 1
rk_err_t RockcodecDevResume(HDC dev);
rk_err_t RockcodecDevSuspend(HDC dev);
void RockcodecIntIsr(void);
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------

*/
//__attribute__((aligned (8))) const

//#define _RockTest_

#ifdef _RockTest_
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
uint8 Codecoutptr[32][176] =
{
    //0
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    //8
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    //16
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    //24
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    {
#include "test01_1K0_441.data"
    },
    //32
};

_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
uint32 length = 176 * 32 / 4;
#endif


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: RockcodecDevCreate
** Input:void
** Return: DEVICE_CLASS *
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:40:31
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_INIT_
INIT API HDC RockCodecDev_Create(uint32 DevID, void *arg)
{
    DEVICE_CLASS* pstDev;
    RockCodec_DEVICE_CLASS * psRockCodecDev;
    ROCKCODEC_DEV_ARG * pstRockCodecArg;

    if (arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstRockCodecArg = (ROCKCODEC_DEV_ARG *)arg;

    psRockCodecDev =  rkos_memory_malloc(sizeof(RockCodec_DEVICE_CLASS));
    if (psRockCodecDev == NULL)
    {
        return (HDC) RK_ERROR;
    }

    psRockCodecDev->osRockCodecWriteSem = rkos_semaphore_create(1,1);
    psRockCodecDev->osRockCodecControlReqSem = rkos_semaphore_create(1,1);

    if ((      psRockCodecDev->osRockCodecWriteSem
               && psRockCodecDev->osRockCodecControlReqSem) == 0)
    {
        rkos_semaphore_delete(psRockCodecDev->osRockCodecWriteSem);
        rkos_semaphore_delete(psRockCodecDev->osRockCodecControlReqSem);

        rkos_memory_free(psRockCodecDev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)psRockCodecDev;

    pstDev->suspend = RockcodecDevSuspend;
    pstDev->resume  = RockcodecDevResume;

    psRockCodecDev->hI2S = pstRockCodecArg->hI2s;
//    psRockCodecDev->AudioInOutType.EffectCtl.Mode = pstRockCodecArg->eqMode;
    psRockCodecDev->CodecVolumeTable = ACodec_HPoutVol_General;// CodecConfig_General;
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_ROCKCODEC_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    if (RockcodecDevInit(psRockCodecDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(psRockCodecDev->osRockCodecWriteSem);
        rkos_semaphore_delete(psRockCodecDev->osRockCodecControlReqSem);

        rkos_memory_free(psRockCodecDev);
        printf("create RockCodecDev fail");
        return (HDC) RK_ERROR;
    }
    gpstRockcodecDevISRHandler = psRockCodecDev;
    printf("create RockCodecDev success");
    return (HDC)pstDev;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: RockcodecDevDelete
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:41:18
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_INIT_
rk_err_t RockCodecDev_Delete(uint32 DevID, void * arg)
{
    ROCKCODEC_DEV_ARG *stRockCodecDevArg = (ROCKCODEC_DEV_ARG *)arg;

    stRockCodecDevArg->hI2s = gpstRockcodecDevISRHandler->hI2S;
    RockcodecDevDeInit(gpstRockcodecDevISRHandler);

    //AudioIOBuf = NULL;
    rkos_semaphore_delete( gpstRockcodecDevISRHandler->osRockCodecWriteSem );
    rkos_semaphore_delete( gpstRockcodecDevISRHandler->osRockCodecControlReqSem );

    rkos_memory_free(gpstRockcodecDevISRHandler);
    gpstRockcodecDevISRHandler = NULL;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_ROCKCODEC_DEV);
#endif

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: RockcodecDevDeInit
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:38:30
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_INIT_
rk_err_t RockcodecDevDeInit(HDC dev)
{
    RockCodec_DEVICE_CLASS * pstRockCodecDev = (RockCodec_DEVICE_CLASS *) dev;
    return RK_SUCCESS;
}


/*******************************************************************************
** Name: RockcodecDevInit
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 11:44:46
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_INIT_
rk_err_t RockcodecDevInit(HDC dev)
{
    RockCodec_DEVICE_CLASS * psRockCodecDev = (RockCodec_DEVICE_CLASS *)dev;
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;

    if(psRockCodecDev == NULL)
    {
        printf("RockcodecDevInit ERROR\n");
        return RK_ERROR;
    }
    Codec_PowerOnInitial();
#if 0
    stI2sDevArg.i2smode = I2S_SLAVE_MODE;//;I2S_SLAVE_MODE
    stI2sDevArg.i2sCS = I2S_IN; //I2S_EXT
    stI2sDevArg.I2S_FS = I2S_FS_44100Hz;
    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;//I2S_FORMAT;
#if CODEC_24BIT //24bit
    stI2sDevArg.Data_width = I2S_DATA_WIDTH24;
#else
    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
#endif
    stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    //ACodec_Set_I2S_Mode(TFS_TX_I2S_MODE,ACodec_I2S_DATA_WIDTH24,IBM_TX_BUS_MODE_NORMAL,I2S_MST_MASTER);
    ACodec_Set_I2S_Mode(TFS_TX_I2S_MODE,stI2sDevArg.Data_width,IBM_TX_BUS_MODE_NORMAL,I2S_MST_MASTER);
    RockcodecDevConfigI2S(dev,&stI2sDevArg );
#endif
    return RK_SUCCESS;
}

#ifdef _USE_SHELL_

_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
static uint8* ShellRockCodecName[] =
{
    "pcb",
    "create",
    "test",
    "delete",
    "suspend",
    "resume",
    "read",
    "write",
    "control",
    "help",
    "\b",                          // the end
};


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: RockcodecDev_GetAudioInOutType
** Input:AudioInOut_Type *AudioInOutType
** Return: rk_size_t
** Owner:cjh
** Date: 2015.7.1
** Time: 20:46:51
*******************************************************************************/
/*
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
COMMON API rk_err_t RockcodecDev_GetAudioInOutType(void *AudioInOutType)
{
    if(gpstRockcodecDevISRHandler == NULL)
       return RK_ERROR;

//    AudioInOutType =(void *)&(gpstRockcodecDevISRHandler->AudioInOutType);

    return RK_SUCCESS;
}
*/
/*******************************************************************************
** Name: RockcodecDevResume
** Input:DEVICE_CLASS *
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:57:19
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDevResume(HDC dev)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: RockcodecDevSuspend
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:56:05
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDevSuspend(HDC dev)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: RockcodecDevConfigI2S
** Input:HDC dev
** Return: void
** Owner:cjh
** Date: 2015.6.12
** Time: 10:07:32
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
COMMON API rk_err_t RockcodecDevConfigI2S(HDC dev, void *arg)
{
    int32  ret = RK_SUCCESS;
    RockCodec_DEVICE_CLASS * pRockCodecDev = (RockCodec_DEVICE_CLASS *)(dev);
    HDC hI2S = pRockCodecDev->hI2S;
    I2S_DEVICE_CONFIG_REQ_ARG *stI2sDevArg = arg;

//    stI2sDevArg.i2smode = I2S_MASTER_MODE;//;I2S_SLAVE_MODE
//    stI2sDevArg.i2sCS = I2S_IN; //I2S_EXT
//    stI2sDevArg.I2S_FS = I2S_FS_44100Hz;//I2S_FS_44100Hz;
//    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
//    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;
    ret = I2sDev_Control(hI2S, I2S_DEVICE_INIT_CMD, arg);

    if (RK_ERROR == ret)
    {
        return ret;
    }
    return ret;
}

/*******************************************************************************
** Name: RockcodecDev_SetMode
** Input:HDC dev,  CodecMode_en_t Codecmode,  CodecFS_en_t CodecFS
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDev_SetMode(HDC dev,  CodecMode_en_t Codecmode,  CodecFS_en_t CodecFS)
{
    RockCodec_DEVICE_CLASS * pRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);

    rkos_semaphore_take(pRockCodecDev->osRockCodecControlReqSem, MAX_DELAY);
    rkos_enter_critical();
    //codec mode set
    Codec_SetMode(Codecmode,CodecFS);
    //Codec_SetMode(Codecmode);
    rkos_exit_critical();
    rkos_semaphore_give(pRockCodecDev->osRockCodecControlReqSem);
    return RK_SUCCESS;

}

/*******************************************************************************
** Name: RockcodecDev_SetRate
** Input:HDC dev,  CodecFS_en_t CodecFS
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDev_SetRate(HDC dev,  CodecFS_en_t CodecFS)
{
    RockCodec_DEVICE_CLASS * pRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);

    rkos_semaphore_take(pRockCodecDev->osRockCodecControlReqSem, MAX_DELAY);
    rkos_enter_critical();
    //codec mode set PLL&
    printf("SetSampleRate=%d\n",CodecFS);
    Codec_SetSampleRate(CodecFS);
    rkos_exit_critical();
    rkos_semaphore_give(pRockCodecDev->osRockCodecControlReqSem);
    return RK_SUCCESS;

}

/*******************************************************************************
** Name: RockcodecDev_SetEQ
** Input:HDC dev,  uint32 vol
** Return: void
** Owner:cjh
** Date: 2015.7.1
** Time: 9:38:09
*******************************************************************************/
/*
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
COMMON API void RockcodecDev_SetEQ(HDC dev, void* userEQ, void* eqArg)
{
    RockCodec_DEVICE_CLASS * pRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);
    RKEffect *pEffect;
    AUDIO_EQ_ARG* eqARG = (AUDIO_EQ_ARG*)eqArg;
    pEffect = (RKEffect *)userEQ;

#ifdef  _RK_EQ_
    if (pEffect->Mode == EQ_BASS)
    {
        if (*(eqARG->playVolume) <= 27 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 12;
            else
                pEffect->max_DbGain = 12;
        }
        else if (*(eqARG->playVolume) == 28 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 10;
            else
                pEffect->max_DbGain = 10;
        }
        else if (*(eqARG->playVolume) == 29 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 8;
            else
                pEffect->max_DbGain = 8;
        }
        else if (*(eqARG->playVolume) == 30 )
        {
            if (VOLTAB_CONFIG == VOL_General)
                pEffect->max_DbGain = 6;
            else
                pEffect->max_DbGain = 6;
        }

    }

    printf("AudioPlayInfo.playVolume = %d",(*(eqARG->playVolume)));
    printf("max_DbGain = %d",pEffect->max_DbGain);
    EffectAdjust(userEQ, (void *)eqARG);

#endif
}
*/
/*******************************************************************************
** Name: RockcodecDev_SetVol
** Input:HDC dev, uint32 userEQMod, uint32 vol
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDev_SetVol(HDC dev, uint32 userEQMod, uint32 vol)
{
    RockCodec_DEVICE_CLASS * pRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);
    uint32 VolumeMode = EQ_NOR;
    //rk_printf("Volume = %d",vol);
    rkos_semaphore_take(pRockCodecDev->osRockCodecControlReqSem, MAX_DELAY);
    rkos_enter_critical();

#ifdef _RK_EQ_
    if(userEQMod < EQ_NOR)
    {
        printf("*** eqMode=%d***\n", userEQMod);
        VolumeMode = userEQMod;
    }
#endif
    printf("*** vol = %d  VolumeMode=%d EQ_NOR=%d***\n", vol, VolumeMode ,EQ_NOR);

    if (vol == 0)
    {
        Codec_DACMute();
    }
    else
    {
        if (ACodec_Get_DAC_MTST())
        {
            printf("ACodec_Get_DAC_MTST \n");
            Codec_DACUnMute();
        }
#if 0
        if (TRUE == ThreadCheck(pMainThread, &MusicThread))
        {
            AudioInOut_Type  *pAudio  = &AudioIOBuf;
            RKEffect          *pEffect = &pAudio->EffectCtl;
            VolumeMode = pEffect->Mode;
        }
#endif

#if 0
        switch (VolumeMode)
        {
            case EQ_NOR:
                if (ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                {
                    printf("ACODEC_OUT_LINE HPAMPVol = %d",ACodec_LineOutVol_General[vol].HP_AMPVol);
                    ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].HP_AMPVol);
                    printf("ACODEC_OUT_LINE DacDigVol = %d",ACodec_LineOutVol_General[vol].Dac_DigVol);
                    ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].Dac_DigVol);
                }
                else
                {
                    printf("HPAMPVol = %d",ACodec_HPoutVol_General[vol].HP_AMPVol);
                    ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].HP_AMPVol);

                    printf("DacDigVol = %d",ACodec_HPoutVol_General[vol].Dac_DigVol);
                    ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].Dac_DigVol);
                }
                break;
            case EQ_POP:
            case EQ_HEAVY:
                if (VOLTAB_CONFIG == VOL_Europe)
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);//DacDigVol
                }
                else
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_General[vol].Dac_DigVol);
                }
                break;
            case EQ_JAZZ:
            case EQ_UNIQUE:
                if (VOLTAB_CONFIG == VOL_Europe)
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);
                }
                else
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_General[vol].Dac_DigVol);
                }
                break;

            case EQ_USER:
                if (VOLTAB_CONFIG == VOL_Europe)
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);
                }
                else
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_General[vol].Dac_DigVol);
                }
                break;

            case EQ_BASS:
                if (VOLTAB_CONFIG == VOL_Europe)
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);
                }
                else
                {
                    ACodec_Set_DAC_DigVol(CodecConfig_General[vol].Dac_DigVol);
                }

                break;

            default:
                break;
        }
#endif

#ifdef _RK_EQ_

        switch (VolumeMode)
        {
            case EQ_NOR:
                if(VOLTAB_CONFIG == VOL_Europe)
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].Dac_DigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(CodecConfig_Europe[vol].HP_AMPVol);
                        ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);
                        ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
                    }
                }
                else
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].Dac_DigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        printf("HPAMPVol = %d\n",ACodec_HPoutVol_General[vol].HP_AMPVol);
                        ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].HP_AMPVol);

                        printf("DacDigVol = %d\n",ACodec_HPoutVol_General[vol].Dac_DigVol);
                        ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].Dac_DigVol);

                        printf("HP_Gain = %d\n",ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                        ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                    }
                }

                break;
            case EQ_POP:
            case EQ_HEAVY:
                printf("\n!!!cjh!!! EQ_HEAVY\n");
                if(VOLTAB_CONFIG == VOL_Europe)
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_POP_HEAVY_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_POP_HEAVY_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(CodecConfig_Europe[vol].EQ_POP_HEAVY_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].EQ_POP_HEAVY_DacDigVol);
                        printf("\n!!!cjh!!! vol=%d table=%d\n",vol ,CodecConfig_Europe[vol].HP_ANTIPOPVol);
                        ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
                    }
                }
                else
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_POP_HEAVY_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_POP_HEAVY_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].EQ_POP_HEAVY_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].EQ_POP_HEAVY_DacDigVol);
                        printf("\n!!!cjh!!!2  vol=%d table=%d\n",vol ,ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                        ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                    }
                }
                break;
            case EQ_JAZZ:
            case EQ_UNIQUE:
                if(VOLTAB_CONFIG == VOL_Europe)
                {
                   if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                   {
                       ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_JAZZ_UNIQUE_HP_AMPVol);
                       ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_JAZZ_UNIQUE_DacDigVol);
                       ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                   }
                   else
                   {
                       ACodec_Set_HP_AMP(CodecConfig_Europe[vol].EQ_JAZZ_UNIQUE_HP_AMPVol);
                       ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].EQ_JAZZ_UNIQUE_DacDigVol);
                       ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
                   }
                }
                else
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_JAZZ_UNIQUE_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_JAZZ_UNIQUE_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].EQ_JAZZ_UNIQUE_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].EQ_JAZZ_UNIQUE_DacDigVol);
                        printf("\n!!!cjh!!!3  vol=%d table=%d\n",vol ,ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                        ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                    }
                }
                break;

            case EQ_USER:

                if(VOLTAB_CONFIG == VOL_Europe)
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_USER_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_USER_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(CodecConfig_Europe[vol].EQ_USER_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].EQ_USER_DacDigVol);
                        ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
                    }
                }
                else
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_USER_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_USER_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].EQ_USER_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].EQ_USER_DacDigVol);
                        ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                    }
                }
                break;

            case EQ_BASS:
                if(VOLTAB_CONFIG == VOL_Europe)
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_BASS_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_BASS_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(CodecConfig_Europe[vol].EQ_BASS_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].EQ_BASS_DacDigVol);
                        ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
                    }
                }
                else
                {
                    if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
                    {
                        ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].EQ_BASS_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].EQ_BASS_DacDigVol);
                        ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
                    }
                    else
                    {
                        ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].EQ_BASS_HP_AMPVol);
                        ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].EQ_BASS_DacDigVol);
                        ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                    }
                }

                break;

            default:
                break;
        }
#else
        if(VOLTAB_CONFIG == VOL_Europe)
        {
            if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
            {
                ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].HP_AMPVol);
                ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].Dac_DigVol);
                ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
            }
            else
            {
                ACodec_Set_HP_AMP(CodecConfig_Europe[vol].HP_AMPVol);
                ACodec_Set_DAC_DigVol(CodecConfig_Europe[vol].Dac_DigVol);
                ACodec_Set_HP_Gain(CodecConfig_Europe[vol].HP_ANTIPOPVol);
            }
        }
        else
        {
            if(ACODEC_OUT_CONFIG == ACODEC_OUT_LINE)
            {
                ACodec_Set_HP_AMP(ACodec_LineOutVol_General[vol].HP_AMPVol);
                ACodec_Set_DAC_DigVol(ACodec_LineOutVol_General[vol].Dac_DigVol);
                ACodec_Set_LO_Gain(ACodec_LineOutVol_General[vol].HP_ANTIPOPVol);
            }
            else
            {
                printf("HPAMPVol = %d\n",ACodec_HPoutVol_General[vol].HP_AMPVol);
                ACodec_Set_HP_AMP(ACodec_HPoutVol_General[vol].HP_AMPVol);

                printf("DacDigVol = %d\n",ACodec_HPoutVol_General[vol].Dac_DigVol);
                ACodec_Set_DAC_DigVol(ACodec_HPoutVol_General[vol].Dac_DigVol);

                printf("HP_Gain = %d\n",ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
                ACodec_Set_HP_Gain(ACodec_HPoutVol_General[vol].HP_ANTIPOPVol);
            }
        }
#endif
    }

    DelayUs(10);
    //Codec_SetVolumet(vol);
    rkos_exit_critical();
    rkos_semaphore_give(pRockCodecDev->osRockCodecControlReqSem);
    return RK_SUCCESS;

}

/*******************************************************************************
** Name: RockcodecDev_SetVolTable
** Input:HDC dev,  uint32 voltable
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDev_SetVolTable(HDC dev,  uint32 voltable)
{
    RockCodec_DEVICE_CLASS * pRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);

    rkos_semaphore_take(pRockCodecDev->osRockCodecControlReqSem, MAX_DELAY);
    rkos_enter_critical();
    //VolumeTable set
    //if null,set default table
#if 1
    if (voltable == VOLTABLE_HPOUTVOL_GENERAL)
    {
        pRockCodecDev->CodecVolumeTable = ACodec_HPoutVol_General;
    }
    else if (voltable == VOLTABLE_LINOUTVOL_GENERAL)
    {
        pRockCodecDev->CodecVolumeTable = ACodec_LineOutVol_General;
    }
    else
    {
        pRockCodecDev->CodecVolumeTable = CodecConfig_Europe;
    }
#endif
#if 0
    if (voltable == VOLTABLE_GENERAL)
    {
        pRockCodecDev->CodecVolumeTable = CodecConfig_General;
    }
    else
    {
        pRockCodecDev->CodecVolumeTable = CodecConfig_Europe;
    }
#endif
    rkos_exit_critical();
    rkos_semaphore_give(pRockCodecDev->osRockCodecControlReqSem);
    return RK_SUCCESS;

}

/*******************************************************************************
** Name: RockcodecDevWrite
** Input:DEVICE_CLASS* dev, uint32 pos, const void* buffer, uint32 size,uint8 mode,pTx_complete Tx_complete
** Return: rk_size_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:46:51
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_COMMON_
rk_err_t RockcodecDev_Write(HDC dev, uint8* buffer, uint32 size,uint8 mode)
{
    RockCodec_DEVICE_CLASS * pstRockCodecDev =  (RockCodec_DEVICE_CLASS *)(dev);
    HDC hI2S = pstRockCodecDev->hI2S;
    int32  ret = RK_SUCCESS;

    if (mode == SYNC_MODE)
    {
        ret = I2sDev_Write(hI2S, (uint32 *)buffer, size, SYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            return ret;
        }
        return size;
    }
    else if (mode == ASYNC_MODE)
    {
        ret = I2sDev_Write(hI2S, (uint32 *)buffer, size, ASYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            return ret;
        }
//        printf("I2sDev_Write success size=%d\n",size);
        return size;
    }

}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: RockCodecDev_Shell
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
rk_err_t RockCodecDev_Shell(HDC dev,  uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;
    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }
    while (memcmp((uint8*)ShellRockCodecName[i],pItem,strlen(ShellRockCodecName[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellRockCodecName[i]) == '\b')
        {
            return RK_ERROR;
        }
    }

    pItem += StrCnt;
    pItem++;                        //remove '.',the point is the useful item
    switch (i)
    {
        case 0x00:  //pcb
            ret = RockCodecShellPcb(dev,pItem);
            break;

        case 0x01:  //create
            ret = RockCodecShellCreate(dev,pItem);
            break;

        case 0x02:  //test
            ret = RockCodecShellTest(dev,pItem);
            break;

        case 0x03:  //Del
            ret = RockCodecShellDel(dev,pItem);
            break;

        case 0x04:  //suspend
            //ret = RockCodecShellSuspend(dev,pItem);
            break;

        case 0x05:  //resume
            //ret = RockCodecShellResume(dev,pItem);
            break;

        case 0x06:  //read
            //ret = RockCodecShellRead(dev,pItem);
            break;

        case 0x07:  //write
            //ret = RockCodecShellWrite(dev,pItem);
            break;

        case 0x08:  //control
            //ret = RockCodecShellControl(dev,pItem);
            break;

        case 0x09:  //help
            ret = RockCodecShellHelp(dev,pItem);
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
** Name: RockCodecShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:cjh
** Date: 2015.6.15
** Time: 8:24:52
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
SHELL FUN rk_err_t RockCodecShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;

    //Get RockCodecDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    if (RKDev_Delete(DEV_CLASS_ROCKCODEC, DevID, &stRockCodecDevArg) != RK_SUCCESS)
    {
        printf("RockCodecDev delete failure DevID %d\n",DevID);
    }
    if (stRockCodecDevArg.hI2s != NULL)
    {
        if (RKDev_Close(stRockCodecDevArg.hI2s) != RK_SUCCESS)
        {
            printf("rockCode i2s failure\n");
            return RK_ERROR;
        }
    }

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: RockcodecDevShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 11:19:43
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
SHELL FUN rk_err_t RockCodecShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if ( StrLenA( pstr) != 0)
        return RK_ERROR;

    rk_print_string("Rockcodec命令集提供了一系列的命令对Rockcodec进行操作\r\n");
    rk_print_string("Rockcodec包含的子命令如下:          \r\n");
    rk_print_string("pcb       显示pcb信息               \r\n");
    rk_print_string("open      打开Rockcodec             \r\n");
    rk_print_string("test      测试Rockcodec             \r\n");
    rk_print_string("close     关闭Rockcodec             \r\n");
    rk_print_string("suspend   suspend Rockcodec         \r\n");
    rk_print_string("resume    resume Rockcodec          \r\n");
    rk_print_string("read      read Rockcodec            \r\n");
    rk_print_string("write     write Rockcodec           \r\n");
    rk_print_string("control   control Rockcodec         \r\n");
    rk_print_string("help      显示Rockcodec命令帮助信息 \r\n");

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: RockCodecShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
rk_err_t RockCodecShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.pcb : pcd info \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: RockCodecShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
rk_err_t RockCodecShellTest(HDC dev, uint8 * pstr)
{
    HDC hRockCodec;
    //RockCodec_DEVICE_CLASS * pDev;
    rk_err_t ret;
    RockCodec_DEVICE_CONFIG_REQ_ARG stArg;
    uint32 vol,time = 0;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    I2S_DEV_ARG stI2Sarg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.test : test RockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif
    SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//测试默认12M
    //uint32 SetI2SFreq(UINT32 I2sId,Clock_Source_Sel Clk_Source,UINT32 TargetFreqHz)
//    #ifdef _JTAG_printf_
//    ret=RKDev_Create(DEV_CLASS_DMA, DMA_CHN1, NULL);
//    if (ret != RK_SUCCESS)
//    {
//        printf("DMA RKDev_Create failure\n");
//        return RK_ERROR;
//    }
//    printf("DMA RKDev_Create success\n");
//    #endif

//    stI2Sarg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN0, NOT_CARE);
//    if (stI2Sarg.hDma == NULL)
//    {
//        printf("DMA RKDev_Open failure\n");
//        return RK_ERROR;
//    }
//    printf("DMA RKDev_Open success\n");
//    stI2Sarg.i2s_SelectMod = I2S_SEL_ACODEC;
//    ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH0, &stI2Sarg);
//    if (ret != RK_SUCCESS)
//    {
//        printf("I2S RKDev_Create failure\n");
//        return RK_ERROR;
//    }
//    printf("\nI2S RKDev_Create success\n");
//    stRockCodecDevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH0, NOT_CARE);
//    if (stRockCodecDevArg.hI2s == NULL)
//    {
//        printf("I2SDev Open failure\n");
//        return RK_ERROR;
//    }
//    printf("\n I2SDev Open OK\n");
//    ret = RKDev_Create(DEV_CLASS_ROCKCODEC, 0,&stRockCodecDevArg);
//    if (ret != RK_SUCCESS)
//    {
//        printf("ROCKCODEC Create failure\n");
//        return RK_ERROR;
//    }
//    printf("ROCKCODEC Create success\n");
    hRockCodec = RKDev_Open(DEV_CLASS_ROCKCODEC,0,NOT_CARE);

    //RockcodecDevInit(hRockCodec);//Open I2S clk CODEC Initialization is the first configuration step

    RockcodecDev_SetRate(hRockCodec,CodecFS_44100Hz);//Smaple rate PLL  CodecFS_44100Hz
    //RockcodecDev_SetVol(hRockCodec,27);
    RockcodecDev_SetVol(hRockCodec, EQ_NOR, 30);

    //ACodec InitI2S_TX/RX En I2SCLK
    //ACodec_Set_I2S_Mode(I2S_MASTER_MODE,I2S_DATA_WIDTH16,I2S_NORMAL_MODE,I2S_MASTER_MODE);

    //enable ADC power on
    RockcodecDev_SetMode(hRockCodec,Codec_DACoutHP,CodecFS_44100Hz);
    while (1)
    {
#ifdef RockTest
        RockcodecDev_Write(hRockCodec,(uint8 *)Codecoutptr, length, ASYNC_MODE);
#endif
        time++;
        if (time == 800)
        {
            UartDev_Write(dev, "test over",9,SYNC_MODE,NULL);
            break;
        }
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: RockCodecShellOpen
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
rk_err_t RockCodecShellOpen(HDC dev, uint8 * pstr)
{
    HDC hRockCodec;
    RockCodec_DEVICE_CLASS * pDev;
    rk_err_t ret;
    RockCodec_DEVICE_CONFIG_REQ_ARG stArg;
    uint32 vol;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    I2S_DEV_ARG  stI2Sarg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.open : openRockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif
#ifdef __DRIVER_I2S_I2SDEVICE_C__
    stI2Sarg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN0, NOT_CARE);
    stI2Sarg.i2s_SelectMod = I2S_SEL_ACODEC;
    ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2S RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }

    stRockCodecDevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, 0, NOT_CARE);
    ret = RKDev_Create(DEV_CLASS_ROCKCODEC, 0,&stRockCodecDevArg);

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "open failure",17,SYNC_MODE,NULL);
    }
#endif

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: RockCodecShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ROCKCODEC_ROCKCODECDEVICE_SHELL_
rk_err_t RockCodecShellCreate(HDC dev, uint8 * pstr)
{
    HDC hRockCodec;
    RockCodec_DEVICE_CLASS * pDev;
    rk_err_t ret;
    RockCodec_DEVICE_CONFIG_REQ_ARG stArg;
    uint32 vol;
    ROCKCODEC_DEV_ARG stRockCodecDevArg;
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.open : openRockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif

#ifdef __DRIVER_I2S_I2SDEVICE_C__
    stRockCodecDevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH0, NOT_CARE);
    ret = RKDev_Create(DEV_CLASS_ROCKCODEC, 0,&stRockCodecDevArg);

    if (ret != RK_SUCCESS)
    {
        printf("open failure");
    }
#endif

    return RK_SUCCESS;
}

#endif
#endif





