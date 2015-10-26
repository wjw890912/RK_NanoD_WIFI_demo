/*
********************************************************************************
*                   Copyright (c) 2008,yangwenjie
*                         All rights reserved.
*
* File Name£º   hw_spi.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-15          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_SPI_H_
#define _HW_SPI_H_

//SPIM_CTRLR0  SPIS_CTRLR0
#define SPI_MASTER_MODE             ((uint32)(0)<<20)
#define SPI_SLAVE_MODE              ((uint32)(1)<<20)

#define TRANSMIT_RECEIVE            ((uint32)(0)<<18)
#define TRANSMIT_ONLY               ((uint32)(1)<<18)
#define RECEIVE_ONLY                ((uint32)(2)<<18)
#define TRANSMIT_RECEIVE_MASK       ((uint32)(3)<<18)

#define MOTOROLA_SPI                ((uint32)(0)<<16)
#define TEXAS_INSTRUMENTS_SSP       ((uint32)(1)<<16)
#define NATIONAL_SEMI_MICROWIRE     ((uint32)(2)<<16)

#define RXD_SAMPLE_NO_DELAY         ((uint32)(0)<<14)
#define RXD_SAMPLE_1CK_DELAY        ((uint32)(1)<<14)
#define RXD_SAMPLE_2CK_DELAY        ((uint32)(2)<<14)
#define RXD_SAMPLE_3CK_DELAY        ((uint32)(3)<<14)

#define APB_BYTE_WR                 ((uint32)(1)<<13)
#define APB_HALFWORD_WR             ((uint32)(0)<<13)

#define MSB_FBIT                    ((uint32)(0)<<12)
#define LSB_FBIT                    ((uint32)(1)<<12)

#define LITTLE_ENDIAN_MODE          ((uint32)(0)<<11)
#define BIG_ENDIAN_MODE             ((uint32)(1)<<11)

#define CS_2_SCLK_OUT_1_2_CK        ((uint32)(0)<<10)
#define CS_2_SCLK_OUT_1_CK          ((uint32)(1)<<10)

#define CS_KEEP_LOW                 ((uint32)(0)<<8)
#define CS_KEEP_1_2_CK              ((uint32)(1)<<8)
#define CS_KEEP_1_CK                ((uint32)(2)<<8)

#define SERIAL_CLOCK_POLARITY_LOW   ((uint32)(0)<<7)
#define SERIAL_CLOCK_POLARITY_HIGH  ((uint32)(1)<<7)

#define SERIAL_CLOCK_PHASE_MIDDLE   ((uint32)(0)<<6)
#define SERIAL_CLOCK_PHASE_START    ((uint32)(1)<<6)

#define DATA_FRAME_4BIT             ((uint32)(0))
#define DATA_FRAME_8BIT             ((uint32)(1))
#define DATA_FRAME_16BIT            ((uint32)(2))

//SPI ENR
#define SPI_DISABLE                 ((uint32)(0x00) << 0)
#define SPI_ENABLE                  ((uint32)(0x01) << 0)

///SPIM_SR  SPIS_SR
#define RECEIVE_FIFO_FULL           ((uint32)(1)<<4)
#define RECEIVE_FIFO_EMPTY          ((uint32)(1)<<3)
#define TRANSMIT_FIFO_EMPTY         ((uint32)(1)<<2)
#define TRANSMIT_FIFO_FULL          ((uint32)(1)<<1)
#define SPI_BUSY_FLAG               ((uint32)(1))

//SPIM_DMACR SPIS_DMACR
#define TRANSMIT_DMA_ENABLE         ((uint32)(1) << 1)
#define RECEIVE_DMA_ENABLE          ((uint32)(1) << 0)

//SPI_ISR
#define FIFO_EMPTY                  ((uint32)(1) << 0)
#define FIFO_FULL                   ((uint32)(1)<<4)
typedef enum
{
    SPI_TRANSMIT = 0,
    SPI_RECEIVE
}SPI_DMA_MODE;

typedef enum SPIM_cs
{
    SPI_CS0 = 1,
    SPI_CS1,

}eSPI_cs_t;

typedef enum SPIM_TRANSFER_MODE
{
    SPI_TRANSMIT_RECEIVE = 0,
    SPI_TRANSMIT_ONLY,
    SPI_RECEIVE_ONLY,
//    SPI_EEPROM_READ
}eSPI_TRANSFER_MODE_t;

typedef enum SPIM_PHASE
{
    SPI_MIDDLE_FIRST_DATA = 0,
    SPI_START_FIRST_DATA
}eSPI_PHASE_t;

typedef enum SPIM_POLARITY
{
    SPI_SERIAL_CLOCK_LOW = 0,
    SPI_SERIAL_CLOCK_HIGH
}eSPI_POLARITY_t;

typedef enum SPIM_DMA
{
    SPI_DMA_DISABLE = 0,
    SPI_DMA_ENABLE
}eSPI_DMA_t;

#if 0
typedef enum
{
    DATA_4BIT = 0,
    DATA_5BIT,
    DATA_6BIT,
    DATA_7BIT,
    DATA_8BIT,
    DATA_9BIT,
    DATA_10BIT,
    DATA_11BIT,
    DATA_12BIT,
    DATA_13BIT,
    DATA_14BIT,
    DATA_15BIT,
    DATA_16BIT
}eSPI_DATA_WIDTH;
#endif

typedef enum
{
    DATA_WIDTH4 = 0,
    DATA_WIDTH8,
    DATA_WIDTH16
}SPI_DATA_WIDTH;

typedef enum SPI_ch
{
    SPI_CH0,
    SPI_CH1,
}eSPI_ch_t;

typedef struct tagSPI_INFO
{
    uint16 spiNum;
    uint16 baudRate;        //kHz
    SPI_DATA_WIDTH dataWidth;
    eSPI_cs_t slaveNumb;
    eSPI_TRANSFER_MODE_t transferMode;
    eSPI_PHASE_t serialClockPhase;
    eSPI_POLARITY_t  polarity;

}SPI_INFO,*pSPI_INFO;
//SPI Registers
typedef volatile struct tagSPI_STRUCT
{
    uint32 SPI_CTRLR0;
    uint32 SPI_CTRLR1;
    uint32 SPI_ENR;
    uint32 SPI_SER;
    uint32 SPI_BAUDR;
    uint32 SPI_TXFTLR;
    uint32 SPI_RXFTLR;
    uint32 SPI_TXFLR;
    uint32 SPI_RXFLR;
    uint32 SPI_SR;
    uint32 SPI_IPR;
    uint32 SPI_IMR;
    uint32 SPI_ISR;
    uint32 SPI_RISR;
    uint32 SPI_ICR;
    uint32 SPI_DMACR;
    uint32 SPI_DMATDLR;
    uint32 SPI_DMARDLR;
    uint32 reserved[(0x400 - 0x0048)/4];
    uint32 SPI_TXDR[(0x800 - 0x400)/4];
    uint32 SPI_RXDR[(0xC00 - 0x800)/4];
}SPI_REG,*pSPI_REG;

#define SPICtl  ((SPI_REG *) SPI0_BASE)

/*
********************************************************************************
*
*                         End of hw_spi.h
*
********************************************************************************
*/

#endif

