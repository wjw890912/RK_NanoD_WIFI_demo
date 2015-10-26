/*
********************************************************************************
*                   Copyright (c) 2008,Yangwenjie
*                         All rights reserved.
*
* File Name£º   spi.c
*
* Description:
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-15          1.0
*    desc:    ORG.
********************************************************************************
*/
#define _IN_SPI_

#include "typedef.h"
#include "DriverInclude.h"
uint32 g_spimFreq;
void SPIClearAllInt(void)
{
    SPICtl->SPI_ICR = 0;
}

void SpiWaitIdle(void)
{
    while(SPICtl->SPI_SR & 0X01);
}

uint32 SpiGetInt(void)
{
    uint32 flag;
    flag = SPICtl->SPI_ISR;
    SPICtl->SPI_ICR = 0;
    return flag;
}

void SpiEnableTxInt(void)
{
    SPICtl->SPI_IMR = FIFO_EMPTY;
}

void SpiDisalbeTxInt(void)
{
    SPICtl->SPI_IMR &= ~FIFO_EMPTY;
}

void SpiEnableRxInt(void)
{
    SPICtl->SPI_IMR = FIFO_FULL;
}

void SpiDisalbeRxInt(void)
{
    SPICtl->SPI_IMR &= ~FIFO_FULL;
}



uint32 SpiEanbleChannel(uint32 ch)
{
    SPICtl->SPI_SER  |= ((uint32)(0x01) << ch);
}


uint32 SpiDisableChannel(uint32 ch)
{
    SPICtl->SPI_SER  &= ~((uint32)(0x01) << ch);
}


/*
--------------------------------------------------------------------------------
  Function name : int32 SPIWrite(uint32 ch, uint32 baudRate, uint32 CtrMode, uint8 *pdata, uint32 size)
  Author        :
  Description   :

  Input         : baudRate:   unit:HZ
                  ch¡êo        spi cs
  Return        :

  History:     <author>         <time>         <version>
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
void SPIDeInit(uint32 ch)
{
    SPICtl->SPI_ENR = 0;
    SPICtl->SPI_SER = 0;
}

/*
--------------------------------------------------------------------------------
  Function name : int32 SPIWrite(uint32 ch, uint32 baudRate, uint32 CtrMode, uint8 *pdata, uint32 size)
  Author        :
  Description   :

  Input         : baudRate:   ¨°?Hz?a¦Ì£¤??
                  ch¡êo        spi cs
  Return        :

  History:     <author>         <time>         <version>
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
rk_err_t SPIPioWrite()
{
    SPICtl->SPI_ENR = SPI_DISABLE;
    SPICtl->SPI_CTRLR0 &= ~(TRANSMIT_RECEIVE_MASK);
    SPICtl->SPI_CTRLR0 |= TRANSMIT_ONLY;
    SPICtl->SPI_ENR = SPI_ENABLE;
    return RK_SUCCESS;
}

rk_size_t SPIWriteFIFO(uint8 *pdata, uint32 size)
{
     int32  totalsize;

     totalsize = size;

     while (size)
     {
         if ((SPICtl->SPI_SR & TRANSMIT_FIFO_FULL) != TRANSMIT_FIFO_FULL)
         {
             SPICtl->SPI_TXDR[0] = *pdata++;
             size--;
         }
         else
         {
             break;
         }
     }

    return (totalsize - size);
}


/*
--------------------------------------------------------------------------------
  Function name : int32 SPIRead(uint32 ch, uint32 baudRate, uint32 CtrMode, uint8 *pdata, uint32 size)
  Author        :
  Description   :

  Input         : baudRate:   unit:HZ
                  ch¡êo        spi cs
  Return        :

  History:     <author>         <time>         <version>
  desc:         ORG
  Note:
--------------------------------------------------------------------------------
*/
rk_err_t SPIPioRead(uint32 size)
{

    SPICtl->SPI_ENR = SPI_DISABLE;
    SPICtl->SPI_CTRLR0 &= ~(TRANSMIT_RECEIVE_MASK);
    SPICtl->SPI_CTRLR0 |= RECEIVE_ONLY;
    SPICtl->SPI_CTRLR1 = size - 1;
    SPICtl->SPI_ENR = SPI_ENABLE;

    return RK_SUCCESS;
}


rk_size_t SPIReadFIFO(uint8 *pdata, uint32 size)
{

    int32  totalsize;

    totalsize = size;

    while (size)
    {
        if ((SPICtl->SPI_SR & RECEIVE_FIFO_EMPTY) != RECEIVE_FIFO_EMPTY)
        {
            *pdata++ = (uint8)(SPICtl->SPI_RXDR[0] & 0xFF);
            size--;
        }
        else
        {
            break;
        }
    }

    return (totalsize - size);


}



/*
--------------------------------------------------------------------------------
  Function name : int32 SPIDmaWrite(uint32 ch, uint32 baudRate, uint32 CtrMode, uint8 *pdata, uint32 size, pFunc CallBack)
  Author        :
  Description   :

  Input         : baudRate:   unit:HZ
                  ch¡êo        spi cs
  Return        :

  History:     <author>         <time>         <version>
  desc:         ORG
  Note:        After finish spi transfer,need call SPIDeInit function to release bus.
--------------------------------------------------------------------------------
*/
rk_err_t SPIDmaWrite()
{
    SPICtl->SPI_ENR = SPI_DISABLE;
    SPICtl->SPI_CTRLR0 &= ~(TRANSMIT_RECEIVE_MASK);
    SPICtl->SPI_CTRLR0 |= SPI_CTL_TX_SPINOR;
    SPICtl->SPI_DMACR   = TRANSMIT_DMA_ENABLE;
    SPICtl->SPI_ENR = SPI_ENABLE;

    return RK_SUCCESS;
}

/*
--------------------------------------------------------------------------------
  Function name : int32 SPIDmaRead(uint32 ch, uint32 baudRate, uint32 CtrMode, uint8 *pdata, uint32 size, pFunc CallBack)
  Author        :
  Description   :

  Input         : baudRate:   unit:HZ
                  ch¡êo        spi cs
  Return        :

  History:     <author>         <time>         <version>
  desc:         ORG
  Note:        After finish spi transfer,need call SPIDeInit function to release bus.
--------------------------------------------------------------------------------
*/
rk_err_t  SPIDmaRead(uint32 size)
{
    SPICtl->SPI_ENR = SPI_DISABLE;
    SPICtl->SPI_CTRLR0 &= ~(TRANSMIT_RECEIVE_MASK);
    SPICtl->SPI_CTRLR0 |= SPI_CTL_RX_SPINOR;
    SPICtl->SPI_CTRLR1 = size - 1;
    SPICtl->SPI_DMACR   = RECEIVE_DMA_ENABLE;
    SPICtl->SPI_ENR = SPI_ENABLE;

    return RK_SUCCESS;
}

#if 0
/*----------------------------------------------------------------------
Name      : SPIMBaudRate(uint32 baudRate,uint32 APBnKHz)
Desc      : ¨¦¨¨??SPI MASTER ¦Ì??¨´?¨¨¡ê?
Params    : baudRate:SPI¦Ì??¦Ì?¨º¡ê?¨°?KHz?a¦Ì£¤??
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
int32 SPIMBaudRate(eSPI_ch_t spiChNumb,uint32 baudRate,uint32 APBnKHz)
{
    int32 ret=0;
    pSPI_REG spiMasterReg;

    if(spiChNumb == SPI_CH0)
    {
        spiMasterReg = (pSPI_REG) SPI0_BASE;
    }
    else if(spiChNumb == SPI_CH1)
    {
         spiMasterReg = (pSPI_REG) SPI1_BASE;
    }
    else
    {
       ret = -1;
    }
    spiMasterReg->SPI_BAUDR = APBnKHz/baudRate;
    if (spiMasterReg->SPI_BAUDR == 0)
    {
        ret = -1;
    }

    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPIMUpdateAllApbFreq(void)
Desc      : ¦Ì¡Â??APB?¦Ì?¨º¨º¡À¨°2?¡ã¨º¡À¦Ì¡Â??SPI¦Ì??¦Ì?¨º
Params    :
Return    :?T
----------------------------------------------------------------------*/
void SPIMUpdateAllApbFreq(uint32 APBnKHz)
{
    if(g_spimFreq > 0)
    {
        SPIMBaudRate(g_spimFreq,APBnKHz);
    }
}

/*----------------------------------------------------------------------
Name      : SPIMInit(uint16 baudRate,  eSPIM_cs_t slaveNumb, eSPIM_TRANSFER_MODE_t transferMode, eSPIM_PHASE_t serialClockPhase, eSPIM_POLARITY_t  polarity)
Desc      : ¨¦¨¨??SPI MASTER ¦Ì?3?¨º??¡¥
Params    : baudRate:SPI MASTER ¦Ì?¨º¡À?¨®?¦Ì?¨º ¨°?KHz?a¦Ì£¤??
            slaveNumb¡êo ¡¤¡é??????¡ä¨®¨¦¨¨¡À?  ;;SPIM_CH0 SPIM_CH1
            transferMode¡êo ¡ä?¨º?¡¤?¨º?¡ê????¨®¨º?¡êo2¡ê???¡¤¡é?¨ª¡êo1; ¡¤¡é?¨ªo¨ª?¨®¨º?¡êo0; EEROM ?¨¢¡êo3
            serialClockPhase¡êo ??D?¡ê?¨º???¦Ì???¨®DD¡ì?¨°¦Ì¨ª¦Ì???¨®DD¡ì  0:¨¦?¨¦y???¨¢¨¨?¦Ì?¨ºy?Y¨®DD¡ì 1¡êo???¦Ì???¨¢¨¨?¦Ì?¨ºy?Y¨®DD¡ì?¡ê
            polarity:¨º¡À?¨®¦Ì?¨®DD¡ì??D?
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê//////?1??1?aSPI¨°?¡ä¨°?a1y?¡ê
----------------------------------------------------------------------*/
int32 SPIMInit(eSPI_ch_t spiChNumb,uint16 baudRate,  SPI_DATA_WIDTH dataWidth,eSPI_cs_t slaveNumb,
                    eSPI_TRANSFER_MODE_t transferMode, eSPI_PHASE_t serialClockPhase,
                    eSPI_POLARITY_t  polarity, uint8 spiImr)
{
    int8 ret=0;
    uint32 APBnKHz;
    pSPI_REG spiMasterReg;

    if(spiChNumb == SPI_CH0)
    {
        spiMasterReg = (pSPI_REG) SPI0_BASE;
    }
    else if(spiChNumb == SPI_CH1)
    {
         spiMasterReg = (pSPI_REG) SPI1_BASE;
    }
    else
    {
       ret = -1;
    }

    if (slaveNumb > 2)
    {
        ret = -1;
    }

    if (transferMode > 2)
    {
        ret = -1;
    }

    if (serialClockPhase > 1)
    {
        ret = -1;
    }
    if (polarity > 1)
    {
        ret = -1;
    }
    if (dataWidth > 2)
    {
        ret = -1;
    }
    //dataWidth = dataWidth + 4;
   // SCUEnableClk(CLK_GATE_SPI0);
    g_spimFreq = baudRate;
    spiMasterReg->SPI_ENR = 0;
    //APBnKHz = GetPerAPBCLK();
    ret = SPIMBaudRate(baudRate,APBnKHz);

    spiMasterReg->SPI_CTRLR0 = 0;
    spiMasterReg->SPI_CTRLR1 = 0;
    spiMasterReg->SPI_SER = 0;

    //spiMasterReg->SPI_CTRLR0 = (spiMasterReg->SPI_CTRLR0 & (~0x3c))  |( (dataWidth-1)<<2)  | (transferMode<<18) | (polarity<<7) | (serialClockPhase<<6) |0x2;
    spiMasterReg->SPI_CTRLR0 = (spiMasterReg->SPI_CTRLR0 & (~0x3))  | dataWidth | (transferMode<<18) | (polarity<<7) | (serialClockPhase<<6);
    spiMasterReg->SPI_CTRLR1 = 0x1f;
    spiMasterReg->SPI_SER = slaveNumb;
    spiMasterReg->SPI_IMR = spiImr;
    spiMasterReg->SPI_ENR = 1;
    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPIMDeinit(void)
Desc      : ¨¦¨¨??SPI MASTER ¦Ì?¡¤¡ä3?¨º??¡¥
Params    :
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
void SPIMDeinit()
{
    uint32 i;
    pSPI_REG spiMasterReg;

    spiMasterReg = (pSPI_REG) SPI0_BASE_ADDR;

    //spiMasterReg->SPI_CTRLR0 = 0;
    //spiMasterReg->SPI_CTRLR1 = 0;
    spiMasterReg->SPI_BAUDR = 0;
    //spiMasterReg->SPI_SER = 0;
    spiMasterReg->SPI_ENR = 0;
    SCUDisableClk(CLK_GATE_SPI0);
}

/*----------------------------------------------------------------------
Name      : SPIMRead(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
Desc      : ¨¦¨¨??SPI MASTER ¦Ì??¨¢¨ºy?Y
Params    : *pdata:?¨¢????
             dataWidth ¡êo?¨¢¦Ì?¡Á??¨ª¡ê?¡ä¨®4??¦Ì?16??
             length¡êo?¨¢¦Ì?3¡è?¨¨¡ê?¨°?¡Á??¨ª?a¦Ì£¤???¡ê
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
int32 SPIMRead(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
{
    uint8 *pdataWrite1;
    uint16 *pdataWrite2;
    int32 ret=0;
    pSPI_REG spiMasterReg;

    	spiMasterReg = (pSPI_REG) SPI0_BASE_ADDR;

    if ( 2 == dataWidth)
    {
        pdataWrite2 = (uint16 *) pdata;
        while (length !=0)
        {
            if (spiMasterReg->SPI_RXFLR!= 0)
            {
                //PRINTF("spiMasterReg->SPI_RXFLR=%d!\n",spiMasterReg->SPI_RXFLR);
                *pdataWrite2++ = spiMasterReg->SPI_RXDR[0];
                //PRINTF("spiMasterReg->SPI_RXFLR=%d!\n",spiMasterReg->SPI_RXFLR);
                length--;
            }
        }
    }
    else
    {
        pdataWrite1 = (uint8 *) pdata;
        while (length !=0)
        {
            if ((spiMasterReg->SPI_SR & RECEIVE_FIFO_NOT_EMPTY) == RECEIVE_FIFO_NOT_EMPTY)
            {
                *pdataWrite1++ = spiMasterReg->SPI_RXDR[0];
                length--;
            }
        }
    }

    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPIMWrite(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
Desc      : ¨¦¨¨??SPI MASTER ¦Ì?D¡ä¨ºy?Y
Params    : *pdata:?¨¢????
             dataWidth ¡êo?¨¢¦Ì?¡Á??¨ª¡ê?¡ä¨®4??¦Ì?16??
             length¡êo?¨¢¦Ì?3¡è?¨¨¡ê?¨°?¡Á??¨ª?a¦Ì£¤???¡ê
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
int32 SPIMWrite(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
{
    uint8 *pdataWrite1;
    uint16 *pdataWrite2;
    int32 ret=0;
    pSPI_REG spiMasterReg;

    spiMasterReg = (pSPI_REG) SPI0_BASE_ADDR;

    //spiMasterReg->SPI_ENR = 1;
    if ( 2 == dataWidth)
    {
    	 pdataWrite2 = (uint16 *) pdata;
	 while (length != 0 )
        {
            if ((spiMasterReg->SPI_SR & TRANSMIT_FIFO_FULL) == TRANSMIT_FIFO_NOT_FULL)
            {
                spiMasterReg->SPI_TXDR[0] = *pdataWrite2++;
		        length--;
            }
        }
    }
    else
    {
    #if 1
        pdataWrite1 = (uint8 *) pdata;
        while (length != 0 )
        {
            if ((spiMasterReg->SPI_SR & TRANSMIT_FIFO_FULL) == TRANSMIT_FIFO_NOT_FULL)
            {
                spiMasterReg->SPI_TXDR[0] = *pdataWrite1++;
                length--;
            }
        }
    #endif
    }

    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPISInit()
Desc      : ¨¦¨¨??SPI SLAVE ¦Ì?3?¨º??¡¥
Params    : transferMode¡êo ¡ä?¨º?¡¤?¨º?¡ê????¨®¨º?¡êo2¡ê???¡¤¡é?¨ª¡êo1 ¡¤¡é?¨ªo¨ª?¨®¨º?¡êo0¡ê?EEROM ?¨¢¡êo3
            serialClockPhase¡êo ??D?¡ê?¨º???¦Ì???¨®DD¡ì?¨°¦Ì¨ª¦Ì???¨®DD¡ì  0:¨¦?¨¦y???¨¢¨¨?¦Ì?¨ºy?Y¨®DD¡ì 1¡êo???¦Ì???¨¢¨¨?¦Ì?¨ºy?Y¨®DD¡ì?¡ê
            spiImr:¨¦¨¨???D??¦Ì?¡Á¡ä¨¬???¡ê?¦Ì¡À2?¨°a¨º¡À¨¦¨¨???a 0
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê////?1??1?aSPI¨°?¡ä¨°?a1y?¡ê
----------------------------------------------------------------------*/
int32 SPISInit(SPI_DATA_WIDTH dataWidth,eSPI_TRANSFER_MODE_t transferMode, eSPI_PHASE_t serialClockPhase, eSPI_POLARITY_t  polarity,uint8 spiImr)

{
    int32 ret=0;
    pSPI_REG spiSlaveReg;

    spiSlaveReg = (pSPI_REG) SPI0_BASE_ADDR;

    if (transferMode > 3)
    {
        ret = -1;
    }

    if (serialClockPhase > 1)
    {
        ret = -1;
    }
    if (dataWidth > 2)
    {
        ret = -1;
    }
    //dataWidth = dataWidth + 4;
    SCUEnableClk(CLK_GATE_SPI0);

    spiSlaveReg->SPI_ENR = 0;
    spiSlaveReg->SPI_CTRLR0 = 0;

    //spiSlaveReg->SPI_CTRLR0 = (spiSlaveReg->SPI_CTRLR0 & (~0x3c)) | ((dataWidth-1)<<2) | SPI_SLAVE_MODE | (transferMode<<18) | (polarity<<7) | (serialClockPhase<<6) |0x2;
    spiSlaveReg->SPI_CTRLR0 = (spiSlaveReg->SPI_CTRLR0 & (~0x3))  | dataWidth | (transferMode<<18) | (polarity<<7) | (serialClockPhase<<6)|SPI_SLAVE_MODE;
    spiSlaveReg->SPI_IMR = spiImr;
    spiSlaveReg->SPI_ENR = 1;

    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPISDeinit()
Desc      : ¨¦¨¨??SPI SLAVE ¦Ì?¡¤¡ä3?¨º??¡¥
Params    :
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
void SPISDeinit()
{
    uint32 i;
    pSPI_REG spiSlaveReg;

    spiSlaveReg = (pSPI_REG) SPI0_BASE_ADDR;

    //spiSlaveReg->SPI_CTRLR0 = 0;
    spiSlaveReg->SPI_ENR = 0;

    SCUDisableClk(CLK_GATE_SPI0);
}

/*----------------------------------------------------------------------
Name      : SPISRead(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
Desc      : ¨¦¨¨??SPI SLAVE ¦Ì??¨¢¨ºy?Y
Params    : *pdata:?¨¢????
             dataWidth ¡êo?¨¢¦Ì?¡Á??¨ª¡ê?¡ä¨®4??¦Ì?16??
             length¡êo?¨¢¦Ì?3¡è?¨¨¡ê?¨°?¡Á??¨ª?a¦Ì£¤???¡ê
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
int32 SPISRead(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
{
    uint8 *pdataWrite1;
    uint16 *pdataWrite2;
    int32 ret=0;
    pSPI_REG spiSlaveReg;

    spiSlaveReg = (pSPI_REG) SPI0_BASE_ADDR;

    if ( 2 == dataWidth)
    {
        pdataWrite2 = (uint16 *) pdata;
        while (length !=0)
        {
            if (spiSlaveReg->SPI_RXFLR!= 0)
            {
            	  *pdataWrite2++ = spiSlaveReg->SPI_RXDR[0];
                  //PRINTF("length = %d!\n",length);
                  length--;
            }
        }
    }
    else
    {
        pdataWrite1 = (uint8 *) pdata;
        while (length !=0)
        {
            if ((spiSlaveReg->SPI_SR & RECEIVE_FIFO_EMPTY) == RECEIVE_FIFO_NOT_EMPTY)
            {
                *pdataWrite1++ = spiSlaveReg->SPI_RXDR[0];
                length--;
            }
        }
    }

    return (ret);
}

/*----------------------------------------------------------------------
Name      : SPISWrite(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
Desc      : ¨¦¨¨??SPI SLAVE ¦Ì?D¡ä¨ºy?Y
Params    : *pdata:?¨¢????
             dataWidth ¡êo?¨¢¦Ì?¡Á??¨ª¡ê?¡ä¨®4??¦Ì?16??
             length¡êo?¨¢¦Ì?3¡è?¨¨¡ê?¨°?¡Á??¨ª?a¦Ì£¤???¡ê
Return    :3¨¦1|?1??0¡ê?¨º¡ì¡ã¨¹?1??-1?¡ê
----------------------------------------------------------------------*/
int32 SPISWrite(void *pdata, SPI_DATA_WIDTH dataWidth, uint32 length)
{
    uint8 *pdataWrite1;
    uint16 *pdataWrite2;
    int32 ret=0;
    pSPI_REG spiSlaveReg;

    spiSlaveReg = (pSPI_REG) SPI0_BASE_ADDR;

    if ( 2 == dataWidth)
    {
        pdataWrite2 = (uint16 *) pdata;
        while (length != 0 )
        {
            if ((spiSlaveReg->SPI_SR & TRANSMIT_FIFO_NOT_FULL) == TRANSMIT_FIFO_NOT_FULL)
            {
                spiSlaveReg->SPI_TXDR[0] = *pdataWrite2++;
                length--;
            }
        }
    }
    else
    {
        pdataWrite1 = (uint8 *) pdata;
        while (length != 0 )
        {
            if ((spiSlaveReg->SPI_SR & TRANSMIT_FIFO_NOT_FULL) == TRANSMIT_FIFO_NOT_FULL)
            {
                spiSlaveReg->SPI_TXDR[0] = *pdataWrite1++;
                length--;
            }
        }
    }

    return (ret);
}
#endif
/*
********************************************************************************
*
*                         End of spi.c
*
********************************************************************************
*/

