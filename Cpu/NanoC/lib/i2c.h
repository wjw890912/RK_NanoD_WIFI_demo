/*
********************************************************************************
*                   Copyright (c) 2008,yangwenjie
*                         All rights reserved.
*
* File Name£º   I2c.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-14          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _I2C_H_
#define _I2C_H_

#undef  EXT
#ifdef _IN_I2C_
#define EXT
#else
#define EXT extern
#endif

/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/
// Interrupt status bit
#define I2C_INT_MACK            ((UINT32)0x00000001 << 0)   //Master receives ACK interrupt status bit
#define I2C_INT_MACKP           ((UINT32)0x00000001 << 1)   //Master ACK period interrupt status bit
#define I2C_INT_SACK            ((UINT32)0x00000001 << 2)   //Slave receives ACK interrupt status bit
#define I2C_INT_SACKP           ((UINT32)0x00000001 << 3)   //Slave ACK period interrupt status bit
#define I2C_INT_SAM             ((UINT32)0x00000001 << 4)   //Slave address matches status bit
#define I2C_INT_SBA             ((UINT32)0x00000001 << 5)   //Broadcast address
#define I2C_INT_SAS             ((UINT32)0x00000001 << 6)   //Abnormal stop occurs
#define I2C_INT_AL              ((UINT32)0x00000001 << 7)   //Arbitration Lost

// I2C operation bit
#define I2C_OPR_DIV_SPEED_MASK   0x3F
#define I2C_OPR_ENABLE          ((UINT32)0x00000001 << 6)      //I2C core enable bit
#define I2C_OPR_RESET           ((UINT32)0x00000001 << 7)      //I2C state machine (both master/slave) reset bit
#define I2C_OPR_S10ADDR         ((UINT32)0x00000001 << 8)       //Slave 10 bits address mode
#define I2C_OPR_S7ADDR          ((UINT32)0x00000000 << 8)       //Slave 10 bits address mode

//I2C control register
#define I2C_LCMR_START          ((UINT32)0x00000001 << 0)
#define I2C_LCMR_STOP           ((UINT32)0x00000001 << 1)
#define I2C_LCMR_RESUME         ((UINT32)0x00000001 << 2)

//I2C line status register
#define I2C_LSR_BUSY            ((UINT32)0x00000001 << 0)   //I2C core busy status bit
#define I2C_LSR_NACK            ((UINT32)0x00000001 << 1)   //I2C receives NACK status bit

// I2C operation mode bit
#define I2C_CON_MASTER_TX       ((UINT32)0x00000003 << 2)
#define I2C_CON_MASTER_RX       ((UINT32)0x00000001 << 2)
#define I2C_CON_ACK             ((UINT32)0x00000000 << 4)   //I2C bus acknowledge enable register
#define I2C_CON_NACK            ((UINT32)0x00000001 << 4)   //I2C bus acknowledge enable register
#define I2C_CONR_MASTER_MASK    ((UINT32)0x00000003 << 2)
#define I2C_CONR_MRX            ((UINT32)0x00000001 << 2)  //Master receive mode
#define I2C_CONR_MTX            ((UINT32)0x00000003 << 2)  //Master transmit mode

#define I2C_CON_MASTER_TX       ((UINT32)0x00000003 << 2)
#define I2C_CON_MASTER_RX       ((UINT32)0x00000001 << 2)

#define I2C_READ_BIT            (1)
#define I2C_WRITE_BIT           (0)

#define I2C_10ADDR_PREFIX       (0x1e)

/* interrupt enable register */
/*#define I2C_IEN			    0x018*/
#define I2C_BTFIEN		        ((UINT32)(1 << 0))        //BYTE transmit finish interrupt enable
#define I2C_BRFIEN		        ((UINT32)(1 << 1))        //BYTE receive finish interrupt enable
#define I2C_MBTFIEN		        ((UINT32)(1 << 2))        //MTXCNT data transmit finish interrupt enable
#define I2C_MBRFIEN		        ((UINT32)(1 << 3))        //MRXCNT data receive finish interrupt enable
#define I2C_STARTIEN		    ((UINT32)(1 << 4))        //start operate finish interrupt enable
#define I2C_STOPIEN		        ((UINT32)(1 << 5))        //stop operate finish interrupt enable
#define I2C_NAKRCVIEN		    ((UINT32)(1 << 6))        //NAK handshake receive interrupt enable

#define IRQ_MST_ENABLE          (I2C_MBTFIEN | I2C_MBRFIEN | I2C_NAKRCVIEN | I2C_STARTIEN | I2C_STOPIEN)
#define IRQ_ALL_DISABLE         0

/* interrupt pending register */
/*#define I2C_IPD                 0x01c*/
#define I2C_BTFIPD              ((UINT32)(1 << 0))    //BYTE transmit finish interrupt pending bit
#define I2C_BRFIPD              ((UINT32)(1 << 1))    //BYTE receive finish interrupt pending bit
#define I2C_MBTFIPD             ((UINT32)(1 << 2))    //MTXCNT data transmit finished interrupt pending bit
#define I2C_MBRFIPD             ((UINT32)(1 << 3))    //MRXCNT data received finished interrupt pending bit
#define I2C_STARTIPD            ((UINT32)(1 << 4))    //start operation finished interrupt pending bit
#define I2C_STOPIPD             ((UINT32)(1 << 5))    //stop operation finished interrupt pending bit
#define I2C_NAKRCVIPD           ((UINT32)(1 << 6))    //NAK handshake received interrupt pending bit
/*
--------------------------------------------------------------------------------

                        Struct Define

--------------------------------------------------------------------------------
*/
 typedef enum I2C_slaveaddr
{
    tvp5145         = 0x5C,
    WM8987codec     = 0x34,
    ALC5633codec    = 0x38,
    RDA5820AD       = 0x22,
    FM5767          = 0xC0,
    FM5800          = 0x20,
    RTCM41          = 0xD0,
    InterCodec      = 0x4E,
    AR1010          = 0x20,
    FM5802          = 0x20
}eI2C_slaveaddr_t;

typedef enum I2C_mode
{
    WriteMode,
    ReadMode,
}eI2C_mode_t;
typedef enum I2C_RW_mode
{
    NormalMode,
    DirectMode,
    RDA_5820Mode,
    YDA_174Mode		//mlc add for yamaha yda174
}eI2C_RW_mode_t;

typedef enum _EI2C_CHANNEL
{
    I2C_CH0,   //0
    I2C_CH1,
    I2C_CH2,

    I2C_CH_MAX,
}eI2C_CHANNEL;


typedef enum I2C_Address_Reg
{
    I2C_10BIT_ADDRESS_16BIT_REG = 0,
    I2C_10BIT_ADDRESS_8BIT_REG,
    I2C_7BIT_ADDRESS_16BIT_REG,
    I2C_7BIT_ADDRESS_8BIT_REG,
}eI2C_Address_Reg_t;
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


extern void  I2CStart(eI2C_CHANNEL i2cPort);
extern void  I2CStop(eI2C_CHANNEL i2cPort);

extern int32 I2CSendData(eI2C_CHANNEL i2cPort,
                            UINT16 slaveaddr,
                            UINT16 regaddr,
                            eI2C_Address_Reg_t addr_reg_mode,
                            UINT8* Databuf,
                            uint32 size,
                            eI2C_RW_mode_t rw_mode);
//extern int32 I2CReadData(eI2C_CHANNEL i2cPort,UINT8 *Databuf,uint32 len);
//extern int32 I2CReadData(eI2C_CHANNEL i2cPort, UINT8 slaver_addr, uint reg_addr, uint r_len,UINT8 *Databuf,uint32 len);
extern int32 I2CReadData(eI2C_CHANNEL i2cPort,
                        UINT8 slaver_addr,
                        uint16 reg_addr,
                        eI2C_Address_Reg_t addr_reg_mode,
                        UINT8 *Databuf,
                        uint32 len,
                        eI2C_RW_mode_t rw_mode);

extern BOOL I2C_Init(eI2C_CHANNEL i2cPort,uint16 speed);
int i2c_write(eI2C_CHANNEL i2cPort,uint16 slaveaddr, uint16 regaddr,eI2C_Address_Reg_t addr_reg_mode, uint8 *buffer, int len,eI2C_RW_mode_t rw_mode);

int i2c_read(eI2C_CHANNEL i2cPort,uint8 slaveaddr, uint16 regaddr, eI2C_Address_Reg_t addr_reg_mode, uint8 *buf, int len,eI2C_RW_mode_t rw_mode);

//extern int i2c_write(eI2C_CHANNEL i2cPort,uint16 slaveaddr, uint16 regaddr, int reglen, uint8 *buffer, int len);

//extern BOOL I2C_Write( eI2C_CHANNEL i2cPort,UINT8 pData,BOOL StartBit);
extern BOOL I2C_ReadConfig(eI2C_CHANNEL i2cPort,BOOL StartBit);
extern BOOL I2C_Deinit(eI2C_CHANNEL i2cPort);
extern BOOL I2C_Read_SendACK(eI2C_CHANNEL i2cPort,BOOL flg);
extern BOOL I2CSetSpeed(eI2C_CHANNEL i2cPort,UINT16 speed);

/*
********************************************************************************
*
*                         End of Example.h
*
********************************************************************************
*/
#endif
