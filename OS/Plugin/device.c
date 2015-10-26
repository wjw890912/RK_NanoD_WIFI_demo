/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                               All rights reserved.
*File    : device.C
* By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/
#include "BspConfig.h"

#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
rk_err_t  RKDeviceRegister(DEVICE_CLASS *pDev);
rk_err_t  RKDeviceUnRegister(DEVICE_CLASS* RKDev);
rk_err_t  RKDeviceFind(uint8 DevClassID,uint8 DevID);

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define DEV_ORIGINAL_ADDRESS  0x0fffffff

typedef struct _DEV_INFO
{
    uint32 ModuleID;
    DEVICE_CLASS *(*pfCreate)(uint32 DevID, void *arg);
    rk_err_t (*pfDelete)(uint32 DevID, void *arg);

}DEV_INFO;


static DEV_INFO DevInfo_Table[DEV_CLASS_MAX_NUM] =
{
    //add new device info here
    //......
#ifdef __DRIVER_PWM_PWMDEVICE_C__
    MODULE_ID_PWM_INIT,PwmDev_Create,PwmDev_Delete,
#endif
#ifdef __DRIVER_DISPLAY_DISPLAYDEVICE_C__
    MODULE_ID_DISPLAY_INIT,DisplayDev_Create,DisplayDev_Delete,
#endif
#ifdef __DRIVER_KEY_KEYDEVICE_C__
    MODULE_ID_KEY_INIT,KeyDev_Create,KeyDev_Delete,
#endif
#ifdef __DRIVER_AD_ADCDEVICE_C__
    MODULE_ID_ADC_INIT,ADCDev_Create,ADCDev_Delete,
#endif
#ifdef __DRIVER_TIMER_TIMERDEVICE_C__
    MODULE_ID_TIMER_INIT,TimerDev_Create,TimerDev_Delete,
#endif
#ifdef __DRIVER_SPIFLASH_SPIFLASHDEV_C__
    NULL,SpiFlashDev_Create,SpiFlashDev_Delete,
#endif
#ifdef __DRIVER_BCORE_BCOREDEVICE_C__
    MODULE_ID_BCORE_INIT,BcoreDev_Create,BcoreDev_Delete,
#endif
#ifdef __DRIVER_MSG_MSGDEVICE_C__
    MODULE_ID_MSG_INIT,MsgDev_Create,MsgDev_Delete,
#endif
#ifdef __DRIVER_FILE_DIRDEVICE_C__
    MODULE_ID_DIR_INIT,DirDev_Create,DirDev_Delete,
#endif
#ifdef __DRIVER_ROCKCODEC_ROCKCODECDEVICE_C__
    MODULE_ID_ROCKCODEC_INIT,RockCodecDev_Create,RockCodecDev_Delete,
#endif
#ifdef __DRIVER_I2S_I2SDEVICE_C__
    MODULE_ID_I2S_INIT,I2SDev_Create,I2SDev_Delete,
#endif

#ifdef __DRIVER_AUDIO_AUDIODEVICE_C__
    MODULE_ID_AUDIO_INIT,AudioDev_Create,AudioDev_Delete,
#endif

#ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
    MODULE_ID_USBMSC_INIT,USBMSCDev_Create,USBMSCDev_Delete,
#endif
#ifdef __DRIVER_MAILBOX_MAILBOXDEVICE_C__
    MODULE_ID_MAILBOX_INIT,MailBoxDev_Create,MailBoxDev_Delete,
#endif
#ifdef __DRIVER_WATCHDOG_WATCHDOGDEVICE_C__
    NULL,WDTDev_Create,NULL,
#endif

#ifdef __DRIVER_WM8987_WM8987DEVICE_C__
    NULL,WM8987Dev_Create,NULL,
#endif
#ifdef __DRIVER_RK61X_RK618DEVICE_C__
    MODULE_ID_RK618_INIT,RK618Dev_Create,RK618Dev_Delete,
#endif
#ifdef __DRIVER_SD_SDDEVICE_C__
    MODULE_ID_SD_INIT,SdDev_Create,SdDev_Delete,
#endif
#ifdef __DRIVER_I2C_I2CDEVICE_C__
    MODULE_ID_I2C_INIT,I2CDev_Create,I2CDev_Delete,
#endif
#ifdef __DRIVER_USB_USBOTGDEV_C__
    MODULE_ID_USBOTG_INIT,UsbOtgDev_Create,UsbOtgDev_Delete,
#endif
#ifdef __DRIVER_WLC_WLCCD_C__
    NULL,WLCCCDev_Create,NULL,
#endif
#ifdef __DRIVER_RK903_RK903DEVICE_C__
    MODULE_ID_RK903_INIT,Rk903Dev_Create,NULL,
#endif
#ifdef  __DRIVER_WLC_WLCDEVICE_C__
    MODULE_ID_WLC_INIT,WlcDev_Create,NULL,
#endif
#ifdef __DRIVER_SDIO_SDIODEVICE_C__
    MODULE_ID_SDIO_INIT,SdioDev_Create,SdioDevDelete,
#endif
#ifdef __DRIVER_SPINOR_SPINORDEVICE_C__
    NULL,SpiNorDev_Create,NULL,
#endif

#ifdef __DRIVER_SPI_SPIDEVICE_C__
    NULL,SpiDev_Create,NULL,
#endif

#ifdef __DRIVER_FIFO_FIFODEVICE_C__
    MODULE_ID_FIFO_INIT,fifoDev_Create,fifoDev_Delete,
#endif

#ifdef __DRIVER_EXAMPLE_DEVICEFORMAT_C__
    NULL,TestDev_Create,NULL,
#endif

#ifdef __DRIVER_UART_UARTDEVICE_C__
    NULL,UartDev_Create,NULL,
#endif

#ifdef __DRIVER_DMA_DMADEVICE_C__
    NULL,DmaDev_Create,NULL,
#endif

#ifdef _SPI_BOOT_
#ifdef __DRIVER_SDMMC_SDMMCDEVICE_C__
    MODULE_ID_SDC_INIT,SdcDev_Create,SdcDevDelete,
#endif
#endif

#ifdef _EMMC_BOOT_
#ifdef __DRIVER_SDMMC_SDMMCDEVICE_C__
    NULL,SdcDev_Create,SdcDevDelete,
#endif
#endif

#ifdef __DRIVER_EMMC_EMMCDEVICE_C__
    NULL,EmmcDev_Create,NULL,
#endif

#ifdef __DRIVER_LUN_LUNDEVICE_C__
    NULL,LunDev_Create,LunDev_Delete,
#endif

#ifdef __DRIVER_PARTION_PARTIONDEVICE_C__
    MODULE_ID_PAR_INIT,ParDev_Create,NULL,
#endif

#ifdef __DRIVER_VOLUME_FATDEVICE_C__
    MODULE_ID_FAT_INIT,FatDev_Create,NULL,
#endif

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    MODULE_ID_FILE_INIT,FileDev_Create,NULL,
#endif

#ifdef __DRIVER_ALC5633_ALC5633DEVICE_C__
    NULL,ALC5633Dev_Create,NULL,
#endif

#ifdef __DRIVER_USB_USBOTGDEVICE_C__
    NULL,USBOtgDevCreate,NULL,
#endif

#ifdef __DRIVER_USB_USBDEVICE_C__
    NULL,USBDevCreate,NULL,
#endif

#ifdef __DRIVER_MSC_MSCDEVICE_C__
    NULL,MscDevCreate,NULL,
#endif

};

static DEVICE_CLASS *DevListHead = NULL;
static pSemaphore osDeviceReqSem;



/*
*********************************************************************************************************
*                                RKDeviceHandler RKDeviceFind(const char *pDeviceName)
*
* Description:  任务通过设备名称找到设备
*
* Argument(s) : const char *pDeviceName
*
* Return(s)   : RKDeviceHandler
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKDeviceFind(uint8 DevClassID,uint8 DevID)
{
    DEVICE_CLASS *pDev = DevListHead;

    while (pDev != NULL)
    {
        if ((pDev->DevClassID == DevClassID)&&(pDev->DevID == DevID))
            return (rk_err_t)pDev;

        pDev = pDev->next;
    }

    return RK_ERROR;
}

/*
*********************************************************************************************************
*                                 rk_err_t RKDeviceRegister(uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void))
*
* Description :  向操作系统注册设备.
*
* Argument(s) : uint DevNO,char *pDeviceName,RKDeviceHandler (*DevCreate)(void)
*
* Return(s)   : rk_err_t
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKDeviceRegister(DEVICE_CLASS *pDev)
{
    DEVICE_CLASS *pDevTemp;

    if(DevListHead == NULL)
    {
        DevListHead = pDev;
    }
    else
    {
        pDevTemp = DevListHead;
        while(pDevTemp->next != NULL)
        {
            pDevTemp = pDevTemp->next;
        }
        pDevTemp->next = pDev;
    }

    return RK_SUCCESS;
}

/*
*********************************************************************************************************
*                                   rk_err_t RKDeviceUnRegister(RKDeviceHandler RKDev)
*
* Description:  将设备从链表中删除.
*
* Argument(s) : none
*
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t RKDeviceUnRegister(DEVICE_CLASS* pDev)
{
    DEVICE_CLASS *pDevTemp = DevListHead;
    DEVICE_CLASS *pLastDevTemp = NULL;

    while (pDevTemp != NULL)
    {
        if (pDevTemp == pDev)
        {
            if(pLastDevTemp == NULL)
            {
                DevListHead = NULL;
            }
            else
            {
                pLastDevTemp->next = pDev->next;
            }
            return RK_SUCCESS;
        }
        pLastDevTemp = pDevTemp;
        pDevTemp = pDevTemp->next;
    }

    return RK_ERROR;
}

/*
*********************************************************************************************************
*                                   rk_err_t RKDeviceUnRegister(RKDeviceHandler RKDev)
*
* Description:  将设备从链表中删除.
*
* Argument(s) : none
*
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/
COMMON API rk_err_t RKDev_Create(uint32 DevClassID, uint32 DevID, void* arg)
{
    DEVICE_CLASS* pDev;
    rk_err_t ret;

    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);

    ret = RKDeviceFind(DevClassID, DevID);

    if (ret == RK_ERROR)
    {
        if(DevInfo_Table[DevClassID].ModuleID != 0)
        {
            #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
            FW_ModuleOverlay(DevInfo_Table[DevClassID].ModuleID, MODULE_OVERLAY_CODE);
            #endif
        }
        pDev = DevInfo_Table[DevClassID].pfCreate(DevID, arg);

        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_RemoveModule(DevInfo_Table[DevClassID].ModuleID);
        #endif

        if((rk_err_t)pDev <= 0)
        {
            rkos_semaphore_give(osDeviceReqSem);
            return RK_ERROR;
        }

        pDev->Cnt = 0;
        pDev->DevClassID = DevClassID;
        pDev->DevID = DevID;
        pDev->next = NULL;
        RKDeviceRegister(pDev);
        rkos_semaphore_give(osDeviceReqSem);
        return RK_SUCCESS;
    }
    else
    {
        rkos_semaphore_give(osDeviceReqSem);
        return RK_ERROR;
    }
}


COMMON API HDC RKDev_Open(uint8 DevClassID, uint8 DevID, uint32 DevMode)
{
    DEVICE_CLASS* pDev;
    rk_err_t ret;

    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);
    ret = RKDeviceFind(DevClassID,DevID);

    if (ret == RK_ERROR)
    {
        rkos_semaphore_give(osDeviceReqSem);
        return NULL;
    }
    else
    {
        pDev = (DEVICE_CLASS *)ret;
        pDev->Cnt++;
        pDev = RK_DevToCompetence(pDev, DevMode);
    }
    rkos_semaphore_give(osDeviceReqSem);
    return pDev;


}

COMMON API rk_err_t RKDev_Close(HDC dev)
{
    DEVICE_CLASS * pDev = (DEVICE_CLASS *)((uint32)dev & DEV_ORIGINAL_ADDRESS);

    if(pDev == NULL)
    {
        return RK_ERROR;
    }

    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);
    if(pDev->Cnt > 0)
        pDev->Cnt--;
    rkos_semaphore_give(osDeviceReqSem);

    return RK_SUCCESS;
}


/*******************************************************************************
** Name:
** Input:
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.2.24
** Time: 10:47:31
*******************************************************************************/
COMMON API HDC RK_CompetenceToDev(HDC dev, uint32 Mode)
{
    HDC ExclusiveDev = NULL;

    if((dev != NULL) && (((uint32)dev & Mode) || (((uint32)dev & 0x07000000) == 0)))
    {
        ExclusiveDev = (HDC)((uint32)dev &= DEV_ORIGINAL_ADDRESS);
        return ExclusiveDev;
    }
    else
    {
        return NULL;
    }

}

/*******************************************************************************
** Name:
** Input:
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.2.24
** Time: 10:47:31
*******************************************************************************/
COMMON API HDC RK_DevToCompetence(HDC dev,uint32 Mode)
{
    HDC ExclusiveDev = NULL;
    if (dev != NULL)
    {
        ExclusiveDev = (HDC)((uint32)dev |= Mode);
        return ExclusiveDev;
    }
    else
    {
        return dev;
    }
}

COMMON API rk_err_t  RKDev_Suspend(HDC* dev)
{
    DEVICE_CLASS* pDev = (DEVICE_CLASS*)RK_DevToCompetence(dev,CTRL_ONLY);

    rk_err_t ret;
    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);
    if(pDev->SuspendCnt >= 0)
    {
         pDev->SuspendCnt--;
    }

    if( pDev->SuspendCnt == 0)
    {
        ret = pDev->suspend(dev);
    }

    rkos_semaphore_give(osDeviceReqSem);
    return ret;
}


COMMON API rk_err_t  RKDev_Resume(HDC* dev)
{
    DEVICE_CLASS* pDev = (DEVICE_CLASS*)RK_DevToCompetence(dev,CTRL_ONLY);
    rk_err_t ret;

    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);
    if(pDev->SuspendCnt < pDev->Cnt)
    {
        pDev->SuspendCnt++;
    }

    if(pDev->SuspendCnt == 0)
    {
        ret = pDev->resume(pDev);
    }
    rkos_semaphore_give(osDeviceReqSem);
    return ret;
}


COMMON API rk_err_t  RKDev_Delete(uint32 DevClassID, uint32 DevID, void * arg)
{
    DEVICE_CLASS* pDev;
    rk_err_t ret;

    rkos_semaphore_take(osDeviceReqSem, MAX_DELAY);
    ret = RKDeviceFind(DevClassID, DevID);

    if (ret != RK_ERROR)
    {
        pDev = (DEVICE_CLASS *)ret;

        if(pDev->Cnt != 0)
        {
            rkos_semaphore_give(osDeviceReqSem);
            return RK_ERROR;
        }

        RKDeviceUnRegister(pDev);

        if(DevInfo_Table[DevClassID].ModuleID != 0)
        {
            #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
            FW_ModuleOverlay(DevInfo_Table[DevClassID].ModuleID, MODULE_OVERLAY_CODE);
            #endif
        }

        ret = DevInfo_Table[DevClassID].pfDelete(DevID, arg);

        #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
        FW_RemoveModule(DevInfo_Table[DevClassID].ModuleID);
        #endif


        if(ret != RK_SUCCESS)
        {
            rkos_semaphore_give(osDeviceReqSem);
            return RK_ERROR;
        }

        rkos_semaphore_give(osDeviceReqSem);
        return RK_SUCCESS;
    }
    else
    {
        rkos_semaphore_give(osDeviceReqSem);
        return RK_ERROR;
    }

}


COMMON API rk_err_t  RKDev_Init(void)
{
    DevListHead = NULL;
    osDeviceReqSem  = rkos_semaphore_create(1,1);
    if(osDeviceReqSem == NULL)
    {
        return RK_ERROR;
    }

    return RK_SUCCESS;
}
