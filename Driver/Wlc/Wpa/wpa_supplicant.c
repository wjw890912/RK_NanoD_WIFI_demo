/*
 * WPA Supplicant
 * Copyright (c) 2003-2008, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 *
 * This file implements functions for registering and unregistering
 * %wpa_supplicant interfaces. In addition, this file contains number of
 * functions for managing network connections.
 */
#include "build_config.h"
#include "wpa_include.h"
#include "ws_common.h"
#include "wpa.h"
#include "wpa_supplicant.h"
#include "config_ssid.h"
#include "wpa_i.h"
//#include "rk903.h"
#include "wpa_sha1.h"
#include "device.h"
#include "WlcDevice.h"

#ifdef WPA_AUTH


uint8 * wpa_alloc_eapol(uint8 type, const void *data, u16 data_len,
                            size_t *msg_len, void **data_pos)
{
	struct ieee802_1x_hdr *hdr;

	*msg_len = sizeof(*hdr) + data_len;

	hdr = os_malloc(*msg_len);
	if (hdr == NULL)
		return NULL;

	hdr->version = 1;
	hdr->type = type;
	hdr->length = host_to_be16(data_len);

	if (data)
		os_memcpy(hdr + 1, data, data_len);
	else
		os_memset(hdr + 1, 0, data_len);

	if (data_pos)
	{
		*data_pos = hdr + 1;
	}

	return (uint8 *)hdr;
}


/**
 * wpa_ether_send - Send Ethernet frame
 * @wpa_s: Pointer to wpa_supplicant data
 * @dest: Destination MAC address
 * @proto: Ethertype in host byte order
 * @buf: Frame payload starting from IEEE 802.1X header
 * @len: Frame payload length
 * Returns: >=0 on success, <0 on failure
 */
int wpa_ether_send(const uint8 *dest, u16 proto, const uint8 *buf, size_t len)
{
	int ret;
    unsigned char  tempbuf[256];

    memset(tempbuf, 0, 256);
    memcpy(tempbuf, dest, 6);
    RK903Dev_GetMac(tempbuf+6);

    tempbuf[12] = 0x88;
    tempbuf[13] = 0x8e;
    memcpy(tempbuf+14, buf, len);

    len += 14;   /*14:ethernet header*/

    ret = dhd_senddata(NULL, tempbuf, len);
    if (ret)
    {
        printf("eapol_host2card error\n");
	}

	return ret;
}

void wpa_supplicant_rx_eapol(void *ctx, uint8 *src_addr, uint8 *buf, size_t len)
{
	Wpa_Sm * wpa = ctx;

	if (wpa == NULL)
	{
		 WlcDev_GetWpaSm(&wpa);
	}

	/*if the type is not psk, Ignored it. */
	if (wpa->key_mgmt != WPA_KEY_MGMT_PSK)
		return;

	wpa_sm_rx_eapol(wpa, src_addr, buf, len);

	return;
}


#endif	 /*WPA_AUTH*/
