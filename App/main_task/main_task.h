/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: App\main_task\main_task.h
* Owner: aaron.sun
* Date: 2015.8.20
* Time: 17:29:00
* Version: 1.0
* Desc: main task
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2015.8.20     17:29:00   1.0
********************************************************************************************
*/


#ifndef __APP_MAIN_TASK_MAIN_TASK_H__
#define __APP_MAIN_TASK_MAIN_TASK_H__

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

typedef struct _RK_TASK_MAIN_ARG
{
    uint32 xxx;
}RK_TASK_MAIN_ARG;

#define _APP_MAIN_TASK_MAIN_TASK_COMMON_  __attribute__((section("app_main_task_main_task_common")))
#define _APP_MAIN_TASK_MAIN_TASK_INIT_  __attribute__((section("app_main_task_main_task_init")))
#define _APP_MAIN_TASK_MAIN_TASK_SHELL_  __attribute__((section("app_main_task_main_task_shell")))
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
extern rk_err_t MainTask_Resume(void);
extern rk_err_t MainTask_Suspend(void);
extern void MainTask_Enter(void);
extern rk_err_t MainTask_DeInit(void *pvParameters);
extern rk_err_t MainTask_Init(void *pvParameters, void *arg);



#endif
