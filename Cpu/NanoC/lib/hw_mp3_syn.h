/*
********************************************************************************************
*
*				  Copyright (c): 2014 - 2014 + 5, WJR
*							   All rights reserved.
*
* FileName: Cpu\NanoC\lib\hw_mp3_syn.h
* Owner: WJR
* Date: 2014.11.10
* Time: 15:43:39
* Desc:
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    WJR     2014.11.10     15:43:39   1.0
********************************************************************************************
*/

#ifndef _CPU_NANOC_LIB_HW_MP3_SYN_H__
#define _CPU_NANOC_LIB_HW_MP3_SYN_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//base------------------------------------------------------------------------------------
#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

typedef volatile unsigned int       REG32;

#define ReadReg32(addr)                     (*(REG32 *)(addr))
#define WriteReg32(addr, data)              (*(REG32 *)(addr) = data)
#define SetRegBits32(addr, databit)         WriteReg32(addr, ReadReg32(addr)|(databit))
#define ClrRegBits32(addr, databit)         WriteReg32(addr, ReadReg32(addr)&~(databit))
#define SetRegBit32(addr,bit)               WriteReg32(addr,(ReadReg32(addr)|(1<<bit)))
#define ClrRegBit32(addr,bit)               WriteReg32(addr,(ReadReg32(addr)&(~(1<<bit))))
#define GetRegBit32(addr,bit)               (ReadReg32(addr)&(1<<bit))
#define MaskRegBits32(addr, y, z)           WriteReg32(addr, (ReadReg32(addr)&~(y))|(z))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

//dma memory map---------------------------------------------------------------------------
//#define DMA_BASE        0x62040000
#define DMA_BASE 			     (0x60000000UL)

#define DMA_CTL1        (DMA_BASE + 0x1C) // DMA Ch1 transfer Control register
#define DMA_ISA1        (DMA_BASE + 0x20) // DMA Channel 1 Initial Source Address register
#define DMA_IDA1        (DMA_BASE + 0x24) // DMA Channel 1 Initial Destination Address register
#define DMA_ICNT1       (DMA_BASE + 0x28) // DMA Channel 1 Initial terminate count register & half trans interrupt value
#define DMA_CSA1        (DMA_BASE + 0x2C) // DMA Channel 1 Current Source Address register
#define DMA_CDA1        (DMA_BASE + 0x30) // DMA Channel 1 Current Destination Address register
#define DMA_CCNT1       (DMA_BASE + 0x34) // DMA Channel 1 current count register

#define DMA_ISR         (DMA_BASE + 0x38) // Interrupt status register
#define DMA_ICON        (DMA_BASE + 0x3C) // Interrupt configure register
#define DMA_DSR         (DMA_BASE + 0x40) // DMA status register
#define DMA_PRIO        (DMA_BASE + 0x44) // DMA arbitration priority mode
#define DMA_RSR         (DMA_BASE + 0x48) // DMA request select register
#define DMA_ICR         (DMA_BASE + 0x4C) // DMA interrupt clear register

#define DMA_CHEN_BIT        (1<<0)

#define DMA_HTIE1_BIT       (1<<4)    // Channel 1 Half_trans_int_enable
#define DMA_TCIE1_BIT       (1<<5)    // Channel 1 Trans_complete_int_enable
#define DMA_HTIM1_BIT       (1<<6)    // Channel 1 Half_trans_int_mask
#define DMA_TCIM1_BIT       (1<<7)   // Channel 1 Trans_complete_int_mask

#define DMA_DIC_BIT         (1<<0)    // DMA_int_clear
#define DMA_HTIC0_BIT       (1<<1)    // Channel 0 Half_trans_int_clear
#define DMA_TCIC0_BIT       (1<<2)    // Channel 0 Trans_complete_int_clear
#define DMA_HTIC1_BIT       (1<<3)    // Channel 1 Half_trans_int_clear
#define DMA_TCIC1_BIT       (1<<4)    // Channel 1 Trans_complete_int_clear
#define DMA_SHORTCUT_TO_SYNTH	(3<<21)
#define DMA_SHORTCUT_FROM_SYNTH	(4<<21)

typedef  signed int mad_fixed_t;

enum mad_layer
{
    MAD_LAYER_I   = 1,			/* Layer I */
    MAD_LAYER_II  = 2,			/* Layer II */
    MAD_LAYER_III = 3			/* Layer III */
};

enum mad_mode
{
    MAD_MODE_SINGLE_CHANNEL = 0,		/* single channel */
    MAD_MODE_DUAL_CHANNEL	  = 1,		/* dual channel */
    MAD_MODE_JOINT_STEREO	  = 2,		/* joint (MS/intensity) stereo */
    MAD_MODE_STEREO	  = 3		/* normal LR stereo */
};

enum mad_emphasis
{
    MAD_EMPHASIS_NONE	  = 0,		/* no emphasis */
    MAD_EMPHASIS_50_15_US	  = 1,		/* 50/15 microseconds emphasis */
    MAD_EMPHASIS_CCITT_J_17 = 3,		/* CCITT J.17 emphasis */
    MAD_EMPHASIS_RESERVED   = 2		/* unknown emphasis */
};

struct mad_header
{
    enum mad_layer layer;			/* audio layer (1, 2, or 3) */
    enum mad_mode mode;			/* channel mode (see above) */
    int mode_extension;			/* additional mode info */
    enum mad_emphasis emphasis;		/* de-emphasis to use (see above) */

    unsigned long bitrate;		/* stream bitrate (bps) */
    unsigned int samplerate;		/* sampling frequency (Hz) */

    unsigned short crc_check;		/* frame CRC accumulator */
    unsigned short crc_target;		/* final target CRC checksum */

    int flags;				/* flags (see below) */
    int private_bits;			/* private bits (see below) */

};
typedef struct mad_frame
{
    struct mad_header header;		/* MPEG audio header */

    int options;				/* decoding options (from stream) */

    mad_fixed_t (*sbsample)[36][32];	/* synthesis subband filter samples */

    mad_fixed_t (*overlap)[2*32*18];	/* Layer III block overlap data */

    short *output_ptrL;
    short *output_ptrR;
    int output_pos[2];	//two channels, current output position

    int phase[2];			//synthesis phase (0~15)
}mad_frame;
struct mad_pcm
{
    unsigned int samplerate;		/* sampling frequency (Hz) */
    unsigned short channels;		/* number of channels */
    unsigned short length;		/* number of samples per channel */
};

struct mad_synth
{
    unsigned int phase;			/* current processing phase */
    struct mad_pcm pcm;			/* PCM output */
};

enum
{
    MAD_PCM_CHANNEL_SINGLE = 0
};

enum
{
    MAD_PCM_CHANNEL_DUAL_1 = 0,
    MAD_PCM_CHANNEL_DUAL_2 = 1
};

enum
{
    MAD_PCM_CHANNEL_STEREO_LEFT  = 0,
    MAD_PCM_CHANNEL_STEREO_RIGHT = 1
};



//synth memory map--------------------------------------------------------------------------
//#define	SYNTH_BASEADDR	0x62044000
#define	SYNTH_BASEADDR 	(0x60020000UL)

#define SYNTH_CTRL_REG	*((volatile unsigned long*)(SYNTH_BASEADDR + 0x0800))
#define SYNTH_CNFG_REG	*((volatile unsigned long*)(SYNTH_BASEADDR + 0x0804))
#define SYNTH_STAT_REG	*((volatile unsigned long*)(SYNTH_BASEADDR + 0x0808))
#define SYNTH_INTR_REG	*((volatile unsigned long*)(SYNTH_BASEADDR + 0x080C))
#define SYNTH_EOIT_REG	*((volatile unsigned long*)(SYNTH_BASEADDR + 0x0810))
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t hw_syn_shell();
extern void Hw_syn_Exit(void);
extern void mad_synth_init(struct mad_synth *synth);
extern void mp3_wait_synth(void);
extern void mad_synth_fouth_frame();
extern void synth_handler(void);



#endif

