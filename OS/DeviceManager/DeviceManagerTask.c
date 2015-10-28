/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: ..\OS\Plugin\DevicePlugin.c
* Owner: Aaron.sun
* Date: 2014.4.9
* Time: 8:56:53
* Desc: device manager task
* History:
*   <author>    <date>       <time>     <version>     <Desc>
* Aaron.sun     2014.4.9     8:56:53   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __OS_PLUGIN_DEVICEPLUGIN_C__

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
#include "FwAnalysis.h"



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

#define _OS_PLUGIN_DEVICEPLUGIN_COMMON_  __attribute__((section("os_plugin_deviceplugin_read")))
#define _OS_PLUGIN_DEVICEPLUGIN_WRITE_ __attribute__((section("os_plugin_deviceplugin_write")))
#define _OS_PLUGIN_DEVICEPLUGIN_INIT_  __attribute__((section("os_plugin_deviceplugin_init")))
#define _OS_PLUGIN_DEVICEPLUGIN_SHELL_  __attribute__((section("os_plugin_deviceplugin_shell")))

typedef struct _DEVICE_TASK_DATA_BLOCK
{
    HDC DeviceList[DEVICE_LIST_NUM + 10][5];
    pQueue  DeviceAskQueue;
    pQueue  DeviceRespQueue;
    HTC hDeviceTask;

}DEVICE_TASK_DATA_BLOCK;

typedef struct _DEVICE_ASK_QUEUE
{
    uint32 cmd;
    uint32 DeviceListID;

}DEVICE_ASK_QUEUE;

typedef struct _DEVICE_RESP_QUEUE
{
    uint32 cmd;
    uint32 DeviceListID;
    uint32 Status;
    uint32 Para[10];

}DEVICE_RESP_QUEUE;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static DEVICE_TASK_DATA_BLOCK *   gpstDeviceTaskDataBlock;


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
rk_err_t CreateDeviceList(uint32 DeviceListID);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: DeviceTask_CreateDeviceList
** Input:uint32 DeviceListID
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.9
** Time: 17:55:46
*******************************************************************************/
_OS_PLUGIN_DEVICEPLUGIN_COMMON_
COMMON API rk_err_t DeviceTask_CreateDeviceList(uint32 DeviceListID)
{
    DEVICE_ASK_QUEUE DeviceAskQueue;
    DEVICE_RESP_QUEUE DeviceRespQueue;
    DeviceAskQueue.cmd = DEVICE_CMD_CREATE_LIST;
    DeviceAskQueue.DeviceListID = DeviceListID;

    rkos_queue_send(gpstDeviceTaskDataBlock->DeviceAskQueue, &DeviceAskQueue, MAX_DELAY);
    rkos_queue_receive(gpstDeviceTaskDataBlock->DeviceRespQueue, &DeviceRespQueue, MAX_DELAY);
    if ((DeviceRespQueue.cmd == DEVICE_CMD_CREATE_LIST) && (DeviceAskQueue.DeviceListID == DeviceListID)
            && DeviceRespQueue.Status == RK_SUCCESS)
    {
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }

}

/*******************************************************************************
** Name: DeviceTask
** Input:void
** Return: void
** Owner:Aaron.sun
** Date: 2014.4.9
** Time: 10:11:27
*******************************************************************************/
_OS_PLUGIN_DEVICEPLUGIN_COMMON_
COMMON API void DeviceTask(void)
{
    DEVICE_ASK_QUEUE DeviceAskQueue;

    UART_DEV_ARG stUartArg;
    rk_err_t ret;

    stUartArg.dwBitWidth = UART_DATA_8B;
    stUartArg.dwBitRate = UART_BR_115200;
    stUartArg.Channel = UART_CHA;

    RKDev_Create(DEV_CLASS_UART, UART_DEV1, &stUartArg);

    UartHDC = RKDev_Open(DEV_CLASS_UART, UART_DEV1, NOT_CARE);


    #ifdef _EMMC_BOOT_
    CreateDeviceList(DEVICE_LIST_EMMC_CODE);
    #endif

    #ifdef _SPI_BOOT_
    CreateDeviceList(DEVICE_LIST_SPI_CODE);
    #endif

    FW_Resource_Init();

    #ifdef _EMMC_BOOT_
    CreateDeviceList(DEVICE_LIST_EMMC_USER1);
    #endif

    #ifdef _SPI_BOOT_
#if 1	
    CreateDeviceList(DEVICE_LIST_SPI_USER1);
#else if
//	CreateDeviceList(DEVICE_LIST_SPI_USER2);
#endif
    #endif

    CreateDeviceList(DEVICE_LIST_AUDIO);

    #ifdef _USE_SHELL_
    RKTaskCreate(TASK_ID_SHELL,0, NULL);
    #endif


    while (1)
    {
        rkos_queue_receive(gpstDeviceTaskDataBlock->DeviceAskQueue, &DeviceAskQueue, MAX_DELAY);

        switch (DeviceAskQueue.cmd)
        {
            case DEVICE_CMD_CREATE_LIST:
            {
                DEVICE_RESP_QUEUE DeviceRespQueue;
                DeviceRespQueue.cmd = DEVICE_CMD_CREATE_LIST;

                DeviceRespQueue.DeviceListID = DeviceAskQueue.DeviceListID;

                ret = CreateDeviceList(DeviceAskQueue.DeviceListID);
                if(ret == RK_SUCCESS)
                {
                    DeviceRespQueue.Status = RK_SUCCESS;
                    rkos_queue_send(gpstDeviceTaskDataBlock->DeviceRespQueue, &DeviceRespQueue, MAX_DELAY);
                }
                else
                {
                    DeviceRespQueue.Status = RK_ERROR;
                    rkos_queue_send(gpstDeviceTaskDataBlock->DeviceRespQueue, &DeviceRespQueue, MAX_DELAY);
                }
                break;
            }
            case DEVICE_CMD_DELETE_LIST:
                break;

            case DEVICE_CMD_RESUME_LIST:
                break;

            case DEVICE_CMD_SUSPEND_LIST:
                break;
        }
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
** Name: CreateDeviceList
** Input:uint32 DeviceListID
** Return: void
** Owner:Aaron.sun
** Date: 2014.4.9
** Time: 11:37:28
*******************************************************************************/
_OS_PLUGIN_DEVICEPLUGIN_COMMON_
COMMON FUN rk_err_t CreateDeviceList(uint32 DeviceListID)
{
    rk_err_t ret;

    switch (DeviceListID)
    {

        case DEVICE_LIST_EMMC_CODE:
            {

#ifdef _EMMC_BOOT_

                SDC_DEV_ARG stSdcArg;
                HDC hSdc,hStorage;
                EMMC_DEV_ARG stEmmcArg;
                LUN_DEV_ARG stLunArg;
                rk_err_t ret;

                rk_print_string("\r\ndma device init...");

                ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\ndma device create failure");
                    goto err;
                }


                rk_print_string("\r\nsdmmc device init...");

                stSdcArg.hDma = RKDev_Open(DEV_CLASS_DMA, 0, NOT_CARE);
                stSdcArg.Channel = EMMC_SDC_CHANNEL;
                if (stSdcArg.hDma <= 0)
                {
                    rk_print_string("\r\ndma device open failure");
                    goto err;
                }

                ret = RKDev_Create(DEV_CLASS_SDC, EMMC_SDC_DEV_ID, &stSdcArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\nsdc device device failure");
                    goto err;
                }

                rk_print_string("\r\nemmc device init...");

                hSdc = RKDev_Open(DEV_CLASS_SDC, 0, NOT_CARE);

                if ((hSdc == NULL) || (hSdc == (HDC)RK_ERROR) || (hSdc == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("\r\nsdc device open failure");
                    goto err;
                }


                stEmmcArg.BusWidth = EMMC_DEV0_BUS_WIDTH;
                stEmmcArg.hSdc = hSdc;


                ret = RKDev_Create(DEV_CLASS_EMMC, 0, &stEmmcArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\nemmc device create failure");
                    goto err;
                }


                rk_print_string("\r\nlun0 device init...");


                hStorage = RKDev_Open(DEV_CLASS_EMMC, 0, NOT_CARE);
                if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
                {

                    rk_print_string("\r\nemmc device open failure");
                    goto err;
                }

                ret = EmmcDev_SetArea(hStorage, EMMC_AREA_USER);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\nemmc device set user failure");
                    goto err;
                }

                stLunArg.dwStartLBA = 0;
                stLunArg.dwEndLBA = EMMC_LUN0_SIZE * 1024 * 2;
                stLunArg.hStorage = hStorage;
                stLunArg.pfStorgeRead = EmmcDev_Read;
                stLunArg.pfStorgeWrite = EmmcDev_Write;

                ret = RKDev_Create(DEV_CLASS_LUN, 0, &stLunArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\nlun0 device create failure");
                    goto err;
                }
#endif

                return RK_SUCCESS;
            }


        case DEVICE_LIST_EMMC_USER1:
            {

#ifdef _EMMC_BOOT_

                HDC hStorage, hLun, hPar, hFileDev;
                LUN_DEV_ARG stLunArg;
                DEVICE_CLASS * pDev;
                rk_err_t ret;
                uint32 StorageSise;
                PAR_DEV_ARG stCreateArg;
                FAT_DEV_ARG stCreateFatArg;
                VOLUME_INF stVolumeInf;

                StorageSise = 0;

                hStorage = RKDev_Open(DEV_CLASS_EMMC,0, NOT_CARE);
                if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("emmc open failure");
                    goto err;
                }


                ret = EmmcDev_GetAreaSize(hStorage, EMMC_AREA_USER, &StorageSise);

                if(ret != RK_SUCCESS)
                {
                    rk_print_string("emmc get user area size failure");
                    goto err;
                }


                stLunArg.dwStartLBA = (EMMC_LUN0_SIZE + EMMC_LUN1_SIZE) << 11;

                stLunArg.dwEndLBA = StorageSise - 1;
                stLunArg.hStorage = hStorage;
                stLunArg.pfStorgeRead = EmmcDev_Read;
                stLunArg.pfStorgeWrite = EmmcDev_Write;

                ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("LUN0 Create Failure");
                }


                hLun = RKDev_Open(DEV_CLASS_LUN, 4, NOT_CARE);

                if((hLun == NULL) || (hLun == (HDC)RK_ERROR) || (hLun == (HDC)RK_PARA_ERR))
                {
                   rk_print_string("\nlun0 open failure");
                   goto err;
                }

                stCreateArg.hLun = hLun;
                stCreateArg.ParStartLBA = 0;

                ret = LunDev_GetSize(hLun, &stCreateArg.ParTotalSec);

                if(ret != RK_SUCCESS)
                {
                   rk_print_string("\nlun0 get size failure");
                   goto err;
                }

                ret = RKDev_Create(DEV_CLASS_PAR, 0, &stCreateArg);

                if(ret != RK_SUCCESS)
                {
                   rk_print_string("\npar0 create failure");
                   goto err;
                }

                hPar = RKDev_Open(DEV_CLASS_PAR, 0, NOT_CARE);
                if ((hPar == NULL) || (hPar == (HDC)RK_ERROR) || (hPar == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("\npar0 open failure");
                    goto err;
                }

                stCreateFatArg.hPar = hPar;

                ret = RKDev_Create(DEV_CLASS_FAT, 0, &stCreateFatArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\nfat0 create failure");
                    goto err;
                }

                ret = RKDev_Create(DEV_CLASS_FILE, 0, NULL);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("file create failure");
                    goto err;
                }

                hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

                if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("file open failure");
                    goto err;
                }

                stVolumeInf.hVolume = RKDev_Open(DEV_CLASS_FAT, 0, NOT_CARE);

                stVolumeInf.VolumeID = 'C';

                stVolumeInf.VolumeType = VOLUME_TYPE_FAT32;

                ret = FileDev_AddVolume(hFileDev, &stVolumeInf);
                if (ret != RK_SUCCESS)
                {
                    rk_print_string("add volume failure");
                    goto err;
                }

                RKDev_Close(hFileDev);

#endif
                return RK_SUCCESS;
            }

        case DEVICE_LIST_SD:
            {
                return RK_SUCCESS;
            }

        case DEVICE_LIST_USBHOST:
            {
                return RK_SUCCESS;
            }

        case DEVICE_LIST_AUDIO:
            {

                I2S_DEV_ARG stI2Sarg;
                ROCKCODEC_DEV_ARG stRockCodecDevArg;
                AUDIO_DEV_ARG stAudioArg;
                BCORE_DEV_ARG stBcoreDevArg;
                HDC hCodec;

                stI2Sarg.hDma = RKDev_Open(DEV_CLASS_DMA, DMA_CHN0, NOT_CARE);
                if (stI2Sarg.hDma == NULL)
                {
                    rk_print_string("\nDMA RKDev_Open failure");
                    goto err;
                }

                stI2Sarg.i2s_SelectMod = I2S_SEL_ACODEC;
                //Create I2SgDev...
                ret=RKDev_Create(DEV_CLASS_I2S, I2S_CH0, &stI2Sarg);
                if (ret != RK_SUCCESS)
                {
                    rk_print_string("I2S RKDev_Create failure");
                    goto err;
                }


                stRockCodecDevArg.hI2s = RKDev_Open(DEV_CLASS_I2S, I2S_CH0, NOT_CARE);
                ret = RKDev_Create(DEV_CLASS_ROCKCODEC, 0,&stRockCodecDevArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("open failure");
                }

                //SetI2SFreq(I2S_CH0, I2S_XIN12M, NULL);//²âÊÔÄ¬ÈÏ12M

                hCodec = RKDev_Open(DEV_CLASS_ROCKCODEC,0,NOT_CARE);

                //hCodec = RKDev_Open(DEV_CLASS_WM8987, 0, NOT_CARE);

                if ((hCodec == NULL) || (hCodec == (HDC)RK_ERROR) || (hCodec == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("codec device open failure");

                    goto err;
                }

                stAudioArg.Bit = I2S_DATA_WIDTH24;
                stAudioArg.Channel = 2; //L/R
                stAudioArg.DevID = 0;
                stAudioArg.hCodec = hCodec;
                stAudioArg.SampleRate = CodecFS_48KHz;//CodecFS_44100Hz;//CodecFS_16KHz;
                stAudioArg.Vol = 25;
#ifdef _RK_EQ_
                stAudioArg.EQMode = EQ_NOR;
#endif
                ret = RKDev_Create(DEV_CLASS_AUDIO, 0, &stAudioArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("audio device create failure");
                    goto err;
                }

                rk_print_string("audio create ok");


                //Init BcoreDev arg...
                //stBcoreDevArg.usbmode = USBOTG_MODE_DEVICE;

                //Create BcoreDev...
                ret = RKDev_Create(DEV_CLASS_BCORE, 0, &stBcoreDevArg);
                if(ret != RK_SUCCESS)
                {
                    rk_print_string("BcoreDev create failure");
                }

                RKTaskCreate(TASK_ID_BCORE, 0, NULL);

                return RK_SUCCESS;
            }

        case DEVICE_LIST_SPI_CODE:
            {
#ifdef _SPI_BOOT_
#if 0
                SPI_DEV_ARG stSpiArg;
                HDC hSpi,hStorage;
                SPINOR_DEV_ARG stSpiNorArg;
                LUN_DEV_ARG stLunArg;
                rk_err_t ret;

                rk_print_string("\r\ndma device init...");

                ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }



                stSpiArg.Ch = 0;
                stSpiArg.CtrlMode = SPI_CTL_MODE;
                stSpiArg.SpiRate = SPI_BUS_CLK;


                rk_print_string("\r\nspi device init...");

                ret = RKDev_Create(DEV_CLASS_SPI, 0, &stSpiArg);
                if (ret != RK_SUCCESS)
                {
                    goto err;
                }

                rk_print_string("\r\nspinor device init...");

                hSpi = RKDev_Open(DEV_CLASS_SPI, 0, NOT_CARE);

                if ((hSpi == NULL) || (hSpi == (HDC)RK_ERROR) || (hSpi == (HDC)RK_PARA_ERR))
                {
                    goto err;
                }

                stSpiNorArg.hSpi = hSpi;
                stSpiNorArg.SpiChannel = 0;

                ret = RKDev_Create(DEV_CLASS_SPINOR, 0, &stSpiNorArg);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }


                rk_print_string("\r\nlun0 device init...");


                hStorage = RKDev_Open(DEV_CLASS_SPINOR,0, NOT_CARE);
                if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
                {
                    goto err;
                }

                ret = SpiNorDev_GetSize(hStorage, &stLunArg.dwEndLBA);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }

                stLunArg.dwStartLBA = 0;
                stLunArg.dwEndLBA--;
                stLunArg.hStorage = hStorage;
                stLunArg.pfStorgeRead = SpiNorDev_Read;
                stLunArg.pfStorgeWrite = SpiNorDev_Write;

                ret = RKDev_Create(DEV_CLASS_LUN, 0, &stLunArg);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }
#endif
                SPIFLASH_DEV_ARG stSpiArg;
                HDC hSpiFlash;
                LUN_DEV_ARG stLunArg;
                rk_err_t ret;

                ret = RKDev_Create(DEV_CLASS_DMA, 0, NULL);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\r\ndma device create failure");
                    goto err;
                }

                stSpiArg.spirate = SPI_BUS_CLK;

                rk_print_string("\r\nspi device init...");

                ret = RKDev_Create(DEV_CLASS_SPIFLASH, 0, &stSpiArg);
                if (ret != RK_SUCCESS)
                {
                    goto err;
                }

                rk_print_string("\r\nspinor device init...");

                hSpiFlash = RKDev_Open(DEV_CLASS_SPIFLASH, 0, NOT_CARE);

                if ((hSpiFlash == NULL) || (hSpiFlash == (HDC)RK_ERROR) || (hSpiFlash == (HDC)RK_PARA_ERR))
                {
                    goto err;
                }

                ret = SpiFlashDev_GetSize(hSpiFlash, &stLunArg.dwEndLBA);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }

                stLunArg.dwStartLBA = 0;
                stLunArg.dwEndLBA--;
                stLunArg.hStorage = hSpiFlash;
                stLunArg.pfStorgeRead = SpiFlashDev_Read;
                stLunArg.pfStorgeWrite = SpiFlashDev_Write;

                ret = RKDev_Create(DEV_CLASS_LUN, 0, &stLunArg);

                if (ret != RK_SUCCESS)
                {
                    goto err;
                }
#endif

                return RK_SUCCESS;
            }

       case DEVICE_LIST_SDIO_FIFO:
           {

#ifdef _DRIVER_AP6181_WIFI_C__
               SDC_DEV_ARG stSdcArg;
               HDC hSdc;
               SDIO_DEV_ARG stSdioArg;
               rk_err_t ret;

               FIFO_DEV_ARG stFifoArg;
               HDC hFileDev;
               FILE_ATTR stFileAttr;


               stSdcArg.hDma = RKDev_Open(DEV_CLASS_DMA, 0, NOT_CARE);
               stSdcArg.Channel = 0;
               if (stSdcArg.hDma <= 0)
               {
                   printf("\r\ndma device open failure\n");
                   goto err;
               }
               ret = RKDev_Create(DEV_CLASS_SDC, SDC1, &stSdcArg);

               if (ret != RK_SUCCESS)
               {
                   printf("\r\nsdc device device failure\n");
                   goto err;
               }

               hSdc = RKDev_Open(DEV_CLASS_SDC, SDC1, NOT_CARE);

               if ((hSdc == NULL) || (hSdc == (HDC)RK_ERROR) || (hSdc == (HDC)RK_PARA_ERR))
               {
                   printf("\r\nsdc device open failure\n");
                   goto err;
               }
               stSdioArg.hSdc = hSdc;

               ret = RKDev_Create(DEV_CLASS_SDIO, 0, &stSdioArg);

              if (ret != RK_SUCCESS)
              {
                  printf("\r\SDIO0 device create failure\n");
                  goto err;
              }
              printf("\r\SDIO0 device create success\n");

              #if 1
              hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

              if((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
              {
                  rk_print_string("File device open failure");
              }

              stFileAttr.Path = L"C:\\";
              stFileAttr.FileName = L"RkFifiDevice.buf";


              stFifoArg.hReadFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);
              if(stFifoArg.hReadFile <= 0)
              {
                  rk_print_string("file cache buffer open fail");
                  goto err;
              }

              stFifoArg.hWriteFile = FileDev_OpenFile(hFileDev, NULL, READ_WRITE, &stFileAttr);
              if(stFifoArg.hReadFile <= 0)
              {
                  rk_print_string("file cache buffer open fail");
                  goto err;
              }

              RKDev_Close(hFileDev);

              #endif

              stFifoArg.BlockCnt = 200000;
              //stFifoArg.BlockCnt = 20;
              stFifoArg.BlockSize = 1024;
              stFifoArg.UseFile = 1;

              ret = RKDev_Create(DEV_CLASS_FIFO, 0, &stFifoArg);

              if(ret != RK_SUCCESS)
              {
                  rk_print_string("fifo dev create failure");
                  goto err;
              }
              rk_print_string("fifo dev create success");

#endif
              return RK_SUCCESS;
        }
        case DEVICE_LIST_SPI_USER1:
            {

#ifdef _SPI_BOOT_

                HDC hStorage, hLun, hPar, hFileDev, hSpiFlash,hDma;
                SPIFLASH_DEV_ARG stSpiArg;
                LUN_DEV_ARG stLunArg;
                SDC_DEV_ARG stSdcArg;
                SD_DEV_ARG stSdDevArg;

                DEVICE_CLASS * pDev;
                rk_err_t ret;
                uint32 StorageSise;
                PAR_DEV_ARG stCreateArg;
                FAT_DEV_ARG stCreateFatArg;
                VOLUME_INF stVolumeInf;

                stSdcArg.Channel = 0;
                stSdcArg.hDma = RKDev_Open(DEV_CLASS_DMA, 0, NOT_CARE);
                if (stSdcArg.hDma <= 0)
                {
                    rk_printf("hDma open failure");
                    goto err;
                }

                rk_printf("\nwill create SDC \n");
                ret = RKDev_Create(DEV_CLASS_SDC, 0, &stSdcArg);
                rk_printf("\nwill RKDev_Create SDC over \n");
                if (ret != RK_SUCCESS)
                {
                    rk_printf("sdc0 open failure");
                    goto err;
                }
                rk_printf("\nwill RKDev_Open SDC \n");
                stSdDevArg.hSdc = RKDev_Open(DEV_CLASS_SDC, 0, NOT_CARE);
                if ((stSdDevArg.hSdc == NULL) || (stSdDevArg.hSdc == (HDC)RK_ERROR) || (stSdDevArg.hSdc == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("SDC0 open failure");
                    goto err;
                }
                stSdDevArg.BusWidth = BUS_WIDTH_4_BIT;

                //Create SdDev...
                rk_printf("\nwill create sd \n");
                ret = RKDev_Create(DEV_CLASS_SD, 0, &stSdDevArg);
                if (ret != RK_SUCCESS)
                {
                    rk_print_string("SdDev create failure");
                    goto err;
                }

#ifdef __DRIVER_SD_SDDEVICE_C__

                hStorage = RKDev_Open(DEV_CLASS_SD,0, NOT_CARE);
                if ((hStorage == NULL) || (hStorage == (HDC)RK_ERROR) || (hStorage == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("sd open failure");
                    goto err;
                }

                ret = SdDev_GetSize(hStorage, &stLunArg.dwEndLBA);				
                if (ret != RK_SUCCESS)
                {
                    rk_print_string("sd get size failure");
                    goto err;
                }
								rk_printf("sd Size %d \r\n",stLunArg.dwEndLBA);	

                stLunArg.dwStartLBA = 0;
                stLunArg.dwEndLBA--;
                stLunArg.hStorage = hStorage;
                stLunArg.pfStorgeRead = SdDev_Read;
                stLunArg.pfStorgeWrite = SdDev_Write;

                rk_printf("\nwill create LUN4 \n");
                ret = RKDev_Create(DEV_CLASS_LUN, 4, &stLunArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("LUN0 Create Failure");
                    goto err;
                }

#endif


                hLun = RKDev_Open(DEV_CLASS_LUN, 4, NOT_CARE);

                if((hLun == NULL) || (hLun == (HDC)RK_ERROR) || (hLun == (HDC)RK_PARA_ERR))
                {
                   rk_print_string("\nlun0 open failure");
                   goto err;
                }

                stCreateArg.hLun = hLun;
                stCreateArg.ParStartLBA = 0;

                ret = LunDev_GetSize(hLun, &stCreateArg.ParTotalSec);

                if(ret != RK_SUCCESS)
                {
                   rk_print_string("\nlun0 get size failure");
                   goto err;
                }
				rk_printf("lun Size %d \r\n",stCreateArg.ParTotalSec);	

                ret = RKDev_Create(DEV_CLASS_PAR, 0, &stCreateArg);

                if(ret != RK_SUCCESS)
                {
                   rk_print_string("\npar0 create failure");
                   goto err;
                }

                hPar = RKDev_Open(DEV_CLASS_PAR, 0, NOT_CARE);
                if ((hPar == NULL) || (hPar == (HDC)RK_ERROR) || (hPar == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("\npar0 open failure");
                    goto err;
                }

                stCreateFatArg.hPar = hPar;

                ret = RKDev_Create(DEV_CLASS_FAT, 0, &stCreateFatArg);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("\nfat0 create failure");
                    goto err;
                }

                ret = RKDev_Create(DEV_CLASS_FILE, 0, NULL);

                if (ret != RK_SUCCESS)
                {
                    rk_print_string("file create failure");
                    goto err;
                }

                hFileDev = RKDev_Open(DEV_CLASS_FILE, 0, READ_WRITE_CTRL);

                if ((hFileDev == NULL) || (hFileDev == (HDC)RK_ERROR) || (hFileDev == (HDC)RK_PARA_ERR))
                {
                    rk_print_string("file open failure");
                    goto err;
                }

                stVolumeInf.hVolume = RKDev_Open(DEV_CLASS_FAT, 0, NOT_CARE);

                stVolumeInf.VolumeID = 'C';

                stVolumeInf.VolumeType = VOLUME_TYPE_FAT32;

                ret = FileDev_AddVolume(hFileDev, &stVolumeInf);
                if (ret != RK_SUCCESS)
                {
                    rk_print_string("add volume failure");
                    goto err;
                }

                RKDev_Close(hFileDev);


#endif
                return RK_SUCCESS;
            }

        case DEVICE_LIST_SPI_USER2:
            {

#ifdef _SPI_BOOT_


				USBOTG_DEV_ARG stUsbOtgDevArg;
				USBMSC_DEV_ARG stUSBMSCDevArg;
				HDC hUsbOtgDev;

                DEVICE_CLASS * pDev;
                rk_err_t ret;



                rk_printf("\nwill create USBOTG \n");
				
				//Init UsbOtgDev arg...
				stUsbOtgDevArg.usbmode = USBOTG_MODE_HOST;
				
				//Create UsbOtgDev...
				ret = RKDev_Create(DEV_CLASS_USBOTG, 0, &stUsbOtgDevArg);
				if (ret != RK_SUCCESS)
				{
					rk_print_string("UsbOtgDev create failure");
					goto err;
				}


				hUsbOtgDev = RKDev_Open(DEV_CLASS_USBOTG, 0, NOT_CARE);
				if ((hUsbOtgDev == NULL) || (hUsbOtgDev == (HDC)RK_ERROR) || (hUsbOtgDev == (HDC)RK_PARA_ERR))
				{
					printf("UsbOtgDev open failure for Init USBMSC\n");
					goto err;
				}
				
			
				//Init USBMSCDev arg...
				stUSBMSCDevArg.hUsbOtgDev = hUsbOtgDev;
				
				//Create USBMSCDev...
				ret = RKDev_Create(DEV_CLASS_USBMSC, 0, &stUSBMSCDevArg);
				if (ret != RK_SUCCESS)
				{
					rk_print_string("USBMSCDev create failure");
					goto err;
				}
				
				//do test otgdev....
				//UsbOtgDev_Connect(hUsbOtgDev, 0);
				//do test otgHost...
				UsbOtgHost_Connect(hUsbOtgDev,0);


#endif
                return RK_SUCCESS;
            }
    }

err:

    return RK_ERROR;

}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



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
** Name: DeviceTaskDeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.9
** Time: 9:32:33
*******************************************************************************/
_OS_PLUGIN_DEVICEPLUGIN_INIT_
INIT API rk_err_t DeviceTaskDeInit(void *pvParameters)
{
    rkos_queue_delete(gpstDeviceTaskDataBlock->DeviceAskQueue);
    rkos_queue_delete(gpstDeviceTaskDataBlock->DeviceRespQueue);
}

/*******************************************************************************
** Name: DeviceTaskInit
** Input:void *
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.4.9
** Time: 9:18:26
*******************************************************************************/
_OS_PLUGIN_DEVICEPLUGIN_INIT_
INIT API rk_err_t DeviceTaskInit(void *pvParameters)
{
    RK_TASK_CLASS*   pDeviceTask = (RK_TASK_CLASS*)pvParameters;
    DEVICE_TASK_DATA_BLOCK*  pDeviceTaskDataBlock;
    uint32 i;

    if (pDeviceTask == NULL)
        return RK_PARA_ERR;

    pDeviceTaskDataBlock = rkos_memory_malloc(sizeof(DEVICE_TASK_DATA_BLOCK));
    memset(pDeviceTaskDataBlock, 0, sizeof(DEVICE_TASK_DATA_BLOCK));
    pDeviceTaskDataBlock->DeviceAskQueue = rkos_queue_create(1, sizeof(DEVICE_ASK_QUEUE));
    pDeviceTaskDataBlock->DeviceRespQueue = rkos_queue_create(1, sizeof(DEVICE_RESP_QUEUE));
    pDeviceTaskDataBlock->hDeviceTask = pDeviceTask->hTask;

    gpstDeviceTaskDataBlock = pDeviceTaskDataBlock;


    return RK_SUCCESS;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

