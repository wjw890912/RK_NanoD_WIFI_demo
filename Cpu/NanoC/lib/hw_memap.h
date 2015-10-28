/*
********************************************************************************
*                   Copyright (c) 2008,Huweiguo
*                         All rights reserved.
*
* File Name£º   Hw_memap.h
*
* Description:
*
*
* History:      <author>          <time>        <version>
*               HuWeiGuo        2009-01-05         1.0
*    desc:    ORG.
********************************************************************************
*/

#ifndef _HW_MEMAP_H_
#define _HW_MEMAP_H_

#define CRU_BASE            ((UINT32)0x40000000)
#define GRF_BASE            ((UINT32)0x40010000)
#define I2S0_BASE            ((UINT32)0x40020000)
#define I2C0_BASE            ((UINT32)0x40030000)
#define SPI0_BASE            ((UINT32)0x40040000)
#define UART0_BASE           ((UINT32)0x40050000)
#define ADC_BASE            ((UINT32)0x40060000)
#define TIMER_BASE          ((UINT32)0x40070000)
#define PWM0_BASE            ((UINT32)0x40080000)
#define EFUSE_BASE          ((UINT32)0x40090000)
#define GPIO0_BASE           ((UINT32)0x400a0000)
#define RTC_BASE            ((UINT32)0x400b0000)
#define DMA_BASE            ((UINT32)0x60000000)
#define IMDCT_BASE          ((UINT32)0x60010000)
#define SYNTH_BASE          ((UINT32)0x60020000)
#define USBOTG20_BASE       ((UINT32)0x60030000)
#define SDMMC_BASE          ((UINT32)0x60070000)
#define NANDC_BASE          ((UINT32)0x60080000)
#define LCD_BASE            ((UINT32)0x60090000)

#define ADC_BASE            ((UINT32)0x40060000)
#define EFUSE_BASE          ((UINT32)0x40090000)
#define RTC_BASE            ((UINT32)0x400b0000)
#define NANDC_BASE          ((UINT32)0x60080000)


//extend
#define MAILBOX_BASE        ((UINT32)0x40110000)

/*
********************************************************************************
*
*                         End of Hw_memap.h
*
********************************************************************************
*/

#endif
