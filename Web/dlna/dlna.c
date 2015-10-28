
/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Web\dlna\dlna.c
* Owner: linyb
* Date: 2015.7.21
* Time: 18:10:48
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    linyb     2015.7.21     18:10:48   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __WIFI_DLNA_C__


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"

extern void dlna_main(void const * argument);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#if 0
typedef  struct _DLNA_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}DLNA_RESP_QUEUE;
typedef  struct _DLNA_ASK_QUEUE
{
    uint32 cmd;

}DLNA_ASK_QUEUE;
typedef  struct _DLNA_TASK_DATA_BLOCK
{
    pQueue  DLNAAskQueue;
    pQueue  DLNARespQueue;

}DLNA_TASK_DATA_BLOCK;
#endif


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//static DLNA_TASK_DATA_BLOCK * gpstDLNAData;



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



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DLNATask_Resume
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.21
** Time: 18:11:12
*******************************************************************************/
//_WEB_DLNA_DLNA_COMMON_
COMMON API rk_err_t DLNATask_Resume(void)
{

}
/*******************************************************************************
** Name: DLNATask_Suspend
** Input:void
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.21
** Time: 18:11:12
*******************************************************************************/
//_WEB_DLNA_DLNA_COMMON_
COMMON API rk_err_t DLNATask_Suspend(void)
{

}
/*******************************************************************************
** Name: DLNATask_Enter
** Input:void
** Return: void
** Owner:linyb
** Date: 2015.7.21
** Time: 18:11:12
*******************************************************************************/
//_WEB_DLNA_DLNA_COMMON_
COMMON API void DLNATask_Enter(void)
{
     dlna_main(NULL);
     while(1)
    {
      rkos_sleep(7000);
    }
}

//overlay xml
rk_err_t Cg_Get_Xml(DEVICE_XML type, char **buf,  uint32 *buf_len)
{
    MODULE_INFO_T stCodeInfo;
    uint32 page_addr;
    uint32 fwsize;
    uint32 offset = 0;

    switch(type)
    {
       case XML_DMR_DEVICE_SCRIPTION:
            FW_GetModuleInfo(MODULE_ID_DMR_DEVICE_DATA, &stCodeInfo);
            break;
       case XML_DMR_CONNECTIONMANAGER_SERVICE_DESCRIPTION:
            FW_GetModuleInfo(MODULE_ID_DMR_CONNECTIONMANAGER_SERVICE_DATA, &stCodeInfo);
            break;
       case XML_DMR_RENDERINGCONTROL_SERVICE_DESCRIPTION:
            FW_GetModuleInfo(MODULE_ID_DMR_RENDERINGCONTROL_SERVICE_DATA, &stCodeInfo);
            break;
       case XML_DMR_AVTRANSPORT_SERVICE_DESCRIPTION:
            FW_GetModuleInfo(MODULE_ID_DMR_AVTRANSPORT_SERVICE_DATA, &stCodeInfo);
            break;
       case XML_DMR_NAMINGBITMAP_DATA:
            FW_GetModuleInfo(MODULE_ID_DMR_NAMINGBITMAP_DATA,  &stCodeInfo);
            break;
       case XML_DMR_NMSTRTPAGES_DATA:
            FW_GetModuleInfo(MODULE_ID_DMR_NMSTRTPAGES_DATA, &stCodeInfo);
            break;
       case XML_DMR_NAMEPAGES_DATA:
            FW_GetModuleInfo(MODULE_ID_DMR_NAMEPAGES_DATA, &stCodeInfo);
            break;
       default:
            printf("no xml\n");
            return RK_ERROR;
    }
    page_addr = stCodeInfo.CodeLoadBase;
    fwsize = stCodeInfo.CodeImageLength;
    offset = 0;

    *buf = rkos_memory_malloc(fwsize);
     if(NULL == *buf)
     {
        printf("xml get malloc fail fwsize = %d\n", fwsize);
        return RK_ERROR;
     }
     FW_ReadResourceData(page_addr, *buf, fwsize);
    *buf_len = fwsize;

    return RK_SUCCESS;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
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
** Name: DLNATask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.21
** Time: 18:11:12
*******************************************************************************/
INIT API rk_err_t DLNATask_DeInit(void *pvParameters)
{

#if 0
    rkos_queue_delete(gpstDLNAData->DLNAAskQueue);
    rkos_queue_delete(gpstDLNAData->DLNARespQueue);
    rkos_memory_free(gpstDLNAData);
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: DLNATask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:linyb
** Date: 2015.7.21
** Time: 18:11:12
*******************************************************************************/
INIT API rk_err_t DLNATask_Init(void *pvParameters, void *arg)
{

#if 0
    RK_TASK_CLASS*   pDLNATask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_DLNA_ARG * pArg = (RK_TASK_DLNA_ARG *)arg;
    DLNA_TASK_DATA_BLOCK*  pDLNATaskData;
    if (pDLNATask == NULL)
        return RK_PARA_ERR;

    pDLNATaskData = rkos_memory_malloc(sizeof(DLNA_TASK_DATA_BLOCK));
    memset(pDLNATaskData, NULL, sizeof(DLNA_TASK_DATA_BLOCK));
#endif
  return RK_SUCCESS;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif


