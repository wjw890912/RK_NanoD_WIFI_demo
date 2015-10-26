/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..System\FirmwareAnalysis\FwAnalysis.h
* Owner: HJ
* Date: 2014.3.10
* Time: 13:51:36
* Desc: FirmwareAnalysis
* History:
*   <author>    <date>       <time>     <version>     <Desc>
*      hj     2014.3.10     13:51:36   1.0
********************************************************************************************
*/

#ifndef __DRIVER_FWAnalysis_FWAnalysis_H__
#define __DRIVER_FWAnalysis_FWAnalysis_H__
#include "ModuleInfoTab.h"
/*
*-------------------------------------------------------------------------------
*
*                           Macro define
*
*-------------------------------------------------------------------------------
*/
//ModuleLoad Type define
#define MODULE_OVERLAY_CODE         0x01
#define MODULE_OVERLAY_DATA         0x02
#define MODULE_OVERLAY_BSS          0x04
#define MODULE_OVERLAY_ALL          (MODULE_OVERLAY_CODE | MODULE_OVERLAY_DATA | MODULE_OVERLAY_BSS)

#define MAX_CHAR_ENC_SUPPORT    5
#define MAX_FONT_SUPPORT        10
#define MAX_MENU_SUPPORT        10
#define MAX_UI_SUPPORT          10

/*
*-------------------------------------------------------------------------------
*
*                           Struct Address
*
*-------------------------------------------------------------------------------
*/
#define MAX_CHAR_ENC_SUPPORT    5
#define MAX_FONT_SUPPORT        10
#define MAX_MENU_SUPPORT        10
#define MAX_UI_SUPPORT          10

typedef enum _ENUM_CODE
{
    USB_BOOT,
    NAND_BOOT1,
    NAND_BOOT2,
    SYS_CODE,
    CODE_NUM

}ENUM_CODE;

typedef enum _ENUM_FONT
{
    FONT12,
    FONT16,
    FONT_NUM

}ENUM_FONT;

typedef enum _ENUM_CHAR_ENC
{
    GBK,
    CHAR_ENC_NUM
}ENUM_CHAR_ENC;

typedef enum _ENUM_UI
{
    UI1,
    UI_NUM
}ENUM_UI;


/***********模块*********/
typedef struct _MODULE_INFO
{

    unsigned int ModuleOffset;
    unsigned int ModuleLength;

} MODULE_INFO;


/***********代码信息***********/
typedef struct _CODE_INFO
{

    unsigned int    CodeNum;
    MODULE_INFO   CodeInfoTbl[CODE_NUM];

} CODE_INFO;

/*************字体**************/
typedef struct _FONT_INFO
{

    unsigned int    FontNum;
    MODULE_INFO   FontInfoTbl[MAX_FONT_SUPPORT];

} FONT_INFO;

/***********字符编码************/
typedef struct _CHAR_ENC_INFO
{

    unsigned int    CharEncNum;
    MODULE_INFO   CharEncInfoTbl[MAX_CHAR_ENC_SUPPORT];

} CHAR_ENC_INFO;

/************菜单***************/
typedef struct _MENU_INFO
{

    unsigned int    MenuNum;
    MODULE_INFO   MenuInfoTbl[MAX_MENU_SUPPORT];

} MENU_INFO;

/*************UI****************/
typedef struct _UI_INFO
{

    unsigned int    UiNum;
    MODULE_INFO   UiInfoTbl[MAX_UI_SUPPORT];

} UI_INFO;
/***********RESOURCE_ADDR**********/
typedef struct _FIRMWARE_RESOURCE_ADDR
{
    //Firmware Info addr
    uint32      Font12LogicAddress;     //char font 12
    uint32      Font16LogicAddress;     //char font 16
    uint32      IDBlockByteAddress;     //reserved
    uint32      CodeLogicAddress;       // code
    uint32      FontLogicAddress;       //char font 12/16
    uint32      GBKLogicAddress;        //GBK
    uint32      MenuLogicAddress;       //Ment
    uint32      ImageLogicAddress;      //Image
    uint32      LoadStartBase;

}FIRMWARE_RESOURCE_ADDR, *PFIRMWARE_RESOURCE_ADDR;

/***********FIRMWARE_HEADER**********/
typedef struct _FIRMWARE_HEADER
{
    unsigned short  Year;               // BCD
    unsigned short  Date;               // BCD
    unsigned short  MasterVer;          // BCD
    unsigned short  SlaveVer;           // BCD
    unsigned short  SmallVer;           // BCD
    unsigned short  LoaderVer;          // BCD
    unsigned char   MachineID[4];       // 机器ID,与固件匹配
    unsigned char   VendorName[32];     // 厂商名
    unsigned char   ModelName[32];      // 型号名
    CODE_INFO       CodeInfo;           // 代码信息，包括UsbBoot, NandBoot1, NandBoot2, SysCode
    FONT_INFO       FontInfo;           // 字库信息
    CHAR_ENC_INFO   CharEncInfo;        // 字符编码信息，目前仅包含GBK编码
    MENU_INFO       MenuInfo;           // 菜单资源信息
    UI_INFO         UiInfo;             // UI资源信息
    unsigned char   Reserved[88];
    unsigned long   FwEndOffset;
    unsigned char   FwSign[8];          // 固件标记，"RkNanoFw"

}FIRMWARE_HEADER, *PFIRMWARE_HEADER;

typedef struct PictureInfoStruct
{
    UINT16  xSize;
    UINT16  ySize;
    UINT16  x;
    UINT16  y;
    UINT32  totalSize;                          /* it equal with xSize multiply ySize */
    UINT32  offsetAddr;                         /* the offset address that picture saved in flash */

} PICTURE_INFO_STRUCT;

/*menu text resource information structure,the order of members cannot be changed. */
typedef struct MenuTextInfoStruct
{
    UINT16  SelfID;                             /* menu item id */
    UINT16  FatherID;                           /* father menu id*/
    UINT16  CurItemInFatherID;                  /* the serial number in father menu */
    UINT16  ChildID ;                           /* frist menu item id */
    UINT16  PrevID;                             /* left brother menu item id.*/
    UINT16  NextID;                             /* right brother menu item id.*/
    UINT16  FunctionIndex;                      /* index of implement function.*/
    UINT16  ChildNum;                           /* total number of child items.*/
    UINT16  ChildIDArray[21];                   /* the id groud of child menu items*/

} MENU_TEXT_INFO_STRUCT;


typedef  struct _D_MODULE_INFO
{
    struct _D_MODULE_INFO * pstDmodule;
    uint32 ModuleID;
    uint32 LoadBase;
    uint32 ImageBase;
    uint32 ImageLen;

}D_MODULE_INFO;

extern D_MODULE_INFO * gpstDmodule;
extern FIRMWARE_RESOURCE_ADDR gstFW_Resourec_addr;



/*
*-------------------------------------------------------------------------------
*
*                           Variable define
*
*-------------------------------------------------------------------------------
*/
//

/*
*-------------------------------------------------------------------------------
*
*                            Function Declaration
*
*-------------------------------------------------------------------------------
*/
extern rk_err_t FWShell(HDC dev,  uint8 * pstr);
extern void FW_GetPicResourceData(uint32 FlashAddr, uint8 *pData, uint16 length);
extern void FW_GetPicInfoWithIDNum(UINT16 pictureIDNum, PICTURE_INFO_STRUCT *psPictureInfo);
extern void FW_GetMenuInfoWithIDNum(UINT32 menuTextID, MENU_TEXT_INFO_STRUCT *pMenuTextInfo);
extern void FW_GetMenuResourceData(UINT16 menuTextID , UINT16 *pMenuStr ,UINT16 StrLen);
extern void FW_ReadResourceData(uint32 Addr, uint8 *pData, uint32 length);
extern rk_err_t FW_GetModuleInfo(uint32 ModuleID, MODULE_INFO_T * Module);
extern rk_err_t FW_ModuleOverlay(uint32 ModuleID, uint8 Type);
extern rk_err_t FW_RemoveModule(uint32 ModuleID);

#endif
