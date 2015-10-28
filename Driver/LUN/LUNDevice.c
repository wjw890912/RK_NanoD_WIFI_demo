/*
********************************************************************************************
*
*          Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: ..\Driver\LUN\LUNDevice.c
* Owner: Aaron.sun
* Date: 2014.3.6
* Time: 16:05:11
* Desc: Lun Device class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.3.6     16:05:11   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_LUN_LUNDEVICE_C__

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



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _LUN_DEVICE_CLASS
{
    DEVICE_CLASS stLUNDevice;

    HDC hStorage;

    rk_err_t (* pfStorgeRead)(HDC dev, uint32 LBA, uint8* buffer, uint32 len);
    rk_err_t (* pfStorgeWrite)(HDC dev, uint32 LBA, uint8* buffer, uint32 len);

    uint32 dwStartLBA;
    uint32 dwEndLBA;
    uint32 dwLunNum;



}LUN_DEVICE_CLASS;


#ifdef ENABLE_MBR

#define PBR_OFFSET 0x40

unsigned char MBR[512] =
{
    0x33, 0xC0, 0x8E, 0xD0, 0xBC, 0x00, 0x7C, 0xFB, 0x50, 0x07, 0x50, 0x1F, 0xFC, 0xBE, 0x1B, 0x7C,
    0xBF, 0x1B, 0x06, 0x50, 0x57, 0xB9, 0xE5, 0x01, 0xF3, 0xA4, 0xCB, 0xBD, 0xBE, 0x07, 0xB1, 0x04,
    0x38, 0x6E, 0x00, 0x7C, 0x09, 0x75, 0x13, 0x83, 0xC5, 0x10, 0xE2, 0xF4, 0xCD, 0x18, 0x8B, 0xF5,
    0x83, 0xC6, 0x10, 0x49, 0x74, 0x19, 0x38, 0x2C, 0x74, 0xF6, 0xA0, 0xB5, 0x07, 0xB4, 0x07, 0x8B,
    0xF0, 0xAC, 0x3C, 0x00, 0x74, 0xFC, 0xBB, 0x07, 0x00, 0xB4, 0x0E, 0xCD, 0x10, 0xEB, 0xF2, 0x88,
    0x4E, 0x10, 0xE8, 0x46, 0x00, 0x73, 0x2A, 0xFE, 0x46, 0x10, 0x80, 0x7E, 0x04, 0x0B, 0x74, 0x0B,
    0x80, 0x7E, 0x04, 0x0C, 0x74, 0x05, 0xA0, 0xB6, 0x07, 0x75, 0xD2, 0x80, 0x46, 0x02, 0x06, 0x83,
    0x46, 0x08, 0x06, 0x83, 0x56, 0x0A, 0x00, 0xE8, 0x21, 0x00, 0x73, 0x05, 0xA0, 0xB6, 0x07, 0xEB,
    0xBC, 0x81, 0x3E, 0xFE, 0x7D, 0x55, 0xAA, 0x74, 0x0B, 0x80, 0x7E, 0x10, 0x00, 0x74, 0xC8, 0xA0,
    0xB7, 0x07, 0xEB, 0xA9, 0x8B, 0xFC, 0x1E, 0x57, 0x8B, 0xF5, 0xCB, 0xBF, 0x05, 0x00, 0x8A, 0x56,
    0x00, 0xB4, 0x08, 0xCD, 0x13, 0x72, 0x23, 0x8A, 0xC1, 0x24, 0x3F, 0x98, 0x8A, 0xDE, 0x8A, 0xFC,
    0x43, 0xF7, 0xE3, 0x8B, 0xD1, 0x86, 0xD6, 0xB1, 0x06, 0xD2, 0xEE, 0x42, 0xF7, 0xE2, 0x39, 0x56,
    0x0A, 0x77, 0x23, 0x72, 0x05, 0x39, 0x46, 0x08, 0x73, 0x1C, 0xB8, 0x01, 0x02, 0xBB, 0x00, 0x7C,
    0x8B, 0x4E, 0x02, 0x8B, 0x56, 0x00, 0xCD, 0x13, 0x73, 0x51, 0x4F, 0x74, 0x4E, 0x32, 0xE4, 0x8A,
    0x56, 0x00, 0xCD, 0x13, 0xEB, 0xE4, 0x8A, 0x56, 0x00, 0x60, 0xBB, 0xAA, 0x55, 0xB4, 0x41, 0xCD,
    0x13, 0x72, 0x36, 0x81, 0xFB, 0x55, 0xAA, 0x75, 0x30, 0xF6, 0xC1, 0x01, 0x74, 0x2B, 0x61, 0x60,
    0x6A, 0x00, 0x6A, 0x00, 0xFF, 0x76, 0x0A, 0xFF, 0x76, 0x08, 0x6A, 0x00, 0x68, 0x00, 0x7C, 0x6A,
    0x01, 0x6A, 0x10, 0xB4, 0x42, 0x8B, 0xF4, 0xCD, 0x13, 0x61, 0x61, 0x73, 0x0E, 0x4F, 0x74, 0x0B,
    0x32, 0xE4, 0x8A, 0x56, 0x00, 0xCD, 0x13, 0xEB, 0xD6, 0x61, 0xF9, 0xC3, 0x49, 0x6E, 0x76, 0x61,
    0x6C, 0x69, 0x64, 0x20, 0x70, 0x61, 0x72, 0x74, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x74, 0x61,
    0x62, 0x6C, 0x65, 0x00, 0x45, 0x72, 0x72, 0x6F, 0x72, 0x20, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E,
    0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74,
    0x65, 0x6D, 0x00, 0x4D, 0x69, 0x73, 0x73, 0x69, 0x6E, 0x67, 0x20, 0x6F, 0x70, 0x65, 0x72, 0x61,
    0x74, 0x69, 0x6E, 0x67, 0x20, 0x73, 0x79, 0x73, 0x74, 0x65, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x44, 0x63, 0xBC, 0xC3, 0x49, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x01, 0x00, 0x0C, 0xFE, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0xE2, 0x0D, 0xCF, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA
};

typedef __packed struct  _PART_TABLE_INFO
{
    uint8 bState;
    uint8 bStartHead;
    uint16 wBeginSec;
    uint8 bFsType;
    uint8 bEndHead;
    uint16 wEndSec;
    uint32 dwSecAddr;
    uint32 dwSecSize;

}PART_TABLE_INFO;

#endif




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/


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
rk_err_t LunShellDelete(HDC dev, uint8 * pstr);
rk_err_t LunShellHelp(HDC dev, uint8 * pstr);
rk_err_t LunShellTest(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: LunDev_GetParTotalCnt
** Input:HDC dev, uint32 * pParTotalCnt
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.18
** Time: 11:18:45
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_COMMON_
COMMON API rk_err_t LunDev_GetParTotalCnt(HDC dev, uint32 * pParTotalCnt)
{

}

/*******************************************************************************
** Name: LunDev_GetPar
** Input:HDC dev,  uint32 * pVolumeType, uint32 * pStartLBA, uint32 * pTotalSize
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.17
** Time: 18:12:41
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_COMMON_
COMMON API rk_err_t LunDev_GetPar(HDC dev, uint32 ParNum, uint32 * pVolumeType, uint32 * pStartLBA, uint32 * pTotalSize)
{

}


/*******************************************************************************
** Name: LunDev_GetSize
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.10
** Time: 9:55:39
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_COMMON_
COMMON API rk_err_t LunDev_GetSize(HDC dev,  uint32 * pSize)
{
    LUN_DEVICE_CLASS * pstLunDev  =  (LUN_DEVICE_CLASS *)dev;

    if (pstLunDev == NULL)
    {
        return RK_ERROR;
    }
    else
    {
        *pSize = pstLunDev->dwEndLBA - pstLunDev->dwStartLBA + 1;
        return RK_SUCCESS;
    }
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*******************************************************************************
** Name: LUNDevRead
** Input:HDC dev,uint32 pos, void* buffer, uint32 size,uint8 mode,pRx_indicate Rx_indicate
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.6
** Time: 16:08:29
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_COMMON_
rk_size_t LunDev_Read(HDC dev, uint32 LBA, uint8 * buffer, uint32 len)
{
    uint32 i;

    LUN_DEVICE_CLASS * pstLunDev  =  (LUN_DEVICE_CLASS *)dev;

    HDC hStorage;

    hStorage = pstLunDev->hStorage;

    return pstLunDev->pfStorgeRead(hStorage, LBA + pstLunDev->dwStartLBA, buffer, len);
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*******************************************************************************
** Name: LunDevWrite
** Input:HDC dev, uint32 pos, const void* buffer, uint32 size,uint8 mode,pTx_complete Tx_complete
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.6
** Time: 16:08:29
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_COMMON_
rk_size_t LunDev_Write(HDC dev, uint32 LBA, uint8* buffer, uint32 len)
{
    uint32 i;

    LUN_DEVICE_CLASS * pstLunDev  =  (LUN_DEVICE_CLASS *)dev;

    HDC hStorage;

    hStorage = pstLunDev->hStorage;

    return pstLunDev->pfStorgeWrite(hStorage, LBA + pstLunDev->dwStartLBA, buffer, len);
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: LUNDevCreate
** Input:void
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.3.6
** Time: 16:08:29
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_INIT_
HDC LunDev_Create(uint32 DevID, void * arg)
{
    LUN_DEV_ARG * pstLunArg = (LUN_DEV_ARG * )arg;
    DEVICE_CLASS* pstDev;
    LUN_DEVICE_CLASS * pstLunDev;


    if (pstLunArg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstLunDev =  rkos_memory_malloc(sizeof(LUN_DEVICE_CLASS));
    if (pstLunDev == NULL)
    {
        return pstLunDev;
    }

    pstDev = (DEVICE_CLASS *)pstLunDev;

    pstDev->suspend = NULL;
    pstDev->resume  = NULL;

    pstLunDev->hStorage = pstLunArg->hStorage;

    pstLunDev->dwStartLBA = pstLunArg->dwStartLBA;

    pstLunDev->dwEndLBA = pstLunArg->dwEndLBA;

    pstLunDev->pfStorgeRead = pstLunArg->pfStorgeRead;
    pstLunDev->pfStorgeWrite = pstLunArg->pfStorgeWrite;

    return pstLunDev;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: LUNDev_Delete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.6
** Time: 16:08:29
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_INIT_
rk_err_t LunDev_Delete(uint32 DevID, void * arg)
{

#if 0
    LUN_DEVICE_CLASS * pstLunDev = (LUN_DEVICE_CLASS *)dev;

    RKDev_Close(pstLunDev->hStorage);

    rkos_memory_free(dev);
#endif

    return RK_SUCCESS;
}


#ifdef _USE_SHELL_
_DRIVER_LUN_LUNDEVICE_SHELL_
static uint8* ShellLunName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "help",
    "\b",                          // the end
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: LunShell
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
rk_err_t LunDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellLunName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:
            ret = LunShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = LunShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = LunShellDelete(dev,pItem);
            break;

        case 0x03:
            ret = LunShellTest(dev,pItem);
            break;

        case 0x04:  //help
            ret = LunShellHelp(dev,pItem);
            break;

        case 0x05:
            //ret = ;
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
** Name: LunShellDelete
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.18
** Time: 9:21:10
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
SHELL FUN rk_err_t LunShellDelete(HDC dev, uint8 * pstr)
{
    HDC hStorage, hLun;
    LUN_DEV_ARG stLunArg;
    DEVICE_CLASS * pDev;
    rk_err_t ret;
    uint32 StorageSise;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("lun.delete : lun delete cmd.\r\n");
            return RK_SUCCESS;
        }
    }
    pstr++;
#endif

    if(RKDev_Delete(DEV_CLASS_LUN, 4, NULL) != RK_SUCCESS)
    {
        rk_print_string("lun4 delete failure");
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: LunShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 15:27:05
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
SHELL FUN rk_err_t LunShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("lun命令集提供了一系列的命令对lun进行操作\r\n");
    rk_print_string("lun包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息          \r\n");
    rk_print_string("mc        创建                 \r\n");
    rk_print_string("test      测试                 \r\n");
    rk_print_string("help      显示lun命令帮助信息  \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: LunShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.5
** Time: 17:41:23
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
SHELL FUN rk_err_t LunShellTest(HDC dev, uint8 * pstr)
{
    HDC hLun;
    uint32 TotalSec;
    uint8 * pBuf;
    int32 BufSec;
    uint32 i, j;
    rk_err_t ret;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("lun.test : lun test.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    hLun = RKDev_Open(DEV_CLASS_LUN, 3, NOT_CARE);
    if(hLun == NULL)
    {
        rk_print_string("LUN 3 device open failure");
        return RK_SUCCESS;
    }

    for(BufSec = 128; BufSec > 0; BufSec--)
    {
        pBuf = rkos_memory_malloc(512 * BufSec);

        if(pBuf != NULL)
        {
            break;
        }
    }

    if(BufSec <= 0)
    {
        return RK_ERROR;
    }

    TotalSec = 0x32000; // 100M

    rk_printf("test start = %d\n", BufSec);

    for(i = 0;  i < (TotalSec / BufSec) * BufSec; i += BufSec)
    {
        /*
        pBuf[0] = i % 256;
        pBuf[1] = i % 256;

        for(j = 2; j < BufSec * 512; j++)
            pBuf[j] = pBuf[j - 1] + pBuf[j - 2];
        */

        ret = LunDev_Write(hLun, i, pBuf, BufSec);

        rk_printf("write: LBA = 0x%08x, Len = %d", i, BufSec);

        if(ret != BufSec)
        {
            rk_print_string("Lun write error");
        }
    }

    if(TotalSec % BufSec != 0)
    {
        /*

        pBuf[0] = i % 256;
        pBuf[1] = i % 256;

        for(j = 2; j < BufSec * 512; j++)
            pBuf[j] = pBuf[j - 1] + pBuf[j - 2];

        */

        ret = LunDev_Write(hLun, i, pBuf, TotalSec % BufSec);

        //rk_printf("write: LBA = 0x%08x, Len = %d", i, TotalSec % BufSec);

        if(ret != TotalSec % BufSec)
        {
            rk_print_string("Lun write error");
        }
    }

    for(i = 1;  i < (TotalSec / BufSec) * BufSec; i += BufSec)
    {

        ret = LunDev_Read(hLun, i - 1, pBuf, BufSec);

        rk_printf("read: LBA = 0x%08x, Len = %d", i, BufSec);

        if(ret != BufSec)
        {
            rk_print_string("lun read error");
        }

        if((pBuf[0] != i % 256) || (pBuf[1] != i % 256))
        {
             rk_print_string("lun data error");
        }


        for(j = 2; j < BufSec * 512; j++)
        {
            if(pBuf[j] != (uint8)(pBuf[j - 1] + pBuf[j - 2]))
            {
                 rk_print_string("lun data error");
            }
        }

    }

    if(TotalSec % BufSec != 0)
    {

        ret = LunDev_Read(hLun, i - 1, pBuf, TotalSec % BufSec);

        rk_printf("read: LBA = 0x%08x, Len = %d", i, TotalSec % BufSec);

        if(ret != TotalSec % BufSec)
        {
            rk_print_string("lun read error");
        }

        if((pBuf[0] != i % 256) || (pBuf[1] != i % 256))
        {
             rk_print_string("lun data error");
        }


        for(j = 2; j < (TotalSec % BufSec) * 512; j++)
        {
            if(pBuf[j] != (uint8)(pBuf[j - 1] + pBuf[j - 2]))
            {
                 rk_print_string("lun data error");
            }
        }

    }

    rk_printf("test end");

    rkos_memory_free(pBuf);
}


/*******************************************************************************
** Name: EmmcShellPcb
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
rk_err_t LunShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("lun.pcb : lun pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_LUN_LUNDEVICE_SHELL_
rk_err_t LunShellCreate(HDC dev, uint8 * pstr)
{
    HDC hStorage, hLun;
    LUN_DEV_ARG stLunArg;
    DEVICE_CLASS * pDev;
    rk_err_t ret;
    uint32 StorageSise;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("lun.mc : lun mc cmd.\r\n");
            return RK_SUCCESS;
        }
    }
    pstr++;
#endif

    if(StrCmpA(pstr, "/emmc", 5) == 0)
    {
        #ifdef __DRIVER_EMMC_EMMCDEVICE_C__
        hStorage = RKDev_Open(DEV_CLASS_EMMC,0, NOT_CARE);
        if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
        {
            rk_print_string("emmc open failure");
            return RK_SUCCESS;
        }

        ret = EmmcDev_SetArea(hStorage, EMMC_AREA_USER);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("emmc set user area failure");
            return RK_SUCCESS;
        }

        StorageSise = 0;

        ret = EmmcDev_GetAreaSize(hStorage, EMMC_AREA_USER, &StorageSise);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("emmc get user area size failure");
            return RK_SUCCESS;
        }

        #ifdef _EMMC_BOOT_
        stLunArg.dwStartLBA = (EMMC_LUN0_SIZE + EMMC_LUN1_SIZE) << 11;
        #else
        stLunArg.dwStartLBA = 0;
        #endif

        stLunArg.dwEndLBA = StorageSise - 1;
        stLunArg.hStorage = hStorage;
        stLunArg.pfStorgeRead = EmmcDev_Read;
        stLunArg.pfStorgeWrite = EmmcDev_Write;

        ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("LUN0 Create Failure");
        }
        #endif

    }
    else if(StrCmpA(pstr, "/spi", 4) == 0)
    {
       #ifdef __DRIVER_SPINOR_SPINORDEVICE_C__

        hStorage = RKDev_Open(DEV_CLASS_SPINOR,0, NOT_CARE);
        if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
        {
            rk_print_string("spi nor open failure");
            return RK_SUCCESS;
        }

        ret = SpiNorDev_GetSize(hStorage, &stLunArg.dwEndLBA);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("spi nor get size failure");
            return RK_SUCCESS;
        }

        stLunArg.dwStartLBA = 0;
        stLunArg.dwEndLBA--;
        stLunArg.hStorage = hStorage;
        stLunArg.pfStorgeRead = SpiNorDev_Read;
        stLunArg.pfStorgeWrite = SpiNorDev_Write;

        ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("LUN0 Create Failure");
        }

      #endif

    }
    else if(StrCmpA(pstr, "/sd", 3) == 0)
    {
        #ifdef __DRIVER_SD_SDDEVICE_C__

        hStorage = RKDev_Open(DEV_CLASS_SD,0, NOT_CARE);
        if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
        {
            rk_print_string("sd open failure");
            return RK_SUCCESS;
        }

        ret = SdDev_GetSize(hStorage, &stLunArg.dwEndLBA);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("sd get size failure");
            return RK_SUCCESS;
        }

        stLunArg.dwStartLBA = 0;
        stLunArg.dwEndLBA--;
        stLunArg.hStorage = hStorage;
        stLunArg.pfStorgeRead = SdDev_Read;
        stLunArg.pfStorgeWrite = SdDev_Write;

        ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("LUN0 Create Failure");
        }

      #endif

    }
    else if(StrCmpA(pstr, "/usb", 4) == 0)
    {
        #ifdef __DRIVER_USBMSC_USBMSCDEVICE_C__
        hStorage = RKDev_Open(DEV_CLASS_USBMSC, 0, NOT_CARE);
        if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
        {
            rk_print_string("lun usbmsc open failure");
            return RK_SUCCESS;
        }
        stLunArg.dwStartLBA = 0;
        stLunArg.dwEndLBA--;
        stLunArg.hStorage = hStorage;
        stLunArg.pfStorgeRead = USBMSCHost_Read;
        stLunArg.pfStorgeWrite = USBMSCHost_Write;

        ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

        if (ret != RK_SUCCESS)
        {
            rk_print_string("LUN0 Create Failure");
        }
        rk_print_string("USB LUN0 Create Success");
        #endif
    }
    else
    {
        return RK_ERROR;
    }

    return RK_SUCCESS;

}

#endif


#endif

