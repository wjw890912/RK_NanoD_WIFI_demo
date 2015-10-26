/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   I2c.c
*
* Description:  C program template
*
* History:      <author>          <time>        <version>
*             yangwenjie      2008-11-20         1.0
*    desc:    ORG.
********************************************************************************
*/
#include "BspConfig.h"
#ifdef  __DRIVER_I2C_I2CDEVICE_C__
#define _IN_I2C_
#include "typedef.h"
#include "DriverInclude.h"

//#ifdef DRIVERLIB_IRAM

/*
--------------------------------------------------------------------------------
  Function name : I2CStart(void)
  Author        : yangwenjie
  Description   : I2C start

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void I2CStart(eI2C_CHANNEL i2cPort)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    // Set start to LCMR
    pReg->I2C_LCMR |= (I2C_LCMR_RESUME | I2C_LCMR_START);
}

/*
--------------------------------------------------------------------------------
  Function name : I2CStop(void)
  Author        : yangwenjie
  Description   : I2C stop

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void I2CStop(eI2C_CHANNEL i2cPort)
{
    int timeout;
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    timeout = 200000;   // timeout must long enogh,that will suit some slow suitation.

    pReg->I2C_LCMR |=( I2C_LCMR_RESUME | I2C_LCMR_STOP);

    while (((pReg->I2C_LCMR & I2C_LCMR_STOP) != 0) && (timeout > 0))
    {
        Delay10cyc(1);
        timeout--;
    }

    pReg->I2C_LCMR = 0x4;
}


/*
--------------------------------------------------------------------------------
  Function name : I2CStart(void)
  Author        : yangwenjie
  Description   : I2C start

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void I2CEnableInt(eI2C_CHANNEL i2cPort, uint32 IntType)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    pReg->I2C_IER |= IntType;
}


/*
--------------------------------------------------------------------------------
  Function name : I2CStart(void)
  Author        : yangwenjie
  Description   : I2C start

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void I2CDisableInt(eI2C_CHANNEL i2cPort, uint32 IntType)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    pReg->I2C_IER &= ~IntType;
}

void I2CDisableIRQ(eI2C_CHANNEL i2cPort)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    //pReg->I2C_IEN = IRQ_ALL_DISABLE ;
}

uint32 I2CGetInt(eI2C_CHANNEL i2cPort)
{
    I2CReg_t *  pReg;
    uint32 intstatus;

    pReg = I2CPORT(i2cPort);

    intstatus = pReg->I2C_ISR;

    if(intstatus & I2C_INT_MACK)
    {
        pReg->I2C_ISR &= ~I2C_INT_MACK;
    }
    else if(intstatus & I2C_INT_MACKP)
    {
        pReg->I2C_ISR &= ~I2C_INT_MACKP;
    }


    return intstatus;
}


/*
--------------------------------------------------------------------------------
  Function name : I2CSendData(UINT8 Data, BOOL StartBit)
  Author        : yangwenjie
  Description   :

  Input         : Data -- data will send(UINT8)

  Return        : 0 -- success
                  non 0 -- failure

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

int32 I2CSendData(eI2C_CHANNEL i2cPort,
                        UINT16 slaveaddr,
                        UINT16 regaddr,
                        eI2C_Address_Reg_t addr_reg_mode,
                        UINT8* Databuf,
                        uint32 size,
                        eI2C_RW_mode_t rw_mode)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);

     // Clear status register
    pReg->I2C_ISR = 0x0;

    pReg->I2C_CONR &= ~(I2C_CONR_MASTER_MASK);
    pReg->I2C_CONR |= I2C_CONR_MTX;
    // Set MTXR

    //I2cReg->I2C_MTXR = Data;//send data
    pReg->I2C_MTXR = *Databuf;//send data
    if (1)
    {
        //I2cReg->I2C_MTXR = Data | I2C_WRITE_BIT;//send slave addr data
        pReg->I2C_LCMR = (I2C_LCMR_RESUME | I2C_LCMR_START);
    }
    else
    {
        pReg->I2C_LCMR = I2C_LCMR_RESUME ;
    }

    // ACK 释放SDA线,等待从设备应答
    I2cReg->I2C_CONR &= ~(I2C_CON_NACK);
    // waiting ACK

}


/*
--------------------------------------------------------------------------------
  Function name : I2CReadData(UINT8 *Data)
  Author        : yangwenjie
  Description   :

  Input         : Data -- to put the read data.

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

int32 I2CReadData(eI2C_CHANNEL i2cPort,
                        UINT8 slaver_addr,
                        uint16 reg_addr,
                        eI2C_Address_Reg_t addr_reg_mode,
                        UINT8 *Databuf,
                        uint32 len,
                        eI2C_RW_mode_t rw_mode)
{
    int intstatus;
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    *Databuf = (UINT8)(pReg ->I2C_MRXR);
    return OK;
}
//#endif

/*
--------------------------------------------------------------------------------
  Function name : I2CSetSpeed(UINT16 speed)
  Author        : yangwenjie
  Description   :

  Input         : speed -- unit is k,for example 400 is mean 400k

  Return        :TRUE -- success
                 FALSE -- failure

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
BOOL I2CSetSpeed(eI2C_CHANNEL i2cPort,UINT16 speed)
{
    UINT32 exp;
    UINT32 rem;
    UINT32 scalespeed;
    UINT32 freqbase;

    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);

    pReg->I2C_OPR &= ~( I2C_OPR_ENABLE);


	freqbase   = (GetPclkFreq()*1000) / 5;//by YWJ 04.19

    scalespeed = speed << 3;      // speed以K为单位   (rem+1)<=8 ,先以8来确定exp

    for (exp = 0;exp < 7;exp++)
    {
        scalespeed = scalespeed << 1;
        if (scalespeed > freqbase)
        {
            break;
        }
    }

    rem = (freqbase) / speed / (1 << (exp + 1))/* - 1*/;

    pReg->I2C_OPR &= ~(I2C_OPR_DIV_SPEED_MASK);
    pReg->I2C_OPR |= ((rem << 3) | exp);

    pReg->I2C_OPR |= I2C_OPR_ENABLE;

    return TRUE;
}

BOOL I2C_Read_SendACK(eI2C_CHANNEL i2cPort,BOOL flg)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);
    if(flg)
    {
        pReg->I2C_CONR |= I2C_CON_NACK;
    }
    else
    {
        pReg->I2C_CONR &= ~(I2C_CON_NACK);
    }
}
/*
--------------------------------------------------------------------------------
  Function name : I2C_Init(UINT8 SlaveAddress, UINT16 nKHz)
  Author        : yangwenjie
  Description   :

  Input         : SlaveAddress :I2C device address
                  nKHz         :I2C max speed

  Return        :TRUE -- ok
                 FALSE -- fail

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
BOOL I2C_Init(eI2C_CHANNEL i2cPort,uint16 speed)
{
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);

    pReg->I2C_OPR &= ~(I2C_OPR_RESET);
    pReg->I2C_IER = 0x0;

    pReg->I2C_OPR &= ~(I2C_OPR_ENABLE);

    pReg->I2C_OPR |= I2C_OPR_ENABLE;

    return TRUE;
}
/*
--------------------------------------------------------------------------------
  Function name : i2c_write(uchar chip, uint addr, int alen, uchar *buf, int len)
  Description   : Write to i2c memory

  Input         : i2cPort: i2c channel
                  slaveaddr:target i2c address
                  regaddr:	address to send to
                  reglen:
                  buffer:	buffer for send data
                  len:	length of bytes to be send

  Return        :TRUE -- ok
                 FALSE --fail

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
int i2c_write(eI2C_CHANNEL i2cPort,
                uint16 slaveaddr,
                uint16 regaddr,
                eI2C_Address_Reg_t addr_reg_mode,
                uint8 *buffer,
                int len,
                eI2C_RW_mode_t rw_mode)
{
	if ((buffer == NULL) && (len != 0))
    {
		printf ("i2c_write: buf == NULL\n");
		return ERROR;
	}
    if(len == 0)
    {
		printf("i2c_read: len = 0 \n");
		return ERROR;
	}

	return I2CSendData(i2cPort, slaveaddr, regaddr, addr_reg_mode, buffer, len, rw_mode);
}

/*
--------------------------------------------------------------------------------
  Function name : i2c_read(uchar slaveaddr, uint regaddr, int reglen, uchar *buf, int len)
  Author        :
  Description   : Read from i2c memory

  Input         : i2cPort: i2c channel
                  slaveaddr:	target i2c address
                  regaddr:	address to read from
                  reglen:
                  buffer:	buffer for read data
                  len:	length of bytes to be read

  Return        :TRUE -- ok
                 FALSE --fail

  History:     <author>         <time>         <version>
  desc:         ORG
--------------------------------------------------------------------------------
*/
int i2c_read(eI2C_CHANNEL i2cPort,
                uint8 slaveaddr,
                uint16 regaddr,
                eI2C_Address_Reg_t addr_reg_mode,
                uint8 *buf,
                int len,
                eI2C_RW_mode_t rw_mode)
{
	if ((buf == NULL) && (len != 0))
    {
		printf("i2c_read: buf == NULL\n");
		return ERROR;
	}

    if(len == 0)
    {
		printf("i2c_read: len = 0 \n");
		return ERROR;
	}

	return I2CReadData(i2cPort, slaveaddr, regaddr, addr_reg_mode, buf, len,rw_mode);
}


/*
--------------------------------------------------------------------------------
  Function name : I2C_Read(UINT8 RegAddr, UINT8 *pData, UINT16 size, eI2C_mode_t mode)
  Author        : yangwenjie
  Description   :

  Input         : RegAddr
                  size -- read size
                  mode -- NormalMode/DirectMode

  Return        :TRUE -- ok
                 FALSE --fail

  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
BOOL I2C_ReadConfig(eI2C_CHANNEL i2cPort,BOOL StartBit)
{
    BOOL ret = TRUE;
    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);

     pReg->I2C_ISR = 0x0;

 // Clear status register
    pReg->I2C_CONR &= ~( I2C_CONR_MASTER_MASK);
    pReg->I2C_CONR |= I2C_CONR_MRX;

    pReg->I2C_LCMR |= I2C_LCMR_RESUME;

    return ret;
}

/*
--------------------------------------------------------------------------------
  Function name : I2C_Deinit(void)
  Author        : yangwenjie
  Description   :
  History:     <author>         <time>         <version>
             yangwenjie     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
BOOL I2C_Deinit(eI2C_CHANNEL i2cPort )
{

    I2CReg_t *  pReg;

    pReg = I2CPORT(i2cPort);

    pReg->I2C_OPR &= ~(I2C_OPR_ENABLE);
    pReg->I2C_OPR |= I2C_OPR_RESET;

    return TRUE;
}

#endif
/*
********************************************************************************
*
*                         End of I2S.c
*
********************************************************************************
*/
