/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, aaron.sun
*                             All rights reserved.
*
* FileName: Cpu\NanoC\lib\usb.c
* Owner: aaron.sun
* Date: 2014.9.24
* Time: 16:16:06
* Desc: usb bsp
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2014.9.24     16:16:06   1.0
********************************************************************************************
*/
#define __CPU_NANOC_LIB_USB_C__
#include "BspConfig.h"
#ifdef __CPU_NANOC_LIB_USB_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "typedef.h"
#include "Bsp.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _CPU_NANOC_LIB_USB_READ_  __attribute__((section("cpu_nanoc_lib_usb_read")))
#define _CPU_NANOC_LIB_USB_WRITE_ __attribute__((section("cpu_nanoc_lib_usb_write")))
#define _CPU_NANOC_LIB_USB_INIT_  __attribute__((section("cpu_nanoc_lib_usb_init")))
#define _CPU_NANOC_LIB_USB_SHELL_  __attribute__((section("cpu_nanoc_lib_usb_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
void UsbControllerInit(bool FullSpeed);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: USBEnumDone
** Input:uint32 * speed
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.10.10
** Time: 18:15:15
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API rk_err_t USBEnumDone(uint32 * speed)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    /* Set the MPS of the IN EP based on the enumeration speed */
    *speed = USB_SPEED_FULL;

    switch ((dev_regs->dsts >> 1) & 0x03)
    {
        case 0: //HS_PHY_30MHZ_OR_60MHZ:
            *speed = USB_SPEED_HIGH;
            dev_regs->in_ep[0].diepctl &= ~0x03;  //64bytes MPS
            break;

        case 1: //FS_PHY_30MHZ_OR_60MHZ:
        case 3: //FS_PHY_48MHZ
            dev_regs->in_ep[0].diepctl &= ~0x03;  //64bytes MPS
            break;

        case 2: //LS_PHY_6MHZ
        default:
            dev_regs->in_ep[0].diepctl |= 0x03;   //8bytes MPS
            break;
    }

    //pDev->request(0, UDC_EV_ENUM_DONE, speed, pDev);
    /* setup EP0 to receive SETUP packets */
    dev_regs->out_ep[0].doeptsiz = (1ul << 29) | (1ul << 19) | 0x40; //supcnt = 1, pktcnt = 1, xfersize = 64*1
    /* Enable OUT EP for receive */
    dev_regs->out_ep[0].doepctl |= (1ul<<31);

    dev_regs->dctl |= 1<<8;         //clear global IN NAK
}

/*******************************************************************************
** Name: USBReconnect
** Input:uint32 param
** Return: void
** Owner:aaron.sun
** Date: 2014.9.29
** Time: 15:35:15
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBReconnect(uint32 restart, uint32 fullspeed)
{

    if (1 == restart)
    {
        USBConnect(fullspeed);
    }
    else
    {
        UsbIntEnable();
    }
    return;

}


/*******************************************************************************
** Name: USBDisconnect
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.9.29
** Time: 15:34:10
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBDisconnect(void)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->dctl |= 0x02;         //soft disconnect
}


/*******************************************************************************
** Name: USBConnect
** Input:bool FullSpeed
** Return: void
** Owner:aaron.sun
** Date: 2014.9.29
** Time: 15:33:44
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBConnect(bool FullSpeed)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    UsbIntDisalbe();
    dev_regs->dctl |= 0x02;         //soft disconnec
    UsbControllerInit(FullSpeed);
    dev_regs->dctl &= ~0x02;        //soft connect
    UsbIntEnable();
}


/*******************************************************************************
** Name: USBReset
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.9.29
** Time: 15:21:37
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBReset(void)
{
    uint32 count;
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    USB_OTG_REG *  otg_core = (USB_OTG_REG * )USB_REG_BASE;

    /* Clear the Remote Wakeup Signalling */
    otg_core->Device.dctl &= ~0x01;

    /* Set NAK for all OUT EPs */

    /* Flush the NP Tx FIFO */
    otg_core->Core.grstctl |= ( 0x10<<6) | (1<<5);     //Flush all Txfifo
    for (count=0; count<10000; count++)
    {
        if ((otg_core->Core.grstctl & (1<<5))==0)
            break;
    }

    /* Flush the Learning Queue. */
    otg_core->Core.grstctl |= 1<<3;

    /* Reset Device Address */
    otg_core->Device.dcfg &= ~0x07f0;

    /* setup EP0 to receive SETUP packets */

    /* Clear interrupt */
    otg_core->Core.gintsts = 0xFFFFFFFF;
}


/*******************************************************************************
** Name: USBGetRxFIFOIntType
** Input:void
** Return: uint32
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 14:06:05
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint32 USBGetRxFIFOIntType(uint32 * epnum, uint32 * datalen)
{
    USB_OTG_REG *  otg_core = (USB_OTG_REG * )USB_REG_BASE;
    DEVICE_GRXSTS_DATA status;
    uint32 inttype;

    /* Disable the Rx Status Queue Level interrupt */
    otg_core->Core.gintmsk &= ~(1<<4);

    /* Get the Status from the top of the FIFO */
    status.d32 = otg_core->Core.grxstsp;
    inttype = 0;
    switch (status.b.pktsts)
    {
        case 0x01: //Global OUT NAK
            inttype |= USB_RX_INT_TYPE_GLOBAL_OUT_NAK;
            break;

        case 0x03://OUT transfer completed
            *epnum = status.b.epnum;
            *datalen = (status.b.bcnt|(status.b.fn<<16));
            inttype |= USB_RX_INT_TYPE_OUT_RECIVE_PACKAGE;
            break;

        case 0x06: ////SETUP data packet received
            *epnum = status.b.epnum;
            *datalen = (status.b.bcnt|(status.b.fn<<16));
            inttype |= USB_RX_INT_TYPE_SETUP_RECIVE_PACKAGE;
            break;
        default:
            break;
    }


    /* Enable the Rx Status Queue Level interrupt */
    otg_core->Core.gintmsk |= (1<<4);
    /* Clear interrupt */
    otg_core->Core.gintsts = (1<<4);

    return inttype;

}

/*******************************************************************************
** Name: USBGetOutEpIntType
** Input:void
** Return: uint32
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 14:00:04
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint32 USBGetOutEpIntType(void)
{
    uint32 ep_intr;
    uint32 doepint;
    uint32 epnum = 0;
    uint32 inttype;
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    /* Read in the device interrupt bits */
    ep_intr = (dev_regs->daint & dev_regs->daintmsk) >> 16;

    inttype = 0;

    while(ep_intr)
    {
        if (ep_intr & 0x1)
        {

            doepint = dev_regs->out_ep[epnum].doepint & dev_regs->doepmsk;

            /* Transfer complete */
            if ( doepint & 0x01 )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                dev_regs->out_ep[epnum].doepint = 0x01;
                //inttype |= USB_OUT_EP_INT_TYPE_TRANS_OK;
            }

            /* Endpoint disable  */
            if ( doepint & 0x02 )
            {
                /* Clear the bit in DOEPINTn for this interrupt */
                dev_regs->out_ep[epnum].doepint = 0x02;
                //inttype |= USB_OUT_EP_INT_TYPE_EP_DISABLE;
            }

            /* AHB Error */
            if ( doepint & 0x04 )
            {
                dev_regs->out_ep[epnum].doepint = 0x04;
                //inttype |= USB_OUT_EP_INT_TYPE_AHB_ERR;
            }

            /* Setup Phase Done (contorl EPs) */
            if ( doepint & 0x08 )
            {
                dev_regs->out_ep[epnum].doepint = 0x08;
                //inttype |= USB_OUT_EP_INT_TYPE_SETUP_PHASE_OVER;
            }

            dev_regs->out_ep[epnum].doepint = 0xffffffff;
        }

        if(inttype)
        {
          inttype |= (epnum << 16);
          break;
        }

        epnum++;
        ep_intr >>= 1;

    }

}

/*******************************************************************************
** Name: USBGetInEpIntType
** Input:void
** Return: uint32
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 13:59:22
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint32 USBGetInEpIntType(void)
{
    uint32 ep_intr;
    uint32 diepint, msk;
    uint32 epnum = 0;
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    uint32 inttype;

    /* Read in the device interrupt bits */
    ep_intr = (dev_regs->daint & dev_regs->daintmsk) & 0xFFFF;

    inttype = 0;
    /* Service the Device IN interrupts for each endpoint */
    while (ep_intr)
    {
        if (ep_intr&0x1)
        {
            msk = dev_regs->diepmsk | ((dev_regs->dtknqr4_fifoemptymsk & 0x01)<<7);   //because of the low 7bit being reserved
            diepint = dev_regs->in_ep[epnum].diepint & msk;

            /* Transfer complete */
            if ( diepint & 0x01 )
            {
                /* Clear the bit in DIEPINTn for this interrupt */
                 //dev_regs->in_ep[epnum].diepint = 0x01;
                 inttype |= USB_IN_EP_INT_TYPE_TRANS_OK;
            }

            /* Endpoint disable  */
            if (diepint & 0x02)
            {
                /* Clear the bit in DIEPINTn for this interrupt */
                //dev_regs->in_ep[epnum].diepint = 0x02;
                inttype |= USB_IN_EP_INT_TYPE_EP_DISABLE;
            }

            /* AHB Error */
            if ( diepint & 0x04 )
            {
                /* Clear the bit in DIEPINTn for this interrupt */
                //dev_regs->in_ep[epnum].diepint = 0x04;
                inttype |= USB_IN_EP_INT_TYPE_AHB_ERR;
            }

            /* TimeOUT Handshake (non-ISOC IN EPs) */
            if ( diepint & 0x08 )
            {
                //dev_regs->in_ep[epnum].diepint = 0x08;
                inttype |= USB_IN_EP_INT_TYPE_TIME_OUT;
            }

            dev_regs->in_ep[epnum].diepint = 0xffffffff;
        }

        if(inttype)
        {
          inttype |= (epnum << 16);
          break;
        }

        epnum++;
        ep_intr >>=1;

    }
}

/*******************************************************************************
** Name: USBSetAddr
** Input:uint32 addr
** Return: void
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 10:56:23
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBSetAddr(uint32 addr)
{
     DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

     dev_regs->dcfg = (dev_regs->dcfg & (~0x07f0)) | (addr << 4);  //reset device addr
}

/*******************************************************************************
** Name: USBInEpSetData0
** Input:uint8 dir, uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:19:26
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void USBInEpSetData0(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->in_ep[epnum].diepctl  |= 1<<28;
}

/*******************************************************************************
** Name: USBOutEpSetData0
** Input:uint8 dir, uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:19:26
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void USBOutEpSetData0(uint8 dir, uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->out_ep[epnum].doepctl |= 1<<28;
}

/*******************************************************************************
** Name: USBInEpSetNak
** Input:uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:23:37
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void  USBInEpSetNak(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->in_ep[(epnum & 0x0f)].diepctl  |= (1ul<<27) ; // set Nak
}

/*******************************************************************************
** Name: USBOutEpSetNak
** Input:uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:23:37
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void  USBOutEpSetNak(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->out_ep[epnum].doepctl |= (1ul<<27) ; // set Nak
}

/*******************************************************************************
** Name: USBInEpClearNak
** Input:uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:22:34
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void USBInEpClearNak(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    dev_regs->in_ep[(epnum & 0x0f)].diepctl |= (1ul<<26) | (1ul<<31);  //Active ep, Clr Nak, endpoint enable
}

/*******************************************************************************
** Name: USBOutEpClearNak
** Input:uint8 epnum
** Return: void
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:22:34
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void USBOutEpClearNak(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    dev_regs->out_ep[epnum].doepctl |= (1ul<<26) | (1ul<<31); //Active ep, Clr Nak, endpoint enable
}

/*******************************************************************************
** Name: USBInEpStall
** Input:uint8 epnum
** Return: void'
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:21:01
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void  USBInEpStall(uint8 epnum)
{
    //write_XDATA(USB_CSR0, CSR0_OUT_SEVD | CSR0_SEND_STALL); 	//send stall
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->in_ep[(epnum & 0x0f)].diepctl |= 1<<21;   //send IN0 stall handshack

}

/*******************************************************************************
** Name: USBOutEpStall
** Input:uint8 epnum
** Return: void'
** Owner:zhuzhe
** Date: 2014.2.27
** Time: 10:21:01
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
void  USBOutEpStall(uint8 epnum)
{
    //write_XDATA(USB_CSR0, CSR0_OUT_SEVD | CSR0_SEND_STALL); 	//send stall
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->out_ep[epnum].doepctl |= 1<<21;  //send OUT0 stall handshack

}

/*******************************************************************************
** Name: USBCheckInEpStall
** Input:uint8 dir, uint8 epnum
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 10:36:15
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint8 USBCheckInEpStall(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    return(dev_regs->in_ep[epnum].diepctl>>21) & 0x1;
}

/*******************************************************************************
** Name: USBCheckInEpStall
** Input:uint8 dir, uint8 epnum
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 10:36:15
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint8 USBCheckOutEpStall(uint8 epnum)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
    return (dev_regs->out_ep[epnum].doepctl>>21) & 0x1;  //Out endpoint send stall handshack

}

/*******************************************************************************
** Name: USBWriteEp
** Input:uint32 epnum, uint32 len, uint8 * buf
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 10:07:30
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API rk_err_t USBWriteEp(uint32 epnum, uint32 len, uint8 * buf)
{
    uint32 i;
    USB_OTG_REG *  otg_core = (USB_OTG_REG * )USB_REG_BASE;
    uint32 *fifo = (uint32 *)USB_EP_FIFO(0);
    if (((otg_core->Core.gnptxsts & 0xFFFF) >= (len+3)/4) && (((otg_core->Core.gnptxsts>>16) & 0xFF) > 0))
    {
        otg_core->Device.in_ep[0].dieptsiz = len | (1<<19);
        otg_core->Device.in_ep[0].diepctl = (1ul<<26) | (1ul<<31);   //Endpoint Enable, Clear NAK

        uint32 *ptr = (uint32*)buf;
        for (i = 0; i < len; i += 4)
        {
            *fifo = *ptr++;
        }

        return (len);
    }
    else
    {
        return (0);
    }
}

/*******************************************************************************
** Name: USBReadEp
** Input:uint32 epnum, uint32 len, uint8 * buf
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 9:31:44
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API rk_err_t USBReadEp(uint32 epnum, uint32 len, uint8 * buf)
{
    uint32 i;
    uint32 *fifo = (uint32 *)USB_EP_FIFO(epnum);

    uint32 *ptr = (uint32*)buf;

    for(i=0; i<len; i+=4)
    {
        *ptr++ = *fifo;
    }

    if(0 == epnum)
    {
        //clear nak

        DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;
        dev_regs->out_ep[0].doeptsiz = 0x40 | (1<<19) | (1<<29);
    }

    return len;
}


/*******************************************************************************
** Name: USBEnableOutEp
** Input:uint32 epnum, uint32 bmAttributes, uint32 wMaxPacketSize
** Return: void
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 9:51:20
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBEnableOutEp(uint32 epnum, uint32 bmAttributes, uint32 wMaxPacketSize)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->daintmsk |= ((1<<epnum)<<16);
    dev_regs->out_ep[epnum].doeptsiz = (1ul << 19) | wMaxPacketSize;
    dev_regs->out_ep[epnum].doepint = 0xFF;
    dev_regs->out_ep[epnum].doepctl = (1ul<<31)|(1<<28)|(1<<26)|((bmAttributes & 0xf7)<<18)|(1<<15)|wMaxPacketSize;  //回复原先参数，与SynchronisationType值无关
}


/*******************************************************************************
** Name: USBEnalbeEp
** Input:uint32 epnum, uint32 bmAttributes, uint32 wMaxPacketSize
** Return: void
** Owner:aaron.sun
** Date: 2014.9.25
** Time: 9:24:28
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void USBEnalbeInEp(uint32 epnum, uint32 EnType, uint32 wMaxPacketSize)
{
    DEVICE_REG *dev_regs = (DEVICE_REG *)USB_DEV_BASE;

    dev_regs->daintmsk |= (1<<epnum);
    dev_regs->in_ep[epnum].diepctl = (1<<28)|(1<<27)|(EnType<<18)|(1<<15)|wMaxPacketSize;
    //if (3==epnum)
    //    dev_regs->in_ep[epnum].diepctl |= (1<<22);  //这个配置,还要进一步debug
    dev_regs->dctl |= 1<<8;
}

/*******************************************************************************
** Name: UsbGetIntType
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.9.24
** Time: 16:21:56
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API uint32 UsbGetIntType(void)
{
    uint32 intr_status;
    uint32 intr_type;
    USB_OTG_REG *  otg_core = (USB_OTG_REG * )USB_REG_BASE;

    intr_status = otg_core->Core.gintsts & otg_core->Core.gintmsk;
    intr_type = 0;

    if (!intr_status)
    {
        return intr_type;
    }

    if (intr_status & (1<<4))   //RxFIFO Non-Empty
    {
        intr_type |= USB_INT_TYPE_RXFIFO_N_EMPTY;
    }

    if (intr_status & (1<<5))   //Non-periodic TxFIFO Empty
    {
        intr_type |= USB_INT_TYPE_NP_TXFIFO_EMPTY;

        otg_core->Core.gintmsk &= ~(1<<5);
        otg_core->Device.dtknqr4_fifoemptymsk = 0;
    }

    if (intr_status & (1<<10))   //Early Suspend
    {
        intr_type |= USB_INT_TYPE_EARLY_SUSPEND;
        otg_core->Core.gintsts = 1<<10;
    }

    if (intr_status & (1<<11))   //USB Suspend
    {
        intr_type |= USB_INT_TYPE_SUSPEND;
        otg_core->Core.gintsts = 1<<11;
    }

    if (intr_status & (1<<12)) //USB Reset
    {
        intr_type |= USB_INT_TYPE_RESET;
        otg_core->Core.gintsts = 1<<12;
    }

    if (intr_status & (1<<13)) //Enumeration Done
    {
        intr_type |= USB_INT_TYPE_E_DONE;
        otg_core->Core.gintsts = 1<<13;
    }

    if (intr_status & (1<<18))      //IN中断
    {
		intr_type |= USB_INT_TYPE_EP_IN;
    }

    if (intr_status & (1<<19))      //OUT中断
    {
       intr_type |= USB_INT_TYPE_EP_OUT;
    }

    if (intr_status & (1<<30))      //USB VBUS中断  this interrupt is asserted when the utmiotg_bvalid signal goes high.
    {
        intr_type |= USB_INT_TYPE_VBUS;
        otg_core->Core.gintsts = 1<<30;
    }

    if (intr_status & (1ul<<31))   //resume
    {
        intr_type |= USB_INT_TYPE_RESUME;
        otg_core->Core.gintsts = 1ul<<31;
    }

    if (intr_status & ((1<<22)|(1<<6)|(1<<7)|(1<<17)))    //ingore the other interrupt
    {
        otg_core->Core.gintsts = intr_status & ((1<<22)|(1<<6)|(1<<7)|(1<<17));
    }

    return intr_type;

}

/*******************************************************************************
** Name:  UsbIntDisalbe
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.3.6
** Time: 15:39:04
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void  UsbIntDisalbe(void)
{
     *((volatile unsigned long*)(0xE000E180)) = 0x00000080;
}
/*******************************************************************************
** Name: UsbIntEnable
** Input:void
** Return: uint32
** Owner:zhuzhe
** Date: 2014.3.6
** Time: 15:37:45
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ API void UsbIntEnable(void)
{
    *((volatile unsigned long*)(0xE000E100)) = 0x00000080;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: UsbControllerInit
** Input:bool FullSpeed
** Return: void
** Owner:aaron.sun
** Date: 2014.9.29
** Time: 15:24:17
*******************************************************************************/
_CPU_NANOC_LIB_USB_READ_
READ FUN void UsbControllerInit(bool FullSpeed)
{
    USB_OTG_REG *  otg_core = (USB_OTG_REG * )USB_REG_BASE;
    int count = 0;

    /* Wait for AHB master IDLE state. */
    for (count=0; count<10000; count++)
    {
        if ((otg_core->Core.grstctl & (1ul<<31))!=0)
            break;
    }

    //core soft reset
    otg_core->Core.grstctl |= 1<<0;               //Core soft reset
    for (count=0; count<10000; count++)
    {
        if ((otg_core->Core.grstctl & (1<<0))==0)
            break;
    }

    otg_core->ClkGate.pcgcctl = 0x0F;             /* Restart the Phy Clock */

    DelayMs(10);
    otg_core->ClkGate.pcgcctl = 0x00;             /* Restart the Phy Clock */


    //Non-Periodic TxFIFO Empty Level interrupt indicates that the IN Endpoint TxFIFO is completely empty
    otg_core->Core.gahbcfg |= 1<<7;

    /* Do device intialization*/

    //usb_write_reg32(USB_PCGCCTL, 0);

    /* High speed PHY running at high speed */
    if (!FullSpeed)
    {
        otg_core->Device.dcfg &= ~0x03;
    }
    else
    {
        //#ifdef FORCE_FS
        otg_core->Device.dcfg |= 0x01;   //Force FS
        //#endif
    }

    /* Reset Device Address */
    otg_core->Device.dcfg &= ~0x07F0;

    /* Flush the FIFOs */
    otg_core->Core.grstctl |= ( 0x10<<6) | (1<<5);     //Flush all Txfifo
    for (count=0; count<10000; count++)
    {
        if ((otg_core->Core.grstctl & (1<<5))==0)
            break;
    }

    otg_core->Core.grstctl |= 1<<4;              //Flush all Rxfifo
    for (count=0; count<10000; count++)
    {
        if ((otg_core->Core.grstctl & (1<<4))==0)
            break;
    }

    /* Flush the Learning Queue. */
    otg_core->Core.grstctl |= 1<<3;

    /* Clear all pending Device Interrupts */
    otg_core->Device.in_ep[0].diepctl = (1<<27)|(1<<30);        //IN0 SetNAK & endpoint disable
    otg_core->Device.in_ep[0].dieptsiz = 0;
    otg_core->Device.in_ep[0].diepint = 0xFF;

    otg_core->Device.out_ep[0].doepctl = (1<<27)|(1<<30);        //OUT0 SetNAK & endpoint disable
    otg_core->Device.out_ep[0].doeptsiz = 0;
    otg_core->Device.out_ep[0].doepint = 0xFF;

    otg_core->Device.diepmsk = 0x2F;              //device IN interrutp mask
    otg_core->Device.doepmsk = 0x0F;              //device OUT interrutp mask
    otg_core->Device.daint = 0xFFFFFFFF;          //clear all pending intrrupt
    otg_core->Device.daintmsk = 0x00010001;         //device all ep interrtup mask(IN0 & OUT0)
    otg_core->Core.gintsts = 0xFFFFFFFF;
    otg_core->Core.gotgint = 0xFFFFFFFF;

    otg_core->Core.gintmsk=(1<<4)|/*(1<<5)|*/(1<<10)|(1<<11)|(1<<12)|(1<<13)|(1<<18)|(1<<19)|(1ul<<30)|(1ul<<31);
    otg_core->Core.gahbcfg |= 0x01;        //Global interrupt mask
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

