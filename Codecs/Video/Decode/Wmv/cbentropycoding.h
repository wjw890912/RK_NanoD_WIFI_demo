//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*************************************************************************

Copyright (C) Microsoft Corporation, 2003

Module Name:

	cbentropycoding.hpp

Abstract:

	Global definition for the context-based entropy coding

Author:

	Jie Liang (jliang@microsoft.com)

    August 2003

Revision History:


*************************************************************************/
#ifndef __CB_ENTROPY_CODING__
#define __CB_ENTROPY_CODING__

//-----------------------------------------------------
//define this flag to turn on new entropy coding for both encoder and decoder
//-----------------------------------------------------
#define  WMVA_NEW_ENTROPY_CODING

#ifdef  WMVA_NEW_ENTROPY_CODING

enum {TOPNBR = 0, LEFTNBR};
enum {GETSIZEONLY = 0, OUTPUTBITS};
enum BlockEdgeTypes {NORM_BLOCK = 0, HORZ_EDGE, VERT_EDGE};


#define DUMMY_HUFF_SYM                  -1

#define BLKHD_CONTEXTS                  6   //RunNo
#define INTRA_BLKHD_CONTEXTS            5
#define LEVEL_CONTEXTS                  3
#define RUN_CONTEXTS                    3
#define INTRA_RUN_CONTEXTS              2
#define TOTALZEROS_CONTEXTS             2

#define MAX_LEVEL_ZONES                 4
#define ESCLEVEL						32
#define INTRA_MAX_LEVEL_ZONES           4
#define INTRA_ESCLEVEL					32

#define MAX_CODED_ALL_ONES_8x8	        4
#define MAX_CODED_ALL_ONES_8x4	        4
#define MAX_CODED_ALL_ONES_4x8	        4
#define MAX_CODED_ALL_ONES_4x4	        4

#define MAX_CODED_ALL_ONES_INTRA_Y	    4
#define MAX_CODED_ALL_ONES_INTRA_UV	    4

#define THRESHOLD_FOR_DIRECT_RUN_CODING 0

#define	MAX_RUNS_OF_LAST_TWO			7

#define	MAX_JOINTLY_CODED_ZERORUNS		4   // set to 1 to disable joInt coding


#define	DEFAULT_NBR_BLK_RUNNO			1
#define	DEFAULT_NBR_BLK_TOTALZEROS		1

#define ESCLEVEL0						0

#define LOW_QP_THRESHOLD                6
#define MED_QP_THRESHOLD                12

#define LEVELSUM_MAX_CODED_ALL_ONES_8x8	4
#define LEVELSUM_MAX_CODED_ALL_ONES_8x4	4
#define LEVELSUM_MAX_CODED_ALL_ONES_4x8	4
#define LEVELSUM_MAX_CODED_ALL_ONES_4x4	3

#define	MAXZONENUM_T1	9
#define RUNISL_ESC_IDX      15
#define RUNISL_ESC_BITS     6

#define NUMZERO_ESC_BITS     5

#define	MAXZONENUM_TOTALZEROS	8

#define	MAXZONENUM_TOTALZEROS_INTRA		9
#define	MAXZONENUM_T1_INTRA		6
#define	ZONENUM_NUMNONONELEVELS		8
#define	ZONENUM_RUNOFONES		9
#define	ZONENUM_NUMNONONELEVELS_INTRA		8
#define	ZONENUM_RUNOFONES_INTRA		9
#define	ZONENUM_RUN_SHORTSEQ	19
#define	ZONENUM_NONZERORUNS	19
#define	ZONENUM_NUMNONZERORUNS	15
#define	ZONENUM_ZEROSBEFORE		15

//----------------------
// Intra
//----------------------
#define	ZONENUM_RUN_SHORTSEQ_INTRA	19

//arith coding

// don't turn on this. Include dir is not set up, will cause build break.
//#define _UseArithmeticCoding_     //Define this flag to use arithmetic coding. Disable this to use Huffman coding.

#ifdef _UseArithmeticCoding_
#define MAXTRAILINGONES_8x8		7
#define MAXTRAILINGONES_8x4		6
#define MAXTRAILINGONES_4x8		6
#define MAXTRAILINGONES_4x4		4

#define MAXRUNNO_8x8			33
#define MAXRUNNO_8x4			32
#define MAXRUNNO_4x8			32
#define MAXRUNNO_4x4			16
#endif

extern I32_WMV g_iNumZeroContextRule[3][3][3];
extern  I32_WMV g_iEdgeMask_8x8[64];

extern I32_WMV g_iIniLevelThreshold[5];
extern I32_WMV g_iSubBlockSize[5];
extern I32_WMV g_iMaxCodedAllOnes[5];
extern I32_WMV iIntraSubBlockSize[5];
extern I32_WMV g_iIntraMaxCodedAllOnes[2];

extern I32_WMV g_iNumCoefContexts[5];
extern I32_WMV iReso_Accu_BlkHeaderContext[5][BLKHD_CONTEXTS];
extern I32_WMV iReso_Accu_LevelHeaderContext[5][3];
extern I32_WMV	iReso_Accu_RunHeaderContext[5][3];
extern I32_WMV	iReso_Accu_RunNoForNumNonZeroRuns[5][3];
extern I32_WMV iReso_Accu_RunNoForNonZeroRuns[5][3];
extern I32_WMV	iReso_Accu_RunNoForZerosBefore[5][3];
extern I32_WMV iReso_Accu_IntraBlkHeaderContext[2][INTRA_BLKHD_CONTEXTS];
extern I32_WMV iReso_Accu_IntraLevelHeaderContext[2][3];
extern I32_WMV	iReso_Accu_IntraRunNoForNumNonZeroRuns[INTRA_RUN_CONTEXTS];
extern I32_WMV iReso_Accu_IntraRunNoForNonZeroRuns[INTRA_RUN_CONTEXTS];
extern I32_WMV	iReso_Accu_IntraRunNoForZerosBefore[2];

//----------------
// Inter tables
//----------------

// Blk Hd

// Level
extern I32_WMV iZoneNum_T1[5];
extern I32_WMV iReso_Accu_T1[5][9];


extern I32_WMV iZoneNum_NumNonOneLevels[5][3];
extern I32_WMV iReso_NumNonOneLevels[5][3][8];
extern I32_WMV iReso_Accu_NumNonOneLevels[5][3][8];
extern I32_WMV iZoneNum_RunOfOnes[5][3];
extern I32_WMV iReso_RunOfOnes[5][3][9];
extern I32_WMV iReso_Accu_RunOfOnes[5][3][9];

// Run
extern I32_WMV iRunNoZonesForTotalZeros[5];
extern I32_WMV iReso_RunNoZoneForTotalZeros[5][2][8];
extern I32_WMV iReso_Accu_RunNoZoneForTotalZeros[5][2][8];

#define ZONES_NUMNONZERORUNS    10
extern I32_WMV iZoneNum_NumNonZeroRuns[5];
extern I32_WMV iReso_NumNonZeroRuns[5][3][ZONES_NUMNONZERORUNS];
extern I32_WMV iReso_Accu_NumNonZeroRuns[3][ZONES_NUMNONZERORUNS];
extern I32_WMV g_iHufOffset_NumNonZeroRuns[3][10];
extern I32_WMV g_iHufArray_NumNonZeroRuns[624];

#define ZONES_NONZERORUNS       11
extern I32_WMV iZoneNum_NonZeroRuns[5][3];
extern I32_WMV iReso_NonZeroRuns[3][ZONES_NONZERORUNS];
extern I32_WMV iReso_Accu_NonZeroRuns[3][ZONES_NONZERORUNS];
extern I32_WMV g_iHufOffset_NonZeroRuns[3][ZONES_NONZERORUNS];
extern I32_WMV g_iHufArray_NonZeroRuns[873];

#define ZONES_ZEROSBEFORE       9
extern I32_WMV iZoneNum_ZerosBefore[5];
extern I32_WMV iReso_ZerosBefore[3][ZONES_ZEROSBEFORE];
extern I32_WMV iReso_Accu_ZerosBefore[3][ZONES_ZEROSBEFORE];
extern I32_WMV g_iHufOffset_ZerosBefore[3][ZONES_ZEROSBEFORE];
extern I32_WMV g_iHufArray_ZerosBefore[585];

extern I32_WMV iReso_Accu_TotalZeros[5];

//----------------
// Intra tables
//----------------

//Blk HD

extern I32_WMV iRunNoZonesForIntraTotalZeros;
extern I32_WMV iReso_RunNoZoneForIntraTotalZeros[8];
extern I32_WMV iReso_Accu_RunNoZoneForIntraTotalZeros[8];

extern I32_WMV iIntraTotalZeroEscThresholdLeft[8];
extern I32_WMV iIntraTotalZeroEscThresholdRight[8];


//Level
extern I32_WMV iZoneNum_IntraT1;
extern I32_WMV iReso_IntraT1[6];
extern I32_WMV iReso_Accu_IntraT1[6];

extern I32_WMV iZoneNum_IntraNumNonOneLevels;
extern I32_WMV iReso_IntraNumNonOneLevels[8];
extern I32_WMV iReso_Accu_IntraNumNonOneLevels[8];

extern I32_WMV iZoneNum_IntraRunOfOnes;
extern I32_WMV iReso_IntraRunOfOnes[9];
extern I32_WMV iReso_Accu_IntraRunOfOnes[9];

//Run
#define	ZONES_NUMNONZERORUNS_INTRA	7
extern I32_WMV iZoneNum_IntraNumNonZeroRuns[2];
extern I32_WMV iReso_IntraNumNonZeroRuns[2][ZONES_NUMNONZERORUNS_INTRA];
extern I32_WMV iReso_Accu_IntraNumNonZeroRuns[2][ZONES_NUMNONZERORUNS_INTRA];
extern I32_WMV g_iHufOffset_IntraNumNonZeroRuns[2][ZONES_NUMNONZERORUNS_INTRA];
extern I32_WMV g_iHufArray_IntraNumNonZeroRuns[310];

#define	ZONES_NONZERORUNS_INTRA	9
extern I32_WMV iZoneNum_IntraNonZeroRuns[2];
extern I32_WMV iReso_IntraNonZeroRuns[2][ZONES_NONZERORUNS_INTRA];
extern I32_WMV iReso_Accu_IntraNonZeroRuns[2][ZONES_NONZERORUNS_INTRA];
extern I32_WMV g_iHufOffset_IntraNonZeroRuns[2][ZONES_NONZERORUNS_INTRA];
extern I32_WMV g_iHufArray_IntraNonZeroRuns[430];

#define	ZONES_ZEROSBEFORE_INTRA		7
extern I32_WMV iZoneNum_IntraZerosBefore[2];
extern I32_WMV iReso_IntraZerosBefore[2][ZONES_ZEROSBEFORE_INTRA];
extern I32_WMV iReso_Accu_IntraZerosBefore[2][ZONES_ZEROSBEFORE_INTRA];
extern I32_WMV g_iHufOffset_IntraZerosBefore[2][ZONES_ZEROSBEFORE_INTRA];
extern I32_WMV g_iHufArray_IntraZerosBefore[306];

//--------------------
// Huffman tables
//--------------------

// Inter

// Inter 1: blk hd
extern I32_WMV* g_piHufArray_TotalCoeffsAndAllOnes[5];
extern I32_WMV iHufOffset_TotalCoeffsAndAllOnes[5][BLKHD_CONTEXTS];


extern I32_WMV g_iHufOffset_TrailingOnesForNotAllOnes_8x8[3];
extern I32_WMV g_iHufArray_TrailingOnesForNotAllOnes_8x8[21];

extern I32_WMV g_iHufArray_TotalCoeffsAndAllOnes_8x8[450];
extern I32_WMV g_iHufArray_TotalCoeffsAndAllOnes_8x4[258];
extern I32_WMV g_iHufArray_TotalCoeffsAndAllOnes_4x4[205];

extern I32_WMV g_iHufOffset_TrailingOnes_8x8[9];
extern I32_WMV g_iHufArray_TrailingOnes_8x8[209];


extern I32_WMV iTotalZeroEscThresholdLeft[2][8];
extern I32_WMV iTotalZeroEscThresholdRight[2][8];
extern I32_WMV g_iHufOffset_TotalZeros_8x8[2][8];
extern I32_WMV g_iHufOffset_TotalZeros_8x4[2][7];
extern I32_WMV g_iHufOffset_TotalZeros_4x4[2][7];

extern I32_WMV g_iHufArray_TotalZeros_8x8[1048];
extern I32_WMV g_iHufArray_TotalZeros_8x4[794];
extern I32_WMV g_iHufArray_TotalZeros_4x4[370];



// Inter 2: level
extern I32_WMV g_iHufOffset_NumNonOneLevels_8x8[3][8];
extern I32_WMV g_iHufArray_NumNonOneLevels_8x8[610];

extern I32_WMV g_iHufArray_NonOneLevels[252];

extern I32_WMV g_iHufOffset_RunOfOnes_8x8[3][9];
extern I32_WMV g_iHufArray_RunOfOnes_8x8[519];


//Intra

extern I32_WMV g_iHufOffset_IntraTotalCoeffsAndAllOnes[INTRA_BLKHD_CONTEXTS];
extern I32_WMV g_iHufArray_IntraTotalCoeffsAndAllOnes[365];


extern I32_WMV g_iHufOffset_IntraTrailingOnes_Y[6];
extern I32_WMV g_iHufArray_IntraTrailingOnes_Y[194];

// Intra 2: level

extern I32_WMV g_iHufOffset_IntraNumNonOneLevels_Y[8];
extern I32_WMV g_iHufArray_IntraNumNonOneLevels_Y[332];


extern I32_WMV g_iHufArray_IntraNonOneLevels[252] ;

extern I32_WMV g_iHufOffset_IntraRunOfOnes_Y[9];
extern I32_WMV g_iHufArray_IntraRunOfOnes_Y[173];



extern I32_WMV g_iHufOffset_IntraTotalZeros_Y[8];
extern I32_WMV g_iHufArray_IntraTotalZeros_Y[526];


#endif

#endif
