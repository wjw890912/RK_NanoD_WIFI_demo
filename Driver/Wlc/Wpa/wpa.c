/*
 * WPA Supplicant - WPA state machine and EAPOL-Key processing
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
#include "build_config.h"
//#include "OsInclude.h"
#include "ws_common.h"
#include "wpa_md5.h"
#include "wpa_sha1.h"
#include "wpa_rc4.h"
#include "aes_wrap.h"
#include "wpa.h"
#include "wpaconfig.h"
#include "wpa_i.h"
#include "wpa_supplicant.h"
#include "driver_wext.h"

#ifdef WPA_AUTH
static const int WPA_SELECTOR_LEN = 4;
static const u8 WPA_OUI_TYPE[] = { 0x00, 0x50, 0xf2, 1 };
static const u16 WPA_VERSION = 1;
static const u8 WPA_AUTH_KEY_MGMT_NONE[] = { 0x00, 0x50, 0xf2, 0 };
static const u8 WPA_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x50, 0xf2, 2 };
static const u8 WPA_CIPHER_SUITE_NONE[] = { 0x00, 0x50, 0xf2, 0 };
static const u8 WPA_CIPHER_SUITE_WEP40[] = { 0x00, 0x50, 0xf2, 1 };
static const u8 WPA_CIPHER_SUITE_TKIP[] = { 0x00, 0x50, 0xf2, 2 };
#if 0
static const u8 WPA_CIPHER_SUITE_WRAP[] = { 0x00, 0x50, 0xf2, 3 };
#endif
static const u8 WPA_CIPHER_SUITE_CCMP[] = { 0x00, 0x50, 0xf2, 4 };
static const u8 WPA_CIPHER_SUITE_WEP104[] = { 0x00, 0x50, 0xf2, 5 };

/* WPA IE version 1
 * 00-50-f2:1 (OUI:OUI type)
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: TKIP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: TKIP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * WPA Capabilities (2 octets, little endian) (default: 0)
 */



struct wpa_ie_hdr
{
    u8 elem_id;
    u8 len;
    u8 oui[3];
    u8 oui_type;
    u8 version[2];
} ;




static const int RSN_SELECTOR_LEN = 4;
static const u16 RSN_VERSION = 1;
static const u8 RSN_AUTH_KEY_MGMT_UNSPEC_802_1X[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X[] = { 0x00, 0x0f, 0xac, 2 };
static const u8 RSN_CIPHER_SUITE_NONE[] = { 0x00, 0x0f, 0xac, 0 };
static const u8 RSN_CIPHER_SUITE_WEP40[] = { 0x00, 0x0f, 0xac, 1 };
static const u8 RSN_CIPHER_SUITE_TKIP[] = { 0x00, 0x0f, 0xac, 2 };
#if 0
static const u8 RSN_CIPHER_SUITE_WRAP[] = { 0x00, 0x0f, 0xac, 3 };
#endif
static const u8 RSN_CIPHER_SUITE_CCMP[] = { 0x00, 0x0f, 0xac, 4 };
static const u8 RSN_CIPHER_SUITE_WEP104[] = { 0x00, 0x0f, 0xac, 5 };
#ifdef CONFIG_IEEE80211W
static const u8 RSN_CIPHER_SUITE_AES_128_CMAC[] = { 0x00, 0x0f, 0xac, 6 };
#endif /* CONFIG_IEEE80211W */

/* EAPOL-Key Key Data Encapsulation
 * GroupKey and PeerKey require encryption, otherwise, encryption is optional.
 */
static const u8 RSN_KEY_DATA_GROUPKEY[] = { 0x00, 0x0f, 0xac, 1 };
#if 0
static const u8 RSN_KEY_DATA_STAKEY[] = { 0x00, 0x0f, 0xac, 2 };
#endif
static const u8 RSN_KEY_DATA_MAC_ADDR[] = { 0x00, 0x0f, 0xac, 3 };
static const u8 RSN_KEY_DATA_PMKID[] = { 0x00, 0x0f, 0xac, 4 };
#ifdef CONFIG_PEERKEY
static const u8 RSN_KEY_DATA_SMK[] = { 0x00, 0x0f, 0xac, 5 };
static const u8 RSN_KEY_DATA_NONCE[] = { 0x00, 0x0f, 0xac, 6 };
static const u8 RSN_KEY_DATA_LIFETIME[] = { 0x00, 0x0f, 0xac, 7 };
static const u8 RSN_KEY_DATA_ERROR[] = { 0x00, 0x0f, 0xac, 8 };
#endif /* CONFIG_PEERKEY */
#ifdef CONFIG_IEEE80211W
/* FIX: IEEE 802.11w/D1.0 is using subtypes 5 and 6 for these, but they were
 * already taken by 802.11ma (PeerKey). Need to update the values here once
 * IEEE 802.11w fixes these. */
static const u8 RSN_KEY_DATA_DHV[] = { 0x00, 0x0f, 0xac, 9 };
static const u8 RSN_KEY_DATA_IGTK[] = { 0x00, 0x0f, 0xac, 10 };
#endif /* CONFIG_IEEE80211W */

#ifdef CONFIG_PEERKEY
enum
{
    STK_MUI_4WAY_STA_AP = 1,
    STK_MUI_4WAY_STAT_STA = 2,
    STK_MUI_GTK = 3,
    STK_MUI_SMK = 4
};

enum
{
    STK_ERR_STA_NR = 1,
    STK_ERR_STA_NRSN = 2,
    STK_ERR_CPHR_NS = 3,
    STK_ERR_NO_STSL = 4
};
#endif /* CONFIG_PEERKEY */

/* 1/4: PMKID
 * 2/4: RSN IE
 * 3/4: one or two RSN IEs + GTK IE (encrypted)
 * 4/4: empty
 * 1/2: GTK IE (encrypted)
 * 2/2: empty
 */

/* RSN IE version 1
 * 0x01 0x00 (version; little endian)
 * (all following fields are optional:)
 * Group Suite Selector (4 octets) (default: CCMP)
 * Pairwise Suite Count (2 octets, little endian) (default: 1)
 * Pairwise Suite List (4 * n octets) (default: CCMP)
 * Authenticated Key Management Suite Count (2 octets, little endian)
 *    (default: 1)
 * Authenticated Key Management Suite List (4 * n octets)
 *    (default: unspec 802.1X)
 * RSN Capabilities (2 octets, little endian) (default: 0)
 * PMKID Count (2 octets) (default: 0)
 * PMKID List (16 * n octets)
 * Management Group Cipher Suite (4 octets) (default: AES-128-CMAC)
 */


struct rsn_ie_hdr
{
    u8 elem_id; /* WLAN_EID_RSN */
    u8 len;
    u8 version[2];

};


struct wpa_eapol_key
{
    u8 type;
    /* Note: key_info, key_length, and key_data_length are unaligned */
    u8 key_info[2];
    u8 key_length[2];
    u8 replay_counter[WPA_REPLAY_COUNTER_LEN];
    u8 key_nonce[WPA_NONCE_LEN];
    u8 key_iv[16];
    u8 key_rsc[8];
    u8 key_id[8]; /* Reserved in IEEE 802.11i/RSN */
    u8 key_mic[16];
    u8 key_data_length[2];
    /* followed by key_data_length bytes of key_data */
} ;


struct rsn_error_kde
{
    u16 mui;
    u16 error_type;
};



#define WPA_KEY_INFO_TYPE_MASK ((u16) (BIT(0) | BIT(1) | BIT(2)))
#define WPA_KEY_INFO_TYPE_HMAC_MD5_RC4 BIT(0)
#define WPA_KEY_INFO_TYPE_HMAC_SHA1_AES BIT(1)
#define WPA_KEY_INFO_KEY_TYPE BIT(3) /* 1 = Pairwise, 0 = Group key */
/* bit4..5 is used in WPA, but is reserved in IEEE 802.11i/RSN */
#define WPA_KEY_INFO_KEY_INDEX_MASK (BIT(4) | BIT(5))
#define WPA_KEY_INFO_KEY_INDEX_SHIFT 4
#define WPA_KEY_INFO_INSTALL BIT(6) /* pairwise */
#define WPA_KEY_INFO_TXRX BIT(6) /* group */
#define WPA_KEY_INFO_ACK BIT(7)
#define WPA_KEY_INFO_MIC BIT(8)
#define WPA_KEY_INFO_SECURE BIT(9)
#define WPA_KEY_INFO_ERROR BIT(10)
#define WPA_KEY_INFO_REQUEST BIT(11)
#define WPA_KEY_INFO_ENCR_KEY_DATA BIT(12) /* IEEE 802.11i/RSN only */
#define WPA_KEY_INFO_SMK_MESSAGE BIT(13)

/**
 * wpa_cipher_txt - Convert cipher suite to a text string
 * @cipher: Cipher suite (WPA_CIPHER_* enum)
 * Returns: Pointer to a text string of the cipher suite name
 */
static const char * wpa_cipher_txt(int cipher)
{
    switch (cipher)
    {
        case WPA_CIPHER_NONE:
            return "NONE";
        case WPA_CIPHER_WEP40:
            return "WEP-40";
        case WPA_CIPHER_WEP104:
            return "WEP-104";
        case WPA_CIPHER_TKIP:
            return "TKIP";
        case WPA_CIPHER_CCMP:
            return "CCMP";
        default:
            return "UNKNOWN";
    }
}


/**
 * wpa_pmk_to_ptk - Calculate PTK from PMK, addresses, and nonces
 * @pmk: Pairwise master key
 * @pmk_len: Length of PMK
 * @label: Label to use in derivation
 * @addr1: AA or SA
 * @addr2: SA or AA
 * @nonce1: ANonce or SNonce
 * @nonce2: SNonce or ANonce
 * @ptk: Buffer for pairwise transient key
 * @ptk_len: Length of PTK
 *
 * IEEE Std 802.11i-2004 - 8.5.1.2 Pairwise key hierarchy
 * PTK = PRF-X(PMK, "Pairwise key expansion",
 *             Min(AA, SA) || Max(AA, SA) ||
 *             Min(ANonce, SNonce) || Max(ANonce, SNonce))
 *
 * STK = PRF-X(SMK, "Peer key expansion",
 *             Min(MAC_I, MAC_P) || Max(MAC_I, MAC_P) ||
 *             Min(INonce, PNonce) || Max(INonce, PNonce))
 */
static void wpa_pmk_to_ptk(const u8 *pmk, size_t pmk_len,
                           const char *label,
                           const u8 *addr1, const u8 *addr2,
                           const u8 *nonce1, const u8 *nonce2,
                           u8 *ptk, size_t ptk_len)
{
    u8 data[2 * ETH_ALEN + 2 * 32];

    if (os_memcmp(addr1, addr2, ETH_ALEN) < 0)
    {
        os_memcpy(data, addr1, ETH_ALEN);
        os_memcpy(data + ETH_ALEN, addr2, ETH_ALEN);
    }
    else
    {
        os_memcpy(data, addr2, ETH_ALEN);
        os_memcpy(data + ETH_ALEN, addr1, ETH_ALEN);
    }

    if (os_memcmp(nonce1, nonce2, 32) < 0)
    {
        os_memcpy(data + 2 * ETH_ALEN, nonce1, 32);
        os_memcpy(data + 2 * ETH_ALEN + 32, nonce2, 32);
    }
    else
    {
        os_memcpy(data + 2 * ETH_ALEN, nonce2, 32);
        os_memcpy(data + 2 * ETH_ALEN + 32, nonce1, 32);
    }

    sha1_prf(pmk, pmk_len, label, data, sizeof(data), ptk, ptk_len);

    wpa_hexdump_key(MSG_DEBUG, "WPA: PMK", pmk, pmk_len);
    wpa_hexdump_key(MSG_DEBUG, "WPA: PTK", ptk, ptk_len);
}

/**
 * wpa_eapol_key_mic - Calculate EAPOL-Key MIC
 * @key: EAPOL-Key Key Confirmation Key (KCK)
 * @ver: Key descriptor version (WPA_KEY_INFO_TYPE_*)
 * @buf: Pointer to the beginning of the EAPOL header (version field)
 * @len: Length of the EAPOL frame (from EAPOL header to the end of the frame)
 * @mic: Pointer to the buffer to which the EAPOL-Key MIC is written
 *
 * Calculate EAPOL-Key MIC for an EAPOL-Key packet. The EAPOL-Key MIC field has
 * to be cleared (all zeroes) when calling this function.
 *
 * Note: 'IEEE Std 802.11i-2004 - 8.5.2 EAPOL-Key frames' has an error in the
 * description of the Key MIC calculation. It includes packet data from the
 * beginning of the EAPOL-Key header, not EAPOL header. This incorrect change
 * happened during final editing of the standard and the correct behavior is
 * defined in the last draft (IEEE 802.11i/D10).
 */
static void wpa_eapol_key_mic(const u8 *key, int ver, const u8 *buf, size_t len, u8 *mic)
{
    if (ver == WPA_KEY_INFO_TYPE_HMAC_MD5_RC4)
    {
        hmac_md5(key, 16, buf, len, mic);
    }
    else if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES)
    {
        u8 hash[SHA1_MAC_LEN];
        hmac_sha1(key, 16, buf, len, hash);
        os_memcpy(mic, hash, MD5_MAC_LEN);
    }
}

static void wpa_eapol_key_send(struct wpa_sm *sm, const u8 *kck,
                               int ver, const u8 *dest, u16 proto,
                               u8 *msg, size_t msg_len, u8 *key_mic)
{
#if 0
    if (os_memcmp(dest, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0 &&
            os_memcmp(sm->bssid, "\x00\x00\x00\x00\x00\x00", ETH_ALEN) == 0)
    {
        /*
         * Association event was not yet received; try to fetch
         * BSSID from the driver.
         */
        if (wpa_sm_get_bssid(sm, sm->bssid) < 0)
        {
            lwip_log("WPA: Failed to read BSSID for EAPOL-Key destination address\n");
        }
        else
        {
            dest = sm->bssid;
            lwip_log("WPA: Use BSSID (" MACSTR ") as the destination for EAPOL-Key\n",
                     MAC2STR(dest));
        }
    }
#endif
    //generate MIC.
    if (key_mic)
    {
        wpa_eapol_key_mic(kck, ver, msg, msg_len, key_mic);
    }

    wpa_ether_send(dest, proto, msg, msg_len);
    os_free(msg);
}



struct wpa_eapol_ie_parse
{
    const u8 *wpa_ie;
    size_t wpa_ie_len;
    const u8 *rsn_ie;
    size_t rsn_ie_len;
    const u8 *pmkid;
    const u8 *gtk;
    size_t gtk_len;
    const u8 *mac_addr;
    size_t mac_addr_len;

};

/**
 * wpa_supplicant_parse_generic - Parse EAPOL-Key Key Data Generic IEs
 * @pos: Pointer to the IE header
 * @end: Pointer to the end of the Key Data buffer
 * @ie: Pointer to parsed IE data
 * Returns: 0 on success, 1 if end mark is found, -1 on failure
 */
static int wpa_supplicant_parse_generic(const u8 *pos, const u8 *end,
                                        struct wpa_eapol_ie_parse *ie)
{
    if (pos[1] == 0)
        return 1;

    if (pos[1] >= 6 &&
            os_memcmp(pos + 2, WPA_OUI_TYPE, WPA_SELECTOR_LEN) == 0 &&
            pos[2 + WPA_SELECTOR_LEN] == 1 &&
            pos[2 + WPA_SELECTOR_LEN + 1] == 0)
    {
        ie->wpa_ie = pos;
        ie->wpa_ie_len = pos[1] + 2;
        return 0;
    }

    if (pos + 1 + RSN_SELECTOR_LEN < end &&
            pos[1] >= RSN_SELECTOR_LEN + PMKID_LEN &&
            os_memcmp(pos + 2, RSN_KEY_DATA_PMKID, RSN_SELECTOR_LEN) == 0)
    {
        ie->pmkid = pos + 2 + RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_GROUPKEY, RSN_SELECTOR_LEN) == 0)
    {
        ie->gtk = pos + 2 + RSN_SELECTOR_LEN;
        ie->gtk_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_MAC_ADDR, RSN_SELECTOR_LEN) == 0)
    {
        ie->mac_addr = pos + 2 + RSN_SELECTOR_LEN;
        ie->mac_addr_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

#ifdef CONFIG_PEERKEY
    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_SMK, RSN_SELECTOR_LEN) == 0)
    {
        ie->smk = pos + 2 + RSN_SELECTOR_LEN;
        ie->smk_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_NONCE, RSN_SELECTOR_LEN) == 0)
    {
        ie->nonce = pos + 2 + RSN_SELECTOR_LEN;
        ie->nonce_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_LIFETIME, RSN_SELECTOR_LEN) == 0)
    {
        ie->lifetime = pos + 2 + RSN_SELECTOR_LEN;
        ie->lifetime_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_ERROR, RSN_SELECTOR_LEN) == 0)
    {
        ie->error = pos + 2 + RSN_SELECTOR_LEN;
        ie->error_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }
#endif /* CONFIG_PEERKEY */

#ifdef CONFIG_IEEE80211W
    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_DHV, RSN_SELECTOR_LEN) == 0)
    {
        ie->dhv = pos + 2 + RSN_SELECTOR_LEN;
        ie->dhv_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }

    if (pos[1] > RSN_SELECTOR_LEN + 2 &&
            os_memcmp(pos + 2, RSN_KEY_DATA_IGTK, RSN_SELECTOR_LEN) == 0)
    {
        ie->igtk = pos + 2 + RSN_SELECTOR_LEN;
        ie->igtk_len = pos[1] - RSN_SELECTOR_LEN;
        return 0;
    }
#endif /* CONFIG_IEEE80211W */

    return 0;
}

/**
 * wpa_supplicant_parse_ies - Parse EAPOL-Key Key Data IEs
 * @buf: Pointer to the Key Data buffer
 * @len: Key Data Length
 * @ie: Pointer to parsed IE data
 * Returns: 0 on success, -1 on failure
 */
static int wpa_supplicant_parse_ies(const u8 *buf, size_t len,
                                    struct wpa_eapol_ie_parse *ie)
{
    const u8 *pos, *end;
    int ret = 0;

    os_memset(ie, 0, sizeof(*ie));


    //check ccmp
#if 0
    {
        unsigned short i, CipherCnt;

        if (((unsigned char *)buf)[0] == 0x30)
        {
            CipherCnt = ((unsigned short *)buf)[4];

            if (CipherCnt > 1)
            {

                for (i = 0; i < CipherCnt; i++)
                {
                    if (((unsigned char *)buf)[13 + i * 4] == 4)
                    {
                        //exist ccmp
                        break;
                    }
                }

                if (i >= CipherCnt)
                {
                    // not exist ccmp, select default
                    //memcpy((void*)bssp->rsn_ie, (void*)ie, 8);
                    ((unsigned short *)buf)[4] = 1;
                    memcpy((void *)(&(((unsigned char *)buf)[10])),(void *)(&(((unsigned char *)buf)[4])), 4);
                    memcpy((void *)(&(((unsigned char *)buf)[14])), (void *)(&(((unsigned char *)buf)[14 + 4 * (CipherCnt - 1)])), len - 8 - (CipherCnt - 1) * 4);
                    len = len - (CipherCnt - 1) * 4;
                }
                else
                {
                    //exist ccmp use it
                    //memcpy((void*)bssp->rsn_ie, (void*)ie, 8);
                    //memcpy((void *)(&(((unsigned char *)bssp->rsn_ie)[4])), (void *)(&(((unsigned char *)ie)[10 + i * 4])), 4);
                    ((unsigned short *)buf)[4] = 1;
                    memcpy((void *)(&(((unsigned char *)buf)[10])),(void *)(&(((unsigned char *)buf)[10 + i * 4])), 4);
                    memcpy((void *)(&(((unsigned char *)buf)[14])), (void *)(&(((unsigned char *)buf)[14 + 4 * (CipherCnt - 1)])), len - 8 - (CipherCnt - 1) * 4);
                    len = len - (CipherCnt - 1) * 4;
                }

                ((unsigned char *)buf)[1] -= (CipherCnt - 1) * 4;
            }
        }


    }
#endif


#if 0
    {
        uint32 i;

        rk_printf("\n\n\n");

        for (i = 0; i < len;)
        {
            rk_printf("id = %d\n",*(((uint8 *)buf)+ i));
            i++;
            len = *(((uint8 *)buf)+ i);
            rk_printf("len = %d\n",len);
            i++;
            len = i + len;

            for (; i < len; i++)
            {
                rk_printf("%02x ", *(((uint8 *)buf)+ i));
            }

            rk_printf("\n\n\n");
        }
    }
#endif




    for (pos = buf, end = pos + len; pos + 1 < end; pos += 2 + pos[1])
    {
        if (pos[0] == 0xdd && ((pos == buf + len - 1) || pos[1] == 0))
        {
            /* Ignore padding */
            break;
        }

        if (pos + 2 + pos[1] > end)
        {
            lwip_log("WPA: EAPOL-Key Key Data underflow (ie=%d len=%d pos=%d)\n",
                     pos[0], pos[1], (int) (pos - buf));
            wpa_hexdump_key(MSG_DEBUG, "WPA: Key Data",
                            buf, len);
            ret = -1;
            break;
        }

        if (*pos == RSN_INFO_ELEM)
        {
            ie->rsn_ie = pos;
            ie->rsn_ie_len = pos[1] + 2;
        }
        else if (*pos == GENERIC_INFO_ELEM)
        {
            ret = wpa_supplicant_parse_generic(pos, end, ie);
            if (ret < 0)
                break;

            if (ret > 0)
            {
                ret = 0;
                break;
            }
        }
        else
        {
            wpa_hexdump(MSG_DEBUG, "WPA: Unrecognized EAPOL-Key Key Data IE", pos, 2 + pos[1]);
        }
    }

    return ret;
}

static int wpa_supplicant_send_2_of_4(struct wpa_sm *sm,
                                      const unsigned char *dst,
                                      const struct wpa_eapol_key *key,
                                      int ver, const u8 *nonce,
                                      const u8 *wpa_ie, size_t wpa_ie_len,
                                      struct wpa_ptk *ptk)
{
    size_t rlen;
    struct wpa_eapol_key *reply;
    u8 *rbuf;

    if (wpa_ie == NULL)
    {
        lwip_log("WPA: No wpa_ie set - cannot generate msg 2/4\n");
        return -1;
    }

    wpa_hexdump(MSG_DEBUG, "WPA: WPA IE for msg 2/4", wpa_ie, wpa_ie_len);

    rbuf = wpa_alloc_eapol(IEEE802_1X_TYPE_EAPOL_KEY,
                           NULL, sizeof(*reply) + wpa_ie_len,
                           &rlen, (void *) &reply);
    if (rbuf == NULL)
        return -1;

    reply->type = sm->proto == WPA_PROTO_RSN ? EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;
    WPA_PUT_BE16(reply->key_info, ver | WPA_KEY_INFO_KEY_TYPE | WPA_KEY_INFO_MIC);
    if (sm->proto == WPA_PROTO_RSN)
        WPA_PUT_BE16(reply->key_length, 0);
    else
        os_memcpy(reply->key_length, key->key_length, 2);
    os_memcpy(reply->replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);

    WPA_PUT_BE16(reply->key_data_length, wpa_ie_len);
    os_memcpy(reply + 1, wpa_ie, wpa_ie_len);

    os_memcpy(reply->key_nonce, nonce, WPA_NONCE_LEN);

    wpa_eapol_key_send(sm, ptk->kck, ver, dst, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);
    //rk_printf("wpa_supplicant_process_2_of_4 exit\n");
    rk_printf("2/4x\n");
    return 0;
}

static void wpa_supplicant_process_1_of_4(struct wpa_sm *sm,
        const unsigned char *src_addr,
        const struct wpa_eapol_key *key,
        u16 ver)
{
    int i;
    struct wpa_eapol_ie_parse ie;
    struct wpa_ptk *ptk;
    u8 buf[8];

    lwip_log("WPA: RX message 1 of 4-Way Handshake from " MACSTR " (ver=%d)\n", MAC2STR(src_addr), ver);

    os_memset(&ie, 0, sizeof(ie));

    if (sm->proto == WPA_PROTO_RSN)
    {
        /* RSN: msg 1/4 should contain PMKID for the selected PMK */
        const u8 *_buf = (const u8 *) (key + 1);
        size_t len = WPA_GET_BE16(key->key_data_length);
        wpa_hexdump(MSG_DEBUG, "RSN: msg 1/4 key data", _buf, len);
        wpa_supplicant_parse_ies(_buf, len, &ie);
        if (ie.pmkid)
        {
            wpa_hexdump(MSG_DEBUG, "RSN: PMKID from Authenticator", ie.pmkid, PMKID_LEN);
        }
    }

    if (sm->renew_snonce)
    {
        if (hostapd_get_rand(sm->snonce, WPA_NONCE_LEN))
        {
            lwip_log("WPA: Failed to get random data for SNonce\n");
            return;
        }
        sm->renew_snonce = 0;
        wpa_hexdump(MSG_DEBUG, "WPA: Renewed SNonce", sm->snonce, WPA_NONCE_LEN);
    }

    /* Calculate PTK which will be stored as a temporary PTK until it has
     * been verified when processing message 3/4. */
    ptk = &sm->tptk;
    wpa_pmk_to_ptk(sm->pmk, sm->pmk_len, "Pairwise key expansion",
                   sm->own_addr, sm->bssid, sm->snonce, key->key_nonce,
                   (u8 *)ptk, sizeof(*ptk));

    /* Supplicant: swap tx/rx Mic keys */
    os_memcpy(buf, ptk->u.auth.tx_mic_key, 8);
    os_memcpy(ptk->u.auth.tx_mic_key, ptk->u.auth.rx_mic_key, 8);
    os_memcpy(ptk->u.auth.rx_mic_key, buf, 8);
    sm->tptk_set = 1;

    os_memcpy(sm->anonce, key->key_nonce, WPA_NONCE_LEN);
    if (wpa_supplicant_send_2_of_4(sm, sm->bssid, key, ver, sm->snonce,
                                   sm->assoc_wpa_ie, sm->assoc_wpa_ie_len, ptk))
    {
        return;
    }
}

static int wpa_supplicant_install_ptk(struct wpa_sm *sm, const struct wpa_eapol_key *key)
{
    int keylen, rsclen, i;
    wpa_alg alg;
    const u8 *key_rsc;
    u8 null_rsc[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    rk_printf("iptk\n");
    switch (sm->pairwise_cipher)
    {
        case WPA_CIPHER_CCMP:
            alg = WPA_ALG_CCMP;
            keylen = 16;
            rsclen = 6;
            break;
        case WPA_CIPHER_TKIP:
            alg = WPA_ALG_TKIP;
            keylen = 32;
            rsclen = 6;
            break;
        case WPA_CIPHER_NONE:
            lwip_log("WPA: Pairwise Cipher Suite: NONE - do not use pairwise keys\n");
            return 0;
        default:
            lwip_log("WPA: Unsupported pairwise cipher %d\n", sm->pairwise_cipher);
            return -1;
    }

    if (sm->proto == WPA_PROTO_RSN)
    {
        key_rsc = null_rsc;
    }
    else
    {
        key_rsc = key->key_rsc;
    }

    if (wpa_driver_wext_set_key_ext(sm, alg, sm->bssid, 0, 1, key_rsc, rsclen, (u8*)sm->ptk.tk1, keylen) < 0)
    {
        lwip_log("WPA: Failed to set PTK to the driver.\n");
        return -1;
    }
    return 0;
}

static int wpa_supplicant_check_group_cipher(int group_cipher,
        int keylen, int maxkeylen,
        int *key_rsc_len, wpa_alg *alg)
{
    int ret = 0;

    switch (group_cipher)
    {
        case WPA_CIPHER_CCMP:
            if (keylen != 16 || maxkeylen < 16)
            {
                ret = -1;
                break;
            }
            *key_rsc_len = 6;
            *alg = WPA_ALG_CCMP;
            break;
        case WPA_CIPHER_TKIP:
            if (keylen != 32 || maxkeylen < 32)
            {
                ret = -1;
                break;
            }
            *key_rsc_len = 6;
            *alg = WPA_ALG_TKIP;
            break;
        case WPA_CIPHER_WEP104:
            if (keylen != 13 || maxkeylen < 13)
            {
                ret = -1;
                break;
            }
            *key_rsc_len = 0;
            *alg = WPA_ALG_WEP;
            break;
        case WPA_CIPHER_WEP40:
            if (keylen != 5 || maxkeylen < 5)
            {
                ret = -1;
                break;
            }
            *key_rsc_len = 0;
            *alg = WPA_ALG_WEP;
            break;
        default:
            lwip_log("WPA: Unsupported Group Cipher %d\n",
                     group_cipher);
            return -1;
    }

    if (ret < 0 )
    {
        lwip_log("WPA: Unsupported %s Group Cipher key length %d (%d).\n",
                 wpa_cipher_txt(group_cipher), keylen, maxkeylen);
    }

    return ret;
}

struct wpa_gtk_data
{
    wpa_alg alg;
    int tx, key_rsc_len, keyidx;
    u8 gtk[32];
    int gtk_len;
};

static int wpa_supplicant_install_gtk(struct wpa_sm *sm, const struct wpa_gtk_data *gd, const u8 *key_rsc)
{
    const u8 *_gtk = gd->gtk;
    u8 gtk_buf[32];

    //rk_printf("wpa_supplicant_install_gtk %d %d\n", sm->group_cipher, sm->pairwise_cipher);
    rk_printf("igtk\n");
    if (sm->group_cipher == WPA_CIPHER_TKIP)
    {
        /* Swap Tx/Rx keys for Michael MIC */
        os_memcpy(gtk_buf, gd->gtk, 16);
        os_memcpy(gtk_buf + 16, gd->gtk + 24, 8);
        os_memcpy(gtk_buf + 24, gd->gtk + 16, 8);
        _gtk = gtk_buf;
    }
    if (sm->pairwise_cipher == WPA_CIPHER_NONE)
    {
        if (wpa_driver_wext_set_key_ext(sm, gd->alg,
                                        (u8 *) "\xff\xff\xff\xff\xff\xff",
                                        gd->keyidx, 1, key_rsc, gd->key_rsc_len,
                                        _gtk, gd->gtk_len) < 0)
        {
            lwip_log("WPA: Failed to set GTK to the driver (Group only).\n");
            return -1;
        }
    }
    else if (wpa_driver_wext_set_key_ext(sm, gd->alg, (u8 *)"\xff\xff\xff\xff\xff\xff",
                                         gd->keyidx, gd->tx, key_rsc, gd->key_rsc_len, _gtk, gd->gtk_len) < 0)
    {
        lwip_log("WPA: Failed to set GTK to the driver.\n");
        return -1;
    }
    //rk_printf("gtk end\n");
    return 0;
}

static int wpa_supplicant_gtk_tx_bit_workaround(const struct wpa_sm *sm,
        int tx)
{
    if (tx && sm->pairwise_cipher != WPA_CIPHER_NONE)
    {
        /* Ignore Tx bit for GTK if a pairwise key is used. One AP
         * seemed to set this bit (incorrectly, since Tx is only when
         * doing Group Key only APs) and without this workaround, the
         * data connection does not work because wpa_supplicant
         * configured non-zero keyidx to be used for unicast. */
        lwip_log("WPA: Tx bit set for GTK, but pairwise keys are used - ignore Tx bit\n");
        return 0;
    }
    return tx;
}

static int wpa_supplicant_pairwise_gtk(struct wpa_sm *sm,
                                       const struct wpa_eapol_key *key,
                                       const u8 *gtk, size_t gtk_len,
                                       int key_info)
{

    struct wpa_gtk_data gd;

    /*
     * IEEE Std 802.11i-2004 - 8.5.2 EAPOL-Key frames - Figure 43x
     * GTK KDE format:
     * KeyID[bits 0-1], Tx [bit 2], Reserved [bits 3-7]
     * Reserved [bits 0-7]
     * GTK
     */

    //rk_printf("wpa_supplicant_pairwise_gtk\n");
    rk_printf("gk\n");
    os_memset(&gd, 0, sizeof(gd));
    wpa_hexdump_key(MSG_DEBUG, "RSN: received GTK in pairwise handshake", gtk, gtk_len);

    if (gtk_len < 2 || gtk_len - 2 > sizeof(gd.gtk))
        return -1;

    gd.keyidx = gtk[0] & 0x3;
    gd.tx = wpa_supplicant_gtk_tx_bit_workaround(sm, !!(gtk[0] & BIT(2)));
    gtk += 2;
    gtk_len -= 2;

    os_memcpy(gd.gtk, gtk, gtk_len);
    gd.gtk_len = gtk_len;

    if (wpa_supplicant_check_group_cipher(sm->group_cipher, gtk_len, gtk_len, &gd.key_rsc_len, &gd.alg)
            || wpa_supplicant_install_gtk(sm, &gd, key->key_rsc))
    {
        rk_printf("RSN: Failed to install GTK\n");
        return -1;
    }

    return 0;

}

#if 0
static int ieee80211w_set_keys(struct wpa_sm *sm,
                               struct wpa_eapol_ie_parse *ie)
{
#ifdef CONFIG_IEEE80211W
    if (sm->mgmt_group_cipher != WPA_CIPHER_AES_128_CMAC)
        return 0;

    if (ie->igtk)
    {
        const struct wpa_igtk_kde *igtk;
        u16 keyidx;
        if (ie->igtk_len != sizeof(*igtk))
            return -1;
        igtk = (const struct wpa_igtk_kde *) ie->igtk;
        keyidx = WPA_GET_LE16(igtk->keyid);
        lwip_log("WPA: IGTK keyid %d pn %02x%02x%02x%02x%02x%02x\n",
                 keyidx, MAC2STR(igtk->pn));
        wpa_hexdump_key(MSG_DEBUG, "WPA: IGTK",
                        igtk->igtk, WPA_IGTK_LEN);
        if (keyidx > 4095)
        {
            lwip_log("WPA: Invalid IGTK KeyID %d\n",
                     keyidx);
            return -1;
        }
        if (wpa_sm_set_key(sm, WPA_ALG_IGTK,
                           (u8 *) "\xff\xff\xff\xff\xff\xff",
                           keyidx, 0, igtk->pn, sizeof(igtk->pn),
                           igtk->igtk, WPA_IGTK_LEN) < 0)
        {
            lwip_log("WPA: Failed to configure IGTK to the driver\n");
            return -1;
        }
    }

    if (ie->dhv)
    {
        const struct wpa_dhv_kde *dhv;
        if (ie->dhv_len != sizeof(*dhv))
            return -1;
        dhv = (const struct wpa_dhv_kde *) ie->dhv;
        wpa_hexdump_key(MSG_DEBUG, "WPA: DHV", dhv->dhv, WPA_DHV_LEN);
        if (wpa_sm_set_key(sm, WPA_ALG_DHV,
                           (u8 *) "\xff\xff\xff\xff\xff\xff", 0, 0,
                           NULL, 0, dhv->dhv, WPA_DHV_LEN) < 0)
        {
            lwip_log("WPA: Failed to configure DHV to the driver\n");
            return -1;
        }
    }

    return 0;
#else /* CONFIG_IEEE80211W */
    return 0;
#endif /* CONFIG_IEEE80211W */
}
#endif
static void wpa_report_ie_mismatch(struct wpa_sm *sm,
                                   const char *reason, const u8 *src_addr,
                                   const u8 *wpa_ie, size_t wpa_ie_len,
                                   const u8 *rsn_ie, size_t rsn_ie_len)
{
    lwip_log("WPA: %s (src=" MACSTR ")\n",
             reason, MAC2STR(src_addr));

    if (sm->ap_wpa_ie)
    {
        wpa_hexdump(MSG_INFO, "WPA: WPA IE in Beacon/ProbeResp",
                    sm->ap_wpa_ie, sm->ap_wpa_ie_len);
    }
    if (wpa_ie)
    {
        if (!sm->ap_wpa_ie)
        {
            lwip_log("WPA: No WPA IE in Beacon/ProbeResp\n");
        }
        wpa_hexdump(MSG_INFO, "WPA: WPA IE in 3/4 msg",
                    wpa_ie, wpa_ie_len);
    }

    if (sm->ap_rsn_ie)
    {
        wpa_hexdump(MSG_INFO, "WPA: RSN IE in Beacon/ProbeResp",
                    sm->ap_rsn_ie, sm->ap_rsn_ie_len);
    }
    if (rsn_ie)
    {
        if (!sm->ap_rsn_ie)
        {
            lwip_log("WPA: No RSN IE in Beacon/ProbeResp\n");
        }
        wpa_hexdump(MSG_INFO, "WPA: RSN IE in 3/4 msg",
                    rsn_ie, rsn_ie_len);
    }
}

static int wpa_supplicant_validate_ie(struct wpa_sm *sm,
                                      const unsigned char *src_addr,
                                      struct wpa_eapol_ie_parse *ie)
{
    struct wpa_ssid *ssid = sm->cur_ssid; //wp

    if (sm->ap_wpa_ie == NULL && sm->ap_rsn_ie == NULL)
    {
        lwip_log("WPA: No WPA/RSN IE for this AP known. Trying to get from scan results\n");
    }

    if (ie->wpa_ie == NULL && ie->rsn_ie == NULL && (sm->ap_wpa_ie || sm->ap_rsn_ie))
    {
        wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match "
                               "with IE in Beacon/ProbeResp (no IE?)",
                               src_addr, ie->wpa_ie, ie->wpa_ie_len,
                               ie->rsn_ie, ie->rsn_ie_len);
        return -1;
    }

    if ((ie->wpa_ie && sm->ap_wpa_ie &&
            (ie->wpa_ie_len != sm->ap_wpa_ie_len ||
             os_memcmp(ie->wpa_ie, sm->ap_wpa_ie, ie->wpa_ie_len) != 0)) ||
            (ie->rsn_ie && sm->ap_rsn_ie &&
             (ie->rsn_ie_len != sm->ap_rsn_ie_len ||
              os_memcmp(ie->rsn_ie, sm->ap_rsn_ie, ie->rsn_ie_len) != 0)))
    {
        wpa_report_ie_mismatch(sm, "IE in 3/4 msg does not match with IE in Beacon/ProbeResp",
                               src_addr, ie->wpa_ie, ie->wpa_ie_len,
                               ie->rsn_ie, ie->rsn_ie_len);

        return -1;
    }

    if (sm->proto == WPA_PROTO_WPA &&
            ie->rsn_ie && sm->ap_rsn_ie == NULL &&
            ssid && (ssid->proto & WPA_PROTO_RSN))
    {
        wpa_report_ie_mismatch(sm, "Possible downgrade attack "
                               "detected - RSN was enabled and RSN IE "
                               "was in msg 3/4, but not in "
                               "Beacon/ProbeResp",
                               src_addr, ie->wpa_ie, ie->wpa_ie_len,
                               ie->rsn_ie, ie->rsn_ie_len);
        return -1;
    }

    return 0;
}

static int wpa_supplicant_send_4_of_4(struct wpa_sm *sm,
                                      const unsigned char *dst,
                                      const struct wpa_eapol_key *key,
                                      u16 ver, u16 key_info,
                                      const u8 *kde, size_t kde_len,
                                      struct wpa_ptk *ptk)
{
    int i;
    size_t rlen;
    struct wpa_eapol_key *reply;
    u8 *rbuf;

    if (kde)
        wpa_hexdump(MSG_DEBUG, "WPA: KDE for msg 4/4", kde, kde_len);

    rbuf = wpa_alloc_eapol(IEEE802_1X_TYPE_EAPOL_KEY, NULL,
                           sizeof(*reply) + kde_len,
                           &rlen, (void *) &reply);

    if (rbuf == NULL)
        return -1;

    reply->type = (sm->proto == WPA_PROTO_RSN) ? EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;
    key_info &= WPA_KEY_INFO_SECURE;
    key_info |= ver | WPA_KEY_INFO_KEY_TYPE | WPA_KEY_INFO_MIC;
    WPA_PUT_BE16(reply->key_info, key_info);
    if (sm->proto == WPA_PROTO_RSN)
        WPA_PUT_BE16(reply->key_length, 0);
    else
        os_memcpy(reply->key_length, key->key_length, 2);
    os_memcpy(reply->replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);

    WPA_PUT_BE16(reply->key_data_length, kde_len);
    if (kde)
        os_memcpy(reply + 1, kde, kde_len);

    wpa_eapol_key_send(sm, ptk->kck, ver, dst, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);
    //rk_printf("wpa_supplicant_process_4_of_4 exit\n");
    rk_printf("4/4x\n");
    return 0;
}

static void wpa_supplicant_process_3_of_4(struct wpa_sm *sm,
        const struct wpa_eapol_key *key,  //size 95
        u16 ver)
{
    u16 key_info, keylen, len;
    const u8 *pos;
    struct wpa_eapol_ie_parse ie;

    lwip_log("WPA: RX message 3 of 4-Way Handshake from "
             MACSTR " (ver=%d)\n", MAC2STR(sm->bssid), ver);

    key_info = WPA_GET_BE16(key->key_info);

    pos = (const u8 *) (key + 1);
    len = WPA_GET_BE16(key->key_data_length);
    wpa_hexdump(MSG_DEBUG, "WPA: IE KeyData", pos, len);
    wpa_supplicant_parse_ies(pos, len, &ie);
    if (ie.gtk && !(key_info & WPA_KEY_INFO_ENCR_KEY_DATA))
    {
        lwip_log("WPA: GTK IE in unencrypted key data\n");
        return;
    }

    /*
    if (wpa_supplicant_validate_ie(sm, sm->bssid, &ie) < 0)
    {
     return;
    }
    */

    if (os_memcmp(sm->anonce, key->key_nonce, WPA_NONCE_LEN) != 0)
    {
        lwip_log("WPA: ANonce from message 1 of 4-Way Handshake differs from 3 of 4-Way Handshake - drop"
                 " packet (src=" MACSTR ")\n", MAC2STR(sm->bssid));

        return;
    }

    keylen = WPA_GET_BE16(key->key_length);
    switch (sm->pairwise_cipher)
    {
        case WPA_CIPHER_CCMP:
            if (keylen != 16)
            {
                lwip_log("WPA: Invalid CCMP key length %d (src=" MACSTR ")\n",
                         keylen, MAC2STR(sm->bssid));
                return;
            }
            break;

        case WPA_CIPHER_TKIP:
            if (keylen != 32)
            {
                lwip_log("WPA: Invalid TKIP key length %d (src=" MACSTR ")\n",
                         keylen, MAC2STR(sm->bssid));
                return;
            }
            break;
    }

    if (wpa_supplicant_send_4_of_4(sm, sm->bssid, key, ver, key_info, NULL, 0, &sm->ptk))
    {
        return;
    }
    /* SNonce was successfully used in msg 3/4, so mark it to be renewed
     * for the next 4-Way Handshake. If msg 3 is received again, the old
     * SNonce will still be used to avoid changing PTK. */
    sm->renew_snonce = 1;

    if (key_info & WPA_KEY_INFO_INSTALL)
    {
        wpa_supplicant_install_ptk(sm, key);
    }
    if (key_info & WPA_KEY_INFO_SECURE)
    {
        //	wpa_sm_mlme_setprotection(
        //		sm, sm->bssid, MLME_SETPROTECTION_PROTECT_TYPE_RX,
        //		MLME_SETPROTECTION_KEY_TYPE_PAIRWISE);
        //	eapol_sm_notify_portValid(sm->eapol, TRUE);
    }

    //	wpa_sm_set_state(sm, WPA_GROUP_HANDSHAKE);
    if (ie.gtk && wpa_supplicant_pairwise_gtk(sm, key, ie.gtk, ie.gtk_len, key_info) < 0)
    {
        lwip_log("RSN: Failed to configure GTK\n");
    }
#if 1
    //MSG_SET_WPA_SH();
    rk_printf("wpa sh is OK-----------------------------");
    WlcDev_SetAuthSuccess(NULL);
    //WPA SH OK
#else
    if (ieee80211w_set_keys(sm, &ie) < 0)
    {
        lwip_log("RSN: Failed to configure DHV/IGTK\n");
    }
    else
    {
        MSG_SET_WPA_SH();
    }
#endif
}



static int wpa_supplicant_process_1_of_2_rsn(struct wpa_sm *sm,
        const u8 *keydata, size_t keydatalen, u16 key_info, struct wpa_gtk_data *gd)
{
    int maxkeylen;
    struct wpa_eapol_ie_parse ie;

    wpa_supplicant_parse_ies(keydata, keydatalen, &ie);
    if (ie.gtk && !(key_info & WPA_KEY_INFO_ENCR_KEY_DATA))
    {
        lwip_log("WPA: GTK IE in unencrypted key data\n");
        return -1;
    }
    if (ie.gtk == NULL)
    {
        lwip_log("WPA: No GTK IE in Group Key msg 1/2\n");
        return -1;
    }
    maxkeylen = gd->gtk_len = ie.gtk_len - 2;

    if (wpa_supplicant_check_group_cipher(sm->group_cipher,
                                          gd->gtk_len, maxkeylen,
                                          &gd->key_rsc_len, &gd->alg))
        return -1;

    wpa_hexdump(MSG_DEBUG, "RSN: received GTK in group key handshake",ie.gtk, ie.gtk_len);
    gd->keyidx = ie.gtk[0] & 0x3;
    gd->tx = wpa_supplicant_gtk_tx_bit_workaround(sm, !!(ie.gtk[0] & BIT(2)));
    if (ie.gtk_len - 2 > sizeof(gd->gtk))
    {
        lwip_log("RSN: Too long GTK in GTK IE (len=%lu)\n", (unsigned long) ie.gtk_len - 2);
        return -1;
    }
    os_memcpy(gd->gtk, ie.gtk + 2, ie.gtk_len - 2);

#if 0
    if (ieee80211w_set_keys(sm, &ie) < 0)
        lwip_log("RSN: Failed to configure DHV/IGTK\n");
#endif

    return 0;
}

static int wpa_supplicant_process_1_of_2_wpa(struct wpa_sm *sm,
        const struct wpa_eapol_key *key,
        size_t keydatalen, int key_info,
        size_t extra_len, u16 ver,
        struct wpa_gtk_data *gd)
{
    size_t maxkeylen;
    u8 ek[32];

    gd->gtk_len = WPA_GET_BE16(key->key_length);
    maxkeylen = keydatalen;
    if (keydatalen > extra_len)
    {
        lwip_log("WPA: Truncated EAPOL-Key packet: key_data_length=%lu > extra_len=%lu\n",
                 (unsigned long) keydatalen,
                 (unsigned long) extra_len);
        return -1;
    }
    if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES)
    {
        if (maxkeylen < 8)
        {
            lwip_log("WPA: Too short maxkeylen (%lu)\n",
                     (unsigned long) maxkeylen);
            return -1;
        }
        maxkeylen -= 8;
    }

    if (wpa_supplicant_check_group_cipher(sm->group_cipher,
                                          gd->gtk_len, maxkeylen,
                                          &gd->key_rsc_len, &gd->alg))
        return -1;

    gd->keyidx = (key_info & WPA_KEY_INFO_KEY_INDEX_MASK) >> WPA_KEY_INFO_KEY_INDEX_SHIFT;

    if (ver == WPA_KEY_INFO_TYPE_HMAC_MD5_RC4)
    {
        os_memcpy(ek, key->key_iv, 16);
        os_memcpy(ek + 16, sm->ptk.kek, 16);
        if (keydatalen > sizeof(gd->gtk))
        {
            lwip_log("WPA: RC4 key data too long (%lu)\n",
                     (unsigned long) keydatalen);
            return -1;
        }
        os_memcpy(gd->gtk, key + 1, keydatalen);
        rc4_skip(ek, 32, 256, gd->gtk, keydatalen);
    }
    else if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES)
    {
        if (keydatalen % 8)
        {
            lwip_log("WPA: Unsupported AES-WRAP len %lu\n", (unsigned long) keydatalen);
            return -1;
        }
        if (maxkeylen > sizeof(gd->gtk))
        {
            lwip_log("WPA: AES-WRAP key data too long (keydatalen=%lu maxkeylen=%lu)\n",
                     (unsigned long) keydatalen,
                     (unsigned long) maxkeylen);
            return -1;
        }
        if (aes_unwrap(sm->ptk.kek, maxkeylen/8, (const u8 *)(key + 1), gd->gtk))
        {
            lwip_log("WPA: AES unwrap failed - could not decrypt GTK\n");
            return -1;
        }
    }
    gd->tx = wpa_supplicant_gtk_tx_bit_workaround(sm, !!(key_info & WPA_KEY_INFO_TXRX));

    return 0;
}

static int wpa_supplicant_send_2_of_2(struct wpa_sm *sm,
                                      const struct wpa_eapol_key *key,
                                      int ver, u16 key_info)
{
    size_t rlen;
    struct wpa_eapol_key *reply;
    u8 *rbuf;

    rk_printf("2/2\n");
    rbuf = wpa_alloc_eapol(IEEE802_1X_TYPE_EAPOL_KEY, NULL, sizeof(*reply), &rlen, (void *) &reply);
    if (rbuf == NULL)
        return -1;

    reply->type = sm->proto == WPA_PROTO_RSN ?
                  EAPOL_KEY_TYPE_RSN : EAPOL_KEY_TYPE_WPA;
    key_info &= WPA_KEY_INFO_KEY_INDEX_MASK;
    key_info |= ver | WPA_KEY_INFO_MIC | WPA_KEY_INFO_SECURE;
    WPA_PUT_BE16(reply->key_info, key_info);
    if (sm->proto == WPA_PROTO_RSN)
        WPA_PUT_BE16(reply->key_length, 0);
    else
        os_memcpy(reply->key_length, key->key_length, 2);
    os_memcpy(reply->replay_counter, key->replay_counter,
              WPA_REPLAY_COUNTER_LEN);

    WPA_PUT_BE16(reply->key_data_length, 0);

    lwip_log("WPA: Sending EAPOL-Key 2/2\n");
    wpa_eapol_key_send(sm, sm->ptk.kck, ver, sm->bssid, ETH_P_EAPOL, rbuf, rlen, reply->key_mic);

    return 0;
}

static void wpa_supplicant_process_1_of_2(struct wpa_sm *sm,
        const unsigned char *src_addr,
        const struct wpa_eapol_key *key,
        int extra_len, u16 ver)
{
    u16 key_info, keydatalen;
    int rekey, ret;
    struct wpa_gtk_data gd;

    os_memset(&gd, 0, sizeof(gd));

    key_info = WPA_GET_BE16(key->key_info);
    keydatalen = WPA_GET_BE16(key->key_data_length);

    if (sm->proto == WPA_PROTO_RSN)
    {
        ret = wpa_supplicant_process_1_of_2_rsn(sm, (const u8 *)(key + 1), keydatalen, key_info, &gd);
    }
    else
    {
        ret = wpa_supplicant_process_1_of_2_wpa(sm, key, keydatalen, key_info, extra_len, ver, &gd);
    }


    if (ret)
        return;

    if (wpa_supplicant_install_gtk(sm, &gd, key->key_rsc) ||
            wpa_supplicant_send_2_of_2(sm, key, ver, key_info))
        return;

    //MSG_SET_WPA_SH();
    //SH OK
    return;
}

static int wpa_supplicant_verify_eapol_key_mic(struct wpa_sm *sm,
        struct wpa_eapol_key *key,
        u16 ver,
        const u8 *buf, size_t len)
{
    u8 mic[16];
    int ok = 0;

    os_memcpy(mic, key->key_mic, 16);
    if (sm->tptk_set)
    {
        os_memset(key->key_mic, 0, 16);
        wpa_eapol_key_mic(sm->tptk.kck, ver, buf, len, key->key_mic);
        if (os_memcmp(mic, key->key_mic, 16) != 0)
        {
            lwip_log("WPA: Invalid EAPOL-Key MIC when using TPTK - ignoring TPTK\n");
        }
        else
        {
            ok = 1;
            sm->tptk_set = 0;
            sm->ptk_set = 1;
            os_memcpy(&sm->ptk, &sm->tptk, sizeof(sm->ptk));
        }
    }

    if (!ok && sm->ptk_set)
    {
        os_memset(key->key_mic, 0, 16);
        wpa_eapol_key_mic(sm->ptk.kck, ver, buf, len, key->key_mic);
        if (os_memcmp(mic, key->key_mic, 16) != 0)
        {
            lwip_log("WPA: Invalid EAPOL-Key MIC - dropping packet\n");
            return -1;
        }
        ok = 1;
    }

    if (!ok)
    {
        lwip_log("WPA: Could not verify EAPOL-Key MIC - dropping packet\n");
        return -1;
    }

    os_memcpy(sm->rx_replay_counter, key->replay_counter, WPA_REPLAY_COUNTER_LEN);
    sm->rx_replay_counter_set = 1;
    return 0;
}

/* Decrypt RSN EAPOL-Key key data (RC4 or AES-WRAP) */
static int wpa_supplicant_decrypt_key_data(struct wpa_sm *sm,
        struct wpa_eapol_key *key, u16 ver)
{
    u16 keydatalen = WPA_GET_BE16(key->key_data_length);

    wpa_hexdump(MSG_DEBUG, "RSN: encrypted key data",(u8 *) (key + 1), keydatalen);
    if (!sm->ptk_set) //wpa_supplicant_verify_eapol_key_mic ÖÐÉèÖÃ
    {
        lwip_log("WPA: PTK not available, cannot decrypt EAPOL-Key key data.\n");
        return -1;
    }

    /* Decrypt key data here so that this operation does not need
     * to be implemented separately for each message type. */
    if (ver == WPA_KEY_INFO_TYPE_HMAC_MD5_RC4)
    {
        u8 ek[32];
        os_memcpy(ek, key->key_iv, 16);
        os_memcpy(ek + 16, sm->ptk.kek, 16);
        rc4_skip(ek, 32, 256, (u8 *) (key + 1), keydatalen);
    }
    else if (ver == WPA_KEY_INFO_TYPE_HMAC_SHA1_AES)
    {
        u8 buf[512];
        if (keydatalen % 8)
        {
            lwip_log("WPA: Unsupported AES-WRAP len %d\n", keydatalen);
            return -1;
        }
        keydatalen -= 8; /* AES-WRAP adds 8 bytes */
        if (buf == NULL)
        {
            lwip_log("WPA: No memory for AES-UNWRAP buffer\n");
            return -1;
        }
        if (aes_unwrap(sm->ptk.kek, keydatalen / 8, (u8 *)(key + 1), buf))
        {
            lwip_log("WPA: AES unwrap failed - could not decrypt EAPOL-Key key data\n");
            return -1;
        }
        os_memcpy(key + 1, buf, keydatalen);
        WPA_PUT_BE16(key->key_data_length, keydatalen);
    }
    wpa_hexdump_key(MSG_DEBUG, "WPA: decrypted EAPOL-Key key data", (u8*)(key + 1), keydatalen);

    return 0;
}


/**
 * wpa_sm_rx_eapol - Process received WPA EAPOL frames
 * @sm: Pointer to WPA state machine data from wpa_sm_init()
 * @src_addr: Source MAC address of the EAPOL packet
 * @buf: Pointer to the beginning of the EAPOL data (EAPOL header)
 * @len: Length of the EAPOL frame
 * Returns: 1 = WPA EAPOL-Key processed, 0 = not a WPA EAPOL-Key, -1 failure
 *
 * This function is called for each received EAPOL frame. Other than EAPOL-Key
 * frames can be skipped if filtering is done elsewhere. wpa_sm_rx_eapol() is
 * only processing WPA and WPA2 EAPOL-Key frames.
 *
 * The received EAPOL-Key packets are validated and valid packets are replied
 * to. In addition, key material (PTK, GTK) is configured at the end of a
 * successful key handshake.
 */
int wpa_sm_rx_eapol(struct wpa_sm *sm, const u8 *src_addr, u8 *buf, size_t len)
{
    size_t plen, data_len, extra_len;
    struct ieee802_1x_hdr *hdr;
    struct wpa_eapol_key *key;
    u16 key_info, ver;
    int ret = -1;

    if (len < (sizeof(*hdr) + sizeof(*key)))
    {
        lwip_log("WPA: EAPOL frame too short to be a WPA EAPOL-Key (len %lu, expecting at least %lu)\n",
                 (unsigned long) len, (unsigned long) sizeof(*hdr) + sizeof(*key));
        return 0;
    }

    hdr = (struct ieee802_1x_hdr *)buf;
    key = (struct wpa_eapol_key *)(hdr + 1);
    plen = be_to_host16(hdr->length);
    data_len = plen + sizeof(*hdr);

    lwip_log("IEEE 802.1X RX: version=%d type=%d length=%lu\n", hdr->version, hdr->type, plen);
    if (hdr->version < EAPOL_VERSION)
    {
        /* TODO: backwards compatibility */
    }
    if (hdr->type != IEEE802_1X_TYPE_EAPOL_KEY)
    {
        lwip_log("WPA: EAPOL frame (type %u) discarded, not a Key frame\n", hdr->type);
        ret = 0;
        goto out;
    }

    if (data_len < len)
    {
        lwip_log("WPA: ignoring %lu bytes after the IEEE 802.1X data\n", (unsigned long) len - data_len);
    }

    /* dgl: len = struct ieee802_1x_hdr + struct wpa_eapol_key + key.data + null.data */
    if ((plen > len - sizeof(*hdr)) || (plen < sizeof(*key)))
    {
        lwip_log("WPA: EAPOL frame payload size %lu invalid (frame size %lu)\n", plen, len);
        ret = 0;
        goto out;
    }

    if ((key->type != EAPOL_KEY_TYPE_WPA) && (key->type != EAPOL_KEY_TYPE_RSN))
    {
        lwip_log("WPA: EAPOL-Key type (%d) unknown, discarded\n", key->type);
        ret = 0;
        goto out;
    }

    key_info = WPA_GET_BE16(key->key_info);
    ver = key_info & WPA_KEY_INFO_TYPE_MASK;
    if ((ver != WPA_KEY_INFO_TYPE_HMAC_MD5_RC4) && (ver != WPA_KEY_INFO_TYPE_HMAC_SHA1_AES))
    {
        lwip_log("WPA: Unsupported EAPOL-Key descriptor version %d.\n", ver);
        goto out;
    }

    if (sm->pairwise_cipher == WPA_CIPHER_CCMP && ver != WPA_KEY_INFO_TYPE_HMAC_SHA1_AES)
    {
        lwip_log("WPA: CCMP is used, but EAPOL-Key descriptor version (%d) is not 2.\n", ver);
        if (sm->group_cipher != WPA_CIPHER_CCMP && !(key_info & WPA_KEY_INFO_KEY_TYPE))
        {
            /* Earlier versions of IEEE 802.11i did not explicitly
             * require version 2 descriptor for all EAPOL-Key
             * packets, so allow group keys to use version 1 if
             * CCMP is not used for them. */
            lwip_log("WPA: Backwards compatibility: allow invalid version for non-CCMP group keys\n");
        }
        else
        {
            goto out;
        }
    }

    /*
    if (!peerkey && sm->rx_replay_counter_set &&
        os_memcmp(key->replay_counter, sm->rx_replay_counter, WPA_REPLAY_COUNTER_LEN) <= 0)
    {
    	rk_printf("WPA: EAPOL-Key Replay Counter did not increase - dropping packet\n");
    	goto out;
    }
    */
    //rk_printf("wpa now:%d old:%d\n", key->replay_counter, sm->rx_replay_counter);

    if (!(key_info & (WPA_KEY_INFO_ACK | WPA_KEY_INFO_SMK_MESSAGE)))
    {
        lwip_log("WPA: No Ack bit in key_info\n");
        goto out;
    }

    if (key_info & WPA_KEY_INFO_REQUEST)
    {
        lwip_log("WPA: EAPOL-Key with Request bit - dropped\n");
        goto out;
    }

    if ((key_info & WPA_KEY_INFO_MIC) && wpa_supplicant_verify_eapol_key_mic(sm, key, ver, buf, data_len))
    {
        goto out;
    }

    extra_len = data_len - sizeof(*hdr) - sizeof(*key);
    if (WPA_GET_BE16(key->key_data_length) > extra_len)
    {
        lwip_log("WPA: Invalid EAPOL-Key frame - key_data overflow (%d > %lu)\n",
                 WPA_GET_BE16(key->key_data_length), (unsigned long)extra_len);
        goto out;
    }
    extra_len = WPA_GET_BE16(key->key_data_length);

    if (sm->proto == WPA_PROTO_RSN && (key_info & WPA_KEY_INFO_ENCR_KEY_DATA))
    {
        if (wpa_supplicant_decrypt_key_data(sm, key, ver))
            goto out;
        extra_len = WPA_GET_BE16(key->key_data_length);
    }

    if (key_info & WPA_KEY_INFO_KEY_TYPE)
    {
        if (key_info & WPA_KEY_INFO_KEY_INDEX_MASK)
        {
            lwip_log("WPA: Ignored EAPOL-Key (Pairwise) with non-zero key index\n");
            goto out;
        }

        if (key_info & WPA_KEY_INFO_MIC)
        {
            /* 3/4 4-Way Handshake */
            //rk_printf("wpa_supplicant_process_3_of_4 exit\n");
            rk_printf("3/4x--------\n");
            wpa_supplicant_process_3_of_4(sm, key, ver);
        }
        else
        {
            /* 1/4 4-Way Handshake */
            //rk_printf("wpa_supplicant_process_1_of_4 exit\n");
            rk_printf("1/4x\n");
           // MSG_CLR_WPA_SH();
           //the sh ok
            wpa_supplicant_process_1_of_4(sm, src_addr, key, ver);
        }
    }
    else if (key_info & WPA_KEY_INFO_SMK_MESSAGE)
    {
    }
    else
    {
        if (key_info & WPA_KEY_INFO_MIC)
        {
            /* 1/2 Group Key Handshake */
            //rk_printf("wpa_supplicant_process_1_of_2\n");
            rk_printf("1/2\n");
            wpa_supplicant_process_1_of_2(sm, src_addr, key, extra_len, ver);
        }
        else
        {
            lwip_log("WPA: EAPOL-Key (Group) without Mic bit - dropped\n");
        }
    }
    ret = 1;
out:
    //rk_printf("wpa_sm_rx_eapol exit\n");
    return ret;
}

#endif /*WPA_AUTH*/

