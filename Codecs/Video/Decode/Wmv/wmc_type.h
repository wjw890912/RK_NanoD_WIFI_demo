//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
#ifndef _WMC_TYPES_H_
#define _WMC_TYPES_H_
//#pragma pack (1)


#define __NO_SCREEN__
#define __NO_SPEECH__
#define  _V9WMADEC_
#define  __SLIM_WMV9__
#define WMVA_NEW_ENTROPY_CODING
#define SMALL_CODE_SIZE
#define __STREAMING_MODE_DECODE_
#define  OPT_DECOLORCONV_ARM
#define OUTPUT_ALL_FORMATS
#define    OUTPUT_RGB12
#define PACKED



#define WMCAPI_NO_DRM

/*********  Basic type Definitions  *************/

#ifndef WMC_TYPE_DEFINED
#define WMC_TYPE_DEFINED

typedef void Void_WMC;
typedef long I32_WMC;
typedef unsigned long U32_WMC;
typedef short I16_WMC;
typedef unsigned short U16_WMC;

#if defined(_Embedded_x86)
typedef unsigned short U16Char_WMC;
#elif defined(macintosh)
typedef wchar_t U16Char_WMC;
#else
typedef unsigned short U16Char_WMC;
#endif

typedef char I8_WMC;
typedef unsigned char U8_WMC;
typedef long Bool_WMC;
typedef double Double_WMC;
typedef float Float_WMC;
typedef  struct tQWORD_WMC
{
    U32_WMC   dwLo;
    U32_WMC   dwHi;
}   QWORD_WMC;

#ifdef macintosh
typedef unsigned long long QWORD;
#endif

#ifdef _XBOX
typedef unsigned __int64    U64_WMC;
typedef __int64    I64_WMC;
#elif defined(macintosh) || defined(_Embedded_x86)
typedef unsigned long long  U64_WMC;
typedef long long  I64_WMC;
#elif defined(HITACHI)
typedef  struct tU64_WMC
{
    U32_WMC   dwLo;
    U32_WMC   dwHi;
}   U64_WMC;
typedef  struct tI64_WMC
{
    I32_WMC   dwLo;
    I32_WMC   dwHi;
}   I64_WMC;
#else
typedef unsigned __int64    U64_WMC;
typedef __int64    I64_WMC;
#endif

typedef  struct tRect_WMC
{
    I32_WMC iLeft;
    I32_WMC iTop;
    I32_WMC iRight;
    I32_WMC iBottom;
}   Rect_WMC;

#define TRUE_WMC    1
#define FALSE_WMC   0
#define NULL_WMC   0
#define MIN_WANTED 512
typedef void * HWMCFILE;
typedef     enum tagMediaType_WMC
{
    Audio_WMC,
    Video_WMC,
    Binary_WMC
} tMediaType_WMC;

typedef  struct tagStreamIdnMediaType_WMC
{
    U16_WMC wStreamId;
    tMediaType_WMC MediaType;
} tStreamIdnMediaType_WMC;

typedef     enum tagOutputType_WMC
{
    Discard_WMC,
    Compressed_WMC,
    Decompressed_WMC
} tOutputType_WMC;
/************************************************************************/
#ifndef MAKEFOURCC_WMC
#define MAKEFOURCC_WMC(ch0, ch1, ch2, ch3) \
        ((U32_WMC)(U8_WMC)(ch0) | ((U32_WMC)(U8_WMC)(ch1) << 8) |   \
        ((U32_WMC)(U8_WMC)(ch2) << 16) | ((U32_WMC)(U8_WMC)(ch3) << 24 ))

#define mmioFOURCC_WMC(ch0, ch1, ch2, ch3)  MAKEFOURCC_WMC(ch0, ch1, ch2, ch3)
#endif




/******* video output type guids, in preference order  *****/
#define FOURCC_WMVP     0x50564d57//mmioFOURCC_WMC('W','M','V','P')
#define FOURCC_WMV3     0x33564d57//mmioFOURCC_WMC('W','M','V','3')
#define FOURCC_WMVA     0x41564d57//mmioFOURCC_WMC('W','M','V','A')
#define FOURCC_WVC1     0x31435657//mmioFOURCC_WMC('W','V','C','1')
#define FOURCC_WMV2     0x32564d57//mmioFOURCC_WMC('W','M','V','2')
#define FOURCC_WMV1     0x31564d57//mmioFOURCC_WMC('W','M','V','1')
#define FOURCC_M4S2     0x3253344d//mmioFOURCC_WMC('M','4','S','2')
#define FOURCC_MP43     0x3334504d// mmioFOURCC_WMC('M','P','4','3')
#define FOURCC_mp43     0x33346d6d//mmioFOURCC_WMC('m','m','4','3')
#define FOURCC_MP4S     0x5334504d//mmioFOURCC_WMC('M','P','4','S')
#define FOURCC_mp4s    0x7334706d// mmioFOURCC_WMC('m','p','4','s')
#define FOURCC_MP42    0x3234504d// mmioFOURCC_WMC('M','P','4','2')
#define FOURCC_mp42    0x32346d6d// mmioFOURCC_WMC('m','m','4','2')
#define FOURCC_MSS1     0x3153534d//mmioFOURCC_WMC('M','S','S','1')
#define FOURCC_MSS2    0x3253534d// mmioFOURCC_WMC('M','S','S','2')
#define FOURCC_WMS2    0x32534d57// mmioFOURCC_WMC('W','M','S','2')
#define FOURCC_WMS1    0x31534d57// mmioFOURCC_WMC('W','M','S','1')
#define FOURCC_MPG4    0x3447504d// mmioFOURCC_WMC('M','P','G','4')
#define FOURCC_MSS1    0x3153534d// mmioFOURCC_WMC('M','S','S','1')
#define FOURCC_MSS2    0x3253534d// mmioFOURCC_WMC('M','S','S','2')

/***********************************************************/


/******* video intput type guids, in preference order  *****/

#define FOURCC_I420             0x30323449
#define FOURCC_IYUV             0x56555949
#define FOURCC_YV12             0x32315659
#define FOURCC_YUY2             0x32595559
#define FOURCC_UYVY             0x59565955
#define FOURCC_YVYU             0x55595659
#define FOURCC_YVU9             0x39555659
#define FOURCC_BI_RGB           0x00000000
#define FOURCC_BI_BITFIELDS     0x00000003
#define FOURCC_BI_RLE8          0x00000001
#define FOURCC_BI_RLE4          0x00000002


////////////////////////////////////////////

#define WAVE_FORMAT_PCM_WMC     0x0001
#define WAVE_FORMAT_WMA1_WMC    0x0160
#define WAVE_FORMAT_WMA8_WMC    0x0161
#define WAVE_FORMAT_WMA9_WMC    0x0162
#define WAVE_FORMAT_WMA9_LOSSLESS_WMC    0x0163
#define WAVE_FORMAT_WMSP9_WMC    0x000A

///////////////////////////////////////////

#define WMC_DRM_RIGHT_NONSDMI   0x02
#define WMC_DRM_RIGHT_SDMI      0x10

#define ARRAYSIZE(x) sizeof(x)/sizeof(x[0])

typedef  struct tagStreamIdPattern_WMC
{
    U16_WMC wStreamId;
    tOutputType_WMC tPattern;
} tStreamIdPattern_WMC;

typedef    struct tagPlannedOutputId_WMC
{
    U16_WMC wStreamIndex;
    tMediaType_WMC tMediaType;
    Bool_WMC    bDone;
} tPlannedOutputId_WMC;

typedef    struct tagPlannedOutput_WMC
{
    U16_WMC wTotalOutput;
    tPlannedOutputId_WMC tPlannedId[127];
} tPlannedOutputWMC;


typedef     enum tagVideoProfile_WMC
{
    SIMPLE_PROFILE = 0,
    MAIN_PROFILE,
    PC_PROFILE,
    MAIN_BROADCAST_PROFILE
} tVideoProfile_WMC;


typedef     enum tagVideoFormat_WMC
{
    // uncompressed
    YUY2_WMV, //0
    UYVY_WMV, //1
    YVYU_WMV, //2
    RGB24_WMV, //3
    RGB555_WMV, //4
    RGB565_WMV, //5
    RGB32_WMV, //6
    RGB8_WMV, //7
    IYUV_WMV, //8
    I420_WMV, //9
    YV12_WMV,//10
    RGB565_444_WMV,//11

    // compressed
    WMV2_WMV, //12
    WMV1_WMV, //13

    // Ignore Audio Only
    IGNORE_VIDEO //14
    /*
    WMS2_WMV,
    WMS1_WMV,
    M4S2_WMV,
    MP4S_WMV,
    MP43_WMV,
    MP42_WMV,
    MPG4_WMV
*/
} tVideoFormat_WMC;

typedef     enum tagWMCDecodeDispRotateDegree
{
    WMC_DispRotate0 = 0,
    WMC_DispRotate90,
    WMC_DispFlip,
    WMC_DispRotate270
} tWMCDecodeDispRotateDegree;



typedef     enum tagAudioType_WMC
{
    // uncompressed
    PCM_WMA,
    WMA3_WMA,
    WMA2_WMA,
    WMA1_WMA
} tAudioType_WMC;


typedef    struct strAudioStreamInfo_WMC
{
    U16_WMC         u16FormatTag;
    U32_WMC         u32BitsPerSecond;
    U16_WMC         u16BitsPerSample;
    U32_WMC         u32SamplesPerSecond;
    U16_WMC         u16NumChannels;
    U32_WMC         u32BlockSize;
    U16_WMC         u16ValidBitsPerSample; // bits of precision
    U32_WMC         u32ChannelMask;       // which channels are present in stream
    U16_WMC         u16StreamId;
} strAudioInfo_WMC;

typedef    struct strVideoStreamInfo_WMC
{
    U32_WMC           u32Compression;
    U32_WMC           u32BitsPerSecond;
    Float_WMC         fltFramesPerSecond; //is always 0
    I32_WMC           i32Width;
    I32_WMC           i32Height;
    U8_WMC            bPalette[3*256]; // Palette
    U16_WMC           u16StreamId;
    U16_WMC           u16VideoOutputBitCount;
    U32_WMC           u32VideoOutputPixelType;
} strVideoInfo_WMC;

typedef    struct strBinaryStreamInfo_WMC
{
    Bool_WMC         bNothing;
    U16_WMC          u16StreamId;
} strBinaryInfo_WMC;


typedef    struct strHeaderInfo_WMC
{
    U32_WMC     u32PlayDuration;
    U32_WMC     u32Preroll;
    U32_WMC     u32SendDuration;
    U32_WMC     u32MaxBitrate;
    U8_WMC      u8HasDRM;
    U32_WMC     u32PacketSize;
} strHeaderInfo_WMC;


typedef    struct tagWMCContentDescription
{
    /* *_len: as [in], they specify how large the corresponding
     *        buffers below are.
     *        as [out], they specify how large the returned
     *        buffers actually are.
     */

    U16_WMC uiTitle_len;
    U16_WMC uiAuthor_len;
    U16_WMC uiCopyright_len;
    U16_WMC uiDescription_len;   /* rarely used */
    U16_WMC uiRating_len;        /* rarely used */

    /* these are two-byte strings
     *   for ASCII-type character set, to convert these
     *   into a single-byte characters, simply skip
     *   over every other bytes.
     */

    U16Char_WMC *pchTitle;
    U16Char_WMC *pchAuthor;
    U16Char_WMC *pchCopyright;
    U16Char_WMC *pchDescription;
    U16Char_WMC *pchRating;

} WMCContentDescription;

/******************************************************************
**   Extended content description types:
**   ECD_STRING -- wchar string
**   ECD_BINARY -- binary (byte) data
**   ECD_BOOL   -- BOOL (int) data
**   ECD_WORD  -- contains one word,
**   ECD_DWORD  -- contains one dword,
**   ECD_QWORD  -- contains one qword,
*******************************************************************/

enum {
    ECD_STRING = 0,
    ECD_BINARY = 1,
    ECD_BOOL = 2,
    ECD_DWORD = 3,
    ECD_QWORD = 4,
    ECD_WORD = 5
};

typedef    struct _ECD_DESCRIPTOR
{
    U16_WMC         cbName;
    U16Char_WMC     *pwszName;
    U16_WMC         data_type;
    U16_WMC         cbValue;
      union
    {
        U16Char_WMC *pwszString;
        U8_WMC *pbBinary;
        Bool_WMC *pfBool;
        U32_WMC *pdwDword;
        U64_WMC *pqwQword;
        U16_WMC  *pwWord;
    } uValue;
} ECD_DESCRIPTOR;

typedef    struct tagWMCExtendedContentDescription
{
    U16_WMC cDescriptors;             // number of descriptors
    ECD_DESCRIPTOR *pDescriptors;  // pointer to all the descriptors
} WMCExtendedContentDesc;


typedef    struct _WMCCommandEntry
{
    U32_WMC         time;
    U16_WMC         type;
    U16Char_WMC     *param;
} WMCCommandEntry;

typedef     struct tagWMCScriptCommand
{
    U16_WMC num_commands;        // number of script commands
    U16_WMC num_types;           // number of types of commands
    U16Char_WMC **type_names;    // command names, URL, FILENAME, CAPTION, etc
    I32_WMC *type_name_len;      // length of the command name
    I32_WMC *command_param_len;  // length of command parameters
    WMCCommandEntry *commands;   // pointer to all the commands
} WMCScriptCommand;


typedef    struct tagWMCMarkerEntry
{
    U64_WMC     m_qOffset;
    U64_WMC     m_qtime;
    U16_WMC     m_wEntryLen;
    U32_WMC     m_dwSendTime;
    U32_WMC     m_dwFlags;
    U32_WMC     m_dwDescLen;
    U16Char_WMC *m_pwDescName;
} WMCMarkerEntry;

typedef    struct tagWMCIndexEntries
{
    U32_WMC dwPacket;
    U16_WMC wSpan;
} WMCINDEXENTRIES;


typedef    struct tagWMCIndexInfo
{
    U16_WMC nStreamId;
    U32_WMC   time_deltaMs;
    U32_WMC   max_packets;
    U32_WMC   num_entries;
    WMCINDEXENTRIES *pIndexEntries;
} WMCINDEXINFO;

typedef    struct tagWMCIndexBlocks
{
    U32_WMC dwNumEntries;
    U64_WMC *pqwBlockPosition;
    U32_WMC *pdwOffset;
} WMCINDEXBLOCKS;

typedef    struct tagWMCIndexSpecifier
{
    U16_WMC wStreamId;
    U16_WMC wIndexType;
} WMCINDEXSPECIFIER;

typedef    struct tagWMCNewIndex
{
    U32_WMC   dwTimeDelta;
    U16_WMC   wIndexSpecifierCount;
    U32_WMC   dwIndexBlockCount;
    U32_WMC   num_entries;
    WMCINDEXSPECIFIER *pIndexSpecifiers;
    WMCINDEXBLOCKS  *pIndexBlocks;
} WMCNEWINDEX;


typedef    struct tagWMCCodecEntry
{
    tMediaType_WMC     m_wCodecType;    // Specifies the type of the codec used. Use one of the following values:
                                        // Values   Meaning
                                        // 0x0001   Video codec
                                        // 0x0002   Audio codec
                                        // 0xFFFF   Unknown codec
    U16_WMC     m_wCodecNameLength;     // Specifies the number of Unicode characters stored in the Codec Name field.
    U16Char_WMC * m_pwCodecName;        // Specifies an array of Unicode characters that contains the name of the codec used to create the content.
    U16_WMC     m_wCodecDescLength;     // Specifies the number of Unicode characters stored in the Codec Description field.
    U16Char_WMC * m_pwCodecDescription; // Specifies an array of Unicode characters that contains the description of the format used to create the content.
    U16_WMC     m_wCodecInfoLen;        // Specifies the number of bytes stored in the Codec Information field.
    U8_WMC      *m_pbCodecInfo;         // Specifies an opaque array of information bytes about the codec used to create the content.
} WMCCodecEntry;

typedef    struct tagWMCMetaDataDescRecords
{
    U16_WMC     wLangIdIndex;   // Specifies the index of the language ID in the Language List Object
    U16_WMC     wStreamNumber;  // Specifies whether the entry applies to a specific media stream or
                                // whether it applies to the whole file. A value of 0 in this field
                                // indicates that it applies to the whole file; otherwise, the entry
                                // applies only to the indicated stream number.
    U16_WMC     wNameLenth;     // Specifies the number of valid bytes stored in the Name field.
    U16_WMC     wDataType;      // Specifies the type of information being stored. The following values are recognized:
                                // Value Type   Description
                                //  0x0000      Unicode string. The data consists of a sequence of Unicode characters.
                                //  0x0001      BYTE array. The type of the data is implementation-specific.
                                //  0x0002      BOOL. The data is 2 bytes long and should be interpreted as a 16-bit unsigned integer. Only 0x0000 or 0x0001 are permitted values.
                                //  0x0003      DWORD. The data is 4 bytes long and should be interpreted as a 32-bit unsigned integer.
                                //  0x0004      QWORD. The data is 8 bytes long and should be interpreted as a 64-bit unsigned integer.
                                //  0x0005      WORD. The data is 2 bytes long and should be interpreted as a 16-bit unsigned integer.
                                //  0x0006      GUID. The data is 8 bytes long and should be interpreted as a 128-bit GUID.
                                //  0x8000      Extended Unicode string. The data consists of a sequence of Unicode characters.
                                //  0x8001      Extended BYTE array. The type of the data is implementation-specific.
                                //  0x8002      Extended BOOL. The data is 2 bytes long and should be interpreted as a 16-bit unsigned integer.  Only 0x0000 or 0x0001 are permitted values.
                                //  0x8003      Extended DWORD. The data is 4 bytes long and should be interpreted as a 32-bit unsigned integer.
                                //  0x8004      Extended QWORD. The data is 8 bytes long and should be interpreted as a 64-bit unsigned integer.
                                //  0x8005      Extended WORD. The data is 2 bytes long and should be interpreted as a 16-bit unsigned integer.
                                //  0x8006      Extended GUID. The data is 8 bytes long and should be interpreted as a 128-bit GUID.
    U32_WMC     wDataLength;    // Specifies the length in bytes of the Data field.
    U16Char_WMC *pwName;        // Specifies the name that uniquely identifies the attribute being described. Names are case-sensitive.
    Void_WMC    *pData;         // Specifies the actual metadata being stored. The Data field should be interpreted according to the value
                                // stored in the Data Type field. If the Data Type is one of the extended values, this field may not contain
                                //any data if the file is being streamed over the wire.
} WMCMetaDataDescRecords;


typedef    struct tagWMCMetaDataEntry
{
    U16_WMC     m_wDescRecordsCount;
    WMCMetaDataDescRecords *pDescRec;
} WMCMetaDataEntry;

typedef     struct tagBITMAPINFOHEADER_WMC
{
    unsigned long      biSize;
    long               biWidth;
    long               biHeight;
    unsigned short     biPlanes;
    unsigned short     biBitCount;
    unsigned long      biCompression;
    unsigned long      biSizeImage;
    long               biXPelsPerMeter;
    long               biYPelsPerMeter;
    unsigned long      biClrUsed;
    unsigned long      biClrImportant;
} BITMAPINFOHEADER_WMC;

#endif
#endif
//#pragma pack ()

