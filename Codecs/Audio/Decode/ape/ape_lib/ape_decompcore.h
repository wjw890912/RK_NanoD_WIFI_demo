#ifndef APE_DECOMPRESS_CORE_H
#define APE_DECOMPRESS_CORE_H


typedef struct
{
  void (*cCAPEDecompressCore)(void *,CIO * pIO, IAPEDecompress * pAPEDecompress);
  void (*dCAPEDecompressCore)(void *);
  void (*GenerateDecodedArrays)(void *,ape_int32 nBlocks, ape_int32 nSpecialCodes, ape_int32 nFrameIndex, ape_int32 nCPULoadBalancingFactor);
  void (*GenerateDecodedArray)(void *,ape_int32 *Input_Array, ape_uint32 Number_of_Elements, ape_int32 Frame_Index, void *pAntiPredictor, ape_int32 CPULoadBalancingFactor );
  ape_int32 * (*GetDataX)(void *);
  ape_int32 * (*GetDataY)(void *);
  CUnBitArrayBase * (*GetUnBitArrray)(void *);

  ape_int32 * m_pTempData;
  ape_int32 * m_pDataX;
  ape_int32 * m_pDataY;
  CAntiPredictor * m_pAntiPredictorX;
  CAntiPredictor * m_pAntiPredictorY;
  CUnBitArrayBase * m_pUnBitArray;
  struct BIT_ARRAY_STATE Ape_gBitArrayStateX;
  struct BIT_ARRAY_STATE Ape_gBitArrayStateY;
  IAPEDecompress * m_pAPEDecompress;
  ape_BOOL m_bMMXAvailable;
  ape_int32 m_nBlocksProcessed;
}CAPEDecompressCore;
#ifdef __cplusplus
    extern "C" {
#endif
//void CAPEDecompressCore_cCAPEDecompressCore(CAPEDecompressCore* aI,CIO * pIO, IAPEDecompress * pAPEDecompress);
//void CAPEDecompressCore_dCAPEDecompressCore(CAPEDecompressCore* aI);
//void CAPEDecompressCore_GenerateDecodedArrays(CAPEDecompressCore* aI,ape_uint32 nBlocks, ape_uint32 nSpecialCodes, ape_uint32 nFrameIndex,ape_int32 nCPULoadBalancingFactor);
//void CAPEDecompressCore_GenerateDecodedArray(CAPEDecompressCore* aI,ape_int32 *Input_Array, ape_uint32 Number_of_Elements, ape_int32 Frame_Index, void *pAntiPredictor, ape_int32 CPULoadBalancingFactor);
//ape_int32 * CAPEDecompressCore_GetDataX(CAPEDecompressCore* aI);
//ape_int32 * CAPEDecompressCore_GetDataY(CAPEDecompressCore* aI);
//CUnBitArrayBase * CAPEDecompressCore_GetUnBitArrray(CAPEDecompressCore* aI);
#ifdef __cplusplus
    }
#endif

#endif // #ifndef APE_DECOMPRESS_CORE_H
