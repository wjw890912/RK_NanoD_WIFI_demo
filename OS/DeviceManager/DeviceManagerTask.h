/*
********************************************************************************************
*
*                  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: ..\OS\Plugin\DevicePlugin.h
* Owner: Aaron.sun
* Date: 2014.4.9
* Time: 8:57:22
* Desc: device manager task
* History:
*     <author>     <date>       <time>     <version>       <Desc>
* Aaron.sun     2014.4.9     8:57:22   1.0
********************************************************************************************
*/

#ifndef __OS_PLUGIN_DEVICEPLUGIN_H__
#define __OS_PLUGIN_DEVICEPLUGIN_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef enum _DEVICE_LIST
{
    DEVICE_LIST_EMMC_CODE,       //start LUN device(CLASS = LUN, ID = 0)
    DEVICE_LIST_EMMC_DATA,       //Start LUN device(CLASS = LUN, ID = 1)
    DEVICE_LIST_EMMC_USER1,      // start Volume Device(CLASS = VOLUME, ID = 0)
    DEVICE_LIST_EMMC_USER2,      // start volume device(CLASS = VOLUME, ID = 1)
    DEVICE_LIST_EMMC_USER3,      // start Volume device(CLASS = VOLUME, ID = 2)
    DEVICE_LIST_AUDIO,           //start audio device(CLASS = AUDIO, ID = 0)
    DEVICE_LIST_GUI,             //start audio gui(CLASS = GUI, ID = 0)
    DEVICE_LIST_SD,              //start sd device(CLASS = SD, ID = 0)
    DEVICE_LIST_USBHOST,         //start host device(CLASS = HOST, ID = 0)

    DEVICE_LIST_NAND_CODE,       //start LUN device(CLASS = LUN, ID = 0)
    DEVICE_LIST_NAND_DATA,       //Start LUN device(CLASS = LUN, ID = 1)
    DEVICE_LIST_NAND_USER1,      // start Volume Device(CLASS = VOLUME, ID = 0)
    DEVICE_LIST_NAND_USER2,      // start volume device(CLASS = VOLUME, ID = 1)
    DEVICE_LIST_NAND_USER3,      // start Volume device(CLASS = VOLUME, ID = 2)

    DEVICE_LIST_SPI_CODE,       //start LUN device(CLASS = LUN, ID = 0)
    DEVICE_LIST_SPI_DATA,       //Start LUN device(CLASS = LUN, ID = 1)
    DEVICE_LIST_SPI_USER1,      // start Volume Device(CLASS = VOLUME, ID = 0)
    DEVICE_LIST_SPI_USER2,      // start volume device(CLASS = VOLUME, ID = 1)
    DEVICE_LIST_SPI_USER3,      // start Volume device(CLASS = VOLUME, ID = 2)

    DEVICE_LIST_SDIO_FIFO,      //start SDIO device(CLASS = LUN, ID = 0)

    DEVICE_LIST_NUM,

}DEVICE_LIST;

typedef enum _DEVICE_CMD
{
    DEVICE_CMD_CREATE_LIST,
    DEVICE_CMD_DELETE_LIST,
    DEVICE_CMD_SUSPEND_LIST,
    DEVICE_CMD_RESUME_LIST,
    DEVICE_CMD_NUM

}DEVICE_CMD;




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

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
extern rk_err_t DeviceTask_CreateDeviceList(uint32 DeviceListID);
extern void DeviceTask(void);
extern rk_err_t DeviceTaskDeInit(void *pvParameters);
extern rk_err_t DeviceTaskInit(void *pvParameters);



#endif

