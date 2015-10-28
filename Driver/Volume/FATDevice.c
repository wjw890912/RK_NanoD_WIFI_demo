/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\Volume\FATDevice.c
* Owner: Aaron.sun
* Date: 2014.3.10
* Time: 15:19:31
* Desc: fat file system device class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.3.10     15:19:31   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_VOLUME_FATDEVICE_C__

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
//direction item link structure.
typedef  struct _FDT_DATA
{
    uint32  DirClus;                            //dir first cluster
    uint32  CurClus;                            //current cluster
    uint16  Cnt;          //cluster chain counter

} FDT_DATA;

typedef struct _FS_CHCHE
{
    uint8    Flag[3];        //读写标记
    uint32    Sec[3];        //扇区地址

}FS_CHCHE;

typedef  struct _FAT_DEVICE_CLASS
{
    DEVICE_CLASS stFATDevice;

    pSemaphore osFatOperReqSem;

    uint32 VolumeID;
    HDC hPar;
    uint8 VolumeName[11];

    uint16 * pwFat[3];  //512 Byte
    uint8 * pbFdt[3];     //512 Byte

    FS_CHCHE stFatCache;
    FS_CHCHE stFdtCache;

    uint32 FATType;
    uint32 LogSecPerClus;
    uint32 LogBytePerSec;

    uint32 BPB_BytsPerSec;    //bytes_per_sector; // usu. =512
    uint32 BPB_SecPerClus;    //sectors_per_cluster;
    uint32 BPB_ResvdSecCnt;   //num_boot_sectors; // usu. =1
    uint32 BPB_NumFATs;       //num_fats;     // usu. =2
    uint32 BPB_RootEntCnt;    //num_root_dir_ents;
    uint32 BPB_RootClus;
    uint32 BPB_FSInfo;
    uint32 FATSz;
    uint32 RootDirSectors;
    uint32 CountofClusters;
    uint32 FirstDataSector;
    uint32 FirstEmpClus;
    uint32 TotSec;            //add by lxs @2005.4.19
    uint32 VolumeFreeSize;
    uint32 VolumeSize;        // uint K Byte

}FAT_DEVICE_CLASS;

typedef struct _FAT_DIR_CLASS
{
    uint32 DirClus;    //SubDirInfo[0].DirClus boot diretion
    uint32 Index;      //SubDirInfo[0].Index   the child direction index of boot direction.
    uint32 CurClus;
    uint32 CurIndex;
    uint32 TotalFile;
    uint32 TotalSubDir;

    FAT_DEVICE_CLASS * pstFatDev;

}FAT_DIR_CLASS;

typedef struct _FAT_FILE_CLASS
{
    uint32  Clus;                               //current cluster number.
    uint32  Sec;
    uint32  CurClus;
    uint32  CurClusS;
    uint32  CurSec;
    uint32  TotalSec;
    uint32  FileSize;
    uint32  FileOffset;
    uint32  WriteFlag;

    FAT_DIR_CLASS * pstFatDir;

}FAT_FILE_CLASS;


#define _DRIVER_VOLUME_FATDEVICE_COMMON_  __attribute__((section("driver_volume_fatdevice_read")))
#define _DRIVER_VOLUME_FATDEVICE_INIT_  __attribute__((section("driver_volume_fatdevice_init")))
#define _DRIVER_VOLUME_FATDEVICE_SHELL_  __attribute__((section("driver_volume_fatdevice_shell")))


typedef __packed  struct _FDT
{
    uint8   Name[11];                           //short file name
    uint8   Attr;                               //file attribute
    uint8   NTRes;                              //reseverd for NT
    uint8   CrtTimeTenth;                       //create time

    uint16  CrtTime;                            //create time
//  uint16  reserve1;                           //reserved

    uint16  CrtDate;                            //create date
//  uint16  reserve2;                           //reseverd

    uint16  LstAccDate;                         //last access date
//  uint16  reserve3;                           //reseverd

    uint16  FstClusHI;                          //the hight two byte of first cluster
//  uint16  reserve4;                           //reseverd

    uint16  WrtTime;                            //last write time
//  uint16  reserve5;                           //reseverd

    uint16  WrtDate;                            //last write date
//  uint16  reserve6;                           //reseverd

    uint16  FstClusLO;                          //the low two byte of first cluster
//  uint16  reserve7;                           //reseverd

    uint32  FileSize;                           //file size
//  uint16  reserve8;                           //reseverd
//  uint16  reserve9;                           //reseverd

} FDT;



/* FDT file property */
#define     ATTR_VOLUME_ID          0x08
#define     ATTR_LFN_ENTRY          0x0F      /* LFN entry attribute */
#define     ATTR_LONG_NAME          ATTR_READ_ONLY |\
                                    ATTR_HIDDEN |\
                                    ATTR_SYSTEM |\
                                    ATTR_VOLUME_ID
#define     ATTR_LONG_NAME_MASK     ATTR_READ_ONLY | \
                                    ATTR_HIDDEN |\
                                    ATTR_SYSTEM |\
                                    ATTR_VOLUME_ID |\
                                    ATTR_DIRECTORY |\
                                    ATTR_ARCHIVE

/* cluster type */
#define     EMPTY_CLUS              0
#define     EMPTY_CLUS_1            1
#define     BAD_CLUS                0x0ffffff7L
#define     EOF_CLUS_1              0x0ffffff8L
#define     EOF_CLUS_END            0x0fffffffL

/* FDT type */
#define     FILE_NOT_EXIST          0
#define     FILE_DELETED            0xe5
#define     ESC_FDT                 0x05


#define     LFN_SEQ_MASK            0x3f
#define     MAX_LFN_ENTRIES         20

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
rk_err_t FatShellHelp(HDC dev, uint8 * pstr);
rk_err_t FatShellPcb(HDC dev, uint8 * pstr);
rk_err_t FatShellCreate(HDC dev, uint8 * pstr);
rk_err_t FatPrepareWriteFile(FAT_FILE_CLASS * pstFatFile, uint32 len);
rk_err_t FatCheckSecContinue(FAT_FILE_CLASS * pstFatFile, uint32 * LBA,  uint32 * SecCnt);
void FATInitCache(FAT_DEVICE_CLASS * pstFatDev);
rk_err_t FATGetLongFromShortName(uint16 *  LongName, uint8 * ShortName);
rk_err_t FATGetPrevClus(FAT_DEVICE_CLASS * pstFatDev, uint32 FirstClus, uint32 Clus);
rk_err_t FileExtNameMatch(uint8 * Name, uint8 * ExtName);
rk_err_t FATDeleteClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus);
rk_err_t FATGetShortFromLongName(uint8 * ShortName, uint16 * LongName);
rk_err_t AddFDTLong(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex, FDT *FDTData, uint32 * pIndex, uint16 * FileName);
rk_err_t ClearClus(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex);
rk_err_t FATSetNextClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Index, uint32 Next);
rk_err_t FATAddClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus);
rk_err_t SetRootFDTInfo(FAT_DEVICE_CLASS * pstFatDev,uint32 Index, FDT *FDTData);
rk_err_t WriteFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *FDTData, uint32 SecIndex, uint16 ByteIndex);
rk_err_t SetFDTInfo(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex, uint32 Index, FDT *FDTData);
rk_err_t AddFDT(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex, FDT *FDTData, uint32 * pIndex);
uint8 FatCheckSum(uint8 * pFileName);
rk_err_t FindFDTInfoLong(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 ClusIndex, uint32 * pIndex, uint16 * FileName);
rk_err_t FindFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 ClusIndex, uint32 * pIndex,  uint8 *FileName);
rk_err_t GetDirClusIndex(FAT_DEVICE_CLASS * pstFatDev, uint16 *Path, uint16 len, uint32 CurDirClus);
uint8 FSRealname(uint8 *dname, uint16 *pDirName);
uint32 FsIsLongName(uint16 *dname, uint16 len);
rk_err_t ReadFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *Rt, uint32 SecIndex, uint16 ByteIndex);
rk_err_t GetRootFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 Index);
void GetFATPosition(FAT_DEVICE_CLASS * pstFatDev, uint32 cluster, uint32 *FATSecNum, uint16 *FATEntOffset);
rk_err_t FATWriteSector(FAT_DEVICE_CLASS * pstFatDev, uint32 LBA, void * Buf);
rk_err_t FATGetNextClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Index);
rk_err_t GetFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *Rt, uint32 * ClusIndex, uint32 Index);
uint32 GetBootInfo(FAT_DEVICE_CLASS * pstFatDev, uint8 * DbrBuf);
int lg2(uint32 arg);
rk_err_t CheckFatBootSector(uint8 * DbrBuf);
rk_err_t FATReadSector(FAT_DEVICE_CLASS * pstFatDev,  uint32 LBA, void * Buf);
rk_err_t FATDevInit(FAT_DEVICE_CLASS * pstFatDev);
rk_err_t FatDevDelete(HDC dev);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FatDev_SynchFileHandler
** Input:HDC hTarget,  HDC hSrc
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.7
** Time: 15:47:48
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_SynchFileHandler(HDC hTarget,  HDC hSrc, uint32 Mode)
{
    FAT_FILE_CLASS * pstTarget = (FAT_FILE_CLASS *)hTarget;
    FAT_FILE_CLASS * pstSrc = (FAT_FILE_CLASS *)hSrc;

    if(pstTarget == NULL || pstSrc == NULL)
    {
        return RK_PARA_ERR;
    }

    if(pstTarget->Clus == 0)
    {
        pstTarget->CurClus = pstSrc->Clus;
        pstTarget->CurSec = pstTarget->pstFatDir->pstFatDev->FirstDataSector
            + (pstTarget->CurClus - 2) * pstTarget->pstFatDir->pstFatDev->BPB_SecPerClus;
    }
    pstTarget->Clus = pstSrc->Clus;
    pstTarget->FileSize = pstSrc->FileSize;
    pstTarget->TotalSec = pstSrc->TotalSec;

    if(Mode == SYNCH_ALL_PARA)
    {
        pstTarget->CurClus = pstSrc->CurClus;
        pstTarget->CurSec = pstSrc->CurSec;
        pstTarget->Sec = pstSrc->Sec;
    }

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: GetTotalDir
** Input:HDC dev, uint32 Clus, uint8 * ExtName, uint8 Attr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 16:38:59
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_GetTotalDir(HDC dev, uint8 * ExtName, uint8 Attr)
{
    uint32 offset;
    uint16 TotalFiles;
    rk_err_t ret;
    FDT    temp;
    uint32 PrevClus;
    uint32 Clus;
    FAT_DIR_CLASS * pstFatDir = (FAT_DIR_CLASS *)dev;

    if(pstFatDir == NULL)
    {
        return RK_ERROR;
    }

    offset     = 0;
    TotalFiles = 0;

    Clus = pstFatDir->DirClus;

    if (Clus == BAD_CLUS)
    {
        return RK_PARA_ERR;
    }

    PrevClus = Clus;

    while (1)
    {
        if (PrevClus != Clus)
        {
            PrevClus = Clus;
            offset = 1;
        }

        ret = GetFDTInfo(pstFatDir->pstFatDev, &temp, &Clus, offset++);

        if (FDT_OVER == ret)
        {
            break;
        }

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (temp.Name[0] == FILE_NOT_EXIST)
        {
            break;
        }

        if (temp.Attr == ATTR_LFN_ENTRY)
        {
            continue;
        }

        if (!(temp.Attr & ATTR_DIRECTORY))
        {
            continue;
        }

        if (temp.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (temp.Name[0] == '.')
        {
            continue;
        }

        if (temp.Attr & Attr)
        {
            continue;
        }

        if (ExtName != NULL)
        {
            if (FileExtNameMatch(&temp.Name[8], ExtName) == RK_SUCCESS)
            {
                TotalFiles++;
            }
        }

    }
    return (TotalFiles);
}


/*******************************************************************************
** Name: GetTotalFile
** Input:HDC dev, uint32 Clus, uint8 * ExtName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 16:08:55
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_GetTotalFile(HDC dev, uint8 * ExtName, uint8 Attr)
{
    uint32 offset;
    uint16 TotalFiles;
    rk_err_t ret;
    FDT    temp;
    uint32 PrevClus;
    uint32 Clus;

    FAT_DIR_CLASS * pstFatDir = (FAT_DIR_CLASS *)dev;

    if (pstFatDir == NULL)
    {
        return RK_ERROR;
    }

    offset     = 0;
    TotalFiles = 0;

    Clus = pstFatDir->DirClus;

    if (Clus == BAD_CLUS)
    {
        return RK_PARA_ERR;
    }

    PrevClus = Clus;

    while (1)
    {

        if (PrevClus != Clus)
        {
            PrevClus = Clus;
            offset = 1;
        }

        ret = GetFDTInfo(pstFatDir->pstFatDev, &temp, &Clus, offset++);

        if (FDT_OVER == ret)
        {
            break;
        }

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (temp.Name[0] == FILE_NOT_EXIST)
        {
            break;
        }

        if (temp.Attr == ATTR_LFN_ENTRY)
        {
            continue;
        }

        if (temp.Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID))
        {
            continue;
        }

        if (temp.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (temp.Attr & Attr)
        {
            continue;
        }

        if (ExtName != NULL)
        {
            if (FileExtNameMatch(&temp.Name[8], ExtName) == RK_SUCCESS)
            {
                TotalFiles++;
            }
        }

    }
    return ((rk_err_t)TotalFiles);
}

/*******************************************************************************
** Name: FATDevWrite
** Input:HDC dev, uint32 pos, const void* buffer, uint32 size,uint8 mode,pTx_complete Tx_complete
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FatDev_WriteFile(HDC dev, uint8* buffer, uint32 len)
{
    FAT_FILE_CLASS * pstFatFile = (FAT_FILE_CLASS *)dev;
    rk_err_t ret;
    uint32 Sec, SecCnt, RealLen, TotalSec;
    uint8 buf[512];

    RealLen = 0;
    //rk_printf("write");

    ret = FatPrepareWriteFile(pstFatFile, len);

    if(ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    TotalSec = len / 512;

    while(TotalSec > 0)
    {
        SecCnt = TotalSec;
        ret = FatCheckSecContinue(pstFatFile, &Sec, &SecCnt);
        if(ret != RK_SUCCESS)
        {
            rk_printf("errr");
            return RK_ERROR;
        }

        //rk_printf("write Sec =  %d,  SecCnt = %d", Sec, SecCnt);

        ret = ParDev_Write(pstFatFile->pstFatDir->pstFatDev->hPar, Sec, buffer, SecCnt);

        //rk_printf("ret = %d", ret);
        if(ret != (rk_err_t)SecCnt)
        {
           return RK_ERROR;
        }

        TotalSec -= SecCnt;
        buffer += (SecCnt * 512);
        RealLen += (SecCnt * 512);

    }

    if(len % 512)
    {
        SecCnt = 1;
        ret = FatCheckSecContinue(pstFatFile, &Sec, &SecCnt);
        if(ret != RK_SUCCESS)
        {
            rk_printf("errr");
            return RK_ERROR;
        }

        //rk_printf("write Sec =  %d,  SecCnt = %d", Sec, SecCnt);

        ret = ParDev_Read(pstFatFile->pstFatDir->pstFatDev->hPar, Sec, buf, SecCnt);
        if(ret != (rk_err_t)SecCnt)
        {
           return RK_ERROR;
        }

        memcpy(buf, buffer, len%512);

        ret = ParDev_Write(pstFatFile->pstFatDir->pstFatDev->hPar, Sec, buf, SecCnt);

        //rk_printf("ret = %d", ret);
        if(ret != (rk_err_t)SecCnt)
        {
           return RK_ERROR;
        }

        RealLen += (len % 512);
    }

    pstFatFile->FileOffset += RealLen;

    if(pstFatFile->FileOffset > pstFatFile->FileSize)
    {
        pstFatFile->FileSize = pstFatFile->FileOffset;
        pstFatFile->WriteFlag = 1;
    }
    return (rk_err_t)RealLen;

}

/*******************************************************************************
** Name: FATDeleteDir
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:00:29
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_DeleteDir(HDC dev, HDC hFather, FILE_ATTR * pstFatArg)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FATDeleteFile
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:00:07
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_DeleteFile(HDC dev, HDC hFather, FILE_ATTR * pstFatArg)
{
    return RK_SUCCESS;
}


/*******************************************************************************
** Name: FATCreateDir
** Input:(HDC dev, FILE_ATTR * pstFatArg)
** Return: Aaron.sun
** Owner:Aaron.sun
** Date: 2014.3.26
** Time: 11:28:21
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_CreateDir(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FAT_DEVICE_CLASS * pstFatDev = (FAT_DEVICE_CLASS *)dev;

    FDT     temp;
    uint32 Clus, Index, SubClus;
    rk_err_t ret;
    uint32 i;

    if (pstFileAttr->FileName == NULL)
    {
        return RK_PARA_ERR;
    }

    if ((hFather == NULL) && (pstFileAttr->Path == NULL))
    {
        return RK_PARA_ERR;
    }

    if (hFather != NULL)
    {
        Clus = ((FAT_DIR_CLASS *)hFather)->DirClus;

        if (hFather != NULL)
        {
            Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), Clus);
        }
    }
    else
    {
        Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), pstFatDev->BPB_RootClus);
    }

    if((Clus == BAD_CLUS) || (Clus <= 1))
    {
        return RK_ERROR;
    }

    SubClus = 0;

    ret = FATAddClus(pstFatDev, 0);

    if (ret < EMPTY_CLUS)
    {
        goto ERROR1;
    }

    SubClus = (uint32)ret;

    if (ClearClus(pstFatDev, SubClus) != RK_SUCCESS)
    {
        goto ERROR1;
    }

    if (FsIsLongName(pstFileAttr->FileName, StrLenW(pstFileAttr->FileName)) == 1)
    {
        if (FATGetShortFromLongName(temp.Name, pstFileAttr->FileName) == RK_SUCCESS)
        {
            temp.Attr = ATTR_DIRECTORY;
            temp.FileSize     = 0;
            temp.NTRes        = 0;
            temp.CrtTimeTenth = 0;
            temp.CrtTime      = 0;
            temp.CrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.LstAccDate   = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.WrtTime      = 0;
            temp.WrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.FstClusLO    = SubClus & 0x0000FFFF;
            temp.FstClusHI    = (SubClus >> 16) & 0x0000FFFF;
            ret = AddFDTLong(pstFatDev, Clus, &temp, &Index, pstFileAttr->FileName);
            if (ret != RK_SUCCESS)
            {
                if (ret == DISK_FULL)
                {
                    FATDeleteClus(pstFatDev, SubClus);
                }

                goto ERROR1;
            }

        }
        else
        {
            FATDeleteClus(pstFatDev, SubClus);
            goto ERROR1;
        }

    }
    else
    {
        temp.Attr = ATTR_DIRECTORY;
        temp.FileSize     = 0;
        temp.NTRes        = FSRealname(temp.Name, pstFileAttr->FileName);
        temp.CrtTimeTenth = 0;
        temp.CrtTime      = 0;
        temp.CrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.LstAccDate   = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.WrtTime      = 0;
        temp.WrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.FstClusLO    = SubClus & 0x0000FFFF;
        temp.FstClusHI    = (SubClus >> 16) & 0x0000FFFF;

        ret = AddFDT(pstFatDev, Clus, &temp, &Index);

        if (ret != RK_SUCCESS)
        {
            if (ret == DISK_FULL)
            {
                FATDeleteClus(pstFatDev, SubClus);
            }

            goto ERROR1;
        }

    }


    for(i = 0; i < 3; i++)
    {

        if (pstFatDev->stFdtCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[i], pstFatDev->pbFdt[i]))
            {
                return RK_ERROR;
            }
        }

        if (pstFatDev->stFatCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[i], pstFatDev->pwFat[i]))
            {
                return RK_ERROR;
            }
        }

    }

    return RK_SUCCESS;


ERROR1:

    return RK_ERROR;

}


/*******************************************************************************
** Name: FATCreateFile
** Input:HDC dev, uint16 * path, uint16 * Name, uint8 Attr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.17
** Time: 16:45:12
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_CreateFile(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FAT_DEVICE_CLASS * pstFatDev = (FAT_DEVICE_CLASS *)dev;

    FDT temp;
    uint32 Clus, Index;
    rk_err_t ret;
    uint32 i;

    if (pstFileAttr->FileName == NULL)
    {
        return RK_PARA_ERR;
    }

    if ((hFather == NULL) && (pstFileAttr->Path == NULL))
    {
        return RK_PARA_ERR;
    }

    if (hFather != NULL)
    {
        Clus = ((FAT_DIR_CLASS *)hFather)->DirClus;

        if (pstFileAttr->Path != NULL)
        {
            Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), Clus);
        }
    }
    else
    {
        Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), pstFatDev->BPB_RootClus);
    }

    if((Clus == BAD_CLUS) || (Clus <= 1))
    {
        return RK_ERROR;
    }

    if (FsIsLongName(pstFileAttr->FileName, StrLenW(pstFileAttr->FileName)) == 1)
    {
        if (FATGetShortFromLongName(temp.Name, pstFileAttr->FileName) == RK_SUCCESS)
        {
            temp.Attr = 0;
            temp.FileSize     = 0;
            temp.NTRes        = 0;
            temp.CrtTimeTenth = 0;
            temp.CrtTime      = 0;
            temp.CrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.LstAccDate   = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.WrtTime      = 0;
            temp.WrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
            temp.FstClusLO    = 0;
            temp.FstClusHI    = 0;
            ret = AddFDTLong(dev, Clus, &temp, &Index, pstFileAttr->FileName);
            rk_printf("ret = %d" ,ret);
            if (ret != RK_SUCCESS)
            {
                goto ERROR1;
            }

        }
        else
        {
            goto ERROR1;
        }

    }
    else
    {
        temp.Attr = 0;
        temp.FileSize     = 0;
        temp.NTRes        = FSRealname(temp.Name, pstFileAttr->FileName);
        temp.CrtTimeTenth = 0;
        temp.CrtTime      = 0;
        temp.CrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.LstAccDate   = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.WrtTime      = 0;
        temp.WrtDate      = (1) | (1 << 5) | ((2014 - 1980) << 9);
        temp.FstClusLO    = 0;
        temp.FstClusHI    = 0;

        ret = AddFDT(dev, Clus, &temp, &Index);

        if (ret != RK_SUCCESS)
        {
            goto ERROR1;
        }

    }

    for(i = 0; i < 3; i++)
    {

        if (pstFatDev->stFdtCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[i], pstFatDev->pbFdt[i]))
            {
                return RK_ERROR;
            }
        }

        if (pstFatDev->stFatCache.Flag[i])
        {
            pstFatDev->stFatCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[i], pstFatDev->pwFat[i]))
            {
                return RK_ERROR;
            }
        }

    }

    return RK_SUCCESS;

ERROR1:

    return RK_ERROR;

}
/*******************************************************************************
** Name: FatDev_FileSeek
** Input:HDC dev , uint32 pos, uint32 SecCnt
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 14:19:37
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_FileSeek(HDC dev , uint32 pos, uint32 Offset)
{
    FAT_FILE_CLASS * pstFatFile = (FAT_FILE_CLASS *)dev;
    rk_err_t ret;
    uint32 ClusCnt, Clus;
    uint32 PrevClus, PrevSec, NewSec;
    uint32 i;
    uint32 SecPerClus, FirstSec, SecCnt;

    if(pstFatFile == NULL)
    {
        return RK_PARA_ERR;
    }

    PrevSec = pstFatFile->Sec;
    PrevClus = pstFatFile->CurClus;
    SecPerClus = pstFatFile->pstFatDir->pstFatDev->BPB_SecPerClus;
    FirstSec = pstFatFile->pstFatDir->pstFatDev->FirstDataSector;

    switch (pos)
    {
        case SEEK_END:                     /* 从文件尾计算 */
             pstFatFile->FileOffset = pstFatFile->FileSize - Offset;
            break;

        case SEEK_SET:
            pstFatFile->FileOffset = Offset;
            break;

        case SEEK_CUR:                  /* 从当前位置计算 */
            pstFatFile->FileOffset += Offset;
            break;

        default:
            return RK_PARA_ERR;

    }

    NewSec = (pstFatFile->FileOffset) / 512;

    ClusCnt = 0;
    Clus = pstFatFile->CurClus;

    if(NewSec > pstFatFile->Sec)
    {
        SecCnt = NewSec - pstFatFile->Sec;

        if(SecCnt < (SecPerClus - (pstFatFile->Sec % SecPerClus)))
        {
            pstFatFile->CurSec += SecCnt;
        }
        else
        {
            ret = FATGetNextClus(pstFatFile->pstFatDir->pstFatDev, pstFatFile->CurClus);
            if(ret <= 0)
            {
                return RK_ERROR;
            }

            if ((ret == EOF_CLUS_END) || (ret == BAD_CLUS)
                || (ret == EMPTY_CLUS) || (ret == EMPTY_CLUS_1) || (ret == EOF_CLUS_1))
            {
                return RK_ERROR;
            }

            Clus = (uint32)ret;

            ClusCnt = (SecCnt - (SecPerClus - (pstFatFile->Sec % SecPerClus)))
                   / SecPerClus;
        }

        pstFatFile->Sec = NewSec;

    }
    else if(NewSec < pstFatFile->Sec)
    {
        SecCnt = pstFatFile->Sec - NewSec;
        if(SecCnt <= pstFatFile->Sec % SecPerClus)
        {
            pstFatFile->CurSec -= SecCnt;
        }
        else
        {
            Clus = pstFatFile->Clus;
            ClusCnt = NewSec / SecPerClus;
        }
        pstFatFile->Sec = NewSec;

       // printf("newsec = %d, ClusCnt = %d, SecPerClus = %d", NewSec, ClusCnt, SecPerClus);
    }

    for(i = 0; i < ClusCnt; i++)
    {
        ret = FATGetNextClus(pstFatFile->pstFatDir->pstFatDev, Clus);
        if(ret <= 0)
        {
            return RK_ERROR;
        }

        if ((ret == EOF_CLUS_END) || (ret == BAD_CLUS)
                || (ret == EMPTY_CLUS) || (ret == EMPTY_CLUS_1) || (ret == EOF_CLUS_1))
        {
            return RK_ERROR;
        }

        Clus = (uint32)ret;
    }

    pstFatFile->CurClus = Clus;
    pstFatFile->CurSec = FirstSec + (Clus - 2) * SecPerClus + pstFatFile->Sec % SecPerClus;

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: FATDevRead
** Input:HDC dev,uint32 pos, void* buffer, uint32 size,uint8 mode,pRx_indicate Rx_indicate
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_ReadFile(HDC dev, uint8* buffer, uint32 len)
{
    FAT_FILE_CLASS * pstFatFile = (FAT_FILE_CLASS *)dev;
    rk_err_t ret;
    uint32 Sec, SecCnt, RealLen, TotalSec;
    uint8 buf[512];

    RealLen = 0;

    if(pstFatFile->FileOffset >= pstFatFile->FileSize)
    {
        return RK_ERROR;
    }

    if((pstFatFile->FileSize - pstFatFile->FileOffset) < len)
    {
        len = pstFatFile->FileSize - pstFatFile->FileOffset;
    }


    TotalSec = len / 512;

    while(TotalSec > 0)
    {
        SecCnt = TotalSec;




        ret = FatCheckSecContinue(pstFatFile, &Sec, &SecCnt);
        if(ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        //rk_printf("read Sec =  %d,  SecCnt = %d", Sec, SecCnt);

        ret = ParDev_Read(pstFatFile->pstFatDir->pstFatDev->hPar, Sec, buffer, SecCnt);
        if(ret != (rk_err_t)SecCnt)
        {
           return RK_ERROR;
        }

        TotalSec -= SecCnt;
        buffer += (SecCnt * 512);
        RealLen += SecCnt * 512;

    }


    if(len % 512)
    {
        SecCnt = 1;

        ret = FatCheckSecContinue(pstFatFile, &Sec, &SecCnt);
        if(ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        //rk_printf("read Sec =  %d,  SecCnt = %d", Sec, SecCnt);

        ret = ParDev_Read(pstFatFile->pstFatDir->pstFatDev->hPar, Sec, buf, SecCnt);
        if(ret != (rk_err_t)SecCnt)
        {
           return RK_ERROR;
        }

        memcpy(buffer, buf, len % 512);
        RealLen += (len % 512);

    }


    pstFatFile->FileOffset += RealLen;

    //rk_printf("RealLen = %d", RealLen);

    return (rk_err_t)RealLen;

}


/*******************************************************************************
** Name: FATOpenFile
** Input:HDC dev, void * args
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.3.26
** Time: 9:03:46
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API HDC FatDev_OpenFile(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FAT_DEVICE_CLASS * pstFatDev = (FAT_DEVICE_CLASS *)dev;

    FAT_FILE_CLASS * pstFatFile;
    FAT_DIR_CLASS * pstFatDir;

    uint16 PathLen;
    uint16 * pPath;
    uint8 Sname[13];

    uint8 Path[518];

    FDT     temp;
    uint32 Clus, Index;

    if ((hFather == NULL) && (pstFileAttr->Path == NULL) && (pstFileAttr->FileName == NULL))
    {
        return (HDC)RK_PARA_ERR;
    }

    memcpy(Path, pstFileAttr->Path, StrLenW(pstFileAttr->Path) * 2 + 2);

    if(pstFileAttr->FileName)
    {
        memcpy(Path + StrLenW(pstFileAttr->Path) * 2, pstFileAttr->FileName, StrLenW(pstFileAttr->FileName) * 2 + 2);
    }

    pPath = (uint16 *)Path;
    PathLen = StrLenW(pPath);

    pPath += PathLen;

    while(*pPath != '\\')
    {
        PathLen--;
        pPath--;
    }

    *pPath = 0;


    pstFatFile = rkos_memory_malloc(sizeof(FAT_FILE_CLASS));
    pstFatDir = rkos_memory_malloc(sizeof(FAT_DIR_CLASS));

    if((pstFatFile == NULL) || (pstFatDir == NULL))
    {
        rkos_memory_free(pstFatFile);
        rkos_memory_free(pstFatDir);
        return (HDC)RK_ERROR;
    }

    if (hFather != NULL)
    {
        Clus = ((FAT_DIR_CLASS *)hFather)->DirClus;

        if (pstFileAttr->Path != NULL)
        {
            Clus = GetDirClusIndex(pstFatDev, (uint16 *)Path, StrLenW((uint16 *)Path), Clus);
        }
    }
    else
    {
        Clus = GetDirClusIndex(pstFatDev, (uint16 *)Path, StrLenW((uint16 *)Path), pstFatDev->BPB_RootClus);
    }


    if (FsIsLongName(pPath + 1, StrLenW(pPath + 1)) == 1)
    {
        if (FindFDTInfoLong(pstFatDev, &temp, Clus, &Index, pPath + 1) != RK_SUCCESS)
        {
            rk_printf("no find file");
            goto ERROR1;
        }

        if (temp.Attr & ATTR_DIRECTORY)
        {
            goto ERROR1;
        }
    }
    else
    {
        FSRealname(Sname, pPath + 1);
        if (FindFDTInfo(pstFatDev, &temp, Clus, &Index, Sname) != RK_SUCCESS)
        {
            goto ERROR1;
        }

        if (temp.Attr & ATTR_DIRECTORY)
        {
            goto ERROR1;
        }
    }
    pstFatDir->pstFatDev = pstFatDev;
    pstFatDir->DirClus = Clus;
    pstFatDir->CurClus = Clus;
    pstFatDir->Index = Index;
    pstFatDir->CurIndex = Index % (pstFatDev->BPB_SecPerClus * 16);


    pstFatFile->Clus = (temp.FstClusHI << 16) | temp.FstClusLO;
    pstFatFile->FileSize = temp.FileSize;
    pstFatFile->FileOffset = 0;
    pstFatFile->CurClus = (temp.FstClusHI << 16) | temp.FstClusLO;
    pstFatFile->CurClusS = 0;
    pstFatFile->CurSec = pstFatDev->FirstDataSector + (pstFatFile->Clus - 2) * pstFatDev->BPB_SecPerClus;
    pstFatFile->Sec = 0;
    pstFatFile->TotalSec = pstFatFile->FileSize / 512 + (pstFatFile->FileSize % 512? 1:0);
    pstFatFile->pstFatDir = pstFatDir;
    pstFatFile->WriteFlag = 0;

    pstFileAttr->CrtDate = temp.CrtDate;
    pstFileAttr->CrtTime = temp.CrtTime;
    pstFileAttr->CrtTimeTenth = temp.CrtTimeTenth;
    pstFileAttr->FileSize = temp.FileSize;
    memcpy(pstFileAttr->ShortName, temp.Name, 11);
    pstFileAttr->LstAccDate = temp.LstAccDate;
    pstFileAttr->WrtDate = temp.WrtDate;
    pstFileAttr->WrtTime = temp.WrtTime;


    return (HDC)pstFatFile;

ERROR1:
    rkos_memory_free(pstFatFile);
    rkos_memory_free(pstFatDir);
    return (HDC)RK_ERROR;

}


/*******************************************************************************
** Name: FATOpenDir
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.3.26
** Time: 14:37:26
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API HDC FatDev_OpenDir(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FAT_DEVICE_CLASS * pstFatDev = (FAT_DEVICE_CLASS *)dev;

    FAT_DIR_CLASS * pstFatDir;
    FDT     temp;
    uint32 Clus, Index;

    if ((hFather == NULL) && (pstFileAttr->Path == NULL))
    {
        return (HDC)RK_PARA_ERR;
    }

    if ((hFather != NULL) && (pstFileAttr->FileName == NULL))
    {
        return (HDC)RK_PARA_ERR;
    }

    pstFatDir = rkos_memory_malloc(sizeof(FAT_DIR_CLASS));
    if(pstFatDir == NULL)
    {
        return (HDC)RK_ERROR;
    }

    if (hFather != NULL)
    {
        Clus = ((FAT_DIR_CLASS *)hFather)->DirClus;

        if (pstFileAttr->Path != NULL)
        {
            Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), Clus);
        }
    }
    else
    {
        Clus = GetDirClusIndex(pstFatDev, pstFileAttr->Path, StrLenW(pstFileAttr->Path), pstFatDev->BPB_RootClus);
    }

//    if (Clus < 0)
//    {
//        goto ERROR;     /*never run  here*/
//    }

    if (pstFileAttr->FileName != NULL)
    {
        if (FsIsLongName(pstFileAttr->FileName, StrLenW(pstFileAttr->FileName)) == RK_SUCCESS)
        {
            if (FindFDTInfoLong(pstFatDev, &temp, Clus, &Index, pstFileAttr->FileName) != RK_SUCCESS)
            {
                goto ERROR1;
            }

            if (!(temp.Attr & ATTR_DIRECTORY))
            {
                goto ERROR1;
            }
        }
        else
        {
            FSRealname(temp.Name, pstFileAttr->FileName);
            if (FindFDTInfo(pstFatDev, &temp, Clus, &Index, temp.Name) != RK_SUCCESS)
            {
                goto ERROR1;
            }

            if (!(temp.Attr & ATTR_DIRECTORY))
            {
                goto ERROR1;
            }

        }

        pstFatDir->DirClus = (temp.FstClusHI << 16) | temp.FstClusLO;
    }
    else
    {
        pstFatDir->DirClus = Clus;
    }

    pstFatDir->CurClus = Clus;
    pstFatDir->CurIndex = (uint32)-1;
    pstFatDir->Index = (uint32)-1;
    pstFatDir->pstFatDev = pstFatDev;
    return pstFatDir;

ERROR1:

    return (HDC)RK_ERROR;

}


/*******************************************************************************
** Name: FATGetFileName
** Input:HDC dev,  uint16 * FileName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.28
** Time: 14:15:13
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_GetFileName(HDC dev,  uint16 * FileName)
{
    FAT_DIR_CLASS * phDir = (FAT_DIR_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;
    rk_err_t ret;
    FDT fdt;
    uint32 LfnCnt, LfnOk;
    uint32 Clus, CurClus, CurIndex, Index;

    uint8 CheckSum;

    uint16 *strCur;
    uint8 *buf;
    uint32 j;

    uint32 TotalChar;

    if (dev == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = phDir->pstFatDev;

    if (pstFatDev == NULL)
    {
        return RK_ERROR;
    }

    LfnCnt = 0;
    LfnOk = 0;

    Clus = phDir->DirClus;
    CurClus = phDir->CurClus;
    CurIndex = phDir->CurIndex;
    Index = phDir->Index;

    TotalChar = 0;

    ret =  GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);

    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    CheckSum = FatCheckSum(fdt.Name);

    do
    {
        if (CurIndex == 0)
        {
            ret = FATGetPrevClus(pstFatDev, Clus, CurClus);

            if (ret < 0)
            {
                return RK_ERROR;
            }

            CurClus = (uint32)ret;

            CurIndex = (pstFatDev->BPB_BytsPerSec * pstFatDev->BPB_SecPerClus) / 32 - 1;
        }
        else
        {
            CurIndex--;
        }

        Index--;

        ret = GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);
        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if ((fdt.Name[0] & LFN_SEQ_MASK) != (LfnCnt + 1))
        {
            break;
        }

        if (fdt.Name[13] != CheckSum)
        {
            break;
        }

        LfnCnt++;

        strCur = FileName + (LfnCnt - 1) * 13;

        /*获取文件名*/
        buf = (uint8 *)&fdt;
        buf++;
        for (j = 0; j < 5; j++)
        {//前面10个是byte
            *strCur = *(uint16 *)buf;
            //printf("char = %04x\n", *plfName);
            buf += 2;
            //*plfName |= ((uint16)(*buf++))<<8;
            strCur++;
            TotalChar++;
            if (TotalChar >= SYS_SUPPROT_STRING_MAX_LEN)
            {
                LfnOk = 1;
                break;
            }
        }
        buf += 3;

        for (j = 0; j<6;j++)
        {
            *strCur = *(uint16 *)buf;
            //printf("char = %04x\n", *plfName);
            buf += 2;
            //*plfName |= ((uint16)(*buf++))<<8;
            strCur++;
            TotalChar++;
            if (TotalChar >= SYS_SUPPROT_STRING_MAX_LEN)
            {
                LfnOk = 1;
                break;
            }
        }
        buf += 2;

        for (j = 0; j<2; j++)
        {
            *strCur = *(uint16 *)buf;
            //printf("char = %04x\n", *plfName);
            buf += 2;
            //*plfName |= ((uint16)(*buf++))<<8;
            strCur++;
            TotalChar++;
            if (TotalChar >= SYS_SUPPROT_STRING_MAX_LEN)
            {
                LfnOk = 1;
                break;
            }
        }

        if (fdt.Name[0] & 0X40)
        {
            LfnOk = 1;
            break;
        }

        if (Index == 0)
        {
            break;
        }

    }
    while (1);

    if (LfnOk == 0)
    {
        CurClus = phDir->CurClus;
        CurIndex = phDir->CurIndex;

        ret =  GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if(fdt.NTRes & 0x08)
        {
            for(j = 0; j < 8; j++)
            {
                if(fdt.Name[j] != 0x20)
                {
                    fdt.Name[j] += 0x20;
                }
            }
        }

        if(fdt.NTRes & 0x10)
        {
            for(j = 8; j < 11; j++)
            {
                if(fdt.Name[j] != 0x20)
                {
                    fdt.Name[j] += 0x20;
                }
            }
        }

        ret = FATGetLongFromShortName(FileName, fdt.Name);

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }
    }
    else
    {
        FileName[TotalChar] = 0;
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: FATCloseFile
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.21
** Time: 10:27:27
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_CloseFile(HDC dev)
{
    FAT_FILE_CLASS * pstFatFile = (FAT_FILE_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;

    uint32 ByteIndex, SecIndex;
    rk_err_t ret;
    FDT Fdt;
    uint32 i;


    if (pstFatFile == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = pstFatFile->pstFatDir->pstFatDev;


    if(pstFatFile->WriteFlag == 1)
    {
        ByteIndex = pstFatFile->pstFatDir->CurIndex * 32;
            SecIndex  = pstFatFile->pstFatDir->pstFatDev->FirstDataSector
                + (pstFatFile->pstFatDir->CurClus - 2) * pstFatFile->pstFatDir->pstFatDev->BPB_SecPerClus;

        ret = ReadFDTInfo(pstFatDev, &Fdt, SecIndex, ByteIndex);

        rk_printf("SecIndex = %d, ByteIndex = %d", SecIndex, ByteIndex);

        if(ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        rk_printf("pstFatFile->FileSize = %d, pstFatFile->Clus = %d", pstFatFile->FileSize, pstFatFile->Clus);

        Fdt.FileSize = pstFatFile->FileSize;
        Fdt.FstClusHI = pstFatFile->Clus >> 16;
        Fdt.FstClusLO = pstFatFile->Clus;

        ret = WriteFDTInfo(pstFatDev, &Fdt, SecIndex, ByteIndex);

        if(ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }
    }



    rkos_semaphore_take(pstFatDev->osFatOperReqSem, MAX_DELAY);
    for(i = 0; i < 3; i++)
    {
        if (pstFatDev->stFdtCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[i], pstFatDev->pbFdt[i]))
            {
                return RK_ERROR;
            }
        }

        if (pstFatDev->stFatCache.Flag[i])
        {
            pstFatDev->stFatCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[i], pstFatDev->pwFat[i]))
            {
                return RK_ERROR;
            }
        }
    }
    rkos_semaphore_give(pstFatDev->osFatOperReqSem);

    rkos_memory_free(pstFatFile->pstFatDir);
    rkos_memory_free(pstFatFile);

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: FATCloseDir
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.26
** Time: 14:29:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_CloseDir(HDC dev)
{
    FAT_DIR_CLASS * pstFatDir = (FAT_DIR_CLASS *)dev;

    FAT_DEVICE_CLASS * pstFatDev = pstFatDir->pstFatDev;
    uint32 i;


    for(i = 0; i < 3; i++)
    {
        if (pstFatDev->stFdtCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[i], pstFatDev->pbFdt[i]))
            {
                return RK_ERROR;
            }
        }

        if (pstFatDev->stFatCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[i], pstFatDev->pwFat[i]))
            {
                return RK_ERROR;
            }
        }
    }

    if (pstFatDir == NULL)
    {
        return RK_PARA_ERR;
    }

    rkos_memory_free(pstFatDir);

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: FATNextFile
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:01:31
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_NextFile(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
{
    FAT_DIR_CLASS * phDir = (FAT_DIR_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;
    rk_err_t ret;
    FDT fdt;

    uint32 Index, CurClus, PrevClus, CurIndex, CurDirNum;

    if (phDir == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = (FAT_DEVICE_CLASS *)phDir->pstFatDev;

    if (pstFatDev == NULL)
    {
        return RK_ERROR;
    }

    Index = phDir->Index;
    CurClus = phDir->CurClus;
    CurIndex = phDir->CurIndex;
    PrevClus = CurClus;

    do
    {
        CurIndex++;
        Index++;
        CurDirNum++;

        if (PrevClus != CurClus)
        {
            PrevClus = CurClus;
            CurIndex = 0;
        }
        ret = GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);
        if ((ret == FDT_OVER) || (fdt.Name[0] == 0))
        {
            phDir->CurClus = CurClus;
            phDir->CurIndex = CurIndex;
            phDir->Index = Index;
            printf("FDT_OVER\n");
            return  FDT_OVER;
        }

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (fdt.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (fdt.Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID))
        {
            continue;
        }

        if (fdt.Attr & FileMaskAttr)
        {
            continue;
        }

        if (ExtName != NULL)
        {
            if (FileExtNameMatch(&fdt.Name[8], ExtName) == RK_SUCCESS)
            {
                phDir->CurClus = CurClus;
                phDir->CurIndex = CurIndex;
                phDir->Index = Index;
                pstFileAttr->CrtDate = fdt.CrtDate;
                pstFileAttr->CrtTime = fdt.CrtTime;
                pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
                pstFileAttr->LstAccDate = fdt.LstAccDate;
                pstFileAttr->WrtDate = fdt.WrtDate;
                pstFileAttr->WrtTime = fdt.WrtTime;
                memcpy(pstFileAttr->ShortName, fdt.Name, 11);
                return RK_SUCCESS;
            }
        }
        else
        {
            phDir->CurClus = CurClus;
            phDir->CurIndex = CurIndex;
            phDir->Index = Index;
            pstFileAttr->CrtDate = fdt.CrtDate;
            pstFileAttr->CrtTime = fdt.CrtTime;
            pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
            pstFileAttr->LstAccDate = fdt.LstAccDate;
            pstFileAttr->WrtDate = fdt.WrtDate;
            pstFileAttr->WrtTime = fdt.WrtTime;
            memcpy(pstFileAttr->ShortName, fdt.Name, 11);
            return RK_SUCCESS;
        }
    }
    while (1);
}


/*******************************************************************************
** Name: FATNextDir
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:01:56
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_NextDir(HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr)
{
    FAT_DIR_CLASS * phDir = (FAT_DIR_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;
    rk_err_t ret;
    FDT fdt;

    uint32 Index, CurClus,PrevClus, CurIndex;

    if (phDir == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = (FAT_DEVICE_CLASS *)phDir->pstFatDev;

    if (pstFatDev == NULL)
    {
        return RK_ERROR;
    }

    Index = phDir->Index;

    CurClus = phDir->CurClus;
    CurIndex = phDir->CurIndex;

    PrevClus = CurClus;

    do
    {
        CurIndex++;
        Index++;

        if (PrevClus != CurClus)
        {
            PrevClus = CurClus;
            CurIndex = 1;
        }


        ret = GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);
        if ((ret == FDT_OVER) || (fdt.Name[0] == 0))
        {
            phDir->CurClus = CurClus;
            phDir->CurIndex = CurIndex;
            phDir->Index = Index;
            return  FDT_OVER;
        }

        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (fdt.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (fdt.Name[0] == '.')
        {
            continue;
        }

        if (!(fdt.Attr & ATTR_DIRECTORY))
        {
            continue;
        }

        if (fdt.Attr & DirMaskAttr)
        {
            continue;
        }

        phDir->CurClus = CurClus;
        phDir->CurIndex = CurIndex;
        phDir->Index = Index;
        pstFileAttr->CrtDate = fdt.CrtDate;
        pstFileAttr->CrtTime = fdt.CrtTime;
        pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
        pstFileAttr->LstAccDate = fdt.LstAccDate;
        pstFileAttr->WrtDate = fdt.WrtDate;
        pstFileAttr->WrtTime = fdt.WrtTime;

        return RK_SUCCESS;

    }
    while (1);
}


/*******************************************************************************
** Name: FATPrevFile
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:02:20
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_PrevFile(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
{
    FAT_DIR_CLASS * phDir = (FAT_DIR_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;
    rk_err_t ret;
    FDT fdt;

    uint32 Clus, Index, CurClus, CurIndex;

    if (phDir == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = (FAT_DEVICE_CLASS *)phDir->pstFatDev;

    if (pstFatDev == NULL)
    {
        return RK_ERROR;
    }

    if (phDir->Index == 0)
    {
        return  FDT_OVER;
    }

    Clus = phDir->DirClus;
    Index = phDir->Index;

    CurClus = phDir->CurClus;
    CurIndex = phDir->CurIndex;

    do
    {
        CurIndex--;
        Index--;

        if (CurIndex == 0)
        {
            if (CurClus != Clus)
            {
                ret = FATGetPrevClus(dev, Clus, CurClus);

                if (ret < 0)
                {
                    return RK_ERROR;
                }

                CurClus = (uint32)ret;
            }
            else
            {
                phDir->CurClus = CurClus;
                phDir->CurIndex = CurIndex;
                phDir->Index = Index;
                return  FDT_OVER;
            }

            CurIndex = (pstFatDev->BPB_BytsPerSec * pstFatDev->BPB_SecPerClus) / 32 - 1;

        }

        ret = GetFDTInfo(dev, &fdt, &CurClus, CurIndex);
        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (fdt.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (fdt.Attr & (ATTR_DIRECTORY | ATTR_VOLUME_ID))
        {
            continue;
        }

        if (fdt.Attr & FileMaskAttr)
        {
            continue;
        }

        if (ExtName != NULL)
        {
            if (FileExtNameMatch(&fdt.Name[8], ExtName) == RK_SUCCESS)
            {
                phDir->CurClus = CurClus;
                phDir->CurIndex = CurIndex;
                phDir->Index = Index;
                pstFileAttr->CrtDate = fdt.CrtDate;
                pstFileAttr->CrtTime = fdt.CrtTime;
                pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
                pstFileAttr->LstAccDate = fdt.LstAccDate;
                pstFileAttr->WrtDate = fdt.WrtDate;
                pstFileAttr->WrtTime = fdt.WrtTime;
                return RK_SUCCESS;
            }
        }
        else
        {
            phDir->CurClus = CurClus;
            phDir->CurIndex = CurIndex;
            phDir->Index = Index;
            pstFileAttr->CrtDate = fdt.CrtDate;
            pstFileAttr->CrtTime = fdt.CrtTime;
            pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
            pstFileAttr->LstAccDate = fdt.LstAccDate;
            pstFileAttr->WrtDate = fdt.WrtDate;
            pstFileAttr->WrtTime = fdt.WrtTime;
            return RK_SUCCESS;
        }
    }
    while (1);

}


/*******************************************************************************
** Name: FATPrevDir
** Input:HDC dev, FILE_ATTR * pstFatArg
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 17:02:57
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON API rk_err_t FatDev_PrevDir(HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr)
{
    FAT_DIR_CLASS * phDir = (FAT_DIR_CLASS *)dev;
    FAT_DEVICE_CLASS * pstFatDev;
    rk_err_t ret;
    FDT fdt;

    uint32 Clus, Index, CurClus, CurIndex;

    if (phDir == NULL)
    {
        return RK_PARA_ERR;
    }

    pstFatDev = (FAT_DEVICE_CLASS *)phDir->pstFatDev;

    if (pstFatDev == NULL)
    {
        return RK_ERROR;
    }

    if (phDir->Index == 0)
    {
        return  FDT_OVER;
    }

    Clus = phDir->DirClus;
    Index = phDir->Index;

    CurClus = phDir->CurClus;
    CurIndex = phDir->CurIndex;

    do
    {
        CurIndex--;
        Index--;

        if (CurIndex == 0)
        {
            if (CurClus != Clus)
            {
                ret = FATGetPrevClus(pstFatDev, Clus, CurClus);

                if (ret < 0)
                {
                    return RK_ERROR;
                }


                CurClus = (uint32)ret;
            }
            else
            {
                phDir->CurClus = CurClus;
                phDir->CurIndex = CurIndex;
                phDir->Index = Index;
                return  FDT_OVER;
            }

            CurIndex = (pstFatDev->BPB_BytsPerSec * pstFatDev->BPB_SecPerClus) / 32 - 1;

        }

        ret = GetFDTInfo(pstFatDev, &fdt, &CurClus, CurIndex);
        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }

        if (fdt.Name[0] == FILE_DELETED)
        {
            continue;
        }

        if (fdt.Name[0] == '.')
        {
            continue;
        }

        if (!(fdt.Attr & ATTR_DIRECTORY))
        {
            continue;
        }

        if (fdt.Attr & DirMaskAttr)
        {
            continue;
        }

        phDir->CurClus = CurClus;
        phDir->CurIndex = CurIndex;
        phDir->Index = Index;
        pstFileAttr->CrtDate = fdt.CrtDate;
        pstFileAttr->CrtTime = fdt.CrtTime;
        pstFileAttr->CrtTimeTenth = fdt.CrtTimeTenth;
        pstFileAttr->LstAccDate = fdt.LstAccDate;
        pstFileAttr->WrtDate = fdt.WrtDate;
        pstFileAttr->WrtTime = fdt.WrtTime;
        return RK_SUCCESS;

    }
    while (1);

}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FatCheckSecContinue
** Input:FAT_FILE_CLASS * pstFatFile,  uint32 SecCnt
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.5.13
** Time: 16:15:26
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FatCheckSecContinue(FAT_FILE_CLASS * pstFatFile, uint32 * LBA,  uint32 * SecCnt)
{
    uint32 Cnt, ClusCnt, i;
    uint32 PrevClus, CurClus, CurSec, Sec, TotalSec;
    uint32 SecPerClus, FirstSec;
    rk_err_t ret;

    Cnt = *SecCnt;

    CurClus = pstFatFile->CurClus;
    CurSec = pstFatFile->CurSec;
    TotalSec = pstFatFile->TotalSec;
    SecPerClus = pstFatFile->pstFatDir->pstFatDev->BPB_SecPerClus;
    FirstSec = pstFatFile->pstFatDir->pstFatDev->FirstDataSector;
    Sec = pstFatFile->Sec;

    if(Cnt <= (SecPerClus - Sec % SecPerClus))
    {
         *LBA = pstFatFile->CurSec;
         pstFatFile->Sec += Cnt;
         pstFatFile->CurSec += Cnt;

         if((pstFatFile->CurSec % SecPerClus) == 0)
         {
            CurClus = FATGetNextClus(pstFatFile->pstFatDir->pstFatDev, pstFatFile->CurClus);
         }

         if(CurClus == EOF_CLUS_END)
         {
            pstFatFile->CurClusS = EOF_CLUS_END;
         }
         else
         {
            pstFatFile->CurClus = CurClus;
         }

         //printf("\n pstFatFile->CurClus = %d",  pstFatFile->CurClus);
         return RK_SUCCESS;
    }
    else
    {
        ClusCnt = ((Cnt - (SecPerClus - Sec % SecPerClus)) / SecPerClus) + (((Cnt - (SecPerClus - Sec % SecPerClus)) % SecPerClus)?1:0);

        for(i = 0; i < ClusCnt; i++)
        {
            ret = FATGetNextClus(pstFatFile->pstFatDir->pstFatDev, CurClus);
            if(ret <= 0)
            {
                return RK_ERROR;
            }

            if ((ret == EOF_CLUS_END) || (ret == BAD_CLUS)
                || (ret == EMPTY_CLUS) || (ret == EMPTY_CLUS_1) || (ret == EOF_CLUS_1))
            {
                rk_printf("get clus error");
                return RK_ERROR;
            }

            PrevClus = CurClus;
            CurClus = (uint32)ret;

            if(PrevClus != (CurClus - 1))
            {
                break;
            }

        }

    }

    *LBA = pstFatFile->CurSec;
    *SecCnt = (SecPerClus - Sec % SecPerClus) + (i - 1) * SecPerClus
        + ((i == ClusCnt)? ((Cnt - (SecPerClus - Sec % SecPerClus)) % SecPerClus) : SecPerClus);

    pstFatFile->Sec += *SecCnt;
    pstFatFile->CurClus = CurClus;

    //printf("\n pstFatFile->CurClus = %d", CurClus);

    pstFatFile->CurSec = FirstSec + (CurClus - 2) * SecPerClus + pstFatFile->Sec % SecPerClus;

    return RK_SUCCESS;


}


/*******************************************************************************
** Name: FATGetLongFromShortName
** Input:uint16 *  LongName, uint8 * ShortName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.28
** Time: 14:47:43
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATGetLongFromShortName(uint16 *  LongName, uint8 * ShortName)
{
    uint32 LongCnt, i, j;

    LongCnt = (uint32)Ascii2Unicode(ShortName, LongName, 16);
    if ((LongCnt <= 0) || (LongCnt > 16))
    {
        return RK_ERROR;
    }

    LongName[LongCnt / 2] = '.';

    LongCnt = (uint32)Ascii2Unicode(ShortName + 8, LongName + (LongCnt + 2) / 2,  6);

    if ((LongCnt <= 0) || (LongCnt > 6))
    {
        return RK_ERROR;
    }

    i = 0;
    j = 0;
    do
    {
        if (*(LongName + j) == 0)
        {
            *(LongName + i) = 0;
            break;
        }
        else if (*(LongName + j) != 0x20)
        {
            *(LongName + i) = *(LongName + j);
            i++;
        }
        else if((*(LongName + j) == 0x20) && (*(LongName + i - 1) == '.'))
        {
            *(LongName + i - 1) = 0;
            break;
        }
        j++;

    }
    while (1);

    return RK_SUCCESS;

}



/*******************************************************************************
** Name: FATGetPrevClus
** Input:HDC dev, uint32 Index
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.28
** Time: 9:26:41
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATGetPrevClus(FAT_DEVICE_CLASS * pstFatDev, uint32 FirstClus, uint32 Clus)
{
    rk_err_t ret;

    if (Clus >= pstFatDev->CountofClusters + 2)
    {
        return (RK_ERROR);
    }

    if (FirstClus >= pstFatDev->CountofClusters + 2)
    {
        return (RK_ERROR);
    }

    /* 计算扇区号和字节索引 */

    do
    {
        ret = FATGetNextClus(pstFatDev, FirstClus);

        if (ret < 0)
        {
            return  RK_ERROR;
        }

        if ((ret == EOF_CLUS_END) || (ret == BAD_CLUS)
                || (ret == EMPTY_CLUS) || (ret == EMPTY_CLUS_1) || (ret == EOF_CLUS_1))
        {
            return RK_ERROR;
        }

        if ((uint32)ret == Clus)
        {
            return (rk_err_t)FirstClus;
        }

        FirstClus = (uint32)ret;

    }
    while (1);

}

/*******************************************************************************
** Name: FileExtNameMatch
** Input:uint8 * Name, uint8 * ExtName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.27
** Time: 16:30:25
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FileExtNameMatch(uint8 * Name, uint8 * ExtName)
{

    while (*ExtName != '\0')
    {
        if (Name[0] == ExtName[0])
        {
            if (Name[1] == ExtName[1])
            {
                if (Name[2] == ExtName[2])
                {

                    return (RK_SUCCESS);
                }
            }
        }
        ExtName += 3;
    }
    return (FALSE);
}

/*******************************************************************************
** Name: FatCheckSum
** Input:uint8 * pFileName
** Return: uint8
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 14:16:13
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN uint8 FatCheckSum(uint8 * pFileName)
{
    uint8 i;
    uint8 Sum;
    Sum = 0;
    for (i = 11; i!=0; i--)
    {
        Sum = ((Sum & 1) ? 0x80 : 0) + (Sum >> 1) + *pFileName++;
    }
    return (Sum);
}


/*******************************************************************************
** Name: FindFDTInfoLong
** Input:HDC dev, FDT * Rt, uint32 ClusIndex, uint32 * pIndex, uint16 * FileName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 10:28:09
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FindFDTInfoLong(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 ClusIndex, uint32 * pIndex, uint16 * FileName)
{
    uint32 i,k,Index;

    uint8 j;
    uint16 state = 0;
    uint16 *strCur, *plfName;
    uint8 *buf;
    uint16 lfName[13];

    uint32 PrevClusIndex;

    uint32 LfnCnt;
    uint8 CheckSum;

    rk_err_t ret;

    LfnCnt = StrLenW(FileName) / 13 + (StrLenW(FileName) % 13? 1:0);

    if (LfnCnt > MAX_LFN_ENTRIES)
    {
        LfnCnt = MAX_LFN_ENTRIES;
    }

    i = 0;
    k = 0;

    if (FileName[0] == FILE_DELETED)
    {
        FileName[0] = ESC_FDT;
    }

    PrevClusIndex = ClusIndex;

    while (1)
    {
        ret = GetFDTInfo(pstFatDev, Rt, &ClusIndex, i);        //返回RETURN_OK\NOT_FAT_DISK\FDT_OVER

        if (PrevClusIndex != ClusIndex)
        {
            PrevClusIndex = ClusIndex;
            i = 0;
        }

        if (ret != RK_SUCCESS)
        {
            ret = RK_ERROR;
            break;
        }

        if (Rt->Name[0] == FILE_NOT_EXIST)
        {
            ret = RK_ERROR;
            break;
        }

        if (Rt->Name[0] == FILE_DELETED)
        {
            if (state == 2)
            {
                state = 0;
            }
            i++;
            k++;
            continue;
        }

        if (Rt->Attr != ATTR_LFN_ENTRY)
        {
            if (state != 2)
            {
                i++;
                k++;
                continue;
            }
        }

        if (state == 0)
        {
            if(Rt->Name[0] == (LfnCnt | 0x40)) //first          //找到长文件名的第1项
            {
                state = 1;
                Index = LfnCnt;
                CheckSum = Rt->Name[13];
            }
            else
            {
                i++;
                k++;
                continue;
            }
        }

        if (state == 1)
        {
            if (CheckSum != Rt->Name[13])
            {
                i++;
                k++;
                state = 0;
                continue;
            }

            if ((Rt->Name[0] & LFN_SEQ_MASK) != Index)
            {
                i++;
                k++;
                state = 0;
                continue;
            }

            strCur = FileName + (Index - 1) * 13;

            /*获取文件名*/
            buf = (uint8 *)Rt;
            plfName = lfName;
            buf++;
            for (j = 0; j<5; j++)
            {//前面10个是byte
                *plfName = *(uint16 *)buf;
                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;
                plfName++;
            }
            buf += 3;

            for (j = 0; j<6;j++)
            {
                *plfName = *(uint16 *)buf;
                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;
                plfName++;
            }
            buf += 2;

            for (j = 0; j<2; j++)
            {
                *plfName = *(uint16 *)buf;
                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;
                plfName++;
            }

            j = 13;

            plfName = lfName;

            if(StrCmpW(plfName, strCur, (StrLenW(strCur) > 13)? 13 : StrLenW(strCur)) == 0)
            {
                Index--;
                if (Index == 0)
                {
                    state = 2;
                }
                i++;
                k++;
                continue;
            }
            else
            {
                i++;
                k++;
                state = 0;
                continue;
            }

        }

        if (state == 2)
        {
            if (FatCheckSum(Rt->Name) == CheckSum)
            {
                *pIndex = k;
                ret = RK_SUCCESS;
                break;
            }
            else
            {
                state = 0;
                i++;
                k++;
                continue;
            }
        }

    }

    if (FileName[0] == ESC_FDT)
    {
        FileName[0] = FILE_DELETED;
    }

    return (ret);
}

/*******************************************************************************
** Name: FindFDTInfo
** Input:HDC dev, FDT * Rt, uint32 ClusIndex, uint32 Index
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 8:33:58
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FindFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 ClusIndex, uint32 * pIndex,  uint8 *FileName)
{
    uint32 i,k;
    int32 temp, j;
    uint32 PrevClusIndex;

    i = 0;
    k = 0;

    if (FileName[0] == FILE_DELETED)
    {
        FileName[0] = ESC_FDT;
    }

    PrevClusIndex = ClusIndex;

    while (1)
    {
        temp = GetFDTInfo(pstFatDev, Rt, &ClusIndex, i);        //返回RETURN_OK\NOT_FAT_DISK\FDT_OVER
        if (temp != RK_SUCCESS)
        {
            break;
        }

        if (Rt->Name[0] == FILE_NOT_EXIST)
        {
            temp = RK_ERROR;
            break;
        }

        if ((Rt->Attr & ATTR_VOLUME_ID) == 0)
        {
            for (j = 0; j < 11; j++)
            {
                if (FileName[j] != Rt->Name[j])
                {
                    break;
                }
            }

            if (j == 11)
            {
                temp = RK_SUCCESS;
                *pIndex = k;
                break;
            }
        }

        if (PrevClusIndex != ClusIndex)
        {
            PrevClusIndex = ClusIndex;
            i = 0;
        }
        i++;
        k++;

    }

    if (FileName[0] == ESC_FDT)
    {
        FileName[0] = FILE_DELETED;
    }

    return (temp);
}


/*******************************************************************************
** Name: GetDirClusIndex
** Input:HDC dev,uint16 *Path, uint16 len
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.3.17
** Time: 18:07:45
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t GetDirClusIndex(FAT_DEVICE_CLASS * pstFatDev, uint16 *Path, uint16 len, uint32 CurDirClus)
{
    uint16  i;
    uint32 DirClusIndex, Index;
    FDT    temp;
    uint8   SName[13];
    uint16 *start;
    uint8 ret;

    DirClusIndex = BAD_CLUS;

    if (Path != NULL)       //null pointer
    {
//***********************************************************************
//支持盘符如A:
//***********************************************************************
        if (Path[1] == L':')
        {
            Path += 2;
            len -= 2;
        }

        DirClusIndex = pstFatDev->BPB_RootClus; //根目录
//***********************************************************************
//A:TEMP、TEMP和.\TEMP都是指当前目录下的TEMP子目录
        if (Path[0] != L'\\')            //* 不是目录分隔符号,表明起点是当前路径
        {
            DirClusIndex = CurDirClus;
        }
        else
        {
            Path++;
            len--;
        }

        if (Path[0] == L'.')             // '\.'表明起点是当前路径
        {
            DirClusIndex = CurDirClus;
            if (Path[1] == L'\0' || Path[1] == L'\\')    //case "." or ".\"
            {
                Path++;
                len--;
            }
        }

        while (len > 0)
        {
            if (Path[0] == L' ')         //首个字符不允许为空格
            {
                DirClusIndex = BAD_CLUS;
                break;
            }

            start = Path;
            for (i = 1; i < 256; i++)
            {
                Path++;
                len--;
                if (*Path == L'\\')
                {
                    Path++;
                    len--;
                    break;
                }
                else if(*Path == 0)
                {
                    break;
                }
            }

            if (FsIsLongName(start, i))
            {
                *(start + i) = L'\0';
                ret = FindFDTInfoLong(pstFatDev, &temp, DirClusIndex, &Index, start);
                *(start + i) = L'\\';
            }
            else
            {
                FSRealname(SName, start);
                ret = FindFDTInfo(pstFatDev, &temp, DirClusIndex, &Index, SName);
            }

            if (ret != RK_SUCCESS)    //获取FDT信息
            {
                DirClusIndex = BAD_CLUS;
                break;
            }


            if ((temp.Attr & ATTR_DIRECTORY) == 0)  //FDT是否是目录
            {
                DirClusIndex = BAD_CLUS;
                break;
            }

            DirClusIndex = ((uint32)(temp.FstClusHI) << 16) + temp.FstClusLO;
        }
    }

    return (DirClusIndex);

}


/*******************************************************************************
** Name: FSRealname
** Input:uint8 *dname, uint16 *pDirName
** Return: void
** Owner:Aaron.sun
** Date: 2014.3.17
** Time: 18:05:19
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN uint8 FSRealname(uint8 *dname, uint16 *pDirName)
{
    uint8 i;
    uint8 *pdname = dname;
    uint16 len;
    uint8 NTRes = 0;

    if((*pDirName >= 'a') && (*pDirName <= 'z'))
    {
        NTRes |= 0x08;
    }


    for (i = 0; i < 11; i++)        //目录项填空格
    {
        dname[i] = ' ';
    }

    for (i = 0; i < 13; i++)   // 11 + '.' + '\0' = 13
    {
        if (*pDirName == '\0' || *pDirName == '\\' )      //到路径结束,
        {
            break;
        }

        if(*pDirName == '.') //文件名要去掉后缀名的.
        {
            pDirName++;
            pdname = &dname[8];

            if((*pDirName >= 'a') && (*pDirName <= 'z'))
            {
                NTRes |= 0x10;
            }
            continue;
        }

        if((*pDirName >= 'a') && (*pDirName <= 'z'))
        {
            *pdname = *pDirName - ('a' - 'A');
        }
        else
        {
            *pdname = *pDirName;
        }
        pdname++;
        pDirName++;

    }

    return NTRes;

}


/*******************************************************************************
** Name: FsIsLongName
** Input:(uint16 *dname, uint16 len)
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.3.17
** Time: 18:00:33
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN uint32 FsIsLongName(uint16 *dname, uint16 len)
{
    uint32  i = len;
    uint16 *p = dname;
    uint16 dot = 0;
    uint8 UpCase, LowCase;

    if (len > 12)
    {
        return 1;
    }

    while(i--)
    {
        if ( (*p > 127)
            ||(*p == '+')
            ||(*p == '=')
            ||(*p == ',')
            ||(*p == ';')
            ||(*p == '[')
            ||(*p == ']')
            ||(*p == ' ')) //有"+ = , ; [ ]"，也是长文件名
        {
            return 1;
        }
        else if (*p == '.')
        {
            dot++;
            if (dot > 1) /* 小数点多于1个，是长文件名 */
            {
                return 1;
            }

            if ((len-i-1) > 8)  /* 文件主名超过8，是长文件名 */
            {
                return 1;
            }

            if (i > 3)   /* 扩展名超过3，是长文件名 */
            {
                return 1;
            }
        }
        p++;
    }

    if ((len == 12) && (dot == 0))
    {
        return 1;
    }

    UpCase = 0;
    LowCase = 0;
    p = dname;

    for(i = 0; i < 8; i++)
    {
        if((*p >='a') && (*p <='z'))
        {
            LowCase = 1;
        }
        else if((*p >='A') && (*p <='Z'))
        {
            UpCase = 1;
        }
        p++;
        if(*p == '.')
        {
            p++;
            break;
        }
        else if(*p == 0)
        {
           if((LowCase == 1) && (UpCase == 1))
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    if((LowCase == 1) && (UpCase == 1))
    {
        return 1;
    }

    UpCase = 0;
    LowCase = 0;

    for(i = 0; i < 3; i++)
    {
        if((*p >='a') && (*p <='z'))
        {
            LowCase = 1;
        }
        else if((*p >='A') && (*p <='Z'))
        {
            UpCase = 1;
        }
        else if(*p == 0)
        {
           break;
        }

        p++;
    }

    if(*p != 0)
    {
        return 1;
    }

    if((LowCase == 1) && (UpCase == 1))
    {
        return 1;
    }

    return 0;
}

/*******************************************************************************
** Name: ReadFDTInfo
** Input:HDC dev,FDT *Rt, uint32 SecIndex, uint16 ByteIndex
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 16:37:22
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t ReadFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *Rt, uint32 SecIndex, uint16 ByteIndex)
{
    //uint8 i;
    uint8 *pRt=(uint8 *)Rt;
    uint8 *Buf;
    int32  status;
    uint8 bufindex = 0;
    uint32 bufMargin[3];

    status = RK_SUCCESS;

    rkos_semaphore_take(pstFatDev->osFatOperReqSem, MAX_DELAY);

    if (pstFatDev->stFdtCache.Sec[0] == SecIndex)
    {
        bufindex = 0;
    }
    else if (pstFatDev->stFdtCache.Sec[1] == SecIndex)
    {
        bufindex = 1;
    }
    else if (pstFatDev->stFdtCache.Sec[3] == SecIndex)
    {
        bufindex = 2;
    }
    else
    {
        if (pstFatDev->stFdtCache.Flag[0] == 0)
        {
            bufindex = 0;
        }
        else if (pstFatDev->stFdtCache.Flag[1] == 0)
        {
            bufindex = 1;
        }
        else if (pstFatDev->stFdtCache.Flag[2] == 0)
        {
            bufindex = 2;
        }
        else
        {
            if (pstFatDev->stFdtCache.Sec[0] > SecIndex)
            {
                bufMargin[0] = pstFatDev->stFdtCache.Sec[0] - SecIndex;
            }
            else
            {
                bufMargin[0] = SecIndex - pstFatDev->stFdtCache.Sec[0];
            }

            if (pstFatDev->stFdtCache.Sec[1] > SecIndex)
            {
                bufMargin[1] = pstFatDev->stFdtCache.Sec[1] - SecIndex;
            }
            else
            {
                bufMargin[1] = SecIndex - pstFatDev->stFdtCache.Sec[1];
            }

            if (pstFatDev->stFdtCache.Sec[2] > SecIndex)
            {
                bufMargin[2] = pstFatDev->stFdtCache.Sec[2] - SecIndex;
            }
            else
            {
                bufMargin[2] = SecIndex - pstFatDev->stFdtCache.Sec[2];
            }


            if ((bufMargin[0] < bufMargin[1]) && (bufMargin[0] < bufMargin[2]))
            {
                bufindex = 0;
            }
            else if ((bufMargin[1] < bufMargin[0]) && (bufMargin[1] < bufMargin[2]))
            {
                bufindex = 1;
            }
            else if ((bufMargin[2] < bufMargin[1]) && (bufMargin[2] < bufMargin[0]))
            {
                bufindex = 2;
            }
            else
            {
                bufindex = 0;
            }
        }

        if (SecIndex != pstFatDev->stFdtCache.Sec[bufindex])
        {
            if (pstFatDev->stFdtCache.Flag[bufindex])
            {
                if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[bufindex], pstFatDev->pbFdt[bufindex]))
                {
                    return RK_ERROR;
                }
            }
            pstFatDev->stFdtCache.Flag[bufindex] = 0;
            pstFatDev->stFdtCache.Sec[bufindex] = SecIndex;
            if (RK_SUCCESS != FATReadSector(pstFatDev, SecIndex, pstFatDev->pbFdt[bufindex]))
            {
                status = RK_ERROR;
                goto exit;
            }
        }

    }

    Buf = pstFatDev->pbFdt[bufindex] + ByteIndex;
    memcpy(pRt, Buf, sizeof(FDT));



    if (Rt->Attr != ATTR_LFN_ENTRY)
    {
        if (Rt->Name[0] == 0xFF && Rt->Name[1] == 0xFF && Rt->Name[2] == 0xFF && Rt->Name[3] == 0xFF)
        {
            Rt->Name[0] = 0;
        }
    }

exit:

    rkos_semaphore_give(pstFatDev->osFatOperReqSem);

    return (status);

}

/*******************************************************************************
** Name: GetRootFDTInfo
** Input:HDC dev, FDT * Rt, uint32 Index
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 16:33:05
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t GetRootFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT * Rt, uint32 Index)
{
    uint16 ByteIndex;
    uint32 SecIndex;
    int32  temp;

    Index = Index << 5;        /* 32:sizeof(FDT) */

    if (pstFatDev->FATType == VOLUME_TYPE_FAT12 || pstFatDev->FATType == VOLUME_TYPE_FAT16)
    {
        temp = FDT_OVER;

        if (Index < ((uint32)pstFatDev->RootDirSectors << pstFatDev->LogBytePerSec))
        {
            ByteIndex = Index & (pstFatDev->BPB_BytsPerSec - 1);
            SecIndex  = (Index >> pstFatDev->LogBytePerSec) + (pstFatDev->FirstDataSector - pstFatDev->RootDirSectors);

            temp      = ReadFDTInfo(pstFatDev, Rt, SecIndex, ByteIndex);
        }
    }

    return (temp);
}

/*******************************************************************************
** Name: GetFATPosition
** Input:(uint32 cluster, uint32 *FATSecNum, uint16 *FATEntOffset)
** Return: void
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 16:19:13
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN void GetFATPosition(FAT_DEVICE_CLASS * pstFatDev, uint32 cluster, uint32 *FATSecNum, uint16 *FATEntOffset)
{
    uint32 FATOffset;

    if (pstFatDev->FATType == VOLUME_TYPE_FAT16)
    {
        FATOffset = cluster << 1;
    }
    else if (pstFatDev->FATType == VOLUME_TYPE_FAT32)
    {
        FATOffset = cluster << 2;
    }
    else if (pstFatDev->FATType == VOLUME_TYPE_FAT12)
    {
        FATOffset = (cluster * 3) >> 1;
    }

    *FATSecNum    = (FATOffset >> pstFatDev->LogBytePerSec) + pstFatDev->BPB_ResvdSecCnt;//+BootSector.FATSz;//改为写FAT1
    *FATEntOffset = (uint16)(FATOffset & (pstFatDev->BPB_BytsPerSec - 1));

}


/*******************************************************************************
** Name: FATReadSector
** Input:HDC dev,  uint32 LBA, uint8 * Buf
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 9:15:17
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATReadSector(FAT_DEVICE_CLASS * pstFatDev,  uint32 LBA, void * Buf)
{
    rk_size_t ret;

    ret = ParDev_Read(pstFatDev->hPar, LBA, Buf, 1);

    if (ret != 1)
    {
        return RK_ERROR;
    }

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: GetNextClus
** Input:HDC dev, uint32 Index, uint32 Cnt
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 15:55:53
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATGetNextClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus)
{
    uint16 ByteIndex;
    uint32 SecIndex;
    uint32 Rt;
    uint8 bufindex = 0;
    uint32 bufMargin[3];

    if (Clus >= pstFatDev->CountofClusters + 2)
    {
        return (RK_ERROR);
    }

    rkos_semaphore_take(pstFatDev->osFatOperReqSem, MAX_DELAY);

    /* 计算扇区号和字节索引 */
    GetFATPosition(pstFatDev, Clus, &SecIndex, &ByteIndex);
    /* 读取FAT表数据 */

    //printf("$c rkos_semaphore_take over\n");

    if (pstFatDev->stFatCache.Sec[0] == SecIndex)
    {
        bufindex = 0;
    }
    else if (pstFatDev->stFatCache.Sec[1] == SecIndex)
    {
        bufindex = 1;
    }
    else if (pstFatDev->stFatCache.Sec[3] == SecIndex)
    {
        bufindex = 2;
    }
    else
    {
        if (pstFatDev->stFatCache.Sec[0] == 0xffffffff)
        {
            bufindex = 0;
        }
        else if (pstFatDev->stFatCache.Sec[1] == 0xffffffff)
        {
            bufindex = 1;
        }
        else if (pstFatDev->stFatCache.Sec[2] == 0xffffffff)
        {
            bufindex = 2;
        }
        else
        {
            if (pstFatDev->stFatCache.Sec[0] > SecIndex)
            {
                bufMargin[0] = pstFatDev->stFatCache.Sec[0] - SecIndex;
            }
            else
            {
                bufMargin[0] = SecIndex - pstFatDev->stFatCache.Sec[0];
            }

            if (pstFatDev->stFatCache.Sec[1] > SecIndex)
            {
                bufMargin[1] = pstFatDev->stFatCache.Sec[1] - SecIndex;
            }
            else
            {
                bufMargin[1] = SecIndex - pstFatDev->stFatCache.Sec[1];
            }

            if (pstFatDev->stFatCache.Sec[2] > SecIndex)
            {
                bufMargin[2] = pstFatDev->stFatCache.Sec[2] - SecIndex;
            }
            else
            {
                bufMargin[2] = SecIndex - pstFatDev->stFatCache.Sec[2];
            }


            if ((bufMargin[0] < bufMargin[1]) && (bufMargin[0] < bufMargin[2]))
            {
                bufindex = 0;
            }
            else if ((bufMargin[1] < bufMargin[0]) && (bufMargin[1] < bufMargin[2]))
            {
                bufindex = 1;
            }
            else if ((bufMargin[2] < bufMargin[1]) && (bufMargin[2] < bufMargin[0]))
            {
                bufindex = 2;
            }
            else
            {
                bufindex = 0;
            }
        }

        if (pstFatDev->stFatCache.Flag[bufindex])
        {
            pstFatDev->stFatCache.Flag[bufindex] = 0;
            if(RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[bufindex],  pstFatDev->pwFat[bufindex]))
            {
             rkos_semaphore_give(pstFatDev->osFatOperReqSem);
             return (RK_ERROR);
            }
        }

        //rk_printf("secg = %d, index=%d", pstFatDev->stFatCache.Sec[bufindex],bufindex);

        pstFatDev->stFatCache.Sec[bufindex] = SecIndex;

        if (RK_SUCCESS != FATReadSector(pstFatDev, SecIndex, pstFatDev->pwFat[bufindex]))
        {
            rkos_semaphore_give(pstFatDev->osFatOperReqSem);
            return (RK_ERROR);
        }
    }


    switch (pstFatDev->FATType)
    {
        case VOLUME_TYPE_FAT16:
            Rt = pstFatDev->pwFat[bufindex][ByteIndex>>1];
            break;

        case VOLUME_TYPE_FAT32:
            Rt = pstFatDev->pwFat[bufindex][ByteIndex>>1] | ((uint32)pstFatDev->pwFat[bufindex][(ByteIndex+2)>>1] << 16);
            break;

        default:
            Rt = BAD_CLUS;
            break;
    }

    if((pstFatDev->FATType == VOLUME_TYPE_FAT16) && Rt >= (BAD_CLUS & 0xffff))
    {
        Rt |= 0x0ffful << 16;
    }

    Rt &= 0x0fffffff;

    rkos_semaphore_give(pstFatDev->osFatOperReqSem);

    return ((rk_err_t)Rt);

}


/*******************************************************************************
** Name: GetFDTInfo
** Input:HDC dev, FDT *Rt, uint32 ClusIndex, uint32 Index
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 15:29:35
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t GetFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *Rt, uint32 * ClusIndex, uint32 Index)
{
    uint16 ByteIndex;
    uint32 SecIndex, i;
    uint32 NextClus;

    if (ClusIndex == EMPTY_CLUS)
    {
        if (pstFatDev->FATType == VOLUME_TYPE_FAT32)
        {
            *ClusIndex = pstFatDev->BPB_RootClus;
        }
        else
        {
            return(GetRootFDTInfo(pstFatDev, Rt, Index));
        }
    }

    if (pstFatDev->FATType == VOLUME_TYPE_FAT12 || pstFatDev->FATType == VOLUME_TYPE_FAT16 || pstFatDev->FATType == VOLUME_TYPE_FAT32)
    {
        Index     = Index << 5;
        ByteIndex = Index & (pstFatDev->BPB_BytsPerSec - 1);
        SecIndex  = Index >> pstFatDev->LogBytePerSec;

        /* 计算目录项所在扇区 */
        i = pstFatDev->BPB_SecPerClus;

        NextClus = *ClusIndex;

        while (SecIndex >= i)
        {
            NextClus = FATGetNextClus(pstFatDev, NextClus);

            if (NextClus <= EMPTY_CLUS_1 || NextClus >= BAD_CLUS)
            {
                return(FDT_OVER);
            }

            SecIndex -= i;
        }

        *ClusIndex = NextClus;

        SecIndex = ((NextClus - 2) << pstFatDev->LogSecPerClus) + SecIndex + pstFatDev->FirstDataSector;

        return(ReadFDTInfo(pstFatDev, Rt, SecIndex, ByteIndex));

    }

    return (RK_ERROR);

}

/*******************************************************************************
** Name: lg2
** Input:uint32 arg
** Return: int
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 9:28:33
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN int lg2(uint32 arg)
{
    uint16 log;

    for (log = 0; log < 32; log++)
    {
        if (arg & 1)
        {
            arg >>= 1;
            return (arg != 0) ? -1 : log;
        }
        arg >>= 1;
    }
    return (-1);
}


/*******************************************************************************
** Name: FatPrepareWriteFile
** Input:FAT_FILE_CLASS * pstFatFile, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.6
** Time: 15:48:25
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FatPrepareWriteFile(FAT_FILE_CLASS * pstFatFile, uint32 len)
{
    uint32 Sec, TotalSec, SecCnt;
    uint32 needlen, needClus;
    uint32 SecPerClus;
    uint32 CurClus;
    //uint32 ByteIndex, SecIndex;

    rk_err_t ret;
    //FDT Fdt;

    Sec = pstFatFile->Sec;
    TotalSec = pstFatFile->TotalSec;
    CurClus = pstFatFile->CurClus;
    SecPerClus = pstFatFile->pstFatDir->pstFatDev->BPB_SecPerClus;

    SecCnt = (len / 512) + ((len % 512)? 1:0);


    //rk_printf("TotalSec = %d, Sec = %d", TotalSec, Sec);

    if((TotalSec - Sec) < SecCnt)
    {
        needlen = SecCnt - TotalSec + Sec;
        needClus = needlen / SecPerClus + ((needlen % SecPerClus)? 1 : 0);
        //printf("%d ", needClus);

        while(needClus--)
        {
            ret = FATAddClus(pstFatFile->pstFatDir->pstFatDev, CurClus);
            if(ret <= 0)
            {
                pstFatFile->TotalSec = TotalSec;
                //Fdt.FileSize = pstFatFile->FileSize;

                //WriteFDTInfo(pstFatFile->pstFatDir->pstFatDev, &Fdt, SecIndex, ByteIndex);
                return RK_ERROR;
            }

            CurClus = (uint32)ret;

            if(pstFatFile->CurClus == 0)
            {
                pstFatFile->Clus = CurClus;
                pstFatFile->CurClus = CurClus;
                pstFatFile->CurSec = (CurClus - 2) * pstFatFile->pstFatDir->pstFatDev->BPB_SecPerClus
                    + pstFatFile->pstFatDir->pstFatDev->FirstDataSector;

               // Fdt.FstClusHI = CurClus >> 16;
               // Fdt.FstClusLO = CurClus;

                rk_printf("CurClus = %d", CurClus);
            }

            if(pstFatFile->CurClusS == EOF_CLUS_END)
            {
                pstFatFile->CurClus = CurClus;
                pstFatFile->CurClusS = 0;
            }

            TotalSec += SecPerClus;

        }

    }

    pstFatFile->TotalSec = TotalSec;

    return RK_SUCCESS;


}


/*******************************************************************************
** Name: FATDeleteClus
** Input:HDC dev, uint32 Clus
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.20
** Time: 10:11:57
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATDeleteClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus)
{
    uint32 PrevClus, ThisClus, MaxClus, ClusCnt;

    rk_err_t ret;

    if (Clus >= BAD_CLUS)
    {
        return RK_PARA_ERR;
    }
    else if (Clus == EMPTY_CLUS_1)
    {
        return RK_PARA_ERR;
    }

    MaxClus = pstFatDev->CountofClusters + 2;

    ClusCnt = 0;

    //查找簇链的最后一个簇
    ThisClus = Clus;
    PrevClus = Clus;

    if (ThisClus != EMPTY_CLUS)
    {
        while (1)
        {
            ret = FATGetNextClus(pstFatDev, ThisClus);
            if (ret < 0)
            {
                return RK_ERROR;
            }
            else if (ret >= EOF_CLUS_1 || ret <= EMPTY_CLUS_1)
            {
                break;                                            //查找到空簇或结束簇
            }

            if (ret == BAD_CLUS)
            {
                return RK_ERROR;
            }

            PrevClus = ThisClus;
            ThisClus = (uint32)ret;
            if (ClusCnt++ > MaxClus)
            {
                return RK_ERROR;
            }

        }
    }

    //若收到为结尾簇增加一个簇
    if (PrevClus != ThisClus)
    {
        ret = FATSetNextClus(pstFatDev, PrevClus, EOF_CLUS_END);
        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }
    }


    ret = FATSetNextClus(pstFatDev, ThisClus, EMPTY_CLUS);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    pstFatDev->VolumeFreeSize += (1 << (pstFatDev->LogSecPerClus - 1));

    return (rk_err_t)(PrevClus);

}

/*******************************************************************************
** Name: FATGetShortFromLongName
** Input:uint8 * ShortName, uint16 * LongName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.19
** Time: 10:16:35
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATGetShortFromLongName(uint8 * ShortName, uint16 * LongName)
{
    uint32 ShortCnt, i;
    uint8 Name[11];
    uint32 NameNum;
    uint16 * pLong;

    Name[10] = 0;
    ShortCnt = Unicode2Ascii(Name, LongName, 4);
    if ((ShortCnt <= 0) || (ShortCnt > 5))
    {
        return RETURN_FAIL;
    }

    memset(ShortName, 0x20, 11);

    //ShortCnt = StrLenA(ShortName);

    NameNum = SysTickCounter % 1000;

    for (i = 0; i < 8; i++)
    {
        if (i < ShortCnt)
        {
            ShortName[i] = Name[i];
        }
        else if (i < 5)
        {
            ShortName[i] = '~';
        }
        else if (i == 5)
        {
            ShortName[i] = NameNum / 100 + 0x30;
            NameNum = NameNum % 100;
        }
        else if (i == 6)
        {
            ShortName[i] = NameNum / 10 + 0x30;
            NameNum = NameNum % 10;
        }
        else if (i == 7)
        {
            ShortName[i] = NameNum  + 0x30;
        }
    }

    i = StrLenW(LongName);

    pLong = LongName + i;

    while ((*pLong != '.') && i != 0)
    {
        pLong--;
        i--;
    }

    if(i == 0)
    {
        return RETURN_OK;
    }

    i++;

    ShortCnt = Unicode2Ascii(Name, LongName + i, 3);
    if ((ShortCnt < 0) || (ShortCnt > 3))
    {
        return RETURN_FAIL;
    }

    for (i = 0; i < ShortCnt; i++)
    {
        ShortName[i + 8] = Name[i];
    }


    for(i = 0; i < 11; i++)
    {
        if((ShortName[i] <= 'z') && (ShortName[i] >= 'a'))
            ShortName[i] = ('A' + (ShortName[i] - 'a'));
    }

    return RETURN_OK;

}


/*******************************************************************************
** Name: AddFDTLong
** Input:HDC dev, uint32 ClusIndex,  uint32 * pIndex, uint16 * FileName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.19
** Time: 9:39:12
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t AddFDTLong(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus, FDT *FDTData, uint32 * pIndex, uint16 * FileName)
{
    uint32 i, k;
    FDT TempFDT;
    rk_err_t ret;
    uint32 PrevClus, ClusCnt;
    uint32 LfnCnt,EmpFdtCnt,EmpFdtClus,EmpFdtIndex;

    uint16 * strCur;
    uint8 * buf;
    uint8 CheckSum;
    uint32 DirClus;
    uint8 FileNameEnd;

    ret = FindFDTInfoLong(pstFatDev, &TempFDT, Clus, pIndex, FileName);        //NOT_FIND_FDT\RETURN_OK

    if (ret == RK_SUCCESS)
    {
        return (FILE_EXIST);
    }

    if (FDTData->Name[0] == FILE_DELETED)
    {
        FDTData->Name[0] = ESC_FDT;
    }

    CheckSum = FatCheckSum(FDTData->Name);

    i = 0;
    k = 0;

    ret = RK_ERROR;

    PrevClus = Clus;
    DirClus = Clus;
    LfnCnt = StrLenW(FileName) / 13 + ((StrLenW(FileName) % 13)? 1:0);
    EmpFdtCnt = 0;
    rk_printf("Clus = %d, LfnCnt = %d", Clus,LfnCnt);
    ret = RK_SUCCESS;

    while (ret == RK_SUCCESS)
    {
        ret = GetFDTInfo(pstFatDev, &TempFDT, &Clus, i);
        rk_printf("ret = %d, TempFDT.Name[0] = %d", ret, TempFDT.Name[0]);
        if (ret == RK_SUCCESS)
        {
            if (TempFDT.Name[0] == FILE_DELETED)
            {
                if (EmpFdtCnt == 0)
                {
                    EmpFdtClus = 0;
                    EmpFdtIndex = i;
                    EmpFdtCnt++;
                }
                else
                {
                    EmpFdtCnt++;
                }
            }
            else if (TempFDT.Name[0] == FILE_NOT_EXIST)
            {
                if (EmpFdtCnt == 0)
                {
                    EmpFdtClus = 0;
                    EmpFdtIndex = i;
                    EmpFdtCnt++;
                }
                else
                {
                    EmpFdtCnt++;
                }

                do
                {
                    if (PrevClus != Clus)
                    {
                        PrevClus = Clus;
                        i = 0;
                    }

                    i++;
                    k++;

                    ret = GetFDTInfo(pstFatDev, &TempFDT, &Clus, i);

                    if (ret == RK_SUCCESS)
                    {
                        EmpFdtCnt++;
                    }
                    else
                    {
                        break;
                    }

                    if (EmpFdtCnt == (LfnCnt + 1))
                    {
                        break;
                    }

                }
                while (1);

            }
            else
            {
                EmpFdtCnt = 0;
            }

            if (EmpFdtCnt == (LfnCnt + 1))
            {
                break;
            }

        }

        if (PrevClus != Clus)
        {
            PrevClus = Clus;
            i = 0;
        }

        i++;
        k++;

    }

    rk_printf("ret = %d", ret);

    if ((ret == FDT_OVER) && (DirClus != EMPTY_CLUS))    //当前目录项簇已满,需增加一个簇
    {
        *pIndex = k;

        ClusCnt = ((LfnCnt - EmpFdtCnt + 1) * 32 / 512) / pstFatDev->BPB_SecPerClus
                  + ((LfnCnt - EmpFdtCnt + 1) * 32 / 512) % pstFatDev->BPB_SecPerClus? 1:0;


        for (i = 0; i < ClusCnt; i++)
        {
            ret = FATAddClus(pstFatDev, Clus);
            if (ret < 0)
            {
                break;
            }
            else
            {
                ClearClus(pstFatDev, Clus);        //2006.11.24 debug by lxs
                Clus = (uint32)ret;
            }
        }
    }
    else if ((ret == FDT_OVER) && (DirClus == EMPTY_CLUS))
    {
        ret = ROOT_FULL;
    }

    if (ret >= 0)
    {

        memset(&TempFDT, 0X00, 32);

       FileNameEnd = 0;

        for (i = 0; i < LfnCnt; i++)
        {
            strCur = FileName + (LfnCnt - i - 1) * 13;

            /*获取文件名*/
            buf = (uint8 *)&TempFDT;

            if(i == 0)
            {
                buf[0] = LfnCnt | 0x40;
            }
            else
            {
                buf[0] = LfnCnt - i;
            }

            buf[11] = ATTR_LFN_ENTRY;
            buf[13] = CheckSum;

            if(i > 0)
            {
                FileNameEnd = 0;
            }

            buf++;
            for (k = 0; k < 5; k++)
            {//前面10个是byte
                if(FileNameEnd)
                {
                    *(uint16 *)buf = 0xffff;
                }
                else
                {
                    *(uint16 *)buf = *strCur;
                }
                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;
                if(*strCur == 0)
                    FileNameEnd = 1;

                if(FileNameEnd == 0)
                    strCur++;
            }
            buf += 3;

            for (k = 0; k < 6;k++)
            {
                if(FileNameEnd)
                {
                   *(uint16 *)buf = 0xffff;
                }
                else
                {
                    *(uint16 *)buf = *strCur;
                }

                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;

                if(*strCur == 0)
                    FileNameEnd = 1;

                if(FileNameEnd == 0)
                    strCur++;

            }
            buf += 2;

            for (k = 0; k < 2; k++)
            {
                if(FileNameEnd)
                {
                    *(uint16 *)buf = 0xffff;
                }
                else
                {
                    *(uint16 *)buf = *strCur;
                }
                //printf("char = %04x\n", *plfName);
                buf += 2;
                //*plfName |= ((uint16)(*buf++))<<8;

                if(*strCur == 0)
                    FileNameEnd = 1;

                if(FileNameEnd == 0)
                    strCur++;
            }


            ret = SetFDTInfo(pstFatDev, EmpFdtClus, EmpFdtIndex + i, &TempFDT);
            if (ret != RK_SUCCESS)
            {
                break;
            }

        }

        ret = SetFDTInfo(pstFatDev, EmpFdtClus, EmpFdtIndex + i, FDTData);

    }

    if (FDTData->Name[0] == ESC_FDT)
    {
        FDTData->Name[0] = FILE_DELETED;
    }

    return (ret);
}

/*******************************************************************************
** Name: ClearClus
** Input:HDC dev, uint32 ClusIndex
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.19
** Time: 9:29:31
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t ClearClus(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex)
{
    uint16 i;
    uint32 SecIndex;
    uint8  buf[512];
    rk_err_t ret;

    memset(buf, 0, 512);

    if (ClusIndex < (pstFatDev->CountofClusters + 2))
    {
        SecIndex = ((ClusIndex - 2) << pstFatDev->LogSecPerClus) + pstFatDev->FirstDataSector;

        for (i = 0; i < pstFatDev->BPB_SecPerClus; i++)
        {
            ret = FATWriteSector(pstFatDev, SecIndex++, buf);
            if (ret != RK_SUCCESS)
            {
                return RK_ERROR;
            }
        }
        return RK_SUCCESS;
    }
    else
    {
        return RK_PARA_ERR;
    }

}

/*******************************************************************************
** Name: FATSetNextClus
** Input:HDC dev, uint32 Index, uint32 Next
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 18:09:39
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATSetNextClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Index, uint32 Next)
{
    rk_err_t ret;

    uint16 ByteIndex;
    uint32 SecIndex;
    uint8 bufindex = 0;
    uint32 bufMargin[3];


    if (Index <= EMPTY_CLUS_1 || Index >= pstFatDev->CountofClusters + 2)
    {
        return RK_ERROR;
    }

     rkos_semaphore_take(pstFatDev->osFatOperReqSem, MAX_DELAY);

    /* 计算扇区号和字节索引 */
    switch (pstFatDev->FATType)
    {
        case VOLUME_TYPE_FAT16:
        case VOLUME_TYPE_FAT32:
            GetFATPosition(pstFatDev,Index, &SecIndex, &ByteIndex);
            break;

        default:
            return RK_ERROR;
    }


    if (pstFatDev->stFatCache.Sec[0] == SecIndex)
    {
        bufindex = 0;
    }
    else if (pstFatDev->stFatCache.Sec[1] == SecIndex)
    {
        bufindex = 1;
    }
    else if (pstFatDev->stFatCache.Sec[3] == SecIndex)
    {
        bufindex = 2;
    }
    else
    {
        if (pstFatDev->stFatCache.Sec[0] == 0xffffffff)
        {
            bufindex = 0;
        }
        else if (pstFatDev->stFatCache.Sec[1] == 0xffffffff)
        {
            bufindex = 1;
        }
        else if (pstFatDev->stFatCache.Sec[2] == 0xffffffff)
        {
            bufindex = 2;
        }
        else
        {
            if (pstFatDev->stFatCache.Sec[0] > SecIndex)
            {
                bufMargin[0] = pstFatDev->stFatCache.Sec[0] - SecIndex;
            }
            else
            {
                bufMargin[0] = SecIndex - pstFatDev->stFatCache.Sec[0];
            }

            if (pstFatDev->stFatCache.Sec[1] > SecIndex)
            {
                bufMargin[1] = pstFatDev->stFatCache.Sec[1] - SecIndex;
            }
            else
            {
                bufMargin[1] = SecIndex - pstFatDev->stFatCache.Sec[1];
            }

            if (pstFatDev->stFatCache.Sec[2] > SecIndex)
            {
                bufMargin[2] = pstFatDev->stFatCache.Sec[2] - SecIndex;
            }
            else
            {
                bufMargin[2] = SecIndex - pstFatDev->stFatCache.Sec[2];
            }


            if ((bufMargin[0] < bufMargin[1]) && (bufMargin[0] < bufMargin[2]))
            {
                bufindex = 0;
            }
            else if ((bufMargin[1] < bufMargin[0]) && (bufMargin[1] < bufMargin[2]))
            {
                bufindex = 1;
            }
            else if ((bufMargin[2] < bufMargin[1]) && (bufMargin[2] < bufMargin[0]))
            {
                bufindex = 2;
            }
            else
            {
                bufindex = 0;
            }
        }


        if (SecIndex != pstFatDev->stFatCache.Sec[bufindex])
        {
            if (pstFatDev->stFatCache.Flag[bufindex])
            {
                pstFatDev->stFatCache.Flag[bufindex] = 0;
                ret = FATWriteSector(pstFatDev,pstFatDev->stFatCache.Sec[bufindex], pstFatDev->pwFat[bufindex]);
                if (ret != RK_SUCCESS)
                {
                    return RK_ERROR;
                }
            }
            //rk_printf("secs = %d, index = %d", pstFatDev->stFatCache.Sec[bufindex], bufindex);

            pstFatDev->stFatCache.Sec[bufindex] = SecIndex;
            ret = FATReadSector(pstFatDev, SecIndex, pstFatDev->pwFat[bufindex]);
            if (ret != RK_SUCCESS)
            {
                return RK_ERROR;
            }

            //rk_printf("SecIndex = %d, pstFatDev->stFatCache.Sec = %d", SecIndex, pstFatDev->stFatCache.Sec);

        }

    }

    switch (pstFatDev->FATType)
    {

        case VOLUME_TYPE_FAT16:
            pstFatDev->pwFat[bufindex][ByteIndex>>1] = Next;
            break;
        case VOLUME_TYPE_FAT32:
            pstFatDev->pwFat[bufindex][ByteIndex>>1] = Next & 0xffff;
            pstFatDev->pwFat[bufindex][(ByteIndex+2)>>1] = (Next >> 16) & 0xffff;
            break;
        default:
            break;
    }

    pstFatDev->stFatCache.Flag[bufindex] = 1;

    rkos_semaphore_give(pstFatDev->osFatOperReqSem);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FATAddClus
** Input:HDC dev, uint32 Index
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 18:06:09
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATAddClus(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus)
{
    uint32 NextClus, ThisClus, MaxClus, ClusCnt;

    rk_err_t ret;

    if (Clus >= BAD_CLUS)
    {
        return RK_PARA_ERR;
    }
    else if (Clus == EMPTY_CLUS_1)
    {
        return RK_PARA_ERR;
    }

    MaxClus = pstFatDev->CountofClusters + 2;

    ClusCnt = 0;

    //查找簇链的最后一个簇
    //printf("\nx = %d", Clus);
    //printf("x");
    ThisClus = Clus;

    if (ThisClus != EMPTY_CLUS)
    {
        while (1)
        {
            ret = FATGetNextClus(pstFatDev, ThisClus);

            if (ret < 0)
            {
                return RK_ERROR;
            }
            else if (ret >= EOF_CLUS_1 || ret <= EMPTY_CLUS_1)
            {
                break;                                            //查找到空簇或结束簇
            }

            if (ret == BAD_CLUS)
            {
                return RK_ERROR;
            }

            ThisClus = (uint32)ret;
            if (ClusCnt++ > MaxClus)
            {
                return RK_ERROR;
            }

        }
    }

    //printf("z");

    //从簇链结尾处开始收索一个空簇
    for (NextClus = pstFatDev->FirstEmpClus; NextClus < MaxClus; NextClus++)
    {
        ret = FATGetNextClus(pstFatDev, NextClus);
        if (ret < 0)
        {
            return RK_ERROR;
        }
        else if (ret == EMPTY_CLUS)
        {
            pstFatDev->FirstEmpClus = NextClus + 1;
            if (pstFatDev->FirstEmpClus >= MaxClus)
            {
                pstFatDev->FirstEmpClus = 2;
            }
            break;
        }
    }

    //收到尾还没收到就再从头收起
    if (NextClus >= MaxClus)
    {
        for (NextClus = EMPTY_CLUS_1 + 1; NextClus < pstFatDev->FirstEmpClus; NextClus++)
        {
            ret = FATGetNextClus(pstFatDev, NextClus);
            if (ret < 0)
            {
                return RK_ERROR;
            }
            else if (ret == EMPTY_CLUS)
            {
                pstFatDev->FirstEmpClus = NextClus + 1;
                if (pstFatDev->FirstEmpClus >= MaxClus)
                {
                    pstFatDev->FirstEmpClus = 2;
                }
                break;
            }
        }
    }

    if (NextClus >= pstFatDev->FirstEmpClus)
    {
        return DISK_FULL;
    }

    //若收到为结尾簇增加一个簇
    ret = FATSetNextClus(pstFatDev, NextClus, EOF_CLUS_END);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }


    if (ThisClus != EMPTY_CLUS)
    {
        ret = FATSetNextClus(pstFatDev,ThisClus, NextClus);
        if (ret != RK_SUCCESS)
        {
            return RK_ERROR;
        }
    }

    pstFatDev->VolumeFreeSize -= (1 << (pstFatDev->LogSecPerClus - 1));

    //printf("y");
    //printf("\npstFatDev->FirstEmpClus = %d", pstFatDev->FirstEmpClus);

    return (rk_err_t)(NextClus);

}

/*******************************************************************************
** Name: SetRootFDTInfo
** Input:HDC dev,uint32 Index, FDT *FDTData
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 17:41:41
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t SetRootFDTInfo(FAT_DEVICE_CLASS * pstFatDev,uint32 Index, FDT *FDTData)
{
    uint16 ByteIndex;
    uint32 SecIndex;
    int    Rt;

    Rt    = RK_ERROR;

    Index = Index << 5;

    if (pstFatDev->FATType == VOLUME_TYPE_FAT12 || pstFatDev->FATType == VOLUME_TYPE_FAT16)
    {
        if (Index < (pstFatDev->RootDirSectors << pstFatDev->LogBytePerSec))
        {
            ByteIndex = Index & (pstFatDev->BPB_BytsPerSec - 1);
            SecIndex = (Index >> pstFatDev->LogBytePerSec) + (pstFatDev->FirstDataSector-pstFatDev->RootDirSectors);

            Rt = WriteFDTInfo(pstFatDev, FDTData, SecIndex, ByteIndex);
        }
    }

    return (Rt);
}

/*******************************************************************************
** Name: WriteFDTInfo
** Input:HDC dev, FDT *FDTData, uint32 SecIndex, uint16 ByteIndex
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 16:47:37
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t WriteFDTInfo(FAT_DEVICE_CLASS * pstFatDev, FDT *FDTData, uint32 SecIndex, uint16 ByteIndex)
{
//    uint8 i;
    uint8 * pRt = (uint8 *)FDTData;
    uint8 * Buf;

    uint8 bufindex = 0;
    uint32 bufMargin[3];

    rk_printf("write fdt");

    rkos_semaphore_take(pstFatDev->osFatOperReqSem, MAX_DELAY);

    if (pstFatDev->stFdtCache.Sec[0] == SecIndex)
    {
        bufindex = 0;
    }
    else if (pstFatDev->stFdtCache.Sec[1] == SecIndex)
    {
        bufindex = 1;
    }
    else if (pstFatDev->stFdtCache.Sec[3] == SecIndex)
    {
        bufindex = 2;
    }
    else
    {
        if (pstFatDev->stFdtCache.Flag[0] == 0)
        {
            bufindex = 0;
        }
        else if (pstFatDev->stFdtCache.Flag[1] == 0)
        {
            bufindex = 1;
        }
        else if (pstFatDev->stFdtCache.Flag[2] == 0)
        {
            bufindex = 2;
        }
        else
        {
            if (pstFatDev->stFdtCache.Sec[0] > SecIndex)
            {
                bufMargin[0] = pstFatDev->stFdtCache.Sec[0] - SecIndex;
            }
            else
            {
                bufMargin[0] = SecIndex - pstFatDev->stFdtCache.Sec[0];
            }

            if (pstFatDev->stFdtCache.Sec[1] > SecIndex)
            {
                bufMargin[1] = pstFatDev->stFdtCache.Sec[1] - SecIndex;
            }
            else
            {
                bufMargin[1] = SecIndex - pstFatDev->stFdtCache.Sec[1];
            }

            if (pstFatDev->stFdtCache.Sec[2] > SecIndex)
            {
                bufMargin[2] = pstFatDev->stFdtCache.Sec[2] - SecIndex;
            }
            else
            {
                bufMargin[2] = SecIndex - pstFatDev->stFdtCache.Sec[2];
            }


            if ((bufMargin[0] < bufMargin[1]) && (bufMargin[0] < bufMargin[2]))
            {
                bufindex = 0;
            }
            else if ((bufMargin[1] < bufMargin[0]) && (bufMargin[1] < bufMargin[2]))
            {
                bufindex = 1;
            }
            else if ((bufMargin[2] < bufMargin[1]) && (bufMargin[2] < bufMargin[0]))
            {
                bufindex = 2;
            }
            else
            {
                bufindex = 0;
            }
        }

        if (pstFatDev->stFdtCache.Sec[bufindex] != SecIndex)
        {
            if (pstFatDev->stFdtCache.Flag[bufindex])
            {
                if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[bufindex], pstFatDev->pbFdt[bufindex]))
                {
                    rkos_semaphore_give(pstFatDev->osFatOperReqSem);
                    return RK_ERROR;
                }
            }

            FATReadSector(pstFatDev, SecIndex, pstFatDev->pbFdt[bufindex]);
            pstFatDev->stFdtCache.Sec[bufindex] = SecIndex;
            pstFatDev->stFdtCache.Flag[bufindex] = 0;
        }
    }

    Buf = pstFatDev->pbFdt[bufindex] + ByteIndex;
    memcpy(Buf, pRt, sizeof(FDT));

    pstFatDev->stFdtCache.Flag[bufindex] = 1;

    rkos_semaphore_give(pstFatDev->osFatOperReqSem);

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: SetFDTInfo
** Input:HDC dev, uint32 ClusIndex, uint32 Index, FDT *FDTData
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 16:43:50
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t SetFDTInfo(FAT_DEVICE_CLASS * pstFatDev, uint32 ClusIndex, uint32 Index, FDT *FDTData)
{
    uint16 ByteIndex;
    uint32 SecIndex;
    uint8 i;

    if (ClusIndex == EMPTY_CLUS)
    {
        if (pstFatDev->FATType == VOLUME_TYPE_FAT32)
        {
            ClusIndex = pstFatDev->BPB_RootClus;
        }
        else
        {
            return (SetRootFDTInfo(pstFatDev,Index, FDTData));
        }
    }

    if (pstFatDev->FATType == VOLUME_TYPE_FAT12 ||pstFatDev->FATType == VOLUME_TYPE_FAT16 || pstFatDev->FATType == VOLUME_TYPE_FAT32)
    {
        Index     = Index << 5;
        ByteIndex = Index & (pstFatDev->BPB_BytsPerSec - 1);
        SecIndex  = Index >> pstFatDev->LogBytePerSec;     /* 计算目录项所在偏移扇区 */
        i = pstFatDev->BPB_SecPerClus;

        while (SecIndex >= i)
        {
            ClusIndex = FATGetNextClus(pstFatDev, ClusIndex);

            if (ClusIndex <= EMPTY_CLUS_1 || ClusIndex >= BAD_CLUS)
            {
                return (RK_ERROR);
            }

            SecIndex -= i;
        }

        SecIndex = ((ClusIndex - 2) << pstFatDev->LogSecPerClus) + SecIndex + pstFatDev->FirstDataSector;

        return (WriteFDTInfo(pstFatDev,FDTData, SecIndex, ByteIndex));
    }

    return (RK_ERROR);
}


/*******************************************************************************
** Name: AddFDT
** Input:HDC dev, uint32 ClusIndex, FDT *FDTData, uint32 * Index
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.18
** Time: 16:17:59
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t AddFDT(FAT_DEVICE_CLASS * pstFatDev, uint32 Clus, FDT *FDTData, uint32 * pIndex)
{
    uint32 i, k;
    FDT TempFDT;
    rk_err_t ret;
    uint32 PrevClus;

    ret = FindFDTInfo(pstFatDev, &TempFDT, Clus, pIndex, FDTData->Name);        //NOT_FIND_FDT\RETURN_OK

    if (ret == RK_SUCCESS)
    {
        return (RK_ERROR);
    }

    if (FDTData->Name[0] == FILE_DELETED)
    {
        FDTData->Name[0] = ESC_FDT;
    }

    i = 0;
    k = 0;

    ret = RK_ERROR;
    PrevClus = Clus;
    while (ret == RK_SUCCESS)
    {
        ret = GetFDTInfo(pstFatDev, &TempFDT, &Clus, i);
        if (ret == RK_SUCCESS)
        {
            if (TempFDT.Name[0] == FILE_DELETED || TempFDT.Name[0] == FILE_NOT_EXIST)
            {
                ret = SetFDTInfo(pstFatDev, Clus, i, FDTData);
                *pIndex = k;
                break;
            }
        }

        if (PrevClus != Clus)
        {
            PrevClus = Clus;
            i = 0;
        }
        i++;
        k++;
    }

    if (ret == FDT_OVER && Clus != EMPTY_CLUS)    //当前目录项簇已满,需增加一个簇
    {
        *pIndex = k;
        ret = FATAddClus(pstFatDev, Clus);
        if (ret > 0)
        {
            ClearClus(pstFatDev, i);        //2006.11.24 debug by lxs
            ret = SetFDTInfo(pstFatDev, i, 0, FDTData);
        }
    }

    if (FDTData->Name[0] == ESC_FDT)
    {
        FDTData->Name[0] = FILE_DELETED;
    }

    return (ret);
}

/*******************************************************************************
** Name: FATWriteSector
** Input:HDC dev, uint32 LBA, uint8 * Buf
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 16:13:43
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_COMMON_
COMMON FUN rk_err_t FATWriteSector(FAT_DEVICE_CLASS * pstFatDev, uint32 LBA, void * Buf)
{
    rk_size_t ret;


    //rk_printf("file write Sector.................................................xxxxxxxxxx........");

    ret = ParDev_Write(pstFatDev->hPar, LBA, Buf, 1);

    if (ret != 1)
    {
        return RK_ERROR;
    }

    return RK_SUCCESS;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FATDevCreate
** Input:void
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT API HDC FatDev_Create(uint32 DevID, void * arg)
{
    FAT_DEV_ARG * pstFatArg = (FAT_DEV_ARG *)arg;

    DEVICE_CLASS* pstDev;
    FAT_DEVICE_CLASS * pstFatDev;

    if (pstFatArg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstFatDev =  rkos_memory_malloc(sizeof(FAT_DEVICE_CLASS));
    if (pstFatDev == NULL)
    {
        return pstFatDev;
    }

    pstFatDev->pbFdt[0] = rkos_memory_malloc(512);
    if(pstFatDev->pbFdt[0] == 0)
    {
        rkos_memory_free(pstFatDev->pbFdt[0]);
        return (HDC)RK_ERROR;
    }
    pstFatDev->pwFat[0] = rkos_memory_malloc(512);
    if(pstFatDev->pwFat[0] == 0)
    {
        rkos_memory_free(pstFatDev->pwFat[0]);
        return (HDC)RK_ERROR;
    }
    pstFatDev->pbFdt[1] = rkos_memory_malloc(512);
    if(pstFatDev->pbFdt[1] == 0)
    {
        rkos_memory_free(pstFatDev->pbFdt[1]);
        return (HDC)RK_ERROR;
    }
    pstFatDev->pwFat[1] = rkos_memory_malloc(512);
    if(pstFatDev->pwFat[1] == 0)
    {
        rkos_memory_free(pstFatDev->pwFat[1]);
        return (HDC)RK_ERROR;
    }
    pstFatDev->pbFdt[2] = rkos_memory_malloc(512);
    if(pstFatDev->pbFdt[2] == 0)
    {
        rkos_memory_free(pstFatDev->pbFdt[2]);
        return (HDC)RK_ERROR;
    }
    pstFatDev->pwFat[2] = rkos_memory_malloc(512);
    if(pstFatDev->pwFat[2] == 0)
    {
        rkos_memory_free(pstFatDev->pwFat[2]);
        return (HDC)RK_ERROR;
    }

    pstFatDev->osFatOperReqSem  = rkos_semaphore_create(1, 1);

    if (pstFatDev->osFatOperReqSem == 0)
    {
        rkos_semaphore_delete(pstFatDev->osFatOperReqSem);
        rkos_memory_free(pstFatDev);
        return (HDC)RK_ERROR;
    }

    pstFatDev->hPar = pstFatArg->hPar;

    pstDev = (DEVICE_CLASS *)pstFatDev;
    pstDev->suspend = NULL;
    pstDev->resume  = NULL;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_FAT_DEV, MODULE_OVERLAY_CODE);
    #endif

    FATInitCache(pstFatDev);
    if(FATDevInit(pstFatDev) != RK_SUCCESS)
    {
        return (HDC)RK_ERROR;
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
** Name: FATInitCache
** Input:HDC dev
** Return: void
** Owner:Aaron.sun
** Date: 2014.4.2
** Time: 15:16:51
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT FUN void FATInitCache(FAT_DEVICE_CLASS * pstFatDev)
{
    uint32 i;

    for(i = 0; i < 3; i++)
    {
        pstFatDev->stFatCache.Flag[i] = 0;
        pstFatDev->stFdtCache.Flag[i] = 0;
    }
}

/*******************************************************************************
** Name: GetBootInfo
** Input:HDC dev, uint32 PBRSector, uint8 DbrBuf
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 9:47:43
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT FUN uint32 GetBootInfo(FAT_DEVICE_CLASS * pstFatDev, uint8 * DbrBuf)
{
    uint8 type;
    uint32 TotSec, DataSec;

    pstFatDev->BPB_BytsPerSec = ((uint16)DbrBuf[12] << 8) + DbrBuf[11];
    pstFatDev->BPB_SecPerClus = DbrBuf[13];

    pstFatDev->LogBytePerSec = lg2(pstFatDev->BPB_BytsPerSec);
    pstFatDev->LogSecPerClus = lg2(pstFatDev->BPB_SecPerClus);

    pstFatDev->BPB_NumFATs = DbrBuf[16];
    pstFatDev->BPB_RootEntCnt = ((uint16)DbrBuf[18] << 8) + DbrBuf[17];
    pstFatDev->FATSz = ((uint32)DbrBuf[23] << 8) + DbrBuf[22];

    if (pstFatDev->FATSz == 0)
    {
        pstFatDev->FATSz = ((uint32)DbrBuf[39] << 8) + DbrBuf[38];
        pstFatDev->FATSz <<= 16;
        pstFatDev->FATSz += ((uint32)DbrBuf[37] << 8) + DbrBuf[36];
    }

    TotSec = ((uint32)DbrBuf[20] << 8) + DbrBuf[19];
    if (TotSec == 0)
    {
        TotSec=((uint32)DbrBuf[35] << 8) + DbrBuf[34];
        TotSec <<= 16;
        TotSec += ((uint32)DbrBuf[33] << 8) + DbrBuf[32];
    }

    pstFatDev->BPB_ResvdSecCnt = ((uint16)DbrBuf[15] << 8) + DbrBuf[14];
    pstFatDev->RootDirSectors = ((pstFatDev->BPB_RootEntCnt * 32) + (pstFatDev->BPB_BytsPerSec - 1)) >> pstFatDev->LogBytePerSec;
    pstFatDev->FirstDataSector = pstFatDev->BPB_ResvdSecCnt + (pstFatDev->BPB_NumFATs * pstFatDev->FATSz) + pstFatDev->RootDirSectors;

    DataSec = TotSec - pstFatDev->FirstDataSector;

    while ((DataSec & (pstFatDev->BPB_SecPerClus - 1)) != 0)  //Modify by lxs @2006.01.10 for No Standard Lib compiler
    {
        DataSec--;
    }

    pstFatDev->CountofClusters = DataSec >> pstFatDev->LogSecPerClus;
    if (pstFatDev->CountofClusters < 4085)
    {
        type = VOLUME_TYPE_FAT12;       // Volume is FAT12
    }
    else if (pstFatDev->CountofClusters < 65525)
    {
        type = VOLUME_TYPE_FAT16;       // Volume is FAT16
    }
    else
    {
        type = VOLUME_TYPE_FAT32;       // Volume is FAT32
    }


    if (type == VOLUME_TYPE_FAT32)
    {
        pstFatDev->BPB_RootClus = ((uint32)DbrBuf[47] << 8) + DbrBuf[46];
        pstFatDev->BPB_RootClus <<= 16;
        pstFatDev->BPB_RootClus += ((uint32)DbrBuf[45] <<8 ) + DbrBuf[44];
    }
    else
    {
        pstFatDev->BPB_RootClus = 0;
    }

    pstFatDev->TotSec = TotSec;       //add by lxs @2005.4.19
    return (type);
}


/*******************************************************************************
** Name: CheckFatBootSector
** Input:uint8 * buf
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.11
** Time: 9:24:37
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT FUN rk_err_t CheckFatBootSector(uint8 * DbrBuf)
{
    uint16 temp;
    int32 bad = RK_SUCCESS;

    if (DbrBuf[0] == 0xE9); // OK
    else if (DbrBuf[0] == 0xEB && DbrBuf[2] == 0x90);   // OK
    else
    {
        bad = RK_ERROR;        // Missing JMP/NOP
    }

    // check other stuff
    temp = DbrBuf[13];

    if ((lg2(temp)) < 0)
    {
        bad = RK_ERROR;        //Sectors per cluster is not a power of 2
    }

    //very few disks have only 1 FAT, but it's valid
    temp = DbrBuf[16];
    if (temp != 1 && temp != 2)
    {
        bad = RK_ERROR;        // Invalid number of FATs
    }

    // can't check against dev.sects because dev.sects may not yet be set
    temp = (DbrBuf[25]<<8)+ DbrBuf[24];
    if (temp == 0 || temp > 63)
    {
        bad = RK_ERROR;        // Invalid number of sectors
    }

    // can't check against dev.heads because dev.heads may not yet be set
    temp = (DbrBuf[27]<<8)+ DbrBuf[26];
    if (temp == 0 || temp > 255)
    {
        bad = RK_ERROR;        // Invalid number of heads
    }

    temp = (DbrBuf[511]<<8)+ DbrBuf[510];
    if (temp != 0xAA55)
    {
        bad = RK_ERROR;        // Invalid signature of FATs
    }

    return (bad);
}

/*******************************************************************************
** Name: FATDevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT FUN rk_err_t FATDevDeInit(FAT_DEVICE_CLASS * pstFatDev)
{
    uint32 i;

    for(i = 0; i < 3; i++)
    {
        if (pstFatDev->stFdtCache.Flag[i])
        {
            pstFatDev->stFdtCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFdtCache.Sec[i], pstFatDev->pbFdt[i]))
            {
                return RK_ERROR;
            }
        }

        if (pstFatDev->stFatCache.Flag[i])
        {
            pstFatDev->stFatCache.Flag[i] = 0;
            if (RK_SUCCESS != FATWriteSector(pstFatDev, pstFatDev->stFatCache.Sec[i], pstFatDev->pwFat[i]))
            {
                return RK_ERROR;
            }
        }
    }
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FATDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
INIT FUN rk_err_t FATDevInit(FAT_DEVICE_CLASS * pstFatDev)
{
    uint8  DbrBuf[512];
    rk_err_t ret;

    ret = FATReadSector(pstFatDev, 0, DbrBuf);         //读DBR扇区
    if (ret != RK_SUCCESS)
    {
        return ret;
    }

    if (RK_ERROR == CheckFatBootSector(DbrBuf)) //not a FAT volume
    {
        return (RK_ERROR);
    }

    pstFatDev->FATType = GetBootInfo(pstFatDev,DbrBuf);

    if (pstFatDev->FATType  == VOLUME_TYPE_FAT32)
    {
        ret = FATReadSector(pstFatDev, 1, DbrBuf);
        if (ret != RK_SUCCESS)
        {
            return ret;
        }
        pstFatDev->FirstEmpClus = *(uint32 *)(DbrBuf + 492);

    }
    else
    {
        pstFatDev->FirstEmpClus = 2;
    }

    return (RK_SUCCESS);
}

/*******************************************************************************
** Name: FatDevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.6
** Time: 16:08:29
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_INIT_
rk_err_t FatDevDelete(HDC dev)
{
    FAT_DEVICE_CLASS * pstFatDev = (FAT_DEVICE_CLASS *)dev;

    FATDevDeInit(pstFatDev);

    rkos_semaphore_delete(pstFatDev->osFatOperReqSem);

    rkos_memory_free(pstFatDev->pbFdt[0]);
    rkos_memory_free(pstFatDev->pwFat[0]);
    rkos_memory_free(pstFatDev->pbFdt[1]);
    rkos_memory_free(pstFatDev->pwFat[1]);
    rkos_memory_free(pstFatDev->pbFdt[2]);
    rkos_memory_free(pstFatDev->pwFat[2]);
    rkos_memory_free(dev);

    return RK_SUCCESS;
}



#ifdef _FAT_SHELL_

_DRIVER_VOLUME_FATDEVICE_SHELL_

const char* ShellFatName[] =
{
    "pcb",
    "create",
    "help",
    "\b"                          // the end
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FATShell
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.10
** Time: 15:21:38
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_SHELL_
SHELL API rk_err_t FatDev_Shell(HDC dev, uint8 * pstr)
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

    while (memcmp((uint8*)ShellFatName[i], pItem, strlen(ShellFatName[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellFatName[i]) == '\b')
            return RK_ERROR;
    }

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:
            ret = FatShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = FatShellCreate(dev,pItem);
            break;

        case 0x02:  //help
            ret = FatShellHelp(dev,pItem);;
            break;

        //ohter sub cmd...
        case 0x03:
            //ret = ;
            break;

        case 0x04:
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
** Name: FatShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:FUN
** Date: 2014.11.3
** Time: 15:10:51
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_SHELL_
SHELL FUN rk_err_t FatShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8*)pstr) != 0)
        return RK_ERROR;

    rk_print_string("fat命令集提供了一系列的命令对fat进行操作\r\n");
    rk_print_string("fat包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息          \r\n");
    rk_print_string("open      fat open             \r\n");
    rk_print_string("help      显示fat命令帮助信息  \r\n");
    //other...

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: FatShellPcb
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_SHELL_
rk_err_t FatShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA((uint8*)pstr, "help", 4) == 0)
        {
            rk_print_string("fat.pcb :fat pcb info.\r\n");
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
** Name: FatShellOpen
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_VOLUME_FATDEVICE_SHELL_
rk_err_t FatShellCreate(HDC dev,  uint8 * pstr)
{
    FAT_DEV_ARG stCreateFatArg;
    HDC hPar,hFat,hDir;
    DEVICE_CLASS * pstDev;
    FILE_ATTR stFileAttr;
    rk_err_t ret;
    uint16 LongName[64];
    uint8 Buf[128];
    uint32 len;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA((uint8*)pstr, "help", 4) == 0)
        {
            rk_print_string("fat.open : open fat.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    hPar = RKDev_Open(DEV_CLASS_PAR, 0, NOT_CARE);
    if ((hPar == NULL) || (hPar == (HDC)RK_ERROR) || (hPar == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev, "par0 open failure", 17, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    stCreateFatArg.hPar = hPar;

    ret = RKDev_Create(DEV_CLASS_FAT, 0, &stCreateFatArg);

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "fat0 create failure", 18, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

#if 0
    hFat = RKDev_Open(DEV_CLASS_FAT, 0, READ_COMMON_CTRL);
    if ((hFat == NULL) || (hFat == (HDC)RK_ERROR) || (hFat == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev, "fat0 open failure", 17, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }



    memset(&stFileAttr, 0, sizeof(FILE_ATTR));

    stFileAttr.Path = L"\\";
    stFileAttr.FileName = L"I Love You!!!!";

    ret = FatDev_CreateDir(hFat, NULL, ATTR_READ_ONLY, &stFileAttr);

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "file create failure", 19, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    memset(&stFileAttr, 0, sizeof(FILE_ATTR));

    stFileAttr.Path = L"\\";
    stFileAttr.FileName = L"I Miss You!!!!";

    ret = FatDev_CreateDir(hFat, NULL, ATTR_READ_ONLY, &stFileAttr);

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "file create failure", 19, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    memset(&stFileAttr, 0, sizeof(FILE_ATTR));

    stFileAttr.Path = L"\\";
    stFileAttr.FileName = L"I Kill You!!!!";

    ret = FatDev_CreateDir(hFat, NULL, ATTR_READ_ONLY, &stFileAttr);

    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "file create failure", 19, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }




    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = NULL;

    hDir = FatDev_OpenDir(hFat, NULL, READ_WRITE_CTRL, &stFileAttr);
    if ((rk_err_t)hDir <= 0)
    {
        UartDev_Write(dev, "dir open failure", 16, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    if (FatDev_NextDir(hDir, 0, &stFileAttr) != RK_SUCCESS)
    {
        UartDev_Write(dev, "file next failure", 17, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    if (FatDev_GetFileName(hDir, LongName) != RK_SUCCESS)
    {
        UartDev_Write(dev, "file get long name failure", 26, SYNC_MODE, NULL);
        return RK_SUCCESS;
    }

    Unicode2Ascii(Buf, LongName, 128);

    len = sprintf((uint8 *)LongName, "the file name is: %s", Buf);

    UartDev_Write(dev, (uint8 *)LongName, len, SYNC_MODE, NULL);

    FatDev_CloseDir(hDir);
    RKDev_Close(hFat);

 #endif

    RKDev_Close(hPar);

    return RK_SUCCESS;
}


#endif
#endif

