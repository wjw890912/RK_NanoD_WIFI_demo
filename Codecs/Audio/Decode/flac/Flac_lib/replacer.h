/*++

Copyright (c) 2006  xxx

Module Name:

    replacer.h

Abstract:



Environment:

    xxxx

Notes:

   Copyright (c) 2006 xxx.  All Rights Reserved.


Revision History:

    Created 7-7-2006, by Fibo

--*/


#ifndef _REPLACER_H
#define _REPLACER_H

//#include "ext/replacer.h"

// memory function
extern void *flac_malloc(unsigned int size) ;
extern void flac_free(void *memblock)  ;

//#define malloc flac_malloc
//#define free flac_free

#ifdef _REPLACER_FILE

//  file function
extern FILE *flac_fopen(const char *filename, const char *mode) ;
extern unsigned int flac_fread(void *buffer, unsigned int size, unsigned int count, FILE *stream) ;
extern unsigned int flac_fwrite(const void *buffer, unsigned int size, unsigned int count, FILE *stream) ;
extern int flac_fseek(FILE *stream, long offset, int origin) ;
extern int flac_fclose(FILE *stream) ;
extern int flac_feof(FILE *stream) ;
extern int flac_ftell(FILE *stream) ;
extern int flac_ferror(FILE *stream) ;
extern int flac_fflush(FILE *stream) ;

#endif

#endif



