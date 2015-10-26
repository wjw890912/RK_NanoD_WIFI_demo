#ifndef APE_PREPARE_H
#define APE_PREPARE_H

#define SPECIAL_FRAME_MONO_SILENCE              1
#define SPECIAL_FRAME_LEFT_SILENCE              1
#define SPECIAL_FRAME_RIGHT_SILENCE             2
#define SPECIAL_FRAME_PSEUDO_STEREO             4

/*****************************************************************************
Manage the preparation stage of compression and decompression

Tasks:

1) convert data to 32-bit
2) convert L,R to X,Y
3) calculate the CRC
4) do simple analysis
5) check for the peak value
*****************************************************************************/

typedef struct
{
	void (*Unprepare)(ape_int32 X, ape_int32 Y,  WAVEFORMATEX * pWaveFormatEx, ape_uchar * pOutputLeft,ape_uchar * pOutputRight);
	ape_int32 (*UnprepareOld)(void *,ape_int32 * pInputX, ape_int32 *pInputY, ape_int32 nBlocks,  WAVEFORMATEX * pWaveFormatEx, ape_uchar * pRawData, ape_uint32 * pCRC, ape_int32 * pSpecialCodes, ape_int32 nFileVersion);
}CPrepare;
#ifdef __cplusplus
    extern "C" {
#endif
void ApeUnprepare(ape_int32 X, ape_int32 Y,  WAVEFORMATEX * pWaveFormatEx, ape_uchar * pOutputLeft,ape_uchar * pOutputRight);
//ape_int32 ApeUnprepareOld(void *,ape_int32 * pInputX, ape_int32 *pInputY, ape_int32 nBlocks, const WAVEFORMATEX * pWaveFormatEx, ape_uchar * pRawData, ape_uint32 * pCRC, ape_int32 * pSpecialCodes, ape_int32 nFileVersion);
#ifdef __cplusplus
    }
#endif
#endif // #ifndef APE_PREPARE_H
