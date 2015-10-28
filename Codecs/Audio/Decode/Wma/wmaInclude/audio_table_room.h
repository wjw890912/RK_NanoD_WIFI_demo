/*
************************************************************************************************************************
*
*  Copyright (C),2009, Fuzhou Rockchip Co.,Ltd.
*
*  File name :     audio_table_room.h
*  Author:         HuWeiGuo
*  Description:
*  Remark:
*
*  History:
*           <author>      <time>     <version>       <desc>
*                        09/01/14       v1.0
*
************************************************************************************************************************
*/

#ifndef _AUDIO_TABLE_ROOM_H_
#define _AUDIO_TABLE_ROOM_H_

#include "..\wmaInclude\predefine.h"
#define NANO_C
/////////////////////////////////// WMA Table Offset ///////////////////////////////////////
#ifdef NANO_C

#define    TBL_OFFSET_getMask                                              0x0000    // 132
#define    TBL_OFFSET_CLSID_CAsfHeaderObjectV0                             0x0084    // 16
#define    TBL_OFFSET_CLSID_CAsfPropertiesObjectV2                         0x0094    // 16
#define    TBL_OFFSET_CLSID_CAsfStreamPropertiesObjectV1                   0x00a4    // 16
#define    TBL_OFFSET_CLSID_CAsfContentDescriptionObjectV0                 0x00b4    // 16
#define    TBL_OFFSET_CLSID_AsfXAcmAudioErrorMaskingStrategy               0x00c4    // 16
#define    TBL_OFFSET_CLSID_AsfXSignatureAudioErrorMaskingStrategy         0x00d4    // 16
#define    TBL_OFFSET_CLSID_AsfXStreamTypeAcmAudio                         0x00e4    // 16
#define    TBL_OFFSET_CLSID_CAsfContentEncryptionObject                    0x00f4    // 16
#define    TBL_OFFSET_CLSID_CAsfExtendedContentDescObject                  0x0104    // 16
#define    TBL_OFFSET_CLSID_CAsfMarkerObjectV0                             0x0114    // 16
#define    TBL_OFFSET_CLSID_CAsfLicenseStoreObject                         0x0124    // 16
#define    TBL_OFFSET_CLSID_CAsfStreamPropertiesObjectV2                   0x0134    // 16
#define    TBL_OFFSET_CLSID_CAsfExtendedStreamPropertiesObject             0x0144    // 16
#define    TBL_OFFSET_CLSID_CAsfClockObjectV0                              0x0154    // 16
#define    TBL_OFFSET_CLSID_AsfXMetaDataObject                             0x0164    // 16
#define    TBL_OFFSET_CLSID_CAsfPacketClock1                               0x0174    // 16
#define    TBL_OFFSET_g_rgiHuffDecTblMsk                                   0x0184    // 452
#define    TBL_OFFSET_g_rgiHuffDecTblNoisePower                            0x0348    // 152
#define    TBL_OFFSET_g_rgiHuffDecTbl44smOb                                0x03e0    // 5696
#define    TBL_OFFSET_rgiMaskMinusPower10                                  0x1a20    // 288
#define    TBL_OFFSET_rgiMaskPlusPower10                                   0x1b40    // 248
#define    TBL_OFFSET_g_rgiBarkFreqV2                                      0x1c38    // 100
#define    TBL_OFFSET_gRun16ssOb                                           0x1c9c    // 436
#define    TBL_OFFSET_gLevel16ssOb                                         0x1e50    // 436
#define    TBL_OFFSET_gRun44smOb                                           0x2004    // 1336
#define    TBL_OFFSET_gLevel44smOb                                         0x253c    // 1336
#define    TBL_OFFSET_gRun44ssOb                                           0x2a74    // 1072
#define    TBL_OFFSET_gLevel44ssOb                                         0x2ea4    // 1072
#define    TBL_OFFSET_gRun44smQb                                           0x32d4    // 664
#define    TBL_OFFSET_gLevel44smQb                                         0x356c    // 664
#define    TBL_OFFSET_gRun44ssQb                                           0x3804    // 556
#define    TBL_OFFSET_gLevel44ssQb                                         0x3a30    // 556
#define    TBL_OFFSET_g_InvQuadRootFraction                                0x3c5c    // 1028
#define    TBL_OFFSET_g_InvQuadRootExponent                                0x4060    // 260
#define    TBL_OFFSET_g_InverseFraction                                    0x4164    // 1028
#define    TBL_OFFSET_g_SqrtFraction                                       0x4568    // 1028
#define    TBL_OFFSET_g_sct64                                              0x496c    // 52
#define    TBL_OFFSET_g_sct128                                             0x49a0    // 52
#define    TBL_OFFSET_g_sct256                                             0x49d4    // 52
#define    TBL_OFFSET_g_sct512                                             0x4a08    // 52
#define    TBL_OFFSET_g_sct1024                                            0x4a3c    // 52
#define    TBL_OFFSET_g_sct2048                                            0x4a70    // 52
#define    TBL_OFFSET_rgDBPower10                                          0x4aa4    // 512
#define    TBL_OFFSET_icosPIbynp                                           0x4ca4    // 64
#define    TBL_OFFSET_isinPIbynp                                           0x4ce4    // 64
#define    TBL_OFFSET_g_rgiLsfReconLevel                                   0x4d24    // 640
#define    TBL_OFFSET_gLZLTable                                            0x4fa4    // 128

#else
                                                                                    // size
#define    TBL_OFFSET_getMask                                             0x0000    //  132
#define    TBL_OFFSET_CLSID_CAsfHeaderObjectV0                            0x0084    //   16
#define    TBL_OFFSET_CLSID_CAsfPropertiesObjectV2                        0x0094    //   16
#define    TBL_OFFSET_CLSID_CAsfStreamPropertiesObjectV1                  0x00a4    //   16
#define    TBL_OFFSET_CLSID_CAsfContentDescriptionObjectV0                0x00b4    //   16
#define    TBL_OFFSET_CLSID_AsfXAcmAudioErrorMaskingStrategy              0x00c4    //   16
#define    TBL_OFFSET_CLSID_AsfXSignatureAudioErrorMaskingStrategy        0x00d4    //   16
#define    TBL_OFFSET_CLSID_AsfXStreamTypeAcmAudio                        0x00e4    //   16
#define    TBL_OFFSET_CLSID_CAsfContentEncryptionObject                   0x00f4    //   16
#define    TBL_OFFSET_CLSID_CAsfExtendedContentDescObject                 0x0104    //   16
#define    TBL_OFFSET_CLSID_CAsfMarkerObjectV0                            0x0114    //   16
#define    TBL_OFFSET_CLSID_CAsfLicenseStoreObject                        0x0124    //   16
#define    TBL_OFFSET_CLSID_CAsfStreamPropertiesObjectV2                  0x0134    //   16
#define    TBL_OFFSET_CLSID_CAsfExtendedStreamPropertiesObject            0x0144    //   16
#define    TBL_OFFSET_CLSID_CAsfClockObjectV0                             0x0154    //   16
#define    TBL_OFFSET_CLSID_AsfXMetaDataObject                            0x0164    //   16
#define    TBL_OFFSET_CLSID_CAsfPacketClock1                              0x0174    //   16
#define    TBL_OFFSET_g_rgiHuffDecTblMsk                                  0x0184    //  452
#define    TBL_OFFSET_g_rgiHuffDecTblNoisePower                           0x0348    //  152
#define    TBL_OFFSET_g_rgiHuffDecTbl16smOb                               0x03e0    // 1744
#define    TBL_OFFSET_g_rgiHuffDecTbl44smOb                               0x0ab0    // 5694
#define    TBL_OFFSET_g_rgiHuffDecTbl16ssOb                               0x20f0    // 1696
#define    TBL_OFFSET_g_rgiHuffDecTbl44ssOb                               0x2790    // 4004
#define    TBL_OFFSET_g_rgiHuffDecTbl44smQb                               0x3734    // 2488
#define    TBL_OFFSET_g_rgiHuffDecTbl44ssQb                               0x40ec    // 2208
#define    TBL_OFFSET_rgiMaskMinusPower10                                 0x498c    //  288
#define    TBL_OFFSET_rgiMaskPlusPower10                                  0x4aac    //  248
#define    TBL_OFFSET_g_rgiBarkFreqV2                                     0x4ba4    //  100
#define    TBL_OFFSET_gRun16smOb                                          0x4c08    //  948
#define    TBL_OFFSET_gLevel16smOb                                        0x4fbc    //  948
#define    TBL_OFFSET_gRun16ssOb                                          0x5370    //  866
#define    TBL_OFFSET_gLevel16ssOb                                        0x56d4    //  866
#define    TBL_OFFSET_gRun44smOb                                          0x5a38    // 2668
#define    TBL_OFFSET_gLevel44smOb                                        0x64a4    // 2668
#define    TBL_OFFSET_gRun44ssOb                                          0x6f10    // 2140
#define    TBL_OFFSET_gLevel44ssOb                                        0x776c    // 2140
#define    TBL_OFFSET_gRun44smQb                                          0x7fc8    // 1328
#define    TBL_OFFSET_gLevel44smQb                                        0x84f8    // 1328
#define    TBL_OFFSET_gRun44ssQb                                          0x8a28    // 1106
#define    TBL_OFFSET_gLevel44ssQb                                        0x8e7c    // 1106
#define    TBL_OFFSET_g_InvQuadRootFraction                               0x92d0    // 1028
#define    TBL_OFFSET_g_InvQuadRootExponent                               0x96d4    //  260
#define    TBL_OFFSET_g_InverseFraction                                   0x97d8    // 1028
#define    TBL_OFFSET_g_SqrtFraction                                      0x9bdc    // 1028
#define    TBL_OFFSET_g_sct64                                             0x9fe0    //   52
#define    TBL_OFFSET_g_sct128                                            0xa014    //   52
#define    TBL_OFFSET_g_sct256                                            0xa048    //   52
#define    TBL_OFFSET_g_sct512                                            0xa07c    //   52
#define    TBL_OFFSET_g_sct1024                                           0xa0b0    //   52
#define    TBL_OFFSET_g_sct2048                                           0xa0e4    //   52
#define    TBL_OFFSET_rgDBPower10                                         0xa118    //  512
#define    TBL_OFFSET_icosPIbynp                                          0xa318    //   64
#define    TBL_OFFSET_isinPIbynp                                          0xa358    //   64
#define    TBL_OFFSET_g_rgiLsfReconLevel                                  0xa398    //  640
#define    TBL_OFFSET_gLZLTable                                           0xa618    //  128

/////////////////////////////////// DRM9 Table Offset //////////////////////////////////////

#define    TBL_OFFSET__DRM_Sel                                            0xa698    // 2048
#define    TBL_OFFSET__DRM_Spbox                                          0xae98    // 2048
#if 0
/////////////////////////////////// FLAC Table Offset //////////////////////////////////////

#define    TBL_OFFSET_byte_to_unary_table                                 0xb698    //  256
#define    TBL_OFFSET_FLAC__crc8_table                                    0xb798    //  256
#define    TBL_OFFSET_FLAC__crc16_table                                   0xb898    //  512

/////////////////////////////////// APE Table Offset //////////////////////////////////////

#define    TBL_OFFSET_Ape_gtPowersOfTwoMinusOne                           0xba98    //  132
#define    TBL_OFFSET_Ape_gtKSumMinBoundary                               0xbb1c    //  128
#define    TBL_OFFSET_Ape_gtRangeTotalOne                                 0xbb9c    //  260
#define    TBL_OFFSET_Ape_gtRangeWidthOne                                 0xbca0    //  256
#define    TBL_OFFSET_Ape_gtRangeTotalTwo                                 0xbda0    //  260
#define    TBL_OFFSET_Ape_gtRangeWidthTwo                                 0xbea4    //  256
#define    TBL_OFFSET_Ape_gtCRC32                                         0xbfa4    // 1024
#endif
#endif


////////////////////////////////////////////////////////////////////////////////////////////

// wma
extern unsigned char *p_getMask                                     ;
extern unsigned char *p_CLSID_CAsfHeaderObjectV0                    ;
extern unsigned char *p_CLSID_CAsfPropertiesObjectV2                ;
extern unsigned char *p_CLSID_CAsfStreamPropertiesObjectV1          ;
extern unsigned char *p_CLSID_CAsfContentDescriptionObjectV0        ;
extern unsigned char *p_CLSID_AsfXAcmAudioErrorMaskingStrategy      ;
extern unsigned char *p_CLSID_AsfXSignatureAudioErrorMaskingStrategy;
extern unsigned char *p_CLSID_AsfXStreamTypeAcmAudio                ;
extern unsigned char *p_CLSID_CAsfContentEncryptionObject           ;
extern unsigned char *p_CLSID_CAsfExtendedContentDescObject         ;
extern unsigned char *p_CLSID_CAsfMarkerObjectV0                    ;
extern unsigned char *p_CLSID_CAsfLicenseStoreObject                ;
extern unsigned char *p_CLSID_CAsfStreamPropertiesObjectV2          ;
extern unsigned char *p_CLSID_CAsfExtendedStreamPropertiesObject    ;
extern unsigned char *p_CLSID_CAsfClockObjectV0                     ;
extern unsigned char *p_CLSID_AsfXMetaDataObject                    ;
extern unsigned char *p_CLSID_CAsfPacketClock1                      ;
extern unsigned char *p_g_rgiHuffDecTblMsk                          ;
extern unsigned char *p_g_rgiHuffDecTblNoisePower                   ;
extern unsigned char *p_g_rgiHuffDecTbl16smOb                       ;
extern unsigned char *p_g_rgiHuffDecTbl44smOb                       ;
extern unsigned char *p_g_rgiHuffDecTbl16ssOb                       ;
extern unsigned char *p_g_rgiHuffDecTbl44ssOb                       ;
extern unsigned char *p_g_rgiHuffDecTbl44smQb                       ;
extern unsigned char *p_g_rgiHuffDecTbl44ssQb                       ;
extern unsigned char *p_rgiMaskMinusPower10                         ;
extern unsigned char *p_rgiMaskPlusPower10                          ;
extern unsigned char *p_g_rgiBarkFreqV2                             ;
extern unsigned char *p_gRun16smOb                                  ;
extern unsigned char *p_gLevel16smOb                                ;
extern unsigned char *p_gRun16ssOb                                  ;
extern unsigned char *p_gLevel16ssOb                                ;
extern unsigned char *p_gRun44smOb                                  ;
extern unsigned char *p_gLevel44smOb                                ;
extern unsigned char *p_gRun44ssOb                                  ;
extern unsigned char *p_gLevel44ssOb                                ;
extern unsigned char *p_gRun44smQb                                  ;
extern unsigned char *p_gLevel44smQb                                ;
extern unsigned char *p_gRun44ssQb                                  ;
extern unsigned char *p_gLevel44ssQb                                ;
extern unsigned char *p_g_InvQuadRootFraction                       ;
extern unsigned char *p_g_InvQuadRootExponent                       ;
extern unsigned char *p_g_InverseFraction                           ;
extern unsigned char *p_g_SqrtFraction                              ;
#if 0
extern unsigned char *p_g_sct64                                     ;
extern unsigned char *p_g_sct128                                    ;
extern unsigned char *p_g_sct256                                    ;
extern unsigned char *p_g_sct512                                    ;
extern unsigned char *p_g_sct1024                                   ;
extern unsigned char *p_g_sct2048                                   ;
#endif
extern unsigned char *p_rgDBPower10                                 ;
extern unsigned char *p_icosPIbynp                                  ;
extern unsigned char *p_isinPIbynp                                  ;
extern unsigned char *p_g_rgiLsfReconLevel                          ;
extern unsigned char *p_gLZLTable                                   ;

// drm9
extern unsigned char *p__DRM_Sel                                    ;
extern unsigned char *p__DRM_Spbox                                  ;
#if 0

// flac
extern unsigned char *p_byte_to_unary_table                         ;
extern unsigned char *p_FLAC__crc8_table                            ;
extern unsigned char *p_FLAC__crc16_table                           ;

// ape
extern unsigned char *p_Ape_gtPowersOfTwoMinusOne                   ;
extern unsigned char *p_Ape_gtKSumMinBoundary                       ;
extern unsigned char *p_Ape_gtRangeTotalOne                         ;
extern unsigned char *p_Ape_gtRangeWidthOne                         ;
extern unsigned char *p_Ape_gtRangeTotalTwo                         ;
extern unsigned char *p_Ape_gtRangeWidthTwo                         ;
extern unsigned char *p_Ape_gtCRC32     ;

#endif
////////////////////////////////////////////////////////////////////////////////////////////

extern void wma_table_room_init(void);
extern void drm9_table_room_init(void);
//extern void flac_table_room_init(void);
//extern void ape_table_room_init(void);

#endif
