/* ***** BEGIN LICENSE BLOCK *****
 * Source last modified: $Id: assembly.h,v 1.1 2005/02/26 01:47:34 jrecker Exp $
 *
 * Portions Copyright (c) 1995-2005 RealNetworks, Inc. All Rights Reserved.
 *
 * The contents of this file, and the files included with this file,
 * are subject to the current version of the RealNetworks Public
 * Source License (the "RPSL") available at
 * http://www.helixcommunity.org/content/rpsl unless you have licensed
 * the file under the current version of the RealNetworks Community
 * Source License (the "RCSL") available at
 * http://www.helixcommunity.org/content/rcsl, in which case the RCSL
 * will apply. You may also obtain the license terms directly from
 * RealNetworks.  You may not use this file except in compliance with
 * the RPSL or, if you have a valid RCSL with RealNetworks applicable
 * to this file, the RCSL.  Please see the applicable RPSL or RCSL for
 * the rights, obligations and limitations governing use of the
 * contents of the file.
 *
 * This file is part of the Helix DNA Technology. RealNetworks is the
 * developer of the Original Code and owns the copyrights in the
 * portions it created.
 *
 * This file, and the files included with this file, is distributed
 * and made available on an 'AS IS' basis, WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, AND REALNETWORKS HEREBY DISCLAIMS
 * ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET
 * ENJOYMENT OR NON-INFRINGEMENT.
 *
 * Technology Compatibility Kit Test Suite(s) Location:
 *    http://www.helixcommunity.org/content/tck
 *
 * Contributor(s):
 *
 * ***** END LICENSE BLOCK ***** */

/**************************************************************************************
 * Fixed-point HE-AAC decoder
 * Jon Recker (jrecker@real.com)
 * February 2005
 *
 * assembly.h - inline assembly language functions and prototypes
 *
 * MULSHIFT32(x, y) 		signed multiply of two 32-bit integers (x and y),
 *                            returns top 32-bits of 64-bit result
 * CLIPTOSHORT(x)			convert 32-bit integer to 16-bit short,
 *                            clipping to [-32768, 32767]
 * FASTABS(x)               branchless absolute value of signed integer x
 * CLZ(x)                   count leading zeros on signed integer x
 * MADD64(sum64, x, y)		64-bit multiply accumulate: sum64 += (x*y)
 **************************************************************************************/

#ifndef _ASSEMBLY_H
#define _ASSEMBLY_H

/* toolchain:           MSFT Visual C++
 * target architecture: x86
 */
#if (defined (_WIN32) && !defined (_WIN32_WCE)) || (defined (__WINS__) && defined (_SYMBIAN)) || (defined (WINCE_EMULATOR)) || (defined (_OPENWAVE_SIMULATOR))

#pragma warning( disable : 4035 )	/* complains about inline asm not returning a value */

 int MULSHIFT32(int x, int y);



 short CLIPTOSHORT(int x);


	/* clip to [-32768, 32767] */



int FASTABS(int x) ;
int CLZ(int x);






	/* count leading zeros with binary search */



typedef __int64 Word64;

typedef union _U64 {
	Word64 w64;
	struct {
		/* x86 = little endian */
		unsigned int lo32;
		signed int   hi32;
	} r;
} U64;

/* returns 64-bit value in [edx:eax] */
 Word64 MADD64(Word64 sum64, int x, int y);



/* asm version
 *
 *	__asm mov	eax, x
 *	__asm imul	y
 *	__asm add   eax, u.r.lo32
 *	__asm adc   edx, u.r.hi32
 */


/* toolchain:           MSFT Embedded Visual C++
 * target architecture: ARM v.4 and above (require 'M' type processor for 32x32->64 multiplier)
 */
#elif defined (_WIN32) && defined (_WIN32_WCE) && defined (ARM)

 short CLIPTOSHORT(int x);

	/* clip to [-32768, 32767] */


 int FASTABS(int x) ;
 int CLZ(int x);





	/* count leading zeros with binary search (function should be 17 ARM instructions total) */



/* implemented in asmfunc.s */
#ifdef __cplusplus
extern "C" {
#endif

typedef __int64 Word64;

typedef union _U64 {
	Word64 w64;
	struct {
		/* ARM WinCE = little endian */
		unsigned int lo32;
		signed int   hi32;
	} r;
} U64;

/* manual name mangling for just this platform (must match labels in .s file) */
#define MULSHIFT32	raac_MULSHIFT32
#define MADD64		raac_MADD64

int MULSHIFT32(int x, int y);
Word64 MADD64(Word64 sum64, int x, int y);

#ifdef __cplusplus
}
#endif

/* toolchain:           ARM ADS or RealView
 * target architecture: ARM v.4 and above (requires 'M' type processor for 32x32->64 multiplier)
 */
#elif defined (__arm) && defined (__ARMCC_VERSION)

  int MULSHIFT32(int x, int y);


  short CLIPTOSHORT(int x);

  int FASTABS(int x) ;

 int CLZ(int x) ;

typedef __int64 Word64;

typedef union _U64 {
	Word64 w64;
	struct {
		/* ARM ADS = little endian */
		unsigned int lo32;
		signed int   hi32;
	} r;
} U64;

 Word64 MADD64(Word64 sum64, int x, int y) ;

/* toolchain:           ARM gcc
 * target architecture: ARM v.4 and above (requires 'M' type processor for 32x32->64 multiplier)
 */
#elif defined(__GNUC__) && defined(__arm__)

 int MULSHIFT32(int x, int y);


short CLIPTOSHORT(int x);

	/* clip to [-32768, 32767] */


 int FASTABS(int x) ;
 int CLZ(int x);







typedef long long Word64;

typedef union _U64 {
	Word64 w64;
	struct {
		/* ARM ADS = little endian */
		unsigned int lo32;
		signed int   hi32;
	} r;
} U64;

 Word64 MADD64(Word64 sum64, int x, int y);




/* toolchain:           x86 gcc
 * target architecture: x86
 */
#elif defined(__GNUC__) && defined(__i386__)

typedef long long Word64;

 int MULSHIFT32(int x, int y);




 short CLIPTOSHORT(int x);

	/* clip to [-32768, 32767] */


 int FASTABS(int x) ;
 int CLZ(int x);





	/* count leading zeros with binary search (function should be 17 ARM instructions total) */



typedef union _U64 {
	Word64 w64;
	struct {
		/* x86 = little endian */
		unsigned int lo32;
		signed int   hi32;
	} r;
} U64;

 Word64 MADD64(Word64 sum64, int x, int y);



#else

#error Unsupported platform in assembly.h

#endif	/* platforms */

#endif /* _ASSEMBLY_H */
