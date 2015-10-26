/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\FIFO\FIFODevice.c
* Owner: Aaron.sun
* Date: 2014.5.27
* Time: 20:37:09
* Desc: FIFO Device Class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    Aaron.sun     2014.5.27     20:37:09   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_FIFO_FIFODEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "AudioControlTask.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _FIFO_DEVICE_CLASS
{
    DEVICE_CLASS stfifoDevice;

    pSemaphore osfifoReadReqSem;
    pSemaphore osfifoWriteReqSem;

    pSemaphore osfifoReadSem;
    pSemaphore osfifoWriteSem;

    pSemaphore osfifoBufLevelSem;

    uint32 BlockSize;
    uint32 BlockCnt;
    uint32 CurSize;
    uint32 TotalSize;
    uint8  *pbuf;
    uint32 front;
    uint32 back;
    uint32 ReadReqSize;
    uint32 ReadStatus;
    uint32 BufLevel;
    uint32 WriteReqSize;
    uint32 WriteStatus;
    uint32 UseFile;
    HDC hReadFile;
    HDC hWriteFile;

    pFIFO_CALL_BACK pfReadCallBack;
    pFIFO_CALL_BACK pfWriteCallBack;

}FIFO_DEVICE_CLASS;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static FIFO_DEVICE_CLASS * gpstfifoDevISR[FIFO_DEV_NUM];



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
rk_err_t fifoDevShellHelp(HDC dev,  uint8 * pstr);
rk_err_t fifoDevShellTest(HDC dev, uint8 * pstr);
rk_err_t fifoDevShellDel(HDC dev, uint8 * pstr);
rk_err_t fifoDevShellMc(HDC dev, uint8 * pstr);
rk_err_t fifoDevShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: fifoDev_GetTotalSize
** Input:HDC dev, uint32 * Totalsize
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 10:03:20
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_GetTotalSize(HDC dev, uint32 * Totalsize)
{
    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;

    if(pstFIFODev->TotalSize == 0)
    {
        rk_printf("get total size");
        rkos_semaphore_take(pstFIFODev->osfifoReadSem, MAX_DELAY);
    }

    *Totalsize = pstFIFODev->TotalSize;
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: fifoDev_GetOffset
** Input:HDC dev, uint32 * offset
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 10:02:42
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_GetOffset(HDC dev, uint32 * offset)
{
    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;
    *offset = pstFIFODev->back;
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: fifoDev_ReadSeek
** Input:HDC dev,  uint32 pos, int32 offset
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.14
** Time: 8:55:38
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_ReadSeek(HDC dev,  uint32 pos, int32 offset)
{

    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;
    uint32 fifolen, front, back, front1, back1;

    if(pstFIFODev == NULL)
    {
        return RK_ERROR;
    }

    rk_printf("file seek1");
    rkos_semaphore_take(pstFIFODev->osfifoReadReqSem, MAX_DELAY);

    rkos_enter_critical();
    fifolen = pstFIFODev->BlockCnt * pstFIFODev->BlockSize + 1;
    front = pstFIFODev->front % fifolen;
    back = pstFIFODev->back % fifolen;
    front1 = pstFIFODev->front;
    back1 = pstFIFODev->back;
    rkos_exit_critical();

    if(pos == SEEK_END)
    {
        return RK_ERROR;
    }
    else if(pos == SEEK_CUR)
    {
        if(pstFIFODev->UseFile == 0)
        {
            if(offset < 0)
            {
                return RK_ERROR;
            }
        }

        while((pstFIFODev->front) <= ( pstFIFODev->back + offset))
        {
            rkos_semaphore_take(pstFIFODev->osfifoReadSem, MAX_DELAY);

            rkos_enter_critical();
            fifolen = pstFIFODev->BlockCnt * pstFIFODev->BlockSize + 1;
            front = pstFIFODev->front % fifolen;
            back = pstFIFODev->back % fifolen;
            front1 = pstFIFODev->front;
            back1 = pstFIFODev->back;
            rkos_exit_critical();


        }

        if(pstFIFODev->UseFile == 0)
        {
            pstFIFODev->back += offset;
        }
        else
        {
            pstFIFODev->back += offset;
            FileDev_FileSeek(pstFIFODev->hReadFile, SEEK_SET, pstFIFODev->back);
        }


        rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
        return RK_SUCCESS;

    }
    else if(pos == SEEK_SET)
    {
        if(pstFIFODev->UseFile == 0)
        {
            if(offset < pstFIFODev->back)
            {
                return RK_ERROR;
            }
        }

        while(pstFIFODev->front <= offset)
        {
            pstFIFODev->ReadStatus = DEVICE_STATUS_SYNC_PIO_READ;
            pstFIFODev->ReadReqSize = offset - pstFIFODev->back;

            rk_printf("file seek suspend");
            rkos_semaphore_take(pstFIFODev->osfifoReadSem, MAX_DELAY);
            rkos_enter_critical();
            fifolen = pstFIFODev->BlockCnt * pstFIFODev->BlockSize + 1;
            front = pstFIFODev->front % fifolen;
            back = pstFIFODev->back % fifolen;
            front1 = pstFIFODev->front;
            back1 = pstFIFODev->back;
            rkos_exit_critical();
        }

        if(pstFIFODev->UseFile == 0)
        {
            pstFIFODev->back = offset;
        }
        else
        {
            pstFIFODev->back = offset;
            FileDev_FileSeek(pstFIFODev->hReadFile, SEEK_SET, pstFIFODev->back);
        }

        rk_printf("fifo seek ok, back = %d", pstFIFODev->back);

        rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
        return RK_SUCCESS;

    }


}

/*******************************************************************************
** Name: fifoDev_SetTotalSize
** Input:HDC dev, uint32 TotalSize
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.9
** Time: 9:14:55
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_SetTotalSize(HDC dev, uint32 TotalSize)
{
    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;
    FILE_ATTR stFileAttr;
    HDC hFileDev;

    pstFIFODev->front = 0;
    pstFIFODev->back = 0;
    pstFIFODev->ReadReqSize = 0;
    pstFIFODev->WriteReqSize = 0;


    if(pstFIFODev->TotalSize == 0)
    {
        pstFIFODev->TotalSize = TotalSize;

        if(pstFIFODev->TotalSize != 0)
        {
            rk_printf("set total size");
            rkos_semaphore_give(pstFIFODev->osfifoReadSem);
        }
    }
    else
    {

        FileDev_FileSeek(pstFIFODev->hReadFile, SEEK_SET, 0);

        if(pstFIFODev->hWriteFile != NULL)
        {
            FileDev_CloseFile(pstFIFODev->hWriteFile);
        }

        stFileAttr.Path = L"C:\\";
        stFileAttr.FileName = L"RkFifiDevice.buf";

        hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

         if((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
         {
             rk_print_string("File device open failure");
         }


        pstFIFODev->hWriteFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);
        if(pstFIFODev->hWriteFile <= 0)
        {
            rk_print_string("file cache buffer open fail");
            return RK_SUCCESS;
        }


        pstFIFODev->TotalSize = TotalSize;
    }

}

/*******************************************************************************
** Name: fifoDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_Read(HDC dev, uint8 * pbuf, uint32 size, uint32 Mode, pFIFO_CALL_BACK pfReadCallBack)
{
    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;
    uint32 fifolen, front, back, front1, back1;

    if(pstFIFODev == NULL)
    {
        return RK_ERROR;
    }

    rkos_semaphore_take(pstFIFODev->osfifoReadReqSem, MAX_DELAY);

    if(pstFIFODev->back >= pstFIFODev->TotalSize)
    {
        rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
        return RK_ERROR;
    }

    if((pstFIFODev->back + size) > pstFIFODev->TotalSize)
    {
        rk_printf("fifo end, back = %d", pstFIFODev->back);
        size = pstFIFODev->TotalSize - pstFIFODev->back;
    }

    if((pstFIFODev->CurSize < 0x10) && (pstFIFODev->front < pstFIFODev->TotalSize))
    {
         rk_printf("buf level wait");

         pstFIFODev->BufLevel = 1;
         rkos_semaphore_take(pstFIFODev->osfifoBufLevelSem,  MAX_DELAY);
    }

    rkos_enter_critical();
    fifolen = pstFIFODev->BlockCnt * pstFIFODev->BlockSize + 1;
    front = pstFIFODev->front % fifolen;
    back = pstFIFODev->back % fifolen;
    front1 = pstFIFODev->front;
    back1 = pstFIFODev->back;
    rkos_exit_critical();

    if((front1 - back1) < size)
    {
        if(Mode == SYNC_MODE)
        {
            pstFIFODev->ReadStatus = DEVICE_STATUS_SYNC_PIO_READ;
            pstFIFODev->ReadReqSize = size;
            rk_printf("fifo read suspend front1 = %d, back1 = %d, size = %d", front1, back1, size);
            rkos_semaphore_take(pstFIFODev->osfifoReadSem, MAX_DELAY);
            rkos_enter_critical();
            front = pstFIFODev->front % fifolen;
            back = pstFIFODev->back % fifolen;
            rkos_exit_critical();
        }
        else if(Mode == ASYNC_MODE)
        {
            pstFIFODev->ReadStatus = DEVICE_STATUS_ASYNC_PIO_READ;
            pstFIFODev->ReadReqSize = size;
            pstFIFODev->pfReadCallBack = pfReadCallBack;
            return 0;
        }
        else
        {
            rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
            return RK_PARA_ERR;
        }
    }

    if(front >= (back + size))
    {
        if(pstFIFODev->UseFile)
        {
            rk_printf("fifo read front1 = %d, back1 = %d", front1, back1);
            FileDev_ReadFile(pstFIFODev->hReadFile, pbuf, size);
        }
        else
        {
            memcpy(pbuf, pstFIFODev->pbuf + back, size);
        }
    }
    else if(front <= (back - 1))
    {
        rk_printf("fifo read err1");

        if(size <= (fifolen - back))
        {
            memcpy(pbuf, pstFIFODev->pbuf + back, size);
        }
        else
        {
            memcpy(pbuf, pstFIFODev->pbuf + back, fifolen - back);

            if(front >= (size - fifolen + back))
            {
                memcpy(pbuf + fifolen - back, pstFIFODev->pbuf, size - fifolen + back);
            }
            else
            {
                rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
                return RK_ERROR;
            }
        }
    }
    else
    {
       rk_printf("fifo read err2, front = %d, back = %d", front, back);
       rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
       return RK_ERROR;
    }

    pstFIFODev->back += size;


    if(pstFIFODev->WriteReqSize != 0)
    {
        if((fifolen - pstFIFODev->front + pstFIFODev->back) >= pstFIFODev->WriteReqSize)
        {
            if(pstFIFODev->WriteStatus == DEVICE_STATUS_ASYNC_PIO_WRITE)
            {
                pstFIFODev->pfWriteCallBack();
                pstFIFODev->WriteStatus = DEVICE_STATUS_IDLE;
                pstFIFODev->WriteReqSize = 0;
                rkos_semaphore_give(pstFIFODev->osfifoWriteReqSem);

            }
            else if(pstFIFODev->WriteStatus == DEVICE_STATUS_SYNC_PIO_WRITE)
            {
                rkos_semaphore_give(pstFIFODev->osfifoWriteSem);
            }
            else
            {
                pstFIFODev->WriteStatus = DEVICE_STATUS_IDLE;
                pstFIFODev->WriteReqSize = 0;
            }
        }
    }

    pstFIFODev->CurSize = pstFIFODev->front - pstFIFODev->back;

    if(pstFIFODev->back >= 0xefffffff)
    {
        pstFIFODev->back -= 0xefffffff;
        pstFIFODev->front -= 0xefffffff;
    }

    rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
    return (rk_err_t)size;

}

/*******************************************************************************
** Name: fifoDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_COMMON_
COMMON API rk_err_t fifoDev_Write(HDC dev, uint8 * pbuf, uint32 size, uint32 Mode, pFIFO_CALL_BACK pfWriteCallBack)
{
    FIFO_DEVICE_CLASS * pstFIFODev = (FIFO_DEVICE_CLASS *)dev;
    uint32 fifolen, front, back, front1, back1;

    if(pstFIFODev == NULL)
    {
        return RK_ERROR;
    }

    if(pstFIFODev->front >= pstFIFODev->TotalSize)
    {
        return RK_ERROR;
    }

    //rk_printf("fifo write");

    rkos_semaphore_take(pstFIFODev->osfifoWriteReqSem, MAX_DELAY);

    rkos_enter_critical();
    fifolen =  pstFIFODev->BlockCnt * pstFIFODev->BlockSize + 1;
    front1 = pstFIFODev->front;
    back1 = pstFIFODev->back;
    front = pstFIFODev->front % fifolen;
    back = pstFIFODev->back % fifolen;
    rkos_exit_critical();

    if((fifolen - front1 + back1 - 1) < size)
    {
        if(Mode == SYNC_MODE)
        {
            pstFIFODev->WriteStatus = DEVICE_STATUS_SYNC_PIO_WRITE;
            pstFIFODev->WriteReqSize = size;
            rkos_semaphore_take(pstFIFODev->osfifoWriteSem, MAX_DELAY);
            rkos_enter_critical();
            front = pstFIFODev->front % fifolen;
            back = pstFIFODev->back % fifolen;
            rkos_exit_critical();
            pstFIFODev->WriteStatus = DEVICE_STATUS_IDLE;
            pstFIFODev->WriteReqSize = 0;
        }
        else if(Mode == ASYNC_MODE)
        {
            pstFIFODev->WriteStatus = DEVICE_STATUS_ASYNC_PIO_WRITE;
            pstFIFODev->WriteReqSize = size;
            pstFIFODev->pfWriteCallBack = pfWriteCallBack;
            return 0;
        }
        else
        {
            rkos_semaphore_give(pstFIFODev->osfifoWriteReqSem);
            return RK_PARA_ERR;
        }
    }

    if(front >= back)
    {
        if(size <= (fifolen - front))
        {
            if(pstFIFODev->UseFile)
            {
                if(pstFIFODev->hWriteFile == NULL)
                {
                    return RK_ERROR;
                }

                FileDev_WriteFile(pstFIFODev->hWriteFile, pbuf, size);

                FileDev_SynchFileHandler(pstFIFODev->hReadFile, pstFIFODev->hWriteFile,SYNCH_TOTAL_SIZE);

                if((pstFIFODev->front + size) >= pstFIFODev->TotalSize)
                {
                    rk_printf("shut off\n");
                    FileDev_CloseFile(pstFIFODev->hWriteFile);
                    pstFIFODev->hWriteFile = NULL;
                }
            }
            else
            {
                memcpy(pstFIFODev->pbuf + front, pbuf, size);
            }
        }
        else
        {

            memcpy(pstFIFODev->pbuf + front, pbuf, fifolen - front);

            if((back - 1) < size - fifolen + front)
            {
                rkos_semaphore_give(pstFIFODev->osfifoWriteReqSem);
                return RK_ERROR;
            }
            else
            {
                memcpy(pstFIFODev->pbuf, pbuf + fifolen - front, size - fifolen + front);
            }
        }
    }
    else if((front + size) <= (back - 1))
    {
        memcpy(pstFIFODev->pbuf + front, pbuf, size);
    }
    else
    {
        rkos_semaphore_give(pstFIFODev->osfifoWriteReqSem);
        return RK_ERROR;
    }

    pstFIFODev->front += size;
    pstFIFODev->CurSize += size;

    if((pstFIFODev->CurSize >= 0x10) || (pstFIFODev->front >= pstFIFODev->TotalSize))
    {
        if(pstFIFODev->BufLevel)
        {
            pstFIFODev->BufLevel = 0;
            rkos_semaphore_give(pstFIFODev->osfifoBufLevelSem);
        }
    }

    //printf("pstFIFODev->ReadReqSize = %d f-b=%d f=%d b=%d\n",pstFIFODev->ReadReqSize,(pstFIFODev->front - pstFIFODev->back),pstFIFODev->front,pstFIFODev->back);

    if(pstFIFODev->ReadReqSize != 0)
    {
        if((pstFIFODev->front - pstFIFODev->back) >= pstFIFODev->ReadReqSize)
        {
            if(pstFIFODev->ReadStatus == DEVICE_STATUS_ASYNC_PIO_READ)
            {
                pstFIFODev->pfReadCallBack();
                pstFIFODev->ReadStatus = DEVICE_STATUS_IDLE;
                pstFIFODev->ReadReqSize = 0;
                rkos_semaphore_give(pstFIFODev->osfifoReadReqSem);
                //rkos_semaphore_give(pstFIFODev->osfifoReadSem);

            }
            else if(pstFIFODev->ReadStatus == DEVICE_STATUS_SYNC_PIO_READ)
            {
                rk_printf("c = %x", pstFIFODev->osfifoReadSem);

                pstFIFODev->ReadStatus = DEVICE_STATUS_IDLE;
                pstFIFODev->ReadReqSize = 0;

                rkos_semaphore_give(pstFIFODev->osfifoReadSem);
                //rk_printf("d");
            }
            else
            {
                pstFIFODev->ReadStatus = DEVICE_STATUS_IDLE;
                pstFIFODev->ReadReqSize = 0;
            }
        }
    }

    rkos_semaphore_give(pstFIFODev->osfifoWriteReqSem);
    return (rk_err_t)size;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
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
/*******************************************************************************
** Name: fifoDev_Create
** Input:void * arg
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_INIT_
INIT API HDC fifoDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS * pstDev;
    FIFO_DEVICE_CLASS * pstFIFODev;
    FIFO_DEV_ARG * pstFIFOArg = (FIFO_DEV_ARG *)arg;

    HDC hFileDev;
    HDC hFile;
    FILE_ATTR stFileAttr;
    rk_err_t ret;

    if (arg == NULL)
    {
        return (HDC)(RK_PARA_ERR);
    }

    pstFIFODev =  rkos_memory_malloc(sizeof(FIFO_DEVICE_CLASS));
    if (pstFIFODev == NULL)
    {
        return pstFIFODev;
    }

    if(pstFIFOArg->UseFile == 0)
    {
        pstFIFODev->pbuf = rkos_memory_malloc(pstFIFOArg->BlockCnt * pstFIFOArg->BlockSize + 1);
        if(pstFIFODev->pbuf == NULL)
        {
            return NULL;
        }
    }
    else
    {
        pstFIFODev->pbuf = 0;
    }

    pstFIFODev->osfifoReadReqSem        = rkos_semaphore_create(1,1);
    pstFIFODev->osfifoWriteReqSem       = rkos_semaphore_create(1,1);


    pstFIFODev->osfifoReadSem        = rkos_semaphore_create(1,0);
    pstFIFODev->osfifoWriteSem       = rkos_semaphore_create(1,0);
    pstFIFODev->osfifoBufLevelSem    = rkos_semaphore_create(1,0);

    if ((pstFIFODev->osfifoReadReqSem && pstFIFODev->osfifoWriteReqSem && pstFIFODev->osfifoReadSem
                && pstFIFODev->osfifoWriteSem && pstFIFODev->osfifoBufLevelSem) == 0)
    {

        rkos_semaphore_delete(pstFIFODev->osfifoReadReqSem);
        rkos_semaphore_delete(pstFIFODev->osfifoWriteReqSem);

        rkos_semaphore_delete(pstFIFODev->osfifoReadSem);
        rkos_semaphore_delete(pstFIFODev->osfifoWriteSem);
        rkos_semaphore_delete(pstFIFODev->osfifoBufLevelSem);
        rkos_memory_free(pstFIFODev);
        rkos_memory_free(pstFIFODev->pbuf);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstFIFODev;

    pstDev->suspend = NULL;
    pstDev->resume  = NULL;


    pstFIFODev->BlockSize = pstFIFOArg->BlockSize;
    pstFIFODev->BlockCnt = pstFIFOArg->BlockCnt;
    pstFIFODev->hReadFile = pstFIFOArg->hReadFile;
    pstFIFODev->hWriteFile = pstFIFOArg->hWriteFile;
    pstFIFODev->UseFile = pstFIFOArg->UseFile;
    pstFIFODev->front = 0;
    pstFIFODev->back = 0;
    pstFIFODev->ReadReqSize = 0;
    pstFIFODev->WriteReqSize = 0;
    pstFIFODev->pfReadCallBack = NULL;
    pstFIFODev->pfWriteCallBack = NULL;
    pstFIFODev->TotalSize = 0;
    pstFIFODev->CurSize = 0;

    pstFIFODev->ReadStatus = DEVICE_STATUS_IDLE;
    pstFIFODev->WriteStatus = DEVICE_STATUS_IDLE;
    pstFIFODev->BufLevel = 0;

    gpstfifoDevISR[DevID] = pstFIFODev;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_FIFO_DEV, MODULE_OVERLAY_CODE);
    #endif

    return pstDev;
}

/*******************************************************************************
** Name: fifoDevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
//_DRIVER_FIFO_FIFODEVICE_INIT_
INIT API rk_err_t fifoDev_Delete(uint32 DevID, void * arg)
{
    FIFO_DEV_ARG * pstFIFOArg = (FIFO_DEV_ARG *)arg;
    rk_printf("enter fifoDev_Delete");

    if (gpstfifoDevISR[DevID] == NULL)
    {
        rk_printf("gpstfifoDevISR NULL id=%d",DevID);
        return RK_ERROR;
    }
    //SdDev deinit...
    if(pstFIFOArg != NULL)
    {
        if(gpstfifoDevISR[DevID]->hReadFile != NULL)
        {
            pstFIFOArg->hReadFile = gpstfifoDevISR[DevID]->hReadFile;
        }
        else
        {
            pstFIFOArg->hReadFile = NULL;
        }

        if(gpstfifoDevISR[DevID]->hWriteFile != NULL)
        {
            pstFIFOArg->hWriteFile = gpstfifoDevISR[DevID]->hWriteFile;
        }
        else
        {
            pstFIFOArg->hWriteFile = NULL;
        }

    }
    rk_printf("fifoDev_Delete \n");

    rk_printf("rkos_semaphore_delete \n");

    rkos_semaphore_delete(gpstfifoDevISR[DevID]->osfifoReadReqSem);
    rkos_semaphore_delete(gpstfifoDevISR[DevID]->osfifoWriteReqSem);
    rkos_semaphore_delete(gpstfifoDevISR[DevID]->osfifoReadSem);
    rkos_semaphore_delete(gpstfifoDevISR[DevID]->osfifoWriteSem);
    rkos_semaphore_delete(gpstfifoDevISR[DevID]->osfifoBufLevelSem);
    rkos_memory_free(gpstfifoDevISR[DevID]);
    rk_printf("FW_RemoveModule \n");

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_FIFO_DEV);
    #endif
    rk_printf("fifo deinit over \n");

    return RK_SUCCESS;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#ifdef _FIFO_DEV_SHELL_
_DRIVER_FIFO_FIFODEVICE_SHELL_
static uint8* ShellfifoName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "help",
    "...",
    "\b",
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: fifoDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL API rk_err_t fifoDev_Shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);
    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellfifoName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00: //pcb
            ret = fifoDevShellPcb(dev,pItem);
            break;

        case 0x01: //mc
            ret = fifoDevShellCreate(dev,pItem);
            break;

        case 0x02://del
            ret = fifoDevShellDel(dev, pItem);
            break;

        case 0x03://test
            ret = fifoDevShellTest(dev, pItem);
            break;

        case 0x04://help
            ret = fifoDevShellHelp(dev, pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: fifoDevShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.4
** Time: 10:22:21
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL FUN rk_err_t fifoDevShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("fifo命令集提供了一系列的命令对fifo进行操作\r\n");
    rk_print_string("fifo包含的子命令如下:            \r\n");
    rk_print_string("pcb       显示pcb信息            \r\n");
    rk_print_string("mc        创建fifo device        \r\n");
    rk_print_string("del       删除fifo device命令    \r\n");
    rk_print_string("test      测试fifo device命令    \r\n");
    rk_print_string("help      显示fifo device命令帮助信息  \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: fifoDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL FUN rk_err_t fifoDevShellTest(HDC dev, uint8 * pstr)
{
    rk_err_t ret;
    HDC hFifo;
    uint32 TotalSize;
    RK_TASK_AUDIOCONTROL_ARG pArg;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("fifo.test : test cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    //FREQ_EnterModule(FREQ_MAX);

    //RKTaskCreate(TASK_ID_MEDIA, 0, NULL);
    //RKTaskCreate(TASK_ID_FILESTREAM, 0, NULL);
    //RKTaskCreate(TASK_ID_AUDIOCONTROL, 0, NULL);
    //RKTaskCreate(TASK_ID_STREAMCONTROL, 0, NULL);

    #if 1
    //ret = HttpTask_Get("http://192.168.1.101/I%20love%20you.pcm");
    //ret = HttpGet_Url("http://192.168.1.100/%E6%9C%88%E4%BA%AE%E4%B9%8B%E4%B8%8A.APE");
    //ret = HttpTask_Get("http://192.168.0.125/%E6%9C%88%E4%BA%AE%E4%B9%8B%E4%B8%8A.APE");

    pArg.ucSelPlayType = SOURRE_FROM_DLNA;
    pArg.FileNum = 1;
    //memcpy(pArg.filepath, "http://192.168.1.100/%E6%89%93%E8%80%81%E8%99%8E.MP3", 200);

    RKTaskCreate(TASK_ID_AUDIOCONTROL, 0, &pArg);
    RKTaskCreate(TASK_ID_STREAMCONTROL, 0, NULL);



    #if 1
    ret = HttpGet_Url("http://192.168.1.104/%E6%89%93%E8%80%81%E8%99%8E.MP3");

    if(ret == RK_SUCCESS)
    {
        printf("\nlocal http ok\n");
        AudioControlTask_SendCmd(AUDIO_CMD_DECSTART, NULL, ASYNC_MODE);
    }
    else
    {
        printf("\nlocal http error\n");
        return RK_SUCCESS;
    }
    #endif

    #endif

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: fifoDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL FUN rk_err_t fifoDevShellDel(HDC dev, uint8 * pstr)
{
    FIFO_DEV_ARG pstFIFOArg;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("fifo.del : del cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...
    //RKDev_Delete(DEV_CLASS_FIFO, 0);
    if (RKDev_Delete(DEV_CLASS_FIFO, 0, &pstFIFOArg) != RK_SUCCESS)
    {
        rk_printf("fifo delete failure");
        return RK_ERROR;
    }

    rk_printf("fifo delete ok");

    if(pstFIFOArg.hWriteFile != NULL)
    {
        if (FileDev_CloseFile(pstFIFOArg.hWriteFile) != RK_SUCCESS)
        {
            rk_printf("WriteFile close failure\n");
            return RK_ERROR;
        }
    }
    if(pstFIFOArg.hWriteFile != NULL)
    {
        if (FileDev_CloseFile(pstFIFOArg.hReadFile) != RK_SUCCESS)
        {
            rk_printf("ReadFile close failure\n");
            return RK_ERROR;
        }
    }
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: fifoDevShellCreate
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL FUN rk_err_t fifoDevShellCreate(HDC dev, uint8 * pstr)
{
    FIFO_DEV_ARG stFifoArg;
    rk_err_t ret;
    HDC hFileDev;
    FILE_ATTR stFileAttr;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("fifodev.create : create cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    #if 1
    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

    if((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("File device open failure");
    }

    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = L"RkFifiDevice.buf";


    stFifoArg.hReadFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);
    if(stFifoArg.hReadFile <= 0)
    {
        rk_print_string("file cache buffer open fail");
        return RK_SUCCESS;
    }

    stFifoArg.hWriteFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);
    if(stFifoArg.hReadFile <= 0)
    {
        rk_print_string("file cache buffer open fail");
        return RK_SUCCESS;
    }

    RKDev_Close(hFileDev);

    #endif

    stFifoArg.BlockCnt = 200000;
    //stFifoArg.BlockCnt = 20;
    stFifoArg.BlockSize = 1024;
    stFifoArg.UseFile = 1;

    ret = RKDev_Create(DEV_CLASS_FIFO, 0, &stFifoArg);

    if(ret != RK_SUCCESS)
    {
        rk_print_string("fifo dev create failure");
        return RK_ERROR;
    }
    rk_print_string("fifo dev create success");

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: fifoDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.27
** Time: 20:43:01
*******************************************************************************/
_DRIVER_FIFO_FIFODEVICE_SHELL_
SHELL FUN rk_err_t fifoDevShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("fifo.pcb : pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}



#endif
#endif

