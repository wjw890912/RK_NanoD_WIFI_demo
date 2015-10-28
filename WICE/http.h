/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: WICE\http.h
* Owner: lyb
* Date: 2015.6.26
* Time: 15:22:25
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    lyb     2015.6.26     15:22:25   1.0
********************************************************************************************
*/


#ifndef __WICE_HTTP_H__
#define __WICE_HTTP_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#include <typedef.h>

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#define _WICE_HTTP_COMMON_  __attribute__((section("wice_http_common")))
#define _WICE_HTTP_INIT_  __attribute__((section("wice_http_init")))
//#define _WICE_HTTP_SHELL_  __attribute__((section("wice_http_shell")))

typedef struct _RK_TASK_HTTP_ARG
{
    uint32 http_arg;
}RK_TASK_HTTP_ARG;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/

typedef void (*HTTP_URL_CALLFUNCTION)(int status);

#define ERR_OK   0
#define FIFO_OPEN_ERR   -1
#define MALLOC_ERR -2
#define FIFO_WRITE_ERR -3
#define TCP_RECIVER_ERR -4

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern void Http_Test(void);

extern void HTTPTask_Enter(void);
extern rk_err_t HTTPTask_DeInit(void *pvParameters);
extern rk_err_t HTTPTask_Init(void *pvParameters, void *arg);

extern rk_err_t HttpGet_Url(uint8 *pburl);
extern void Http_Close(void);

#endif
