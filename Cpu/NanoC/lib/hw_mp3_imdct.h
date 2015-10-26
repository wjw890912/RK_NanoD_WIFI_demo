
/*
********************************************************************************************
*
*				  Copyright (c): 2014 - 2014 + 5, WJR
*							   All rights reserved.
*
* FileName: Cpu\NanoC\lib\hw_mp3_imdct.h
* Owner: WJR
* Date: 2014.11.10
* Time: 17:36:35
* Desc:
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    WJR     2014.11.10     17:36:35   1.0
********************************************************************************************
*/

#ifndef __CPU_NANOC_LIB_HW_MP3_IMDCT_H__
#define __CPU_NANOC_LIB_HW_MP3_IMDCT_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/

//为了打包做库方便，这里独立定义硬件配置内容，如外部硬件地址映射改变这里应做相应修改

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

#define DMA_SHORTCUT_TO_IMDCT	(1<<21)
#define DMA_SHORTCUT_FROM_IMDCT	(2<<21)


//imdct36 memory map------------------------------------------------------------------------
//#define IMDCT_BASEADDR	0x62042000
#define IMDCT_BASEADDR 	(0x60010000UL)

#define IMDCT_CTRL_REG	*((volatile unsigned long*)(IMDCT_BASEADDR + 0x0800))
#define IMDCT_STAT_REG	*((volatile unsigned long*)(IMDCT_BASEADDR + 0x0804))
#define IMDCT_INTR_REG	*((volatile unsigned long*)(IMDCT_BASEADDR + 0x0808))
#define IMDCT_EOIT_REG	*((volatile unsigned long*)(IMDCT_BASEADDR + 0x080C))

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/

extern volatile int dma1_busy;
extern volatile int dma1_trans_type;
extern volatile int imdct_finish;


extern volatile int imdct36_auto_output_addr;




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t hw_imdct_shell();
extern void imdct36_hw_pipeline(long *X,long *z);

extern void Hw_imdct_Exit(void);
extern void Hw_imdct_Init(void);

extern void imdct36_handler(void);



#endif


