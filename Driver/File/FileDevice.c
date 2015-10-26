/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\File\FileDevice.c
* Owner: Aaron.sun
* Date: 2014.3.31
* Time: 18:03:12
* Desc: File Device Class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.3.31     18:03:12   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_FILE_FILEDEVICE_C__

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
typedef struct _VOLUME_CLASS
{
    uint32 VolumeType;
    HDC    hVolume;

}VOLUME_CLASS;

typedef struct _FILE_OPER_CLASS
{
    VOLUME_CLASS * pstVolume;
    HDC hOper;
    uint32 CurOffset;
    uint32 FileSize;

}FILE_OPER_CLASS;

typedef  struct _FILE_DEVICE_CLASS
{
    DEVICE_CLASS stFileDevice;
    VOLUME_CLASS stVolume[VOLUME_NUM_MAX];
    FILE_OPER_CLASS stFileOper[32];

}FILE_DEVICE_CLASS;


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
rk_err_t FileShellOperHelp(HDC dev,  uint8 * pstr);
rk_err_t FileShellHelp(HDC dev,  uint8 * pstr);
rk_err_t FileShellTest(HDC dev, uint8 * pstr);
rk_err_t FileShellDel(HDC dev, uint8 * pstr);
rk_err_t FileShellCreate(HDC dev, uint8 * pstr);
rk_err_t FileShellDir(HDC dev, uint8 * pstr);
rk_err_t FileShellOper(HDC dev, uint8 * pstr);
FILE_OPER_CLASS * GetFileOperClass(FILE_DEVICE_CLASS * pstFileDev);
rk_err_t FileShellPcb(HDC dev, uint8 * pstr);
rk_err_t FileDevDelete(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FileDev_SynchFileHandler
** Input:HDC hTarget, HDC hSrc, uint32 Mode
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.7.7
** Time: 15:49:22
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_SynchFileHandler(HDC hTarget, HDC hSrc, uint32 Mode)
{
    FILE_OPER_CLASS * pstTarget = (FILE_OPER_CLASS *)hTarget;
    FILE_OPER_CLASS * pstSrc = (FILE_OPER_CLASS *)hSrc;
    VOLUME_CLASS * pstTargetVolume, * pstSrcVolume;

    if(pstTarget == NULL || pstSrc == NULL)
    {
        return RK_PARA_ERR;
    }

    pstTargetVolume = pstTarget->pstVolume;
    pstSrcVolume = pstSrc->pstVolume;

    if(((pstTargetVolume->VolumeType == VOLUME_TYPE_FAT12) && (pstSrcVolume->VolumeType == VOLUME_TYPE_FAT12))
        || ((pstTargetVolume->VolumeType == VOLUME_TYPE_FAT16) && (pstSrcVolume->VolumeType == VOLUME_TYPE_FAT16))
        || ((pstTargetVolume->VolumeType == VOLUME_TYPE_FAT32)) && (pstSrcVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
        return FatDev_SynchFileHandler(pstTarget->hOper, pstSrc->hOper, Mode);
    }


}

/*******************************************************************************
** Name: FileDev_GetFileSize
** Input:HDC dev, uint32 * size
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 17:51:23
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetFileSize(HDC dev, uint32 * size)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;

    *size = pstFileOper->FileSize;

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: FileDev_GetFileOffset
** Input:HDC dev, uint32 * offset
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.6.18
** Time: 17:40:56
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetFileOffset(HDC dev, uint32 * offset)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;

    *offset = pstFileOper->CurOffset;

    return RK_SUCCESS;
}


/*******************************************************************************
** Name: FileDev_FileSeek
** Input:HDC dev, uint32 pos, uint32 secoffset
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.24
** Time: 11:30:26
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_FileSeek(HDC dev, uint32 pos, uint32 Offset)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    pstFileOper->CurOffset = Offset;

    rk_printf("pos = %d, Offset = %d", pos, Offset);

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_FileSeek(pstFileOper->hOper, pos, Offset);
    }

    return RK_ERROR;

}


/*******************************************************************************
** Name: DateToString
** Input:uint16 Data, uint8 * pstr
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.5.7
** Time: 14:10:11
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API uint32 DateToString(uint16 date, uint8 * pstr)
{
    uint32 temp;

    temp = (date >> 9) + 1980;

    pstr[3] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[2] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[1] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[0] = temp % 10 + 0x30;

    pstr[4] = '-';

    temp = (date >> 5) & 0x000F;

    pstr[6] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[5] = temp % 10 + 0x30;

    pstr[7] = '-';


    temp = date & 0x001F;

    pstr[9] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[8] = temp % 10 + 0x30;

    return 10;

}

/*******************************************************************************
** Name: TimeToString
** Input:uint16 time, uint8 ms, uint8 * pstr
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.5.7
** Time: 14:08:59
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API uint32 TimeToString(uint16 time, uint8 ms, uint8 * pstr)
{
    uint32 temp;
    temp = time >> 11;

    pstr[1] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[0] = temp % 10 + 0x30;

    pstr[2] = ':';

    temp = (time >> 5) & 0x003F;

    pstr[4] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[3] = temp % 10 + 0x30;

    pstr[5] = ':';


    temp = (time & 0x001F) * 2 + (ms * 10) / 1000;

    pstr[7] = temp % 10 + 0x30;
    temp = temp / 10;
    pstr[6] = temp % 10 + 0x30;

    return 8;

}


/*******************************************************************************
** Name:  FileDev_DeleteVolume
** Input:HDC dev, VOLUME_INF pstVolumeInfo
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 15:07:06
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t  FileDev_DeleteVolume(HDC dev, VOLUME_INF * pstVolumeInf)
{
     FILE_DEVICE_CLASS * pstFileDev =  (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
     VOLUME_CLASS * pstVolumeClass;

     if(pstFileDev == NULL)
     {
         return RK_ERROR;
     }

     pstVolumeClass = &pstFileDev->stVolume[pstVolumeInf->VolumeID - 'C'];

     if(pstVolumeClass->VolumeType == 0)
     {
         return RK_ERROR;
     }

     pstVolumeClass->VolumeType = 0;

     return RK_SUCCESS;

}

/*******************************************************************************
** Name: FileDev_AddVolume
** Input:HDC dev,  VOLUME_INF * pstVolumeInf
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 15:03:46
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_AddVolume(HDC dev,  VOLUME_INF * pstVolumeInf)
{
    FILE_DEVICE_CLASS * pstFileDev =  (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolumeClass;


    if(pstFileDev == NULL)
    {
        return RK_ERROR;
    }

    pstVolumeClass = &pstFileDev->stVolume[pstVolumeInf->VolumeID - 'C'];

    if(pstVolumeClass->VolumeType != 0)
    {
        return RK_ERROR;
    }

    pstVolumeClass->hVolume = pstVolumeInf->hVolume;
    pstVolumeClass->VolumeType = pstVolumeInf->VolumeType;

    return RK_SUCCESS;


}

/*******************************************************************************
** Name: FileDev_GetVolumeInf
** Input:HDC dev, VOLUME_INF * pstVolumeInfo
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:47:53
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetVolumeInf(HDC dev, uint32 VolumeNum, VOLUME_INF * pstVolumeInf)
{
    FILE_DEVICE_CLASS * pstFileDev =  (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    uint32 i;

    for(i = 0; i < VOLUME_NUM_MAX; i++)
    {
        if(pstFileDev->stVolume[i].VolumeType != 0)
        {
            if(VolumeNum == 0)
            {
                break;
            }
            else
            {
               VolumeNum--;
            }
        }
    }

    if(i < VOLUME_NUM_MAX)
    {
        pstVolumeInf->hVolume = pstFileDev->stVolume[i].hVolume;
        pstVolumeInf->VolumeType = pstFileDev->stVolume[i].VolumeType;
        pstVolumeInf->VolumeID = i + 'C';
        return RK_SUCCESS;
    }

    return RK_ERROR;

}

/*******************************************************************************
** Name: FileDev_GetVolumeTotalCnt
** Input:HDC dev, uint32 * pVolumeCnt
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:46:44
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetVolumeTotalCnt(HDC dev, uint32 * pVolumeCnt)
{
      FILE_DEVICE_CLASS * pstFileDev =  (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
      uint32 i, Cnt;

      Cnt = 0;

      for(i = 0; i < VOLUME_NUM_MAX; i++)
      {
          if(pstFileDev->stVolume[i].VolumeType != 0)
          {
              Cnt++;
          }
      }

      *pVolumeCnt = Cnt;

      return RK_SUCCESS;

}

/*******************************************************************************
** Name: FileDev_GetTotalFile
** Input:HDC dev, uint8 * ExtName, uint8 Attr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:39:44
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetTotalFile(HDC dev, uint8 * ExtName, uint8 Attr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_GetTotalFile(pstFileOper->hOper, ExtName, Attr);
    }

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FileDev_GetTotalDir
** Input:HDC dev, uint8 * ExtName, uint8 Attr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:38:46
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetTotalDir(HDC dev, uint8 * ExtName, uint8 Attr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_GetTotalDir(pstFileOper->hOper, ExtName, Attr);
    }

    return RK_ERROR;



}

/*******************************************************************************
** Name: FileDev_CloseDir
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:28:48
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_CloseDir(HDC dev)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;
    rk_err_t ret;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       ret = FatDev_CloseDir(pstFileOper->hOper);
       pstFileOper->hOper = 0;
       return ret;
    }

    return RK_ERROR;
}


/*******************************************************************************
** Name: FileDev_CloseFile
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:28:20
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_CloseFile(HDC dev)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;
    rk_err_t ret;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       ret = FatDev_CloseFile(pstFileOper->hOper);
       pstFileOper->hOper = 0;
       return ret;
    }

    return RK_ERROR;

}


/*******************************************************************************
** Name: FileDev_OpenFile
** Input: HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:24:43
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API HDC FileDev_OpenFile(HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolume;
    FILE_ATTR stFileAttr;
    FILE_OPER_CLASS * pstFileOper;
    //rk_err_t ret;

    if(pstFileDev == NULL)
    {
        return (HDC)RK_ERROR;
    }

    pstFileOper = GetFileOperClass(pstFileDev);

    if(pstFileOper == NULL)
    {
        return NULL;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    pstFileOper->CurOffset = 0;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       pstFileOper->hOper = FatDev_OpenFile(pstVolume->hVolume, hFather, pstFileAttr);
       if((rk_err_t) pstFileOper->hOper <= 0)
       {
            pstFileOper->hOper = 0;
            return (HDC)RK_ERROR;
       }
       pstFileOper->FileSize = pstFileAttr->FileSize;
       pstFileOper->pstVolume = pstVolume;
       return pstFileOper;
    }

    return (HDC)RK_ERROR;

}


/*******************************************************************************
** Name: FileDev_OpenDir
** Input:HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:23:11
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API HDC FileDev_OpenDir(HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolume;
    FILE_ATTR stFileAttr;
    FILE_OPER_CLASS * pstFileOper;
    rk_err_t ret;

    if(pstFileDev == NULL)
    {
        return (HDC)RK_ERROR;
    }

    pstFileOper = GetFileOperClass(pstFileDev);

    if(pstFileOper == NULL)
    {
        return NULL;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       pstFileOper->hOper = FatDev_OpenDir(pstVolume->hVolume, hFather, pstFileAttr);
       if((rk_err_t) pstFileOper->hOper <= 0)
       {
            pstFileOper->hOper = 0;
            return (HDC)RK_ERROR;
       }

       pstFileOper->pstVolume = pstVolume;

       return pstFileOper;
    }

    return (HDC)RK_ERROR;

}

/*******************************************************************************
** Name: FileDev_GetFileName
** Input:HDC dev,  uint16 * FileName
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:21:17
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_GetFileName(HDC dev,  uint16 * FileName)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_GetFileName(pstFileOper->hOper, FileName);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_NextFile
** Input:(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:20:23
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_NextFile(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
        return FatDev_NextFile(pstFileOper->hOper,FileMaskAttr, ExtName, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_PrevFile
** Input:(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:19:58
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_PrevFile(HDC dev, uint8 FileMaskAttr, uint8 * ExtName, FILE_ATTR * pstFileAttr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_PrevFile(pstFileOper->hOper,FileMaskAttr, ExtName, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_NextFile
** Input:HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:18:35
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_NextDir(HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_NextDir(pstFileOper->hOper,DirMaskAttr, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_PrevFile
** Input:HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:17:50
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_PrevDir(HDC dev, uint8 DirMaskAttr, FILE_ATTR * pstFileAttr)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_PrevDir(pstFileOper->hOper,DirMaskAttr, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_DeleteFile
** Input:HDC dev,  HDC hFather, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:36:24
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_DeleteFile(HDC dev,  HDC hFather, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileDev == NULL)
    {
        return RK_ERROR;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_DeleteFile(pstVolume->hVolume, hFather, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_DeleteDir
** Input:HDC dev,  HDC hFather, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:35:48
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_DeleteDir(HDC dev,  HDC hFather, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolume;

    if(pstFileDev == NULL)
    {
        return RK_ERROR;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_DeleteDir(pstVolume->hVolume, hFather, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_CreateFile
** Input:HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:22:44
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_CreateFile(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolume;

    if(pstFileDev == NULL)
    {
        return RK_ERROR;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_CreateFile(pstVolume->hVolume, hFather, pstFileAttr);
    }

    return RK_ERROR;
}

/*******************************************************************************
** Name: FileDev_CreateDir
** Input:HDC dev, HDC hFather, uint32 Mode, FILE_ATTR * pstFileAttr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.22
** Time: 14:22:18
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON API rk_err_t FileDev_CreateDir(HDC dev, HDC hFather, FILE_ATTR * pstFileAttr)
{
    FILE_DEVICE_CLASS * pstFileDev = (FILE_DEVICE_CLASS *)RK_CompetenceToDev(dev, CTRL_ONLY);
    VOLUME_CLASS * pstVolume;

    if(pstFileDev == NULL)
    {
        return RK_ERROR;
    }

    if(hFather != NULL)
    {
        pstVolume = ((FILE_OPER_CLASS *)hFather)->pstVolume;
    }
    else
    {
        pstVolume = &pstFileDev->stVolume[pstFileAttr->Path[0] - 'C'];
    }

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_CreateDir(pstVolume->hVolume, hFather, pstFileAttr);
    }

    return RK_ERROR;
}


/*******************************************************************************
** Name: FileDevWrite
** Input:HDC dev, uint32 pos, const void* buffer, uint32 size,uint8 mode,pTx_complete Tx_complete
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON FUN rk_err_t FileDev_WriteFile(HDC dev, uint8* buffer, uint32 len)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_WriteFile(pstFileOper->hOper, buffer, len);
    }

    return RK_ERROR;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: FileDevCreate
** Input:void * Args
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_INIT_
INIT API HDC FileDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS* pstDev;
    FILE_DEVICE_CLASS * pstFileDev;


    pstFileDev =  rkos_memory_malloc(sizeof(FILE_DEVICE_CLASS));
    if (pstFileDev == NULL)
    {
        return pstFileDev;
    }

    memset((uint8 *)pstFileDev, 0, sizeof(FILE_DEVICE_CLASS));

    pstDev = (DEVICE_CLASS *)pstFileDev;
    pstDev->suspend = NULL;
    pstDev->resume  = NULL;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_FILE_DEV, MODULE_OVERLAY_CODE);
    #endif

    return pstDev;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: GetFileOperClass
** Input:HDC dev
** Return: FILE_OPER_CLASS *
** Owner:Aaron.sun
** Date: 2014.4.8
** Time: 9:10:25
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON FUN FILE_OPER_CLASS * GetFileOperClass(FILE_DEVICE_CLASS * pstFileDev)
{
    uint32 i;

    for (i = 0; i < VOLUME_NUM_MAX; i++)
    {
        if (pstFileDev->stFileOper[i].hOper == 0)
        {
            return &pstFileDev->stFileOper[i];
        }
    }

    return NULL;

}

/*******************************************************************************
** Name: FileDevRead
** Input:HDC dev,uint32 pos, void* buffer, uint32 size,uint8 mode,pRx_indicate Rx_indicate
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_COMMON_
COMMON FUN rk_err_t FileDev_ReadFile(HDC dev, uint8* buffer, uint32 len)
{
    FILE_OPER_CLASS * pstFileOper = (FILE_OPER_CLASS *)dev;
    VOLUME_CLASS * pstVolume;

    if(pstFileOper == NULL)
    {
        return RK_ERROR;
    }

    pstVolume = pstFileOper->pstVolume;

    pstFileOper->CurOffset += len;

    if((pstVolume->VolumeType == VOLUME_TYPE_FAT12)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT16)
        || (pstVolume->VolumeType == VOLUME_TYPE_FAT32))
    {
       return FatDev_ReadFile(pstFileOper->hOper, buffer, len);
    }

    return RK_ERROR;
}




/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: FileDevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_INIT_
INIT FUN rk_err_t FileDevDelete(HDC dev)
{
    return RK_SUCCESS;
}


#ifdef _FILE_SHELL_
_DRIVER_FILE_FILEDEVICE_SHELL_
static uint8 * ShellFileName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "c",
    "d",
    "e",
    "f",
    "g",
    "h",
    "i",
    "j",
    "k",
    "l",
    "help",
    "\b",
};

static uint8 * ShellFileOper[] =
{
    "dir",
    "make",
    "delete",
    "help",
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
** Name: FileShell
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL API rk_err_t FileDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellFileName, pItem, StrCnt);
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
            ret = FileShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = FileShellCreate(dev,pItem);
            break;

        case 0x02:
            ret = FileShellTest(dev, pItem);
            break;

        case 0x03: // 'c'
        case 0x04: // 'd'
        case 0x05: // 'e'
        case 0x06: // 'f'
        case 0x07: // 'g'
        case 0x08: // 'h'
        case 0x09: // 'i'
        case 0x0a: // 'j'
        case 0x0b: // 'k'
        case 0x0c: // 'l'
            ret = FileShellOper(dev,pItem);
            break;

        case 0x0d:  //help
            ret = FileShellHelp(dev,pItem);
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
** Name: FileShellOperHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:FUN
** Date: 2014.11.3
** Time: 14:39:31
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellOperHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if( StrLenA( (uint8 *)pstr) != 0)
        return RK_ERROR;

    rk_print_string("fileoper命令集提供了一系列的命令对file进行操作\r\n");
    rk_print_string("fileoper包含的子命令如下:             \r\n");
    rk_print_string("dir       目录文件                    \r\n");
    rk_print_string("make      make                        \r\n");
    rk_print_string("delete    删除                        \r\n");
    rk_print_string("help      显示fileoper命令帮助信息    \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: FileShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma
** Date: 2014.11.3
** Time: 14:29:10
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if( StrLenA( (uint8 *)pstr) != 0)
        return RK_ERROR;

    rk_print_string("file命令集提供了一系列的命令对file进行操作\r\n");
    rk_print_string("file包含的子命令如下:             \r\n");
    rk_print_string("pcb       显示pcb信息             \r\n");
    rk_print_string("mc        创建                    \r\n");
    rk_print_string("test      测试file命令            \r\n");
    rk_print_string("help      显示file命令帮助信息    \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: FileShellTest
** Input:HDC dev, uint8 * pItem
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.9
** Time: 9:21:06
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellTest(HDC dev, uint8 * pstr)
{
    HDC hFileDev;
    HDC hFile;
    FILE_ATTR stFileAttr;
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
            rk_print_string("file.test : file 测试命令\r\n");
            return RK_SUCCESS;
        }
    }
#endif


    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

    if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("File device open failure");
    }

    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = L"RkTest.bin";

    #if 0
    ret = FileDev_CreateFile(hFileDev, NULL, &stFileAttr);

    if(ret != RK_SUCCESS)
    {
        rk_print_string("File Create Failure");
        return RK_SUCCESS;
    }
    #endif

    hFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);

    if(hFile <= 0)
    {
        rk_print_string("File Open Failure");
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

    //TotalSec = 0x3200; // 100M

    TotalSec = 0x80;

    rk_printf("test start = %d\n", BufSec);

    for(i = 1;  i <= (TotalSec / BufSec) * BufSec; i += BufSec)
    {

        pBuf[0] = i % 256;
        pBuf[1] = i % 256;

        for(j = 2; j < BufSec * 512; j++)
            pBuf[j] = pBuf[j - 1] + pBuf[j - 2];


        ret = FileDev_WriteFile(hFile, pBuf, BufSec * 512);

        rk_printf("write: LBA = 0x%08x, Len = %d", i - 1, BufSec);

        if(ret != BufSec)
        {
            rk_print_string("file write error");
        }
    }

    if(TotalSec % BufSec != 0)
    {


        pBuf[0] = i % 256;
        pBuf[1] = i % 256;

        for(j = 2; j < BufSec * 512; j++)
            pBuf[j] = pBuf[j - 1] + pBuf[j - 2];



        ret = FileDev_WriteFile(hFile,pBuf, (TotalSec % BufSec) * 512);

        rk_printf("write: LBA = 0x%08x, Len = %d", i, TotalSec % BufSec);

        if(ret != TotalSec % BufSec)
        {
            rk_print_string("file write error");
        }
    }

    FileDev_FileSeek(hFile, 0, 0);

    for(i = 1;  i <= (TotalSec / BufSec) * BufSec; i += BufSec)
    {

        ret = FileDev_ReadFile(hFile, pBuf, BufSec * 512);

        rk_printf("read: LBA = 0x%08x, Len = %d", i - 1, BufSec);

        if(ret != BufSec)
        {
            rk_print_string("file read error");
        }

        if((pBuf[0] != i % 256) || (pBuf[1] != i % 256))
        {
             rk_print_string("file data error");
        }


        for(j = 2; j < BufSec * 512; j++)
        {
            if(pBuf[j] != (uint8)(pBuf[j - 1] + pBuf[j - 2]))
            {
                 rk_print_string("file data error");
            }
        }

    }

    if(TotalSec % BufSec != 0)
    {

        ret = FileDev_ReadFile(hFile, pBuf, (TotalSec % BufSec) * 512);

        rk_printf("read: LBA = 0x%08x, Len = %d", i, TotalSec % BufSec);

        if(ret != TotalSec % BufSec)
        {
            rk_print_string("file read error");
        }

        if((pBuf[0] != i % 256) || (pBuf[1] != i % 256))
        {
             rk_print_string("file data error");
        }


        for(j = 2; j < (TotalSec % BufSec) * 512; j++)
        {
            if(pBuf[j] != (uint8)(pBuf[j - 1] + pBuf[j - 2]))
            {
                 rk_print_string("file data error");
            }
        }

    }

    rk_printf("test end");

    rkos_memory_free(pBuf);

    FileDev_CloseFile(hFile);

    RKDev_Close(hFileDev);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FileShellDelete
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.23
** Time: 15:42:26
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellDel(HDC dev, uint8 * pstr)
{
    return RK_SUCCESS;
}

/*******************************************************************************
** Name: FileShellMake
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.23
** Time: 15:42:03
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellCreate(HDC dev, uint8 * pstr)
{
    rk_err_t ret;
    HDC hFileDev;
    VOLUME_INF stVolumeInf;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("file.mc : create file devic cmd .\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    ret = RKDev_Create(DEV_CLASS_FILE, 0, NULL);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("file create failure");
        return RK_SUCCESS;
    }

    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

    if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("file open failure");
        return RK_SUCCESS;
    }

    stVolumeInf.hVolume = RKDev_Open(DEV_CLASS_FAT, 0, NOT_CARE);

    stVolumeInf.VolumeID = 'C';

    stVolumeInf.VolumeType = VOLUME_TYPE_FAT32;

    ret = FileDev_AddVolume(hFileDev, &stVolumeInf);
    if (ret != RK_SUCCESS)
    {
        rk_print_string("add volume failure");
        return RK_ERROR;
    }

    RKDev_Close(hFileDev);

    return ret;

}

/*******************************************************************************
** Name: FileShellDir
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.23
** Time: 15:41:40
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellDir(HDC dev, uint8 * pstr)
{
    HDC hFileDev, hDir;
    FILE_ATTR stFileAttr;
    uint16 LongName[258];
    uint8 Buf[1024];
    uint32 TotalFile, TotalDir,BufLen;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("file.dir : 目录信息 \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

    if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("file open failure");
        return RK_SUCCESS;
    }


    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = NULL;

    hDir = FileDev_OpenDir(hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
    if ((rk_err_t)hDir <= 0)
    {
        rk_print_string("dir open failure");
        RKDev_Close(hFileDev);
        return RK_SUCCESS;
    }

    rk_print_string("\ndir and file as Follow:");

    TotalDir = 0;

    BufLen = 0;
    while(1)
    {
        rk_print_string("\n");

        printf("xxx");

        if (FileDev_NextDir(hDir, 0, &stFileAttr) != RK_SUCCESS)
        {
            break;
        }


        BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
        Buf[BufLen] = ' ';
        BufLen++;
        BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
        Buf[BufLen] = ' ';
        BufLen++;

        memcpy(Buf + BufLen, "<DIR>", 5);

        BufLen += 5;

        Buf[BufLen] = ' ';
        BufLen++;

        memcpy(Buf+ BufLen, stFileAttr.ShortName, 8);
        BufLen += 8;
        *(Buf + BufLen) = '.';
        BufLen += 1;
        memcpy(Buf+ BufLen, stFileAttr.ShortName + 8, 3);
        BufLen += 3;
        UartDev_Write(dev, Buf, BufLen, SYNC_MODE, NULL);

        TotalDir++;
        BufLen = 0;

    }

    FileDev_CloseDir(hDir);

    rk_print_string("\n");


    stFileAttr.Path = L"C:\\";
    stFileAttr.FileName = NULL;

    hDir = FileDev_OpenDir(hFileDev, NULL, READ_WRITE_CTRL, &stFileAttr);
    if ((rk_err_t)hDir <= 0)
    {
        rk_print_string("dir open failure");
        RKDev_Close(hFileDev);
        return RK_ERROR;
    }

    TotalFile = 0;

    BufLen = 0;
    while(1)
    {
        if (FileDev_NextFile(hDir, 0, NULL, &stFileAttr) != RK_SUCCESS)
        {
            break;
        }

        BufLen += DateToString(stFileAttr.CrtDate, Buf + BufLen);
        Buf[BufLen] = ' ';
        BufLen++;
        BufLen += TimeToString(stFileAttr.CrtTime, stFileAttr.CrtTimeTenth, Buf + BufLen);
        Buf[BufLen] = ' ';
        BufLen++;

        memcpy(Buf + BufLen, "<FILE>", 6);

        BufLen += 6;

        Buf[BufLen] = ' ';
        BufLen++;

        memcpy(Buf+ BufLen, stFileAttr.ShortName, 8);
        BufLen += 8;
        *(Buf + BufLen) = '.';
        BufLen += 1;
        memcpy(Buf+ BufLen, stFileAttr.ShortName + 8, 3);
        BufLen += 3;
        UartDev_Write(dev, Buf, BufLen, SYNC_MODE, NULL);

        rk_print_string("\r\n");

        TotalFile++;
        BufLen = 0;

    }

    BufLen = sprintf(Buf, "   total dir %d, totoal file %d", TotalDir, TotalFile);
    UartDev_Write(dev, Buf, BufLen, SYNC_MODE, NULL);

    FileDev_CloseDir(hDir);
    RKDev_Close(hFileDev);

    return RK_SUCCESS;

}


/*******************************************************************************
** Name: FileShellOper
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.23
** Time: 15:32:40
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellOper(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellFileOper, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;

    switch (i)
    {
        case 0x00:  //dir
            ret = FileShellDir(dev,pItem);
            break;

        case 0x01:  //make
            break;

        case 0x02:  //delete
            break;

        case 0x03:  //help
            ret = FileShellOperHelp(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }

    return ret;

}


/*******************************************************************************
** Name: FileShellPcb
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.3.31
** Time: 18:06:38
*******************************************************************************/
_DRIVER_FILE_FILEDEVICE_SHELL_
SHELL FUN rk_err_t FileShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA((uint8 *)pstr, "help", 4) == 0)
        {
            rk_print_string("file.pcb  : file device pcb info.\r\n");
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

