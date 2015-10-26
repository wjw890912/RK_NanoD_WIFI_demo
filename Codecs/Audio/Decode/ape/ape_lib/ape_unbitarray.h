#ifndef APE_UNBITARRAY_H
#define APE_UNBITARRAY_H

#include "ape_unbitarraybase.h"

//class IAPEDecompress;

//struct RANGE_CODER_STRUCT_DECOMPRESS
//{
//    ape_uint32 low;       // low end of interval
//    ape_uint32 range;     // length of interval
//    ape_uint32 buffer;    // buffer for input/output
//};

/*Mod by Wei.Hisung 2007.03.06*/

typedef struct
{
  void       (*CUnBitArray)(CIO * , ape_int32 );
  //void (*dCUnBitArray)();
  ape_uint32 (*DecodeValue)(enum DECODE_VALUE_METHOD , ape_int32 , ape_int32 );
  void       (*GenerateArray)(ape_int32 * , ape_int32 , ape_int32 );
  ape_int32  (*DecodeValueRange)(struct UNBIT_ARRAY_STATE *);
  void       (*FlushState)(struct UNBIT_ARRAY_STATE *);
  void       (*FlushBitArray)();
  void       (*Finalize)();
  void       (*GenerateArrayRange)(ape_int32 * , ape_int32 );
  ape_int32  (*RangeDecodeFast)(ape_int32 );
  ape_int32  (*RangeDecodeFastWithUpdate)(ape_int32 );
  ape_uchar  (*GetC)();
}CUnBitArray;
#ifdef __cplusplus
    extern "C" {
#endif
void       ApeUnBitArrayInitialize(CIO * pIO, ape_int32 nVersion);
ape_uint32 ApeUnBitArrayDecodeValue(enum DECODE_VALUE_METHOD DecodeMethod, ape_int32 nParam1 , ape_int32 nParam2 );
void       ApeUnBitArrayGenerateArray(ape_int32 * pOutputArray, ape_int32 nElements, ape_int32 nBytesRequired );
ape_int32  ApeUnBitArrayDecodeValueRange(struct UNBIT_ARRAY_STATE * BitArrayState);
ape_int32 ApeUnBitArrayDecodeValueRangeMergeXAndY(struct UNBIT_ARRAY_STATE * BitArrayStateY,struct UNBIT_ARRAY_STATE * BitArrayStateX);
void       ApeUnBitArrayFlushState(struct UNBIT_ARRAY_STATE * BitArrayState);
void       ApeUnBitArrayFlushBitArray(void);
void       ApeUnBitArrayFinalize(void);
void       ApeUnBitArrayGenerateArrayRange(ape_int32 * pOutputArray, ape_int32 nElements);
//ape_int32  ApeUnBitArrayRangeDecodeFast(ape_int32 nShift);
//ape_int32  ApeUnBitArrayRangeDecodeFastWithUpdate(ape_int32 nShift);
ape_uchar  ApeUnBitArrayGetC(void);
//-----------------------------------------------
ape_int32  ApeUnBitArrayCreateHelper(CIO * pIO, ape_int32 nBytes, ape_int32 nVersion);
ape_uint32 ApeUnBitArrayDecodeValueXBits(ape_uint32 nBits);
ape_int32  ApeUnBitArrayFillBitArray(void);
ape_int32  ApeUnBitArrayFillBitArrayOutside(void);
void       ApeUnBitArrayAdvanceToByteBoundary(void);
ape_int32  ApeUnBitArrayFillAndResetBitArray(ape_int32 nFileLocation, ape_int32 nNewBitIndex) ;
#ifdef __cplusplus
}
#endif

#endif // #ifndef APE_UNBITARRAY_H
