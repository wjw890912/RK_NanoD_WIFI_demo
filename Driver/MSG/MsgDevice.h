/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Driver\MSG\MsgDevice.h
* Owner: aaron.sun
* Date: 2015.6.23
* Time: 15:58:38
* Version: 1.0
* Desc: Msg Device Class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.6.23     15:58:38   1.0
********************************************************************************************
*/


#ifndef __DRIVER_MSG_MSGDEVICE_H__
#define __DRIVER_MSG_MSGDEVICE_H__

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
typedef  struct _MSG_DEV_ARG
{
    uint32 Channel;
}MSG_DEV_ARG;

#define _DRIVER_MSG_MSGDEVICE_COMMON_  __attribute__((section("driver_msg_msgdevice_common")))
#define _DRIVER_MSG_MSGDEVICE_INIT_  __attribute__((section("driver_msg_msgdevice_init")))
#define _DRIVER_MSG_MSGDEVICE_SHELL_  __attribute__((section("driver_msg_msgdevice_shell")))

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
extern rk_err_t MsgDev_CheckMsg(HDC dev,  uint32 MsgID);
extern rk_err_t MsgDev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t MsgDev_SendMsg(HDC dev, uint32 MsgID);
extern rk_err_t MsgDev_GetMsg(HDC dev, uint32 MsgID);
extern rk_err_t MsgDev_Delete(uint32 DevID, void * arg);
extern HDC MsgDev_Create(uint32 DevID, void * arg);



#endif
