//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) 1996-1999  Microsoft Corporation

Module Name:

 strmdec_wma.h

Abstract:

 Decoder BitStream

Author:

 Craig Dowell (craigdo@microsoft.com) 10-December-1996
 Ming-Chieh Lee (mingcl@microsoft.com) 10-December-1996
 Bruce Lin (blin@microsoft.com) 10-December-1996

Revision History:
 Bruce Lin (blin@microsoft.com) 02-May-1997
 Add getMaxBits()
    Wei-ge Chen (wchen@microsoft.com) 20-July-1999
    Make it in C.

*************************************************************************/

#ifndef __STRMDEC_WMA_H_
#define __STRMDEC_WMA_H_

#include "wmadec.h"

#define VALID    0
#define END_OF_FILE   1
#define ILLEGAL_PATTERN  3
//#ifndef EOF
//#define EOF                 (-1)
//#endif

#define NONSUPER_WRAPAROUND 1024

#ifdef __cplusplus
extern "C"    // only need to export C interface if
{
    // used by C++ source code
#endif




    typedef struct CAudioObjectDecoder CAudioObjectDecoder;
    typedef struct CWMAInputBitStream
    {

//wchen: add for streaming mode
        PFNGETMOREDATA  m_pfnGetMoreData;
        Void*           m_pUser;
        CAudioObjectDecoder*  m_dwOwner; // not really 'dw' these days

        U32     m_dwHeaderBuf;      // hold the packet header data
        U32     m_dwHeaderBufTemp;  // temporarilly, until frame end

        // The following four variables, while similar in spirit, are used in
        // slightly different ways in packet (normal) mode vs SPDIF mode.
        // In SPDIF mode, BufferBegin refers to the most recent buffer supplied
        // by the caller, and Buffer refers to the current section between gaps.
        // In normal mode, BufferBegin is what it says below and Buffer is the
        // most recent buffer supplied by the caller.
        U8     *m_pBufferBegin;    // begining of packet for reset
        U32     m_cbBuflenBegin;    // used in association of m_pBufferBegin
        U8  *m_pBuffer;
        U32  m_cbBuflen;
        U32     m_dwDot;
        U32     m_dwBitsLeft;
        U32     m_dwDotT;           // hold extra bits for m_dwDot
        U32   m_cBitDotT;
        Int     m_iPrevPacketNum;
        Bool    m_fAllowPackets;
        Bool    m_fSuppressPacketLoss;

        Int     m_cFrmBitCnt;

        // SPDIF gap
        Bool m_fDeferredGap;
        U32  m_cbToDeferredGap;
        Bool m_fNoMoreInput;
        U32  m_cbLastProcessedGapLocationBackFromEndOfLastBuffer;
    } CWMAInputBitStream;

    typedef enum { ModeLookFor, ModePeek, ModeGetFlush, ModePktHdr } GetMoreDataMode;

    Void        ibstrmInit(CWMAInputBitStream* pibstrm, CAudioObjectDecoder* pOwner);
//Void        ibstrmAttach(CWMAInputBitStream *pibstrm, U32 dwDot, U32 dwBitsLeft, U8* pbSrc, I32 cbSrc);
    WMARESULT   ibstrmAttach(CWMAInputBitStream *pibstrm,   U8* pbSrc, I32 cbSrc, Bool fNewPacket, Bool fNoMoreInput, Bool fSPDIF, Int iVersion);
    Void ibstrmReset(CWMAInputBitStream *pibstrm);

    INLINE Void ibstrmSuppressPacketLoss(CWMAInputBitStream *pibstrm)
    {
        pibstrm->m_fSuppressPacketLoss = WMAB_TRUE;
    }
    INLINE UInt ibstrmBitsLeft(const CWMAInputBitStream* pibstrm)
    {
        return pibstrm->m_dwBitsLeft;
    }
    INLINE U32  ibstrmDwBuf(const CWMAInputBitStream* pibstrm)
    {
        return pibstrm->m_dwDot;
    }
    INLINE U8*  ibstrmBuf(const CWMAInputBitStream* pibstrm)
    {
        return pibstrm->m_pBuffer;
    }
    INLINE Int  ibstrmBufLen(const CWMAInputBitStream* pibstrm)
    {
        return pibstrm->m_cbBuflen;
    }
    Int         ibstrmBitsInDots(const CWMAInputBitStream* pibstrm);
    Bool        ibstrmMoreInputAfterGap(const CWMAInputBitStream* pibstrm);
//    Bool        ibstrmNoMoreInput(const CWMAInputBitStream* pibstrm);
    INLINE Void ibstrmSetGetMoreData(CWMAInputBitStream *pibs, PFNGETMOREDATA pfnGetMoreData)
    {
        pibs->m_pfnGetMoreData = pfnGetMoreData;
    }
    INLINE Void ibstrmSetUserData(CWMAInputBitStream *pibs, Void* pUser)
    {
        pibs->m_pUser = pUser;
    }
    INLINE U32  ibstrmGetPacketHeader(CWMAInputBitStream *pibs)
    {

        return pibs->m_dwHeaderBuf;
    }
    INLINE Void ibstrmSetPacketHeader(CWMAInputBitStream *pibs, U32 iVal)
    {
        pibs->m_dwHeaderBuf = iVal;
    }
    INLINE U32  ibstrmGetPacketHeaderT(CWMAInputBitStream *pibs)
    {
        return pibs->m_dwHeaderBufTemp;
    }
    INLINE Void ibstrmSetPacketHeaderT(CWMAInputBitStream *pibs, U32 iVal)
    {
        pibs->m_dwHeaderBufTemp = iVal;
    }
    INLINE Void ibstrmSetPrevPacketNum(CWMAInputBitStream *pibs, U32 iVal)
    {
        pibs->m_iPrevPacketNum = iVal;
    }
    Void        ibstrmResetPacket(CWMAInputBitStream *pibstrm); // reset to the beginning of packet
    Void        ibstrmSetGap(CWMAInputBitStream *pibs, U32 cBitsToGap);
    Void        ibstrmProcessGap(CWMAInputBitStream *pibs);
    Void        ibstrmLoadBits(CWMAInputBitStream *pibs, I32 iFirstBit);
    WMARESULT   ibstrmGetMoreData(CWMAInputBitStream *pibstrm, GetMoreDataMode mode, U32 cBitsNeeded);
    WMARESULT   ibstrmPeekBits(CWMAInputBitStream *pibstrm, register UInt dwNumBits,
                               U32 *piNumBits);
    WMARESULT   ibstrmLookForBits(CWMAInputBitStream *pibstrm, UInt dwNumBits);
    WMARESULT   ibstrmFlushBits(CWMAInputBitStream *pibstrm, register UInt dwNumBits);
    INLINE Void ibstrmFlush(CWMAInputBitStream* pibstrm)
    {
        pibstrm->m_dwBitsLeft -= (pibstrm->m_dwBitsLeft % 8);
    }

// WMA Voice Related
    WMARESULT wmspibstrmLookForBits(CWMAInputBitStream *pibstrm, UInt dwNumBits);
    WMARESULT wmspibstrmLookForBitsNonStop(CWMAInputBitStream *pibstrm,
                                           UInt dwNumBits);
    WMARESULT wmspibstrmPeekBits(CWMAInputBitStream *pibstrm, register UInt dwNumBits,
                                 U32 *piRetBits);
    WMARESULT wmspibstrmFlushBits(CWMAInputBitStream *pibstrm, register UInt dwNumBits);
    WMARESULT wmspibstrmGetBits(CWMAInputBitStream* pibstrm, register UInt dwNumBits,
                                U32 *piResult);
    WMARESULT wmspibstrmPeekBitsNonStop(CWMAInputBitStream *pibstrm, register UInt dwNumBits,
                                        U32 *piRetBits);
// end of WMA Voice related

//    INLINE Void ibstrmFlushByteNoLoad(CWMAInputBitStream *pibstrm)
//    {

//        assert(pibstrm->m_dwBitsLeft >= 8);
//        pibstrm->m_dwBitsLeft -= 8;
//    }

//private
    WMARESULT prvFinalLoad(CWMAInputBitStream* pibstrm,
                           UInt             dwRetval,
                           UInt             dwBitsToAdd,
                           U32             *piResult);

    WMARESULT ibstrmGetBits(CWMAInputBitStream* pibstrm, register UInt dwNumBits,
                            U32 *piResult);

    /*
    {
        register UInt dwRetval;
        register UInt dwShift;

    #ifdef _DEBUG
        g_cBitGet += dwNumBits;
    #endif  // _DEBUG


        if (pibstrm->m_dwBitsLeft <= 24 && pibstrm->m_cbBuflen <= 0)
            ibstrmGetMoreData (pibstrm);
        assert (dwNumBits <= 24);
        //wchen: since the dwDot is always 24bits or more
        //the following code can be greatly simplified. Do it later.

        //
        // This is a little unwieldly, but make sure the end-of-file test falls
        // through in the usual (not end-of-file) case to avoid CPU pipeline stalls
        // due to a branch (or mispredictions on more sophisticated processors).
        //
        if (pibstrm->m_fStreamStaus == VALID &&
            ((UInt) (pibstrm->m_cbBuflen << 3) >= dwNumBits ||
            pibstrm->m_dwBitsLeft + (pibstrm->m_cbBuflen << 3) >= dwNumBits)) {
            //
            // Do the most common case first.  If this doesn't play, we have one branch
            // to get to the next most common case (usually 1/32 of the time in the case
            // of the codec doing a huffman decode).  Note that we use a mask array to
            // avoid a special case branch when the bitcount is 32 (even though this is
            // relatively unlikely) since a left shift operation where the shift count
            // is equal to or greater than the number of bits in the destination is
            // undefined.
            //
            if (dwNumBits <= pibstrm->m_dwBitsLeft) {
                pibstrm->m_dwBitsLeft -= dwNumBits;
       return (pibstrm->m_dwDot >> pibstrm->m_dwBitsLeft) & getMask[dwNumBits];
            }
            //
            // The next most common case is when we have lots of data left in the buffer.
            // and we can fully load (i.e., all 32-bits) our accumulator.  This is
            // hard-wired to allow an optimizer to go crazy with all of the constants.
            // Note that the data is byte-swapped on the way in.
            //
            dwRetval = pibstrm->m_dwDot & getMask[pibstrm->m_dwBitsLeft];
            dwShift = dwNumBits - pibstrm->m_dwBitsLeft;

            if (pibstrm->m_cbBuflen >= 4) {
                register UInt     dwAcc;

                dwAcc = (UInt)pibstrm->m_pBuffer[3];
                dwAcc |= (UInt)(pibstrm->m_pBuffer[2]) << 8;
                dwAcc |= (UInt)(pibstrm->m_pBuffer[1]) << 16;
                dwAcc |= (UInt)(pibstrm->m_pBuffer[0]) << 24;
                pibstrm->m_dwDot = dwAcc;
                pibstrm->m_pBuffer += 4;
                pibstrm->m_cbBuflen -= 4;
                dwRetval <<= dwShift;
                dwRetval |= (dwAcc >> (32 - dwShift));// & getMask[dwShift];
                pibstrm->m_dwBitsLeft = 32 - dwShift;
                return dwRetval;
            }
            //
            // The final, and least likely case, is when we're at the end of the buffer.
            // Since there's really no point in having this inlined since it'll only
            // happen once, I'll call a function to make it happen and save the space
            // in the inline instances of getBits().
            //
      return prvFinalLoad(pibstrm, dwRetval, dwShift);
        }
     else {
            pibstrm->m_fStreamStaus = END_OF_FILE;
            return 0;
        }
    }
    */
#ifdef __cplusplus
}
#endif

#endif // __STRMDEC_WMA_H_


