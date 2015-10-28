/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*File    : device.h
* By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/
#ifndef _DEVICE_H_
#define _DEVICE_H_
#include "source_macro.h"
/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/
#define DEV_NAME_MAX_SIZE           32
#define DEV_MAX_NUM                 128

//DEVICE_CLASS->flag
#define RK_CREATED                  0x05

//DEVICE_CLASS->mode
#define READ_ONLY                   ((uint32)(1<<30))
#define WRITE_ONLY                  ((uint32)(1<<29))
#define CTRL_ONLY                   ((uint32)(1<<28))

#define READ_WRITE                  (READ_ONLY | WRITE_ONLY)
#define READ_CTRL                   (READ_ONLY | CTRL_ONLY)
#define WRITE_CTRL                  (WRITE_ONLY | CTRL_ONLY)
#define READ_WRITE_CTRL             (READ_ONLY | WRITE_ONLY | CTRL_ONLY)
typedef     void (*pFunc)(void);
#define NOT_CARE                    0

//DEVICE_CLASS->read(write) mode
#define SYNC_MODE               0x00 //SYNC
#define ASYNC_MODE              0x01 //ASYNC physical DEVICE INT and logical device complete(event)

typedef enum _DEVICE_STATUS
{
    DEVICE_STATUS_IDLE = 0,
    DEVICE_STATUS_ASYNC_PIO_READ,
    DEVICE_STATUS_ASYNC_DMA_READ,
    DEVICE_STATUS_SYNC_PIO_READ,
    DEVICE_STATUS_SYNC_DMA_READ,
    DEVICE_STATUS_ASYNC_PIO_WRITE,
    DEVICE_STATUS_ASYNC_DMA_WRITE,
    DEVICE_STATUS_SYNC_PIO_WRITE,
    DEVICE_STATUS_SYNC_DMA_WRITE,
    DEVICE_STATUS_IO_CONTROL,
    DEVICE_STATUS_NUM

}DEVICE_STATUS;


/*
--------------------------------------------------------------------------------

                        Struct Define

--------------------------------------------------------------------------------
*/

typedef void * HDC;


typedef struct _DEVICE_CLASS
{
    struct _DEVICE_CLASS * next;

    uint32 Cnt;
    uint32 SuspendCnt;
    uint32  DevClassID;
    uint32  DevID;

    rk_err_t  (*suspend)(HDC dev);
    rk_err_t  (*resume) (HDC dev);

}DEVICE_CLASS;

typedef enum DEV_CLASS_ID
{
    //add new device ID here
    //...
#ifdef __DRIVER_PWM_PWMDEVICE_C__
    DEV_CLASS_PWM,
#endif
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
    DEV_CLASS_DISPLAY,
#endif
#ifdef __DRIVER_KEY_KEYDEVICE_C__
    DEV_CLASS_KEY,
#endif
#ifdef __DRIVER_AD_ADCDEVICE_C__
    DEV_CLASS_ADC,
#endif
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    DEV_CLASS_TIMER,
#endif
#ifdef __DRIVER_SPIFLASH_SPIFLASHDEV_C__
    DEV_CLASS_SPIFLASH,
#endif
#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    DEV_CLASS_BCORE,
#endif
#ifdef __DRIVER_MSG_MSGDEVICE_C__
    DEV_CLASS_MSG,
#endif
#ifdef __DRIVER_FILE_DIRDEVICE_C__
    DEV_CLASS_DIR,
#endif
#ifdef __DRIVER_ROCKCODEC_ROCKCODECDEVICE_C__
    DEV_CLASS_ROCKCODEC,
#endif
#ifdef __DRIVER_I2S_I2SDEVICE_C__
    DEV_CLASS_I2S,
#endif

#ifdef __DRIVER_AUDIO_AUDIODEVICE_C__
    DEV_CLASS_AUDIO,
#endif

#ifdef __DRIVER_USBHUB_USBHUBDEVICE_C__
    DEV_CLASS_USBHUB,
#endif

#ifdef __DRIVER_USBETH_USBETHDEVICE_C__
    DEV_CLASS_USBETH,
#endif

#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    DEV_CLASS_USBMSC,
#endif
#ifdef __DRIVER_MAILBOX_MAILBOXDEVICE_C__
    DEV_CLASS_MAILBOX,
#endif
#ifdef __DRIVER_WATCHDOG_WATCHDOGDEVICE_C__
    DEV_CLASS_WDT,
#endif

#ifdef __DRIVER_WM8987_WM8987DEVICE_C__
    DEV_CLASS_WM8987,
#endif
#ifdef __DRIVER_RK61X_RK618DEVICE_C__
    DEV_CLASS_RK618,
#endif
#ifdef __DRIVER_SD_SDDEVICE_C__
    DEV_CLASS_SD,
#endif
#ifdef __DRIVER_I2C_I2CDEVICE_C__
    DEV_CLASS_I2C,
#endif
#ifdef __DRIVER_USB_USBOTGDEV_C__
    DEV_CLASS_USBOTG,
#endif
#ifdef __DRIVER_WLC_WLCCD_C__
    DEV_CLASS_WLCCC,
#endif
#ifdef __DRIVER_RK903_RK903DEVICE_C__
    DEV_CLASS_RK903,
#endif
#ifdef __DRIVER_WLC_WLCDEVICE_C__
    DEV_CLASS_WLC,
#endif
#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    DEV_CLASS_SDIO,
#endif
#ifdef __DRIVER_SPINOR_SPINORDEVICE_C__
    DEV_CLASS_SPINOR,
#endif

#ifdef __DRIVER_SPI_SPIDEVICE_C__
    DEV_CLASS_SPI,
#endif

#ifdef __DRIVER_FIFO_FIFODEVICE_C__
    DEV_CLASS_FIFO,
#endif

#ifdef __DRIVER_EXAMPLE_DEVICEFORMAT_C__
    DEV_CLASS_TEST,
#endif

#ifdef __DRIVER_UART_UARTDEVICE_C__
    DEV_CLASS_UART,
#endif

#ifdef __DRIVER_DMA_DMADEVICE_C__
    DEV_CLASS_DMA,
#endif

#ifdef __DRIVER_SDMMC_SDMMCDEVICE_C__
    DEV_CLASS_SDC,
#endif

#ifdef __DRIVER_EMMC_EMMCDEVICE_C__
    DEV_CLASS_EMMC,
#endif

#ifdef __DRIVER_LUN_LUNDEVICE_C__
    DEV_CLASS_LUN,
#endif

#ifdef __DRIVER_PARTION_PARTIONDEVICE_C__
    DEV_CLASS_PAR,
#endif

#ifdef __DRIVER_VOLUME_FATDEVICE_C__
    DEV_CLASS_FAT,
#endif

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    DEV_CLASS_FILE,
#endif

#ifdef __DRIVER_ALC5633_ALC5633DEVICE_C__
    DEV_CLASS_ALC5633,
#endif

#ifdef __DRIVER_USB_USBOTGDEVICE_C__
    DEV_CLASS_USBOtg,
#endif

#ifdef __DRIVER_USB_USBDEVICE_C__
    DEV_CLASS_USBDev,
#endif

#ifdef __DRIVER_MSC_MSCDEVICE_C__
    DEV_CLASS_MSC,
#endif

    DEV_CLASS_MAX_NUM

}DEV_CLASS_ID;
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

extern HDC RKDev_Open(uint8 DevClassID, uint8 DevID, uint32 DevMode);
extern rk_err_t RKDev_Close(HDC dev);
extern rk_err_t RKDev_Create(uint32 DevClassID, uint32 DevID, void * arg);
extern rk_err_t  RKDev_Suspend(HDC* dev);
extern rk_err_t  RKDev_Resume(HDC* dev);
extern rk_err_t  RKDev_Delete(uint32 DevClassID, uint32 DevID, void * arg);
extern HDC RK_CompetenceToDev(HDC dev, uint32 Mode);
extern HDC RK_DevToCompetence(HDC dev, uint32 Mode);
extern rk_err_t  RKDev_Init(void);

#endif

