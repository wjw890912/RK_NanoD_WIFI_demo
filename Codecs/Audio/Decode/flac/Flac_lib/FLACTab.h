#ifndef FLACTAB_H
#define FLACTAB_H

#include   "ordinals.h"
#if 0
#define    FLAC_TAB_BASE_ADDR                                             0x1000c400
#define    TBL_OFFSET_byte_to_unary_table                                 0xb698    //  256
#define    TBL_OFFSET_FLAC__crc8_table                                    0xb798    //  256
#define    TBL_OFFSET_FLAC__crc16_table                                   0xb898    //  512
#endif
void flac_table_room_init(void);
// flac
extern unsigned char *p_byte_to_unary_table                         ;
extern unsigned char *p_FLAC__crc8_table                            ;
extern unsigned char *p_FLAC__crc16_table                           ;



#endif