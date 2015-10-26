#ifndef _PFLAC_DEC_H_
#define _PFLAC_DEC_H_

//#include "../buffer/buffer.h"
#include "ape_globalvardeclaration.h"

//////////////////////////////////

// 注意:下面两个宏必须跟APEDec.h里的宏APE_PCM_BUFFER_SIZE\APE_ENCODED_DATA_SIZE一致, HuWeiGuo

//#define  PCM_BUFFER_SIZE    (BLOCKS_PER_DECODE * 2)//可以保存32位PCM输出
#define  PCM_BUFFER_SIZE    (BLOCKS_PER_DECODE)//不支持32位PCM输出
#define  ENCODED_DATA_SIZE  ((BLOCKS_PER_BITSTREAMREAD+16)*4)//以后要修改输入Buffer长度就修改BLOCKS_PER_BITSTREAMREAD宏

#if 0
///定义文件操作结构体类型,其成员为函数指针类型 added by hxd 20070723
typedef struct{
	int (*ApeReadBase)(void* ptr ,int size ,int nitems , FILE* strem);
	int (*ApeSeekBase)(FILE* stream , long offset , int fromwhere);
	int (*ApeTellBase)(FILE* stream);
	int (*ApeWriteBase)(void* ptr ,int size, int nitems, FILE* stream);
}tAPEFileIo;
#endif

typedef struct
{

  //DecoderList *workspace;
  //eDecoderStatus status;
  long             status;
  //sDecoderBitstream bitstream;
  //sDecoderOutput output;

  //tmInstance *pmInstance;
  //tmScratch *pmScratch;


  char pcEncodedData[ENCODED_DATA_SIZE];//编码的输出Buffer,解码的输入Buffer,(4096+16)*4 bytes<=> ape_uint32 ape_gInputBuffer[BLOCKS_PER_BITSTREAMREAD+16]


  short psLeft[PCM_BUFFER_SIZE];//编码的左声道输入Buffer,解码的输出左声道Buffer
  short psRight[PCM_BUFFER_SIZE];//编码的右声道输入Buffer,解码的输出右声道Buffer

  //BufferState *pOutput;
  int buffer_ptr[2];

  //unsigned short usValid;

  unsigned long usSampleRate;

  unsigned char ucChannels;

  //unsigned char ucIsVBR;

  //unsigned long ulFirstFrame;

  unsigned long ulLength;

  // The number of samples in each encoded block of audio.
  unsigned short usSamplesPerBlock;	// block

  // The length of the file in milliseconds.
  unsigned long ulTimeLength;

  unsigned long ulBitRate;

  // The number of samples that have been encoded/decoded.
  unsigned long ulTimePos;

} tAPE;


extern tAPE gAPEStruct;           // APE解码输出结构体
extern void *pApeRawFileCache;       // APE文件指针

extern long ApeFread(void *pHandle, void *pBuf, unsigned long size);
extern long ApeFseek(void *pHandle, long offset, unsigned long mode);
extern long ApeFtell(void *pHandle);

#endif
