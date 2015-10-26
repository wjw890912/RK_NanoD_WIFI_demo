/******************************************************************************
*
*  Copyright (C),2007, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     flacbuffer.h
*  Description:
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*           Huweiguo     07/05/07      1.0
*
*******************************************************************************/


#ifndef __FLAC_DECODER_INFO_H__
#define __FLAC_DECODER_INFO_H__

#include "ordinals.h"

/** State values for a FLAC__StreamDecoder
 *
 *  The decoder's state can be obtained by calling FLAC__stream_decoder_get_state().
 */
typedef enum
{

    FLAC__STREAM_DECODER_SEARCH_FOR_METADATA = 0,
    /**< The decoder is ready to search for metadata. */

    FLAC__STREAM_DECODER_READ_METADATA,
    /**< The decoder is ready to or is in the process of reading metadata. */

    FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC,
    /**< The decoder is ready to or is in the process of searching for the frame sync code. */

    FLAC__STREAM_DECODER_READ_FRAME,
    /**< The decoder is ready to or is in the process of reading a frame. */

    FLAC__STREAM_DECODER_END_OF_STREAM,
    /**< The decoder has reached the end of the stream. */

    FLAC__STREAM_DECODER_ABORTED,
    /**< The decoder was aborted by the read callback. */

    FLAC__STREAM_DECODER_UNPARSEABLE_STREAM,
    /**< The decoder encountered reserved fields in use in the stream. */

    FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR,
    /**< An error occurred allocating memory. */

    FLAC__STREAM_DECODER_ALREADY_INITIALIZED,
    /**< FLAC__stream_decoder_init() was called when the decoder was
     * already initialized, usually because
     * FLAC__stream_decoder_finish() was not called.
     */

    FLAC__STREAM_DECODER_INVALID_CALLBACK,
    /**< FLAC__stream_decoder_init() was called without all callbacks being set. */

    FLAC__STREAM_DECODER_UNINITIALIZED
    /**< The decoder is in the uninitialized state. */

} FLAC__StreamDecoderState;


typedef struct
{
    unsigned blocksize; // 我们只支持固定 blocksize
    unsigned long sample_rate; // 8, 16, 22, 24, 32, 44.1, 48, 96KHz
    unsigned channels;
    unsigned bits_per_sample;
    unsigned long total_samples; // 总的样点数
    unsigned long samples_decoded;
    unsigned long bits_per_second; // bps
    int total_play_time; // 歌曲总播放时间，以秒为计数
    int curr_play_time; // 歌曲当前播放时间，以秒为计数
    long audio_data_offset; // FLAC文件audio数据开始的地方, byte

    /* ID3 信息 */
    unsigned short Title[30];
    unsigned short Artist[30];
    unsigned short Album[30];
} FLAC_Decoder_Info;


/** SeekPoint structure used in SEEKTABLE blocks.  (c.f. <A HREF="../format.html#seekpoint">format specification</A>)
 */
typedef struct
{
    FLAC__uint64 sample_number;
    /**<  The sample number of the target frame. */

    FLAC__uint64 stream_offset;
    /**< The offset, in bytes, of the target frame with respect to
     * beginning of the first frame. */

    //unsigned frame_samples;
    /**< The number of samples in the target frame. */
} FLAC__StreamMetadata_SeekPoint;

#define MAX__SEEK_POINTS_NAMBLE 1024

#endif
