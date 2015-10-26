/*
********************************************************************************
*                   Copyright (c) 2008,yangwenjie
*                         All rights reserved.
*
* File Name£º   Hw_I2c.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2008-9-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_I2C_H_
#define _HW_I2C_H_

#undef  EXT
#ifdef _IN_I2C_
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                         I2c memap register define
*
*-------------------------------------------------------------------------------
*/
typedef volatile struct I2CReg
{
    UINT32 I2C_MTXR;   // 0x00
    UINT32 I2C_MRXR;   // 0x04
    UINT32 I2C_STXR;   // 0x08
    UINT32 I2C_SRXR;   // 0x0C
    UINT32 I2C_SADDR;  // 0x10
    UINT32 I2C_IER;    // 0x14
    UINT32 I2C_ISR;    // 0x18
    UINT32 I2C_LCMR;   // 0x1C
    UINT32 I2C_LSR;    // 0x20
    UINT32 I2C_CONR;   // 0x24
    UINT32 I2C_OPR;    // 0x28
}I2CReg_t, *pI2CReg_t;
#define I2cReg                   	((I2CReg_t *) I2C0_BASE)

#define I2CPORT(n)                  ( I2cReg + n )
/*
********************************************************************************
*
*                         End of hw_i2s.h
*
********************************************************************************
*/

#endif
