#include "BspConfig.h"
#ifdef __DRIVER_USBETH_USBETHDEVICE_C__

#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "stdint.h"


typedef    struct tagETH_HOST
{
    uint8       valid;
    uint8       IntIn;
    uint8       BulkIn;
    uint8       BulkOut;

    uint32      SendCtrlPipe;
    uint32      RecvCtrlPipe;
    uint32      RecvIntPipe;
    uint32      SendBulkPipe;
    uint32      RecvBulkPipe;

    HOST_DEV    *pDev;
}ETH_HOST, *pETH_HOST;



typedef  struct _USBETH_DEVICE_CLASS
{
    DEVICE_CLASS 		stUSBETHDevice;
    xSemaphoreHandle 	osUSBETHOperSem;
    HDC					hOtg;
    uint16_t			phy_id;
    uint8_t				link_up;
    HOST_INTERFACE   	*interfaceDes;
    uint8_t *			rx_data;
    uint8_t *			tx_data;
    ETH_HOST			gEthHost;

}USBETH_DEVICE_CLASS;

static USBETH_DEVICE_CLASS * gpstUSBETHDevISR[1]=
{
    (USBETH_DEVICE_CLASS *)NULL
};


_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_read_cmd(ETH_HOST *pEthHost, uint8_t cmd,
                                      uint16_t value,
                                      uint16_t index,
                                      uint16_t size,
                                      uint8_t *data)
{
    return  HostCtrlMsg(pEthHost->pDev, pEthHost->RecvCtrlPipe,
                        USB_D2H | USB_REQ_TYPE_VENDOR | USB_REQ_RECIPIENT_DEVICE,
                        cmd,
                        value,
                        index,
                        data,
                        size,
                        USB_CTRL_GET_TIMEOUT);

}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_write_cmd(ETH_HOST *pEthHost, uint8_t cmd,
                                       uint16_t value,
                                       uint16_t index,
                                       uint16_t size,
                                       uint8_t *data)
{
    return  HostCtrlMsg(pEthHost->pDev, pEthHost->SendCtrlPipe,
                        USB_H2D | USB_REQ_TYPE_VENDOR | USB_REQ_RECIPIENT_DEVICE,
                        cmd,
                        value,
                        index,
                        data,
                        size,
                        USB_CTRL_SET_TIMEOUT);

}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_init(ETH_HOST *pEthHost)
{
    int32 status;
    uint8_t buf[64];
    ///0
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SW_PHY_SELECT,0x0001,0,0,NULL);
    if (status < 0)
        return status;
    ///1
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SW_RESET,0x0020,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);
    ///2
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SW_RESET,0x0000,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);
    ///3
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SW_RESET,0x0028,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);
    ///4
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_WRITE_RX_CTL,0x0000,0,0,NULL);
    if (status < 0)
        return status;
    ///5
    status = Eth_ax8872b_read_cmd(pEthHost, AX_CMD_READ_PHY_ID,0,0,2,buf);
    if (status < 0)
        return status;
    gpstUSBETHDevISR[0]->phy_id = buf[1] & 0x1F;
    printf("phy_id:%x \r\n",gpstUSBETHDevISR[0]->phy_id);
    ///6
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SET_SW_MII,0,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);
    ///7
    *((unsigned short *)buf) = ADVERTISE_ALL;
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_WRITE_MII_REG,gpstUSBETHDevISR[0]->phy_id,MII_ADVERTISE,2,buf);
    if (status < 0)
        return status;
    ///8
    *((unsigned short *)buf) = BMCR_FULLDPLX | BMCR_SPEED100 | BMCR_ANENABLE | BMCR_ANRESTART;;
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_WRITE_MII_REG,gpstUSBETHDevISR[0]->phy_id,MII_BMCR,2,buf);
    if (status < 0)
        return status;
    ///9
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_SET_HW_MII,0,0,0,NULL);
    if (status < 0)
        return status;
    ///10
    status = Eth_ax8872b_write_cmd(pEthHost, AX_CMD_WRITE_MEDIUM_MODE,AX88772_MEDIUM_DEFAULT,0,0,NULL);
    if (status < 0)
        return status;
    ///11
    status = Eth_ax8872b_write_cmd(pEthHost, 0x2A,0x8000,0x8001,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);

    ///12
    printf("\r\n\r\nax8872b_init ok\r\n");
    return status;




}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_start(ETH_HOST *pEthHost)
{
    int32 status;
    uint8_t buff[6];
    //status = Eth_ax8872b_write_cmd(pEthHost,  AX_CMD_WRITE_RX_CTL,0x88,0,0,NULL);
    status = Eth_ax8872b_write_cmd(pEthHost,  AX_CMD_WRITE_RX_CTL,0x8A,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);

    status = Eth_ax8872b_read_cmd(pEthHost,  AX_CMD_READ_NODE_ID,0,0,6,buff);
    if (status < 0)
        return status;

    printf("ax8872b_start ok:%d\r\nmac:%02x %02x %02x %02x %02x %02x \r\nWaiting for Ethernet connection\r\n",
           status,buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
    return status;
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_stop(ETH_HOST *pEthHost)
{
    int32 status;
    status = Eth_ax8872b_write_cmd(pEthHost,  AX_CMD_WRITE_RX_CTL,0,0,0,NULL);
    if (status < 0)
        return status;
    rkos_delay(100);
    printf("ax8872b_stop:%d\r\n",status);
    return status;
}


_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_link(ETH_HOST *pEthHost)
{
    int32 status;
    uint8_t buf[4];
    unsigned short mr0;
    unsigned short mode;
    unsigned char link;
    status = Eth_ax8872b_write_cmd(pEthHost,AX_CMD_SET_SW_MII,0,0,0,NULL);
    if (status < 0)
        return status;

    status = Eth_ax8872b_read_cmd(pEthHost,AX_CMD_READ_MII_REG,gpstUSBETHDevISR[0]->phy_id,MII_BMSR,2,buf);
    if (status < 0)
        return status;

    link = (*((unsigned short *)buf)) & BMSR_LSTATUS ? 1 : 0;

    if (link)
    {
        gpstUSBETHDevISR[0]->link_up = 1;
        status = Eth_ax8872b_read_cmd(pEthHost,AX_CMD_READ_MII_REG,gpstUSBETHDevISR[0]->phy_id,MII_BMCR,2,buf);
        if (status < 0)
            return status;

        mr0 = *((unsigned short *)buf);
        mode = AX88772_MEDIUM_DEFAULT;
        if (!(mr0 & BMCR_FULLDPLX))
        {
            mode &= ~AX88772_MEDIUM_FULL_DUPLEX;
            printf("\r\nduplex half");
        }
        else
            printf("\r\nduplex full");
        if (!(mr0 & BMCR_SPEED100))
        {
            mode &= ~AX88772_MEDIUM_100MB;
            printf(" speed 10\n\r");
        }
        else
            printf(" speed 100\n\r");

        status = Eth_ax8872b_write_cmd(pEthHost,AX_CMD_WRITE_MEDIUM_MODE,mode,0,0,NULL);
        if (status < 0)
            return status;
    }

    status = Eth_ax8872b_write_cmd(pEthHost,AX_CMD_SET_HW_MII, 0, 0, 0, NULL);
    if (status < 0)
        return status;

    return status;
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_recv(ETH_HOST *pEthHost)
{

    return HostBulkMsg(pEthHost->pDev, pEthHost->RecvBulkPipe,
                       gpstUSBETHDevISR[0]->rx_data, 4096, USB_CNTL_TIMEOUT*5);
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_send(ETH_HOST *pEthHost,uint8_t *data,uint32_t len)
{

    return HostBulkMsg(pEthHost->pDev, pEthHost->SendBulkPipe,
                       data, len, USB_CNTL_TIMEOUT*5);
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 Eth_ax8872b_ext_send(uint8_t *data,uint32_t len)
{
    ETH_HOST *pEthHost = &(gpstUSBETHDevISR[0]->gEthHost);
    return HostBulkMsg(pEthHost->pDev, pEthHost->SendBulkPipe,
                       data, len, USB_CNTL_TIMEOUT*5);
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API uint8_t* Eth_ax8872b_get_send_buf(void)
{
    return gpstUSBETHDevISR[0]->tx_data;
}


_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 EthHostGetPipe(ETH_HOST *pEthHost, HOST_INTERFACE* intf)
{
    int32 i, ret = -HOST_ERR;
    USB_ENDPOINT_DESCRIPTOR *ep = &intf->EpDesc[0];
    uint8 epnum = intf->IfDesc.bNumEndpoints;

    for (i=0; i<epnum; i++, ep++)
    {
        if (USBEpType(ep) == USB_ENDPOINT_XFER_INT)
        {
            if (USBEpDirIn(ep))
            {
                if (!pEthHost->IntIn)
                    pEthHost->IntIn = USBEpNum(ep);
            }
        }
        else if (USBEpType(ep) == USB_ENDPOINT_XFER_BULK)
        {
            if (USBEpDirIn(ep))
            {
                if (!pEthHost->BulkIn)
                    pEthHost->BulkIn = USBEpNum(ep);
            }
            else
            {
                if (!pEthHost->BulkOut)
                    pEthHost->BulkOut = USBEpNum(ep);
            }
        }
    }

    if (pEthHost->IntIn && pEthHost->BulkOut && pEthHost->BulkIn)
    {
        pEthHost->SendCtrlPipe = HostCreatePipe(intf->pDev, 0, 0);
        pEthHost->RecvCtrlPipe = HostCreatePipe(intf->pDev, 0, 1);
        pEthHost->RecvIntPipe =  HostCreatePipe(intf->pDev, pEthHost->IntIn, 1);
        pEthHost->SendBulkPipe = HostCreatePipe(intf->pDev, pEthHost->BulkOut, 0);
        pEthHost->RecvBulkPipe = HostCreatePipe(intf->pDev, pEthHost->BulkIn, 1);
        pEthHost->pDev = intf->pDev;
        ret = HOST_OK;
    }

    return ret;
}


_DRIVER_USBETH_USBETHDEVICE_COMMON_
int32 EthHostProbe(HOST_INTERFACE* intf)
{
    int32 i, j, ret;
    ETH_HOST *pEthHost = &(gpstUSBETHDevISR[0]->gEthHost);
    uint8_t buffer[64];
    memset (pEthHost, 0, sizeof(ETH_HOST));

    ret =  EthHostGetPipe(pEthHost, intf);
    if (ret < 0)
        return ret;
    printf("Eth_ax8872b_init:\r\n");
    ret = Eth_ax8872b_init(pEthHost);
    if (ret < 0)
        return ret;
    printf("Eth_ax8872b_init ok:");


    printf("Eth_ax8872b_start:\r\n");
    ret = Eth_ax8872b_start(pEthHost);
    if (ret < 0)
        return ret;
    printf("Eth_ax8872b_start ok:");

}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
const uint8_t arp_request_data[255]=
{

    0xff,0xff,0xff,0xff,0xff,0xff,	//dest addr
    0x00,0x60,0x6e,0x00,0x2c,0x31,	//src addr
    0x80,0x60, //???
    0x00,0x01,
    0x80,0x00,
    0x06,0x04,
    0x00,0x01,
    0x00,0x60,0x6e,0x00,0x2c,0x31,
    0xa8,0xa8,0x64,0xe6,
    0x00,0x00,0x00,0x00,0x00,0x00,
    0xa8,0xa8,0x64,0xc8,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,
};

#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API void USBETHDev_Task_Enter(void)
{
    int32 ret;
    uint32_t i;
    //uint8_t i[4] = 0;
    struct pbuf *p = NULL ,*q;
    unsigned char *rx_buf;
    unsigned short header1, header2;
    uint16_t length,pos;
    ETH_HOST *pEthHost = &(gpstUSBETHDevISR[0]->gEthHost);
    rk_printf("USBETHDev_Task_Enter...\r\n");
    rkos_semaphore_take(gpstUSBETHDevISR[0]->osUSBETHOperSem, MAX_DELAY);
    EthHostProbe(gpstUSBETHDevISR[0]->interfaceDes);

    //lwip
    FW_ModuleOverlay(MODULE_ID_AP6181_WIFI, MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_LWIP, MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_AP6181_INIT,MODULE_OVERLAY_CODE);
    wiced_network_init();
    RKTaskCreate(TASK_ID_ETH, 0, NULL);
    wiced_network_up(3, 0, NULL);
    //httpd_init();
    websocket_init();
    while (1)
    {
        //rk_printf("wiced_network_up\r\n");
        rkos_delay(1000);
    }

#if 0
    while (1)
    {


        if (gpstUSBETHDevISR[0]->link_up == 0)
        {
            rkos_delay(100);
            Eth_ax8872b_link(pEthHost);
            rk_printf(".");
        }
        else
        {
            //rk_printf("eth_task_delay\r\n");
            ret = Eth_ax8872b_recv(pEthHost);

            if (ret < 0)
            {

            }
            else
            {


                rx_buf = gpstUSBETHDevISR[0]->rx_data;
                while (1)
                {
                    header1 = *((unsigned short *)rx_buf);
                    header2 = *((unsigned short *)(rx_buf + 2));
                    rx_buf += 4;

                    if ((header1 & 0x7FF) != (~header2 & 0x7FF))
                        break;

                    length = header1 & 0x7FF;
                    if ((length > 1514) || (length < 60))
                        break;


                    p = pbuf_alloc(PBUF_RAW, length, PBUF_POOL_RX);
                    if (p != NULL)
                    {
                        pos = 0;
                        for (q = p; q != NULL; q = q->next)
                        {
                            memcpy(q->payload,rx_buf + pos, q->len);
                            pos += q->len;
                        }
                    }
                    else
                    {
                        break;
                    }
                    host_network_process_ethernet_data( p, 3 );

                    rx_buf = rx_buf + ((length + 1) & 0xFFFE);
                }




                /*
                				rk_printf("Eth_ax8872b_recv:%d\r\n",ret);
                				for(i=0;i<12;i++)
                				{
                					printf("%02x ",gpstUSBETHDevISR[0]->rx_data[i]);
                				}
                				printf("\r\n");
                */
            }


            /*
            			{
            				uint32_t packet_len;
            				uint8_t *msg = gpstUSBETHDevISR[0]->tx_data;
            				packet_len = (((length) ^ 0x0000ffff) << 16) + (length);
            				memcpy(msg, &packet_len, sizeof(packet_len));
            				memcpy(msg + sizeof(packet_len), arp_request_data, length);
            				ret = Eth_ax8872b_send(pEthHost,msg,length + sizeof(packet_len));
            				//rk_printf("Eth_ax8872b_send:%d\r\n",ret);
            				//rkos_delay(1);
            				length ++;
            				if(length >1536)
            					length = 60;

            			}
            */

        }

    }
#endif

}



_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON FUN rk_err_t USBETHDevResume(HDC dev)
{
    //USBMSCDev Resume...
    USBETH_DEVICE_CLASS * pstUSBETHDev =  (USBETH_DEVICE_CLASS *)dev;
    if (pstUSBETHDev == NULL)
    {
        return RK_ERROR;
    }

}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON FUN rk_err_t USBETHDevSuspend(HDC dev)
{
    //USBMSCDev Suspend...
    USBETH_DEVICE_CLASS * pstUSBETHDev =  (USBETH_DEVICE_CLASS *)dev;
    if (pstUSBETHDev == NULL)
    {
        return RK_ERROR;
    }

}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API rk_err_t UsbEthDev_Connect(HDC dev, uint32 Speed)
{
    int ret;
    USBETH_DEVICE_CLASS * pstUsbEthDev = (USBETH_DEVICE_CLASS *)dev;
    FREQ_EnterModule(FREQ_MAX);

    ret = UsbOtgDev_Connect (dev,Speed);

    if (ret != RK_SUCCESS)
    {
        //rk_printf ("UsbOtg connect fail!\n");
        return RK_ERROR;
    }

    return RK_SUCCESS;
}


_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API int32 EthHost(HOST_INTERFACE* interfaceDes)
{

    int32 i, n, ret=0;
    printf ("USB_ETH:%d\n",interfaceDes->IfDesc.bInterfaceClass);
    gpstUSBETHDevISR[0]->interfaceDes = interfaceDes;
    rkos_semaphore_give(gpstUSBETHDevISR[0]->osUSBETHOperSem);

    return ret;
}


#ifdef __WICE_ETH_TRANS_C__
_DRIVER_USBETH_USBETHDEVICE_COMMON_
INIT API rk_err_t ethTask_DeInit(void *pvParameters)
{
    return RK_SUCCESS;
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
INIT API rk_err_t ethTask_Init(void *pvParameters, void *arg)
{
    return RK_SUCCESS;
}

_DRIVER_USBETH_USBETHDEVICE_COMMON_
COMMON API void ethTask_Enter(void)
{
    int32 ret;
    //uint32_t i;
    struct pbuf *p = NULL ,*q;
    unsigned char *rx_buf;
    unsigned short header1, header2;
    uint16_t length,pos;
    ETH_HOST *pEthHost = &(gpstUSBETHDevISR[0]->gEthHost);
    rk_printf("ethTask_Enter...\r\n");
    uint8_t *buf;
    uint8_t i;


    while (1)
    {


        if (gpstUSBETHDevISR[0]->link_up == 0)
        {
            rkos_delay(100);
            Eth_ax8872b_link(pEthHost);
            rk_printf(".");
        }
        else
        {
            //rk_printf("eth_task_delay\r\n");
            ret = Eth_ax8872b_recv(pEthHost);

            if (ret < 0)
            {

            }
            else
            {


                rx_buf = gpstUSBETHDevISR[0]->rx_data;
                while (1)
                {
                    header1 = *((unsigned short *)rx_buf);
                    header2 = *((unsigned short *)(rx_buf + 2));
                    rx_buf += 4;

                    if ((header1 & 0x7FF) != (~header2 & 0x7FF))
                        break;

                    length = header1 & 0x7FF;
                    if ((length > 1514) || (length < 60))
                        break;


                    p = pbuf_alloc(PBUF_RAW, length, PBUF_POOL_RX);
                    if (p != NULL)
                    {
                        pos = 0;
                        for (q = p; q != NULL; q = q->next)
                        {
                            memcpy(q->payload,rx_buf + pos, q->len);
                            pos += q->len;
                        }
                    }
                    else
                    {
                        break;
                    }
                    host_network_process_ethernet_data( p, 3 );
                    rx_buf = rx_buf + ((length + 1) & 0xFFFE);
#if 0
                    rk_printf("host_network_process_ethernet_data\r\n");
                    buf = p->payload;
                    for (i=0;i<16;i++)
                    {
                        printf("%02x ",buf[i]);
                    }
                    printf("\r\n");
#endif

                }

            }

        }

    }

}



#endif







_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT API rk_err_t USBETHDev_Task_DeInit(void *pvParameters)
{
    return RK_SUCCESS;
}

_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT API rk_err_t USBETHDev_Task_Init(void *pvParameters)
{
    //gpstUsbOtgDevISR[0]->pMscHost = HubHost;
    rk_err_t ret;
    USBOGT_DESCRIPTOR gto_desc;
    gto_desc.pMscHost = EthHost;
    ret = UsbOtgDev_SetDes(gpstUSBETHDevISR[0]->hOtg,&gto_desc);
    if (ret == RK_ERROR)
    {
        printf ("ETH Usb Otg Set Descriptor failure\n");
    }
    printf ("ETH Usb Otg Set Descriptor ok\n");
    return RK_SUCCESS;
}


_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT API rk_err_t USBETHDev_Delete(uint32 DevID, void * arg)
{
//#if 1
    DevID = 0;
//    USBHUB_DEV_ARG* pstUSBHUBDevArg = (USBHUB_DEV_ARG*)arg;
//    pstUSBHUBDevArg->hUsbOtgDev = gpstUSBHUBDevISR[DevID]->hOtg;

    //Check USBMSCDev is not exist...
    printf ("\nUSB ETH Delete \n");
    if (gpstUSBETHDevISR[DevID] == NULL)
    {
        printf ("Delete error\n");
        return RK_ERROR;
    }
    //USBMSCDev deinit...
    USBETHDevDeInit(gpstUSBETHDevISR[DevID]);

    rkos_semaphore_delete(gpstUSBETHDevISR[DevID]->osUSBETHOperSem);
    rkos_memory_free(gpstUSBETHDevISR[DevID]->rx_data);
    rkos_memory_free(gpstUSBETHDevISR[DevID]->tx_data);
    rkos_memory_free(gpstUSBETHDevISR[DevID]);
    //Delete USBMSCDev...
    gpstUSBETHDevISR[DevID] = NULL;
    //Delete USBMSCDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_USBETH_DEV);
#endif

//#endif

    return RK_SUCCESS;

}

_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT API HDC USBETHDev_Create(uint32 DevID, void * arg)
{
    USBETH_DEV_ARG * pstUSBETHDevArg;
    DEVICE_CLASS* pstDev;
    USBETH_DEVICE_CLASS * pstUSBETHDev;

    if (arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstUSBETHDevArg = (USBETH_DEV_ARG *)arg;
    pstUSBETHDev =  rkos_memory_malloc(sizeof(USBETH_DEVICE_CLASS));
    if (pstUSBETHDev == NULL)
    {
        return (HDC) RK_ERROR;;
    }
    memset(pstUSBETHDev, 0, sizeof(USBETH_DEVICE_CLASS));

    pstUSBETHDev->rx_data = rkos_memory_malloc(ETH_RX_BUFFER_SIZE);
    pstUSBETHDev->tx_data = rkos_memory_malloc(ETH_TX_BUFFER_SIZE);
    pstUSBETHDev->osUSBETHOperSem =  rkos_semaphore_create(1,0);
    if (pstUSBETHDev->rx_data == NULL || pstUSBETHDev->tx_data == NULL || pstUSBETHDev->osUSBETHOperSem == NULL)
    {
        rkos_semaphore_delete(pstUSBETHDev->osUSBETHOperSem);
        rkos_memory_free(pstUSBETHDev->rx_data);
        rkos_memory_free(pstUSBETHDev->tx_data);
        rkos_memory_free(pstUSBETHDev);
        return (HDC) RK_ERROR;
    }
    pstDev = (DEVICE_CLASS *)pstUSBETHDev;
    pstDev->suspend = USBETHDevSuspend;
    pstDev->resume  = USBETHDevResume;
    //init arg...
    //pstUSBMSCDev->usbmode = pstUSBMSCDevArg->usbmode;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_USBETH_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    //gpstUSBHUBDevISR[DevID] = NULL;
    pstUSBETHDev->hOtg = pstUSBETHDevArg->hUsbOtgDev;

//    pstUSBMSCDev->hOtg = pstUSBMSCDevArg->hUsbOtgDev;
//    pstUSBMSCDev->hLun = pstUSBMSCDevArg->hLun;

    gpstUSBETHDevISR[DevID] = pstUSBETHDev;
    if (USBETHDevInit(pstUSBETHDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstUSBETHDev->osUSBETHOperSem);
        rkos_memory_free(pstUSBETHDev->rx_data);
        rkos_memory_free(pstUSBETHDev->tx_data);
        rkos_memory_free(pstUSBETHDev);
        gpstUSBETHDevISR[DevID] = NULL;
        return (HDC) RK_ERROR;
    }
    return (HDC)pstDev;

}


_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT FUN rk_err_t USBETHDevDeInit(USBETH_DEVICE_CLASS * pstUSBETHDev)
{
    UsbOtgDev_DisConnect();
    //RKDev_Close(pstUSBMSCDev->hLun);
    //RKDev_Close(pstUSBMSCDev->hOtg);
//    rkos_memory_free(pstUSBMSCDev->EpData);
    //RKTaskDelete (TASK_ID_USBMSC, 0, SYNC_MODE);

    return RK_SUCCESS;

}

_DRIVER_USBETH_USBETHDEVICE_INIT_
INIT FUN rk_err_t USBETHDevInit(USBETH_DEVICE_CLASS * pstUSBETHDev)
{
//    MSC_DEVICE *pMsc = &pstUSBMSCDev->mscdev;
//    memset(pMsc, 0, sizeof(MSC_DEVICE));
//    pMsc->connected = 1;

//    pstUSBMSCDev->verify_params.number = VERIFY_NUMBER;
//    //pstUSBMSCDev->EpData_Head.listlen = 0;
//    pstUSBMSCDev->mutux_status = 0;
//    pstUSBMSCDev->write_status = 0;
//    pstUSBMSCDev->invalid_cbw  = 0;

//    pMsc->BulkOut = BULK_OUT_EP;
//    pMsc->BulkIn = BULK_IN_EP;
//    pMsc->BulkXfer.TransSec = 0;

//    /**Device Part**/
//    usbMscDes.pDevDes = &HSDeviceDescr;
//    usbMscDes.pConfigDes = (USB_CONFIGURATION_DESCRIPTOR*)&allDescriptor;
//    usbMscDes.pInterfaceDes = &HSDeviceInterface;
//    usbMscDes.pEnDes[0] = &HSDeviceEnDes_IN;
//    usbMscDes.pEnDes[1] = &HSDeviceEnDes_OUT;
//    usbMscDes.pEnDes[2] = NULL;
//    usbMscDes.pOtherConfigDes = NULL;
//    usbMscDes.pHsQualifier = &HS_Device_Qualifier;
//    usbMscDes.pUsbPowerDes = NULL;
//    usbMscDes.pUsbHubDes = NULL;
//    usbMscDes.pUsbLangId = (USB_STRING_DESCRIPTOR*)USBLangId;
//    usbMscDes.pDevString = (USB_STRING_DESCRIPTOR*)StringProduct;
//    usbMscDes.pMString = (USB_STRING_DESCRIPTOR*)StringManufacture;
//    usbMscDes.pSerialNum = (USB_STRING_DESCRIPTOR*)StringSerialNumbel;

//    usbMscDes.pConfigString = (USB_STRING_DESCRIPTOR*)StringConfig;
//    usbMscDes.pInterfaceString= (USB_STRING_DESCRIPTOR*)StringInterface;
//    usbMscDes.pfReadEp = MscScsiCmdHandle;
//    usbMscDes.ConfigLen = sizeof(USB_FSG_CONFIGS_DESCRIPTOR);

//    /**Host Part**/
//    usbMscDes.pMscHost = MscHost;

    RKTaskCreate(TASK_ID_USBETH, 0, NULL);

    return RK_SUCCESS;

}




#ifdef _USBETH_DEV_SHELL_

_DRIVER_USBETH_USBETHDEVICE_SHELL_
static char* ShellUSBETHName[] =
{
    "pcb",
    "create",
    "delete",
    "test",
    "help",
    "\b",
};



_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL API rk_err_t USBETHDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellUSBETHName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;
    switch (i)
    {
        case 0x00:
            ret = USBETHDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = USBETHDevShellMc(dev,pItem);
            break;

        case 0x02:
            ret = USBETHDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = USBETHDevShellTest(dev,pItem);
            break;
        case 0x04:
            ret = USBETHDevShellHelp(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}


_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL FUN rk_err_t USBETHDevShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if ( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    printf("          UsbEth命令集提供了一系列的命令对UsbEth进行操作\r\n");
    printf("UsbEth    包含的子命令如下:           \r\n");
    printf("help      显示help信息           \r\n");
    printf("create    打开UsbEth              \r\n");
    printf("delete    删除UsbEth              \r\n");
    printf("test      测试UsbEth命令          \r\n");
    printf("pcb       显示pcb信息  \r\n");

    return RK_SUCCESS;
}


_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL FUN rk_err_t USBETHDevShellMc(HDC dev, uint8 * pstr)
{
    USBHUB_DEV_ARG stUSHUBCDevArg;
    rk_err_t ret;
    uint32 DevID;
    HDC hUSBMSCDev;
    HDC hUsbOtgDev;
    HDC hLun;

    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    hUsbOtgDev = RKDev_Open(DEV_CLASS_USBOTG, 0, NOT_CARE);
    if ((hUsbOtgDev == NULL) || (hUsbOtgDev == (HDC)RK_ERROR) || (hUsbOtgDev == (HDC)RK_PARA_ERR))
    {
        printf("UsbOtgDev open failure for Init USBETH\n");
        return RK_ERROR;
    }

    //Init USBMSCDev arg...
    stUSHUBCDevArg.hUsbOtgDev = hUsbOtgDev;

    //Create USBMSCDev...
    ret = RKDev_Create(DEV_CLASS_USBETH, DevID, &stUSHUBCDevArg);
    if (ret != RK_SUCCESS)
    {
        rk_print_string("USBETHDev create failure");
    }

    return RK_SUCCESS;

}


_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL FUN rk_err_t USBETHDevShellDel(HDC dev, uint8 * pstr)
{
    uint32 DevID;
    USBHUB_DEV_ARG pstUSBHUBDevArg;

    //Get USBMSCDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

    if (RKDev_Delete(DEV_CLASS_USBETH, DevID, (void*)&pstUSBHUBDevArg) != RK_SUCCESS)
    {
        rk_print_string("USBETHDev delete failure");
    }
    RKDev_Close(pstUSBHUBDevArg.hUsbOtgDev);
    RKTaskDelete (TASK_ID_USBETH, 0, SYNC_MODE);
    return RK_SUCCESS;

}


_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL FUN rk_err_t USBETHDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hUSBETHDev;
    uint32 DevID;

    //Get USBMSCDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }
    //FREQ_EnterModule(FREQ_USB);

    //Open USBMSCDev...
    hUSBETHDev = RKDev_Open(DEV_CLASS_USBETH, 0, NOT_CARE);
    if ((hUSBETHDev == NULL) || (hUSBETHDev == (HDC)RK_ERROR) || (hUSBETHDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("USBETHDev open failure");
        return RK_SUCCESS;
    }
    //do test....
    UsbEthDev_Connect(hUSBETHDev, 0);
    //close USBMSCDev...
    RKDev_Close(hUSBETHDev);
    return RK_SUCCESS;

}


_DRIVER_USBETH_USBETHDEVICE_SHELL_
SHELL FUN rk_err_t USBETHDevShellPcb(HDC dev, uint8 * pstr)
{
    HDC hUSBMSCDev;
    uint32 DevID;

    //Get USBMSCDev ID...
    if (StrCmpA(pstr, "0", 1) == 0)
    {
        DevID = 0;
    }
    else if (StrCmpA(pstr, "1", 1) == 0)
    {
        DevID = 1;
    }
    else
    {
        DevID = 0;
    }

//    //Display pcb...
//    if (gpstUSBMSCDevISR[DevID] != NULL)
//    {
//        rk_print_string("rn");
//        rk_printf_no_time("USBMSCDev ID = %d Pcb list as follow:", DevID);
//        rk_printf_no_time("    &stUSBMSCDevice---0x%08x", &gpstUSBMSCDevISR[DevID]->stUSBMSCDevice);
//        rk_printf_no_time("    osUSBMSCOperReqSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCOperReqSem);
//        rk_printf_no_time("    osUSBMSCOperSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCOperSem);
//        rk_printf_no_time("    osUSBMSCReadReqSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCReadReqSem);
//        rk_printf_no_time("    osUSBMSCReadSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCReadSem);
//        rk_printf_no_time("    osUSBMSCWriteReqSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCWriteReqSem);
//        rk_printf_no_time("    osUSBMSCWriteSem---0x%08x", gpstUSBMSCDevISR[DevID]->osUSBMSCWriteSem);
//        //rk_printf_no_time("    usbmode---0x%08x", gpstUsbOtgDevISR[DevID]->usbmode);
//        //rk_printf_no_time("    usbspeed---0x%08x", gpstUsbOtgDevISR[DevID]->usbspeed);
//    }
//    else
    {
        rk_print_string("rn");
        rk_printf_no_time("USBETHDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}



#endif



#endif

