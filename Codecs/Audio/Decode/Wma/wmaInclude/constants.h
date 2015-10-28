//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 2002

Module Name:

    Constants.h

Abstract:

    Contants and macros used in thruout the code.

Author:

    Wei-ge Chen (wchen) 12-March-1998

Revision History:


*************************************************************************/

#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

// Angle can go from -MAX_ROTATION_ANGLE_INDEX to (MAX_ROTATION_ANGLE_INDEX-1).
#define ROTATION_ANGLE_BITS 6
#define MAX_ROTATION_ANGLES (1<<ROTATION_ANGLE_BITS)
#define MAX_ROTATION_ANGLE_INDEX (MAX_ROTATION_ANGLES>>1)

#define MAX_CHANNEL 8

//#ifdef  PI
//#undef  PI
//#endif
#define PI  3.1415926535897932384626433832795
// value prior to 10/5/99 as a float: #define PI  3.14159265359F

#define FLOAT_SHRT_MAX  32767.0F - 10.0F
#define FLOAT_SHRT_MIN -32768.0F + 10.0F
#define FLOAT_INT_MAX  2147483647.0F - 10.0F
#define FLOAT_INT_MIN -2147483648.0F + 10.0F

//#ifdef  BITS_PER_BYTE
//#undef  BITS_PER_BYTE
//#endif
#define BITS_PER_BYTE 8

#ifdef  BITS_PER_DWORD
#undef  BITS_PER_DWORD
#endif
#define BITS_PER_DWORD      32

#ifdef  BITS_PER_QWORD
#undef  BITS_PER_QWORD
#endif
#define BITS_PER_QWORD      64

#ifdef  NBITS_PACKET_CNT
#undef  NBITS_PACKET_CNT
#endif
#define NBITS_PACKET_CNT    4

#ifdef  NBITS_FRM_CNT
#undef  NBITS_FRM_CNT
#endif
#define NBITS_FRM_CNT       4

#ifdef  NBITS_PACKET_EMPTYNESS
#undef  NBITS_PACKET_EMPTYNESS
#endif
#define NBITS_PACKET_EMPTYNESS 1

#ifdef  NBITS_FORCE_PACKETLOSS
#undef  NBITS_FORCE_PACKETLOSS
#endif
#define NBITS_FORCE_PACKETLOSS 1

#ifdef  NUM_BARK_BAND
#undef  NUM_BARK_BAND
#endif
#define NUM_BARK_BAND 28

#ifdef  LOW_CUTOFF_V1
#undef  LOW_CUTOFF_V1
#endif
#define LOW_CUTOFF_V1 3        //TBD

#ifdef  LOW_CUTOFF
#undef  LOW_CUTOFF
#endif
#define LOW_CUTOFF 0        //wchen: no more due to bug#19144

#define PWRTODB_STEP        ((Float)2.5)
#define PWRTODB_NOSCALE     24 // 61/0.4, Calculated using B=608 in 10*log10(0xFFFFFFFF*2^0/B) - 7
#define PWRTODB_SIZE        57 // 143.5/0.4, Calculated using B=1 in 10*10log10(0xFFFFFFFF*2^18/B) - 7
#define PWRTODB_FULLSCALE   20 // We chop off 18 whole bits (+ any fraction bits) in full-scale pwr-to-db tbl

// Number of bits to discard when calculating power (max power > U32, need to make room)
#define PWRTODB_DEFSACRIFICE    (PWRTODB_FULLSCALE + 2*TRANSFORM_FRACT_BITS)

// power reduction factor in dB for masking @ same frequency
#ifdef INTEGER_ENCODER
#define MIN_THRESHOLD_IN_DB_V1  ((I32)(25 / PWRTODB_STEP ))
#else   // INTEGER_ENCODER
#define MIN_THRESHOLD_IN_DB_V1  25
#endif  // INTEGER_ENCODER

#ifdef  MIN_MASKQ_IN_DB_V1
#undef  MIN_MASKQ_IN_DB_V1
#endif
#define MIN_MASKQ_IN_DB_V1 (Int) (MIN_THRESHOLD_IN_DB_V1 / MASK_QUANT)

#ifdef  MASK_QUANT      // in db
#undef  MASK_QUANT
#endif

// MASK_QUANT is used only in V1, V2.
#define MASK_QUANT 2.5

#ifdef  NBITS_FIRST_MASKQUANT
#undef  NBITS_FIRST_MASKQUANT
#endif
#define NBITS_FIRST_MASKQUANT 5

#ifdef  FIRST_V2_MASKQUANT
#undef  FIRST_V2_MASKQUANT
#endif
#define FIRST_V2_MASKQUANT 36
#define FIRST_V3_MASKQUANT 45

#ifdef  MAX_MASKQUANT_DIFF
#undef  MAX_MASKQUANT_DIFF
#endif
#define MAX_MASKQUANT_DIFF 60  // limited by the huffman table

#ifdef  NUM_BITS_QUANTIZER
#undef  NUM_BITS_QUANTIZER
#endif
#define NUM_BITS_QUANTIZER 7

#ifdef  NUM_BITS_QUANTIZER_V3
#undef  NUM_BITS_QUANTIZER_V3
#endif
#define NUM_BITS_QUANTIZER_V3 5


// both as the escape code and the range...
#ifdef  MSA_MAX_QUANT_ESC
#undef  MSA_MAX_QUANT_ESC
#endif
#define MSA_MAX_QUANT_ESC ((1<<NUM_BITS_QUANTIZER) - 1)

#ifdef  MSA_MAX_QUANT_ESC_V3
#undef  MSA_MAX_QUANT_ESC_V3
#endif
#define MSA_MAX_QUANT_ESC_V3 ((1<<NUM_BITS_QUANTIZER_V3) - 1)

#ifdef  MAX_QUANT
#undef  MAX_QUANT
#endif
#define MAX_QUANT 129   /* 138 has been seen by decoder */

#ifdef  MAX_QUANTSTEPSIZE
#undef  MAX_QUANTSTEPSIZE
#endif
#define MAX_QUANTSTEPSIZE (2 * MAX_QUANT + 1)

#ifdef  MIN_QUANT
#undef  MIN_QUANT
#endif
#define MIN_QUANT 1     /*  was once 10; 22 is lowest value seen by decoder */

#define MAX_RUNLEVEL_44SM_QB 60
#define MAX_RUNLEVEL_44SS_QB 40
#define MAX_RUNLEVEL_44SM_OB 340
#define MAX_RUNLEVEL_44SS_OB 180
#define MAX_RUNLEVEL_16SM_OB 70
#define MAX_RUNLEVEL_16SS_OB 40

//lpc constants;need clean up
#define LPCORDER 10
#define FOURTYFOURS_QB 1
#define FOURTYFOURS_OB 2
#define SIXTEENS_OB 3

#define MSA_STEREO_WEIGHT 1.6
// Assumes subwoofer presence: 30Kbps savings@128 kbps wrt independent coding (158/128) * 6/5
#define MSA_MULTICH_WEIGHT 1.48F

#define MIN_EQ -35
#define MAX_EQ 15
#define MIN_RATE BITS_PER_BYTE

#define NUM_PREV_POWER 8
#define NUM_TRANS_FILTER 3

// Constants
#define QUANTSTEP_DENOMINATOR 20
#define QUANTSTEP_FRACT_BITS 28

#define TRANSFORM_FRACT_BITS    5

// The following should only be used when compiling the integer encoder
#ifdef INTEGER_ENCODER
#define INVWEIGHTFACTOR_FRACTBITS   27
#define INVQUANTSTEP_FRACTBITS      27
#define WEIGHTINPUT_FRACTBITS   14  // prvWeightInput
#define WEIGHT_FRACTBITS        4   // prvGetMaskingThresholdMono and others in masking.c

// Discard bits to allow power value to stay within U32 range. We only need to discard
// 3 bits but discard 1 extra so sqrt(2^TRANSIENT_BITDISCARD) is power of 2
#define TRANSIENT_THRFRACTBITS  4       // Allows 1/16 precision (see threshold init)
#define TRANSIENT_BITDISCARD    (3 + 1 + TRANSIENT_THRFRACTBITS)
#endif  // INTEGER_ENCODER


#define NOT_DETECTED_YET -2
#define NO_TRANSIENT     -1

// Moved here from msaudio.h
#define MAX_SUBFRAMES       32

// V3 LLM. Chao. Add.
#define MAX_LPC_ORDER_UNIFIED       32
#define MIN_LPC_ORDER_UNIFIED       1
#define LPC_ORDER_UNIFIED_BITS      5 // 5 bits can represent order from 1 to 32.

#define MAX_LPC_INTBITS_UNIFIED     8
#define MIN_LPC_INTBITS_UNIFIED     1
#define BITS_LPC_INTBITS_UNIFIED    3


#define LLM_LPC_ORDER               (U32)40
#define LLM_LPC_ORDER_MLLM          4   // must less than LLM_LPC_ORDER!

#define MAX_LSP_ORDER_WMA 50
#define MAX_LPC_ORDER_WMA 50

#define LSQ_QUAN_STEPSIZE           0.0025F
#define LOSSY_OR_MIXEDLOSSLESS_MODE 0x0000

#define NO_CHANNEL_MIXING           WMAB_FALSE
#define DO_CHANNEL_MIXING           WMAB_TRUE
#define NO_MCLMS                    WMAB_FALSE
#define DO_MCLMS                    WMAB_TRUE
#define NO_ARITHMETIC_CODING        WMAB_FALSE
#define DO_ARITHMETIC_CODING        WMAB_TRUE


typedef enum {CH_MIX_INDPDT, CH_MIX_S_TRAN, CH_MIX_DIFF_L, CH_MIX_DIFF_R} ChannelMixingMode;
typedef enum {COLOMB_CODING, COLOMB_ARITHMATIC_CODING} EntropyCodingScheme;

//V3 LLMA
#define MAX_CHANNEL_MIXING_ROUND 0

#define MLLM_FILTER_SCALING             6   // 24+1 = 25 bits. 32-25 = 7. We use 6 bits for scaling.
#define MLLM_QUANTSTEPSIZE_BITS         8   // Bits for quantization stepsize.

//  4 (2 bits) is the maximum value in its worst case (24 bits audio) since we use I32 and scaling factor is 6. (2 + 6 + 24 = 32)
#define LLM_VERB_ACFILTER_ORDER_MIN     1
#define LLM_VERB_ACFILTER_ORDER_MAX     4  //max length (order) of AC filter in LLM_VERB.
#define LLM_VERB_ACFILTER_ORDER_BITS    4  // 2+2 We give 2 more bits. Then the actuall ORDER_MAX is 2^4=16.

//#define LLM_VERB_ACFILTER_SCALING       6   //scaling factor of AC filter coefficients.

//V3 LLM-B. Chao. Add. 03-18-02
#define LLMB_MCLMS_TTL_MAX              1  // sorry, only 1 right now.
//we set all filter order and scaling in ResetAll.
//#define LLMB_MCLMS_ORDER_EACHCH         2
#define LLMB_MCLMS_MIN_ORDER_EACHCH     2
#define LLMB_MCLMS_MAX_ORDER_EACHCH     8  //Multi-channel LMS's max order in each channel. The total order is LLMB_MCLMS_ORDER_EACHCH * cChannel
#define LLMB_MCLMS_ORDER_BITS           4 //8/2=4   We give 2 more bits. Then the actuall ORDER_MAX is 2*2^4=32.
//#define LLMB_MCLMS_ORDER_EACHCH         2
//#define LLMB_MCLMS_LAMDA         20

//V3 LLM-B. Chao. Add. 03-15-02
#define LLMB_CLMSFLT_TTL_MIN            1
#if defined (BUILD_WMAPRO) && !defined (BUILD_WMALSL)
#define LLMB_CLMSFLT_TTL_MAX            2        // we support up to 2 cascaded LMS filters in Unified WMA.
#else
#define LLMB_CLMSFLT_TTL_MAX            4        // we support up to 4 cascaded LMS filters.
#endif
#define LLMB_CLMSFLT_TTL_BITS           3       // 1,2,3,4 2 bits. We give 1 more bit. Then the actual TTL_MAX is 2^3 = 8;

#define LLMB_CLMSFLT_MIN_ORDER          8       // This is the limitation for bitstream.
#define LLMB_CLMSFLT_MIN_ORDER_REAL     8      // This is the actuall limitation due to MMX implementation.
#if defined (BUILD_WMAPRO) && !defined (BUILD_WMALSL)
#define LLMB_CLMSFLT_MAX_ORDER          32
#else
#define LLMB_CLMSFLT_MAX_ORDER          256
#endif
#define LLMB_CLMSFLT_ORDER_BITS         7   //256/8=32 We give 2 more bits. Then the actual MAX_ORDER is 8*2^7 = 1024;

#define LLMB_INTEGERIZATION_SCALING_MAX     12  // actuall speed 1/2^12
#define LLMB_INTEGERIZATION_SCALING_MIN     0   // actuall speed 1/2^0
#define LLMB_INTEGERIZATION_SCALING_BITS    4   // 12-0+1 = 13. And I give one more bit. The actuall SCALING_MAX is 15

#define LLMB_QUANTSTEPSIZE_B4_PLLM          1
#define LLMB_QUANTSTEPSIZE_B4_PLLM_MIN      1
#define LLMB_QUANTSTEPSIZE_B4_PLLM_MAX      128
#define LLMB_QUANTSTEPSIZE_B4_PLLM_BITS     8   // we give one more bit. The actuall MAX is 2^8 = 256

// I don't know how to define an array. I just hard coded them in msaudio.c files.
// Search LLMB_CLMS_ORDERS[LLMB_CLMS_TTL] in msaudio.c
//LLMB_CLMS_ORDERS[LLMB_CLMS_TTL] = { 600, 60 , 6};
//#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_HALF      (U32)16
//#define LLMB_RESIDUE_MEMORY_TTLWEIGHT           (U32)32
#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_SCALING   5
//#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_X_2       (U32)64
//#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_X_1_5      (U32)48
//#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_X_3       (U32)96
#define LLMB_RESIDUE_MEMORY_TTLWEIGHT_BITS      3 // 3 bits can support 2^(0-7) = 1-128 scaling.

#define LLMA_RESIDUE_MEMORY_TTLWEIGHT (U32)32
// V3 LLMB
#define TRANSIENT_POSITION_DELAY 32
#define RESQ_ESCAPE_VALUE   32

// default v3 stereo channel xform
#define INT_SQRT2 362
#define INT_SQRT2_BITS 8
#define FLT_SQRT2 (362.0F / 256.0F)
#define FLT_INV_SQRT2 (1.0F / FLT_SQRT2)
//#define FLT_SQRT2 1.4142135623730950488016887242097F
//#define FLT_INV_SQRT2 0.70710678118654752440084436210485F

// **************************************************************************
// Encoding Options
// **************************************************************************
//v2
#define ENCOPT_BARK             0x0001
#define ENCOPT_V5LPC            0x0020
#define ENCOPT_SUPERFRAME       0x0002
#define ENCOPT_SUBFRAME         0x0004
#define ENCOPT_SUBFRAMEDIVMASK  0x0018
#define ENCOPT_SUBFRAMEDIVSHR   3
//v3
#define ENCOPT3_PURE_LOSSLESS          0x0001
#define ENCOPT3_FRM_SIZE_MOD           0x0006
#define ENCOPT3_SUBFRM_DIV             0x0038
#define ENCOPT3_WRITE_FRAMESIZE_IN_HDR 0x0040
#define ENCOPT3_GENERATE_DRC_PARAMS    0x0080
#define ENCOPT3_RTMBITS                0x0100
//#define ENCOPT3_ADV_FRMSIZE_MOD_HIGH   0x0600
//#define ENCOPT3_ADV_FRMSIZE_MOD_ON     0x0800
#define ENCOPT3_EXTRABITS_IN_PKTHDR    0x1000
#define ENCOPT3_UNSUPPORTED_OPTS       0xec00 // adjust this when adding opts

// Upper limit of buffer size (Bucket buffer, Vbb buffer, cbOutputBuffer)/ Raw PCM size.
// can be changed to 1.1 to save space.
// FrmBucket buffer is Raw PCM size * LLM_BUFSIZE_OVER_RAWPCM. So theoretically, vbb's buffer size
// should be cBitPCM * LLM_BUFSIZE_OVER_RAWPCM + overhead bits. But since we already leave enough
// room in FrmBucket buffer, we are safe.
#define LLM_BUFSIZE_OVER_RAWPCM      2
#define MLLM_BUFSIZE_OVER_RAWPCM     1.5
// This value is also defined in upper layer. For example in liteencoder.
// I hope upper layer can use this define.
#define MAX_PACKET_SIZE             64000
#define MATRIX_INVERT_TINY ((Float)1.0e-5)

#define MCMASK_MONO   SPEAKER_FRONT_CENTER
#define MCMASK_STEREO (SPEAKER_FRONT_LEFT | SPEAKER_FRONT_RIGHT)
#define MCMASK_THREE  (SPEAKER_FRONT_CENTER | MCMASK_STEREO)
#define MCMASK_BACK   (SPEAKER_BACK_LEFT | SPEAKER_BACK_RIGHT)
#define MCMASK_QUAD   (MCMASK_STEREO | MCMASK_BACK)
#define MCMASK_5      (MCMASK_THREE | MCMASK_BACK)
#define MCMASK_5DOT1  (MCMASK_5 | SPEAKER_LOW_FREQUENCY)
#define MCMASK_6DOT1  (MCMASK_5DOT1 | SPEAKER_BACK_CENTER)
#define MCMASK_7DOT1  (MCMASK_5DOT1 | SPEAKER_FRONT_LEFT_OF_CENTER | SPEAKER_FRONT_RIGHT_OF_CENTER)

#endif //__CONSTANTS_H_
