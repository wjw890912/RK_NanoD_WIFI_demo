#ifndef APE_CIRCLEBUFFER_H
#define APE_CIRCLEBUFFER_H

#include "ape_all.h"//added by hxd 20070319

typedef struct
{
	  void      (*cCCircleBuffer)(void *);
    //void    (*dCCircleBuffer)(void *);
    void      (*CreateBuffer)(void *,ape_int32 nBytes, ape_int32 nMaxDirectWriteBytes);
    ape_int32 (*MaxAdd)(void *);
    ape_int32 (*MaxGet)(void *);
    ape_int32 (*Get)(void *,ape_uchar * pBuffer/*ape_uint16 * pBuffer*/, ape_int32 nBytes);
    void      (*Empty)(void *);
    ape_int32 (*RemoveHead)(void *,ape_int32 nBytes);
    ape_int32 (*RemoveTail)(void *,ape_int32 nBytes);
	  ape_uchar*(*GetDirectWritePointer)(void *);
	  void      (*UpdateAfterDirectWrite)(void *,ape_int32 nBytes);
    ape_int32  m_nTotal;
    ape_int32  m_nMaxDirectWriteBytes;
    ape_int32  m_nEndCap;
    ape_int32  m_nHead;
    ape_int32  m_nTail;
    ape_uchar* m_pBuffer;//ape_uint16 * m_pBuffer;
}CCircleBuffer;

#ifdef __cplusplus
extern "C"{
#endif
//void CCircleBuffer_cCCircleBuffer(void *);
//void CCircleBuffer_dCCircleBuffer(void *);
//void CCircleBuffer_CreateBuffer(void *,ape_int32 nBytes, ape_int32 nMaxDirectWriteBytes);
//ape_int32 CCircleBuffer_MaxAdd(void *);
//ape_int32 CCircleBuffer_MaxGet(void *);
//ape_int32 CCircleBuffer_Get(void *,ape_uint16 * pBuffer, ape_int32 nBytes);
//void CCircleBuffer_Empty(void *);
//ape_int32 CCircleBuffer_RemoveHead(void *,ape_int32 nBytes);
//ape_int32 CCircleBuffer_RemoveTail(void *,ape_int32 nBytes);
//ape_uint16* CCircleBuffer_GetDirectWritePointer(void *);
//void CCircleBuffer_UpdateAfterDirectWrite(void *,ape_int32 nBytes);
#ifdef __cplusplus
}
#endif
#endif // #ifndef APE_CIRCLEBUFFER_H
