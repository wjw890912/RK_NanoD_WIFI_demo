//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
//+-------------------------------------------------------------------------
//
//  Microsoft Windows Media
//
//  Copyright (C) Microsoft Corporation, 1998 - 1998
//
//  File:       huffman.h
//
//--------------------------------------------------------------------------

/*  -*-c++-*-
 *  Sanjeev Mehrotra
 *  6/19/1998
 *  Chuang Gu
 *  2/1/2000
 */
#ifndef _HUFFMAN_H
#define _HUFFMAN_H
#include "wmc_typedef.h"
//extern U32_WMV getMask [33];
#define BITSTREAM_READ 1
//class CInputBitStream_WMV;
typedef     enum tagVResultCode
{
  vrNoError        = 0,
  vrFail           = 1,
  vrNotFound       = 2,
  vrExists         = 3,
  vrEof            = 4,
  vrOutOfMemory    = 5,
  vrOutOfResource  = 6,
  vrOutOfBounds    = 7,
  vrBadParam       = 8,
  vrBadInput       = 9,
  vrIOError        = 10,
  vrInterrupted    = 11,
  vrNotSupported   = 12,
  vrNotImplemented = 13,
  vrDropped        = 14,
  vr_ENDOFALLERRORS
} VResultCode;

#define MAX_STAGES 3

#if !defined(UNDER_CE) && !defined(MIMIC_CE_ON_DESKTOP) && !defined (_APOGEE_)
    // define stages assuming large memory and cache
#   define BITS_STAGE1 10
#   define BITS_STAGE2 11
#else
    // define stages assuming small memory and cache
#   define BITS_STAGE1 6
#   define BITS_STAGE2 15
#endif

// Mode            Entry Size   Max # Symbols   Max codeword length
// ----------------------------------------------------------------
// LARGE_HUFFMAN    8 bytes         Large             32 bits
// MED_HUFFMAN      4 bytes         65536             32 bits
// SMALL_HUFFMAN    2 bytes          4096             32 bits

//#define LARGE_HUFFMAN
#ifndef LARGE_HUFFMAN
#define SMALL_HUFFMAN
#endif

#     define ILLEGAL_SYMBOL 4095        /* 2^HUFFDEC_SYMBOL_BITS - 1 */

#if defined(OPT_HUFFMAN_GET_WMV) || 1
  typedef  struct _HuffDecInfo {

#   if ((defined(UNDER_CE) && 1) || (defined(MIMIC_CE_ON_DESKTOP) && 1) || defined(_APOGEE_)) && 0
        // The largest symbol is 1099 because we put indexes, not the real symbol in these tables
        // By using three stages, we
#       define HUFFDEC_SYMBOL_BITS 12
#       define HUFFDEC_LENGTH_BITS 4
        U16_WMV symbol : HUFFDEC_SYMBOL_BITS;
        U16_WMV length : HUFFDEC_LENGTH_BITS;
#   else
        // The following comment does not seem to be true any longer
            //MIPS implementation rely on the specific offsets of these structs. so DON'T CHANGE THEM.
            // - weidongz Nov.3, 2000
#       define HUFFDEC_SYMBOL_BITS 31
#       define HUFFDEC_LENGTH_BITS 31
        U32_WMV symbol;
        U32_WMV length;
#   endif
  } HuffDecInfo;

  typedef  struct _TableInfo {
    I32_WMV bits;
    HuffDecInfo *table;
  } TableInfo;

#else
  typedef  struct _HuffDecInfo {
#     define HUFFDEC_SYMBOL_BITS 12
#     define HUFFDEC_LENGTH_BITS 4

      U16_WMV symbol : 12;
      U16_WMV length : 4;
  } HuffDecInfo;

  typedef  struct _TableInfo {
    I32_WMV bits;
    HuffDecInfo *table;
  } TableInfo;

#endif

typedef  struct _TableInitInfo {
  I32_WMV prefix;     // the prefix that got you there
  I32_WMV start, end; // start, end tableNums of children tables
  I32_WMV bits;       // the bits for this table
  I32_WMV maxBits;    // the maximum # of bits of things entering this table
} TableInitInfo;

#define MAX_STAGE_NUMS 6

//class CVideoObjectDecoder ;
//class CDCTTableInfo_Dec;
typedef  struct _Huffman_WMV
{


//  static U32_WMV m_mask[33]; // 32 bit dependency

  // It is critical that encTable be 32 bits and decTable be 16 bits for
  // the given huffman routines to work
    U32_WMV m_uiUserData;
    I32_WMV m_alphabetSize;
    I32_WMV m_maxCodeLength;
    I32_WMV m_numDecEntries;
    I32_WMV m_allocTables;
    I32_WMV m_allocDecEntries;
    tPackHuffmanCode_WMV *m_encInfo;
  //HuffEncInfo   *m_encInfo;

#if defined ( OPT_HUFFMAN_GET_WMV)
  // it is critical for the mips implementation to work that the order and
  // the types of these members are exactly as shown here.
  // -weidongz Nov.9.2000
#   ifndef __arm
      union
      {
        HuffDecInfo   *m_decInfo;
        U32_WMV OPT_HUFFMAN_GET_Hbase;
      };
#else
    HuffDecInfo   *m_decInfo;
#endif //__arm

  TableInfo     *m_tableInfo;
  I32_WMV m_firstLength;

#else
  HuffDecInfo   *m_decInfo;
  TableInfo     *m_tableInfo;
  I32_WMV m_firstLength;
#endif

  TableInitInfo *m_initInfo;

#ifdef NO_INFO_TABLE

  I32_WMV unchached_table_base;
  HuffDecInfo   *m_decInfo_stage1;
  I32_WMV m_allocDecEntries_stage0;
  I32_WMV m_allocDecEntries_stage1;
  Void_WMV * pWMVDec;
#endif

  I32_WMV stage_size[MAX_STAGE_NUMS];


}Huffman_WMV;

//-----------------------------------------------------------------------------
// Inline definitions
/*
inline I32_WMV Huffman::length(I32_WMV inpSym) {
#ifndef PROFILE
  Assert(inpSym >= 0 && inpSym < m_alphabetSize);
#endif
  return (I32_WMV)m_encInfo[inpSym].length;
}
*/
#endif

