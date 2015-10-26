/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\SpiNor\SpiNorDevice.h
* Owner: Aaron.sun
* Date: 2014.5.30
* Time: 9:09:04
* Desc: SPI NOR DEVICE CLASS
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    Aaron.sun     2014.5.30     9:09:04   1.0
********************************************************************************************
*/

#ifndef __DRIVER_SPINOR_SPINORDEVICE_H__
#define __DRIVER_SPINOR_SPINORDEVICE_H__

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
#define _DRIVER_SPINOR_SPINORDEVICE_COMMON_  __attribute__((section("driver_spinor_spinordevice_read")))
#define _DRIVER_SPINOR_SPINORDEVICE_INIT_  __attribute__((section("driver_spinor_spinordevice_init")))
#define _DRIVER_SPINOR_SPINORDEVICE_SHELL_  __attribute__((section("driver_spinor_spinordevice_shell")))

#define SPI_DEV_MAX_CNT 2

typedef  struct _SPINOR_DEV_ARG
{
   uint32 SpiChannel;
   HDC hSpi;

}SPINOR_DEV_ARG;


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
extern rk_err_t SpiNorDev_GetSize(HDC dev, uint32 * Size);
extern rk_err_t SpiNorDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t SpiNorDev_Write(HDC dev, uint32 LBA, uint8 * pBuf, uint32 Len);
extern rk_err_t SpiNorDev_Read(HDC dev, uint32 LBA, uint8 * pBuf, uint32 Len);
extern HDC SpiNorDev_Create(uint32 DevID, void * arg);


#endif

