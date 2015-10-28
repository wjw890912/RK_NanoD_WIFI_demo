#ifndef APE_SCALEDFIRSTORDERFILTER_H
#define APE_SCALEDFIRSTORDERFILTER_H
#include "ape_all.h"


typedef struct
{
  ape_int32 MULTIPLY;
  ape_int32 SHIFT;
  ape_int32 m_nLastValue;
  void      (*SetMS)(void* ,ape_int32 ,ape_int32 );
  void      (*Flush)(void *);
  ape_int32 (*Compress)(void *, ape_int32 );
  ape_int32 (*Decompress)(void *, ape_int32 );
}CScaledFirstOrderFilter;

#ifdef __cplusplus
extern "C" {
#endif
  void      ApeCScaledFirstOrderFilterSetMS(void* ,ape_int32 ,ape_int32 );
  void      ApeCScaledFirstOrderFilterFlush(void *);
  ape_int32 ApeCScaledFirstOrderFilterCompress(void *, ape_int32 );
  ape_int32 ApeCScaledFirstOrderFilterDecompress(void *, ape_int32 );
#ifdef __cplusplus
}
#endif
#endif // #ifndef APE_SCALEDFIRSTORDERFILTER_H
