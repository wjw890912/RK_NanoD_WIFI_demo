#ifndef APE_UNBITARRAYBASE_H
#define APE_UNBITARRAYBASE_H

#include "ape_io1.h"

#ifdef __cplusplus
	extern "C" {
#endif
struct UNBIT_ARRAY_STATE
{
    ape_uint16 k;
    ape_uint32 nKSum;
};

enum DECODE_VALUE_METHOD
{
    DECODE_VALUE_METHOD_UNSIGNED_INT,
    DECODE_VALUE_METHOD_UNSIGNED_RICE,
    DECODE_VALUE_METHOD_X_BITS
};

/*Mod by Wei.Hisung 2007.03.06*/

typedef struct
{
    void       (*dCUnBitArrayBase)();
    ape_int32  (*FillBitArray)();
    ape_int32  (*FillAndResetBitArray)(ape_int32 , ape_int32 );
//    void       (*GenerateArray)(ape_int32 * , ape_int32 , ape_int32 ) ;
    ape_uint32 (*DecodeValue)(enum DECODE_VALUE_METHOD , ape_int32 , ape_int32 ) ;
    void       (*AdvanceToByteBoundary)();
	  ape_int32  (*DecodeValueRange)(struct UNBIT_ARRAY_STATE * );
    void       (*FlushState)(struct UNBIT_ARRAY_STATE * );
    void       (*FlushBitArray)();
    void       (*Finalize)();
    ape_int32  (*CreateHelper)(CIO * , ape_int32 , ape_int32 );
    ape_uint32 (*DecodeValueXBits)(ape_uint32 );
}CUnBitArrayBase;

ape_int32  ApeUnBitArrayBaseFillBitArray(void);
ape_int32  ApeUnBitArrayBase_FillAndResetBitArray(ape_int32 nFileLocation, ape_int32 nNewBitIndex);
void       ApeUnBitArrayBase_AdvanceToByteBoundary(void);
ape_int32  ApeUnBitArrayBaseCreateHelper(CIO * pIO, ape_int32 nBytes, ape_int32 nVersion);
ape_uint32 ApeUnBitArrayBase_DecodeValueXBits(ape_uint32 nBits);
void       ApeUnBitArrayBaseDelete(void);
void       ApeUnBitArrayBaseFinalize(void);
void       ApeUnBitArrayBaseFlushBitArray(void);
ape_int32  ApeUnBitArrayBaseDecodeValueRange(struct UNBIT_ARRAY_STATE * BitArrayState);
void       ApeUnBitArrayBaseFlushState(struct UNBIT_ARRAY_STATE * BitArrayState);
void       ApeUnBitArrayBaseGenerateArray(ape_int32 * pOutputArray, ape_int32 nElements, ape_int32 nBytesRequired );
ape_uint32 ApeUnBitArrayBaseDecodeValue(enum DECODE_VALUE_METHOD DecodeMethod, ape_int32 nParam1, ape_int32 nParam2);
CUnBitArrayBase* ApeUnBitArrayCreate(void * pAPEDecompress, ape_int32 nVersion);
#ifdef __cplusplus
	}
#endif

#endif // #ifndef APE_UNBITARRAYBASE_H
