/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, zhuzhe
*                             All rights reserved.
*
* FileName: Web\Task\WebApp.c
* Owner: zhuzhe
* Date: 2014.6.19
* Time: 17:01:55
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.6.19     17:01:55   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __WEB_TASK_WEBAPP_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "Bsp.h"
#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"
#include "AppInclude.h"
#include "ModuleFormat.h"
#include "LcdDriver.h"
#include "FwAnalysis.h"
#include "GUITask.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _WEBAPP_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}WEBAPP_RESP_QUEUE;
typedef  struct _WEBAPP_ASK_QUEUE
{
    uint32 cmd;

}WEBAPP_ASK_QUEUE;

typedef  struct _WEBAPP_TASK_DATA_BLOCK
{
    void*           Databuf;
    int             Size;
    int             ConnectStatus;
    WebGetEthData   pGetEthData;
    xQueueHandle    WebEventQueue;
    HDC             pRk903Dev;
    HDC             pWlcDev;
}WEBAPP_TASK_DATA_BLOCK;

#define _WEB_TASK_WEBAPP_READ_  __attribute__((section("web_task_webapp_read")))
#define _WEB_TASK_WEBAPP_WRITE_ __attribute__((section("web_task_webapp_write")))
#define _WEB_TASK_WEBAPP_INIT_  __attribute__((section("web_task_webapp_init")))
#define _WEB_TASK_WEBAPP_SHELL_  __attribute__((section("web_task_webapp_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static WEBAPP_TASK_DATA_BLOCK * gpstWebAppDataBlock;



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
rk_err_t WebAppTaskShellTest(HDC dev, uint8 * pstr);
rk_err_t WebAppTaskShellDel(HDC dev, uint8 * pstr);
rk_err_t WebAppTaskShellMc(HDC dev, uint8 * pstr);
rk_err_t WebAppTaskShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WebAppTask_Enter
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_READ_
COMMON API void WebAppTask_Enter(void)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WebAppTask_DevInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_INIT_
INIT API rk_err_t WebAppTask_DevInit(void *pvParameters)
{

}
/*******************************************************************************
** Name: WebAppTask_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_INIT_
INIT API rk_err_t WebAppTask_Init(void *pvParameters)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


#ifdef _WEBAPP_TASK_SHELL_
_WEB_TASK_WEBAPP_SHELL_
const char* ShellWebAppName[] =
{
    "pcb",
    "mc",
    "del",
    "test",
    "...",
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
** Name: WebAppTask_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_SHELL_
SHELL API rk_err_t WebAppTask_Shell(HDC dev, uint8 * pstr)
{

}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WebAppTaskShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_SHELL_
SHELL FUN rk_err_t WebAppTaskShellTest(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebAppTaskShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_SHELL_
SHELL FUN rk_err_t WebAppTaskShellDel(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebAppTaskShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_SHELL_
SHELL FUN rk_err_t WebAppTaskShellMc(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebAppTaskShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.19
** Time: 17:02:17
*******************************************************************************/
_WEB_TASK_WEBAPP_SHELL_
SHELL FUN rk_err_t WebAppTaskShellPcb(HDC dev, uint8 * pstr)
{

}



#endif
#endif

