/*
********************************************************************************
*                   Copyright (c) 2008,Yangwenjie
*                         All rights reserved.
*
* File Name£º   i2s.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-14          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _I2S_H_
#define _I2S_H_

#undef  EXT
#ifdef _IN_I2S_
#define EXT
#else
#define EXT extern
#endif


/*
--------------------------------------------------------------------------------

                        Struct Define

--------------------------------------------------------------------------------
*/

typedef enum
{
    I2S_CH0,
    I2S_CH1,
    I2S_CH_MAX
}I2S_CHannel;
typedef enum
{
    I2S_INT_TX = 0,
    I2S_INT_RX,
    I2S_INT_RTX
}I2S_INT_t;
typedef enum
{
    I2S_START_NULL = 0,
    I2S_START_PIO_RX,
    I2S_START_DMA_RX,
    I2S_START_PIO_TX,
    I2S_START_DMA_TX,
    I2S_START_PIO_RTX,
    I2S_START_DMA_RTX
}I2S_Start_t;

typedef enum
{
    I2S_NORMAL_MODE = 0,
    I2S_LEFT_MODE,
    I2S_RIGHT_MODE
}I2S_BUS_MODE_t;

typedef enum
{
    PCM_EARLY_MODE = 0,
    PCM_LATE_MODE
}PCM_BUS_MODE_t;

typedef enum
{
    I2S_BUS_MODE = 0,
    PCM_BUS_MODE
}I2S_PCM_SEL_t;


typedef enum
{
    I2S_SLAVE_MODE = 0,
    I2S_MASTER_MODE,
    I2S_MASTER_MODE_LOCK,
    I2S_MASTER_MODE_UNLOCK
}I2S_mode_t;
typedef enum I2S_FS
{
    I2S_FS_8000Hz   = 8000,
    I2S_FS_11025Hz  = 11025,
    I2S_FS_12KHz    = 12000,
    I2S_FS_16KHz    = 16000,
    I2S_FS_22050Hz  = 22050,
    I2S_FS_24KHz    = 24000,
    I2S_FS_32KHz    = 32000,
    I2S_FS_44100Hz  = 44100,
    I2S_FS_48KHz    = 48000,
    I2S_FS_96KHz    = 96000,
    I2S_FS_192KHz   = 192000,
    I2S_FSSTOP      = 192000
} I2sFS_en_t;
typedef enum
{
    I2S_FORMAT = 0,
    PCM_FORMAT  = 1,
}eI2sFormat_t;
typedef enum
{
    I2S_EXT = 0,
    I2S_IN  = 1,

}eI2sCs_t;
typedef enum
{
    I2S_DATA_WIDTH16 = 16,
    I2S_DATA_WIDTH24  = 24,
}eI2sDATA_WIDTH_t;
typedef enum
{
    TX_interrupt_active = 1,
    RX_interrupt_active = 2,
}eI2sINT_SR_Event;
/*
--------------------------------------------------------------------------------

                        Funtion Declaration

--------------------------------------------------------------------------------
*/


extern int32 I2SStart(I2S_CHannel I2S_CH,I2S_Start_t TxOrRx);
extern void  I2SStop(I2S_CHannel I2S_CH,I2S_Start_t TxOrRx);
//extern int32 I2SInit(I2S_mode_t mode);
extern void  I2SDeInit(I2S_CHannel I2S_CH);
extern void I2SIntEnable(I2S_CHannel I2S_CH,I2S_INT_t INT_TxRx);
extern void I2SIntDisable(I2S_CHannel I2S_CH,I2S_INT_t INT_TxRx);
extern int32 I2SDMAEnable(I2S_CHannel I2S_CH,I2S_Start_t INT_TxRx);
extern int32 I2SDMADisable(I2S_CHannel I2S_CH,I2S_Start_t INT_TxRx);


#if 0

typedef int32 (*pI2SStart)(I2S_Start_t TxOrRx);
typedef void  (*pI2SStop)(I2S_Start_t TxOrRx);
typedef int32 (*pI2SInit)(I2S_mode_t mode);
typedef void  (*pI2SDeInit)(void);

#define I2SStart(TxOrRx) (((pI2SStart )(Addr_I2SStart ))(TxOrRx))
#define I2SStop(TxOrRx)  (((pI2SStop  )(Addr_I2SStop  ))(TxOrRx))
#define I2SInit(mode)    (((pI2SInit  )(Addr_I2SInit  ))(mode))
#define I2SDeInit()      (((pI2SDeInit)(Addr_I2SDeInit))())

#endif
extern uint32 I2sGetInt(I2S_CHannel I2S_CH);
extern gI2S_t I2sGetCH(I2S_CHannel I2S_CH);
extern uint32 I2sGetRxFIFOaddr(I2S_CHannel I2S_CH);
extern uint32 I2sGetTxFIFOaddr(I2S_CHannel I2S_CH);
extern uint32 I2SGetIntType(I2S_CHannel I2S_CH);

extern uint32 I2S_PIO_Read(I2S_CHannel I2S_CH, uint32* buffer,uint32 size);
extern uint32 I2S_PIO_Write(I2S_CHannel I2S_CH, uint32* buffer,uint32 size);
extern int32  I2SInit(I2S_CHannel I2S_CH,
              I2S_mode_t I2S_mode,
              eI2sCs_t I2S_CS,
              I2sFS_en_t I2S_FS,
              eI2sFormat_t BUS_FORMAT,
              eI2sDATA_WIDTH_t I2S_Data_width,
              I2S_BUS_MODE_t I2S_Bus_mode);
/*
********************************************************************************
*
*                         End of i2s.h
*
********************************************************************************
*/
#endif
