/*
********************************************************************************
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                        All rights reserved.
*
* File Name£º   SysReservedOperation.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-9-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _SYSRESERVED_OP_H_
#define _SYSRESERVED_OP_H_

/*
*-------------------------------------------------------------------------------
*
*                           Macro define
*
*-------------------------------------------------------------------------------
*/

/*reserved area data storage,bit definition logic address LBA*/
#define SYSDATA_BLK            0

#define BOOKMARK_BLK        1

#define WIFI_AP_PW_BLK        2

#define XML_BUF_BLK             3

#define MEDIA_BLK            4

/*
*-------------------------------------------------------------------------------
*
*                           Struct Address
*
*-------------------------------------------------------------------------------
*/


//system keeping information for music module
#ifdef _MUSIC_
typedef __packed struct _MUSICCONFIG
{
    uint16 FileNum;                 //current file number.
    uint32 CurTime;                 //current time.
    uint8  HoldOnPlaySaveFlag;
    uint8  RepeatMode;              //repeat mode
    uint8  RepeatModeBak;
    uint8  PlayOrder;               //play order
    uint32 ShuffleSeed;
    uint8  BassBoost;//PAGE
    RKEffect Eq;                    //Eq
    UINT32 HoldMusicFullInfoSectorAddr ;
    UINT32 HoldMusicSortInfoSectorAddr;
    UINT16 HoldMusicuiBaseSortId[4];
    UINT16 HoldMusicPlayType ;
    UINT16 HoldMusicTypeSelName[SYS_SUPPROT_STRING_MAX_LEN + 1];//PAGE
    UINT16 HoldMusicucCurDeep ;
    UINT16 HoldMusicuiCurId[4]  ;
    UINT16 HoldMusicTotalFiles;
    UINT16 HoldClusSave;
}MUSIC_CONFIG;
#endif

//system setting parameter structure definition.
typedef __packed struct _SYSCONFIG
{

    uint16 FirmwareYear;
    uint16 FirmwareDate;
    uint16 MasterVersion;
    uint16 SlaveVersion;
    uint16 SmallVersion;

    uint32 eFuseSaveData;

    uint8  Memory;                  //storage media options.
    UINT8  SysLanguage;             //current system language environment.
    uint8  ImageIndex;

    UINT8  OutputVolume;            //system volume
    UINT8  BLmode;                  //backlight display mode.
    UINT8  BLevel;                  //backlight level
    UINT8  BLtime;                  //backlight time.
    UINT8  LockTime;                //lock keyboard time
    UINT8  ShutTime;                //auto close time
    UINT8  SDEnable;                //sd card select
    UINT8  FMEnable;                //FM setting menu judge.
    UINT8  KeyNum;                  //it is used for 5.6.7 keys judgement.
    UINT8  Softin;

#ifdef _MUSIC_
    MUSIC_CONFIG MusicConfig;
#endif




} SYSCONFIG,*PSYSCONFIG;


/*
*-------------------------------------------------------------------------------
*
*                           Variable define
*
*-------------------------------------------------------------------------------
*/
//
extern SYSCONFIG  gSysConfig;

/*
*-------------------------------------------------------------------------------
*
*                            Function Declaration
*
*-------------------------------------------------------------------------------
*/
extern void LoadSysInformation(void);
extern void SaveSysInformation(void);
extern void ReadReservedData(uint32 LBA , uint8 *Buf, int16 Length);
extern void WriteReservedData(uint32 LBA , uint8 *Buf, uint16 Length);
/*
********************************************************************************
*
*                         End of SysReservedOperation.h
*
********************************************************************************
*/
#endif
