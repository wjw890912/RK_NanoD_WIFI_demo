/*
********************************************************************************
*                   Copyright (c) 2008,ZhengYongzhi
*                         All rights reserved.
*
* File Name£º   systick.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             ZhengYongzhi      2008-8-13          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#undef  EXT
#ifdef _IN_SYSTICK_
#define EXT
#else
#define EXT extern
#endif

/*
*-------------------------------------------------------------------------------
*
*                            Struct Define
*
*-------------------------------------------------------------------------------
*/

/*
*-------------------------------------------------------------------------------
*
*                            Function Declaration
*
*-------------------------------------------------------------------------------
*/


extern void   SysTickClkSourceSet(UINT32 Source);
extern BOOL   SysTickClkSourceGet(void);
extern void   SysTickEnable(void);
extern void   SysTickDisable(void);
extern UINT32 SysTickPeriodGet(void);
extern UINT32 SysTickValueGet(void);
extern BOOL   SysTickRefClockCheck(void);
extern BOOL   SysTickCalibrationCheck(void);
extern UINT32 SysTickCalibrationTenMsGet(void);
extern void SysTickPeriodSet(UINT32 mstick );

/*
********************************************************************************
*
*                         End of systick.h
*
********************************************************************************
*/
#endif

