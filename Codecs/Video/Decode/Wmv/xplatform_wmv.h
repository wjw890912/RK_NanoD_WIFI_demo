//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef __XPLATFORM__
#define __XPLATFORM__
//#pragma pack (1)

#if defined(_MAC) || defined(macintosh)
#define macintosh
#define _MAC
#define __MACVIDEOPRE__
#define __MACVIDEO__
#define NO_WINDOWS
#define _BIG_ENDIAN_
#define _SLIM_C_
#define __STREAMING_MODE_DECODE_
#define OUTPUT_ALL_FORMATS
#define _SUPPORT_POST_FILTERS_
#define _MAC_VEC_OPT
//#define (__arm)

//#define _EMB_WMV2_  //turn off until we fix big-endian&little-endian problem
//#define OPT_HUFFMAN_GET_WMV
//#define DebugBreak() (void)0

//#include "MacPort.h"
#include <wchar_t.h>
#include <stdlib.h>


#endif //macintosh

#ifdef _XBOX
#include <xtl.h>
#endif

//#define COVERAGE_STATS //at project level to get some coverage statistics

//**************************************************************************************************


//**************************************************************************************************
#if defined(__arm)
    // Embedded ARM
#   define _EMB_WMV2_
#   define __STREAMING_MODE_DECODE_
#   define _SLIM_C_                 // no multithread
#   define OUTPUT_ALL_FORMATS       // all color conversion
#   define __huge
#   define NO_WINDOWS
    typedef void* LPVOID;
  	typedef void* HANDLE;
    typedef long long LARGE_INTEGER;
    typedef char TCHAR;
    typedef char* LPCTSTR;
#   define _T(a) a
#   define TEXT(a) a
#   define _stprintf sprintf
#   define UINT_MAX  0xffffffffU
    typedef unsigned long DWORD;
#endif



//**************************************************************************************************



#define B_FRAME
#define _XFORMSWITCHING2_
#define _STARTCODE_
#define B_FRAME_411
#define INTERLACEV2_B
#define WMVA_NEW_ENTROPY_CODING
#define _NEW_SEQUENCE_ENTRY_

#undef _EMB_WMV2_


#ifdef  _EMB_WMV3_
//#   define OPT_HUFFMAN_GET_WMV
//#   define _SSIMD32_
#endif

//#define _SUPPORT_POST_FILTERS_

#ifdef _EMB_WMV2_                     //  conditional defines for CE devices
#   define COMBINE_LOOPFILTER_RENDERING
#   define DYNAMIC_EDGEPAD          // Verify: setting of motioncomp_arm.s -PD "DYNAMIC_EDGEPAD SETA 1", repeatpad_arm.s as -PD"EXPANDY_REFVOP SETA 8"
#   define DYNAMIC_EDGEPAD_0
#   define OPT_HUFFMAN_GET_WMV          // from wmvideo optimization - OK for all the platforms
#   define OPT_SMALL_CODE_SIZE      // without this, some loops get unrolled

//These 2 flags are not turned on by default, since their impact to performance is very platform-dependent.
// However, they are turned on for mipsxx, see mips section for more comments

//#   define NO_INFO_TABLE
//#   define COMBINED_LEVELRUN

//The NO_INFO_TABLE flag also controls a feature in which the huffman tables after a predefined stage will be
//placed in uncached memory. Use this flag to turn it off.
#ifdef NO_INFO_TABLE
//#   define NO_UNCACHED_STAGES
#endif

    ////#    define OPT_TINY_CODE_SIZE       // trade-off for smaller code size with added complexity within loops.
#   if defined(OPT_TINY_CODE_SIZE) && !defined(OPT_SMALL_CODE_SIZE)
#       define OPT_SMALL_CODE_SIZE
#   endif

#ifdef  OPT_HUFFMAN_GET_WMV
#   define STRMPARSER_NUMBITS 32 // default using 32bit buffers for bit stream parsing. Only 64bit platform can define this to 64
#endif

#   ifdef macintosh
#       undef DYNAMIC_EDGEPAD
#       undef DYNAMIC_EDGEPAD_0
#   endif



#   if defined(_ARM_) || defined(__arm)
#       if !defined (__arm)
#           define _EMB_ASM_ARM_STRMHUFF_
#       endif

#       define IPAQ_HACK
#       define _ARM_ASM_MOTIONCOMP_OPT_
#       define _ARM_ASM_LOOPFILTER_OPT_
#       define _ARM_ASM_BLKDEC_OPT_
#       define IDCTDEC_32BIT_INPUT      // To enable using ARM multiple load/stores
#       define OPT_IDCT_ARM
#       define OPT_DECOLORCONV_ARM
#       ifdef OPT_DECOLORCONV_ARM
#           define OUTPUT_RGB12
#       endif
//#       define IPP_COLORCONV
#       define OPT_ZEROMOTION_ARM
#       define OPT_REPEATPAD_ARM
#       define OPT_X8_ARM
#       if defined(DYNAMIC_EDGEPAD)
#           define _ARM_ASM_DYNAMICPAD_OPT_
#       endif
      // define STINGER in Stinger specific project files
#       if defined(STINGER)
//#           define _WMVONLY_
#           ifdef OUTPUT_ALL_FORMATS
#               undef OUTPUT_ALL_FORMATS
#           endif
#           ifndef OUTPUT_RGB12
#               define OUTPUT_RGB12
#           endif
#           define STATIC_PAD_0
#       endif

      // ARCH_V3 should define OPT_SLOW_MULTIPLY

#   endif
#   if defined(__arm)
//#       define CE_DECODEPMB_GLOBAL_FUNCTABLE    // otherwise C++ errors in vopdec_ce.cpp
#   endif

#   ifdef _SH3_
        // SH3_DSP is defined at the project level
        // It is also defined in the custom build settings of each sh3_dsp.src and sh3.src file.
#       ifdef SH3_DSP
#           define USE_SH3_DSP_ASM
#           ifdef USE_SH3_DSP_ASM
                // you must change the custom build asm settings to match these defines in each sh3_dsp.src file
#               define SH3_DSP_ASM_DECOLORCONV      // asm defines USE_YUV_RGB16_ASM, USE_YUV_RGB24_ASM, USE_YUV_RGB32_ASM
#               define SH3_DSP_ASM_LOOPFILTER
#               define SH3_DSP_ASM_IDCT
#               define SH3_DSP_ASM_MOTION_COMP
#           endif
#       else
#           define USE_SH3_ASM          ;; you must change *_sh3.src asm settings to match these
#           ifdef USE_SH3_ASM
                // you must change the custom build asm settings to match these defines in each sh3.src file
#               define SH3_ASM_DECOLORCONV
#               define SH3_ASM_LOOPFILTER
#               define SH3_ASM_MOTION_COMP
#               define OPT_IDCT_SHX 3
#               define _SHX_ASM_X8_OPT_ 3
#           endif
#       endif
#       if defined(SH3_ASM_MOTION_COMP) || defined(SH3_DSP_ASM_MOTION_COMP)
#           undef DYNAMIC_EDGEPAD
#           undef DYNAMIC_EDGEPAD_0
#       endif
#   endif


#   ifdef _SH4_

// you must change the custom build asm settings to match these defines in each sh4.src file

#       define _SH4_ASM_LOOPFILTER_OPT_
#       define OPT_DECOLORCONV_SH4
#       define _SH4_ASM_MOTIONCOMP_OPT_
#       define OPT_IDCT_SHX 4
#       define _SHX_ASM_X8_OPT_ 4
#   endif


#   ifdef _MIPS_

//  with tests done on Boston, I found this combination of the flags works best
#       define NO_INFO_TABLE
#       define COMBINED_LEVELRUN
#       define NO_UNCACHED_STAGES

#       define _MIPS_ASM_MOTIONCOMP_OPT_
#       define _MIPS_ASM_LOOPFILTER_OPT_
#       define _6BIT_COLORCONV_OPT_
#       define _MIPS_ASM_IDCTDEC_OPT_
//# define IDCTDEC_MIPSASM
#       define _EMB_ASM_MIPS_BLKDEC_
#       ifdef _MIPS_ASM_IDCTDEC_OPT_
#           define IDCTDEC_16BIT_3ARGS
#       endif
#       define _EMB_ASM_MIPS_STRMDEC_
#       define _EMB_ASM_MIPS_HUFFDEC_
#       define _MIPS_ASM_X8_OPT_
#       define _EMB_ASM_MIPS_DECOLORCONV_
#       define _EMB_ASM_MIPS_REPEATPAD_
#       ifdef  _MIPS64
#         define _64BIT_
#         define _MIPS_5432_
#         define _MIPS64_OPTS_
#         undef STRMPARSER_NUMBITS
#         define STRMPARSER_NUMBITS 64
#         ifndef REG_SIZE
#           define REG_SIZE 8
#         endif
#       else
#         ifndef REG_SIZE
#           define REG_SIZE 4
#         endif
#       endif
#   endif
#endif //_EMB_WMV2_


//**************************************************************************************************
// memcpy of aligned U32_WMV with cbSiz = number of bytes to copy
// assumes pDst and pSrc are aligned, but casts them in case they are actually byte or short pointers.
#define ALIGNED32_MEMCPY(pDst,pSrc,cbSiz) {                 \
            U32_WMV* pD = (U32_WMV*)(pDst), *pS = (U32_WMV*)(pSrc);     \
            int j;                                         \
            for(j=(cbSiz)>>2; j>0;j--)                  \
                *pD++ = *pS++;                              \
        }
#define ALIGNED32_MEMCPY16(pDst,pSrc) {                     \
            U32_WMV* pD = (U32_WMV*)(pDst), *pS = (U32_WMV*)(pSrc);     \
            *pD++ = *pS++;                                  \
            *pD++ = *pS++;                                  \
            *pD++ = *pS++;                                  \
            *pD   = *pS;                                    \
        }
#define ALIGNED32_MEMCPY8(pDst,pSrc) {                      \
            U32_WMV* pD = (U32_WMV*)(pDst), *pS = (U32_WMV*)(pSrc);     \
            *pD++ = *pS++;                                  \
            *pD   = *pS;                                    \
        }
// memset of aligned U32_WMV with cbSiz = number of bytes to copy
#define ALIGNED32_MEMSET_U32(pDst,u32C,cbSiz) {             \
            U32_WMV* pD=(U32_WMV*)(pDst);                   \
            int j;                                          \
            for( j=(cbSiz)>>2; j>0;j--)                  \
                *pD++ = u32C;                               \
        }
#define ALIGNED32_MEMSET_2U32(pDst,u32C) {                  \
            U32_WMV* pD = (U32_WMV*)(pDst);                         \
            *pD++ = u32C;                                   \
            *pD   = u32C;                                   \
        }
// replicate a byte 4 times in an U32_WMV.
#ifndef OPT_SLOW_MULTIPLY
#   define ALIGNED32_REPLICATE_4U8(a) (((U8_WMV)(a)) * 0x01010101u)
#else
#   define ALIGNED32_REPLICATE_4U8(a) (((U8_WMV)(a)<<24) | ((U8_WMV)(a)<<16) | ((U8_WMV)(a)<<8) | (U8_WMV)(a))
#endif
// memset of aligned U32_WMV with a replicated U8_WMV constant
#define ALIGNED32_MEMSET_U8(pDst,u8C,cbSiz) {               \
            const U8_WMV  u8T = (U8_WMV)(u8C);                      \
            const U16_WMV u16T = (u8T<<8) | u8T;                \
            const U32_WMV u32C = (u16T<<16) | u16T;             \
            ALIGNED32_MEMSET_U32( (pDst), u32C, (cbSiz) );  \
        }
#define ALIGNED32_MEMSET_8U8(pDst,u8C) {                    \
            const U8_WMV  u8T = (U8_WMV)(u8C);                      \
            const U16_WMV u16T = (u8T<<8) | u8T;                \
            const U32_WMV u32C = (u16T<<16) | u16T;             \
            U32_WMV* pD=(U32_WMV*)(pDst);                           \
            *pD++ = u32C;                                   \
            *pD   = u32C;                                   \
        }
#ifndef OPT_SMALL_CODE_SIZE
#   define ALIGNED32_MEMSET_20U8(pDst,u8C) {                \
            const U8_WMV  u8T = (U8_WMV)(u8C);                      \
            const U16_WMV u16T = (u8T<<8) | u8T;                \
            const U32_WMV u32C = (u16T<<16) | u16T;             \
            U32_WMV* pD=(U32_WMV*)(pDst);                           \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD   = u32C;                                   \
        }
#   define ALIGNED32_MEMSET_24U8(pDst,u8C) {                \
            const U8_WMV  u8T = (U8_WMV)(u8C);                      \
            const U16_WMV u16T = (u8T<<8) | u8T;                \
            const U32_WMV u32C = (u16T<<16) | u16T;             \
            U32_WMV* pD=(U32_WMV*)(pDst);                           \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD++ = u32C;                                   \
            *pD   = u32C;                                   \
        }
#   define ALIGNED32_MEMCLR_128U8(pDst) {                   \
            U32_WMV* pD=(U32_WMV*)(pDst);                           \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
            *pD++ = 0;  *pD++ = 0;  *pD++ = 0; *pD++ = 0;   \
        }
#else // OPT_SMALL_CODE_SIZE
#   define ALIGNED32_MEMSET_20U8(pDst,u8C) ALIGNED32_MEMSET_U8((pDst),(u8C),20)
#   define ALIGNED32_MEMSET_24U8(pDst,u8C) ALIGNED32_MEMSET_U8((pDst),(u8C),24)
#   define ALIGNED32_MEMCLR_128U8(pDst)    ALIGNED32_MEMSET_U32((pDst),0,128)
#endif
// Average of 8 bytes
#define ALIGNED32_AVERAGE_8U8(avg,pSrc,Rnd) {                                                                           \
            const U32_WMV u32S0 = *((U32_WMV*)(pSrc)), u32S1 = *((U32_WMV*)(pSrc+4));                                               \
            avg = (u32S0 & 0x00ff00ff) + ((u32S0>>8) & 0x00ff00ff) + (u32S1 & 0x00ff00ff) + ((u32S1>>8) & 0x00ff00ff);  \
            avg = ((avg & 0x0000ffff) + ((avg>>16) & 0x0000ffff) + Rnd) >> 3;                                           \
        }
// Sum of 8 bytes (takes 13 ops + 2 reads)
#define ALIGNED32_SUM_8U8(sum,pSrc) {                                                                                   \
            const U32_WMV u32S0 = *((U32_WMV*)(pSrc)), u32S1 = *((U32_WMV*)(pSrc+4));                                               \
            sum = (u32S0 & 0x00ff00ff) + ((u32S0>>8) & 0x00ff00ff) + (u32S1 & 0x00ff00ff) + ((u32S1>>8) & 0x00ff00ff);  \
            sum = (sum & 0x0000ffff) + ((sum>>16) & 0x0000ffff);                                                        \
        }

//**************************************************************************************************
#ifdef PROFILE
#   include "AutoProfile.h"
#else
#   define FUNCTION_PROFILE(fp)
#   define FUNCTION_PROFILE_START(fp,id)
#   define FUNCTION_PROFILE_STOP(fp)
#   define FUNCTION_PROFILE_DECL_START(fp,id)
#   define FUNCTION_PROFILE_SWITCH(fp,id)
#endif

//**************************************************************************************************
#ifndef DEBUG_ONLY
#   if defined(DEBUG) || defined(_DEBUG)
#       define DEBUG_ONLY(a) a
#   else
#       define DEBUG_ONLY(a)
#   endif
#endif // DEBUG_ONLY
#ifndef DEBUG_CON_ONLY
#   if (defined(DEBUG) || defined(_DEBUG)) && !defined(UNDER_CE)
        // Some winCE platforms do not support printf.
#       define DEBUG_CON_ONLY(a) a
#   else
#       define DEBUG_CON_ONLY(a)
#   endif
#endif // DEBUG_CON_ONLY
#ifndef DEBUG_HEAP_ADD
#   if defined(WANT_HEAP_MEASURE)
#       define DEBUG_HEAP_ADD(c,a) {if (!g_HeapMeasureInitialize) {int ci;for (ci = 0;ci<6;ci++){g_cbHeapSize[ci]=0; g_cbMaxHeapSize[ci]=0;} g_HeapMeasureInitialize=1;} g_cbHeapSize[g_iHeapLastClass=c] += a;  if (g_cbHeapSize[c]>g_cbMaxHeapSize[c]) g_cbMaxHeapSize[c] = g_cbHeapSize[c]; }
#       define DEBUG_HEAP_SUB(c,a) { g_cbHeapSize[g_iHeapLastClass=c] -= a; }
#       ifdef __cplusplus
            extern "C"
#       else
            extern
#       endif

        int g_cbMaxHeapSize[7], g_cbHeapSize[7], g_iHeapLastClass,g_HeapMeasureInitialize;

#       define DHEAP_STRUCT 0
#       define DHEAP_FRAMES 1
#       define DHEAP_HUFFMAN 2
#       define DHEAP_LOCALHUFF 3
#       define DHEAP_LOCAL_COUNT 4
#   else
#       define DEBUG_HEAP_ADD(c,a)
#       define DEBUG_HEAP_SUB(c,a)
#   endif
#endif // DEBUG_HEAP_MEASURE




//#pragma pack ()

#endif




