/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name：   Pwm.c
*
* Description:  C program template
*
* History:      <author>          <time>        <version>
*             anzhiguo      2008-11-20         1.0
*    desc:    ORG.
********************************************************************************
*/
#include "BspConfig.h"

#ifdef __DRIVER_PWM_PWMDEVICE_C__
#include "Typedef.h"
#include "DriverInclude.h"

#define _IN_PWM_

/*
--------------------------------------------------------------------------------
  Function name : void PWM_Start(int num)
  Author        : LinChongSen
  Description   : 启动PWM 输出

  Input         : num -- PWM号(0~4)

  Return        : *rate -- 当前PWM的占空比
                  *PWM_freq  -- 当前PWM的频率
--------------------------------------------------------------------
*/
//_ATTR_DRIVER_CODE_
void PWM_Start(ePWM_CHN ch)
{
    ScuClockGateCtr(CLOCK_GATE_PWM, 1);
}

/*
--------------------------------------------------------------------------------
  Function name : void PWM_Stop(int num)
  Author        : LinChongSen
  Description   : 关闭PWM 输出

  Input         : num -- PWM号(0~4)

  Return        :
--------------------------------------------------------------------------------
*/
//_ATTR_DRIVER_CODE_
void PWM_Stop(ePWM_CHN ch)
{
    Pwm->CHANNEL[ch].Tacmd &= ~(PWM_ENABLE);
    ScuClockGateCtr(CLOCK_GATE_PWM, 0);
}

//#ifdef DRIVERLIB_IRAM
/*
--------------------------------------------------------------------------------
  Function name : void PwmRegReset(int num)
  Author        : LinChongSen
  Description   : 复位PWM

  Input         : num -- PWM号(0~4)

  Return        :
--------------------------------------------------------------------------------
*/
//_ATTR_DRIVERLIB_CODE_
void PwmRegReset(ePWM_CHN ch)
{
    Pwm->CHANNEL[ch].Tacmd |= PWM_REGRESET;
}

/*
--------------------------------------------------------------------------------
  Function name : void PwmPrescalefFctorSet(int num,int mpwmPrescalefFctor)
  Author        : LinChongSen
  Description   : PWM 分频率设置

  Input         : num -- PWM号(0~4)
                  mpwmPrescalefFctor -- 0000: 1/2             0001: 1/4  (0~15)
                                        0000: 1/8             0001: 1/16
                                        0100: 1/32            0101: 1/64
                                        0110: 1/128           0111: 1/256
                                        1000: 1/512           1001: 1/1024
                                        1010: 1/2048          1011: 1/4096
                                        1100: 1/8192          1101: 1/16384
                                        1110: 1/32768         1111: 1/65536

  Return        :
  desc          : 不调用，默认为1/2分频
--------------------------------------------------------------------------------
*/
//_ATTR_DRIVERLIB_CODE_
void PwmPrescalefFctorSet(ePWM_CHN ch, ePWM_PRESCALE mpwmPrescalefFctor)
{
    Pwm->CHANNEL[ch].Tacmd |= (mpwmPrescalefFctor << 9);
}

/*
--------------------------------------------------------------------------------
  Function name : int PwmPrescalefFctorGet(int num)
  Author        : LinChongSen
  Description   : PWM 分频率读取

  Input         : num -- PWM号(0~4)
  Output        : mpwmPrescalefFctor  -- 返回2的次方数 如:2，4，8，16....65536
                                         可以参考PwmPrescalefFctorSet中的mpwmPrescalefFctor注释
  Return        :
--------------------------------------------------------------------------------
*/
//_ATTR_DRIVERLIB_CODE_
uint32 PwmPrescalefFctorGet(ePWM_CHN ch)
{
    return ((Pwm->CHANNEL[ch].Tacmd) & 0x00001E00) >> 9;
}


/*
--------------------------------------------------------------------------------
  Function name : BOOL PwmRateSet(int num,UINT32 rate, UINT32 PWM_freq)
  Author        : LinChongSen
  Description   : PWM 占空比设置

  Input         : num -- PWM号(0~4)
                  rate -- 高与低的比值(0~100)
                  PWM_freq  -- 当前PWM的频率(<10k)

  Return        :
  desc          : data1 过小,则无法正常设置
--------------------------------------------------------------------
*/
//_ATTR_DRIVER_CODE_
int32 PwmRateSet(ePWM_CHN ch, UINT32 rate, UINT32 PWM_freq)
{
    UINT32 data0,data1;
    UINT32 pwmclk;
    UINT32 pwmPrescalefFctor;

    if (rate == 100)
    {
        Pwm->CHANNEL[ch].Tacmd &= ~(PWM_ENABLE);
        Pwm->CHANNEL[ch].Tacmd |= PWM_REGRESET;

        return 0;
    }
    else if (rate == 0)
    {
        Pwm->CHANNEL[ch].Tacmd &= ~(PWM_ENABLE);
        Pwm->CHANNEL[ch].Tacmd |= PWM_REGRESET;

        return 0;
    }

    if((rate > 0) && (rate < 100))
    {
        pwmclk = GetPclkFreq();
        data1 = ((pwmclk * 1000/* * 1000*/) >> ((PwmPrescalefFctorGet(ch)) + 1)) / PWM_freq;
        data0 = rate * data1 / 100;

    	Pwm->CHANNEL[ch].Tacmd &= ~(PWM_ENABLE | PWM_OUTPUT_ENABLE | PWM_TIMEINTEN);
        Pwm->CHANNEL[ch].Tacnt = 0;
	    Pwm->CHANNEL[ch].Tadata0 = data0;
	    Pwm->CHANNEL[ch].Tadata1 = data1;
        Pwm->CHANNEL[ch].Tacmd |= (PWM_OUTPUT_ENABLE | PWM_TIMEINTEN | PWM_ENABLE);

        return 0;
    }

    return -1;
}


#endif
/*
********************************************************************************
*
*                         End of pwm.c
*
********************************************************************************
*/

