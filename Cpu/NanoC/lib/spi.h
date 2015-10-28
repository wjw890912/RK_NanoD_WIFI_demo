/*
********************************************************************************
*                   Copyright (c) 2008,Yangwenjie
*                         All rights reserved.
*
* File Name£º   spi.h
*
* Description:  define the master/slave register structure bit marco and related definition\interfaces.
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-15          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _SPI_H_
#define _SPI_H_

#undef  EXT
#ifdef _IN_SPI_
#define EXT
#else
#define EXT extern
#endif
/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/
#define SPI_CTL_TX_SPINOR   ( SPI_MASTER_MODE | TRANSMIT_ONLY | MOTOROLA_SPI \
                            | RXD_SAMPLE_NO_DELAY | APB_BYTE_WR \
                            | MSB_FBIT | LITTLE_ENDIAN_MODE | CS_2_SCLK_OUT_1_CK \
                            | CS_KEEP_LOW | SERIAL_CLOCK_POLARITY_HIGH \
                            | SERIAL_CLOCK_PHASE_START | DATA_FRAME_8BIT)

#define SPI_CTL_RX_SPINOR   ( SPI_MASTER_MODE | RECEIVE_ONLY | MOTOROLA_SPI \
                            | RXD_SAMPLE_NO_DELAY | APB_BYTE_WR \
                            | MSB_FBIT | LITTLE_ENDIAN_MODE | CS_2_SCLK_OUT_1_CK \
                            | CS_KEEP_LOW | SERIAL_CLOCK_POLARITY_HIGH \
                            | SERIAL_CLOCK_PHASE_START | DATA_FRAME_8BIT)

#define SPI_CTL_TXRX_SPINOR ( SPI_MASTER_MODE | TRANSMIT_RECEIVE | MOTOROLA_SPI \
                            | RXD_SAMPLE_NO_DELAY | APB_BYTE_WR \
                            | MSB_FBIT | LITTLE_ENDIAN_MODE | CS_2_SCLK_OUT_1_CK \
                            | CS_KEEP_LOW | SERIAL_CLOCK_POLARITY_HIGH \
                            | SERIAL_CLOCK_PHASE_START | DATA_FRAME_8BIT)

#define SPI_MAX_CH 2
/*
--------------------------------------------------------------------------------

                        Struct Define

--------------------------------------------------------------------------------
*/

/*
--------------------------------------------------------------------------------

                        Variable Define

--------------------------------------------------------------------------------
*/


/*
--------------------------------------------------------------------------------

                        Funtion Declaration

--------------------------------------------------------------------------------
*/
extern void SPIInit(uint32 ch, uint32 baudRate, uint32 CtrMode);
extern void SPIDeInit(uint32 ch);

extern rk_err_t SPIPioWrite(void);
extern rk_size_t SPIWriteFIFO(uint8 *pdata, uint32 size);

rk_err_t SPIPioRead(uint32 size);
rk_size_t SPIReadFIFO(uint8 *pdata, uint32 size);

extern rk_err_t SPIDmaWrite(void);
extern rk_err_t  SPIDmaRead(uint32 size);
extern uint32 SpiGetInt(void);
extern void SPIClearAllInt(void);
extern uint32 SpiEanbleChannel(uint32 ch);
extern uint32 SpiDisableChannel(uint32 ch);
extern void SpiEnableTxInt(void);
extern void  SpiDisalbeTxInt(void);
extern void SpiEnableRxInt(void);
extern void  SpiDisalbeRxInt(void);

extern void SpiWaitIdle(void);



/*
********************************************************************************
*
*                         End of spi.h
*
********************************************************************************
*/
#endif
