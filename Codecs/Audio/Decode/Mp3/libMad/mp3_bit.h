/*
********************************************************************************
*                   Copyright (c) 2008, Rockchip
*                         All rights reserved.
*
* File Name£º   bit.h
*
* Description:  use for bitstream
*
* History:      <author>          <time>        <version>
*             Vincent Hsiung     2009-1-14         1.0
*    desc:    ORG.
********************************************************************************
*/

# ifndef LIBMAD_BIT_H
# define LIBMAD_BIT_H

struct mad_bitptr {
  unsigned long *ptr;
  unsigned long readbit;
};

#define MACROHL  0


#if MACROHL
#define mad_bit_nextbyte(bitptr)  ((unsigned char const*)(bitptr)->ptr + (((bitptr)->readbit + 7) >> 3))
#define mad_bit_skip(bitptr,len)  do { (bitptr)->readbit += (len);}while(0)
#define mad_bit_bitsleft(bitptr)  (unsigned char)(8 - ((bitptr)->readbit & 7));
#define mad_bit_length(begin,end)  ((unsigned int )((end)->readbit - (begin)->readbit))
#define mad_bit_init(bitptr,byte) do { \
	  (bitptr)->ptr     = (unsigned long*)((unsigned int)(byte) & ~3);\
  	  (bitptr)->readbit = ((unsigned long)(byte) & 3) << 3; \
  								}while(0)
#define mad_bit_finish(bitptr)



#else
void mad_bit_init(struct mad_bitptr *, unsigned char const *);

#define mad_bit_finish(bitptr)		/* nothing */

unsigned int mad_bit_length(struct mad_bitptr const *,
			    struct mad_bitptr const *);

unsigned char mad_bit_bitsleft(struct mad_bitptr const *bitptr);
unsigned char const *mad_bit_nextbyte(struct mad_bitptr const *);

void mad_bit_skip(struct mad_bitptr *, unsigned int);
unsigned long mad_bit_read(struct mad_bitptr *, unsigned int);
void mad_bit_write(struct mad_bitptr *, unsigned int, unsigned long);

unsigned short mad_bit_crc(struct mad_bitptr, unsigned int, unsigned short);


#endif

# endif
