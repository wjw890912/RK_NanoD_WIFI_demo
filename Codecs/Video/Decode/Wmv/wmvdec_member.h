//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef __WMVDEC_MEMBER_H_
#define __WMVDEC_MEMBER_H_

#include "wmc_typedef.h"

#include "xplatform_wmv.h"
#include "wmvdec_api.h"
#include "stdio.h"
#include "Huffman_wmv.h"
#include "strmdec_wmv.h"
#include "motioncomp_wmv.h"
#include "bitfields.h"
#include "wmvdec_api.h"
#include "cbentropycoding.h"
#include "decolorconv_shared.h"

//class CInputBitStream_WMV;
//class ifstream;
//class istream;
//class Huffman_WMV;

#define BLOCK_SIZE_2    32
#define BLOCK_SQUARE_SIZE_2 256
#define BLOCK_SQUARE_SIZE_2MINUS2 252
#define BLOCK_SQUARE_SIZE_MINUS1_4 252

#define MAXHALFQP           8
#define MAXHIGHRATEQP       8
#define MAX3QP              8


typedef     enum DCACPREDDIR {LEFT, TOP, NONE} DCACPREDDIR;

extern I32_WMV g_iStepRemap[];      // QP-index to QP mapping array used for 5QP deadzone quantizer.

// Profiles for WMV3
enum { NOT_WMV3 = -1, WMV3_SIMPLE_PROFILE, WMV3_MAIN_PROFILE, WMV3_PC_PROFILE, WMV3_ADVANCED_PROFILE, WMV3_SCREEN };

// enum { NOT_WMV3 = -1, WMV3_SIMPLE_PROFILE, WMV3_MAIN_PROFILE};


extern Bool_WMV g_bSupportMMX_WMV;

#ifdef _6BIT_COLORCONV_OPT_
typedef    struct
{
    I8_WMV g_iYscale [256];
    I8_WMV g_iVtoR [256];
    I8_WMV g_iVtoG [256];
    I8_WMV g_iUtoG [256];
    I8_WMV g_iUtoB [256];

}
YUV2RGB_6BIT_TABLE;

#endif

#define MAX_ADDR_RELOCABLE 8

typedef    struct tUNCACHE_RELOC_CTL
{

    U8_WMV ** ppAddrReloc[MAX_ADDR_RELOCABLE];

    I32_WMV   iNumAddrReloc;

}
UNCACHE_RELOC_CTL;

typedef    struct tUNCACHE_CTL
{
 //   U8_WMV ** ppAddrRelocatable;
    UNCACHE_RELOC_CTL reloc;

    I32_WMV  addr;
    struct tUNCACHE_CTL * next;
}
UNCACHE_CTL;

typedef    struct tCMVPred
{
    Bool_WMV bSameFieldHybridMV;
    Bool_WMV bOppFieldHybridMV;
    I32_WMV iPredSameX1;
    I32_WMV iPredSameY1;
    I32_WMV iPredSameX2;
    I32_WMV iPredSameY2;
    I32_WMV iPredOppX1;
    I32_WMV iPredOppY1;
    I32_WMV iPredOppX2;
    I32_WMV iPredOppY2;
    I32_WMV iPolarity;
} CMVPred;


#define MAXPANSCANWINDOWS       4
typedef    struct tagCPanScanWindowInfo {
    I32_WMV     iWidth;
    I32_WMV     iHeight;
    I32_WMV     iHorizOffset;
    I32_WMV     iVertOffset;
}CPanScanWindowInfo;

typedef    struct tagCPanScanInfo {
    CPanScanWindowInfo  sWindowInfo [MAXPANSCANWINDOWS];
}CPanScanInfo;


#ifdef _EMB_WMV2_

/* the relative offsets of the fields of these classes are important for CE assembly optimizations. Affected files are:
    blkdec_ce_arm.s
*/


struct tagWMVDecInternalMember;

typedef    struct _CDCTTableInfo_Dec {
	Huffman_WMV* hufDCTACDec;
	U32_WMV  iTcoef_ESCAPE;
	U32_WMV  iStartIndxOfLastRunMinus1;
    U8_WMV* puiNotLastNumOfLevelAtRun;
    U8_WMV* puiLastNumOfLevelAtRun;
    U8_WMV* puiNotLastNumOfRunAtLevel;
    U8_WMV* puiLastNumOfRunAtLevel;
    I8_WMV* pcLevelAtIndx;
    U8_WMV* puiRunAtIndx;
#ifdef COMBINED_LEVELRUN
    I16_WMV * combined_levelrun;
#endif
}CDCTTableInfo_Dec;

/* the relative offsets of the fields of these classes are important for CE assembly optimizations. Affected files are:
    blkdec_ce_arm.s
*/
typedef    struct tagEMB_DecodeP_ShortCut
{

    struct tagWMVDecInternalMember * pVideoDecoderObject;

    I32_WMV * m_rgiCoefRecon;
    own CInputBitStream_WMV * m_pbitstrmIn;
    I32_WMV m_i2DoublePlusStepSize;
    I32_WMV m_iDoubleStepSize;
    I32_WMV m_iStepMinusStepIsEven;

    I32_WMV XFormMode;
    I32_WMV m_iDCTHorzFlags;

}EMB_DecodeP_ShortCut;

#else

typedef    struct _CDCTTableInfo_Dec {
	Huffman_WMV* hufDCTACDec;
	U32_WMV  iTcoef_ESCAPE;
	U32_WMV  iStartIndxOfLastRunMinus1;
    U8_WMV* puiNotLastNumOfLevelAtRun;
    U8_WMV* puiLastNumOfLevelAtRun;
    U8_WMV* puiNotLastNumOfRunAtLevel;
    U8_WMV* puiLastNumOfRunAtLevel;
    I8_WMV* pcLevelAtIndx;
    U8_WMV* puiRunAtIndx;
}CDCTTableInfo_Dec;

#endif


typedef     enum THREADTYPE_DEC {REPEATPAD, DECODE, RENDER, LOOPFILTER, DEBLOCK, DEBLOCK_RENDER, DEINTERLACE} THREADTYPE_DEC;
typedef     enum KEYS {FPPM, CPPM, PPM, RES, BITRATE} KEYS;
typedef     enum SKIPBITCODINGMODE {Normal = 0, RowPredict, ColPredict} SKIPBITCODINGMODE;
//I32_WMV AccessRegistry(enum KEYS k, char rw, I32_WMV iData);

// Intra X8 stuff
typedef    struct t_SpatialPredictor {
    const  U8_WMV   *m_pClampTable;
    const U8_WMV    *m_pRef;
    I32_WMV         m_iRefStride;
    Bool_WMV        m_bFlat;
    I32_WMV         m_iDcValue;
    U8_WMV          *m_pLeft, *m_pTop;
    U8_WMV          *m_pNeighbors;
    Bool_WMV        m_bInitZeroPredictor;
    U16_WMV         *m_pSums;
    U16_WMV         *m_pLeftSum;
    U16_WMV         *m_pTopSum;
    I16_WMV         *m_pBuffer16; // buffer for setupZeroPredictor speedup

        Void_WMV  (*m_pIDCT_Dec) (union Buffer * piDst, union Buffer * piSrc, I32_WMV iOffset, I32_WMV iFlags);  // decoder side

} t_CSpatialPredictor;


typedef    struct t_ContextWMV {
    I32_WMV m_iRow;
    I32_WMV m_iCol;
    I32_WMV m_iColGt2;
    U8_WMV *m_pData; // one copy enough?
} t_CContextWMV;

struct t_AltTablesDecoder;

/**********************************************************************
 CLocalHuffmanDecoder : definition
**********************************************************************/
typedef    struct t_LocalHuffmanDecoder
{
    struct t_AltTablesDecoder  *m_pAltTable;

    I32_WMV  m_iN;  // number of symbols inc escapes
    const U8_WMV *m_pFine;
    I32_WMV  m_iMode;
    I32_WMV  m_i1shiftMode;
    I32_WMV  m_iK;  // number of codeword tables

    U8_WMV   *m_pCodeTablePtr;
    U8_WMV   *opt;

    I32_WMV  *fineMask;
    Huffman_WMV  *m_pHuff;
    I32_WMV  logK;
    tPackHuffmanCode_WMV      *m_pCodeTable;

} t_CLocalHuffmanDecoder;


enum { LH_INTER0, LH_INTER, LH_INTRAY0, LH_INTRAY,
// IntraUV uses Inter tables
       LH_ORIENT, LH_INTRAZ, LH_INTRANZ, LH_INTRAC0,
       LH_MV0, LH_MV1, LH_CBP,
       LH_INTRAC1 };
// New Intra coding


typedef    struct t_AltTablesDecoder
{
    U32_WMV    m_uiUserData;
    I32_WMV    m_iNewFrame;
    I32_WMV    m_iNewIntraFrame;
    struct t_LocalHuffmanDecoder   *m_paLH[LH_INTRAC1+1];
    Bool_WMV   m_bIntraFrame;
    I32_WMV    m_iNContexts;

    I32_WMV    m_iQLev;
    Bool_WMV   m_bIsFirstRun;
    I32_WMV    m_iHalfPelMV; // X9

} t_CAltTablesDecoder;

// WMV2's new routines

#ifdef DYNAMIC_EDGEPAD
typedef    struct
{
    I32_WMV m_iStartOfLastLine;
    I16_WMV m_iPMBMV_limit_x;  //m_iWidth-BLOCKSIZE_PLUS_2EDGES
    I16_WMV m_iPMBMV_limit_y;  //m_iHeight-BLOCKSIZE_PLUS_2EDGES
    I16_WMV m_iWidth;
    I16_WMV m_iHeight;
}EdgePad_Y_UV_Data;

typedef    struct
{
    U8_WMV * m_ppxliRef; //MB(Y) or Blk(UV) starting address for ref plane
    U8_WMV * m_ppxliCurr; //MB(Y) or Blk(UV) starting address for curr plane
    U8_WMV * m_ppxliRef0; //origins of ref for the YUV planes

    I32_WMV m_BlkInc;  //block incremental in the normal case
}
EdgePad_Blk_Data;

typedef    struct _WMVEdgePad
{
    I16_WMV m_iPBMV_x;  //position of the (enlarged) block : block + surrounding edges
    I16_WMV m_iPBMV_y;
    I16_WMV m_iWidthYMinusAllEdges;
    I16_WMV m_iHeightYMinusAllEdges;
    I16_WMV x_off_left;
    I16_WMV x_off_right;
    I32_WMV src_offset;
    U8_WMV * ppxlcRefBufferOrig;
    U8_WMV * ppxlcRef;
    U8_WMV * ppxlcCurr;
    U32_WMV m_iWidthCombine;
    U32_WMV m_iround_ctrl;

    EdgePad_Y_UV_Data y_uv_data[2];

    EdgePad_Blk_Data blk_data[6];


    U8_WMV * buffer;

#ifdef MEMBER_EDGEPAD_FUNC_TABLE
    Void_WMV (*EdgePadFuncTable[16])(struct _WMVEdgePad * pEdgePad, EdgePad_Y_UV_Data * pData)
#endif


}
tWMVEdgePad;

#endif
// basic structs

typedef     enum tagFrameType_WMV
{
//    IFRAME_WMV = 0,
//    PFRAME_WMV
      IVOP = 0,
      PVOP,
      BVOP,
      SPRITE,
      BIVOP,
      SKIPFRAME
} tFrameType_WMV;

typedef    struct tagYUV420Frame_WMV
{
    U8_WMV* m_pucYPlane;
    U8_WMV* m_pucUPlane;
    U8_WMV* m_pucVPlane;

    U8_WMV* m_pucYPlane_Unaligned;
    U8_WMV* m_pucUPlane_Unaligned;
    U8_WMV* m_pucVPlane_Unaligned;

} tYUV420Frame_WMV;

typedef    struct tagMotionVector_WMV
{
    I16_WMV m_iMVX;
    I16_WMV m_iMVY;
} tMotionVector_WMV;

typedef    struct {
    I32_WMV iWidthY;
    I32_WMV iWidthUV;
    I32_WMV iHeightY;
    I32_WMV iHeightUV;
    I32_WMV iFrmWidthSrc;
    I32_WMV iFrmHeightSrc;
    I32_WMV iWidthYPlusExp;
    I32_WMV iWidthUVPlusExp;
//    I32_WMV iAreaY;
    Bool_WMV bMBAligned;
    U32_WMV uiNumMBX;
    U32_WMV uiNumMBY;
    U32_WMV uiNumMB;
    U32_WMV uiRightestMB;
    I32_WMV iWidthPrevY;
    I32_WMV iWidthPrevUV;
    I32_WMV iHeightPrevY;
    I32_WMV iHeightPrevUV;
    I32_WMV iWidthPrevYXExpPlusExp;
    I32_WMV iWidthPrevUVXExpPlusExp;
    I32_WMV iMBSizeXWidthPrevY;
    I32_WMV iBlkSizeXWidthPrevUV;
} MULTIRES_PARAMS;

typedef    struct _CQueueElement
{

//    CQueueElement(){};
    struct _CQueueElement* m_pNext;
    Void_WMV* m_pElement;
}CQueueElement;

typedef    struct
{
    CQueueElement *m_pUsedHead;
    CQueueElement *m_pUsedTail;
    CQueueElement *m_pAvailHead;
    CQueueElement *m_pAvailTail;

    I32_WMV m_iCurrQueueElements;
    I32_WMV m_iMaxQueueElements;
}CQueue;

typedef    struct
{

    I32_WMV m_iCurrRefFrames;
    I32_WMV m_iFrameNum;
    I32_WMV m_iFramesChecked;
    CQueue *m_cFrameQueue;
}CReferenceLibrary;

//#ifdef X9
typedef    struct _CDiffMV
{
    I32_WMV      iX : 16;
    I32_WMV      iY : 12;
    U32_WMV     iLast : 1;
    U32_WMV     iIntra : 1;
    U32_WMV     iHybrid : 2;   // 2 bits enough
}CDiffMV;

// Zone_VLC
#define END1STZONE 3
#define END2NDZONE 10

#define MAXHALFQP           8
#define MAXHIGHRATEQP       8
#define MAX3QP              8



#define NUM_LIBREF_FRAMES   2


// Frame level motion mode
enum { MIXED_MV, ALL_1MV, ALL_1MV_HALFPEL, ALL_1MV_HALFPEL_BILINEAR };
// MB level motion mode
enum { MB_1MV, MB_4MV, MB_FIELD, MB_FIELD4MV,
MB_INTRA};

// Filter type
enum { FILTER_BILINEAR, FILTER_BICUBIC, FILTER_6TAP };
// Frame level skipbit (or MV switch) coding mode
enum { SKIP_RAW, SKIP_NORM2, SKIP_DIFF2, SKIP_NORM6, SKIP_DIFF6, SKIP_ROW, SKIP_COL, NOT_DECIDED};
#define  IBLOCKMV  0x4000


typedef    struct {
    I32_WMV iDoubleStepSize;
    I32_WMV iStepMinusStepIsEven;
    I32_WMV i2DoublePlusStepSize;
    I32_WMV i2DoublePlusStepSizeNeg;
    I32_WMV iDCStepSize; // For Intra
} DQuantDecParam;

typedef    struct {
    I8_WMV    chFlag[6];
} LOOPF_FLAG;

#define HARIZONTALEDGEMB 0x05
#define VERTICALEDGEMB 0x0a
#define DQUANTALLEDGEMB 0x0f

//#ifdef _STARTCODE_
#define STARTCODE1 170 // 0xAA
#define STARTCODE2 171 // 0xAB


#define LENSTARTCODE 24
//#endif

typedef    struct tCVector_X9
{
    I16_WMV   x;
    I16_WMV   y;
}   CVector_X9;
typedef    struct tCMotionVector_X9
{
    CVector_X9 m_vctTrueHalfPel;
    CVector_X9 m_vctPredMVHalfPel;
}   CMotionVector_X9;

//#define COVERAGE_STATS at project level to get some coverage statistics
#if defined(COVERAGE_STATS)
    // this provides general coverage info and is not required to decode
    // it is initialized at a sequence header and dumped at the end of the file or at the next sequence header
    typedef    struct _tagCoverage {
        Bool_WMV    m_bFilled;
        // from sequence header
        I32_WMV     m_iWMV3Profile;
        I32_WMV     m_iLevel;
        I32_WMV     m_iChromaFormat;
        Double_WMV  m_dFrameRate;
        I32_WMV     m_iFrameRateSource;
        I32_WMV     m_iFrameRateQuant;
        I32_WMV     m_iBitRate;
        I32_WMV     m_bPostProcInfoPresent;
        I32_WMV     m_iMaxCodedWidth;               // Source
        I32_WMV     m_iMaxCodedHeight;              // Source
        I32_WMV     m_bBroadcastFlags;
        I32_WMV     m_bInterlacedSource;
        I32_WMV     m_bTemporalFrmCntr;
        I32_WMV     m_bSeqFrameInterpolation;
        I32_WMV     m_iaCntASFBinding[9];

        I32_WMV     m_iDispSizeFlagCnt;
        I32_WMV     m_iDisplayWidth;
        I32_WMV     m_iDisplayHeight;
        I32_WMV     m_iAspectRatioFlagCnt;
        I32_WMV     m_iAspectRatio;
        I32_WMV     m_iAspect15HorizSize;
        I32_WMV     m_iAspect15VertSize;
        I32_WMV     m_iColorFormatFlagCnt;
        I32_WMV     m_iColorPrim;
        I32_WMV     m_iTransferChar;
        I32_WMV     m_iMatrixCoef;
        I32_WMV     m_ihrd_num_leaky_buckets;
        I32_WMV     m_iBitRateExponent;
        I32_WMV     m_iBufferSizeExponent;
        I32_WMV     m_ui16aHrdRate[32];
        I32_WMV     m_ui16aHrdBuffer[32];
        I32_WMV     m_iCntUserData [5];

        // from Entry Point Header
        I32_WMV     m_iCntResolutionChanges;
        I32_WMV     m_iCodedWidth;
        I32_WMV     m_iCodedHeight;
        I32_WMV     m_iMinCodedArea;
        I32_WMV     m_iMinCodedWidth;
        I32_WMV     m_iMinCodedHeight;

        I32_WMV     m_iCntEntryPointHeaders;
        I32_WMV     m_iCntBrokenLink;
        I32_WMV     m_iCntClosedEntryPoint;
        I32_WMV     m_iCntPanScanPresent;
        I32_WMV     m_iCntRefDistPresent;
        I32_WMV     m_iCntLoopFilter;
        I32_WMV     m_iCntUVHpelBilinear;
        I32_WMV     m_iCntExtendedMvMode;
        I32_WMV     m_iaCntDQuantCodingOn[4];
        I32_WMV     m_iaCntQuantiser[4];
        I32_WMV     m_iCntXformSwitch;
        I32_WMV     m_iCntSequenceOverlap;
        I32_WMV     m_iCntExplicitSeqQuantizer;
        I32_WMV     m_iCntUse3QPDZQuantizer;
        I32_WMV     m_iCntExplicitFrameQuantizer;
        I32_WMV     m_iCntExtendedDeltaMvMode;
        I32_WMV     m_iaCntRangeRedY[9];
        I32_WMV     m_iaCntRangeRedUV[9];
        U8_WMV      m_pucHrdFullness[32];

        // From Frame Decodes
        I32_WMV     m_iCountFrames;
        I32_WMV     m_iMaxIFrameDistance;
        I32_WMV     m_iMaxIFrameAtFrame;
        I32_WMV     m_iCntNonIFrame;
        I32_WMV     m_iCntIFrames;
        I32_WMV     m_iMaxIFrames;                  // I frames in a row
        I32_WMV     m_iMaxIAtFrame;
        I32_WMV     m_iMaxEPHDistance;
        I32_WMV     m_iMaxEPHAtFrame;
        I32_WMV     m_iCntEPHDistance;
        I32_WMV     m_iaCntFrameType[9];            // count frames of each type, I, P, B, BI, Skip
        I32_WMV     m_iaCntFirstFieldType[9];
        I32_WMV     m_iaCntSecondFieldType[9];
        I32_WMV     m_iaCntWMVFrameTypes[9];        // progressive, interlace frame, interlace field
        I32_WMV     m_iCntSliceTypes[9];            // progressive, interlace frame, interlace field

        I32_WMV     m_iCntTopFieldFirst;
        I32_WMV     m_iCntRepeatFirstField;
        I32_WMV     m_iCntRepeatFrameCount;
        I32_WMV     m_iMaxBFrames;
        I32_WMV     m_iMaxBAtFrame;
        I32_WMV     m_iCntBFrames;
        I32_WMV     m_iCntByteStuffing;             // not implemented yet
        I32_WMV     m_iCntStartCodes[256];
        I32_WMV     m_iaCntMBQuantization[8];       // not implemented yet
        I32_WMV     m_iCntQPIndex[32];
        I32_WMV     m_iCntHalfStep;
        I32_WMV     m_iaCntQP[128];
        I32_WMV     m_iaCntDQuantBiLevelStepSize[128];
        I32_WMV     m_iCntRangeReduxDown;               // really only Main Profile, not AP
        I32_WMV     m_iCntRangeReduxUp;               // really only Main Profile, not AP
        I32_WMV     m_iCntIntensityCompensation;
        I32_WMV     m_iCntRangeRedY;
        I32_WMV     m_iCntRangeRedUV;
        I32_WMV     m_iCntDecRange;
        I32_WMV     m_iMaxFrameBits;
        I32_WMV     m_iMaxFrameBitsAtFrame;
        I32_WMV     m_iStartFrameBits;
        I32_WMV     m_iMax1FrameBits;
        I32_WMV     m_iMax1FrameBitsAtFrame;
        I32_WMV     m_iMax5FrameBits;
        I32_WMV     m_iMax5FrameBitsAtFrame;
        I32_WMV     m_iMax30FrameBits;
        I32_WMV     m_iMax30FrameBitsAtFrame;
        I32_WMV     m_iMax90FrameBits;
        I32_WMV     m_iMax90FrameBitsAtFrame;
        I32_WMV     m_iaStartFrameBits[90];
        I32_WMV     m_idxStartFrameBits;
        I32_WMV     m_iaTotalMBRowBits[9];
        I32_WMV     m_iaCountMBRow[9];
        I32_WMV     m_iaMaxMBRowBits[9];
        I32_WMV     m_iaMaxMBRowBitsAtFrame[9];
        I32_WMV     m_iaMaxMBRowBitsAtRow[9];
        I32_WMV     m_iStartMBRowBits;
        I32_WMV     m_iLimitMBRowBits;
        I32_WMV     m_iaCntExcessMBRowBits[9];
        I32_WMV     m_iCntMultiresEnabled;
        I32_WMV     m_iStartCode;
        I32_WMV     m_iCntPreProcRange;
        I32_WMV     m_iNumBFrames;
        I32_WMV     m_iCntTwoFieldRefPic;
        I32_WMV     m_iCntOppFieldRefPic;
        I32_WMV     m_iCntSameFieldRefPic;
        I32_WMV     m_iCntMVMode[4];

} tCoverage;
#   define COVERAGE_ONLY(a) a
#else  //COVERAGE_STATS
#   define COVERAGE_ONLY(a)
#endif //COVERAGE_STATS


// ===========================================================================
// private data members
// ===========================================================================
struct tagWMVDecInternalMember;
 typedef     struct tagWMVDecInternalMember
{
    // Don't add any thing before this variable! It has been used by ARM assembly code


        I32_WMV* m_rgiDithering;
        I32_WMV m_iDitheringIndex;


    U32_WMV m_uiUserData;
    Bool_WMV m_bSafeMode;
    Bool_WMV m_bStrongARM;

    // sequence layer info

    U32_WMV m_uiFOURCCCompressed;
    I32_WMV m_iPostFilterLevel;

    I32_WMV m_iFrameRate;
    I32_WMV m_iBitRate; // in Kbps

    //VOLMode m_volmd; // vol mode
    //VOPMode m_vopmd; // vop mode

    Bool_WMV m_fPrepared;
    Bool_WMV m_fDecodedI;
    Bool_WMV m_bMBAligned;
    Bool_WMV m_bSizeMBAligned;
    // size info
    I32_WMV m_iWidthSource; // source width, might not be MB aligned
    I32_WMV m_iHeightSource; // source height, might not be MB aligned
    I32_WMV m_iWidthInternal; // internal width, MB aligned
    I32_WMV m_iWidthInternalTimesMB;
    I32_WMV m_iHeightInternal;
    I32_WMV m_iWidthInternalUV;
    I32_WMV m_iWidthInternalUVTimesBlk;
    I32_WMV m_iHeightInternalUV;
    U32_WMV m_uiNumMBX;
    U32_WMV m_uiNumMB;
    U32_WMV m_uiNumMBY;
    U32_WMV m_uintNumMBX;
    U32_WMV m_uintNumMB;
    U32_WMV m_uintNumMBY;
    U32_WMV m_uiRightestMB;



    I32_WMV m_iFrmWidthSrc;
    I32_WMV m_iFrmHeightSrc; // original, non-MB-aligned size of input bitmap
    I32_WMV m_iWidthYRepeatPad;
    I32_WMV m_iWidthUVRepeatPad; // equal original for MMIDRATE43, else = m_iWidthY
    I32_WMV m_iHeightYRepeatPad;
    I32_WMV m_iHeightUVRepeatPad; // equal original for MMIDRATE43, else = m_iHeightY
    I32_WMV m_iWidthY;
    I32_WMV m_iWidthYPlusExp;
    I32_WMV m_iHeightY;
    I32_WMV m_iWidthUV;
    I32_WMV m_iWidthUVPlusExp;
    I32_WMV m_iHeightUV;
    I32_WMV m_iWidthPrevY;
    I32_WMV m_iHeightPrevY;
    I32_WMV m_iHeightPrevUV;
    I32_WMV m_iWidthPrevYXExpPlusExp;
    I32_WMV m_iWidthPrevUVXExpPlusExp;
	I32_WMV m_iMBSizeXWidthPrevY; // MB size (16) * width of Y-Plane, for accessing MB data
	I32_WMV m_iBlkSizeXWidthPrevUV;
    I32_WMV m_iWidthPrevYxBlkMinusBlk;
    I32_WMV m_iMVLeftBound, m_iMVRightBound;
    I32_WMV m_iWidthPrevUV;



    // tables
    const U8_WMV* m_rgiClapTabDec;
    CInputBitStream_WMV     *m_pInputBitstream;
    own CInputBitStream_WMV* m_pbitstrmIn;
    U8_WMV* m_rgiStack;
     U8_WMV* m_rgiClapTab4BitDec;
      Float_WMV m_fltFrameRate;
    Float_WMV m_fltBitRate;
     U16_WMV m_uiNumFramesLeftForOutput;

    // pictures, frame's data
    tYUV420Frame_WMV* m_pfrmCurrQ;
  //  tYUV420Frame_WMV* m_pfrmPrev;
    tYUV420Frame_WMV* m_pfrmRef1Q;
    tYUV420Frame_WMV* m_pfrmRef0Q;
	tYUV420Frame_WMV* m_pfrmMultiresQ;
    tYUV420Frame_WMV* m_pfrmPostQ;
    tYUV420Frame_WMV* m_pfrmPostPrevQ;
// 	U8_WMV* m_ppxliCurrQPlusExpY;
//	U8_WMV* m_ppxliCurrQPlusExpU;
//	U8_WMV* m_ppxliCurrQPlusExpV;


//    U8_WMV* m_ppxliPostQPlusExpY;
//    U8_WMV* m_ppxliPostQPlusExpU;
//    U8_WMV* m_ppxliPostQPlusExpV;

    CVector* m_rgmv;                    // 2 MB row's MV
    U8_WMV* m_ppxliCurrQU;
    CWMVMBMode*	m_rgmbmd;               // 2 MB row's MV
    U8_WMV* m_ppxliRef0Y;
    CWMVMBMode*	m_pmbmdZeroCBPCY;       // Pointer to the ZeroCBPCY
    U8_WMV* m_ppxliCurrQV;
   // U8_WMV* m_rgchSkipPrevFrame;            // Prev frame's Skip bits

    // sequence layer info
    Bool_WMV m_bIFrameDecoded;
    U8_WMV* m_ppxliMultiresY;
    Bool_WMV m_bXintra8Switch;
    Bool_WMV m_bXformSwitch;
    U8_WMV* m_ppxliCurrQY;
    Bool_WMV m_bSKIPBIT_CODING_;
    U8_WMV* m_ppxliRef0V;
    Bool_WMV m_bNEW_PCBPCY_TABLE;
    U8_WMV* m_ppxliMultiresV;
    Bool_WMV m_bMixedPel;
    U8_WMV* m_ppxliRef0U;
    Bool_WMV m_bLoopFilter;
    U8_WMV* m_ppxliMultiresU;
    Bool_WMV m_bNeedLoopFilterOnCurrFrm;
    U8_WMV* m_ppxliRef0YPlusExp;
    Bool_WMV m_bRndCtrlOn;
    U8_WMV* m_ppxliRef1Y;
    I32_WMV m_iRndCtrl;
    U8_WMV* m_ppxliRef1V;
//    Bool_WMV m_iSliceCode;
    I32_WMV m_iSliceCode;
    U8_WMV* m_ppxliRef1YPlusExp;
    U32_WMV m_uintNumMBYSlice; // slice size (number of MB's vertically)
    U8_WMV* m_ppxliRef1U;
    CodecVersion m_cvCodecVersion;
    U8_WMV* m_ppxliPostQY;
    Bool_WMV m_bMainProfileOn;
    U8_WMV* m_ppxliPostPrevQY;
    Bool_WMV m_bMMXSupport;
    U8_WMV* m_ppxliPostQU;

    // picture layer info.
    tFrameType_WMV m_tFrmType;
    U8_WMV* m_ppxliPostPrevQU;
    tFrameType_WMV m_tFrmPredType;
    U8_WMV* m_ppxliPostQV;
    SKIPBITCODINGMODE m_SkipBitCodingMode;
    U8_WMV* m_ppxliPostPrevQV;
    Bool_WMV m_bXintra8;
    Bool_WMV m_bFrmHybridMVOn;
    Bool_WMV m_bDCTTable_MB, m_bDCTTable_MB_ENABLED, m_bDCPred_IMBInPFrame;
    Bool_WMV m_bCODFlagOn;          // COD flag is on (coded) for a P frame
    I32_WMV m_iMvResolution;        // Indicate current frame's MV resolution

    I32_WMV m_iQPIndex;

    I32_WMV m_iStepSize;

    I32_WMV m_iDCStepSize, m_iDCStepSizeC;
    I32_WMV m_i2DoublePlusStepSize, m_i2DoublePlusStepSizeNeg;
    I32_WMV m_iDoubleStepSize, m_iStepMinusStepIsEven;
    Bool_WMV m_bStepSizeIsEven;

    // MB layer info
    Bool_WMV m_bMBHybridMV;         // Indicates whether MB-based HybridMV is on
    Bool_WMV m_bMBXformSwitching;   // Indicates whether MB-based transform switching is chosen for the frame
    I32_WMV m_iMixedPelMV;          // 0 -> True MV, 1 -> Mixed
    I32_WMV m_iFrameXformMode;      // Transform mode (8x8, 8x4 or 4x8) for entire frame
    I32_WMV m_iOffsetToTopMB;       // m_uintNumMBX or -m_uintNumMBX to cycle 2 rows of MB data

    // Block layer buffers
    U8_WMV m_riReconBuf[64*8+32];   // IDCT coeff recon buffer. Allow enough space for 32-byte alignment
    U8_WMV m_riPixelError[64*8+32]; // Pixel error buffer. Allow enough space for 32-byte alignment
    I32_WMV* m_rgiCoefReconPlus1;
    I32_WMV* m_rgiCoefRecon;
    union Buffer* m_ppxliErrorQ;
    union Buffer* m_rgiCoefReconBuf;

    // zigzag scan
    Bool_WMV m_bRotatedIdct;
    U8_WMV* m_pZigzagInv_I;
    U8_WMV* m_pHorizontalZigzagInv;
    U8_WMV* m_pVerticalZigzagInv;
    U8_WMV* m_pZigzagInvRotated_I;
    U8_WMV* m_pHorizontalZigzagInvRotated;
    U8_WMV* m_pVerticalZigzagInvRotated;
    U8_WMV* m_pZigzagInvRotated;
    U8_WMV* m_pZigzagInv;
    //zigzag of _XFORMSWITCHING_
    U8_WMV* m_pZigzagScanOrder;
    U8_WMV* m_p8x4ZigzagInv;
    U8_WMV* m_p4x8ZigzagInv;
    U8_WMV* m_p8x4ZigzagInvRotated;
    U8_WMV* m_p4x8ZigzagInvRotated;
    U8_WMV* m_p8x4ZigzagScanOrder;
    U8_WMV* m_p4x8ZigzagScanOrder;

    // for DCT AC pred.
    I32_WMV m_iNumOfQuanDctCoefForACPredPerRow;
    I16_WMV* m_rgiQuanCoefACPred;
    I16_WMV** m_rgiQuanCoefACPredTable;
    I16_WMV* m_pAvgQuanDctCoefDec;
    I16_WMV* m_pAvgQuanDctCoefDecC;
    I16_WMV* m_pAvgQuanDctCoefDecLeft;
    I16_WMV* m_pAvgQuanDctCoefDecTop;



    // DCT-DCAC prediction
    I32_WMV m_iACColPredOffset, m_iACRowPredOffset;
    I32_WMV m_iACColPredShift, m_iACRowPredShift;



    // Misc
    U32_WMV m_iEscRunDiffV2V3; // Correct EscRunCoding error in MMIDRATE42
    I32_WMV m_iDCPredCorrect;   // Correct DC Pred error in MMIDRATE42
    I32_WMV m_iDCTHorzFlags;    // Sparse IDCT flag
    Bool_WMV m_bFirstEscCodeInFrame;                    //WMV1 vlc EscCoding
    I32_WMV m_iNUMBITS_ESC_LEVEL, m_iNUMBITS_ESC_RUN;   //WMV1 vlc EscCoding
    I32_WMV sm_iIDCTDecCount;    // idct resource counter

//    Bool_WMV* m_pbSkipMB;

    // for X8 : nonflat quantization
    I32_WMV  m_iNonflatQuant;
    t_CSpatialPredictor  *m_pSp;
    t_CContextWMV        *m_pContext;
    t_CAltTablesDecoder  *m_pAltTables;

    // output BMP info
    Bool_WMV m_bBMPInitialized;
    U32_WMV m_uiFOURCCOutput;
    U16_WMV m_uiBitsPerPixelOutput;
    U32_WMV m_uiWidthBMPOutput;
    Bool_WMV m_bRefreshBMP;
    tWMVDecodeDispRotateDegree m_iDispRotateDegree;

//I32_WMV m_iPostProcessMode;
    Bool_WMV m_bPlanarDstBMP;
    Bool_WMV m_bYUVDstBMP;
    I32_WMV m_iDstBMPOffsetU;
    I32_WMV m_iDstBMPOffsetV;
    U32_WMV m_uiRedscale, m_uiGreenscale;
    U32_WMV m_uiRedmask, m_uiGreenmask;
    I32_WMV* m_piYscale;
    I32_WMV* m_piVtoR;
    I32_WMV* m_piUtoG;
    I32_WMV* m_piVtoG;
    I32_WMV* m_piUtoB;
    I32_WMV m_iWidthBMP;
    I32_WMV m_iWidthBMPUV;
    I32_WMV m_iBMPPointerStart;
    I32_WMV m_iBMPMBIncrement, m_iBMPBlkIncrement;
    I32_WMV m_iBMPMBIncrementUV;
    I32_WMV m_iBMPMBHeightIncrement, m_iBMPBlkHeightIncrement, m_iBMPMBHeightIncrementUV;

    // bitmap conversion
    U8_WMV* m_pBMPBits;
    I32_WMV m_iWidthPrevYTimes8Minus8, m_iWidthPrevUVTimes4Minus4;


    // Huffman_WMV Tables
	U8_WMV* m_puXMvFromIndex;
	U8_WMV* m_puYMvFromIndex;
	Huffman_WMV* m_pHufMVDec;
    Huffman_WMV m_hufMVDec_Talking;
    Huffman_WMV m_hufMVDec_HghMt;

	Huffman_WMV* m_pHufMVDec_Set[2];
	U8_WMV* m_puMvFromIndex_Set[4];
    U32_WMV m_iMVTable;

//I32_WMV m_iPostProcessMode;

    // IntraDC
	//def _DCTDC
    Huffman_WMV m_hufDCTDCyDec_Talking;
    Huffman_WMV m_hufDCTDCcDec_Talking;
    Huffman_WMV m_hufDCTDCyDec_HghMt;
    Huffman_WMV m_hufDCTDCcDec_HghMt;

    U32_WMV m_iIntraDCTDCTable;
    Huffman_WMV* m_pHufDCTDCyDec;
    Huffman_WMV* m_pHufDCTDCcDec;
	Huffman_WMV* m_pHufDCTDCDec_Set[4];

    //NEW_PCBPCY_TABLE
    Huffman_WMV m_hufICBPCYDec, m_hufPCBPCYDec;
	Huffman_WMV* m_pHufNewPCBPCYDec;
	Huffman_WMV m_hufPCBPCYDec_HighRate;
	Huffman_WMV m_hufPCBPCYDec_MidRate;
	Huffman_WMV m_hufPCBPCYDec_LowRate;
	//_DCTAC
	// For Table switching
    Huffman_WMV m_hufDCTACInterDec_HghMt;
    Huffman_WMV m_hufDCTACIntraDec_HghMt;
    Huffman_WMV m_hufDCTACInterDec_Talking;
    Huffman_WMV m_hufDCTACIntraDec_Talking;
    Huffman_WMV m_hufDCTACInterDec_MMIDRATE;
    Huffman_WMV m_hufDCTACIntraDec_MMIDRATE;
    Huffman_WMV m_hufDCTACIntraDec_HghRate;

	CDCTTableInfo_Dec InterDCTTableInfo_Dec_HghMt;
	CDCTTableInfo_Dec IntraDCTTableInfo_Dec_HghMt;
	CDCTTableInfo_Dec InterDCTTableInfo_Dec_Talking;
	CDCTTableInfo_Dec IntraDCTTableInfo_Dec_Talking;
	CDCTTableInfo_Dec InterDCTTableInfo_Dec_MMIDRATE;
	CDCTTableInfo_Dec IntraDCTTableInfo_Dec_MMIDRATE;
    CDCTTableInfo_Dec IntraDCTTableInfo_Dec_HghRate;




	CDCTTableInfo_Dec** m_ppInterDCTTableInfo_Dec;
	CDCTTableInfo_Dec** m_ppIntraDCTTableInfo_Dec;
	CDCTTableInfo_Dec* m_pInterDCTTableInfo_Dec_Set[3];
	CDCTTableInfo_Dec* m_pIntraDCTTableInfo_Dec_Set[3];


    U32_WMV m_rgiDCTACInterTableIndx[3];
    U32_WMV m_rgiDCTACIntraTableIndx[3];
    // rendering function pointers
    Void_WMV (*m_pWMVideoDecUpdateDstMB) (
        struct tagWMVDecInternalMember *pWMVDec,
        U8_WMV* pBits,
        const U8_WMV* pucCurrY, const U8_WMV* pucCurrU, const U8_WMV* pucCurrV,
        I32_WMV iWidthY, I32_WMV iWidthUV,
        I32_WMV iBitmapWidth
    );
    Void_WMV (*m_pWMVideoDecUpdateDstPartialMB) (
        struct tagWMVDecInternalMember *pWMVDec,
        U8_WMV* pBits,
        const U8_WMV* pucCurrY, const U8_WMV* pucCurrU, const U8_WMV* pucCurrV,
        I32_WMV iWidthY, I32_WMV iWidthUV,
        I32_WMV iRgbWidth, I32_WMV iRgbHeight
    );
    Void_WMV (*m_pWMVideoDecUpdateDstBlk) (
        struct tagWMVDecInternalMember *pWMVDec,
        U8_WMV* pBits,
        const U8_WMV* pucCurrY, const U8_WMV* pucCurrU, const U8_WMV* pucCurrV,
        I32_WMV iWidthY, I32_WMV iWidthUV,
        I32_WMV iBitmapWidth
    );

    Void_WMV (*m_pWMVideoDecUpdateDstYUVMB) (
        struct tagWMVDecInternalMember *pWMVDec,
        U8_WMV* pBitsY, U8_WMV* pBitsU, U8_WMV* pBitsV,
        const U8_WMV* pucCurrY, const U8_WMV* pucCurrU, const U8_WMV* pucCurrV,
        I32_WMV iWidthY, I32_WMV iWidthUV,
        I32_WMV iBitmapWidthY, I32_WMV iBitmapWidthUV
    );
    Void_WMV (*m_pWMVideoDecUpdateDstPartialYUVMB) (
        struct tagWMVDecInternalMember *pWMVDec,
        U8_WMV* pBitsY, U8_WMV* pBitsU, U8_WMV* pBitsV,
        const U8_WMV* pucCurrY, const U8_WMV* pucCurrU, const U8_WMV* pucCurrV,
        I32_WMV iWidthY, I32_WMV iWidthUV,
        I32_WMV iBitmapWidthY, I32_WMV iBitmapWidthUV,
        I32_WMV iValidWidthY, I32_WMV iValidHeightY
    );

    int (*m_pDecodeI) (struct tagWMVDecInternalMember *pWMVDec);
    int (*m_pDecodeP) (struct tagWMVDecInternalMember *pWMVDec);
    int (*m_pDecodeIMBAcPred) (
        struct tagWMVDecInternalMember *pWMVDec,
        CWMVMBMode*            pmbmd,
        U8_WMV*      ppxliTextureQMBY,
        U8_WMV*      ppxliTextureQMBU,
        U8_WMV*      ppxliTextureQMBV,
        I16_WMV*                piQuanCoefACPred,
        I16_WMV**                piQuanCoefACPredTable,
        Bool_WMV                bLeftMB,
        Bool_WMV                bTopMB,
        Bool_WMV                bLeftTopMB,
        I32_WMV                  iMBX
        );
     tWMVDecodeStatus (*m_pDecodePMB) (
        struct tagWMVDecInternalMember *pWMVDec,
        CWMVMBMode*        pmbmd,
	    U8_WMV* ppxlcCurrQYMB,
	    U8_WMV* ppxlcCurrQUMB,
	    U8_WMV* ppxlcCurrQVMB,
	    CoordI xRefY, CoordI yRefY,
	    CoordI xRefUV, CoordI yRefUV
    );

    // Decoder MB Header
    tWMVDecodeStatus (* m_pDecodeMBOverheadOfIVOP) (struct tagWMVDecInternalMember *pWMVDec, CWMVMBMode* pmbmd, I32_WMV imbX, I32_WMV imbY);
    tWMVDecodeStatus (* m_pDecodeMBOverheadOfPVOP) (struct tagWMVDecInternalMember *pWMVDec, CWMVMBMode* pmbmd);

    //block decoding fuctions
    tWMVDecodeStatus (*m_pDecodeInverseInterBlockQuantize) (struct tagWMVDecInternalMember *pWMVDec, CDCTTableInfo_Dec** InterDCTTableInfo_Dec, U8_WMV* pZigzag, I32_WMV iXformMode,
    DQuantDecParam *pDQ
    );

//I32_WMV m_iPostProcessMode;
    // IDCT routines
    Void_WMV (*m_pIntraIDCT_Dec) (U8_WMV* piDst, I32_WMV iOffsetToNextRowForDCT, const I32_WMV __huge* rgiCoefRecon);
    Void_WMV (*m_pInterIDCT_Dec) (union Buffer* piDst, union Buffer* piSrc, I32_WMV iOffsetToNextRowForDCT, I32_WMV iDCTHorzFlags);
    Void_WMV (*m_pInter8x4IDCT_Dec) (union Buffer* piDst, I32_WMV iOffsetToNextRowForDCT, const union Buffer* rgiCoefRecon, I32_WMV iHalf);
    Void_WMV (*m_pInter4x8IDCT_Dec) (union Buffer* piDst, I32_WMV iOffsetToNextRowForDCT, const union Buffer* rgiCoefRecon, I32_WMV iHalf);
#   ifdef _XFORMSWITCHING2_
    Void_WMV (*m_pInter4x4IDCT_Dec) (union Buffer __huge* piDst, I32_WMV iOffsetToNextRowForDCT, const union Buffer __huge* rgiCoefRecon, I32_WMV iQuadrant);
#   endif


#if !defined(_EMB_WMV2_) || defined(_WMV_TARGET_X86_)
    // Motion Comp
    Void_WMV (*m_pMotionCompAndAddError) (MOTION_COMP_ADD_ERROR_ARGS);
    Void_WMV (*m_pMotionComp) (MOTION_COMP_ARGS);
    Void_WMV (*m_pMotionCompAndAddErrorRndCtrlOn) (MOTION_COMP_ADD_ERROR_ARGS);
    Void_WMV (*m_pMotionCompRndCtrlOn) (MOTION_COMP_ARGS);
    Void_WMV (*m_pMotionCompAndAddErrorRndCtrlOff) (MOTION_COMP_ADD_ERROR_ARGS);
    Void_WMV (*m_pMotionCompRndCtrlOff) (MOTION_COMP_ARGS);

    Void_WMV (*m_pMotionCompUV) (MOTION_COMP_ARGS);
    Void_WMV (*m_pMotionCompAndAddErrorUV) (MOTION_COMP_ADD_ERROR_ARGS);
    Void_WMV (*m_pMotionCompMixed) (MOTION_COMP_ARGS);
    Void_WMV (*m_pMotionCompMixedAndAddError) (MOTION_COMP_ADD_ERROR_ARGS);
#endif
    Void_WMV (*m_pMotionCompZero) (MOTION_COMP_ZERO_ARGS);
//I32_WMV m_iPostProcessMode;

	// Deblocking filter
    Void_WMV (*m_pFilterHorizontalEdge)(U8_WMV* pV, I32_WMV iPixelDistance, I32_WMV iStepSize, I32_WMV iNumPixel);
    Void_WMV (*m_pFilterVerticalEdge)(U8_WMV* pV, I32_WMV iPixelDistance, I32_WMV iStepSize, I32_WMV iNumPixel);
    // IMBInPFrame DC Pred
    I32_WMV (*m_pBlkAvgX8_MMX) (const U8_WMV* ppxlcCurrRecnMB, I32_WMV iWidthPrev, I32_WMV iStepSize);

//I32_WMV m_iPostProcessMode;

// MMIDRATE-compliant variables
    U32_WMV uiFCode;
    I32_WMV iRange;
    I32_WMV iScaleFactor;
    I32_WMV m_iClockRate;

    Time m_t; // current time
    I32_WMV m_iVPMBnum;
    Time m_tModuloBaseDecd;
    Time m_tModuloBaseDisp;
    Time m_tOldModuloBaseDecd;
    Time m_tOldModuloBaseDisp;
    I32_WMV m_iNumBitsTimeIncr;
    Bool_WMV m_bResyncDisable;
    // For MPG4

    I32_WMV m_rgiDCRef [3]; // intra DC predictor


#ifndef _SLIM_C_
    // Multithread event for clearing frame
    // for multi-thread handles

    HANDLE hDecEvent0, hDecDone0;
    HANDLE hDecEvent1, hDecDone1;
    HANDLE threadDec0, threadDec1; //, threadDec2, threadDec3;
    Bool_WMV m_bShutdownThreads;
    THREADTYPE_DEC m_iMultiThreadType;
    I32_WMV m_iCurrQYOffset1, m_iCurrQUVOffset1;
    //Move to vo.h
    U32_WMV m_uiMBStart0, m_uiMBEnd0, m_uiMBEndX0;
	CoordI m_iStartY0, m_iEndY0;
	CoordI m_iStartUV0, m_iEndUV0;
	I32_WMV m_iOffsetPrevY0, m_iOffsetPrevUV0;
    U32_WMV m_uiMBStart1, m_uiMBEnd1, m_uiMBEndX1;
	CoordI m_iStartY1, m_iEndY1;
	CoordI m_iStartUV1, m_iEndUV1;
	I32_WMV m_iOffsetPrevY1, m_iOffsetPrevUV1;
#endif
    //Color Conv, Deblocking, Color Contrl, Possibly removable
//I32_WMV m_iPostProcessMode;

    Bool_WMV m_bDeblockOn;
    Bool_WMV m_bDeringOn; // deblock must be on for dering to be on
    I32_WMV m_bUseOldSetting;
    Bool_WMV m_bRefreshDisplay_AllMB_Enable;
    Bool_WMV m_bRefreshDisplay_AllMB;
    I32_WMV m_iRefreshDisplay_AllMB_Cnt;

    I32_WMV m_iRefreshDisplay_AllMB_Period;
    Bool_WMV m_bCopySkipMBToPostBuf;
    Bool_WMV m_bDefaultColorSetting;
    Bool_WMV m_bCPUQmoved;
    I32_WMV m_iPostProcessMode;
    I32_WMV m_iHurryUp;

    Bool_WMV m_bDisplay_AllMB;
    Bool_WMV m_bRenderFromPostBuffer;


#ifdef _EMB_WMV2_

    U32_WMV m_iMotionCompRndCtrl;
    EMB_DecodeP_ShortCut m_EMBDecodePShortCut;
    tWMVDecodeStatus (*m_CoefDecoderAndIdctDecTable[4])(struct tagWMVDecInternalMember * pThis, CDCTTableInfo_Dec* pInterDCTTableInfo_Dec, I32_WMV iXFormMode);
    tWMVDecodeStatus (*m_pCoefDecoderAndIdctDec8x4or4x8Table[8])(struct tagWMVDecInternalMember * pThis, CDCTTableInfo_Dec* pInterDCTTableInfo_Dec);
    Void_WMV (*m_MotionCompAndAddErrorTable[16])(U8_WMV * ppxlcCurrQMB, const U8_WMV * ppxlcRef, I32_WMV iWidthFrm, I32_WMV * pErrorBuf);

#endif

#ifdef DYNAMIC_EDGEPAD
    tWMVEdgePad m_tEdgePad;
#endif

#ifdef _6BIT_COLORCONV_OPT_
    YUV2RGB_6BIT_TABLE *p_yuv2rgb_6bit_table;
    U8_WMV * m_rgiClapTab6BitDec;
#endif

#ifdef NO_INFO_TABLE
    I32_WMV heap_uncached_size;
    I32_WMV heap_uncached_maxsize;
    U8_WMV * heap_uncached_lowbound;
    I32_WMV heap_uncached_curr;
    I32_WMV X8_uncache_base;
    struct
    {
        UNCACHE_CTL * head;
        UNCACHE_CTL * tail;
    }UncacheCtl;

#endif





    Bool_WMV m_bX9;


    U32_WMV m_uiNumProcessors;

    Bool_WMV m_bMultiresEnabled;
    Bool_WMV m_bRefLibEnabled;

        //Zone_VLC
    U32_WMV m_rgiEndIndexOfCurrZone[3];
    Bool_WMV m_bInterZoneVLCTable;

    // for multires coding
    I32_WMV m_iResIndex;
    I32_WMV m_iResIndexPrev;
//    I32_WMV m_iMultiresFilter;
//    I32_WMV m_iMultiresFilterPrev;
    MULTIRES_PARAMS m_rMultiResParams[4];

    Void_WMV (*m_pHorizDownsampleFilter9)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pHorizDownsampleFilter5)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertDownsampleFilter9)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertDownsampleFilter5)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pHorizUpsampleFilter7)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iVScale, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pHorizUpsampleFilter3)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iVScale, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertUpsampleFilter7)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertUpsampleFilter3)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pHorizDownsampleFilter)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertDownsampleFilter)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pHorizUpsampleFilter)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iVScale, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);
    Void_WMV (*m_pVertUpsampleFilter)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV  iWidthUV,  I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV * pTempBuf);


    // _REFLIBRARY_
    I32_WMV m_iRefFrameNum;
    I32_WMV m_iNewRefFlag;
    CReferenceLibrary *m_cReferenceLibrary;

//#ifdef INTERLACE

    I16_WMV *m_pFieldMvX, *m_pFieldMvY; // Field MV
    // temporary

    I16_WMV *m_ppxliFieldMBBuffer; // Unaligned Buffer
    I16_WMV *m_ppxliFieldMB; // Aligned Buffer


    Bool_WMV m_bShutdownThreads;

    tWMVDecodeStatus (*m_pDeinterlaceProc)(struct tagWMVDecInternalMember * pWMVDec, U8_WMV *pucInput, U8_WMV *pucOutput0, I32_WMV iFirstLine, I32_WMV iLastLine, I32_WMV iWidth, I32_WMV iHeight);


    Huffman_WMV m_hufPCBPCYDec_V9_1;
	Huffman_WMV m_hufPCBPCYDec_V9_2;
	Huffman_WMV m_hufPCBPCYDec_V9_3;
	Huffman_WMV m_hufPCBPCYDec_V9_4;

    Huffman_WMV m_HufMVTable_V9_MV1;
    Huffman_WMV m_HufMVTable_V9_MV2;
    Huffman_WMV m_HufMVTable_V9_MV3;
    Huffman_WMV m_HufMVTable_V9_MV4;

    Huffman_WMV* m_pHufMVTable_V9;

    Huffman_WMV* m_pHufPCBPCYTables[4];

    Huffman_WMV* m_pHufMVTables[8];

    //Huffman_WMV* m_pHufNewICBPCYDec;
    Huffman_WMV* m_pHufICBPCYTables[4];



    Huffman_WMV m_hufDCTACInterDec_HghRate;
	CDCTTableInfo_Dec InterDCTTableInfo_Dec_HghRate;


    Bool_WMV m_bX9HalfPelMotion;
    I32_WMV  m_iX9MVRange;
    I32_WMV  m_iX9MVMode;
    Bool_WMV m_bLuminanceWarp;
    I32_WMV  m_iLuminanceScale, m_iLuminanceShift;
    I32_WMV  m_iSkipbitCodingMode, m_iMVSwitchCodingMode;

    I32_WMV  m_iFilterType; // 2/4/6 tap filter choice
    I16_WMV  *m_pX9dct, *m_pX9dctU, *m_pX9dctV;
    CDiffMV *m_pDiffMV;
    // X and Y motion vectors
       I8_WMV *m_pMultiresBuffer;
    I8_WMV *m_pAlignedMultiresBuffer;

    I32_WMV m_iPanning; // image moving bot<<3 | right<<2 | top <<1 | left


    U8_WMV* m_ppxliRefYPreproc;
    U8_WMV* m_ppxliRefUPreproc;
    U8_WMV* m_ppxliRefVPreproc;

    U8_WMV* m_ppxliCurrQPlusExpY0;
    U8_WMV* m_ppxliCurrQPlusExpU0;
    U8_WMV* m_ppxliCurrQPlusExpV0;


        // Deinterlace
    I32_WMV m_iStartDeinterlace1;
    I32_WMV m_iStartDeinterlace2;
    I32_WMV m_iStartDeinterlace3;
    Bool_WMV m_bHostDeinterlace;


   // I32_WMV m_iACColPredOffset;
   // I32_WMV m_iACRowPredOffset;
  //  I32_WMV m_iACColPredShift;
   // I32_WMV m_iACRowPredShift;

        // for multi-thread handles
    HANDLE hDecEvent0, hDecDone0;
    HANDLE hDecEvent1, hDecDone1;
    HANDLE hDecEvent2, hDecDone2;
    HANDLE hDecEvent3, hDecDone3;
    HANDLE threadDec0, threadDec1, threadDec2, threadDec3;
    THREADTYPE_DEC m_iMultiThreadType;

        // Multithread event for clearing frame
    HANDLE   hX8Clear, hX8MB[4];
    Bool_WMV     m_bWaitingForEvent[4];

    I32_WMV m_iCurrQYOffset1, m_iCurrQUVOffset1;

    U32_WMV m_uiMBStart0, m_uiMBEnd0, m_uiMBEndX0;
	I32_WMV m_iStartY0, m_iEndY0;
	I32_WMV m_iStartUV0, m_iEndUV0;
	I32_WMV m_iOffsetPrevY0, m_iOffsetPrevUV0;


    U32_WMV m_uiMBStart1, m_uiMBEnd1, m_uiMBEndX1;
	I32_WMV m_iStartY1, m_iEndY1;
	I32_WMV m_iStartUV1, m_iEndUV1;
	I32_WMV m_iOffsetPrevY1, m_iOffsetPrevUV1;

#ifdef _WMV_TARGET_X86_
    HANDLE   hHorizontalLpFltrDone[4];
    HANDLE   hReconComplete;
#endif

    U8_WMV * m_pcClapTable;

//    Bool_WMV m_bX4LoopFilter;



 //   I32_WMV m_iModeXform_index;
  //  I8_WMV * m_pchXformType;

    LOOPF_FLAG* m_pLoopFilterFlags;


    Bool_WMV m_iDQuantCodingOn; // seq. level bit indicating possibility of DQuant at frame level
    Bool_WMV m_bDQuantOn; // frame level bit indicating DQuant

    Bool_WMV m_bDQuantBiLevel; // frame level bit indicating bilevel DQ or not
    I32_WMV m_iDQuantBiLevelStepSize; // value of the bilevel DQ


#ifdef OPT_X9MV
struct CX9MV
{
    I32_WMV      iX;
    I32_WMV      iY;
    U32_WMV     iIntra;
};

    CX9MV *m_pChromaMvX9;
    CX9MV *m_pCurrChromaMv;
    I32_WMV m_iMVVectorSize;
    CX9MV *m_pCurrMotion;
    I32_WMV m_iOffsetToTopBlock;
    I32_WMV m_iOffsetToTopBlockMod4;
    I32_WMV m_iCurrBlock;
    I32_WMV m_iCurrTopBlockOffset;
    CX9MV *m_pMotion;
#else
        I16_WMV  *m_pXMotion, *m_pYMotion, *m_pXMotionC, *m_pYMotionC;


#endif

    DQuantDecParam m_rgDQuantParam3QPDeadzone [64];
    DQuantDecParam m_rgDQuantParam5QPDeadzone [64];
    DQuantDecParam *m_prgDQuantParam;

    //DQuantDecParam m_rgDQuantParam [32], m_rgDQuantParam2 [32]; // stepsize 1-32 offset by 1


    Bool_WMV m_bYUV411;

    Bool_WMV m_bRenderFromRef;

    Void_WMV (*m_pIntraX9IDCT_Dec) (union Buffer* piDst, union Buffer* piSrc, I32_WMV  iOffsetToNextRowForDCT, I32_WMV  iDCTHorzFlags);

    Void_WMV (*m_pInterpolateMBRow) (struct tagWMVDecInternalMember * pWMVDec,
        U8_WMV *pSrc, I32_WMV iSrcStride,
        U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iXFrac, I32_WMV iYFrac,
        I32_WMV iFilterType, I32_WMV iRndCtrl,
        I32_WMV iNumRows);

    Void_WMV (*m_pCopyMBRow) (
        U8_WMV *pSrc, I32_WMV iSrcStride,
        U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iNumRows);

    Void_WMV (*m_pInterpolateBlockBicubic) (const U8_WMV *pSrc, I32_WMV iSrcStride,
                                U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iRndCtrl);
    Void_WMV (*m_pInterpolateBlockBilinear) (const U8_WMV *pSrc, I32_WMV iSrcStride,
                                U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iRndCtrl);
    Void_WMV (*m_pAddError) (U8_WMV* ppxlcDst, U8_WMV* ppxlcRef, I16_WMV* ppxliError, I32_WMV iPitch, U8_WMV* pcClapTabMC);

    Void_WMV (*m_pInterpolate20x20RefBicubic) (const U8_WMV *pSrc, I32_WMV iSrcStride,
                                U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iRndCtrl);
    Void_WMV (*m_pInterpolateVariableRefBicubic) (const U8_WMV *pSrc, I32_WMV iSrcStride,
                                U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iWidth, I32_WMV iHeight,I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iRndCtrl);
    Void_WMV (*m_pInterpolate20x20RefBilinear) (const U8_WMV *pSrc, I32_WMV iSrcStride,
                                U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iRndCtrl);

        U8_WMV *m_puchFilter8x8RowFlagY0, *m_puchFilter8x8RowFlagY1;
    U8_WMV *m_puchFilter8x4RowFlagY0, *m_puchFilter8x4RowFlagY1;
    U8_WMV *m_puchFilter8x8RowFlagU0, *m_puchFilter8x8RowFlagU1;
    U8_WMV *m_puchFilter8x4RowFlagU0, *m_puchFilter8x4RowFlagU1;
    U8_WMV *m_puchFilter8x8RowFlagV0, *m_puchFilter8x8RowFlagV1;
    U8_WMV *m_puchFilter8x4RowFlagV0, *m_puchFilter8x4RowFlagV1;

    U8_WMV *m_puchFilter8x8ColFlagY0, *m_puchFilter8x8ColFlagY1;
    U8_WMV *m_puchFilter4x8ColFlagY0, *m_puchFilter4x8ColFlagY1;
    U8_WMV *m_puchFilter8x8ColFlagU0, *m_puchFilter8x8ColFlagU1;
    U8_WMV *m_puchFilter4x8ColFlagU0, *m_puchFilter4x8ColFlagU1;
    U8_WMV *m_puchFilter8x8ColFlagV0, *m_puchFilter8x8ColFlagV1;
    U8_WMV *m_puchFilter4x8ColFlagV0, *m_puchFilter4x8ColFlagV1;

        Bool_WMV    m_b16bitXform;     // bilinear 16 bit transform flag
    Bool_WMV    m_bNewDCQuant;     //indicates whether improved DC quantization is used at low QP

	Bool_WMV m_bUVHpelBilinear; // UV speedup

        I32_WMV  m_iWMV3Profile;

   // Bool_WMV m_nExtendedMvMode;
    Bool_WMV m_bStartCode;
    I32_WMV  m_iMVRangeIndex, m_iLogXRange, m_iLogYRange, m_iXMVRange, m_iYMVRange, m_iXMVFlag, m_iYMVFlag;

#ifdef  _NEW_DECODE_
     U8_WMV m_rgiDCTHorzFlags[6];
#endif

     union Buffer * m_rgErrorBlock;          //This needs to be cache block alligned
                                       //the whole class should be alligned so we dont need a pointer for this
    union Buffer m_rgBottomOrRightDequan[6]; //These should be merged with m_rgErrorBlock but the IDCT and
    union Buffer m_rgTopOrLeftDequan[6];     //the 4x8 and 8x4 zigZagOrder need to be changed to do this

#ifdef _XFORMSWITCHINGI2_
    I32_WMV m_i4x4ACColPredOffset, m_i4x4ACRowPredOffset;
    I32_WMV m_i4x4ACColPredShift, m_i4x4ACRowPredShift;
#endif

        Void_WMV (*m_pHorizDownsampleFilter6)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV iWidthUV, I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV* pTempBuf);
        Void_WMV (*m_pVertDownsampleFilter6)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, U8_WMV* pDstY, U8_WMV* pDstU, U8_WMV* pDstV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV iWidthUV, I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV* pTempBuf);
        Void_WMV (*m_pHorizUpsampleFilter10)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iVScale, I32_WMV iWidth, I32_WMV iHeight, I32_WMV iWidthUV, I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV* pTempBuf);
        Void_WMV (*m_pVertUpsampleFilter10)(U8_WMV* pSrcY, U8_WMV* pSrcU, U8_WMV* pSrcV, I32_WMV iWidth, I32_WMV iHeight, I32_WMV iWidthUV, I32_WMV iHeightUV, I32_WMV iPitchY, I32_WMV iPitchUV, I8_WMV* pTempBuf);

    I32_WMV  m_iNumBFrames;
   // I32_WMV  m_iBFrameCounter;
#ifdef B_FRAME
    I32_WMV  m_iBFrameReciprocal;
    MBType  m_mbtShortBSymbol, m_mbtLongBSymbol;
#endif // B_FRAME

        Bool_WMV m_bBFrameOn;   // global flag to indicate B frame coding mode
    Bool_WMV m_bX9LongLoopFilter;




    // overlapped transform stuff
    Bool_WMV   m_bSequenceOverlap;
    I32_WMV m_iOverlap;
    I16_WMV  *m_pIntraBlockRowData, *m_pIntraBlockRow0[3], *m_pIntraMBRowU0[2], *m_pIntraMBRowV0[2];
    I16_WMV  *m_pIntraBlockRow1[4], *m_pIntraMBRowU1[3], *m_pIntraMBRowV1[3];


    Bool_WMV m_bPreProcRange;
    I32_WMV m_iRangeState;
    I32_WMV m_iReconRangeState;
#ifdef B_FRAME
	I32_WMV  m_iDirectCodingMode;
#endif

#ifdef _XFORMSWITCHING2_
        // 4x4 DCT subblock pattern
    Huffman_WMV m_huf4x4PatternHighQPDec;
    Huffman_WMV m_huf4x4PatternMidQPDec;
    Huffman_WMV m_huf4x4PatternLowQPDec;
    Huffman_WMV* m_pHuf4x4PatternDec;

    Huffman_WMV m_hufMBXformTypeLowQPDec;
    Huffman_WMV m_hufMBXformTypeMidQPDec;
    Huffman_WMV m_hufMBXformTypeHighQPDec;
    Huffman_WMV* m_pHufMBXformTypeDec;

    Huffman_WMV m_hufBlkXformTypeLowQPDec;
    Huffman_WMV m_hufBlkXformTypeMidQPDec;
    Huffman_WMV m_hufBlkXformTypeHighQPDec;
    Huffman_WMV* m_pHufBlkXformTypeDec;
#endif

#ifdef B_FRAME
	CMotionVector_X9* m_rgmv1;

	tYUV420Frame_WMV *m_pvopfRef1;
    CWMVMBMode* m_rgmbmd1;

    CVector_X9* m_rgmv_X9;
	CVector_X9* m_rgmvForwardPred;
	CVector_X9* m_rgmvBackwardPred;


    I32_WMV m_iBState;
   // I32_WMV m_iDroppedBFrame;
    I32_WMV m_iPrevIFrame;
    I32_WMV m_iDroppedPFrame;
#endif

#   ifdef _XFORMSWITCHING2_
        U8_WMV  *m_p4x4ZigzagScanOrder;
#   endif

#ifdef _XFORMSWITCHING2_
    U8_WMV* m_p4x4ZigzagInv;
    U8_WMV* m_p4x4ZigzagInvRotated;
#endif

       Void_WMV (*m_pFieldBlockSub128) ( I16_WMV *pBlock, I32_WMV iNumCoef);
    Void_WMV (*m_pFieldBlockAdd128) ( I16_WMV *pBlock, I32_WMV iNumCoef);
    Void_WMV (*m_pFieldAddError8x8) (I16_WMV *pSrc, U8_WMV *pDst, I32_WMV iDstOffset);
    Void_WMV (*m_pFieldAddError4x8) (I16_WMV *pSrc, U8_WMV *pDst, I32_WMV iDstOffset);

//       U8_WMV** m_rgiQuanCoef_IMBInPFrame_DCPredTable0;
//    U8_WMV** m_rgiQuanCoef_IMBInPFrame_DCPredTable1;

    Bool_WMV m_bPrevAnchor;
    Bool_WMV m_bIsBChangedToI;

#ifdef B_FRAME
//    I32_WMV  m_iBFrameReciprocal;
    I32_WMV  m_iBNumerator;
    I32_WMV  m_iBDenominator;
  //  MBType  m_mbtShortBSymbol, m_mbtLongBSymbol;
#endif // B_FRAME

        Void_WMV (*m_pInterpolate4ByteRow) (struct tagWMVDecInternalMember * pWMVDec,
        U8_WMV *pSrc, I32_WMV iSrcStride, U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iFilterType, I32_WMV iRndCtrl, I32_WMV iNumRows);

     Bool_WMV m_bOldRenderFromPostBuffer;

     tYUV420Frame_WMV* m_pvopcRenderThis; // render this frame

         Bool_WMV m_bSkipThisFrame;  // For B frames to handle seeking in the player

    Bool_WMV m_bHalfStep;
    Bool_WMV m_bUse3QPDZQuantizer;
    Bool_WMV m_bExplicitQuantizer;
    Bool_WMV m_bExplicitSeqQuantizer;
    Bool_WMV m_bExplicitFrameQuantizer;

	Bool_WMV m_bSeqFrameInterpolation, m_bInterpolateCurrentFrame;

#ifdef B_FRAME
    U8_WMV *m_pInterpY0Buffer;

    U8_WMV *m_pInterpY0;        // Aligned interpolation buffers
    U8_WMV *m_pInterpY1;
    U8_WMV *m_pInterpU0;
    U8_WMV *m_pInterpU1;
    U8_WMV *m_pInterpV0;
    U8_WMV *m_pInterpV1;
#endif

#ifdef B_FRAME_411
	I16_WMV *m_pFieldMvX_Pbuf, *m_pFieldMvY_Pbuf;
	I16_WMV *m_pFieldMvX_FPred, *m_pFieldMvY_FPred;
	I16_WMV *m_pFieldMvX_BPred, *m_pFieldMvY_BPred;
	I16_WMV *m_pB411InterpX, *m_pB411InterpY;
#endif


	Bool_WMV m_bRenderDirect;

	Void_WMV  (*m_pOverlapBlockVerticalEdge) (I16_WMV *pInOut, I32_WMV iStride);
	Void_WMV  (*m_pOverlapBlockHorizontalEdge) (I16_WMV *pSrcTop, I16_WMV *pSrcCurr,
											I32_WMV iSrcStride, U8_WMV *pDst, I32_WMV iDstStride,
											Bool_WMV bTop, Bool_WMV bCurrent, Bool_WMV bWindup);
    Void_WMV (*m_pPixelMean) (const U8_WMV *pSrc1, I32_WMV iStride1, const U8_WMV *pSrc2,
        I32_WMV iStride2, U8_WMV *pDst, I32_WMV iDstStride, I32_WMV iWidth, I32_WMV iHeight);

	Bool_WMV m_bPrevRefInPrevPost;
	Bool_WMV m_bFrameWasOutput;

	I32_WMV m_iNumFrameDropped;


    I32_WMV m_bSpriteMode;
    I32_WMV m_iNumMotionVectorSets;
    I32_WMV m_iSpriteWidthDsiplay    ;
    I32_WMV m_iSpriteHeightDisplay   ;
    I32_WMV m_iSpriteWidthDsiplayMB  ;
    I32_WMV m_iSpriteHeightDisplayMB ;

    I32_WMV m_iOldWidth  ;
    I32_WMV m_iOldHeight ;
    I32_WMV m_iNewWidth  ;
    I32_WMV m_iNewHeight ;
    I32_WMV m_uiNumSlices;
    const unsigned char *m_dibBitsY;
    const unsigned char *m_dibBitsU;
    const unsigned char *m_dibBitsV;
    unsigned char *m_pFrameY;
    unsigned char *m_pFrameU;
    unsigned char *m_pFrameV;
    I32_WMV m_iAffinePattern;


    // m_hAsyncProcess & m_hSyncEvent are only needed for the Asynchronous extension to the WMVideoDec INterface.
    // state for Asynchronous interface
    Handle_WMV m_hAsyncProcess;             // some sort of handle on the process that does the async decoding
    Handle_WMV m_hSyncEvent;                // holds a handle to an event to be called on each state transistion
    tWMVDecodeStatus m_tWMVDecodeStatus;    // Decode status from last async function

    I32_WMV m_iBetaRTMMismatchIndex;    // Used by V9 loopfilter for RTM/Beta mismatch.

        // Flags to indicate encoder used to generate V9 content
    Bool_WMV m_bBetaContent;
    Bool_WMV m_bRTMContent;

    //Bool_WMV bNotEndOfFrame;


	Bool_WMV m_bInterlaceV2;
        Bool_WMV m_bFieldMode;
    I32_WMV m_iCurrentField;

    I32_WMV m_iFieldDctIFrameCodingMode;

    I16_WMV* m_ppxliIntraRowY;
    I16_WMV* m_ppxliIntraRowU;
    I16_WMV* m_ppxliIntraRowV;


    I32_WMV m_iACPredIFrameCodingMode;


	 Void_WMV (*m_pInterpolateBlockRowBilinear) (struct tagWMVDecInternalMember * pWMVDec,
        U8_WMV *pSrc, I32_WMV iSrcStride, U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iFilterType, I32_WMV iRndCtrl, I32_WMV iNumRows);

    Void_WMV (*m_pInterpolate4ByteRowBilinear) (struct tagWMVDecInternalMember * pWMVDec,
        U8_WMV *pSrc, I32_WMV iSrcStride, U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iFilterType, I32_WMV iRndCtrl, I32_WMV iNumRows);

	Void_WMV (*m_pInterpolateBlockRow) (struct tagWMVDecInternalMember * pWMVDec,
        U8_WMV *pSrc, I32_WMV iSrcStride, U8_WMV *pDst, I32_WMV iDstStride,
        I32_WMV iXFrac, I32_WMV iYFrac, I32_WMV iFilterType, I32_WMV iRndCtrl, I32_WMV iNumRows);

	Bool_WMV m_bLuminanceWarpTop, m_bLuminanceWarpBottom;
	I32_WMV  m_iLuminanceScaleTop, m_iLuminanceShiftTop;
    I32_WMV  m_iLuminanceScaleBottom, m_iLuminanceShiftBottom;

    U8_WMV *m_pHorizontalZigzagScanOrder;
    U8_WMV *m_pVerticalZigzagScanOrder;

    Bool_WMV  m_bAdvancedProfile;

        Bool_WMV m_bBFrameDropped;

        Huffman_WMV *m_pInterlaceFrameMBMode;

    Huffman_WMV *m_pInterlaceFrame4MvMBModeTables [4];
    Huffman_WMV m_hufInterlaceFrame4MvMBMode0;
    Huffman_WMV m_hufInterlaceFrame4MvMBMode1;
    Huffman_WMV m_hufInterlaceFrame4MvMBMode2;
    Huffman_WMV m_hufInterlaceFrame4MvMBMode3;

    Huffman_WMV *m_pInterlaceFrame1MvMBModeTables [4];
    Huffman_WMV m_hufInterlaceFrame1MvMBMode0;
    Huffman_WMV m_hufInterlaceFrame1MvMBMode1;
    Huffman_WMV m_hufInterlaceFrame1MvMBMode2;
    Huffman_WMV m_hufInterlaceFrame1MvMBMode3;


    	// Field Picture
    Huffman_WMV* m_pHufMBMode;
    Huffman_WMV* m_pHufMBModeMixedMVTables[8];
    Huffman_WMV* m_pHufMBMode1MVTables[8];

    Huffman_WMV* m_p4MVBP;
    Huffman_WMV*m_p4MVBPTables[4];

    Huffman_WMV* m_p2MVBP;
    Huffman_WMV*m_p2MVBPTables[4];

    Huffman_WMV m_hufCBPCYDec_Interlace_1;
    Huffman_WMV m_hufCBPCYDec_Interlace_2;
    Huffman_WMV m_hufCBPCYDec_Interlace_3;
    Huffman_WMV m_hufCBPCYDec_Interlace_4;
    Huffman_WMV m_hufCBPCYDec_Interlace_5;
    Huffman_WMV m_hufCBPCYDec_Interlace_6;
    Huffman_WMV m_hufCBPCYDec_Interlace_7;
    Huffman_WMV m_hufCBPCYDec_Interlace_8;
    Huffman_WMV m_HufMVTable_Interlace_MV1;
    Huffman_WMV m_HufMVTable_Interlace_MV2;
    Huffman_WMV m_HufMVTable_Interlace_MV3;
    Huffman_WMV m_HufMVTable_Interlace_MV4;
    Huffman_WMV m_HufMVTable_Interlace_MV5;
    Huffman_WMV m_HufMVTable_Interlace_MV6;
    Huffman_WMV m_HufMVTable_Interlace_MV7;
    Huffman_WMV m_HufMVTable_Interlace_MV8;
    Huffman_WMV m_HufMVTable_Progressive_MV1;
    Huffman_WMV m_HufMVTable_Progressive_MV2;
    Huffman_WMV m_HufMVTable_Progressive_MV3;
    Huffman_WMV m_HufMVTable_Progressive_MV4;
    Huffman_WMV m_hufFieldPictureMBMode1MV1;
    Huffman_WMV m_hufFieldPictureMBMode1MV2;
    Huffman_WMV m_hufFieldPictureMBMode1MV3;
    Huffman_WMV m_hufFieldPictureMBMode1MV4;
    Huffman_WMV m_hufFieldPictureMBMode1MV5;
    Huffman_WMV m_hufFieldPictureMBMode1MV6;
    Huffman_WMV m_hufFieldPictureMBMode1MV7;
    Huffman_WMV m_hufFieldPictureMBMode1MV8;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV1;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV2;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV3;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV4;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV5;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV6;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV7;
    Huffman_WMV m_hufFieldPictureMBModeMixedMV8;
    Huffman_WMV m_hufFieldPictureMVBP1;
    Huffman_WMV m_hufFieldPictureMVBP2;
    Huffman_WMV m_hufFieldPictureMVBP3;
    Huffman_WMV m_hufFieldPictureMVBP4;
    Huffman_WMV m_hufInterlaceFrame2MVBP1;
    Huffman_WMV m_hufInterlaceFrame2MVBP2;
    Huffman_WMV m_hufInterlaceFrame2MVBP3;
    Huffman_WMV m_hufInterlaceFrame2MVBP4;


    Bool_WMV m_bCBEntropyCoding;

#ifdef WMVA_NEW_ENTROPY_CODING

#ifdef WMVB_ENTROPY_DUMP_DECODING_EVENTS
	FILE *fpNewEntropyDecoding;
	FILE *fp_MoreDecMsg;
	FILE *fpDecodedEventsDump;
	FILE *fpDequantNew;
#endif

	I32_WMV		m_imbX_NewErrorCoding;
	I32_WMV		m_imbY_NewErrorCoding;
	I32_WMV		m_iBlk_NewErrorCoding;
	I32_WMV		m_iSubBlk_NewErrorCoding;
    I32_WMV     m_iBlkXModeForHuf;
    I32_WMV     m_iYUVMode;	// for INTRA only
	I32_WMV		m_iHuffDecBuf[BLOCK_SQUARE_SIZE_TIMES2];
	I32_WMV		m_iHuffRunLevelBuf[BLOCK_SQUARE_SIZE];
	I32_WMV		m_iRunNoForHuff;
	I8_WMV*		m_cRunNoBuf;
	I8_WMV*		m_cNumOfZerosBuf;

	//-----------------
    // Inter BlkHd Info
    //-----------------

    // Total Coeffs and All Ones
    Huffman_WMV m_Huf_TotalCoeffsAndAllOnes[5][BLKHD_CONTEXTS];  //8x8, 8x4, 4x8, 4x4

    // T1 for short RunNo
    Huffman_WMV m_Huf_TrailingOnesForNotAllOnes[MAX_CODED_ALL_ONES_8x8 - 1]; // 5 modes, 4 ctxs, and RunNo = 2, 3, 4

    // T1 for long RunNo
    Huffman_WMV m_Huf_TrailingOnes[5][MAXZONENUM_T1];

    // Total Zeros
    Huffman_WMV m_Huf_TotalZeros[5][TOTALZEROS_CONTEXTS][MAXZONENUM_TOTALZEROS];

    // Level: nonone values
    Huffman_WMV m_Huf_NonOneLevels[MAX_LEVEL_ZONES];

    // Level: number of nonone levels
    Huffman_WMV m_Huf_NumNonOneLevels[5][LEVEL_CONTEXTS][ZONENUM_NUMNONONELEVELS];

    // Level: run of ones
    Huffman_WMV m_Huf_RunOfOnes[5][LEVEL_CONTEXTS][ZONENUM_RUNOFONES];

    //Run: NumNonZeroRuns
    Huffman_WMV m_Huf_NumNonZeroRuns[5][RUN_CONTEXTS][ZONES_NUMNONZERORUNS];

    //Run: NonZeroRuns
    Huffman_WMV m_Huf_NonZeroRuns[5][RUN_CONTEXTS][ZONES_NONZERORUNS];

    //Run: ZerosBefore
    Huffman_WMV m_Huf_ZerosBefore[5][RUN_CONTEXTS][ZONES_ZEROSBEFORE];

    //-----------------
    // INTRA BlkHd Info
    //-----------------

    // Total Coeffs and All Ones
    Huffman_WMV m_Huf_IntraTotalCoeffsAndAllOnes[INTRA_BLKHD_CONTEXTS];  // Y, UV

    // T1 for short RunNo
    //Huffman_WMV m_Huf_IntraTrailingOnesForNotAllOnes[MAX_CODED_ALL_ONES_INTRA_Y - 1];

    // T1 for long RunNo
    Huffman_WMV m_Huf_IntraTrailingOnes[MAXZONENUM_T1_INTRA];

    // Total Zeros
    Huffman_WMV m_Huf_IntraTotalZeros[MAXZONENUM_TOTALZEROS_INTRA];

    // Level: nonone values
    Huffman_WMV m_Huf_IntraNonOneLevels[INTRA_MAX_LEVEL_ZONES];

    // Level: number of nonone levels
    Huffman_WMV m_Huf_IntraNumNonOneLevels[ZONENUM_NUMNONONELEVELS_INTRA];
    // Level: run of ones
    Huffman_WMV m_Huf_IntraRunOfOnes[ZONENUM_RUNOFONES_INTRA];


    //Run: IntraNumNonZeroRuns
    Huffman_WMV m_Huf_IntraNumNonZeroRuns[INTRA_RUN_CONTEXTS][ZONES_NUMNONZERORUNS_INTRA];

    //Run: IntraNonZeroRuns
    Huffman_WMV m_Huf_IntraNonZeroRuns[INTRA_RUN_CONTEXTS][ZONES_NONZERORUNS_INTRA];

    // ZerosBefore
    Huffman_WMV m_Huf_IntraZerosBefore[INTRA_RUN_CONTEXTS][ZONES_ZEROSBEFORE_INTRA];

    Huffman_WMV m_Huf_Prog_TotalZeros[5][TOTALZEROS_CONTEXTS][MAXZONENUM_TOTALZEROS];
    Huffman_WMV m_Huf_Prog_IntraTotalZeros[MAXZONENUM_TOTALZEROS_INTRA];

#endif

        // Extension Header / GOP
    Bool_WMV m_bBroadcastFlags,  m_bTopFieldFirst, m_bRepeatFirstField;
    Bool_WMV m_bSkipFrame, m_bTemporalFrmCntr;



    Double_WMV m_dFrameRate;

    Bool_WMV m_bExtendedMvMode;

    Bool_WMV m_bPostProcInfoPresent;

    I32_WMV m_iPostProcLevel;

    U8_WMV m_pZigzagInvDCTHorzFlags [BLOCK_SQUARE_SIZE];

    I32_WMV m_iOverlapIMBCodingMode;

    I32_WMV     m_iNbrBlkRunNoContext;
    Bool_WMV    m_bISLOnly;

    videoCC_HANDLE m_videoCC;





    Bool_WMV m_bExtendedDeltaMvMode;
    I32_WMV  m_iDeltaMVRangeIndex;
    I32_WMV  m_iExtendedDMVX, m_iExtendedDMVY;

    Bool_WMV m_bProgressive420;
    //Bool_WMV m_bRefreshSequenceHeader;
    Bool_WMV m_bUseMostRecentFieldForRef;
    Bool_WMV m_bUseSameFieldForRef;
    Bool_WMV m_bUseOppFieldForRef;
    Bool_WMV m_bTwoRefPictures;

    I32_WMV  m_iCurrentTemporalField;
    I32_WMV m_iPrevSpatialField;


    Huffman_WMV* m_pHufICBPCYDec;
    Huffman_WMV* m_pHufPCBPCYDec;
    Huffman_WMV* m_pHufInterlaceCBPCYTables[8];



    I32_WMV     m_iTopNbrMBIdx;
    I32_WMV     m_iTopNbrBlkIdx;
    Bool_WMV    m_bTopNbrMode;

    I32_WMV     m_iLeftNbrMBIdx;
    I32_WMV     m_iLeftNbrBlkIdx;
    Bool_WMV    m_bLeftNbrMode;

    tFrameType_WMV m_PredTypePrev;

    tFrameType_WMV m_FirstFieldType;
    tFrameType_WMV m_SecondFieldType;
    I32_WMV m_iRefFrameDistance;        // Used for P fields and forward B
    I32_WMV m_iBackRefFrameDistance;    // Used for backward B
    I32_WMV m_iMaxZone1ScaledFarMVX;
    I32_WMV m_iMaxZone1ScaledFarMVY;
    I32_WMV m_iZone1OffsetScaledFarMVX;
    I32_WMV m_iZone1OffsetScaledFarMVY;
    I32_WMV m_iFarFieldScale1;
    I32_WMV m_iFarFieldScale2;
    I32_WMV m_iNearFieldScale;
    I32_WMV m_iMaxZone1ScaledFarBackMVX;
    I32_WMV m_iMaxZone1ScaledFarBackMVY;
    I32_WMV m_iZone1OffsetScaledFarBackMVX;
    I32_WMV m_iZone1OffsetScaledFarBackMVY;
    I32_WMV m_iFarFieldScaleBack1;
    I32_WMV m_iFarFieldScaleBack2;
    I32_WMV m_iNearFieldScaleBack;

    I32_WMV     m_iTopNbrXformMode;
    I32_WMV     m_iLeftNbrXformMode;

    I32_WMV     m_iTopNbrEdgeMask;
    I32_WMV     m_iLeftNbrEdgeMask;




    Bool_WMV m_bInterlacedSource;
    I32_WMV m_iRepeatFrameCount;



    //variables for re-init sequence header
    I32_WMV m_iPrevSeqFrmWidthSrc, m_iPrevSeqFrmHeightSrc, m_iMaxPrevSeqFrameArea;
    Bool_WMV m_bLargerSeqBufferRequired, m_bSeqFrameWidthOrHeightChanged, m_bPrevSeqCBEntropyCoding;
    I32_WMV m_iMaxPrevFrmWidthSrc, m_iMaxPrevFrmHeightSrc;


    I32_WMV m_iChromaFormat;


    Bool_WMV m_bHRD_PARAM_FLAG;
    I32_WMV  m_ihrd_num_leaky_buckets;
    Bool_WMV m_bRangeRedY_Flag, m_bRangeRedUV_Flag;
    I32_WMV m_iRangeRedY, m_iRangeRedUV;

    Bool_WMV m_bEnableCBEntropyCoding;

    I32_WMV m_iLevel;
    I32_WMV m_iMaxPrevA2LCFrameArea;

    Bool_WMV m_bSliceWMVA;
    U32_WMV *m_puiFirstMBRowOfSlice, m_uiNumSlicesWMVA;
    U32_WMV m_uiNumSlicesField1WMVA;
    U32_WMV *m_puiNumBytesOfSliceWMVA, m_uiNumBytesFrameHeaderSlice, m_uiNumBytesFieldHeaderSlice;
    Bool_WMV *m_pbStartOfSliceRow, *m_pbStartOfSliceRowOrig;
    U32_WMV m_uiCurrSlice;
    I32_WMV m_iSliceBufSize;


    I32_WMV m_iFrameNumber;
    Bool_WMV m_bParseStartCodeFrameOnly;
    I32_WMV m_iParseStartCodeLastType;
    I32_WMV m_iParseStartCodeRemLen;
    I32_WMV m_iBufferedUserDataLen;
    U8_WMV * m_pBufferedUserData;

    // members associated with StartCode Callback
    Void_WMV * m_pCallBackApp;          // Opaque pointer passed back to the App in the callback
    U32_WMV m_u32SCEnableMask;

    Bool_WMV m_bResizeDimensions;

    Bool_WMV m_bDelaySetSliceWMVA;
    I32_WMV m_iDelaySetSliceWMVALen;
    U8_WMV m_cDelaySetSliceWMVATmp;
    Bool_WMV m_bParseStartCodeSecondField;

    U32_WMV m_u32StartCodeParseAltBitBufferLen;
    U8_WMV * m_pStartCodeParseAltBitBuffer;
#ifdef DUMPLOG
    FILE * m_pFileLog;
    FILE * m_pFileLog2;
#endif

    I32_WMV m_iMaxFrmWidthSrc, m_iMaxFrmHeightSrc; // Maximum coded frame size for sequence


    Bool_WMV m_bSwitchingEntryPoint;

    Bool_WMV m_bBrokenLink, m_bClosedEntryPoint, m_bPanScanPresent, m_bRefDistPresent;

    I32_WMV m_iFrmWidthSrcOld, m_iFrmHeightSrcOld;

    Bool_WMV m_bRenderDirectWithPrerender;

    Bool_WMV m_bGetOutputLastFlush;
    COVERAGE_ONLY( tCoverage m_tCoverage; )

    Bool_WMV m_bDiscontinuity;
    WMVFrameType m_iFrameType;

        Bool_WMV m_bBackRefUsedHalfPel;
    Bool_WMV m_bBackRefTopFieldHalfPelMode;
    Bool_WMV m_bBackRefBottomFieldHalfPelMode;




    Bool_WMV m_bPostCrescent;

    I32_WMV m_iWidthYOld, m_iHeightYOld, m_iWidthUVOld, m_iHeightUVOld;
    I32_WMV m_iWidthYOrig, m_iHeightYOrig, m_iWidthUVOrig, m_iHeightUVOrig;


    WMVFrameType m_iCurrAnchorFrameType;
    WMVFrameType m_iPrevAnchorFrameType;

    I32_WMV m_iQuantiserSpecifier;

    I32_WMV   m_iBackMVRangeIndex, m_iBackMVRangeIndexTop, m_iBackMVRangeIndexBottom;


    Bool_WMV m_bPostRC1;

        Bool_WMV m_bPsF;


} tWMVDecInternalMember;
#include "dectable_c.h"


#endif // __WMVDEC_MEMBER_H_
