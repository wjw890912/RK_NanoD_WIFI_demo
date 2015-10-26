/*
********************************************************************************
*                   Copyright (c) 2008, Rockchip
*                         All rights reserved.
*
* File Name£º   decoder.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             Vincent Hsiung     2009-1-14         1.0
*    desc:    ORG.
********************************************************************************
*/

# ifndef LIBMAD_DECODER_H
# define LIBMAD_DECODER_H

# include "mp3_stream.h"
# include "mp3_frame.h"
# include "mp3_synth.h"

enum mad_decoder_mode {
  MAD_DECODER_MODE_SYNC  = 0,
  MAD_DECODER_MODE_ASYNC
};

enum mad_flow {
  MAD_FLOW_CONTINUE = 0x0000,	/* continue normally */
  MAD_FLOW_STOP     = 0x0010,	/* stop decoding normally */
  MAD_FLOW_BREAK    = 0x0011,	/* stop decoding and signal an error */
  MAD_FLOW_IGNORE   = 0x0020	/* ignore the current frame */
};

struct sync_t {
    struct mad_stream stream;	// definito main_data_t un array di circa 4K
    struct mad_frame frame;
    struct mad_synth synth;
};

struct mad_decoder {
  enum mad_decoder_mode mode;

  int options;

  struct {
    long pid;
    int in;
    int out;
  } async;

  struct sync_t *sync;

  void *cb_data;

  enum mad_flow (*input_func)(void *, struct mad_stream *);
  enum mad_flow (*header_func)(void *, struct mad_header const *);
  enum mad_flow (*filter_func)(void *,
			       struct mad_stream const *, struct mad_frame *);
  enum mad_flow (*output_func)(void *,
			       struct mad_header const *, struct mad_pcm *);
  enum mad_flow (*error_func)(void *, struct mad_stream *, struct mad_frame *);
  enum mad_flow (*message_func)(void *, void *, unsigned int *);
};

void mad_decoder_init(struct mad_decoder *, void *,
		      enum mad_flow (*)(void *, struct mad_stream *),
		      enum mad_flow (*)(void *, struct mad_header const *),
		      enum mad_flow (*)(void *,
					struct mad_stream const *,
					struct mad_frame *),
		      enum mad_flow (*)(void *,
					struct mad_header const *,
					struct mad_pcm *),
		      enum mad_flow (*)(void *,
					struct mad_stream *,
					struct mad_frame *),
		      enum mad_flow (*)(void *, void *, unsigned int *));
int mad_decoder_finish(struct mad_decoder *);

# define mad_decoder_options(decoder, opts)  \
    ((void) ((decoder)->options = (opts)))

int mad_decoder_run(struct mad_decoder *, enum mad_decoder_mode);
int mad_decoder_message(struct mad_decoder *, void *, unsigned int *);

# endif
