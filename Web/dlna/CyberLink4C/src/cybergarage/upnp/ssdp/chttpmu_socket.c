/******************************************************************
*
*    CyberLink for C
*
*    Copyright (C) Satoshi Konno 2005
*
*       Copyright (C) 2006 Nokia Corporation. All rights reserved.
*
*       This is licensed under BSD-style license,
*       see file COPYING.
*
*    File: chttpmu_socket.c
*
*    Revision:
*
*    02/18/05
*        - first revision
*
******************************************************************/
#include "source_macro.h"
#ifdef __WIFI_UPNP_C__
#include <cybergarage/upnp/ssdp/cssdp_server.h>
#include <cybergarage/util/clog.h>

//#pragma arm section code = "upnpCode", rodata = "upnpCode", rwdata = "upnpdata", zidata = "upnpdata"


/****************************************
* cg_upnp_httpmu_socket_bind
****************************************/

BOOL cg_upnp_httpmu_socket_bind(CgUpnpHttpMuSocket *sock, const char *mcastAddr, int port, const char *bindAddr)
{
    cg_log_debug_l4("Entering...\n");

    if (cg_socket_bind(sock, port, bindAddr, FALSE, TRUE) == FALSE)
        return FALSE;
	printf("cg_socket_joingroup_e\r\n");
    if (cg_socket_joingroup(sock, mcastAddr, bindAddr) == FALSE) {
        cg_socket_close(sock);
        return FALSE;
    }
	printf("cg_socket_joingroup_l\r\n");

    cg_log_debug_l4("Leaving...\n");

    return TRUE;
}

/****************************************
* cg_upnp_httpmu_socket_recv
****************************************/

ssize_t cg_upnp_httpmu_socket_recv(CgUpnpHttpMuSocket *sock, CgUpnpSSDPPacket *ssdpPkt)
{
    CgDatagramPacket *dgmPkt;
    char *ssdpData;
    ssize_t recvLen;

    cg_log_debug_l4("Entering...\n");

    dgmPkt = cg_upnp_ssdp_packet_getdatagrampacket(ssdpPkt);
    recvLen = cg_socket_recv(sock, dgmPkt);

    if (recvLen <= 0)
        return recvLen;

    ssdpData = cg_socket_datagram_packet_getdata(dgmPkt);

    /* set header information to the packets headerlist,
       this will leave only the request line in the datagram packet
       which is need to verify the message */
    cg_upnp_ssdp_packet_setheader(ssdpPkt, ssdpData);

    cg_log_debug_l4("Leaving...\n");

    return recvLen;
}
//#pragma arm section code
#endif
