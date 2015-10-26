/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, zhuzhe
*                             All rights reserved.
*
* FileName: Web\Task\WebTask.c
* Owner: zhuzhe
* Date: 2014.6.16
* Time: 15:15:52
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.6.16     15:15:52   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __WEB_TASK_WEBTASK_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"
#include "init.h"
#include "dhcp.h"
#include "tcpip.h"
#include "etharp.h"
#include "udp.h"
#include "Netif.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/


#define WEB_CMD_NONE                 0x00
#define WEB_CMD_RXDATA               0x01
#define WEB_CMD_TXDATA               0x02
#define WEB_CMD_CONFIG               0x03
#define WEB_CMD_ERR                  0x04

typedef  struct _WEB_EVENT_CLASS
{
    uint32 cmd;
    uint32 para;
    void*  buf;

}WEB_EVENT_CLASS;

typedef  struct _WEB_TASK_CTRL_BLOCK
{
    char*         pData;
    char*         pDataEnd;
    void*         Databuf;
    int           Size;
    int           ConnectStatus;
    struct netif *  pWifiNetif;
    xQueueHandle  WebEventQueue;
}WEB_TASK_CTRL_BLOCK;

#define WEB_EVENT_QUEUE_LENTH         8
#define WEB_EVENT_QUEUE_SIZE          sizeof(WEB_EVENT_CLASS)

#define _WEB_TASK_WEBTASK_READ_  __attribute__((section("web_task_webtask_read")))
#define _WEB_TASK_WEBTASK_WRITE_ __attribute__((section("web_task_webtask_write")))
#define _WEB_TASK_WEBTASK_INIT_  __attribute__((section("web_task_webtask_init")))
#define _WEB_TASK_WEBTASK_SHELL_  __attribute__((section("web_task_webtask_shell")))


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static WEB_TASK_CTRL_BLOCK * gpstWebCtrlBlock;



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
void WebHttpd(void);
void WebEthIntit(void);
void WebHardInit(void);
rk_err_t WebTaskShellTest(HDC dev, uint8 * pstr);
rk_err_t WebTaskShellDel(HDC dev, uint8 * pstr);
rk_err_t WebTaskShellMc(HDC dev, uint8 * pstr);
rk_err_t WebTaskShellPcb(HDC dev, uint8 * pstr);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: Web_NetifRx
** Input:struct netif *netif, u8_t * buf, int len
** Return: void
** Owner:zhuzhe
** Date: 2014.8.19
** Time: 17:35:59
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON API void Web_NetifRx(struct netif *netif, uint8 * buf, int len)
{
    WEB_TASK_CTRL_BLOCK*  pWebTaskControlBlock  = gpstWebCtrlBlock;


    if ( gpstWebCtrlBlock != NULL)
    {
        if(netif == NULL)
            {
                  ethernetif_input(pWebTaskControlBlock->pWifiNetif, buf, len);
            }
        else
            {

                  ethernetif_input(netif, buf, len);
            }

    }
}

/*******************************************************************************
** Name: WebGetNetif
** Input:void
** Return:  struct netif
** Owner:zhuzhe
** Date: 2014.7.3
** Time: 16:46:48
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON API  struct netif *  WebGetNetif(void)
{

}
/*******************************************************************************
** Name: WebGetQueue
** Input:void
** Return: xQueueHandle
** Owner:zhuzhe
** Date: 2014.7.3
** Time: 16:19:41
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON API xQueueHandle WebGetQueue(void)
{

}
/*******************************************************************************
** Name: WebDNS
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.25
** Time: 15:33:52
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON API void WebDNS(void)
{
#if LWIP_DNS
    {
        char hostname[] = "www.baidu.com";
        ip_addr_t addr;

        dns_gethostbyname(hostname, &addr, dns_found, NULL);
    }
#endif
}
/*******************************************************************************
** Name: WebTask_Enter
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON API void WebTask_Enter(void)
{
    WEB_TASK_CTRL_BLOCK*  pWebTaskControlBlock;
    WEB_EVENT_CLASS TempWebEventItem;
    uint32 timeout;
    struct netif Netif;

    if ( gpstWebCtrlBlock != NULL)
    {
        //ethernetif_input(netif, buf, len);
        pWebTaskControlBlock = gpstWebCtrlBlock;

        WebHardInit();

        WebEthIntit();

        httpd_init();
		CGIInit();

       // WebDNS();
do_keepalive:
        while (1)
        {
            vTaskDelay(1000);
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
** Name: WebHttpd
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.25
** Time: 15:35:53
*******************************************************************************/
_WEB_TASK_WEBTASK_READ_
COMMON FUN void WebHttpd(void)
{
#if LWIP_HTTPD
    {
        httpd_init();
        CGIInit();
        rk_printf("httpd init success\n");
    }
#endif
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
** Name: WebTask_DevInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_INIT_
INIT API rk_err_t WebTask_DeInit(void *pvParameters)
{

    RK_TASK_CLASS*     TaskHandler = (RK_TASK_CLASS*)pvParameters;

    portENTER_CRITICAL();

    gpstWebCtrlBlock = NULL;
    portEXIT_CRITICAL();
    return RK_SUCCESS;

exit:
    portEXIT_CRITICAL();
    return RK_ERROR;

}
/*******************************************************************************
** Name: WebTask_Init
** Input:void *pvParameters
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_INIT_
INIT API rk_err_t WebTask_Init(void *pvParameters)
{
    RK_TASK_CLASS*   pWebTask = (RK_TASK_CLASS*)pvParameters;
    WEB_TASK_CTRL_BLOCK*  pWebTaskControlBlock;
    rk_err_t ret;

    if ( pvParameters == NULL)
        return RK_PARA_ERR;

    portENTER_CRITICAL();

    pWebTaskControlBlock = ( WEB_TASK_CTRL_BLOCK* ) pWebTask->pData;

    pWebTaskControlBlock->pData = pWebTask->pData;                 // will copy the pdata to shell task block
    pWebTaskControlBlock->pDataEnd  = pWebTask->pData + pWebTask->DataStackSize;

    pWebTaskControlBlock->pData += sizeof(WEB_TASK_CTRL_BLOCK);


    if (pWebTaskControlBlock->pData > pWebTaskControlBlock->pDataEnd)
        goto exit;

    pWebTaskControlBlock->WebEventQueue = xQueueCreate(WEB_EVENT_QUEUE_LENTH,WEB_EVENT_QUEUE_SIZE);
    pWebTaskControlBlock->ConnectStatus = NULL;
    pWebTaskControlBlock->pWifiNetif =  (struct netif *)pWebTaskControlBlock->pData;
    pWebTaskControlBlock->pData+= sizeof(struct netif);

    if ( pWebTaskControlBlock->WebEventQueue == NULL || pWebTaskControlBlock->pData > pWebTaskControlBlock->pDataEnd)
        goto exit;

    gpstWebCtrlBlock = pWebTaskControlBlock;

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
** Name: WebEthIntit
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.24
** Time: 11:08:41
*******************************************************************************/
_WEB_TASK_WEBTASK_INIT_
INIT FUN void WebEthIntit(void)
{
    WEB_TASK_CTRL_BLOCK*  pWebTaskControlBlock = gpstWebCtrlBlock;
    WEB_EVENT_CLASS TempWebEventItem;
    struct netif * pNetif  =  pWebTaskControlBlock->pWifiNetif;
    uint32 timeout;


    tcpip_init(NULL, NULL);
    memset(pWebTaskControlBlock->pWifiNetif, 0, sizeof(struct netif));
	netif_add(pWebTaskControlBlock->pWifiNetif, NULL, NULL, NULL, NULL,ethernetif_init, ethernet_input);
    netif_set_default(pWebTaskControlBlock->pWifiNetif);
    netif_set_link_up(pWebTaskControlBlock->pWifiNetif);
    dhcp_start(pWebTaskControlBlock->pWifiNetif);

    do
    {
        vTaskDelay(100);
        if ( netif_is_up(pWebTaskControlBlock->pWifiNetif))
        {
                   rk_printf("IP %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
                   ip4_addr1_16(&(pNetif->ip_addr)), ip4_addr2_16(&(pNetif->ip_addr)),
                   ip4_addr3_16(&(pNetif->ip_addr)), ip4_addr4_16(&(pNetif->ip_addr)));
                   rk_printf("GW %"U16_F".%"U16_F".%"U16_F".%"U16_F"\n",
                          ip4_addr1_16(&(pNetif->ip_addr)), ip4_addr2_16(&(pNetif->ip_addr)),
                          ip4_addr3_16(&(pNetif->ip_addr)), ip4_addr4_16(&(pNetif->ip_addr)));
                          break;
                      }
                  }
                  while (1);


}
/*******************************************************************************
** Name: WebHardInit
** Input:void
** Return: void
** Owner:zhuzhe
** Date: 2014.6.18
** Time: 9:23:35
*******************************************************************************/
_WEB_TASK_WEBTASK_INIT_
INIT FUN void WebHardInit(void)
{
   #if 0
    WEB_TASK_CTRL_BLOCK*  pWebTaskControlBlock = gpstWebCtrlBlock;
    WEB_EVENT_CLASS TempWebEventItem;
    uint32 timeout;


do_connect:
    if (pWebTaskControlBlock != NULL)
    {
        //UserConToAP();  cofig the password ,wait the hardware config
        timeout = 30;
        do
        {
            vTaskDelay(100);
            if (pWebTaskControlBlock->ConnectStatus  == 1)
            {
                pWebTaskControlBlock->ConnectStatus = 0;
                break;
            }
            if (!timeout--)
            {
                goto do_connect;
            }
        }
        while (1);
    }
    #endif
}


#ifdef _WEB_TASK_SHELL_
_WEB_TASK_WEBTASK_SHELL_
const char* ShellWebName[] =
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
** Name: WebTask_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_SHELL_
SHELL API rk_err_t WebTask_Shell(HDC dev, uint8 * pstr)
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
** Name: WebTaskShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_SHELL_
SHELL FUN rk_err_t WebTaskShellTest(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebTaskShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_SHELL_
SHELL FUN rk_err_t WebTaskShellDel(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebTaskShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_SHELL_
SHELL FUN rk_err_t WebTaskShellMc(HDC dev, uint8 * pstr)
{

}
/*******************************************************************************
** Name: WebTaskShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.6.16
** Time: 17:03:04
*******************************************************************************/
_WEB_TASK_WEBTASK_SHELL_
SHELL FUN rk_err_t WebTaskShellPcb(HDC dev, uint8 * pstr)
{

}



#endif
#endif

