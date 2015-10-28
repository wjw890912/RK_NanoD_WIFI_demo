#ifndef APE_UNBITARRAY_OLD_H
#define APE_UNBITARRAY_OLD_H

#include "ape_all.h"
#ifdef BACKWARDS_COMPATIBILITY

#include "ape_unbitarraybase.h"
#ifdef __cplusplus
	extern "C" {
#endif
//class IAPEDecompress;

// decodes 0000 up to and including 3890
/*Mod by Wei.Hisung 2007.03.06*/

typedef struct
{
  void       (*CUnBitArrayOld)(IAPEDecompress *, ape_int32 );
  void       (*dCUnBitArrayOld)();
  void       (*GenerateArray)(ape_int32 *, ape_int32 , ape_int32 );
  ape_uint32 (*DecodeValue)(enum DECODE_VALUE_METHOD , ape_int32 , ape_int32 );
  void       (*GenerateArrayOld)(ape_int32* , ape_uint32 , ape_int32 );
  void       (*GenerateArrayRice)(ape_int32* , ape_uint32 , ape_int32 );
  ape_uint32 (*DecodeValueRiceUnsigned)(ape_uint32);
  ape_int32  (*DecodeValueNew)(ape_BOOL );
  ape_uint32 (*GetBitsRemaining)();
  ape_uint32 (*Get_K)(ape_uint32 );
}CUnBitArrayOld;
  void       ApeUnBitArrayOldInitialize(IAPEDecompress *pAPEDecompress, ape_int32 nVersion);
  void       ApeUnBitArrayOldDelete();
  void       ApeUnBitArrayOldGenerateArray(ape_int32 *pOutputArray, ape_int32 nElements, ape_int32 nBytesRequired );
  ape_uint32 ApeUnBitArrayOldDecodeValue(enum DECODE_VALUE_METHOD DecodeMethod, ape_int32 nParam1 , ape_int32 nParam2 );
  void       ApeUnBitArrayOldGenerateArrayOld(ape_int32* pOutputArray, ape_uint32 NumberOfElements, ape_int32 MinimumBitArrayBytes);
  void       ApeUnBitArrayOldGenerateArrayRice(ape_int32* pOutputArray, ape_uint32 NumberOfElements, ape_int32 MinimumBitArrayBytes);
  ape_uint32 ApeUnBitArrayOldDecodeValueRiceUnsigned(ape_uint32 k);
  ape_int32  ApeUnBitArrayOldDecodeValueNew(ape_BOOL bCapOverflow);
  ape_uint32 ApeUnBitArrayOldGetBitsRemaining();
  ape_uint32 ApeUnBitArrayOldGet_K(ape_uint32 x);
  ape_int32  ApeUnBitArrayOldCreateHelper(CIO * pIO, ape_int32 nBytes, ape_int32 nVersion);
  ape_uint32 ApeUnBitArrayOldDecodeValueXBits(ape_uint32 nBits);
  ape_int32  ApeUnBitArrayOldFillBitArray();
  void       ApeUnBitArrayOldAdvanceToByteBoundary();
  ape_int32  ApeUnBitArrayOldFillAndResetBitArray(ape_int32 nFileLocation, ape_int32 nNewBitIndex) ;
  ape_int32  ApeUnBitArrayOldDecodeValueRange(struct UNBIT_ARRAY_STATE *	BitArrayState);
  void       ApeUnBitArrayOldFinalize();
  void       ApeUnBitArrayOldFlushBitArray();
  void       ApeUnBitArrayOldFlushState(struct UNBIT_ARRAY_STATE * BitArrayState) ;
#ifdef __cplusplus
	}
#endif

#endif

#endif    // #ifndef APE_UNBITARRAY_OLD_H


