
#ifndef __DRIVER_USBETH_USBETHDEVICE_H__
#define __DRIVER_USBETH_USBETHDEVICE_H__


#include "asix88772b.h"

#define _DRIVER_USBETH_USBETHDEVICE_COMMON_  __attribute__((section("driver_usbeth_usbethdevice_common")))
#define _DRIVER_USBETH_USBETHDEVICE_INIT_    __attribute__((section("driver_usbeth_usbethdevice_init")))
#define _DRIVER_USBETH_USBETHDEVICE_SHELL_   __attribute__((section("driver_usbeth_usbethdevice_shell")))


#define ETH_RX_BUFFER_SIZE	2048
#define ETH_TX_BUFFER_SIZE	1540


typedef  struct _USBETH_DEV_ARG
{
    uint channel;
    HDC hUsbOtgDev;
    uint usbmode;

}USBETH_DEV_ARG;

extern rk_err_t USBETHDev_Shell(HDC dev, uint8 * pstr);
extern HDC USBETHDev_Create(uint32 DevID, void * arg);
extern rk_err_t USBETHDev_Delete(uint32 DevID, void * arg);
extern void USBETHDev_Task_Enter(void);
extern rk_err_t USBETHDev_Task_DeInit(void *pvParameters);
extern rk_err_t USBETHDev_Task_Init(void *pvParameters);

extern rk_err_t ethTask_DeInit(void *pvParameters);
extern rk_err_t ethTask_Init(void *pvParameters, void *arg);
extern void ethTask_Enter(void);



#endif



