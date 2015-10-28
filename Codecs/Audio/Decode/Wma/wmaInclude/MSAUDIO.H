//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 1999

Module Name:

    MsAudio.h

Abstract:

    Implementation of public member functions for CAudioObject.

Author:

    Wei-ge Chen (wchen) 11-March-1998

Revision History:


*************************************************************************/

// Compile Time Flags normally defined in project seetings
// these vary from specific project to project
// BUILD_INTEGER - build integer version
// BUILD_INT_FLOAT - build float version of integer tree = !BUILD_INTEGER
// ENCODER - build encoder
// DECODER - build decoder = !ENCODER
// ENABLE_ALL_ENCOPT - build support for all encoder options (otherwise just HighRate)
// ENABLE_LPC - build LPC spectrogram weighting for LowRate configurations (only defined if ENABLE_ALL_ENCOPT)
// ENABLE_EQUALIZER - build support for equalizer
// Known Useful Combinations
//   !BUILD_INTEGER &&  ENABLE_ALL_ENCOPT &&  ENABLE_LPC    = Normal Encoder
//    BUILD_INTEGER &&  ENABLE_ALL_ENCOPT &&  ENABLE_LPC    = Partially Integerized Encoder
//    BUILD_INTEGER &&  ENABLE_ALL_ENCOPT &&  ENABLE_LPC    = Normal Integer Do-It-All Decoder
//    BUILD_INTEGER &&  ENABLE_ALL_ENCOPT && !ENABLE_LPC    = Normal Integer HighRate & MidRate Decoder
//    BUILD_INTEGER && !ENABLE_ALL_ENCOPT && !ENABLE_LPC    = Normal Integer HighRate Only Decoder
//   !BUILD_INTEGER &&  ENABLE_ALL_ENCOPT &&  ENABLE_LPC    = IntFloat Do-It-All Decoder
//   !BUILD_INTEGER &&  ENABLE_ALL_ENCOPT && !ENABLE_LPC    = IntFloat HighRate & MidRate Decoder
//   !BUILD_INTEGER && !ENABLE_ALL_ENCOPT && !ENABLE_LPC    = IntFloat HighRate Only Decoder
#ifndef __MSAUDIO_H_
#define __MSAUDIO_H_


// ***********************************************

#ifdef __cplusplus
extern "C"    // only need to export C interface if
{
    // used by C++ source code
#endif

//#include <stdio.h>
//#include <stdlib.h>

#include "macros.h"
//#include "aumalloc.h"
#include "DecTables.h"
#include "audio_table_room.h"

#ifdef WMA_TARGET_ARM
//ARM specific optimization
//define as 1 to turn on asm optimization, 0 to turn it off
#define WMA_OPT_LPCSTD_ARM              0       //lpcstd_arm.s
#define WMA_OPT_FFT_ARM                 0       //fft_arm.s
#define WMA_OPT_AURECON_ARM             1       //msaudio_arm.s
#define WMA_OPT_INVERSQUAN_LOWRATE_ARM  0       //lowrate_arm.s
#define WMA_OPT_SUBFRAMERECON_ARM       1       //msaudiostdpro_arm.s
#endif //WMA_TARGET_ARM


// #define ENABLE_ARITHCODER
// If we want to turn ENABLE_AIRTHCODER on here, we also need to define it in dectbls.c since dectbls.c
// does not include msaudio.h

//#ifdef ENABLE_ARITHCODER
//#include "arithcoder_c.h"
//#endif //ENABLE_ARITHCODER

#include "playropt.h"

// code for testing.
// #define TEST_CODE
//#ifdef TEST_CODE
//#include <time.h>
//    //#define TEST_CORRUPT_BITSTREAM
//    //#define TEST_V3_RTM_BITSTREAM
//    //#define TEST_UNIFIED_MIXEDLLM
//    //#define TEST_STANDALONE_PURELLM
//    //#define TEST_LLM_COMPUTATION_CONSISTENCE
//    //#define TEST_LLM_TRANSIENT_DETECTION
//    //#define TEST_UNIFIED_PURELLM
//#endif // TEST_CODE

// code for experiment. maybe used for further improvement.
// #define EXPT_CODE


// Encoder-only/decoder-only defines are no longer allowed. All audio components
// must be built with same defines so that encoder and decoder may both be linked into
// one EXE.
//#if defined(ENCODER) || defined(DECODER)
//#error Encoder- and Decoder-specific defines are no longer allowed. Encoder/decoder-specific code must perform encoder/decoder identification at runtime.
//#endif  // defined(ENCODER) || defined(DECODER)

// The INTEGER_ENCODER #define was, where possible, replaced with BUILD_INTEGER.
// The remaining INTEGER_ENCODER's have remained as "tombstones" and should be replaced
// with a run-time decision of which codepath to take. As described above, encoder- and
// decoder-specific defines are no longer allowed.
//#ifdef INTEGER_ENCODER
//#error Encoder-specific defines no longer allowed. See above comments.
//#endif  // INTEGER_ENCODER


// Forward data type decl
    typedef struct PerChannelInfoENC PerChannelInfoENC;
    typedef struct _DRC DRC;

    typedef void (*PFNSETSAMPLE)(const PCMSAMPLE iValue, PCMSAMPLE *pCurrentPos,
                                 const struct CAudioObject *pau, const Int iOffset);
    typedef PCMSAMPLE(*PFNGETSAMPLE)(const PCMSAMPLE *pCurrentPos,
                                     const Int nBytePerSample,
                                     const Int nValidBitsPerSample,
                                     const Int iOffset);


    typedef enum {STEREO_INVALID = -1, STEREO_LEFTRIGHT = 0, STEREO_SUMDIFF} StereoMode;
    typedef enum {MULTICH_INVALID = 0, MULTICH_HADAMARD = 1, MULTICH_IDENTITY = 2, MULTICH_DCT = 3} PredefinedXformType;
    typedef enum {RLCTBL_SM = 0, RLCTBL_SS} RlcTblType;
    typedef enum {CONFIG_CURRFRM = 0, CONFIG_NEXTFRM = 1} ConfigFrame;

#define BITDEPTH_ID(validBits, bytePerSample)  (((validBits - 1) << 2) | (bytePerSample - 1))
    typedef enum {BITDEPTH_24   = BITDEPTH_ID(24, 3),
                  BITDEPTH_2024 = BITDEPTH_ID(20, 3),
                  BITDEPTH_16   = BITDEPTH_ID(16, 2)
                 } BITDEPTH;

    typedef enum {CODEC_NULL, CODEC_STEADY, CODEC_DONE, CODEC_BEGIN, CODEC_LAST, CODEC_ONHOLD, CODEC_HEADER, CODEC_DATA} Status;
    typedef enum {FMU_DONE = 0, FMU_QUANTIZE, FMU_PROCESS} FMU_STATUS;
    typedef enum {FFT_FORWARD = 0, FFT_INVERSE} FftDirection;

    typedef void (*PFNFFT)(CoefType data[], Int nLog2np);

    typedef struct SubFrameConfigInfo
    {
        I16  m_cSubFrame;
        I16  m_cMaskUpdate;                  //for enc only
        own I16* m_rgiSizeBuf; //include prev and next subfrm
        I16* m_rgiSubFrameSize;
        I16* m_rgiSubFrameStart;
        U8*  m_rgfMaskUpdate;
        U8*  m_bUnifiedLLM; //encoder only
    } SubFrameConfigInfo;

#define m_cSubband m_cSubFrameSampleHalf //to be used as in freq. domain

    /*****************************************************************************
        Lossless defines. Root of all lossless code.
    *****************************************************************************/
//V3 LLM-B. Chao. Add. 03-20-02
// --- Pure Lossless Mode --- //

    typedef struct MCLMSPredictor
    {
        I32     m_iOrder;       //predictor order.
        I32     m_iOrder_X_CH;
        I32     m_iScaling; //scaling ratio between floating point and integer version of the filter. in power of 2.
        I32     m_iScalingOffset; //offset value before scaling.
        I32     m_cBitsSentPerCoeff;
        I16     m_iUpdStepSize; //stepsize of updating.
        I32     m_iRecent;      //point to the last (most recent) value in PrevBuffer.
        own I32 * m_rgiPrevVal; //circular buffer of previous values used for predict the current value.
        own I16 * m_rgiFilter;  //filter.
        own I16 * m_rgiFilterBackup;  //filter.
        own I16 * m_rgiFilterCurrTime;  //filter used for prediction within the current PCM samples.
        //For example, in stereo, we can use L to predict R.
        own I16 * m_rgiFilterCurrTimeBackup;
        own I16 * m_rgiUpdate;
    } MCLMSPredictor;

//V3 LLM-B. Chao. Add. 03-15-02
    typedef struct LMSPredictor
    {
        I32     m_iOrder;       //predictor order
        I32     m_iOrder_Div_2;   //order divided by 2
        I32     m_iOrder_Div_4;   //order divided by 4
        I32     m_iOrder_Div_8;   //order divided by 8
        I32     m_iOrder_Div_16;   //order divided by 16
        I32     m_cCoeffsSent;      //How many coefficients are sent in bitstream
        I32     m_cBitsSentPerCoeff; //If a coefficient is sent, how many bits are sent. >2.
        //First 2 bits are used for sign (1) and integer part (1).
        //Therefore the region is 01.11-10.00 if we use 4 bits.
        I32     m_iScaling; //scaling ratio between floating point and integer version of the filter. in power of 2.
        I32     m_iScalingOffset; //offset value before scaling.
        I16     m_iUpdStepSize; //stepsize of updating. will replace m_fltUpdLmt.
        I16     m_iUpdStepSize_X_8;
        I32     m_iRecent;      //point to the last (most recent) value in PrevBuffer.
        own I32 * m_rgiPrevVal; //circular buffer of previous values used for predict the current value.
#if 0
        own I32 * m_rgiPrevValBackup;
#endif
        own I16 * m_rgiUpdate;
        own I16 * m_rgiUpdateBackup;
        own I16 * m_rgiFilter;  //filter.
        own I16 * m_rgiFilterBackup; // Previous filter. This is also used for filter coefficients which are sent in bitstream.
    } LMSPredictor;

//#ifdef EXPT_CODE
////V3 LLMB. Chao. Add. 04-02-02
//    typedef struct RLSPredictor
//    {
//        U16     m_iOrder;       //predictor order.
//        Float   m_fltLamda;
//        Float   m_fltDelta;
//
//        I16     m_iRecent;      //point to the last (most recent) value in PrevBuffer.
//        own Float * m_rgfltPrevVal; //circular buffer of previous values used for predict the current value.
//        own Float * m_rgfltFilter;  //filter.
//        own Float * m_rgfltP;   //iOrder X iOrder
//        own Float * m_rgfltPp;   //iOrder X iOrder
//        own Float * m_rgfltp;   //iOrder X 1
//        own Float * m_rgfltK;   //iOrder X 1
//    } RLSPredictor;
//#endif //EXPT_CODE

    #define NANO_C
    typedef I32 (*PFGETRUNLEVELVALUE)(const U8 *tbl, U32 idx);

    typedef struct PerChannelInfo
    {
        //whenever the memory gest moved to buckets, update is needed
        // Decoder and Global Members
        I32*           m_rgiCoefQ;                      //quantized coef
        Int*           m_rgiMaskQ;                      //quantized maskq ((could be I16))
        Int*           m_rgiMaskQResampled;             // V3. Anchor mask resampled for current subframe size.

        U8*            m_rgbBandNotCoded;               //band not coded
        FastFloat*     m_rgffltSqrtBWRatio;             //MidRate/LowRate for Bands not coded
        Int*           m_rgiNoisePower;
        const U16*     m_rgiHuffDecTbl;                 //decoder only

#ifdef NANO_C
			const U8*	  m_rgiRunEntry;				   //decoder only
			const U8*	  m_rgiLevelEntry;				   //decoder only
			PFGETRUNLEVELVALUE m_getRunValue;
			PFGETRUNLEVELVALUE m_getLevelValue;
#else
			const U16*	   m_rgiRunEntry;					//decoder only
			const U16*	   m_rgiLevelEntry; 				//decoder only
#endif


        Int            m_cSubbandActual;
        Int            m_iPower;                        //channel power
        // V3 LLM
        Int            m_iPowerOri;                     //original channel power.
        I8             m_iPaddedZeros;                  //Sometimes the lowest n bits of PCM samples are all zeros.

        Int            m_iActualPower;                  // Power as suggested by CoefQ/CoefRecon
        UInt*          m_rguiWeightFactor;              //weighting factor
        CBT*           m_rgiCoefRecon;                  //reconstructed coef
        CBT*           m_rgiPCMBuffer;                  // PCM output buffer pointer
        Int            m_iMaxMaskQ;
        StereoMode     m_stereoMode;                    // would be per bucket per channel

#define CURRGETPCM_INVALID  0x7FFF
        I16            m_iCurrGetPCM_SubFrame;          // reconstruction index

        //wchen: these were defined as V4V5_COMPARE, retired
        //wchen: we may not need these to the lookup table case
        BP2Type        m_fiSinRampUpStart;
        BP2Type        m_fiCosRampUpStart;
        BP2Type        m_fiSinRampUpPrior;
        BP2Type        m_fiCosRampUpPrior;
        BP2Type        m_fiSinRampUpStep;
        BP2Type        m_fiSinRampDownStart;
        BP2Type        m_fiCosRampDownStart;
        BP2Type        m_fiSinRampDownPrior;
        BP2Type        m_fiCosRampDownPrior;
        BP2Type        m_fiSinRampDownStep;

        I16            m_iCurrSubFrame;                    // for tiling
        I16            m_iCurrCoefPosition;   // current subframe starting point
        I16            m_cSubFrameSampleHalf;
        I16            m_cSubbandAdjusted;
        I16            m_iSizePrev;
        I16            m_iSizeCurr;
        I16            m_iSizeNext;
        I16            m_iCoefRecurQ1;
        I16            m_iCoefRecurQ2;
        I16            m_iCoefRecurQ3;
        I16            m_iCoefRecurQ4;

        //I16     m_cSubband;
        //Int     m_cSubbandAdjusted;
        Int     m_cSubFrameSampleAdjusted;
        Int     m_cSubFrameSampleHalfAdjusted;

        // Float-only members
        // The following is NOT YET INTEGERIZED but after it is,
        // it should be moved into the #ifndef BUILD_INTEGER above
        Float*         m_rgfltCoefRecon;                //reconstructed coef soon to be removed

        // The following are NOT YET INTEGERIZED but after they are,
        // they should be moved into the #ifndef BUILD_INTEGER above
        Float*         m_rgfltWeightFactor;
        FinalWeightType m_wtMaxWeight;

        Int            m_cLeftShiftBitsQuant;
        Int            m_cLeftShiftBitsTotal;
        QuantFloat     m_qfltMaxQuantStep;

        // Pointer to encoder data, if applicable
        PerChannelInfoENC*  ppcinfoENC;

        // This is encoder-only but I've put it here to avoid having to #include "msaudioenc.h"
        // in common files.
        Bool*            m_rgfMaskNeededForBark;   // for header squeezing

        // V3 stuff
        //U8               m_ucQuantStepModifierIndex;         // Quality balancing
        //QuantStepType    m_qstQuantStepModifier;             // Quality balancing

        SubFrameConfigInfo* m_rgsubfrmconfig;             // Use CONFIG_CURRFRM  & CONFIG_NEXTFRM for indexing.
        //Int              m_cSubFrameSampleHalfWithUpdate;    // Values known to be available at the decoder (V3)
        //Int              m_cValidBarkBandLatestUpdate;       // Values known to be available at the decoder (V3)
        Int              m_iMaskQuantMultiplier;             // Scales MASK_QUANT for better compression
        Bool             m_fAnchorMaskAvailable;

        // V3 unseekable subframe
        // ppcinfo and ppcinfoENC usually appear as pair. In encoder, there are several copies of ppcinfo/ppcinfoENC. Each
        // frame bucket has one pair and pau has one common pair. In encoder although we malloc the common pair
        // as pau->rgpcinfo pauenc->rgpcinfoENC, we don't actually use them to access the common pair. pau->rgpcinfo/ENC
        // is saved as pauenc->rgpcinfo/ENC/Com in prvInitEncoderSecondaryAllocated and pau->rgpcinfo/ENC is used to access the current bucket's pair.
        // In decoder, this is only one pair (common pair).
        // The common pair is malloced and initialized to zero in prvInitCommonAllocate.
        // Next two arrays are only valid in common pair.

        //V3 LLM-B. Chao. Add. 03-15-02
        //I16             m_cLMSPredictors;
        //LMSPredictor    m_rgLMSPredictors[LLMB_CLMSFLT_TTL_MAX];
        //I16             m_iLMSPredictorCurr;

        // we suppose 24 bits memory len < 256
        U32             m_iSum;
        U32             m_iSumBackup;
        I16             m_iColombK;

        Bool            m_bUpdateSpeedUp; // 0: regular, 1: fast.
        I32             m_iUpdSpdUpSamples;
        I32             m_iTransientPosition;
        //TODO Is U32 enough
        U32             m_iShrtAve;     // need to be reset at seekable frame
        U32             m_iLongAve;     // need to be reset at seekable frame
        //I32             m_iLastSamples[LLM_VERB_ACFILTER_ORDER_MAX];  // need to be reset at seekable frame

        /*  moved to pau
        I32             m_iACFilter[LLM_VERB_ACFILTER_ORDER_MAX]; // AC filter.
        I32             m_iACFilterOrder;
        I32             m_iACFilterCurr; // for decoder.
        */



        Int m_iVecsInNonRLMode;

        // Needed by auGetPCM
        U32 m_cSamplesLeft;
        U16 m_cSamplesFromHistory;
        //U16 m_cLastCodedIndexV3;     // To reduce decoder complexity in V3

        // We can save some memory if we move some of following arrays to pau or pauenc then use pointer to access them.
        // Some of them are not necessary to be per FrmBucket.
        // V3 lossless Mode LPC. first half and second half.
        // currently since there is no subframe support in PLLM, this works well. But later if we (we should) use
        // subframe or tile structure, following definition should be moved to SubFrameConfigInfo. Chao.
        //Double           m_rgrgdblLLMLPCFilter[2][LLM_LPC_ORDER];  // should be moved to PerChannelInfo.
        //Float            m_rgrgfltLLMLPCFilterRec[2][LLM_LPC_ORDER]; // reconstructed LPC filter used on both encoder/decoder.
        // Residue should go to PerChannelInfoENC
        //I32              m_rgiLPCFilter[LLM_LPC_ORDER];
        //I32              m_rgiLPCPrevSamples[LLM_LPC_ORDER];
        short *dst_rgiCoefRecon;//add by evan wu
    } PerChannelInfo;

    typedef enum {LPC_MODE = 0, BARK_MODE} WeightMode;

    typedef struct CChannelGroupInfo
    {
        Int   m_cChannelsInGrp;           // Number of channels in the group
        own Bool* m_rgfChannelMask;
        Bool  m_fIsSuperGroupXform;       // Is this xform top level in hierarchy?
        Bool  m_fIsPredefinedXform;
        PredefinedXformType  m_predefinedXformType;
        Bool  m_fAllBarksOn;
        Bool  m_rgfXformOn[NUM_BARK_BAND];
        I8*   m_rgbRotationAngle;         // Quantized index
        I8*   m_rgbRotationSign;          // 0 or 1

        own ChXFormType *m_rgfltMultiXForward;
        own ChXFormType *m_rgfltMultiXInverse;
    } CChannelGroupInfo;

// Forward data type decl
    typedef struct CAudioObject CAudioObject;
    typedef struct CAudioObject
    {
        //Step Size
        QuantStepType  m_qstQuantStep;

        // packetization (has nothing to do with the rest)
        Int m_iPacketCurr;
        Int m_cBitPackedFrameSize;
        Int m_cBitPacketLength;

        //utility vars for entropy decoding for high rate only
        I32 m_cRunOfZeros;
        I32 m_iLevel;
        Int m_iSign;

        // utility vars for runlevel decoding of V3 mask: should go to decoder.
        I16 m_cRunOfMaskZeros;
        I16 m_iMaskLevel;

        I16 m_iCurrReconMaskBand;
        U16     m_cChannel;

        Int m_iMaskSign;

        Bool  m_fNoiseSub;
        Float m_fltBitsPerSample;
        Float m_fltWeightedBitsPerSample;

        Int m_iMaxEscSize;          // in bits
        Int m_iMaxEscLevel;         // (1<<m_iMaxEscSize)

        Int     m_iVersion;
        I32     m_iEncodeOpt;
        Status  m_codecStatus;          //current status of the codec
        Bool    m_fSeekAdjustment;
        Int     m_iSamplingRate;        //audio sampling rate in Hz
        Int     m_cBytePerSec;
        U32     m_nBytePerSample;
        U32     m_nBitsPerSample;       // Same as m_nBytePerSample * 8, just avoid the calculation
        BITDEPTH m_iBitDepthSelector;
        U32     m_nChannelMask;
        I16     m_nSubWooferChannel;    // Channel number of subwoofer in channel array. -1 if it does not exist
        U16     m_nValidBitsPerSample;

        //I32     m_iPCMSampleMin;
        //I32     m_iPCMSampleMax;

        // V3 RTM bit stream
        //Bool    m_bV3RTM;

        //V3 LLM only! for mixed lossless mode. ChaoHe 8-20-2001
        //Bool    m_bEnforcedUnifiedLLM;          //Encoder encodes certain frames in pure lossless mode in WMA Pro.
        //Bool    m_fEnableUnifiedLLM;            // Two-pass encoder disables mixed lossless mode for now
        //Bool    m_bUnifiedLLM;       //Current subframe is coded in lossless or lossy mode. ChaoHe 8-20-2001

        //Bool    m_bUnifiedMixedLLM;
//    Bool    m_bMLLMUsePLLMPrevFrm;      //We need to know the prev frame in prvcalcQ1Q2 function.
        //Bool    m_bUnifiedPureLLMCurrFrm;      //We use pure lossless codec in mix mode.
        //Bool    m_bUnifiedPureLLMNextFrm;      // because we have frame delay in encoding (like confignextsubframe).

        //Bool    m_bFirstUnifiedPureLLMFrm;
        //Bool    m_bSecondUnifiedPureLLMFrm;
        //Bool    m_bLastUnifiedPureLLMFrm;

        //Bool    m_bDoUpdUpdtingSpd;

        //Bool    m_bDoLPC;
        //I16     m_iLPCOrder;
        //I16     m_iLPCFilterScaling;
        //I16     m_iLPCFilterIntBits;

//    I16     m_iMLLMFilterCoeff;         //
        //I16     m_iMLLMResidueQStepSize;    // Quantization stepsize.
        //V3 True LLM mode.
        //Bool    m_bPureLosslessMode;

        //Bool    m_bDoACFilter;          // apply AC filter to remove DC component at the begining.
        //Bool    m_bDoInterChDecorr;     // any inter channel decorrelation in current subframe.
        //Bool    m_bDoArithmCoding;      // use arithmetic coding to code LPC residues.
        //Bool    m_bSeekable;            // Whether the current frame is seekable. (only 1 subfrm per frm now.)
        // We need a seekable subFrm. But it may not be the current sbFrm depending on whether it can be seekable. (pau->m_bNextSbFrmCanBeSeekable)
        // in MLLMUsePLLM, it means next frame must be seekable because the current frame is RAMPCM.
        //Bool    m_bNeedSeekable;

        //I8      m_iPCMTrailZeros;       // bitplane Trailing zeros of current subframe.
        // Dynamic range compression stuff
        //U8 m_ucDrcFrameScaleFactor;
        I16     m_iCurrReconCoef;       //which coef we are doing; util. var for state machine


        //Bool    m_bOutputRawPCM;        // Output raw PCM if LPC fails.
        //I16     m_bPLLMQStepSize;       // Stepsize before pure lossless mode.
        //TODO: more the paudec.
        //U16     m_cGetPCMSamplesDone; // How many samples have been outputed in current frame.

        Bool    m_fAllowSuperFrame;
        Bool    m_fAllowSubFrame;
        Bool    m_fV5Lpc;

        Int     m_iSubFrameSizeWithUpdate;
        Int     m_iMaxSubFrameDiv;      //maximum no. division into subwindows
        Int     m_cMinSubFrameSample;   //min size of subframes in current frame; many change from frame to frame
        Int     m_cMinSubFrameSampleHalf;   //min size of subframes in current frame; many change from frame to frame
        Int     m_cMinSubFrameSampleQuad;   //min size of subframes in current frame; many change from frame to frame
        Int     m_cPossibleWinSize;

        //should be reset every subfrm in next version
        Int     m_cBitsSubbandMax;      //no. of bits for m_cSubband;
        Int     m_cFrameSample;         //no. of input/output samples = 2 * cSubband
        Int     m_cFrameSampleHalf;     //no. of input/output samples = cSubband
        Int     m_cFrameSampleQuad;     //no. of input/output samples = cSubband / 2

        Int     m_cLowCutOff;           //How many coeffs to zero at the low end of spectrum
        Int     m_cHighCutOff;          //How many coeffs to zero at the high end of spectrum
        Int     m_cLowCutOffLong;
        Int     m_cHighCutOffLong;

        Int  m_iWeightingMode;
        StereoMode  m_stereoMode;       // left-right or sum-difference coding
        Int     m_iEntropyMode;         //entropy coding mode
        Float   m_fltDitherLevel;       //=.12; Relative intensity of Roberts' pseudo-random noise quantization
        Int     m_iQuantStepSize;
        //Float   m_fltDctScale;
        Int     m_cValidBarkBand;       //valid bark band in range; some are out of range in the subband decompositio
        const Int*  m_rgiBarkIndex;     //corresponding coef index for each valid bark
        Int     m_cSubWooferCutOffIndex;//zeros after this
        I32* m_rgiCoefQ;             // just a pointer

        //random accessed memory
        //overall
        PerChannelInfo* m_rgpcinfo; //this is just a pointer now

        //transform
        own CBT*    m_rgiCoefReconOrig;
        //own CBT*    m_rgiCoefReconMLLMOrig; // Used by Mixed lossless decoder, when WMA Pro is in half-tranform mode.

        //weighting
        own Int*    m_rgiMaskQ;
        own Int*    m_rgiMaskQResampled;
        own Int*    m_rgcValidBarkBand;
        own Int*    m_rgiBarkIndexOrig;
        own U8***   m_rgrgrgnBarkResampleMatrix;
        own Int*    m_rgcSubWooferCutOffIndex;

        //output
        I32        *m_rgiPCMInHistory;  //num completed PCM samples in hist buf
        I32        *m_rgiDiscardSilence;
        CBT        *m_rgiInterlacedInput;
        CBT        *m_rgiInterlacedInputT;
        Bool        m_fLeadingSilence;
        U32         m_u32LeadingSize;
        U32         m_u32TrailingSize;
        U32         m_u32ValidSamples; // frame size - trailing size. Only used in LLM
        I32         m_iPCMReconStart;   // the start point in PCM recon buffer for output
        I32         m_iPCMReconEnd;     // the end point in PCM recon buffer

        Float   m_fltFirstNoiseFreq;
        Int     m_iFirstNoiseBand;
        Int     m_iFirstNoiseIndex;
        Int     m_iNoisePeakIgnoreBand; // Num of coefficients to ignore when detecting peaks during noise substitution

        //only used for NoiseSub mode
        own Int*        m_rgiFirstNoiseBand;
        own U8*         m_rgbBandNotCoded;
        own FastFloat*  m_rgffltSqrtBWRatio;
        own Int*        m_rgiNoisePower;


        Float*  m_rgfltWeightFactor;
        UInt*   m_rguiWeightFactor;   //share with m_rgiMaskQ when bark

        U32 m_iFrameNumber; // frame counter to the common code

        // 1/2^n and 2^n transforms
        //Bool    m_fHalfTransform;
        //Bool    m_fPad2XTransform;
        //Int     m_iXformSamplingRate;
        Int     m_iAdjustSizeShiftFactor;
        //Bool    m_fWMAProHalfTransform;  // To reduce WMA Pro decoder memory requirements for half-transform

        // the following values are adjusted to reflect HalfTransform or PAd2XTransform
        // They are used for the DCT and for Reconstruction.
        Int     m_cFrameSampleAdjusted;
        Int     m_cFrameSampleHalfAdjusted;
        Int     m_cHighCutOffAdjusted;
        // m_iCoefRecurQ1..4 and Trig values only exist in their adjusted states

        // Function Pointers
        WMARESULT(*aupfnInverseQuantize)(struct CAudioObject* pau,
                                         PerChannelInfo* ppcinfo, Int* rgiWeightFactor);
        WMARESULT(*prvpfnInverseTransformMono)(struct CAudioObject* pau,
                                               PerChannelInfo* ppcinfo, PCMSAMPLE* piOutput, PCMSAMPLE* piPrevOutput,
                                               I16* piPrevOutputSign, Bool fForceTransformAll);
        WMARESULT(*aupfnGetNextRun)(Void* paudec, PerChannelInfo* ppcinfo);

//    WMARESULT (*aupfnReconSample) (struct CAudioObject* pau, PCMSAMPLE* piOutput, U16 cSamples);
//    WMARESULT (*m_pfnPostProcess) (Void *pDecHandle, U16* pcSamplesReturned, U8 *pbDst, U32 cbDstLength);

        WMARESULT(*aupfnDctIV)(CoefType* rgiCoef, long fltAfterScaleFactor,  //Float fltAfterScaleFactor,
                               const Int cSubbandAdjusted, short nFacExponent);

        // --- Lossless LMS function pointers. We will have many sets of functions MMX/X86 16bits/24Bits --- //
        // Multi-channel LMS function pointers.
        //Void (* pprvMCLMSPred)(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT *rgiInput, CBT *rgiPred, Bool bDecoder);
        //Void (* pprvMCLMSUpdate)(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT * rgiInput, CBT *rgiPred);
        // Cascaded LMS function pointers.
        //I32 (* pprvCDLMSPred)(LMSPredictor * pLMSPredictor);
        //Void (* pprvCDLMSUpdate)(CAudioObject * pau, LMSPredictor * pLMSPredictor, I32 iInput, I32 iPred);

        PFNFFT aupfnFFT;

        PFNSETSAMPLE m_pfnSetSample;
        PFNGETSAMPLE m_pfnGetSample;


        tRandState m_tRandState;



        // V3 stuff: These are really in encoder buckets or decoder.
        Int     m_cChannelGroup;
        CChannelGroupInfo *m_rgChannelGrpInfo;
        I16     m_cChInTile;                    //how many src channels are included
        I16     m_iACFilterScaling;

        I16*    m_rgiChInTile;                  //which src chs are included

        Bool m_fUseVecCoder;
        Bool m_fExpVecsInNonRLMode;

        Bool m_fByteAlignFrame;
        Bool m_fWriteFrameSize;
        Bool m_fExtraBitsInPktHeader;
        Int m_cComprFrameSizeBits;
        Int m_cBitsBytePacketLength;
        Int m_cBitsBitsPacketLength;
        Int m_cBytePacketLength;

        // generate drc scale factors
        Bool m_fGenerateDrcParams;
        // V3 LLM.
        //Bool    m_bDoChMixVoting;
        // V3 LLM. AC filter
        //I32             m_iACFilter[LLM_VERB_ACFILTER_ORDER_MAX]; // AC filter.
        //I32             m_iACFilterOrder;
        //I32             m_iACFilterCurr; // for decoder.
        // V3 LLMB Chao multichannel LMS
        //Bool    m_bDoMCLMS;
        //Bool    m_bSendMCLMS;
        //MCLMSPredictor  m_MCLMSPredictor;

        //Bool    m_bSendCDLMS;

        I32     m_iSampleMaxValue;
        I32     m_iSampleMinValue;

        I16     m_iResidueMemScaling;  // LLMB_RESIDUE_MEMORY_TTLWEIGHT
        //U16  m_cLastCodedIndexV3;     // To reduce decoder complexity in V3


        Int  m_cLeftShiftBitsFixedPre;
        Int  m_cLeftShiftBitsFixedPost;

        // V3 LLMA Chao multichannel mixing sequence
        //U16  m_iMltChlsMixingSeq[(MAX_CHANNEL_MIXING_ROUND + 1)][2];
        //U16  m_iMltChlsMixingRnd;

//#ifdef ENABLE_ARITHCODER
//        FreqT m_rgiResQPMF[RESQ_ESCAPE_VALUE + 1];
//#endif //ENABLE_ARITHCODER
        Bool m_bNextSbFrmCanBeSeekable;
        Bool m_bNextSbFrmMustBeSeekable;
        // Temporary space required for auGetPCM
        own U16  *m_rgcSamplesREQ;

        Bool m_bMonoV3; // for skipping some header bits


//#ifdef BUILD_INTEGER
        // used for calling auPreScaleCoeffsV3()
        I32 m_iLog2MaxIntvalCh;
        CoefType m_maxOutputAllowed;
//#endif

        own ChXFormType ***m_rgrgrgfltMultiXIDCT;
        own CoefType      *m_rgfltCoefDst;           // Temporary for inverse channel transform in V3.
        own CoefType     **m_rgpfltCoefGrpSrc;       // Temporary for inverse channel transform in V3.
        own ChXFormType  *m_rgfltGivensTmp0;         // Temporary vector for inverse multi
        own ChXFormType  *m_rgfltGivensTmp1;         // Temporary vector for inverse multi

        // Dynamic range compression stuff
        DRC *m_pDrc; // Class to do dynamic range compression, encode/decode

    } CAudioObject;


//public and protected
#ifdef __cplusplus
    extern "C"    // only need to export C interface if
    {
        // used by C++ source code
#endif

//#define HALF(fHalfTransform, x) \
//((fHalfTransform) ? ((x)>>pau->m_iAdjustSizeShiftFactor) : (x))
//#define DOUBLE(fPad2X, x) \
//((fPad2X) ? ((x)<<pau->m_iAdjustSizeShiftFactor) : (x))
//#define AU_HALF_OR_DOUBLE(fHalf, fPad2X, x) \
//((fHalf) ? ((x)>>pau->m_iAdjustSizeShiftFactor) : ((fPad2X) ? ((x)<<pau->m_iAdjustSizeShiftFactor) : (x)))

//#include "configcommon.h"
        U32 MaxSamplesPerPacket(U32 ulVersion, U32 ulSamplingRate, U16 ulChannels, U32 ulBitrate);

        I32 msaudioGetSamplePerFrame(I32   cSamplePerSec, U32   dwBitPerSec, I32   cChannel, I32   iVersion, U16   wEncOpt);
//        U32 DefaultChannelMask(U32 nChannels);

        CAudioObject* auNew();
        Void    auDelete(CAudioObject* pau);         //destructor
        WMARESULT auInit(CAudioObject* pau, Int iVersionNumber, Int cSubband,
                         Int iSamplingRate, U16 cChannel,
                         U32 nBlockAlign,
                         U16   nValidBitsPerSample,
                         U32   nChannelMask,
                         Int cBytePerSec, Int cbPacketLength,
                         U16 iEncodeOpt, Int iOutputSamplingRate, WMAPlayerInfo *pWmaPlayerInfo);

        WMARESULT auInvChannelXForm(CAudioObject *pau, Bool fInvertSuperGroupXform);
//WMARESULT prvMultiXIDCTAllocate  (CAudioObject *pau);
//WMARESULT prvMultiXIDCTInit (CAudioObject *pau);
//Void      prvMultiXIDCTDelete  (CAudioObject *pau);
        Void    auPreGetPCM(CAudioObject* pau, U16* pcSampleDecoded, I16* pcSamplesSeekAdj, Bool fSPDIF);
        WMARESULT auSubframeRecon(CAudioObject *pau);
        WMARESULT auGetPCM(CAudioObject* pau, U16* pcSample, short* ppbDst, U32 cbDstLength);

//#define CAP_NODISCARDSILENCE    0
//#define CAP_DISCARDSILENCE      1
//#define CAP_SUBFRAMELVL         0
//#define CAP_ENTIREFRAME         1
        void prvCountAlignedPCM(CAudioObject *pau,
                                U16 *piAlignedPCM,
                                Bool fDiscardSilence,
                                Bool fEntireFrame,
                                Bool fSPDIF);
        void prvGetFramePCM(CAudioObject* pau, Int *piFramePCM);
        void prvShiftPCMBuffer(CAudioObject* pau);

        I32 prvNextSeekPoint(const CAudioObject *pau);
        I32 prvContinuousDecodeAdjustment(const CAudioObject *pau);
        void prvCalcQ1Q2(CAudioObject * pau, Bool bCurrWindow, const I16 iSizePrev, const I16 iSizeCurr,
                         I16 *piCoefRecurQ1, I16 *piCoefRecurQ2);
        void prvCalcQ3Q4(CAudioObject * pau, Bool bCurrWindow, const I16 iSizeCurr, const I16 iSizeNext,
                         const Int cSubFrameSampleHalfAdjusted,
                         I16 *piCoefRecurQ3, I16 *piCoefRecurQ4);

        void prvInitDiscardSilence(CAudioObject *pau, Bool fSPDIF);

//#ifdef TRAILING_SILENCE
//        void prvInitTrailingSilence(CAudioObject *pau);
//#endif  // TRAILING_SILENCE

//WMARESULT auReconSample(CAudioObject* pau, PCMSAMPLE* piOutput, U16 cSamples);

        WMARESULT auAdaptToSubFrameConfig(CAudioObject* pau);
//wchen: need to move the msaudiodec.h
        WMARESULT auAdaptToSubFrameConfigDEC(CAudioObject* pau);
        WMARESULT prvAdaptTrigToSubframeConfig(CAudioObject *pau);
        WMARESULT prvAdaptTrigToSubframeConfig_INT(CAudioObject *pau);


        Int     auPowerToRMS(CAudioObject* pau, Int iCurrMaskQ, Int iPrevMaskQ, Int iPrevResult);
        Void    auUpdateMaxEsc(CAudioObject* pau, Int iQuantStepSize);

// DCT type 4
        WMARESULT auDctIV(CoefType* rgiCoef, long fltAfterScaleFactor,//Float fltAfterScaleFactor,
                          const Int cSubbandAdjusted, short nFacExponent);

        /*#ifdef USE_INTELOPT_LIBRARY
        WMARESULT auDctIV_SSE(CoefType* rgiCoef, Float fltAfterScaleFactor, U32 *piMagnitude,
                          const Int cSubbandAdjusted, PFNFFT pfnFFT, const Int iFrameNumber,
                          const Int iCurrSubFrame, const Int cFrameSampleAdjusted,
                          const Int cSubFrameSampleAdjusted);
        #endif  // USE_INTELOPT_LIBRARY
        */

        WMARESULT auResampleMaskV3(CAudioObject* pau,
                                   Int* rgiMaskQIn, Int cSubbandIn, Int cValidBarksIn,
                                   Int* rgiMaskQOut, Int cSubbandOut, Int cValidBarksOut);

// LPC functions
//#ifdef ENABLE_ALL_ENCOPT
        Void    auResampleWeightFactorLPC(CAudioObject* pau, PerChannelInfo* ppcinfo);
        Void    auLsp2lpc(CAudioObject* pau, U8 *lsfQ, LpType *lpc, Int order);
//#endif //ENABLE_ALL_ENCOPT

        WMARESULT prvAllocate(CAudioObject* pau);

#ifdef __cplusplus
    }
#endif

//private:

// *** TEMPORARY: prvAllocate moved from here to extern "C" block above ***
//Void       auInitBarkResampleMatrix      (CAudioObject* pau);
//WMARESULT  prvAllocateBarkResampleMatrix (CAudioObject* pau);
//Void       prvDeleteBarkResampleMatrix   (CAudioObject* pau);

    Void    prvSetBarkIndex(CAudioObject* pau);
//Void    prvSetSubWooferCutOffs (CAudioObject* pau);
    WMARESULT prvInverseQuantizeHighRate(CAudioObject* pau, PerChannelInfo* ppcinfo, Int* rgiWeightFactor);

//WMARESULT auPreScaleCoeffsV3(CAudioObject *pau,
    //                             CoefType iLog2MaxOutputAllowed,
    //                             Bool fUseQuantStep,
    //                             Bool fUseGlobalScale,
    //                             CoefType *iMaxGain);

    WMARESULT auPreScaleQuantV3(CAudioObject *pau,
                                Bool fUseGlobalQuantScale);
//WMARESULT auPostScaleCoeffsV3(CAudioObject *pau);

//WMARESULT auGetTransformFromGivensFactors(I8* rgbRotationAngle,
    //                                          I8* rgbRotationSign,
    //                                          Int cSize,
    //                                          Int cMaxChannel,
    //                                          ChXFormType* rgfltMultiXInverse,
    //                                          ChXFormType* rgfltTmpCurr,
    //                                          ChXFormType* rgfltTmpFinal);


    WMARESULT auInvWeightSpectrumV3(CAudioObject* pau,
                                    PerChannelInfo* ppcinfo,
                                    U8 fMaskUpdate);

//Void prvGetChannelNumAtSpeakerPosition (U32 nChannelMask,
//                                        U32 nSpeakerPosition,
//                                        I16 *pnChannelNo);

    WMARESULT prvInverseTransformMono(CAudioObject* pau, PerChannelInfo* ppcinfo,
                                      PCMSAMPLE* piOutput, PCMSAMPLE* piPrevOutput,
                                      I16* piPrevOutputSign, Bool fForceTransformAll);

// V3 Pure LLM. Chao. Add.
    WMARESULT auReconMonoPureLLM(CAudioObject* pau,
                                 PerChannelInfo* ppcinfo,
                                 PCMSAMPLE* piOutput,
                                 I16* pcSampleGet,
                                 Bool fForceTransformAll,
                                 I16 iCh);
    WMARESULT auReconstructPureLLM(CAudioObject* pau, U8** ppbDst,
                                   I16* pcSampleGet, Bool fForceTransformAll);

    Void      prvForwSDWT(I32 * rgiBuf, I32 * rgiBufT, U32 iLen);
    Void      prvInveSDWT(I32 * rgiBuf, I32 * rgiBufT, U32 iLen);
//V3 LLMA Chao 03-25-02
    Void prvChsForwSTransform(I32 * rgiInput1, I32 * rgiInput2, I32 * rgiS, I32 * rgiD, U32 cLen);
    Void prvChsInveSTransform(I32 * rgiInput1, I32 * rgiInput2, I32 * rgiS, I32 * rgiD, U32 cLen);

//V3 LLMB chao 04-24-02
    void prvResetColombKinLLMB(CAudioObject * pau, PerChannelInfo * ppcinfo);

    Void    prvFFT_Int(Int data[], Int np);
    Void    prvFFT4DCT(CoefType data[], const Int nLog2np);

//#ifdef ENABLE_ALL_ENCOPT
//for noise sub and low rates only
    WMARESULT prvInitNoiseSubSecondaryFixed(CAudioObject* pau);
    WMARESULT prvInitNoiseSubSecondaryAllocated(CAudioObject* pau);

    WMARESULT prvInverseQuantizeMidRate(CAudioObject* pau, PerChannelInfo* ppcinfo, Int* rgiWeightFactor);
    WMARESULT prvInverseQuantizeLowRate(CAudioObject* pau, PerChannelInfo* ppcinfo, Int* rgiWeightFactor);

//private lpc functions
    WMARESULT prvLpcToSpectrum(CAudioObject* pau, const LpType* rgfltLpcCoef, PerChannelInfo* ppcinfo);
    Void prvInitInverseQuadRootTable(CAudioObject* pau);
//#endif //ENABLE_ALL_ENCOPT

#ifdef __cplusplus
}
#endif

#if 1//ndef PREVOUTPUT_16BITS
// **************************************************************************
// Signbit Macros
// I've put them here because they're used in both msaudio.c and float.c
// **************************************************************************
//#define I17_MIN ((signed) 0xFFFF0000)
//#define I17_MAX ((signed) 0x0000FFFF)
//
//#define OVERLAPADD_MIN  PCMSAMPLE_MIN(pau->m_nValidBitsPerSample + 1)
//#define OVERLAPADD_MAX  PCMSAMPLE_MAX(pau->m_nValidBitsPerSample + 1)
//
//#define CALC_SIGN_OFFSETS16(piSrcBase, piSrc, piSrcSignBase, piSrcSign,         \
//                           uiSignbitData, uiSignbitCount, cChan)                \
//    {                                                                           \
//    const int c_iOffset = (piSrc - piSrcBase) / cChan;                          \
//    const int c_iSignOffset = (c_iOffset+((BITS_PER_BYTE * sizeof(I16))-1))     \
//                            / (BITS_PER_BYTE * sizeof(I16));                    \
//    assert(2 == pau->m_nBytePerSample);                                         \
//    piSrcSign = piSrcSignBase + c_iSignOffset;                                  \
//    uiSignbitCount = 16 - (c_iOffset % (BITS_PER_BYTE * sizeof(I16)));          \
//    if (uiSignbitCount == 16)                                                   \
//        uiSignbitData = *piSrcSign++;                                           \
//    else                                                                        \
//        uiSignbitData = *(piSrcSign-1);                                         \
//    }
//
//#define CALC_SIGN_OFFSETS(piSrcBase, piSrc, piSrcSignBase, piSrcSign,           \
//                           uiSignbitData, uiSignbitCount, cChan)                \
//    {                                                                           \
//    const int c_iOffset = prvCountSamples(piSrc, piSrcBase, pau, cChan);        \
//    const int c_iSignOffset = (c_iOffset+((BITS_PER_BYTE * sizeof(I16))-1))     \
//                            / (BITS_PER_BYTE * sizeof(I16));                    \
//    const unsigned int c_iBitsPerSignbitReg = (BITS_PER_BYTE * sizeof(I16));    \
//    piSrcSign = piSrcSignBase + c_iSignOffset;                                  \
//    uiSignbitCount = c_iBitsPerSignbitReg - (c_iOffset % c_iBitsPerSignbitReg); \
//    if (uiSignbitCount == c_iBitsPerSignbitReg)                                 \
//        uiSignbitData = *piSrcSign++;                                           \
//    else                                                                        \
//        uiSignbitData = *(piSrcSign-1);                                         \
//    }
//
//// Calculate sign offset, but don't load signbit register. For use by auSaveHistoryMono.
//#define CALC_SIGN_OFFSETS_NOLOAD(piSrcBase, piSrc, piSrcSignBase, piSrcSign, cChan) \
//    {                                                                           \
//    const int c_iOffset = prvCountSamples(piSrc, piSrcBase, pau, cChan);        \
//    const int c_iSignOffset = (c_iOffset+((BITS_PER_BYTE * sizeof(I16))-1))     \
//                            / (BITS_PER_BYTE * sizeof(I16));                    \
//    piSrcSign = piSrcSignBase + c_iSignOffset;                                  \
//    }
//
//#define RECORD_SIGNBIT(iResult, piDstSign, uiSignbitData, uiSignbitCount, incr) \
//    uiSignbitData <<= 1;                                                        \
//    uiSignbitData |= ((iResult >> 31) & 0x00000001);                            \
//    uiSignbitCount += 1;                                                        \
//    if (uiSignbitCount >= (BITS_PER_BYTE * sizeof(I16)))                        \
//    {   /* Signbit register is full! Write it out */                            \
//        *piDstSign = (I16) uiSignbitData;                                       \
//        piDstSign += incr;                                                      \
//        DEBUG_ONLY( uiSignbitData = 0 );                                        \
//        uiSignbitCount = 0;                                                     \
//    }

#define SIGNBIT_MASK(iValidBits)    (~((1 << iValidBits) - 1))

//#define COMBINE_SIGNBIT(iResult, iSrc, piSrcSign, uiSignbitData, uiSignbitCount, iValidBits) \
//    if (0 == uiSignbitCount)                                                     \
//    {   /* Signbit register is empty! Read new one in */                         \
//        uiSignbitData = *piSrcSign;                                              \
//        piSrcSign += 1;                                                          \
//        uiSignbitCount = 16;                                                     \
//    }                                                                            \
//    if (uiSignbitData & 0x00008000)                                              \
//        iResult = (-1) & SIGNBIT_MASK(iValidBits);                               \
//    else                                                                         \
//        iResult = 0;                                                             \
//    iResult |= (iSrc & ~SIGNBIT_MASK(iValidBits));                               \
//    uiSignbitData <<= 1;                                                         \
//    uiSignbitCount -= 1;
//
//#define SAVE_SIGNBIT_STATE(piSrcSign,uiSignbitData)                              \
//    if (0 != uiSignbitCount)                                                     \
//        *(piSrcSign-1) = (I16)uiSignbitData;    /* must save partial shift */
//
//#define ASSERT_SIGNBIT_POINTER(piSrcOrDstSign)                                   \
//    assert( piSrcOrDstSign != NULL );

#else   // PREVOUTPUT_16BITS

// Use I16 min/max in place of I17
//#define I17_MIN SHRT_MIN
//#define I17_MAX SHRT_MAX
//
//#define CALC_SIGN_OFFSETS16(piSrcBase, piSrc, piSrcSignBase, piSrcSign,          \
//                           uiSignData, uiSignCount, cChan)
//
//#define CALC_SIGN_OFFSETS(piSrcBase, piSrc, piSrcSignBase, piSrcSign,            \
//                           uiSignData, uiSignCount, cChan)
//#define RECORD_SIGNBIT(iResult, piDstSign, uiSignbitData, uiSignbitCount, incr)
//#define COMBINE_SIGNBIT(iResult, iSrc, piSrcSign, uiSignbitData, uiSignbitCount, \
//                        iValidBits) \
//    iResult = iSrc
//#define SAVE_SIGNBIT_STATE(piSrcSign,uiSignData)
//#define ASSERT_SIGNBIT_POINTER(piSrcOrDstSign)

#endif  // PREVOUTPUT_16BITS

#define INTERPOL_FRACTBITS  30

void auLowPass(CAudioObject *pau, CoefType *rgiCoef, Int iNumCoefs,
               Int iPassThruCoefs);

//#ifndef BUILD_INTEGER_LPC
//// do not scale WeightFactor when using float
//#undef WEIGHTFACTOR_FRACT_BITS
//#define WEIGHTFACTOR_FRACT_BITS 0
//#endif

//Void auCalcQuantStep(Int iQuantStepSize, U32 *piQuantStep, U32 *piFractBits);
QuantStepType qstCalcQuantStep(Int iQuantStepSize,int* isvalid);
//QuantStepType qstCalcQuantStepModifier(Int iQuantStepSize, Int cLeftShiftBits);
QuantFloat prvWeightedQuantization(CAudioObject *pau, PerChannelInfo *ppcinfo, Int iBark);

void SetActualPower(const I32 *piCoefQ, const I32 iCount,
                    PerChannelInfo *ppcinfo, const Status codecStatus);
void SetActualPowerHighRate(const CBT *piCoefRecon, const int iCount,
                            PerChannelInfo *ppcinfo, const Status codecStatus);




//#if 0//def TEST_CORRUPT_BITSTREAM
//#define CORRUPT_PROBABILITY 100
//#define REPORT_BITSTREAM_CORRUPTION() //{WMAPrintf("We encountered a bit stream corruption!\n"); assert(WMAB_FALSE);}
//#else
//#define BITSTREAM_CORRUPTION_ASSERTS
//#ifdef BITSTREAM_CORRUPTION_ASSERTS
//#define REPORT_BITSTREAM_CORRUPTION() assert (WMAB_FALSE)
//#else
#define REPORT_BITSTREAM_CORRUPTION()
//#endif
//#endif //TEST_CORRUPT_BITSTREAM

#define REPORT_BITSTREAM_CORRUPTION_AND_EXIT(wr) { \
  REPORT_BITSTREAM_CORRUPTION(); \
  wr = TraceResult(WMA_E_BROKEN_FRAME); \
  goto exit; \
}

#include "playropt.h"

#define MSAUDIO_PEAKANDRMS_CMP


//***************************************************************************
// Encoder/Decoder Separation
//***************************************************************************

#define PRECESSION_BITS_FOR_INVQUADROOT 12

//#if defined(_DEBUG) && defined(LPC_COMPARE)
//void prvSetupLpcSpecTrig(CAudioObject* pau);
//#define SETUP_LPC_SPEC_TRIG(pau) prvSetupLpcSpecTrig(pau)
//#else
#define SETUP_LPC_SPEC_TRIG(pau)
//#endif


//***************************************************************************
// 24-bit Encoding
//***************************************************************************

void prvInitGetSetFn(PFNGETSAMPLE *ppfnGetSample,
                     PFNSETSAMPLE *ppfnSetSample,
                     const BITDEPTH iBitDepthSelector);

//static INLINE PCMSAMPLE *prvAdvancePCMPointer(const PCMSAMPLE *pCurrentPos,
//                                const Int nBytePerSample,
//                                const Int iOffset)
//{
//    return (PCMSAMPLE *) (((U8*) pCurrentPos) + (nBytePerSample * iOffset));



//static INLINE PCMSAMPLE *prvAdvancePCMPointer16(const PCMSAMPLE *pCurrentPos,
//                                const Int nBytePerSample,
//                                const Int iOffset)
//{
//    return (PCMSAMPLE *) ((I16*) pCurrentPos + iOffset);



//static INLINE PCMSAMPLE *prvAdvancePCMPointer24(const PCMSAMPLE *pCurrentPos,
//                                const Int nBytePerSample,
//                                const Int iOffset)
//{
//    return (PCMSAMPLE *) ((U8*) pCurrentPos + 3*iOffset);


// =========================================

//#define PAUPRVADVANCEPCMPTR(pCurrentPos, iOffset) \
//  prvAdvancePCMPointer((pCurrentPos), pau->m_nBytePerSample, (iOffset))
//
//#define PAUPFNGETSAMPLE(pCurrentPos, iOffset) \
//  pau->m_pfnGetSample((pCurrentPos), pau->m_nBytePerSample, \
//                      pau->m_nValidBitsPerSample, (iOffset))

//#define PAUPFNGETSAMPLE0(pCurrentPos) \
//  PAUPFNGETSAMPLE((pCurrentPos), 0)

//#define PAUPFNSETSAMPLE(iValue, pCurrentPos, iOffset) \
//  pau->m_pfnSetSample((iValue), (pCurrentPos), pau, (iOffset))
//
//#define PAUPFNSETSAMPLE0(iValue, pCurrentPos) \
//  PAUPFNSETSAMPLE((iValue), (pCurrentPos), 0)
//
//// =========================================
//
//#define NPAUPRVADVANCEPCMPTR(pCurrentPos, iOffset) \
//  prvAdvancePCMPointer((pCurrentPos), nBytePerSample, (iOffset))
//
//#define NPAUPFNGETSAMPLE(pCurrentPos, iOffset) \
//  pfnGetSample((pCurrentPos), nBytePerSample, nValidBitsPerSample, (iOffset))
//
//#define NPAUPFNGETSAMPLE0(pCurrentPos) \
//  NPAUPFNGETSAMPLE((pCurrentPos), 0)
//
//#define NPAUPFNSETSAMPLE(iValue, pCurrentPos, iOffset) \
//  pfnSetSample((iValue), (pCurrentPos), NULL, (iOffset))
//
//#define NPAUPFNSETSAMPLE0(iValue, pCurrentPos) \
//  NPAUPFNSETSAMPLE((iValue), (pCurrentPos), 0)
//
//#define NPAUNPFNSETSAMPLE(iValue, pCurrentPos, iOffset) \
//  pfnSetSample((iValue), (pCurrentPos), pau, (iOffset))
//
//#define NPAUNPFNSETSAMPLE0(iValue, pCurrentPos) \
//  NPAUNPFNSETSAMPLE((iValue), (pCurrentPos), 0)

// ========================================

//static INLINE Bool prvIsNPauBD(BITDEPTH bd)
//{
//    return (bd == BITDEPTH_24) || (bd == BITDEPTH_2024) ||
//           (bd == BITDEPTH_16);
//}
//
//static INLINE BITDEPTH prvBitDepthSelector(int nBytePerSample,
//        int nValidBitsPerSample)
//{
//    return (BITDEPTH)((nBytePerSample - 1) | ((nValidBitsPerSample - 1) << 2));
//}

//Int prvCountSamples(const PCMSAMPLE *pCurrentPos,
//                    const PCMSAMPLE *pBasePos,
//                    const CAudioObject *pau,
//                    const Int iChannels);

//static INLINE PCMSAMPLE prvGetSample16INLINE(const PCMSAMPLE *pCurrentPos,
//        const Int nBytePerSample,
//        const Int nValidBitsPerSample,
//        const Int iOffset)
//{
//    U8 *pbSrc = (U8*) pCurrentPos;
//
//    pbSrc += 2 * iOffset; // Skip iOffset worth of samples
//    return *((I16*)pbSrc);
//}
//
//static INLINE void prvSetSample16INLINE(const PCMSAMPLE iValue,
//                                        PCMSAMPLE *pCurrentPos,
//                                        const CAudioObject *pau,
//                                        const Int iOffset)
//{
//    U8 *pbDst = (U8*) pCurrentPos;
//    U8 *pbSrc = (U8*) & iValue;
//
//    pbDst += 2 * iOffset; // Skip iOffset worth of samples
//#ifdef BIG_ENDIAN
//    *((I16*)pbDst) = *((I16*)(pbSrc + 2));
//#else
//    *((I16*)pbDst) = *((I16*)pbSrc);
//#endif
//}


//static INLINE PCMSAMPLE prvGetSample24INLINE(const PCMSAMPLE *pCurrentPos,
//        const Int nBytePerSample,
//        const Int nValidBitsPerSample,
//        const Int iOffset)
//{
//    U8 *pbSrc = (U8*) pCurrentPos;
//    I32 iResult;
//
//    pbSrc += 3 * iOffset; // Skip iOffset worth of samples
//#ifdef BIG_ENDIAN
//    iResult = (*(I32*)pbSrc) >> 8;
//#else
//    iResult = (*((U16*)pbSrc) | (*((I8*)(pbSrc + 2)) << 16));
//#endif
//    return iResult;
//}

#if 0
//static INLINE PCMSAMPLE prvGetSample24INLINE32(const PCMSAMPLE *pCurrentPos,
//        const Int nBytePerSample,
//        const Int nValidBitsPerSample,
//        const Int iOffset)
//{
//    U8 *pbSrc = (U8*) pCurrentPos;
//    I32 iResult;
//
//    pbSrc += 3 * iOffset; // Skip iOffset worth of samples
//    iResult = (*((U16*)pbSrc) | (*((I8*)(pbSrc + 2)) << 16));
//    iResult <<= 8;
//    //iResult >>= 8;
//
//    return iResult;
//}
//
//static PCMSAMPLE INLINE prvGetSample2024INLINE(const PCMSAMPLE *pCurrentPos,
//        const Int nBytePerSample,
//        const Int nValidBitsPerSample,
//        const Int iOffset)
//{
//    U8 *pbSrc = (U8*) pCurrentPos;
//    I32 iResult;
//
//    pbSrc += 3 * iOffset; // Skip iOffset worth of samples
//    iResult = (*((U16*)pbSrc) | (*((I8*)(pbSrc + 2)) << 16));
//
//    assert(0 == (iResult & 0x0F)); // Verify that 4 least signficant bits always zero
//
//    iResult >>= 4; // Dump the 4 least-significant bits (should always be zero)
//    return iResult;
//}
//
//
//static PCMSAMPLE INLINE prvGetSample2024INLINE32(const PCMSAMPLE *pCurrentPos,
//        const Int nBytePerSample,
//        const Int nValidBitsPerSample,
//        const Int iOffset)
//{
//    U8 *pbSrc = (U8*) pCurrentPos;
//    I32 iResult;
//
//    pbSrc += 3 * iOffset; // Skip iOffset worth of samples
//    iResult = (*((U16*)pbSrc) | (*((I8*)(pbSrc + 2)) << 16));
//
//    assert(0 == (iResult & 0x0F)); // Verify that 4 least signficant bits always zero
//
//    // Sign extension
//    iResult <<= 8;
//    //iResult >>= 12; // Dump the 4 least-significant bits (should always be zero)
//
//    return iResult;
//}
//
//
//static INLINE void prvSetSample24INLINE(const PCMSAMPLE iValue,
//                                        PCMSAMPLE *pCurrentPos,
//                                        const CAudioObject *pau,
//                                        const Int iOffset)
//{
//    U8 *pbDst = (U8*) pCurrentPos;
//    U8 *pbSrc = (U8*) & iValue;
//
//    pbDst += 3 * iOffset; // Skip iOffset worth of samples
//#ifdef BIG_ENDIAN
//    *((I8*)pbDst) = *((I8*)pbSrc + 1);
//    *((I8*)(pbDst + 1)) = *((I8*)(pbSrc + 2));
//    *((I8*)(pbDst + 2)) = *((I8*)(pbSrc + 3));
//#else
//    *((I16*)pbDst) = *((I16*)pbSrc);
//    *((I8*)(pbDst + 2)) = *((I8*)(pbSrc + 2));
//#endif
//}
//
//
//static void INLINE prvSetSample2024INLINE(const PCMSAMPLE iValue,
//        PCMSAMPLE *pCurrentPos,
//        const CAudioObject *pau,
//        const Int iOffset)
//{
//    I32 iNewValue;
//    U8 *pbDst = (U8*) pCurrentPos;
//    U8 *pbSrc = (U8*) & iNewValue;
//
//    // We used to check if incoming value was < 20-bit: don't, because auSaveHistory
//    // can feed us values > 20-bit. Expectation is that we truncate to 20 bits.
//    iNewValue = (iValue << 4);
//
//    pbDst += 3 * iOffset; // Skip iOffset worth of samples
//    *((I16*)pbDst) = *((I16*)pbSrc);
//    *((I8*)(pbDst + 2)) = *((I8*)(pbSrc + 2));
//}
#endif
//*****************************************************************************************
// Forward Function Declarations
//*****************************************************************************************

//void prvSetSample(const PCMSAMPLE iValue, PCMSAMPLE *pCurrentPos,
//                  const CAudioObject *pau, const Int iOffset);
void prvSetSample16(const PCMSAMPLE iValue, PCMSAMPLE *pCurrentPos,
                    const CAudioObject *pau, const Int iOffset);
//void prvSetSample24(const PCMSAMPLE iValue, PCMSAMPLE *pCurrentPos,
//                    const CAudioObject *pau, const Int iOffset);
//void prvSetSample2024(const PCMSAMPLE iValue,
//                      PCMSAMPLE *pCurrentPos,
//                      const CAudioObject *pau,
//                      const Int iOffset);
//
//PCMSAMPLE prvGetSample(const PCMSAMPLE *pCurrentPos,
//                       const Int nBytePerSample,
//                       const Int nValidBitsPerSample,
//                       const Int iOffset);
PCMSAMPLE prvGetSample16(const PCMSAMPLE *pCurrentPos,
                         const Int nBytePerSample,
                         const Int nValidBitsPerSample,
                         const Int iOffset);
//PCMSAMPLE prvGetSample24(const PCMSAMPLE *pCurrentPos,
//                         const Int nBytePerSample,
//                         const Int nValidBitsPerSample,
//                         const Int iOffset);
//PCMSAMPLE prvGetSample2024(const PCMSAMPLE *pCurrentPos,
//                           const Int nBytePerSample,
//                           const Int nValidBitsPerSample,
//                           const Int iOffset);

//************************************************************************

//#include <math.h>
#if 0//ndef FFLT_SQRT_RATIO
INLINE FastFloat ffltSqrtRatio(FastFloat fflt1, FastFloat fflt2)
{
#if 1//defined(BUILD_INTEGER)
    U64 u64Ratio;
    FastFloat fflt;
    Int iMSF8;
    U32 uiMSF;
    U32 uiFrac1;
    Int iExp = 0;
    if (fflt2.iFraction == 0)
    {
        assert(fflt2.iFraction != 0);   // divide by 0
        fflt.iFraction = 0x7FFFFFFF;
        fflt.iFracBits = 0;
        return fflt;
    }
    //// fflt1.iFraction*0.5F/(1<<(fflt1.iFracBits-1))
    //// fflt2.iFraction*0.5F/(1<<(fflt2.iFracBits-1))
    u64Ratio = (((U64)fflt1.iFraction) << 32) / fflt2.iFraction;
    uiMSF = (U32)(u64Ratio >> 32);
    if (uiMSF == 0)
    {
        iExp = 32;
        uiMSF = (U32)u64Ratio;
    }
    assert(uiMSF != 0) ;
    // normalize the most significant fractional part
    while ((uiMSF & 0xF0000000) == 0)
    {
        iExp += 4;
        uiMSF <<= 4;
    }
    while ((uiMSF & 0x80000000) == 0)
    {
        iExp++;
        uiMSF <<= 1;
    }
    // discard the most significant one bit (it's presence is built into g_InvQuadRootFraction)
    iExp++;
    // get all 32 bits from source
#if defined(PLATFORM_OPTIMIZE_MINIMIZE_BRANCHING)
    uiMSF = (U32)((u64Ratio << iExp) >> 32);
#else
    uiMSF = (iExp > 32) ? (U32)(u64Ratio << (iExp - 32)) : (U32)(u64Ratio >> (32 - iExp));
#endif
    // split into top SQRT_FRACTION_TABLE_LOG2_SIZE==8 bits for fractional lookup and bottom bits for interpolation
    iMSF8 = uiMSF >> (32 - SQRT_FRACTION_TABLE_LOG2_SIZE);
    uiMSF <<= SQRT_FRACTION_TABLE_LOG2_SIZE;
    // lookup and interpolate
#ifdef WMA_TABLE_ROOM_VERIFY
    uiFrac1  = ((const UInt*)p_g_SqrtFraction)[iMSF8++];   // BP2
    uiFrac1 += MULT_HI_UDWORD(uiMSF, ((const UInt*)p_g_SqrtFraction)[iMSF8] - uiFrac1);
#else
    uiFrac1  = g_SqrtFraction[iMSF8++];   // BP2
    uiFrac1 += MULT_HI_UDWORD(uiMSF,  g_SqrtFraction[iMSF8] - uiFrac1);
#endif
    // adjust by sqrt(1/2) if expoenent is odd
    if ((iExp + fflt1.iFracBits - fflt2.iFracBits) & 1)
    {
        // multiply by 1/sqrt(2) and adjust fracbits by 1/2
        uiFrac1 = MULT_HI_UDWORD(uiFrac1, UBP0_FROM_FLOAT(0.70710678118654752440084436210485));
        fflt.iFracBits = ((fflt1.iFracBits - (fflt2.iFracBits + 1)) >> 1) + (iExp - 3);
    }
    else
    {
        fflt.iFracBits = ((fflt1.iFracBits - fflt2.iFracBits) >> 1) + (iExp - 3);
    }
    fflt.iFraction = uiFrac1 >> 1;  // make sure sign is positive
    //// fflt.iFraction*0.5F/(1<<(fflt.iFracBits-1))
    Norm4FastFloatU(&fflt);
//#if defined(_DEBUG) && 0
//    {
//        // old way used float
//        Float flt = (Float)sqrt(FloatFromFastFloat(fflt1) / FloatFromFastFloat(fflt2));
//        if (fabs(flt - FloatFromFastFloat(fflt)) > 0.01)
//        {
//            DEBUG_BREAK();
//        }
//    }
//#endif
    return fflt;
#else
    return (Float)sqrt(fflt1 / fflt2);
#endif
}
#   define FFLT_SQRT_RATIO(fflt1,fflt2) ffltSqrtRatio(fflt1,fflt2)
#endif
extern FastFloat ffltSqrtRatio(FastFloat fflt1, FastFloat fflt2);
#   define FFLT_SQRT_RATIO(fflt1,fflt2) ffltSqrtRatio(fflt1,fflt2)


//************************************************************************


/*****************************************************************************
    Lossless functions.
*****************************************************************************/

WMARESULT prvInitCommonParameters(CAudioObject *pau,
                                  Int           iVersionNumber,
                                  Int           cSubband,
                                  Int           cSamplePerSec,
                                  U16           cChannel,
                                  U32           nBytePerSample,
                                  U16           nValidBitsPerSample,
                                  U32           nChannelMask,
                                  Int           cBytePerSec,
                                  Int           cbPacketLength,
                                  U16           iEncodeOpt,
                                  Int           iOutputSamplingRate,
                                  WMAPlayerInfo *pWmaPlayerInfo);
WMARESULT prvInitCommonSecondaryFixed(CAudioObject *pau);
WMARESULT prvInitCommonAllocate(CAudioObject *pau);
WMARESULT prvInitCommonSecondaryAllocated(CAudioObject *pau);
WMARESULT prvInitCommonResetOperational(CAudioObject *pau);
WMARESULT prvInitCommonSetFunctionPtrs(CAudioObject *pau);
Void auDeletePcInfoCommon(CAudioObject* pau, PerChannelInfo* rgpcinfo);
WMARESULT prvAllocatePcInfoCommon(CAudioObject* pau, PerChannelInfo* rgpcinfo);
WMARESULT prvResetPcInfoCommon(CAudioObject* pau, PerChannelInfo* rgpcinfo);

//WMARESULT prvAllocChannelGrpInfo(CChannelGroupInfo **prgChannelGrpInfo, Int cChannel);
//Void prvDeleteChannelGrpInfo(CChannelGroupInfo **prgChannelGrpInfo, Int cChannel);


/*****************************************************************************
    V3 LLM Lossless functions Chao
*****************************************************************************/
// Noise feedback coding.
Void prvNoiseFeedbackFilterEncode(I32 *rgiInput, I32 iLen, I16 iQuant);
Void prvNoiseFeedbackFilterDecode(CBT *rgiInput, I32 iLen, I16 iQuant);

//WMARESULT prvInitLMSPredictor(LMSPredictor * pLMSPrdtr, U16 iMaxOrder, U8 * pMem, I32 * pcbMemUsed, I32 cbMemTtl);
//WMARESULT prvResetLMSPredictor(CAudioObject * pau, LMSPredictor * pLMSPrdtr);
Void    prvLMSPredictorUpdateSpeedChange(CAudioObject * pau, LMSPredictor * pLMSPredictor, Bool bFast);

//I32     prvLMSPredictorPred_I16_C(LMSPredictor * pLMSPredictor);
//Void    prvLMSPredictorUpdate_I16_C(CAudioObject * pau, LMSPredictor * pLMSPredictor, I32 iInput, I32 iPred);
I32     prvLMSPredictorPred_I16_MMX(LMSPredictor * pLMSPredictor);
Void    prvLMSPredictorUpdate_I16_MMX(CAudioObject * pau, LMSPredictor * pLMSPredictor, I32 iInput, I32 iPred);
//I32     prvLMSPredictorPred_I32_C(LMSPredictor * pLMSPredictor);
//Void    prvLMSPredictorUpdate_I32_C(CAudioObject * pau, LMSPredictor * pLMSPredictor, I32 iInput, I32 iPred);
// I32     prvLMSPredictorPred_I32_MMX(LMSPredictor * pLMSPredictor);
// Void    prvLMSPredictorUpdate_I32_MMX(CAudioObject * pau, LMSPredictor * pLMSPredictor, I32 iInput, I32 iPred);
//Void    prvFreeLMSPredictor(LMSPredictor * pLMSPrdtr);

//WMARESULT prvInitMCLMSPredictor(CAudioObject * pau, MCLMSPredictor * pMCLMSPrdtr, I16 iMaxOrder);
//Void    prvMCLMSPredictorReset(CAudioObject * pau, MCLMSPredictor * pMCLMSPrdtr);
//Void    prvMCLMSPredictorPred_I16_C(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT *rgiInput, CBT *rgiPred, Bool bDecoder);
//Void    prvMCLMSPredictorUpdate_I16_C(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT * rgiInput, CBT *rgiPred);
//Void    prvMCLMSPredictorPred_I32_C(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT *rgiInput, CBT *rgiPred, Bool bDecoder);
//Void    prvMCLMSPredictorUpdate_I32_C(CAudioObject * pau, MCLMSPredictor * pMCLMSPredictor, CBT * rgiInput, CBT *rgiPred);
//Void    prvFreeMCLMSPredictor(MCLMSPredictor * pMCLMSPrdtr);

WMARESULT prvLLMVerBResetAllCom(CAudioObject * pau, PerChannelInfo * ppcInfoComms);
WMARESULT prvLLMVerBResetAllCom_MLLM(CAudioObject * pau, PerChannelInfo * ppcInfoComms);

Void prvBackupFiltersLLM_VERB(CAudioObject * pau, PerChannelInfo * ppcInfoComms);
//Void prvRestoreFiltersLLM_VERB(CAudioObject * pau, PerChannelInfo * ppcInfoComms);

Void prvColombCodingBackup(CAudioObject * pau, PerChannelInfo * ppcInfoComms);
Void prvColombCodingRestore(CAudioObject * pau, PerChannelInfo * ppcInfoComms);

//V3 LLMB. leave here temporarily.
Void prvSubFrameQuantize(CAudioObject *pau);
Void prvSubFrameDeQuantize(CAudioObject *pau);

//#ifdef LLM_RLS_EXPT
//WMARESULT prvInitRLSPredictor(RLSPredictor * pRLSPrdtr, U16 iOrder, Float fltLamda, Float fltDelta);
//WMARESULT prvResetRLSPredictor(RLSPredictor * pRLSPrdtr, Bool fClearHistory);
//Float     prvRLSPredictorPred(RLSPredictor * pRLSPredictor);
//Void      prvRLSPredictorUpdate(RLSPredictor * pRLSPredictor, Float fltInput, Float fltPred);
//Void      prvFreeRLSPredictor(RLSPredictor * pRLSPrdtr);
//#endif //LLM_RLS_EXPT

/* End of lossless */

#if 0
//WMARESULT prvMatrixMultiply(Float * rgMatrixInput1, Float * rgMatrixInput2, Float * rgMatrixOutput,
//                            const Int cSize1, const Int cSize2, const Int cSize3);
//
//static INLINE Float auFabs(const Float fltIn)
//{
//    Float fltOut = fltIn;
//    I32 iFltAbs = ((*((Int*)(&fltOut))) & 0x7fffffff);
//
//    fltOut = *((Float*)(&iFltAbs));
//    return fltOut;
//}
//
//// Arithmetic coder stuff =========================================
//
//INLINE Int zeroPerToContext(Float nzp)
//{
//    if (nzp >= 0.90) return 0;
//    if (nzp >= 0.80) return 1;
//    if (nzp >= 0.60) return 2;
//    return 3;
//}
//
//INLINE Int nonRLLevelContext(Int prevLevel2, Int prevLevel)
//{
//    if (prevLevel == 0 && prevLevel2 == 0)
//    {
//        return 0; // 0
//    }
//    else if (prevLevel < 2)
//    {
//        return prevLevel + 1; // 1, 2
//    }
//    else
//    {
//        return 3; // 3
//    }
//}
//
//INLINE Int runContext(Int zeroPerIndex, Int prevRun)
//{
//    return zeroPerIndex*2 + (prevRun == 0 ? 0 : 1);
//}
//
//INLINE Int levelContext(Int prevLevel2, Int prevRun, Int prevLevel, Int curRun)
//{
//#define MRUN (1)
//    if (curRun > MRUN && prevRun > MRUN) return 0; // 0
//    else if (prevRun > MRUN)   // only current run small
//    {
//        if (prevLevel == 1 || prevLevel == 2) return prevLevel; // 1, 2
//        else return 3; // 3
//    }
//    else   // 2 small runs
//    {
//        if (prevLevel2 == 1 && prevLevel == 1)
//            return 4; // 4
//        else if (prevLevel == 1 || prevLevel == 2)
//            return prevLevel + 4; // 5, 6
//        else
//            return 7; // 7
//    }
//#undef MRUN
//}
//
//INLINE Int signContext(Int prevSign2, Int prevRun, Int prevSign, Int curRun)
//{
//#define MRUN (1)
//    if (curRun > MRUN && prevRun > MRUN) return 0; // 0
//    else if (prevRun > MRUN)   // current run <= MRUN
//    {
//        return (prevSign ? 1 : 0) + 1; // 1, 2
//    }
//    else
//    {
//        return (prevSign2 ? 1 : 0)*2 + (prevSign ? 1 : 0) + 3; // 3,4,5,6
//    }
//#undef MRUN
//}
#endif
//*****************************************************************************************
//
// VERIFY_DECODED_COEFFICENT
//
// define VERIFY_DECODED_COEFS and set fltCoefIntFloatThreshold
//
//*****************************************************************************************
//#define VERIFY_DECODED_COEFS
//#if defined(_DEBUG) && defined(VERIFY_DECODED_COEFS)
//#define VERIFY_DECODED_COEFFICENT(idxBark) VerifyDecodedCoefficentHR(pau, ppcinfo, idxBark, iRecon, iLevel, ctCoefRecon)
//#else
#define VERIFY_DECODED_COEFFICENT(idxBark)
//#endif

// *************************************************************************************
// DEBUG_BREAK at a particular Frame in auReconCoefficentsHighRate
//
//#define DECODE_COEF_AT_FRAME 435
//#define DECODE_COEF_AT_SUBFRAME 0
//
//#if 0//defined(_DEBUG) && ( defined(DECODE_COEF_AT_FRAME) || defined(DECODE_COEF_AT_SUBFRAME) )
////#   pragma COMPILER_MESSAGE(__FILE__ "(1154) : Warning - Decode Coefficient Debug Code Enabled.")
//#   if defined(DECODE_COEF_AT_FRAME) && defined(DECODE_COEF_AT_SUBFRAME)
//#       define DEBUG_BREAK_AT_FRAME_DECODE                                                          \
//            if (  ( pau->m_iFrameNumber == DECODE_COEF_AT_FRAME || DECODE_COEF_AT_FRAME < 0 )       \
//                &&( pau->m_iCurrSubFrame == DECODE_COEF_AT_SUBFRAME || DECODE_COEF_AT_SUBFRAME < 0 ) ) \
//                DEBUG_BREAK();
//#   elif defined(DECODE_COEF_AT_FRAME)
//#       define DEBUG_BREAK_AT_FRAME_DECODE                                                          \
//            if ( pau->m_iFrameNumber == DECODE_COEF_AT_FRAME || DECODE_COEF_AT_FRAME < 0 )          \
//                DEBUG_BREAK();
//#   else
//#       define DEBUG_BREAK_AT_FRAME_DECODE                                                          \
//            if ( pau->m_iCurrSubFrame == DECODE_COEF_AT_SUBFRAME || DECODE_COEF_AT_SUBFRAME < 0 )   \
//                DEBUG_BREAK();
//#   endif
//#else   // defined(_DEBUG) && defined(DECODE_COEF_AT_FRAME)
#   define DEBUG_BREAK_AT_FRAME_DECODE
//#endif  // defined(_DEBUG) && defined(DECODE_COEF_AT_FRAME)

//#define RL_PRINT_SRC
#   define DBG_RUNLEVEL(a,b,c,d,e)



// DecodeCoefsHighRate with combined INTEGER and INT_FLOAT code



#define MULT_QUANT(iLevel,ffltQuantizer) MULT_HI(((iLevel)<<16),ffltQuantizer.iFraction)

//#define MAXINTVAL_IQ MAX32BITVAL
//#define MAXINTVAL_IQ_LOG2 (LOG2MAX32BITVAL)   //LOG2(MAXINTVAL_IQ)
//#define MULT_QUANT_AND_SCALE(iLevel,ffltQuantizer)    (CoefType)((((I64)(iLevel))*((I64)(ffltQuantizer.iFraction))) >> (ffltQuantizer.iFracBits))


// When branching is not a high penaty activity, do it the simplier way
//   iCoefRecon = (t=iFracBits-5)<0 ? iCoefRecon>>-t : iCoefRecon<<t
// This SCALE_COEF_RECON requires 3 ops plus 1 branch or 2 ops plus 1 branch.
// This MAKE_MASK_FOR_SCALING requires 2 ops
// SCALE_COEFFICENT gets executed 25x as often as MAKE_MASK_FOR_SCALING, so this method requires 2.58 ops plus 0.04 branches per SCALE_COEFFICENT
// On one test on a 500 MHz Pentium 686, This way saves 1% execution time over masking.
#   define SCALE_COEF_RECON(iCR) (iShift<0) ? (iCR)<<-iShift : (iCR)>>iShift
#   define MAKE_MASK_FOR_SCALING(iFB) iShift=iFB+(16-31-TRANSFORM_FRACT_BITS)


// ===============================================================
// Entropy Code testing **************************

//#ifdef WMA_ENTROPY_TEST
//#include "../../../tools/enttest/entropytest.h"
//extern Bool g_maskTest;
//#endif

//#if defined(RECALC_SINCOS)
//#define RECALC
//#endif

//#if 0//def RECALC
//extern Int g_rcFFT;
//extern Int g_rcEverySampleFFT;
//extern Int g_rcNxFFT;
//extern Int g_rcNFFT;
//extern Int g_rcR;
//extern Int g_rcEverySampleR;
//extern Int g_rcNxR;
//extern Int g_rcNR;
//extern I64 g_maxValFFT;
//#if 0
//#define DORECALC(x, i, L) (g_rc##x && \
//                           (g_rcEverySample##x || \
//                           (g_rcNx##x && ((i%(L/g_rcN##x))==0))))
//#else
//#define DORECALC(x, i, L) (g_rc##x && \
//                           (g_rcEverySample##x || \
//                           (g_rcNx##x && ((i%g_rcN##x)==0))))
//#endif
//#define DORECALC_FFT(i, L) DORECALC(FFT, i, L)
//#define DORECALC_R(i) DORECALC(R, i, iOverlapSize)
////#undef MAXINTVAL_FFT
////#define MAXINTVAL_FFT g_maxValFFT
//#endif

//#if 0
//INLINE void gReadExtraArgs()
//{
//    FILE *f = NULL;
//    f = FOPEN("exargs.txt", "rt");
//    if (f == NULL)
//    {
//#ifdef BUILD_INTEGER
//#ifdef COEF64BIT
//        g_maxValFFT = MAX40BITVAL;
//#else
//        g_maxValFFT = MAX32BITVAL;
//#endif
//#endif
//    }
//    else
//    {
//        fscanf(f, "%d", &g_rcFFT);
//        fscanf(f, "%d", &g_rcEverySampleFFT);
//        fscanf(f, "%d", &g_rcNxFFT);
//        fscanf(f, "%d", &g_rcNFFT);
//        fscanf(f, "%d", &g_rcR);
//        fscanf(f, "%d", &g_rcEverySampleR);
//        fscanf(f, "%d", &g_rcNxR);
//        fscanf(f, "%d", &g_rcNR);
//        fscanf(f, "%I64x", &g_maxValFFT);
//    }
//    FCLOSE(f);
//}
//#endif

#endif //__MSAUDIO_H_

