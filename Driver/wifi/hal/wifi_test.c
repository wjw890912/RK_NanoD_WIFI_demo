#include "debug_print.h"

//#include <includes.h>
#include "app_cfg.h"
//#include "bsp.h"
//#include "nvic.h"
//#include "delay.h"
//#include "uart.h"
//#include "key.h"
//#include "lcd.h"
//#include "sram.h"
//#include "malloc.h"

#include "lwip/netifapi.h"
#include "FreeRTOS.h"
#include "RKOS.h"
#include "lwip/dhcp.h"
#include "lwip/ip_addr.h"

#include "wifi_test.h"
#include "wifi_netlink.h"
//#include "wifi_shell.h"
#ifdef __DRIVER_INNOPHACE_WIFI_C__
#pragma arm section code = "innowifiCode", rodata = "innowifiCode", rwdata = "innowifidata", zidata = "innowifidata"
uint8  dhcp_ip_got = FALSE;
extern xQueueHandle wifi_event_queue_handle;
#define LCD_ShowString(a,b,c,d,e,f)

#define command_test_mode()
err_t tcpip_input(struct pbuf *p, struct netif *inp);
#ifdef CONFIG_WIFI_STA_MODE

extern struct netif wifi_netif;
uint8 need_do_dhcp = TRUE;
extern WIFI_NETLINK_OPERATIONS_T wifi_netlink_ops;
extern _os_task  wifi_test_task_tcb;
//struct netif wifi_netif;
uint8 lwip_init_wifi = 0;
#if 1

#include "lwip/api.h"
_os_task  tcp_task_tcb;

uint8 tcp_buf[2048];

void tcp_test_task(void *arg)
{
    struct tcp_pcb *pcb;
    struct netconn *conn;
    struct netconn *conn_new;
    struct netbuf * buf;
    err_t err;

    conn = netconn_new(NETCONN_TCP);
    if(conn == NULL)
    {
        printf("netconn_new alloc fail\n");

        while(1)
        {
            freertos_msleep(100);
        }
    }

    netconn_bind(conn , IP_ADDR_ANY, 90);

    netconn_listen(conn);

    netconn_accept(conn, &conn_new);
    memset(tcp_buf, 0x55, 2048);
    while(1)
    {
        if(netconn_recv(conn_new, &buf) == 0)
        {
             netbuf_delete(buf);
             //printf("get data len = %d\n");
            //netconn_write(conn_new, tcp_buf, 512, NETCONN_COPY);
        }
    }
}

void tcp_test_init(void)
{
    freertos_create_task(&tcp_task_tcb,"tcp_test_task",256*4,0,WIFI_TEST_TASK_PRIO+6, tcp_test_task, NULL);
}
#endif

void lwip_my_init(void)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;

  tcpip_init(NULL,NULL);

#ifdef CONFIG_WIFI_SOFTAP_MODE
  IP4_ADDR(&ipaddr, 192, 168, 237, 1);
  IP4_ADDR(&netmask, 255, 255, 255, 0);
  IP4_ADDR(&gw, 192, 168, 237, 1);
#else
  IP4_ADDR(&ipaddr, 0, 0, 0, 0);
  IP4_ADDR(&netmask, 0, 0, 0, 0);
  IP4_ADDR(&gw, 0, 0, 0, 0);
#endif

  netif_add(&wifi_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

  netif_set_default(&wifi_netif);

  /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&wifi_netif);

  tcp_test_init();
  httpd_init();
  httpd_ssi_init();
  httpd_cgi_init();
  lwip_init_wifi = 1;
}

#if 0
void lwip_period_timer(void)
{
	OS_ERR  err;

	OSTimeDly(1,OS_OPT_TIME_DLY,&err);
	ethernetif_input(&wifi_netif); 			//???‘那?﹞??車那?米?那y?Y

}
#endif

void wifi_main()
{
//	OS_ERR  err;


//	OSTaskCreate((OS_TCB     *)&wifi_test_task_tcb,                /* Create the wifi test task                                */
//	         (CPU_CHAR   *)"wifi_test_task",
//	         (OS_TASK_PTR ) wifi_test_task,
//	         (void       *) 0,
//	         (OS_PRIO     ) WIFI_TEST_TASK_PRIO,
//	         (CPU_STK    *)&wifi_test_task_stk[0],
//	         (CPU_STK_SIZE) WIFI_TEST_TASK_STK_SIZE / 50,
//	         (CPU_STK_SIZE) WIFI_TEST_TASK_STK_SIZE,
//	         (OS_MSG_QTY  ) 16u,
//	         (OS_TICK     ) 0u,
//	         (void       *) 0,
//	         (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//	         (OS_ERR     *)&err);

//#if OS_CFG_DBG_EN
//	DEBUGPRINT("\n\rstart task: used stack = %d, free stack = %d", start_task_TCB.StkUsed, start_task_TCB.StkFree);
//#endif

//	OSTaskDel(NULL, &err);


    freertos_create_task(&wifi_test_task_tcb,"wifi_test_task",256*4,0,WIFI_TEST_TASK_PRIO, wifi_test_task, NULL);

//#endif
}

void lwip_test_main(void)
{
    uint8 spitestbuf[50];
    printf("lwip_test_main\n");
    wifi_spi_init();

//    memset(spitestbuf, 0, 50);

//    spitestbuf[0]= 0x0f;
//    spitestbuf[0]= 0x00;
//    spitestbuf[0]= 0x00;
//    spitestbuf[0]= 0x00;
//
#if 0
//    while(1)
//    {
//        SPISetRwMode(0,0);
//        SPISetRwMode(0,1);
//    }
    while(1)
    {
        spitestbuf[0]= 0x0f;
        spitestbuf[1]= 0x00;
        spitestbuf[2]= 0x00;
        spitestbuf[3]= 0x00;

//        spitestbuf[0]= 0x0f;
//        spitestbuf[1]= 0x04;
//        spitestbuf[2]= 0x04;
//        spitestbuf[3]= 0x10;

        wifi_spi_ctl_bus(1);
        wifi_spi_tx( spitestbuf, 4);
        memset(spitestbuf, 0, 50);
        wifi_spi_rx( spitestbuf, 4);
        if(spitestbuf[0] != 0x40 || spitestbuf[1] != 0x40 || spitestbuf[2] != 0x40 ||spitestbuf[3] != 0x00)
        {
            printf("read reg fail\n");
        }
        wifi_spi_rx( spitestbuf, 4);
        wifi_spi_rx( spitestbuf, 4);
        wifi_spi_ctl_bus(0);
        rkos_sleep(10);
    }
#endif

#ifdef CONFIG_LCD_SHOW
	LCD_ShowString(60,90,200,16,16,"Init Lwip and WiFi ... ");
#endif

	lwip_my_init();
	exti_init();
    //wifi_spi_data_log_enable();
	wifi_main();


}



#ifdef CONFIG_LCD_SHOW
void LCD_show_scan_list(WIFI_NETLINK_INFO_T *wifi_netlink)
{
	uint8 count = 0;
	int8 buffer[MAX_SSID_LEN+8] = {0};
	WIFISUPP_SCAN_AP_INFO_T *p_scan_list = wifi_netlink->scan_list_head;

//	POINT_COLOR = RED;
	LCD_ShowString(60,180,200,16,16,"[WiFi Scan List:]");

//	POINT_COLOR = BLACK;
	LCD_ShowString(60, 200, 200, 12, 12, "------------------------");

	for (count=0; count<13; count++)
		LCD_ShowString(60, (220+count*20), 240, 12, 12, "                                        ");

	count = 0;
	while (p_scan_list != NULL) {
		snprintf((char *)buffer, (p_scan_list->ssid.ssid_len+1), "%s", (char *)&p_scan_list->ssid.ssid);
		switch (p_scan_list->encryp_protocol) {
		case WIFISUPP_ENCRYP_PROTOCOL_OPENSYS:
			snprintf(((char *)buffer+p_scan_list->ssid.ssid_len), sizeof(",Open"), ",Open");
			break;
		case WIFISUPP_ENCRYP_PROTOCOL_WEP:
			snprintf(((char *)buffer+p_scan_list->ssid.ssid_len), sizeof(",WEP"), ",WEP");
			break;
		case WIFISUPP_ENCRYP_PROTOCOL_WPA:
			snprintf(((char *)buffer+p_scan_list->ssid.ssid_len), sizeof(",WPA"), ",WPA");
			break;
		case WIFISUPP_ENCRYP_PROTOCOL_WPA2:
			snprintf(((char *)buffer+p_scan_list->ssid.ssid_len), sizeof(",WPA2"), ",WPA2");
			break;
		default:
			snprintf(((char *)buffer+p_scan_list->ssid.ssid_len), sizeof(",Unkwn"), ",Unkwn");
			break;
		}
		LCD_ShowString(60, (220+count*20), 240, 12, 12, (uint8 *)buffer);
		printf("get ap : %s\n", buffer);
		count++;
		if (count >= 13)
			break;
		p_scan_list = p_scan_list->next_ptr;
	}

//	POINT_COLOR = BLUE;
}
#endif

WIFI_USER_ACTION_TYPE_E user_key_trigger_detected()
{
	uint8 key_value;
	WIFI_USER_ACTION_TYPE_E user_action = WIFI_USER_ACTION_MAX;

//	key_value = KEY_Scan(0);
//   	if (key_value != 0) {
//		switch(key_value)	{
//		case KEY_UP:
//			user_action = WIFI_USER_DISCONNECT_WITH_AP;
//			break;
//		case KEY_LEFT:
//			user_action = WIFI_USER_CONNECT_AP;
//			break;
//		case KEY_DOWN:
//			user_action = WIFI_USER_DUMMY;
//			break;
//		case KEY_RIGHT:
//			user_action = WIFI_USER_ENTER_INTERACTIVE;;
//			break;
//		default:
//			break;
//		}
//	}

	return user_action;
}

void  wifi_test_task (void *p_arg)
{
#ifdef CONFIG_LCD_SHOW
	int	  err;
//	void *msg_ptr = NULL;
    char msg_ptr[20];
	int msg_size = 0;
	WIFI_TEST_STATE_TYPE_E state_type = WIFI_TEST_STATE_DISCONNECTED;
	WIFI_MESSAGE_T	*p_wifi_msg = NULL;
	WIFI_NETLINK_INFO_T *wifi_netlink = NULL;
	uint32  i;
	//uint8  count = 0;
	WIFI_USER_ACTION_TYPE_E user_action;
	uint8  start_softap_mode = FALSE;
	uint8  err_status = 0;
	int8  buffer[MAX_SSID_LEN+8] = {0};
	//struct ip_addr ipaddr;
	//struct ip_addr netmask;
	//struct ip_addr gw;

	wifi_netlink = wifi_netlink_ops.init_func();

//	POINT_COLOR = BLUE;

//    while(1)
//    {
//        freertos_msleep(3000);
//    }

	for (;;) {

		switch (state_type) {

		case WIFI_TEST_STATE_DISCONNECTED:
			LCD_ShowString(60,120,200,16,16,"WiFi State: Disconnected!");
			LCD_ShowString(60,140,240,12,12,"                                       ");
			LCD_ShowString(60,160,240,12,12,"                                       ");
            printf("WiFi State: Disconnected!\n");
			memset(buffer, 0, MAX_SSID_LEN+8);
			freertos_msleep(3000);

			//set scan
			//OSTaskQFlush(NULL, &err);
			xQueueReset(wifi_event_queue_handle);
			wifi_netlink_ops.set_scan_func(wifi_netlink);
			state_type = WIFI_TEST_STATE_SCANNING;
			break;

		case WIFI_TEST_STATE_SCANNING:
			LCD_ShowString(60,120,200,16,16,"WiFi State: Scanning....!");
             printf("WiFi State: Scanning....!\n");
			//msg_ptr = OSTaskQPend(0, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &err);

            err = xQueueReceive(wifi_event_queue_handle, msg_ptr, portMAX_DELAY);
			if (err == pdTRUE) {
				if (msg_ptr != NULL) {
					p_wifi_msg = (WIFI_MESSAGE_T *) msg_ptr;
					if (p_wifi_msg->msg_type == WIFI_MESSAGE_NOTIFY_SCAN_RESULT_SUCC) {
						freertos_msleep(10);
						//get scan list
						wifi_netlink_ops.get_scan_list_func(wifi_netlink);
						LCD_show_scan_list(wifi_netlink);
						state_type =WIFI_TEST_STATE_SCAN_DONE;
					} else if (p_wifi_msg->msg_type == WIFI_MESSAGE_NOTIFY_SCAN_RESULT_FAIL) {
						state_type =WIFI_TEST_STATE_DISCONNECTED;
					}
					//freertos_mfree(msg_ptr);
				}
			} else {
				DEBUGPRINT("WIFI_TEST_STATE_SCANNING: OSTaskQPend error!\n");
			}

			break;

		case WIFI_TEST_STATE_SCAN_DONE:
			LCD_ShowString(60,120,200,16,16,"WiFi State: Scan finish!!");
            printf("WiFi State: Scan finish!!\n");
			for (i=0; i < 3000; i++) {
				user_action = user_key_trigger_detected();
				if (user_action <= WIFI_USER_ACTION_MAX) {
                    user_action = WIFI_USER_CONNECT_AP;
					break;
				} else {
					freertos_msleep(10);
				}
			}
			if ((user_action == WIFI_USER_CONNECT_AP) && (wifi_netlink->scan_ap_num >= 1)) {
				//connect req
				//OSTaskQFlush(NULL, &err);
				xQueueReset(wifi_event_queue_handle);
				LCD_ShowString(60,120,200,16,16,"WiFi State: Connecting...");
                printf("!!!WiFi State: Connecting...\n");
				err_status = wifi_netlink_ops.connect_req_func(wifi_netlink, NULL, NULL);
				if (err_status == 0) {
					state_type = WIFI_TEST_STATE_CONNECTING;
				}
			} else if (user_action == WIFI_USER_ENTER_INTERACTIVE) {
				//enter interactive mode
				state_type = WIFI_TEST_STATE_COMMAND_TESTING;
			} else {
 				//set scan
 				//OSTaskQFlush(NULL, &err);
 				xQueueReset(wifi_event_queue_handle);
 				wifi_netlink_ops.set_scan_func(wifi_netlink);
 				state_type = WIFI_TEST_STATE_SCANNING;
			}

#if 0
			msg_ptr = OSTaskQPend(30000, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &err);
			if (err == OS_ERR_NONE) {
				if (msg_ptr != NULL) {
					wifi_msg = (WIFI_MESSAGE_T) (*msg_ptr);
					freertos_mfree(SRAMIN, msg_ptr);
				}

				if (wifi_msg.msg_type == WIFI_MESSAGE_USER_CONNECT) {
					//connect req
					state_type = WIFI_TEST_STATE_CONNECTING;
				}

			} else if (err == OS_ERR_TIMEOUT) {
 				//set scan
 				wifi_netlink_ops.set_scan_func(wifi_netlink);
 				state_type = WIFI_TEST_STATE_SCANNING;
 			} else {
				DEBUGPRINT("WIFI_TEST_STATE_SCAN_DONE: OSTaskQPend error!\n");
			}
#endif

			break;

		case WIFI_TEST_STATE_CONNECTING:
			//LCD_ShowString(60,120,200,16,16,"WiFi State: Connecting...");
			printf("WiFi State: Connecting...\n");
			freertos_msleep(10);
			//msg_ptr = OSTaskQPend(20000, OS_OPT_PEND_BLOCKING, &msg_size, (CPU_TS *)0, &err);
            err = xQueueReceive(wifi_event_queue_handle, msg_ptr, portMAX_DELAY);
			if (err == pdTRUE) {
				if (msg_ptr != NULL) {
					p_wifi_msg = (WIFI_MESSAGE_T *) msg_ptr;
					if (p_wifi_msg->msg_type == WIFI_MESSAGE_INDICATE_CONNECT) {
						state_type = WIFI_TEST_STATE_CONNECTED;
					} else if (p_wifi_msg->msg_type == WIFI_MESSAGE_INDICATE_DISCONNECT) {
						state_type = WIFI_TEST_STATE_DISCONNECTED;
					}
					//freertos_mfree(msg_ptr);
				}
			}
//            else if (err == OS_ERR_TIMEOUT) {
//  				state_type = WIFI_TEST_STATE_DISCONNECTED;
// 			}
            else {
				DEBUGPRINT("WIFI_TEST_STATE_CONNECTING: OSTaskQPend error!\n");
			}

			break;

		case WIFI_TEST_STATE_CONNECTED:
			LCD_ShowString(60,120,200,16,16,"WiFi State: Connected! :)");
			snprintf((char *)buffer, sizeof("SSID: "), "SSID: ");
			snprintf((char *)buffer+sizeof("SSID: ")-1, wifi_netlink->connected_ap_info.ssid.ssid_len+1, "%s", wifi_netlink->connected_ap_info.ssid.ssid);
			LCD_ShowString(60,140,240,12,12, (uint8 *)buffer);

            printf("%s\n", buffer);
			freertos_msleep(500);
			if (need_do_dhcp) {
				netifapi_dhcp_start(&wifi_netif);
				state_type = WIFI_TEST_STATE_DHCP_DOING;
			}
			break;

		case WIFI_TEST_STATE_DHCP_DOING:
			LCD_ShowString(60,120,200,16,16,"WiFi State: DHCP doing...");
			for (i=0; i < 300; i++) {
				freertos_msleep(100);
				if (wifi_netif.dhcp->state == DHCP_BOUND) {
					dhcp_ip_got = TRUE;
					wifi_netlink_ops.set_ipaddr_func(wifi_netlink, (unsigned int *)&wifi_netif.ip_addr);
					break;
				}
			}

			if (dhcp_ip_got) {
				state_type = WIFI_TEST_STATE_IP_CONFIGED;
			} else {
				DEBUGPRINT("wifi test task: dhcp timeout!\n");
				state_type = WIFI_TEST_STATE_CONNECTED;
			}

			break;

		case WIFI_TEST_STATE_IP_CONFIGED:
			LCD_ShowString(60,120,200,16,16,"WiFi State: IP configed!!");
			snprintf((char *)buffer, sizeof("IP: "), "IP: ");
			snprintf((char *)buffer+sizeof("IP: ")-1, 16, "%s", ip_ntoa(&wifi_netif.ip_addr));
			LCD_ShowString(60,160,240,12,12, (uint8 *)buffer);
            printf("%s\n", buffer);
			for (i=0; i < 1000; i++) {
				user_action = user_key_trigger_detected();
				if (user_action < WIFI_USER_ACTION_MAX) {
					break;
				} else {
					freertos_msleep(10);
     			}
			}

			if (user_action == WIFI_USER_ENTER_INTERACTIVE) {
				//enter interactive mode
				state_type = WIFI_TEST_STATE_COMMAND_TESTING;
			} else if (user_action == WIFI_USER_DISCONNECT_WITH_AP) {
				//disconnect req
				wifi_netlink_ops.disconnect_req_func(wifi_netlink);
				netifapi_dhcp_stop(&wifi_netif);
				memset(&wifi_netif.ip_addr, 0, sizeof(wifi_netif.ip_addr));
				memset(&wifi_netif.gw, 0, sizeof(wifi_netif.gw));
				dhcp_ip_got = FALSE;
				state_type = WIFI_TEST_STATE_DISCONNECTED;
			} else {
				//msg_ptr = OSTaskQPend(1000, OS_OPT_PEND_NON_BLOCKING, &msg_size, (CPU_TS *)0, &err);
                err = xQueueReceive(wifi_event_queue_handle, msg_ptr, portMAX_DELAY);
				if (err == pdTRUE) {
					if (msg_ptr != NULL) {
						p_wifi_msg = (WIFI_MESSAGE_T *) msg_ptr;
						if (p_wifi_msg->msg_type == WIFI_MESSAGE_INDICATE_DISCONNECT) {
							netifapi_dhcp_stop(&wifi_netif);
							memset(&wifi_netif.ip_addr, 0, sizeof(wifi_netif.ip_addr));
							memset(&wifi_netif.gw, 0, sizeof(wifi_netif.gw));
							dhcp_ip_got = FALSE;
							state_type = WIFI_TEST_STATE_DISCONNECTED;
							break;
						}
						//freertos_mfree(msg_ptr);
					}
				}
			}
			break;

		case WIFI_TEST_STATE_COMMAND_TESTING:
			LCD_ShowString(60,120,200,16,16,"WiFi State: Cmd testing");
            printf("WiFi State: Cmd testing");
			command_test_mode();
			if (dhcp_ip_got) {
				state_type = WIFI_TEST_STATE_IP_CONFIGED;
			} else {
				state_type = WIFI_TEST_STATE_DISCONNECTED;
			}
			break;

		default:
			DEBUGPRINT("wifi test task: invalid wifi test state!!!\n");
			break;

		}

		if (start_softap_mode)
			break;
	}

	wifi_netlink_ops.deinit_func(wifi_netlink);

	for (i=0; i<4; i++)
		LCD_ShowString(60,(120+i*20),200,16,16,"                         ");
	for (i=0; i<14; i++)
		LCD_ShowString(60, (200+i*20), 240, 12, 12, "                                        ");

#else

	WIFI_NETLINK_INFO_T *wifi_netlink = NULL;

	wifi_netlink = wifi_netlink_ops.init_func();

	for (;;) {
		command_test_mode();
		freertos_msleep(10000);
	}

	wifi_netlink_ops.deinit_func(wifi_netlink);

#endif
}
#endif

#ifdef CONFIG_WIFI_SOFTAP_MODE

extern int wifi_softap_entry(unsigned char *ssid, unsigned char security, unsigned char *password);
extern void dhcpd_daemon(void);
extern void ttcp_test(int test_mode);

void  wifi_test_task (void *p_arg)
{
	unsigned char security_mode;
	unsigned char ssid[32+1];
	unsigned char password[32+1];
	//WIFI_NETLINK_INFO_T *wifi_netlink = NULL;

#ifdef CONFIG_LCD_SHOW
	LCD_ShowString(60,110,200,16,16,"Start SoftAP...");
#endif

	memcmp(ssid, CONFIG_SOFTAP_SSID, strlen(CONFIG_SOFTAP_SSID)+1);
	security_mode = CONFIG_SECURITY_MODE;
	memcmp(password, CONFIG_SOFTAP_PASSWORD, strlen(CONFIG_SOFTAP_PASSWORD)+1);

	wifi_softap_entry(ssid, security_mode, password);

#ifdef CONFIG_LCD_SHOW
	LCD_ShowString(60,130,200,16,16,"SoftAP: u_soft_ap started.");
	LCD_ShowString(60,150,200,16,16,"IP: 192.168.237.1");
#endif

	dhcpd_daemon();

	//ttcp_test(0);

	//wifi_netlink = wifi_netlink_ops.init_func();

	for (;;) {
		command_test_mode();
		freertos_msleep(10000);
	}

	//wifi_netlink_ops.deinit_func(wifi_netlink);
}
#endif

_os_task  wifi_test_task_handle;
_os_task  wifi_test_task_handle1;

_mutex wifi_test_mutex;
_mutex wifi_test_mutex1;
void api_test_task_timer(uint32 handle)
{
    printf("api_test_task_timer work");
     int i,j;

    i = 0;
    i = j;
}


void  api_test_task (void *p_arg)
{
    _timer timer;
    freertos_init_timer(&timer,NULL, api_test_task_timer,NULL,"api_test_task_timer");
    freertos_set_timer(&timer,50);
    while(1)
    {
        freertos_spin_lock(&wifi_test_mutex1);
        printf("#######################");
        freertos_msleep(500);
        freertos_spin_unlock(&wifi_test_mutex);

    }

}


void api_test_task_timer1(uint32 handle)
{
    printf("api_test_task_timer1dfd work");
    int i,j;

    i = 0;
    i = j;
}
void  api_test_task1 (void *p_arg)
{
    _timer timer;
    freertos_init_timer(&timer,NULL, api_test_task_timer1,NULL,"api_test_task_timer1");
    freertos_set_timer(&timer,100);

    while(1)
    {
        freertos_spin_lock(&wifi_test_mutex);
        printf("!!!!!!!!!!!!!!!!!!!");
        freertos_msleep(500);
        freertos_spin_unlock(&wifi_test_mutex1);

    }

}

void os_api_test()
{
    freertos_spinlock_init(&wifi_test_mutex);
    freertos_spinlock_init(&wifi_test_mutex1);
    freertos_create_task(&wifi_test_task_handle,"wifi_test_task",256*4,0,18, api_test_task, NULL);
    freertos_create_task(&wifi_test_task_handle1,"wifi_test_task",256*4,0,17, api_test_task1, NULL);
}
#pragma arm section code
#endif /*#ifdef __DRIVER_INNOPHACE_WIFI_C__*/