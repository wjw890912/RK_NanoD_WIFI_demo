/******************************************************************************
*
*  Copyright (C),2007, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     flacbuffer.h
*  Description:    FLAC解码器的输入输出Buffer.
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*           Huweiguo     07/04/23      1.0
*
*******************************************************************************/

#ifndef __FLAC_BUFFER_H__
#define __FLAC_BUFFER_H__

#include "ordinals.h"


#define FLAC__BITBUFFER_MAX_REMAINDER    (64)  // 每次fill buffer时都有可能剩余数据，我们认为剩余数据不会超过64个
//#define FLAC__BITBUFFER_DEFAULT_CAPACITY (4096/2)
#define FLAC__BITBUFFER_DEFAULT_CAPACITY (4096/8)	//test
#define FLAC__PCMBUFFER_DEFAULT_CAPACITY (4608)

#define FLAC__MAX_LPC_ORDER (32u)// 应与format.h里的一致
#define FLAC__MAX_CHANNEL 2

/*
 * 输出
 * 1、目前我们只支持 bits_per_sample = 16 的情况;
 * 2、当 bits_per_sample > 16 时，现有的输出 Buffer 将不够用;
 * 3、FLAC__MAX_LPC_ORDER 个 dword 用于存放 warmup data;
 *    什么是warmup data?
 *    假设一个线性滤波器为： y(n) = a0 x(n) + a1 x(n-1) + a2 x(n-2),
 *    那么初始状态的 x(n), x(n-1), x(n-2) 就是warmup data， 只有获得了这些值，我们才能求得 y(n)
 */
extern unsigned char g_FlacInputBuffer[]; //FLAC__BITBUFFER_MAX_REMAINDER 用于存放buffer中残留的数据

	// 输出,
#ifdef FLAC_MINUS_ZI_DATA
	//modified by evan wu,2009-3-18,for saving zi data
extern	unsigned short g_FlacOutputBufferMidOrLeft[];
extern	unsigned long g_FlacOutputBufferRightOrSide[];
#else

#ifdef HALF_FRAME_BY_HALF_FRAME
extern unsigned long g_FlacOutputBuffer[FLAC__MAX_CHANNEL][FLAC__PCMBUFFER_DEFAULT_CAPACITY/2+FLAC__MAX_LPC_ORDER+FLAC__MAX_LPC_ORDER];
#else
extern unsigned long g_FlacOutputBuffer[FLAC__MAX_CHANNEL][FLAC__PCMBUFFER_DEFAULT_CAPACITY+FLAC__MAX_LPC_ORDER+FLAC__MAX_LPC_ORDER];
#endif


#endif//FLAC_MINUS_ZI_DATA



#ifdef HALF_FRAME_BY_HALF_FRAME
//extern	unsigned short g_FlacCodecBuffer[2][FLAC__PCMBUFFER_DEFAULT_CAPACITY+FLAC__MAX_LPC_ORDER];
extern unsigned short *  g_FlacCodecBuffer[2];
#else
extern	unsigned short g_FlacCodecBuffer[];
#endif



#endif
