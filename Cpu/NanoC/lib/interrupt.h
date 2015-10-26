/*
********************************************************************************
*                   Copyright (c) 2008,ZhengYongzhi
*                         All rights reserved.
*
* File Name：   interrupt.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-8-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#undef  EXT
#ifdef _IN_INTTERUPT_
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                           Macro define
*
*-------------------------------------------------------------------------------
*/
//中断服务程序类型定义
typedef void(*ExecFunPtr)(void) __irq;

//section define
#define _ATTR_VECTTAB_              __attribute__((section("vect")))
#define _ATTR_INTRRUPT_CODE_        __attribute__((section("SysCode")))
#define _ATTR_INTRRUPT_DATA_        __attribute__((section("SysData")))
#define _ATTR_INTRRUPT_BSS_         __attribute__((section("SysBss"),zero_init))

//中断优先级定义
#define NUM_PRIORITY_BITS           4

/*
*-------------------------------------------------------------------------------
*
*                           Struct define
*
*-------------------------------------------------------------------------------
*/
//中断ID号定义
typedef enum
{
    //Cortex M3 System Intterupt
    FAULT_ID0_REV = (UINT32)0,                                  //SP Point
    FAULT_ID1_REV,                                              //Reset
    FAULT_ID2_NMI,                                              //NMI fault
    FAULT_ID3_HARD,                                             //Hard fault
    FAULT_ID4_MPU,                                              //MPU fault
    FAULT_ID5_BUS,                                              //Bus fault
    FAULT_ID6_USAGE,                                            //Usage fault
    FAULT_ID7_REV,                                              //
    FAULT_ID8_REV,                                              //
    FAULT_ID9_REV,                                              //
    FAULT_ID10_REV,                                             //
    FAULT_ID11_SVCALL,                                          //SVCall
    FAULT_ID12_DEBUG,                                           //Debug monitor
    FAULT_ID13_REV,                                             //
    FAULT_ID14_PENDSV,                                          //PendSV
    FAULT_ID15_SYSTICK,                                         //System Tick

    //RKNano Process Intterupt
    INT_ID_DMA,                                               //00 int_dma
    INT_ID_UART0,                                              //01 uart
    INT_ID_SDMMC,                                             //02 sd mmc
    INT_ID_PWM1,                                              //03 pwm1
    INT_ID_PWM2,                                              //04 pwm2
    INT_ID_IMDCT36,                                           //05 imdct36
    INT_ID_SYNTHESIZE,                                        //06 synthesize
    INT_ID_USB,                                               //07 usb
    INT_ID_I2C0,                                               //08 i2c
    INT_ID_I2S0,                                               //09 i2s
    INT_ID_GPIO,                                              //10 gpio
    INT_ID_SPI,                                               //11 spi
    INT_ID_PWM0,                                              //12 pwm0
    INT_ID_TIMER0,                                             //13 timer
    INT_ID_ADC,                                               //14 sar-adc
    INT_ID_RESERVED0,                                          //15 reserved
    INT_ID_RESERVED1,                                          //16 reserved
    INT_ID_RESERVED2,                                          //17 reserved
    INT_ID_RTC,                                               //18 rtc
    INT_ID_RESERVED3,                                          //19 reserved
    INT_ID_OOL_BAT_SNSEN,                                     //20 ool_bat_snsen
    INT_ID_RESERVED4,                                          //21 reserved
    INT_ID_OOL_PAUSE,                                         //22 ool_pause
    INT_ID_OOL_PLAYON,                                        //23 ool_PlayOn
    INT_ID_PWR_5V_READY,                                      //24 pwr_5v_ready
    INT_ID_PWR_UVLO_VDD,                                      //25 pwr_uvlo_vdd
    INT_ID_PWR_UVP,                                           //26 pwr_uvp
    INT_ID_PWR_OVP,                                           //27 pwr_ovp
    INT_ID_PWR_OT,                                            //28 pwr_ot
    INT_ID_PWR_OC,                                            //29 pwr_oc
    INT_ID_PWR_CHARGE,                                        //30 pwr_charge
    INT_ID_RESERVED5,
    NUM_INTERRUPTS,                                              //Interrupts Total Nums
    INT_ID_UART5,
    INT_ID_UART4,
    INT_ID_UART3,
    INT_ID_UART2,
    INT_ID_UART1,
    INT_ID_I2C2,
    INT_ID_I2C1,
    INT_ID_TIMER1,
    INT_ID_MAILBOX0,
    INT_ID_I2S1,

} INTERRUPT_ID;


/*
*-------------------------------------------------------------------------------
*
*                           Functon Declaration
*
*-------------------------------------------------------------------------------
*/

extern void   IntMasterEnable(void);
extern void   IntMasterDisable(void);
extern void   FaultMasterEnable(void);
extern void   FaultMasterDisable(void);
extern void   IntPriorityGroupingSet(UINT32 ulBits);
extern UINT32 IntPriorityGroupingGet(void);
extern void   IntPrioritySet(UINT32 ulInterrupt, UINT32 ucPriority);
extern UINT32 IntPriorityGet(UINT32 ulInterrupt);
extern void   IntEnable(UINT32 ulInterrupt);
extern void   IntDisable(UINT32 ulInterrupt);
extern void   IntPendingSet(UINT32 ulInterrupt);
extern void   IntPendingClear(UINT32 ulInterrupt);
extern BOOL   IntISRPendingCheck(void);
extern UINT32 IntVectPendingGet(void);
extern UINT32 IntVectActiveGet(void);
extern uint32 IsrDisable(UINT32 ulInterrupt);
extern void   IsrEnable(UINT32 ulInterrupt, uint32 IsEnable);
extern __irq void IntDefaultHandler(void);
extern void IntRegister(UINT32 ulInterrupt, ExecFunPtr(*pfnHandler)(void));
extern void IntUnregister(UINT32 ulInterrupt);
extern uint32 UserIsrDisable(void);
extern void UserIsrEnable(uint32 data);
extern void UserIsrRequest(void);
extern void IrqPriorityInit(void);


/*
********************************************************************************
*
*                         End of interrupt.h
*
********************************************************************************
*/
#endif

