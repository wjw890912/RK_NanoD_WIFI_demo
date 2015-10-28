
#ifndef __DRIVER_USBHUB_USBHUBDEVICE_H__
#define __DRIVER_USBHUB_USBHUBDEVICE_H__


#define _DRIVER_USBHUB_USBHUBDEVICE_COMMON_  __attribute__((section("driver_usbhub_usbhubdevice_common")))
#define _DRIVER_USBHUB_USBHUBDEVICE_INIT_    __attribute__((section("driver_usbhub_usbhubdevice_init")))
#define _DRIVER_USBHUB_USBHUBDEVICE_SHELL_   __attribute__((section("driver_usbhub_usbhubdevice_shell")))

typedef  struct _USBHUB_DEV_ARG
{
    uint channel;
    HDC hUsbOtgDev;
    uint usbmode;

}USBHUB_DEV_ARG;



/*
 * Port feature numbers
 */
#define USB_PORT_FEAT_CONNECTION     0
#define USB_PORT_FEAT_ENABLE         1
#define USB_PORT_FEAT_SUSPEND        2
#define USB_PORT_FEAT_OVER_CURRENT   3
#define USB_PORT_FEAT_RESET          4
#define USB_PORT_FEAT_POWER          8
#define USB_PORT_FEAT_LOWSPEED       9
#define USB_PORT_FEAT_HIGHSPEED      10
#define USB_PORT_FEAT_C_CONNECTION   16
#define USB_PORT_FEAT_C_ENABLE       17
#define USB_PORT_FEAT_C_SUSPEND      18
#define USB_PORT_FEAT_C_OVER_CURRENT 19
#define USB_PORT_FEAT_C_RESET        20
#define USB_PORT_FEAT_TEST           21

/* wPortChange bits */
#define USB_PORT_STAT_C_CONNECTION  0x0001
#define USB_PORT_STAT_C_ENABLE      0x0002
#define USB_PORT_STAT_C_SUSPEND     0x0004
#define USB_PORT_STAT_C_OVERCURRENT 0x0008
#define USB_PORT_STAT_C_RESET       0x0010

/* wPortStatus bits */
#define USB_PORT_STAT_CONNECTION    0x0001
#define USB_PORT_STAT_ENABLE        0x0002
#define USB_PORT_STAT_SUSPEND       0x0004
#define USB_PORT_STAT_OVERCURRENT   0x0008
#define USB_PORT_STAT_RESET         0x0010
#define USB_PORT_STAT_POWER         0x0100
#define USB_PORT_STAT_LOW_SPEED     0x0200
#define USB_PORT_STAT_HIGH_SPEED    0x0400	/* support for EHCI */
#define USB_PORT_STAT_SUPER_SPEED   0x0600	/* faking support to XHCI */
#define USB_PORT_STAT_SPEED_MASK	\
	(USB_PORT_STAT_LOW_SPEED | USB_PORT_STAT_HIGH_SPEED)



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t USBHUBDev_Shell(HDC dev, uint8 * pstr);
extern HDC USBHUBDev_Create(uint32 DevID, void * arg);
extern rk_err_t USBHUBDev_Delete(uint32 DevID, void * arg);
extern void USBHUBDev_Task_Enter(void);
extern rk_err_t USBHUBDev_Task_DeInit(void *pvParameters);
extern rk_err_t USBHUBDev_Task_Init(void *pvParameters);

#endif




