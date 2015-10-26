/*
 *
 *  Rockchip dlna service.
 *
 *    2014-01-14 by rockchip IOT team.
 *
 *
 */
#include "source_macro.h"
#include "rkos.h"
#ifdef __WIFI_DLNA_C__

// Log tag.
#include <rtos_config.h>
#include <string.h>
#include <dlna_config.h>
 #if !defined(FREE_RTOS)
#define LOG_TAG "Dlna"
#include <utils/Log.h>
#include <sys/time.h>
#include <server/dms.h>

#else
#include <cybergarage/net/cinterface.h>
#include <cybergarage/util/cthread.h>
#include <utils/dlna_log.h>
#include <cybergarage/upnp/std/av/cmediarenderer.h>
#endif

#include <stdlib.h>

//#pragma arm section code = "dlnaCode", rodata = "dlnaCode", rwdata = "dlnadata", zidata = "dlnadata"


#define WIFI_CONFIG_FILE "/data/misc/wifi/lollipop.conf"

// macros
#define DEVICE_CHECK_SECOND (5)
#define MAX_ADDRESS_LEN (256)
#define READ_BUF_SIZE (512)

// statics
static char ip_address[MAX_ADDRESS_LEN];
static char tmp_address[MAX_ADDRESS_LEN];



#if defined(HAVE_FILE_SYS)
static int get_device_name(char* buf, int len)
{
    #define LINE_SIZE (128)
    #define DEVICE_NAME "device_name"
    FILE* fp;
    char line[LINE_SIZE] = { 0 };

    fp = fopen(WIFI_CONFIG_FILE, "r");
    if (!fp) {
        ALOGE("Cannot open file: %s.", WIFI_CONFIG_FILE);
        return -1;
    }

    while (fgets(line, LINE_SIZE, fp)) {
        char* value = strchr(line, '=');
        if (value) {
            int slen = strlen(line);
            *value = 0; // Remove =
            *(line + slen - 1) = 0; // Remove /n
            if (!strcmp(line, DEVICE_NAME)) {
                strncpy(buf, value + 1, len);
                ALOGD("Get device name: %s", buf);
                fclose(fp);
                return 0;
            }
        }
        memset(line, 0, LINE_SIZE);
    }

    fclose(fp);
    ALOGE("Get device name failed.");
    return -1;
}
#else
static int get_device_name(char* buf, int len)
{
    strcpy(buf, "SPEAKER 1314520");
    return 0;
}
#endif

#ifdef HAVE_MEDIA_SERVER
static CgUpnpAvServer * g_dms;

static void dlna_dms_start()
{
    char suffix[256] = { 0 };

        // Get suffix name.
    if (g_dms== NULL) {
        if (get_device_name(suffix, 256))
            strncpy(suffix, ip_address, 256);

        dms_create(suffix);
    } else {
        dms_restart(g_dms);
    }

}

static void dlna_dms_stop()
{
    if (g_dms) {
        dms_destroy(g_dms);
        g_dms = NULL;
    }
}
#endif

#ifdef HAVE_MEDIA_RENDERER
static CgUpnpAvRenderer * g_dmr;
static void dlna_dmr_start()
{
    rk_printf("dlna dmr start\n");
    //FREQ_EnterModule(FREQ_MAX);
    if (g_dmr == NULL) {
        char suffix[256] = { 0 };

        // Get suffix name.
        if (get_device_name(suffix, 256))
            strncpy(suffix, ip_address, 256);

        g_dmr = (CgUpnpAvRenderer *)dmr_create(suffix);
    } else {
        dmr_restart(g_dmr);
    }
}
static void dlna_dmr_stop()
{
    //FREQ_ExitModule(FREQ_MAX);
    if (g_dmr) {
        dmr_destroy(g_dmr);
        g_dmr = NULL;
    }
}
#endif
static int get_bind_address(char* addr, int len) // get sta ip only
{
    CgNetworkInterfaceList *netIfList;
    CgNetworkInterface *netIf;
    char *bindAddr;
#ifdef DOUBLE_WAP
    char *ipflag;
#endif

    netIfList = cg_net_interfacelist_new();
    cg_net_gethostinterfaces(netIfList);

    for (netIf = cg_net_interfacelist_gets(netIfList); netIf; netIf = cg_net_interface_next(netIf)) {

        bindAddr = cg_net_interface_getaddress(netIf);

        if (cg_strlen(bindAddr) <= 0)
            continue;

        if (strcmp(bindAddr, "0.0.0.0") == 0)
            continue;

#ifdef DOUBLE_WAP
        if ((ipflag = strrchr(bindAddr, '.')) != NULL) {
            ipflag++;
            if (strcmp(ipflag, "1") == 0) // if it is softap ,ignore it
                continue;
        }
#endif

        memset(addr, 0, len);
        if (cg_strlen(bindAddr) > len) {
            ALOGE("CyberLink: get_bind_address buffer overflow.");
        }
        memcpy(addr, bindAddr, cg_strlen(bindAddr));

    }

    cg_net_interfacelist_delete(netIfList);

    return 0;
}
void start_dlna_device(void)
{
#ifdef HAVE_MEDIA_RENDERER
    dlna_dmr_start();
#endif

#ifdef HAVE_MEDIA_SERVER
    dlna_dms_start();
#endif
}
void stop_dlna_device(void)
{
#ifdef HAVE_MEDIA_RENDERER
    dlna_dmr_stop();
#endif

#ifdef HAVE_MEDIA_SERVER
    dlna_dms_stop();
#endif
}
static void check_dlna_device(void)
{
    //ALOGD("Check devcie.\n");
#if 1
    get_bind_address(tmp_address, MAX_ADDRESS_LEN);
    if (strlen(tmp_address) < 2) {
        ALOGD("Device has no connect to net.\r\n");
    } else {
        if (strcmp(tmp_address, ip_address) == 0) {
            //ALOGD("The ip address has no change: %s\n", g_ip);
        } else {
            ALOGD("Get ip address: %s\r\n", tmp_address);

            memset(ip_address, 0, MAX_ADDRESS_LEN);
            memcpy(ip_address, tmp_address, MAX_ADDRESS_LEN);

            //stop_dlna_device();
            start_dlna_device();
        }
    }
#else
        start_dlna_device();
#endif
}

static void timeout_handle(void * parameter)
{
    while(1) {
        check_dlna_device();

#if defined(FREE_RTOS)
        cg_wait(5000);
      //  printf("--test-- go to cg_wait\r\n");
#endif
    }
}

#if !defined(FREE_RTOS)
static struct itimerval timer_val;

static void init_sigaction(void)
{
    struct sigaction act;

    act.sa_handler = timeout_handle;
    act.sa_flags   = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);
}

static void init_timer()
{
    timer_val.it_value.tv_sec = DEVICE_CHECK_SECOND;
    timer_val.it_value.tv_usec = 0;
    timer_val.it_interval = timer_val.it_value;
    setitimer(ITIMER_REAL, &timer_val, NULL);
}

static void* event_listener(void *arg)
{
    while (1) {
       /*
        char rcvBuf[READ_BUF_SIZE] = { 0 };
        lollipop_socket_server_rcv(SOCK_FILE_DLNA, rcvBuf, READ_BUF_SIZE);

        if (NULL == rcvBuf)
            continue;

        if (strstr(rcvBuf, IPC_VOLUME_CHANGE) == rcvBuf) {
            int vol = system_volume_get();

            dmr_set_volume(g_dmr, vol);

            ALOGD("Volume update: %d\n", vol);
        } else if (strstr(rcvBuf, IPC_NETWORK_CHANGE) == rcvBuf) {
            char *buf_tok = NULL;

            buf_tok = strtok(rcvBuf, " ");
            if (buf_tok) {
                buf_tok = strtok(NULL, " ");
                ALOGI("buf_tok: %s\n", buf_tok);
            }

            if (buf_tok) {
                if (!strncmp(buf_tok, MSG_NETWORK_CONNECTED, strlen(MSG_NETWORK_CONNECTED))) {
                    ALOGD("DLNA MSG_NETWORK_CONNECTED\n");
                } else if (!strncmp(buf_tok, MSG_NETWORK_DISCONNECTED, strlen(MSG_NETWORK_DISCONNECTED))) {
                    ALOGD("DLNA MSG_NETWORK_DISCONNECTED\n");
                }
            }
        }*/
    }

    pthread_exit((void *)0);
    ALOGI("Event listener thead exit.\n");

    return NULL;
}

static void create_event_listener(void)
{
    void *t_retval;
    pthread_t key_handle;

    pthread_create(&key_handle, NULL, event_listener, (void*)NULL);

    ALOGD("Create event listener thread.\n");
}


#else
static void dlna_start_intime()
{
#if 0
    CgThread *thread = cg_thread_new();
    cg_thread_setaction(thread, timeout_handle);
    FreeRtos_setParamter(thread, DLNA_MINIMAL_STACK_SIZE*20, DLNA_PRIORITY);
    cg_thread_start(thread);
#else
    timeout_handle(NULL);
#endif


}
#endif

void dlna_main(void const * argument)
{

     ALOGD("goin mian\r\n");
#if !defined(FREE_RTOS)
    init_sigaction();
    init_timer();
    create_event_listener();
#else
    dlna_start_intime();
#endif

    return ;
}
//#pragma arm section code
#endif

