/*
********************************************************************************************
*
*  Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\WM8987\wm8987Device.c
* Owner: chad.Ma
* Date: 2014.12.3
* Time: 16:07:47
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    chad.Ma     2014.12.3     16:07:47   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_WM8987_WM8987DEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"
#include "DeviceInclude.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define  WM8987_Codec_IICAdress   (0x1a<<1)
#define  WM8987_Codec_IICSpeed    100//200


typedef  struct _WM8987_DEVICE_CLASS
{
    DEVICE_CLASS    stWM8987Device;            //device father class


    pSemaphore osWM8987cDevSem;
    pSemaphore osWM8987ReadSem;
    pSemaphore osWM8987WriteSem;
    pSemaphore osWM8987ControlReqSem;
    pSemaphore osWM8987OperSem;

//    WM8987_CODEC_CONFIG *CodecVolumeTable;

    HDC              hI2S;
    HDC              hI2C;
    I2S_mode_t       I2S_mode;
    eI2sDATA_WIDTH_t DataWidth;
    eI2sCs_t         I2S_CS;

}WM8987_DEVICE_CLASS;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static WM8987_DEVICE_CLASS * gpstWM8987DevISRHandler;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

uint8 WM8987outptr[32][176] =
{
    //0
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //8
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //16
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //24
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    {
    #include "test01_1K0_441.data"
    },
    //32
};
const uint32 WM8987length = 176 * 32 / 4;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
void WM8987DACunMute(HDC dev);
void WM8987DACMute(HDC dev);
void WM8987SetVolume(HDC dev,uint32 Volume);
void WM8987SetSampleRate(HDC dev,CodecFS_en_t CodecFS);
void WM8987SetMode(HDC dev,CodecMode_en_t Codecmode);
void WM89877DevDeinit(HDC dev);
rk_err_t WM8987ShellTest(HDC dev,  uint8 * pstr);
rk_err_t WM8987ShellHelp(HDC dev,  uint8 * pstr);
rk_err_t WM8987ShellOpen(HDC dev,  uint8 * pstr);
rk_err_t WM8987ShellPcb(HDC dev,  uint8 * pstr);
void WM8987DevInit(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON API rk_err_t WM8987DevConfigI2S(HDC dev,I2S_DEVICE_CONFIG_REQ_ARG *DevReqArg)
{
    int32  ret = RK_SUCCESS;
    WM8987_DEVICE_CLASS * pWM8987Dev =  (WM8987_DEVICE_CLASS *)dev;
    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;

    HDC hI2S = pWM8987Dev->hI2S;

    #if 1
    stI2sDevArg.i2smode = I2S_SLAVE_MODE;
    stI2sDevArg.i2sCS = I2S_EXT;
    stI2sDevArg.I2S_FS = I2S_FS_44100Hz;
    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;
    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
    stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    #endif
    ret = I2sDev_Control(hI2S, I2S_DEVICE_INIT_CMD, &stI2sDevArg);
    //ret = I2sDev_Control(hI2S, I2S_DEVICE_INIT_CMD, DevReqArg);

    if(RK_ERROR == ret)
    {
        ret = RK_ERROR;
        return ret;
    }
    return ret;
}
/*******************************************************************************
** Name: WM8987Dev_Controk
** Input:HDC dev, uint8  cmd, void *args
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 16:54:17
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON API rk_err_t WM8987Dev_Control(HDC dev, uint8  cmd, void *args)
{
    WM8987_DEVICE_CLASS * pWM8987Dev =  (WM8987_DEVICE_CLASS *)dev;
    WM8987_DEVICE_CONFIG_REQ_ARG * pCmdReqArg;
    I2S_DEVICE_CONFIG_REQ_ARG *I2sDevReqArg;

   rkos_semaphore_take(pWM8987Dev->osWM8987ControlReqSem, MAX_DELAY);

   if(cmd == WM8987_DEVICE_CONFIG_I2S_CMD)
   {
        I2sDevReqArg = (I2S_DEVICE_CONFIG_REQ_ARG *)args;
        WM8987DevConfigI2S(dev,I2sDevReqArg);
        rkos_semaphore_give(pWM8987Dev->osWM8987ControlReqSem);
        return RK_SUCCESS;
    }
    else if(cmd == WM8987_DEVICE_SET_MODE_CMD)
    {
        pCmdReqArg = (WM8987_DEVICE_CONFIG_REQ_ARG *)args;
        //codec mode set
        WM8987SetMode(dev,pCmdReqArg->Codecmode);
        rkos_semaphore_give(pWM8987Dev->osWM8987ControlReqSem);
        return RK_SUCCESS;
    }
    else if(cmd == WM8987_DEVICE_SET_SAMPLERATE_CMD)
    {
        pCmdReqArg = (WM8987_DEVICE_CONFIG_REQ_ARG *)args;
        //codec samplerate set
        WM8987DACunMute(dev);
        WM8987SetSampleRate(dev,pCmdReqArg->CodecFS);
        rkos_semaphore_give(pWM8987Dev->osWM8987ControlReqSem);
        return RK_SUCCESS;
    }
    else if(cmd == WM8987_DEVICE_SET_VOLUME_CMD)
    {
         WM8987SetVolume(dev,31);
         rkos_semaphore_give(pWM8987Dev->osWM8987ControlReqSem);
         return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}
/*******************************************************************************
** Name: WM8987Dec_Read
** Input:HDC dev,const void* buffer, uint32 size,uint8 mode
** Return: rk_err_T
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 16:52:17
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON API rk_err_t WM8987Dec_Read(HDC dev,const void* buffer, uint32 size,uint8 mode)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static
UINT16 Codec8987CommandRead(HDC dev,UINT32 RegAddr)
{
    UINT8 data[2];
    UINT16 Regdata;
    I2C_CMD_ARG stArg;
    WM8987_DEVICE_CLASS * pWM8987Dev =  (WM8987_DEVICE_CLASS *)dev;

    HDC hI2c = pWM8987Dev->hI2C;
    if(hI2c != NULL)
    {
        stArg.SlaveAddress = WM8987_Codec_IICAdress;
        stArg.RegAddr = (uint8)(RegAddr << 1);
        stArg.RWmode = NormalMode;
        stArg.speed = WM8987_Codec_IICSpeed;
        stArg.addr_reg_fmt  = I2C_7BIT_ADDRESS_8BIT_REG;


        I2cDev_ReadData(hI2c,data,1,&stArg);
    }
    Regdata = data[0];
    Regdata<<=8;
    Regdata |= data[1];
    return Regdata;
}

static
void Codec8987CommandSet(HDC dev,UINT32 RegAddr, UINT16 uData)
{
    uint8 cmd,reg_cmd;
    I2C_CMD_ARG stArg;
    rk_size_t size;

    WM8987_DEVICE_CLASS * pWM8987Dev =  (WM8987_DEVICE_CLASS *)dev;

    HDC hI2c = pWM8987Dev->hI2C;
    if(hI2c != NULL)
    {
        cmd = (UINT8)uData;
        reg_cmd = (UINT8)(((RegAddr << 9) | uData) >> 8);

        stArg.SlaveAddress  = WM8987_Codec_IICAdress;
        stArg.RegAddr       = reg_cmd;
        stArg.RWmode        = NormalMode;
        stArg.speed         = WM8987_Codec_IICSpeed;
        stArg.addr_reg_fmt  = I2C_7BIT_ADDRESS_8BIT_REG;


        size = I2cDev_SendData(hI2c,&cmd,1,&stArg);
        printf("size = %d\n",size);
        while(size == 1)
        {
            size = I2cDev_SendData(hI2c,&cmd,1,&stArg);
            DelayMs(5);
            printf("send data\n");
        }
    }

}

static
void Codec8987_PowerOnInitial(WM8987_DEVICE_CLASS *dev)
{
    Codec8987CommandSet(dev,WM8987_R15, 0x012f);                        //software reset wm8750
    printf("WM8987_R15\n");

    Codec8987CommandSet(dev,WM8987_R27, 0x0040);                        // set VREF to output 40K
    printf("WM8987_R27\n");

    Codec8987CommandSet(dev,WM8987_R10, WM_VOL_MUTE);                   // left DAC mute//声音控制
    printf("WM8987_R10\n");

    Codec8987CommandSet(dev,WM8987_R11, WM_VOL_MUTE | WM_UPDATE_VOL);   // right DAC mute
    printf("WM8987_R11\n");

    Codec8987CommandSet(dev,WM8987_R5,  0x0008);                        //Mute
    printf("WM8987_R5\n");


    DelayMs(10);
    Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF);          // setup Vmid and Vref, Vmid =
    printf("WM8987_R25\n");

    DelayMs(10);
    Codec8987CommandSet(dev,WM8987_R26, 0x0000);
    printf("WM8987_R26\n");

    //setup digital audio  interface
    Codec8987CommandSet(dev,WM8987_R7,  WM_MASTER_MODE|WM_I2S_MODE);     //WM_MASTER_MODE | WM_I2S_MODE);   //Audio inteface Data length and Data formate,16Bit
    printf("WM8987_R7\n");

    Codec8987CommandSet(dev,WM8987_R8,  FREQ441kHz | WM_USB_MODE);      //ADC and DAC sample frequence 44.118Khz,USB mode
    printf("WM8987_R8\n");

    //setup left and right MIXER
    Codec8987CommandSet(dev,WM8987_R34, LD2LO | LO0DB);
    printf("WM8987_R34\n");

    Codec8987CommandSet(dev,WM8987_R37, RD2RO | RO0DB);
    printf("WM8987_R37\n");

    //setup no used register
    Codec8987CommandSet(dev,WM8987_R35, 0x0070);                        // Right DAC to left MIXER: mute
    printf("WM8987_R35\n");

    Codec8987CommandSet(dev,WM8987_R36, 0x0070);                        //
    printf("WM8987_R36\n");

    Codec8987CommandSet(dev,WM8987_R38, 0x0070);                        //Mono out Mix Default: Mono Mixer no input, Lmixsel volume=0
    printf("WM8987_R38\n");

    Codec8987CommandSet(dev,WM8987_R39, 0x0070);                        //Mono out Mix Default: Mono Mixer no input, Rmixsel volume=0
    printf("WM8987_R39\n");

    Codec8987CommandSet(dev,WM8987_R2,  0x0100);                        // set R  output 1 mute
    printf("WM8987_R2\n");

    Codec8987CommandSet(dev,WM8987_R40, 0x0080);                        // set output 2 mute
    printf("WM8987_R40\n");

    Codec8987CommandSet(dev,WM8987_R41, 0x0180);                        // set output 2 mute
    printf("WM8987_R41\n");

    Codec8987CommandSet(dev,WM8987_R24, 0x0004);                        // set output 3 ref
    printf("WM8987_R24\n");

    Codec8987CommandSet(dev,EQ3D_enhance, EQ3D_disable);
    printf("EQ3D_enhance\n");


}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM8987Dev_Write
** Input:HDC dev,const void* buffer, uint32 size,uint8 mode
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 16:51:07
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON API rk_err_t WM8987Dev_Write(HDC dev,const void* buffer, uint32 size,uint8 mode)
{
    uint32 i;
    WM8987_DEVICE_CLASS * pstWM8987Dev =  (WM8987_DEVICE_CLASS *)dev;
    uint32 realsize;
    HDC hI2S = pstWM8987Dev->hI2S;
    int32  ret = RK_SUCCESS;

    if (mode == SYNC_MODE)
    {
        ret = I2sDev_Write(hI2S, (const void *)buffer, size, SYNC_MODE);
        return size;
    }
    else if (mode == ASYNC_MODE)
    {
        ret = I2sDev_Write(hI2S, (const void *)buffer, size, ASYNC_MODE);
        return size;
    }

    return size;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM8987DACunMute
** Input:HDC dev
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:50:01
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON FUN void WM8987DACunMute(HDC dev)
{
    Codec8987CommandSet(dev,WM8987_R5,   0x0000); // DAC soft unmute
}

/*******************************************************************************
** Name: WM8987DACMute
** Input:void
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:48:53
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON FUN void WM8987DACMute(HDC dev)
{
     Codec8987CommandSet(dev,WM8987_R5,   0x0008);      // DAC soft mute
}

/*******************************************************************************
** Name: WM8987SetVolume
** Input:HDC dev,uint32 Volume
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:46:03
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON FUN void WM8987SetVolume(HDC dev,uint32 Volume)
{
    UINT32 Vol ;

    if (Volume > MAX_VOLUME)
        Volume = MAX_VOLUME;

    Vol = 41 + (Volume << 1);

    if (Volume == 0)
    {
        Vol = 0;
    }

    if (Volume <= 7)
        Vol = 47 + Volume;

    printf("Vol = %d\n",Vol);
    Codec8987CommandSet(dev,WM8987_R40, Vol | 0x0080);    // set output 1 volume
    Codec8987CommandSet(dev,WM8987_R41, Vol | 0x0180);
}

/*******************************************************************************
** Name: WM8987SetSampleRate
** Input:CodecFS_en_t CodecFS
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:42:29
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON FUN void WM8987SetSampleRate(HDC dev,CodecFS_en_t CodecFS)
{
    switch (CodecFS)
    {
        case CodecFS_8000Hz:
            Codec8987CommandSet(dev,WM8987_R8,    FREQ8kHz | WM_USB_MODE);    //12MHZ,8KHZ
            break;
        case CodecFS_11025Hz:
            Codec8987CommandSet(dev,WM8987_R8,    FREQ11025kHz | WM_USB_MODE);    //12MHZ,11.025K
            break;
        case CodecFS_12KHz:
            Codec8987CommandSet(dev,WM8987_R8,    FREQ12kHz | WM_USB_MODE);    //12MHZ,12K
            break;
        case CodecFS_16KHz:
            Codec8987CommandSet(dev,WM8987_R8,    FREQ16kHz | WM_USB_MODE);    //12MHZ,16K
            break;
        case CodecFS_22050Hz:
            Codec8987CommandSet(dev,WM8987_R8,     FREQ2205kHz | WM_USB_MODE);    //12MHZ,22.05K
            break;
        case CodecFS_24KHz:
            Codec8987CommandSet(dev,WM8987_R8,     FREQ24kHz | WM_USB_MODE);    //12MHZ,24K
            break;
        case CodecFS_32KHz:
            Codec8987CommandSet(dev,WM8987_R8,     FREQ32kHz | WM_USB_MODE);    //12MHZ,32K
            break;
        case CodecFS_48KHz:
            Codec8987CommandSet(dev,WM8987_R8,     FREQ48kHz | WM_USB_MODE);    //12MHZ,48K
            break;
        case CodecFS_44100Hz:
        default:
            Codec8987CommandSet(dev,WM8987_R8,     FREQ441kHz | WM_USB_MODE);    //12MHZ,44.1KHZ
            break;
    }

}

/*******************************************************************************
** Name: WM8987SetMode
** Input:WM8987Mode_en_t mode
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:37:29
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_COMMON_
COMMON FUN void WM8987SetMode(HDC dev,CodecMode_en_t Codecmode)
{
    switch (Codecmode)
    {
        case Codec_DACout:
        {
//            Codec8987CommandSet(WM8987_R7,  WM_SLAVE_MODE | WM_I2S_MODE );
            Codec8987CommandSet(dev,WM8987_R8,  FREQ441kHz | WM_USB_MODE);

            Codec8987CommandSet(dev,WM8987_R34, LD2LO | LO0DB);
            Codec8987CommandSet(dev,WM8987_R37, RD2RO | RO0DB);

            Codec8987CommandSet(dev,WM8987_R10, WM_VOL_0DB);
            Codec8987CommandSet(dev,WM8987_R11, WM_VOL_0DB | WM_UPDATE_VOL);

            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF);
            Codec8987CommandSet(dev,WM8987_R26, WM_DACL | WM_DACR | WM_LOUT2 | WM_ROUT2 | WM_OUT3);
        }
        break;

        case Codec_FMin:
        {
            Codec8987CommandSet(dev,WM8987_R34, LI2LO | LO0DB);
            Codec8987CommandSet(dev,WM8987_R37, RI2RO | RO0DB);

            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF);
            Codec8987CommandSet(dev,WM8987_R26, WM_LOUT2 | WM_ROUT2 | WM_OUT3);
        }
        break;

        case Codec_FMADC:
        {
            Codec8987CommandSet(dev,WM8987_R0,  0x003f);    //Left channel PGA  0dB
            Codec8987CommandSet(dev,WM8987_R1,  0x013f);    //Left channel PGA   0dB

            Codec8987CommandSet(dev,WM8987_R21, 0x00c3);    //Left digital ADC 0dB
            Codec8987CommandSet(dev,WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec8987CommandSet(dev,WM8987_R31, 0x0000);

            Codec8987CommandSet(dev,WM8987_R32, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB
            Codec8987CommandSet(dev,WM8987_R33, 0x0000);    //Left ADC select = Linput3 MICBoost = 0dB

            Codec8987CommandSet(dev,WM8987_R34, LI2LO | LO0DB);
            Codec8987CommandSet(dev,WM8987_R37, RI2RO | RO0DB);

            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF | WM_ADCL | WM_ADCR);
            Codec8987CommandSet(dev,WM8987_R26, WM_LOUT2 | WM_ROUT2 | WM_OUT3);
        }
        break;

        case Codec_LineADC:
        {
            Codec8987CommandSet(dev,WM8987_R0,  0x0057);    //Left channel PGA  0dB
            Codec8987CommandSet(dev,WM8987_R1,  0x0157);    //Left channel PGA   0dB

            Codec8987CommandSet(dev,WM8987_R21, 0x00c3);    //Left digital ADC 0dB
            Codec8987CommandSet(dev,WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec8987CommandSet(dev,WM8987_R31, 0x0000);

            Codec8987CommandSet(dev,WM8987_R32, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB
            Codec8987CommandSet(dev,WM8987_R33, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB

            Codec8987CommandSet(dev,WM8987_R34, LO0DB);
            Codec8987CommandSet(dev,WM8987_R37, RO0DB);

            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF | WM_ADCL | WM_ADCR);
            Codec8987CommandSet(dev,WM8987_R26, 0x0000);
        }
        break;

        case Codec_MICAdc:
        {
            Codec8987CommandSet(dev,WM8987_R0,  0x0117);    //Left channel PGA  0dB
            Codec8987CommandSet(dev,WM8987_R1,  0x0117);    //Left channel PGA   0dB

            Codec8987CommandSet(dev,WM8987_R21, 0x01c3);    //Left digital ADC 0dB
            Codec8987CommandSet(dev,WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec8987CommandSet(dev,WM8987_R10, 0x01ff);    //Left digital DAC 0dB
            Codec8987CommandSet(dev,WM8987_R11, 0x01ff);    //Right  digital DAC 0dB

            Codec8987CommandSet(dev,WM8987_R31, 0x0040);    // adc use mono mix, select left adc

            Codec8987CommandSet(dev,WM8987_R32, 0x00a0);    //Left ADC select = Linput3 MICBoost = 20dB
            Codec8987CommandSet(dev,WM8987_R33, 0x0080);    //Left ADC select = Linput3 MICBoost = 00dB

            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF | WM_AINL | WM_ADCL | WM_MICB);   //Left ADC and Left PGA power on,Right ADC and Right PGA power off
            Codec8987CommandSet(dev,WM8987_R26, 0x00);
        }
        break;

        case Codec_Standby:
        {
            Codec_DACMute();
            Codec8987CommandSet(dev,WM8987_R25, WM_VMID50k | WM_VREF);
            Codec8987CommandSet(dev,WM8987_R26, 0x00);
        }
        break;

    }
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM8987Dev_Create
** Input:uint32 DevID, void *arg
** Return: HDC
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 16:49:44
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_INIT_
INIT API HDC WM8987Dev_Create(uint32 DevID, void *arg)
{
    DEVICE_CLASS* pstDev;
    WM8987_DEVICE_CLASS * psWM8987Dev;
    WM8987_DEV_ARG * pstWM8987Arg;

    if(arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstWM8987Arg = (WM8987_DEV_ARG *)arg;
    rkos_enter_critical();

    psWM8987Dev =  rkos_memory_malloc(sizeof(WM8987_DEVICE_CLASS));
    if (psWM8987Dev == NULL)
    {
        return (HDC) RK_ERROR;
    }

    psWM8987Dev->osWM8987WriteSem       = rkos_semaphore_create(1,1);
    psWM8987Dev->osWM8987ControlReqSem  = rkos_semaphore_create(1,1);

    if ((  psWM8987Dev->osWM8987WriteSem
            && psWM8987Dev->osWM8987ControlReqSem) == 0)
    {
        rkos_semaphore_delete(psWM8987Dev->osWM8987WriteSem);
        rkos_semaphore_delete(psWM8987Dev->osWM8987ControlReqSem);

        rkos_memory_free(psWM8987Dev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)psWM8987Dev;

    pstDev->suspend = NULL;//WM8987DevSuspend;
    pstDev->resume  = NULL;//WM8987DevResume;


    psWM8987Dev->hI2S = pstWM8987Arg->hI2s;
    psWM8987Dev->hI2C = pstWM8987Arg->hI2c;

    gpstWM8987DevISRHandler = psWM8987Dev;

    rkos_exit_critical();
    WM8987DevInit(psWM8987Dev);

    return pstDev;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM89877DevDeinit
** Input:HDC dev
** Return: void
** Owner:chad.Ma
** Date: 2014.12.18
** Time: 9:35:33
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_INIT_
INIT FUN void WM89877DevDeinit(HDC dev)
{
    Codec8987CommandSet(dev,WM8987_R25,    WM_VREF);
    Codec8987CommandSet(dev,WM8987_R26,    0x00);
}


/*******************************************************************************
** Name: WM8987DevInit
** Input:HDC dev
** Return: void
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 17:08:37
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_INIT_
INIT FUN void WM8987DevInit(HDC dev)
{
    WM8987_DEVICE_CLASS * psWM8987Dev;
    psWM8987Dev = (WM8987_DEVICE_CLASS *)dev;
    WM8987DevConfigI2S(dev,NULL);
    Codec8987_PowerOnInitial(psWM8987Dev);
}


#ifdef _WM8987_DEV_SHELL_
_DRIVER_WM8987_WM8987DEVICE_SHELL_
static char* ShellWM8987Name[] =
{
    "pcb",
    "open",
    "close",
    "suspend",
    "resume",
    "read",
    "write",
    "control",
    "help",
    "test",
    "\b",                          // the end
};
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM8987Dev_Shell
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 16:55:23
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_SHELL_
SHELL API rk_err_t WM8987Dev_Shell(HDC dev,  uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    uint   ret;
    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem,&Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    while (memcmp((uint8*)ShellWM8987Name[i],pItem,strlen(ShellWM8987Name[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellWM8987Name[i]) == '\b')
            return RK_ERROR;
    }

    pItem += StrCnt;
    pItem++;                           //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = WM8987ShellPcb(dev,pItem);
            break;

        case 0x01:  //open
            ret = WM8987ShellOpen(dev,pItem);
            break;

        case 0x02:  //close
            //ret = WM8987ShellClose(dev,pItem);
            break;

        case 0x03:  //suspend
            //ret = WM8987ShellSuspend(dev,pItem);
            break;

        case 0x04:  //resume
            //ret = WM8987ShellRsume(dev,pItem);

        case 0x05:  //read
            //ret = WM8987ShellRead(dev,pItem);
            break;

        case 0x06:  //write
            //ret = WM8987ShellWrite(dev,pItem);
            break;

        case 0x07:  //control
            //ret = WM8987ShellControl(dev,pItem);
            break;

        case 0x08:  //help
        #ifdef SHELL_HELP
            ret = WM8987ShellHelp(dev,pItem);
        #endif
            break;

        case 0x09:
            ret = WM8987ShellTest(dev, pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WM8987ShellTest
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 18:40:35
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_SHELL_
SHELL FUN rk_err_t WM8987ShellTest(HDC dev,  uint8 * pstr)
{
    HDC hWM8987;
    rk_err_t ret;
    uint32 vol,time = 0;

    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    WM8987_DEVICE_CONFIG_REQ_ARG stWM8987DevArg;
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.test : test RockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    stI2sDevArg.i2smode = I2S_SLAVE_MODE;
    stI2sDevArg.i2sCS = I2S_EXT;
    stI2sDevArg.I2S_FS = I2S_FS_44100Hz;
    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;
    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
    stI2sDevArg.I2S_Bus_mode = I2S_NORMAL_MODE;

    stWM8987DevArg.Codecmode = Codec_DACout;
    stWM8987DevArg.CodecFS = CodecFS_44100Hz;

    hWM8987 = RKDev_Open(DEV_CLASS_WM8987,0,NOT_CARE);

    WM8987Dev_Control(hWM8987, WM8987_DEVICE_CONFIG_I2S_CMD, &stI2sDevArg);
    WM8987Dev_Control(hWM8987, WM8987_DEVICE_SET_MODE_CMD, &stWM8987DevArg);
    WM8987Dev_Control(hWM8987, WM8987_DEVICE_SET_SAMPLERATE_CMD, &stWM8987DevArg);
    WM8987Dev_Control(hWM8987, WM8987_DEVICE_SET_VOLUME_CMD, NULL);
    while(1)
    {
        ret = WM8987Dev_Write(hWM8987,(const void *)WM8987outptr, WM8987length, ASYNC_MODE);
    }
    return RK_SUCCESS;

}
/*******************************************************************************
** Name: WM8987ShellHelp
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 18:40:02
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_SHELL_
SHELL FUN rk_err_t WM8987ShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("8987命令集提供了一系列的命令对WM8987进行操作\r\n");
    rk_print_string("8987包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("open      打开WM8987        \r\n");
    rk_print_string("close     关闭WM8987命令    \r\n");
    rk_print_string("suspend   挂起WM8987命令    \r\n");
    rk_print_string("resume    恢复WM8987命令    \r\n");
    rk_print_string("read      读取WM8987命令    \r\n");
    rk_print_string("write     写入WM8987命令    \r\n");
    rk_print_string("help      显示WM8987命令帮助信息  \r\n");

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: WM8987ShellOpen
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 18:39:26
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_SHELL_
SHELL FUN rk_err_t WM8987ShellOpen(HDC dev,  uint8 * pstr)
{
    HDC hWM8987;
    WM8987_DEVICE_CLASS * pDev;
    rk_err_t ret;
    WM8987_DEV_ARG stWM8987DevArg;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("8987.open : wm89987 open cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    stWM8987DevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, 0, NOT_CARE);
    stWM8987DevArg.hI2c = RKDev_Open(DEV_CLASS_I2C, I2C_CH0, NOT_CARE);

    if((stWM8987DevArg.hI2s == NULL)
        || (stWM8987DevArg.hI2s == (HDC)RK_ERROR)
        || (stWM8987DevArg.hI2s == (HDC)RK_PARA_ERR)
        )
    {
        UartDev_Write(dev,"I2S open failure",17,SYNC_MODE,NULL);
        return RK_SUCCESS;
    }


    if((stWM8987DevArg.hI2c == NULL)
        || (stWM8987DevArg.hI2c == (HDC)RK_ERROR)
        || (stWM8987DevArg.hI2c == (HDC)RK_PARA_ERR)
        )
    {
        UartDev_Write(dev,"I2c open failure",17,SYNC_MODE,NULL);
        return RK_SUCCESS;
    }


    RKDev_Create(DEV_CLASS_WM8987, 0, &stWM8987DevArg);

    hWM8987 = RKDev_Open(DEV_CLASS_WM8987,0,NOT_CARE);
    if((hWM8987 == NULL)
        || (hWM8987 == (HDC)RK_ERROR)
        || (hWM8987 == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev,"WM8987 open failure",17,SYNC_MODE,NULL);
    }

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: WM8987ShellPcb
** Input:HDC dev,  uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.12.3
** Time: 18:38:41
*******************************************************************************/
_DRIVER_WM8987_WM8987DEVICE_SHELL_
SHELL FUN rk_err_t WM8987ShellPcb(HDC dev,  uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("8987.pcb : pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

	return RK_SUCCESS;
}

#endif

#endif

