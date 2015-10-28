/*****************************************************************************************
ape_Infodec.h
Copyright (C) 2000 by Matthew T. Ashland   All Rights Reserved.

Simple method for working with APE files... it encapsulates reading, writing and getting
file information.  Just create a CAPEInfo class, call OpenFile(), and use the class methods
to do whatever you need... the destructor will take care of any cleanup

Notes:
    -Most all functions return 0 upon success, and some error code (other than 0) on
    failure.  However, all of the file functions that are wrapped from the Win32 API
    return 0 on failure and some other number on success.  This applies to ReadFile,
    WriteFile, SetFilePointer, etc...

WARNING:
    -This class driven system for using Monkey's Audio is still in development, so
    I can't make any guarantees that the classes and libraries won't change before
    everything gets finalized.  Use them at your own risk
*****************************************************************************************/

#ifndef APE_APEINFO_H
#define APE_APEINFO_H

#include "ape_io1.h"
#include "ape_tag.h"
#include "ape_maclib.h"

/*****************************************************************************************
APE_FILE_INFO - structure which describes most aspects of an APE file
(used internally for speed and ease)
*****************************************************************************************/
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

/*****************************************************************************************
Helper macros (sort of hacky)
*****************************************************************************************/
#define GET_USES_CRC(APE_INFO) (((APE_INFO)->GetInfo(APE_INFO_FORMAT_FLAGS) & MAC_FORMAT_FLAG_CRC) ? TRUE : FALSE)
#define GET_FRAMES_START_ON_BYTES_BOUNDARIES(APE_INFO) (((APE_INFO)->GetInfo(APE_INFO_FILE_VERSION) > 3800) ? TRUE : FALSE)
#define GET_USES_SPECIAL_FRAMES(APE_INFO) (((APE_INFO)->GetInfo(APE_INFO_FILE_VERSION) > 3820) ? TRUE : FALSE)
#define GET_IO(APE_INFO) ((CIO *) (APE_INFO)->GetInfo(APE_INFO_IO_SOURCE))
#define GET_TAG(APE_INFO) ((CAPETag *) (APE_INFO)->GetInfo(APE_INFO_TAG))

/*****************************************************************************************
CAPEInfo - use this for all work with APE files
*****************************************************************************************/

typedef struct
{
  void      (*cCAPEInfoFile)(void *,ape_int32* pErrorCode, CAPETag * pTag);
  void      (*cCAPEInfoIO)(void *,ape_int32 * pErrorCode, CIO * pIO, CAPETag * pTag);
  void      (*dCAPEInfo)(void *);
  ape_int32 (*GetInfo)(void *,enum APE_DECOMPRESS_FIELDS Field, ape_int32 nParam1 , ape_int32 nParam2 );
  ape_int32 (*GetFileInformation)(void *,ape_BOOL bGetTagInformation );
  ape_int32 (*CloseFile)(void *);
  ape_BOOL m_bHasFileInformationLoaded;
  CIO* m_spIO;
  CAPETag* m_spAPETag;
  struct APE_FILE_INFO    m_APEFileInfo;
}CAPEInfo;

#ifdef __cplusplus
extern "C" {
#endif
  void      ApeInfoFile(CAPEInfo *aI,ape_int32 * pErrorCode, CAPETag * pTag );
  void      ApeInfoIo(CAPEInfo *aI,ape_int32 * pErrorCode, CIO * pIO, CAPETag * pTag );
  void ApeInfoDelete(CAPEInfo *aI);
  ape_int32 ApeInfoGetInfo(CAPEInfo *aI,enum APE_DECOMPRESS_FIELDS Field, ape_int32 nParam1 , ape_int32 nParam2);
  ape_int32 ApeInfoGetFileInfo(CAPEInfo *aI,ape_BOOL bGetTagInformation );
  ape_int32 ApeInfoCloseFile(CAPEInfo *aI);
#ifdef __cplusplus
}
#endif


#endif // #ifndef APE_APEINFO_H
