/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                        All rights reserved.
*
* FileName: Driver\RK61X\RK618Device.c
* Owner: aaron.sun
* Date: 2014.12.16
* Time: 16:32:48
* Desc: RK618 Device Driver
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    aaron.sun     2014.12.16     16:32:48   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_RK61X_RK618DEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "FreeRTOS.h"
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
#define RK618_DEV_NUM 2

typedef  struct _RK618_DEVICE_CLASS
{
	DEVICE_CLASS stRK618Device;
	xSemaphoreHandle osRK618ReadReqSem;
	xSemaphoreHandle osRK618ReadSem;
	xSemaphoreHandle osRK618WriteReqSem;
	xSemaphoreHandle osRK618WriteSem;
	xSemaphoreHandle osRK618OperReqSem;
	xSemaphoreHandle osRK618OperSem;

    HDC              hI2C;

}RK618_DEVICE_CLASS;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static RK618_DEVICE_CLASS * gpstRK618DevISR[RK618_DEV_NUM] = {(RK618_DEVICE_CLASS *)NULL,(RK618_DEVICE_CLASS *)NULL};



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
rk_err_t RK618DevShellHelp(HDC dev,uint8 * pstr);
rk_err_t RK618DevShellTest(HDC dev, uint8 * pstr);
rk_err_t RK618DevShellDel(HDC dev, uint8 * pstr);
rk_err_t RK618DevShellMc(HDC dev, uint8 * pstr);
rk_err_t RK618DevShellPcb(HDC dev, uint8 * pstr);
void RK618DevIntIsr1(void);
void RK618DevIntIsr0(void);
void RK618DevIntIsr(uint32 DevID);
rk_err_t RK618DevDeInit(RK618_DEVICE_CLASS * pstRK618Dev);
rk_err_t RK618DevInit(RK618_DEVICE_CLASS * pstRK618Dev);
rk_err_t RK618DevResume(HDC dev);
rk_err_t RK618DevSuspend(HDC dev);


#define  SPKOUT_VOLUME    24 //0~31
#define  HPOUT_VOLUME     24 //0~31

/* volume setting
 *  0: -18dB
 *  12: 0dB
 *  31: 28.5dB
 *  Step: 1.5dB
*/
#define   CAPTURE_VOL     24 //0-31

//sleep for MOSFET or SPK power amplifier chip
#define SPK_AMP_DELAY 150
#define HP_MOS_DELAY 50

//for route
#define RK618_CODEC_PLAYBACK	1
#define RK618_CODEC_CAPTURE	2
#define RK618_CODEC_INCALL	4
#define RK618_CODEC_ALL	(RK618_CODEC_PLAYBACK | RK618_CODEC_CAPTURE | RK618_CODEC_INCALL)

static
uint32 g_rk618_reg_cache[RK618_PGAR_AGC_CTL5 + 1 - RK618_CODEC_BASE] = {0};   //0x125

static uint32 rk618_reg_defaults[RK618_PGAR_AGC_CTL5 + 1 - RK618_CODEC_BASE] = {
	0x0003,  //RK618_RESET
	0x0046,  //RK618_DAC_VOL
	0x0050,  //RK618_ADC_INT_CTL1
	0x000e,  //RK618_ADC_INT_CTL2
	0x0050,  //RK618_DAC_INT_CTL1
	0x000e,  //RK618_DAC_INT_CTL2
	0x00,    //none 0x18
	0x0021,    //RK618_CLK_CHPUMP
	0x00,    //0x20
	0x00,    //0x24
	0x000c,   //RK618_PGA_AGC_CTL
	0x00,    //0x2c
	0x00,    //0x30
	0x00,    //0x34
	0x00,    //0x38
	0x007c,  //RK618_PWR_ADD1
	0x0099,   //RK618_BST_CTL
	0x0024,    //RK618_DIFFIN_CTL
	0x001f,    //RK618_MIXINL_CTL
	0x0024,   //RK618_MIXINL_VOL1
	0x0004,   //RK618_MIXINL_VOL2
	0x003f,   //RK618_MIXINR_CTL
	0x0024,   //RK618_MIXINR_VOL1
	0x0024,   //RK618_MIXINR_VOL2
	0x00cc,  //RK618_PGAL_CTL
	0x00cc,  //RK618_PGAR_CTL
	0x00ff,  //RK618_PWR_ADD2
	0x003f,   //RK618_DAC_CTL
	0x001f,   //RK618_LINEMIX_CTL
	0x003c,   //RK618_MUXHP_HPMIX_CTL
	0x00ff,   //RK618_HPMIX_CTL
	0x0000,    //RK618_HPMIX_VOL1
	0x0000,    //RK618_HPMIX_VOL2
	0x0060,  //RK618_LINEOUT1_CTL
	0x0060,  //RK618_LINEOUT2_CTL
	0x00e0,  //RK618_SPKL_CTL
	0x00e0,  //RK618_SPKR_CTL
	0x00e0,   //RK618_HPL_CTL
    0x00e0,   //RK618_HPR_CTL
	0x00ff,   //RK618_MICBIAS_CTL
	0x0028,    //RK618_MICKEY_DET_CTL
	0x000f,  //RK618_PWR_ADD3
	0x0036,   //RK618_ADC_CTL
	0x003f,   //RK618_SINGNAL_ZC_CTL1
	0x00ff,   //RK618_SINGNAL_ZC_CTL2
	0x00,     //0xb4
	0x00,     //0xb8
	0x00,     //0xbc
	0x0010, //RK618_PGAL_AGC_CTL1
	0x0025, //RK618_PGAL_AGC_CTL2
	0x0041, //RK618_PGAL_AGC_CTL3
	0x002c, //RK618_PGAL_AGC_CTL4
	0x0000,  //RK618_PGAL_ASR_CTL
	0x0026,    //RK618_PGAL_AGC_MAX_H
	0x0040,    //RK618_PGAL_AGC_MAX_L
	0x0036,    //RK618_PGAL_AGC_MIN_H
	0x0020,    //RK618_PGAL_AGC_MIN_L
	0x0038,    //RK618_PGAL_AGC_CTL5
	0x00,      //0xe8
	0x00,      //0xec
	0x00,      //0xf0
	0x00,      //0xf4
	0x00,      //0xf8
	0x00,      //0xfc
	0x0010,     //RK618_PGAR_AGC_CTL1
	0x0025,     //RK618_PGAR_AGC_CTL2
	0x0041,     //RK618_PGAR_AGC_CTL3
	0x002c,     //RK618_PGAR_AGC_CTL4
	0x0000,     //RK618_PGAR_ASR_CTL
	0x0026,    //RK618_PGAR_AGC_MAX_H
	0x0040,    //RK618_PGAR_AGC_MAX_L
	0x0036,    //RK618_PGAR_AGC_MIN_H
	0x0020,    //RK618_PGAR_AGC_MIN_L
	0x0038,     //RK618_PGAR_AGC_CTL5
};

/* mfd registers default list */
static struct rk618_reg_val_typ rk618_mfd_reg_defaults[] = {
	{CRU_CODEC_DIV, 0x00000000},
	{CRU_IO_CON0, (I2S1_OUT_DISABLE | I2S0_OUT_DISABLE | I2S1_PD_DISABLE | I2S0_PD_DISABLE) |
		((I2S1_OUT_DISABLE | I2S0_OUT_DISABLE | I2S1_PD_DISABLE | I2S0_PD_DISABLE) << 16)},
	{CRU_IO_CON1, (I2S1_SI_EN | I2S0_SI_EN) | ((I2S1_SI_EN | I2S0_SI_EN) << 16)},
	{CRU_PCM2IS2_CON2, (0) | ((PCM_TO_I2S_MUX | APS_SEL | APS_CLR | I2S_CHANNEL_SEL) << 16)},
	{CRU_CFGMISC_CON, 0x00000000},
};

/* mfd registers cache list */
static struct rk618_reg_val_typ rk618_mfd_reg_cache[] = {
	{CRU_CODEC_DIV, 0x00000000},
	{CRU_IO_CON0, (I2S1_OUT_DISABLE | I2S0_OUT_DISABLE | I2S1_PD_DISABLE | I2S0_PD_DISABLE) |
		((I2S1_OUT_DISABLE | I2S0_OUT_DISABLE | I2S1_PD_DISABLE | I2S0_PD_DISABLE) << 16)},
	{CRU_IO_CON1, (I2S1_SI_EN | I2S0_SI_EN) | ((I2S1_SI_EN | I2S0_SI_EN) << 16)},
	{CRU_PCM2IS2_CON2, (0) | ((PCM_TO_I2S_MUX | APS_SEL | APS_CLR | I2S_CHANNEL_SEL) << 16)},
	{CRU_CFGMISC_CON, 0x00000000},
};
#define RK618_MFD_REG_LEN   ARRAY_SIZE(rk618_mfd_reg_cache)

static bool Is_rk618_mfd_register(unsigned int reg)
{
	int i;

	for (i = 0; i < RK618_MFD_REG_LEN; i++) {
		if (rk618_mfd_reg_cache[i].reg == reg)
			return TRUE;
	}

	return FALSE;
}

/* If register's bit16-31 is mask bit add to this fun */
static bool Is_rk618_mfd_mask_register(unsigned int reg)
{
	switch (reg) {
	case CRU_IO_CON0:
	case CRU_IO_CON1:
	case CRU_PCM2IS2_CON2:
		return TRUE;
	default:
		return FALSE;
	}
}

static struct rk618_reg_val_typ rk618_mfd_codec_bit_list[] =
{
	{CRU_CFGMISC_CON, AD_DA_LOOP | MICDET2_PIN_F_CODEC | MICDET1_PIN_F_CODEC},
};
#define RK616_MFD_CODEC_BIT_LEN ARRAY_SIZE(rk618_mfd_codec_bit_list)

static int rk618_mfd_codec_bit(unsigned int reg)
{
	int i;

	for (i = 0; i < RK616_MFD_CODEC_BIT_LEN; i++) {
		if (rk618_mfd_codec_bit_list[i].reg == reg)
			return i;
	}

	return -1;
}

static struct rk618_init_bit_typ rk618_init_bit_list[] =
{
	{RK618_SPKL_CTL, RK618_MUTE, RK618_INIT_MASK},
	{RK618_SPKR_CTL, RK618_MUTE, RK618_INIT_MASK},
	{RK618_HPL_CTL, RK618_MUTE, RK618_INIT_MASK},
	{RK618_HPR_CTL, RK618_MUTE, RK618_INIT_MASK},
	{RK618_MUXHP_HPMIX_CTL, RK618_HML_PWRD, RK618_HML_INIT_MASK},
	{RK618_MUXHP_HPMIX_CTL, RK618_HMR_PWRD, RK618_HMR_INIT_MASK},
};

#define RK618_INIT_BIT_LIST_LEN ARRAY_SIZE(rk618_init_bit_list)

static int rk618_init_bit_register(unsigned int reg, int i)
{
	for (; i < RK618_INIT_BIT_LIST_LEN; i++) {
		if (rk618_init_bit_list[i].reg == reg)
			return i;
	}

	return -1;
}
static UINT32 Codec618CmdRead(HDC dev,UINT16 RegAddr);

static unsigned int rk618_set_init_value(HDC dev,unsigned int reg, unsigned int value)
{
	unsigned int read_value, power_bit, set_bit;
	int i;

	// read codec init register
	i = rk618_init_bit_register(reg, 0);

	// set codec init bit
	// widget init bit should be setted 0 after widget power up or unmute,
	// and should be setted 1 after widget power down or mute.
	if (i >= 0) {
		read_value = Codec618CmdRead(dev,reg);
		while (i >= 0) {
			power_bit = rk618_init_bit_list[i].power_bit;
			set_bit = rk618_init_bit_list[i].init_bit;

			if ((read_value & power_bit) != (value & power_bit))
				value = (value & ~set_bit) | ((value & power_bit) ?  set_bit : 0);

			i = rk618_init_bit_register(reg, ++i);
		}
	}

	return value;
}

static BOOL Is_rk618_volatile_register(unsigned int reg)
{
	switch (reg)
    {
	case RK618_RESET:
	case RK618_CLK_CHPUMP:
	case RK618_MICKEY_DET_CTL:
	case CRU_CFGMISC_CON:
		return TRUE;
	default:
		return FALSE;
	}
}

/*register is or not rk618 codec register */
static BOOL Is_rk618_codec_register(uint32 reg)
{
	switch (reg)
    {
	case RK618_RESET:
	case RK618_DAC_VOL:
	case RK618_ADC_INT_CTL1:
	case RK618_ADC_INT_CTL2:
	case RK618_DAC_INT_CTL1:
	case RK618_DAC_INT_CTL2:
	case RK618_CLK_CHPUMP:
	case RK618_PGA_AGC_CTL:
	case RK618_PWR_ADD1:
	case RK618_BST_CTL:
	case RK618_DIFFIN_CTL:
	case RK618_MIXINL_CTL:
	case RK618_MIXINL_VOL1:
	case RK618_MIXINL_VOL2:
	case RK618_MIXINR_CTL:
	case RK618_MIXINR_VOL1:
	case RK618_MIXINR_VOL2:
	case RK618_PGAL_CTL:
	case RK618_PGAR_CTL:
	case RK618_PWR_ADD2:
	case RK618_DAC_CTL:
	case RK618_LINEMIX_CTL:
	case RK618_MUXHP_HPMIX_CTL:
	case RK618_HPMIX_CTL:
	case RK618_HPMIX_VOL1:
	case RK618_HPMIX_VOL2:
	case RK618_LINEOUT1_CTL:
	case RK618_LINEOUT2_CTL:
	case RK618_SPKL_CTL:
	case RK618_SPKR_CTL:
	case RK618_HPL_CTL:
	case RK618_HPR_CTL:
	case RK618_MICBIAS_CTL:
	case RK618_MICKEY_DET_CTL:
	case RK618_PWR_ADD3:
	case RK618_ADC_CTL:
	case RK618_SINGNAL_ZC_CTL1:
	case RK618_SINGNAL_ZC_CTL2:
	case RK618_PGAL_AGC_CTL1:
	case RK618_PGAL_AGC_CTL2:
	case RK618_PGAL_AGC_CTL3:
	case RK618_PGAL_AGC_CTL4:
	case RK618_PGAL_ASR_CTL:
	case RK618_PGAL_AGC_MAX_H:
	case RK618_PGAL_AGC_MAX_L:
	case RK618_PGAL_AGC_MIN_H:
	case RK618_PGAL_AGC_MIN_L:
	case RK618_PGAL_AGC_CTL5:
	case RK618_PGAR_AGC_CTL1:
	case RK618_PGAR_AGC_CTL2:
	case RK618_PGAR_AGC_CTL3:
	case RK618_PGAR_AGC_CTL4:
	case RK618_PGAR_ASR_CTL:
	case RK618_PGAR_AGC_MAX_H:
	case RK618_PGAR_AGC_MAX_L:
	case RK618_PGAR_AGC_MIN_H:
	case RK618_PGAR_AGC_MIN_L:
	case RK618_PGAR_AGC_CTL5:
		return TRUE;
	default:
		return FALSE;
	}
}

#define  RK618_Codec_I2CAddr   (0x50)
#define  RK618_Codec_I2CSpeed    100

static uint32 rk618_read_reg_cache(unsigned int reg)
{
	int i;

	if (Is_rk618_codec_register(reg))
		return  g_rk618_reg_cache[reg - RK618_CODEC_BASE];

	if (Is_rk618_mfd_register(reg))
    {
		for (i = 0; i < RK618_MFD_REG_LEN; i++) {
			if (rk618_mfd_reg_cache[i].reg == reg)
				return rk618_mfd_reg_cache[i].value;
		}
	}

	printf("%s : reg error!\n", __func__);

	return -EINVAL;
}

static void rk618_write_reg_cache(unsigned int reg, unsigned int value)
{
	int i;

	if (Is_rk618_codec_register(reg))
    {
		g_rk618_reg_cache[reg - RK618_CODEC_BASE] = value;
		return;
	}

	if (Is_rk618_mfd_register(reg))
    {
		for (i = 0; i < RK618_MFD_REG_LEN; i++)
        {
			if (rk618_mfd_reg_cache[i].reg == reg)
            {
				rk618_mfd_reg_cache[i].value = value;
				return;
			}
		}
	}

	printf("%s : reg error!\n", __func__);
}


static UINT32 Codec618CmdRead(HDC dev,UINT16 RegAddr)
{
    UINT8 data[4];
    UINT16 Regdata;
    I2C_CMD_ARG stArg;
    RK618_DEVICE_CLASS * p618Dev =  (RK618_DEVICE_CLASS *)dev;

    HDC hI2c = p618Dev->hI2C;
    if(hI2c != NULL)
    {
        stArg.SlaveAddress  = RK618_Codec_I2CAddr;
        stArg.RegAddr       = RegAddr;
        stArg.RWmode        = NormalMode;
        stArg.addr_reg_fmt  = I2C_7BIT_ADDRESS_16BIT_REG;
        stArg.speed         = RK618_Codec_I2CSpeed;

        if(!Is_rk618_codec_register(RegAddr))
            return RK_PARA_ERR;

        I2cDev_ReadData(hI2c,data,4,&stArg);
    }

    Regdata = data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0];
    return Regdata;
}

static int Codec618CmdWrite(RK618_DEVICE_CLASS *dev,UINT16 RegAddr, UINT32 uData)
{
    uint8 cmd,data[4];
    I2C_CMD_ARG stArg;
    uint32 set_bit, read_value, new_value;
    int i;

    HDC hI2c = dev->hI2C;
    if(hI2c != NULL)
    {
        if (!Is_rk618_mfd_register(RegAddr) && !Is_rk618_codec_register(RegAddr))
        {
    		rk_printf("%s : reg error!\n", __func__);
    		return -EINVAL;
    	}

    	// set codec mask bit
    	i = rk618_mfd_codec_bit(RegAddr);
    	if (i >= 0)
        {
    		set_bit = rk618_mfd_codec_bit_list[i].value;
    		read_value = Codec618CmdRead(dev, RegAddr);
    		uData = (read_value & ~set_bit) | (uData & set_bit);
    	}
        else if (Is_rk618_mfd_mask_register(RegAddr))
        {
    		uData = ((0xffff0000 & rk618_read_reg_cache(RegAddr)) | (uData & 0x0000ffff));
    	}

        new_value = rk618_set_init_value(dev, RegAddr, uData);

//        data[0] = (0xff & uData);
//        data[1] = (0xff00 & uData) >> 8;
//        data[2] = (0xff0000 & uData)>>16;
//        data[3] = (0xff000000 & uData)>>24;
        MemCpy(data, (uint8 *)&uData, 4);

        stArg.SlaveAddress  = RK618_Codec_I2CAddr;
        stArg.RegAddr       = RegAddr;
        stArg.RWmode        = NormalMode;
        stArg.speed         = RK618_Codec_I2CSpeed;
        stArg.addr_reg_fmt  = I2C_7BIT_ADDRESS_16BIT_REG;

        I2cDev_SendData(hI2c,data,4,&stArg);

	    if (new_value != uData)
        {
            MemCpy(data, (uint8 *)&new_value, 4);
            I2cDev_SendData(hI2c,data,4,&stArg);
		    uData = new_value;
	    }

	    rk618_write_reg_cache(RegAddr, uData);
    }

    return 0;
}

static UINT32 Codec618CmdUpdataBits(HDC dev,UINT16 RegAddr,uint32 mask,uint32 value)
{
	int change;
	unsigned int old, newVal;
	int ret;

	ret = Codec618CmdRead(dev, RegAddr);
	if (ret < 0)
		return ret;

	old = ret;
	newVal = (old & ~mask) | value;
	change = (old != newVal);
	if (change)
    {
	    Codec618CmdWrite(dev, RegAddr, newVal);
	}

	return change;

}



static int Rk618CodecReset(RK618_DEVICE_CLASS *dev)
{
	int i;
	int change;
	unsigned int old, newVal;
	int ret;

	Codec618CmdWrite(dev, RK618_RESET, 0xfc);
	DelayUs(10);
	Codec618CmdWrite(dev, RK618_RESET, 0x43);
	DelayUs(10);

	for (i = 0; i < RK618_MFD_REG_LEN; i++)
		Codec618CmdWrite(dev, rk618_mfd_reg_defaults[i].reg,
			rk618_mfd_reg_defaults[i].value);

	memcpy(g_rk618_reg_cache, rk618_reg_defaults,
	       sizeof(rk618_reg_defaults));

	//close charge pump
	Codec618CmdWrite(dev, RK618_CLK_CHPUMP, 0x41);

	//bypass zero-crossing detection
	Codec618CmdWrite(dev, RK618_SINGNAL_ZC_CTL1, 0x3f);
	Codec618CmdWrite(dev, RK618_SINGNAL_ZC_CTL2, 0xff);

	//set ADC Power for MICBIAS
	//set ADC Power for MICBIAS
	Codec618CmdUpdataBits(dev, RK618_PWR_ADD1,
		RK618_ADC_PWRD, 0);

	return 0;
}


static struct rk618_reg_val_typ playback_power_up_list[] = {
	{0x804, 0x46}, //DAC DSM, 0x06: x1, 0x26: x1.25, 0x46: x1.5, 0x66: x1.75
	{0x868, 0x02}, //power up
	{0x86c, 0x0f}, //DACL/R UN INIT
	{0x86c, 0x00}, //DACL/R and DACL/R CLK power up
	{0x86c, 0x30}, //DACL/R INIT
	{0x874, 0x14}, //Mux HPMIXR from HPMIXR(bit 0), Mux HPMIXL from HPMIXL(bit 1),HPMIXL/R power up
	{0x878, 0xee}, //HPMIXL/HPMIXR from DACL/DACR(bit 4, bit 0)
#ifdef VIRTUAL_HPGND
	{0x898, 3<<5}, //power up HPOUTR (bit 7), volume (bit 0-4)
	{0x894, 3<<5}, //power up HPOUTL (bit 7), volume (bit 0-4)
	{0x898, 0x00}, //unmute HPOUTR (bit 5), volume (bit 0-4)
	{0x894, 0x00}, //unmute HPOUTL (bit 5), volume (bit 0-4)
#endif
	{0x88c, 3<<5|SPKOUT_VOLUME},//power up SPKOUTL (bit 7), volume (bit 0-4)
	{0x890, 3<<5|SPKOUT_VOLUME}, //power up SPKOUTR (bit 7), volume (bit 0-4)
	{0x88c, SPKOUT_VOLUME}, //unmute SPKOUTL (bit 5), volume (bit 0-4)
	{0x890, SPKOUT_VOLUME}, //unmute SPKOUTR (bit 5), volume (bit 0-4)
};
#define RK618_CODEC_PLAYBACK_POWER_UP_LIST_LEN ARRAY_SIZE(playback_power_up_list)

static struct rk618_reg_val_typ playback_power_down_list[] = {
	{0x898, 0xe0}, //mute HPOUTR (bit 5), volume (bit 0-4)
	{0x894, 0xe0}, //mute HPOUTL (bit 5), volume (bit 0-4)
	{0x890, 0xe0}, //mute SPKOUTR (bit 5), volume (bit 0-4)
	{0x88c, 0xe0}, //mute SPKOUTL (bit 5), volume (bit 0-4)
	{0x878, 0xff}, //HPMIXL/HPMIXR from DACL/DACR(bit 4, bit 0)
	{0x874, 0x3c}, //Power down HPMIXL/R
	{0x86c, 0x3f}, //DACL/R INIT
	{0x868, 0xff}, //power down
};
#define RK618_CODEC_PLAYBACK_POWER_DOWN_LIST_LEN ARRAY_SIZE(playback_power_down_list)

static struct rk618_reg_val_typ capture_power_up_list[] = {
	{0x848, 0x06}, //MIXINL power up and unmute, MININL from MICMUX, MICMUX from BST_L
	{0x84c, 0x3c}, //MIXINL from MIXMUX volume (bit 3-5)
	{0x860, CAPTURE_VOL}, //PGAL power up unmute,volume (bit 0-4)
	{0x828, 0x09}, //Set for Capture pop noise
	{0x83c, 0x00}, //power up
	{0x840, 0x69}, //BST_L power up, unmute, and Single-Ended(bit 6), volume 0-20dB(bit 5)
	{0x8a8, 0x09}, //ADCL/R power, and clear ADCL/R buf
	{0x8a8, 0x00}, //ADCL/R power, and clear ADCL/R buf
};
#define RK618_CODEC_CAPTURE_POWER_UP_LIST_LEN ARRAY_SIZE(capture_power_up_list)

static struct rk618_reg_val_typ capture_power_down_list[] = {
	{0x8a8, 0x3f}, //ADCL/R power down, and clear ADCL/R buf
	{0x860, 0xc0 | CAPTURE_VOL}, //PGAL power down ,mute,volume 0dB(bit 0-4)
	{0x84c, 0x3c}, //MIXINL from MIXMUX volume 0dB(bit 3-5)
	{0x848, 0x1f}, //MIXINL power down and mute, MININL No selecting, MICMUX from BST_L
	{0x840, 0x99}, //BST_L power down, mute, and Single-Ended(bit 6), volume 0(bit 5)
	{0x83c, 0x3c}, //power down
};
#define RK618_CODEC_CAPTURE_POWER_DOWN_LIST_LEN ARRAY_SIZE(capture_power_down_list)

static int Rk618CodecPowerUp(RK618_DEVICE_CLASS *dev,int type)
{
	int i;

	// mute output for pop noise
	if ((type & RK618_CODEC_PLAYBACK) ||
		(type & RK618_CODEC_INCALL))
	{
		Gpio_SetPinLevel(GPIO_CH0,/*spk_ctl_gpio*/0,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,/*hp_ctl_gpio*/0,GPIO_LOW);
	}

	if (type & RK618_CODEC_PLAYBACK)
    {
		for (i = 0; i < RK618_CODEC_PLAYBACK_POWER_UP_LIST_LEN; i++)
        {
			Codec618CmdWrite(dev, playback_power_up_list[i].reg,
				playback_power_up_list[i].value);
		}
//		#ifdef CONFIG_MACH_RK_FAC
//			rk618_codec_set_spk(!get_hdmi_state());
//		#else
//			codec_set_spk(!get_hdmi_state());
//		#endif
	}

	if (type & RK618_CODEC_CAPTURE) {
		for (i = 0; i < RK618_CODEC_CAPTURE_POWER_UP_LIST_LEN; i++) {
			Codec618CmdWrite(dev, capture_power_up_list[i].reg,
				capture_power_up_list[i].value);
		}
	}

	if (type & RK618_CODEC_INCALL)
    {
		Codec618CmdUpdataBits(dev, RK618_PGA_AGC_CTL,
			0x0f, 0x09); //set for capture pop noise

		Codec618CmdUpdataBits(dev, RK618_MIXINL_CTL,
			RK618_MIL_F_IN3L | RK618_MIL_MUTE | RK618_MIL_PWRD,
			0); //IN3L to MIXINL, unmute IN3L
		Codec618CmdUpdataBits(dev, RK618_PWR_ADD1,
			RK618_ADC_PWRD | RK618_DIFFIN_MIR_PGAR_RLPWRD |
			RK618_MIC1_MIC2_MIL_PGAL_RLPWRD |
			RK618_ADCL_RLPWRD | RK618_ADCR_RLPWRD, 0);
		Codec618CmdUpdataBits(dev, RK618_MIXINL_VOL2,
			RK618_MIL_F_IN3L_VOL_MASK, 0); //IN3L to MIXINL vol
		Codec618CmdUpdataBits(dev, RK618_PGAL_CTL,
			0xff, 0x15); //PU unmute PGAL,PGAL vol
		Codec618CmdUpdataBits(dev, RK618_HPMIX_CTL,
			RK618_HML_F_PGAL | RK618_HMR_F_PGAL, 0);
		//set min volume for incall voice volume setting
		Codec618CmdUpdataBits(dev, RK618_SPKL_CTL,
			RK618_VOL_MASK, 0); //, volume (bit 0-4)
		Codec618CmdUpdataBits(dev, RK618_SPKR_CTL,
			RK618_VOL_MASK, 0);
	}

	return 0;
}

static int Rk618CodecPowerDown(RK618_DEVICE_CLASS *dev,int type)
{
	int i;

	// mute output for pop noise
	if ((type & RK618_CODEC_PLAYBACK) ||
		(type & RK618_CODEC_INCALL))
    {
        Gpio_SetPinLevel(GPIO_CH0,/*spk_ctl_gpio*/0,GPIO_LOW);
        Gpio_SetPinLevel(GPIO_CH0,/*hp_ctl_gpio*/0,GPIO_LOW);
	}

	if (type & RK618_CODEC_CAPTURE) {
		for (i = 0; i < RK618_CODEC_CAPTURE_POWER_DOWN_LIST_LEN; i++) {
			Codec618CmdWrite(dev, capture_power_down_list[i].reg,
				capture_power_down_list[i].value);
		}
	}

	if (type & RK618_CODEC_PLAYBACK) {
		for (i = 0; i < RK618_CODEC_PLAYBACK_POWER_DOWN_LIST_LEN; i++) {
			Codec618CmdWrite(dev, playback_power_down_list[i].reg,
				playback_power_down_list[i].value);
		}
	}

	if (type & RK618_CODEC_INCALL) {
		//close incall route
		Codec618CmdUpdataBits(dev, RK618_HPMIX_CTL,
			RK618_HML_F_PGAL | RK618_HMR_F_PGAL,
			RK618_HML_F_PGAL | RK618_HMR_F_PGAL);
		Codec618CmdUpdataBits(dev, RK618_PGA_AGC_CTL,
			0x0f, 0x0c);
		Codec618CmdUpdataBits(dev, RK618_MIXINL_CTL,
			RK618_MIL_F_IN3L | RK618_MIL_MUTE | RK618_MIL_PWRD,
			RK618_MIL_F_IN3L | RK618_MIL_MUTE | RK618_MIL_PWRD);
		Codec618CmdUpdataBits(dev, RK618_MIXINL_VOL2,
			RK618_MIL_F_IN3L_VOL_MASK, 0);
		Codec618CmdUpdataBits(dev, RK618_PGAL_CTL,
			0xff, 0xd5);
	}

	return 0;
}

static int Rk618CodecSetBiasLevel(RK618_DEVICE_CLASS *dev,
				 enum bias_level level)
{
	switch (level)
    {
	case BIAS_ON:
		break;

	case BIAS_PREPARE:
			Codec618CmdUpdataBits(dev, RK618_MICBIAS_CTL,
				RK618_MICBIAS2_PWRD | RK618_MICBIAS2_V_MASK,
				RK618_MICBIAS2_V_1_7);
			DelayUs(100);
		break;

	case BIAS_STANDBY:
//		if (codec->dapm.bias_level == BIAS_OFF)
        {
			/* set power */
			Codec618CmdUpdataBits(dev, RK618_PWR_ADD1,
				RK618_ADC_PWRD | RK618_DIFFIN_MIR_PGAR_RLPWRD |
				RK618_MIC1_MIC2_MIL_PGAL_RLPWRD |
				RK618_ADCL_RLPWRD | RK618_ADCR_RLPWRD, 0);

			Codec618CmdUpdataBits(dev, RK618_PWR_ADD2,
				RK618_HPL_HPR_PWRD | RK618_DAC_PWRD |
				RK618_DACL_SPKL_RLPWRD | RK618_DACL_RLPWRD |
				RK618_DACR_SPKR_RLPWRD | RK618_DACR_RLPWRD |
				RK618_LM_LO_RLPWRD | RK618_HM_RLPWRD, 0);

			Codec618CmdUpdataBits(dev, RK618_PWR_ADD3,
				RK618_ADCL_ZO_PWRD | RK618_ADCR_ZO_PWRD |
				RK618_DACL_ZO_PWRD | RK618_DACR_ZO_PWRD,
				RK618_ADCL_ZO_PWRD | RK618_ADCR_ZO_PWRD |
				RK618_DACL_ZO_PWRD | RK618_DACR_ZO_PWRD );

			Codec618CmdUpdataBits(dev, RK618_MICBIAS_CTL,
				RK618_MICBIAS2_PWRD | RK618_MICBIAS2_V_MASK,
				RK618_MICBIAS2_V_1_7);
		}
		break;

	case BIAS_OFF:
		Codec618CmdWrite(dev, RK618_PWR_ADD1, (0x007c & ~RK618_ADC_PWRD));
		Codec618CmdWrite(dev, RK618_PWR_ADD2, 0x00ff);
		Codec618CmdWrite(dev, RK618_PWR_ADD3, 0x000f);
		Codec618CmdUpdataBits(dev, RK618_MICBIAS_CTL,
			RK618_MICBIAS1_PWRD,
			RK618_MICBIAS1_PWRD);
		break;
	}

	return 0;
}

static int Rk618CodecSetDaiSysClk(RK618_DEVICE_CLASS *dev,
				 int clk_id, unsigned int freq, int dir)
{
//	rk618->stereo_sysclk = freq;
//
//	//set I2S mclk for mipi
//	rk618_mclk_set_rate(rk618_mfd->mclk, freq);

	return 0;
}

static int Rk618CodecSetDaiFmt(RK618_DEVICE_CLASS *dev,
			      uint32 fmt)
{
	unsigned int adc_aif1 = 0, adc_aif2 = 0, dac_aif1 = 0, dac_aif2 = 0;

	switch (fmt & DAIFMT_MASTER_MASK) {
	case DAIFMT_CBS_CFS:
		adc_aif2 |= RK618_I2S_MODE_SLV;
		break;
	case DAIFMT_CBM_CFM:
		adc_aif2 |= RK618_I2S_MODE_MST;
		break;
	default:
		printf("%s : set master mask failed!\n", __func__);
		return -EINVAL;
	}

	switch (fmt & DAIFMT_FORMAT_MASK) {
	case DAIFMT_DSP_A:
		adc_aif1 |= RK618_ADC_DF_PCM;
		dac_aif1 |= RK618_DAC_DF_PCM;
		break;
	case DAIFMT_DSP_B:
		break;
	case DAIFMT_I2S:
		adc_aif1 |= RK618_ADC_DF_I2S;
		dac_aif1 |= RK618_DAC_DF_I2S;
		break;
	case DAIFMT_RIGHT_J:
		adc_aif1 |= RK618_ADC_DF_RJ;
		dac_aif1 |= RK618_DAC_DF_RJ;
		break;
	case DAIFMT_LEFT_J:
		adc_aif1 |= RK618_ADC_DF_LJ;
		dac_aif1 |= RK618_DAC_DF_LJ;
		break;
	default:
		printf("%s : set format failed!\n", __func__);
		return -EINVAL;
	}

	switch (fmt & DAIFMT_INV_MASK) {
	case DAIFMT_NB_NF:
		adc_aif1 |= RK618_ALRCK_POL_DIS;
		adc_aif2 |= RK618_ABCLK_POL_DIS;
		dac_aif1 |= RK618_DLRCK_POL_DIS;
		dac_aif2 |= RK618_DBCLK_POL_DIS;
		break;
	case DAIFMT_IB_IF:
		adc_aif1 |= RK618_ALRCK_POL_EN;
		adc_aif2 |= RK618_ABCLK_POL_EN;
		dac_aif1 |= RK618_DLRCK_POL_EN;
		dac_aif2 |= RK618_DBCLK_POL_EN;
		break;
	case DAIFMT_IB_NF:
		adc_aif1 |= RK618_ALRCK_POL_DIS;
		adc_aif2 |= RK618_ABCLK_POL_EN;
		dac_aif1 |= RK618_DLRCK_POL_DIS;
		dac_aif2 |= RK618_DBCLK_POL_EN;
		break;
	case DAIFMT_NB_IF:
		adc_aif1 |= RK618_ALRCK_POL_EN;
		adc_aif2 |= RK618_ABCLK_POL_DIS;
		dac_aif1 |= RK618_DLRCK_POL_EN;
		dac_aif2 |= RK618_DBCLK_POL_DIS;
		break;
	default:
		printf("%s : set dai format failed!\n", __func__);
		return -EINVAL;
	}

	Codec618CmdUpdataBits(dev, RK618_ADC_INT_CTL1,
			RK618_ALRCK_POL_MASK | RK618_ADC_DF_MASK, adc_aif1);
	Codec618CmdUpdataBits(dev, RK618_ADC_INT_CTL2,
			RK618_ABCLK_POL_MASK | RK618_I2S_MODE_MASK, adc_aif2);
	Codec618CmdUpdataBits(dev, RK618_DAC_INT_CTL1,
			RK618_DLRCK_POL_MASK | RK618_DAC_DF_MASK, dac_aif1);
	Codec618CmdUpdataBits(dev, RK618_DAC_INT_CTL2,
			RK618_DBCLK_POL_MASK, dac_aif2);

	return 0;
}

static int Rk618CodecDigitaMute(RK618_DEVICE_CLASS *dev, int mute)
{
	if (mute)
    {
        Gpio_SetPinLevel(GPIO_CH0,/*spk_ctl_gpio*/0,GPIO_LOW);

        Gpio_SetPinLevel(GPIO_CH0,/*hp_ctl_gpio*/0,GPIO_LOW);

        Gpio_SetPinLevel(GPIO_CH0,/*rcv_ctl_gpio*/0,GPIO_LOW);
	}
    else
	{
        Gpio_SetPinLevel(GPIO_CH0,/*spk_ctl_gpio*/0,GPIO_HIGH);

        Gpio_SetPinLevel(GPIO_CH0,/*hp_ctl_gpio*/0,GPIO_HIGH);

        Gpio_SetPinLevel(GPIO_CH0,/*rcv_ctl_gpio*/0,GPIO_HIGH);
	}

	return 0;
}

static int Rk618CodecSuspend(RK618_DEVICE_CLASS *dev)
{
	Rk618CodecSetBiasLevel(dev, BIAS_OFF);
	return 0;
}

static int Rk618CodecResume(RK618_DEVICE_CLASS *dev)
{
	Rk618CodecSetBiasLevel(dev, BIAS_STANDBY);
	return 0;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618Dev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON API rk_err_t RK618Dev_Read(HDC dev)
{
	//RK618Dev Read...
	RK618_DEVICE_CLASS * pstRK618Dev =  (RK618_DEVICE_CLASS *)dev;
	if(pstRK618Dev == NULL)
	{
		return RK_ERROR;
	}

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618DevIntIsr1
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON FUN void RK618DevIntIsr1(void)
{
	//Call total int service...
	RK618DevIntIsr(1);

}
/*******************************************************************************
** Name: RK618DevIntIsr0
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON FUN void RK618DevIntIsr0(void)
{
	//Call total int service...
	RK618DevIntIsr(0);

}
/*******************************************************************************
** Name: RK618DevIntIsr
** Input:uint32 DevID
** Return: void
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON FUN void RK618DevIntIsr(uint32 DevID)
{
	uint32 RK618DevIntType;


	//Get RK618Dev Int type...
	//RK618DevIntType = GetIntType();
	if (gpstRK618DevISR[DevID] != NULL)
	{
		//if (RK618DevIntType & INT_TYPE_MAP)
		{
			//write serice code...
		}

		//wirte other int service...
	}

}
/*******************************************************************************
** Name: RK618DevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON FUN rk_err_t RK618DevResume(HDC dev)
{
	//RK618Dev Resume...
	RK618_DEVICE_CLASS * pstRK618Dev =  (RK618_DEVICE_CLASS *)dev;
	if(pstRK618Dev == NULL)
	{
		return RK_ERROR;
	}

}
/*******************************************************************************
** Name: RK618DevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON FUN rk_err_t RK618DevSuspend(HDC dev)
{
	//RK618Dev Suspend...
	RK618_DEVICE_CLASS * pstRK618Dev =  (RK618_DEVICE_CLASS *)dev;
	if(pstRK618Dev == NULL)
	{
		return RK_ERROR;
	}

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618Dev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_COMMON_
COMMON API rk_err_t RK618Dev_Write(HDC dev)
{
	//RK618Dev Write...
	RK618_DEVICE_CLASS * pstRK618Dev =  (RK618_DEVICE_CLASS *)dev;
	if(pstRK618Dev == NULL)
	{
		return RK_ERROR;
	}

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618Dev_Delete
** Input:uint32 DevID
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_INIT_
INIT API rk_err_t RK618Dev_Delete(uint32 DevID, void *arg)
{
	//Check RK618Dev is not exist...
	if(gpstRK618DevISR[DevID] == NULL)
	{
		return RK_ERROR;
	}

	//RK618Dev deinit...
	RK618DevDeInit(gpstRK618DevISR[DevID]);

	//Free RK618Dev memory...
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618OperReqSem);
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618OperSem);
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618ReadReqSem);
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618ReadSem);
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618WriteReqSem);
	rkos_semaphore_delete(gpstRK618DevISR[DevID]->osRK618WriteSem);
	rkos_memory_free(gpstRK618DevISR[DevID]);

	//Delete RK618Dev...
	gpstRK618DevISR[DevID] = NULL;

	//Delete RK618Dev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
	FW_RemoveModule(MODULE_ID_RK618_DEV);
#endif

	return RK_SUCCESS;

}
/*******************************************************************************
** Name: RK618Dev_Create
** Input:uint32 DevID, void * arg
** Return: HDC
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_INIT_
INIT API HDC RK618Dev_Create(uint32 DevID, void * arg)
{
	RK618_DEV_ARG * pstRK618DevArg;
	DEVICE_CLASS* pstDev;
	RK618_DEVICE_CLASS * pstRK618Dev;

	if(arg == NULL)
	{
		return (HDC)RK_PARA_ERR;
	}

	// Create handler...
	pstRK618DevArg = (RK618_DEV_ARG *)arg;
	pstRK618Dev =  rkos_memory_malloc(sizeof(RK618_DEVICE_CLASS));
	memset(pstRK618Dev, 0, sizeof(RK618_DEVICE_CLASS));

	if (pstRK618Dev == NULL)
	{
		return NULL;
	}

	//init handler...
	pstRK618Dev->osRK618OperReqSem  = rkos_semaphore_create(1,1);
	pstRK618Dev->osRK618OperSem  = rkos_semaphore_create(1,0);

	pstRK618Dev->osRK618ReadReqSem  = rkos_semaphore_create(1,1);
	pstRK618Dev->osRK618ReadSem  = rkos_semaphore_create(1,0);

	pstRK618Dev->osRK618WriteReqSem  = rkos_semaphore_create(1,1);
	pstRK618Dev->osRK618WriteSem  = rkos_semaphore_create(1,0);
	if(((pstRK618Dev->osRK618OperReqSem) == 0)
		|| ((pstRK618Dev->osRK618OperSem) == 0)
		|| ((pstRK618Dev->osRK618ReadReqSem) == 0)
		|| ((pstRK618Dev->osRK618ReadSem) == 0)
		|| ((pstRK618Dev->osRK618WriteReqSem) == 0)
		|| ((pstRK618Dev->osRK618WriteSem) == 0))
	{
		rkos_semaphore_delete(pstRK618Dev->osRK618OperReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618OperSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618ReadReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618ReadSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618WriteReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618WriteSem);
		rkos_memory_free(pstRK618Dev);
		return (HDC) RK_ERROR;
	}
	pstDev = (DEVICE_CLASS *)pstRK618Dev;
	pstDev->suspend = RK618DevSuspend;
	pstDev->resume  = RK618DevResume;

	//init arg...
	//pstRK618Dev->usbmode = pstRK618DevArg->usbmode;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
	FW_ModuleOverlay(MODULE_ID_RK618_DEV, MODULE_OVERLAY_CODE);
#endif
	//device init...
	gpstRK618DevISR[DevID] = NULL;
	if(RK618DevInit(pstRK618Dev) != RK_SUCCESS)
	{
		rkos_semaphore_delete(pstRK618Dev->osRK618OperReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618OperSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618ReadReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618ReadSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618WriteReqSem);
		rkos_semaphore_delete(pstRK618Dev->osRK618WriteSem);
		rkos_memory_free(pstRK618Dev);
		return (HDC) RK_ERROR;
	}
	gpstRK618DevISR[DevID] = pstRK618Dev;
	return (HDC)pstDev;


}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618DevDeInit
** Input:RK618_DEVICE_CLASS * pstRK618Dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_INIT_
INIT FUN rk_err_t RK618DevDeInit(RK618_DEVICE_CLASS * pstRK618Dev)
{
	//If ip device first step unregister int
	//IntDisable(INT_ID17_UART);
	//IntPendingClear(INT_ID17_UART);
	//IntUnregister(INT_ID17_UART);

	//If ip device second step close clk
	//ScuClockGateCtr(CLOCK_GATE_UART,0);
	return RK_ERROR;

}
/*******************************************************************************
** Name: RK618DevInit
** Input:RK618_DEVICE_CLASS * pstRK618Dev
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_INIT_
INIT FUN rk_err_t RK618DevInit(RK618_DEVICE_CLASS * pstRK618Dev)
{
    Rk618CodecReset(pstRK618Dev);
	return RK_SUCCESS;
}


#ifdef _RK618_DEV_SHELL_
_DRIVER_RK61X_RK618DEVICE_SHELL_
static char* ShellRK618Name[] =
{
    "pcb",
    "mc",
    "del",
    "test",
    "help",
    "...",
    "\b",
};

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK618Dev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL API rk_err_t RK618Dev_Shell(HDC dev, uint8 * pstr)
{
	uint32 i = 0;
	uint8  *pItem;
	uint16 StrCnt = 0;
	rk_err_t   ret;
	uint8 Space;
	StrCnt = ShellItemExtract(pstr, &pItem, &Space);
	if (StrCnt == 0)
	{
		return RK_ERROR;
	}

	while (memcmp((uint8*)ShellRK618Name[i],pItem,StrLenA(ShellRK618Name[i])) != 0)
	{
		//compare the name
		i++;
		if (*((uint8*)ShellRK618Name[i]) == 'b')
			return RK_ERROR;
	}

	pItem += StrCnt;
	pItem++;
	switch (i)
	{
		case 0x00:
			ret = RK618DevShellPcb(dev,pItem);
			break;

		case 0x01:
			ret = RK618DevShellMc(dev,pItem);
			break;

		case 0x02:
			ret = RK618DevShellDel(dev,pItem);
			break;

		case 0x03:
			ret = RK618DevShellTest(dev,pItem);
			break;

		case 0x04:  //help
			ret = RK618DevShellHelp(dev,pItem);
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
** Name: RK618DevShellHelp
** Input:HDC dev,uint8 * pstr
** Return: rk
** Owner:chad.Ma
** Date: 2014.12.22
** Time: 16:32:14
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL FUN rk_err_t RK618DevShellHelp(HDC dev,uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("rk618命令集提供了一系列的命令对rk618进行操作\r\n");
    rk_print_string("rk618包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示rk618 pcb信息         \r\n");
    rk_print_string("mc        打开rk618        \r\n");
    rk_print_string("del       删除rk618命令    \r\n");
    rk_print_string("test      rk618测试命令    \r\n");
    rk_print_string("help      显示rk618命令帮助信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: RK618DevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL FUN rk_err_t RK618DevShellTest(HDC dev, uint8 * pstr)
{
	HDC hRK618Dev;
	uint32 DevID;

	//Get RK618Dev ID...
	if(StrCmpA(pstr, "0", 1) == 0)
	{
		DevID = 0;
	}
	else if(StrCmpA(pstr, "1", 1) == 0)
	{
		DevID = 1;
	}
	else
	{
		DevID = 0;
	}

	//Open RK618Dev...
	hRK618Dev = RKDev_Open(DEV_CLASS_RK618, 0, NOT_CARE);
	if((hRK618Dev == NULL) || (hRK618Dev == (HDC)RK_ERROR) || (hRK618Dev == (HDC)RK_PARA_ERR))
	{
		rk_print_string("RK618Dev open failure");
		 return RK_SUCCESS;
	}

	//do test....

	//close RK618Dev...
	RKDev_Close(hRK618Dev);
	return RK_SUCCESS;

}
/*******************************************************************************
** Name: RK618DevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL FUN rk_err_t RK618DevShellDel(HDC dev, uint8 * pstr)
{
	uint32 DevID;

	//Get RK618Dev ID...
	if(StrCmpA(pstr, "0", 1) == 0)
	{
		DevID = 0;
	}
	else if(StrCmpA(pstr, "1", 1) == 0)
	{
		DevID = 1;
	}
	else
	{
		DevID = 0;
	}
	if(RKDev_Delete(DEV_CLASS_RK618, DevID) != RK_SUCCESS)
	{
		rk_print_string("RK618Dev delete failure");
	}
	return RK_SUCCESS;

}
/*******************************************************************************
** Name: RK618DevShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL FUN rk_err_t RK618DevShellMc(HDC dev, uint8 * pstr)
{
	RK618_DEV_ARG stRK618DevArg;
	rk_err_t ret;
	uint32 DevID;

	if(StrCmpA(pstr, "0", 1) == 0)
	{
		DevID = 0;
	}
	else if(StrCmpA(pstr, "1", 1) == 0)
	{
		DevID = 1;
	}
	else
	{
		DevID = 0;
	}

	//Init RK618Dev arg...
	//stRK618DevArg.usbmode = USBOTG_MODE_DEVICE;

	//Create RK618Dev...
	ret = RKDev_Create(DEV_CLASS_RK618, DevID, &stRK618DevArg);
	if(ret != RK_SUCCESS)
	{
		rk_print_string("RK618Dev create failure");
	}
	return RK_SUCCESS;

}
/*******************************************************************************
** Name: RK618DevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2014.12.16
** Time: 17:00:38
*******************************************************************************/
_DRIVER_RK61X_RK618DEVICE_SHELL_
SHELL FUN rk_err_t RK618DevShellPcb(HDC dev, uint8 * pstr)
{
	HDC hRK618Dev;
	uint32 DevID;

	//Get RK618Dev ID...
	if(StrCmpA(pstr, "0", 1) == 0)
	{
		DevID = 0;
	}
	else if(StrCmpA(pstr, "1", 1) == 0)
	{
		DevID = 1;
	}
	else
	{
		DevID = 0;
	}

	//Display pcb...
	if(gpstRK618DevISR[DevID] != NULL)
	{
		rk_print_string("rn");
		rk_printf_no_time("RK618Dev ID = %d Pcb list as follow:", DevID);
		rk_printf_no_time("	&stRK618Device---0x%08x", &gpstRK618DevISR[DevID]->stRK618Device);
		rk_printf_no_time("	osRK618OperReqSem---0x%08x", gpstRK618DevISR[DevID]->osRK618OperReqSem);
		rk_printf_no_time("	osRK618OperSem---0x%08x", gpstRK618DevISR[DevID]->osRK618OperSem);
		rk_printf_no_time("	osRK618ReadReqSem---0x%08x", gpstRK618DevISR[DevID]->osRK618ReadReqSem);
		rk_printf_no_time("	osRK618ReadSem---0x%08x", gpstRK618DevISR[DevID]->osRK618ReadSem);
		rk_printf_no_time("	osRK618WriteReqSem---0x%08x", gpstRK618DevISR[DevID]->osRK618WriteReqSem);
		rk_printf_no_time("	osRK618WriteSem---0x%08x", gpstRK618DevISR[DevID]->osRK618WriteSem);
		//rk_printf_no_time("	usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
		//rk_printf_no_time("	usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
	}
	else
	{
		rk_print_string("rn");
		rk_printf_no_time("RK618Dev ID = %d not exit", DevID);
	}
	return RK_SUCCESS;

}



#endif
#endif

