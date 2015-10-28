/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\SpiNor\SpiNorDevice.c
* Owner: Aaron.sun
* Date: 2014.5.30
* Time: 9:08:13
* Desc: SPI NOR DEVICE CLASS
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    Aaron.sun     2014.5.30     9:08:13   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_SPINOR_SPINORDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _SPINOR_DEVICE_CLASS
{
    DEVICE_CLASS stSpiNorDevice;
    pSemaphore osSpiNorOperReqSem;
    HDC hSpi;
    uint32 SpiChannel;

    uint32 Manufacturer;
    uint32 MemType;
    uint32 PageSize;
    uint32 BlockSize;
    uint32 capacity;
    uint32 DummySize;

}SPINOR_DEVICE_CLASS;



#define SPI_NOR_BUS_CLK (12 * 1000 * 1000)

#define SPI_NOR_CTL_MODE (SPI_MASTER_MODE | TRANSMIT_RECEIVE | MOTOROLA_SPI | RXD_SAMPLE_NO_DELAY | APB_BYTE_WR \
                            | MSB_FBIT | LITTLE_ENDIAN_MODE | CS_2_SCLK_OUT_1_CK | CS_KEEP_LOW | SERIAL_CLOCK_POLARITY_HIGH \
                            | SERIAL_CLOCK_PHASE_START | DATA_FRAME_8BIT)



#define JEDECID_WINBOND     0xEF
#define JEDECID_ST          0x20
#define JEDECID_SPANSION    0x01
#define JEDECID_CFEON       0x1C


#define CMD_READ_JEDECID        (0x9F)
#define CMD_READ_DATA           (0x03)
#define CMD_READ_STATUS         (0x05)
#define CMD_WRITE_STATUS        (0x01)
#define CMD_PAGE_PROG           (0x02)
#define CMD_SECTOR_ERASE        (0x20)
#define CMD_BLOCK_ERASE         (0xD8)
#define CMD_CHIP_ERASE          (0xC7)
#define CMD_WRITE_EN            (0x06)
#define CMD_WRITE_DIS           (0x04)


#define NOR_PAGE_SIZE           256
#define NOR_BLOCK_SIZE          (64*1024)
#define NOR_SEC_PER_BLK         (NOR_BLOCK_SIZE/512)

#define SPI_CTRL_CONFIG         (SPI_MASTER_MODE|MOTOROLA_SPI|RXD_SAMPLE_NO_DELAY|APB_BYTE_WR|MSB_FBIT|LITTLE_ENDIAN_MODE|CS_2_SCLK_OUT_1_CK|CS_KEEP_LOW|SERIAL_CLOCK_POLARITY_HIGH|SERIAL_CLOCK_PHASE_START|DATA_FRAME_8BIT)
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static SPINOR_DEVICE_CLASS * gpstSpiNorDevISR[SPI_DEV_MAX_CNT];

_DRIVER_SPINOR_SPINORDEVICE_INIT_
const uint8 NorDeviceCode[] =
{
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x17,
    0x18,
    0x19
};

_DRIVER_SPINOR_SPINORDEVICE_INIT_
const uint32 NorMemCapacity[] =
{
    0x20000,        //128k-byte
    0x40000,        //256k-byte
    0x80000,        //512k-byte
    0x100000,       // 1M-byte
    0x200000,       // 2M-byte
    0x400000,       // 4M-byte
    0x800000,       // 8M-byte
    0x1000000,      //16M-byte
    0x2000000       // 32M-byte
};

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
rk_err_t SpiNorDevShellHelp(HDC dev,  uint8 * pstr);
rk_err_t SPINorBlkErase(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint32 LBA);
rk_err_t SpiNorWrite(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint32 LBA, uint8 * pBuf, uint32 Len);
rk_err_t SPINorWaitBusy(SPINOR_DEVICE_CLASS * pstSpiNorDev);
rk_err_t SPIProgCmd(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint8 * pCmd, uint32 len,  uint8 * pData,  uint32 PageSize);
rk_err_t SpiSendCmd(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint8 *pCmd, uint32 CmdLen, uint8 *pData, uint32 size);
rk_err_t SpiNorDevShellTest(HDC dev, uint8 * pstr);
rk_err_t SpiNorDevShellDel(HDC dev, uint8 * pstr);
rk_err_t SpiNorDevShellMc(HDC dev, uint8 * pstr);
rk_err_t SpiNorDevShellPcb(HDC dev, uint8 * pstr);
void SpiNorDevIntIsr(void);
rk_err_t SpiNorDevDelete(HDC dev);
rk_err_t SpiNorDevDeInit(SPINOR_DEVICE_CLASS * pstSpiNorDev);
rk_err_t SpiNorDevInit(SPINOR_DEVICE_CLASS * pstSpiNorDev);
rk_err_t SpiNorDevResume(HDC dev);
rk_err_t SpiNorDevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SpiNorDev_GetSize
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.25
** Time: 10:46:36
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON API rk_err_t SpiNorDev_GetSize(HDC dev, uint32 * Size)
{
    SPINOR_DEVICE_CLASS * pstSpiNorDev = (SPINOR_DEVICE_CLASS *)dev;
    if (pstSpiNorDev == NULL)
    {
        return RK_ERROR;
    }

    *Size = pstSpiNorDev->capacity;
    return RK_SUCCESS;
}


/*******************************************************************************
** Name: SpiNorDev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON API rk_err_t SpiNorDev_Read(HDC dev, uint32 LBA, uint8 * pBuf, uint32 Len)
{
    SPINOR_DEVICE_CLASS * pstSpiNorDev = (SPINOR_DEVICE_CLASS *)dev;
    rk_err_t ret;
    uint32 Addr;
    uint8 cmd[4];

    Addr = LBA << 9;
    cmd[0] = CMD_READ_DATA;
    cmd[1] = (Addr >> 16) & 0xFF;
    cmd[2] = (Addr >> 8) & 0xFF;
    cmd[3] = (Addr & 0xFF);

    rkos_semaphore_take(pstSpiNorDev->osSpiNorOperReqSem, MAX_DELAY);
    ret = SpiSendCmd(pstSpiNorDev, cmd, 4, pBuf, Len * 512);
    rkos_semaphore_give(pstSpiNorDev->osSpiNorOperReqSem);


    if ((uint32)ret != (Len * 512))
    {
        return RK_ERROR;
    }

    return (rk_err_t) (Len);
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SpiSendCmd
** Input:uint32 *pCmd, uint32 CmdLen, uint8 *pData, uint32 size
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 19:13:13
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN rk_err_t SpiSendCmd(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint8 *pCmd, uint32 CmdLen, uint8 *pData, uint32 size)
{
    rk_err_t ret;

    SpiEanbleChannel(pstSpiNorDev->SpiChannel);

    ret = SpiDev_Write(pstSpiNorDev->hSpi, pCmd, CmdLen, SYNC_MODE, NULL);
    if ((uint32)ret != CmdLen)
    {
        SpiDisableChannel(pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }


    if (size == 0)
    {
        SpiDisableChannel(pstSpiNorDev->SpiChannel);
        return 0;
    }

    ret = SpiDev_Read(pstSpiNorDev->hSpi,pData, size, pstSpiNorDev->DummySize, SYNC_MODE, NULL);
    if ((uint32)ret != size)
    {
        SpiDisableChannel(pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    SpiDisableChannel(pstSpiNorDev->SpiChannel);
    return ret;

}

/*******************************************************************************
** Name: SpiNorDevIntIsr
** Input:void
** Return: void
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN void SpiNorDevIntIsr(void)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SpiNorDev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON API rk_err_t SpiNorDev_Write(HDC dev, uint32 LBA, uint8 * pBuf, uint32 Len)
{

    SPINOR_DEVICE_CLASS * pstSpiNorDev = (SPINOR_DEVICE_CLASS *)dev;

    rk_err_t ret;
    uint32 SecCnt,len, BlkOffset;


    rkos_semaphore_take(pstSpiNorDev->osSpiNorOperReqSem, MAX_DELAY);

    BlkOffset = LBA & (NOR_SEC_PER_BLK - 1);
    SecCnt = Len;

    if (BlkOffset)
    {
        len = NOR_SEC_PER_BLK-BlkOffset;
        len = (SecCnt>len)? len : SecCnt;
        ret = SpiNorWrite(pstSpiNorDev, LBA, pBuf, len);
        if ((uint32)ret != len)
        {
            rkos_semaphore_give(pstSpiNorDev->osSpiNorOperReqSem);
            return RK_ERROR;
        }
        LBA += len;
        SecCnt -= len;
        pBuf += (len * 512);
    }

    while (SecCnt)
    {
        len = (Len > NOR_SEC_PER_BLK)? NOR_SEC_PER_BLK : SecCnt;

        ret = SPINorBlkErase(pstSpiNorDev, LBA);

        if (RK_SUCCESS != ret)
        {
            rkos_semaphore_give(pstSpiNorDev->osSpiNorOperReqSem);
            return RK_ERROR;
        }


        ret = SpiNorWrite(pstSpiNorDev, LBA, pBuf, len);
        if ((uint32)ret != len)
        {
            rkos_semaphore_give(pstSpiNorDev->osSpiNorOperReqSem);
            return RK_ERROR;
        }


        LBA += len;
        SecCnt -= len;
        pBuf += (len * 512);

    }

    rkos_semaphore_give(pstSpiNorDev->osSpiNorOperReqSem);

    return(rk_err_t)Len;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SPINorBlkErase
** Input:SPINOR_DEVICE_CLASS * pstSpiNorDev, uint32 LBA,
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.4
** Time: 11:33:38
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN rk_err_t SPINorBlkErase(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint32 LBA)
{
    rk_err_t ret;
    uint8 cmd[4];
    uint32 blk;

    blk = LBA / NOR_SEC_PER_BLK;
    cmd[0] = CMD_WRITE_EN;

    blk = blk * NOR_BLOCK_SIZE;

    ret = SpiSendCmd(pstSpiNorDev, cmd, 1, NULL, 0);
    if ((uint32)ret != 0)
    {
        return RK_ERROR;
    }

    cmd[0] = CMD_BLOCK_ERASE;
    cmd[1] = (blk >> 16) & 0xFF;
    cmd[2] = (blk >> 8) & 0xFF;
    cmd[3] = (blk & 0xFF);

    ret = SpiSendCmd(pstSpiNorDev, cmd, 4, NULL, 0);
    if ((uint32)ret != 0)
    {
        return RK_ERROR;
    }

    ret = SPINorWaitBusy(pstSpiNorDev);

    return ret;

}

/*******************************************************************************
** Name: SpiNorWrite
** Input:uint32 LBA, uint8 * pBuf, uint32 Len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.4
** Time: 9:16:01
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN rk_err_t SpiNorWrite(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint32 LBA, uint8 * pBuf, uint32 Len)
{
    rk_err_t ret;
    uint8 cmd[4];
    uint32 PageSize, nPage;
    uint32 Addr, size;

    PageSize = pstSpiNorDev->PageSize;
    Addr = LBA << 9;
    size = Len << 9;

    if (Addr & (PageSize - 1))   //地址必须是PAGE对齐
    {
        return RK_ERROR;
    }

    nPage = size/PageSize;

    while (nPage)
    {

        cmd[0] = CMD_WRITE_EN;
        ret = SpiSendCmd(pstSpiNorDev, cmd, 1, NULL, 0);
        if (ret != 0)
        {
            return RK_ERROR;
        }

        cmd[0] = CMD_PAGE_PROG;
        cmd[1] = (Addr >> 16) & 0xFF;
        cmd[2] = (Addr >> 8) & 0xFF;
        cmd[3] = (Addr & 0xFF);

        SPIProgCmd(pstSpiNorDev, cmd, 4, pBuf, PageSize);

        ret = SPINorWaitBusy(pstSpiNorDev);
        if (RK_SUCCESS != ret)
        {
            return RK_ERROR;
        }
        Addr += PageSize;
        pBuf += PageSize;
        nPage--;

    }

    return (rk_err_t)Len;


}


/*******************************************************************************
** Name: SPINorWaitBusy
** Input:void
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 19:59:29
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN rk_err_t SPINorWaitBusy(SPINOR_DEVICE_CLASS * pstSpiNorDev)
{
    uint8 cmd[1];
    uint8 status;
    uint32 i;

    cmd[0] = CMD_READ_STATUS;

    for (i = 0; i < 200000; i++) // 2000ms
    {
        SpiSendCmd(pstSpiNorDev, cmd, 1, &status, 1);
        if ((status & 0x01) == 0)		//busy pin
        {
            return RK_SUCCESS;
        }
        DelayUs(10);
    }

    return RK_ERROR;

}

/*******************************************************************************
** Name: SPIProgCmd
** Input:SPINOR_DEVICE_CLASS * pstSpiNorDev, uint8 * pCmd, uint32 len,  uint8 * pData,  uint32 PageSize
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 19:51:01
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_COMMON_
COMMON FUN rk_err_t SPIProgCmd(SPINOR_DEVICE_CLASS * pstSpiNorDev, uint8 * pCmd, uint32 CmdLen,  uint8 * pData,  uint32 Size)
{
    rk_err_t ret;

    SpiEanbleChannel(pstSpiNorDev->SpiChannel);

    ret = SpiDev_Write(pstSpiNorDev->hSpi, pCmd, CmdLen, SYNC_MODE, NULL);
    if ((uint32)ret != CmdLen)
    {
        SpiDisableChannel(pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    ret = SpiDev_Write(pstSpiNorDev->hSpi, pData, Size, SYNC_MODE, NULL);
    if ((uint32)ret != Size)
    {
        SpiDisableChannel(pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    SpiDisableChannel(pstSpiNorDev->SpiChannel);
    return ret;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SpiNorDev_Create
** Input:void * arg
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT API HDC SpiNorDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS * pstDev;
    SPINOR_DEVICE_CLASS * pstSpiNorDev;
    SPINOR_DEV_ARG * pstSpiNorArg = (SPINOR_DEV_ARG *)arg;

    if (arg == NULL)
    {
        return (HDC)(RK_PARA_ERR);
    }

    pstSpiNorDev =  rkos_memory_malloc(sizeof(SPINOR_DEVICE_CLASS));
    if (pstSpiNorDev == NULL)
    {
        return pstSpiNorDev;
    }

    pstSpiNorDev->osSpiNorOperReqSem        = rkos_semaphore_create(1,1);


    if (pstSpiNorDev->osSpiNorOperReqSem == 0)
    {

        rkos_semaphore_delete(pstSpiNorDev->osSpiNorOperReqSem);

        rkos_memory_free(pstSpiNorDev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstSpiNorDev;

    pstDev->suspend = SpiNorDevSuspend;
    pstDev->resume  = SpiNorDevResume;

    pstSpiNorDev->hSpi = pstSpiNorArg->hSpi;
    pstSpiNorDev->SpiChannel = pstSpiNorArg->SpiChannel;

    gpstSpiNorDevISR[DevID] = pstSpiNorDev;

    if (SpiNorDevInit(pstSpiNorDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstSpiNorDev->osSpiNorOperReqSem);
        rkos_memory_free(pstSpiNorDev);
        return (HDC) RK_ERROR;
    }

    return pstDev;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: SpiNorDevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT FUN rk_err_t SpiNorDevDelete(HDC dev)
{

}
/*******************************************************************************
** Name: SpiNorDevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT FUN rk_err_t SpiNorDevDeInit(SPINOR_DEVICE_CLASS * pstSpiNorDev)
{

}
/*******************************************************************************
** Name: SpiNorDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT FUN rk_err_t SpiNorDevInit(SPINOR_DEVICE_CLASS * pstSpiNorDev)
{
    int32 i;
    uint8 cmd[4];
    uint8 data[4];
    rk_err_t ret;


    SpiDev_SetChannel(pstSpiNorDev->hSpi, pstSpiNorDev->SpiChannel, SPI_NOR_BUS_CLK, SPI_NOR_CTL_MODE);

    SpiDisableChannel(pstSpiNorDev->SpiChannel);

    pstSpiNorDev->DummySize = 0;

    cmd[0] = CMD_READ_JEDECID;

    if (3 != SpiSendCmd(pstSpiNorDev, cmd, 1, data, 3))
    {
        SpiDev_Realse_Channel(pstSpiNorDev->hSpi, pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    if ((0xFF==data[0] && 0xFF==data[1]) || (0x00==data[0] && 0x00==data[1]))
    {
        SpiDev_Realse_Channel(pstSpiNorDev->hSpi, pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    pstSpiNorDev->BlockSize = NOR_BLOCK_SIZE;
    pstSpiNorDev->PageSize = NOR_PAGE_SIZE;
    pstSpiNorDev->Manufacturer = data[0];
    pstSpiNorDev->MemType = data[1];

    for (i=0; i<sizeof(NorDeviceCode); i++)
    {
        if (data[2] == NorDeviceCode[i])
        {
            pstSpiNorDev->capacity = NorMemCapacity[i] >> 9;
            rk_printf("total sec = %d", pstSpiNorDev->capacity);
            break;
        }
    }

    if (i >= sizeof(NorDeviceCode))
    {
        SpiDev_Realse_Channel(pstSpiNorDev->hSpi, pstSpiNorDev->SpiChannel);
        return RK_ERROR;
    }

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: SpiNorDevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT FUN rk_err_t SpiNorDevResume(HDC dev)
{

}
/*******************************************************************************
** Name: SpiNorDevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_INIT_
INIT FUN rk_err_t SpiNorDevSuspend(HDC dev)
{

}


#ifdef _SPINOR_DEV_SHELL_
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
static uint8* ShellSpiNorName[] =
{
    "pcb",
    "mc",
    "del",
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
** Name: SpiNorDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL API rk_err_t SpiNorDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellSpiNorName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = SpiNorDevShellPcb(dev,pItem);
            break;

        case 0x01:  //mc
            ret = SpiNorDevShellMc(dev,pItem);
            break;

        case 0x02:  //del
            ret = SpiNorDevShellDel(dev, pItem);
            break;

        case 0x03:  //test
            ret = SpiNorDevShellTest(dev, pItem);
            break;

        case 0x04:  //help
            ret = SpiNorDevShellHelp(dev, pItem);
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
** Name: SpiNorDevShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.3
** Time: 17:17:15
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL FUN rk_err_t SpiNorDevShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if ( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("SpiNorDev命令集提供了一系列的命令对SpiNorDev进行操作\r\n");
    rk_print_string("SpiNorDev包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("mc        创建SpiNorDev        \r\n");
    rk_print_string("del       删除SpiNorDev        \r\n");
    rk_print_string("test      测试SpiNorDev命令    \r\n");
    rk_print_string("help      显示SpiNorDev命令帮助信息  \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: SpiNorDevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL FUN rk_err_t SpiNorDevShellTest(HDC dev, uint8 * pstr)
{
#if 0
    HDC hSpiNor;
    uint8 Buf[512];
    uint32 i;
    rk_err_t ret;

    hSpiNor = RKDev_Open(DEV_CLASS_SPINOR, 0, READ_WRITE_CTRL);

    if ((hSpiNor == NULL) || (hSpiNor == (HDC)RK_ERROR) || (hSpiNor == (HDC)RK_PARA_ERR))
    {
        rk_print_string("SPI Nor Device Open Failure");
        return RK_SUCCESS;
    }

    Buf[0] = 1;
    Buf[1] = 1;

    for (i = 2; i < 512; i++)
    {
        Buf[i] = Buf[i - 1] + Buf[i - 2];
    }

    // memset(Buf, 1, 512);

    ret = SpiNorDev_Write(hSpiNor, 512, Buf, 1);

    if ((uint32)ret != 1)
    {
        rk_print_string("SPI Nor Device Write Failure");
        RKDev_Close(hSpiNor);
        return RK_SUCCESS;
    }

    memset(Buf, 0, 512);

    ret = SpiNorDev_Read(hSpiNor, 512, Buf, 1);

    if ((uint32)ret != 1)
    {
        rk_print_string("SPI Nor Device Read Failure");
        RKDev_Close(hSpiNor);
        return RK_SUCCESS;
    }
#else

    HDC hSpiNor, hFileDev;
    HDC hFile;
    FILE_ATTR stFileAttr;
    rk_err_t ret;

    uint8 Buf[512];
    uint8 Buf2[512];

    uint32 Bank, LBA;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("spinor.test : test cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    hSpiNor = RKDev_Open(DEV_CLASS_SPINOR, 0, NOT_CARE);

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, NOT_CARE);
#else
    hFileDev = 0;
#endif

    if ((hSpiNor == NULL) || (hSpiNor == (HDC)RK_ERROR) || (hSpiNor == (HDC)RK_PARA_ERR))
    {
        rk_print_string("spinor device open failure");
        while (1);
    }

    if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("File device open failure");
        RKDev_Close(hSpiNor);
        while (1);
    }


    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = L"I love you.wma";

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    hFile = FileDev_OpenFile(hFileDev, NULL, READ_ONLY, &stFileAttr);
#else
    hFile = (HDC)-1;
#endif
    if ((rk_err_t)hFile <= 0)
    {
        rk_print_string("File Open Failure");

        RKDev_Close(hSpiNor);
        RKDev_Close(hFileDev);
        while (1);
    }

    LBA = 0;

#ifdef __DRIVER_FILE_FILEDEVICE_C__
    while (FileDev_ReadFile(hFile, Buf,  512) == 512)
#else
        while (0)
#endif
        {

            ret = SpiNorDev_Write(hSpiNor, LBA, Buf, 1);
            if (ret != 1)
            {
                while (1);
            }

            ret = SpiNorDev_Read(hSpiNor, LBA, Buf2, 1);
            if (ret != 1)
            {
                while (1);
            }

            if (StrCmpA(Buf, Buf2, 512) != 0)
            {
                while (1);
            }

            LBA++;

            if (LBA == 1920)
            {
                break;
            }

        }
    rk_print_string("file end");

#endif

}
/*******************************************************************************
** Name: SpiNorDevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL FUN rk_err_t SpiNorDevShellDel(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("spinor.format : spinordev delete cmd.\r\n");
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
** Name: SpiNorDevShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL FUN rk_err_t SpiNorDevShellMc(HDC dev, uint8 * pstr)
{

    HDC hSpi;
    SPINOR_DEV_ARG stSpiNorArg;
    rk_err_t ret;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("spinor.mc : spinordev mc cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    hSpi = RKDev_Open(DEV_CLASS_SPI, 0, NOT_CARE);

    if ((hSpi == NULL) || (hSpi == (HDC)RK_ERROR) || (hSpi == (HDC)RK_PARA_ERR))
    {
        rk_print_string("SPI Device Open Failure");
        return RK_SUCCESS;
    }

    stSpiNorArg.hSpi = hSpi;
    stSpiNorArg.SpiChannel = 1;

    ret = RKDev_Create(DEV_CLASS_SPINOR, 0, &stSpiNorArg);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("SpiNor Device Create Failure");
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: SpiNorDevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.30
** Time: 9:20:09
*******************************************************************************/
_DRIVER_SPINOR_SPINORDEVICE_SHELL_
SHELL FUN rk_err_t SpiNorDevShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("spinor.pcb : pcb info cmd.\r\n");
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

