/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, zhuzhe
*                             All rights reserved.
*
* FileName: Web\Task\WebTran.c
* Owner: zhuzhe
* Date: 2014.7.2
* Time: 15:58:59
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.7.2     15:58:59   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __WEB_TASK_WEBTRAN_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "Bsp.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef rk_size_t (*WebGetEthData)(struct _WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskCtrl,void *buf);

typedef  struct _WEB_TRAN_EVENT_CLASS
{
    uint32 cmd;
    uint32 para;
    void*  buf;

}WEB_EVENT_CLASS;

typedef  struct _WEB_TRAN_TASK_CTRL_BLOCK
{
    void*           Databuf;
    int             Size;
    int             ConnectStatus;
    WebGetEthData   pGetEthData;
    struct netif *  pWifiNetif;
    xQueueHandle    WebEventQueue;
}WEB_TRAN_TASK_CTRL_BLOCK;


#define WEB_NETWORE_BUFFER_SIZE

#define _WEB_TASK_WEBTRAN_READ_  __attribute__((section("web_task_webtran_read")))
#define _WEB_TASK_WEBTRAN_WRITE_ __attribute__((section("web_task_webtran_write")))
#define _WEB_TASK_WEBTRAN_INIT_  __attribute__((section("web_task_webtran_init")))
#define _WEB_TASK_WEBTRAN_SHELL_  __attribute__((section("web_task_webtran_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static WEB_TRAN_TASK_CTRL_BLOCK * gpstWenTranCtrlBlock;



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
rk_size_t WebGetEthData903(struct _WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskCtrl,void *buf);
rk_err_t WenTranHardInit(WEB_TRAN_TASK_CTRL_BLOCK* pWebTranCtrl);
rk_err_t WenTranTaskShellTest(HDC dev, uint8 * pstr);
rk_err_t WenTranTaskShellDel(HDC dev, uint8 * pstr);
rk_err_t WenTranTaskShellMc(HDC dev, uint8 * pstr);
rk_err_t WenTranTaskShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WenTranTask_Enter
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_READ_
COMMON API void WebTranTask_Enter(void)
{

    WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskControlBlock;
    WEB_EVENT_CLASS TempWebEventItem;
    uint32 timeout;
    uint8* buf;
    uint32 len;

    if ( gpstWenTranCtrlBlock != NULL)
    {
        ethernetif_input(netif, buf, len);
        pWebTranTaskControlBlock = gpstWebCtrlBlock;
        WenTranHardInit(pWebTranTaskControlBlock);

        while(1)
        {
            buf  = pvPortMalloc(WEB_NETWORE_BUFFER_SIZE);
            len = WebGetEthData903(pWebTranTaskControlBlock,buf);

            if(len != NULL)
                ethernetif_input(pWebTranTaskControlBlock->pWifiNetif, buf,len);
        }

    }
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WebGetEthData903
** Input:struct _WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskCtrl,void *buf
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.4
** Time: 10:53:59
*******************************************************************************/
_WEB_TASK_WEBTRAN_READ_
COMMON FUN rk_size_t WebGetEthData903(struct _WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskCtrl,void *buf)
{

}



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
** Name: WenTranTask_DevInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_INIT_
INIT API rk_err_t WenTranTask_DevInit(void *pvParameters)
{

    RK_TASK_CLASS*     TaskHandler = (RK_TASK_CLASS*)pvParameters;

    portENTER_CRITICAL();

    TaskHandler->pData= (uint8*)gpstGUICtrlBlock;
    portEXIT_CRITICAL();
    return RK_SUCCESS;

exit:
    portEXIT_CRITICAL();
    return RK_ERROR;
}
/*******************************************************************************
** Name: WenTranTask_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_INIT_
INIT API rk_err_t WenTranTask_Init(void *pvParameters)
{
    RK_TASK_CLASS*   pWebTranTask = (RK_TASK_CLASS*)pvParameters;
    WEB_TRAN_TASK_CTRL_BLOCK*  pWebTranTaskControlBlock;
    rk_err_t ret;

    if ( pvParameters == NULL)
        return RK_PARA_ERR;

    portENTER_CRITICAL();

    pWebTranTaskControlBlock = (GUI_TASK_CTRL_BLOCK*) pWebTranTask->pData;

    pWebTranTaskControlBlock->pData = pWebTranTask->pData;                 // will copy the pdata to shell task block
    pWebTranTaskControlBlock->pDataEnd  = pWebTranTask->pData + pWebTranTask->DataStackSize;
    pWebTranTaskControlBlock->pData += sizeof( WEB_TRAN_TASK_CTRL_BLOCK);


    if (pWebTranTaskControlBlock->pData > pWebTranTaskControlBlock->pDataEnd)
        goto exit;

    pWebTranTaskControlBlock->WebEventQueue = WebGetQueue();
    pWebTranTaskControlBlock->ConnectStatus = NULL;
    pWebTranTaskControlBlock->pWifiNetif =  WebGetNetif();
    pWebTranTaskControlBlock->pGetEthData = WebGetEthData903;
    if (pWebTranTaskControlBlock->WebEventQueue == NULL || pWebTranTaskControlBlock->pData > pWebTranTaskControlBlock->pDataEnd)
        goto exit;

    gpstWenTranCtrlBlock = pWebTranTaskControlBlock;

    portEXIT_CRITICAL();
    return RK_SUCCESS;

exit:
    portEXIT_CRITICAL();
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
** Name: WenTranHardInit
** Input:WEB_TRAN_TASK_CTRL_BLIOCK * pWebTranCtrl
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.4
** Time: 9:07:06
*******************************************************************************/
_WEB_TASK_WEBTRAN_INIT_
INIT FUN rk_err_t WenTranHardInit(WEB_TRAN_TASK_CTRL_BLOCK* pWebTranCtrl)
{

}


#ifdef _WENTRAN_TASK_SHELL_
_WEB_TASK_WEBTRAN_SHELL_
const char* ShellWenTranName[] =
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
** Name: WenTranTask_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_SHELL_
SHELL API rk_err_t WenTranTask_Shell(HDC dev, uint8 * pstr)
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
** Name: WenTranTaskShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_SHELL_
SHELL FUN rk_err_t WenTranTaskShellTest(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WenTranTaskShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_SHELL_
SHELL FUN rk_err_t WenTranTaskShellDel(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WenTranTaskShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_SHELL_
SHELL FUN rk_err_t WenTranTaskShellMc(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WenTranTaskShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.2
** Time: 16:41:17
*******************************************************************************/
_WEB_TASK_WEBTRAN_SHELL_
SHELL FUN rk_err_t WenTranTaskShellPcb(HDC dev, uint8 * pstr)
{

}



#endif
#endif

