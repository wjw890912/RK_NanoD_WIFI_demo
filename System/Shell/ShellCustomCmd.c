/*
*********************************************************************************************************
*                                       NANO_OS The Real-Time Kernel
*                                         FUNCTIONS File for V0.X
*
*                                    (c) Copyright 2013, RockChip.Ltd
*                                          All Rights Reserved
*File    : NanoShell.C
*By      : Zhu Zhe
*Version : V0.x
*
*********************************************************************************************************
*/
#if SHELL_CUSTOM_PAR
#include "BspCOnfig.h"
#include "RKOS.h"
#include "Bsp.h"

const char* ShellCustomName[] =
{
    "function",
    "variable",
    "\b"
};

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellFunctionRun(HDC dev, uint8 * pstr)
{
}

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellVariableValue(HDC dev, uint8 * pstr)
{
}

/*
*********************************************************************************************************
*                                      voidShellTaskDeInit(void)
*
* Description:  This function is the Timer Task.
*
* Argument(s) : void *p_arg
*
* Return(s)   : none
*
* Note(s)     : none.
*********************************************************************************************************
*/
rk_err_t ShellCustomParsing(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if(StrCnt == 0)
        return RK_ERROR;



    ret = ShellCheckCmd(ShellCustomName, pItem, StrCnt);
    if(ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pstr += StrCnt;
    switch (i)
    {
        case 0x00:
            ret = ShellFunctionRun(dev,pstr);
            break;

        case 0x01:
            ret = ShellVariableValue(dev,pstr);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;


}

#endif