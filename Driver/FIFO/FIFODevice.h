/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\FIFO\FIFODevice.h
* Owner: Aaron.sun
* Date: 2014.5.27
* Time: 20:38:01
* Desc: FIFO DEVICE CLASS
* History:
*    <author>     <date>       <time>     <version>       <Desc>
*    Aaron.sun     2014.5.27     20:38:01   1.0
********************************************************************************************
*/

#ifndef __DRIVER_FIFO_FIFODEVICE_H__
#define __DRIVER_FIFO_FIFODEVICE_H__

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
#define _DRIVER_FIFO_FIFODEVICE_COMMON_  __attribute__((section("driver_fifo_fifodevice_common")))
#define _DRIVER_FIFO_FIFODEVICE_INIT_  __attribute__((section("driver_fifo_fifodevice_init")))
#define _DRIVER_FIFO_FIFODEVICE_SHELL_  __attribute__((section("driver_fifo_fifodevice_shell")))

#define FIFO_DEV_NUM 2

typedef  struct _FIFO_DEV_ARG
{
    uint32 BlockSize;
    uint32 BlockCnt;
    uint32 UseFile;
    HDC hReadFile;
    HDC hWriteFile;

}FIFO_DEV_ARG;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef void (* pFIFO_CALL_BACK)(void);      //tx callback funciton



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t fifoDev_GetTotalSize(HDC dev, uint32 * Totalsize);
extern rk_err_t fifoDev_GetOffset(HDC dev, uint32 * offset);
extern rk_err_t fifoDev_ReadSeek(HDC dev,  uint32 pos, int32 offset);
extern rk_err_t fifoDev_SetTotalSize(HDC dev, uint32 TotalSize);
extern rk_err_t fifoDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t fifoDev_Write(HDC dev, uint8 * pbuf, uint32 size, uint32 Mode, pFIFO_CALL_BACK pfWriteCallBack);
extern rk_err_t fifoDev_Read(HDC dev, uint8 * pbuf, uint32 size, uint32 Mode, pFIFO_CALL_BACK pfReadCallBack);
extern rk_err_t fifoDev_Delete(uint32 DevID, void * arg);
extern HDC fifoDev_Create(uint32 DevID, void * arg);



#endif

