#ifndef APE_GLOBALVAR_DEFINE2_H
#define APE_GLOBALVAR_DEFINE2_H

#include "ape_all.h"
#include "ape_globalvardeclaration.h"
#include "ape_decompress.h"
#include "ape_unbitarray.h"
#include "ape_unmac.h"
#include "ape_newpredictor.h"
#include "ape_rollbuffer.h"

#if 0
ape_uint32 Ape_gtPowersOfTwoMinusOne[33];
ape_uint32 Ape_gtKSumMinBoundary[32];
ape_uint32 Ape_gtRangeTotalOne[65];
ape_uint32 Ape_gtRangeWidthOne[64];
ape_uint32 Ape_gtRangeTotalTwo[65];
ape_uint32 Ape_gtRangeWidthTwo[64];
#endif

//ape_char    Ape_gReadBuffer[512];
ape_int32 Ape_gParyMMalloc[1/*3*/][2][NORDER];
ape_int32 Ape_gDeltaMCreateMalloc[1/*3*/][2][(NN_WINDOW_ELEMENTS + NORDER)];
ape_int32 Ape_gInputCreateMalloc[1/*3*/][2][(NN_WINDOW_ELEMENTS + NORDER)];

//CNNFilter Ape_gCNNFilterMalloc[2];
//IPredictorDecompress* Ape_pNewPredictorX;//定义X预测器指针
//IPredictorDecompress* Ape_pNewPredictorY;//定义Y预测器指针
//CPredictorDecompress3950toCurrent Ape_gPredDecomMallocX;//开辟X预测器结构体
//CPredictorDecompress3950toCurrent Ape_gPredDecomMallocY;//开辟Y预测器结构体
//ape_int16 Ape_gBytesToSkipMalloc[16];//need to verify the space


#if 0
ape_char    Ape_gReadBuffer[512];

IPredictorDecompress* Ape_pNewPredictorX;//定义X预测器指针
IPredictorDecompress* Ape_pNewPredictorY;//定义Y预测器指针

CPredictorDecompress3950toCurrent Ape_gPredDecomMallocX;//开辟X预测器结构体
CPredictorDecompress3950toCurrent Ape_gPredDecomMallocY;//开辟Y预测器结构体
ape_int16 Ape_gBytesToSkipMalloc[16];//need to verify the space

/////the following global variables are used in RollBuffer.c/////////////////////////////////
ape_int32 Ape_gPredABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
ape_int32 Ape_gPredBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
ape_int32 Ape_gAdaptABufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];
ape_int32 Ape_gAdaptBBufferFastMalloc[2][(WINDOW_ELEMENTS + HISTORY_ELEMENTS)];


/////the following global variables are used in NNFilter.c/////////////////////////////////
ape_int32 Ape_gParyMMalloc[3][2][NORDER];//malloc(sizeof(short)*(aNNF->m_nOrder));//assume aNNF->m_nOrder is 16,but must test aNNF->m_nOrder using other ape file.

/////the following global variables are used in NewPredictor.c/////////////////////////////////
CNNFilter Ape_gCNNFilterMalloc[2];//[36];//malloc(sizeof( CNNFilter));
CNNFilter Ape_gCNNFilter1Malloc[2];//[36];//malloc(sizeof( CNNFilter));
CNNFilter Ape_gCNNFilter2Malloc[2];//[36];//malloc(sizeof( CNNFilter));
#endif
#if 0
//--------------------------------------------------------------------------------
struct UNBIT_ARRAY_STATE Ape_gBitArrayStateX;//开辟X熵编码结构体
struct UNBIT_ARRAY_STATE Ape_gBitArrayStateY;//开辟Y熵编码结构体
//--------------------------------------------------------------------------------
#endif
#endif // #ifndef APE_GLOBALVAR_DEFINE_H
