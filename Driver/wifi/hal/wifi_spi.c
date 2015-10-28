#include "FreeRTOS.h"
#include "RKOS.h"
#include "wifi_spi.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "device.h"
//#include "spi_dma.h"
//#include "debug_print.h"
#ifdef __DRIVER_INNOPHACE_WIFI_C__
#pragma arm section code = "innowifiCode", rodata = "innowifiCode", rwdata = "innowifidata", zidata = "innowifidata"
extern int wifi_interrupt(void);

#define RKNANOD_SDK_MAIN_V12_20150515HJH 0
#define RKNANOD_SDK_MAIN_V10_20150126HJH 1
#define SPI_WIFI_BOARD_VERSIONS RKNANOD_SDK_MAIN_V12_20150515HJH
#if 1

static HDC g_hSpi;
#define WIFI_SPI_CLK GPIOPortC_Pin2
#define WIFI_SPI_IN  GPIOPortC_Pin1
#define WIFI_SPI_OUT GPIOPortC_Pin0
#define WIFI_SPI_CS  GPIOPortC_Pin5
#define WIFI_SPI_INT GPIOPortC_Pin7

#define SPI_CTL_TXRX_MASTER ( SPI_MASTER_MODE | TRANSMIT_RECEIVE | MOTOROLA_SPI \
                            | RXD_SAMPLE_NO_DELAY | APB_BYTE_WR \
                            | MSB_FBIT | LITTLE_ENDIAN_MODE | CS_2_SCLK_OUT_1_2_CK \
                            | CS_KEEP_LOW | SERIAL_CLOCK_POLARITY_HIGH\
                            | SERIAL_CLOCK_PHASE_MIDDLE | DATA_FRAME_8BIT)

#if SPI_WIFI_BOARD_VERSIONS == RKNANOD_SDK_MAIN_V10_20150126HJH
void wifi_spi_init(void)
{
#if 1
    SPI_DEV_ARG stSpiArg;
    HDC hSpi,hStorage;
    HDC hDma;
    rk_err_t ret;

//    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_CS, 0);
//    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_CS, GPIO_OUT);
//    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_HIGH);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_CS, ENABLE);
//
//    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_OUT, 0);
//    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_OUT, GPIO_OUT);
//    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_OUT, GPIO_HIGH);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_OUT, ENABLE);


//
//    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_CLK, 0);
//    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_CLK, GPIO_OUT);
//    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CLK, GPIO_HIGH);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_CLK, ENABLE);
    //rk_print_string("\r\ndma device init...");

//    ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

//    if (ret != RK_SUCCESS)
//    {
//        goto err;
//    }

//    hDma = RKDev_Open(DEV_CLASS_DMA, 0, NOT_CARE);
//    if (hDma <= 0)
//    {
//        return ;
//    }

    stSpiArg.Ch = 0;
    stSpiArg.CtrlMode = SPI_CTL_TXRX_MASTER;//SPI_CTL_MODE;
    stSpiArg.SpiRate = 2000*1000;


    //rk_print_string("\r\nspi device init...");

    ret = RKDev_Create(DEV_CLASS_SPI, 0, &stSpiArg);
    if (ret != RK_SUCCESS)
    {
        goto err;
    }

    //rk_print_string("\r\nspinor device init...");

    hSpi = RKDev_Open(DEV_CLASS_SPI, 0, NOT_CARE);
    g_hSpi = hSpi;
    if ((hSpi == NULL) || (hSpi == (HDC)RK_ERROR) || (hSpi == (HDC)RK_PARA_ERR))
    {
        goto err;
    }
err:

    Grf_GpioMuxSet(GPIO_CH0, GPIOPortC_Pin5, 0);
    Gpio_SetPinDirection(GPIO_CH0, GPIOPortC_Pin5, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH0, GPIOPortC_Pin5, GPIO_HIGH);
    Grf_GPIO_SetPinPull(GPIO_CH0, GPIOPortC_Pin5, DISABLE);




    return;
#endif

}

void wifi_spi_irq_hook(void)
{
//    printf("wifi_spi_irq\n");
    wifi_interrupt();
}

#define WIFI_SPI_INT GPIOPortC_Pin7

void exti_init(void)
{
    printf("exti_init enter\n");
    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_INT, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_INT, GPIO_IN);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_INT, ENABLE);
    Gpio_SetIntMode(GPIO_CH0,WIFI_SPI_INT, IntrTypeHighLevel);
    GpioIsrRegister(GPIO_CH0,WIFI_SPI_INT, wifi_spi_irq_hook);
    Gpio_UnMaskInt(GPIO_CH0,WIFI_SPI_INT);
    Gpio_EnableInt(GPIO_CH0,WIFI_SPI_INT);
    IntRegister(INT_ID_GPIO0,GpioInt0);
    IntPendingClear(INT_ID_GPIO0);
    IntEnable(INT_ID_GPIO0);

    if(Gpio_GetPinLevel(GPIO_CH0, WIFI_SPI_INT))
    {
        printf("spi int has happen\n");
        wifi_interrupt();
    }
    printf("exti_init exit\n");

}


void wifi_spi_ctl_bus(uint8 cs_select)
{
    if(cs_select)
    {
        Gpio_SetPinLevel(GPIO_CH0, GPIOPortC_Pin5, GPIO_LOW);

    }
    else
    {
        Gpio_SetPinLevel(GPIO_CH0, GPIOPortC_Pin5, GPIO_HIGH);

    }

}

uint32 wifi_spi_read( uint8 * buf_ptr, uint32 length)
{
	uint32 len = length;
	uint8 *data = buf_ptr;
    SpiDev_Read(g_hSpi,buf_ptr, length,0, SYNC_MODE, NULL);
    //printf("spi read size :%d\n",length);
	//debug_hex(buf_ptr,length,16);
	return 0;
}

uint32 wifi_spi_write( uint8 * buf_ptr, uint32 length)
{
    //rk_printf("spi write size :%d\n",length);
    //debug_hex(buf_ptr,length,16);
    uint len = length;
    uint len1 =0;
    #if 0
    while(len)
    {
      if(len >= 4)
      {
         SpiDev_Write(g_hSpi,buf_ptr+len1, 4, SYNC_MODE, NULL);
         len -= 4;
         len1 += 4;
      }
      else
      {
          SpiDev_Write(g_hSpi,buf_ptr, len, SYNC_MODE, NULL);
          len = 0;
      }


    }
    #endif
    SpiDev_Write(g_hSpi,buf_ptr, length, SYNC_MODE, NULL);
    DelayUs(1);
    return 0;
}

uint32 wifi_spi_rx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_read(buf_ptr, length);
	return 0;
}

uint32 wifi_spi_tx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_write(buf_ptr,length);
	return 0;
}

void wifi_spi_enable_interrupt(void)
{
    Gpio_EnableInt(GPIO_CH0,WIFI_SPI_INT);
}




void wifi_spi_disable_interrupt(void)
{
    Gpio_DisableInt(GPIO_CH0,WIFI_SPI_INT);
}
#endif

#if SPI_WIFI_BOARD_VERSIONS == RKNANOD_SDK_MAIN_V12_20150515HJH
void wifi_spi_init(void)
{
    SPI_DEV_ARG stSpiArg;
    HDC hSpi,hStorage;
    HDC hDma;
    rk_err_t ret;

//    ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

//    if (ret != RK_SUCCESS)
//    {
//        goto err;
//    }

//    hDma = RKDev_Open(DEV_CLASS_DMA, 0, NOT_CARE);
//    if (hDma <= 0)
//    {
//        return ;
//    }

    stSpiArg.Ch = 0;
    stSpiArg.CtrlMode = SPI_CTL_TXRX_MASTER;//SPI_CTL_MODE;
    stSpiArg.SpiRate = 4000*1000;


    //rk_print_string("\r\nspi device init...");

    ret = RKDev_Create(DEV_CLASS_SPI, 1, &stSpiArg);
    if (ret != RK_SUCCESS)
    {
        goto err;
    }

    //rk_print_string("\r\nspinor device init...");

    hSpi = RKDev_Open(DEV_CLASS_SPI, 1, NOT_CARE);
    g_hSpi = hSpi;
    if ((hSpi == NULL) || (hSpi == (HDC)RK_ERROR) || (hSpi == (HDC)RK_PARA_ERR))
    {
        goto err;
    }
err:

    Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin5, 0); // cs
    Gpio_SetPinDirection(GPIO_CH1, GPIOPortA_Pin5, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH1, GPIOPortA_Pin5, GPIO_HIGH);
    Grf_GPIO_SetPinPull(GPIO_CH1, GPIOPortA_Pin5, DISABLE);


    Grf_GpioMuxSet(GPIO_CH0, GPIOPortB_Pin6, 0);  // wifi en
    Gpio_SetPinDirection(GPIO_CH0, GPIOPortB_Pin6, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH0, GPIOPortB_Pin6, GPIO_LOW);
    Grf_GPIO_SetPinPull(GPIO_CH0, GPIOPortB_Pin6, DISABLE);


    Grf_GpioMuxSet(GPIO_CH1, GPIOPortB_Pin2, 0);  // wifi rst
    Gpio_SetPinDirection(GPIO_CH1, GPIOPortB_Pin2, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH1, GPIOPortB_Pin2, GPIO_LOW);
    Grf_GPIO_SetPinPull(GPIO_CH1, GPIOPortB_Pin2, DISABLE);


    //rkos_sleep(10);
    DelayMs(10);
    Gpio_SetPinLevel(GPIO_CH0, GPIOPortB_Pin6, GPIO_HIGH);
    //rkos_sleep(10);
    DelayMs(10);
    Gpio_SetPinLevel(GPIO_CH1, GPIOPortB_Pin2, GPIO_HIGH);
    return;

}


void wifi_spi_irq_hook(void)
{
    //printf("wifi_spi_irq\n");
    wifi_interrupt();
}

#define WIFI_SPI_INT_PORT GPIO_CH1
#define WIFI_SPI_INT GPIOPortB_Pin1

void exti_init(void)
{
    printf("exti_init enter\n");
    Grf_GpioMuxSet(WIFI_SPI_INT_PORT, WIFI_SPI_INT, 0);
    Gpio_SetPinDirection(WIFI_SPI_INT_PORT, WIFI_SPI_INT, GPIO_IN);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_INT, ENABLE);
    Gpio_SetIntMode(WIFI_SPI_INT_PORT,WIFI_SPI_INT, IntrTypeHighLevel);
    GpioIsrRegister(WIFI_SPI_INT_PORT,WIFI_SPI_INT, wifi_spi_irq_hook);
    Gpio_UnMaskInt(WIFI_SPI_INT_PORT,WIFI_SPI_INT);
    Gpio_EnableInt(WIFI_SPI_INT_PORT,WIFI_SPI_INT);

    IntRegister(INT_ID_GPIO1,GpioInt1);
    IntPendingClear(INT_ID_GPIO1);
    IntEnable(INT_ID_GPIO1);

    if(Gpio_GetPinLevel(WIFI_SPI_INT_PORT, WIFI_SPI_INT))
    {
        printf("spi int has happen\n");
        wifi_interrupt();
    }
    printf("exti_init exit\n");

}


void wifi_spi_ctl_bus(uint8 cs_select)
{
    if(cs_select)
    {
        Gpio_SetPinLevel(GPIO_CH1, GPIOPortA_Pin5, GPIO_LOW);

    }
    else
    {
        Gpio_SetPinLevel(GPIO_CH1, GPIOPortA_Pin5, GPIO_HIGH);

    }

}

uint32 wifi_spi_read( uint8 * buf_ptr, uint32 length)
{
	uint32 len = length;
	uint8 *data = buf_ptr;
    SpiDev_Read(g_hSpi,buf_ptr, length,0, SYNC_MODE, NULL);
    //printf("spi read size :%d\n",length);
	//debug_hex(buf_ptr,length,16);
	return 0;
}

uint32 wifi_spi_write( uint8 * buf_ptr, uint32 length)
{

    uint len = length;
    uint len1 =0;
    #if 0
    while(len)
    {
      if(len >= 4)
      {
         SpiDev_Write(g_hSpi,buf_ptr+len1, 4, SYNC_MODE, NULL);
         len -= 4;
         len1 += 4;
      }
      else
      {
          SpiDev_Write(g_hSpi,buf_ptr, len, SYNC_MODE, NULL);
          len = 0;
      }


    }
    #endif

    //printf("spi write size :%d\n",length);
    //debug_hex(buf_ptr,length,16);
    SpiDev_Write(g_hSpi,buf_ptr, length, SYNC_MODE, NULL);

    //printf("wifi_spi_write exit\n");
    //DelayUs(1);
    return 0;
}

uint32 wifi_spi_rx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_read(buf_ptr, length);
	return 0;
}

uint32 wifi_spi_tx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_write(buf_ptr,length);
	return 0;
}

void wifi_spi_enable_interrupt(void)
{
    Gpio_EnableInt(GPIO_CH1,WIFI_SPI_INT);
}




void wifi_spi_disable_interrupt(void)
{
    Gpio_DisableInt(GPIO_CH1,WIFI_SPI_INT);
}

#endif

#else

#define WIFI_SPI_CLK GPIOPortC_Pin2
#define WIFI_SPI_IN  GPIOPortC_Pin1
#define WIFI_SPI_OUT GPIOPortC_Pin0
#define WIFI_SPI_CS  GPIOPortC_Pin5
#define WIFI_SPI_INT GPIOPortC_Pin7

#define WIFI_SPI_RATE  (1000000)
#define WIFI_CLK_DELAY (4)

#define SET_CLK_HIGH  Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CLK, GPIO_HIGH)
#define SET_CLK_LOW   Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CLK, GPIO_LOW)

#define GET_SPI_IN    Gpio_GetPinLevel(GPIO_CH0, WIFI_SPI_IN)

#define SET_SPI_OUT_HIGT  Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_OUT, GPIO_HIGH)
#define SET_SPI_OUT_LOW   Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_OUT, GPIO_LOW)

void wifi_spi_irq_hook(void)
{
    //printf("wifi_spi_irq\n");
    wifi_interrupt();
}

void wifi_spi_delay(UINT32 time)
{
    uint32 SysFreq;
    volatile uint32 i;

//    SysFreq = chip_freq.hclk_sys_core/10000000;
//    i = (SysFreq*time)/5/time;

//	if(i)
//        ASMDelay(i);
}


void wifi_spi_init(void)
{
#if 1


    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_CS, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_CS, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_HIGH);
    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_CS, DISABLE);

    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_IN, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_IN, GPIO_IN);
    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_IN, ENABLE);

    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_OUT, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_OUT, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_OUT, GPIO_HIGH);
    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_OUT, DISABLE);



    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_CLK, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_CLK, GPIO_OUT);
    Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CLK, GPIO_LOW);
    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_CLK, DISABLE);


    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_INT, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_INT, GPIO_IN);
    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_INT, DISABLE);
//    exti_init();
    return;
#endif

}

void gpio_spi_write(uint8 *data , uint len)
{
    int i,j;
    vTaskSuspendAll();
    uint8 *pdata = data;
    for(i=0; i<len;i++)
    {
        for(j=7;j>=0;j--)
        {
            if(pdata[i] & (1<< j))
            {
                SET_SPI_OUT_HIGT;
            }
            else
            {
                SET_SPI_OUT_LOW;
            }
            //wifi_spi_delay(WIFI_CLK_DELAY);
            SET_CLK_HIGH;
            //wifi_spi_delay(WIFI_CLK_DELAY);
            SET_CLK_LOW;
        }
    }
    xTaskResumeAll();
}

void gpio_spi_read(uint8 *data , uint len)
{
    int i,j;
    vTaskSuspendAll();
    for(i=0; i<len;i++)
    {
        data[i]=0;
        for(j=7;j>=0;j--)
        {
            if(GET_SPI_IN)
            {
                data[i] |= 1<< j;
            }
            //wifi_spi_delay(WIFI_CLK_DELAY);
            SET_CLK_HIGH;
            //wifi_spi_delay(WIFI_CLK_DELAY);
            SET_CLK_LOW;
        }
    }
    xTaskResumeAll();
}


void exti_init(void)
{
    printf("exti_init enter\n");
    Grf_GpioMuxSet(GPIO_CH0, WIFI_SPI_INT, 0);
    Gpio_SetPinDirection(GPIO_CH0, WIFI_SPI_INT, GPIO_IN);
//    Grf_GPIO_SetPinPull(GPIO_CH0, WIFI_SPI_INT, ENABLE);
    Gpio_SetIntMode(GPIO_CH0,WIFI_SPI_INT, IntrTypeHighLevel);
    GpioIsrRegister(GPIO_CH0,WIFI_SPI_INT, wifi_spi_irq_hook);
    Gpio_UnMaskInt(GPIO_CH0,WIFI_SPI_INT);
    Gpio_EnableInt(GPIO_CH0,WIFI_SPI_INT);
    IntRegister(INT_ID_GPIO0,GpioInt0);
    IntPendingClear(INT_ID_GPIO0);
    IntEnable(INT_ID_GPIO0);

    if(Gpio_GetPinLevel(GPIO_CH0, WIFI_SPI_INT))
    {
        printf("spi int has happen\n");
        wifi_interrupt();
    }
    printf("exti_init exit\n");
}


void wifi_spi_ctl_bus(uint8 cs_select)
{
    if(cs_select)
    {
        Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_LOW);
        //Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_LOW);
    }
    else
    {
        Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_HIGH);
        //Gpio_SetPinLevel(GPIO_CH0, WIFI_SPI_CS, GPIO_HIGH);

        SET_SPI_OUT_LOW;
    }

}

uint spi_log_enable = 0;

void wifi_spi_data_log_enable()
{
    spi_log_enable = 1;
}

void wifi_spi_data_log_disable()
{
    spi_log_enable = 0;
}

uint32 wifi_spi_read( uint8 * buf_ptr, uint32 length)
{
	uint32 len = length;
	uint8 *data = buf_ptr;

    gpio_spi_read(buf_ptr, length);
    if(spi_log_enable)
    {
        //printf("spi read size :%d\n",length);
        //debug_hex(buf_ptr,length,16);
    }
	return 0;
}

uint32 wifi_spi_write( uint8 * buf_ptr, uint32 length)
{
    if(spi_log_enable)
    {
        //printf("spi write size :%d\n",length);
        //debug_hex(buf_ptr,length,16);
    }
    gpio_spi_write(buf_ptr, length);
    return 0;
}

uint32 wifi_spi_rx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_read(buf_ptr, length);
	return 0;
}

uint32 wifi_spi_tx( uint8 * buf_ptr, uint32 length)
{
    wifi_spi_write(buf_ptr,length);
	return 0;
}

void wifi_spi_enable_interrupt(void)
{
    //printf("wifi_spi_enable_interrupt\n");
    Gpio_EnableInt(GPIO_CH0,WIFI_SPI_INT);
}




void wifi_spi_disable_interrupt(void)
{
    //printf("wifi_spi_disable_interrupt\n");
    Gpio_DisableInt(GPIO_CH0,WIFI_SPI_INT);
}

#endif
#pragma arm section code
#endif /*#ifdef __DRIVER_INNOPHACE_WIFI_C__*/