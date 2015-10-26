#ifndef APE_NNFILTER_H
#define APE_NNFILTER_H

#include "ape_rollbuffer.h"
//Mod by Vincent Hsiung , Feb 17th,2008
#define NN_WINDOW_ELEMENTS    128//256//512

typedef struct
{
  void      (*cCNNFilter)(void *, ape_int32 , ape_int32, ape_int32 ,ape_int16,ape_int16);
  //void (*dCNNFilter)(void*);
  ape_int32 (*Decompress)(void *, ape_int32 nInput);
  void      (*Flush)(void * );
  ape_int16 (*GetSaturatedShortFromInt)(void *, ape_int32 nValue) ;
  ape_int32 (*CalculateDotProductNoMMX)(void *, ape_int16 * pA, ape_int16 * pB, ape_int32 nOrder);
  void      (*AdaptNoMMX)(void *, ape_int16 * pM, ape_int16 * pAdapt, ape_int32 nDirection, ape_int32 nOrder);
  ape_int32 m_nOrder;
  ape_int32 m_nShift;
  ape_int32 Ape_gVersion;
  ape_BOOL  m_bMMXAvailable;
  ape_int32 m_nRunningAverage;
  CRollBuffer m_rbInput;
  CRollBuffer m_rbDeltaM;
  ape_int32 * m_paryM;//ÏµÊý
}CNNFilter;
#ifdef __cplusplus
extern "C" {
#endif
  void      ApeCNNFilterInitialize(CNNFilter *aNNF, ape_int32 nOrder, ape_int32 nShift, ape_int32 nVersion,ape_int16 xyflag,ape_int16 filterflag);
  //void ApeCNNFilterDelete(CNNFilter *aNNF);
  ape_int32 ApeCNNFilterDecompress(CNNFilter *aNNF, ape_int32 nInput);
  void      ApeCNNFilterFlush(CNNFilter  *aNNF);
  ape_int16 ApeCNNFilterGetSaturateShortFromInt(CNNFilter *aNNF, ape_int32 nValue);
  ape_int32 ApeCNNFilterCalculateDotProduct(CNNFilter *aNNF, ape_int16 * pA, ape_int16 * pB, ape_int32 nOrder);
  void      ApeCNNFilterAdapt(CNNFilter *aNNF, ape_int16 * pM, ape_int16 * pAdapt, ape_int32 nDirection, ape_int32 nOrder);
#ifdef __cplusplus
}
#endif
#endif // #ifndef APE_NNFILTER_H
