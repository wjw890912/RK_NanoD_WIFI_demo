
#ifndef APE_GLOBALFUNCTION_DECLARATION_H
#define APE_GLOBALFUNCTION_DECLARATION_H
#include "ape_globalvardeclaration.h"

ape_int16 ApeBlocksDecode(ape_char * pInputFilename, ape_char * pOutputFilename, ape_int32 nOutputMode, ape_int32 nCompressionLevel,ape_int32 * pKillFlag,ape_int32* nBlocks);
ape_int16 ApeHeaderDecode(ape_char * pInputFilename,ape_char * pOutputFilename,ape_int32 nOutputMode, ape_int32 nCompressionLevel, ape_int32 * pPercentageDone,ape_int32 * pKillFlag);
void      ApeDecodeInitialize(void);

#endif //APE_GLOBALFUNCTION_DECLARATION_H