/* Copyright (C) 2012 ROCK-CHIP FUZHOU. All Rights Reserved. */
/*
File: lwipopts.h
Desc:
Author: dgl
Date: 2013-11-20
Notes:
#define NO_SYS                          1
#define LWIP_NETCONN                    0
#define LWIP_SOCKET                     0

#define LWIP_RAND             igmp_get_random
#define LWIP_IGMP                         1
#define LWIP_DHCP                        1
#define LWIP_DNS                        1


#define TCP_SND_QUEUELEN                40
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_SND_BUF                     (12 * TCP_MSS)
#define TCP_WND                         (10 * TCP_MSS)

#define MEM_ALIGNMENT                   4
#define MEM_USE_POOLS                   1
#define MEMP_USE_CUSTOM_POOLS           1

#define DHCP_CREATE_RAND_XID            0
#define LWIP_NETIF_API                  1
*/
#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__
#include "source_macro.h"
#ifdef _DRIVER_AP6181_WIFI_C__
#include "include/network/wwd_network_constants.h"
#endif
#define USE_LWIP

#define LWIP_NETIF_API              1

/*
   -----------------------------------------------
   ---------- Platform specific locking ----------
   -----------------------------------------------
*/
#define SYS_LIGHTWEIGHT_PROT        1
#define NO_SYS                      0

/*
   ------------------------------------
   ---------- Memory options ----------
   ------------------------------------
*/
#define MEM_LIBC_MALLOC          1
#define MEMP_MEM_MALLOC          1

/**
 * MEM_ALIGNMENT: should be set to the alignment of the CPU
 *    4 byte alignment -> #define MEM_ALIGNMENT 4
 *    2 byte alignment -> #define MEM_ALIGNMENT 2
 */
#define MEM_ALIGNMENT           4

#define MEM_USE_POOLS           0
#define MEMP_USE_CUSTOM_POOLS    0

/*
   ------------------------------------------------
   ---------- Internal Memory Pool Sizes ----------
   ------------------------------------------------
*/


//#define MEMP_NUM_RAW_PCB 4

/* MEMP_NUM_UDP_PCB: the number of UDP protocol control blocks. One
   per active UDP "connection". */
#define MEMP_NUM_UDP_PCB        20

#define MEMP_NUM_NETDB          10
/**
 * MEMP_NUM_TCP_PCB: the number of simulatenously active TCP connections.
 * (requires the LWIP_TCP option)
 */
#define MEMP_NUM_TCP_PCB         15
#define MEMP_NUM_TCP_SEG         (TCP_SND_QUEUELEN+1)//20

#define SO_REUSE     1
#define LWIP_STATS   0

#define MEMP_NUM_NETCONN                15


//#define MEMP_NUM_REASSDATA 5     // 最多同时在队列等待重装的IP包数，是IP整包，不是IP分片。

//#define MEMP_NUM_ARP_QUEUE 30 //

//#define MEMP_NUM_IGMP_GROUP 8 //

//#define MEMP_NUM_NETBUF 2       // netbufs结构体数目，仅当使用seq API是需要

//#define MEMP_NUM_NETCONN 4     // netconns结构体数目，仅当使用seq API是需要

//#define MEMP_NUM_TCPIP_MSG_API 8 // tcpip_msg结构体数目

//#define MEMP_NUM_TCPIP_MSG_INPKT 8 //接收包时tcpip_msg结构体数目


/*
   --------------------------------
   ---------- ARP options -------
   --------------------------------
*/
//#define LWIP_ARP 1
//#define ARP_TABLE_SIZE              10
//#define ARP_QUEUEING                1

/** ETH_PAD_SIZE: number of bytes added before the ethernet header to ensure
 * alignment of payload after that header. Since the header is 14 bytes long,
 * without this padding e.g. addresses in the IP header will not be aligned
 * on a 32-bit boundary, so setting this to 2 can speed up 32-bit-platforms.
 */

#define ETH_PAD_SIZE                    0

#define ETHARP_SUPPORT_STATIC_ENTRIES   1

/*
   --------------------------------
   ---------- IP options ----------
   --------------------------------
*/
#define IP_FORWARD                      1
#define IP_OPTIONS_ALLOWED              1
#define IP_REASSEMBLY                   0
#define IP_FRAG                         0
/**
 * IP_REASS_MAXAGE: Maximum time (in multiples of IP_TMR_INTERVAL - so seconds, normally)
 * a fragmented IP packet waits for all fragments to arrive. If not all fragments arrived
 * in this time, the whole packet is discarded.
 */
#define IP_REASS_MAXAGE                     3

#define LWIP_RANDOMIZE_INITIAL_LOCAL_PORTS     1



/** STF 8
 * PBUF_POOL_SIZE: the number of buffers in the pbuf pool.
 *
 * This is just a default designed to be overriden by the FreeRTOS.mk makefile
 * To perform this override, define the makefile variable LWIP_NUM_PACKET_BUFFERS_IN_POOL
 */
#ifndef PBUF_POOL_TX_SIZE
#define PBUF_POOL_TX_SIZE                 (50)
#endif

#ifndef PBUF_POOL_RX_SIZE
#define PBUF_POOL_RX_SIZE                 (50)
#endif


/*
 * IP_REASS_MAX_PBUFS: Total maximum amount of pbufs waiting to be reassembled.
 * Since the received pbufs are enqueued, be sure to configure
 * PBUF_POOL_SIZE > IP_REASS_MAX_PBUFS so that the stack is still able to receive
 * packets even if the maximum amount of fragments is enqueued for reassembly!
 *
 */
#if PBUF_POOL_TX_SIZE > 2
#ifndef IP_REASS_MAX_PBUFS
#define IP_REASS_MAX_PBUFS              (PBUF_POOL_TX_SIZE - 2)
#endif
#else
#define IP_REASS_MAX_PBUFS              0
#define IP_REASSEMBLY                   0
#endif

#if IP_REASS_MAX_PBUFS > 1
#ifndef MEMP_NUM_REASSDATA
#define MEMP_NUM_REASSDATA              (IP_REASS_MAX_PBUFS - 1)
#endif
#else
#define MEMP_NUM_REASSDATA              0
#endif
/**
 * PBUF_POOL_BUFSIZE: the size of each pbuf in the pbuf pool. The default is
 * designed to accomodate single full size TCP frame in one pbuf, including
 * TCP_MSS, IP header, and link header.
 */
//#define PBUF_POOL_BUFSIZE              (LWIP_MEM_ALIGN_SIZE(WICED_LINK_MTU) + LWIP_MEM_ALIGN_SIZE(sizeof(struct pbuf)) + 1)

#ifdef TX_PACKET_POOL_SIZE
#define TCP_SND_BUF                    ((TX_PACKET_POOL_SIZE/2) * TCP_MSS)
#else
#define TCP_SND_BUF                    (2 * TCP_MSS)//(6 * TCP_MSS)
#endif

/* TCP Window size */
#ifdef RX_PACKET_POOL_SIZE
#define TCP_WND                        (5 * TCP_MSS) //((RX_PACKET_POOL_SIZE/2) * TCP_MSS)
#endif

/**
 * MEMP_NUM_PBUF: the number of memp struct pbufs (used for PBUF_ROM and PBUF_REF).
 * If the application sends a lot of data out of ROM (or other static memory),
 * this should be set high.
 */
#define MEMP_NUM_PBUF            (PBUF_POOL_TX_SIZE + PBUF_POOL_RX_SIZE + 2 )//20

#define MEMP_NUM_NETBUF          PBUF_POOL_RX_SIZE

/*
   ----------------------------------
   ---------- ICMP options ----------
   ----------------------------------
*/

/*
   ----------------------------------
   ---------- RAW options -----------
   ----------------------------------
*/
#define LWIP_RAW                        0

/*
   ----------------------------------
   ---------- DHCP options ----------
   ----------------------------------
*/
#define LWIP_DHCP                       1

#define DHCP_DOES_ARP_CHECK             0

/*
   ------------------------------------
   ---------- AUTOIP options ----------
   ------------------------------------
*/
/*
   ----------------------------------
   ---------- SNMP options ----------
   ----------------------------------
*/
/*
   ----------------------------------
   ---------- IGMP options ----------
   ----------------------------------
*/

#define LWIP_IGMP                       1


/**
 * When using IGMP, LWIP_RAND() needs to be defined to a random-function returning an u32_t random value
 */
#define LWIP_RAND()                    (42)
/*
   ----------------------------------
   ---------- DNS options -----------
   ----------------------------------
*/

/*
   ---------------------------------
   ---------- UDP options ----------
   ---------------------------------
*/
/*
   ---------------------------------
   ---------- TCP options ----------
   ---------------------------------
*/

/*
 *     LWIP_EVENT_API==1: The user defines lwip_tcp_event() to receive all
 *         events (accept, sent, etc) that happen in the system.
 *     LWIP_CALLBACK_API==1: The PCB callback function is called directly
 *         for the event. This is the default.
*/

#define TCP_MSS                         (1152)
/*
   ----------------------------------
   ---------- Pbuf options ----------
   ----------------------------------
*/



/*
   ------------------------------------------------
   ---------- Network Interfaces options ----------
   ------------------------------------------------
*/
/*
 * LWIP_NETIF_HWADDRHINT==1: Cache link-layer-address hints (e.g. table indices)
*/
#define LWIP_NETIF_HWADDRHINT           1


/*
   ------------------------------------
   ---------- LOOPIF options ----------
   ------------------------------------
*/
/*
   ------------------------------------
   ---------- SLIPIF options ----------
   ------------------------------------
*/


/*
   ------------------------------------
   ---------- Thread options ----------
   ------------------------------------
*/
#define TCPIP_THREAD_NAME               "tcpip_thread"
#define TCPIP_THREAD_STACKSIZE          (512)

#define TCPIP_THREAD_PRIO               65
#define TCPIP_MBOX_SIZE                 100

/*
   ----------------------------------------------
   ---------- Sequential layer options ----------
   ----------------------------------------------
*/
#define LWIP_NETCONN    1
/*
   ------------------------------------
   ---------- Socket options ----------
   ------------------------------------
*/
#define LWIP_SOCKET       1
/*
   ----------------------------------------
   ---------- Statistics options ----------
   ----------------------------------------
*/

/*
   ---------------------------------
   ---------- PPP options ----------
   ---------------------------------
*/
/*
   --------------------------------------
   ---------- Checksum options ----------
   --------------------------------------
*/

/*
   ---------------------------------------
   ---------- Hook options ---------------
   ---------------------------------------
*/

/*
   ---------------------------------------
   ---------- Debugging options ----------
   ---------------------------------------
   */






//#define WICED_LWIP_DEBUG
#ifdef WICED_LWIP_DEBUG
#define LWIP_DEBUG
#define MEMP_OVERFLOW_CHECK            ( 2 )
#define MEMP_SANITY_CHECK              ( 1 )

#define MEM_DEBUG                      (LWIP_DBG_OFF)
#define MEMP_DEBUG                     (LWIP_DBG_OFF)
#define PBUF_DEBUG                     (LWIP_DBG_OFF)
#define API_LIB_DEBUG                  (LWIP_DBG_OFF)
#define API_MSG_DEBUG                  (LWIP_DBG_OFF)
#define TCPIP_DEBUG                    (LWIP_DBG_OFF)
#define NETIF_DEBUG                    (LWIP_DBG_OFF)
#define SOCKETS_DEBUG                  (LWIP_DBG_OFF)
#define DEMO_DEBUG                     (LWIP_DBG_OFF)
#define IP_DEBUG                       (LWIP_DBG_OFF)
#define IP_REASS_DEBUG                 (LWIP_DBG_OFF)
#define RAW_DEBUG                      (LWIP_DBG_OFF)
#define ICMP_DEBUG                     (LWIP_DBG_OFF)
#define UDP_DEBUG                      (LWIP_DBG_OFF)
#define TCP_DEBUG                      (LWIP_DBG_OFF)
#define TCP_INPUT_DEBUG                (LWIP_DBG_OFF)
#define TCP_OUTPUT_DEBUG               (LWIP_DBG_OFF)
#define TCP_RTO_DEBUG                  (LWIP_DBG_OFF)
#define TCP_CWND_DEBUG                 (LWIP_DBG_OFF)
#define TCP_WND_DEBUG                  (LWIP_DBG_OFF)
#define TCP_FR_DEBUG                   (LWIP_DBG_OFF)
#define TCP_QLEN_DEBUG                 (LWIP_DBG_OFF)
#define TCP_RST_DEBUG                  (LWIP_DBG_OFF)
#define PPP_DEBUG                      (LWIP_DBG_OFF)
#define ETHARP_DEBUG                   (LWIP_DBG_OFF)
#define IGMP_DEBUG                     (LWIP_DBG_OFF)
#define INET_DEBUG                     (LWIP_DBG_OFF)
#define SYS_DEBUG                      (LWIP_DBG_OFF)
#define TIMERS_DEBUG                   (LWIP_DBG_OFF)
#define SLIP_DEBUG                     (LWIP_DBG_OFF)
#define DHCP_DEBUG                     (LWIP_DBG_OFF)
#define AUTOIP_DEBUG                   (LWIP_DBG_OFF)
#define SNMP_MSG_DEBUG                 (LWIP_DBG_OFF)
#define SNMP_MIB_DEBUG                 (LWIP_DBG_OFF)
#define DNS_DEBUG                      (LWIP_DBG_OFF)
#endif
#ifdef _DRIVER_AP6181_WIFI_C__
#define LWIP_NOASSERT                LWIP_DBG_OFF

//rk add
#define PBUF_LINK_HLEN                 (WICED_PHYSICAL_HEADER)

#define SUB_ETHERNET_HEADER_SPACE      (WICED_LINK_OVERHEAD_BELOW_ETHERNET_FRAME_MAX)


#endif
/*
#define TCP_SND_QUEUELEN                40
#define MEMP_NUM_TCP_SEG                TCP_SND_QUEUELEN
#define TCP_SND_BUF                     (12 * TCP_MSS)
#define TCP_WND                         (10 * TCP_MSS)
*/


#endif /* __LWIPOPTS_H__ */
