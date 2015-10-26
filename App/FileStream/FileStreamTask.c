/*
********************************************************************************************
*
*        Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: ..\App\FileStream\FileStreamTask.c
* Owner: Aaron.sun
* Date: 2014.5.7
* Time: 17:43:17
* Desc: File Stream
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.5.7     17:43:17   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __APP_FILESTREAM_FILESTREAMTASK_C__

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
#include "AppInclude.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef struct _FILESTREAM_TASK_DATA_BLOCK
{

    uint8 * buf;
    uint32 len;
    uint32 front;
    uint32 back;
    uint32 ShuffleMode;
    uint32 LoopMode;

    pQueue  FileStreamAskQueue;
    pQueue  FileStreamRespQueue;
    pSemaphore FileSycnSem;
    uint32 FileSycnCmd;
    uint32 TaskObjectID;

}FILESTREAM_TASK_DATA_BLOCK;

typedef  struct _FILESTREAM_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}FILESTREAM_RESP_QUEUE;

typedef  struct _FILESTREAM_ASK_QUEUE
{
    uint32 cmd;

}FILESTREAM_ASK_QUEUE;


typedef struct _FILE_READ_st
{
    uint8 * buf;
    uint32  blocksize;

}F_READ_PARAM,*pF_READ_PARAM;

typedef struct _HIFI_FILE_SEEK_st
{
    uint8 Whence;
    int32 SecNum;

}HF_SEEK_PARAM,*pHF_SEEK_PARAM;


uint32 gReadCmd;
uint32 gReadData;
F_READ_PARAM gReadParam;

uint32 gHifiFlag1;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static FILESTREAM_TASK_DATA_BLOCK * gpstFileStreamDataBlock;



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
rk_err_t FileStreamTaskShellHelp(HDC dev, uint8 * pstr);
rk_err_t FileStreamTaskShellTest(HDC dev, uint8 * pstr);
rk_err_t FileStreamTaskShellDel(HDC dev, uint8 * pstr);
rk_err_t FileStreamTaskShellMc(HDC dev, uint8 * pstr);
rk_err_t FileStreamTaskShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: FileStreamTask_Enter
** Input:void
** Return: void
** Owner:Aaron.sun
** Date: 2014.5.26
** Time: 19:17:01
*******************************************************************************/
_APP_FILESTREAM_FILESTREAMTASK_COMMON_
COMMON API void FileStreamTask_Enter(void)
{
    HDC hFifo, hFileDev;
    HDC hFile;
    FILE_ATTR stFileAttr;
    rk_err_t ret;

    uint8 Buf[4096];
    uint32 Bank, LBA;


    hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, NOT_CARE);
    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);
    rk_print_string("FileStreamTask_Enter");
    if ((hFifo == NULL) || (hFifo == (HDC)RK_ERROR) || (hFifo == (HDC)RK_PARA_ERR))
    {
        rk_print_string("fifo device open failure");
        while(1);
    }

    if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("File device open failure");
        RKDev_Close(hFifo);
        while(1);
    }


    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = L"I love you.pcm";
    //stFileAttr.FileName = L"44.1_2000_16ÑÌ»¨Ò×Àä3990.ape";

    hFile = FileDev_OpenFile(hFileDev, NULL, READ_ONLY, &stFileAttr);
    if((rk_err_t)hFile <= 0)
    {
        rk_print_string("File Open Failure");

        RKDev_Close(hFifo);
        RKDev_Close(hFileDev);
        while(1);
    }

    rk_printf("!!!!stFileAttr.FileSize = 0x%08x!!!\n",stFileAttr.FileSize);

    fifoDev_SetTotalSize(hFifo, stFileAttr.FileSize);

    LBA = 0;

    while(FileDev_ReadFile(hFile, Buf,  8192) == 8192)
    {
        LBA += 1;
        //rk_printf("LBA = %d", LBA);
        ret = fifoDev_Write(hFifo, Buf, 4096, SYNC_MODE, NULL);

        if(ret != 4096)
        {
            goto over;
        }

    }
    rk_print_string("file end");

over:

    FileDev_CloseFile(hFile);
    RKDev_Close(hFifo);
    RKDev_Close(hFileDev);
    RKTaskDelete(TASK_ID_FILESTREAM, gpstFileStreamDataBlock->TaskObjectID, ASYNC_MODE);

    while(1);

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
** Name: FileStreamTask_DevInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.26
** Time: 19:17:01
*******************************************************************************/
_APP_FILESTREAM_FILESTREAMTASK_INIT_
INIT API rk_err_t FileStreamTask_DevInit(void *pvParameters)
{
    rkos_queue_delete(gpstFileStreamDataBlock->FileStreamAskQueue);
    rkos_queue_delete(gpstFileStreamDataBlock->FileStreamRespQueue);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FileStreamTask_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.26
** Time: 19:17:01
*******************************************************************************/
_APP_FILESTREAM_FILESTREAMTASK_INIT_
INIT API rk_err_t FileStreamTask_Init(void *pvParameters)
{
    RK_TASK_CLASS*   pDeviceTask = (RK_TASK_CLASS*)pvParameters;
    FILESTREAM_TASK_DATA_BLOCK*  pFileStreamDataBlock;

    uint32 i;

    if(pDeviceTask == NULL)
        return RK_PARA_ERR;


    pFileStreamDataBlock = rkos_memory_malloc(sizeof(FILESTREAM_TASK_DATA_BLOCK));
    if(pFileStreamDataBlock == NULL)
    {
        return RK_ERROR;
    }

    memset(pFileStreamDataBlock, 0, sizeof(FILESTREAM_TASK_DATA_BLOCK));


    pFileStreamDataBlock->FileStreamAskQueue = rkos_queue_create(1, sizeof(FILESTREAM_ASK_QUEUE));
    pFileStreamDataBlock->FileStreamRespQueue = rkos_queue_create(1, sizeof(FILESTREAM_RESP_QUEUE));
    pFileStreamDataBlock->TaskObjectID = pDeviceTask->TaskObjectID;

    pFileStreamDataBlock->FileSycnSem = rkos_semaphore_create(1, 0);
    gpstFileStreamDataBlock = pFileStreamDataBlock;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_FILESTREAM_TASK, MODULE_OVERLAY_ALL);
    #endif

    return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#endif

