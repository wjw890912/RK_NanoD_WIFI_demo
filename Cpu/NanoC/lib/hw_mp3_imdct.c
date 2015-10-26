


/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: Cpu\NanoC\lib\hw_mp3_imdct.c
* Owner: WJR
* Date: 2014.11.10
* Time: 17:29:02
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    WJR     2014.11.10     17:29:02   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __CPU_NANOC_LIB_HW_MP3_IMDCT_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "interrupt.h"
#include "hw_mp3_imdct.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _CPU_NANOC_LIB_HW_MP3_IMDCT_READ_  __attribute__((section("cpu_nanoc_lib_hw_mp3_imdct_read")))
#define _CPU_NANOC_LIB_HW_MP3_IMDCT_WRITE_ __attribute__((section("cpu_nanoc_lib_hw_mp3_imdct_write")))
#define _CPU_NANOC_LIB_HW_MP3_IMDCT_INIT_  __attribute__((section("cpu_nanoc_lib_hw_mp3_imdct_init")))
#define _CPU_NANOC_LIB_HW_MP3_IMDCT_SHELL_  __attribute__((section("cpu_nanoc_lib_hw_mp3_imdct_shell")))


#define UINT32 unsigned long

//用于开关中断
#define SETENA0_REG      *((volatile unsigned long*)(0xE000E100))
#define CLRENA0_REG      *((volatile unsigned long*)(0xE000E180))
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

volatile int dma1_busy = 0;
volatile int dma1_trans_type = 0;
volatile int imdct_finish = 1;
volatile int imdct36_auto_output_addr;  //表示在做完imdct36运算后，自动将结果存放到哪里



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
int DMA1ToImdct(unsigned long pSrc);
int DMA1FromImdct(unsigned long pDst);


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------

/*******************************************************************************
** Name: imdct36_hw_pipeline
** Input:long *X, long* z
** Return: void
** Owner:WJR
** Date: 2014.11.10
** Time: 15:37:13
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
READ API void imdct36_hw_pipeline(long *X, long* z)
{
    imdct36_auto_output_addr = (int)z;

    if (!DMA1ToImdct((long)X))      //dma1 is busy, use memcpy
    {
        //memcpy((void *)IMDCT_BASEADDR,X,sizeof(int)*18);
        long *p = (long *)IMDCT_BASEADDR;
        p[0] = X[0];
        p[1] = X[1];
        p[2] = X[2];
        p[3] = X[3];
        p[4] = X[4];
        p[5] = X[5];
        p[6] = X[6];
        p[7] = X[7];
        p[8] = X[8];
        p[9] = X[9];
        p[10] = X[10];
        p[11] = X[11];
        p[12] = X[12];
        p[13] = X[13];
        p[14] = X[14];
        p[15] = X[15];
        p[16] = X[16];
        p[17] = X[17];
    }
}
/*******************************************************************************
** Name: imdct36_handler
** Input:void
** Return: void
** Owner:WJR
** Date: 2014.11.10
** Time: 15:36:22
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
READ API void imdct36_handler(void)
{
    IMDCT_EOIT_REG = 0;            //clear the interrupt

    if (!DMA1FromImdct((unsigned long)imdct36_auto_output_addr)) //dma busy, use memcpy
    {
        long *p = (long *)imdct36_auto_output_addr;
        long *q = (long *)(IMDCT_BASEADDR + 18*4);
        p[ 0] = q[ 0];
        p[ 1] = q[ 1];
        p[ 2] = q[ 2];
        p[ 3] = q[ 3];
        p[ 4] = q[ 4];
        p[ 5] = q[ 5];
        p[ 6] = q[ 6];
        p[ 7] = q[ 7];
        p[ 8] = q[ 8];
        p[ 9] = q[ 9];
        p[10] = q[10];
        p[11] = q[11];
        p[12] = q[12];
        p[13] = q[13];
        p[14] = q[14];
        p[15] = q[15];
        p[16] = q[16];
        p[17] = q[17];
        p[18] = q[18];
        p[19] = q[19];
        p[20] = q[20];
        p[21] = q[21];
        p[22] = q[22];
        p[23] = q[23];
        p[24] = q[24];
        p[25] = q[25];
        p[26] = q[26];
        p[27] = q[27];
        p[28] = q[28];
        p[29] = q[29];
        p[30] = q[30];
        p[31] = q[31];
        p[32] = q[32];
        p[33] = q[33];
        p[34] = q[34];
        p[35] = q[35];
        imdct_finish = 1;
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
** Name: DMA1ToImdct
** Input:unsigned long pSrc
** Return: int
** Owner:WJR
** Date: 2014.11.10
** Time: 15:34:55
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
READ FUN int DMA1ToImdct(unsigned long pSrc)
{

    int tmp;

    //for debug
    return false;

    tmp = SETENA0_REG;

    //关中断
    CLRENA0_REG = tmp;//0xfffffffe;
    if (dma1_busy != 0)
    {
        SETENA0_REG = tmp;//0xfffffffe;
        return false;
    }

    dma1_busy = 1;
    dma1_trans_type = 1;
    imdct_finish = 0;

    WriteReg32(DMA_ISA1, pSrc);
    WriteReg32(DMA_CTL1, DMA_SHORTCUT_TO_IMDCT);        //DMA传输会在配置的同时立刻开始

    //开中断
    SETENA0_REG = tmp; //0xfffffffe;

    return true;

}
/*******************************************************************************
** Name: DMA1FromImdct
** Input:unsigned long pDst
** Return: int
** Owner:WJR
** Date: 2014.11.10
** Time: 15:33:26
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
READ FUN int DMA1FromImdct(unsigned long pDst)
{
    int tmp;

    //for debug
    return false;

    if (dma1_busy != 0)
        return false;

    tmp = SETENA0_REG;

    //关中断
    CLRENA0_REG = tmp;//0xfffffffe;

    dma1_busy = 1;
    dma1_trans_type = 2;

    WriteReg32(DMA_IDA1, pDst);
    WriteReg32(DMA_CTL1, DMA_SHORTCUT_FROM_IMDCT);      //DMA传输会在配置的同时立刻开始

    //开中断
    SETENA0_REG = tmp; //0xfffffffe;

    return true;

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
** Name: Hw_imdct_Exit
** Input:void
** Return: void
** Owner:WJR
** Date: 2014.11.10
** Time: 15:39:12
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
INIT API void Hw_imdct_Exit(void)
{
    IMDCT_EOIT_REG = 0;
    IMDCT_CTRL_REG = 0;                         //Disable IMDCT
}
/*******************************************************************************
** Name: Hw_imdct_Init
** Input:void
** Return: void
** Owner:WJR
** Date: 2014.11.10
** Time: 15:38:29
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_READ_
INIT API void Hw_imdct_Init(void)
{
    dma1_busy = 0;
    dma1_trans_type = 0;
    imdct_finish = 1;

    //Clear Int flags
    IMDCT_EOIT_REG = 0;
    IMDCT_CTRL_REG = 3;
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
/*******************************************************************************
** Name: hw_imdct_shell
** Input:void
** Return: rk_err_t
** Owner:WJR
** Date: 2014.11.10
** Time: 17:51:08
*******************************************************************************/
_CPU_NANOC_LIB_HW_MP3_IMDCT_SHELL_
long test_data_imdct[]=
    {0xf8584 ,0x1557f0 ,0xe2c7c ,0x0 ,0x0 ,0xfffe27b4 ,
     0x0 ,0x0 ,0xffc00000 ,0x0 ,0xffc00400 ,0xffff8c00 ,
     0x0 ,0xfffcfa00 ,0x0 ,0x0 ,0x0 ,0xffc92000
    };

long test_out_data_imdct[36]=
    {0x135 ,0xfffffcba ,0x5bc ,0xfffffb3f ,0xfffffda1 ,0xa ,
    0x99f ,0xfffff818 ,0xfffffddc ,0x223 ,0x7e7 ,0xfffff660 ,
    0xfffffff5 ,0x25e ,0x4c0 ,0xfffffa43 ,0x345 ,0xfffffeca ,
    0x339 ,0xb4 ,0x146 ,0xfffff9e5 ,0x244 ,0x2d7 ,
    0xfffffa87 ,0xfffff656 ,0x290 ,0x290 ,0xfffff656 ,0xfffffa87 ,
    0x2d7 ,0x244 ,0xfffff9e5 ,0x146 ,0xb4 ,0x339
    };
SHELL API rk_err_t hw_imdct_shell()
{
    long *pSrc = test_data_imdct;
    long *pDst ;
    Hw_imdct_Init();
    IntPendingClear(INT_ID_IMDCT);
    IntRegister(INT_ID_IMDCT, imdct36_handler);
    IntEnable(INT_ID_IMDCT);
    imdct36_hw_pipeline(pSrc,pDst);
    Hw_imdct_Exit();
    {
        int i;
        int *imdct36_auto_output=&imdct36_auto_output_addr;
        for (i = 0;i<36 ;i++)
        {
            if (imdct36_auto_output[i] != test_out_data_imdct[i])
            {
                break;
            }
        }
        if (i == 36)
        {
            rk_print_string1("\r\n imdct test over");
            return  RK_SUCCESS;
        }
        else
        {
            rk_printf("\r\n imdct test error  %d",i);
                {
                    int i;
                    for(i = 0;i<36;i++)
                    {
                      rk_printf(" %0x,",imdct36_auto_output[i]);
                      if((i+1)%6 == 0)
                        {
                          rk_printf("\n");
                        }
                    }
                }
            return RK_ERROR  ;
        }
    }

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

