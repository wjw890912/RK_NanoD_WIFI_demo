//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) 1996, 1997  Microsoft Corporation

Module Name:

    strmdec.h

Abstract:

    Decoder BitStream

Author:

    Craig Dowell (craigdo@microsoft.com) 10-December-1996
    Ming-Chieh Lee (mingcl@microsoft.com) 10-December-1996
    Bruce Lin (blin@microsoft.com) 10-December-1996
    Chuang Gu (chuanggu@microsoft.com) 2-Feb-2000

Revision History:
    Bruce Lin (blin@microsoft.com) 02-May-1997
    Add getMaxBits()

*************************************************************************/

#ifndef __STRMDEC_H_
#define __STRMDEC_H_

extern int dbg_currbits[0x10000];
extern int dbg_currbits_idx;
//#pragma pack (1)


//Class istream;

// Hide Debugging/Analysis macros called HUFFMANGET_DBG... in another file
#include "strmdecdbg.h"



#if STRMPARSER_NUMBITS != 64
#define STRMPARSER_NUMBITS 32
#define STRMPARSER_MASK ((U32_WMV)0xffffffff)
typedef U32_WMV STRMPARSER_BUFFER;
#else
#define STRMPARSER_MASK ((U64_WMV)0xffffffffffffffffL)
typedef U64_WMV STRMPARSER_BUFFER;
#endif



#ifdef __STREAMING_MODE_DECODE_
extern  I32_WMV getVideoData(I32_WMV ioffset, U8_WMV** ppBuffer, U32_WMV* dwBufferLen, Bool_WMV* bNotEndOfFrame);
#endif

#define VALID               0
#define END_OF_FILE         1
#define ILLEGAL_PATTERN     3
//#define EOF                 (-1)

extern U32_WMV GetMask_WMV [33];




#   define COVERAGE_ONLY(a)


inline  STRMPARSER_BUFFER ReadIntNBO(U8_WMV *pos)
{
  U32_WMV tmp;
  tmp=*(U32_WMV *)pos;
  tmp= (tmp<<24)|(tmp>>24)|((tmp&0xff00)<<8)|((tmp>>8)&0xff00);



  return tmp;


}

//class CVideoObjectDecoder ;
//class CDCTTableInfo_Dec;


typedef  struct tagCInputBitStream_WMV {


      U32_WMV     m_dwDot;
      I32_WMV     m_cbBuflen;
      U32_WMV     m_dwBitsLeft;
      I32_WMV     m_fStreamStaus;
      U8_WMV     *m_pBuffer;

      Bool_WMV    m_bNotEndOfFrame;

    U32_WMV m_uiUserData;
    U32_WMV Padding;

    //Bool_WMV bNotEndOfFrame;


    Bool_WMV m_bStartcodePrevention;

    I32_WMV m_iStartcodePreventionStatus;

    struct tagWMVDecInternalMember * m_pWMVDec;

    COVERAGE_ONLY( I32_WMV  m_iCntTotalBits;  )     // used for m_pWMVDec->m_tCoverage bit counting

    //To avoid too much dynamic WMV_malloc and WMV_free for tmp buffer.
    //Try to reuse the m_pTmp first if it's big enough.
    U8_WMV * m_pTmp;
    I32_WMV  m_iTmpLen;

}CInputBitStream_WMV;


  extern Bool_WMV BS_invalid (CInputBitStream_WMV * pThis);/*  { return pThis->BNMS.m_BMS8.m_fStreamStaus; }*/
  extern  Void_WMV BS_setInvalid(CInputBitStream_WMV * pThis);/*  { pThis->BNMS.m_BMS8.m_fStreamStaus = ILLEGAL_PATTERN; }*/
  extern  U32_WMV BS_BitsLeft (CInputBitStream_WMV * pThis) ;/*  { return pThis->BNMS.m_BMS8.m_wBitsLeft; }*/

extern Void_WMV BS_addDataToBuffer(
    CInputBitStream_WMV * pThis,
    U8_WMV          *pBuffer,
    U32_WMV        dwBuflen,
    Bool_WMV        bNotEndOfFrame
    );


extern U32_WMV BS_finalLoad( CInputBitStream_WMV * pThis, U32_WMV    dwRetval, U32_WMV       dwBitsToAdd);



Void_WMV BS_StartCodeProcess(CInputBitStream_WMV * pThis, U8_WMV * pBuffer, I32_WMV * pdwBufferLen, Bool_WMV bNotEndOfFrame);
U32_WMV BS_getBits (CInputBitStream_WMV * pThis, register U32_WMV     dwNumBits);
U8_WMV BS_getMaxBits (CInputBitStream_WMV * pThis, register U32_WMV dwNumBits, U8_WMV* rgCodeSymSize);
U32_WMV BS_getBit (CInputBitStream_WMV * pThis);

  U32_WMV BS_peekBits (CInputBitStream_WMV * pThis, const U32_WMV numBits);            //peek bits

    U32_WMV BS_peekBitsTillByteAlign (CInputBitStream_WMV * pThis, I32_WMV* pnBitsToPeek);
    U32_WMV BS_peekBitsFromByteAlign (CInputBitStream_WMV * pThis, I32_WMV numBits); // peek from byte bdry
   inline  I32_WMV BS_eof(CInputBitStream_WMV * pThis)  {

            if (pThis->m_fStreamStaus == END_OF_FILE)
                return EOF;

        return (~EOF);
    }
#endif // __STRMDEC_HPP_

//#pragma pack ()

