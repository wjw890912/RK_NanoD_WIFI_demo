/*
********************************************************************************************
*
*				  Copyright (c): 2014 - 2014 + 5, zhuzhe
*							   All rights reserved.
*
* FileName: Driver\Wlc\WlcMacro.h
* Owner: zhuzhe
* Date: 2014.7.11
* Time: 10:33:25
* Desc:
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    zhuzhe     2014.7.11     10:33:25   1.0
********************************************************************************************
*/

#ifndef __DRIVER_WLC_WLCMACRO_H__
#define __DRIVER_WLC_WLCMACRO_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
//903 ETH IF    -----------------------------------------------------------------------------------
/*
 *	IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *	and FCS/CRC (frame check sequence).
 */
#define WPA_AUTH


#ifndef OFFSETOF
#define	OFFSETOF(type, member)	((uint)&((type *)0)->member)
#endif

#ifndef ARRAYSIZE
#define ARRAYSIZE(a)		(sizeof(a)/sizeof(a[0]))
#endif



#define BCM_EVENT_MSG_VERSION		2
#define BCM_MSG_IFNAME_MAX		16

#define CH_UPPER_SB			0x01
#define CH_LOWER_SB			0x02
#define CH_EWA_VALID			0x04
#define CH_20MHZ_APART			4
#define CH_10MHZ_APART			2
#define CH_5MHZ_APART			1
#define CH_MAX_2G_CHANNEL		14
#define WLC_MAX_2G_CHANNEL		CH_MAX_2G_CHANNEL
#define	MAXCHANNEL		224

#define WL_CHANSPEC_CHAN_MASK		0x00ff
#define WL_CHANSPEC_CHAN_SHIFT		0

#define WL_CHANSPEC_CTL_SB_MASK		0x0300
#define WL_CHANSPEC_CTL_SB_SHIFT	     8
#define WL_CHANSPEC_CTL_SB_LOWER	0x0100
#define WL_CHANSPEC_CTL_SB_UPPER	0x0200
#define WL_CHANSPEC_CTL_SB_NONE		0x0300

#define WL_CHANSPEC_BW_MASK		0x0C00
#define WL_CHANSPEC_BW_SHIFT		    10
#define WL_CHANSPEC_BW_10		0x0400
#define WL_CHANSPEC_BW_20		0x0800
#define WL_CHANSPEC_BW_40		0x0C00

#define WL_CHANSPEC_BAND_MASK		0xf000
#define WL_CHANSPEC_BAND_SHIFT		12
#define WL_CHANSPEC_BAND_5G		0x1000
#define WL_CHANSPEC_BAND_2G		0x2000
#define INVCHANSPEC			255


#include <Config_ssid.h>
#include <Wpa_common.h>
#include <Wpa_sha1.h>
#include <Wpa_i.h>
#include "Wpa.h"

#define	ETHER_ADDR_LEN		6


#define	ETHER_TYPE_LEN		2


#define	ETHER_CRC_LEN		4


#define	ETHER_HDR_LEN		(ETHER_ADDR_LEN * 2 + ETHER_TYPE_LEN)


#define	ETHER_MIN_LEN		64


#define	ETHER_MIN_DATA		46


#define	ETHER_MAX_LEN		1518


#define	ETHER_MAX_DATA		1500
/*
 *	These are the defined Ethernet Protocol ID's.
 */

/* Modes of operation */
#define IW_MODE_AUTO	0	/* Let the driver decides */
#define IW_MODE_ADHOC	1	/* Single cell network */
#define IW_MODE_INFRA	2	/* Multi cell network, roaming, ... */
#define IW_MODE_MASTER	3	/* Synchronisation master or Access Point */
#define IW_MODE_REPEAT	4	/* Wireless Repeater (forwarder) */
#define IW_MODE_SECOND	5	/* Secondary master/repeater (backup) */
#define IW_MODE_MONITOR	6	/* Passive monitor (listen only) */
#define IW_MODE_MESH	7	/* Mesh (IEEE 802.11s) network */

/* Statistics flags (bitmask in updated) */
#define IW_QUAL_QUAL_UPDATED	0x01	/* Value was updated since last read */
#define IW_QUAL_LEVEL_UPDATED	0x02
#define IW_QUAL_NOISE_UPDATED	0x04
#define IW_QUAL_ALL_UPDATED	0x07
#define IW_QUAL_DBM		0x08	/* Level + Noise are dBm */
#define IW_QUAL_QUAL_INVALID	0x10	/* Driver doesn't provide value */
#define IW_QUAL_LEVEL_INVALID	0x20
#define IW_QUAL_NOISE_INVALID	0x40
#define IW_QUAL_RCPI		0x80	/* Level + Noise are 802.11k RCPI */
#define IW_QUAL_ALL_INVALID	0x70

/* Frequency flags */
#define IW_FREQ_AUTO		0x00	/* Let the driver decides */
#define IW_FREQ_FIXED		0x01	/* Force a specific value */

/* Maximum number of size of encoding token available
 * they are listed in the range structure */
#define IW_MAX_ENCODING_SIZES	8

/* Maximum size of the encoding token in bytes */
#define IW_ENCODING_TOKEN_MAX	64	/* 512 bits (for now) */


#define ETH_P_LOOP	0x0060		/* Ethernet Loopback packet	*/
#define ETH_P_PUP	0x0200		/* Xerox PUP packet		*/
#define ETH_P_PUPAT	0x0201		/* Xerox PUP Addr Trans packet	*/
#define ETH_P_IP	0x0800		/* Internet Protocol packet	*/
#define ETH_P_X25	0x0805		/* CCITT X.25			*/
#define ETH_P_ARP	0x0806		/* Address Resolution packet	*/
#define	ETH_P_BPQ	0x08FF		/* G8BPQ AX.25 Ethernet Packet	[ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_P_IEEEPUP	0x0a00		/* Xerox IEEE802.3 PUP packet */
#define ETH_P_IEEEPUPAT	0x0a01		/* Xerox IEEE802.3 PUP Addr Trans packet */
#define ETH_P_DEC       0x6000          /* DEC Assigned proto           */
#define ETH_P_DNA_DL    0x6001          /* DEC DNA Dump/Load            */
#define ETH_P_DNA_RC    0x6002          /* DEC DNA Remote Console       */
#define ETH_P_DNA_RT    0x6003          /* DEC DNA Routing              */
#define ETH_P_LAT       0x6004          /* DEC LAT                      */
#define ETH_P_DIAG      0x6005          /* DEC Diagnostics              */
#define ETH_P_CUST      0x6006          /* DEC Customer use             */
#define ETH_P_SCA       0x6007          /* DEC Systems Comms Arch       */
#define ETH_P_TEB	0x6558		/* Trans Ether Bridging		*/
#define ETH_P_RARP      0x8035		/* Reverse Addr Res packet	*/
#define ETH_P_ATALK	0x809B		/* Appletalk DDP		*/
#define ETH_P_AARP	0x80F3		/* Appletalk AARP		*/
#define ETH_P_8021Q	0x8100          /* 802.1Q VLAN Extended Header  */
#define ETH_P_IPX	0x8137		/* IPX over DIX			*/
#define ETH_P_IPV6	0x86DD		/* IPv6 over bluebook		*/
#define ETH_P_PAUSE	0x8808		/* IEEE Pause frames. See 802.3 31B */
#define ETH_P_SLOW	0x8809		/* Slow Protocol. See 802.3ad 43B */
#define ETH_P_WCCP	0x883E		/* Web-cache coordination protocol
					 * defined in draft-wilson-wrec-wccp-v2-00.txt */
#define ETH_P_PPP_DISC	0x8863		/* PPPoE discovery messages     */
#define ETH_P_PPP_SES	0x8864		/* PPPoE session messages	*/
#define ETH_P_MPLS_UC	0x8847		/* MPLS Unicast traffic		*/
#define ETH_P_MPLS_MC	0x8848		/* MPLS Multicast traffic	*/
#define ETH_P_ATMMPOA	0x884c		/* MultiProtocol Over ATM	*/
#define ETH_P_LINK_CTL	0x886c		/* HPNA, wlan link local tunnel */
#define ETH_P_ATMFATE	0x8884		/* Frame-based ATM Transport
					 * over Ethernet
					 */
#define ETH_P_PAE	0x888E		/* Port Access Entity (IEEE 802.1X) */
#define ETH_P_AOE	0x88A2		/* ATA over Ethernet		*/
#define ETH_P_TIPC	0x88CA		/* TIPC 			*/
#define ETH_P_1588	0x88F7		/* IEEE 1588 Timesync */
#define ETH_P_FCOE	0x8906		/* Fibre Channel over Ethernet  */
#define ETH_P_FIP	0x8914		/* FCoE Initialization Protocol */
#define ETH_P_EDSA	0xDADA		/* Ethertype DSA [ NOT AN OFFICIALLY REGISTERED ID ] */

/*
 *	Non DIX types. Won't clash for 1500 types.
 */

#define ETH_P_802_3	0x0001		/* Dummy type for 802.3 frames  */
#define ETH_P_AX25	0x0002		/* Dummy protocol id for AX.25  */
#define ETH_P_ALL	0x0003		/* Every packet (be careful!!!) */
#define ETH_P_802_2	0x0004		/* 802.2 frames 		*/
#define ETH_P_SNAP	0x0005		/* Internal only		*/
#define ETH_P_DDCMP     0x0006          /* DEC DDCMP: Internal only     */
#define ETH_P_WAN_PPP   0x0007          /* Dummy type for WAN PPP frames*/
#define ETH_P_PPP_MP    0x0008          /* Dummy type for PPP MP frames */
#define ETH_P_LOCALTALK 0x0009		/* Localtalk pseudo type 	*/
#define ETH_P_CAN	0x000C		/* Controller Area Network      */
#define ETH_P_PPPTALK	0x0010		/* Dummy type for Atalk over PPP*/
#define ETH_P_TR_802_2	0x0011		/* 802.2 frames 		*/
#define ETH_P_MOBITEX	0x0015		/* Mobitex (kaz@cafe.net)	*/
#define ETH_P_CONTROL	0x0016		/* Card specific control frames */
#define ETH_P_IRDA	    0x0017		/* Linux-IrDA			*/
#define ETH_P_ECONET	0x0018		/* Acorn Econet			*/
#define ETH_P_HDLC	0x0019		/* HDLC frames			*/
#define ETH_P_ARCNET	0x001A		/* 1A for ArcNet :-)            */
#define ETH_P_DSA	0x001B		/* Distributed Switch Arch.	*/
#define ETH_P_TRAILER	0x001C		/* Trailer switch tagging	*/
#define ETH_P_PHONET	0x00F5		/* Nokia Phonet frames          */
#define ETH_P_IEEE802154 0x00F6		/* IEEE802.15.4 frame		*/
#define ETH_P_CAIF	0x00F7		/* ST-Ericsson CAIF protocol	*/
/* The level of bus communication with the dongle */



#define WLC_IOCTL_MAGIC     0x14e46c77


#define WLC_IOCTL_VERSION   1

#define WLC_IOCTL_MAXLEN        8192//1024//dgl 8192
#define WLC_IOCTL_SMLEN         256
#define WLC_IOCTL_MEDLEN        1536


#ifndef EPICTRL_COOKIE
#define EPICTRL_COOKIE      0xABADCEDE
#endif


typedef struct _Bss_Descriptor_Info
{
     uint8 total_num;
     Bss_Descriptor *pBss;
}Bss_Descriptor_Info;

#define WL_TXFIFO_SZ_MAGIC  0xa5a5

#define WLC_IOV_NAME_LEN 30

typedef uint16 chanspec_t;

typedef struct wl_bss_info
{
	uint32      version;
	uint32      length;
	struct ether_addr BSSID;
	uint16      beacon_period;
	uint16      capability;
	uint8       SSID_len;
	uint8       SSID[32];
	struct
	{
		uint    count;
		uint8   rates[16];
	} rateset;
	chanspec_t  chanspec;
	uint16      atim_window;
	uint8       dtim_period;
	int16       RSSI;
	int8        phy_noise;

	uint8       n_cap;
	uint32      nbss_cap;
	uint8       ctl_ch;
	uint32      reserved32[1];
	uint8       flags;
	uint8       reserved[3];
	uint8       basic_mcs[16]; //dgl [MCSSET_LEN]

	uint16      ie_offset;
	uint32      ie_length;
	int16       SNR;
} wl_bss_info_t;

typedef struct wlc_iov_trx_s {
	uint8 module;
	uint8 type;
	char name[WLC_IOV_NAME_LEN];
} wlc_iov_trx_t;
typedef struct wl_escan_result {
	uint32 buflen;
	uint32 version;
	uint16 sync_id;
	uint16 bss_count;
    wl_bss_info_t bss_info[1];
} wl_escan_result_t;

typedef struct
{
	uint16	version;
	uint16	flags;
	uint32	event_type;
	uint32	status;
	uint32	reason;
	uint32	auth_type;
	uint32	datalen;
	struct  ether_addr	addr;
	char	ifname[BCM_MSG_IFNAME_MAX];
	uint8	ifidx;
	uint8	bsscfgidx;
} wl_event_msg_t;

typedef struct wl_wsec_key
{
	uint32      index;
	uint32      len;
	uint8       data[32]; //[DOT11_MAX_KEY_SIZE]
	uint32      pad_1[18];
	uint32      algo;
	uint32      flags;
	uint32      pad_2[2];
	int     	pad_3;
	int     	iv_initialized;
	int     	pad_4;

	struct
	{
		uint32  hi;
		uint16  lo;
	} rxiv;
	uint32      pad_5[2];
	struct ether_addr ea;
} wl_wsec_key_t;

#define WLC_IOCTL_MAGIC     0x14e46c77


#define WLC_IOCTL_VERSION   1

#define WLC_IOCTL_MAXLEN        8192//1024//dgl 8192
#define WLC_IOCTL_SMLEN         256
#define WLC_IOCTL_MEDLEN        1536


#define WLC_GET_MAGIC               0
#define WLC_GET_VERSION             1
#define WLC_UP                  2
#define WLC_DOWN                3
#define WLC_GET_LOOP                4
#define WLC_SET_LOOP                5
#define WLC_DUMP                6
#define WLC_GET_MSGLEVEL            7
#define WLC_SET_MSGLEVEL            8
#define WLC_GET_PROMISC             9
#define WLC_SET_PROMISC             10
#define WLC_OVERLAY_IOCTL           11
#define WLC_GET_RATE                12

#define WLC_GET_INSTANCE            14




#define WLC_GET_INFRA               19
#define WLC_SET_INFRA               20
#define WLC_GET_AUTH                21
#define WLC_SET_AUTH                22
#define WLC_GET_BSSID               23
#define WLC_SET_BSSID               24
#define WLC_GET_SSID                25
#define WLC_SET_SSID                26
#define WLC_RESTART             27
#define WLC_TERMINATED             28

#define WLC_GET_CHANNEL             29
#define WLC_SET_CHANNEL             30
#define WLC_GET_SRL             31
#define WLC_SET_SRL             32
#define WLC_GET_LRL             33
#define WLC_SET_LRL             34
#define WLC_GET_PLCPHDR             35
#define WLC_SET_PLCPHDR             36
#define WLC_GET_RADIO               37
#define WLC_SET_RADIO               38
#define WLC_GET_PHYTYPE             39
#define WLC_DUMP_RATE               40
#define WLC_SET_RATE_PARAMS         41
#define WLC_GET_FIXRATE             42
#define WLC_SET_FIXRATE             43


#define WLC_GET_KEY             44
#define WLC_SET_KEY             45
#define WLC_GET_REGULATORY          46
#define WLC_SET_REGULATORY          47
#define WLC_GET_PASSIVE_SCAN            48
#define WLC_SET_PASSIVE_SCAN            49
#define WLC_SCAN                50
#define WLC_SCAN_RESULTS            51
#define WLC_DISASSOC                52
#define WLC_REASSOC             53
#define WLC_GET_ROAM_TRIGGER            54
#define WLC_SET_ROAM_TRIGGER            55
#define WLC_GET_ROAM_DELTA          56
#define WLC_SET_ROAM_DELTA          57
#define WLC_GET_ROAM_SCAN_PERIOD        58
#define WLC_SET_ROAM_SCAN_PERIOD        59
#define WLC_EVM                 60
#define WLC_GET_TXANT               61
#define WLC_SET_TXANT               62
#define WLC_GET_ANTDIV              63
#define WLC_SET_ANTDIV              64


#define WLC_GET_CLOSED              67
#define WLC_SET_CLOSED              68
#define WLC_GET_MACLIST             69
#define WLC_SET_MACLIST             70
#define WLC_GET_RATESET             71
#define WLC_SET_RATESET             72

#define WLC_LONGTRAIN               74
#define WLC_GET_BCNPRD              75
#define WLC_SET_BCNPRD              76
#define WLC_GET_DTIMPRD             77
#define WLC_SET_DTIMPRD             78
#define WLC_GET_SROM                79
#define WLC_SET_SROM                80
#define WLC_GET_WEP_RESTRICT            81
#define WLC_SET_WEP_RESTRICT            82
#define WLC_GET_COUNTRY             83
#define WLC_SET_COUNTRY             84
#define WLC_GET_PM              85
#define WLC_SET_PM              86
#define WLC_GET_WAKE                87
#define WLC_SET_WAKE                88

#define WLC_GET_FORCELINK           90
#define WLC_SET_FORCELINK           91
#define WLC_FREQ_ACCURACY           92
#define WLC_CARRIER_SUPPRESS            93
#define WLC_GET_PHYREG              94
#define WLC_SET_PHYREG              95
#define WLC_GET_RADIOREG            96
#define WLC_SET_RADIOREG            97
#define WLC_GET_REVINFO             98
#define WLC_GET_UCANTDIV            99
#define WLC_SET_UCANTDIV            100
#define WLC_R_REG               101
#define WLC_W_REG               102


#define WLC_GET_MACMODE             105
#define WLC_SET_MACMODE             106
#define WLC_GET_MONITOR             107
#define WLC_SET_MONITOR             108
#define WLC_GET_GMODE               109
#define WLC_SET_GMODE               110
#define WLC_GET_LEGACY_ERP          111
#define WLC_SET_LEGACY_ERP          112
#define WLC_GET_RX_ANT              113
#define WLC_GET_CURR_RATESET            114
#define WLC_GET_SCANSUPPRESS            115
#define WLC_SET_SCANSUPPRESS            116
#define WLC_GET_AP              117
#define WLC_SET_AP              118
#define WLC_GET_EAP_RESTRICT            119
#define WLC_SET_EAP_RESTRICT            120
#define WLC_SCB_AUTHORIZE           121
#define WLC_SCB_DEAUTHORIZE         122
#define WLC_GET_WDSLIST             123
#define WLC_SET_WDSLIST             124
#define WLC_GET_ATIM                125
#define WLC_SET_ATIM                126
#define WLC_GET_RSSI                127
#define WLC_GET_PHYANTDIV           128
#define WLC_SET_PHYANTDIV           129
#define WLC_AP_RX_ONLY              130
#define WLC_GET_TX_PATH_PWR         131
#define WLC_SET_TX_PATH_PWR         132
#define WLC_GET_WSEC                133
#define WLC_SET_WSEC                134
#define WLC_GET_PHY_NOISE           135
#define WLC_GET_BSS_INFO            136
#define WLC_GET_PKTCNTS             137
#define WLC_GET_LAZYWDS             138
#define WLC_SET_LAZYWDS             139
#define WLC_GET_BANDLIST            140
#define WLC_GET_BAND                141
#define WLC_SET_BAND                142
#define WLC_SCB_DEAUTHENTICATE          143
#define WLC_GET_SHORTSLOT           144
#define WLC_GET_SHORTSLOT_OVERRIDE      145
#define WLC_SET_SHORTSLOT_OVERRIDE      146
#define WLC_GET_SHORTSLOT_RESTRICT      147
#define WLC_SET_SHORTSLOT_RESTRICT      148
#define WLC_GET_GMODE_PROTECTION        149
#define WLC_GET_GMODE_PROTECTION_OVERRIDE   150
#define WLC_SET_GMODE_PROTECTION_OVERRIDE   151
#define WLC_UPGRADE             152


#define WLC_GET_IGNORE_BCNS         155
#define WLC_SET_IGNORE_BCNS         156
#define WLC_GET_SCB_TIMEOUT         157
#define WLC_SET_SCB_TIMEOUT         158
#define WLC_GET_ASSOCLIST           159
#define WLC_GET_CLK             160
#define WLC_SET_CLK             161
#define WLC_GET_UP              162
#define WLC_OUT                 163
#define WLC_GET_WPA_AUTH            164
#define WLC_SET_WPA_AUTH            165
#define WLC_GET_UCFLAGS             166
#define WLC_SET_UCFLAGS             167
#define WLC_GET_PWRIDX              168
#define WLC_SET_PWRIDX              169
#define WLC_GET_TSSI                170
#define WLC_GET_SUP_RATESET_OVERRIDE        171
#define WLC_SET_SUP_RATESET_OVERRIDE        172





#define WLC_GET_PROTECTION_CONTROL      178
#define WLC_SET_PROTECTION_CONTROL      179
#define WLC_GET_PHYLIST             180
#define WLC_ENCRYPT_STRENGTH            181
#define WLC_DECRYPT_STATUS          182
#define WLC_GET_KEY_SEQ             183
#define WLC_GET_SCAN_CHANNEL_TIME       184
#define WLC_SET_SCAN_CHANNEL_TIME       185
#define WLC_GET_SCAN_UNASSOC_TIME       186
#define WLC_SET_SCAN_UNASSOC_TIME       187
#define WLC_GET_SCAN_HOME_TIME          188
#define WLC_SET_SCAN_HOME_TIME          189
#define WLC_GET_SCAN_NPROBES            190
#define WLC_SET_SCAN_NPROBES            191
#define WLC_GET_PRB_RESP_TIMEOUT        192
#define WLC_SET_PRB_RESP_TIMEOUT        193
#define WLC_GET_ATTEN               194
#define WLC_SET_ATTEN               195
#define WLC_GET_SHMEM               196
#define WLC_SET_SHMEM               197


#define WLC_SET_WSEC_TEST           200
#define WLC_SCB_DEAUTHENTICATE_FOR_REASON   201
#define WLC_TKIP_COUNTERMEASURES        202
#define WLC_GET_PIOMODE             203
#define WLC_SET_PIOMODE             204
#define WLC_SET_ASSOC_PREFER            205
#define WLC_GET_ASSOC_PREFER            206
#define WLC_SET_ROAM_PREFER         207
#define WLC_GET_ROAM_PREFER         208
#define WLC_SET_LED             209
#define WLC_GET_LED             210
#define WLC_GET_INTERFERENCE_MODE       211
#define WLC_SET_INTERFERENCE_MODE       212
#define WLC_GET_CHANNEL_QA          213
#define WLC_START_CHANNEL_QA            214
#define WLC_GET_CHANNEL_SEL         215
#define WLC_START_CHANNEL_SEL           216
#define WLC_GET_VALID_CHANNELS          217
#define WLC_GET_FAKEFRAG            218
#define WLC_SET_FAKEFRAG            219
#define WLC_GET_PWROUT_PERCENTAGE       220
#define WLC_SET_PWROUT_PERCENTAGE       221
#define WLC_SET_BAD_FRAME_PREEMPT       222
#define WLC_GET_BAD_FRAME_PREEMPT       223
#define WLC_SET_LEAP_LIST           224
#define WLC_GET_LEAP_LIST           225
#define WLC_GET_CWMIN               226
#define WLC_SET_CWMIN               227
#define WLC_GET_CWMAX               228
#define WLC_SET_CWMAX               229
#define WLC_GET_WET             230
#define WLC_SET_WET             231
#define WLC_GET_PUB             232


#define WLC_GET_KEY_PRIMARY         235
#define WLC_SET_KEY_PRIMARY         236

#define WLC_GET_ACI_ARGS            238
#define WLC_SET_ACI_ARGS            239
#define WLC_UNSET_CALLBACK          240
#define WLC_SET_CALLBACK            241
#define WLC_GET_RADAR               242
#define WLC_SET_RADAR               243
#define WLC_SET_SPECT_MANAGMENT         244
#define WLC_GET_SPECT_MANAGMENT         245
#define WLC_WDS_GET_REMOTE_HWADDR       246
#define WLC_WDS_GET_WPA_SUP         247
#define WLC_SET_CS_SCAN_TIMER           248
#define WLC_GET_CS_SCAN_TIMER           249
#define WLC_MEASURE_REQUEST         250
#define WLC_INIT                251
#define WLC_SEND_QUIET              252
#define WLC_KEEPALIVE           253
#define WLC_SEND_PWR_CONSTRAINT         254
#define WLC_UPGRADE_STATUS          255
#define WLC_CURRENT_PWR             256
#define WLC_GET_SCAN_PASSIVE_TIME       257
#define WLC_SET_SCAN_PASSIVE_TIME       258
#define WLC_LEGACY_LINK_BEHAVIOR        259
#define WLC_GET_CHANNELS_IN_COUNTRY     260
#define WLC_GET_COUNTRY_LIST            261
#define WLC_GET_VAR             262
#define WLC_SET_VAR             263
#define WLC_NVRAM_GET               264
#define WLC_NVRAM_SET               265
#define WLC_NVRAM_DUMP              266
#define WLC_REBOOT              267
#define WLC_SET_WSEC_PMK            268
#define WLC_GET_AUTH_MODE           269
#define WLC_SET_AUTH_MODE           270
#define WLC_GET_WAKEENTRY           271
#define WLC_SET_WAKEENTRY           272
#define WLC_NDCONFIG_ITEM           273
#define WLC_NVOTPW              274
#define WLC_OTPW                275
#define WLC_IOV_BLOCK_GET           276
#define WLC_IOV_MODULES_GET         277
#define WLC_SOFT_RESET              278
#define WLC_GET_ALLOW_MODE          279
#define WLC_SET_ALLOW_MODE          280
#define WLC_GET_DESIRED_BSSID           281
#define WLC_SET_DESIRED_BSSID           282
#define WLC_DISASSOC_MYAP           283
#define WLC_GET_NBANDS              284
#define WLC_GET_BANDSTATES          285
#define WLC_GET_WLC_BSS_INFO            286
#define WLC_GET_ASSOC_INFO          287
#define WLC_GET_OID_PHY             288
#define WLC_SET_OID_PHY             289
#define WLC_SET_ASSOC_TIME          290
#define WLC_GET_DESIRED_SSID            291
#define WLC_GET_CHANSPEC            292
#define WLC_GET_ASSOC_STATE         293
#define WLC_SET_PHY_STATE           294
#define WLC_GET_SCAN_PENDING            295
#define WLC_GET_SCANREQ_PENDING         296
#define WLC_GET_PREV_ROAM_REASON        297
#define WLC_SET_PREV_ROAM_REASON        298
#define WLC_GET_BANDSTATES_PI           299
#define WLC_GET_PHY_STATE           300
#define WLC_GET_BSS_WPA_RSN         301
#define WLC_GET_BSS_WPA2_RSN            302
#define WLC_GET_BSS_BCN_TS          303
#define WLC_GET_INT_DISASSOC            304
#define WLC_SET_NUM_PEERS           305
#define WLC_GET_NUM_BSS             306
#define WLC_NPHY_SAMPLE_COLLECT         307
#define WLC_UM_PRIV             308
#define WLC_GET_CMD             309

#define WLC_SET_INTERFERENCE_OVERRIDE_MODE  311
#define WLC_GET_INTERFERENCE_OVERRIDE_MODE  312
#define WLC_GET_WAI_RESTRICT            313
#define WLC_SET_WAI_RESTRICT            314
#define WLC_SET_WAI_REKEY           315
#define WLC_SET_PEAKRATE            316
#define WLC_GET_PEAKRATE            317
#define WLC_LAST                318

#define WL_DECRYPT_STATUS_SUCCESS   1
#define WL_DECRYPT_STATUS_FAILURE   2
#define WL_DECRYPT_STATUS_UNKNOWN   3


#define WLC_UPGRADE_SUCCESS         0
#define WLC_UPGRADE_PENDING         1


#define WL_AUTH_OPEN_SYSTEM     0
#define WL_AUTH_SHARED_KEY      1
#define WL_AUTH_OPEN_SHARED		3


#define WL_RADIO_SW_DISABLE     (1<<0)
#define WL_RADIO_HW_DISABLE     (1<<1)
#define WL_RADIO_MPC_DISABLE        (1<<2)
#define WL_RADIO_COUNTRY_DISABLE    (1<<3)

#define WL_SPURAVOID_OFF    0
#define WL_SPURAVOID_ON1    1
#define WL_SPURAVOID_ON2    2


#define WL_TXPWR_OVERRIDE   (1U<<31)
#define WL_TXPWR_NEG   (1U<<30)

#define WL_PHY_PAVARS_LEN   6

#define WL_PHY_PAVARS2_NUM	3
#define WL_PHY_PAVAR_VER	1

#define MAX_WLIW_IOCTL_LEN 1024

#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
#define	bzero(b, len)			memset((b), '\0', (len))
#define	bcmp(b1, b2, len)      memcmp((b1), (b2), (len))

#define WL_EVENTING_MASK_LEN    16
#define WLC_CNTRY_BUF_SZ    4

typedef struct wl_bss_config
{
	uint32  atim_window;
	uint32  beacon_period;
	uint32  chanspec;
} wl_bss_config_t;

typedef struct wlc_ssid
{
	uint32      SSID_len;
	uint8       SSID[32];
} wlc_ssid_t;

typedef struct wl_country
{
	char country_abbrev[WLC_CNTRY_BUF_SZ];
	int32 rev;
	char ccode[WLC_CNTRY_BUF_SZ];
} wl_country_t;

typedef struct wl_scan_params
{
	wlc_ssid_t ssid;
	struct ether_addr bssid;
	int8 bss_type;
	uint8 scan_type;
	int32 nprobes;
	int32 active_time;
	int32 passive_time;
	int32 home_time;
	int32 channel_num;
	uint16 channel_list[1];
} wl_scan_params_t;
typedef struct wl_escan_params
{
	uint32 version;
	uint16 action;
	uint16 sync_id;
	wl_scan_params_t params;
} wl_escan_params_t;

typedef struct wl_assoc_params
{
	struct ether_addr bssid;
	uint16 bssid_cnt;
	int32 chanspec_num;
	chanspec_t chanspec_list[1];
} wl_assoc_params_t;

typedef struct wl_join_params {
	wlc_ssid_t ssid;
	wl_assoc_params_t params;
} wl_join_params_t;

typedef struct wl_iw_extra_params
{
	int 	target_channel;
} wl_iw_extra_params_t;

#define WL_SCAN_PARAMS_FIXED_SIZE 64
#define ESCAN_REQ_VERSION 1
#define WL_SCAN_ACTION_START      1
#define WL_SCAN_ACTION_CONTINUE   2
#define WL_SCAN_ACTION_ABORT      3

#define QDBM_OFFSET 153     /* Offset for first entry */
#define QDBM_TABLE_LEN 40   /* Table size */
/* Smallest mW value that will round up to the first table entry, QDBM_OFFSET.
 * Value is ( mW(QDBM_OFFSET - 1) + mW(QDBM_OFFSET) ) / 2
 */
#define QDBM_TABLE_LOW_BOUND 6493 /* Low bound */
typedef struct dhd_pub
{
	/* Linkage ponters */
	//osl_t *osh;		/* OSL handle */
	struct dhd_bus *bus;	/* Bus module handle */
	struct dhd_prot *prot;	/* Protocol module handle */
	struct dhd_info *info;  /* Info module handle */
	//struct dhd_cmn	*cmn;	/* dhd_common module handle */

	/* Internal dhd items */
	bool up;		/* Driver up/down (to OS) */
	bool txoff;		/* Transmit flow-controlled */
	bool dongle_reset;  /* TRUE = DEVRESET put dongle into reset */
	uint dhd_bus_state_busstate;
	uint hdrlen;		/* Total DHD header length (proto + bus) */
	uint maxctl;		/* Max size rxctl request from proto to bus */
	uint rxsz;		/* Rx buffer size bus module should use */
	uint8 wme_dp;	/* wme discard priority */

	/* Dongle media info */
	bool iswl;		/* Dongle-resident driver is wl */
	uint drv_version;	/* Version of dongle-resident driver */
	struct ether_addr mac;	/* MAC address obtained from dongle */
	//dngl_stats_t dstats;	/* Stats for dongle-based data */

	/* Additional stats for the bus level */
	uint tx_packets;	/* Data packets sent to dongle */
	uint tx_multicast;	/* Multicast data packets sent to dongle */
	uint tx_errors;	/* Errors in sending data to dongle */
    uint tx_ctlpkts;	/* Control packets sent to dongle */
	uint tx_ctlerrs;	/* Errors sending control frames to dongle */
	uint rx_packets;	/* Packets sent up the network interface */
	uint rx_multicast;	/* Multicast packets sent up the network interface */
	uint rx_errors;	/* Errors processing rx data packets */
	uint rx_ctlpkts;	/* Control frames processed from dongle */
	uint rx_ctlerrs;	/* Errors in processing rx control frames */
	uint rx_dropped;	/* Packets dropped locally (no memory) */
	uint rx_flushed;  /* Packets flushed due to unscheduled sendup thread */
	uint wd_dpc_sched;   /* Number of times dhd dpc scheduled by watchdog timer */

	uint rx_readahead_cnt;	/* Number of packets where header read-ahead was used. */
	uint tx_realloc;	/* Number of tx packets we had to realloc for headroom */
	uint fc_packets;       /* Number of flow control pkts recvd */

	/* Last error return */
	int bcmerror;
	uint tickcnt;

	/* Last error from dongle */
	int dongle_error;

	/* Suspend disable flag and "in suspend" flag */
	int suspend_disable_flag; /* "1" to disable all extra powersaving during suspend */
	int in_suspend;			/* flag set to 1 when early suspend called */
	int pno_enable;                 /* pno status : "1" is pno enable */
	int dtim_skip;         /* dtim skip , default 0 means wake each dtim */

	/* Pkt filter defination */
	char * pktfilter[100];
	int pktfilter_count;

	wl_country_t dhd_cspec;		/* Current Locale info */
	char eventmask[WL_EVENTING_MASK_LEN];
	int	op_mode;				/* STA, HostAPD, WFD, SoftAP */

	uint16	maxdatablks;
	bool	dongle_isolation;
	int   hang_was_sent;
	int   rxcnt_timeout;		/* counter rxcnt timeout to send HANG */
	int   txcnt_timeout;		/* counter txcnt timeout to send HANG */
    int   ifIdx;
} dhd_pub_t;



#define WEP_ENABLED     0x0001
#define TKIP_ENABLED        0x0002
#define AES_ENABLED     0x0004
#define WSEC_SWFLAG     0x0008
#define SES_OW_ENABLED      0x0040


#define WPA_AUTH_DISABLED   0x0000
#define WPA_AUTH_NONE       0x0001
#define WPA_AUTH_UNSPECIFIED    0x0002
#define WPA_AUTH_PSK        0x0004

#define WPA2_AUTH_UNSPECIFIED   0x0040
#define WPA2_AUTH_PSK       0x0080
#define BRCM_AUTH_PSK           0x0100
#define BRCM_AUTH_DPT       0x0200
#define WPA2_AUTH_MFP           0x1000
#define WPA2_AUTH_TPK		0x2000
#define WPA2_AUTH_FT		0x4000


#define DOT11_MAX_DEFAULT_KEYS  4
#define DOT11_MAX_KEY_SIZE  32
#define DOT11_MAX_IV_SIZE   16
#define DOT11_EXT_IV_FLAG   (1<<5)
#define DOT11_WPA_KEY_RSC_LEN   8

#define WEP1_KEY_SIZE       5
#define WEP1_KEY_HEX_SIZE   10
#define WEP128_KEY_SIZE     13
#define WEP128_KEY_HEX_SIZE 26
#define TKIP_MIC_SIZE       8
#define TKIP_EOM_SIZE       7
#define TKIP_EOM_FLAG       0x5a
#define TKIP_KEY_SIZE       32
#define TKIP_MIC_AUTH_TX    16
#define TKIP_MIC_AUTH_RX    24
#define TKIP_MIC_SUP_RX     TKIP_MIC_AUTH_TX
#define TKIP_MIC_SUP_TX     TKIP_MIC_AUTH_RX
#define AES_KEY_SIZE        16
#define AES_MIC_SIZE        8

#define CRYPTO_ALGO_OFF         0
#define CRYPTO_ALGO_WEP1        1
#define CRYPTO_ALGO_TKIP        2
#define CRYPTO_ALGO_WEP128      3
#define CRYPTO_ALGO_AES_CCM     4
#define CRYPTO_ALGO_AES_OCB_MSDU    5
#define CRYPTO_ALGO_AES_OCB_MPDU    6
#define CRYPTO_ALGO_NALG        7
//#define CRYPTO_ALGO_SMS4        11
#define CRYPTO_ALGO_PMK			12

#define WSEC_GEN_MIC_ERROR  0x0001
#define WSEC_GEN_REPLAY     0x0002
#define WSEC_GEN_ICV_ERROR  0x0004

#define WL_SOFT_KEY (1 << 0)
#define WL_PRIMARY_KEY  (1 << 1)
#define WL_KF_RES_4 (1 << 4)
#define WL_KF_RES_5 (1 << 5)
#define WL_IBSS_PEER_GROUP_KEY  (1 << 6)



#define CH_UPPER_SB			0x01
#define CH_LOWER_SB			0x02
#define CH_EWA_VALID			0x04
#define CH_20MHZ_APART			4
#define CH_10MHZ_APART			2
#define CH_5MHZ_APART			1
#define CH_MAX_2G_CHANNEL		14
#define WLC_MAX_2G_CHANNEL		CH_MAX_2G_CHANNEL
#define	MAXCHANNEL		224

#define WF_CHAN_FACTOR_2_4_G		4814
#define WF_CHAN_FACTOR_5_G		10000
#define WF_CHAN_FACTOR_4_G		8000
#define WL_JOIN_PARAMS_FIXED_SIZE   (sizeof(wl_join_params_t) - sizeof(chanspec_t))

typedef wl_assoc_params_t wl_join_assoc_params_t;

typedef struct wl_join_scan_params {
	uint8 scan_type;
	int32 nprobes;
	int32 active_time;
	int32 passive_time;
	int32 home_time;
} wl_join_scan_params_t;

typedef struct wl_extjoin_params
{
	wlc_ssid_t ssid;
	wl_join_scan_params_t scan;
	wl_join_assoc_params_t assoc;
} wl_extjoin_params_t;
typedef struct wl_iw {
//	char nickname[IW_ESSID_MAX_SIZE];

//	struct iw_statistics wstats;

//	int spy_num;
	int wpaversion;
	int pcipher;
	int gcipher;
	int privacy_invoked;

//	struct ether_addr spy_addr[IW_MAX_SPY];
//	struct iw_quality spy_qual[IW_MAX_SPY];
//	void  *wlinfo;
//	dhd_pub_t * pub;
} wl_iw_t;
struct iw_event
{
	unsigned short		len;			/* Real length of this stuff */
	unsigned short		cmd;			/* Wireless IOCTL */
	union iwreq_data	u;		/* IOCTL fixed payload */
};



#define WL_EXTJOIN_PARAMS_FIXED_SIZE    (sizeof(wl_extjoin_params_t) - sizeof(chanspec_t))

#define IW_AUTH_WPA_VERSION		0
#define IW_AUTH_CIPHER_PAIRWISE		1
#define IW_AUTH_CIPHER_GROUP		2
#define IW_AUTH_KEY_MGMT		3
#define IW_AUTH_TKIP_COUNTERMEASURES	4
#define IW_AUTH_DROP_UNENCRYPTED	5
#define IW_AUTH_80211_AUTH_ALG		6
#define IW_AUTH_WPA_ENABLED		7
#define IW_AUTH_RX_UNENCRYPTED_EAPOL	8
#define IW_AUTH_ROAMING_CONTROL		9
#define IW_AUTH_PRIVACY_INVOKED		10
#define IW_AUTH_CIPHER_GROUP_MGMT	11
#define IW_AUTH_MFP			12

/* IW_AUTH_80211_AUTH_ALG values (bit field) */
#define IW_AUTH_ALG_OPEN_SYSTEM	0x00000001
#define IW_AUTH_ALG_SHARED_KEY	0x00000002
#define IW_AUTH_ALG_LEAP	0x00000004

/* IW_AUTH_KEY_MGMT values (bit field) */
#define IW_AUTH_KEY_MGMT_802_1X	1
#define IW_AUTH_KEY_MGMT_PSK	2


#define WL_ASSOC_PARAMS_FIXED_SIZE  (sizeof(wl_assoc_params_t) - sizeof(chanspec_t))

#define WL_SCAN_ACTIVE_TIME	 40
#define WL_SCAN_PASSIVE_TIME	130

/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   802.11e clarifies the figure in section 7.1.2. The frame body is
   up to 2304 octets long (maximum MSDU size) plus any crypt overhead. */
#define IEEE80211_MAX_DATA_LEN		2304
/* 30 byte 4 addr hdr, 2 byte QoS, 2304 byte MSDU, 12 byte crypt, 4 byte FCS */
#define IEEE80211_MAX_FRAME_LEN		2352

#define IEEE80211_MAX_SSID_LEN		32

#define IEEE80211_MAX_MESH_ID_LEN	32

#define WL_ESCAN_RESULTS_FIXED_SIZE (sizeof(wl_escan_result_t) - sizeof(wl_bss_info_t))

//802.11h
#define DOT11_CAP_ESS               0x0001
#define DOT11_CAP_IBSS              0x0002
#define DOT11_CAP_POLLABLE          0x0004
#define DOT11_CAP_POLL_RQ           0x0008
#define DOT11_CAP_PRIVACY           0x0010
#define DOT11_CAP_SHORT             0x0020
#define DOT11_CAP_PBCC              0x0040
#define DOT11_CAP_AGILITY           0x0080
#define DOT11_CAP_SPECTRUM          0x0100
#define DOT11_CAP_SHORTSLOT         0x0400
#define DOT11_CAP_RRM           0x1000
#define DOT11_CAP_CCK_OFDM          0x2000

#define DOT11_MNG_SSID_ID           0
#define DOT11_MNG_RATES_ID          1
#define DOT11_MNG_FH_PARMS_ID           2
#define DOT11_MNG_DS_PARMS_ID           3
#define DOT11_MNG_CF_PARMS_ID           4
#define DOT11_MNG_TIM_ID            5
#define DOT11_MNG_IBSS_PARMS_ID         6
#define DOT11_MNG_COUNTRY_ID            7
#define DOT11_MNG_HOPPING_PARMS_ID      8
#define DOT11_MNG_HOPPING_TABLE_ID      9
#define DOT11_MNG_REQUEST_ID            10
#define DOT11_MNG_QBSS_LOAD_ID          11
#define DOT11_MNG_EDCA_PARAM_ID         12
#define DOT11_MNG_CHALLENGE_ID          16
#define DOT11_MNG_PWR_CONSTRAINT_ID     32
#define DOT11_MNG_PWR_CAP_ID            33
#define DOT11_MNG_TPC_REQUEST_ID        34
#define DOT11_MNG_TPC_REPORT_ID         35
#define DOT11_MNG_SUPP_CHANNELS_ID      36
#define DOT11_MNG_CHANNEL_SWITCH_ID     37
#define DOT11_MNG_MEASURE_REQUEST_ID        38
#define DOT11_MNG_MEASURE_REPORT_ID     39
#define DOT11_MNG_QUIET_ID          40
#define DOT11_MNG_IBSS_DFS_ID           41
#define DOT11_MNG_ERP_ID            42
#define DOT11_MNG_TS_DELAY_ID           43
#define DOT11_MNG_HT_CAP            45
#define DOT11_MNG_QOS_CAP_ID            46
#define DOT11_MNG_NONERP_ID         47
#define DOT11_MNG_RSN_ID            48
#define DOT11_MNG_EXT_RATES_ID          50
#define DOT11_MNG_AP_CHREP_ID       51
#define DOT11_MNG_NBR_REP_ID        52
#define DOT11_MNG_MDIE_ID       54
#define DOT11_MNG_FTIE_ID       55
#define DOT11_MNG_FT_TI_ID      56
#define DOT11_MNG_RDE_ID			57
#define DOT11_MNG_REGCLASS_ID           59
#define DOT11_MNG_EXT_CSA_ID            60
#define DOT11_MNG_HT_ADD            61
#define DOT11_MNG_EXT_CHANNEL_OFFSET        62

#define DOT11_MNG_RRM_CAP_ID        70
#define DOT11_MNG_HT_BSS_COEXINFO_ID        72
#define DOT11_MNG_HT_BSS_CHANNEL_REPORT_ID  73
#define DOT11_MNG_HT_OBSS_ID            74
#define DOT11_MNG_CHANNEL_USAGE			97
#define DOT11_MNG_LINK_IDENTIFIER_ID	101
#define DOT11_MNG_WAKEUP_SCHEDULE_ID	102
#define DOT11_MNG_CHANNEL_SWITCH_TIMING_ID	104
#define DOT11_MNG_PTI_CONTROL_ID		105
#define DOT11_MNG_PU_BUFFER_STATUS_ID	106
#define DOT11_MNG_EXT_CAP_ID           127
#define DOT11_MNG_WPA_ID            221
#define DOT11_MNG_PROPR_ID          221

/* Max number of char in custom event - use multiple of them if needed */
#define IW_CUSTOM_MAX		256	/* In bytes */

/* Generic information element */
#define IW_GENERIC_IE_MAX	1024

/* MLME requests (SIOCSIWMLME / struct iw_mlme) */
#define IW_MLME_DEAUTH		0
#define IW_MLME_DISASSOC	1
#define IW_MLME_AUTH		2
#define IW_MLME_ASSOC		3

/* SIOCSIWAUTH/SIOCGIWAUTH struct iw_param flags */
#define IW_AUTH_INDEX		0x0FFF
#define IW_AUTH_FLAGS		0xF000
/* SIOCSIWAUTH/SIOCGIWAUTH parameters (0 .. 4095)
 * (IW_AUTH_INDEX mask in struct iw_param flags; this is the index of the
 * parameter that is being set/get to; value will be read/written to
 * struct iw_param value field) */
#define IW_AUTH_WPA_VERSION		0
#define IW_AUTH_CIPHER_PAIRWISE		1
#define IW_AUTH_CIPHER_GROUP		2
#define IW_AUTH_KEY_MGMT		3
#define IW_AUTH_TKIP_COUNTERMEASURES	4
#define IW_AUTH_DROP_UNENCRYPTED	5
#define IW_AUTH_80211_AUTH_ALG		6
#define IW_AUTH_WPA_ENABLED		7
#define IW_AUTH_RX_UNENCRYPTED_EAPOL	8
#define IW_AUTH_ROAMING_CONTROL		9
#define IW_AUTH_PRIVACY_INVOKED		10
#define IW_AUTH_CIPHER_GROUP_MGMT	11
#define IW_AUTH_MFP			12

/* IW_AUTH_WPA_VERSION values (bit field) */
#define IW_AUTH_WPA_VERSION_DISABLED	0x00000001
#define IW_AUTH_WPA_VERSION_WPA		0x00000002
#define IW_AUTH_WPA_VERSION_WPA2	0x00000004

/* IW_AUTH_PAIRWISE_CIPHER, IW_AUTH_GROUP_CIPHER, and IW_AUTH_CIPHER_GROUP_MGMT
 * values (bit field) */
#define IW_AUTH_CIPHER_NONE	0x00000001
#define IW_AUTH_CIPHER_WEP40	0x00000002
#define IW_AUTH_CIPHER_TKIP	0x00000004
#define IW_AUTH_CIPHER_CCMP	0x00000008
#define IW_AUTH_CIPHER_WEP104	0x00000010
#define IW_AUTH_CIPHER_AES_CMAC	0x00000020

/* IW_AUTH_KEY_MGMT values (bit field) */
#define IW_AUTH_KEY_MGMT_802_1X	1
#define IW_AUTH_KEY_MGMT_PSK	2

/* IW_AUTH_80211_AUTH_ALG values (bit field) */
#define IW_AUTH_ALG_OPEN_SYSTEM	0x00000001
#define IW_AUTH_ALG_SHARED_KEY	0x00000002
#define IW_AUTH_ALG_LEAP	0x00000004

/* IW_AUTH_ROAMING_CONTROL values */
#define IW_AUTH_ROAMING_ENABLE	0	/* driver/firmware based roaming */
#define IW_AUTH_ROAMING_DISABLE	1	/* user space program used for roaming
					 * control */

/* IW_AUTH_MFP (management frame protection) values */
#define IW_AUTH_MFP_DISABLED	0	/* MFP disabled */
#define IW_AUTH_MFP_OPTIONAL	1	/* MFP optional */
#define IW_AUTH_MFP_REQUIRED	2	/* MFP required */



/* IWEVMICHAELMICFAILURE : struct iw_michaelmicfailure ->flags */
#define IW_MICFAILURE_KEY_ID	0x00000003 /* Key ID 0..3 */
#define IW_MICFAILURE_GROUP	0x00000004
#define IW_MICFAILURE_PAIRWISE	0x00000008
#define IW_MICFAILURE_STAKEY	0x00000010
#define IW_MICFAILURE_COUNT	0x00000060 /* 1 or 2 (0 = count not supported)
					    */

/* Bit field values for enc_capa in struct iw_range */
#define IW_ENC_CAPA_WPA		0x00000001
#define IW_ENC_CAPA_WPA2	0x00000002
#define IW_ENC_CAPA_CIPHER_TKIP	0x00000004
#define IW_ENC_CAPA_CIPHER_CCMP	0x00000008
#define IW_ENC_CAPA_4WAY_HANDSHAKE	0x00000010

/* Event capability macros - in (struct iw_range *)->event_capa
 * Because we have more than 32 possible events, we use an array of
 * 32 bit bitmasks. Note : 32 bits = 0x20 = 2^5. */
#define IW_EVENT_CAPA_BASE(cmd)		((cmd >= SIOCIWFIRSTPRIV) ? \
					 (cmd - SIOCIWFIRSTPRIV + 0x60) : \
					 (cmd - SIOCIWFIRST))
#define IW_EVENT_CAPA_INDEX(cmd)	(IW_EVENT_CAPA_BASE(cmd) >> 5)
#define IW_EVENT_CAPA_MASK(cmd)		(1 << (IW_EVENT_CAPA_BASE(cmd) & 0x1F))
/* Event capability constants - event autogenerated by the kernel
 * This list is valid for most 802.11 devices, customise as needed... */
#define IW_EVENT_CAPA_K_0	(IW_EVENT_CAPA_MASK(0x8B04) | \
				 IW_EVENT_CAPA_MASK(0x8B06) | \
				 IW_EVENT_CAPA_MASK(0x8B1A))
#define IW_EVENT_CAPA_K_1	(IW_EVENT_CAPA_MASK(0x8B2A))
/* "Easy" macro to set events in iw_range (less efficient) */

#define CHSPEC_CHANNEL(chspec)	((uint8)((chspec) & WL_CHANSPEC_CHAN_MASK))

typedef struct bcm_tlv {
	uint8	id;
	uint8	len;
	uint8	data[1];
} bcm_tlv_t;

typedef struct wl_mkeep_alive_pkt {
	uint16	version;
	uint16	length;
	uint32	period_msec;
	uint16	len_bytes;
	uint8	keep_alive_id;
	uint8	data[1];
} wl_mkeep_alive_pkt_t;

#define WL_MKEEP_ALIVE_VERSION		1
#define WL_MKEEP_ALIVE_FIXED_LEN	OFFSETOF(wl_mkeep_alive_pkt_t, data)
#define WL_MKEEP_ALIVE_PRECISION	500


#define DOT11_BSSTYPE_INFRASTRUCTURE        0
#define DOT11_BSSTYPE_INDEPENDENT       1
#define DOT11_BSSTYPE_ANY           2
#define DOT11_SCANTYPE_ACTIVE           0
#define DOT11_SCANTYPE_PASSIVE          1

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define WL_SCAN_PARAMS_COUNT_MASK 0x0000ffff
#define WL_SCAN_PARAMS_NSSID_SHIFT 16

#define WL_SCAN_ACTION_START      1
#define WL_SCAN_ACTION_CONTINUE   2
#define WL_SCAN_ACTION_ABORT      3

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

#ifndef BIT
#define BIT(n) (1 << (n))
#endif
#define WPA_KEY_MGMT_IEEE8021X BIT(0)
#define WPA_KEY_MGMT_PSK BIT(1)
#define WPA_KEY_MGMT_NONE BIT(2)
#define WPA_KEY_MGMT_IEEE8021X_NO_WPA BIT(3)
#define WPA_KEY_MGMT_WPA_NONE BIT(4)


#define WPA_PROTO_WPA BIT(0)
#define WPA_PROTO_RSN BIT(1)

typedef struct {
	uint32  val;
	struct ether_addr ea;
} scb_val_t;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/



#endif

