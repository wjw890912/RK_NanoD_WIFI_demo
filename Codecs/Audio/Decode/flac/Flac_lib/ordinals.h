/* libFLAC - Free Lossless Audio Codec library
 * Copyright (C) 2000,2001,2002,2003,2004,2005  Josh Coalson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the Xiph.org Foundation nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FLAC__ORDINALS_H
#define FLAC__ORDINALS_H

#define FLAC_TABLE_ROOM_VERIFY//使用table rom
//#define FLAC_MINUS_ZI_DATA
#define HALF_FRAME_BY_HALF_FRAME //以半帧为单位解码
#define FLAC_FSEEK_OPT

#define assert(a)

//by Vincent
#define MDK_PLATFORM

//#define MDK_PLATFORM
#ifdef MDK_PLATFORM
#define __int16 short
#define __int32 int
#endif
#if 1
//typedef signed short FLAC__int8;
//typedef unsigned short FLAC__uint8;
typedef signed char FLAC__int8;
typedef unsigned char FLAC__uint8;

typedef __int16 FLAC__int16;
typedef __int32 FLAC__int32;
typedef __int32 FLAC__int64;
typedef unsigned __int16 FLAC__uint16;
typedef unsigned __int32 FLAC__uint32;
typedef unsigned __int32 FLAC__uint64;
#else
typedef char FLAC__int8;
typedef unsigned char FLAC__uint8;
typedef short FLAC__int16;
typedef int FLAC__int32;
typedef int FLAC__int64;
typedef unsigned short FLAC__uint16;
typedef unsigned int FLAC__uint32;
typedef unsigned int FLAC__uint64;
#endif
typedef int FLAC__bool;

typedef FLAC__uint8 FLAC__byte;


// result
#ifdef true
#undef true
#endif

#ifdef false
#undef false
#endif

#ifndef __cplusplus
#define true 1
#define false 0
#endif

#if defined(FLAC_MINUS_ZI_DATA)||defined(HALF_FRAME_BY_HALF_FRAME)
extern unsigned gFLACchannel;
#endif
extern FLAC__bool isNeedDecByHalfFrmae;
#define rk_nano_board

#endif
