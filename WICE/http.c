/*
********************************************************************************************
*
*        Copyright (c): 2015 - 2015 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                             All rights reserved.
*
* FileName: WICE\http.c
* Owner: lyb
* Date: 2015.6.26
* Time: 15:21:16
* Version: 1.0
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    lyb     2015.6.26     15:21:16   1.0
********************************************************************************************
*/


#include "BspConfig.h"

#ifdef __WICE_HTTP_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "BSP.h"
#include <stdlib.h>
#include <stdio.h>
#include "lwipopts.h"
#include "def.h"
#include "tcp.h"
#include "netif.h"
#include "ip.h"
#include "pbuf.h"
#include "api.h"
#include "dns.h"
#include "etharp.h"
#include "ip_addr.h"
#include <string.h>
#include "err.h"
#include "http.h"
#include "wwd_constants.h"
extern wiced_bool_t                 wiced_sta_link_up;
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

//typedef struct
#define  DNSRETRYNUM  5

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/

static struct netconn *http_conn = NULL;
static uint8 running = 0;
static uint8 *fifo = NULL;
static struct pbuf *httpfile = NULL;
static uint32 http_len= 0;
HTTP_URL_CALLFUNCTION HTTP_Status_Callback = NULL;
HDC hFifo;
static uint8 *head_buf = NULL;
static uint16 head_len = 0;
static uint16 al_len = 0;
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
static void AtoAddr(char *s, char *proto, char *user, char *pwrd, char *host, unsigned int *port, char *file);
static uint8 * strstri(uint8 *s, uint8 *pattern);
static char* strrmvspace(char *dst, const char *src);
static uint32 Http_HdParam(uint8 * dst, uint8 blen, uint8 * src);
static rk_err_t Http_Com(uint8 *host, uint8 *file);
static uint32 Http_Response(uint8 * src);
static int Http_Get_Header_Len(char *data, int dlen);
static void GetFile( uint32 len);
unsigned int atou(const char *s);
uint32 tcp_connnect(ip_addr_t *ripaddr, uint16 localPort, uint16 remoteport, struct netconn **conn);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: Http_Test
** Input:void
** Return: void
** Owner:lyb
** Date: 2015.8.14
** Time: 11:33:50
*******************************************************************************/
COMMON API void Http_Test(void)
{
    uint8 dwRet;
    printf("http test");
    dwRet = HttpGet_Url("http://192.168.1.100/I%20love%20you.pcm");
    if(dwRet == RK_SUCCESS)
        printf("\nhttp ok\n");
    else
        printf("\nhttp error\n");
}

#if 0
COMMON API rk_err_t HttpTask_Get(uint32 len)
{

    HTTP_ASK_QUEUE HttpAskQueue;
    HTTP_RESP_QUEUE HttpRespQueue;

    memset(&HttpAskQueue, 0x00, sizeof(HTTP_ASK_QUEUE ));
    HttpAskQueue.len = len;
    printf("HttpAskQueue.len  = %d\n", HttpAskQueue.len);
    rkos_queue_send(gpstHTTPData->HTTPAskQueue, &HttpAskQueue, MAX_DELAY);
    rkos_queue_receive(gpstHTTPData->HTTPRespQueue, &HttpRespQueue, MAX_DELAY);
    if (HttpRespQueue.status == RK_SUCCESS)
    {
        return RK_SUCCESS;
    }
    else
    {
        return RK_ERROR;
    }
}
#endif

/*******************************************************************************
** Name: HTTPTask_Enter
** Input:void
** Return: void
** Owner:lyb
** Date: 2015.6.26
** Time: 15:26:43
*******************************************************************************/
COMMON API void HTTPTask_Enter(void)
{
    printf("http_len = %d\n", http_len);
    if( http_len == 0 )
    {
        RKTaskDelete(TASK_ID_HTTP,0, ASYNC_MODE);
        while(1);
    }
    else
    {
        GetFile(http_len);
        return;
    }

}


/*******************************************************************************
** Name: Http_Close
** Input:void
** Return: void
** Owner:lyb
** Date: 2015.8.13
** Time: 20:58:57
*******************************************************************************/
COMMON API void Http_Close(void)
{
    int ret = 0;
    if(running)
    {
       printf("http close\n");
       RKTaskDelete(TASK_ID_HTTP,0, ASYNC_MODE);
       netconn_delete(http_conn);
       if(hFifo != NULL)
       {
          RKDev_Close(hFifo);
          hFifo = NULL;
       }
       if(fifo != NULL)
       {
           rkos_memory_free(fifo);
           fifo = NULL;
       }
       if(httpfile != NULL)
       {
            pbuf_free(httpfile);
            httpfile = NULL;
       }
    }
    running = 0;
    http_len = 0;
    if(head_buf != NULL)
    {
        rkos_memory_free(head_buf);
        head_buf = NULL;
    }
    head_len = 0;
    al_len = 0;
    return;
}


/*******************************************************************************
** Name: HttpGet_Url
** Input:uint8
** Return: int
** Owner:
** Date: 2015.6.24
** Time: 8:50:20
*******************************************************************************/
rk_err_t HttpGet_Url(uint8 *pburl)
{
    uint8 proto[8];
    uint8 bhttpbuf[1024] = {0};
    uint8 *bHost;
    uint8 *bFile;
    unsigned int port;
    uint8 localport = 0;
    uint8 trycount;
    ip_addr_t stIp;
    int ResStatus;

#if 0
    if( wiced_sta_link_up == WICED_FALSE)
    {
        rk_print_string("wiced is down\n");
        return RK_ERROR;
    }
#endif

    if(running == 1)
    {
        rk_print_string("http is running");
        return RK_ERROR;
    }

    bHost = &bhttpbuf[0];
    bFile = &bhttpbuf[100];
    //printf("pburl=  %s\n", pburl);
    AtoAddr(pburl, proto, 0, 0, bHost, &port, bFile);   //get host, port and file

    if (strcmp(proto, "http") != 0)
    {
        rk_printf("not http message\n");
        return RK_ERROR;
    }
     //printf("bHost=  %s\n", bHost);
     //printf("port=  %d\n", port);
     //printf("bFile=  %s\n", bFile);
    for (trycount = 0; trycount < DNSRETRYNUM; trycount++)  //get ip by dns
    {
        if ( netconn_gethostbyname(bHost, &stIp) == 0)
        {
            printf("dns host and ip:%s = %x\n", bHost, stIp);
            break;
        }
        else
        {
            printf("dns fail,clear arp list.\n");
            etharp_init();
        }
    }
    if ((trycount >= DNSRETRYNUM) && (stIp.addr == 0))
    {
        printf("DNS Error!!!\n");
        return RK_ERROR;
    }

    localport  = (localport + 1) % 256 + 4096;  //get local port
    printf("tcp connect ok\n");
    do
    {
        ResStatus = tcp_connnect(&stIp, localport, port, &http_conn);
        trycount++;

    }while (( ResStatus == RK_ERROR) && trycount < 5);

    if( ResStatus == RK_ERROR)
    {
        printf("HTTP:tcp connet fail\n");
        return RK_ERROR;
    }

    if(Http_Com(bHost, bFile) == 0)
        return RK_SUCCESS;
    netconn_delete(http_conn);
    return RK_ERROR;
}

static rk_err_t Http_Com(uint8 *host, uint8 *file)
{
    uint16 tx_len;
    struct pbuf *tcp_buf;
    uint32 rx_len;
    uint32 HttpHeader_len;
    uint8 pbuf[32];
    uint32 data_len = 0;
    uint8 trycnt;
    int Httpstatus = 0;
    int ResStatus = 0;

    head_buf = rkos_memory_malloc(2048);
    if(head_buf == NULL)
    {
        printf("head_buf malloc fail\n");
    }

    tx_len = sprintf(head_buf, "GET %s HTTP/1.0\r\n"
                     "Host: %s\r\n"
                     "User-Agent: RkNanocPlayer\r\n"
                     "Ultravox-transport-type: TCP\r\n"
                     "Accept: */*\r\n"
                     "Icy-MetaData: 1\r\n"
                     "Connection: close\r\n"
                     "\r\n",
                     file, host);
    netconn_write(http_conn, head_buf, tx_len, NETCONN_COPY);  //set message to recive file

    rkos_memset(head_buf, 0x00, 2048);

    /*get data header information*/
    while (1)
    {
        netconn_recv_tcp_pbuf(http_conn, &tcp_buf);
        rx_len = tcp_buf->tot_len;
        head_len = rx_len;
        if (rx_len > 0)
        {
            pbuf_copy_partial(tcp_buf, head_buf, rx_len, 0);
            pbuf_free(tcp_buf);
            Httpstatus = Http_Response(head_buf);
            if ((Httpstatus != 200) && (Httpstatus != 206))
            {
                printf("\n 2_http error\n");
                return RK_ERROR;
            }
            break;
        }
    }
    printf("head_buf = %s\n", head_buf);
    trycnt = 0;
    while (1)
    {
        if (Http_HdParam(pbuf, 31, head_buf) == 0)
        {
            data_len = atoi(pbuf);
            printf("Content-Length:%d\n", data_len);
        }

        //debug_hex(head_buf, head_len, 16);
        HttpHeader_len= Http_Get_Header_Len(head_buf, head_len);
        if (HttpHeader_len)
        {
            printf("http hlen:%d\n", HttpHeader_len);
            al_len = head_len - HttpHeader_len; //already recieve data
            head_len = HttpHeader_len;          //
            break;
        }
        netconn_recv_tcp_pbuf(http_conn, &tcp_buf);
        rx_len = tcp_buf->tot_len;
        pbuf_copy_partial(tcp_buf, head_buf+head_len, rx_len, 0);
        pbuf_free(tcp_buf);
        head_len += rx_len;

        trycnt++;
        if(trycnt > DNSRETRYNUM)
        {
            printf("trycnt fail\n");
            return RK_ERROR;
        }
    }
    rk_printf("Http_HdParam ok\n");
    http_len = data_len;
    RKTaskCreate(TASK_ID_HTTP,0, NULL);
    running = 1;
    return RK_SUCCESS;
}

static void GetFile(uint32 len)
{
//write 1kb
    err_t err;
    uint32 curlength = 0;
    uint32 leavelength;
    uint32 fifo_len = 0;
    uint16 i,j,k;
    uint16 offset = 0;
    uint16 leftset = 0;
    int ret;
    uint8 buf[1152] = {0};

    if(al_len == 0)
    {
        leavelength = len;
        j = 0;
    }
    else
    {
        leavelength = len - al_len;
        j = al_len;
        rkos_memcpy(buf, head_buf+head_len, j);
    }
    rkos_memory_free(head_buf);
    head_buf = NULL;
    head_len = 0;
    al_len = 0;
#if 0
    FIFO_DEV_ARG stFifoArg;
    stFifoArg.BlockCnt = 1;
    stFifoArg.BlockSize = 512;
    ret = RKDev_Create(DEV_CLASS_FIFO, 0, &stFifoArg);
    if(ret != RK_SUCCESS)
    {
        rk_print_string("fifo dev create failure");
    }
#endif
    rk_printf("get file\n");
#if 1
    hFifo = RKDev_Open(DEV_CLASS_FIFO, 0, NOT_CARE);
    if ((hFifo == NULL) || (hFifo == (HDC)RK_ERROR) || (hFifo == (HDC)RK_PARA_ERR))
    {
        rk_print_string("fifo device open failure");
        err = FIFO_OPEN_ERR;
        if( HTTP_Status_Callback != NULL)
            HTTP_Status_Callback(err);
        netconn_delete(http_conn);
        running = 0;
        http_len = 0;
        RKTaskDelete(TASK_ID_HTTP, 0, ASYNC_MODE);
        while(1);
       // return;
    }
    fifoDev_SetTotalSize(hFifo, len);

#endif
    do
    {
        err =  netconn_recv_tcp_pbuf(http_conn, &httpfile);
        if (err == ERR_OK)
        {
            leavelength = leavelength - httpfile->tot_len;

            curlength = httpfile->tot_len+j;  //
            if(fifo_len < curlength)
            {
                if(fifo != NULL)
                {
                    rkos_memory_free(fifo);
                    fifo = NULL;
                }

                fifo_len = curlength;
                fifo = rkos_memory_malloc(fifo_len);
                if(fifo == NULL)
                {
                    rk_print_string("fifo malloc fail\n");
                    err = MALLOC_ERR;
                    if( HTTP_Status_Callback != NULL)
                        HTTP_Status_Callback(err);
                    netconn_close(http_conn);
                    netconn_delete(http_conn);
                    running = 0;
                    http_len = 0;
                    pbuf_free(httpfile);
                    httpfile = NULL;
                    //fifoDev_SetTotalSize(hFifo, 0);
                    RKDev_Close(hFifo);
                    RKTaskDelete(TASK_ID_HTTP, 0, ASYNC_MODE);
                    while(1);
                    //return;
                }
             }

            rkos_memcpy(fifo, buf, j);
            pbuf_copy_partial(httpfile, fifo+j, httpfile->tot_len, 0);
            pbuf_free(httpfile);
            httpfile = NULL;
            i = curlength/1024;
            j = curlength%1024;
            for(k=0; k<i; k++)
            {
#if 1
                ret = fifoDev_Write(hFifo, fifo+k*1024, 1024, SYNC_MODE, NULL);
                if(ret != 1024)
                {
                   printf("fifo write error ret = %d\n ", ret);
                   goto OVER;
                }
#endif
                //test
                //debug_hex(fifo+k*1024, 1024, 16);
            }
            offset = i*1024;
            if(leavelength <= 0)
            {
                rk_printf("fifo write end\n");
                if(j == 0)
                {
                    //debug_hex(fifo+(i-1)*1024, 1024, 16);
                    break;

                }
                else
                {
                    i = j/512;
                    for(k=0; k<i; k++)
                    {
#if 1
                        ret = fifoDev_Write(hFifo, fifo+offset+k*512, 512, SYNC_MODE, NULL);
                        if(ret != 512)
                        {
                           printf("fifo write error ret = %d\n ", ret);
                           goto OVER;
                        }
#endif
                        //debug_hex(fifo+offset+k*512, 512, 16);
                        j = j-512;
                    }
                    if(j>0)
                    {
                        rkos_memcpy(buf, fifo+offset+k*512, j);
                        //leftset = 512 - j;
                        //rkos_memset(fifo, 0x00, leftset);
                        //rkos_memcpy(buf+j, fifo, leftset);
#if 1
                        ret = fifoDev_Write(hFifo, buf, j, SYNC_MODE, NULL);
                        if(ret != j)
                        {
                           printf("the last fifo write error\n");
                           goto OVER;
                        }
#endif
                        //debug_hex(buf, 512, 16);
                    }
                    break;
                }
            }
            else if((j > 0) && (leavelength > 0))
            {
                rkos_memcpy(buf, fifo+offset, j);   //left copy
            }
        }

    }while ((err == ERR_OK) && (leavelength > 0));
    if(err != ERR_OK)
    {
        err = TCP_RECIVER_ERR;
    }
    else
    {
        err = ERR_OK;
    }

    if( HTTP_Status_Callback != NULL)
        HTTP_Status_Callback(err);
    netconn_delete(http_conn);


    http_conn = NULL;
    running = 0;
    http_len = 0;
    if(fifo != NULL)
    {
        rkos_memory_free(fifo);
        fifo = NULL;
    }
    RKDev_Close(hFifo);
    RKTaskDelete(TASK_ID_HTTP, 0, ASYNC_MODE);
    while(1);
OVER:
    err = FIFO_WRITE_ERR;
    if( HTTP_Status_Callback != NULL)
        HTTP_Status_Callback(err);
    netconn_delete(http_conn);
    running = 0;
    http_conn = NULL;
    http_len = 0;
    if(fifo != NULL)
    {
        rkos_memory_free(fifo);
        fifo = NULL;
    }
    RKDev_Close(hFifo);
    RKTaskDelete(TASK_ID_HTTP, 0, ASYNC_MODE);
    while(1);
}

uint32 tcp_connnect(ip_addr_t *ripaddr, uint16 localPort, uint16 remoteport, struct netconn **conn)
{

    err_t err;
    struct ip_addr *l_ipaddr;
    l_ipaddr = ripaddr;

    //ip_addr_t l_ipaddr;

    /*
        l_ipaddr.addr = 0;
        l_ipaddr.addr |= atou(ipaddr)<<0;
        while(isdigit(*ipaddr)){ ipaddr++; }; while(!isdigit(*ipaddr)){ ipaddr++; };
        l_ipaddr.addr |= atou(ipaddr)<<8;
        while(isdigit(*ipaddr)){ ipaddr++; }; while(!isdigit(*ipaddr)){ ipaddr++; };
        l_ipaddr.addr |= atou(ipaddr)<<16;
        while(isdigit(*ipaddr)){ ipaddr++; }; while(!isdigit(*ipaddr)){ ipaddr++; };
        l_ipaddr.addr |= atou(ipaddr)<<24;
    */

    if (remoteport == 0)
        remoteport = 80;


    *conn = netconn_new(NETCONN_TCP);
    if (*conn == NULL)
    {
        printf("netconn_new alloc fail\n");

        while (1)
        {
            rkos_sleep(100);
        }
    }

    netconn_bind(*conn, IP_ADDR_ANY, localPort);
    //netconn_listen(conn);
    err = netconn_connect(*conn, l_ipaddr, remoteport);
    if (err == ERR_OK)
        return RK_SUCCESS;

    return RK_ERROR;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


static void AtoAddr(char *s, char *proto, char *user, char *pwrd, char *host, unsigned int *port, char *file)
{
    /*
    for example:s = "http://api.xbd61.com/ApiForM/TjMusicList.php?Apiadmin=xiaobuding"
    */
    if (proto)
    {
        *proto = 0;
    }
    if (user)
    {
        *user  = 0;
    }
    if (pwrd)
    {
        *pwrd  = 0;
    }
    if (host)
    {
        *host  = 0;
    }
    if (port)
    {
        *port  = 0;
    }
    if (file)
    {
        *file++ = '/';
        *file = 0;
    }

    while (*s == ' ')
    {
        s++;    //skip spaces
    }

    //get proto "http://"
    if (StrCmpA(s, "http://", 7) == 0)
    {
        s += 7;
        if (proto)
        {
            strcpy(proto, "http");
        }
    }
    else
    {
        return;
    }

    //get user & password
    if (strstr(s, "@") != 0)//find string "@" form string "s",
    {
        while (*s && (*s!=':'))
        {
            if (user)
            {
                *user++ = *s;
                *user   = 0;
            }
            s++;
        }
        s++; //skip ":"
        while (*s && (*s!='@'))
        {
            if (pwrd)
            {
                *pwrd++ = *s;
                *pwrd   = 0;
            }
            s++;
        }
        s++; //skip "@"
    }

    //get host:"api.xbd61.com"
    while (*s && (*s!=':') && (*s!='/'))
    {
        if (host)
        {
            *host++ = *s;
            *host   = 0;
        }
        s++;
    }

    //get port.the part:"80"
    if (*s == ':')
    {
        s++; //skip ":"
        if (port)
        {
            *port = atoi(s);
        }
        while (isdigit(*s))//skip port
        {
            s++;
        };
    }

    //get file:"ApiForM/TjMusicList.php?Apiadmin=xiaobuding"
    if (*s == '/')// "/ApiForM/TjMusicList.php?Apiadmin=xiaobuding"
    {
        s++; //skip "/"
        while (*s && !isspace(*s))
        {
            if (file)
            {
                *file++ = *s;
                *file   = 0;
            }
            s++;
        }
    }
    return;
}

#if 0
ip_addr_t AtoIp(uint8 *s, uint8 * ipaddr)
{
    ip_addr_t ip = {0};
    uint8 res;

    if (isdigit(*s)) //ip
    {
        memcpy(ipaddr, s, strlen(s));

        ip.addr |= atou(s)<<0;
        while (isdigit(*s))
        {
            s++;
        }
        ;
        while (!isdigit(*s))
        {
            s++;
        };
        ip.addr |= atou(s)<<8;
        while (isdigit(*s))
        {
            s++;
        }
        ;
        while (!isdigit(*s))
        {
            s++;
        };
        ip.addr |= atou(s)<<16;
        while (isdigit(*s))
        {
            s++;
        }
        ;
        while (!isdigit(*s))
        {
            s++;
        };
        ip.addr |= atou(s)<<24;


    }
    else //get ip -> dns resolve
    {

        ip = dns_getip(s, ipaddr);
    }

    return ip;
}

void my_dns_found_callback(const char *name, ip_addr_t *ipaddr, void *callback_arg)
{
    ip_addr_t *dipaddr = callback_arg;

    if (ipaddr == NULL)
    {
        if (dipaddr)
        {
            dipaddr->addr = 0;
        }
        MSG_WIFI_DNS_RETURN = 1;
    }
    else
    {
        if (dipaddr)
        {
            dipaddr->addr = ipaddr->addr;
        }
        MSG_WIFI_DNS_RETURN = 1;
    }
    return;
}

ip_addr_t dns_getip(uint8 *domain, uint8 * ipaddr)
{
    ip_addr_t dnsAddr = {0};


    if (dns_gethostbyname(domain, &dnsAddr, my_dns_found_callback, (void*)&dnsAddr))
    {
        MSG_WIFI_DNS_RETURN = 0;
        while (1)
        {
            if (MSG_WIFI_DNS_RETURN)
            {
                MSG_WIFI_DNS_RETURN = 0;
                break;
            }
        }
    }

    sprintf(ipaddr, "%u.%u.%u.%u", ((dnsAddr.addr & 0x000000FF)), ((dnsAddr.addr & 0x0000FF00)>>8),
            ((dnsAddr.addr & 0x00FF0000)>>16),((dnsAddr.addr & 0xFF000000)>>24));

    return dnsAddr;
}
#endif

static int Http_Get_Header_Len(char *data, int dlen)
{
    int i;

    if ((data == NULL) || (dlen < 4))
    {
        return 0;
    }

    for (i = 0; i < dlen; i++)
    {
        if ((data[i+0] == '\r') && (data[i+1] == '\n') &&
                (data[i+2] == '\r') && (data[i+3] == '\n'))
        {
            return (i + 4);
        }
    }

    return 0;
}
static uint32 Http_Response(uint8 * src)
{
    unsigned int search = 16;

    /* skip spaces*/
    while ((*src==' ') && search)
    {
        src++;
        search--;
    }
    if ((StrCmpA(src, "ICY", 3)  == 0) ||
            (StrCmpA(src, "HTTP", 4) == 0) ||
            (StrCmpA(src, "RTSP", 4) == 0))
    {
        /* skip proto name*/
        while (*src && (*src!=' ') && search)
        {
            src++;
            search--;
        }
        /* skip spaces*/
        while ((*src==' ') && search)
        {
            src++;
            search--;
        }
        if (search)
        {
            return atoi(src);
        }
    }

    return 0;
}


static uint32 Http_HdParam(uint8 * dst, uint8 blen, uint8 * src)
{
    char *ptr;
    uint8 *param = "Content-Length:";
    uint8 *param1 = "CONTENT-LENGTH:";
    uint8 *src1,*src2;
    ptr = dst; //save dst

    src1 = strstri(src, param);
    if (src1)
    {
        src1 += strlen(param);
        for (; blen != 0; blen--)
        {
            if ((*src1==0) || (*src1=='\n') || (*src1=='\r'))
            {
                break;
            }
            *dst++ = *src1++;
        }
        *dst = 0;
        //remove space at start and end of string
        strrmvspace(ptr, ptr);
        if (strlen(ptr))
        {
            return RETURN_OK;
        }
    }
    else
    {
        src2 = strstri(src, param1);
        if (src2)
        {
            src2 += strlen(param1);
            for (; blen != 0; blen--)
            {
                if ((*src2==0) || (*src2=='\n') || (*src2=='\r'))
                {
                    break;
                }
                *dst++ = *src2++;
            }
            *dst = 0;
            strrmvspace(ptr, ptr);
            if (strlen(ptr))
            {
                return RETURN_OK;
            }
        }
    }

    return RETURN_FAIL;
}


static uint8 * strstri(uint8 *s, uint8 *pattern)
{
    size_t len;

    if (*pattern)
    {
        len = strlen(pattern);
        while (*s)
        {
            if (StrCmpA(s, pattern, len) == 0)
            {
                return s;
            }
            s++;
        }
    }

    return 0;
}


static char* strrmvspace(char *dst, const char *src)//remove the space at the end of head and tail...
{
    unsigned int i;

    if (*src == 0)
    {
        *dst = 0;
    }
    else
    {
        //at start
        for (i=0; isspace(src[i]); i++);
        strcpy(dst, &src[i]);

        //at end
        i=strlen(dst)-1;
        for (i=strlen(dst)-1; isspace(dst[i]); i--)
        {
            dst[i] = 0;
        }
    }

    return dst;
}


unsigned int atou(const char *s)
{
    unsigned int value=0;

    if (!s)
    {
        return 0;
    }

    while (*s)
    {
        if ((*s >= '0') && (*s <= '9'))
        {
            value = (value*10) + (*s-'0');
        }
        else
        {
            break;
        }
        s++;
    }

    return value;
}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: HTTPTask_DeInit
** Input:void *pvParameters
** Return: rk_err_t
** Owner:lyb
** Date: 2015.6.26
** Time: 15:26:43
*******************************************************************************/
INIT API rk_err_t HTTPTask_DeInit(void *pvParameters)
{
#if 0
    rkos_queue_delete(gpstHTTPData->HTTPAskQueue);
    rkos_queue_delete(gpstHTTPData->HTTPRespQueue);
    rkos_memory_free(gpstHTTPData);
    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_HTTP_TASK, MODULE_OVERLAY_ALL);
    #endif
#endif
    return RK_SUCCESS;
}
/*******************************************************************************
** Name: HTTPTask_Init
** Input:void *pvParameters, void *arg
** Return: rk_err_t
** Owner:lyb
** Date: 2015.6.26
** Time: 15:26:43
*******************************************************************************/
INIT API rk_err_t HTTPTask_Init(void *pvParameters, void *arg)
{
#if 0
    RK_TASK_CLASS*   pHTTPTask = (RK_TASK_CLASS*)pvParameters;
    RK_TASK_HTTP_ARG * pArg = (RK_TASK_HTTP_ARG *)arg;
    HTTP_TASK_DATA_BLOCK*  pHTTPTaskData;
    if (pHTTPTask == NULL)
        return RK_PARA_ERR;

    pHTTPTaskData = rkos_memory_malloc(sizeof(HTTP_TASK_DATA_BLOCK));
    memset(pHTTPTaskData, NULL, sizeof(HTTP_TASK_DATA_BLOCK));

    pHTTPTaskData->HTTPAskQueue = rkos_queue_create(1, sizeof(HTTP_ASK_QUEUE));
    pHTTPTaskData->HTTPRespQueue = rkos_queue_create(1, sizeof(HTTP_RESP_QUEUE));


    if ((pHTTPTaskData->HTTPAskQueue && pHTTPTaskData->HTTPRespQueue) == 0)
    {
        rkos_semaphore_delete(pHTTPTaskData->HTTPAskQueue);
        rkos_semaphore_delete(pHTTPTaskData->HTTPRespQueue);

        rkos_memory_free(pHTTPTaskData);

        return RK_ERROR;
    }

    gpstHTTPData = pHTTPTaskData;
    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
    FW_ModuleOverlay(MODULE_ID_HTTP_TASK, MODULE_OVERLAY_ALL);
    #endif
#endif
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
