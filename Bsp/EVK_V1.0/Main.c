/*
*********************************************************************************************************
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                              All rights reserved.
*File    : APP.C
* By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            DESCRIPTION
*  RK_NANO_OS is a system designed specifically for real-time embedded SOC operating system ,before using
*RK_NANO_OS sure you read the user's manual
*  The TASK NAME TABLE:
*
*
*  The DEVICE NAME TABLE:
*  "UartDevice",              Uart Serial communication devices
*  "ADCDevice",               The analog signal is converted to a digital signal device
*  "KeyDevice",               Key driver device
*
*
*
*
*
*
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include "BspCOnfig.h"
#include "RKOS.h"
#include "Bsp.h"
#include "deviceinclude.h"

/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/
#define mainTimer_TASK_PRIORITY     configMAX_PRIORITIES-1
#define mainTest1_TASK_PRIORITY 	2
#define mainTest2_TASK_PRIORITY 	1
#define mainTest_QUEUE_SIZE			3

/*
*********************************************************************************************************
*                                        Variable Define
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                              Main(void)
*
* Description:  This Function is the first function.
*
* Argument(s) : none
*
* Return(s)   : int
*
* Note(s)     : none.
*********************************************************************************************************
*/
int Main(void)
{
    UART_DEV_ARG stUartArg;

    BSP_Init();
    rkos_init();

    RKDev_Init();

    rkos_start();

    return 0;
}
