//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef _BUF_FILT_H
#define _BUF_FILT_H

typedef Float CompType;
typedef Int   DataType;

typedef struct BufFilt
{

    CompType *m_filtCoeffs;
    Int m_iStartPos;
    Int m_iLength;

    DataType *m_dataBuf;
    Int m_curBufLen;

    DataType *m_flushBuf;

} BufFilt;

void bufFiltZero(BufFilt *p);
void bufFiltFree(BufFilt *p);
WMARESULT bufFiltInit(BufFilt *p, Double *coeffs, Int startPos, Int endPos);
WMARESULT bufFilt(BufFilt *p,
                  DataType *in, Int inLen,
                  DataType *out, Int *pOutLen);

// In place delay & scale

typedef struct BufScaleDelay
{

    CompType m_scale;
    Int m_iDelay;

    DataType *m_dataBuf;
    DataType *m_endDataBuf;
    Int m_curBufLen;

} BufScaleDelay;

void bufScaleDelayZero(BufScaleDelay *p);
void bufScaleDelayFree(BufScaleDelay *p);
WMARESULT bufScaleDelayInit(BufScaleDelay *p, Double scale, Int delay);
WMARESULT bufScaleDelay(BufScaleDelay *p,
                        DataType *in, Int inLen,
                        Int *pOutLen);

// LtRt downmix functions ==========================

//#define USE_FILTS

typedef struct LtRtDownmix
{

    // 4 buffer filters...
    BufFilt m_bfSurround;
#ifdef USE_FILTS
    BufFilt m_bfLeft, m_bfRight, m_bfCenter, m_bfSubwoofer;
#else
    BufScaleDelay m_bfLeft, m_bfRight, m_bfCenter, m_bfSubwoofer;
#endif

    Int m_maxVal;
    Float m_lostScale;
    Float m_curGain;
    Float m_attackCoeff, m_oneMinusAttackCoeff;
    Float m_releaseCoeff, m_oneMinusReleaseCoeff;
    Int m_xThresh;
    Float m_yThresh;
    Float m_quadScale;

} LtRtDownmix;

void ltrtDownmixZero(LtRtDownmix *p);
void ltrtDownmixFree(LtRtDownmix *p);
WMARESULT ltrtDownmixInit(LtRtDownmix *p, Int nHalfFilterLen,
                          Int samplingRate, Int maxVal);
WMARESULT ltrtDownmix51(LtRtDownmix *p,
                        Int *pLeft, Int *pRight,
                        Int *pCenter, Int *pSubwoofer,
                        Int *pBackLeft, Int *pBackRight,
                        Int *pFiltSurround,
                        Int len, Int *pnOut);
WMARESULT ltrtDownmix51Flush(LtRtDownmix *p,
                             Int *pLeft, Int *pRight,
                             Int *pCenter, Int *pSubwoofer,
                             Int *pBackLeft, Int *pBackRight,
                             Int *pFiltSurround,
                             Int len, Int *pnOut);

#endif

