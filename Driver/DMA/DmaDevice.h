/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\DMA\DmaDevice.h
* Owner: Aaron.sun
* Date: 2014.4.24
* Time: 10:57:36
* Desc: Dma Device Class
* History:
*	 <author>	 <date> 	  <time>	 <version>	   <Desc>
* Aaron.sun     2014.4.24     10:57:36   1.0
********************************************************************************************
*/

#ifndef __DRIVER_DMA_DMADEVICE_H__
#define __DRIVER_DMA_DMADEVICE_H__

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
#define _DRIVER_DMA_DMADEVICE_COMMON_  __attribute__((section("driver_dma_dmadevice_read")))
#define _DRIVER_DMA_DMADEVICE_INIT_  __attribute__((section("driver_dma_dmadevice_init")))
#define _DRIVER_DMA_DMADEVICE_SHELL_  __attribute__((section("driver_dma_dmadevice_shell")))

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
extern rk_err_t DmaDev_RealseChannel(HDC dev ,uint32 ch);
extern rk_err_t DmaDev_GetChannel(HDC dev);
extern rk_err_t DmaDev_DmaReStart(HDC dev, uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack);
extern rk_err_t DmaDev_DmaStart(HDC dev, uint32 ch, uint32 srcAddr, uint32 dstAddr, uint32 size, pDMA_CFGX g_dmaPar, pFunc CallBack);
extern rk_err_t DmaDev_Shell(HDC dev, uint8 * pstr);
extern HDC DmaDev_Create(uint32 DevID, void * arg);



#endif

