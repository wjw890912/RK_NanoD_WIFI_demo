#ifndef APE_GLOBALVAR_DECLARATION_H
#define APE_GLOBALVAR_DECLARATION_H

#include "ape_decompress.h"
#include "ape_unbitarray.h"
#include "ape_unmac.h"
#include "ape_newpredictor.h"
#include "ape_rollbuffer.h"

// 使用nano table room里的表格
#define APE_USE_TABLE_ROOM
#define APE_CHECK_TABLE_ROOM


extern ape_char Ape_gReadBuffer[512];

/////Macro control////////////////////
#define DECODE_BLOCK_SIZE        0//4096  //this macro control extended blocks of outbuffer,one block includes 16bits R,L samples.
#define BLOCKS_PER_DECODE_CUSTOM_HANDLE 1152 //means that outbuffer blocks is 1152.
#define BLOCKALIGN               2  //align by int16
#define BLOCKS_PER_DECODE               1152//the macro represents length of block which each is decoded and written to wav file.
//#define NORDER                   1280
#define NORDER                   64//Mod By Vincent Hsiung, Feb 16th,2008//256 //16 is Fast or Normal,64 is High,256 is ExtHigh,1280 is Insane

//the above two macros decide that length of outbuffer is (2304+4096)*4+1+256 bytes.

#define BLOCKS_PER_BITSTREAMREAD  2048
//1024 //the macro represents that length of inbuffer is (1080+16)*4 bytes.
//修改输入BUFFER大小 BY VINCENT HISUNG , DEC 17,2007 FOR TEST
//#define BLOCKS_PER_BITSTREAMREAD  4096*4//1024 //the macro represents that length of inbuffer is (1080+16)*4 bytes.

////////allocate Global Space//////////////////////////////
//extern ape_uint16 ape_dec_buffer[(BLOCKS_PER_DECODE_CUSTOM_HANDLE+DECODE_BLOCK_SIZE)* BLOCKALIGN+ 1 + BLOCKALIGN * 64];//allocate decode_shift circle buffer
//extern ape_uint16  Ape_gOutBuffer[2][BLOCKS_PER_DECODE * BLOCKALIGN];//allocate ape out buffer for codec
extern ape_uint32  Ape_gOutBufferOffset0;
extern ape_uint32  Ape_gOutBufferOffset1;
extern ape_uint16  Ape_gDualBufferIndex;
extern ape_uint16* Ape_pOutBufferLeft;//该指针指向tAPE结构体的psLeft空间
extern ape_uint16* Ape_pOutBufferRight;//该指针指向tAPE结构体的psRight空间
//extern ape_uint32  Ape_gInBuffer[BLOCKS_PER_BITSTREAMREAD + 16];//allocate ape input buffer
extern ape_uint32* Ape_pInBuffer;//该指针指向tAPE结构体里的pcEncodedData空间
extern ape_int16   Ape_gBeginDecodeFrameFlag;//added by hxd 20070317
extern ape_int16   Ape_gForwardBackIndex;//added by hxd 20070428
extern ape_int16   Ape_gForwardBackFlag;//20070428
extern ape_uint32  Ape_gForwardBackOffset;//20070428
extern ape_uint16  Ape_gDecodeErrorFlag;//20070703
extern ape_int32   Ape_gTotalBlocks;//20070516
extern ape_int32   Ape_gBlocksLeft;//表示未解码的blocks数目
extern ape_int32   Ape_gYValue;//20071124
extern ape_int32   Ape_gnYValue;//20071126

//---global variable in Maclib.c--------------------------------------
//extern ape_int16 Ape_gIApeDecompressMalloc[(sizeof(CAPEDecompress)+1)/2];//sizeof(CAPEDecompress)
extern CAPEDecompress Ape_gIApeDecompressMalloc;//[11];//sizeof(CAPEDecompress)

//---global variable in APEDecompress.c--------------------------------------
extern ape_int32  Ape_gBlockAlign;
extern ape_int32  Ape_gCurrentFrame;

extern ape_int32  Ape_gStartBlock;
extern ape_int32  Ape_gFinishBlock;
extern ape_int32  Ape_gCurrentBlock;
extern ape_BOOL   Ape_gIsRanged;
extern ape_BOOL   Ape_gDecompressorInitial;

// static variable used during decoding
extern CPrepare Ape_gPrepare;
extern WAVEFORMATEX Ape_gWfeInput;
extern ape_uint32  Ape_gCRC;
extern ape_uint32  Ape_gStoredCRC;
extern ape_int32   Ape_gSpecialCodes;
extern CAPEInfo*   Ape_pApeInfo;
extern CUnBitArrayBase* Ape_pUnBitArray;
extern struct UNBIT_ARRAY_STATE Ape_gBitArrayStateX;
extern struct UNBIT_ARRAY_STATE Ape_gBitArrayStateY;

extern IPredictorDecompress* Ape_pNewPredictorX;
extern IPredictorDecompress* Ape_pNewPredictorY;

extern ape_int32 Ape_gLastXValue;

extern ape_BOOL Ape_gErrorDecodingCurrentFrame;
extern ape_int32 Ape_gCurrentFrameBufferBlock;
extern ape_int32 Ape_gFrameBufferFinishedBlocks;
extern CCircleBuffer Ape_gCbFrameBuffer;
extern CPredictorDecompress3950toCurrent Ape_gPredDecomMallocX;
extern CPredictorDecompress3950toCurrent Ape_gPredDecomMallocY;
extern ape_int16 Ape_gBytesToSkipMalloc[16];//need to verify the space
//-----------------------------------------

/////the following global variables are used in UnBitArray.c/////////////////////////////////
//add by Wei.Hisung 2007.03.06
extern ape_uint32 Ape_gElements;
extern ape_uint32 Ape_gBytesNum;
extern ape_uint32 Ape_gBitsNum;
extern ape_int32 Ape_gVersion;
extern CIO * Ape_pIO;
extern ape_uint32 Ape_gCurrentBitIndex;
extern ape_uint32 * Ape_pBitArray;
//----------------------------------
extern ape_int32 Ape_gFlushCounter;
extern ape_int32 Ape_gFinalizeCounter;
//extern struct RANGE_CODER_STRUCT_DECOMPRESS    Ape_gRangeCoderInfo;
extern ape_uint32 Ape_gBufferValue;
extern ape_uint32 Ape_gLowValue;
extern ape_uint32 Ape_gRangeValue;
extern ape_uint32 Ape_gRefillBitThreshold;
//-----------------------------------

#ifdef APE_USE_TABLE_ROOM
extern ape_uint32 *Ape_gtPowersOfTwoMinusOne;
extern ape_uint32 *Ape_gtKSumMinBoundary;
extern ape_uint32 *Ape_gtRangeTotalOne;
extern ape_uint32 *Ape_gtRangeWidthOne;
extern ape_uint32 *Ape_gtRangeTotalTwo;
extern ape_uint32 *Ape_gtRangeWidthTwo;
#else
extern ape_uint32 Ape_gtPowersOfTwoMinusOne[33];
//extern ape_uint32 Ape_gtPowersOfTwoMinusOneReversed[33];

extern ape_uint32 Ape_gtKSumMinBoundary[32];

extern ape_uint32 Ape_gtRangeTotalOne[65];
extern ape_uint32 Ape_gtRangeWidthOne[64];

extern ape_uint32 Ape_gtRangeTotalTwo[65];
extern ape_uint32 Ape_gtRangeWidthTwo[64];
#endif


/////the following global variables are used in UnBitArrayBase.c/////////////////////////////////
//static const ape_uint32 Ape_gtPowersOfTwoMinusOne[33] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,4294967295};
//add by Wei.Hisung 2007.03.06
extern ape_uint32 Ape_gElementsBase;
extern ape_uint32 Ape_gBytesValueBase;
extern ape_uint32 Ape_gBitsValueBase;
extern ape_int32  Ape_gVersionBase;
extern CIO * Ape_pIOBase;
extern ape_uint32 Ape_gCurrentBitIndexBase;
extern ape_uint32* Ape_pBitArrayBase;

//extern ape_int16 Ape_gUnBitArrayBaseMalloc[sizeof(CUnBitArray)/2];//(CUnBitArrayBase * )malloc(sizeof(CUnBitArray))or(CUnBitArrayBase * )malloc(sizeof(CUnBitArrayOld))
extern CUnBitArray Ape_gUnBitArrayBaseMalloc;//[12];//(CUnBitArrayBase * )malloc(sizeof(CUnBitArray))or(CUnBitArrayBase * )malloc(sizeof(CUnBitArrayOld))
//----------------------------------

/////the following global variables are used in Prepare.c/////////////////////////////////
#ifdef APE_USE_TABLE_ROOM
extern ape_uint32 *Ape_gtCRC32;
#else
extern ape_uint32 Ape_gtCRC32[256];
#endif

/////the following global variables are used in APEDecompressOld.c/////////////////////////////////
//----------------------------------
extern ape_char* Ape_pBuffer;
extern ape_int32 Ape_gBufferTail;
extern ape_int32 Ape_gBlockAlignOld;
extern ape_int32 Ape_gCurrentFrameOld;
extern ape_int32 Ape_gStartBlockOld;
extern ape_int32 Ape_gFinishBlockOld;
extern ape_int32 Ape_gCurrentBlockOld;
extern ape_BOOL  Ape_gIsRangedOld;
extern CUnMAC Ape_gUnMAC;
extern CAPEInfo* Ape_pAPEInfoOld;
extern ape_BOOL Ape_gDecompressorInitialOld;

/////the following global variables are used in UnBitArrayOld.c/////////////////////////////////
//add by Wei.Hisung 2007.03.06
extern ape_uint32 Ape_gElementsOld;
extern ape_uint32 Ape_gBytesValueOld;
extern ape_uint32 Ape_gBitsValueOld;
extern ape_int32  Ape_gVersionOld;
extern CIO * Ape_pIOOld;
extern ape_uint32 Ape_gCurrentBitIndexOld;
extern ape_uint32* Ape_pBitArrayOld;
//----------------------------------
extern ape_uint32 Ape_gKBitArrayOld;
extern ape_uint32 Ape_gKSum;
extern ape_uint32 Ape_gRefillBitThresholdOld;
//----------------------------------

//const ape_uint32 Ape_gtPowersOfTwoMinusOne[33] = {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535,131071,262143,524287,1048575,2097151,4194303,8388607,16777215,33554431,67108863,134217727,268435455,536870911,1073741823,2147483647,4294967295};

extern ape_uint32 Ape_gtKSumMinBoundaryOld[32];
extern ape_uint32 Ape_gtKSumMaxBoundaryOld[32];
extern ape_uint32 Ape_gtPowersOfTwo[32];
extern ape_uint32 Ape_gtPowersOfTwoReversed[32];
//extern ape_uint32 Ape_gtPowersOfTwoMinusOne[33];
extern ape_uint32 Ape_gtPowersOfTwoMinusOneReversed[33];

//const ape_uint32 Ape_gtKSumMinBoundary[32] = {0,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432,67108864,134217728,268435456,536870912,1073741824,2147483648,0,0,0,0};
extern ape_uint32 Ape_gtKSumMaxBoundary[32];

/////the following global variables are used in RollBuffer.c/////////////////////////////////
extern ape_int32 Ape_gPredABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
extern ape_int32 Ape_gPredBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
extern ape_int32 Ape_gAdaptABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
extern ape_int32 Ape_gAdaptBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];

//extern ape_int32 Ape_gDeltaMCreateMalloc[3][2][(NN_WINDOW_ELEMENTS + NORDER)];
//extern ape_int32 Ape_gInputCreateMalloc[3][2][(NN_WINDOW_ELEMENTS + NORDER)];
extern ape_int32 Ape_gDeltaMCreateMalloc[1][2][(NN_WINDOW_ELEMENTS + NORDER)];
extern ape_int32 Ape_gInputCreateMalloc[1][2][(NN_WINDOW_ELEMENTS + NORDER)];


/////the following global variables are used in NNFilter.c/////////////////////////////////
//Mod by Vincent Hsiung, Feb 16th, 2008
extern ape_int32 Ape_gParyMMalloc[1/*3*/][2][NORDER];//malloc(sizeof(short)*(aNNF->m_nOrder));//assume aNNF->m_nOrder is 16,but must test aNNF->m_nOrder using other ape file.


/////the following global variables are used in NewPredictor.c/////////////////////////////////
//extern ape_int32 m_pBuffer_malloc[2][HISTORY_ELEMENTS + WINDOW_BLOCKS];
extern CNNFilter Ape_gCNNFilterMalloc[2];//[36];//malloc(sizeof( CNNFilter));
extern CNNFilter Ape_gCNNFilter1Malloc[2];//[36];//malloc(sizeof( CNNFilter));
extern CNNFilter Ape_gCNNFilter2Malloc[2];//[36];//malloc(sizeof( CNNFilter));


//----------maclib.c----ApeDecompressCreate()-----------------
extern CAPEInfo Ape_gDecompressInfoCreate;
//----------APEHeader.c---ApeHeaderAnalyzeCurrent()----------
extern struct APE_DESCRIPTOR Ape_gHeaderDescriptor;
#define SEEKBYTE_TABLE_MAX 512//0x2000//4426//512
#define WAVE_HEADER_MAX    50
extern ape_uint32 Ape_gHeaderSeekByteTableOffset;
extern ape_uint32 Ape_gHeaderSeekByteTable[SEEKBYTE_TABLE_MAX];
extern ape_uchar Ape_gHeaderSeekBitTable[SEEKBYTE_TABLE_MAX];
extern ape_uchar Ape_gWavHeaderData[WAVE_HEADER_MAX];
//----------APEInfo.c-----ApeInfoFile()------
extern CIO Ape_gInfoIO;
extern CAPETag Ape_gInfoApeTag;
//----------APETag.c-----ApeTagAnalyze()------
//extern ape_char CAPETag_RawTag[512];  -->Ape_gReadBuffer
//----------APETag.c-----ApeTagLoadField()----
#define APETAG_FIELDS_NUM_MAX    64
//extern ape_char Ape_pApeTagAryFields[APETAG_FIELDS_NUM_MAX][32];
extern ape_char* Ape_pApeTagAryFields;
//----------APETag.c-----ApeTagFileInitialize()------
extern CIO Ape_gApeTagIO;
extern struct ID3_TAG Ape_gId3Tag;
///////////////////the following global variable in APESimple.c/////////
extern CIO* Ape_pIOOutput;
extern IAPEDecompress* Ape_pApeDecompress;
extern CIO Ape_gCIODecompressCore; //for write file

extern ape_int16 Ape_gTitleIndex,Ape_gArtistIndex,Ape_gAlbumIndex;
#endif // #ifndef

