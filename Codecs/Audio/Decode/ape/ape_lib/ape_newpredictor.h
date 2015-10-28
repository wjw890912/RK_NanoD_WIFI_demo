#ifndef APE_NEWPREDICTOR_H
#define APE_NEWPREDICTOR_H

#include "ape_predictor.h"

#include "ape_rollbuffer.h"
#include "ape_nnfilter.h"
#include "ape_scaledfirstorderfilter.h"

/*************************************************************************************************
Functions to create the interfaces
*************************************************************************************************/
/*Mod by Wei.Hisung 2007.03.05*/
IPredictorDecompress *  CreateIPredictorDecompress(void);

#define WINDOW_BLOCKS           256//512

#define BUFFER_COUNT            1
#define HISTORY_ELEMENTS        8
#define M_COUNT                 8


typedef struct
{
  void      (*cCPredictorDecompressNormal3930to3950)(void *,ape_int32 nCompressionLevel, ape_int32 nVersion);
  void      (*dCPredictorDecompressNormal3930to3950)(void *);
  ape_int32 (*DecompressValue)(void *,ape_int32 nInput, ape_int32);
  ape_int32 (*Flush)(void *);
  ape_int32 * m_pBuffer[BUFFER_COUNT];
  ape_int32 m_aryM[M_COUNT];
  ape_int32 * m_pInputBuffer;
  ape_int32 m_nCurrentIndex;
  ape_int32 m_nLastValue;
  CNNFilter * m_pNNFilter;
  CNNFilter * m_pNNFilter1;
}CPredictorDecompressNormal3930to3950;
#ifdef __cplusplus
extern "C" {
#endif
  ape_int32 ApeDecompressPredictor3930To3950(CPredictorDecompressNormal3930to3950 *,ape_int32 nCompressionLevel, ape_int32 nVersion,ape_int16 flag);
  //void ApePredictorDecompress3930To3950Delete(CPredictorDecompressNormal3930to3950 *);
  ape_int32 ApePredictorDecompressValue3930To3950(CPredictorDecompressNormal3930to3950 *,ape_int32 nInput, ape_int32);
  ape_int32 ApePredictorFlush3930To3950(CPredictorDecompressNormal3930to3950 *);
#ifdef __cplusplus
}
#endif


//class CPredictorDecompress3950toCurrent : public IPredictorDecompress
typedef struct
{
  void       (*cCPredictorDecompress3950toCurrent)(void *,ape_int32 nCompressionLevel, ape_int32 nVersion,ape_int16 xyflag);
  void       (*dCPredictorDecompress3950toCurrent)(void *);
  ape_int32  (*DecompressValue)(void *,ape_int32 nA, ape_int32 nB );
  ape_int32  (*Flush)(void *);
  ape_int32  m_aryMA[M_COUNT];
  ape_int32  m_aryMB[M_COUNT];
  CRollBufferFast m_rbPredictionA;
  CRollBufferFast m_rbPredictionB;
  CRollBufferFast m_rbAdaptA;
  CRollBufferFast m_rbAdaptB;
  CScaledFirstOrderFilter m_Stage1FilterA;
  CScaledFirstOrderFilter m_Stage1FilterB;
  ape_int32 m_nCurrentIndex;
  ape_int32 m_nLastValueA;
  ape_int32 Ape_gVersion;
  CNNFilter * m_pNNFilter;
  CNNFilter * m_pNNFilter1;
  CNNFilter * m_pNNFilter2;
}CPredictorDecompress3950toCurrent;
#ifdef __cplusplus
extern "C" {
#endif
  ape_int32 ApePredictorDecompress3950ToCurrent(void *,ape_int32 nCompressionLevel, ape_int32 nVersion, ape_int16 flag);
  //void ApePredictorDecompress3950ToCurrentDelete(void *);
  ape_int32 ApePredictorDecompressValue3950ToCurrent(void *,ape_int32 nA, ape_int32 nB);
  ape_int32  ApePredictorDecompressValue3950ToCurrentMergeXAndY(ape_int32 nY,ape_int32 llastxvalue,ape_int32 nX);
  ape_int32 ApePredictorFlush3950ToCurrent(void *);
#ifdef __cplusplus
}
#endif
#endif // #ifndef APE_NEWPREDICTOR_H
