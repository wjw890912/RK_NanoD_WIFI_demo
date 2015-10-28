

#include "BspConfig.h"
#ifdef __DRIVER_USBHUB_USBHUBDEVICE_C__

#include "FreeRTOS.h"
#include "RKOS.h"
#include "BSP.h"
#include "DeviceInclude.h"
#include "stdint.h"


typedef __packed struct _Usb_Port_Status {
	unsigned short wPortStatus;
	unsigned short wPortChange;
}Usb_Port_Status;

typedef __packed  struct _Usb_Hub_Status {
	unsigned short wHubStatus;
	unsigned short wHubChange;
}Usb_Hub_Status;


typedef __packed struct _HubDescriptor
{
	uint8_t   	bDescLength;
	uint8_t  	bDescriptorType;
	uint8_t		bNbrPorts;
	uint16_t  	wHubCharacteristics;
	uint8_t   	bPwrOn2PwrGood;  
	uint8_t   	bHubContrCurrent; 
	uint8_t   	DeviceRemovable;    
	uint8_t   	PortPwrCtrlMask;
}USBH_Hub_Descriptor;

typedef    struct tagHUB_HOST
{
    uint8       valid;
    uint8       IntIn;

    uint32      SendCtrlPipe;
    uint32      RecvCtrlPipe;
    uint32      RecvIntPipe;

    HOST_DEV    *pDev;
}HUB_HOST, *pHUB_HOST;

typedef struct _HUB_PORT
{
  	HOST_DEV    *pDev;
	int32 (*pHub_Host)(HOST_INTERFACE* interfaceDes);
}HUB_PORT,*pHUB_PORT;


typedef  struct _USBHUB_DEVICE_CLASS
{
    DEVICE_CLASS stUSBHUBDevice;
	xSemaphoreHandle osUSBHUBOperSem;
    uint32_t      SendCtrlPipe;
    uint32_t      RecvCtrlPipe;
    uint32_t      RecvIntPipe;	
	uint8_t usbdev_alloc_addr;
	uint8_t usbdev_addr;
	uint8_t	NbrPorts;
	HDC              hOtg;
	HOST_INTERFACE   *interfaceDes;	
	HUB_PORT		**phub_port;	
	HUB_HOST	gHubHost;	

}USBHUB_DEVICE_CLASS;

static USBHUB_DEVICE_CLASS * gpstUSBHUBDevISR[1]=
{
    (USBHUB_DEVICE_CLASS *)NULL
};

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 HostInterruptMsg(HOST_DEV *pDev, uint32 pipe, void * data, int len, int timeout)
{

}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 HubHostGetPipe(HUB_HOST *pHubHost, HOST_INTERFACE* intf)
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
                if (!pHubHost->IntIn)
                    pHubHost->IntIn = USBEpNum(ep);
            }
        }
    }

    if (pHubHost->IntIn)
    {
        pHubHost->SendCtrlPipe = HostCreatePipe(intf->pDev, 0, 0);
        pHubHost->RecvCtrlPipe = HostCreatePipe(intf->pDev, 0, 1);
        pHubHost->RecvIntPipe =  HostCreatePipe(intf->pDev, pHubHost->IntIn, 1);
        pHubHost->pDev = intf->pDev;
        ret = HOST_OK;
    }

    return ret;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN uint8_t USB_HUBDevAllocAddr(void)
{
	uint8_t i;
    //USBMSCDev Resume...
    USBHUB_DEVICE_CLASS * pstUSBHUBDev = gpstUSBHUBDevISR[0];
    if (pstUSBHUBDev == NULL)
    {
        return 0;
    }
	for(i=0;i<8;i++)
	{
		if(!(pstUSBHUBDev->usbdev_alloc_addr & (0x01<<i)))
		{
			pstUSBHUBDev->usbdev_alloc_addr |= (0x01<<i);
			return i + 2;
		}
	}
	return 0;
}



_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN void USB_HUBDevFreeAddr(uint8_t addr)
{
	USBHUB_DEVICE_CLASS * pstUSBHUBDev = gpstUSBHUBDevISR[0];
	if (pstUSBHUBDev == NULL || addr > 9|| addr < 2)
    {
        return;
    }
	pstUSBHUBDev->usbdev_alloc_addr &=~(0x01<<(addr - 2));
}




_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Get_Descriptor(HUB_HOST *pHubHost,void *data, int size)
{
    return  HostCtrlMsg(pHubHost->pDev, pHubHost->RecvCtrlPipe, USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_DEVICE, USB_REQ_GET_DESCRIPTOR,
                    0x2900, 0x0000, data, size, USB_CTRL_GET_TIMEOUT);
}
_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Get_Hub_Status(HUB_HOST *pHubHost,void* data)
{
    return  HostCtrlMsg(pHubHost->pDev, pHubHost->RecvCtrlPipe, USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_DEVICE, USB_REQ_GET_STATUS,
                    0x0000, 0x0000, data, sizeof(Usb_Hub_Status), USB_CTRL_GET_TIMEOUT);

}
_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Get_Port_Status(HUB_HOST *pHubHost,int port,void* data)
{
    return  HostCtrlMsg(pHubHost->pDev, pHubHost->RecvCtrlPipe, USB_D2H | USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_OTHER, USB_REQ_GET_STATUS,
                    0x0000, port, data, sizeof(Usb_Hub_Status), USB_CTRL_GET_TIMEOUT);

}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Set_Port_Feature(HUB_HOST *pHubHost, int port, int feature)
{
    return  HostCtrlMsg(pHubHost->pDev, pHubHost->SendCtrlPipe, USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_OTHER, USB_REQ_SET_FEATURE,
                    feature, port, NULL, 0, USB_CTRL_GET_TIMEOUT);
}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Clear_Port_Feature(HUB_HOST *pHubHost, int port, int feature)
{
    return  HostCtrlMsg(pHubHost->pDev, pHubHost->SendCtrlPipe, USB_REQ_TYPE_CLASS | USB_REQ_RECIPIENT_OTHER, USB_REQ_CLEAR_FEATURE,
                    feature, port, NULL, 0, USB_CTRL_GET_TIMEOUT);
}

#if 1

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_StatusChange(HUB_HOST *pHubHost)
{
	int32 status ;
	printf("usbh_hub_StatusChange");
	while(1 == 1);
	return status;
}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Port_Reset(HUB_HOST *pHubHost, uint8_t port_num)
{
	uint8_t i;
	uint8_t buf[4];
	unsigned short portstatus, portchange;
	int32 status;
	for(i=0 ;i<5; i++)
	{
		status = Hub_Set_Port_Feature(pHubHost,port_num,USB_PORT_FEAT_RESET);
		if(status < 0)
			return status;
		DelayMs(200); 
		status = Hub_Get_Port_Status(pHubHost,port_num,buf);
		if(status < 0)
			return status;
		portstatus = buf[1]<<8 | buf[0];
		portchange = buf[3]<<8 | buf[2];
		printf("portstatus %x, change %x \r\n", portstatus, portchange);
		if (portstatus & USB_PORT_STAT_ENABLE)
			break;

		DelayMs(200);
	}
		if (i == 5) 
		{
			printf("Cannot enable port %i after %i retries, " \
						"disabling port.\n", port_num, 5);
			printf("Maybe the USB cable is bad?\n");
			return -1;
		}
	return Hub_Clear_Port_Feature(pHubHost,port_num, USB_PORT_FEAT_C_RESET);
}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN int32 Hub_HostSetAddr(HOST_DEV *pHDev)
{
    int32 ret, i;
    uint8 devnum = USB_HUBDevAllocAddr();
	if( devnum ==0 )
		return -1;
    for(i=0; i<2; i++)
    {
        ret = HostCtrlMsg(pHDev, HostCreatePipe(pHDev, 0, 0),
                 0, USB_REQ_SET_ADDRESS, devnum, 0,
                NULL, 0, USB_CTRL_SET_TIMEOUT);

        if (ret >= 0)
        {
            pHDev->DevNum = devnum;
            pHDev->state = USB_STATE_ADDRESS;
            break;
        }
        DelayMs(200);
    }

    return ret;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Enum_Dev(HUB_HOST *pHubHost,uint8_t port_num)
{
	int32 ret;
	uint16 length;
	uint32 buffer[256/4];
	uint8_t i;
	uint8_t *buf;
	HOST_DEV    *host_Dev;
	USB_DEVICE_DESCRIPTOR *pDesc;
	host_Dev =  rkos_memory_malloc(sizeof(HOST_DEV));
	if(host_Dev ==NULL)
	{
		return -1;
	}
	memset(host_Dev, 0, sizeof(HOST_DEV));
	HostEp0ReInit(host_Dev, 8);
	host_Dev->valid = pHubHost->pDev->valid;
	host_Dev->speed = pHubHost->pDev->speed;
    ret = HostGetDesc(host_Dev, USB_DT_DEVICE, 0, buffer, 8);
    if (ret < 0)
        return ret;

    pDesc = (USB_DEVICE_DESCRIPTOR *)buffer;
    if (8 != pDesc->bMaxPacketSize0)
    {
        HostEp0ReInit(host_Dev, pDesc->bMaxPacketSize0);
    }	
	printf("bMaxPacketSize0:%d %d\r\n",pDesc->bMaxPacketSize0,host_Dev->ep0.wMaxPacketSize);

    ret = Hub_HostSetAddr(host_Dev);
    if (ret < 0)
        return ret;

    DelayMs(5);
	printf("dev_addr:%d\r\n",host_Dev->DevNum);
    ret = HostGetDesc(host_Dev, USB_DT_DEVICE, 0, buffer, USB_DT_DEVICE_SIZE);
    if (ret < 0)
        return ret;	
	
	memcpy(&host_Dev->DevDesc, buffer, USB_DT_DEVICE_SIZE);
	buf = (uint8_t *)&host_Dev->DevDesc;
	for(i=0;i<USB_DT_DEVICE_SIZE;i++)
	{
		printf("%02x ",buf[i]);
	}
	printf("\r\n-->> VID : %04Xh\r\n" , host_Dev->DevDesc.idVendor);
	printf("-->> PID : %04Xh\r\n" , host_Dev->DevDesc.idProduct);	

	ret = HostNewDev(host_Dev);
    if (ret < 0)
        return ret;		
	(gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pDev = host_Dev;
	buf = (uint8_t *)&host_Dev->CfgDesc;
	for(i=0;i<USB_DT_CONFIG_SIZE;i++)
	{
		printf("%02x ",buf[i]);
	}	
	
	printf ("\r\n---------------------------------------------\n");
	printf("Enum USB Device OK! \nUSB Device Class is:0x%x\n",
		   host_Dev->intf.IfDesc.bInterfaceClass);
	printf ("0x01:USB Audio Device\n");
	printf ("0x02:USB Commucation Device\n");
	printf ("0x03:USB HID Device\n");
	printf (".....\n");
	printf ("0x08:USB Massage Storage Device\n");
	printf ("\n---------------------------------------------\n");
	switch (host_Dev->intf.IfDesc.bInterfaceClass)
	{
		case USB_CLASS_AUDIO:
			break;
		case USB_CLASS_COMM:
			break;
		case USB_CLASS_HID:
			break;
		case USB_CLASS_PHYSICAL:
			break;
		case USB_CLASS_STILL_IMAGE:
			break;
		case USB_CLASS_PRINTER:
			break;
		case USB_CLASS_MASS_STORAGE:
			(gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pHub_Host = MscHost;
			(gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pHub_Host(&host_Dev->intf);
			break;
		case USB_CLASS_HUB:
			//gpstUsbOtgDevISR[0]->pMscHost(&host_Dev->intf);
			break;
		default:
			break;
	}


	return ret;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Port_Connect_Change(HUB_HOST *pHubHost,uint8_t port_num,uint16_t portstatus)
{
	int32 status ;
	do{
		status = Hub_Clear_Port_Feature(pHubHost,port_num,USB_PORT_FEAT_C_CONNECTION);
	}while(status < 0);
	
	if(((!(portstatus & USB_PORT_STAT_CONNECTION)) &&(!(portstatus & USB_PORT_STAT_ENABLE))))
	{
		if (!(portstatus & USB_PORT_STAT_CONNECTION))			
		{		
			printf("hub prot%d disconnection\r\n",port_num);
			USB_HUBDevFreeAddr((gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pDev->DevNum);
			rkos_memory_free((gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pDev);
			(gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pDev = NULL;
			(gpstUSBHUBDevISR[0]->phub_port)[port_num - 1]->pHub_Host = NULL;
			return 0;
		}
	}



	status = Hub_Port_Reset(pHubHost,port_num);
	if(status < 0)
			return status;
	DelayMs(100);
	printf("start usbh_hub_enum_dev\r\n");
	status = Hub_Enum_Dev(pHubHost,port_num);
	printf("finish usbh_hub_enum_dev:%d\r\n",status);
/*	if(usbh_hub_dev_analyze(pdev,phost,port_num) == USBH_OK)
	{
		printf("finish usbh_hub_enum_dev\r\n");
	}
*/
	return status;
}




_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_prot_Status_Change(HUB_HOST *pHubHost,uint8_t port_num)
{
	uint8_t i,data_buf[4];
	unsigned short portstatus, portchange;
	int32 st;
	int32 status = 0;
	st = Hub_Get_Port_Status(pHubHost,port_num,data_buf);
	if(st < 0)
	{
		return st;
	}
	printf("get prot%d descriptor ok:",port_num);	
	for(i = 0;i < 4;i++)
	{
		printf("%02x ", data_buf[i]);	
	}
	printf("\r\n");	
	portstatus = data_buf[1]<<8 | data_buf[0];
	portchange = data_buf[3]<<8 | data_buf[2];	
	printf("Port %d Status 0x%04X Change 0x%04X\r\n",port_num, portstatus, portchange);
	if (portchange & USB_PORT_STAT_C_CONNECTION) {
			printf("port %d connection change\r\n", port_num);
			status = Hub_Port_Connect_Change(pHubHost, port_num,portstatus);
		}
	if (portchange & USB_PORT_STAT_C_ENABLE) {
			printf("port %d enable change, status %x\r\n",
			      port_num, portstatus);		
		status = Hub_Clear_Port_Feature(pHubHost, port_num,
						USB_PORT_FEAT_C_ENABLE);
	}
		if (portstatus & USB_PORT_STAT_SUSPEND) {
			printf("port %d suspend change\r\n", port_num);
			status = Hub_Clear_Port_Feature(pHubHost, port_num,
						USB_PORT_FEAT_SUSPEND);
		}	

		if (portchange & USB_PORT_STAT_C_OVERCURRENT) {
			printf("port %d over-current change\r\n", port_num);
			status = Hub_Clear_Port_Feature(pHubHost, port_num,
						USB_PORT_FEAT_C_OVER_CURRENT);
			status = Hub_Set_Port_Feature(pHubHost, port_num,
						USB_PORT_FEAT_POWER);
		}

		if (portchange & USB_PORT_STAT_C_RESET) {
			printf("port %d reset change\r\n", port_num);
			status = Hub_Clear_Port_Feature(pHubHost, port_num,
						USB_PORT_FEAT_C_RESET);
		}		
	return status;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 Hub_Status_Change(uint32_t bitmap)
{
	uint8_t i;
	int32 status;
	HUB_HOST *pHubHost = &(gpstUSBHUBDevISR[0]->gHubHost);
	for (i = 0; i < gpstUSBHUBDevISR[0]->NbrPorts + 1 ; i++)
	{
	  if (bitmap & (1 << i))
	  {
		  if(i == 0)
		  {
			  status = Hub_StatusChange(pHubHost);
		  }
		  else
		  {
			  status = Hub_prot_Status_Change(pHubHost,i);
		  }
	  }
	}
	return status;	

}


#endif

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API void Hub_Power_on(HUB_HOST *pHubHost)
{
	int i;
	for (i = 0; i < gpstUSBHUBDevISR[0]->NbrPorts; i++) 
	{
		Hub_Set_Port_Feature(pHubHost, i + 1, USB_PORT_FEAT_POWER);
	}

}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API void Hub_Indicator_on(HUB_HOST *pHubHost)
{
	int i;
	for (i = 0; i < gpstUSBHUBDevISR[0]->NbrPorts; i++) 
	{
		Hub_Set_Port_Feature(pHubHost, i + 1, 0x0016);
	}

}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
int32 HubHostProbe(HOST_INTERFACE* intf)
{
    int32 i, j, ret;
    HUB_HOST *pHubHost = &(gpstUSBHUBDevISR[0]->gHubHost);
	uint8_t buffer[64];
    memset (pHubHost, 0, sizeof(HUB_HOST));

    ret =  HubHostGetPipe(pHubHost, intf);
    if (ret < 0)
        return ret;
	printf("get hub descriptorstart:\r\n");
	ret = Hub_Get_Descriptor(pHubHost,buffer,0x0009);
	printf("get hub descriptor ok:");	
	for(i = 0;i < 0x0009;i++)
	{
		printf("%02x ", buffer[i]);	
	}
	printf("\r\n"); 
	gpstUSBHUBDevISR[0]->NbrPorts = ((USBH_Hub_Descriptor *)buffer)->bNbrPorts;
	gpstUSBHUBDevISR[0]->phub_port=  rkos_memory_malloc(sizeof(HUB_PORT) * gpstUSBHUBDevISR[0]->NbrPorts); 
	printf("this hub port:%d\r\n",gpstUSBHUBDevISR[0]->NbrPorts);
	if(gpstUSBHUBDevISR[0]->phub_port == NULL)
		return -1;
	ret = Hub_Get_Hub_Status(pHubHost,buffer);
    if (ret < 0)
        return ret;

	printf("get hub status ok:");	
	for(i = 0;i < sizeof(Usb_Hub_Status);i++)
	{
		printf("%02x ", buffer[i]);	
	}
	printf("\r\n");	
	Hub_Power_on(pHubHost);
	Hub_Indicator_on(pHubHost);
	for(i = 0;i < gpstUSBHUBDevISR[0]->NbrPorts;i++)
	{
		Hub_Get_Port_Status(pHubHost,i+1,buffer);
		printf("get hub port%d status ok:",i+1);	
		for(j =0;j<sizeof(Usb_Port_Status);j++)
		{
			printf("%02x ", buffer[j]);	
		}
		printf("\r\n");	
	}	

}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
int32 Hub_Get_Change_BitMap(void *bitmap)
{
	int32 ret;
	HUB_HOST *pHubHost = &(gpstUSBHUBDevISR[0]->gHubHost);
//	printf("RecvIntPipe:0x%08x\r\n",pHubHost->RecvIntPipe);
    return HostBulkMsg(pHubHost->pDev, pHubHost->RecvIntPipe,
                        bitmap, 1, 5000);

}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API void USBHUBDev_Task_Enter(void)
{
	int32 ret;
	uint8_t i[4] = 0;
	rkos_semaphore_take(gpstUSBHUBDevISR[0]->osUSBHUBOperSem, MAX_DELAY);
	HubHostProbe(gpstUSBHUBDevISR[0]->interfaceDes);
	while(1)
	{
		rkos_delay(1000);
		ret = Hub_Get_Change_BitMap(i);
		if(ret > 0)
		{
			rk_printf("hub_task_delay ret:%d,i:0x%02x\r\n",ret,i[0]);
			Hub_Status_Change(i[0]);
			
		}
	}

}


//_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
//COMMON FUN uint8_t USB_HUBDevAllocAddr(HDC dev)
//{
//	uint8_t i;
//    //USBMSCDev Resume...
//    USBHUB_DEVICE_CLASS * pstUSBHUBDev =  (USBHUB_DEVICE_CLASS *)dev;
//    if (pstUSBHUBDev == NULL)
//    {
//        return 0;
//    }
//	for(i=0;i<8;i++)
//	{
//		if(!(pstUSBHUBDev->usbdev_alloc_addr & (0x01<<i)))
//		{
//			pstUSBHUBDev->usbdev_alloc_addr |= (0x01<<i);
//			return i + 2;
//		}
//	}
//	return 0;
//}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API HOST_PIPE USB_HUBCreatePipe(uint8_t addr, USB_ENDPOINT_DESCRIPTOR *Ep)
{
	HOST_PIPE	pipe;
	
	pipe.b.mps     = Ep->wMaxPacketSize;
	pipe.b.epnum   = Ep->bEndpointAddress & 0x7f;
	pipe.b.epdir   = Ep->bEndpointAddress >> 7;
	pipe.b.eptype  = Ep->bmAttributes;
	pipe.b.devaddr = addr;
	
    return pipe;
}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN static int32 USB_HUBGetDesc(HOST_DEV *pHDev,uint32_t pipe,uint8 type, uint8 index, void *buf, int size)
{
    int32 i, ret;

    for (i = 0; i < 3; i++)
    {
        ret = HostCtrlMsg(pHDev,pipe,  USB_DIR_IN, USB_REQ_GET_DESCRIPTOR,
                    (type << 8) + index, 0, buf, size, USB_CTRL_GET_TIMEOUT);
        if (ret <= 0)
        {
            continue;
        }
        if (ret>1 && ((uint8 *)buf)[1] != type)
        {
            ret = -HOST_ERR;
            continue;
        }
        break;
    }
    return ret;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN rk_err_t USBHUBDevResume(HDC dev)
{
    //USBMSCDev Resume...
    USBHUB_DEVICE_CLASS * pstUSBHUBDev =  (USBHUB_DEVICE_CLASS *)dev;
    if (pstUSBHUBDev == NULL)
    {
        return RK_ERROR;
    }

}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON FUN rk_err_t USBHUBDevSuspend(HDC dev)
{
    //USBMSCDev Suspend...
    USBHUB_DEVICE_CLASS * pstUSBHUBDev =  (USBHUB_DEVICE_CLASS *)dev;
    if (pstUSBHUBDev == NULL)
    {
        return RK_ERROR;
    }

}

_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API rk_err_t UsbHubDev_Connect(HDC dev, uint32 Speed)
{
    int ret;
    USBHUB_DEVICE_CLASS * pstUsbMscDev = (USBHUB_DEVICE_CLASS *)dev;
    FREQ_EnterModule(FREQ_MAX);

    ret = UsbOtgDev_Connect (dev,Speed);

    if (ret != RK_SUCCESS)
    {
        //rk_printf ("UsbOtg connect fail!\n");
        return RK_ERROR;
    }

    return RK_SUCCESS;
}


_DRIVER_USBHUB_USBHUBDEVICE_COMMON_
COMMON API int32 HubHost(HOST_INTERFACE* interfaceDes)
{

    int32 i, n, ret=0;
    printf ("USB_HUB:%d\n",interfaceDes->IfDesc.bInterfaceClass);
    gpstUSBHUBDevISR[0]->interfaceDes = interfaceDes;
    rkos_semaphore_give(gpstUSBHUBDevISR[0]->osUSBHUBOperSem);

    return ret;
}



_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT API rk_err_t USBHUBDev_Task_DeInit(void *pvParameters)
{
	return RK_SUCCESS;	
}

_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT API rk_err_t USBHUBDev_Task_Init(void *pvParameters)
{
	//gpstUsbOtgDevISR[0]->pMscHost = HubHost;
	rk_err_t ret;
	USBOGT_DESCRIPTOR gto_desc;
	gto_desc.pMscHost = HubHost;
	ret = UsbOtgDev_SetDes(gpstUSBHUBDevISR[0]->hOtg,&gto_desc);
	if (ret == RK_ERROR)
	{
			printf ("HUB Usb Otg Set Descriptor failure\n");
	}
	printf ("HUB Usb Otg Set Descriptor ok\n");
	return RK_SUCCESS;	
}


_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT API rk_err_t USBHUBDev_Delete(uint32 DevID, void * arg)
{
//#if 1
    DevID = 0;
//    USBHUB_DEV_ARG* pstUSBHUBDevArg = (USBHUB_DEV_ARG*)arg;
//    pstUSBHUBDevArg->hUsbOtgDev = gpstUSBHUBDevISR[DevID]->hOtg;

    //Check USBMSCDev is not exist...
    printf ("\nUSB HUB Delete \n");
    if (gpstUSBHUBDevISR[DevID] == NULL)
    {
        printf ("Delete error\n");
        return RK_ERROR;
    }
    //USBMSCDev deinit...
    USBHUBDevDeInit(gpstUSBHUBDevISR[DevID]);
//    //Free USBMSCDev memory...
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCOperReqSem);
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCOperSem);
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCReadReqSem);
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCReadSem);
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCWriteReqSem);
//    rkos_semaphore_delete(gpstUSBHUBDevISR[DevID]->osUSBMSCWriteSem);
	if(gpstUSBHUBDevISR[DevID]->phub_port)
		rkos_memory_free(gpstUSBHUBDevISR[DevID]->phub_port);
    rkos_memory_free(gpstUSBHUBDevISR[DevID]);
    //Delete USBMSCDev...
    gpstUSBHUBDevISR[DevID] = NULL;
    //Delete USBMSCDev Read and Write Module...
#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_RemoveModule(MODULE_ID_USBHUB_DEV);
#endif

//#endif

    return RK_SUCCESS;

}


_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT API HDC USBHUBDev_Create(uint32 DevID, void * arg)
{
    USBHUB_DEV_ARG * pstUSBHUBDevArg;
    DEVICE_CLASS* pstDev;
    USBHUB_DEVICE_CLASS * pstUSBHUBDev;

    if (arg == NULL)
    {
        return (HDC)RK_PARA_ERR;
    }

    // Create handler...
    pstUSBHUBDevArg = (USBHUB_DEV_ARG *)arg;
    pstUSBHUBDev =  rkos_memory_malloc(sizeof(USBHUB_DEVICE_CLASS));    
		if (pstUSBHUBDev == NULL)
    {
        return (HDC) RK_ERROR;;
    }
    memset(pstUSBHUBDev, 0, sizeof(USBHUB_DEVICE_CLASS));

		pstUSBHUBDev->osUSBHUBOperSem =  rkos_semaphore_create(1,0);
		if(pstUSBHUBDev->osUSBHUBOperSem ==NULL)
		{
			 return (HDC) RK_ERROR;
		}
    pstDev = (DEVICE_CLASS *)pstUSBHUBDev;
    pstDev->suspend = USBHUBDevSuspend;
    pstDev->resume  = USBHUBDevResume;
    //init arg...
    //pstUSBMSCDev->usbmode = pstUSBMSCDevArg->usbmode;

#ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_USBHUB_DEV, MODULE_OVERLAY_CODE);
#endif
    //device init...
    gpstUSBHUBDevISR[DevID] = NULL;
	pstUSBHUBDev->hOtg = pstUSBHUBDevArg->hUsbOtgDev;

//    pstUSBMSCDev->hOtg = pstUSBMSCDevArg->hUsbOtgDev;
//    pstUSBMSCDev->hLun = pstUSBMSCDevArg->hLun;

    gpstUSBHUBDevISR[DevID] = pstUSBHUBDev;
    if (USBHUBDevInit(pstUSBHUBDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstUSBHUBDev->osUSBHUBOperSem);
        rkos_memory_free(pstUSBHUBDev);
        gpstUSBHUBDevISR[DevID] = NULL;
        return (HDC) RK_ERROR;
    }
    return (HDC)pstDev;

}


_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT FUN rk_err_t USBHUBDevDeInit(USBHUB_DEVICE_CLASS * pstUSBMSCDev)
{
    UsbOtgDev_DisConnect();
    //RKDev_Close(pstUSBMSCDev->hLun);
    //RKDev_Close(pstUSBMSCDev->hOtg);
//    rkos_memory_free(pstUSBMSCDev->EpData);
    //RKTaskDelete (TASK_ID_USBMSC, 0, SYNC_MODE);

    return RK_SUCCESS;

}

_DRIVER_USBHUB_USBHUBDEVICE_INIT_
INIT FUN rk_err_t USBHUBDevInit(USBHUB_DEVICE_CLASS * pstUSBMSCDev)
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

    RKTaskCreate(TASK_ID_USBHUB, 0, NULL);

    return RK_SUCCESS;

}






#ifdef _USBHUB_DEV_SHELL_

_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
static char* ShellUSBHUBName[] =
{
	"pcb",
	"create",
	"delete",
	"test",
	"help",
	"\b",
};



_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL API rk_err_t USBHUBDev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellUSBHUBName, pItem, StrCnt);
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
            ret = USBHUBDevShellPcb(dev,pItem);
            break;

        case 0x01:
            ret = USBHUBDevShellMc(dev,pItem);
            break;

        case 0x02:
            ret = USBHUBDevShellDel(dev,pItem);
            break;

        case 0x03:
            ret = USBHUBDevShellTest(dev,pItem);
            break;
        case 0x04:
            ret = USBHUBDevShellHelp(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    return ret;

}


_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL FUN rk_err_t USBHUBDevShellHelp(HDC dev,  uint8 * pstr)
{
    pstr--;

    if ( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    printf("          UsbHub命令集提供了一系列的命令对Usbhub进行操作\r\n");
    printf("UsbHub包含的子命令如下:           \r\n");
    printf("help      显示help信息           \r\n");
    printf("create    打开UsbHub              \r\n");
    printf("delete    删除UsbHub              \r\n");
    printf("test      测试UsbHub命令          \r\n");
    printf("pcb       显示pcb信息  \r\n");

    return RK_SUCCESS;
}


_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL FUN rk_err_t USBHUBDevShellMc(HDC dev, uint8 * pstr)
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
        printf("UsbOtgDev open failure for Init USBHUB\n");
        return RK_ERROR;
    }

    //Init USBMSCDev arg...
    stUSHUBCDevArg.hUsbOtgDev = hUsbOtgDev;

    //Create USBMSCDev...
    ret = RKDev_Create(DEV_CLASS_USBHUB, DevID, &stUSHUBCDevArg);
    if (ret != RK_SUCCESS)
    {
        rk_print_string("USBHUBDev create failure");
    }

    return RK_SUCCESS;

}


_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL FUN rk_err_t USBHUBDevShellDel(HDC dev, uint8 * pstr)
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

    if (RKDev_Delete(DEV_CLASS_USBHUB, DevID, (void*)&pstUSBHUBDevArg) != RK_SUCCESS)
    {
        rk_print_string("USBHUBDev delete failure");
    }
    RKDev_Close(pstUSBHUBDevArg.hUsbOtgDev);
    RKTaskDelete (TASK_ID_USBHUB, 0, SYNC_MODE);
    return RK_SUCCESS;

}


_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL FUN rk_err_t USBHUBDevShellTest(HDC dev, uint8 * pstr)
{
    HDC hUSBHUBDev;
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
    hUSBHUBDev = RKDev_Open(DEV_CLASS_USBHUB, 0, NOT_CARE);
    if ((hUSBHUBDev == NULL) || (hUSBHUBDev == (HDC)RK_ERROR) || (hUSBHUBDev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("USBHUBDev open failure");
        return RK_SUCCESS;
    }
    //do test....
    UsbHubDev_Connect(hUSBHUBDev, 0);
    //close USBMSCDev...
    RKDev_Close(hUSBHUBDev);
    return RK_SUCCESS;

}


_DRIVER_USBHUB_USBHUBDEVICE_SHELL_
SHELL FUN rk_err_t USBHUBDevShellPcb(HDC dev, uint8 * pstr)
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
        rk_printf_no_time("USBHUBDev ID = %d not exit", DevID);
    }
    return RK_SUCCESS;

}



#endif

#endif

