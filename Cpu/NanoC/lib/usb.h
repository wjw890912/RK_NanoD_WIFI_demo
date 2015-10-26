/*
********************************************************************************************
*
*	 Copyright (c): 2014 - 2014 + 5, aaron.sun
*							   All rights reserved.
*
* FileName: Cpu\NanoC\lib\usb.h
* Owner: aaron.sun
* Date: 2014.9.24
* Time: 16:16:25
* Desc: usb bsp
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    aaron.sun     2014.9.24     16:16:25   1.0
********************************************************************************************
*/

#ifndef __CPU_NANOC_LIB_USB_H__
#define __CPU_NANOC_LIB_USB_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define USB_INT_TYPE_EARLY_SUSPEND      (1 << 0)
#define USB_INT_TYPE_SUSPEND            (1 << 1)
#define USB_INT_TYPE_RESET              (1 << 2)
#define USB_INT_TYPE_E_DONE             (1 << 3)
#define USB_INT_TYPE_VBUS               (1 << 4)
#define USB_INT_TYPE_RESUME             (1 << 5)
#define USB_INT_TYPE_EP_IN              (1 << 6)
#define USB_INT_TYPE_EP_OUT             (1 << 7)
#define USB_INT_TYPE_RXFIFO_N_EMPTY     (1 << 8)
#define USB_INT_TYPE_NP_TXFIFO_EMPTY    (1 << 9)


#define USB_RX_INT_TYPE_GLOBAL_OUT_NAK         (1 << 0)
#define USB_RX_INT_TYPE_OUT_RECIVE_PACKAGE     (1 << 1)
#define USB_RX_INT_TYPE_SETUP_RECIVE_PACKAGE   (1 << 2)


#define USB_OUT_EP_INT_TYPE_TRANS_OK            (1 << 0)
#define USB_OUT_EP_INT_TYPE_EP_DISABLE          (1 << 1)
#define USB_OUT_EP_INT_TYPE_AHB_ERR             (1 << 2)
#define USB_OUT_EP_INT_TYPE_SETUP_PHASE_OVER    (1 << 3)

#define USB_IN_EP_INT_TYPE_TRANS_OK            (1 << 0)
#define USB_IN_EP_INT_TYPE_TIME_OUT            (1 << 1)
#define USB_IN_EP_INT_TYPE_EP_DISABLE          (1 << 2)
#define USB_IN_EP_INT_TYPE_AHB_ERR             (1 << 3)
#define USB_IN_EP_INT_TYPE_SETUP_PHASE_OVER    (1 << 4)



enum USB_DEVICE_SPEED
{
    USB_SPEED_UNKNOWN = 0,	    /* enumerating */
    USB_SPEED_LOW,
    USB_SPEED_FULL,		        /* usb 1.1 */
    USB_SPEED_HIGH,				/* usb 2.0 */
    USB_SPEED_WIRELESS,			/* wireless (usb 2.5) */
    USB_SPEED_SUPER,			/* usb 3.0 */
};



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t USBEnumDone(uint32 * speed);
extern void USBReconnect(uint32 restart, uint32 fullspeed);
extern void USBDisconnect(void);
extern void USBConnect(bool FullSpeed);
extern void USBReset(void);
extern uint32 USBGetRxFIFOIntType(uint32 * epnum, uint32 * datalen);
extern uint32 USBGetOutEpIntType(void);
extern uint32 USBGetInEpIntType(void);
extern void USBSetAddr(uint32 addr);
extern uint8 USBCheckEpStall(uint8 dir, uint8 epnum);
extern rk_err_t USBWriteEp(uint32 epnum, uint32 len, uint8 * buf);
extern void USBEnableOutEp(uint32 epnum, uint32 bmAttributes, uint32 wMaxPacketSize);
extern rk_err_t USBReadEp(uint32 epnum, uint32 len, uint8 * buf);
extern void USBEnalbeEp(uint32 epnum, uint32 EnType, uint32 wMaxPacketSize);
extern uint32 UsbGetIntType(void);
extern void  UsbIntDisalbe(void);
extern void UsbIntEnable(void);



#endif

