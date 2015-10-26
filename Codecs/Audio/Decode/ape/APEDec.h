/* Copyright (C) 2007 ROCK-CHIPS FUZHOU . All Rights Reserved. */
/*
File : \Audio\APEDec
Desc : head file for APE format decode.

Author : huangxd , Vincent Hisung
Date : 2007-08-xx
Notes :

$Log :
* huangxd . create the file at 08.xx.2007
*
* vincent .	amendment at 08.xx.2007.
*
*/
/****************************************************************/
#ifndef _APE_DEC_H_
#define _APE_DEC_H_

//#include "../buffer/buffer.h"
//#include <stdio.h>

#if 0

enum
{
    SUBFN_CODEC_GETNAME,
    SUBFN_CODEC_GETARTIST,
    SUBFN_CODEC_GETTITLE,
    SUBFN_CODEC_GETBITRATE,
    SUBFN_CODEC_GETSAMPLERATE,
    SUBFN_CODEC_GETCHANNELS,
    SUBFN_CODEC_GETLENGTH,
    SUBFN_CODEC_GETTIME,
    SUBFN_CODEC_OPEN_DEC,
    SUBFN_CODEC_OPEN_ENC,
    SUBFN_CODEC_GETCAPTUREBUFFER,
    SUBFN_CODEC_SETBUFFER,
    SUBFN_CODEC_DECODE,
    SUBFN_CODEC_ENCODE,
    SUBFN_CODEC_SEEK,
    SUBFN_CODEC_CLOSE,
    SUBFN_CODEC_STORETIME            // add  by huangzf
};
#endif

#ifdef TRUE
#undef TRUE
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifdef FALSE
#undef FALSE
#endif

#ifndef FALSE
#define FALSE 0
#endif


//////////////////////////////////
#define APE_BLOCKS_PER_BITSTREAMREAD        2048
#define APE_BLOCKS_PER_DECODE               1152
#define MAX_PATH    260
//#define TRUE    1
//#define FALSE    0
#define ERROR_SUCCESS                                   0
#define ERROR_INVALID_CHECKSUM                          1009
#define ERROR_DECOMPRESSING_FRAME                       1010
//-----------------------------------

typedef unsigned short   ape_uint16;
typedef short            ape_int16;
typedef unsigned long    ape_uint32;
typedef long             ape_int32;
typedef unsigned char    ape_uchar;
typedef char             ape_char;
typedef long             ape_BOOL;
typedef void *                HANDLE;

enum APE_DECOMPRESS_FIELDS
{
    APE_INFO_FILE_VERSION = 1000,               // version of the APE file * 1000 (3.93 = 3930) [ignored, ignored]
    APE_INFO_COMPRESSION_LEVEL = 1001,          // compression level of the APE file [ignored, ignored]
    APE_INFO_FORMAT_FLAGS = 1002,               // format flags of the APE file [ignored, ignored]
    APE_INFO_SAMPLE_RATE = 1003,                // sample rate (Hz) [ignored, ignored]
    APE_INFO_BITS_PER_SAMPLE = 1004,            // bits per sample [ignored, ignored]
    APE_INFO_BYTES_PER_SAMPLE = 1005,           // number of bytes per sample [ignored, ignored]
    APE_INFO_CHANNELS = 1006,                   // channels [ignored, ignored]
    APE_INFO_BLOCK_ALIGN = 1007,                // block alignment [ignored, ignored]
    APE_INFO_BLOCKS_PER_FRAME = 1008,           // number of blocks in a frame (frames are used internally)  [ignored, ignored]
    APE_INFO_FINAL_FRAME_BLOCKS = 1009,         // blocks in the final frame (frames are used internally) [ignored, ignored]
    APE_INFO_TOTAL_FRAMES = 1010,               // total number frames (frames are used internally) [ignored, ignored]
    APE_INFO_WAV_HEADER_BYTES = 1011,           // header bytes of the decompressed WAV [ignored, ignored]
    APE_INFO_WAV_TERMINATING_BYTES = 1012,      // terminating bytes of the decompressed WAV [ignored, ignored]
    APE_INFO_WAV_DATA_BYTES = 1013,             // data bytes of the decompressed WAV [ignored, ignored]
    APE_INFO_WAV_TOTAL_BYTES = 1014,            // total bytes of the decompressed WAV [ignored, ignored]
    APE_INFO_APE_TOTAL_BYTES = 1015,            // total bytes of the APE file [ignored, ignored]
    APE_INFO_TOTAL_BLOCKS = 1016,               // total blocks of audio data [ignored, ignored]
    APE_INFO_LENGTH_MS = 1017,                  // length in ms (1 sec = 1000 ms) [ignored, ignored]
    APE_INFO_AVERAGE_BITRATE = 1018,            // average bitrate of the APE [ignored, ignored]
    APE_INFO_FRAME_BITRATE = 1019,              // bitrate of specified APE frame [frame index, ignored]
    APE_INFO_DECOMPRESSED_BITRATE = 1020,       // bitrate of the decompressed WAV [ignored, ignored]
    APE_INFO_PEAK_LEVEL = 1021,                 // peak audio level (obsolete) (-1 is unknown) [ignored, ignored]
    APE_INFO_SEEK_BIT = 1022,                   // bit offset [frame index, ignored]
    APE_INFO_SEEK_BYTE = 1023,                  // byte offset [frame index, ignored]
    APE_INFO_WAV_HEADER_DATA = 1024,            // error code [buffer *, max bytes]
    APE_INFO_WAV_TERMINATING_DATA = 1025,       // error code [buffer *, max bytes]
    APE_INFO_WAVEFORMATEX = 1026,               // error code [waveformatex *, ignored]
    APE_INFO_IO_SOURCE = 1027,                  // I/O source (CIO *) [ignored, ignored]
    APE_INFO_FRAME_BYTES = 1028,                // bytes (compressed) of the frame [frame index, ignored]
    APE_INFO_FRAME_BLOCKS = 1029,               // blocks in a given frame [frame index, ignored]
    APE_INFO_TAG = 1030,                        // point to tag (CAPETag *) [ignored, ignored]

    APE_DECOMPRESS_CURRENT_BLOCK = 2000,        // current block location [ignored, ignored]
    APE_DECOMPRESS_CURRENT_MS = 2001,           // current millisecond location [ignored, ignored]
    APE_DECOMPRESS_TOTAL_BLOCKS = 2002,         // total blocks in the decompressors range [ignored, ignored]
    APE_DECOMPRESS_LENGTH_MS = 2003,            // total blocks in the decompressors range [ignored, ignored]
    APE_DECOMPRESS_CURRENT_BITRATE = 2004,      // current bitrate [ignored, ignored]
    APE_DECOMPRESS_AVERAGE_BITRATE = 2005,      // average bitrate (works with ranges) [ignored, ignored]

    APE_INTERNAL_INFO = 3000                    // for internal use -- don't use (returns APE_FILE_INFO *) [ignored, ignored]
};

struct ID3_TAG
{
    ape_char Header[3];             // should equal 'TAG'
    ape_char Title[30];             // title
    ape_char Artist[30];            // artist
    ape_char Album[30];             // album
    ape_char Year[4];               // year
    ape_char Comment[29];           // comment
    ape_uchar Track;        // track
    ape_uchar Genre;        // genre
};

typedef struct
{
    ape_int32(*Open)(void *);
    ape_int32(*Close)(void *);
    ape_int32(*Read)(void *, void * , unsigned int , unsigned int *);
    ape_int32(*Write)(void *, void * , unsigned int , unsigned int *);
    ape_int32(*Seek)(void *, unsigned int nDistance, unsigned int nMoveMode);
    ape_int32(*SetEOF)(void *);
    ape_int32(*Create)(void *, char * pName);
    ape_int32(*Delete)(void *);
    ape_int32(*GetPosition)(void *);
    ape_int32(*GetSize)(void *);
    ape_int32(*GetName)(void *, char * pBuffer);

    HANDLE    m_hFile;
    ape_char  m_cFileName[MAX_PATH];
    ape_BOOL  m_bReadOnly;
}CIO;

typedef struct
{
    void (*cCAPETagIO)(void*, CIO * pIO, ape_BOOL bAnalyze);
    void (*cCAPETagFile)(void*, ape_char * pFilename, ape_BOOL bAnalyze);
    void (*dCAPETag)(void*);

    ape_int32(*GetTagBytes)(void*);
    ape_int32(*CreateID3Tag)(void*, struct ID3_TAG * pID3Tag);

    ape_int32(*Analyze)(void*);
    ape_int32(*GetTagFieldIndex)(void*, ape_char * pFieldName);
    ape_int32(*WriteBufferToEndOfIO)(void*, void * pBuffer, ape_int32 nBytes);
    ape_int32(*LoadField)(void*, ape_char * pBuffer, ape_int32 nMaximumBytes, ape_int32 * pBytes);
    ape_int32(*SortFields)(void*);
    ape_int32(*CompareFields)(void * pA,  void * pB);
    ape_BOOL(*GetHasID3Tag)(void*);
    ape_BOOL(*GetHasAPETag)(void*);
    ape_int32(*GetAPETagVersion)(void*);
    CIO *m_spIO;
    ape_BOOL m_bAnalyzed;
    ape_int32 m_nTagBytes;
    ape_int32 m_nFields;
    ape_char * m_aryFields[256];
    ape_BOOL m_bHasAPETag;
    ape_int32 m_nAPETagVersion;
    ape_BOOL m_bHasID3Tag;
    ape_BOOL m_bIgnoreReadOnly;
}CAPETag;

struct APE_DESCRIPTOR
{
    ape_char    cID[4];                             // should equal 'MAC '//20070528 fixed
    ape_uint16  nVersion;                           // version number * 1000 (3.81 = 3810)
    ape_uint16  tmp;

    ape_uint32  nDescriptorBytes;                   // the number of descriptor bytes (allows later expansion of this header)
    ape_uint32  nHeaderBytes;                       // the number of header APE_HEADER bytes
    ape_uint32  nSeekTableBytes;                    // the number of bytes of the seek table
    ape_uint32  nHeaderDataBytes;                   // the number of header data bytes (from original file)
    ape_uint32  nAPEFrameDataBytes;                 // the number of bytes of APE frame data
    ape_uint32  nAPEFrameDataBytesHigh;             // the high order number of APE frame data bytes
    ape_uint32  nTerminatingDataBytes;              // the terminating data of the file (not including tag data)

    ape_uchar cFileMD5[16];
};

struct APE_FILE_INFO
{
    ape_int32 nVersion;                                   // file version number * 1000 (3.93 = 3930)
    ape_int32 nCompressionLevel;                          // the compression level
    ape_int32 nFormatFlags;                               // format flags
    ape_int32 nTotalFrames;                               // the total number frames (frames are used internally)
    ape_int32 nBlocksPerFrame;                            // the samples in a frame (frames are used internally)
    ape_int32 nFinalFrameBlocks;                          // the number of samples in the final frame
    ape_int32 nChannels;                                  // audio channels
    ape_int32 nSampleRate;                                // audio samples per second
    ape_int32 nBitsPerSample;                             // audio bits per sample
    ape_int32 nBytesPerSample;                            // audio bytes per sample
    ape_int32 nBlockAlign;                                // audio block align (channels * bytes per sample)
    ape_int32 nWAVHeaderBytes;                            // header bytes of the original WAV
    ape_int32 nWAVDataBytes;                              // data bytes of the original WAV
    ape_int32 nWAVTerminatingBytes;                       // terminating bytes of the original WAV
    ape_int32 nWAVTotalBytes;                             // total bytes of the original WAV
    ape_int32 nAPETotalBytes;                             // total bytes of the APE file
    ape_int32 nTotalBlocks;                               // the total number audio blocks
    ape_int32 nLengthMS;                                  // the length in milliseconds
    ape_int32 nAverageBitrate;                            // the kbps (i.e. 637 kpbs)
    ape_int32 nDecompressedBitrate;                       // the kbps of the decompressed audio (i.e. 1440 kpbs for CD audio)
    ape_int32 nJunkHeaderBytes;                           // used for ID3v2, etc.
    ape_int32 nSeekTableElements;                         // the number of elements in the seek table(s)

    /*Mod by Wei.Hisung 2007.03.06*/
    ape_uint32* spSeekByteTable;              // the seek table (byte)
    ape_uchar* spSeekBitTable;        // the seek table (bits -- legacy)
    ape_uchar* spWaveHeaderData;      // the pre-audio header data
    struct APE_DESCRIPTOR* spAPEDescriptor;      // the descriptor (only with newer files)
};

typedef struct
{
    void (*cCAPEInfoFile)(void *, ape_int32* pErrorCode, CAPETag * pTag);
    void (*cCAPEInfoIO)(void *, ape_int32 * pErrorCode, CIO * pIO, CAPETag * pTag);
    void (*dCAPEInfo)(void *);
    ape_int32(*GetInfo)(void *, enum APE_DECOMPRESS_FIELDS Field, ape_int32 nParam1 , ape_int32 nParam2);
    ape_int32(*GetFileInformation)(void *, ape_BOOL bGetTagInformation);
    ape_int32(*CloseFile)(void *);
    ape_BOOL m_bHasFileInformationLoaded;
    CIO* m_spIO;
    CAPETag* m_spAPETag;
    struct APE_FILE_INFO    m_APEFileInfo;
}CAPEInfo;

#undef WORD
#undef DWORD

typedef unsigned short      WORD;
typedef unsigned long       DWORD;

typedef struct tWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
    /* extra information (after cbSize) */
} WAVEFORMATEX;

typedef struct
{
    void (*cIAPEDecompress)(ape_int32 * pErrorCode, void  * pAPEInfo, ape_int32 nStartBlock, ape_int32 nFinishBlock);
    void (*dIAPEDecompress)();
    ape_int32(*GetData)(void*, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
    ape_int32(*Seek)(void *, ape_int32 nBlockOffset);
    ape_int32(*GetInfo)(ape_int32 Field, ape_int32 nParam1, ape_int32 nParam2) ;
}IAPEDecompress;

extern IAPEDecompress* Ape_pApeDecompress;
extern CAPEInfo Ape_gDecompressInfoCreate;
extern ape_int32   Ape_gBlocksLeft;
extern ape_uint16  Ape_gDecodeErrorFlag;
extern ape_uint16* Ape_pOutBufferLeft;
extern ape_uint16* Ape_pOutBufferRight;
extern ape_int32   Ape_gTotalBlocks;
extern ape_int32  Ape_gStartBlock;
extern ape_int32  Ape_gCurrentBlock;
extern ape_BOOL   Ape_gDecompressorInitial;
extern ape_int16   Ape_gBeginDecodeFrameFlag;

//warn by WeiGuo Hu:the two Macro below must be consist with macro CM_BUFFER_SIZE\ENCODED_DATA_SIZE in file ape_lib\pAPE_DEC.h

#define  APE_PCM_BUFFER_SIZE    (APE_BLOCKS_PER_DECODE)//not support 32 bits PCM output.
#define  APE_ENCODED_DATA_SIZE  ((APE_BLOCKS_PER_BITSTREAMREAD+16)*4)

#if 0
///定义文件操作结构体类型,其成员为函数指针类型 added by hxd 20070723
typedef struct
{
    int (*ApeReadBase)(void* ptr , int size , int nitems , MY_FILE* strem);
    int (*ApeSeekBase)(MY_FILE* stream , long offset , int fromwhere);
    int (*ApeTellBase)(MY_FILE* stream);
    int (*ApeWriteBase)(void* ptr , int size, int nitems, MY_FILE* stream);
}tAPEFileIo;
#endif

typedef struct
{
    long             status;

    char pcEncodedData[APE_ENCODED_DATA_SIZE];//Buffer for coding input and decoding output.(4096+16)*4 bytes<=> ape_uint32 ape_gInputBuffer[BLOCKS_PER_BITSTREAMREAD+16]


    short psLeft[APE_PCM_BUFFER_SIZE]; //Buffer for coding input and decoding output of left channel.
    short psRight[APE_PCM_BUFFER_SIZE];//Buffer for coding input and decoding output of right channel.

    //BufferState *pOutput;
    int buffer_ptr[2];

    unsigned long usSampleRate;

    unsigned char ucChannels;

    unsigned long ulLength;

    // The number of samples in each encoded block of audio.
    unsigned short usSamplesPerBlock; // block

    // The length of the file in milliseconds.
    unsigned long ulTimeLength;

    unsigned long ulBitRate;

    // The number of samples that have been encoded/decoded.
    unsigned long ulTimePos;
    unsigned long bitPerSamp;

} tAPE;

extern tAPE gAPEStruct;           //decoding output struct for APE format.
extern void *pApeRawFileCache;    //ape file pointer.

//add by Vincent Hisung ,Nov 6,2007
extern int ApeUnBitArrayFillBitArrayOutside(void);
//extern IAPEDecompress *ApeDecompressCoreCreate();
extern void * ApeDecompressCoreCreate(CAPEInfo * , ape_int32 , ape_int32 , ape_int32 *);


#endif
