/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..System\FwAnalysis\FwAnalysis.c
* Owner: HJ
* Date: 2014.3.10
* Time: 13:51:36
* Desc: FirmwareAnalysis
* History:
*   <author>    <date>       <time>     <version>     <Desc>
*      hj     2014.3.10     13:51:36   1.0
********************************************************************************************
*/
#include "BspConfig.h"

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__

#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"
#include "FwAnalysis.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define     CHIP_SIGN           0x4F4E414E                  //NANO
#define     FLASH_MAGIC         0x4E414E44
#define     FLASH_INFO_VER      0x100

#define SPI_IDB_NUM             1
#define SPI_IDB_SIZE            128

#define EMMC_IDB_NUM    5

#define  EMMC_BOOT_SIZE         (4*1024*1024/512)   //BOOT分区大小

#define  EMMC_SYS_OFFSET        EMMC_BOOT_SIZE      //固件从4M偏移开始

#define  EMMC_BOOT_OFFSET       (64)                //IDB 从32K偏移开始

#define SDM_BLOCK_SIZE             (512*1024/512)


#define D_MODULE_CNT 64

/*ID BLOCK SECTOR 0 INFO*/
typedef __packed struct tagIDSEC0
{
    uint32  magic;              //0x0ff0aa55, MASKROM限定不能更改
    uint8   reserved[56-4];
    uint32  nandboot2offset;
    uint32  nandboot2len;
    uint32  nandboot1offset1;
    uint32  nandboot1len1;
    uint32  nandboot1offset2;
    uint32  nandboot1len2;
    uint8   CRC[16];
    uint8   reserved1[512-96];

} IDSEC0, *pIDSEC0;

/*ID BLOCK SECTOR 1 INFO*/
typedef __packed struct tagIDSEC1
{

    uint16  SysAreaBlockRaw;        //系统保留块, 以原始Block为单位
    uint16  SysProgDiskCapacity;    //系统固件盘容量, 以MB为单位
    uint16  SysDataDiskCapacity;
    uint16  Disk2Size;
    uint16  Disk3Size;
    uint32  ChipTag;
    uint32  MachineId;

    uint16  LoaderYear;
    uint16  LoaderDate;
    uint16  LoaderVer;
    uint16  FirmMasterVer;        // (24-25)    BCD编码 固件主版本
    uint16  FirmSlaveVer;            // (26-27)    BCD编码 固件从版本
    uint16  FirmSmallVer;            // (28-29)    BCD编码 固件子版本
    uint16  LastLoaderVer;
    uint16  UpgradeTimes;
    uint16  MachineInfoLen;
    uint8   MachineInfo[30];
    uint16  ManufactoryInfoLen;
    uint8   ManufactoryInfo[30];
    uint16  FlashInfoOffset;
    uint16  FlashInfoLen;
    uint8   Reserved2[382];                // (102-483)保留
    uint32  FlashSize;                    //以sector为单位
    uint16  BlockSize;                  //以SEC表示的BLOCK SIZE
    uint8   PageSize;                  //以SEC表示的PAGE SIZE
    uint8   ECCBits;
    uint8   AccessTime;                //读写cycle时间, ns
    uint8   reserved2[5];
    uint16  FirstReservedBlock;        // (498-499)Idblock之后第一个保留块的的位置        (单位:block)
    uint16  LastReservedBlock;        // (500-501)Idblock之后最后一个保留块的的位置    (单位:block)
    uint16  IdBlock0;
    uint16  IdBlock1;
    uint16  IdBlock2;
    uint16  IdBlock3;
    uint16  IdBlock4;

}IDSEC1, *pIDSEC1;

#define          LANGUAGE_CHINESE_S               0      //Simplified Chinese.
#define          MENU_ITEM_LENGTH            (unsigned short)254             // The length of menu item in bytes
#define          TOTAL_MENU_ITEM             (unsigned short)171
#define          TOTAL_LANAUAGE_NUM          (unsigned short)19
#define          MAX_SUBMENU_NUM             (unsigned short)19
#define          MENU_CONTENT_OFFSET         (unsigned short)54


#define _SYSTEM_FWANALYSIS_FWANALYSIS_READ_  __attribute__((section("system_fwanalysis_fwanalysis_read")))
#define _SYSTEM_FWANALYSIS_FWANALYSIS_WRITE_ __attribute__((section("system_fwanalysis_fwanalysis_write")))
#define _SYSTEM_FWANALYSIS_FWANALYSIS_INIT_  __attribute__((section("system_fwanalysis_fwanalysis_init")))
#define _SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_  __attribute__((section("system_fwanalysis_fwanalysis_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//uint32 MDReadData(MEMDEV_ID DevID, uint32 offset, uint32 len, void *buf);
FIRMWARE_RESOURCE_ADDR gstFW_Resourec_addr;
static HDC ghLunDev;
static D_MODULE_INFO * gpstEmpDmodule;
D_MODULE_INFO * gpstDmodule;
static D_MODULE_INFO  stDmodule[D_MODULE_CNT];
uint16  UpgradeTimes;
uint16  LoaderVer;

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
rk_err_t FWShellHelp(HDC dev, uint8 * pstr);
rk_err_t FWShellPcb(HDC dev, uint8 * pstr);
rk_err_t FWShellPcb(HDC dev, uint8 * pstr);
rk_err_t FWShellTest(HDC dev, uint8 * pstr);
rk_err_t FWShellSuspend(HDC dev, uint8 * pstr);
rk_err_t FWShellOpen(HDC dev, uint8 * pstr);
rk_err_t FWShellResume(HDC dev, uint8 * pstr);
rk_err_t FWShellRead(HDC dev, uint8 * pstr);
rk_err_t FWShellWrite(HDC dev, uint8 * pstr);
rk_err_t FWShellControl(HDC dev, uint8 * pstr);


/*******************************************************************************
** Name: LUNReadData
** Input:uint32 offset, uint32 len, void *buf
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
uint32 LUNReadData(uint32 offset, uint32 len, void *buf)
{
    LunDev_Read(ghLunDev,offset,buf,len);
}

/*******************************************************************************
** Name: FW_ReadResourceData
** Input:uint32 Addr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
void FW_ReadResourceData(uint32 Addr, uint8 *pData, uint32 length)
{
    uint32 sectorNum;
    uint32 sectorOffset;
    uint32 count;
    uint32 i;
    uint8 *  FlashBuf;
    uint8 *pbuf = pData;

    FlashBuf = rkos_memory_malloc(512);

    sectorNum = Addr >> 9;
    sectorOffset = Addr & 511;
    while (length)
    {
        if (sectorOffset != 0 || length < 512)
        {
            LUNReadData(sectorNum,1,FlashBuf);

            count = 512-sectorOffset;
            count = (count>length)? length : count;
            for (i = sectorOffset; i< (count+sectorOffset); i++)
            {
                *pbuf++ = FlashBuf[i];
            }
            length -= count;
            sectorOffset = 0;
        }
        else
        {
            LUNReadData(sectorNum, 1, pbuf); //read 512 byte
            pbuf +=  512;
            length -=  512;
        }
        sectorNum++;

    }

    rkos_memory_free(FlashBuf);

}


/*******************************************************************************
** Name: FW_GetModuleInfo
** Input:uint32 Addr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
rk_err_t FW_GetModuleInfo(uint32 ModuleID, MODULE_INFO_T * Module)
{
    uint32 CodeInfoAddr;
    uint8  FlashBuf[512];
    MODULE_INFO_T * pModule;
    uint32 CodeLogicAddress;
    uint32 LoadStartBase;
    rk_err_t ret;
    FIRMWARE_INFO_T * pFirmwareModuleInfo;

    CodeLogicAddress = gstFW_Resourec_addr.CodeLogicAddress;
    LoadStartBase = gstFW_Resourec_addr.LoadStartBase;


    CodeInfoAddr  = CodeLogicAddress + sizeof(pFirmwareModuleInfo -> LoadStartBase);
    CodeInfoAddr  = CodeInfoAddr + sizeof(pFirmwareModuleInfo->ModuleInfo.ModuleNum) + ModuleID * sizeof(MODULE_INFO_T);
    FW_ReadResourceData(CodeInfoAddr, FlashBuf, sizeof(MODULE_INFO_T));

    pModule = (MODULE_INFO_T *)FlashBuf;


    Module->CodeLoadBase    = pModule->CodeLoadBase - LoadStartBase + CodeLogicAddress;
    Module->CodeImageBase   = pModule->CodeImageBase;
    Module->CodeImageLength = pModule->CodeImageLength;

    Module->DataLoadBase    = pModule->DataLoadBase - LoadStartBase + CodeLogicAddress;
    Module->DataImageBase   = pModule->DataImageBase;
    Module->DataImageLength = pModule->DataImageLength;

    Module->BssImageBase   = pModule->BssImageBase;
    Module->BssImageLength = pModule->BssImageLength;

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: FW_ModuleAdd
** Input:uint32 Addr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
rk_err_t FW_ModuleAdd(uint32 ModuleID, MODULE_INFO_T * pModule)
{
    D_MODULE_INFO * pDmodule;

    pDmodule = gpstDmodule;

    while (pDmodule != NULL)
    {
        if (((pModule->CodeImageBase >= pDmodule->ImageBase) &&
                (pModule->CodeImageBase < (pDmodule->ImageBase + pDmodule->ImageLen)))
                || ((pDmodule->ImageBase >= pModule->CodeImageBase) &&
                    (pDmodule->ImageBase < (pModule->CodeImageBase + pModule->CodeImageLength))))
        {
        	rk_printf("pModule->CodeLoadBase = 0x%08x \n",pModule->CodeLoadBase);
			rk_printf("pModule->CodeImageBase = 0x%08x \n",pModule->CodeImageBase);
			rk_printf("pModule->CodeImageLength = 0x%08x->%d \n",pModule->CodeImageLength,pModule->CodeImageLength);
			
			rk_printf("pDmodule->LoadBase = 0x%08x \n",pDmodule->LoadBase);
			rk_printf("pDmodule->ImageBase = 0x%08x \n",pDmodule->ImageBase);
			rk_printf("pDmodule->ImageLen = 0x%08x->%d \n",pDmodule->ImageLen,pDmodule->ImageLen);
			
            rk_printf("module add err src= %d, target = %d", pDmodule->ModuleID, ModuleID);
            return RK_ERROR;
        }

        pDmodule = pDmodule->pstDmodule;
    }

    if (gpstEmpDmodule == NULL)
    {
        rk_printf("module too many target = %d", ModuleID);
        return RK_ERROR;
    }

    pDmodule = gpstEmpDmodule;

    gpstEmpDmodule = gpstEmpDmodule->pstDmodule;

    pDmodule->ImageBase = pModule->CodeImageBase;
    pDmodule->ImageLen = pModule->CodeImageLength;
    pDmodule->LoadBase = pModule->CodeLoadBase - gstFW_Resourec_addr.LoadStartBase + gstFW_Resourec_addr.CodeLogicAddress;
    pDmodule->ModuleID = ModuleID;
    pDmodule->pstDmodule = gpstDmodule;
    gpstDmodule = pDmodule;

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: FW_ModuleOverlay
** Input:uint32 Addr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
rk_err_t FW_ModuleOverlay(uint32 ModuleID, uint8 Type)
{
#ifndef _JTAG_DEBUG_

    uint32 CodeInfoAddr;
    uint8 * FlashBuf;
    MODULE_INFO_T * pModule;
    uint32 CodeLogicAddress;
    uint32 LoadStartBase;
    rk_err_t ret;
    FIRMWARE_INFO_T * pFirmwareModuleInfo;
    uint8 *pBss;
    uint32 i;

    uint32 LoadBase;
    uint32 ImageBase;
    uint32 ImageLength;

    CodeLogicAddress = gstFW_Resourec_addr.CodeLogicAddress;
    LoadStartBase = gstFW_Resourec_addr.LoadStartBase;
    FlashBuf = rkos_memory_malloc(512);


    CodeInfoAddr  = CodeLogicAddress + sizeof(pFirmwareModuleInfo -> LoadStartBase);
    CodeInfoAddr  = CodeInfoAddr + sizeof(pFirmwareModuleInfo->ModuleInfo.ModuleNum) + ModuleID * sizeof(MODULE_INFO_T);
    FW_ReadResourceData(CodeInfoAddr, FlashBuf, sizeof(MODULE_INFO_T));


    pModule = (MODULE_INFO_T *)FlashBuf;

//    rk_printf("CodeLogicAddress = 0x%04x\n",gstFW_Resourec_addr.CodeLogicAddress);
//    rk_printf("LoadStartBase = 0x%04x\n",gstFW_Resourec_addr.LoadStartBase);
//
//    rk_printf("pModule->CodeImageBase = 0x%04x\n",pModule->CodeImageBase);
//    rk_printf("pModule->CodeImageLength = 0x%04x\n",pModule->CodeImageLength);

    rk_printf("ModuleID = %d", ModuleID);
    if (FW_ModuleAdd(ModuleID, pModule) != RK_SUCCESS)
    {
        rkos_memory_free(FlashBuf);
        return RK_ERROR;
    }

    if (Type & MODULE_OVERLAY_CODE)
    {
        LoadBase    = pModule->CodeLoadBase - LoadStartBase + CodeLogicAddress;
        ImageBase   = pModule->CodeImageBase;
        ImageLength = pModule->CodeImageLength;
        FW_ReadResourceData(LoadBase, (uint8 *)ImageBase, ImageLength);
    }

    if (Type & MODULE_OVERLAY_DATA)
    {
        LoadBase    = pModule->DataLoadBase - LoadStartBase + CodeLogicAddress;
        ImageBase   = pModule->DataImageBase;
        ImageLength = pModule->DataImageLength;
        FW_ReadResourceData(LoadBase, (uint8 *)ImageBase, ImageLength);
    }

    if (Type & MODULE_OVERLAY_BSS)
    {
        ImageBase   =  pModule->BssImageBase;
        ImageLength =  pModule->BssImageLength;
        pBss = (uint8 *)ImageBase;
        for (i = 0; i < ImageLength; i++)
        {
            *pBss++ = 0;
        }
    }

    rkos_memory_free(FlashBuf);

    return RK_SUCCESS;

#else

    return RK_SUCCESS;

#endif

}

/*******************************************************************************
** Name: FW_RemoveModule
** Input:uint32 Addr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
rk_err_t FW_RemoveModule(uint32 ModuleID)
{
#ifndef _JTAG_DEBUG_

    D_MODULE_INFO * pDmodule, * pPrev;

    pDmodule = gpstDmodule;
    pPrev = NULL;

    while (pDmodule != NULL)
    {
        if (pDmodule->ModuleID == ModuleID)
        {
            if (pPrev == NULL)
            {
                gpstDmodule = pDmodule->pstDmodule;
            }
            else
            {
                pPrev->pstDmodule = pDmodule->pstDmodule;
            }

            pDmodule->pstDmodule = gpstEmpDmodule;
            gpstEmpDmodule = pDmodule;
            return RK_SUCCESS;
        }

        pPrev = pDmodule;
        pDmodule = pDmodule->pstDmodule;
    }

    return  RK_SUCCESS;

#else
    return  RK_SUCCESS;
#endif

}



/*******************************************************************************
** Name: FW_GetMenuResourceData
** Input:UINT16 menuTextID , UINT16 *pMenuStr ,UINT16 StrLen
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
void FW_GetMenuResourceData(UINT16 menuTextID , UINT16 *pMenuStr ,UINT16 StrLen)
{

    UINT32   CharInNFAddr;
    UINT32   languegeOffsetAddr;

    languegeOffsetAddr = gstFW_Resourec_addr.MenuLogicAddress + LANGUAGE_CHINESE_S*(TOTAL_MENU_ITEM)+TOTAL_LANAUAGE_NUM*4+2;

    CharInNFAddr = languegeOffsetAddr + menuTextID*MENU_ITEM_LENGTH+MENU_CONTENT_OFFSET;
    FW_ReadResourceData(CharInNFAddr, (UINT8*)pMenuStr, StrLen);

}
/*******************************************************************************
** Name: FW_GetPicResourceData
** Input:uint32 FlashAddr, uint8 *pData, uint16 length
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
void FW_GetPicResourceData(uint32 FlashAddr, uint8 *pData, uint16 length)
{
    FW_ReadResourceData(gstFW_Resourec_addr.ImageLogicAddress+FlashAddr,pData,length);
}
/*******************************************************************************
** Name: FW_GetMenuInfoWithIDNum
** Input:UINT32 menuTextID,
         MENU_TEXT_INFO_STRUCT *pMenuTextInfo
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
void FW_GetMenuInfoWithIDNum(UINT32 menuTextID,
                             MENU_TEXT_INFO_STRUCT *pMenuTextInfo)
{
    UINT32   CharInNFAddr;
    UINT32   languegeOffsetAddr;

    //languegeOffsetAddr = gpstFW_Resourec_addr->MenuLogicAddress + LANGUAGE_CHINESE_S*MENU_ITEM_LENGTH*(TOTAL_MENU_ITEM-9)+TOTAL_LANAUAGE_NUM*4+2;
    languegeOffsetAddr = gstFW_Resourec_addr.MenuLogicAddress + LANGUAGE_CHINESE_S*(TOTAL_MENU_ITEM)+TOTAL_LANAUAGE_NUM*4+2;
    CharInNFAddr = languegeOffsetAddr + menuTextID * MENU_ITEM_LENGTH;
    FW_ReadResourceData(CharInNFAddr, (UINT8*)pMenuTextInfo, sizeof(MENU_TEXT_INFO_STRUCT));
}
/*******************************************************************************
** Name: FW_GetPicInfoWithIDNum
** Input:UINT16 pictureIDNum, PICTURE_INFO_STRUCT *psPictureInfo
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
void FW_GetPicInfoWithIDNum(UINT16 pictureIDNum, PICTURE_INFO_STRUCT *psPictureInfo)
{
    UINT32 flashAddr,i;
    int8  hMainFile;
    uint8  FlashBuf[512];

    //32是固件中图片资源头信息的长度 IMAGE_RESOURCE_INFO_SIZE 是每个图片信息结构体所占用的字节数(16)
    flashAddr = gstFW_Resourec_addr.ImageLogicAddress + 32 + ((UINT32)pictureIDNum * 16);
    FW_ReadResourceData(flashAddr, (UINT8*)psPictureInfo, 16);
    return;
}

/*******************************************************************************
** Name: FW_Resource_Init
** Input:void
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_INIT_
void FW_Resource_Init(void)
{
    uint32 i,j=0;
    uint32 Len;
    uint32 SystemDefaultParaAddr;
    //uint8  TempBuffer[1024];
    uint8  FlashBuf[1024];
    pIDSEC0 IdSec0;
    pIDSEC1 IdSec1;

    uint16 FirmwareYear;
    uint16 FirmwareDate;
    uint16 MasterVersion;
    uint16 SlaveVersion;
    uint16 SmallVersion;
    uint32 MenuLogicAddress0, MenuLogicAddress1;

    FIRMWARE_HEADER       *pFWHead;

    FIRMWARE_RESOURCE_ADDR * pFW_Resourec_addr;


    ghLunDev = RKDev_Open(DEV_CLASS_LUN,0,NOT_CARE);

    pFW_Resourec_addr =  &gstFW_Resourec_addr;


    ////////////////////////////////////////////////////////////////////////////
    //read resource module address.
    pFWHead = (FIRMWARE_HEADER *)FlashBuf;


#ifdef _SPI_BOOT_
    LunDev_Read(ghLunDev, 0, FlashBuf, 2);
    IdSec0 = (pIDSEC0)FlashBuf;
    if(IdSec0->magic == 0xFCDC8C3B)   //0x0ff0aa55 + rc4
    {
        IdSec1=(pIDSEC1)(FlashBuf + 512);      //IdSec1 NOT RC4
        if (IdSec1->ChipTag == CHIP_SIGN)
        {

            LunDev_Read(ghLunDev, SPI_IDB_SIZE, FlashBuf, 1);

            pFW_Resourec_addr->CodeLogicAddress = pFWHead->CodeInfo.CodeInfoTbl[SYS_CODE].ModuleOffset + SPI_IDB_SIZE * 512;

            FW_ReadResourceData(pFW_Resourec_addr->CodeLogicAddress, FlashBuf, 512);
            pFW_Resourec_addr->LoadStartBase = *(uint32 *)FlashBuf;
        }
    }
#endif


#ifdef _EMMC_BOOT_
    LunDev_Read(ghLunDev, EMMC_SYS_OFFSET, FlashBuf, 1);


    pFW_Resourec_addr->Font12LogicAddress = pFWHead->FontInfo.FontInfoTbl[FONT12].ModuleOffset;
    pFW_Resourec_addr->Font16LogicAddress = pFWHead->FontInfo.FontInfoTbl[FONT16].ModuleOffset;

    pFW_Resourec_addr->CodeLogicAddress = pFWHead->CodeInfo.CodeInfoTbl[SYS_CODE].ModuleOffset;
    pFW_Resourec_addr->GBKLogicAddress  = pFWHead->CharEncInfo.CharEncInfoTbl[GBK].ModuleOffset;


    MenuLogicAddress0  = pFWHead->MenuInfo.MenuInfoTbl[0].ModuleOffset;
    MenuLogicAddress1  = pFWHead->MenuInfo.MenuInfoTbl[1].ModuleOffset;

    pFW_Resourec_addr->MenuLogicAddress  = MenuLogicAddress0; //if FM ENABLE else MenuLogicAddress1
    pFW_Resourec_addr->ImageLogicAddress = pFWHead->UiInfo.UiInfoTbl[0].ModuleOffset;
    pFW_Resourec_addr->CodeLogicAddress  =  pFWHead->CodeInfo.CodeInfoTbl[SYS_CODE].ModuleOffset + EMMC_SYS_OFFSET * 512;
    FW_ReadResourceData(pFW_Resourec_addr->CodeLogicAddress, FlashBuf, 512);
    pFW_Resourec_addr->LoadStartBase = *(uint32 *)FlashBuf;
#endif

    {

        D_MODULE_INFO * pstEmpDmodule;
        MODULE_INFO_T module;

        gpstEmpDmodule = &stDmodule[0];
        pstEmpDmodule = gpstEmpDmodule;

        for (i = 1; i < D_MODULE_CNT; i++)
        {
            pstEmpDmodule->pstDmodule = &stDmodule[i];
            pstEmpDmodule = pstEmpDmodule->pstDmodule;
        }

        pstEmpDmodule->pstDmodule = NULL;
        gpstDmodule = NULL;

        gpstDmodule = gpstEmpDmodule;
        gpstEmpDmodule = gpstEmpDmodule->pstDmodule;

        gpstDmodule->pstDmodule = NULL;

        FW_GetModuleInfo(MODULE_ID_SYS, &module);

        gpstDmodule->ImageBase = module.CodeImageBase;
        gpstDmodule->ImageLen = module.CodeImageLength;
        gpstDmodule->LoadBase = module.CodeLoadBase ;
        gpstDmodule->ModuleID = MODULE_ID_SYS;
    }

}

#ifdef _USE_SHELL_

_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
static uint8* ShellFWName[] =
{
    "pcb",
    "open",
    "test",
    "close",
    "suspend",
    "resume",
    "read",
    "write",
    "control",
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
** Name: FWShell
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShell(HDC dev,  uint8 * pstr)
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

    while (memcmp((uint8*)ShellFWName[i],pItem, strlen(ShellFWName[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellFWName[i]) == '\b')
            return RK_ERROR;
    }

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:
            ret = FWShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = FWShellOpen(dev,pItem);
            break;

        case 0x02:
            ret = FWShellTest(dev,pItem);
            break;

        case 0x03:
            //ret = ShellSystemParsing(pItem,UartDeviceHandler);
            ret = FWShellClose(dev,pItem);
            break;

        case 0x04:
            //ret = ShellCustomParsing(pItem,UartDeviceHandler);
            ret = FWShellSuspend(dev,pItem);
            break;

        case 0x05:
            ret = FWShellResume(dev,pItem);
            break;

        case 0x06:
            ret = FWShellRead(dev,pItem);
            break;

        case 0x07:
            ret = FWShellWrite(dev,pItem);
            break;

        case 0x08:
            ret = FWShellControl(dev,pItem);
            break;

        case 0x09:  //help
            ret = FWShellHelp(dev,pItem);
            break;
        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}


/*******************************************************************************
** Name: FWShellPcb
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.pcb : pcb info cmd.  \r\n");
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
** Name: FWShellClose
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellClose(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.close : close firmware.   \r\n");
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
** Name: FWShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if ( StrLenA( (uint8 *)pstr) != 0)
        return RK_ERROR;

    rk_print_string("fw命令集提供了一系列的命令对firmware进行操作\r\n");
    rk_print_string("fw包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("open      打开firmware        \r\n");
    rk_print_string("test      测试firmware命令    \r\n");
    rk_print_string("close     关闭firmware        \r\n");
    rk_print_string("suspend   suspend命令         \r\n");
    rk_print_string("resume    resume命令          \r\n");
    rk_print_string("read      读取firmware        \r\n");
    rk_print_string("write     写操作              \r\n");
    rk_print_string("control   控制操作            \r\n");
    rk_print_string("help      显示fw命令帮助信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FWShellTest
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellTest(HDC dev,  uint8 * pstr)
{
    HDC hFW;
    rk_err_t ret;
    uint32 i,j;
    HDC hExDev;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.test : test \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2S open failure",17,SYNC_MODE,NULL);
    }

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FWShellOpen
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:HJ
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellOpen(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.open : open  firmware.  \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    FW_Resource_Init();
}

/*******************************************************************************
** Name: FWShellSuspend
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellSuspend(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.suspend : suspend    \r\n");
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
** Name: FWShellResume
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellResume(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.resume : resume   \r\n");
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
** Name: FWShellRead
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellRead(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.read : read firmware.   \r\n");
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
** Name: FWShellWrite
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellWrite(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.write : write firmware.   \r\n");
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
** Name: FWShellControl
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.10.31
** Time: 16:51:05
*******************************************************************************/
_SYSTEM_FWANALYSIS_FWANALYSIS_SHELL_
rk_err_t FWShellControl(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("fw.control : control sub cmd.   \r\n");
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
/*
********************************************************************************
*
*                         End of FwAnalysis.c.c
*
********************************************************************************
*/
