/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   i2s.c
*
* Description:  C program template
*
* History:      <author>          <time>        <version>
*             yangwenjie      2009-1-14         1.0
*    desc:    ORG.
********************************************************************************
*/
#include "Bspconfig.h"
#ifdef __DRIVER_I2S_I2SDEVICE_C__
#define _IN_I2S_
#include "typedef.h"
#include <DriverInclude.h>


/*
--------------------------------------------------------------------------------
  Function name : void I2sStart()
  Author        : yangwenjie
  Description   : start i2s transfer

  Input         : direction，
                            TX:  send to codec
                            RX:  input to inside
  Return        : NULL

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 I2SDMADisable(I2S_CHannel I2S_CH,I2S_Start_t DMA_TxRx)
{
    if(DMA_TxRx == I2S_START_DMA_TX)
    {
        //disable DMA
        I2s_Reg->I2S_DMACR &= (~DMA_Transmit_enabled);
    }
    else if (DMA_TxRx == I2S_START_DMA_RX)
    {
        //disable DMA
        I2s_Reg->I2S_DMACR &= (~DMA_Receive_enabled);                         ;
    }
    else if(DMA_TxRx == I2S_START_DMA_RTX)
    {
        //disable DMA
        I2s_Reg->I2S_DMACR &= ~(DMA_Transmit_enabled | DMA_Receive_enabled);
    }
    return 0;

}


/*
--------------------------------------------------------------------------------
  Function name : void I2sStart()
  Author        : yangwenjie
  Description   : start i2s transfer

  Input         : direction，
                            TX:  send to codec
                            RX:  input to inside
  Return        : NULL

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 I2SDMAEnable(I2S_CHannel I2S_CH,I2S_Start_t DMA_TxRx)
{
    if(DMA_TxRx == I2S_START_DMA_TX)
    {
        //enable DMA
        I2s_Reg->I2S_DMACR |= (0x10 << 0)               //transmit data level
                         |(DMA_Transmit_enabled)    //transmit data enable
                         ;
    }
    else if (DMA_TxRx == I2S_START_DMA_RX)
    {
        //enable DMA
        I2s_Reg->I2S_DMACR |= (0x10<<16)                //receive data level
                         |(DMA_Receive_enabled)     //receive data enable
                         ;
    }
    else if(DMA_TxRx == I2S_START_DMA_RTX)
    {
        //enable DMA
        I2s_Reg->I2S_DMACR |= (0x10<<16)                //receive data level
                         |(DMA_Receive_enabled)     //receive data enable
                         |(0x10 << 0)               //transmit data level
                         |(DMA_Transmit_enabled)    //transmit data enable
                         ;
    }
    return 0;
}



uint32 I2SGetIntType(I2S_CHannel I2S_CH)
{
    uint32 dwI2sIntStaus;
    uint32 event;

    dwI2sIntStaus = I2s_Reg->I2S_INTSR;

    event = 0;

    if(dwI2sIntStaus & INTSR_TX_empty_interrupt_active)
    {
        event |= TX_interrupt_active;
    }

    if(dwI2sIntStaus & INTCR_RX_full_interrupt_enable)
    {
        event |= RX_interrupt_active;
    }


    return event;
}

uint32 I2sGetRxFIFOaddr(I2S_CHannel I2S_CH)
{
    return (uint32)(&(I2s_Reg->I2S_RXDR));
}

uint32 I2sGetTxFIFOaddr(I2S_CHannel I2S_CH)
{
    return (uint32)(&(I2s_Reg->I2S_TXDR));
}


void I2SIntEnable(I2S_CHannel I2S_CH,I2S_INT_t INT_TxRx)
{

    if(INT_TxRx == I2S_INT_TX)
    {
        I2s_Reg->I2S_INTCR |= INTCR_TX_empty_interrupt_enable;
    }
    else if (INT_TxRx == I2S_INT_RX)
    {
        I2s_Reg->I2S_INTCR |= INTCR_RX_full_interrupt_enable;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void I2sStart()
  Author        : yangwenjie
  Description   : start i2s transfer

  Input         : direction，
                            TX:  send to codec
                            RX:  input to inside
  Return        : NULL

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void I2SIntDisable(I2S_CHannel I2S_CH,I2S_INT_t INT_TxRx)
{

    if(INT_TxRx == I2S_INT_TX)
    {
        I2s_Reg->I2S_INTCR &= ~INTCR_TX_empty_interrupt_enable;
    }
    else if (INT_TxRx == I2S_INT_RX)
    {
        I2s_Reg->I2S_INTCR &= ~INTCR_RX_full_interrupt_enable;
    }
}


/*
--------------------------------------------------------------------------------
  Function name : void DataPortMuxSet(eDataPortIOMux_t data)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void I2sCs(I2S_CHannel I2S_CH,eI2sCs_t data)
{
    Grf->IOMUX_CON1 = (0x00040000 | (data << 2));
}

//#ifdef DRIVERLIB_IRAM
/*
--------------------------------------------------------------------------------
  Function name : void I2sStart()
  Author        : yangwenjie
  Description   : start i2s transfer

  Input         : direction，
                            TX:  send to codec
                            RX:  input to inside
  Return        : NULL

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 I2SStart(I2S_CHannel I2S_CH, I2S_Start_t txRx)
{

    if((txRx == I2S_START_PIO_TX) || (txRx == I2S_START_DMA_TX))
    {
        if(txRx == I2S_START_DMA_TX)
        {
            //enable DMA
            I2s_Reg->I2S_DMACR |= (0x10 << 0)               //transmit data level
                             |(DMA_Transmit_enabled);    //transmit data enable

        }
        I2s_Reg->I2S_XFER |= TX_transfer_start;
    }
    else if ((txRx == I2S_START_PIO_RX) || (txRx == I2S_START_DMA_RX))
    {
        if(txRx == I2S_START_DMA_RX)
        {
            //enable DMA
            I2s_Reg->I2S_DMACR |= (0x10<<16)                //receive data level
                             |(DMA_Receive_enabled)     //receive data enable
                             ;
        }
        else
        {
            //enable INT
            I2s_Reg->I2S_INTCR |= (INTCR_RX_full_interrupt_enable | INTCR_RX_overrun_interrupt_enable);
            IntEnable(INT_ID_I2S0);
        }

        I2s_Reg->I2S_XFER |= RX_transfer_start;
    }
    else
    {
        if(txRx == I2S_START_DMA_RTX)
        {
            //enable DMA
            I2s_Reg->I2S_DMACR |= (0x10<<16)                //receive data level
                             |(DMA_Receive_enabled)     //receive data enable
                             |(0x10 << 0)               //transmit data level
                             |(DMA_Transmit_enabled)    //transmit data enable
                             ;
        }
        else
        {
            //enable INT
            I2s_Reg->I2S_INTCR |= (   INTCR_RX_full_interrupt_enable
                                    | INTCR_RX_overrun_interrupt_enable
                                    | INTCR_TX_empty_interrupt_enable
                                    | INTCR_TX_underrun_interrupt_enable);
        }

        I2s_Reg->I2S_XFER |= (RX_transfer_start | TX_transfer_start);
    }
    return 0;
}

/*
--------------------------------------------------------------------------------
  Function name : I2sStop(void)
  Author        : yangwenjie
  Description   : stop transfer

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void I2SStop(I2S_CHannel I2S_CH, I2S_Start_t txRx)
{
    if((txRx == I2S_START_PIO_TX) || (txRx == I2S_START_DMA_TX))
    {
        if(txRx == I2S_START_DMA_TX)
            I2s_Reg->I2S_DMACR &= DMA_Transmit_disabled;

        I2s_Reg->I2S_XFER  &= TX_transfer_stop;
    }
    else if((txRx == I2S_START_PIO_RX) || (txRx == I2S_START_DMA_RX))
    {
        if(txRx == I2S_START_DMA_RX)
            I2s_Reg->I2S_DMACR &= DMA_Receive_disabled;

        I2s_Reg->I2S_XFER  &= RX_transfer_stop;
    }
    else
    {
        if(txRx == I2S_START_DMA_RTX)
            I2s_Reg->I2S_DMACR &= (DMA_Transmit_disabled | DMA_Receive_disabled);

        I2s_Reg->I2S_XFER  &= (TX_transfer_stop | RX_transfer_stop);
    }
}
/*----------------------------------------------------------------------
Name      : I2SInit(I2S_mode_t mode, I2S_Start_t TxOrRx)
Desc      : initializatin
Params    : mode：I2S_MASTER_MODE or I2S_SLAVE_MODE master or slave
Return    : 0 ok，-1 fail。
            fs(kHz)\Fmclk(MHz)	    12	    24.576	12.288	8.192	11.2896
                            8	    1500	3072	1536	1024	--
                            11.025	1088*	--	    --	    --	    1024
                            12	    1000	2048	1024	--	    --
                            16	    750	    1536	768	    512	    --
                            22.05	544*	--	    --	    --	    512
                            24	    500	    1024	512	    --	    --
                            32	    375	    768	    384	    256	    --
                            44.1	272*	--	    --	    --	    256
                            48	    250	    512	    256	    --	    --
                            96	    125	    256	    128	    --	    --
                            192	    62.5	128	    64	    --	    --
                            表格 7  Fmclk与Flrck比值表
            Fs=Fmclk/(RMD*RSD);
            Fs=Fmclk/(TMD*TSD);
            Fmclk is the frequency of mclk
            TMD(RMD) is divider rato of mclk to sclk,TMD(RMD) = Fmclk/Fsclk,TMD(RMD) is even from 2 to 64.
            TSD(RSD) is divider rato of sclk to lrck,TSD(RSD) = Fsclk/Flrck,TSD(RSD) is any number from 32 to 287.
            for keep 32 bit transfer,so the max value of TSD/4 and RSD is needed to bigger than 64(2X32bit).
            the range of TMD(RMD) is get from this,so the smallest drivider is 48.

----------------------------------------------------------------------*/
int32 I2SInit(I2S_CHannel I2S_CH,
              I2S_mode_t I2S_mode,
              eI2sCs_t I2S_CS,
              I2sFS_en_t I2S_FS,
              eI2sFormat_t BUS_FORMAT,
              eI2sDATA_WIDTH_t I2S_Data_width,
              I2S_BUS_MODE_t I2S_Bus_mode)
{
    uint32 config;
    uint32 timeout = 20000;
    // clock gate&reset, iomux
    // modify I2S XFER, disable i2s
    I2s_Reg->I2S_XFER = (TX_transfer_stop |RX_transfer_stop);

    // modify I2S_CLR to reset i2s
    // wait until I2S_CLR become 0
    I2s_Reg->I2S_CLR= CLR_TXC_cleard;
    while(I2s_Reg->I2S_CLR)
    {
	    if (--timeout == 0)
	    {
            break;
	    }
    }
    if((I2S_CS == I2S_EXT) || (I2S_CS == I2S_IN))
        I2sCs(I2S_CH,I2S_CS);

    if(I2S_mode == I2S_SLAVE_MODE)
    {
        I2s_Reg->I2S_CKR  = ((CKR_TSD_Value)                   //Fsclk/Ftxlrck TSD=187
                             |(CKR_RSD_Value)                   //Fsclk/Frxlrck RSD=124
                             |(CKR_MDIV_Value)                  //MDIV=6M  Fmclk/Ftxsck-1,panda_FPGA:MCLK=12M
                             |(CKR_TLP_normal)                  //rx/tx lrck polarity
                             |(CKR_RLP_normal)
                             |(CKR_CKP_posedge)                 //rx/tx sclk polarity
                             |(CKR_MSS_slave)                   //slave mode
                             );
    }
    else
    {
        I2s_Reg->I2S_CKR  = ((CKR_TSD_Value)                   //Fsclk/Ftxlrck TSD=187
                             |(CKR_RSD_Value)                   //Fsclk/Frxlrck RSD=124
                             |(CKR_MDIV_Value)                  //MDIV=6M  Fmclk/Ftxsck-1,panda_FPGA:MCLK=12M
                             |(CKR_TLP_normal)                  //rx/tx lrck polarity
                             |(CKR_RLP_normal)
                             |(CKR_CKP_posedge)                 //rx/tx sclk polarity
                             );
    }
    if(I2S_Data_width == I2S_DATA_WIDTH24)
    {
        I2s_Reg->I2S_TXCR = ((Valid_Data_width24)            	//Valid data width
                             |(TXCR_TFS_I2S)            		//Tranfer format select, 0:I2S, 1:PCM
                             |(TXCR_PCM_no_delay)            	//PCM bus mode
                             |(TXCR_I2S_normal)            		//I2S bus mode
                             |(TXCR_FBM_MSB)           		    //First Bit Mode
                             |(TXCR_SJM_right_justified)        //store justified mode,0:right,1:left
                             |(TXCR_HWT_Bit32)           		//halfword word transform
                             |(TXCR_Channel_0_Enable) 		    //channel select register
                             |(0<<17)						    //RCNT
                             );

        I2s_Reg->I2S_RXCR = ((Valid_Data_width24)            	//Valid data width
                             |(RXCR_TFS_I2S)            		//Tranfer format select, 0:I2S, 1:PCM
                             |(RXCR_PCM_no_delay)            	//PCM bus mode
                             |(RXCR_I2S_normal)            		//I2S bus mode
                             |(RXCR_FBM_MSB)           		    //First Bit Mode
                             |(RXCR_SJM_right_justified)        //store justified mode,0:right,1:left
                             |(RXCR_HWT_Bit16) 			        //halfword word transform
                             );
    }
    else
    {
    I2s_Reg->I2S_TXCR = ((Valid_Data_width16)            	//Valid data width
                         |(TXCR_TFS_I2S)            		//Tranfer format select, 0:I2S, 1:PCM
                         |(TXCR_PCM_no_delay)            	//PCM bus mode
                         |(TXCR_I2S_normal)            		//I2S bus mode
                         |(TXCR_FBM_MSB)           		    //First Bit Mode
                         |(TXCR_SJM_right_justified)        //store justified mode,0:right,1:left
                         |(TXCR_HWT_Bit16)           		//halfword word transform
                         |(TXCR_Channel_0_Enable) 		    //channel select register
                         |(0<<17)						    //RCNT
                         );

    I2s_Reg->I2S_RXCR = ((Valid_Data_width16)            	//Valid data width
                         |(RXCR_TFS_I2S)            		//Tranfer format select, 0:I2S, 1:PCM
                         |(RXCR_PCM_no_delay)            	//PCM bus mode
                         |(RXCR_I2S_normal)            		//I2S bus mode
                         |(RXCR_FBM_MSB)           		    //First Bit Mode
                         |(RXCR_SJM_right_justified)        //store justified mode,0:right,1:left
                         |(RXCR_HWT_Bit16) 			        //halfword word transform
                         );
    }

   I2s_Reg->I2S_INTCR = ((INTCR_TX_empty_interrupt_disabled)        // tx empty interrupt. 0:disable,1:enable
                         |(INTCR_TX_underrun_interrupt_disabled)    // tx under run int enable.0:disable,1:enable
                         |(0<<2)                                    // write 1 to clear TX underrun int
                         |(0x10<<4)                                    // transmit FIFO threshold
                         |(INTCR_RX_full_interrupt_disabled)        // RX Full int
                         |(INTCR_RX_overrun_interrupt_disabled)     // RX overrun
                         |(0<<18)                                   // write 1 to clear RX overrun
                         |(0x1f<<20)                                // Reveive FIFO threshold
                         );

    return 0;
}

uint32 I2S_PIO_Read(I2S_CHannel I2S_CH,uint32* buffer,uint32 size)
{
     memcpy(buffer,&(I2s_Reg->I2S_RXDR), size);
}

uint32 I2S_PIO_Write(I2S_CHannel I2S_CH,uint32* buffer, uint32 size)
{
    uint32 dwRealSize, cnt;

    cnt = size > 16? 16 : size;
    //cnt = size;

    dwRealSize = 0;
    while (cnt--)
    {
        /*
        if(I2s_Reg->I2S_FIFOLR & 0x3f >= 0x1e)
        {
            break;
        }
        */

        *(uint32 *)(&(I2s_Reg->I2S_TXDR))  = *buffer;
         buffer++;
         dwRealSize++;
    }

    return dwRealSize;

}
/*
--------------------------------------------------------------------------------
  Function name : I2S_PowerOnInit(I2S_mode_t mode)
  Author        : yangwenjie
  Description   : I2S auti-initial

  Input         : NULL

  Return        : NULL

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  note:        if do not use i2s device,for example,using internal codec,
  			   must call this function after power on.
--------------------------------------------------------------------------------
*/
void I2SDeInit(I2S_CHannel I2S_CH)
{

    I2s_Reg->I2S_XFER = 0;

//    #if(BOARDTYPE == CK610_FPGA)

 //   #else
//        SCUDisableClk(CLK_GATE_I2S0);
 //   #endif
}
//#endif

#endif
/*
********************************************************************************
*
*                         End of i2s.c
*
********************************************************************************
*/
