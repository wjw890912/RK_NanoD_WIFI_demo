#ifndef pWMA_H
#define pWMA_H

#include ".\wmaInclude\WMAUDIO.H"

typedef struct
{
    int (*read_func)(void *pbuf, long size, long cnt, void* fhandle);
    int (*seed_func)(void *fhandle, long offset, int whence);
    int (*write_func)(void *pbuf, long size, long cnt, void* fhandle);
} WMA_CALLBACKS;

typedef struct
{
	//intput
	unsigned long curPtr;
	unsigned long fhandle;
	unsigned long msSeekTo;

	//output
	unsigned long frameLen;
	int isHighRate;
	unsigned int decoded_samples;
	tWMAFileHeader* hdr;
	tWMAFileContDesc *pdesc;
}WMA_DEC_INFO;
#endif

