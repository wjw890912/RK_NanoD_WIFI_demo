/*
********************************************************************************
*                   Copyright (c) 2008, Rockchip
*                         All rights reserved.
*
* File Name：   synth.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             Vincent Hsiung     2009-1-14         1.0
*    desc:    ORG.
********************************************************************************
*/

/*
    1. 使用硬件模块进行synth,删去软件所用fifo空间

    by Vincent Hsiung.
*/

# ifndef LIBMAD_SYNTH_H
# define LIBMAD_SYNTH_H

# include "mp3_fixed.h"
# include "mp3_frame.h"

struct mad_pcm {
  unsigned int samplerate;	    /* sampling frequency (Hz)         */
  unsigned short channels;		/* number of channels              */
  unsigned short length;		/* number of samples per channel   */
};

struct mad_synth {
  unsigned int phase;			/* current processing phase */
  struct mad_pcm pcm;			/* PCM output */
};

/* single channel PCM selector */
_ATTR_MP3DEC_DATA_
enum {
  MAD_PCM_CHANNEL_SINGLE = 0
};

/* dual channel PCM selector */
_ATTR_MP3DEC_DATA_
enum {
  MAD_PCM_CHANNEL_DUAL_1 = 0,
  MAD_PCM_CHANNEL_DUAL_2 = 1
};

/* stereo PCM selector */
_ATTR_MP3DEC_DATA_
enum {
  MAD_PCM_CHANNEL_STEREO_LEFT  = 0,
  MAD_PCM_CHANNEL_STEREO_RIGHT = 1
};

void mad_synth_init(struct mad_synth *);

# define mad_synth_finish(synth)  /* nothing */

void mad_synth_mute(struct mad_synth *);

void mad_synth_frame(struct mad_synth *, struct mad_frame const *);

void synth_fouth_frame_hw(struct mad_frame * frame, mad_fixed_t (fouth_frame_sbsample)[36][32],
		unsigned int ch, unsigned int ns);

# endif
