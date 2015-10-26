/*
 * WPA Supplicant - driver interaction with generic Linux Wireless Extensions
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
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
 * This file implements a driver interface for the Linux Wireless Extensions.
 * When used with WE-18 or newer, this interface can be used as-is with number
 * of drivers. In addition to this, some of the common functions in this file
 * can be used by other driver interface implementations that use generic WE
 * ioctls, but require private ioctls for some of the functionality.
 */

#include "build_config.h"

//#include "rk903.h"
#include "ws_common.h"
#include "wpadriver.h"
#include "wpa_supplicant.h"
#include "priv_netlink.h"
#include "driver_wext.h"
#include "wpa.h"

#ifdef WPA_AUTH

#define htod32(i) i
#define htod16(i) i
#define dtoh32(i) i
#define dtoh16(i) i

int wpa_driver_wext_set_key_ext(void *priv, wpa_alg alg, const u8 *addr, int key_idx,
				       int set_tx, const u8 *seq, size_t seq_len, const u8 *key, size_t key_len)
{
	struct iwreq iwr;
	int ret = 0, i;
	struct iw_encode_ext *ext;
	uint32 totallen;

	if (seq_len > IW_ENCODE_SEQ_MAX_SIZE)
	{
		return -1;
	}

	totallen = sizeof(*ext) + key_len;
	ext = os_malloc(totallen);
	if (ext == NULL)
		return -1;

	os_memset(&iwr, 0, sizeof(iwr));
	iwr.u.encoding.flags = key_idx + 1;
	if (alg == WPA_ALG_NONE)
	{
		iwr.u.encoding.flags |= IW_ENCODE_DISABLED;
	}
	iwr.u.encoding.pointer = (void *)ext;
	iwr.u.encoding.length = totallen;

	if (addr == NULL || os_memcmp(addr, "\xff\xff\xff\xff\xff\xff", ETH_ALEN) == 0)
	{
		ext->ext_flags |= IW_ENCODE_EXT_GROUP_KEY;
	}
	if (set_tx)
	{
		ext->ext_flags |= IW_ENCODE_EXT_SET_TX_KEY;
	}

	ext->addr.sa_family = ARPHRD_ETHER;
	if (addr)
	{
		os_memcpy(ext->addr.sa_data, addr, ETH_ALEN);
	}
	else
	{
		os_memset(ext->addr.sa_data, 0xff, ETH_ALEN);
	}

	if (key && key_len)
	{
		os_memcpy(&ext->key[0], key, key_len);
		ext->key_len = key_len;
	}
	switch (alg)
	{
	case WPA_ALG_NONE:
		ext->alg = IW_ENCODE_ALG_NONE;
		break;
	case WPA_ALG_WEP:
		ext->alg = IW_ENCODE_ALG_WEP;
		break;
	case WPA_ALG_TKIP:
		ext->alg = IW_ENCODE_ALG_TKIP;
		break;
	case WPA_ALG_CCMP:
		ext->alg = IW_ENCODE_ALG_CCMP;
		break;
	default:
		os_free(ext);
		return -1;
	}

	if (seq && seq_len)
	{
		ext->ext_flags |= IW_ENCODE_EXT_RX_SEQ_VALID;
		os_memcpy(ext->rx_seq, seq, seq_len);
	}

    WlcDev_SetEncodeExt(NULL, NULL, &(iwr.u.data), iwr.u.data.pointer);
	os_free(ext);
	return ret;
}

#endif /*WPA_AUTH*/

