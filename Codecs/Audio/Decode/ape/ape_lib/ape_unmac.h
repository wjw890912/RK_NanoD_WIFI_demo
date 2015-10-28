/*****************************************************************************************
ape_unmac.h
Copyright (C) 2000-2001 by Matthew T. Ashland   All Rights Reserved.

Methods for decompressing or verifying APE files

Notes:
    -none
*****************************************************************************************/

#ifndef APE_UNMAC_H
#define APE_UNMAC_H

#include "ape_bitarray.h"
#include "ape_unbitarraybase.h"

/*****************************************************************************************
CUnMAC class... a class that allows decoding on a frame-by-frame basis
*****************************************************************************************/
/*Mod by Wei.Hisung 2007.03.06*/

typedef struct
{
  void       (*CUnMAC)();
  void       (*dCUnMAC)();
  ape_int32  (*Initialize)(void *);
  ape_int32  (*Uninitialize)();
  ape_int32  (*DecompressFrame)(ape_uchar *pOutputData, ape_int32 FrameIndex, ape_int32 CPULoadBalancingFactor );
  ape_int32  (*SeekToFrame)(ape_int32 FrameIndex);
  ape_int32  (*DecompressFrameOld)(ape_uchar *pOutputData, ape_int32 FrameIndex, ape_int32 CPULoadBalancingFactor);
  ape_uint32 (*CalculateOldChecksum)(ape_int32 *pDataX, ape_int32 *pDataY, ape_int32 nChannels, ape_int32 nBlocks);
}CUnMAC;
#ifdef __cplusplus
extern "C" {
#endif
//void ApeUnMacInitialize(void);
//void ApeUnMacDelete(void);
//ape_int32 CUnMAC_Initialize(IAPEDecompress *pAPEDecompress);
//ape_int32 CUnMAC_Initialize(void *pAPEDecompress);
//ape_int32 CUnMAC_Uninitialize(void);
//ape_int32 CUnMAC_DecompressFrame(ape_uchar *pOutputData, ape_int32 FrameIndex, ape_int32 CPULoadBalancingFactor);
//ape_int32 CUnMAC_SeekToFrame(ape_int32 FrameIndex);
//void CUnMAC_GenerateDecodedArrays(ape_int32 nBlocks, ape_int32 nSpecialCodes, ape_int32 nFrameIndex, ape_int32 nCPULoadBalancingFactor);
//void CUnMAC_GenerateDecodedArray(ape_int32 *Input_Array, ape_uint32 Number_of_Elements, ape_int32 Frame_Index, void *pAntiPredictor, ape_int32 CPULoadBalancingFactor = 0);
//ape_int32 CUnMAC_CreateAntiPredictors(ape_int32 nCompressionLevel, ape_int32 nVersion);
//ape_int32 CUnMAC_DecompressFrameOld(ape_uchar *pOutputData, ape_int32 FrameIndex, ape_int32 CPULoadBalancingFactor);
//ape_uint32 CUnMAC_CalculateOldChecksum(ape_int32 *pDataX, ape_int32 *pDataY, ape_int32 nChannels, ape_int32 nBlocks);
#ifdef __cplusplus
}
#endif


#endif // #ifndef APE_UNMAC_H
