/*
********************************************************************************
*                   Copyright (c) 2008, Rockchip
*                         All rights reserved.
*
* File Name£º   layer3.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             Vincent Hsiung     2009-1-14         1.0
*    desc:    ORG.
********************************************************************************
*/

# ifndef LIBMAD_LAYER3_H
# define LIBMAD_LAYER3_H

# include "mp3_stream.h"
# include "mp3_frame.h"

int mad_layer_III(struct mad_stream *, struct mad_frame *);
extern main_data_t MainData;

# endif
