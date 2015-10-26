//*@@@+++@@@@******************************************************************
//
// Microsoft Windows Media
// Copyright (C) Microsoft Corporation. All rights reserved.
//
//*@@@---@@@@******************************************************************
/*
 * Windows Media Audio (WMA) Decoder API
 *
 * Copyright (c) Microsoft Corporation 1999.  All Rights Reserved.
 */

#ifndef _WMAUDIO_API_H_
#define _WMAUDIO_API_H_

#include "asfwmatypes.h"



typedef struct _PCMFormat PCMFormat;

/* ...........................................................................
 *
 * Types and Constants
 * ===================
 */
#define WMA_NONSDMI_LIC 0x02
#define WMA_SDMI_LIC    0x10
#define WMA_BURNCD_LIC  0x08


#define WMA_MAX_DATA_REQUESTED  512//4096//512//128 20080724
#define WMA_MAX_METADATA_LEN    512     // Max length (in bytes) of any metadata field - note that metadata is WCHAR, so use even numbers
// We will truncate incoming metadata to this length (and skip past the remaining data).
// If you want to support max allowable size, set this to 65535 (0xFFFF).

enum
{
    ECD_STRING = 0,
    ECD_BINARY = 1,
    ECD_BOOL = 2,
    ECD_DWORD = 3,
    ECD_QWORD = 4,
    ECD_WORD = 5
};


/* 8-bit signed type  */

typedef unsigned char tWMA_U8;


/* 16-bit signed type  */


typedef short tWMA_I16;


/* 16-bit unsigned type  */

typedef unsigned short tWMA_U16;


/* 32-bit unsigned type  */

typedef unsigned long tWMA_U32;


/* Bool */

typedef long tWMA_Bool;
typedef unsigned __int64    tWMA_U64;
typedef __int64    tWMA_I64;

#define DESC_NAME_MAX_LENGTH   64
typedef struct _MarkerEntry
{
    QWORD   m_qOffset;
    QWORD   m_qtime;
    tWMA_U16    m_wEntryLen;
    tWMA_U32   m_dwSendTime;
    tWMA_U32   m_dwFlags;
    tWMA_U32   m_dwDescLen;
    tWMA_U16   m_pwDescName[DESC_NAME_MAX_LENGTH];
} MarkerEntry;

typedef struct _ECD_DESCRIPTOR
{
    tWMA_U16         cbName;
    tWMA_U16     *pwszName;
    tWMA_U16         data_type;
    tWMA_U16         cbValue;
    union
    {
        tWMA_U16 *pwszString;
        tWMA_U8 *pbBinary;
        tWMA_Bool *pfBool;
        tWMA_U32 *pdwDword;
        tWMA_U64 *pqwQword;
        tWMA_U16  *pwWord;
    } uValue;
} ECD_DESCRIPTOR;

typedef struct tagWMAExtendedContentDescription
{
    tWMA_U16 cDescriptors;             // number of descriptors
    ECD_DESCRIPTOR *pDescriptors;  // pointer to all the descriptors
} tWMAExtendedContentDesc;

/* status */
typedef enum tagWMAFileStatus
{
    cWMA_NoErr,                 /* -> always first entry */
    /* remaining entry order is not guaranteed */
    cWMA_Failed,
    cWMA_BadArgument,
    cWMA_BadAsfHeader,
    cWMA_BadPacketHeader,
    cWMA_BrokenFrame,
    cWMA_NoMoreFrames,
    cWMA_BadSamplingRate,
    cWMA_BadNumberOfChannels,
    cWMA_BadVersionNumber,
    cWMA_BadWeightingMode,
    cWMA_BadPacketization,

    cWMA_BadDRMType,
    cWMA_DRMFailed,
    cWMA_DRMUnsupported,

    cWMA_DemoExpired,

    cWMA_BadState,
    cWMA_Internal,               /* really bad */
    cWMA_NoMoreDataThisTime
} tWMAFileStatus;

/* versions */
typedef enum tagWMAFileVersion
{
    cWMA_V1 = 1,
    cWMA_V2 = 2

} tWMAFileVersion;


/* channels */
typedef enum tagWMAFileChannels
{
    cWMA_C_Mono = 1,
    cWMA_C_Stereo = 2

} tWMAFileChannels;
/* ...........................................................................
 *
 * Structures
 * ==========
 */

/* header */

typedef PACKED struct tagWMAFileHeader
{
    tWMAFileVersion version;         /* version of the codec */
    tWMA_U32 sample_rate;  /* sampling rate */
    tWMAFileChannels num_channels;   /* number of audio channels */
    tWMA_U32 duration;               /* of the file in milliseconds */
    tWMA_U32 packet_size;            /* size of an ASF packet */
    tWMA_U64 first_packet_offset;    /* byte offset to the first ASF packet */
    tWMA_U64 last_packet_offset;     /* byte offset to the last ASF packet */

    tWMA_U32 has_DRM;                /* does it have DRM encryption? */

    tWMA_U32 LicenseLength;          /* License Length in the header */

    tWMA_U32 bitrate;                /* bit-rate of the WMA bitstream */

    // Added in V9
    tWMA_U16 pcm_format_tag;         /* wFormatTag in pcm header */

    tWMA_U16 bits_per_sample;        /* number of bits per sample of mono data (container size, always multiple of 8) */

    tWMA_U16 valid_bits_per_sample;  /* actual valid bits per sample of mono data (less than or equal to bits_per_sample) */

    tWMA_U32 subformat_data1;        /* GUID information */

    tWMA_U16 subformat_data2;        /* GUID information */

    tWMA_U16 subformat_data3;        /* GUID information */

    tWMA_U8  subformat_data4[8];     /* GUID information */

    tWMA_U32 channel_mask;           /* which channels are present in stream */

    /* HongCho: what else? */

} tWMAFileHeader;



/* content description */

typedef PACKED struct tagWMAFileContDesc
{
    /* *_len: as [in], they specify how large the corresponding
     *        buffers below are.
     *        as [out], they specify how large the returned
     *        buffers actually are.
     */

    tWMA_U16 title_len;
    tWMA_U16 author_len;
    tWMA_U16 copyright_len;
    tWMA_U16 description_len;   /* rarely used */
    tWMA_U16 rating_len;        /* rarely used */

    /* these are two-byte strings
     *   for ASCII-type character set, to convert these
     *   into a single-byte characters, simply skip
     *   over every other bytes.
     */

    unsigned char *pTitle;
    unsigned char *pAuthor;
    unsigned char *pCopyright;
    unsigned char *pDescription;
    unsigned char *pRating;

} tWMAFileContDesc;


/* license params */

typedef PACKED struct tagWMAFileLicParams
{
    unsigned char *pPMID;       /* portable media id */
    tWMA_U32 cbPMID;            /* length of the pPMID buffer */

} tWMAFileLicParams;



typedef void * tHWMAFileState;

/* Date Params */
typedef PACKED struct tagWMADateParams
{
    tWMA_U16 year;
    tWMA_U8 month;
    tWMA_U8 day;


} tWMADateParams;
/* another internal state */

typedef PACKED struct tagWMAFileHdrState
{
    unsigned char internal[
        240
#ifdef WMDRM_PORTABLE
        + 8
#endif
#ifdef WMDRM_NETWORK
        + 144
#endif
    ]; // additional 4 bytes for MBR support
    // unsigned char internal[212];
} tWMAFileHdrState;
/* ...........................................................................
 *
 * Functions
 * =========
 */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

    /*
     *        WMAFileDecodeCreate
     *        ===================
     *
     * Description
     * -----------
     * Create the WMAudio decoder.
     *
     * Must be called before WMAFileDecodeInit.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileDecodeCreate(tHWMAFileState *phstate);
     *
     * where:
     *
     *   phstate        pointer to the handle that receives the Handle of the internal
     *                  state of the WMAudio decoder
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder initialized
     *   cWMA_Failed               Memory allocation failed
     *   cWMA_BadArgument          bad argument passed in
     *
     */
    tWMAFileStatus WMAFileDecodeCreate(tHWMAFileState* phstate);

#ifdef WMDRM_NETWORK
    tWMAFileStatus WMASetWMDRMNetworkAESKey(tHWMAFileState phstate, DRM_AES_KEY *pKey);
#endif

    /*
     *        WMAFileDecodeInit
     *        =================
     *
     * Description
     * -----------
     * Initializes the WMAudio decoder.
     *
     * This function or WMAFileDecodeInitEx must be called before starting to decode a WMA file.
     *
     * Syntax
     * ------
     * tWMAFileStatus WMAFileDecodeInit (tHWMAFileState phstate, tWMA_U16 nPlayerOpt);
     *
     * where:
     *
     *   phstate        pointer to the handle that holds the internal
     *                  state of the WMAudio decoder
     *                  This memory should be cleared before the first call
     *                  to WMAFileDecodeInit
     *
     *  nPlayerOpt:  can be 0.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder initialized
     *   cWMA_BadArgument          bad argument passed in
     *   cWMA_BadAsfHeader         bad ASF header
     *   cWMA_BadDRMType           unknown encryption type
     *   cWMA_DRMFailed            DRM failed
     *   cWMA_Failed               decoder Initialization failed
     */
    tWMAFileStatus WMAFileDecodeInit(tHWMAFileState phstate, tWMA_U16 nPlayerOpt);

    /*
     *        WMAFileDecodeInitEx
     *        =================
     *
     * Description
     * -----------
     * Initializes the WMAudio decoder.
     *
     * Must be called before starting to decode a WMA file.
     *
     * Syntax
     * ------
     * tWMAFileStatus WMAFileDecodeInitEx (tHWMAFileState phstate, tWMA_U16 nDecoderFlags,
     *                                     tWMA_U16 nDRCSetting, tWMA_U32 nDstChannelMask,
     *                                     tWMA_U32 nInterpResampRate,
     *                                     PCMFORMAT *pPCMFormat,
     *                                     tWMA_U16 wTargetAudioStream);
     *
     * where:
     *
     *   phstate        pointer to the handle that holds the internal
     *                  state of the WMAudio decoder
     *                  This memory should be cleared before the first call
     *                  to WMAFileDecodeInit
     *
     *  nDecoderFlags    : bit-or'd options DECOPT*. Can be 0
     *  nDRCSetting      : 0, 1, or 2.
     *  nDstChannelMask  : Need not have any relation to source channels.
     *                     Used only if "nDecoderFlags | DECOPT_CHANNEL_DOWNMIXING" is true.
     *  nInterpResampRate: can be 0.
     *  pPCMFormat is filled by the function.
     *  wTargetAudioStream Specify which audio stream among all audio streams to decode. 1 or above.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder initialized
     *   cWMA_BadArgument          bad argument passed in
     *   cWMA_BadAsfHeader         bad ASF header
     *   cWMA_BadDRMType           unknown encryption type
     *   cWMA_DRMFailed            DRM failed
     *   cWMA_Failed               decoder Initialization failed
     */
    tWMAFileStatus WMAFileDecodeInitEx(tHWMAFileState phstate, tWMA_U16 nDecoderFlags,
                                       tWMA_U16 nDRCSetting, BOOL bDropPacket,
                                       tWMA_U32 nDstChannelMask,
                                       tWMA_U32 nInterpResampRate,
                                       PCMFormat* pPCMFormat,
                                       tWMA_U16 wTargetAudioStream);


    /*
     *        WMAFileMBRAudioStreams
     *        =================
     *
     * Description
     * -----------
     * Given all the streams in ASF file, this function provides
     * the otal number of audio streams in the ASF file.
     *
     * Must have called WMAFileIsWMA prior to calling this function.
     *
     * Syntax
     * ------
     * tWMAFileStatus WMAFileMBRAudioStreams (tHWMAFileState phstate,
     *                                        tWMA_U16 *pwNumberOfAudioStreams);
     *
     * where:
     *
     *   phstate                pointer to the handle that holds the internal
     *                          state of the WMAudio decoder
     *   pwNumberOfAudioStreams pointer to return the number of audio streams in the file
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder initialized
     *   cWMA_BadArgument          bad argument passed in
     */
    tWMAFileStatus WMAFileMBRAudioStreams(tHWMAFileState phstate,
                                          tWMA_U16 *pwTotalNumberOfAudioStreams);

    /*
     *        WMAFileSetTargetMBRAudioStream
     *        =================
     *
     * Description
     * -----------
     * Given all the streams in ASF file, this function sets the
     * the audio streams the decoder would like to deal with.
     *
     * Must have called before calling WMAFileIsWMA. That is, even before it is ascertained
     * that the file is MBR!
     *
     * Syntax
     * ------
     * tWMAFileStatus WMAFileSetTargetMBRAudioStream (tHWMAFileState phstate,
     *                                                tWMA_U16 wTargetAudioStream);
     *
     * where:
     *
     *   phstate             pointer to the handle that holds the internal
     *                       state of the WMAudio decoder
     *   wTargetAudioStream  Specify which audio stream the decoder is attempting to handle.
     *                       A number greater than or equal to 1 is expected.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder initialized
     *   cWMA_BadArgument          bad argument passed in
     */
    tWMAFileStatus WMAFileSetTargetMBRAudioStream(tHWMAFileState phstate,
            tWMA_U16 wTargetAudioStream);

    /*
     *        WMAFileIsWMA
     *        ============
     *
     * Description
     * -----------
     * Checks to see if the file is decodable.  Uses WMAFileCBGetData(),
     * but the first argument to it is not really of type tHWMAFileState,
     * but tWMAFileIdState.
     *
     * To decode the file, the app should call WMAFileDecodeInit() or WMAFileDecodeInitEx()
     * instead.
     *
     * This function was added in case the app wants to determine the
     * type of the file without fully allocating tHWMAFileState.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileIsWMA(tWMAFileHdrState *state);
     *
     * where:
     *
     *   state          pointer to the smaller structure that contains
     *                  state of the checking function
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                yes, it's a WMA file
     *   cWMA_BadArgument          bad argument passed in
     *   cWMA_BadAsfHeader         bad ASF header
     *
     */
    tWMAFileStatus WMAFileIsWMA(tWMAFileHdrState *state);


    /*
     *        WMAGetNumberOfMarkers
     *        ============
     *
     * Description
     * -----------
     * Return the total number of Markers in the asf file
     *
     * Syntax
     * ------
     *  int WMAGetNumberOfMarkers(tWMAFileHdrState *state);
     *
     * where:
     *
     *   state          pointer to the smaller structure that contains
     *                  state of the checking function
     *
     * Return Value
     * ------------
     *   total number of Markers in the asf file
     *
     */

    int WMAGetNumberOfMarkers(tWMAFileHdrState *state);



    /*
     *        WMAGetMarker
     *        ============
     *
     * Description
     * -----------
     * WMAGetMarker(...) is designed to return the Marker specified by an index integer iIndex
     * If the total number of Markers returned by WMAGetNumberOfMarkers is total_num_of_markers,
     * all the Markers are indexed from 0 to total_num_of_markers - 1.
     * When a Marker structure MarkerEntry is allocated by the application
     * and the pointer to that Marker structure is passed in WMAGetMarker(...) with pEntry,
     * succesful calling of WMAGetMarker(...) will fill up the Marker structure *pEntry
     * with the Marker identified by the index iIndex.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAGetMarker(tWMAFileHdrState *state, int iIndex, MarkerEntry *pEntry);
     *
     * where:
     *
     *   state          pointer to the smaller structure that contains
     *                  state of the checking function
     *   pEntry         pointer to the Marker structure MarkerEntryEx
     *                  allocated by the application
     *   iIndex         index to specify which Marker to return
     *
     * Return Value
     * ------------
     * return cWMA_NoErr
     *      if WMAGetMarker(...) succeed in returning the Marker specified by iIndex.
     * return cWMA_Failed
     *      if WMAGetMarker(...) failed in returning the Marker specified by iIndex.
     *   cWMA_BadArgument          bad argument passed in
     */

    tWMAFileStatus WMAGetMarker(tWMAFileHdrState *state, int iIndex, MarkerEntry *pEntry);



    /*
     *        WMAGetLicenseStore
     *        ============
     *
     * Description
     * -----------
     * Gives the size of License and pointer to license data.
     *
     * Syntax
     * ------
     * BYTE * WMAGetLicenseStore(tWMAFileHdrState *pstate,tWMA_U32 *pLen)
     *
     * where:
     *
     *   state          pointer to the smaller structure that contains
     *                  state of the checking function
     *   pLen           Address of the pointer to store the size of license.
     *
     * Return Value
     * ------------
     * Pointer to data chunk
     *
     */
    unsigned char * WMAGetLicenseStore(tWMAFileHdrState *pstate, tWMA_U32 *pLen);

    /*
     *        WMAFileCBGetData
     *        ================
     *
     * Description
     * -----------
     * Supplies more data to the WMAudio decoder.  Not implemented by
     * the WMAudio decoder library.
     *
     * It is a callback function implemented by the application.  When
     * the WMAudio library needs more data to process (both to parse
     * informatio and to decode the actual data bitstream), the library
     * will call this function.
     *
     * Called by WMAFileDecodeInit(), WMAFileContentDesc() and
     * WMAFileDecodeData().
     *
     * Syntax
     * ------
     *   tWMA_U32 WMAFileCBGetData(tHWMAFileState hstate,
     *                             tWMA_U32 offset,
     *                             tWMA_U32 num_bytes,
     *                             unsigned char  **ppData);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   offset         offset into the WMA file to get the data from
     *   num_bytes      how many bytes to get
     *   ppData         pointer to contain the returned buffer with more
     *                  data
     *                  The WMA library may over-write this buffer
     *
     * Return Value
     * ------------
     *   tWMA_U32       how many actually returned
     *                  should be same as num_bytes except for at the
     *                  very end of the file
     *
     */
    extern tWMA_U32 WMAFileCBGetData(
            tHWMAFileState hstate,
            tWMA_U64 offset,
            tWMA_U32 num_bytes,
            unsigned char **ppData);


    /*
     *        WMAFileCBGetLicenseData
     *        =======================
     *
     * Description
     * -----------
     * Supplies more data to the WMAudio DRM decryption function.  Not
     * implemented by the WMAudio decoder library.
     *
     * It is a callback function implemented by the application.  When
     * the WMAudio's DRM decryption library needs more data to process
     * the library will call this function.
     *
     * Called by WMAFileLicenseInit().
     *
     * Syntax
     * ------
     *   tWMA_U32 WMAFileCBGetLicenseData(tHWMAFileState *state,
     *                                    tWMA_U32 offset,
     *                                    tWMA_U32 num_bytes,
     *                                    unsigned char **ppData);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   offset         offset into the license file to get the data from
     *   num_bytes      how many bytes to get
     *   ppData         pointer to contain the returned buffer with more
     *                  data
     *
     * Return Value
     * ------------
     *   tWMA_U32       how many actually returned
     *                  should be same as num_bytes except for at the
     *                  very end of the file
     *
     */
    extern tWMA_U32 WMAFileCBGetLicenseData(
            tHWMAFileState *pstate,
            tWMA_U32 offset,
            tWMA_U32 num_bytes,
            unsigned char **ppData);



    /*
     *        WMAFileIsLosslessWMA
     *        =================
     *
     * Description
     * -----------
     * Check if the WMA file is Lossless WMA
     *
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileIsLosslessWMA (tHWMAFileState hstate);
     *
     * where:
     *
     *   phstate        pointer to the handle that holds the internal
     *                  state of the WMAudio decoder
     *                  This memory should be cleared before the first call
     *                  to WMAFileDecodeInit
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                Is a WMA Lossless file.
     */
//tWMAFileStatus WMAFileIsLosslessWMA(tHWMAFileState hstate);

    /*
     *        WMAFileDecodeClose
     *        =================
     *
     * Description
     * -----------
     * Closes the WMAudio decoder and frees all allocated memories.
     *
     * Must be called after decoding a WMA file.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileDecodeClose (tHWMAFileState* phstate);
     *
     * where:
     *
     *   phstate        pointer to the handle that holds the internal
     *                  state of the WMAudio decoder
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                decoder closed
     *   cWMA_BadArgument     bad argument(s)
     */



//tWMAFileStatus WMAFileDecodeClose (tHWMAFileState* phstate);

    /*
     *       WMAFileDecodeInfo
     *       =================
     *
     * Description
     * -----------
     * Retrieves the header information for the WMA file.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileDecodeInfo(tHWMAFileState hstate,
     *                                    tWMAFileHeader *hdr);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   hdr            pointer to the structure that holds the WMA file
     *                  header info
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                header retrieved ok
     *   cWMA_BadArgument          bad argument(s)
     *
     */

    tWMAFileStatus WMAFileDecodeInfo(tHWMAFileState hstate, tWMAFileHeader *hdr);


    /*
     *       WMAFileContentDesc
     *       ==================
     *
     * Description
     * -----------
     * Retrieves the content description for the WMA file.  Content
     * descriptions are such as the song title, the author, the
     * copyright info, the song description, and the rating info.
     * All these are part of the standard ASF description.
     *
     * Not all songs have content descriptions.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileContentDesc (tHWMAFileState hstate, )
     *                                      const tWMAFileContDesc **ppDesc);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   ppDesc         pointer of pointer to the Extended Content Desc
     *                  structure that holds in the ASF file.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                description retrieved ok
     *   cWMA_BadArgument          bad argument(s)
     *   ppDesc is the pointer of pointer to the ASF content desc object.
     ***** DO NOT RELEASE THIS POINTER *************
     */
    tWMAFileStatus WMAFileContentDesc(tHWMAFileState hstate, const tWMAFileContDesc **ppDesc);

    /*              WMAFileExtendedContentDesc
     *
     * Description
     * -----------
     * Retrieves the extended content descriptions for the ASF file.
     * Not all ASF files have extended content descriptions.
     *
     * Syntax
     * ------
     * tWMAFileStatus WMAFileExtendedContentDesc (tHWMAFileState hstate, const tWMAExtendedContentDesc **pECDesc);
     *
     * where:
     *
     *   hstate                     Pointer to the structure that holds the internal
     *                              state of the WMAudio decoder
     *   pECDesc                    pointer of pointer to the Extended Content Desc
     *                              structure that holds in the ASF file.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                  description retrieved ok
     *   cWMA_BadArgument            bad argument(s)
     *   pECDesc is the pointer of pointer to the ASF extended content desc object.
     ***** DO NOT RELEASE THIS POINTER *************
     */

    tWMAFileStatus WMAFileExtendedContentDesc(tHWMAFileState hstate, const tWMAExtendedContentDesc **pECDesc);

    /*
     *       WMAFileLicenseInit
     *       ==================
     *
     * Description
     * -----------
     * Reads the DRM license file and check the license for the file.
     *
     * Uses WMAFileCBGetLicenseData to retrieve the license file.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileLicenseInit(tHWMAFileState hstate,
     *                                     tWMAFileLicParams *lic_params,
     *                                     tWMA_U32 rights,
     *                                     tWMADateParams currentDate);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   lic_params     pointer to the structure that passes in the
     *                  parameters for the license initialization.
     *   rights         This parameter is for rights. Values are:
     *                  WMA_NONSDMI_LIC
     *                  WMA_SDMI_LIC
     *                  WMA_BURNCD_LIC
     *
     *                  These values are defined in this file.
     *   currentDate
     *                  currentDate structure is set to zero if no date available
     *                  currentDate.year_hi  = Year / 100
     *                  currentDate.year_low  = Year % 100
     *                  currentDate.month  = Month
     *                  currentDate.day  = Day
     *
     *
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                license checked ok
     *   cWMA_BadArgument          bad argument
     *   cWMA_DRMFailed            no license
     *   cWMA_DRMUnsupported       DRM is not supported for this version
     *
     */
    tWMAFileStatus WMAFileLicenseInit(tHWMAFileState hstate, tWMAFileLicParams *lic_params, tWMA_U32 rights, tWMADateParams currentDate);


    /*
     *       WMAFileDecodeData
     *       =================
     *
     * Description
     * -----------
     * Decodes WMAudio bitstream.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileDecodeData (tHWMAFileState hstate, tWMA_U32 *pnDecodedSamples);
     *
     * where:
     *
     *   state              pointer to the structure that holds the internal
     *                      state of the WMAudio decoder
     *   pnDecodedSamples   pointer to the number of samples decoded by this call.
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                frame decoded ok
     *   cWMA_BadArgument          bad argument
     *   cWMA_NoMoreFrames         no more data to decode
     *   cWMA_BadPacketHeader      Bad Packet header
     *
     */


    tWMAFileStatus WMAFileDecodeData(tHWMAFileState hstate, tWMA_U32 *pnDecodedSamples);

    /*
     *       WMAFileGetPCM
     *       =============
     *
     * Description
     * -----------
     * Write PCM samples from the WMAudio bitstream previously decoded by
     * WMAFileDecodeData().
     *
     * WMAFileGetPCM() may have to be called more than once to obtain all
     * of the samples for the frame. The frame is complete when the number
     * of samples written is less than the number of samples requested.
     *
     * Syntax
     * ------
     *   tWMA_U32 WMAFileGetPCM(tHWMAFileState hstate,
     *                          tWMA_I16 *pi16Channel0, tWMA_I16 *pi16Channel1,
     *                          tWMA_U32 iSizeOfChannel0, tWMA_U32 max_nsamples
                                tWMA_I64* piTimeStamp);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   pi16Channel0   pointer to output buffer for left-channel PCM, or the entire PCM output if it is inter-leaved form
     *   pi16Channel1   pointer to output buffer for right-channel PCM, NULL if the output PCM is in parallel form
     *   iSizeOfChannel0 size of the pi16Channel0 buffer in bytes. It is assumed that Channel1 is the same size.
     *                   *** NOTE*** If Channel downmixing, frequency resampling or bit-depth conversion is being requested,
     *                               the memory provided should be for maximum of source & destination sizes.
     *   max_nsamples   maximum number of samples to write per channel
     *   piTimeStamp    Pointer for getting TimeSatmp value.
     *
     * Return Value
     * ------------
     *   tWMA_U32       number of samples written
     *                  < max_nsamples when frame complete
     *
     */


    tWMA_U32 WMAFileGetPCM(tHWMAFileState hstate,
                           tWMA_I16 *pi16Channel0,
                           tWMA_U32 iSizeOfChannel0,
                           tWMA_U32 max_nsamples,
                           tWMA_I64* piTimeStamp);


    /*
     *       WMAFileSeek
     *       ===========
     *
     * Description
     * -----------
     * Seek to a time into the WMA file.  It seeks to the nearest seek
     * point.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileSeek (tHWMAFileState hstate, tWMA_U32 msSeek, tWMA_U32 *pmsActual);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   msSeek         time position to seek from the beginning of the
     *                  clip, in milliseconds
     *  pmsActual       actual time position from the beginning of the
     *                  clip, in milliseconds
     *
     * Return Value
     * ------------
     *   cWMA_NoErr                Seek OK
     *   cWMA_BadArgument          bad argument
     *   cWMA_Failed               Decoder Failed to reset
     *   cWMA_BadPacketHeader      Bad Packet/Payload header
     */

tWMAFileStatus WMAFileSeek (tHWMAFileState hstate, tWMA_U32 msSeek, tWMA_U32 *pmsActual);

    /*
     *       WMAFileSeekBase
     *       ===========
     *
     * Description
     * -----------
     * Seek to a time into the WMA file.  It seeks to the nearest seek
     * point.
     *
     * Syntax
     * ------
     *   tWMAFileStatus WMAFileSeekBase(tHWMAFileState hstate,
     *                                  tWMA_U32 * pmsSeek,
     *                                  tWMA_U32 * piPacket,
     *                                  tWMA_U32 * pmsActual,
     *                                  BOOL fProWMAEncoder,
     *                                  BOOL fSeekToPacket);
     *
     * where:
     *
     *   state          pointer to the structure that holds the internal
     *                  state of the WMAudio decoder
     *   pmsSeek        points to the time you want to seek to
     *
     *  pmsActual       actual time position from the beginning of the
     *
     *   piPacket       points to the packet you want to seek to
     *
     *   fProWMAENcoder Is the function called by Professional WMA encoder?
     *
     *   fSeekToPacket  indicate whether to seek to packet or to seek to timestamp.
     *                  So if fSeekToPacket is true, piPacket will be used, otherwise,
     *                  pmsSeek will be used. But after the successfully seeking,
     *                  both piPacket and pmsSeek will be updated to the actual timestamp and packet we seek into.
     * Return Value
     * ------------
     *   cWMA_NoErr                Seek OK
     *   cWMA_BadArgument          bad argument
     *   cWMA_Failed               Decoder Failed to reset
     *   cWMA_BadPacketHeader      Bad Packet/Payload header
     *
     *  That function was added purely for the purpose of the WM Compression tool. Except the WM Compression tool,
     *  all other callers should call WMAFileSeek.
     */

    tWMAFileStatus WMAFileSeekBase(tHWMAFileState hstate,
                                       tWMA_U32 * pmsSeek,
                                       tWMA_U32 * piPacket,
                                       tWMA_U32 * pmsActual,
                                       BOOL fProWMAEncoder,
                                       BOOL fSeekToPacket);


    /*
     *       WMADebugMessage
     *       ===============
     *
     * Description
     * -----------
     * Display a debugging message. This function needs to be implemented by the app that calls WMEPAK.
     * These messages are meant for the device/application developer, not the end user.
     * Display the message using fprintf, messagebox, or whatever method is suitable to your platform.
     * See the sample code for an example implementation.
     * Make it a empty-body function if the intent is to skip this operation.
     *
     * Syntax
     * ------
     *      WMADebugMessage(const char* pszFmt,
     *                      ...);
     *
     * where:
     *
     *   pszFmt         pointer to a null terminated printf like format string
     *   ...            variable number of arguments referenced by the format string
     *
     * Return Value
     * ------------
     *   none
     *
     */
    extern void WMADebugMessage(const char*pszFmt, ...);



#ifdef __cplusplus
}
#endif /* __cplusplus */

/*
#ifdef USE_PACK_HEADERS
#include <poppack.h>
#endif
*/

//***********************************************************************
// Exercising Rapier Special Modes, not everything available all the time
//***********************************************************************
#define DECOPT_CHANNEL_DOWNMIXING      0x00000001
#define DECOPT_DRC                     0x00000002
#define DECOPT_INTERPOLATED_DOWNSAMPLE 0x00000004
#define DECOPT_HALF_TRANSFORM          0x00000008
#define DECOPT_HALF_UP_TRANSFORM       0x00000010
#define DECOPT_2X_TRANSFORM            0x00000020
#define DECOPT_REQUANTTO16             0x00000040
#define DECOPT_DOWNSAMPLETO44OR48      0x00000080
#define DECOPT_LTRTDOWNMIX             0x00000100

#include "..\pWMA.h"  //evan wu,2009-2-11


#endif /* _WMAUDIO_API_H_ */
