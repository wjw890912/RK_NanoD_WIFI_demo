/*
********************************************************************************
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* File Name£º  SysReservedOperation.c
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-9-13          1.0
*    desc:
********************************************************************************
*/
#include <BspConfig.h>
#include <FreeRTOS.h>
#include "RKOS.h"
#include <global.h>
#include <FreeRTOSConfig.h>
#include "effect.h"
#include "SysInfoSave.h"

#define SYSCONFIGSIZE     512
SYSCONFIG  gSysConfig;

/*
--------------------------------------------------------------------------------
  Function name : uint32 FindIDBlock(void)
  Author        : ZHengYongzhi
  Description   : find IDBlock

  Input         :
  Return        : IDBlock OR 0xffffffff

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
//extern uint32  Load$$WIFI_FIRMWARE_ONE$$Base;
void LoadSysInformation(void)
{

}

/*
--------------------------------------------------------------------------------
  Function name : uint32 FindIDBlock(void)
  Author        : ZHengYongzhi
  Description   : find IDBlock

  Input         :
  Return        : IDBlock or 0xffffffff

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void SaveSysInformation(void)
{

}

/*
********************************************************************************
*
*                         End of SysReservedOperation.c
*
********************************************************************************
*/

