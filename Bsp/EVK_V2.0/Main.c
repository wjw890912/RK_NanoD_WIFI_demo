/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                                    (c) Copyright 2013, RockChip.Ltd
*                                          All Rights Reserved
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
#include "BspConfig.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"

/*
*********************************************************************************************************
*                                        Macro Define
*********************************************************************************************************
*/
#define mainTimer_TASK_PRIORITY     configMAX_PRIORITIES-1
#define mainTest1_TASK_PRIORITY     2
#define mainTest2_TASK_PRIORITY     1
#define mainTest_QUEUE_SIZE         3

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

    Grf_NOC_Remap_Sel(NOC_PMU_SRAM);

    bsp_init();

    rkos_init();

    RKDev_Init();

    rkos_start();

    return 0;
}

__attribute__ ((section ("link_entry")))
int link_main(void)
{
    Main();
}


