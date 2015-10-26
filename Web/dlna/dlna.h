

/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Web\dlna\dlna.h
* Owner: linyb
* Date: 2015.7.21
* Time: 18:10:56
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    linyb     2015.7.21     18:10:56   1.0
********************************************************************************************
*/


#ifndef __WEB_DLNA_DLNA_H__
#define __WEB_DLNA_DLNA_H__
#include "typedef.h"
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



//#define _WEB_DLNA_DLNA_COMMON_  __attribute__((section("web_dlna_dlna_common")))
//#define _WEB_DLNA_DLNA_INIT_  __attribute__((section("web_dlna_dlna_init")))
//#define _WEB_DLNA_DLNA_SHELL_  __attribute__((section("web_dlna_dlna_shell")))
#define _DLNA_INIT_ __attribute__((section("_DLNA_INIT_")))
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/

typedef enum __Device_Xml
{
   XML_DMR_DEVICE_SCRIPTION = 0,
   XML_DMR_CONNECTIONMANAGER_SERVICE_DESCRIPTION,
   XML_DMR_RENDERINGCONTROL_SERVICE_DESCRIPTION,
   XML_DMR_AVTRANSPORT_SERVICE_DESCRIPTION,
   XML_DMR_NAMINGBITMAP_DATA,
   XML_DMR_NMSTRTPAGES_DATA,
   XML_DMR_NAMEPAGES_DATA
}DEVICE_XML;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern rk_err_t DLNATask_Resume(void);
extern rk_err_t DLNATask_Suspend(void);
extern void DLNATask_Enter(void);
extern rk_err_t DLNATask_DeInit(void *pvParameters);
extern rk_err_t DLNATask_Init(void *pvParameters, void *arg);
extern rk_err_t Cg_Get_Xml(DEVICE_XML type, char **buf,  unsigned long *buf_len);



#endif

