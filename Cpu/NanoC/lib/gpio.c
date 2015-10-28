/*
********************************************************************************
*                   Copyright (c) 2008, Rock-Chips
*                         All rights reserved.
*
* File Name£º   GPIO.c
*
* Description:  C program template
*
* History:      <author>          <time>        <version>
*             anzhiguo      2008-11-20         1.0
*    desc:    ORG.
********************************************************************************
*/
#define _IN_GPIO_
#include <typedef.h>
#include <DriverInclude.h>

/*
--------------------------------------------------------------------------------
  Function name : GpioMuxSet()
  Author        : anzhiguo
  Description   : set GPIO work mode.

  Input         : null

  Return        : null

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void GpioMuxSet(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum, eGPIOPinIOMux_t data)
{
    UINT8  Group;
    UINT8  pin;

    Group = GPIOPinNum / 8 ;
    pin   = GPIOPinNum % 8;

    Grf->GPIO_IOMUX[Group] = ((0x00030000ul + data) << (pin << 0x01ul));
}

/*
--------------------------------------------------------------------------------
  Function name : GpioMuxGet()
  Author        : anzhiguo
  Description   : get gpio current work mode

  Input         : GPIOPinNum -- GPIO

  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 GpioMuxGet(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    UINT8  Group;
    UINT8  pin;

    Group  = GPIOPinNum / 8 ;
    pin    = GPIOPinNum % 8;

    return (((Grf->GPIO_IOMUX[Group]) >> (pin << 0x01ul)) & 0x03);
}

/*
--------------------------------------------------------------------------------
  Function name : void DataPortMuxSet(eDataPortIOMux_t data)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
void DataPortMuxSet(eDataPortIOMux_t data)
{
    Grf->IOMUX_CON0 = (0xffff0000ul + data);
}

/*
--------------------------------------------------------------------------------
  Function name : int32 DataPortMuxGet(void)
  Author        : anzhiguo
  Description   :

  Input         :

  Return        : null

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:         ORG
--------------------------------------------------------------------------------
*/
int32 DataPortMuxGet(void)
{
    return (Grf->IOMUX_CON0 & 0x0000ffff);
}

//#ifdef DRIVERLIB_IRAM
/*
--------------------------------------------------------------------------------
  Function name : GPIO_SetPinDirection(eGPIOPinNum_t GPIOPinNum, eGPIOPinDirection_t direction)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum --
                : direction --
  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
void Gpio_SetPinDirection(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum, eGPIOPinDirection_t direction)
{
    UINT8  Group;
    UINT8  pin;

    Group  = GPIOPinNum / 32 ;
    pin    = GPIOPinNum % 32;

    if (direction)
    {
        Gpio->Port[Group].Dir |= (1 << pin);
    }
    else
    {
        Gpio->Port[Group].Dir &= ~(1 << pin);
    }
}
/*
--------------------------------------------------------------------------------
  Function name : GPIO_GetPinDirection(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin number

  Return        : not 0 -- out direction
                  0   -- in direction
  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
UINT8 Gpio_GetPinDirection(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    UINT8  Group;
    UINT8  pin;

    Group = GPIOPinNum / 32 ;
    pin   = GPIOPinNum % 32;

    return (((Gpio->Port[Group].Dir) & (0x1 << pin)) >> pin);
}

/*
--------------------------------------------------------------------------------
  Function name : GPIO_SetPinLevel(eGPIOPinNum_t GPIOPinNum, eGPIOPinLevel_t level)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin number
                  level -- 1 or 0(high or low)
  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:      GPIOPinNum define the enum by need in future.
--------------------------------------------------------------------------------
*/
void Gpio_SetPinLevel(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum, eGPIOPinLevel_t level)
{
    UINT8  Group;
    UINT8  pin;
    UINT32 GPIOPortData;

    Group = GPIOPinNum / 32 ;
    pin   = GPIOPinNum % 32;

    if (level)
    {
        Gpio->Port[Group].OutData |= (1 << pin);
    }
    else
    {
        Gpio->Port[Group].OutData &= ~(1 << pin);
    }

}
/*
--------------------------------------------------------------------------------
  Function name : GPIO_GetPinLevel(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin number

  Return        : no 0 is high, 0 is low

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
UINT8 Gpio_GetPinLevel(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    UINT8  Group;
    UINT8  pin;

    Group = GPIOPinNum / 32 ;
    pin   = GPIOPinNum % 32;

    return (((Gpio->InData[Group]) & (0x1 << pin)) >> pin);
}


/*
--------------------------------------------------------------------------------
  Function name : GPIO_SetIntMode(eGPIOPinNum_t GPIOPinNum, GPIOIntrType_c type)
  Author        : anzhiguo
  Description   : set GPIO pin interrupt type

  Input         : GPIOPinNum -- GPIO pin
                : type --
  Return        : null

  note          :recommend use edge type.
  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
void Gpio_SetIntMode(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum, GPIOIntrType_c type)
{
    switch (type)
    {
        case IntrTypeRisingEdge://rise edge

            Gpio->PortAIntType     |= (1 << GPIOPinNum);//edge trigger
            Gpio->PortAIntPolarity |= (1 << GPIOPinNum);//level trigger
            break;

        case IntrTypeFallingEdge://down edge

            Gpio->PortAIntType     |=  (1 << GPIOPinNum);//edge trigger
            Gpio->PortAIntPolarity &= ~(1 << GPIOPinNum);//low level valid
            break;

        case IntrTypeHighLevel://high voltage

            Gpio->PortAIntType     &= ~(1 << GPIOPinNum);//level trigger
            Gpio->PortAIntPolarity |=  (1 << GPIOPinNum);//high leve valid
            break;

        case IntrTypeLowLevel://low level

            Gpio->PortAIntType     &= ~(1 << GPIOPinNum);//level trigger
            Gpio->PortAIntPolarity &= ~(1 << GPIOPinNum);//low level valid
            break;

        default:

            break;
    }
}

/*
--------------------------------------------------------------------------------
  Function name : GPIO_GetIntMode(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   : get interrupt type:edge or level

  Input         : GPIOPinNum -- GPIO pin
                :
  Return        : 0:level

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
UINT32 Gpio_GetIntMode(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    return ((Gpio->PortAIntType) & (0x1 << GPIOPinNum));
}

/*
--------------------------------------------------------------------------------
  Function name : GPIO_ClearInt_Level(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   : mask interrupt.

  Input         : GPIOPinNum -- GPIO pin
                :
  Return        : null

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
  note          : after clear interrupt trigger type,must clear interrupt mask bit,if not,it will affect
                  next interrupt.
--------------------------------------------------------------------------------
*/
void Gpio_ClearInt_Level(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    Gpio->PortAIntMask |= (0x1 << GPIOPinNum);
}

/*
--------------------------------------------------------------------------------
  Function name : GPIO_EnableInt(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin

  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
void Gpio_EnableInt(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
      Gpio->PortAIntEnable |= (1 << GPIOPinNum);
}
/*
--------------------------------------------------------------------------------
  Function name : GPIO_DisableInt(eGPIOPinNum_t GPIOPinNum)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin

  Return        : null

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
  desc:        ORG
--------------------------------------------------------------------------------
*/
void Gpio_DisableInt(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum)
{
    Gpio->PortAIntEnable &= ~(1 << GPIOPinNum);
}
//#endif

/*
--------------------------------------------------------------------------------
  Function name : GPIO_SetPinPull(eGPIOPinNum_t GPIOPinNum, BOOL type)
  Author        : anzhiguo
  Description   :

  Input         : GPIOPinNum -- GPIO pin
                  type --  1 pull up
  Return        :

  History:     <author>         <time>         <version>
             anzhiguo     2009-1-14         Ver1.0
--------------------------------------------------------------------------------
*/
void GPIO_SetPinPull(eGPIO_CHANNEL gpioPort,eGPIOPinNum_t GPIOPinNum, eGPIOPinPull_t pull)
{
    UINT8  Group;
    UINT8  pin;

    Group = GPIOPinNum / 8 ;
    pin   = GPIOPinNum % 8;

    Grf->GPIO_PULL[Group] = ((0x00010000ul + pull) << pin);
}

/*
*/
pFunc GPIOIsrCallBack[GPIOPortD_Pin4]=
{
    //GPIO_A
    0,0,0,0,0,0,0,0,
    //GPIO_B
    0,0,0,0,0,0,0,0,
    //GPIO_C
    0,0,0,0,0,0,0,0,
    //GPIO_D
    0,0,0,0
};

/*
*/
int32 GpioIsrRegister(uint32 pin, pFunc CallBack)
{
    if (pin >= GPIOPortD_Pin4)
    {
        //rk_printf("Dma Channel error: ch = %d!\n", pin);
        return RK_ERROR;
    }

    GPIOIsrCallBack[pin] = (pFunc)CallBack;

    return RK_SUCCESS;
}

/*
*/
int32 GPIOIsrUnRegister(uint32 pin)
{
    if (pin >= GPIOPortD_Pin4)
    {
        //rk_printf("Dma Channel error: ch = %d!\n", pin);
        return RK_ERROR;
    }

    GPIOIsrCallBack[pin] = 0;

    return RK_SUCCESS;
}

/*
*/
void GpioInt(eGPIO_CHANNEL gpioPort)
{
    int ch = 0;
    uint32 IsEnable;
    uint32 GPIOPinBitTmp;
    pFunc GPIOCallBack;

    uint32 pin;
    uint32 rawStatus;
    pFunc  CallBack;

    //rk_printf("Gpio Isr1 \n");

    //IsEnable = IsrDisable(INT_ID17_GPIO);
    do
    {
        rawStatus = (uint32)(Gpio->PortAIntStatus) & 0x0fffffff;
        Gpio->PortAIntClear = rawStatus;

        pin = 0;
        do
        {
             if (rawStatus & (0x01ul << pin))
             {
                rawStatus &= ~(0x01ul << pin);
                CallBack = GPIOIsrCallBack[pin];
                if (CallBack)
                {
                    //GPIOIsrCallBack[pin] = NULL;
                    CallBack();
                }
             }
             pin++;

        }while(rawStatus & 0x0fffffff);

    }while((uint32)(Gpio->PortAIntStatus) & 0x0fffffff);

    //IsrEnable(INT_ID17_GPIO, IsEnable);

}

/*
********************************************************************************
*
*                         End of Gpio.c
*
********************************************************************************
*/

