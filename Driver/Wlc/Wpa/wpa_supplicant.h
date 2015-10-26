/*
 * wpa_supplicant - Exported functions for wpa_supplicant modules
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
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

#ifndef WPA_SUPPLICANT_H
#define WPA_SUPPLICANT_H

#define _ATTR_WPA_DATA_     __attribute__((section("WpaSupplicant")))

void wpa_supplicant_rx_eapol(void *ctx, uint8 *src_addr,uint8 *buf, size_t len);
uint8 * wpa_alloc_eapol(uint8 type, const void *data, uint16 data_len, size_t *msg_len, void **data_pos);
int wpa_ether_send(const uint8 *dest, uint16 proto, const uint8 *buf, size_t len);


#endif /* WPA_SUPPLICANT_H */
