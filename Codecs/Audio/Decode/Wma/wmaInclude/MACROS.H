//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 1999

Module Name:

    Macros.h

Abstract:

    Processor-specific macros, defines, and data types.

Author:

    Raymond Cheng (raych)       Jul 29, 1999

Revision History:


*************************************************************************/

#ifndef __COMMON_MACROS_H
#define __COMMON_MACROS_H
#include "predefine.h"
#include "asfwmatypes.h"
// ***********************************************************************
// Arithmetic coding, and testing
// **********************************************************************

//#define DEFINE_ARITH_CODER

// ***********************************************************************

// If BUILD_INTEGER or BUILD_INT_FLOAT is not defined by project set default state

#include "constants.h"
#pragma arm section zidata = "WmaDecoderBss",rwdata = "WmaDecoderData"

//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <stdarg.h>
// ======================================================
//ARM Platform
// ======================================================
#define INLINE __inline
#define LITTLE_ENDIAN
typedef unsigned __int64 U64;
typedef __int64 I64;


// **************************************************************************
// Default Fundamental Typedefs
// **************************************************************************
typedef unsigned long int U32;
typedef long int I32;
typedef unsigned short U16;
typedef short I16;
typedef unsigned char U8;
typedef signed char I8;

#include "wmaerror.h"
#pragma arm section zidata = "WmaDecoderBss",rwdata = "WmaDecoderData"
enum
{
// internal SUCCESS codes
    WMA_S_SWITCHCHMODE   = 0x00010001,// Internal error, corrected internally
// the matrix input to matrixinvert is or close to singular. can not invert or the inversion is not precise.
    WMA_S_SINGULARMATRIX = 0x00010002,
    WMA_S_MODELLING      = 0x00010003, // Modelling, skip full encode steps
    WMA_S_CANTMAKEBITS   = 0x00010004,// Can't make the cMin/cMax bit constraint
    WMA_S_DEFAULT        = 0x00010005,
    WMA_S_NOMOREOUTPUT   = 0x00010006,
    WMA_S_PLLM_NOLPC     = 0x00010007, // Did not get reliable LPC filter.
    WMA_S_PLLM_UNSTABLELPC = 0x00010008, // unstable LPC can not be converted to LSP domain.
};
//ON_HOLD: the decoder can't continue operating because of sth, most
//       likely no more data for flushing, getbits or peekbits. all functions
//       stop *immediately*.
enum
{
// internal error codes
    WMA_E_BUFFERUNDERFLOW =  0x80010001,
    WMA_E_INVALIDRECHDR  =   0x80010002,
    WMA_E_SYNCERROR      =   0x80010003,
    WMA_E_NOTIMPL        =   0x80010004,
    WMA_E_FLUSHOVERFLOW  =   0x80010005, // Too much input data provided to flush
    WMA_E_CANNOTPREDICT  =   0x80010007, // Cannot make bit-constrained VBR prediction due to insufficient data
    WMA_E_COEFSTRMFILE   =   0x80010008, // Failure reading or writing from coefficient stream file.
    WMA_E_FILESTRMFILEIO =   0x80010009, // Error in filestrm.c with File I/O (opening, reading or writing).
    WMA_E_MODELINVALID   =   0x8001000A, // Two-pass VBR model cannot predict using given dependent var
    WMA_E_INPUTBUFUNDERFLOW = 0x8001000B, // Two-pass VBR, 2nd pass, not enough input PCM supplied to correlate to output
    WMA_E_FRAMECAPOVERFLOW = 0x8001000C, // v2 only
    WMA_E_NMRPREDICTFAILURE = 0x8001000D, // Two-pass VBR: predictor is unable to generate a meaningful estimate for global NMR
    WMA_E_MODELDEVIATION =   0x8001000E, // Two-pass VBR: current results directly conflict with previous model datapoints (eg, same QStep, diff NMR).
    WMA_E_MODELNOROOM =      0x8001000F, // Two-pass VBR: no more room for additional datapoints!
};
#define WMAB_TRUE 1
#define WMAB_FALSE 0

#define own // used as "reserved word" to indicate ownership or transfer to distinguish from const

typedef double Double;
typedef float Float;
typedef void Void;
typedef U32 UInt;
typedef I32 Int;
typedef I32 Bool; // In Win32, BOOL is an "int" == 4 bytes. Keep it this way to reduce problems.



//***************************************************************************
// 24-bit Encoding
//***************************************************************************

#define PCMSAMPLE_MAX(iValidBits)   ((1 << ((iValidBits) - 1)) - 1)
#define PCMSAMPLE_MIN(iValidBits)   (~((1 << ((iValidBits) - 1)) - 1))

typedef I32 PCMSAMPLE;
#define PCMSAMPLE_BITS  32

#define I24_MIN     ((I32)0xFF800000)
#define I24_MAX     ((I32)0x007FFFFF)
#define I2024_MIN   ((I32)0xFFF80000)
#define I2024_MAX   ((I32)0x0007FFFF)
#define I25_MIN     ((I32)0xFF000000)
#define I25_MAX     ((I32)0x00FFFFFF)
#define I2025_MIN   ((I32)0xFFF00000)
#define I2025_MAX   ((I32)0x000FFFFF)



#define MULT_HI(a,b) (MULT_HI_DWORD(a,b)<<1)


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define checkRange(x, a, b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))

#define ARRAY_SIZE(ar) (sizeof(ar)/sizeof((ar)[0]))

#define DEBUG_ONLY(x)



typedef Int CBT;

// Create types which are change implementation between BUILD_INTEGER and BUILD_INT_FLOAT
// Some platforms, like the X86 and the SH4, can implement either build efficently.
// Other platforms, with only emulated floating point support, are typically only build with BUILD_INTEGER
//
// Each type defined below is either a float (BUILD_INT_FLOAT) and an I32 with some number of fractional bits (BUILD_INTEGER)
// Use float or and I32 with five fractional bits
// And create a type for trig functions (Float or I32 with 30 fractional bits)

#define FLOAT_FROM_FRAC(a, bits) ((a)/((Float)(((I64)1<<(bits)))))
#define FRAC_FROM_FLOAT(a, bits) ((I32)((a)*(Float)(((I64)1<<(bits)))))


#define MULT_HI32_SHIFT(a, b, bits) ((I32)(((I64)(a)*(I64)(b))>>bits))



typedef I32 CoefType;

#   define COEF_FRAC_BITS TRANSFORM_FRACT_BITS // == 5
#   define COEF_FRAC_SCALE (1<<COEF_FRAC_BITS)
#   define COEF_FROM_FLOAT(flt) ((CoefType)(flt*COEF_FRAC_SCALE))
#   define FLOAT_FROM_COEF(coef) (coef/((Float)COEF_FRAC_SCALE))

#   define COEF_FROM_INT(i) ((i)<<pau->m_cLeftShiftBitsFixedPre)
#   define INT_FROM_COEF(cf) ((cf)>>pau->m_cLeftShiftBitsFixedPost)

#   define FRAC_FROM_RATIO(a, b, bits) (DIVI64BYU32((I64)(a)<<(bits), (b)))

// Fractional Type with range -2.0 <= x < 2.0 used by FFT Trig Recurrsion
typedef I32 BP2Type;
#   define BP2_FRAC_BITS 30
#   define BP2_FRAC_SCALE NF2BP2
#   define BP2_FROM_FLOAT(flt) ((BP2Type)(flt*BP2_FRAC_SCALE))
#   define FLOAT_FROM_BP2(bp2) (bp2/((Float)BP2_FRAC_SCALE))
#   define BP2_FROM_BP1(bp1) ((bp1)>>1)
#   define MULT_BP2(a,b) MULT_HI_DWORD_DOWN((a),(b))
// Fractional type with range -1.0 <= x < 1.0 used by DCT Trig Recurrsion
typedef I32 BP1Type;
#   define BP1_FRAC_BITS 31
#   define BP1_FRAC_SCALE NF2BP1
#   define BP1_FROM_FLOAT(flt) ((BP2Type)(flt*BP1_FRAC_SCALE))
#   define FLOAT_FROM_BP1(bp1) (bp1/((Float)BP1_FRAC_SCALE))
#   define MULT_BP1(a,b) MULT_HI((a),(b))

#   define MAX40BITVAL ((I64)0xfffffffff0)
#   define LOG2MAX40BITVAL (39)
#   define MAX32BITVAL ((I32)0x7ffffff0)
#   define LOG2MAX32BITVAL (30)


#   define MAXINTVAL_FFT MAX32BITVAL
#   define MAXINTVAL_FFT_LOG2 (LOG2MAX32BITVAL)  //LOG2(MAXINTVAL_FFT)
#   define MULT_CBP2(a,b) MULT_BP2(a,b)
#   define MULT_CBP1(a,b) MULT_BP1(a,b)


// Channel transform
typedef I32 ChXFormType;
#   define CH_FRAC_BITS (30)

#   define MAXINTVAL_CH MAX32BITVAL
#   define MAXINTVAL_CH_LOG2 (LOG2MAX32BITVAL)   //LOG2(MAXINTVAL_CH)
#   define MULT_CH(a,b) MULT_HI32_SHIFT(b, a, CH_FRAC_BITS)
#   define MULT_CH_SQRT2(a) MULT_HI32_SHIFT(a, INT_SQRT2, INT_SQRT2_BITS)

#   define MULT_CH_MATRIX(a,b) MULT_HI32_SHIFT(a, b, CH_FRAC_BITS)
#   define CH_FROM_FLOAT(flt) FRAC_FROM_FLOAT(flt, CH_FRAC_BITS)
#   define FLOAT_FROM_CH(ch) FLOAT_FROM_FRAC(ch, CH_FRAC_BITS)

typedef ChXFormType ChXFormMType;
#   define CHM_FROM_FLOAT(flt) CH_FROM_FLOAT(flt)
#   define FLOAT_FORM_CHM(ch) FLOAT_FROM_CH(ch)

typedef I32 ChPPXFormType;
#   define CHPP_FRAC_BITS (30)
#   define MULT_CHPP(a,b) MULT_HI32_SHIFT(a, b, CHPP_FRAC_BITS)
#   define CHPP_FROM_FLOAT(flt) FRAC_FROM_FLOAT(flt, CHPP_FRAC_BITS)

// Downmix transform
typedef I32 ChDnMixType;
#   define CHDN_FRAC_BITS (23)
#   define MULT_CHDN(a,b) MULT_HI32_SHIFT(a, b, CHDN_FRAC_BITS)
#   define CHDN_FROM_FLOAT(flt) FRAC_FROM_FLOAT(flt, CHDN_FRAC_BITS)

// Half transform for mllm
typedef I32 HTFilterType;
#   define HTFT_FRAC_BITS (28)
#   define MULT_HTFT(a,b) MULT_HI32_SHIFT(a, b, HTFT_FRAC_BITS)
#   define HTFT_FROM_FLOAT(flt) FRAC_FROM_FLOAT(flt, HTFT_FRAC_BITS)


// fix-point binary points conversion factors
// convert float or double to BP1 integer ( -1.0 <= x < +1.0 )
#define NF2BP1 0x7FFFFFFF
// convert float or double to BP2 integer ( -2.0 <= x < +2.0 )
#define NF2BP2 0x3FFFFFFF//0x10000//
// Fraction is stored at 2^32 (BP0)
#define UBP0_FROM_FLOAT(x) ((U32)(x*4294967296.0F))

typedef I32 FracBitsType;
typedef struct
{
    //I8 exponent;  Old exponent was shift from binary point at 24th position
    FracBitsType iFracBits;
    I32 iFraction;
} FastFloat;

// Type for holding quantization magnitide returned by prvWeightedQuantization
typedef FastFloat QuantFloat;



#   define INTEGER_ONLY(a) a
#   define INTEGER_OR_INT_FLOAT(a,b) a

#define MULT_SQRT2(a) (I32)(((I64)(a) * INT_SQRT2) >> INT_SQRT2_BITS)


#define BUILD_INTEGER_LPC

//****************************************************************************
//         Types, constants related to LPC.
//         Lot of these could be moved to constants.h or lpcConst.h for cleanliness
//****************************************************************************

//  LSP (aka LSF), used for inverse quantized LSF.
typedef Int LspType;
#   define FRACT_BITS_LSP 30
#   define LSP_SCALE (1<<FRACT_BITS_LSP)
#   define LSP_FROM_FLOAT(a) ((LpType)((a)*LSP_SCALE))
#   define MULT_LSP(a,b) (I32)((((I64)(a))*((I64)(b)))>>FRACT_BITS_LSP)
// MULT_LSP may be redefined by processor specific code in macros_cpu.h

//  LP, used for P, Q, and LPC values
typedef Int LpType;
//#   ifndef MATCH_ARM_WAY
#       define FRACT_BITS_LP 27
//#   else
//#       define FRACT_BITS_LP 26
//#   endif
#   define LP_SCALE (1<<FRACT_BITS_LP)
#   define FLOAT_FROM_LP(a) ((Float)((a)*(1.0f/LP_SCALE)))
#   define LP_FROM_FLOAT(a) ((LpType)((a)*LP_SCALE))

//  LP Spectrum: used during FFT of LPC values
//  LP Spectrum power: sum-squares of FFT output
typedef Int LpSpecType;
//                Uses 64-bit intermediates
//#   ifndef MATCH_ARM_WAY
#       define FRACT_BITS_LP_SPEC 25
//#   else
//  ARM's use of 26 instead of 25 produces BIG relative errors compared to LPC_COMPARE
//  at least when using the old version of the Lpc code.
//#       define FRACT_BITS_LP_SPEC 26
//#       define FRACT_BITS_LP_SPEC 25
//#   endif
typedef U64 LpSpecPowerType;
#   define BITS_LP_SPEC_POWER 64
#   define FRACT_BITS_LP_SPEC_POWER 30
#   define QR_FRACTION_FRAC_BITS 24
#   define QR_EXPONENT_FRAC_BITS 29

#   define LP_SPEC_SCALE (1<<FRACT_BITS_LP_SPEC)
#   define LP_SPEC_POWER_SCALE ((float)(1L<<FRACT_BITS_LP_SPEC_POWER))
#   define LP_SPEC_FROM_FLOAT(a) ((LpSpecType)((a)*LP_SPEC_SCALE))
#   define LP_SPEC_POWER_FROM_FLOAT(a) ((LpSpecType)((a)*LP_SPEC_POWER_SCALE))
#   define FLOAT_FROM_LP_SPEC(a) ((Float)((a)*(1.0f/LP_SPEC_SCALE)))
#   define LP_SPEC_FROM_LP(a) ((LpSpecType)((a)>>(FRACT_BITS_LP-FRACT_BITS_LP_SPEC)))

//  Weights, computed from LPC (and Bark?).
typedef UInt WeightType;
//#if 1//def BUILD_INTEGER
typedef UInt FinalWeightType;
#   define FINALWEIGHT_FROM_FLOAT(flt) WEIGHT_FROM_FLOAT(flt)
#   define FINALWEIGHT_FROM_WEIGHT(wt) (wt)
//#else
//typedef Float FinalWeightType;
//#   define FINALWEIGHT_FROM_FLOAT(flt) (flt)
//#   define FINALWEIGHT_FROM_WEIGHT(wt) FLOAT_FROM_WEIGHT(wt)
//#endif
//               We see weight factor values upto 64.53440857: 7 bits enough to cover integer part,
//               leaving upto 25 bits for fraction. However, only 21 bits of fraction are generated
//               upstream, making other 4 bits zero, even if used.
//#   ifndef MATCH_ARM_WAY
#       define WEIGHTFACTOR_FRACT_BITS     21
//#   else
//#       define WEIGHTFACTOR_FRACT_BITS     25
//#   endif
#   define WEIGHT_FROM_FLOAT(a) ((WeightType)((a)*(1<<WEIGHTFACTOR_FRACT_BITS)))
#   define FLOAT_FROM_WEIGHT(a) ((Float)((a)*(1.0f/(1<<WEIGHTFACTOR_FRACT_BITS))))

// convert float or double to BP2 integer ( -2.0 <= x < +2.0 )
#define NF2BP2LPC 0x3FFFFFFF

// Fractional Type with range -2.0 <= x < 2.0 used by FFT Trig Recurrsion
typedef I32 BP2LPCType;
#   define BP2LPC_FRAC_BITS 30
#   define BP2LPC_FRAC_SCALE NF2BP2LPC
#   define BP2LPC_FROM_FLOAT(flt) ((BP2LPCType)((flt)*BP2LPC_FRAC_SCALE))
#   define FLOAT_FROM_BP2LPC(bp2) ((bp2)/((Float)BP2LPC_FRAC_SCALE))
//#if 1//defined(BUILD_INTEGER)
#   define BP2LPC_FROM_BP1(bp1) ((bp1)>>1)
#   define BP2LPC_FROM_BP2(bp2) (bp2)
//#else
//#   define BP2LPC_FROM_BP1(bp1) BP2LPC_FROM_FLOAT(bp1)
//#   define BP2LPC_FROM_BP2(bp2) BP2LPC_FROM_FLOAT(bp2)
//#endif
#   define MULT_BP2LPC(a,b) MULT_HI_DWORD_DOWN((a),(b))
#   define MULT_BP2LPC_I32(a,b) ((I32) (((a) * (I64) (b)) >> BP2LPC_FRAC_BITS))

// Fraction is stored at 2^32 (BP0)
#define UBP0_FROM_FLOAT_LPC(x) ((U32)(x*4294967296.0F))

#   define DIV2LPC(a) ((a)>>1)
#   define MUL2LPC(a) ((a)<<1)

#   define INTEGER_ONLY_LPC(a) a
#   define INTEGER_OR_INT_FLOAT_LPC(a,b) a


//#if 1//!(defined(REFERENCE_RAND_24) || defined(REFERENCE_RAND_16))
//****************************************************************************
// quick and dirty rand generator, assuming low word multiply, only works for
// 32-bit machines see Numerical Recipes p.284.
// Note this is the referenced generator but the 24-bit or 16-bit versions can
// be used for compliance comparisions with 24-bit or 16-bit implementations.
//****************************************************************************

typedef struct tagRandState
{
    I32 iPrior;         // prior value
    U32 uiRand;         // current value
} tRandState;
#if 0
INLINE I32 quickRand(tRandState* ptRandState)
{
    const U32 a = 1664525;
    const U32 c = 1013904223;
    I32 iTemp, iTemp1;
    //a*x + c has to be done with unsigned 32 bit
    ptRandState->uiRand =  a * ptRandState->uiRand + c;

    // uiRand values starting from a 0 seed are: 0x3c6ef35f, 0x47502932, 0xd1ccf6e9, 0xaaf95334, 0x6252e503, 0x9f2ec686, 0x57fe6c2d, ...

    // do not change the above - this reference generator has been extensively tested and has excellent randomness properties
    // a truism in the world of random number generator theory and practice is:
    // "any change, no matter how small, can and will change the properties of the generator and must be fully tested"
    // In case you don't know, it can easily take a person-month to fully test a generator.

    // Notwithstanding the above, it is fair to take a function of a random number to shape its range or distribution.
    // This we do below to give it a triangular distrbution between -2.5 and 2.5 to roughly approximate a Guassian distribution.

    // cast and shift to make the range (-1, 1) with Binary Point 3.
    iTemp = ((I32)(ptRandState->uiRand)) >> 2;

    // *1.25 to make the range (-1.25, 1.25)
    iTemp += (iTemp >> 2);

    // Difference of two random numbers gives a triangle distribution and a range of (-2.5, 2.5)
    // it also gives a serial correlation of -0.5 at lag 1.  But all the other lags have normally small correlations.
    iTemp1 = iTemp - ptRandState->iPrior;

    // Save first term of this difference for next time.
    ptRandState->iPrior = iTemp;

    //return -2.5 to 2.5 with Binary Point = 3 with a triangle distribution
    return iTemp1;
}

INLINE void RandStateClear(tRandState* ptRandState)
{
    ptRandState->iPrior = 0;
    ptRandState->uiRand = 0;
}
#endif

I32 LOG2(U32 i);

#define log2 LOG2


//****************************************************************************
// Normalize a dynamically scaled unsigned int
//****************************************************************************
#if 0//def WMAMIDRATELOWRATE
#pragma arm section code = "WmaLowRateCode"

INLINE void NormUInt(UInt* puiValue, Int* pcFracBits, const UInt uiTarget)
{
    const UInt uiTarget2 = uiTarget >> 1;
    register UInt uiV = *puiValue;
    register Int cFB = *pcFracBits;
    assert(uiV > 0);
    if (uiV == 0) return; // useful if asserts are disabled

    while (uiV < uiTarget2)
    {
        uiV <<= 2;
        cFB += 2;
    }
    if (uiV < uiTarget)
    {
        uiV <<= 1;
        cFB += 1;
    }
    *puiValue = uiV;
    *pcFracBits = cFB;
}

//****************************************************************************
// Align a dynamically scaled int to a particular binary point position
//****************************************************************************
#if 0
INLINE Int Align2FracBits(const Int iValue, const Int cFracBits, const Int cAlignFracBits)
{
    const Int iShift = cFracBits - cAlignFracBits;

    if (iShift < 0)
    {
        return (iValue << -iShift);
    }
    else if (iShift < 32)
    {
        return (iValue >> iShift);
    }
    else
    {
        return 0;
    }
}
#endif
#pragma arm section code

#endif

typedef I32     FLOATORI32;
typedef U32     FLOATORU32;
typedef U16     FLOATORU16;
#define FLOATORU32_CONST(flop,fb)           ((U32)((flop)*(1 << (fb))))
#define FLOATORINT_TOFLOAT(x,fb)            ((Float)(x)/(Float)(1 << (fb)))
#define FLOATORINT_SELECTTOFLOAT(f,i,fb)    ((Float)(i)/(Float)(1 << (fb)))
#define FLOATORINT_ASSERT(x)                assert(x)
#define FLOATORINT_AB(a,b)                  (a)

// The following is used to allow non-integerized code to convert a FLOP
// result to a bit-discarded integer result
#define FLOATORU32_DISCARDBITSFROMFLT(x,fb) ((U32)((x)/(float)(1 << (fb))))
#define FLOATORU32_DISCARDBITSFROMU32(x,fb) ((U32)(x) >> (fb))
#define FLOATORU32_DISCARDBITSFROMI64(x,fb) ((I64)(x) >> (fb))
#define FLOATORU32_ADDFRACTBITS(i,fb)       ((i) << (fb))
#define FLOATORI32_DISCARDBITSFROMI32(x,fb) ((I32)(x) >> (fb))


//--------trace -----------------------
#define TraceResult(code)   (code)

#define TRACEWMA_EXIT(wmaResult, action) \
                            (wmaResult) = (action);                                             \
                            if (WMA_FAILED(wmaResult))                                          \
                            {                                                                   \
                                TraceResult(wmaResult);                                         \
                                goto exit;                                                      \
                            } else {}

#define ASSERTWMA_EXIT(wmaResult, action) \
                            (wmaResult) = (action);                                             \
                            if (WMA_FAILED(wmaResult))                                          \
                            {                                                                   \
                                assert(WMAB_FALSE);                                             \
                                TraceResult(wmaResult);                                         \
                                goto exit;                                                      \
                            } else {}

#define CHECKWMA_EXIT(wmaResult) \
                            if (WMA_FAILED(wmaResult))                                          \
                            {                                                                   \
                                TraceResult(wmaResult);                                         \
                                goto exit;                                                      \
                            } else {}

// Bring in platform-independent macros
#define MULT_HI_DWORD_DOWN(a,b) (I32)((((I64)(a))*((I64)(b)))>>30)
//#define MULT_HI_DWORD_DOWN(a,b) (I32)((((I64)(a))*((I64)(b)))>>16)

#define MULT_HI_DWORD(a,b) (I32)((((I64)(a))*((I64)(b)))>>32)

// Unsigned version of multiply
#define MULT_HI_UDWORD(a,b) (U32)((((U64)(a))*((U64)(b)))>>32)
// **************************************************************************
// Overridable Macros (optimized for some platforms, but not all)
// **************************************************************************
// The following can be overridden within a particular platform macro file
//#ifndef PLATFORM_SPECIFIC_DIVI64BYU32
#if 0
static  INLINE I32 DIVI64BYU32(I64 a, U32 b)
{
    return (I32)((a) / (b));
}
#endif
static INLINE I32 ROUNDF(float f)
{

    if (f < 0.0f)
        return (I32)(f - 0.5f);
    else
        return (I32)(f + 0.5f);
}
//**********************************************************************
// Support for FastFloat
//**********************************************************************
extern void Norm4FastFloat(FastFloat* pfflt);
#if 0//defined(WMAHIGHRATE) || defined(WMAMIDRATELOWRATE)
// Normalize a FastFloat
INLINE
static void Norm4FastFloat(FastFloat* pfflt)
{   // use the faster Norm4FastFloatU when you know the value is positive
    register UInt uiF = abs(pfflt->iFraction);
    register Int iFB = 0;
    if (uiF == 0)
    {
        pfflt->iFracBits = 0;
        return;
    }
    while (uiF < 0x1FFFFFFF)
    {
        uiF <<= 2;
        iFB +=  2;
    }
    if (uiF < 0x3FFFFFFF)
    {
        iFB +=  1;
    }
    pfflt->iFraction <<= iFB;
    pfflt->iFracBits += iFB;
}
#endif// defined(WMAHIGHRATE) || defined(WMAMIDRATELOWRATE)


INLINE
static void Norm4FastFloatU(FastFloat* pfflt)
{   // same as above when we know value is positive (which we often do)
    register UInt uiF = pfflt->iFraction;
    register Int iFB = 0;
    assert(uiF > 0);
    while (uiF < 0x1FFFFFFF)
    {
        uiF  <<= 2;
        iFB +=  2;
    }
    if (uiF < 0x3FFFFFFF)
    {
        uiF  <<= 1;
        iFB +=  1;
    }
    pfflt->iFraction = uiF;
    pfflt->iFracBits += iFB;
}


//INLINE
//static FastFloat ffltMultiply(FastFloat ffltA, FastFloat ffltB)
//{
//    FastFloat ffltP;
//    ffltP.iFraction = MULT_HI(ffltA.iFraction, ffltB.iFraction);
//    ffltP.iFracBits = (ffltA.iFracBits + ffltB.iFracBits - 31);
//    Norm4FastFloat(&ffltP);
//    return ffltP;
//}
#   define FASTFLOAT_MULT(a,b) ffltMultiply((a),(b))

//INLINE
//static FastFloat ffltfltMultiply(FastFloat ffltA, Int B , Int bits)
//{
//    FastFloat ffltP;
//    ffltP.iFracBits = ffltA.iFracBits;
//    ffltP.iFraction = MULT_HI32_SHIFT(ffltA.iFraction, B, bits);
//    Norm4FastFloat(&ffltP);
//    return ffltP;
//}
#   define FASTFLOAT_FLOAT_MULT(a,b,bits) ffltfltMultiply((a), (b), (bits))
#if 0
INLINE
static FastFloat ffltAdd(FastFloat ffltA, FastFloat ffltB)
{
    FastFloat ffltP;

    if (ffltA.iFraction > 0x3FFFFFFF)
    {
        ffltA.iFraction >>= 1;
        ffltA.iFracBits--;
    }
    if (ffltB.iFraction > 0x3FFFFFFF)
    {
        ffltB.iFraction >>= 1;
        ffltB.iFracBits--;
    }

    if (ffltA.iFracBits >= ffltB.iFracBits)
    {
        ffltP.iFracBits = ffltB.iFracBits;
        ffltP.iFraction = ffltB.iFraction + (ffltA.iFraction >> (ffltA.iFracBits - ffltB.iFracBits));
    }
    else
    {
        ffltP.iFracBits = ffltA.iFracBits;
        ffltP.iFraction = ffltA.iFraction + (ffltB.iFraction >> (ffltB.iFracBits - ffltA.iFracBits));
    }
    Norm4FastFloat(&ffltP);
    return ffltP;
}
#endif
#   define FASTFLOAT_ADD(a,b) ffltAdd((a),(b))

#if 0
INLINE
static FastFloat FastFloatFromFloat(Float flt)
{
    FastFloat fflt;
    Float fltScale = (Float)(1 << (31 - 24));
    fflt.iFracBits = 24;
    while (flt < -fltScale || fltScale < flt)
    {
        fflt.iFracBits -= 1;
        fltScale *= 2;
    }
    fflt.iFraction = (I32)(flt * (1 << fflt.iFracBits));
    Norm4FastFloat(&fflt);
    return fflt;
}

// floor of log2(flt)
INLINE
static Int Log2FromFloat(Float flt)
{
    Int i = 0;
    Float fltScale = 2.0;
    assert(flt >= 0.0);
    while (fltScale <= flt)
    {
        i++;
        fltScale *= 2;
    }
    return i;
}


INLINE
static Float FloatFromFastFloat(FastFloat fflt)
{
    assert(0 <= fflt.iFracBits && fflt.iFracBits <= 50);
    if (fflt.iFracBits > 30)
        return fflt.iFraction / (1048576.0F*(1 << (fflt.iFracBits - 20)));
    else
        return fflt.iFraction / ((Float)(1 << fflt.iFracBits));

}
#endif

//#   define FASTFLOAT_FROM_FLOAT(flt) FastFloatFromFloat(flt)
//#   define FLOAT_FROM_FASTFLOAT(fflt) FloatFromFastFloat(fflt)
#   define DOUBLE_FROM_FASTFLOAT(fflt) ((double)fflt.iFraction/(1<<fflt.iFracBits))


INLINE
static FastFloat FastFloatFromU64(U64 u64, Int cExp)
{
    FastFloat fflt;
    U32 uiMSF = (U32)(u64 >> 32);
    Int iExp = 0;
    if (uiMSF == 0)
    {
        iExp = 32;
        uiMSF = (U32)u64;
    }
    if (uiMSF == 0)
    {
        fflt.iFracBits = 0;
        fflt.iFraction = 0;
        return fflt;
    }
    // normalize the most significant fractional part
    while ((uiMSF & 0xF0000000) == 0)
    {
        iExp += 3;
        uiMSF <<= 3;
    }
    while ((uiMSF & 0xC0000000) == 0)
    {
        iExp++;
        uiMSF <<= 1;
    }
    // number of fractional bits
    fflt.iFracBits = iExp + cExp - 32;

    fflt.iFraction = (iExp > 32) ? (U32)(u64 << (iExp - 32)) : (U32)(u64 >> (32 - iExp));

    return fflt;
}
#define FASTFLOAT_FROM_U64(u64,exp) FastFloatFromU64(u64,exp)


typedef FastFloat QuantStepType;
#define DOUBLE_FROM_QUANTSTEPTYPE(qst) DOUBLE_FROM_FASTFLOAT(qst)
#define  FLOAT_FROM_QUANTSTEPTYPE(qst) FLOAT_FROM_FASTFLOAT(qst)
#define FASTFLOAT_FROM_QUANTSTEPTYPE(qst) (qst)

//#define DUMPFLOATDATATOFILE(Data, Len, FileName)
//#define DUMPINTDATATOFILE(Data, Len, FileName)
#ifdef SATURERATE_AFTER_FFT
#define WMA_16BITS_SATURATE(a) if(a > 32767)               \
                                 a = 32767;                \
                               else if(a < -32768)         \
							   	 a = -32768;

#define WMA_OVERLAP_SHIFT(a)  a
typedef short pcmType ;
#else
#define WMA_16BITS_SATURATE(a)
#define WMA_OVERLAP_SHIFT(a)  ((a)>> pau->m_cLeftShiftBitsFixedPost)
typedef long pcmType 	;
#endif
#define TRACE_QUANT_STEP(a) if(a == 0xFFFFFFFF)\
	                           return WMA_S_UNDOWN_ERROR;
#endif  // __COMMON_MACROS_H

