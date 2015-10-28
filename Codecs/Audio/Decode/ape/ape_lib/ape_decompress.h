#ifndef APE_APEDECOMPRESS_H
#define APE_APEDECOMPRESS_H

#include "ape_predictor.h"
#include "ape_decompress.h"

//class CUnBitArray;
//class CPrepare;
//class CAPEInfo;
//class IPredictorDecompress;
#include "ape_unbitarraybase.h"
#include "ape_maclib.h"
#include "ape_prepare.h"
#include "ape_circlebuffer.h"
#include "ape_Infodec.h"

typedef struct
{
  void      (*cCAPEDecompress)(void*,ape_int32 * pErrorCode, CAPEInfo * pAPEInfo, ape_int32 nStartBlock , ape_int32 nFinishBlock );
  void      (*dCAPEDecompress)();
  ape_int32 (*GetData)(void*, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
  ape_int32 (*Seek)(void*,ape_int32 nBlockOffset);
  ape_int32 (*GetInfo)(void*,ape_int32 Field, ape_int32 nParam1, ape_int32 nParam2);
}CAPEDecompress;
#ifdef __cplusplus
    extern "C" {
#endif

  void      ApeDecompressInitialize(ape_int32 * pErrorCode, CAPEInfo * pAPEInfo, ape_int32 nStartBlock , ape_int32 nFinishBlock );
  void      ApeDecompressDelete(void);
  ape_int32 ApeDecompressGetData(CAPEDecompress *aI, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
  ape_int32 ApeDecompressSeek(CAPEDecompress *aI,ape_int32 nBlockOffset);

  ape_int32 ApeDecompressGetInfo(ape_int32 Field, ape_int32 nParam1, ape_int32 nParam2);
  ape_int32 ApeDecompressSeekToFrame(ape_int32 nFrameIndex);
  void      ApeDecompressDecodeBlocks(ape_int32 nBlocks);
  ape_int32 ApeDecompressFillFrameBuffer(void);
  void      ApeDecompressStartFrame(void);
  void      ApeDecompressEndFrame(void);
  ape_int32 ApeDecompressInitializeDecompressor(CAPEDecompress * aI);
#ifdef __cplusplus
    }
#endif

#endif // #ifndef APE_APEDECOMPRESS_H
