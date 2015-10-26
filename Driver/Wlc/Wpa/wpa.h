/*
 * wpa_supplicant - WPA definitions
 * Copyright (c) 2003-2006, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef WPA_H
#define WPA_H

#include "wpa_defs.h"
#include "wpa_common.h"

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

#define WPA_CAPABILITY_PREAUTH BIT(0)
#define WPA_CAPABILITY_MGMT_FRAME_PROTECTION BIT(6)
#define WPA_CAPABILITY_PEERKEY_ENABLED BIT(9)

#define GENERIC_INFO_ELEM 0xdd
#define RSN_INFO_ELEM 0x30

#ifndef ETH_P_EAPOL
#define ETH_P_EAPOL 0x888e
#endif

#ifndef ETH_P_RSN_PREAUTH
#define ETH_P_RSN_PREAUTH 0x88c7
#endif

enum
{
	REASON_UNSPECIFIED = 1,
	REASON_DEAUTH_LEAVING = 3,
	REASON_INVALID_IE = 13,
	REASON_MICHAEL_MIC_FAILURE = 14,
	REASON_4WAY_HANDSHAKE_TIMEOUT = 15,
	REASON_GROUP_KEY_UPDATE_TIMEOUT = 16,
	REASON_IE_IN_4WAY_DIFFERS = 17,
	REASON_GROUP_CIPHER_NOT_VALID = 18,
	REASON_PAIRWISE_CIPHER_NOT_VALID = 19,
	REASON_AKMP_NOT_VALID = 20,
	REASON_UNSUPPORTED_RSN_IE_VERSION = 21,
	REASON_INVALID_RSN_IE_CAPAB = 22,
	REASON_IEEE_802_1X_AUTH_FAILED = 23,
	REASON_CIPHER_SUITE_REJECTED = 24
};

#define PMKID_LEN 16
/* SIOCSIWENCODEEXT definitions */
#define IW_ENCODE_SEQ_MAX_SIZE	8
/* struct iw_encode_ext ->alg */
#define IW_ENCODE_ALG_NONE	0
#define IW_ENCODE_ALG_WEP	1
#define IW_ENCODE_ALG_TKIP	2
#define IW_ENCODE_ALG_CCMP	3
#define IW_ENCODE_ALG_PMK	4
#define IW_ENCODE_ALG_AES_CMAC	5
/* struct iw_encode_ext ->ext_flags */
#define IW_ENCODE_EXT_TX_SEQ_VALID	0x00000001
#define IW_ENCODE_EXT_RX_SEQ_VALID	0x00000002
#define IW_ENCODE_EXT_GROUP_KEY		0x00000004
#define IW_ENCODE_EXT_SET_TX_KEY	0x00000008

/* Flags for encoding (along with the token) */
#define IW_ENCODE_INDEX		0x00FF	/* Token index (if needed) */
#define IW_ENCODE_FLAGS		0xFF00	/* Flags defined below */
#define IW_ENCODE_MODE		0xF000	/* Modes defined below */
#define IW_ENCODE_DISABLED	0x8000	/* Encoding disabled */
#define IW_ENCODE_ENABLED	0x0000	/* Encoding enabled */
#define IW_ENCODE_RESTRICTED	0x4000	/* Refuse non-encoded packets */
#define IW_ENCODE_OPEN		0x2000	/* Accept non-encoded packets */
#define IW_ENCODE_NOKEY		0x0800  /* Key is write only, so not present */
#define IW_ENCODE_TEMP		0x0400  /* Temporary key */


struct wpa_sm;
struct wpa_ssid;
struct wpa_config_blob;

struct sockaddr {
	unsigned short sa_family;	/* address family, AF_xxx	*/
	char		sa_data[14];	/* 14 bytes of protocol address	*/
};  //wp add

#define	IFNAMSIZ	16

 struct iw_request_info
 {
     unsigned short       cmd;        /* Wireless Extension command */
     unsigned short       flags;      /* More to come ;-) */
 };

 struct	iw_param
{
	signed int		value;		/* The value of the parameter itself */
	unsigned char	fixed;		/* Hardware should not use auto select */
	unsigned char	disabled;	/* Disable the feature */
	unsigned short	flags;		/* Various specifc flags (if any) */
};

struct	iw_point
{
	void 	*pointer;	/* Pointer to the data  (in user space) */
	unsigned short		length;		/* number of fields or size in bytes */
	unsigned short		flags;		/* Optional params */
};

struct	iw_freq
{
	signed int		m;		/* Mantissa */
	signed short	e;		/* Exponent */
	unsigned char	i;		/* List index (when in range struct) */
	unsigned char	flags;		/* Flags (fixed/auto) */
};

struct	iw_quality
{
	unsigned char		qual;		/* link quality (%retries, SNR, missed beacons or better...) */
	unsigned char		level;		/* signal level (dBm) */
	unsigned char		noise;		/* noise level (dBm) */
	unsigned char		updated;	/* Flags to know if updated */
};

union iwreq_data
{
	/* Config - generic */
	char		name[IFNAMSIZ];
	/* Name : used to verify the presence of  wireless extensions.
	 * Name of the protocol/provider... */

	struct iw_point	essid;		/* Extended network name */
	struct iw_param	nwid;		/* network id (or domain - the cell) */
	struct iw_freq	freq;		/* frequency or channel :
					 * 0-1000 = channel
					 * > 1000 = frequency in Hz */

	struct iw_param	sens;		/* signal level threshold */
	struct iw_param	bitrate;	/* default bit rate */
	struct iw_param	txpower;	/* default transmit power */
	struct iw_param	rts;		/* RTS threshold threshold */
	struct iw_param	frag;		/* Fragmentation threshold */
	unsigned int	mode;		/* Operation mode */
	struct iw_param	retry;		/* Retry limits & lifetime */

	struct iw_point	encoding;	/* Encoding stuff : tokens */
	struct iw_param	power;		/* PM duration/timeout */
	struct iw_quality qual;		/* Quality part of statistics */

	struct sockaddr	ap_addr;	/* Access point address */
	struct sockaddr	addr;		/* Destination address (hw/mac) */

	struct iw_param	param;		/* Other small parameters */
	struct iw_point	data;		/* Other large parameters */
};

struct	iwreq
{
	union
	{
		char	ifrn_name[IFNAMSIZ];	/* if name, e.g. "eth0" */
	} ifr_ifrn;

	/* Data part (defined just above) */
	union iwreq_data	u;
};


struct wpa_sm_ctx {
	void *ctx; /* pointer to arbitrary upper level context */

	void (*set_state)(void *ctx, wpa_states state);
	wpa_states (*get_state)(void *ctx);
	void (*req_scan)(void *ctx, int sec, int usec);
	void (*deauthenticate)(void * ctx, int reason_code);
	void (*disassociate)(void *ctx, int reason_code);
	int (*set_key)(void *ctx, wpa_alg alg,
		       const uint8 *addr, int key_idx, int set_tx,
		       const uint8 *seq, size_t seq_len,
		       const uint8 *key, size_t key_len);
	void (*scan)(void *eloop_ctx, void *timeout_ctx);
	struct wpa_ssid * (*get_ssid)(void *ctx);
	int (*get_bssid)(void *ctx, uint8 *bssid);
	int (*ether_send)(void *ctx, const uint8 *dest, uint16 proto, const uint8 *buf,
			  size_t len);
	int (*get_beacon_ie)(void *ctx);
	void (*cancel_auth_timeout)(void *ctx);
	uint8 * (*alloc_eapol)(void *ctx, uint8 type, const void *data, uint16 data_len,
			    size_t *msg_len, void **data_pos);
	int (*add_pmkid)(void *ctx, const uint8 *bssid, const uint8 *pmkid);
	int (*remove_pmkid)(void *ctx, const uint8 *bssid, const uint8 *pmkid);
	void (*set_config_blob)(void *ctx, struct wpa_config_blob *blob);
	const struct wpa_config_blob * (*get_config_blob)(void *ctx,
							  const char *name);
	int (*mlme_setprotection)(void *ctx, const uint8 *addr,
				  int protection_type, int key_type);
};


enum wpa_sm_conf_params {
	RSNA_PMK_LIFETIME /* dot11RSNAConfigPMKLifetime */,
	RSNA_PMK_REAUTH_THRESHOLD /* dot11RSNAConfigPMKReauthThreshold */,
	RSNA_SA_TIMEOUT /* dot11RSNAConfigSATimeout */,
	WPA_PARAM_PROTO,
	WPA_PARAM_PAIRWISE,
	WPA_PARAM_GROUP,
	WPA_PARAM_KEY_MGMT,
	WPA_PARAM_MGMT_GROUP
};

struct wpa_ie_data {
	int proto;
	int pairwise_cipher;
	int group_cipher;
	int key_mgmt;
	int capabilities;
	int num_pmkid;
	const uint8 *pmkid;
	int mgmt_group_cipher;
};

//#define      DBUGPBUF
#define ARPHRD_ETHER 	    1		/* Ethernet 10Mbps		*/

struct	iw_encode_ext
{
	unsigned int		ext_flags; /* IW_ENCODE_EXT_* */
	unsigned char		tx_seq[IW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	unsigned char		rx_seq[IW_ENCODE_SEQ_MAX_SIZE]; /* LSB first */
	struct sockaddr	addr; /* ff:ff:ff:ff:ff:ff for broadcast/multicast
			       * (group) keys or unicast address for
			       * individual keys */
	unsigned short		alg; /* IW_ENCODE_ALG_* */
	unsigned short		key_len;
	unsigned char		key[1];		  //wp key[0]´ýÐÞ¸Ä
};
int wpa_sm_rx_eapol(struct wpa_sm *sm, const uint8 *src_addr, uint8 *buf, size_t len);

#endif /* WPA_H */
