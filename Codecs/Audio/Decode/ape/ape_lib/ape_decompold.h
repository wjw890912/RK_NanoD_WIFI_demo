#ifndef _apedecompressold_h_
#define _apedecompressold_h_

#include "ape_decompress.h"
#include "ape_unmac.h"

typedef struct
{
    void      (*cCAPEDecompressOld)(ape_int32 * pErrorCode, CAPEInfo * pAPEInfo, ape_int32 nStartBlock , ape_int32 nFinishBlock );
    void      (*dCAPEDecompressOld)();
    ape_int32 (*GetData)(ape_char * pBuffer, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
    ape_int32 (*Seek)(ape_int32 nBlockOffset);
	  ape_int32 (*GetInfo)(ape_int32 Field, ape_int32 nParam1, ape_int32 nParam2);
}CAPEDecompressOld;
#ifdef __cplusplus
    extern "C" {
#endif

    void      ApeDecompressOldCreate(ape_int32 * pErrorCode, CAPEInfo * pAPEInfo, ape_int32 nStartBlock , ape_int32 nFinishBlock );
    void      ApeDecompressOldDelete(void);
    ape_int32 ApeDecompressOldGetData(CAPEDecompressOld *aI,ape_char * pBuffer, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
    ape_int32 ApeDecompressOldSeek(CAPEDecompressOld *aI,ape_int32 nBlockOffset);
	  ape_int32 ApeDecompressOldGetInfo(ape_int32 Field, ape_int32 nParam1 , ape_int32 nParam2 );
	  ape_int32 ApeDecompressOldSeekToFrame(ape_int32 nFrameIndex);
    void      ApeDecompressOldDecodeBlocks(ape_int32 nBlocks);
    ape_int32 ApeDecompressOldFillFrameBuffer(void);
    void      ApeDecompressOldStartFrame(void);
    void      ApeDecompressOldEndFrame(void);
    ape_int32 ApeDecompressOldInitialize(CAPEDecompressOld * aI);

#ifdef __cplusplus
    }
#endif

#endif //_apedecompressold_h_

