/*
********************************************************************************
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* File Name：   interrupt.c
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-7-21          1.0
*    desc:
********************************************************************************
*/
#define _IN_INTTERUPT_

#include <typedef.h>
#include <DriverInclude.h>


typedef UINT32       (*p__MRS_MSP)(void);

extern int Main(void);
extern int PowerOn_Reset(void);
extern UINT32 Image$$AP_MAIN_STACK$$ZI$$Limit;


__attribute__((used,section("APMainStack"))) __align(4) uint32 MainStack[512];
__attribute__((used,section("IdleStack"))) __align(4) uint32 IdleStack[512];
/*
--------------------------------------------------------------------------------
  Function name :
  Author        : ZHengYongzhi
  Description   : CortexM3 interrupt vector table

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
_ATTR_VECTTAB_AP_
ExecFunPtr exceptions_table[NUM_INTERRUPTS] =
{

    /* Core Fixed interrupts start here...*/

    (ExecFunPtr)(&Image$$AP_MAIN_STACK$$ZI$$Limit),
    (ExecFunPtr)PowerOn_Reset,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler,

    //RKNano Process Intterupt
    IntDefaultHandler,                                          //00 int_dma
    IntDefaultHandler,                                          //01 uart
    IntDefaultHandler,                                          //02 sd mmc
    IntDefaultHandler,                                          //03 pwm1
    IntDefaultHandler,                                          //04 pwm2
    IntDefaultHandler,                                          //05 imdct36
    IntDefaultHandler,                                          //06 synthesize
    IntDefaultHandler,                                          //07 usb
    IntDefaultHandler,                                          //08 i2c
    IntDefaultHandler,                                          //09 i2s
    IntDefaultHandler,                                          //10 gpio
    IntDefaultHandler,                                          //11 spi
    IntDefaultHandler,                                          //12 pwm0
    IntDefaultHandler,                                          //13 timer
    IntDefaultHandler,                                          //14 sar-adc
    IntDefaultHandler,                                          //15 reserved
    IntDefaultHandler,                                          //16 reserved
    IntDefaultHandler,                                          //17 reserved
    IntDefaultHandler,                                          //18 rtc
    IntDefaultHandler,                                          //19 reserved
    IntDefaultHandler,                                          //20 ool_bat_snsen
    IntDefaultHandler,                                          //21 reserved
    IntDefaultHandler,                                          //22 ool_pause
    IntDefaultHandler,                                          //23 ool_PlayOn
    IntDefaultHandler,                                          //24 pwr_5v_ready
    IntDefaultHandler,                                          //25 pwr_uvlo_vdd
    IntDefaultHandler,                                          //26 pwr_uvp
    IntDefaultHandler,                                          //27 pwr_ovp
    IntDefaultHandler,                                          //28 pwr_ot
    IntDefaultHandler,                                          //29 pwr_oc
    IntDefaultHandler,                                          //30 pwr_charge
    IntDefaultHandler,                                          //31 reserved
    IntDefaultHandler,
    IntDefaultHandler,
    IntDefaultHandler

};
//Interrupts Total Nums
/*
--------------------------------------------------------------------------------
  Function name :  IntDefaultHandler(void)
  Author        :  ZHengYongzhi
  Description   :  default interrupt service program.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

__asm uint32 __MSR_XPSR(void)
{
    MRS     R0, XPSR
    BX      R14
}

__asm __irq void IntDefaultHandler(void)
{
    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'B'
    BL UART_SEND_BYTE
    MOV R0, #'A'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'E'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'R'
    BL UART_SEND_BYTE
    MOV R0, #'I'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R4, BASEPRI
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'I'
    BL UART_SEND_BYTE
    MOV R0, #'N'
    BL UART_SEND_BYTE
    MOV R0, #'T'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R4, IPSR
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'H'
    BL UART_SEND_BYTE
    MOV R0, #'F'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'R'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MOV R0, #0XED2C ;HFSR
    MOV R1, #0XE000
    LSL R1, #16
    ORR R0, R1
    LDR R4, [R0]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'B'
    BL UART_SEND_BYTE
    MOV R0, #'F'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'R'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MOV R0, #0XED29 ;HFSR
    MOV R1, #0XE000
    LSL R1, #16
    ORR R0, R1
    LDR R4, [R0]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'M'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'-'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'C'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R6, MSP
    LDR R4, [R6, #0x18]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'M'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'-'
    BL UART_SEND_BYTE
    MOV R0, #'L'
    BL UART_SEND_BYTE
    MOV R0, #'R'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R6, MSP
    LDR R4, [R6, #0x14]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'M'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R4, MSP
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'-'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'C'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R6, PSP
    LDR R4, [R6, #0x18]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'-'
    BL UART_SEND_BYTE
    MOV R0, #'L'
    BL UART_SEND_BYTE
    MOV R0, #'R'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R6, PSP
    LDR R4, [R6, #0x14]
    BL PRINT_REG

    MOV R0, #'\r'
    BL UART_SEND_BYTE
    MOV R0, #'\n'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #'S'
    BL UART_SEND_BYTE
    MOV R0, #'P'
    BL UART_SEND_BYTE
    MOV R0, #':'
    BL UART_SEND_BYTE

    MRS R4, PSP
    BL PRINT_REG

HERE
    B        HERE




PRINT_REG
    PUSH {LR}
    MOV R5, #0x08
NEXT1
    MOV R0, R4
    LSR R0, #28
    CMP R0, #0x09
    BLS LSA
    ADD R0, #0x37

    B HIA
LSA
    ADD R0, #0x30
HIA
    BL UART_SEND_BYTE
    LSL R4, #4
    SUB R5, #1
    CBZ R5, NEXT2
    B NEXT1
NEXT2
    POP {LR}
    BX LR


UART_SEND_BYTE
    LDR   R1, =0x400b0000 ;UART
    LDR   R2, [R1, #0x7C] ;USR
    MOV   R3, #0X02
    AND        R2, R3
    CBNZ   R2, UART_SEND_BYTE_END
    B UART_SEND_BYTE
UART_SEND_BYTE_END
    STR  R0, [R1]  ;THR
    BX LR

}



/*
--------------------------------------------------------------------------------
  Function name :  IntMasterEnable(void)
  Author        :  ZHengYongzhi
  Description   :  Enable processor interrupts.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntMasterEnable(void)
{
    __SETPRIMASK();
}

/*
--------------------------------------------------------------------------------
  Function name :  IntMasterEnable(void)
  Author        :  ZHengYongzhi
  Description   :  Disable processor interrupts.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntMasterDisable(void)
{
    __RESETPRIMASK();
}

/*
--------------------------------------------------------------------------------
  Function name :  IntMasterEnable(void)
  Author        :  ZHengYongzhi
  Description   :  Enable processor interrupts.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void FaultMasterEnable(void)
{
    __RESETFAULTMASK();
}

/*
--------------------------------------------------------------------------------
  Function name :  IntMasterEnable(void)
  Author        :  ZHengYongzhi
  Description   :  Disable processor interrupts.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void FaultMasterDisable(void)
{
    __SETFAULTMASK();
}


/*
--------------------------------------------------------------------------------
  Function name : IntRegister(UINT32 ulInterrupt, void (*pfnHandler)(void))
  Author        : ZHengYongzhi
  Description   : interrupts register

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntRegister(UINT32 ulInterrupt, ExecFunPtr(*pfnHandler)(void))
{
    UINT32 ulIdx;

    // Check the arguments.
//    ASSERT(ulInterrupt < NUM_INTERRUPTS);
//
//    // Make sure that the RAM vector table is correctly aligned.
//    ASSERT(((UINT32)exceptions_table & 0x000003ff) == 0);

    // See if the RAM vector table has been initialized.
    if (nvic->VectorTableOffset != (UINT32)exceptions_table)
    {
        // Initiale the RAM vector table.
        exceptions_table[0] = (ExecFunPtr)&Image$$AP_MAIN_STACK$$ZI$$Limit;
        exceptions_table[1] = (ExecFunPtr)Main;
        for (ulIdx = 2; ulIdx < NUM_INTERRUPTS; ulIdx++)
        {
            exceptions_table[ulIdx] = (ExecFunPtr)IntDefaultHandler;
        }

        // Point NVIC at the RAM vector table.
        nvic->VectorTableOffset = (UINT32)exceptions_table;
    }


    // Save the interrupt handler.
    exceptions_table[ulInterrupt] = (ExecFunPtr)pfnHandler;
}

/*
--------------------------------------------------------------------------------
  Function name : IntUnregister(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : interrupts Unregister

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntUnregister(UINT32 ulInterrupt)
{
    //
    // Check the arguments.
    //
//    ASSERT(ulInterrupt < NUM_INTERRUPTS);

    //
    // Reset the interrupt handler.
    //
    exceptions_table[ulInterrupt] = (ExecFunPtr)IntDefaultHandler;
}


/*
--------------------------------------------------------------------------------
  Function name : void IntPriorityGroupingSet(UINT32 ulBits)
  Author        : ZHengYongzhi
  Description   : priority is grouping setting
                  This function specifies the split between preemptable priority
                  levels and subpriority levels in the interrupt priority speci-
                  fication.  The range of the grouping values are dependent upon
                  the hardware implementation.

  Input         : ulBits specifies the number of bits of preemptable priority.
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntPriorityGroupingSet(UINT32 ulBits)
{
    //
    // Set the priority grouping.
    //
    nvic->APIntRst = NVIC_APINTRST_VECTKEY | ((7 - ulBits) << 8);
}

/*
--------------------------------------------------------------------------------
  Function name : void IntPriorityGroupingSet(UINT32 ulBits)
  Author        : ZHengYongzhi
  Description   : get priority grouping.
                  This function returns the split between preemptable priority
                  levels and subpriority levels in the interrupt priority spe-
                  cification.
  Input         :
  Return        : The number of bits of preemptable priority.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

UINT32 IntPriorityGroupingGet(void)
{
    UINT32 ulValue;

    //
    // Read the priority grouping.
    //
    ulValue = nvic->APIntRst & NVIC_APINTRST_PRIGROUP_MASK;

    //
    // Return the number of priority bits.
    //
    return(7 - (ulValue >> 8));
}

/*
--------------------------------------------------------------------------------
  Function name : void IntPrioritySet(UINT32 ulInterrupt, unsigned char ucPriority)
  Author        : ZHengYongzhi
  Description   : Sets the priority of an interrupt.
                  This function is used to set the priority of an interrupt.
  Input         : ulInterrupt -- INT_ID;   ucPriority -- Priority Num.
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntPrioritySet(UINT32 ulInterrupt, UINT32 ucPriority)
{
    UINT32 ulBitOffset;
    UINT32 *pRegister;

    //
    // Set the interrupt priority.
    //
    ulBitOffset   = (ulInterrupt & 0x03) * 8;

    if (ulInterrupt < 16 )
    {
        pRegister = (UINT32*)((uint32)nvic->SystemPriority - 4);
    }
    else
    {
        pRegister = (UINT32 *)nvic->Irq.Priority;
    }
    pRegister = pRegister + (ulInterrupt >> 2);

    *pRegister = *pRegister & (~(0xFF << ulBitOffset)) | (ucPriority << ulBitOffset);
}

/*
--------------------------------------------------------------------------------
  Function name :  UINT32 IntPriorityGet(UINT32 ulInterrupt)
  Author        :  ZHengYongzhi
  Description   :  Gets the priority of an interrupt.
                   This function gets the priority of an interrupt.
  Input         :  ulInterrupt -- INT_ID
  Return        :  Returns the interrupt priority.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


UINT32 IntPriorityGet(UINT32 ulInterrupt)
{
    UINT32 ulBitOffset;
    UINT32 *pRegister;

    //
    // Return the interrupt priority.
    //
    ulBitOffset   = (ulInterrupt & 0x03) * 8;

    if (ulInterrupt < 16 )
    {
        pRegister = (UINT32 *)(nvic->SystemPriority - 4);
    }
    else
    {
        pRegister = (UINT32 *)nvic->Irq.Priority;
    }
    pRegister = pRegister + (ulInterrupt & 0x03);

    return((*pRegister >> ulBitOffset) & 0xFF);
}

/*
--------------------------------------------------------------------------------
  Function name : void IntEnable(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : Enables an interrupt.
                  The specified interrupt is enabled in the interrupt controller.
  Input         : ulInterrupt -- INT_ID
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntEnable(UINT32 ulInterrupt)
{
    //
    // Determine the interrupt to enable.
    //
    if (ulInterrupt == FAULT_ID4_MPU)
    {
        //
        // Enable the MemManage interrupt.
        //
        nvic->SystemHandlerCtrlAndState |= NVIC_SYSHANDCTRL_MEMFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID5_BUS)
    {
        //
        // Enable the bus fault interrupt.
        //
        nvic->SystemHandlerCtrlAndState |= NVIC_SYSHANDCTRL_BUSFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID6_USAGE)
    {
        //
        // Enable the usage fault interrupt.
        //
        nvic->SystemHandlerCtrlAndState |= NVIC_SYSHANDCTRL_USGFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID15_SYSTICK)
    {
        //
        // Enable the System Tick interrupt.
        //
        nvic->SysTick.Ctrl |= NVIC_SYSTICKCTRL_TICKINT;
    }
    else if (ulInterrupt >= 16)
    {
        //
        // Enable the general interrupt.
        //
        nvic->Irq.Enable[(ulInterrupt - 16) / 32] = 1 << (ulInterrupt - 16) % 32;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void IntDisable(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : Disables an interrupt.
                  The specified interrupt is Disabled in the interrupt controller.
  Input         : ulInterrupt -- INT_ID
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntDisable(UINT32 ulInterrupt)
{
    //
    // Determine the interrupt to Disable.
    //
    if (ulInterrupt == FAULT_ID4_MPU)
    {
        //
        // Disable the MemManage interrupt.
        //
        nvic->SystemHandlerCtrlAndState &= ~NVIC_SYSHANDCTRL_MEMFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID5_BUS)
    {
        //
        // Disable the bus fault interrupt.
        //
        nvic->SystemHandlerCtrlAndState &= ~NVIC_SYSHANDCTRL_BUSFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID6_USAGE)
    {
        //
        // Disable the usage fault interrupt.
        //
        nvic->SystemHandlerCtrlAndState &= ~NVIC_SYSHANDCTRL_USGFAULTENA;
    }
    else if (ulInterrupt == FAULT_ID15_SYSTICK)
    {
        //
        // Disable the System Tick interrupt.
        //
        nvic->SysTick.Ctrl &= ~NVIC_SYSTICKCTRL_TICKINT;
    }
    else if (ulInterrupt >= 16)
    {
        //
        // Disable the general interrupt.
        //
        nvic->Irq.Disable[(ulInterrupt - 16) / 32] = 1 << (ulInterrupt - 16) % 32;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void IntPendingSet(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : Set pending bit of the interrupt.

  Input         :
  Return        : Pending IRQ Channel Identifier.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

void IntPendingSet(UINT32 ulInterrupt)
{
    //
    // Determine the interrupt pend to Set.
    //
    if (ulInterrupt == FAULT_ID2_NMI)
    {
        //
        // Set the NMI interrupt pend.
        //
        nvic->INTcontrolState |= NVIC_INTCTRLSTA_NMIPENDSET;
    }
    else if (ulInterrupt == FAULT_ID14_PENDSV)
    {
        //
        // Set the PendSV interrupt pend.
        //
        nvic->INTcontrolState |= NVIC_INTCTRLSTA_PENDSVSET;
    }
    else if (ulInterrupt == FAULT_ID15_SYSTICK)
    {
        //
        // Set the System Tick interrupt pend.
        //
        nvic->INTcontrolState |= NVIC_INTCTRLSTA_PENDSTSET;
    }
    else if (ulInterrupt >= 16)
    {
        //
        // Set the general interrupt pend.
        //
        nvic->Irq.SetPend[(ulInterrupt - 16) / 32] = 1 << (ulInterrupt - 16) % 32;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : void IntPendClear(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : Clear pending bit of the irq.

  Input         :
  Return        : Pending IRQ Channel Identifier.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


void IntPendingClear(UINT32 ulInterrupt)
{
    //
    // Determine the interrupt pend to Set.
    //
    if (ulInterrupt == FAULT_ID14_PENDSV)
    {
        //
        // Set the PendSV interrupt pend.
        //
        nvic->INTcontrolState |= NVIC_INTCTRLSTA_PENDSVCLR;
    }
    else if (ulInterrupt == FAULT_ID15_SYSTICK)
    {
        //
        // Set the System Tick interrupt pend.
        //
        nvic->INTcontrolState |= NVIC_INTCTRLSTA_PENDSTCLR;
    }
    else if (ulInterrupt >= 16)
    {
        //
        // Set the general interrupt pend.
        //
        nvic->Irq.ClearPend[(ulInterrupt - 16) / 32] = 1 << (ulInterrupt - 16) % 32;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : BOOL IntISRPendingCheck(void)
  Author        : ZHengYongzhi
  Description   : Check interrupt pending or not.

  Input         :
  Return        : pending or not.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


BOOL IntISRPendingCheck(void)
{
    //
    // Check the arguments.
    //
    return ((BOOL)(nvic->INTcontrolState & NVIC_INTCTRLSTA_ISRPENDING));
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 IntVectPendingGet(void)
  Author        : ZHengYongzhi
  Description   : Get interrupt number of the highest priority pending ISR.

  Input         :
  Return        : Return interrupt number of the highest priority pending ISR.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

UINT32 IntVectPendingGet(void)
{
    //
    // Check the arguments.
    //
    return ((nvic->INTcontrolState & NVIC_INTCTRLSTA_ISRPENDING) >> 12);
}

/*
--------------------------------------------------------------------------------
  Function name : UINT32 IntVectActiveGet(void)
  Author        : ZHengYongzhi
  Description   : Get interrupt number of the currently running ISR.

  Input         :
  Return        : Return interrupt number of the currently running ISR.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


UINT32 IntVectActiveGet(void)
{
    //
    // Check the arguments.
    //
    return (nvic->INTcontrolState & NVIC_INTCTRLSTA_VECTACTIVE_MASK);
}

/*
--------------------------------------------------------------------------------
  Function name : uint32 IsrDisable(UINT32 ulInterrupt)
  Author        : ZHengYongzhi
  Description   : close interrupt temporarily,go to use with IsrEnable.

  Input         :
  Return        : current interrupt status.

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


uint32 IsrDisable(UINT32 ulInterrupt)
{
    uint32 IsEnable;

    //
    // Determine the interrupt to enable.
    //
    if (ulInterrupt == FAULT_ID4_MPU)
    {
        //
        // Is the MemManage interrupt Enabled.
        //
        IsEnable = (nvic->SystemHandlerCtrlAndState & NVIC_SYSHANDCTRL_MEMFAULTENA);
    }
    else if (ulInterrupt == FAULT_ID5_BUS)
    {
        //
        // Is the bus fault interrupt Enabled.
        //
        IsEnable = (nvic->SystemHandlerCtrlAndState & NVIC_SYSHANDCTRL_BUSFAULTENA);
    }
    else if (ulInterrupt == FAULT_ID6_USAGE)
    {
        //
        // Is the usage fault interrupt Enabled.
        //
        IsEnable = (nvic->SystemHandlerCtrlAndState & NVIC_SYSHANDCTRL_USGFAULTENA);
    }
    else if (ulInterrupt == FAULT_ID15_SYSTICK)
    {
        //
        // Is the System Tick interrupt Enabled.
        //
        IsEnable = (nvic->SysTick.Ctrl & NVIC_SYSTICKCTRL_TICKINT);
    }
    else if (ulInterrupt >= 16)
    {
        //
        // Is the general interrupt Enabled.
        //
        IsEnable = (nvic->Irq.Enable[(ulInterrupt - 16) / 32] & (1 << (ulInterrupt - 16) % 32));
    }

    IntDisable(ulInterrupt);

    return(IsEnable);

}

/*
--------------------------------------------------------------------------------
  Function name : void IsrEnable(UINT32 ulInterrupt, uint32 IsEnable)
  Author        : ZHengYongzhi
  Description   : open interrupt and it act in concert with IsrDisable.

  Input         :
  Return        :

  History:     <author>         <time>         <version>
             ZhengYongzhi     2008/07/21         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/


void IsrEnable(UINT32 ulInterrupt, uint32 IsEnable)
{
    if (IsEnable)
    {
        IntEnable(ulInterrupt);
    }
}


/*
--------------------------------------------------------------------------------
  Function name : void UserIsrEnable(uint32 data)
  Author        : ZhengYongzhi
  Description   : PendSV作为软中断，不能进行Disable和Enable，使用信号量MSG_USER_ISR_ENABLE
                  进行PendSV的使能标志，和UserIsrDisable配合使用，打开时需要判断FAULT_ID14_PENDSV
                  的状态。使用这两个函数之前必须对信号量MSG_USER_ISR_ENABLE和MSG_USER_ISR_STATUES
                  进行初始化
  Input         :
  Return        : 无

  History:     <author>         <time>         <version>
             ZhengYongzhi      2008-1-15          Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/

uint32 IrqPriorityTab[NUM_INTERRUPTS] =
{
    //  group        SubPriority                                 //Cortex M3 System Intterupt
    -3,                                 //SP Point                FAULT_ID0_REV,
    -3,                                 //Reset                   FAULT_ID1_REV,
    -2,                                 //NMI fault               FAULT_ID2_NMI,
    -1,                                 //Hard fault              FAULT_ID3_HARD,
    ((0x00 << 6) | (0x00 << 3)),        //MPU fault               FAULT_ID4_MPU,
    ((0x00 << 6) | (0x00 << 3)),        //Bus fault               FAULT_ID5_BUS,
    ((0x00 << 6) | (0x00 << 3)),        //Usage fault             FAULT_ID6_USAGE,
    ((0xFF << 6) | (0xFF << 3)),        //                        FAULT_ID7_REV,
    ((0xFF << 6) | (0xFF << 3)),        //                        FAULT_ID8_REV,
    ((0xFF << 6) | (0xFF << 3)),        //                        FAULT_ID9_REV,
    ((0xFF << 6) | (0xFF << 3)),        //                        FAULT_ID10_REV,
    ((0xFF << 6) | (0xFF << 3)),        //SVCall                  FAULT_ID11_SVCALL,
    ((0xFF << 6) | (0xFF << 3)),        //Debug monitor           FAULT_ID12_DEBUG,
    ((0xFF << 6) | (0xFF << 3)),        //                        FAULT_ID13_REV,
    ((0x03 << 6) | (0x00 << 3)),        //PendSV                  FAULT_ID14_PENDSV,
    ((0x02 << 6) | (0x01 << 3)),        //System Tick             FAULT_ID15_SYSTICK,

    //RKNano Process Int
    ((0x00 << 6) | (0x01 << 3)),        //00 sfc                  INT_ID16_SFC,
    ((0x02 << 6) | (0x00 << 3)),        //01 synth                INT_ID17_SYNTH,
    ((0x02 << 6) | (0x07 << 3)),        //02 ebc                  INT_ID18_EBC,
    ((0x02 << 6) | (0x00 << 3)),        //03 emmc                 INT_ID19_EMMC,
    ((0x02 << 6) | (0x00 << 3)),        //04 sdmmc                INT_ID20_SDMMC,
    ((0x02 << 6) | (0x00 << 3)),        //05 usbc                 INT_ID21_USBC,
    ((0x02 << 6) | (0x00 << 3)),        //06 dma                  INT_ID22_DMA,
    ((0x01 << 6) | (0x01 << 3)),        //07 imdct                INT_ID23_IMDCT,
    ((0x02 << 6) | (0x02 << 3)),        //08 wdt                  INT_ID24_WDT,
    ((0x02 << 6) | (0x03 << 3)),        //09 mailbox              INT_ID25_MAILBOX0,
    ((0x02 << 6) | (0x03 << 3)),        //10 mailbox              INT_ID26_MAILBOX1,
    ((0x02 << 6) | (0x03 << 3)),        //12 mailbox              INT_ID27_MAILBOX2,
    ((0x02 << 6) | (0x03 << 3)),        //13 mailbox              INT_ID28_MAILBOX3,
    ((0x02 << 6) | (0x04 << 3)),        //14 pwm4                 INT_ID29_PWM4,
    ((0x02 << 6) | (0x05 << 3)),        //15 pwm3                 INT_ID30_PWM3,
    ((0x02 << 6) | (0x06 << 3)),        //16 pwm2                 INT_ID31_PWM2,
    ((0x02 << 6) | (0x07 << 3)),        //17 pwm1                 INT_ID32_PWM1,
    ((0x01 << 6) | (0x02 << 3)),        //18 pwm0                 INT_ID33_PWM0,
    ((0xFF << 6) | (0xFF << 3)),        //19 timer                INT_ID34_TIMER1,
    ((0xFF << 6) | (0xFF << 3)),        //20 timer0               INT_ID35_TIMER0,
    ((0xFF << 6) | (0xFF << 3)),        //21 sradc                INT_ID36_SRADC,
    ((0x02 << 6) | (0x07 << 3)),        //22 uart5                INT_ID37_UART5,
    ((0xFF << 6) | (0xFF << 3)),        //23 uart4                INT_ID38_UART4,
    ((0x02 << 6) | (0x07 << 3)),        //24 uart3                INT_ID39_UART3,
    ((0xFF << 6) | (0xFF << 3)),        //25 uart2                INT_ID40_UART2,
    ((0x02 << 6) | (0x07 << 3)),        //26 uart1                INT_ID41_UART1,
    ((0x02 << 6) | (0x07 << 3)),        //27 uart0                INT_ID42_UART0,
    ((0x02 << 6) | (0x07 << 3)),        //28 sp1                  INT_ID43_SP1,
    ((0x02 << 6) | (0x07 << 3)),        //29 sp0                  INT_ID44_SP0,
    ((0x02 << 6) | (0x07 << 3)),        //30 i2c2                 INT_ID45_I2C2,
    ((0x02 << 6) | (0x07 << 3)),        //31 i2c1                 INT_ID46_I2C1,
    ((0x02 << 6) | (0x07 << 3)),        //32 i2c0                 INT_ID47_I2C0,
    ((0x02 << 6) | (0x07 << 3)),        //33 i2s1                 INT_ID48_I2S1,
    ((0x02 << 6) | (0x07 << 3)),        //34 i2s0                 INT_ID49_I2S0,
    ((0xFF << 6) | (0xFF << 3)),        //35 hifi                 INT_ID50_HIFI,
    ((0xFF << 6) | (0xFF << 3)),        //36 pmu                  INT_ID51_PMU,
    ((0xFF << 6) | (0xFF << 3)),        //37 gpio1                INT_ID52_GPIO1,
    ((0xFF << 6) | (0xFF << 3)),        //38 gpio0                INT_ID53_GPIO0,
};

void IrqPriorityInit(void)              //中断优先级初始化, 在IntPriorityGroupingSet执行后调用
{
    uint32 i;

    IntPriorityGroupingSet(2);
    for (i = 4; i < NUM_INTERRUPTS; i++)
    {
        IntPrioritySet(i, IrqPriorityTab[i]);
    }
}

/*
********************************************************************************
*
*                         End of interrupt.c
*
********************************************************************************
*/


