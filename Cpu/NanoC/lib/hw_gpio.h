/*
********************************************************************************
*                   Copyright (c) 2008,anzhiguo
*                         All rights reserved.
*
* File Name£º  Hw_Gpio.h
*
* Description:
*
* History:      <author>          <time>        <version>
*             anzhiguo      2009-1-14          1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_GPIO_H_
#define _HW_GPIO_H_

#undef  EXT
#ifdef  _IN_GPIO_
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

    UINT32 OutData;
    UINT32 Dir;
    UINT32 Source;

}GPIO_PORT,*gGPIO_PORT;

typedef volatile struct {

    #if 0
    UINT32 PortAOutData;
    UINT32 PortADir;
    UINT32 PortASource;

    UINT32 PortBOutData;
    UINT32 PortBDir;
    UINT32 PortBSource;

    UINT32 PortCOutData;
    UINT32 PortCDir;
    UINT32 PortCSource;

    UINT32 PortDOutData;
    UINT32 PortDDir;
    UINT32 PortDSource;
    #endif
    GPIO_PORT Port[4];

    UINT32 PortAIntEnable;
    UINT32 PortAIntMask;
    UINT32 PortAIntType;    //edge or level trigger.
    UINT32 PortAIntPolarity;//high level is valid or low level valid.
    UINT32 PortAIntStatus;
    UINT32 PortAIntRawStatus;
    UINT32 PortAIntDebounce;
    UINT32 PortAIntClear;

    #if 0
    UINT32 PortAInData;
    UINT32 PortBInData;
    UINT32 PortCInData;
    UINT32 PortDInData;
    #endif

    UINT32 InData[4];

    UINT32 IntLevelSync;

} GPIO_t,*gGPIO_t;

#define Gpio    ((GPIO_t *) GPIO0_BASE)

/*
********************************************************************************
*
*                         End of hw_gpio.h
*
********************************************************************************
*/
#endif
