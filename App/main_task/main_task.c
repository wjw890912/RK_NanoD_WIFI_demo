/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\main_task\main_task.c
* Owner: aaron.sun
* Date: 2015.8.20
* Time: 17:28:49
* Version: 1.0
* Desc: main task
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.8.20     17:28:49   1.0
********************************************************************************************
*/


#include "BspConfig.h"
#ifdef __APP_MAIN_TASK_MAIN_TASK_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
typedef  struct _MAIN_RESP_QUEUE
{
    uint32 cmd;
    uint32 status;

}MAIN_RESP_QUEUE;
typedef  struct _MAIN_ASK_QUEUE
{
    uint32 cmd;

}MAIN_ASK_QUEUE;
typedef  struct _MAIN_TASK_DATA_BLOCK
{
    pQueue  MainAskQueue;
    pQueue  MainRespQueue;

}MAIN_TASK_DATA_BLOCK;



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static MAIN_TASK_DATA_BLOCK * gpstMainData;



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
** Name: MainTask_Resume
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 17:30:03
*******************************************************************************/
_APP_MAIN_TASK_MAIN_TASK_COMMON_
COMMON API rk_err_t MainTask_Resume(void)
{

}
/*******************************************************************************
** Name: MainTask_Suspend
** Input:void
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 17:30:03
*******************************************************************************/
_APP_MAIN_TASK_MAIN_TASK_COMMON_
COMMON API rk_err_t MainTask_Suspend(void)
{

}
/*******************************************************************************
** Name: MainTask_Enter
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 17:30:03
*******************************************************************************/
_APP_MAIN_TASK_MAIN_TASK_COMMON_
COMMON API void MainTask_Enter(void)
{
    UART_DEV_ARG stUartArg;
    void * uarthdc;
    stUartArg.dwBitWidth = UART_DATA_8B;
    stUartArg.dwBitRate = UART_BR_115200;
    stUartArg.Channel = UART_CHA;
    FREQ_EnterModule(FREQ_INIT);
    //FREQ_EnterModule(FREQ_BLON);
    RKDev_Create(DEV_CLASS_UART, UART_DEV1, &stUartArg);

    UartHDC = RKDev_Open(DEV_CLASS_UART, UART_DEV1, NOT_CARE);
    rk_print_string("aaaaaaaaaaaaaaaa\n");
    rk_print_string("aaaaaaaaaaaaaaaa\n");
    rk_print_string("aaaaaaaaaaaaaaaa\n");
    // Grf_GpioMuxSet(GPIO_CH2, GPIOPortA_Pin3, 2);
    // OBS_output_Source_sel(obs_clk_cal_core);

    IntMasterEnable();
#ifndef _JTAG_DEBUG_
#ifdef __DRIVER_INNOPHACE_WIFI_C__
    FW_ModuleOverlay(MODULE_ID_INNO_WIFI, MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_LWIP, MODULE_OVERLAY_CODE);
#endif
#endif

#ifdef __WICE_HTTP_C__
    RKTaskCreate(TASK_ID_HTTP, 0, NULL);
#endif

#ifdef _DRIVER_AP6181_WIFI_C__
  if (DeviceTask_CreateDeviceList(DEVICE_LIST_SDIO_FIFO) != RK_SUCCESS)
  {
        printf("sdio  create fail\n");
  }
#ifndef _JTAG_DEBUG_
    FW_ModuleOverlay(MODULE_ID_AP6181_WIFI, MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_LWIP, MODULE_OVERLAY_CODE);
#endif
    wifi_application_start_test();
#endif

#ifdef __DRIVER_INNOPHACE_WIFI_C__
    lwip_test_main();
#endif
    // os_api_test();
    while (1)
    {
        rkos_sleep(10000);
    }
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
** Name: MainTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 17:30:03
*******************************************************************************/
_APP_MAIN_TASK_MAIN_TASK_INIT_
INIT API rk_err_t MainTask_DeInit(void *pvParameters)
{

    rkos_queue_delete(gpstMainData->MainAskQueue);
    rkos_queue_delete(gpstMainData->MainRespQueue);
    rkos_memory_free(gpstMainData);
}
/*******************************************************************************
** Name: MainTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2015.8.20
** Time: 17:30:03
*******************************************************************************/
_APP_MAIN_TASK_MAIN_TASK_INIT_
INIT API rk_err_t MainTask_Init(void *pvParameters, void *arg)
{

    RK_TASK_CLASS*   pMainTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_MAIN_ARG * pArg = (RK_TASK_MAIN_ARG *)arg;
    MAIN_TASK_DATA_BLOCK*  pMainTaskData;
    if (pMainTask == NULL)
        return RK_PARA_ERR;

    pMainTaskData = rkos_memory_malloc(sizeof(MAIN_TASK_DATA_BLOCK));
    memset(pMainTaskData, NULL, sizeof(MAIN_TASK_DATA_BLOCK));

    //gpstMainData->DeviceAskQueue = rkos_queue_create(1, sizeof(MAIN_ASK_QUEUE));
    //gpstMainData->DeviceRespQueue = rkos_queue_create(1, sizeof(MAIN_RESP_QUEUE));


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
