#ifndef _MOVPARSERINTERFACE_H
#define _MOVPARSERINTERFACE_H

//#define size_t unsigned int






#define MOVPARSERBSS   //__attribute__ ((section(".DSECT parser_bss")))

#ifdef  _IN_MOVPARSERINTERFACE_H
#undef  EXT
#define EXT
#else
#define EXT extern
#endif







#endif
