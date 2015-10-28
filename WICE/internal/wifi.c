/*
 * Copyright 2015, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 */

#include <stdlib.h>
#include "ap6181wifi.h"
#include "wiced_wifi.h"
#include "RKOS.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "wiced_network.h"
#include "wwd_events.h"
#include "platform_dct.h"
#include "string.h"
#include "wwd_wifi.h"
#include "../security/BESL/host/WICED/wiced_wps.h"
#include "wiced_utilities.h"
#include "wiced_internal_api.h"
#include "wwd_management.h"
#include "wiced_management.h"
#include "wiced_platform.h"
#include "wiced_framework.h"
#include "../Web/Lwip/lwip-1.4.1/src/include/lwip/api.h"
#include "source_macro.h"
#include "http.h"
#include "easy_setup.h"
/*
#include "devicemanagertask.h"
#include "TaskPlugin.h"
#include "moduleinfotab.h"
#include "fwanalysis.h"
#include "PowerManager.h"
*/
#include "gpio.h"

#ifdef _DRIVER_AP6181_WIFI_C__
/******************************************************
 *                      Macros
 ******************************************************/

#define CHECK_RETURN( expr )  { wiced_result_t check_res = (wiced_result_t)(expr); if ( check_res != WICED_SUCCESS ) { wiced_assert("Command failed\n", 0); return check_res; } }

/******************************************************
 *                    Constants
 ******************************************************/

#define WLC_EVENT_MSG_LINK      (0x01)    /** link is up */

#define SCAN_BSSID_LIST_LENGTH   (100)
#define SCAN_LONGEST_WAIT_TIME  (5000)    /* Dual band devices take over 4000 milliseconds to complete a scan */
#define HANDSHAKE_TIMEOUT_MS    (3000)

#define EASY_SETUP_ENABLE


/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    wiced_scan_result_handler_t results_handler;
    void*                       user_data;
} internal_scan_handler_t;

extern easy_setup_result_t wifi_setup_result;
extern wiced_security_t wifi_setup_security;
/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void*          wiced_link_events_handler ( const wwd_event_header_t* event_header, const uint8_t* event_data, void* handler_user_data );
static void           wiced_scan_result_handler       ( wiced_scan_result_t** result_ptr, void* user_data, wiced_scan_status_t status );
static void           handshake_timeout_handler ( void* arg );
static wiced_result_t handshake_error_callback  ( void* arg );
static void           link_up                   ( void );
static void           link_down                 ( void );
static void           link_renew                ( void );
static void           link_handshake_error      ( void );

static void* softap_event_handler( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@returned@*/void* handler_user_data );

/******************************************************
 *               Variable Definitions
 ******************************************************/

/* Link management variables */

pTimer  TimerHandler;
static const wwd_event_num_t        link_events[]           = { WLC_E_LINK, WLC_E_DEAUTH_IND, WLC_E_ROAM, WLC_E_NONE };
static wiced_bool_t                 wiced_core_initialised  =   WICED_FALSE;
static const wwd_event_num_t        ap_client_events[]      = { WLC_E_DEAUTH_IND, WLC_E_DISASSOC_IND, WLC_E_ASSOC_IND, WLC_E_REASSOC_IND, WLC_E_NONE };
static wiced_bool_t                 wiced_wlan_initialised  =   WICED_FALSE;
wiced_bool_t                 wiced_sta_link_up       =   WICED_FALSE;
static wiced_security_t             wiced_sta_security_type =   WICED_SECURITY_UNKNOWN;
static wiced_timer_t                wiced_sta_handshake_timer;

/* Scanning variables */
static wiced_semaphore_t            scan_semaphore;
static wiced_scan_handler_result_t* off_channel_results;
static wiced_scan_handler_result_t* scan_result_ptr;
static wiced_mac_t*                 scan_bssid_list = NULL;
static int                          current_bssid_list_length = 0;
static void host_platform_wifi_flicker_led(void);
void host_platform_wifi_status_led(uint8 led_status);
void WifiLedStatusIsr(void);

static wiced_wifi_softap_event_handler_t ap_event_handler;
static xSemaphoreHandle wifitest_semaphore;

static uint8* ShellWifiName[] =
{
    "start",
    "dlna",
    "delete",
    "\b",
};

/*******************************************************************************
** Name: host_platform_wifi_status_led
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.8.29
** Time: 10:13:39
*******************************************************************************/
void host_platform_wifi_flicker_led(void)
{
    UINT8 temp;
    temp = Gpio_GetPinLevel(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN);
    Gpio_SetPinLevel(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, !temp);
}

/*******************************************************************************
** Name: KeyDevIsr
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.8.29
** Time: 14:17:17
*******************************************************************************/
void WifiLedStatusIsr(void)
{
    rkos_start_timer(TimerHandler);
    printf("t");
    host_platform_wifi_flicker_led();
}

/*******************************************************************************
** Name: host_platform_wifi_status_led
** Input:void
** Return: void
** Owner:cjh
** Date: 2015.8.29
** Time: 10:13:39
*******************************************************************************/
void host_platform_wifi_status_led(uint8 led_status)
{
    Gpio_SetPinLevel(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, led_status);
}


/*******************************************************************************
** Name: music_shell_ff_resume
** Input:void
** Return: rk_err_t
** Owner:cjh
** Date: 2015.7.23
** Time: 14:52:44
*******************************************************************************/
SHELL FUN rk_err_t wifi_del(uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if (pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if (StrCmpA(pstr, "help", 4) == 0)
        {
            printf("music.del : del wifi \r\n");
            return RK_SUCCESS;
        }
    }
#endif


#if 1
    printf("!!! del wifi fifo \r\n");
    FW_ModuleOverlay(MODULE_ID_AP6181_INIT,MODULE_OVERLAY_CODE);
    Http_Close();
    printf("Http_Close over\n");

    wiced_deinit();

    printf("wifi close over\n");

    FW_RemoveModule(MODULE_ID_AP6181_INIT);

#else
    printf("!!! del wifi fifo \r\n");
    FW_ModuleOverlay(MODULE_ID_AP6181_INIT,MODULE_OVERLAY_CODE);
    wiced_deinit();

    printf("wifi close over\n");

    FW_RemoveModule(MODULE_ID_AP6181_INIT);
#endif

    return RK_SUCCESS;;
}

void wifi_shell_test(void)
{
#ifdef _DRIVER_AP6181_WIFI_C__
#ifndef _JTAG_DEBUG_
    FW_ModuleOverlay(MODULE_ID_AP6181_WIFI, MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_LWIP, MODULE_OVERLAY_CODE);
#endif
    wifi_application_start_test();
#endif
      //rk_printf("while = %dwifi start ok\n",wiced_sta_link_up);
    while (wiced_sta_link_up == WICED_FALSE)
    {
        //rk_printf("while wiced_sta_link_up = %dwifi start ok\n",wiced_sta_link_up);
        host_rtos_delay_milliseconds(1000);
    }
    rk_printf("wifi start ok\n");
    return;
}


rk_err_t wifi_shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;
    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);
    if (StrCnt == 0)
    {
        printf("\n StrCnt = 0 \n");
        return RK_ERROR;
    }
    ret = ShellCheckCmd(ShellWifiName, pItem, StrCnt);
    if (ret < 0)
    {
        printf("\n ret < 0 \n");
        return RK_ERROR;
    }
    i = (uint32)ret;
    pItem += StrCnt;
    pItem++;         //remove '.',the point is the useful item
    switch (i)
    {
        case 0x00:
            wifi_shell_test();
            break;

        case 0x01:
#ifdef __WIFI_DLNA_C__
#ifndef _JTAG_DEBUG__
            FW_ModuleOverlay(MODULE_ID_DLNA,MODULE_OVERLAY_CODE);
            FW_ModuleOverlay(MODULE_ID_UPNP,MODULE_OVERLAY_CODE);
#endif
            RKTaskCreate(TASK_ID_DLNA,0, NULL);
#endif
            break;

         case 0x02:
            ret = wifi_del(pItem);
            break;

        default:
            ret = RK_ERROR;
            break;
    }
    ret = RK_SUCCESS;
    return ret;
}

#pragma arm section code = "ap6181wifiCode", rodata = "ap6181wifiCode", rwdata = "ap6181wifidata", zidata = "ap6181wifidata"

wiced_thread_t task_handle; //rk change 2015-7-27
void wifi_application_thread(void * arg)
{

    int dwRet;

    printf("wifi application test\n");
#ifndef _JTAG_DEBUG_
    FW_ModuleOverlay(MODULE_ID_AP6181_INIT,MODULE_OVERLAY_CODE);
#endif

    wiced_init();
    /* enable protocols */
#ifdef EASY_SETUP_ENABLE
    easy_setup_enable_cooee(); /* broadcom cooee */
    //easy_setup_enable_neeze(); /* broadcom neeze */
    //easy_setup_enable_akiss(); /* wechat akiss */
    //easy_setup_enable_changhong(); /* changhong */

    /* set cooee key */
    //cooee_set_key("1111111111111111");

    /* set airkiss key */
    //akiss_set_key("1111111111111111");

    /* start easy setup */
    if (easy_setup_start() != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("easy setup failed.\r\n"));
    }
    else
    {
        WPRINT_APP_INFO(("easy setup done.\r\n"));
    }
    host_rtos_delay_milliseconds(2000);
#endif
    // wiced_wifi_scan_networks(scan_handler, NULL);
    wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

#ifndef _JTAG_DEBUG_
    FW_RemoveModule(MODULE_ID_AP6181_INIT);
#endif

#ifdef __WICE_HTTP_SERVER__
//    httpd_init();
    //httpd_cgi_init();
#endif

#if 0
#ifdef __WIFI_DLNA_C__
    //host_rtos_delay_milliseconds(100);
#ifndef _JTAG_DEBUG__
    FW_ModuleOverlay(MODULE_ID_DLNA,MODULE_OVERLAY_CODE);
    FW_ModuleOverlay(MODULE_ID_UPNP,MODULE_OVERLAY_CODE);
#endif
    RKTaskCreate(TASK_ID_DLNA,0, NULL);
#endif
#endif

    while (1)
    {
        //wiced_wifi_scan_networks(scan_handler, NULL);
        host_rtos_delay_milliseconds(7000);
    }
}
void wifi_application_start_test()
{
#ifndef  __WICE_WIFITHREAD_C__

    wiced_rtos_create_thread( &task_handle, 10, NULL, wifi_application_thread, 2048*4, NULL); //
#else
    RKTaskCreate(TASK_ID_WIFI_APPLICATION, 0, NULL);

#endif
}

/******************************************************
 *               Function Definitions
 ******************************************************/
_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_init( void )
{
    TimerHandler = NULL;
    Grf_GpioMuxSet(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, Type_Gpio);
    Gpio_SetPinDirection(WIFI_STATE_LED_GPIO_CH, WIFI_STATE_LED_GPIO_PIN, GPIO_OUT);
    host_platform_wifi_status_led(WIFI_START_LED_OFF_STATE);
    printf("wifi LED timer init off\n");

    TimerHandler = rkos_create_timer(100, 100, WifiLedStatusIsr);
    rkos_start_timer(TimerHandler);

    printf("\n wifi LED filcker 1S timer ");

    //FREQ_EnterModule(FREQ_MAX);
    WPRINT_WICED_INFO( ("\nStarting WICED v" WICED_VERSION "\n") );

    CHECK_RETURN( wiced_core_init( ) );

    CHECK_RETURN( wiced_wlan_connectivity_init( ) );

    return WICED_SUCCESS;
}


static wiced_result_t scan_handler( wiced_scan_handler_result_t* malloced_scan_result )
{
#if 0
    /* Check if scan is finished (Invalid scan result) */
    if (malloced_scan_result->scan_complete != WICED_TRUE)
    {
        char temp_buffer[70];
        char* temp_ptr;
        uint16_t temp_length;
        int i;

        wiced_tcp_stream_t* stream = scan_data->stream;

        /* Result ID */
        temp_length = (uint16_t) sprintf( temp_buffer, "%d\n", scan_data->result_count );
        scan_data->result_count++;
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);

        /* SSID */
        temp_ptr = temp_buffer;
        for ( i = 0; i < malloced_scan_result->ap_details.SSID.len; i++)
        {
            temp_ptr += sprintf( temp_ptr, "%02X", malloced_scan_result->ap_details.SSID.val[i] );
        }
        temp_ptr += sprintf( temp_ptr, "\n" );

        wiced_tcp_stream_write(stream, temp_buffer, (uint32_t)( temp_ptr - temp_buffer ) );

        /* Security */
        temp_length = (uint16_t) sprintf( temp_buffer, "%d\n", malloced_scan_result->ap_details.security );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);
        temp_length = (uint16_t) sprintf( temp_buffer, "%s\n", (   malloced_scan_result->ap_details.security == WICED_SECURITY_OPEN )? "OPEN" :
                                          ( ( malloced_scan_result->ap_details.security & WEP_ENABLED   ) != 0 )? "WEP"  :
                                          ( ( malloced_scan_result->ap_details.security & WPA_SECURITY  ) != 0 )? "WPA"  :
                                          ( ( malloced_scan_result->ap_details.security & WPA2_SECURITY ) != 0 )? "WPA2" : "UNKNOWN" );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);

        /* RSSI */
        temp_length = (uint16_t) sprintf( temp_buffer, "%d\n", malloced_scan_result->ap_details.signal_strength );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);

        /* Channel */
        temp_length = (uint16_t) sprintf( temp_buffer, "%d\n", malloced_scan_result->ap_details.channel );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);

        /* BSSID */
        temp_length = (uint16_t) sprintf( temp_buffer, "%02X%02X%02X%02X%02X%02X\n", malloced_scan_result->ap_details.BSSID.octet[0], malloced_scan_result->ap_details.BSSID.octet[1], malloced_scan_result->ap_details.BSSID.octet[2], malloced_scan_result->ap_details.BSSID.octet[3], malloced_scan_result->ap_details.BSSID.octet[4], malloced_scan_result->ap_details.BSSID.octet[5] );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);

        /* Remembered */
        temp_length = (uint16_t) sprintf( temp_buffer, "%d\n", 0 );
        wiced_tcp_stream_write(stream, temp_buffer, temp_length);


    }
#endif
    vPortFree(malloced_scan_result);

    return WICED_SUCCESS;
}

#if 0
uint8 tcp_buf[2048];
void ap6181tcp_test_task(void *arg)
{

    struct tcp_pcb *pcb;
    struct netconn *conn;
    struct netconn *conn_new;
    struct netbuf * buf;
    ip_addr_t lipaddr;
    err_t err;

    conn = netconn_new(NETCONN_TCP);
    if (conn == NULL)
    {
        printf("netconn_new alloc fail\n");

        while (1)
        {
            host_rtos_delay_milliseconds(100);
        }
    }

    netconn_bind(conn , IP_ADDR_ANY, 90);

    netconn_listen(conn);

    netconn_accept(conn, &conn_new);
    while (1)
    {
        if (netconn_recv(conn_new, &buf) == 0)
        {
            netbuf_delete(buf);
        }
        else
        {
            printf("netconn_close\n");
            netconn_close(conn_new);
            netconn_delete(conn_new);
            netconn_accept(conn, &conn_new);
            printf("netconn_accept\n");

        }
    }
}
void ap6181tcp_test_init(void)
{
    wiced_thread_t   tcp_task_tcb;
    host_rtos_create_thread(&tcp_task_tcb, ap6181tcp_test_task, "ap6181tcp_test_task", NULL, 512, 55);
}
#endif

_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_core_init( void )
{
    if ( wiced_core_initialised == WICED_TRUE )
    {
        return WICED_SUCCESS;
    }

    CHECK_RETURN( wiced_platform_init( ) );

    CHECK_RETURN( wiced_rtos_init( ) );

    wiced_core_initialised = WICED_TRUE;
    host_rtos_delay_milliseconds(1000);

    return WICED_SUCCESS;
}
//rk change
#if 0
wiced_result_t wiced_wlan_connectivity_init( void )
{
    wiced_result_t              result;
    wiced_country_code_t        country;
    platform_dct_wifi_config_t* wifi_config;

#ifdef WPRINT_ENABLE_NETWORK_INFO
    wiced_mac_t  mac;
    char         version[200];
#endif

    if ( wiced_wlan_initialised == WICED_TRUE )
    {
        return WICED_SUCCESS;
    }

    CHECK_RETURN( wiced_network_init( ) );

    /* Initialise Wiced */
    CHECK_RETURN( wiced_dct_read_lock( (void**) &wifi_config, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, 0, sizeof(platform_dct_wifi_config_t) ) );

    if (wifi_config->device_configured == WICED_TRUE)
    {
        country = wifi_config->country_code;
    }
    else
    {
        country = WICED_COUNTRY_UNITED_STATES;
    }

    wiced_dct_read_unlock( wifi_config, WICED_FALSE );

    result = (wiced_result_t) wwd_management_init( country, wiced_packet_pools );
    if ( result != WICED_SUCCESS )
    {
        WPRINT_NETWORK_ERROR( ("Error %d while starting WICED!\n", result) );
        return result;
    }
#define BUS "SDIO"
    WPRINT_NETWORK_INFO( ( "WWD " BUS " interface initialised\n") );

    CHECK_RETURN( wiced_rtos_init_semaphore( &scan_semaphore ) );

    CHECK_RETURN( wiced_rtos_set_semaphore( &scan_semaphore ) ); /* Semaphore starts at 1 */

    CHECK_RETURN( wiced_rtos_init_timer( &wiced_sta_handshake_timer, HANDSHAKE_TIMEOUT_MS, handshake_timeout_handler, 0 ) );

#ifdef WPRINT_ENABLE_NETWORK_INFO
    {
        wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE );
        WPRINT_NETWORK_INFO(("WLAN MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]));

        memset( version, 0, sizeof( version ) );
        wwd_wifi_get_wifi_version( version, sizeof( version ) );
        WPRINT_NETWORK_INFO( ("WLAN Firmware    : %s\r\n", version ) );
    }
#endif

    /* Configure roaming parameters */
    wwd_wifi_set_roam_trigger( WICED_WIFI_ROAMING_TRIGGER_MODE );
    wwd_wifi_set_roam_delta( WICED_WIFI_ROAMING_TRIGGER_DELTA_IN_DBM );
    wwd_wifi_set_roam_scan_period( WICED_WIFI_ROAMING_SCAN_PERIOD_IN_SECONDS );

    wiced_wlan_initialised = WICED_TRUE;

    ap_event_handler = NULL;

    return WICED_SUCCESS;
}
#else
_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_wlan_connectivity_init( void )
{
    wiced_result_t              result;
    wiced_country_code_t        country;
    wiced_mac_t    mac;
#ifdef WPRINT_ENABLE_NETWORK_INFO
    char                        version[200];
#endif

    CHECK_RETURN( wiced_network_init( ) );
    country = WICED_COUNTRY_UNITED_STATES;
    result = wwd_management_init( country, wiced_packet_pools );
    if ( result != WICED_SUCCESS )
    {
        WPRINT_NETWORK_ERROR( ("Error %d while starting WICED!\n", result) );
        return result;
    }

#define BUS "SDIO"

    WPRINT_NETWORK_INFO( ( "WWD " BUS " interface initialised\n") );

    CHECK_RETURN( wiced_rtos_init_semaphore( &scan_semaphore ) );

    CHECK_RETURN( wiced_rtos_set_semaphore( &scan_semaphore ) ); /* Semaphore starts at 1 */

    CHECK_RETURN( wiced_rtos_init_timer( &wiced_sta_handshake_timer, HANDSHAKE_TIMEOUT_MS, handshake_timeout_handler, 0 ) );

#if 1//def WPRINT_ENABLE_NETWORK_INFO
    wwd_wifi_get_mac_address( &mac, WWD_STA_INTERFACE );
    WPRINT_NETWORK_INFO(("WLAN MAC Address : %02X:%02X:%02X:%02X:%02X:%02X\r\n", mac.octet[0],mac.octet[1],mac.octet[2],mac.octet[3],mac.octet[4],mac.octet[5]));

    memset( version, 0, sizeof( version ) );
    wwd_wifi_get_wifi_version( version, sizeof( version ) );
    WPRINT_NETWORK_INFO( ("WLAN Firmware    : %s\r\n", version ) );
#endif
    wiced_wlan_initialised = WICED_TRUE;
    return WICED_SUCCESS;
}

#endif

_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_core_deinit( void )
{
    if ( wiced_core_initialised != WICED_TRUE )
    {
        return WICED_SUCCESS;
    }

    wiced_rtos_deinit();

    wiced_core_initialised = WICED_FALSE;

    return WICED_SUCCESS;
}

_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_wlan_connectivity_deinit( void )
{
    if ( wiced_wlan_initialised != WICED_TRUE )
    {
        return WICED_SUCCESS;
    }

    // wiced_network_down( WICED_AP_INTERFACE );
    wiced_network_down( WICED_STA_INTERFACE );

    wiced_rtos_deinit_timer( &wiced_sta_handshake_timer );

    wiced_rtos_deinit_semaphore( &scan_semaphore );

    wwd_management_wifi_off( );

    wiced_network_deinit( );

    wiced_wlan_initialised = WICED_FALSE;

    return WICED_SUCCESS;
}

_ap6181WICE_WIFI_INIT_
wiced_result_t wiced_deinit( void )
{
    //rkos_delete_timer(internal_scan_handler_t->TimerHandler);
    wiced_wlan_connectivity_deinit( );
    wiced_core_deinit( );
    //FREQ_ExitModule(FREQ_MAX);
#ifndef  __WICE_WIFITHREAD_C__
    wiced_rtos_delete_thread(&task_handle);
#else
    RKTaskDelete(TASK_ID_WIFI_APPLICATION, 0, SYNC_MODE);
#endif
#ifndef _JTAG_DEBUG_
    FW_RemoveModule(MODULE_ID_AP6181_WIFI);
    FW_RemoveModule(MODULE_ID_LWIP);
#endif
    return WICED_SUCCESS;
}


#ifdef RK_WIFI_AP
wiced_result_t wiced_start_ap( wiced_ssid_t* ssid, wiced_security_t security, const char* key, uint8_t channel )
{
    return (wiced_result_t) wwd_wifi_start_ap(ssid, security, (uint8_t*)key, (uint8_t) strlen(key), channel);
}

wiced_result_t wiced_stop_ap( void )
{
    return (wiced_result_t) wwd_wifi_stop_ap();
}

#endif
wiced_result_t wiced_wifi_register_softap_event_handler( wiced_wifi_softap_event_handler_t event_handler )
{
    ap_event_handler = event_handler;
    return wwd_management_set_event_handler( ap_client_events, softap_event_handler, NULL, WWD_AP_INTERFACE );
}

wiced_result_t wiced_enable_powersave( void )
{
    wiced_result_t result;

    /* Enable MCU powersave */
    wiced_platform_mcu_enable_powersave();

    /* Enable Wi-Fi powersave */
    result = wiced_wifi_enable_powersave();
    if (result != WICED_SUCCESS)
    {
        goto done;
    }

    /* Suspend networking timers */
    result = wiced_network_suspend();

done:
    return result;
}

wiced_result_t wiced_disable_powersave( void )
{
    wiced_network_resume();
    wiced_wifi_disable_powersave();
    wiced_platform_mcu_disable_powersave();
    return WICED_SUCCESS;
}

/** Join a Wi-Fi network as a client
 *      The AP SSID/Passphrase credentials used to join the AP must be available in the DCT
 *      Use the low-level wiced_wifi_join() API to directly join a specific AP if required
 *
 * @return WICED_SUCCESS : Connection to the AP was successful
 *         WICED_ERROR   : Connection to the AP was NOT successful
 */

wiced_result_t wiced_join_ap( void )
{
    unsigned int             a;
    int                      retries;
    wiced_result_t result = WICED_NO_STORED_AP_IN_DCT;
    wiced_result_t      join_result = WICED_ERROR;

#ifndef EASY_SETUP_ENABLE
    const wiced_ssid_t WLAN0 = {13, "cpsb-lyb-test"};
    //const wiced_ssid_t WLAN0 = {8, "TIANTIAN"};
    //const wiced_ssid_t WLAN0 = {24, "nanod_lowpower_wifi_test"};
#else
    //const wiced_ssid_t WLAN0 = {13, "cpsb-lyb-test"};
     //const wiced_ssid_t WLAN0 = {8, "TIANTIAN"};
    printf("ssid= %s,len = %d\npassword = %s,passlen = %d\n",wifi_setup_result.ap_ssid.val,wifi_setup_result.ap_ssid.len, wifi_setup_result.security_key,wifi_setup_result.security_key_length);
    printf("secruty = %x\n", wifi_setup_security);
#endif
//rk change
#if 0
    wiced_config_ap_entry_t* ap;
    wiced_result_t           result = WICED_NO_STORED_AP_IN_DCT;

    for ( retries = WICED_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
    {
        /* Try all stored APs */
        for ( a = 0; a < CONFIG_AP_LIST_SIZE; ++a )
        {
            /* Check if the entry is valid */
            CHECK_RETURN( wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) , sizeof(wiced_config_ap_entry_t) ) );
            if ( ap->details.SSID.length != 0 )
            {
                result = wiced_join_ap_specific( &ap->details, ap->security_key_length, ap->security_key );

#ifdef FIRMWARE_WITH_PMK_CALC_SUPPORT
                if ( result == WICED_SUCCESS )
                {
                    /* Extract the calculated PMK and store it in the DCT to speed up future associations */
                    if ( ( ap->security_key_length != WSEC_MAX_PSK_LEN ) && ( ( ap->details.security & IBSS_ENABLED ) == 0 ) )
                    {
                        wiced_config_ap_entry_t ap_temp;
                        memcpy( &ap_temp, ap, sizeof(wiced_config_ap_entry_t) );
                        if ( wwd_wifi_get_pmk( ap_temp.security_key, ap_temp.security_key_length, ap_temp.security_key ) == WWD_SUCCESS )
                        {
                            ap_temp.security_key_length = WSEC_MAX_PSK_LEN;
                            if ( WICED_SUCCESS != wiced_dct_write( &ap_temp, DCT_WIFI_CONFIG_SECTION, OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) , sizeof(wiced_config_ap_entry_t) ) )
                            {
                                WPRINT_WICED_INFO(("Failed to write ap list to DCT: \n"));
                            }

                        }
                    }
                }
#endif /* FIRMWARE_WITH_PMK_CALC_SUPPORT */
            }

            wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );

            if ( result == WICED_SUCCESS )
            {
                return result;
            }
        }
    }
#endif
    if ( join_result != WICED_SUCCESS )
    {
        /* If join-specific failed, try scan and join AP */
        //join_result = wwd_wifi_join( (char*) ap->details.SSID.val, ap->details.security, (uint8_t*) ap->security_key, ap->security_key_length, NULL );
        WPRINT_NETWORK_INFO(("wwd_wifi_join start\r\n"));
#ifdef EASY_SETUP_ENABLE
     //join_result = wwd_wifi_join(&WLAN0, WICED_SECURITY_OPEN, NULL, 0, NULL );
     //join_result = wwd_wifi_join(&WLAN0, WICED_SECURITY_WPA_AES_PSK, "tiantian", 8, NULL );
    if(wifi_setup_result.security_key_length == 0)
    {
        //printf("security_key_length = %d\n", wifi_setup_result.security_key_length);
        join_result = wwd_wifi_join((wiced_ssid_t *)&(wifi_setup_result.ap_ssid), WICED_SECURITY_OPEN, NULL, 0, NULL );
    }
    else
    {
        join_result = wwd_wifi_join((wiced_ssid_t *)&(wifi_setup_result.ap_ssid), wifi_setup_security, wifi_setup_result.security_key, wifi_setup_result.security_key_length, NULL );
    }
#else
        join_result = wwd_wifi_join(&WLAN0, WICED_SECURITY_OPEN, NULL, 0, NULL );
       //join_result = wwd_wifi_join(&WLAN0, WICED_SECURITY_WPA_AES_PSK, "tiantian", 8, NULL );
#endif
        WPRINT_NETWORK_INFO(("wwd_wifi_join end\r\n"));
    }

    //  rkos_sleep(5);
    if ( join_result == WICED_SUCCESS )
    {

        WPRINT_NETWORK_INFO(("link_up\n"));
        link_up();

        wwd_management_set_event_handler( link_events, wiced_link_events_handler, NULL, WWD_STA_INTERFACE );

        return WICED_SUCCESS;
    }
    else
    {
        printf("Failed to join\n");
    }

    return result;

}

wiced_result_t wiced_join_ap_specific( wiced_ap_info_t* details, uint8_t security_key_length, const char security_key[ 64 ] )
{

    wiced_result_t      join_result = WICED_STA_JOIN_FAILED;
    wiced_scan_result_t temp_scan_result;

    WPRINT_WICED_INFO(("Joining : %s\n", (char*)details->SSID.value));

    memcpy( &temp_scan_result, details, sizeof( *details ) );

    /* Try join AP with last know specific details */
    if ( !( NULL_MAC(details->BSSID.octet) ) && details->channel != 0 )
    {
        join_result = (wiced_result_t) wwd_wifi_join_specific( &temp_scan_result, (uint8_t*) security_key, security_key_length, NULL, WWD_STA_INTERFACE );
    }

    if ( join_result != WICED_SUCCESS )
    {
        wiced_security_t security;

        security = details->security;
        if (details->bss_type == WICED_BSS_TYPE_ADHOC)
        {
            security |= IBSS_ENABLED;
        }

        /* If join-specific failed, try scan and join AP */
        join_result = (wiced_result_t) wwd_wifi_join( &details->SSID, security, (uint8_t*) security_key, security_key_length, NULL );
    }

    if ( join_result == WICED_SUCCESS )
    {
        WPRINT_WICED_INFO( ( "Successfully joined : %s\n", (char*)details->SSID.value ) );

        wiced_sta_link_up       = WICED_TRUE;
        wiced_sta_security_type = details->security;

        wwd_management_set_event_handler( link_events, wiced_link_events_handler, NULL, WWD_STA_INTERFACE );
        return WICED_SUCCESS;
    }
    else
    {
        WPRINT_WICED_INFO(("Failed to join: %s\n", (char*)details->SSID.value));
    }
    return join_result;
}


wiced_result_t wiced_leave_ap( wiced_interface_t interface )
{
    /* Deregister the link event handler and leave the current AP */
    wwd_management_set_event_handler(link_events, NULL, 0, interface);
    wwd_wifi_leave( interface );
    if ( interface == WICED_STA_INTERFACE )
    {
        rk_printf("wifi leave LED flicker");
        TimerHandler = rkos_create_timer(1000, 1000, WifiLedStatusIsr);
        rkos_start_timer(TimerHandler);
        wiced_sta_link_up = WICED_FALSE;
    }
    return WICED_SUCCESS;
}

wiced_result_t wiced_wifi_scan_networks( wiced_scan_result_handler_t results_handler, void* user_data )
{
    static internal_scan_handler_t scan_handler;
    wiced_result_t result;
    const uint16_t chlist[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,0 };
    const wiced_scan_extended_params_t extparam = { 5, 110, 110, 50 };

    wiced_assert("Bad args", results_handler != NULL);

    /* Initialise the semaphore that will prevent simultaneous access - cannot be a mutex, since
     * we don't want to allow the same thread to start a new scan */
    result = wiced_rtos_get_semaphore( &scan_semaphore, SCAN_LONGEST_WAIT_TIME );
    if ( result != WICED_SUCCESS )
    {
        /* Return error result, but set the semaphore to work the next time */
        wiced_rtos_set_semaphore( &scan_semaphore );
        return result;
    }

    off_channel_results = NULL;
    current_bssid_list_length = 0;

    /* Prepare space to keep track of seen BSSIDs */
    if (scan_bssid_list != NULL)
    {
        vPortFree(scan_bssid_list);
    }
    scan_bssid_list = (wiced_mac_t*) pvPortMalloc(SCAN_BSSID_LIST_LENGTH * sizeof(wiced_mac_t));
    if (scan_bssid_list == NULL)
    {
        goto exit;
    }
    memset(scan_bssid_list, 0, SCAN_BSSID_LIST_LENGTH * sizeof(wiced_mac_t));

    /* Convert function pointer to object so it can be passed around */
    scan_handler.results_handler  = results_handler;
    scan_handler.user_data        = user_data;

    /* Initiate scan */
    scan_result_ptr = MALLOC_OBJECT("scan result", wiced_scan_handler_result_t);
    if (scan_result_ptr == NULL)
    {
        goto error_with_bssid_list;
    }
    memset( scan_result_ptr, 0, sizeof( wiced_scan_handler_result_t ) );
    scan_result_ptr->status    = WICED_SCAN_INCOMPLETE;
    scan_result_ptr->user_data = user_data;

    if ( wwd_wifi_scan( WICED_SCAN_TYPE_ACTIVE, WICED_BSS_TYPE_ANY, NULL, NULL, chlist, &extparam, wiced_scan_result_handler, (wiced_scan_result_t**) &scan_result_ptr, &scan_handler, WWD_STA_INTERFACE ) != WWD_SUCCESS )
    {
        goto error_with_result_ptr;
    }

    return WICED_SUCCESS;

error_with_result_ptr:
    vPortFree(scan_result_ptr);
    scan_result_ptr = NULL;

error_with_bssid_list:
    vPortFree(scan_bssid_list);
    scan_bssid_list = NULL;
exit:
    wiced_rtos_set_semaphore(&scan_semaphore);
    return WICED_ERROR;
}

wiced_result_t wiced_wifi_add_custom_ie( wiced_interface_t interface, const wiced_custom_ie_info_t* ie_info )
{
    wiced_assert("Bad args", ie_info != NULL);

    return (wiced_result_t) wwd_wifi_manage_custom_ie( interface, WICED_ADD_CUSTOM_IE, (const uint8_t*)ie_info->oui, ie_info->subtype, (const void*)ie_info->data, ie_info->length, ie_info->which_packets );
}

wiced_result_t wiced_wifi_remove_custom_ie( wiced_interface_t interface, const wiced_custom_ie_info_t* ie_info )
{
    wiced_assert("Bad args", ie_info != NULL);

    return (wiced_result_t) wwd_wifi_manage_custom_ie( interface, WICED_REMOVE_CUSTOM_IE, (const uint8_t*)ie_info->oui, ie_info->subtype, (const void*)ie_info->data, ie_info->length, ie_info->which_packets );
}

/*rk del start*/
#if 0
wiced_result_t wiced_wps_enrollee(wiced_wps_mode_t mode, const wiced_wps_device_detail_t* details, const char* password, wiced_wps_credential_t* credentials, uint16_t credential_count)
{
    wiced_result_t result    = WICED_SUCCESS;
    wps_agent_t*   workspace = (wps_agent_t*) calloc_named("wps", 1, sizeof(wps_agent_t));

    if ( workspace == NULL )
    {
        return WICED_OUT_OF_HEAP_SPACE;
    }

    besl_wps_init ( workspace, (besl_wps_device_detail_t*) details, WPS_ENROLLEE_AGENT, WWD_STA_INTERFACE );
    result = (wiced_result_t) besl_wps_start( workspace, (besl_wps_mode_t) mode, password, (besl_wps_credential_t*) credentials, credential_count );
    host_rtos_delay_milliseconds( 10 ); /* Delay required to allow WPS thread to run and initialize */
    if ( result == WICED_SUCCESS )
    {
        besl_wps_wait_till_complete( workspace );
        result = (wiced_result_t) besl_wps_get_result( workspace );
    }

    besl_wps_deinit( workspace );
    vPortFree( workspace );
    workspace = NULL;

    return result;
}

wiced_result_t wiced_wps_registrar(wiced_wps_mode_t mode, const wiced_wps_device_detail_t* details, const char* password, wiced_wps_credential_t* credentials, uint16_t credential_count)
{
    wiced_result_t result    = WICED_SUCCESS;
    wps_agent_t*   workspace = (wps_agent_t*) calloc_named("wps", 1, sizeof(wps_agent_t));

    if ( workspace == NULL )
    {
        return WICED_OUT_OF_HEAP_SPACE;
    }

    besl_wps_init ( workspace, (besl_wps_device_detail_t*) details, WPS_REGISTRAR_AGENT, WWD_AP_INTERFACE );
    result = (wiced_result_t) besl_wps_start( workspace, (besl_wps_mode_t) mode, password, (besl_wps_credential_t*) credentials, credential_count );
    if ( result == WICED_SUCCESS )
    {
        besl_wps_wait_till_complete( workspace );
        result = (wiced_result_t) besl_wps_get_result( workspace );
    }

    besl_wps_deinit( workspace );
    vPortFree( workspace );
    workspace = NULL;

    return result;
}
#endif
/*rk del end*/

/******************************************************
 *                    Static Functions
 ******************************************************/

static void link_up( void )
{
    if ( wiced_sta_link_up == WICED_FALSE )
    {

        wiced_network_notify_link_up( WICED_STA_INTERFACE );
        wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, wiced_wireless_link_up_handler, 0 );
        wiced_sta_link_up = WICED_TRUE;

        rkos_delete_timer(TimerHandler);
        rk_printf("\n wifi link_up led on\n");
        host_platform_wifi_status_led(WIFI_START_LED_ON_STATE);
    }
}

static void link_down( void )
{
    if ( wiced_sta_link_up == WICED_TRUE )
    {
        /* Notify network stack that the link is down. Further processing will be done in the link down handler */
        wiced_network_notify_link_down( WICED_STA_INTERFACE );

        /* Force awake the networking thread. It might still be blocked on receive or send timeouts */
        wiced_rtos_thread_force_awake( &(WICED_NETWORKING_WORKER_THREAD->thread) );

        wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, wiced_wireless_link_down_handler, 0 );
        wiced_sta_link_up = WICED_FALSE;
    }
}

static void link_renew( void )
{
    if ( wiced_sta_link_up == WICED_TRUE )
    {
        wiced_wireless_link_renew_handler();
    }
}

static void link_handshake_error( void )
{
    wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, handshake_error_callback, 0 );
}

static wiced_result_t handshake_error_callback( void* arg )
{
    unsigned int   a;
    int   retries;
    wiced_config_ap_entry_t* ap;

    UNUSED_PARAMETER( arg );
    printf("33333333333event_header->event_type\n");
    /* Explicitly leave AP and then rejoin */
    wiced_leave_ap( WICED_STA_INTERFACE );

    for ( retries = WICED_JOIN_RETRY_ATTEMPTS; retries != 0; --retries )
    {
        /* Try all stored APs */
        for ( a = 0; a < CONFIG_AP_LIST_SIZE; ++a )
        {
            /* Check if the entry is valid */
            CHECK_RETURN( wiced_dct_read_lock( (void**) &ap, WICED_FALSE, DCT_WIFI_CONFIG_SECTION, OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) , sizeof(wiced_config_ap_entry_t) ) );
            if ( ap->details.SSID.length != 0 )
            {
                wiced_result_t      join_result = WICED_ERROR;
                wiced_scan_result_t temp_scan_result;

                memcpy( &temp_scan_result, &ap->details, sizeof( ap->details ) );

                /* Try join AP with last know specific details */
                if ( !( NULL_MAC(ap->details.BSSID.octet) ) && ap->details.channel != 0 )
                {
                    join_result = (wiced_result_t) wwd_wifi_join_specific( &temp_scan_result, (uint8_t*) ap->security_key, ap->security_key_length, NULL, WWD_STA_INTERFACE );
                }

                if ( join_result != WICED_SUCCESS )
                {
                    /* If join-specific failed, try scan and join AP */
                    join_result = (wiced_result_t) wwd_wifi_join( &ap->details.SSID, ap->details.security, (uint8_t*) ap->security_key, ap->security_key_length, NULL );
                }

                if ( join_result == WICED_SUCCESS )
                {
                    link_up();
                    wiced_sta_security_type = ap->details.security;

                    wwd_management_set_event_handler( link_events, wiced_link_events_handler, NULL, WWD_STA_INTERFACE );

#ifdef FIRMWARE_WITH_PMK_CALC_SUPPORT
                    /* Extract the calculated PMK and store it in the DCT to speed up future associations */
                    if ( ap->security_key_length != WSEC_MAX_PSK_LEN )
                    {
                        wiced_config_ap_entry_t ap_temp;
                        memcpy( &ap_temp, ap, sizeof(wiced_config_ap_entry_t) );
                        if ( wwd_wifi_get_pmk( ap_temp.security_key, ap_temp.security_key_length, ap_temp.security_key ) == WWD_SUCCESS )
                        {
                            wiced_dct_read_unlock( ap, WICED_FALSE );
                            ap_temp.security_key_length = WSEC_MAX_PSK_LEN;
                            if ( WICED_SUCCESS != wiced_dct_write( &ap_temp, DCT_WIFI_CONFIG_SECTION, OFFSETOF(platform_dct_wifi_config_t,stored_ap_list) + a * sizeof(wiced_config_ap_entry_t) , sizeof(wiced_config_ap_entry_t) ) )
                            {
                                WPRINT_WICED_INFO(("Failed to write ap list to DCT: \n"));
                            }

                        }
                    }
#endif /* FIRMWARE_WITH_PMK_CALC_SUPPORT */

                    wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );
                    printf("555555555555555555event_header->event_type\n");
                    return WICED_SUCCESS;
                }
            }
            wiced_dct_read_unlock( (wiced_config_ap_entry_t*) ap, WICED_FALSE );
        }
    }
    printf("44444444444444event_header->event_type\n");
    return WICED_STA_JOIN_FAILED;
}

static void handshake_timeout_handler( void* arg )
{
    UNUSED_PARAMETER( arg );

    wiced_rtos_stop_timer( &wiced_sta_handshake_timer );

    /* This RTOS timer callback runs in interrupt context. Defer event management to WICED_NETWORKING_WORKER_THREAD */
    wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, handshake_error_callback, 0 );
}

static void* wiced_link_events_handler( const wwd_event_header_t* event_header, const uint8_t* event_data, void* handler_user_data )
{
    UNUSED_PARAMETER( event_data );

    if ( event_header->interface != (uint8_t) WICED_STA_INTERFACE )
    {
        return handler_user_data;
    }
    printf("event_header->event_type = %d\n",event_header->event_type);

    switch ( event_header->event_type )
    {
        case WLC_E_ROAM:
            /* when roam attempt completed successfully, we will renew the existing link */
            /* otherwise ignore all roam events */
            /* please keep in mind that if roaming was successful wlan chip wont send any link down event to the host */
            /* driver */
            if ( event_header->status == WLC_E_STATUS_SUCCESS )
            {
                link_renew( );
            }
            break;

        case WLC_E_LINK:
            if ( ( event_header->flags & WLC_EVENT_MSG_LINK ) != 0 )
            {
                switch ( wiced_sta_security_type )
                {
                    case WICED_SECURITY_OPEN:
                    case WICED_SECURITY_IBSS_OPEN:
                    case WICED_SECURITY_WPS_OPEN:
                    case WICED_SECURITY_WPS_SECURE:
                    case WICED_SECURITY_WEP_PSK:
                    case WICED_SECURITY_WEP_SHARED:
                        {
                            /* Advertise link-up immediately as no EAPOL is required */
                            link_up();
                            break;
                        }

                    case WICED_SECURITY_WPA_TKIP_PSK:
                    case WICED_SECURITY_WPA_AES_PSK:
                    case WICED_SECURITY_WPA_MIXED_PSK:
                    case WICED_SECURITY_WPA2_AES_PSK:
                    case WICED_SECURITY_WPA2_TKIP_PSK:
                    case WICED_SECURITY_WPA2_MIXED_PSK:
                    case WICED_SECURITY_WPA_TKIP_ENT:
                    case WICED_SECURITY_WPA_AES_ENT:
                    case WICED_SECURITY_WPA_MIXED_ENT:
                    case WICED_SECURITY_WPA2_TKIP_ENT:
                    case WICED_SECURITY_WPA2_AES_ENT:
                    case WICED_SECURITY_WPA2_MIXED_ENT:
                        {
                            /* Start a timer and wait for WLC_E_PSK_SUP event */
                            wiced_rtos_start_timer( &wiced_sta_handshake_timer );
                            break;
                        }

                    case WICED_SECURITY_UNKNOWN:
                    case WICED_SECURITY_FORCE_32_BIT:
                    default:
                        {
                            printf("bad security type\r\n");
                            //wiced_assert( "Bad Security type\r\n", 0 != 0 );
                            break;
                        }
                }
            }
            else
            {
                link_down( );
            }
            break;

        case WLC_E_DEAUTH_IND:
            link_down( );
            break;

        case WLC_E_PSK_SUP:
            {
                /* WLC_E_PSK_SUP is received. Stop timer, check for status and handle appropriately */
                wiced_rtos_stop_timer( &wiced_sta_handshake_timer );

                if ( event_header->status == WLC_SUP_KEYED )
                {
                    /* Successful WPA key exchange. Announce link up event to application */
                    link_up();
                }
                else if ( event_header->status != WLC_SUP_LAST_BASIC_STATE && event_header->status != WLC_SUP_KEYXCHANGE )
                {
                    /* WPA handshake error */
                    link_handshake_error();
                }
                break;
            }


            /* Note - These are listed to keep gcc pedantic checking happy */
        case WLC_E_NONE:
        case WLC_E_SET_SSID:
        case WLC_E_JOIN:
        case WLC_E_START:
        case WLC_E_AUTH:
        case WLC_E_AUTH_IND:
        case WLC_E_DEAUTH:
        case WLC_E_ASSOC:
        case WLC_E_ASSOC_IND:
        case WLC_E_REASSOC:
        case WLC_E_REASSOC_IND:
        case WLC_E_DISASSOC:
        case WLC_E_DISASSOC_IND:
        case WLC_E_QUIET_START:
        case WLC_E_QUIET_END:
        case WLC_E_BEACON_RX:
        case WLC_E_MIC_ERROR:
        case WLC_E_NDIS_LINK:
        case WLC_E_TXFAIL:
        case WLC_E_PMKID_CACHE:
        case WLC_E_RETROGRADE_TSF:
        case WLC_E_PRUNE:
        case WLC_E_AUTOAUTH:
        case WLC_E_EAPOL_MSG:
        case WLC_E_SCAN_COMPLETE:
        case WLC_E_ADDTS_IND:
        case WLC_E_DELTS_IND:
        case WLC_E_BCNSENT_IND:
        case WLC_E_BCNRX_MSG:
        case WLC_E_BCNLOST_MSG:
        case WLC_E_ROAM_PREP:
        case WLC_E_PFN_NET_FOUND:
        case WLC_E_PFN_NET_LOST:
        case WLC_E_RESET_COMPLETE:
        case WLC_E_JOIN_START:
        case WLC_E_ROAM_START:
        case WLC_E_ASSOC_START:
        case WLC_E_IBSS_ASSOC:
        case WLC_E_RADIO:
        case WLC_E_PSM_WATCHDOG:
        case WLC_E_CCX_ASSOC_START:
        case WLC_E_CCX_ASSOC_ABORT:
        case WLC_E_PROBREQ_MSG:
        case WLC_E_SCAN_CONFIRM_IND:
        case WLC_E_COUNTRY_CODE_CHANGED:
        case WLC_E_EXCEEDED_MEDIUM_TIME:
        case WLC_E_ICV_ERROR:
        case WLC_E_UNICAST_DECODE_ERROR:
        case WLC_E_MULTICAST_DECODE_ERROR:
        case WLC_E_TRACE:
        case WLC_E_BTA_HCI_EVENT:
        case WLC_E_IF:
        case WLC_E_P2P_DISC_LISTEN_COMPLETE:
        case WLC_E_RSSI:
        case WLC_E_PFN_SCAN_COMPLETE:
        case WLC_E_EXTLOG_MSG:
        case WLC_E_ACTION_FRAME:
        case WLC_E_ACTION_FRAME_COMPLETE:
        case WLC_E_PRE_ASSOC_IND:
        case WLC_E_PRE_REASSOC_IND:
        case WLC_E_CHANNEL_ADOPTED:
        case WLC_E_AP_STARTED:
        case WLC_E_DFS_AP_STOP:
        case WLC_E_DFS_AP_RESUME:
        case WLC_E_WAI_STA_EVENT:
        case WLC_E_WAI_MSG:
        case WLC_E_ESCAN_RESULT:
        case WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE:
        case WLC_E_PROBRESP_MSG:
        case WLC_E_P2P_PROBREQ_MSG:
        case WLC_E_DCS_REQUEST:
        case WLC_E_FIFO_CREDIT_MAP:
        case WLC_E_ACTION_FRAME_RX:
        case WLC_E_WAKE_EVENT:
        case WLC_E_RM_COMPLETE:
        case WLC_E_HTSFSYNC:
        case WLC_E_OVERLAY_REQ:
        case WLC_E_CSA_COMPLETE_IND:
        case WLC_E_EXCESS_PM_WAKE_EVENT:
        case WLC_E_PFN_SCAN_NONE:
        case WLC_E_PFN_SCAN_ALLGONE:
        case WLC_E_GTK_PLUMBED:
        case WLC_E_ASSOC_IND_NDIS:
        case WLC_E_REASSOC_IND_NDIS:
        case WLC_E_ASSOC_REQ_IE:
        case WLC_E_ASSOC_RESP_IE:
        case WLC_E_ASSOC_RECREATED:
        case WLC_E_ACTION_FRAME_RX_NDIS:
        case WLC_E_AUTH_REQ:
        case WLC_E_TDLS_PEER_EVENT:
        case WLC_E_SPEEDY_RECREATE_FAIL:
        case WLC_E_NATIVE:
        case WLC_E_PKTDELAY_IND:
        case WLC_E_AWDL_AW:
        case WLC_E_AWDL_ROLE:
        case WLC_E_AWDL_EVENT:
        case WLC_E_NIC_AF_TXS:
        case WLC_E_NIC_NIC_REPORT:
        case WLC_E_BEACON_FRAME_RX:
        case WLC_E_SERVICE_FOUND:
        case WLC_E_GAS_FRAGMENT_RX:
        case WLC_E_GAS_COMPLETE:
        case WLC_E_P2PO_ADD_DEVICE:
        case WLC_E_P2PO_DEL_DEVICE:
        case WLC_E_WNM_STA_SLEEP:
        case WLC_E_TXFAIL_THRESH:
        case WLC_E_PROXD:
        case WLC_E_IBSS_COALESCE:
        case WLC_E_AWDL_RX_PRB_RESP:
        case WLC_E_AWDL_RX_ACT_FRAME:
        case WLC_E_AWDL_WOWL_NULLPKT:
        case WLC_E_AWDL_PHYCAL_STATUS:
        case WLC_E_AWDL_OOB_AF_STATUS:
        case WLC_E_AWDL_SCAN_STATUS:
        case WLC_E_AWDL_AW_START:
        case WLC_E_AWDL_AW_END:
        case WLC_E_AWDL_AW_EXT:
        case WLC_E_AWDL_PEER_CACHE_CONTROL:
        case WLC_E_CSA_START_IND:
        case WLC_E_CSA_DONE_IND:
        case WLC_E_CSA_FAILURE_IND:
        case WLC_E_CCA_CHAN_QUAL:
        case WLC_E_BSSID:
        case WLC_E_TX_STAT_ERROR:
        case WLC_E_BCMC_CREDIT_SUPPORT:
        case WLC_E_PSTA_PRIMARY_INTF_IND:
        case WLC_E_LAST:
        case WLC_E_FORCE_32_BIT:
        default:
            printf("received event which was not registered\n");
            //wiced_assert( "Received event which was not registered\n", 0 != 0 );
            break;
    }

    return handler_user_data;
}

static void wiced_scan_result_handler( wiced_scan_result_t** result_ptr, void* user_data, wiced_scan_status_t status )
{
    wiced_scan_handler_result_t* result_iter;
    wiced_scan_handler_result_t* current_result = (wiced_scan_handler_result_t*)(*result_ptr);
    wiced_mac_t*                 mac_iter     = NULL;
    internal_scan_handler_t*     scan_handler = (internal_scan_handler_t*) user_data;

    /* Check if we don't have a scan result to send to the user */
    if ( scan_result_ptr == NULL )
    {
        return;
    }

    /* Check if scan is complete */
    if ( status == WICED_SCAN_COMPLETED_SUCCESSFULLY || status == WICED_SCAN_ABORTED )
    {
        /* Go through the list of remaining off-channel results and report them to the application */
        for ( result_iter = off_channel_results; result_iter != NULL ; )
        {
            if ( wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, (event_handler_t) scan_handler->results_handler, (void*) ( result_iter ) ) != WICED_SUCCESS )
            {
                void* temp = result_iter;
                result_iter = result_iter->next;
                vPortFree( temp);
                temp = NULL;
            }
            else
            {
                result_iter = result_iter->next;
            }
        }

        /* Indicate to the scan handler that scanning is complete */
        scan_result_ptr->status = status;
        if ( wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, (event_handler_t) scan_handler->results_handler, (void*) ( scan_result_ptr ) ) != WICED_SUCCESS )
        {
            vPortFree( scan_result_ptr );
            scan_result_ptr = NULL;
        }
        malloc_transfer_to_curr_thread(scan_bssid_list);
        vPortFree(scan_bssid_list);
        scan_bssid_list     = NULL;
        off_channel_results = NULL;
        wiced_rtos_set_semaphore(&scan_semaphore);
        return;
    }

    /* Check if the result is "on channel" */
    if ((*result_ptr)->on_channel == WICED_TRUE)
    {
        /* Check if this result obsoletes any off channel results we've received earlier */
        wiced_scan_handler_result_t* previous_result = NULL;
        for ( result_iter = off_channel_results; result_iter != NULL ; result_iter = result_iter->next )
        {
            /* Check for matching MAC address */
            if ( CMP_MAC( result_iter->ap_details.BSSID.octet, (*result_ptr)->BSSID.octet ) )
            {
                /* Delete the off channel result */
                if ( previous_result != NULL )
                {
                    previous_result->next = result_iter->next;
                }
                else
                {
                    off_channel_results = NULL;
                }
                vPortFree( result_iter );
                result_iter = NULL;
                break;
            }
            previous_result = result_iter;
        }

        /* Check if we've seen this AP before, if not then mac_iter will point to the place where we should put the new result */
        for ( mac_iter = scan_bssid_list; ( mac_iter < scan_bssid_list + current_bssid_list_length ); ++mac_iter )
        {
            /* Check for matching MAC address */
            if ( CMP_MAC( mac_iter->octet, current_result->ap_details.BSSID.octet ) )
            {
                /* Ignore this result. Clean up the result and let it be reused */
                memset( *result_ptr, 0, sizeof(wiced_scan_result_t) );
                return;
            }
        }

        /* New BSSID - add it to the list */
        if ( current_bssid_list_length < SCAN_BSSID_LIST_LENGTH )
        {
            memcpy( &mac_iter->octet, current_result->ap_details.BSSID.octet, sizeof(wiced_mac_t) );
            current_bssid_list_length++;
        }

        /* Post event in worker thread */
        if ( wiced_rtos_send_asynchronous_event( WICED_NETWORKING_WORKER_THREAD, (event_handler_t) scan_handler->results_handler, (void*) ( scan_result_ptr ) ) != WICED_SUCCESS )
        {
            vPortFree( *result_ptr );
            *result_ptr = NULL;
        }
    }
    else
    {
        /* Check if its already in the off-channel list */
        for ( result_iter = off_channel_results; result_iter != NULL ; result_iter = result_iter->next )
        {
            /* Check for matching MAC address */
            if ( CMP_MAC( result_iter->ap_details.BSSID.octet, current_result->ap_details.BSSID.octet ) )
            {
                /* Ignore this result. Clean up the result and let it be reused */
                memset(*result_ptr, 0, sizeof(wiced_scan_result_t));
                return;
            }
        }

        /* Add it to the list of off-channel results */
        current_result->next = off_channel_results;
        off_channel_results = current_result;
    }

    /* Allocate new storage space for next scan result */
    scan_result_ptr = MALLOC_OBJECT("scan result", wiced_scan_handler_result_t);
    if (scan_result_ptr != NULL)
    {
        memset( scan_result_ptr, 0, sizeof(wiced_scan_handler_result_t));
        scan_result_ptr->status    = WICED_SCAN_INCOMPLETE;
        scan_result_ptr->user_data = scan_handler->user_data;
    }
    *result_ptr = (wiced_scan_result_t*)scan_result_ptr;
}

static void* softap_event_handler( const wwd_event_header_t* event_header, const uint8_t* event_data, /*@returned@*/ void* handler_user_data )
{
    wiced_wifi_softap_event_t event = WICED_AP_UNKNOWN_EVENT;

    UNUSED_PARAMETER( handler_user_data );
    UNUSED_PARAMETER( event_data );

    if ( ap_event_handler != NULL )
    {
        if ( event_header->event_type == WLC_E_DEAUTH_IND || event_header->event_type == WLC_E_DISASSOC_IND )
        {
            event = WICED_AP_STA_LEAVE_EVENT;
        }
        else if ( event_header->event_type == WLC_E_ASSOC_IND || event_header->event_type == WLC_E_REASSOC_IND )
        {
            event = WICED_AP_STA_JOINED_EVENT;
        }

        ap_event_handler( event, &event_header->addr );
    }

    return NULL;
}




#pragma arm section code
#endif
