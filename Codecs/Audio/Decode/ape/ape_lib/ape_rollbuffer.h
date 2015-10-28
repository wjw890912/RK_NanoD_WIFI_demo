#ifndef APE_ROLLBUFFER_H
#define APE_ROLLBUFFER_H
#include "ape_all.h"

#define WINDOW_ELEMENTS 256//512

#define HISTORY_ELEMENTS 8
#ifdef __cplusplus
 extern "C"{
#endif
typedef struct
{
  void      (*cCRollBuffer)(void *);
  ape_int32 (*Create)(void *,ape_int32 nWindowElements, ape_int32 nHistoryElements,ape_int16 xypredictorflag,ape_int16 filtetyperflag);
  void      (*Flush)(void *);
  void      (*Roll)(void *);
  void      (*IncrementSafe)(void *);
  void      (*IncrementFast)(void *);
  ape_int32* m_pData;//ape_int16* m_pData;
  ape_int32* m_pCurrent;//ape_int16* m_pCurrent;
  ape_int32  m_nHistoryElements;
  ape_int32  m_nWindowElements;
}CRollBuffer;
 void      ApeRollBufferInitialize(void *);
 //void ApeRollBufferDelete(void *);
 ape_int32 ApeRollBufferCreate_rbInput(void *,ape_int32 nWindowElements, ape_int32 nHistoryElements,ape_int16 xyflag,ape_int16 filterflag);
 ape_int32 ApeRollBufferCreate_rbDeltaM(void *,ape_int32 nWindowElements, ape_int32 nHistoryElements,ape_int16 xyflag,ape_int16 filterflag);
 void      ApeRollBufferFlush(void *);
 void      ApeRollBufferRoll(void *);
 void      ApeRollBufferIncrementSafe(void *);
 void      ApeRollBufferIncrementFast(void *);

typedef struct
{
  void      (*cCRollBufferFast)(void *,ape_int16 xyflag);
  //void (*dCRollBufferFast)(void *);
  void      (*Flush)(void *);
  void      (*Roll)(void *);
  void      (*IncrementSafe)(void *);
  void      (*IncrementFast)(void *);
  ape_int32* m_pData;
  ape_int32* m_pCurrent;
}CRollBufferFast;

 void ApeRollBufferFastPredictionAInitialize(void *aI,ape_int16 xyflag);
 void ApeRollBufferFastPredictionBInitialize(void *aI,ape_int16 xyflag);
 void ApeRollBufferFastAdaptAInitialize(void *aI,ape_int16 xyflag);
 void ApeRollBufferFastAdaptBInitialize(void *aI,ape_int16 xyflag);
 //void ApeRollBufferFastDelete(void *aI);
 void ApeRollBufferFastFlush(void *aI);
 void ApeRollBufferFastRoll(void *aI);
 void ApeRollBufferFastIncrementSafe(void *aI);
 void ApeRollBufferFastIncrementFast(void *aI);
#ifdef __cplusplus
 }
#endif
#endif // #ifndef APE_ROLLBUFFER_H
