/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\File\DirDevice.h
* Owner: aaron.sun
* Date: 2015.6.23
* Time: 13:54:36
* Version: 1.0
* Desc: Dir Device Class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.23     13:54:36   1.0
********************************************************************************************
*/


#ifndef __DRIVER_FILE_DIRDEVICE_H__
#define __DRIVER_FILE_DIRDEVICE_H__

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
typedef  struct _DIR_DEV_ARG
{
    uint32 Channel;

}DIR_DEV_ARG;

#define _DRIVER_FILE_DIRDEVICE_COMMON_  __attribute__((section("driver_file_dirdevice_common")))
#define _DRIVER_FILE_DIRDEVICE_INIT_  __attribute__((section("driver_file_dirdevice_init")))
#define _DRIVER_FILE_DIRDEVICE_SHELL_  __attribute__((section("driver_file_dirdevice_shell")))

typedef enum
{
   MUSIC_DB = 1,
   RECORD_DB,
   FS_FAT,
   FS_EXFAT,
   FS_NTFS,
   FS_FAT_EX_VOICE,
   FS_EXFAT_EX_VOICE,
   FS_NTFS_EX_VOICE

}FS_TYPE;


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
extern rk_err_t DirDev_GetCurFilePath(HDC dev, uint16 * path);
extern rk_err_t DirDev_GotoNextDir(HDC dev);
extern rk_err_t DirDev_GotoRootDir(HDC dev);
extern rk_err_t DirDev_GetCurFileNum(HDC dev, uint32 FileNum, uint32 * CurFileNum);
extern HDC DirDev_BuildDirInfo(HDC dev, uint8* ExtName, FS_TYPE FsType, uint32 * TotalNum);
extern rk_err_t DirDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t DirDev_Write(HDC dev);
extern rk_err_t DirDev_Read(HDC dev);
extern rk_err_t DirDev_Delete(uint32 DevID, void * arg);
extern HDC DirDev_Create(uint32 DevID, void * arg);



#endif
