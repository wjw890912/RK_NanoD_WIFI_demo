/*
********************************************************************************
*                   Copyright (c) 2008,anzhiguo
*                         All rights reserved.
*
* File Name£º   hw_timer.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             anzhiguo      2009-1-14          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#undef  EXT
#ifdef  _IN_TIMER_
#define EXT
#else
#define EXT extern
#endif

/*
--------------------------------------------------------------------------------

            external memory control memap register define

--------------------------------------------------------------------------------
*/
typedef volatile struct tagTIMER_REG{

    uint32 Timer1LoadCount;     // Load Count Register
    uint32 Timer1CurrentValue;  // Current Value Register
    uint32 Timer1ControlReg;    // Control Register
    uint32 Timer1EOI;           // End-of-Interrupt Register
    uint32 Timer1IntStatus;     // Interrupt Status Register
    uint32 Reserved[(0xa0 - 0x14)/4];
    uint32 TimersIntStatus;     // Interrupt Status Register
    uint32 TimersEOI;           // End-of-Interrupt Register
    uint32 TimersRawIntStatus;  // Raw Interrupt Status Register

} TIMER_REG,*pTIMER_REG;

typedef enum _TIMER_NUM
{
    TIMER0 = 0,
    TIMER1,
    TIMER_MAX
}eTIMER_NUM;
/*
*-------------------------------------------------------------------------------
*
*  The following define the bit fields in the TimerControlReg .
*
*-------------------------------------------------------------------------------
*/
#define TIMR_EN                ((UINT32)1<<0) //timer enable
#define TIMR_USER_MODE         ((UINT32)1<<1)  // 0:free-running, 1:usermode
#define TIMR_IMASK             ((UINT32)1<<2) //timer interrupt mask

/*
********************************************************************************
*
*                         End of hw_timer.h
*
********************************************************************************
*/
#endif
