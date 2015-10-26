#ifndef APE_BITARRAY_H
#define APE_BITARRAY_H

#include "ape_io1.h"
/*Mod by Wei.Hisung 2007.03.05*/
//#include "MD5.h"

//#define BUILD_RANGE_TABLE

struct RANGE_CODER_STRUCT_COMPRESS
{
    ape_uint32 low;        // low end of interval
    ape_uint32 range;        // length of interval
    ape_uint32 help;        // bytes_to_follow resp. intermediate value
    ape_uchar buffer;    // buffer for input / output
};

struct BIT_ARRAY_STATE
{
    ape_uint32    k;
    ape_uint32    nKSum;
};

#endif // #ifndef APE_BITARRAY_H
