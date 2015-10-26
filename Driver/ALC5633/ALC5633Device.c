/*
********************************************************************************************
*
*      Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: ..\Driver\ALC5633\ALC5633Device.c
* Owner: HJ
* Date: 2014.3.10
* Time: 13:51:36
* Desc: Uart Device Class
* History:
*   <author>    <date>       <time>     <version>     <Desc>
*      hj     2014.3.10     13:51:36   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_ALC5633_ALC5633DEVICE_C__

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
UINT8 I2C_slaveaddr ;

#define _DRIVER_ALC5633_ALC5633DEVIE_READ_  __attribute__((section("driver_ALC5633_ALC5633devie_read")))
#define _DRIVER_ALC5633_ALC5633DEVIE_WRITE_ __attribute__((section("driver_ALC5633_ALC5633devie_write")))
#define _DRIVER_ALC5633_ALC5633DEVIE_INIT_  __attribute__((section("driver_ALC5633_ALC5633devie_init")))
#define _DRIVER_ALC5633_ALC5633DEVIE_SHELL_  __attribute__((section("driver_ALC5633_ALC5633devie_shell")))

typedef struct _ALC5633_CODEC_CONFIG
{
    UINT8 DacAnaVol;
    UINT8 DacDigVol;
}ALC5633_CODEC_CONFIG;

ALC5633_CODEC_CONFIG VOL_ALC5633[30 + 1] =
{
    //+0dB                //+4dB               //+7db        //+12dB      //BASS
    0, 254,   //  0
    0, 142,     //  1
    0, 137,   //  2
    0, 132,   //  3
    0, 126,   //  4
    0, 120,   //  5
    0, 114,  //  6
    0, 109,  //  7
    0, 103,  //  8
    0, 97,   //  9
    0, 91,  //  10
    0, 87,   //  11
    0, 81,   //  12
    0, 75,  //  13
    0, 69,    //  14
    0, 64,   //  15
    0, 59,  //  16
    0, 51,  //  17
    0, 47,   //  18
    0, 42,   //  19
    0, 36,    //  20
    0, 28,    //  21
    0, 23,    //  22
    0, 17,    //  23
    0, 12,    //  24
    0, 7,     //  25
    0, 0,     //  26
    2, 0,     //  27
    4, 0,     //  28
    6, 0,     //  29
    8, 2,    //  30

};

typedef struct _ALC5633_DEVICE_CONFIG_REQ_ARG
{
    CodecMode_en_t Codecmode;
    CodecFS_en_t CodecFS;

}ALC5633_DEVICE_CONFIG_REQ_ARG;

typedef  struct _ALC5633_DEVICE_CLASS
{
    DEVICE_CLASS    stALC5633Device;            //device father class


    pSemaphore osALC5633cDevSem;
    pSemaphore osALC5633ReadSem;
    pSemaphore osALC5633WriteSem;
    pSemaphore osALC5633ControlReqSem;
    pSemaphore osALC5633OperSem;

    ALC5633_CODEC_CONFIG *CodecVolumeTable;

    HDC              hI2S;
    HDC              hI2C;
    I2S_mode_t I2S_mode;
    eI2sDATA_WIDTH_t   DataWidth;
    eI2sCs_t I2S_CS;

}ALC5633_DEVICE_CLASS;


typedef  struct _I2C_TX_ITEM
{
    UINT8 SlaveAddress;
    UINT8 RegAddr;
    eI2C_RW_mode_t RWmode;
    eI2C_Address_Reg_t addr_reg_fmt;
    uint8* DataBuf;
    uint32 size;

}I2C_TX_ITEM;


typedef  struct _I2C_RX_ITEM
{
    UINT8 SlaveAddress;
    UINT8 RegAddr;
    eI2C_RW_mode_t RWmode;
    eI2C_Address_Reg_t addr_reg_fmt;
    uint8* DataBuf;
    uint32 size;
}I2C_RX_ITEM;

typedef  struct _I2C_DEVICE_CLASS
{
    DEVICE_CLASS    stI2CDevice;            //device father class

    pSemaphore osI2CReadReqSem;
    pSemaphore osI2CWriteReqSem;

    pSemaphore osI2CReadSem;
    pSemaphore osI2CWriteSem;


    I2C_TX_ITEM          stTxItem;
    I2C_RX_ITEM          stRxItem;

    uint32           dwTxNeedtoSend;
    uint32           dwRxNeedtoRev;
    uint32           dwTxStatus;
    uint32           dwRxStatus;

    UINT16 speed ;
    eI2C_mode_t mode;

}I2C_DEVICE_CLASS;


/*
--------------------------------------------------------------------------------

                        Macro define

--------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static ALC5633_DEVICE_CLASS * gpstALC5633DevISRHandler;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
rk_err_t ALC5633ShellTest(HDC dev, uint8 * pstr);
rk_err_t ALC5633ShellHelp(HDC dev, uint8 * pstr);
#if 1
rk_err_t ALC5633DevResume(HDC dev);
rk_err_t ALC5633DevSuspend(HDC dev);
rk_err_t ALC5633DevDelete(HDC dev);
void ALC5633IntIsr(void);
#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------

*/
__attribute__((aligned (8))) const uint8 ALC5633outptr[32][176] =
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
const uint32 ALC5633length = 176 * 32 / 4;

//(0x1C<<1)
#define  ALC5633_Codec_IICAdress   0x38
#define  ALC5633_Codec_IICSpeed    200
static void Codec5633CommandSet(ALC5633_DEVICE_CLASS *dev,UINT8 uCmd, UINT16 uData);

void Codec5633_DACMute(HDC dev)
{
    Codec5633CommandSet(dev,RT5633_DAC_CTRL,DAC_L_MUTE|DAC_R_MUTE);       // DAC soft mute
}

UINT16 Codec5633CommandRead(HDC dev,UINT8 RegAddr)
{
    UINT8 ret,data[2];
    UINT16 Regdata;
    I2C_CMD_ARG stArg;
    ALC5633_DEVICE_CLASS * pALC5633Dev =  (ALC5633_DEVICE_CLASS *)dev;

    HDC hI2c = pALC5633Dev->hI2C;
    if (hI2c != NULL)
    {
        stArg.SlaveAddress = ALC5633_Codec_IICAdress;
        stArg.RegAddr = RegAddr;
        stArg.RWmode = NormalMode;
        stArg.addr_reg_fmt = I2C_7BIT_ADDRESS_8BIT_REG;
        stArg.speed = ALC5633_Codec_IICSpeed;

        I2cDev_ReadData(hI2c,data,2,&stArg);
    }
    Regdata = data[0];
    Regdata<<=8;
    Regdata |= data[1];
    return Regdata;
}

static void Codec5633CommandSet(ALC5633_DEVICE_CLASS *dev,UINT8 RegAddr, UINT16 uData)
{
    uint8 cmd,ret,data[2];
    I2C_CMD_ARG stArg;
    I2C_DEVICE_ARG stI2cArg;
    //I2C_DEVICE_CLASS *dev_5633;

    HDC hI2c = dev->hI2C;
    if (hI2c != NULL)
    {
        //cmd  = ((RegAddr << 1) | (uData >> 8) & 0x01);
        //data = (uint8)(uData & 0xff);
        data[0] = (0xff00 & uData) >> 8;
        data[1] = (0xff & uData);

        stArg.SlaveAddress = ALC5633_Codec_IICAdress;
        stArg.RegAddr = RegAddr;
        stArg.RWmode = NormalMode;
        stArg.speed = ALC5633_Codec_IICSpeed;
        stArg.addr_reg_fmt  = I2C_7BIT_ADDRESS_8BIT_REG;

        //if(RK_SUCCESS != I2cDev_SendData(hI2c,data,2,&stArg))//hI2c
        //{

        //}
        ret= I2cDev_SendData(hI2c,data,2,&stArg);//hI2c
        if (ret <= 0) //ack not recived,try again.
        {
            if (ret == TIMEOUT)
                rk_printf("i2c write time out ! \n");

            if (ret == ERROR)
                rk_printf("i2c write error happen (noACK)! \n" );

            if (ret == 0)
                rk_printf("i2c write success! \n" );

//   	    DelayMs(5);
//          I2C_Write(I2C_CH1,uCmd,data,2, NormalMode);
//          printf("send data %d \n",cmd);
        }
        //I2C_Deinit();
    }

}


void ALC5633CodecPowerOnInitial(ALC5633_DEVICE_CLASS *dev)
{
    UINT8 uData[16] = {0,};
    UINT16 ret;
    I2C_Init(I2C_CH1, ALC5633_Codec_IICSpeed);//commandset read 中有ALC5633_Codec_IICAdress
    Codec5633CommandSet(dev,RT5633_RESET, 0x00);   //reset 0x00
    ///////////////////////////////////////////////////////////
    //power
    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD3,PWR_VREF | PWR_MAIN_BIAS | PWR_HP | PWR_HP_AMP);   //power on

    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD1,(PWR_MAIN_I2S_EN |
                        //PWR_ADC_L_CLK | PWR_ADC_R_CLK |
                        PWR_DAC_L_CLK | PWR_DAC_R_CLK | PWR_DAC_REF |
                        PWR_DAC_L_TO_MIXER | PWR_DAC_R_TO_MIXER));                       //power I2S and DAC

    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD2,(PWR_PLL |
                        PWR_HPMIXER_L | PWR_HPMIXER_R));
    // PWR_RECMIXER_L|PWR_RECMIXER_R |
    //PWR_LINEIN1_L |PWR_LINEIN1_R));                         //POWER PLL

    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD4, (PWR_HP_L_VOL | PWR_HP_R_VOL));  //power hp vol

    // input set
    Codec5633CommandSet(dev,RT5633_REC_MIXER_CTRL, 0xf7f7); //0xf7f7                //Line 1 Input
    Codec5633CommandSet(dev,RT5633_ADC_CTRL,0x0000);   //0x0000                     //ADC unmute
    Codec5633CommandSet(dev,RT5633_LINE_IN_1_VOL,0x0000); //0x0000 12dB                  // Line 1 Vol 0db
    Codec5633CommandSet(dev,RT5633_ADC_DIG_VOL, 0x0000);                    // ADC vol 0db

    // output set
    Codec5633CommandSet(dev,RT5633_DAC_CTRL,0x0202);  //cjh                       // 0x0000 DAC unmute
    Codec5633CommandSet(dev,RT5633_DAC_DIG_VOL,0x0000);                      // 0db
    Codec5633CommandSet(dev,RT5633_HPMIXER_CTRL,0xfefe);                    // HpMix DAC output
    //Codec5633CommandSet(dev,RT5633_HPMIXER_CTRL,0xdfdf);                    // HpMix RecMix output
    Codec5633CommandSet(dev,RT5633_SPK_HP_MIXER_CTRL,0x20); //0x20              // hp vol to hpout
    Codec5633CommandSet(dev,RT5633_HP_OUT_VOL, 0x4040);                     // HPMIX output

    //setup digital audio  interface
    //setup digital audio  interface

#ifdef BIT_TEST
    Codec5633CommandSet(dev,RT5633_SDP_CTRL,  0xA000);//cjh //I2S codec is master0x0000
    //Codec5633CommandSet(dev,RT5633_SDP_CTRL,  0x0008);
    // IIS interface WM_MASTER_MODE | WM_I2S_MODE);   //Audio inteface Data length and Data formate,24Bit
    Codec5633CommandSet(dev,RT5633_STEREO_AD_DA_CLK_CTRL, 0x1A00); //cjh       0x0000 // IIS 16bit BCLK = LRCK * 32
#else
    //codec is slave
    Codec5633CommandSet(dev,RT5633_SDP_CTRL,  0x0000 | SDP_MODE_SEL_SLAVE); // IIS interface WM_MASTER_MODE | WM_I2S_MODE);   //Audio inteface Data length and Data formate,16Bit
    //Codec5633CommandSet(dev,RT5633_SDP_CTRL,  0x0000);
    // IIS interface WM_MASTER_MODE | WM_I2S_MODE);   //Audio inteface Data length and Data formate,16Bit
    Codec5633CommandSet(dev,RT5633_STEREO_AD_DA_CLK_CTRL,  0x1000);         // IIS 16bit BCLK = LRCK * 32
#endif

    Codec5633CommandSet(dev,RT5633_GBL_CLK_CTRL,SYSCLK_IS_PLL);             // sysclk is pll
    Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x4d4c);                        //44.1kHz

    ret = Codec5633CommandRead(dev, RT5633_HP_OUT_VOL);
    ret = Codec5633CommandRead(dev, RT5633_HP_OUT_VOL);
    rk_printf("RT5633_HP_OUT_VOL = %x\n", ret);
    ///CJH

    Codec5633CommandSet(dev,RT5633_SPK_OUT_VOL, 0x8080);	//un-mute speaker volume
    Codec5633CommandSet(dev,RT5633_HP_OUT_VOL, 0x8080);	//un-mute hp volume
    // HP det
    Codec5633CommandSet(dev,RT5633_DEPOP_CTRL_1,PW_SOFT_GEN|EN_DEPOP_2|EN_SOFT_FOR_S_M_DEPOP|EN_HP_R_M_UM_DEPOP|EN_HP_L_M_UM_DEPOP);
    Codec5633CommandSet(dev,RT5633_DEPOP_CTRL_2, 0x8000);
    // DigitalBeep Gen
    Codec5633CommandSet(dev,RT5633_DIG_BEEP_IRQ_CTRL, 0xF407);
    // ALC Function Control EN
    Codec5633CommandSet(dev,RT5633_ALC_CTRL_3, 0x60C0);
    // Spatial EffectControl EN
    Codec5633CommandSet(dev,RT5633_PSEUDO_SPATL_CTRL, 0xF443);
    // EQ EN
    Codec5633CommandSet(dev,RT5633_EQ_CTRL_1, 0x0100);


}




/*
--------------------------------------------------------------------------------
  Function name : void ALC5633CodecSetMode(CodecMode_en_t Codecmode)
  Author        :
  Description   :

  Input         :

  Return        : ?

  History:     <author>         <time>         <version>
             yangwenjie     2008-11-20         Ver1.0
  desc:         ORG
  ??:      if exit from application, like FM or MIC , please set codec to standby mode
--------------------------------------------------------------------------------
*/
_DRIVER_I2S_I2SDEVICE_SHELL_
SHELL FUN void ALC5633CodecSetMode(ALC5633_DEVICE_CLASS *dev, CodecMode_en_t Codecmode)
{
    switch (Codecmode)
    {
        case Codec_DACout:
        {
             Codec5633CommandSet(dev, RT5633_PWR_MANAG_ADD3, PWR_MAIN_BIAS|PWR_VREF);
             Codec5633CommandSet(dev, RT5633_PWR_MANAG_ADD4, PWR_HP_L_VOL|PWR_HP_R_VOL);
             Codec5633CommandSet(dev, RT5633_PWR_MANAG_ADD3, PWR_HP_AMP);
             Codec5633CommandSet(dev, RT5633_DEPOP_CTRL_1,PW_SOFT_GEN|EN_DEPOP_2);

             //Codec5633CommandSet(RT5633_PWR_MANAG_ADD3, PWR_HP_DIS_DEPOP|PWR_HP_AMP_DRI);
             Codec5633CommandSet(dev, RT5633_DEPOP_CTRL_1, 0);
        }
        break;
#if 0
        case Codec_FMin:
        {
            Codec5633CommandSet(dev, WM8987_R34, LI2LO | LO0DB);
            Codec5633CommandSet(dev, WM8987_R37, RI2RO | RO0DB);

            Codec5633CommandSet(dev, WM8987_R25, WM_VMID50k | WM_VREF);
            Codec5633CommandSet(dev, WM8987_R26, WM_LOUT2 | WM_ROUT2 | WM_OUT3);
        }
        break;

        case Codec_FMADC:
        {
            Codec5633CommandSet(dev, WM8987_R0,  0x003f);    //Left channel PGA  0dB
            Codec5633CommandSet(dev, WM8987_R1,  0x013f);    //Left channel PGA   0dB

            Codec5633CommandSet(dev, WM8987_R21, 0x00c3);    //Left digital ADC 0dB
            Codec5633CommandSet(dev, WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec5633CommandSet(dev, WM8987_R31, 0x0000);

            Codec5633CommandSet(dev, WM8987_R32, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB
            Codec5633CommandSet(dev, WM8987_R33, 0x0000);    //Left ADC select = Linput3 MICBoost = 0dB

            Codec5633CommandSet(dev, WM8987_R34, LI2LO | LO0DB);
            Codec5633CommandSet(dev, WM8987_R37, RI2RO | RO0DB);

            Codec5633CommandSet(dev, WM8987_R25, WM_VMID50k | WM_VREF | WM_ADCL | WM_ADCR);
            Codec5633CommandSet(dev, WM8987_R26, WM_LOUT2 | WM_ROUT2 | WM_OUT3);
        }
        break;

        case Codec_LineADC:
        {
            Codec5633CommandSet(dev, WM8987_R0,  0x0057);    //Left channel PGA  0dB
            Codec5633CommandSet(dev, WM8987_R1,  0x0157);    //Left channel PGA   0dB

            Codec5633CommandSet(dev, WM8987_R21, 0x00c3);    //Left digital ADC 0dB
            Codec5633CommandSet(dev, WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec5633CommandSet(dev, WM8987_R31, 0x0000);

            Codec5633CommandSet(dev, WM8987_R32, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB
            Codec5633CommandSet(dev, WM8987_R33, 0x0000);    //Left ADC select = Linput0 MICBoost = 0dB

            Codec5633CommandSet(dev, WM8987_R34, LO0DB);
            Codec5633CommandSet(dev, WM8987_R37, RO0DB);

            Codec5633CommandSet(dev, WM8987_R25, WM_VMID50k | WM_VREF | WM_ADCL | WM_ADCR);
            Codec5633CommandSet(dev, WM8987_R26, 0x0000);
        }
        break;

        case Codec_MICAdc:
        {
            Codec5633CommandSet(dev, WM8987_R0,  0x0117);    //Left channel PGA  0dB
            Codec5633CommandSet(dev, WM8987_R1,  0x0117);    //Left channel PGA   0dB

            Codec5633CommandSet(dev, WM8987_R21, 0x01c3);    //Left digital ADC 0dB
            Codec5633CommandSet(dev, WM8987_R22, 0x01c3);    //Right  digital ADC 0dB

            Codec5633CommandSet(dev, WM8987_R10, 0x01ff);    //Left digital DAC 0dB
            Codec5633CommandSet(dev, WM8987_R11, 0x01ff);    //Right  digital DAC 0dB

            Codec5633CommandSet(dev, WM8987_R31, 0x0040);    // adc use mono mix, select left adc

            Codec5633CommandSet(dev, WM8987_R32, 0x00a0);    //Left ADC select = Linput3 MICBoost = 20dB
            Codec5633CommandSet(dev, WM8987_R33, 0x0080);    //Left ADC select = Linput3 MICBoost = 00dB

            Codec5633CommandSet(dev, WM8987_R25, WM_VMID50k | WM_VREF | WM_AINL | WM_ADCL | WM_MICB);   //Left ADC and Left PGA power on,Right ADC and Right PGA power off
            Codec5633CommandSet(dev, WM8987_R26, 0x00);
        }
        break;

        case Codec_Standby:
        {
            Codec_DACMute();
            Codec5633CommandSet(dev, WM8987_R25, WM_VMID50k | WM_VREF);
            Codec5633CommandSet(dev, WM8987_R26, 0x00);
        }
        break;
#endif

    }

}


void ALC5633Codec_DeInitial(ALC5633_DEVICE_CLASS *dev)
{
    //Codec5633CommandSet_Mask(RT5633_SPK_OUT_VOL, 0x8080, 0x8080);	//mute speaker volume
    //Codec5633CommandSet_Mask(RT5633_HP_OUT_VOL, 0x8080, 0x8080);	//mute hp volume
    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD1, 0x0000);
    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD2, 0x0000);
    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD3, 0x0000);
    Codec5633CommandSet(dev,RT5633_PWR_MANAG_ADD4, 0x0000);
}

void ALC5633CodecSetSampleRate(ALC5633_DEVICE_CLASS *dev,CodecFS_en_t CodecFS)
{
    Codec5633CommandSet(dev,RT5633_SDP_CTRL,  0x0000);
    // IIS interface WM_MASTER_MODE | WM_I2S_MODE);   //Audio inteface Data length and Data formate,16Bit
    Codec5633CommandSet(dev,RT5633_STEREO_AD_DA_CLK_CTRL,  0x1000);         // IIS 16bit BCLK = LRCK * 32
    switch (CodecFS)
    {
        case CodecFS_8000Hz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x0040);      //12MHZ,8KHZ
            break;
        case CodecFS_11025Hz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x123f);      //12MHZ,11.025K
            break;
        case CodecFS_12KHz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x0020);      //12MHZ,12K
            break;
        case CodecFS_16KHz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x276d);      //12MHZ,16K
            break;
        case CodecFS_22050Hz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x2e4f);      //12MHZ,22.05K
            break;
        case CodecFS_24KHz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x294c);      //12MHZ,24K
            break;
        case CodecFS_32KHz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x456b);      //12MHZ,32K
            break;
        case CodecFS_48KHz:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x291c);      //12MHZ,48K
            break;
        case CodecFS_44100Hz:
        default:
            Codec5633CommandSet(dev,RT5633_PLL_CTRL,0x4d4c);      //12MHZ,44.1KHZ
            break;
    }
}

void ALC5633CodecSetVolumet(HDC dev,unsigned int VolumeL,unsigned int VolumeR)
{

    UINT32 Vol_L,Vol_R ;


    Vol_L = (VolumeL << 8);
    Vol_R = (VolumeR << 0);

    Codec5633CommandSet(dev,RT5633_HP_OUT_VOL, Vol_L |Vol_R|HP_L_VOL_SEL_HPMIX_L|HP_R_VOL_SEL_HPMIX_R);    // set output 1 volume

}
void ALC5633CodecExitMode(void)
{

}

void ALC5633CodecSetDACUnMute(HDC dev)
{
    Codec5633CommandSet(dev,RT5633_DAC_CTRL,DAC_L_UN_MUTE|DAC_R_UN_MUTE);       // DAC soft Unmute
    //Grf->MPMU_DAC_CON = ((MPMU_DAC_MUTE_MASK) << 16) | (MPMU_DAC_MUTE);
    //Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_1200MV_MASK) << 16) | MPMU_HP_FORCE_1200MV_ACTIVE;
}

void ALC5633CodecDACMute(HDC dev)
{
    Codec5633CommandSet(dev,RT5633_DAC_CTRL,DAC_L_MUTE|DAC_R_MUTE);       // DAC soft mute
    //Grf->MPMU_DAC_CON = ((MPMU_DAC_MUTE_MASK) << 16) | (MPMU_DAC_MUTE);
    //Grf->MPMU_HP_CON = ((MPMU_HP_FORCE_1200MV_MASK) << 16) | MPMU_HP_FORCE_1200MV_ACTIVE;
}
rk_err_t ALC5633DevConfigI2S(HDC dev,I2S_DEVICE_CONFIG_REQ_ARG *DevReqArg)
{
    int32  ret = RK_SUCCESS;
    ALC5633_DEVICE_CLASS * pALC5633Dev =  (ALC5633_DEVICE_CLASS *)dev;

    HDC hI2S = pALC5633Dev->hI2S;

    //ret = I2sDev_Control(hI2S, I2S_DEVICE_RST_CMD, DevReqArg);
    ret = I2sDev_Control(hI2S, I2S_DEVICE_INIT_CMD, DevReqArg);
    if (RK_ERROR == ret)
    {
        ret = RK_ERROR;
        return ret;
    }
    return ret;
}
/*******************************************************************************
** Name: ALC5633DevControl
** Input:DEVICE_CLASS* dev, uint8  cmd, void *args
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:53:09
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_READ_
rk_err_t ALC5633Dev_Control(HDC dev, uint8  cmd, void *args)
{
    ALC5633_DEVICE_CLASS * pALC5633Dev =  (ALC5633_DEVICE_CLASS *)dev;
    ALC5633_DEVICE_CONFIG_REQ_ARG * pCmdReqArg;
    ALC5633_CODEC_CONFIG *VolumeTable;
    I2S_DEVICE_CONFIG_REQ_ARG *I2sDevReqArg;

    rkos_semaphore_take(pALC5633Dev->osALC5633ControlReqSem, MAX_DELAY);

    if (cmd == ALC5633_DEVICE_CONFIG_I2S_CMD)
    {
        I2sDevReqArg = (I2S_DEVICE_CONFIG_REQ_ARG *)args;
        //VolumeTable set
        //config i2s
        ALC5633DevConfigI2S(dev,I2sDevReqArg);

        rkos_semaphore_give(pALC5633Dev->osALC5633ControlReqSem);
        return RK_SUCCESS;
    }
    else if (cmd == ALC5633_DEVICE_SET_MODE_CMD)
    {
        pCmdReqArg = (ALC5633_DEVICE_CONFIG_REQ_ARG *)args;
        //codec mode set
        ALC5633CodecSetMode(pALC5633Dev,pCmdReqArg->Codecmode);
        rkos_semaphore_give(pALC5633Dev->osALC5633ControlReqSem);
        return RK_SUCCESS;
    }
    else if (cmd == ALC5633_DEVICE_SET_SAMPLERATE_CMD)
    {
        pCmdReqArg = (ALC5633_DEVICE_CONFIG_REQ_ARG *)args;
        //codec samplerate set
        ALC5633CodecSetSampleRate(pALC5633Dev,CodecFS_8000Hz);
        rkos_semaphore_give(pALC5633Dev->osALC5633ControlReqSem);
        return RK_SUCCESS;
    }
    else if (cmd == ALC5633_DEVICE_SET_VOLTABLE_CMD)
    {
        VolumeTable = (ALC5633_CODEC_CONFIG *)args;
        //if null,set default table
        if (VolumeTable != NULL)
        {
            pALC5633Dev->CodecVolumeTable = VolumeTable;
        }
        rkos_semaphore_give(pALC5633Dev->osALC5633ControlReqSem);
        return RK_SUCCESS;
    }
    else if (cmd == ALC5633_DEVICE_SET_VOLUME_CMD)
    {
        ALC5633CodecSetVolumet(dev,pALC5633Dev->CodecVolumeTable[*(uint32 *)args].DacAnaVol,
                               pALC5633Dev->CodecVolumeTable[*(uint32 *)args].DacDigVol);
        rkos_semaphore_give(pALC5633Dev->osALC5633ControlReqSem);
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }

}

/*******************************************************************************
** Name: ALC5633DevRead
** Input:DEVICE_CLASS* dev,uint32 pos, void* buffer, uint32 size,uint8 mode,pRx_indicate Rx_indicate
** Return: rk_size_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:48:03
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_READ_
rk_size_t ALC5633Dev_Read(HDC dev, void* buffer, uint32 size,uint8 mode)
{
    uint32 i;
    ALC5633_DEVICE_CLASS * pALC5633Dev =  (ALC5633_DEVICE_CLASS *)dev;
    uint32 realsize;
    int32  ret = RK_SUCCESS;

    //rkos_semaphore_take(pALC5633Dev->osALC5633ReadSem, MAX_DELAY);

    if (mode == SYNC_MODE)
    {
        ret = I2S_PIO_Read(I2S_CH1,(uint32*)buffer,size);
        //ret = I2sDev_Write(hI2S, (const void *)buffer, size, SYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            goto I2S_ERROR;
        }
        return size;
    }
    else if (mode == ASYNC_MODE)
    {
        ret = I2S_PIO_Read(I2S_CH1,(uint32*)buffer,size);
        //ret = I2sDev_Write(hI2S, (const void *)buffer, size, ASYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            return ret;
        }
        return size;
    }


    //rkos_semaphore_give(pALC5633Dev->osALC5633ReadSem);
    return size;
I2S_ERROR:

    return ret;
}

/*******************************************************************************
** Name: ALC5633DevWrite
** Input:DEVICE_CLASS* dev, uint32 pos, const void* buffer, uint32 size,uint8 mode,pTx_complete Tx_complete
** Return: rk_size_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:46:51
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_WRITE_
rk_err_t ALC5633Dev_Write(HDC dev,const void* buffer, uint32 size,uint8 mode)
{
    uint32 i;
    ALC5633_DEVICE_CLASS * pstALC5633Dev =  (ALC5633_DEVICE_CLASS *)dev;
    uint32 realsize;
    HDC hI2S = pstALC5633Dev->hI2S;
    int32  ret = RK_SUCCESS;


    if (mode == SYNC_MODE)
    {
        ret = I2S_PIO_Write(I2S_CH1,(uint32*)buffer,size);
        //ret = I2sDev_Write(hI2S, (const void *)buffer, size, SYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            goto I2S_ERROR;
        }
        return size;
    }
    else if (mode == ASYNC_MODE)
    {
        ret = I2S_PIO_Write(I2S_CH1,(uint32*)buffer,size);
        //ret = I2sDev_Write(hI2S, (const void *)buffer, size, ASYNC_MODE);
        if (ret != RK_SUCCESS)
        {
            return ret;
        }
        return size;
    }

    return size;
I2S_ERROR:

    return ret;
}


/*******************************************************************************
** Name: ALC5633DevCreate
** Input:void
** Return: DEVICE_CLASS *
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:40:31
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
HDC ALC5633Dev_Create(uint32 DevID, void *arg)
{
    DEVICE_CLASS* pstDev;
    ALC5633_DEVICE_CLASS * psALC5633Dev;
    ALC5633_DEV_ARG * pstALC5633Arg;
    I2C_DEVICE_CLASS *dev_ALC5633;

    if (arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    pstALC5633Arg = (ALC5633_DEV_ARG *)arg;
    //rkos_enter_critical();

    psALC5633Dev =  rkos_memory_malloc(sizeof(ALC5633_DEVICE_CLASS));
    if (psALC5633Dev == NULL)
    {
        return (HDC) RK_ERROR;
    }

    psALC5633Dev->osALC5633WriteSem       = rkos_semaphore_create(1,1);
    psALC5633Dev->osALC5633ControlReqSem     = rkos_semaphore_create(1,1);

    if ((      psALC5633Dev->osALC5633WriteSem
               && psALC5633Dev->osALC5633ControlReqSem) == 0)
    {
        rkos_semaphore_delete(psALC5633Dev->osALC5633WriteSem);
        rkos_semaphore_delete(psALC5633Dev->osALC5633ControlReqSem);

        rkos_memory_free(psALC5633Dev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)psALC5633Dev;

    pstDev->suspend = ALC5633DevSuspend;
    pstDev->resume  = ALC5633DevResume;

    psALC5633Dev->hI2S = pstALC5633Arg->hI2s;
    psALC5633Dev->hI2C = pstALC5633Arg->hI2c;

    psALC5633Dev->CodecVolumeTable = VOL_ALC5633;

    gpstALC5633DevISRHandler = psALC5633Dev;

    ALC5633DevInit(psALC5633Dev);

    rk_printf("I2S_SLAVE_MODE\n");
    ALC5633CodecSetSampleRate(psALC5633Dev,I2S_FS_44100Hz);
    //Codec5633_DACMute(psALC5633Dev);
    ALC5633CodecSetVolumet(psALC5633Dev,200,200);
//    I2SInit(I2S_CH1,I2S_SLAVE_MODE,I2S_EXT,I2S_FS_44100Hz,I2S_FORMAT,I2S_DATA_WIDTH16,I2S_NORMAL_MODE);

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
** Name: ALC5633DevResume
** Input:DEVICE_CLASS *
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:57:19
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
rk_err_t ALC5633DevResume(HDC dev)
{

}
/*******************************************************************************
** Name: ALC5633DevSuspend
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:56:05
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
rk_err_t ALC5633DevSuspend(HDC dev)
{

}

/*******************************************************************************
** Name: ALC5633DevDelete
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:41:18
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
rk_err_t ALC5633DevDelete(HDC dev)
{
    ALC5633_DEVICE_CLASS * pstALC5633DevHandler = (ALC5633_DEVICE_CLASS *) dev;

    gpstALC5633DevISRHandler = NULL;
    rkos_semaphore_delete( pstALC5633DevHandler->osALC5633WriteSem );
    rkos_semaphore_delete( pstALC5633DevHandler->osALC5633ControlReqSem );

    rkos_memory_free(dev);

    return RK_SUCCESS;
}



/*******************************************************************************
** Name: ALC5633DevDeInit
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 13:38:30
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
rk_err_t ALC5633DevDeInit(HDC dev)
{
    ALC5633_DEVICE_CLASS * pstALC5633Dev = (ALC5633_DEVICE_CLASS *) dev;


    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ALC5633DevInit
** Input:DEVICE_CLASS * dev
** Return: rk_err_t
** Owner:Aaron
** Date: 2014.2.17
** Time: 11:44:46
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_INIT_
rk_err_t ALC5633DevInit(ALC5633_DEVICE_CLASS *ALC5633Dev)
{
    //DEVICE_CLASS ALC5633DEVICE;
    //open I2S clk

    //ACodec_Soft_Reset();

    //Grf_I2S0_Sel(I2S_SEL_ACODEC);//I2S1_SEL//ALC5633Q

    //Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin0, IOMUX_GPIO1A0_I2S1A_CLK);
    //Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin2, IOMUX_GPIO1A2_I2S1A_SCLK);
    //Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin4, IOMUX_GPIO1A4_I2S1A_SDI);
    //Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin1, IOMUX_GPIO1A1_I2S1A_LRCK);
    //Grf_GpioMuxSet(GPIO_CH1, GPIOPortA_Pin3, IOMUX_GPIO1A3_I2S1A_SDO);


    //ScuClockGateCtr(CLOCK_GATE_GRF, 1);
    //ScuClockGateCtr(CLK_I2S_GATE, 1);//CLOCK_GATE_I2S

    //open rst uart ip
    //ScuSoftResetCtr(I2S1_SRST, 1);
    //DelayMs(1);
    //ScuSoftResetCtr(I2S1_SRST, 0);


    ALC5633CodecPowerOnInitial(ALC5633Dev);


    return RK_SUCCESS;
}

#ifdef _USE_SHELL_
_DRIVER_ALC5633_ALC5633DEVIE_SHELL_
static char* ShellALC5633Name[] =
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
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_SHELL_
rk_err_t ALC5633Shell(HDC dev,  uint8 * pstr)
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

    while (memcmp((uint8*)ShellALC5633Name[i],pItem,strlen(ShellALC5633Name[i])) != 0)   //compare the name
    {
        i++;
        if (*((uint8*)ShellALC5633Name[i]) == '\b')
            return RK_ERROR;
    }

    pItem += StrCnt;
    pItem++;                           //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00:  //pcb
            ret = ALC5633ShellPcb(dev,pItem);
            break;

        case 0x01:  //open
            ret = ALC5633ShellOpen(dev,pItem);
            break;

        case 0x02:  //close
            ret = ALC5633ShellClose(dev,pItem);
            break;

        case 0x03:  //suspend
            //ret = ALC5633ShellSuspend(dev,pItem);
            break;

        case 0x04:  //resume
            //ret = ALC5633ShellRsume(dev,pItem);

        case 0x05:  //read
            //ret = ALC5633ShellRead(dev,pItem);
            break;

        case 0x06:  //write
            //ret = ALC5633ShellWrite(dev,pItem);
            break;

        case 0x07:  //control
            //ret = ALC5633ShellControl(dev,pItem);
            break;

        case 0x08:  //help
#ifdef SHELL_HELP
            ret = ALC5633ShellHelp(dev,pItem);
#endif
            break;

        case 0x09:
            ret = ALC5633ShellTest(dev, pItem);
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
** Name: ALC5633ShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.11.29
** Time: 9:53:03
*******************************************************************************/
_DRIVER_ALC5633_ALC5633DEVIE_SHELL_
SHELL FUN rk_err_t ALC5633ShellTest(HDC dev, uint8 * pstr)
{
    HDC hALC5633;
    rk_err_t ret;
    uint32 i=6000,time = 0;
    //I2sFS_en_t I2S_TEST_FS;

    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
    ALC5633_DEVICE_CONFIG_REQ_ARG stArg;
    ALC5633_DEV_ARG stALC5633DevArg;
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("RockCodec.test : test RockCodec \r\n");
            return RK_SUCCESS;
        }
    }
#endif

    //SetI2SClkOutFreq(XIN12M);   //OUT 12M TO CODEC

    SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL); //OUT 12M TO CODEC
    ////////////////////////////Create I2S_CH1////////////////////
    ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2S RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }
    //SetI2SClkOutFreq(XIN12M);   //OUT 12M TO CODEC

    ////////////////////////////Create I2C_CH1////////////////////

    ret=RKDev_Create(DEV_CLASS_I2C, I2C_CH1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2C RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }
    //////////////////////////// ////////////////////
    stALC5633DevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH1, NOT_CARE);
    stALC5633DevArg.hI2c = RKDev_Open(DEV_CLASS_I2C, I2C_CH1, NOT_CARE);
    stALC5633DevArg.hDma = NULL;

    if ((stALC5633DevArg.hI2s == NULL) || (stALC5633DevArg.hI2s == (HDC)RK_ERROR)
            || (stALC5633DevArg.hI2s == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev,"ALC5633ShellOpen I2S open failure",34,SYNC_MODE,NULL);
        return RK_SUCCESS;
    }
    ////////////////////////////Create DEV_CLASS_ALC5633 I2S_CH1////////////////////
    RKDev_Create(DEV_CLASS_ALC5633, I2S_CH1, &stALC5633DevArg);
    hALC5633 = RKDev_Open(DEV_CLASS_ALC5633,I2S_CH1,NOT_CARE);
    if ((hALC5633 == NULL) || (hALC5633 == (HDC)RK_ERROR)
            || (hALC5633 == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev,"ALC5633 open failure",20,SYNC_MODE,NULL);
    }
    //ALC5633CodecSetSampleRate(&psALC5633Dev,I2S_FS_44100Hz);
    //I2SInit(I2S_CH1,I2S_SLAVE_MODE,I2S_EXT,I2S_FS_44100Hz,I2S_FORMAT,I2S_DATA_WIDTH16,I2S_NORMAL_MODE);


    stI2sDevArg.i2smode = I2S_SLAVE_MODE;
    stI2sDevArg.i2sCS = I2S_EXT;
    stI2sDevArg.I2S_FS = CodecFS_44100Hz;
    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;

    ret = ALC5633Dev_Control(hALC5633, ALC5633_DEVICE_CONFIG_I2S_CMD, &stI2sDevArg);

    I2SIntEnable(I2S_CH1,I2S_INT_TX);
    I2SStart(I2S_CH1,I2S_START_PIO_TX);

     //enable ADC power on
    ALC5633CodecSetMode(hALC5633,Codec_DACout);
    //ADC set
    //ALC5633CodecSetSampleRate(hALC5633,CodecFS_44100Hz);//Smaple rate PLL
    ALC5633CodecSetDACUnMute(Codec_DACout);
    //ALC5633CodecSetVolumet(hALC5633,200,200);

    while (1)
    {
        ret = ALC5633Dev_Write(hALC5633,(const void *)ALC5633outptr, ALC5633length, ASYNC_MODE);
        time++;
        if (time == 800)
        {
            UartDev_Write(dev, "test over",9,SYNC_MODE,NULL);
            break;
        }
    }
    return RK_SUCCESS;
}


//_DRIVER_ALC5633_ALC5633DEVIE_SHELL_
//SHELL FUN rk_err_t ALC5633ShellTest(HDC dev, uint8 * pstr)
//{
//    HDC hALC5633;
//    rk_err_t ret;
//    uint32 vol,time = 0;
//    I2S_DEVICE_CONFIG_REQ_ARG stI2sDevArg;
//    ALC5633_DEVICE_CONFIG_REQ_ARG stArg;
//    ALC5633_DEV_ARG stALC5633DevArg;
//
//#ifdef SHELL_HELP
//    pstr--;
//    if(pstr[0] == '.')
//    {
//        //list have sub cmd
//        pstr++;
//        if(StrCmpA(pstr, "help", 4) == 0)
//        {
//            rk_print_string("RockCodec.test : test RockCodec \r\n");
//            return RK_SUCCESS;
//        }
//    }
//#endif
//    //ALC5633ShellOpen();
//
//  ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH1, NULL);
//	if(ret != RK_SUCCESS)
//	{
//	    UartDev_Write(dev, "I2S RKDev_Create failure\n",25,SYNC_MODE,NULL);
//	}
//	ret=RKDev_Create(DEV_CLASS_I2C, I2C_CH1, NULL);
//	if(ret != RK_SUCCESS)
//    {
//		UartDev_Write(dev, "I2C RKDev_Create failure\n",25,SYNC_MODE,NULL);
//    }

//    stALC5633DevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH1, NOT_CARE);
//    stALC5633DevArg.hI2c = RKDev_Open(DEV_CLASS_I2C, I2C_CH1, NOT_CARE);
//    stALC5633DevArg.hDma = NULL;
//
//    if((stALC5633DevArg.hI2s == NULL) || (stALC5633DevArg.hI2s == (HDC)RK_ERROR)
//        || (stALC5633DevArg.hI2s == (HDC)RK_PARA_ERR))
//    {
//        UartDev_Write(dev,"ALC5633ShellOpen I2S open failure",34,SYNC_MODE,NULL);
//        return RK_SUCCESS;
//    }
//
//    RKDev_Create(DEV_CLASS_ALC5633, I2S_CH1, &stALC5633DevArg);
//    hALC5633 = RKDev_Open(DEV_CLASS_ALC5633,I2S_CH1,NOT_CARE);
//    if((hALC5633 == NULL) || (hALC5633 == (HDC)RK_ERROR)
//        || (hALC5633 == (HDC)RK_PARA_ERR))
//    {
//        UartDev_Write(dev,"ALC5633 open failure",20,SYNC_MODE,NULL);
//    }
//
//    stI2sDevArg.i2smode = I2S_MASTER_MODE;//I2S_SLAVE_MODE;I2S_MASTER_MODE
//    stI2sDevArg.i2sCS = I2S_EXT;
//    stI2sDevArg.I2S_FS = CodecFS_8000Hz;
//    stI2sDevArg.Data_width = I2S_DATA_WIDTH16;
//    stI2sDevArg.BUS_FORMAT = I2S_FORMAT;
//
//    ALC5633Dev_Control(hALC5633, ALC5633_DEVICE_CONFIG_I2S_CMD, &stI2sDevArg);
//
//    I2SStart(I2S_CH1,I2S_START_PIO_TX);
//    while(1)
//    {
//        ret = ALC5633Dev_Write(hALC5633,ALC5633outptr, ALC5633length, ASYNC_MODE);
//        time++;
//        if(time == 800)
//        {
//            UartDev_Write(dev, "test over",9,SYNC_MODE,NULL);
//            break;
//        }
//    }
//    return RK_SUCCESS;
//}


/*******************************************************************************
** Name: ALC5633ShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.5
** Time: 9:55:13
*******************************************************************************/
//_VER_ALC5633_ALC5633DEVICE_SHELL_
SHELL FUN rk_err_t ALC5633ShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if ( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("5633命令集提供了一系列的命令对ALC5633进行操作\r\n");
    rk_print_string("5633包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("open      打开ALC5633        \r\n");
    rk_print_string("close     关闭ALC5633命令    \r\n");
    //rk_print_string("suspend   挂起ALC5633命令    \r\n");
    //rk_print_string("resume    恢复ALC5633命令    \r\n");
    //rk_print_string("read      读取ALC5633命令    \r\n");
    //rk_print_string("write     写入ALC5633命令    \r\n");
    rk_print_string("help      显示ALC5633命令帮助信息  \r\n");
    rk_print_string("test      显示ALC5633命令帮助信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/

rk_err_t ALC5633ShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("5633.pcb : pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: EmmcDevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t ALC5633ShellOpen(HDC dev, uint8 * pstr)
{
    HDC hALC5633;
    ALC5633_DEVICE_CLASS * pDev;
    rk_err_t ret;
    ALC5633_DEV_ARG stALC5633DevArg;
//    I2S_DEV_ARG stALC5633DevI2sArg;
//    I2C_DEVICE_ARG stALC5633DevI2cArg;

#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("5633.open : alc5633 open cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2S RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }
    ret=RKDev_Create(DEV_CLASS_I2C, I2C_CH1, NULL);
    if (ret != RK_SUCCESS)
    {
        UartDev_Write(dev, "I2C RKDev_Create failure\n",25,SYNC_MODE,NULL);
    }

    stALC5633DevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH1, NOT_CARE);
    stALC5633DevArg.hI2c = RKDev_Open(DEV_CLASS_I2C, I2C_CH1, NOT_CARE);
    stALC5633DevArg.hDma = NULL;

    if ((stALC5633DevArg.hI2s == NULL) || (stALC5633DevArg.hI2s == (HDC)RK_ERROR)
            || (stALC5633DevArg.hI2s == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev,"ALC5633ShellOpen I2S open failure",34,SYNC_MODE,NULL);
        return RK_SUCCESS;
    }

    RKDev_Create(DEV_CLASS_ALC5633, I2S_CH1, &stALC5633DevArg);
    hALC5633 = RKDev_Open(DEV_CLASS_ALC5633,I2S_CH1,NOT_CARE);
    if ((hALC5633 == NULL) || (hALC5633 == (HDC)RK_ERROR)
            || (hALC5633 == (HDC)RK_PARA_ERR))
    {
        UartDev_Write(dev,"ALC5633 open failure",20,SYNC_MODE,NULL);
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: ALC5633ShellClose
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.2.24
** Time: 10:43:17
*******************************************************************************/
rk_err_t ALC5633ShellClose(HDC dev, uint8 * pstr)
{
    ALC5633DevDeInit(dev);
    ALC5633DevDelete(dev);

    return RK_SUCCESS;

}
#endif
#endif

