#ifndef APE_GLOBALFUNCTIONS_H
#define APE_GLOBALFUNCTIONS_H
#include "ape_io1.h"
/*************************************************************************************
Definitions
*************************************************************************************/
//class CIO;

/*************************************************************************************
Read / Write from an IO source and return failure if the number of bytes specified
isn't read or written
*************************************************************************************/
#ifdef __cplusplus
 extern "C"{
#endif
ape_int32 ApeReadSafe(CIO * pIO, void * pBuffer, ape_int32 nBytes);
ape_int32 ApeWriteSafe(CIO * pIO, void * pBuffer, ape_int32 nBytes);
#ifdef __cplusplus
 }
#endif
#endif // #ifndef APE_GLOBALFUNCTIONS_H
