//#ifdef IO_USE_WIN_FILE_IO

#ifndef _winfileio_h_
#define _winfileio_h_

#include "ape_io2.h"

typedef struct
{
  ape_int32 (*Open)(void *);
  ape_int32 (*Close)(void *);
  //ape_int32 (*Read)(void *,void * , ape_uint32 , ape_uint32 * );
  ape_int32 (*Read)(void *,void * , unsigned int , unsigned int * );
  ape_int32 (*Write)(void *, void * , unsigned int , unsigned int * );
  ape_int32 (*Seek)(void *,ape_int32 nDistance, ape_uint32 nMoveMode);
  ape_int32 (*SetEOF)(void *);
  ape_int32 (*Create)(void *,char * pName);
  ape_int32 (*Delete)(void *);
  ape_int32 (*GetPosition)(void *);
  ape_int32 (*GetSize)(void *);
  ape_int32 (*GetName)(void *,char * pBuffer);

  HANDLE    m_hFile;
  ape_char  m_cFileName[MAX_PATH];
  ape_BOOL  m_bReadOnly;
}CIO;

  void      ApeIoInitialize(CIO* aI);
  ape_int32 ApeIoOpen(CIO* aI);
  ape_int32 ApeIoClose(CIO* aI);
  ape_int32 ApeIoRead(CIO* aI,void * pBuffer, ape_uint32 nBytesToRead, ape_uint32 * pBytesRead);
  ape_int32 ApeIoWrite(CIO* aI, void * pBuffer, ape_uint32 nBytesToWrite, ape_uint32 * pBytesWritten);
  ape_int32 ApeIoSeek(CIO* aI,ape_int32 nDistance, ape_uint32 nMoveMode);
  ape_int32 ApeIoSetEof(CIO* aI);
  ape_int32 ApeIoCreate(CIO* aI,ape_char* pName);
  ape_int32 ApeIoDelete(CIO* aI);
  ape_int32 ApeIoGetPosition(CIO* aI);
  ape_int32 ApeIoGetSize(CIO* aI);
  ape_int32 ApeIoGetName(CIO* aI,ape_char* pBuffer);
#endif //_winfileio_h_

//#endif //IO_USE_WIN_FILE_IO
