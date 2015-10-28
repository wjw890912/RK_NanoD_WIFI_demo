#ifndef APE_HEADER_H
#define APE_HEADER_H
#include "ape_io1.h"

/*****************************************************************************************
APE header that all APE files have in common (old and new)
*****************************************************************************************/
struct APE_COMMON_HEADER
{
    ape_char cID[4];                            // should equal 'MAC '//20070528 fixed
    ape_uint16 nVersion;                        // version number * 1000 (3.81 = 3810)
};

/*****************************************************************************************
APE header structure for old APE files (3.97 and earlier)
*****************************************************************************************/
struct APE_HEADER_OLD
{
    ape_char cID[4];                            // should equal 'MAC '
    ape_uint16 nVersion;                        // version number * 1000 (3.81 = 3810)
    ape_uint16 nCompressionLevel;               // the compression level
    ape_uint16 nFormatFlags;                    // any format flags (for future use)
    ape_uint16 nChannels;                       // the number of channels (1 or 2)
    ape_uint32 nSampleRate;                     // the sample rate (typically 44100)
    ape_uint32 nHeaderBytes;                    // the bytes after the MAC header that compose the WAV header
    ape_uint32 nTerminatingBytes;               // the bytes after that raw data (for extended info)
    ape_uint32 nTotalFrames;                    // the number of frames in the file
    ape_uint32 nFinalFrameBlocks;               // the number of samples in the final frame
};

struct APE_FILE_INFO;
//class CIO;

/*****************************************************************************************
CAPEHeader - makes managing APE headers a little smoother (and the format change as of 3.98)
*****************************************************************************************/

typedef struct
{
    void      (*cCAPEHeader)(void *,CIO * pIO);
    void      (*dCAPEHeader)(void *);
    ape_int32 (*Analyze)(void *,struct APE_FILE_INFO * pInfo);
    ape_int32 (*AnalyzeCurrent)(void *,struct APE_FILE_INFO * pInfo);
    ape_int32 (*AnalyzeOld)(void *,struct APE_FILE_INFO * pInfo);
    ape_int32 (*FindDescriptor)(void *,ape_BOOL bSeek);
    CIO * Ape_pIO;
}CAPEHeader;
#ifdef __cplusplus
    extern "C" {
#endif
void      ApeHeaderInitialize(CAPEHeader *aI,CIO * pIO);
void      ApeHeaderDelete(CAPEHeader *aI);
ape_int32 ApeHeaderAnalyze(CAPEHeader *aI,struct APE_FILE_INFO * pInfo);
ape_int32 ApeHeaderAnalyzeCurrent(CAPEHeader *aI,struct APE_FILE_INFO * pInfo);
ape_int32 ApeHeaderAnalyzeOld(CAPEHeader *aI,struct APE_FILE_INFO * pInfo);
ape_int32 ApeHeaderFindDescriptor(CAPEHeader *aI,ape_BOOL bSeek);
#ifdef __cplusplus
	}
#endif

#endif // #ifndef APE_HEADER_H

