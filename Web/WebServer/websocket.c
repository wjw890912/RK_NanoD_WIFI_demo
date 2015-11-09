#include "lwip/opt.h"
#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/api.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
//#include "lwip/timers.h"
#include "lwip/inet_chksum.h"
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"

#include "audiocontroltask.h"
#include "device.h"

#define HTTPD_DEBUG 		LWIP_DBG_ON
#define WS_SEND_TO_CLIENT(pcb,ws,data,len)	ws_senddata_(pcb,ws,ws_make_frame(data,len,0x01))

enum tcp_server_states
{
    WS_TCPSERVER_NONE = 0,
    WS_TCPSERVER_ACCEPTED,
    WS_TCPSERVER_HANDSHAKE,
    WS_TCPSERVER_CLOSING,
};

struct websocket_state
{
    unsigned char state;
    struct tcp_pcb *pcb;
    struct pbuf *p;
};

#define UP		"up"
#define DOWN	"down"
#define LEFT	"left"
#define RIGHT	"right"
#define OK		"ok"
unsigned char play_ = 0;
int ws_senddata_(struct tcp_pcb *pcb, struct websocket_state * ws,struct pbuf *p);
struct pbuf *ws_make_frame(unsigned char *data,unsigned int len,unsigned char frame_type);
int ws_server_recv_msg(struct tcp_pcb *pcb, struct websocket_state * ws,unsigned char *data,unsigned int len)
{
    if (memcmp(data,UP,len)==0)
    {
        printf("%s\r\n",UP);
        music_shell_set_vol_up(UP);
    }
    else if (memcmp(data,DOWN,len)==0)
    {
        printf("%s\r\n",DOWN);
        music_shell_set_vol_down(DOWN);
    }
    else if (memcmp(data,LEFT,len)==0)
    {
        printf("%s\r\n",LEFT);
        music_shell_up(LEFT);
    }
    else if (memcmp(data,RIGHT,len)==0)
    {
        printf("%s\r\n",RIGHT);
        music_shell_next(RIGHT);
    }
    else if (memcmp(data,OK,len)==0)
    {
        printf("%s\r\n",OK);

        if (play_)
            music_shell_play(OK);
        else
            music_shell_pause(OK);

        play_ ^= 0x01;
    }
    else
    {
        printf("recv msg invalid!\r\n");
        WS_SEND_TO_CLIENT(pcb,ws,data,len);
    }
}


const char *sec_key         ="Sec-WebSocket-Key: ";
const char *string_key      = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
const char *strint_response = "HTTP/1.1 101 Switching Protocols\r\nUpgrade: WebSocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept:%s\r\n\r\n";
void key_gen (char *client_key,unsigned int len, char *server_key,unsigned int server_key_len)
{
    char result[20];
    unsigned char key[128];
    memcpy(key,client_key,len);
    memcpy(key + len,string_key,strlen(string_key));
    key[strlen(string_key) + len] = 0;
//	printf("client_key_len:%d key_len:%d key:%s\r\n",len,strlen(string_key) + len,key);
    sha1_buffer (key, strlen(string_key) + len, result);
    base64_encode (result, 20, server_key, server_key_len);
}
int ws_parse_handshake(struct tcp_pcb *pcb, struct websocket_state * ws,struct pbuf *p)
{
    unsigned int sec_key_offset,sec_key_end,i;
    unsigned char handshake[64];
    unsigned char response[256];

    if (pbuf_memcmp(p,0,"GET ",4) == 0)
    {
//		printf("find GET \r\n");
    }
    else
    {
//		printf("no find GET \r\n");
        return -1;
    }

    if ((sec_key_offset = pbuf_memfind(p,sec_key,strlen(sec_key),0)) != 0xFFFF)
    {
//		printf("find key: %d \r\n",sec_key_offset);
        //for(i=sec_key_offset+strlen(sec_key);i<p->len;i++)
        //{
        //	printf("%c",((char *)(p->payload))[i]);
        //}

        if ((sec_key_end = pbuf_memfind(p,"\r\n",strlen("\r\n"),sec_key_offset+strlen(sec_key))) != 0xFFFF)
        {
            //printf("find sec_key_end: %d \r\n",sec_key_end);
            for (i=sec_key_offset+strlen(sec_key);i<sec_key_end;i++)
            {
                printf("%c",((char *)(p->payload))[i]);
            }
            printf("\r\n");
            //printf("sec_key_end:%d sec_key_offset+strlen(sec_key):%d\r\n ",sec_key_end,sec_key_offset+strlen(sec_key));
            key_gen(&((char *)(p->payload))[sec_key_offset+strlen(sec_key)],sec_key_end - (sec_key_offset+strlen(sec_key)) ,handshake,64);
            printf("%s\r\n",handshake);
            i = sprintf(response,strint_response,handshake);
            response[i]= 0;
            //printf("resp_len:%d::%s\r\n",i,response);
            ws_senddata(pcb,ws,response,i);
        }
        else
        {
            //printf("no find sec_key_end \r\n");
            return -1;
        }
    }
    else
    {
        //printf("no find sec_key \r\n");
        return -1;
    }

    return 0;
}

int ws_get_payload_len(struct pbuf *p,unsigned char *ext_len)
{
    int len;
    unsigned short pyload_len = p->tot_len;
    if (pyload_len < 2)
        return -1;
    len = pbuf_get_at(p,1) & 0x7F;
    *ext_len = 0;

    if (len == 0x7E )
    {
        *ext_len = 2;
        pbuf_copy_partial(p,&len,2,2);
    }
    else if (len == 0x7F)
    {
        *ext_len = 8;
        pbuf_copy_partial(p,&len,6,4);
    }
    return len;
}



int ws_parse_frame(struct pbuf *p,unsigned char *frame_type,unsigned char **payload)
{
    unsigned char mask_buf[4];
    int len,i;
    unsigned char ext_len = 2;
    if (p->tot_len < 2)
        return -1;
    if ((pbuf_get_at(p,0) & 0x80) != 0x80)//we haven't continuation frames support
        return -2;

    len = pbuf_get_at(p,1) & 0x7F;
    *frame_type = pbuf_get_at(p,0) & 0x0F;
    if (len == 0x7E )
    {
        ext_len += 2;
        pbuf_copy_partial(p,&len,2,2);
    }
    else if (len == 0x7F)
    {
        ext_len += 8;
        pbuf_copy_partial(p,&len,6,4);
    }
    if (pbuf_get_at(p,1) & 0x80) //mask bit
    {
        pbuf_copy_partial(p,mask_buf,4,ext_len);
        ext_len+=4;
        for (i = 0; i < len; i++)
        {
            ((unsigned char *)p->payload)[ext_len + i] = ((unsigned char *)p->payload)[ext_len + i] ^ mask_buf[i%4];
        }
    }
    *payload  = ((unsigned char *)p->payload) + ext_len ;
    return len;
}

struct pbuf *ws_make_frame(unsigned char *data,unsigned int len,unsigned char frame_type)
{
    struct pbuf *p;
    unsigned int pubf_len;
    unsigned char ext_len = 2;
    //if(data && len)
    //	return NULL;


    if (len < 125)
    {

    }
    else if (len <0xffff)
    {
        ext_len+= 2;
    }
    else
    {
        ext_len+=8;
    }

    p = pbuf_alloc(PBUF_TRANSPORT,len + ext_len,PBUF_RAM);
    if (p == NULL)
        return p;
    memset(p->payload,0,ext_len);
    ((unsigned char*)p->payload)[0] = 0x80 | frame_type;
    if (len < 125)
    {
        ((unsigned char*)p->payload)[1] = len;
    }
    else if (len <0xffff)
    {
        memcpy(((unsigned char*)p->payload) + 2 ,&len,2);
        ((unsigned char*)p->payload)[1] = 126;
    }
    else
    {
        memcpy(((unsigned char*)p->payload) + 6 ,&len,4);
        ((unsigned char*)p->payload)[1] = 127;
    }

    memcpy(((unsigned char*)p->payload) + ext_len ,data,len);

    return p;
}

void ws_server_senddata(struct tcp_pcb *pcb, struct websocket_state * ws)
{
    struct pbuf *ptr;
    //unsigned short plen;
    err_t wr_err=ERR_OK;
    while ((wr_err==ERR_OK)&&ws->p&&(ws->p->len<=tcp_sndbuf(pcb)))
    {
        ptr=ws->p;
        wr_err=tcp_write(pcb,ptr->payload,ptr->len,1);
        if (wr_err==ERR_OK)
        {
            //plen=ptr->len;
            ws->p=ptr->next;
            if (ws->p)pbuf_ref(ws->p);
            pbuf_free(ptr);
            //tcp_recved(pcb,plen);
        }
        else if (wr_err==ERR_MEM)
        {
            ws->p=ptr;
        }
    }
}

int ws_senddata(struct tcp_pcb *pcb, struct websocket_state * ws,unsigned char *data,unsigned int len)
{
    struct pbuf *temp_p;
    temp_p = pbuf_alloc(PBUF_TRANSPORT,len,PBUF_POOL_TX);
    if (temp_p == NULL)
        return -1;
    pbuf_take(temp_p,data,len);
    //if(ws->p ==NULL)
    //{
    //	ws->p = temp_p;
    //}
    //else
    //{
    //	pbuf_cat(ws->p,temp_p);
    //}
    //ws_server_senddata(pcb,ws);
    //return 0;
    return ws_senddata_(pcb,ws,temp_p);
}

int ws_senddata_(struct tcp_pcb *pcb, struct websocket_state * ws,struct pbuf *p)
{

    if (ws->p ==NULL)
    {
        ws->p = p;
    }
    else
    {
        pbuf_cat(ws->p,p);
    }
    ws_server_senddata(pcb,ws);
    return 0;
}

void ws_server_connection_close(struct tcp_pcb *tpcb, struct websocket_state *ws)
{
    printf("ws_server_connection_close");
    tcp_close(tpcb);
    tcp_arg(tpcb,NULL);
    tcp_sent(tpcb,NULL);
    tcp_recv(tpcb,NULL);
    tcp_err(tpcb,NULL);
    tcp_poll(tpcb,NULL,0);
    if (ws)mem_free(ws);
}

err_t
ws_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
    unsigned int i;
    struct pbuf *q;
    struct websocket_state *ws;
    err_t ret_err;
    LWIP_ASSERT("arg != NULL",arg != NULL);
    ws=(struct websocket_state *)arg;
    if (p==NULL)
    {
        ws->state=WS_TCPSERVER_CLOSING;
        ws->p=p;
        ret_err=ERR_OK;
    }
    else if (err!= ERR_OK)
    {
        if (p)pbuf_free(p);
        ret_err=err;
    }
    else if (ws->state==WS_TCPSERVER_ACCEPTED)
    {
        if (p!=NULL)
        {
            //printf("ws_recv_len:%d count:%d",p->tot_len,pbuf_clen(p));

            //for(q=p;q!=NULL;q=q->next)
            //{
            //	for(i=0;i<q->len;i++)
            //	{
            //		printf("%c",((char *)(q->payload))[i]);
            //	}
            //}
            //printf("\r\n");
            //ws_senddata(pcb,ws,p->payload,p->len);
            if (ws_parse_handshake(pcb,ws,p)==0)
                ws->state = WS_TCPSERVER_HANDSHAKE;
            tcp_recved(pcb,p->tot_len);
            pbuf_free(p);
            ret_err=ERR_OK;
        }
    }
    else if (ws->state==WS_TCPSERVER_HANDSHAKE)
    {
        unsigned char frame_type;
        unsigned char *payload;
        int len;
        if (p!=NULL)
        {
            //printf("ws_recv:%d:",p->tot_len);

            //for(q=p;q!=NULL;q=q->next)
            //{
            //	for(i=0;i<q->len;i++)
            //	{
            //		printf("%c",((char *)(q->payload))[i]);
            //	}
            //}
            //printf("\r\n");
            //printf("ws_recv:%d:",p->tot_len);

            //for(q=p;q!=NULL;q=q->next)
            //{
            //	for(i=0;i<q->len;i++)
            //	{
            //		printf("%02x ",((unsigned char *)(q->payload))[i]);
            //	}
            //}
            //printf("\r\n");
            //ws_senddata(pcb,ws,p->payload,p->len);
            len = ws_parse_frame(p,&frame_type,&payload);
            //if(len > 0)
            //{
            //	printf("msg_len:%d\r\n",len);
            //	for(i=0;i<len;i++)
            //	{
            //		printf("%c",payload[i]);
            //	}
            //}
            if (frame_type == 0x01)
            {
                //ws_senddata_(pcb,ws,ws_make_frame(payload,len,0x01));
                ws_server_recv_msg(pcb,ws,payload,len);
            }
            else
            {
                ws_server_connection_close(pcb,ws);
            }

            tcp_recved(pcb,p->tot_len);
            pbuf_free(p);
            ret_err=ERR_OK;
        }
    }
    else
    {
        tcp_recved(pcb,p->tot_len);
        ws->p=NULL;
        pbuf_free(p);
        ret_err=ERR_OK;
    }
    return ret_err;


}

void
ws_err(void *arg, err_t err)
{
    struct websocket_state *ws = (struct websocket_state *)arg;
    LWIP_UNUSED_ARG(err);

    LWIP_DEBUGF(HTTPD_DEBUG, ("http_err: %s", lwip_strerr(err)));
    printf("tcp error:%x\r\n",(unsigned int)arg);
    if (ws != NULL)
    {
        mem_free(ws);
    }
}

err_t
ws_poll(void *arg, struct tcp_pcb *pcb)
{

    struct websocket_state *ws=(struct websocket_state*)arg;
//	printf("ws_poll\r\n");

    if (ws!=NULL)
    {
        if (ws->state==WS_TCPSERVER_CLOSING)
        {
            printf("ws_poll:ws_server_connection_close");
            ws_server_connection_close(pcb,ws);
            return ERR_OK;
        }

    }
    else
    {
        printf("tcp_abort(pcb);");
        tcp_abort(pcb);
        return ERR_OK;
    }
    return ERR_OK;
}

err_t
ws_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
    struct websocket_state *ws;
    LWIP_UNUSED_ARG(len);
    ws = (struct websocket_state *)arg;
//	printf("ws_sent\r\n");

    if (ws->p)
        ws_server_senddata(pcb,ws);

    return ERR_OK;
}

err_t
websocket_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
    struct websocket_state *ws;
    struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen*)arg;
    LWIP_UNUSED_ARG(err);
    LWIP_DEBUGF(HTTPD_DEBUG, ("http_accept %p / %p\n", (void*)pcb, arg));

    tcp_accepted(lpcb);
    tcp_setprio(pcb, 1);

    ws = (struct websocket_state *)mem_malloc(sizeof(struct websocket_state));
    if (ws == NULL)
    {
        LWIP_DEBUGF(HTTPD_DEBUG, ("http_accept: Out of memory, RST\n"));
        return ERR_MEM;
    }

    ws->state=WS_TCPSERVER_ACCEPTED;
    ws->pcb=pcb;
    ws->p=NULL;

    tcp_arg(pcb, ws);
    tcp_recv(pcb, ws_recv);
    tcp_err(pcb, ws_err);
    tcp_poll(pcb, ws_poll, 4);
    tcp_sent(pcb, ws_sent);
    printf("\r\nwebsocket_accep tremote IP:%d.%d.%d.%d:%d \r\n",pcb->remote_ip.addr&0xff,
           (pcb->remote_ip.addr >>8)&0xff,
           (pcb->remote_ip.addr >>16)&0xff,
           (pcb->remote_ip.addr >>24)&0xff,
           pcb->remote_port);
    return ERR_OK;
}

void websocket_init(void)
{
    struct tcp_pcb *pcb;
    err_t err;

    pcb = tcp_new();
    LWIP_ASSERT("httpd_init: tcp_new failed", pcb != NULL);
    tcp_setprio(pcb, 1);

    err = tcp_bind(pcb, IP_ADDR_ANY, 8080);
    LWIP_ASSERT("httpd_init: tcp_bind failed", err == ERR_OK);
    pcb = tcp_listen(pcb);
    LWIP_ASSERT("httpd_init: tcp_listen failed", pcb != NULL);

    tcp_arg(pcb, pcb);
    tcp_accept(pcb, websocket_accept);

}


