/*****************************************************************************************
Monkey's Audio ape_maclib.h (include for using MACLib.lib in your projects)
Copyright (C) 2000-2003 by Matthew T. Ashland   All Rights Reserved.

Overview:

There are two main interfaces... create one (using CreateIAPExxx) and go to town:

    IAPECompress - for creating APE files
    IAPEDecompress - for decompressing and analyzing APE files

Note(s):

Unless otherwise specified, functions return ERROR_SUCCESS (0) on success and an
error code on failure.

The terminology "Sample" refers to a single sample value, and "Block" refers
to a collection    of "Channel" samples.  For simplicity, MAC typically uses blocks
everywhere so that channel mis-alignment cannot happen. (i.e. on a CD, a sample is
2 bytes and a block is 4 bytes ([2 bytes per sample] * [2 channels] = 4 bytes))

Questions / Suggestions:

Please direct questions or comments to the Monkey's Audio developers board:
http://www.monkeysaudio.com/cgi-bin/YaBB/YaBB.cgi -> Developers
or, if necessary, matt @ monkeysaudio.com
*****************************************************************************************/

#ifndef APE_MACLIB_H
#define APE_MACLIB_H
#include "ape_io1.h"

/*************************************************************************************************
APE File Format Overview: (pieces in order -- only valid for the latest version APE files)

    JUNK - any amount of "junk" before the APE_DESCRIPTOR (so people that put ID3v2 tags on the files aren't hosed)
    APE_DESCRIPTOR - defines the sizes (and offsets) of all the pieces, as well as the MD5 checksum
    APE_HEADER - describes all of the necessary information about the APE file
    SEEK TABLE - the table that represents seek offsets [optional]
    HEADER DATA - the pre-audio data from the original file [optional]
    APE FRAMES - the actual compressed audio (broken into frames for seekability)
    TERMINATING DATA - the post-audio data from the original file [optional]
    TAG - describes all the properties of the file [optional]

Notes:

    Junk:

    This block may not be supported in the future, so don't write any software that adds meta data
    before the APE_DESCRIPTOR.  Please use the APE Tag for any meta data.

    Seek Table:

    A 32-bit unsigned integer array of offsets from the header to the frame data.  May become "delta"
    values someday to better suit huge files.

    MD5 Hash:

    Since the header is the last part written to an APE file, you must calculate the MD5 checksum out of order.
    So, you first calculate from the tail of the seek table to the end of the terminating data.
    Then, go back and do from the end of the descriptor to the tail of the seek table.
    You may wish to just cache the header data when starting and run it last, so you don't
    need to seek back in the I/O.
*************************************************************************************************/

#include "ape_all.h"

/*****************************************************************************************
Defines
*****************************************************************************************/


#define MAC_FORMAT_FLAG_8_BIT                 1    // is 8-bit [OBSOLETE]
#define MAC_FORMAT_FLAG_CRC                   2    // uses the new CRC32 error detection [OBSOLETE]
#define MAC_FORMAT_FLAG_HAS_PEAK_LEVEL        4    // ape_uint32 nPeakLevel after the header [OBSOLETE]
#define MAC_FORMAT_FLAG_24_BIT                8    // is 24-bit [OBSOLETE]
#define MAC_FORMAT_FLAG_HAS_SEEK_ELEMENTS    16    // has the number of seek elements after the peak level
#define MAC_FORMAT_FLAG_CREATE_WAV_HEADER    32    // create the wave header on decompression (not stored)

#define CREATE_WAV_HEADER_ON_DECOMPRESSION    -1
#define MAX_AUDIO_BYTES_UNKNOWN -1


/*****************************************************************************************
WAV header structure
*****************************************************************************************/
struct WAVE_HEADER
{
  // RIFF header
  ape_char cRIFFHeader[4];//20070528 fixed
  ape_uint32 nRIFFBytes;

  // data type
  ape_char cDataTypeID[4];//20070528 fixed

  // wave format
  ape_char cFormatHeader[4];//20070528 fixed
  ape_uint32 nFormatBytes;

  ape_uint16 nFormatTag;
  ape_uint16 nChannels;
  ape_uint32 nSamplesPerSec;
  ape_uint32 nAvgBytesPerSec;
  ape_uint16 nBlockAlign;
  ape_uint16 nBitsPerSample;

  // data chunk header
  ape_char cDataHeader[4];//20070528 fixed
  ape_uint32 nDataBytes;
};

/*****************************************************************************************
APE_DESCRIPTOR structure (file header that describes lengths, offsets, etc.)
*****************************************************************************************/
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

  //uint8   cFileMD5[16];                       // the MD5 hash of the file (see notes for usage... it's a littly tricky)
  ape_uchar cFileMD5[16];//20070528 fixed
  //ape_uchar cFileMD5[8];//20070528 fixed
};

/*****************************************************************************************
APE_HEADER structure (describes the format, duration, etc. of the APE file)
*****************************************************************************************/
struct APE_HEADER
{
  ape_uint16    nCompressionLevel;                 // the compression level (see defines I.E. COMPRESSION_LEVEL_FAST)
  ape_uint16    nFormatFlags;                      // any format flags (for future use)

  ape_uint32    nBlocksPerFrame;                   // the number of audio blocks in one frame
  ape_uint32    nFinalFrameBlocks;                 // the number of audio blocks in the final frame
  ape_uint32    nTotalFrames;                      // the total number of frames

  ape_uint16    nBitsPerSample;                    // the bits per sample (typically 16)
  ape_uint16    nChannels;                         // the number of channels (1 or 2)
  ape_uint32    nSampleRate;                       // the sample rate (typically 44100)
};

/*************************************************************************************************
Classes (fully defined elsewhere)
*************************************************************************************************/
//class CIO;
//class CInputSource;
//class CAPEInfo;

/*************************************************************************************************
IAPEDecompress fields - used when querying for information

Note(s):
-the distinction between APE_INFO_XXXX and APE_DECOMPRESS_XXXX is that the first is querying the APE
information engine, and the other is querying the decompressor, and since the decompressor can be
a range of an APE file (for APL), differences will arise.  Typically, use the APE_DECOMPRESS_XXXX
fields when querying for info about the length, etc. so APL will work properly.
(i.e. (APE_INFO_TOTAL_BLOCKS != APE_DECOMPRESS_TOTAL_BLOCKS) for APL files)
*************************************************************************************************/
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

/*************************************************************************************************
IAPEDecompress - interface for working with existing APE files (decoding, seeking, analyzing, etc.)
*************************************************************************************************/

typedef struct
{
  void      (*cIAPEDecompress)(ape_int32 * pErrorCode, void  * pAPEInfo, ape_int32 nStartBlock, ape_int32 nFinishBlock);
  void      (*dIAPEDecompress)();
  ape_int32 (*GetData)(void*, ape_int32 nBlocks, ape_int32 * pBlocksRetrieved);
  ape_int32 (*Seek)(void *,ape_int32 nBlockOffset);
  ape_int32 (*GetInfo)(ape_int32 Field, ape_int32 nParam1, ape_int32 nParam2) ;
}IAPEDecompress;

#ifdef __cplusplus
extern "C"
{
#endif
  void  ApeDecompressCreate(ape_int32 * pErrorCode);
  //void * CreateIAPEDecompressEx(struct CIO * pIO, int * pErrorCode);
  //void * CreateIAPEDecompressEx(CIO * pIO, ape_int32 * pErrorCode);
  //IAPEDecompress *  CreateIAPEDecompressEx2(CAPEInfo * pAPEInfo, ape_int32 nStartBlock = -1, ape_int32 nFinishBlock = -1, ape_int32 * pErrorCode = NULL);
  //IAPECompress *  CreateIAPECompress(ape_int32 * pErrorCode = NULL);
#ifdef __cplusplus
}
#endif

/*************************************************************************************************
Simple functions - see the SDK sample projects for usage examples
*************************************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
  //ape_int32  DecompressFileW(ape_char * pInputFilename, ape_char * pOutputFilename, ape_int32 * pPercentageDone, APE_PROGRESS_CALLBACK ProgressCallback, ape_int32 * pKillFlag);
  ape_int32 ApeFillWaveFormatEx(WAVEFORMATEX * pWaveFormatEx, ape_int32 nSampleRate , ape_int32 nBitsPerSample , ape_int32 nChannels );
  ape_int32 ApeFillWaveHeader(struct WAVE_HEADER * pWAVHeader, ape_int32 nAudioBytes, WAVEFORMATEX * pWaveFormatEx, ape_int32 nTerminatingBytes);
#ifdef __cplusplus
}
#endif

#endif // #ifndef APE_MACLIB_H
