/*
********************************************************************************
*                   Copyright (c) 2008,anzhiguo
*                         All rights reserved.
*
* File Name：   Hw_pwm.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             anzhiguo      2009-1-14          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_PWM_H_
#define _HW_PWM_H_

#undef  EXT
#ifdef  _IN_PWM_
#define EXT
#else
#define EXT extern
#endif


/*
--------------------------------------------------------------------------------

            external memory control memap register define

--------------------------------------------------------------------------------
*/
typedef volatile struct {

    UINT32 Tacnt;       //count 寄存器 0x40006010  R
    UINT32 Tadata0;     //DATA0寄存器 0x40006008   R/W
    UINT32 Tadata1;     //DATA1寄存器 0x4000600c  R/W
    UINT32 Tacmd;       //命令寄存器 bit0 timer enable， bit1 是否清（1）count reg， bit2 中断enble位 地址0x40006004 R/W

} PWM_t,*gPWM_t;

typedef volatile struct
{
    PWM_t CHANNEL[5];

}PWM_REGs,*gPWM_REGs;

#define Pwm     ((PWM_REGs *) PWM_BASE)

/*
*-------------------------------------------------------------------------------
*
*  The following define the bit fields in the PWMControlReg .
*
*-------------------------------------------------------------------------------
*/
#define PWM_ENABLE              ((UINT32)1<<0)  //timer 使能 0:Disable 1:Enable
#define PWM_OUTPUT_ENABLE       ((UINT32)1<<3)  //output 使能 0:Disable 1:Enable
#define PWM_SCMODE              ((UINT32)1<<4)  //计数方式
#define PWM_TIMEINTEN           ((UINT32)1<<5)  //timer 中断使能 0:Disable 1:Enable
#define PWM_INTCLR              ((UINT32)1<<6)  //写1 清中断（TA_INT0） TACON寄存器的0位
#define PWM_REGRESET            ((UINT32)1<<7)  //0: Normal operation  1: Reset PWM
#define PWM_CMODEEN             ((UINT32)1<<8)  //Capture mode 使能 0:Disable 1:Enable
#define PWM_PRESCALE_FACTOR     ((UINT32)1<<9)  //0000:1/2 1111:1/65536

/*
********************************************************************************
*
*                         End of hw_pwm.h
*
********************************************************************************
*/
#endif

