//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 1996 - 1999

Module Name:

    lowrate_common.h

Abstract:

    Macros from lowrate_common.c, placed here so that the macros may be
    used by encoder-only parts of the code.

Author:

    Raymond Cheng (raych)       Jan 18, 2001

*************************************************************************/

#ifndef __LOWRATE_COMMON_H
#define __LOWRATE_COMMON_H


//***************************************************************************
// Encoder/Decoder Separation
//***************************************************************************

// From lowrate_common.c

//#ifdef ENABLE_ALL_ENCOPT
//******************************************************************************************
// Support routines for Resampling indexes to those used when the MaskQ's were last updated
//******************************************************************************************
#define MASKQ_RESAMPLE_OFFSET 6
typedef struct MaskResampleInfo
{
    Int iMaskResampleRatio;
    Int iMaskResampleRatioPow;
    Int cValidBarkBandLatestUpdate;
} MaskResampleInfo;


#ifdef WMAMIDRATELOWRATE
#pragma arm section code = "WmaLowRateCode"

static INLINE Int iResampleIndex(Int iRecon, const MaskResampleInfo MRI)
{ // return resampled linear index suitable for frame that was last updated with MaskQ
    // equvilent to iRecon shifted left or right by (MRI.iMaskResampleRatioPow-MASKQ_RESAMPLE_OFFSET)
    return ((((iRecon) << MRI.iMaskResampleRatioPow) >> MASKQ_RESAMPLE_OFFSET));
}

#if 0
static INLINE Int iUnResampleIndex(Int iResampled, const MaskResampleInfo MRI)

{ // return the reverse of iResampleIndex()
    // that is, convert from resampled indexes of MaskQ or WeightFactor to natural index for the current frame
    if (MRI.iMaskResampleRatioPow > MASKQ_RESAMPLE_OFFSET)
    {
        // Rounding for down shift
        // Although V4 did not round when resampling, this inverse funtion needs to round ???
        return (iResampled + (1 << (MRI.iMaskResampleRatioPow - (MASKQ_RESAMPLE_OFFSET + 1)))) >> (MRI.iMaskResampleRatioPow - MASKQ_RESAMPLE_OFFSET);
        // return iResampled >> (MRI.iMaskResampleRatioPow-MASKQ_RESAMPLE_OFFSET);
    }
    else
    {
        return iResampled << (MASKQ_RESAMPLE_OFFSET - MRI.iMaskResampleRatioPow);
    }
}
//#endif
//*****************************************************************************************
// Look up (10^(1/16)) ^ iMaskQ
//*****************************************************************************************

static INLINE FastFloat ffltMaskPower10(Int iMaskQ)
{ // (10^(1/16)) ^ iMaskQ
    FastFloat fflt;

    // There is, unfortunately, no easy way to keep this assert with our new
    // combined decoder/encoder build.
//#if !defined (ENCODER)
//  assert( -MASK_MINUS_POWER_TABLE_SIZE < iMaskQ && iMaskQ <= MASK_PLUS_POWER_TABLE_SIZE );
//#endif

    if (iMaskQ <= 0)
    {
        if (iMaskQ < -MASK_MINUS_POWER_TABLE_SIZE)
            iMaskQ = -(MASK_MINUS_POWER_TABLE_SIZE - 1);
#ifdef WMA_TABLE_ROOM_VERIFY
        fflt.iFraction = ((const MaskPowerType *)p_rgiMaskMinusPower10)[ -iMaskQ ];
#else
        fflt.iFraction = rgiMaskMinusPower10[ -iMaskQ ]; // with MASK_POWER_FRAC_BITS==28 fractional bits
#endif
        fflt.iFracBits = MASK_POWER_FRAC_BITS + (-iMaskQ >> 2);

    }
    else
    {
        if (iMaskQ >= MASK_PLUS_POWER_TABLE_SIZE)
            iMaskQ = MASK_PLUS_POWER_TABLE_SIZE;
#ifdef WMA_TABLE_ROOM_VERIFY
        fflt.iFraction = ((const MaskPowerType *)p_rgiMaskPlusPower10)[ iMaskQ-1 ];
#else
        fflt.iFraction = rgiMaskPlusPower10[ iMaskQ-1 ]; // with MASK_POWER_FRAC_BITS==28 fractional bits
#endif
        fflt.iFracBits = MASK_POWER_FRAC_BITS - (iMaskQ >> 2);

    }
    return fflt;
}


#endif  // ENABLE_ALL_ENCOPT



//******************************************************************************************
//******************************************************************************************
// prvScanForNextBarkIndex scans for the next resampled bark index and then returns its unresampled linear index
//   iRecon and iHighLimit are linear indexes in the current frames natural scale.
//   iBarkResampled is the bark index in the resampled bark scale (appropriate for the frame when the MaskQ's were last updated)
//   rgiBarkIndex is the resampled bark index scale from that last updated MaskQ subframe
#if 0
static INLINE Int prvScanForNextBarkIndex(const Int iRecon, Int* piBarkResampled, const int iHighLimit,
        const Int* const rgiBarkIndexResampled, const MaskResampleInfo MRI)
{   //Scan for the next resampled bark index
    Int iNextBarkIndex, iBarkResampled, iReconResampled;
    iReconResampled = iResampleIndex(iRecon, MRI);
    while (iReconResampled >= rgiBarkIndexResampled [*piBarkResampled+1])
        ++(*piBarkResampled);
    iBarkResampled = *piBarkResampled;
    if (iBarkResampled >= MRI.cValidBarkBandLatestUpdate)
    {
        assert(iBarkResampled < MRI.cValidBarkBandLatestUpdate);
        return(MRI.cValidBarkBandLatestUpdate);
    }
    iNextBarkIndex = iUnResampleIndex(rgiBarkIndexResampled [iBarkResampled + 1], MRI);
    if (iNextBarkIndex > iHighLimit)
        iNextBarkIndex = iHighLimit;
    return iNextBarkIndex;
}
#endif

#pragma arm section code

#endif




static const I32 cDitherFactorMR = 0x28F5C28F;  // BARK 0.02 * 2^35 (but I am not sure how to derive the 35)


#endif  // __LOWRATE_COMMON_H
