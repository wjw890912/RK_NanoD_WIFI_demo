/*
********************************************************************************************
*
*                Copyright (c): 2014 - 2014 + 5, zhuzhe
*                             All rights reserved.
*
* FileName: Driver\Wlc\WlcDevice.c
* Owner: zhuzhe
* Date: 2014.7.10
* Time: 19:14:11
* Desc:
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    zhuzhe     2014.7.10     19:14:11   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_WLC_WLCDEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include "RKOS.h"
#include "Bsp.h"

#include "Deviceinclude.h"
#include "FwAnalysis.h"
#include "bcmevent.h"
#include "bcmendian.h"
#include "ethernet.h"

#include "Rk903Macro.h"
#include "WlcMacro.h"
#include "BCMEvent.h"
#include "Ieee80211.h"


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
char TestSsid[32] = "long";
char TestPassword[32] = "87654321";
//char TestSsid[32] = "dlink-nanoc";
//char TestPassword[32] = "1234567890123";
#define WPADATALEN   512
typedef  struct _WLC_DEVICE_CLASS
{
    DEVICE_CLASS stWlcDevice;

    HDC  pstRk903Dev;
    pSemaphore osWlcControlReqSem;
    pSemaphore osWlcControlScanSem;
    pSemaphore osWlcAuthSem;
    pSemaphore osWlcControlConnectSem;
    Bss_Descriptor_Info BssInfo;   // all bss
    Bss_Descriptor      ConnectBss;  //already connect bss
    char *               p80211Buff;
    uint16               p80211Len;
    wl_iw_extra_params_t WlcIwParams; //connect channel
    wlc_ssid_t           WlcSSID;
    Wpa_Sm               WpaSm;
}WLC_DEVICE_CLASS;

#define _DRIVER_WLC_WLCDEVICE_READ_  __attribute__((section("driver_wlc_wlcdevice_read")))
#define _DRIVER_WLC_WLCDEVICE_WRITE_ __attribute__((section("driver_wlc_wlcdevice_write")))
#define _DRIVER_WLC_WLCDEVICE_INIT_  __attribute__((section("driver_wlc_wlcdevice_init")))
#define _DRIVER_WLC_WLCDEVICE_SHELL_  __attribute__((section("driver_wlc_wlcdevice_shell")))

#define MAX_WPA_IE_LEN  	64
#define MAX_BSS         	50
#define MAX_PASSWORD_LEN	32//64

#define ENCODE_TYPE_WEP 1
#define ENCODE_TYPE_WPA  2
#define ENCODE_TYPE_WPA2 3
#define ENCODE_TYPE_WPA_WPA2 4
#define ENCODE_TYPE_NO   0

#define WEP_KEY_INDEX_1 1
#define WEP_KEY_INDEX_2 2
#define WEP_KEY_INDEX_3 3
#define WEP_KEY_INDEX_4 4

#define WL_JOIN_PARAMS_MAX 1600

#define htod32(i) i
#define htod16(i) i
#define dtoh32(i) i
#define dtoh16(i) i
#define htodchanspec(i) i
#define dtohchanspec(i) i

#define WPA_OUI         "\x00\x50\xF2"
#define WPA_OUI_LEN     3

//static const struct ether_addr ether_bcast = {{255, 255, 255, 255, 255, 255}};
//static const struct ether_addr ether_null = {{0, 0, 0, 0, 0, 0}};
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static WLC_DEVICE_CLASS * gpstWlcDevISR;

static  uint16 nqdBm_to_mW_map[QDBM_TABLE_LEN] =
{
    /* qdBm:    +0  +1  +2  +3  +4  +5  +6  +7 */
    /* 153: */      6683,   7079,   7499,   7943,   8414,   8913,   9441,   10000,
    /* 161: */      10593,  11220,  11885,  12589,  13335,  14125,  14962,  15849,
    /* 169: */      16788,  17783,  18836,  19953,  21135,  22387,  23714,  25119,
    /* 177: */      26607,  28184,  29854,  31623,  33497,  35481,  37584,  39811,
    /* 185: */      42170,  44668,  47315,  50119,  53088,  56234,  59566,  63096
};


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
rk_err_t WlcDevShellHelp(HDC dev, uint8 * pstr);
rk_err_t WlcDevSetIntBuf(HDC dev,char *name,char *buf, int len);
rk_err_t WlcDev_WpaAuthInit(HDC dev);
rk_err_t WlcDevShellAuth(HDC dev,uint8 *pstr);
int ieee80211_frequency_to_channel(int freq);
int WlcDevIeIsWpsIe(uint8 **wpsie, uint8 **tlvs, int *tlvs_len);
bcm_tlv_t * WlcDevBcmParseTlvs(void *buf, int buflen, uint key);
rk_err_t WlcDevHandleScanresults(Bss_Descriptor *bssp, char *end,struct iw_request_info *info, wl_bss_info_t *bi);
rk_err_t WlcCompareEtherAddr(const unsigned char *addr1, const unsigned char *addr2);
rk_err_t WlcDevIsSameNetwork(Bss_Descriptor *src, Bss_Descriptor *dst);
rk_err_t WlcDevSetBuf(HDC dev, int8 *iovar_name, void *param, int32 paramlen,void *buf, int32 buflen, void * buf_sync);
rk_err_t WlcDevJoinBss(HDC dev,Bss_Descriptor*  bss);
rk_err_t WlcDevBufVarGet(HDC dev,char *name,char *buf, int buflen);
rk_err_t WlcDevBufVarSet(HDC dev,char *name,char *buf, int len);
rk_err_t WlcDevChToChanspec(int ch, wl_join_params_t *join_params, int *join_params_size);
rk_err_t WlcDevMhzToCha(uint freq, uint start_factor);
rk_err_t WlcDevIntvarSet(HDC dev,char *name, int  retval);
rk_err_t WlcDevIntvarGet(HDC dev,char *name, int *retval);
void WlcSwapKeyFromBe(wl_wsec_key_t *key);
rk_err_t WlcDevSetEssid(HDC dev,struct iw_request_info *info, struct iw_point *dwrq, char *extra);
rk_err_t WlcDdvSetFreq(HDC dev,struct iw_request_info *info, struct iw_freq *fwrq, char *extra);

rk_err_t WlcDevIwSetMode(HDC dev,struct iw_request_info *info,uint32 *uwrq,char *extra);
rk_err_t WlcDevSetEncode(HDC dev,struct iw_request_info *info, struct iw_point *dwrq, char *extra);
rk_err_t WlcDevCreateWpaAuthWsec(HDC dev,wl_iw_t *iw);
rk_err_t WlcDevSetWpaAuth(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra);
rk_err_t WlcDevGetSSID(HDC dev,void* ssid, int index, UINT8 bwc);
rk_err_t WlcDevGetApRssi(Bss_Descriptor * Bss);
rk_err_t WlcDevGetEncodeType(Bss_Descriptor * Bss);
rk_err_t WlcDevWepConnect(HDC dev,char * password, int pwd_len);
rk_err_t WlcDevWpaConnect(HDC dev,char * password, int pwd_len);
rk_err_t WlcDev_BssInfoInit(HDC dev);
uint8    WlcDevBcmToQdbm(uint16 mw);
rk_err_t WlcDevWldevIovarSetbuf(HDC dev,int8 *iovar_name, void *param, int32 paramlen,
                                void *buf, int32 buflen, void * buf_sync);
rk_err_t WlcDev_WlScanPrep(HDC dev,struct wl_scan_params *params, void *request);
rk_err_t WlcDev_WlcNtvarGet(HDC dev,char *name,int * val);
rk_err_t WlcDev_WlcNtvarSet(HDC dev,char *name,int val);
extern rk_size_t bcm_mkiovar(char *name, char *data, uint datalen, char *buf, uint buflen);
rk_err_t WlcDevSdioPend(HDC dev);
rk_err_t WlcDevEscanSet(HDC dev);
rk_err_t WlcDev_GetTxpow(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra);
rk_err_t WlcDev_SetTxpow(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra);
rk_err_t WlcDev_BssInfoInit(HDC dev);
rk_err_t WlcDevShellScan(HDC dev, uint8 * pstr);
rk_err_t WlcDevDelete(HDC dev);
rk_err_t WlcDevDeInit(HDC dev);
rk_err_t WlcDevInit(HDC dev);
rk_err_t WlcDevResume(HDC dev);
rk_err_t WlCDevSuspend(HDC dev);

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WlcDev_SetAuthSuccess
** Input:HDC Dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.18
** Time: 9:44:03
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_SetAuthSuccess(HDC Dev)
{
    WLC_DEVICE_CLASS * pstWlcDev;
    if(Dev == NULL)
    {
        if(gpstWlcDevISR == NULL)
        {
            return RK_ERROR;
        }
        else
        {
            pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;

        }
   }
   else
   {
            pstWlcDev =  (WLC_DEVICE_CLASS *)Dev;
   }

   rkos_semaphore_give(pstWlcDev->osWlcAuthSem);
   return RK_SUCCESS;

}
/*******************************************************************************
** Name: WlcDev_80211DataInput
** Input:void *data, uint len
** Return: rk_err_t
** Owner:WlcSetIntBuf
** Date: 2014.8.13
** Time: 10:56:23
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_80211DataInput(void *data, uint len)
{
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;

    if (gpstWlcDevISR != NULL)
    {
        if (data)
        {
            memcpy(pstWlcDev->p80211Buff, data, len);
            pstWlcDev->p80211Len = len;
            rkos_semaphore_give(pstWlcDev->osWlcControlConnectSem);
        }
        return RK_SUCCESS;
    }
    return RK_ERROR;
}
/*******************************************************************************
** Name: WlcDev_SetEncodeExt
** Input:HDC dev,struct iw_request_info *info, struct iw_point *dwrq,char *extra
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.8
** Time: 14:40:08
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_SetEncodeExt(HDC dev,void* info, void* dwrq, char *extra)
{
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;
    wl_wsec_key_t key;
    struct iw_point * TempDwrq = (struct iw_point *)dwrq;
    int error;
    struct iw_encode_ext *iwe;
    char encodeext_ioctl_buf[WLC_IOCTL_MEDLEN];

    if (!extra)
        return -EINVAL;

    if (dev == NULL)
    {
        pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;
        if (gpstWlcDevISR == NULL)
            return RK_ERROR;
    }
    else
    {
        pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    }

    memset(&key, 0, sizeof(key));
    iwe = (struct iw_encode_ext *)extra;

    if (TempDwrq->flags & IW_ENCODE_DISABLED)
    {
        ;
    }
    key.index = 0;
    if (TempDwrq->flags & IW_ENCODE_INDEX)
    {
        key.index = (TempDwrq->flags & IW_ENCODE_INDEX) - 1;
    }

    key.len = iwe->key_len;


    if (!ETHER_ISMULTI(iwe->addr.sa_data))
        bcopy((void *)&iwe->addr.sa_data, (char *)&key.ea, ETHER_ADDR_LEN);

    //printf("key.len = %d\n", key.len); == 16
    if (key.len == 0)
    {
        if (iwe->ext_flags & IW_ENCODE_EXT_SET_TX_KEY)
        {
            rk_printf("Changing the the primary Key to %d\n", key.index);

            key.index = htod32(key.index);
            error = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_KEY_PRIMARY, &key.index, sizeof(key.index), 0);
            if (error)
                goto exit;
        }
        else
        {
            dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_KEY, &key, sizeof(key), 0);
        }
    }
    else
    {
        if (iwe->key_len > sizeof(key.data))
            return -EINVAL;

        rk_printf("Setting the key index %d\n", key.index);
        if (iwe->ext_flags & IW_ENCODE_EXT_SET_TX_KEY)
        {
            rk_printf("key is a Primary Key\n");
            key.flags = WL_PRIMARY_KEY;
        }

        bcopy((void *)iwe->key, key.data, iwe->key_len);

        if (iwe->alg == IW_ENCODE_ALG_TKIP)
        {
            uint8 keybuf[8];
            bcopy(&key.data[24], keybuf, sizeof(keybuf));
            bcopy(&key.data[16], &key.data[24], sizeof(keybuf));
            bcopy(keybuf, &key.data[16], sizeof(keybuf));
        }

        /*
                if (iwe->ext_flags & IW_ENCODE_EXT_RX_SEQ_VALID) {
        			uchar *ivptr;
        			ivptr = (uchar *)iwe->rx_seq;
        			key.rxiv.hi = (ivptr[5] << 24) | (ivptr[4] << 16) |
        				(ivptr[3] << 8) | ivptr[2];
        			key.rxiv.lo = (ivptr[1] << 8) | ivptr[0];
        			key.iv_initialized = TRUE;
        		}
        */

        switch (iwe->alg)
        {
            case IW_ENCODE_ALG_NONE:
                key.algo = CRYPTO_ALGO_OFF;
                break;
            case IW_ENCODE_ALG_WEP:
                if (iwe->key_len == WEP1_KEY_SIZE)
                    key.algo = CRYPTO_ALGO_WEP1;
                else
                    key.algo = CRYPTO_ALGO_WEP128;
                break;
            case IW_ENCODE_ALG_TKIP:
                key.algo = CRYPTO_ALGO_TKIP;
                break;
            case IW_ENCODE_ALG_CCMP:
                key.algo = CRYPTO_ALGO_AES_CCM;
                break;

            default:
                break;
        }
        error = WlcDevSetBuf(pstWlcDev, "wsec_key", &key, sizeof(key),
                                       encodeext_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);

        if (error)
            goto exit;
    }
exit:
    return error;
}

/*******************************************************************************
** Name: WlcDev_GetConnectBss
** Input:void *buf
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.7
** Time: 16:09:12
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_GetConnectBss(void * buf)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;
	uint32 * TempBuf = (uint32*)buf;

    if (pstWlcDev == NULL)
    {
        return RK_ERROR;

    }
    else
    {
        *TempBuf = (uint32)&(pstWlcDev->ConnectBss);
        return RK_SUCCESS;
    }
}
/*******************************************************************************
** Name: WlcDev_GetWpaSm
** Input:void * buf
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.7
** Time: 10:44:08
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_GetWpaSm(void * buf)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;

    if (pstWlcDev == NULL)
    {
        return RK_ERROR;

    }
    else
    {
        *((Wpa_Sm **)buf) = &(pstWlcDev->WpaSm);
        return RK_SUCCESS;
    }
}


/*******************************************************************************
** Name: WlcDev_Connect
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.5
** Time: 9:53:28
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_ConnectPro(HDC dev,char * ssid, char * password, int pwd_len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    uint timeleft = 10000;//dgl set 10s is one loop...
    int conn_try, ret;
    int wpa_try_num = 5;

    if (WlcDev_ConnectInit(dev,ssid, password, pwd_len))
    {
        return -1;
    }

    WlcDev_WpaAuthInit(dev);

    while (wpa_try_num--)
    {
        conn_try = 10;
        while (conn_try--)
        {
            ret = WlcDev_Connect(dev, password, pwd_len);
            rk_printf("the ret wlc is %d",ret);

            vTaskDelay(10);
            #if 0
            if (0 == ret)
            {
                break;
            }
            else
            {
                vTaskDelay(10);
            }
            #endif
        }

        if (ret)
        {
            return -1;
        }

        if ((pstWlcDev->ConnectBss.encode_type == ENCODE_TYPE_WEP) ||
                (pstWlcDev->ConnectBss.encode_type == ENCODE_TYPE_NO) )
        {
            return 0;
        }

        timeleft = 2000;

        //vTaskDelay(1000);
        while(1)
        {
            rkos_semaphore_take(pstWlcDev->osWlcControlConnectSem,timeleft);
            rk_wpa_auth_data_input(pstWlcDev->p80211Buff,pstWlcDev->p80211Len);
            if(rkos_semaphore_take(pstWlcDev->osWlcAuthSem,0) == pdPASS)
                return RK_SUCCESS;
            rk_printf("wpa connect timeout\n");
        }
        #if 0
        while (timeleft)
        {

#if 0
            if (SDC_GetSdioEvent(0) == SDC_SUCCESS)
            {
                process_sdio_pending_irqs();
            }
#endif
#if 0

            //rk_80211_data_handle();
            //will write

            //will get no link
            scb_val_t scbval;
            uint8  curbssid[ETH_ALEN];

            rk_printf("send WLC_DISASSOC cmd\n");
            RK903Dev_GetMac(curbssid);
            scbval.val = WLAN_REASON_DEAUTH_LEAVING;
            memcpy(&scbval.ea, curbssid, ETHER_ADDR_LEN);
            scbval.val = htod32(scbval.val);
            dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_DISASSOC, &scbval, sizeof(scb_val_t), 1);
            DelayMs(500);

            break;
#endif
            //link ok
#if 0
            if (!MSG_LINK_DOWN())
            {
                return 0;
            }
#endif

            DelayMs(1);
            if (--timeleft == 0)
            {
                rk_printf("wpa connect timeout\n");
            }
        }
        #endif
    }

}
/*******************************************************************************
** Name: RK903Dev_ScanWork
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.7
** Time: 17:41:13
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_ScanWork(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    int passive_scan = 0;
    rk_err_t ret;


    rk_printf("wifi start scan");
    if (pstWlcDev->BssInfo.pBss != NULL)
        WlcDev_BssInfoInit(dev);

    WlcDev_SetTxpow(dev,NULL,NULL,NULL);

    WlcDev_GetTxpow(dev,NULL,NULL,NULL);

    //will use 903 api
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev,WLC_SET_PASSIVE_SCAN, &passive_scan, sizeof(passive_scan), 0);

    if (!ret)
    {
        ret = WlcDevEscanSet( pstWlcDev);
    }

    rkos_semaphore_take(pstWlcDev->osWlcControlScanSem,MAX_DELAY);
    return RK_SUCCESS;

}


/*******************************************************************************
** Name: WlcDev_EsacanHandle
** Input:wl_event_msg_t *e, void *data
** Return: void
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 9:38:59
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API int32 WlcDev_EscanHandle(void *e, void *data)
{
    int32 ret, status;
    wl_bss_info_t *  bi;
    wl_escan_result_t *escan_result;
    uint32 bi_length;
    int i,j;
    Bss_Descriptor  *bssnew = NULL;
    uint8 isFound = 0;
    wl_event_msg_t * pTempMsg = (wl_event_msg_t *)e;

    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)gpstWlcDevISR;
    Bss_Descriptor * pTempBss;

    status = ntoh32(pTempMsg->status);

    if (status == WLC_E_STATUS_PARTIAL)
    {
        escan_result = (wl_escan_result_t *)data;

        if (!escan_result)
        {
            goto exit;
        }
        if (dtoh16(escan_result->bss_count) != 1)
        {
            goto exit;
        }
        bi = escan_result->bss_info;

        if (bi == NULL)
        {
            goto exit;
        }
        bi_length = dtoh32(bi->length);


#if 1
        if (bi_length != (dtoh32(escan_result->buflen) - WL_ESCAN_RESULTS_FIXED_SIZE))
        {
            rk_printf("the bi_lenth is %x,the escan is %x",bi_length,dtoh32(escan_result->buflen) - WL_ESCAN_RESULTS_FIXED_SIZE);
            goto exit;
        }
#endif

        if (dtoh16(bi->capability) & DOT11_CAP_IBSS) //dgl Ignoring ADHOC IBSS
        {
            goto exit;
        }
        if (!bi->SSID_len)
        {
            goto exit;//dgl no the hidden ssid....
        }
        else
        {
            rk_printf("BSSID="MACSTR", chan=%d, RSSI=%d, SSID=%-16.16s",
                      MAC2STR(bi->BSSID.octet), bi->ctl_ch, dtoh16(bi->RSSI), bi->SSID);

        }


        bssnew = rkos_memory_malloc(sizeof(Bss_Descriptor));

        if (bssnew ==NULL)
        {
            rk_printf("bssnew malloc fail");
        }
		 else
		 {
			 memset(bssnew,0,sizeof(Bss_Descriptor));
		 }

        /* AP mac addr*/
        memcpy((void*)bssnew->bssid, (void*)&bi->BSSID, ETHER_ADDR_LEN);

        /*scan AP SSID Name..*/
        bssnew->ssid_len = dtoh32(bi->SSID_len);
        rk_printf("the bi SSID is %d",bi->SSID_len);
        memcpy((void*)bssnew->ssid, (void*)&bi->SSID, dtoh32(bi->SSID_len));

        bssnew->rssi = -bi->RSSI;
        bssnew->channel = bi->ctl_ch ? bi->ctl_ch : CHSPEC_CHANNEL(bi->chanspec);

        if (dtoh16(bi->capability) & (DOT11_CAP_ESS | DOT11_CAP_IBSS))
        {
            if (dtoh16(bi->capability) & DOT11_CAP_ESS)
            {
                bssnew->mode = IW_MODE_INFRA;
            }
            else
            {
                bssnew->mode = IW_MODE_ADHOC;
            }
        }

        bssnew->encode_type = ENCODE_TYPE_NO;

        if (dtoh16(bi->capability) & DOT11_CAP_PRIVACY)
        {
            bssnew->encode_type = ENCODE_TYPE_WEP; //只要启动wep wpa wpa2 中任一种都会进来
            bssnew->encode_flag = IW_ENCODE_ENABLED | IW_ENCODE_NOKEY;
        }
        else
        {
            bssnew->encode_flag = IW_ENCODE_DISABLED;
            rk_printf("Encode is Disable");
            goto exit;   //不支持其它格式
        }


        if (bi->ie_length > 0x1000)
            rk_printf("the bi_lenth is %x",bi->ie_length);

        ret = WlcDevHandleScanresults(bssnew, NULL, NULL, bi);

        if (ret < 0)
        {
            goto exit;
        }

        /* filter the repeated bssid. */
        isFound = 0;
        if (pstWlcDev->BssInfo.pBss == NULL)
        {
            bssnew->Next = NULL;
            pstWlcDev->BssInfo.pBss = bssnew;
            pstWlcDev->BssInfo.total_num++;
            return BCME_OK ;

        }
        else
        {
            pTempBss = pstWlcDev->BssInfo.pBss;
            for (j=0; j< (pstWlcDev->BssInfo.total_num); j++)
            {

                if (WlcDevIsSameNetwork(bssnew,pTempBss))
                {
                    if (pTempBss->rssi > bssnew->rssi)
                    {
                        pTempBss->rssi = bssnew->rssi;
                    }
                    isFound = 1;
                    break;
                }
                if (pTempBss->Next != NULL)
                    pTempBss = pTempBss->Next;
            }

            if ((!isFound) && ((pstWlcDev->BssInfo.total_num + 1) < MAX_BSS))
            {
                pTempBss->Next = bssnew;
                bssnew->Next = NULL;

                rk_printf("SSID+++ the num is %x",pstWlcDev->BssInfo.total_num);


                pstWlcDev->BssInfo.total_num++;
                return BCME_OK ;

#if 0  //信号强度排序，先忽略
                for (i = bss_descriptor_info.total_num; i > 0; i--)
                {
                    if (bss_descriptor_info.bss[i].rssi < bss_descriptor_info.bss[i-1].rssi)
                    {
                        memcpy(bssnew, &(bss_descriptor_info.bss[i]), sizeof(struct bss_descriptor));
                        memcpy(&(bss_descriptor_info.bss[i]), &(bss_descriptor_info.bss[i-1]), sizeof(struct bss_descriptor));
                        memcpy(&(bss_descriptor_info.bss[i-1]), bssnew, sizeof(struct bss_descriptor));
                    }
                }
#endif

            }
        }
    }
    else if ((status == WLC_E_STATUS_SUCCESS) || (status == WLC_E_STATUS_ABORT))
    {

        rk_printf("SSID Scan Success");
        rkos_semaphore_give(pstWlcDev->osWlcControlScanSem);
    }
    else
    {
        rk_printf("SSID Scan Other");
        rkos_semaphore_give(pstWlcDev->osWlcControlScanSem);
    }


exit:
    rkos_memory_free(bssnew);

    return BCME_OK;
}
/*******************************************************************************
** Name: WlcDev_SSIDCheck
** Input:HDC dev
** Return: rk_errr_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:52:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_SSIDCheck(HDC dev)
{

}
/*******************************************************************************
** Name: WlcDev_Connect
** Input:dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:37:08
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_Connect(HDC dev,char * password, int pwd_len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    Bss_Descriptor * TempBss =  &(pstWlcDev->ConnectBss);

    rk_printf("the encode_typeis %d ",TempBss->encode_type);
    if ( TempBss->encode_type == ENCODE_TYPE_WEP || TempBss->encode_type == ENCODE_TYPE_NO)
    {
        return WlcDevWepConnect(dev, password, pwd_len);
    }
    else
    {
        return WlcDevWpaConnect(dev, password, pwd_len);
    }
}
/*******************************************************************************
** Name: WlcDev_ConnectInit
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:18:06
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON API rk_err_t WlcDev_ConnectInit(HDC dev,char * ssid, char * password, int pwd_len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    Bss_Descriptor *TempBss;
    int i;

    TempBss = pstWlcDev->BssInfo.pBss;

    for (i=0; i< pstWlcDev->BssInfo.total_num;i++)
    {
        if (! memcmp(ssid,TempBss->ssid,TempBss->ssid_len) )
        {
            break;
        }
        TempBss = TempBss->Next;
    }

    if (i == pstWlcDev->BssInfo.total_num)
    {
        return -1;
    }

    memcpy(&(pstWlcDev->ConnectBss), TempBss, sizeof(Bss_Descriptor));

    //pstWlcDev->ConnectBss.password = rkos_memory_malloc(MAX_PASSWORD_LEN);

    memcpy(pstWlcDev->ConnectBss.password, password,pwd_len);
    //*((pstWlcDev->ConnectBss.password)+= pwd_len) = 0x00;
    pstWlcDev->ConnectBss.password[pwd_len] = 0x00;

    rk_printf("the ssid is %s,password is %s",pstWlcDev->ConnectBss.ssid,pstWlcDev->ConnectBss.password);
    return 0;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WlcDevSetIntBuf
** Input:HDC dev,char *name,int val
** Return: rk_err_t
** Owner:WlcSetIntBuf
** Date: 2014.8.13
** Time: 9:31:21
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSetIntBuf(HDC dev,char *name,char *buf, int len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    char ioctlbuf[MAX_WLIW_IOCTL_LEN];
    uint buflen;

    buflen = bcm_mkiovar(name, buf, len, ioctlbuf, sizeof(ioctlbuf));

    return (dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_VAR, ioctlbuf, buflen, 0));

}
/*******************************************************************************
** Name: WlcDev_WpaAuthInit
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.5
** Time: 14:52:07
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_WpaAuthInit(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    memset((void *)&pstWlcDev->WpaSm, 0, sizeof(Wpa_Sm));

    pstWlcDev->WpaSm.renew_snonce = 1;
    pstWlcDev->WpaSm.ap_rsn_ie = (uint8 *)pstWlcDev->ConnectBss.rsn_ie;
    pstWlcDev->WpaSm.ap_rsn_ie_len = pstWlcDev->ConnectBss.rsn_ie_len;
    pstWlcDev->WpaSm.ap_wpa_ie = (uint8 *)pstWlcDev->ConnectBss.wpa_ie;
    pstWlcDev->WpaSm.ap_wpa_ie_len = pstWlcDev->ConnectBss.wpa_ie_len;

    if (pstWlcDev->ConnectBss.rsn_ie_len)
    {
        pstWlcDev->WpaSm.assoc_wpa_ie = (uint8 *)pstWlcDev->ConnectBss.rsn_ie;
        pstWlcDev->WpaSm.assoc_wpa_ie_len = pstWlcDev->ConnectBss.rsn_ie_len;
    }
    else if (pstWlcDev->ConnectBss.wpa_ie_len)
    {
        pstWlcDev->WpaSm.assoc_wpa_ie = (uint8 *)pstWlcDev->ConnectBss.wpa_ie;
        pstWlcDev->WpaSm.assoc_wpa_ie_len = pstWlcDev->ConnectBss.wpa_ie_len;
    }

    RK903Dev_GetMac(pstWlcDev->WpaSm.own_addr);
    memcpy(pstWlcDev->WpaSm.bssid , (const char*)pstWlcDev->ConnectBss.bssid, ETH_ALEN);
    pstWlcDev->WpaSm.key_mgmt = WPA_KEY_MGMT_PSK;

    pstWlcDev->WpaSm.pairwise_cipher = pstWlcDev->ConnectBss.pcipher_type << 1;
    pstWlcDev->WpaSm.group_cipher = pstWlcDev->ConnectBss.gcipher_type << 1;

    if (pstWlcDev->ConnectBss.encode_type == ENCODE_TYPE_WPA)
    {
        pstWlcDev->WpaSm.proto = WPA_PROTO_WPA;
    }
    else
    {
        pstWlcDev->WpaSm.proto = WPA_PROTO_RSN;
    }

    pstWlcDev->WpaSm.pmk_len = 32;

    pbkdf2_sha1((const char *)pstWlcDev->ConnectBss.password, (const char *)pstWlcDev->ConnectBss.ssid,
                pstWlcDev->ConnectBss.ssid_len, 4096, pstWlcDev->WpaSm.pmk, 32);
}
/*******************************************************************************
** Name: ieee80211_frequency_to_channel
** Input:int freq
** Return: int
** Owner:zhuzhe
** Date: 2014.7.30
** Time: 9:39:25
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN int ieee80211_frequency_to_channel(int freq)
{

    /* see 802.11 17.3.8.3.2 and Annex J */
    if (freq == 2484)
        return 14;
    else if (freq < 2484)
        return (freq - 2407) / 5;
    else if (freq >= 4910 && freq <= 4980)
        return (freq - 4000) / 5;
    else
        return (freq - 5000) / 5;
}
/*******************************************************************************
** Name: WlcDevIeIsWpsIe
** Input:uint8 **wpsie, uint8 **tlvs, int *tlvs_len\
** Return: int
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 14:13:27
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN int WlcDevIeIsWpaIe(uint8 **wpsie, uint8 **tlvs, int *tlvs_len)
{
    uint8 *ie = *wpsie;

    if ((ie[1] >= 4) && !bcmp((const void *)&ie[2], (const void *)(WPA_OUI "\x04"), 4))
    {
        return TRUE;
    }

    ie += ie[1] + 2;

    *tlvs_len -= (int)(ie - *tlvs);

    *tlvs = ie;
    return FALSE;
}
/*******************************************************************************
** Name: WlcDevIeIsWpsIe
** Input:uint8 **wpsie, uint8 **tlvs, int *tlvs_len\
** Return: int
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 14:13:27
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN int WlcDevIeIsWpsIe(uint8 **wpsie, uint8 **tlvs, int *tlvs_len)
{
    uint8 *ie = *wpsie;

    if ((ie[1] >= 4) &&
            !bcmp((const void *)&ie[2], (const void *)(WPA_OUI "\x04"), 4))
    {
        return TRUE;
    }

    ie += ie[1] + 2;

    *tlvs_len -= (int)(ie - *tlvs);

    *tlvs = ie;
    return FALSE;
}
/*******************************************************************************
** Name: WlcDevBcmParseTlvs
** Input:void *buf, int buflen, uint key
** Return: bcm_tlv_t *
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 14:09:44
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN bcm_tlv_t * WlcDevBcmParseTlvs(void *buf, int buflen, uint key)
{
    bcm_tlv_t *elt;
    int totlen;

    elt = (bcm_tlv_t*)buf;
    totlen = buflen;

    /* find tagged parameter */
    while (totlen >= 2)
    {
        int len = elt->len;

        /* validate remaining totlen */
        if ((elt->id == key) && (totlen >= (len + 2)))
            return (elt);

        /* move to next tlv. */
        elt = (bcm_tlv_t*)((uint8*)elt + (len + 2));
        totlen -= (len + 2);
    }

    return NULL;
}
/*******************************************************************************
** Name: WlcDevHandleScanresults
** Input:Bss_Descriptor *bssp, char *end,struct iw_request_info *info, wl_bss_info_t *bi
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 11:51:37
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevHandleScanresults(Bss_Descriptor *bssp, char *end,struct iw_request_info *info, wl_bss_info_t *bi)
{

    struct iw_event	iwe;
    int ret = -1;
    unsigned short i, CipherCnt;

    if (bi->ie_length)
    {
        bcm_tlv_t *ie;    //the ie data is address
        uint8 *ptr = ((uint8 *)bi) + sizeof(wl_bss_info_t);
        int ptr_len = bi->ie_length;

        /* WPA2*/
        /* tlv: id + len + data*/
        //id(1)+len(1)+num(2)+00-0F-AC-XX(g)+num(2)+00-0F-AC-XX(p)
        ie = WlcDevBcmParseTlvs(ptr, ptr_len, DOT11_MNG_RSN_ID);

        if (ie)
        {
            //TKIP OR CMMP
            rk_printf("ie1 is %x",ie->len+2);
            if( ie->len + 2 > MAX_WPA_IE_LEN)
                return -1;
            memcpy((void*)bssp->rsn_ie, (void*)ie, ie->len + 2);
            bssp->rsn_ie_len = ie->len + 2;
            bssp->encode_type = ENCODE_TYPE_WPA2;

            /* group cipher suite. */
            if (((char*)ie)[7] == 2) //TKIP
            {
                bssp->gcipher_type = IW_AUTH_CIPHER_TKIP;

            }
            else if (((char*)ie)[7] == 4) //CCMP
            {
                bssp->gcipher_type = IW_AUTH_CIPHER_CCMP;
            }
            else
            {
                bssp->gcipher_type = IW_AUTH_CIPHER_CCMP;
            }

            /* pairwise cipher suite. */
            CipherCnt = ((unsigned short *)ie)[4];
            if (CipherCnt == 1)
            {
                if (((char*)ie)[13] == 2) //TKIP
                {
                    bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                }
                else if (((char*)ie)[13] == 4) //CCMP
                {
                    bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                }
                else
                {
                    bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                }
            }

  #if 0
            else if (CipherCnt > 1)
            {
                for (i = 0; i < CipherCnt; i++)
                {
                    if (((unsigned char *)ie)[13 + i * 4] == 4) //CCMP
                    {
                        //exist ccmp
                        break;
                    }
                }

                if (i >= CipherCnt)
                {
                    if (((char*)ie)[7] == 2) //TKIP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                    }
                    else if (((char*)ie)[7] == 4) //CCMP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }
                    else
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }

#if 1
                    ((unsigned short *)bssp->rsn_ie)[4] = 1;
                    rk_printf("ie2 is %x",bssp->rsn_ie_len - 8 - (CipherCnt - 1) * 4);
                    memcpy((void *)(&(((unsigned char *)bssp->rsn_ie)[10])),(void *)(&(((unsigned char *)bssp->rsn_ie)[4])), 4);
                    memcpy((void *)(&(((unsigned char *)bssp->rsn_ie)[14])), (void *)(&(((unsigned char *)ie)[14 + 4 * (CipherCnt - 1)])), bssp->rsn_ie_len - 8 - (CipherCnt - 1) * 4);

                    (((unsigned char *)bssp->rsn_ie)[1]) -=  ((CipherCnt - 1) * 4);
                    bssp->rsn_ie_len -=  ((CipherCnt - 1) * 4);

#endif
                }
                else
                {
                    if (((char*)ie)[13 + i * 4] == 2) //TKIP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                    }
                    else if (((char*)ie)[13 + i * 4] == 4) //CCMP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }
                    else
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }

#if 1
                    ((unsigned short *)bssp->rsn_ie)[4] = 1;
                    rk_printf("ie3 is %x", bssp->rsn_ie_len - 8 - (CipherCnt - 1) * 4);
                    memcpy((void *)(&(((unsigned char *)bssp->rsn_ie)[10])),(void *)(&(((unsigned char *)ie)[10 + i * 4])), 4);
                    memcpy((void *)(&(((unsigned char *)bssp->rsn_ie)[14])), (void *)(&(((unsigned char *)ie)[14 + 4 * (CipherCnt - 1)])), bssp->rsn_ie_len - 8 - (CipherCnt - 1) * 4);

                    (((unsigned char *)bssp->rsn_ie)[1]) -=  ((CipherCnt - 1) * 4);
                    bssp->rsn_ie_len -= ((CipherCnt - 1) * 4);

#endif
                }


            }
#endif

            ret = 0;
        }

#if 0
        /* WPS*/
        ptr = ((uint8 *)bi) + sizeof(wl_bss_info_t);
        while ((ie = WlcDevBcmParseTlvs(ptr, ptr_len, DOT11_MNG_WPA_ID)))
        {
            if (WlcDevIeIsWpsIe(((uint8 **)&ie), &ptr, &ptr_len))
            {
                /*
                memcpy(bssp->wpa_ie, ie,ie->len + 2);
                bssp->wpa_ie_len = ie->len + 2;
                if(ENCODE_TYPE_WPA2 ==bssp->encode_type) //不接收wps认证方式
                {
                    bssp->encode_type = ENCODE_TYPE_WPA_WPA2;
                }
                else
                {
                    bssp->encode_type = ENCODE_TYPE_WPA;
                }
                */
                break;
            }
        }
#endif

        /* WPA */

        ptr = ((uint8 *)bi) + sizeof(wl_bss_info_t);
        ptr_len = bi->ie_length;
        while ((ie = WlcDevBcmParseTlvs(ptr, ptr_len, DOT11_MNG_WPA_ID)))
        {

            if (WlcDevIeIsWpaIe(((uint8 **)&ie), &ptr, &ptr_len))
            {
                if( ie->len + 2 > MAX_WPA_IE_LEN)
                    return -1;
                memcpy((void*)bssp->wpa_ie, (void*)ie, ie->len + 2);
                bssp->wpa_ie_len = ie->len + 2;

                if (ENCODE_TYPE_WPA2 == bssp->encode_type)
                {
                    break;
                }
                else
                {
                    bssp->encode_type = ENCODE_TYPE_WPA;
					return -1;


                }
#if 0
                if (((char*)ie)[11] == 2) //TKIP
                {
                    bssp->gcipher_type = IW_AUTH_CIPHER_TKIP;
                }
                else if (((char*)ie)[11] == 4) //CCMP
                {
                    bssp->gcipher_type = IW_AUTH_CIPHER_CCMP;
                }
                else
                {
                    bssp->gcipher_type = IW_AUTH_CIPHER_CCMP;
                }

                CipherCnt = ((unsigned short *)ie)[6];
                rk_printf("the CipherCnt is%d",CipherCnt);
                if (CipherCnt == 1)
                {
                    if (((char*)ie)[17] == 2) //TKIP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                    }
                    else if (((char*)ie)[17] == 4) //CCMP
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }
                    else
                    {
                        bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                    }
                }
                else if (CipherCnt > 1)
                {
                    for (i = 0; i < CipherCnt; i++)
                    {
                        if (((unsigned char *)ie)[17 + i * 4] == 4)
                        {
                            //exist ccmp
                            break;
                        }
                    }

                    if (i >= CipherCnt)
                    {
                        if (((char*)ie)[11] == 2) //TKIP
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                        }
                        else if (((char*)ie)[11] == 4) //CCMP
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                        }
                        else
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                        }

#if 1
                        ((unsigned short *)bssp->wpa_ie)[6] = 1;
                        memcpy((void *)(&(((unsigned char *)bssp->wpa_ie)[14])),(void *)(&(((unsigned char *)bssp->wpa_ie)[8])), 4);
                        memcpy((void *)(&(((unsigned char *)bssp->wpa_ie)[18])), (void *)(&(((unsigned char *)ie)[18 + 4 * (CipherCnt - 1)])), bssp->wpa_ie_len - 8 - (CipherCnt - 1) * 4);

                        (((unsigned char *)bssp->wpa_ie)[1]) -=  ((CipherCnt - 1) * 4);
                        bssp->wpa_ie_len -=  ((CipherCnt - 1) * 4);

#endif
                    }
                    else
                    {
                        if (((char*)ie)[17 + i * 4] == 2) //TKIP
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_TKIP;
                        }
                        else if (((char*)ie)[17 + i * 4] == 4) //CCMP
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                        }
                        else
                        {
                            bssp->pcipher_type = IW_AUTH_CIPHER_CCMP;
                        }

#if 1
                        ((unsigned short *)bssp->wpa_ie)[6] = 1;
                        rk_printf("ie4 is %x", bssp->wpa_ie_len - 8 - (CipherCnt - 1) * 4);
                        memcpy((void *)(&(((unsigned char *)bssp->wpa_ie)[14])),(void *)(&(((unsigned char *)ie)[14 + i * 4])), 4);
                        memcpy((void *)(&(((unsigned char *)bssp->wpa_ie)[18])), (void *)(&(((unsigned char *)ie)[18 + 4 * (CipherCnt - 1)])), bssp->wpa_ie_len - 8 - (CipherCnt - 1) * 4);

                        (((unsigned char *)bssp->wpa_ie)[1]) -=  ((CipherCnt - 1) * 4);
                        bssp->wpa_ie_len -=  ((CipherCnt - 1) * 4);

#endif
#endif
                    }

                 ret = 0;
                break;
                }

            }


    if ((ENCODE_TYPE_WEP == bssp->encode_type) || (ENCODE_TYPE_NO == bssp->encode_type))
    {
        ret = 0;
    }

    return ret;
}
/*******************************************************************************
** Name: WlcCompareEtherAddr
** Input:const unsigned char *addr1, const unsigned char *addr2
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 10:15:31
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcCompareEtherAddr(const unsigned char *addr1, const unsigned char *addr2)
{
    const unsigned char *a = (const unsigned char *) addr1;
    const unsigned char *b = (const unsigned char *) addr2;

    if (ETH_ALEN != 6)
        return -1;

    return (((a[0] ^ b[0]) | (a[1] ^ b[1]) | (a[2] ^ b[2]) | (a[3] ^ b[3]) | (a[4] ^ b[4]) | (a[5] ^ b[5])) != 0);
}
/*******************************************************************************
** Name: WlcDevIsSameNetwork
** Input:BssDescriptor *src, BssDescriptor *dst
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.21
** Time: 10:12:46
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevIsSameNetwork(Bss_Descriptor *src, Bss_Descriptor *dst)
{
    return ((src->ssid_len == dst->ssid_len) &&
            (src->channel == dst->channel) &&
            !WlcCompareEtherAddr(src->bssid, dst->bssid) &&
            !memcmp(src->ssid, dst->ssid, src->ssid_len));
}
/*******************************************************************************
** Name: WlcDevSetBuf
** Input:HDC dev, int8 *iovar_name, void *param, int32 paramlen,
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.18
** Time: 16:24:30
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSetBuf(HDC dev, int8 *iovar_name, void *param, int32 paramlen,void *buf, int32 buflen, void * buf_sync )
{
    int32 ret = 0;
    int32 iovar_len;
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    iovar_len = bcm_mkiovar(iovar_name, param, paramlen, buf, buflen);
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_VAR, buf, iovar_len, 1);
    return ret;
}
/*******************************************************************************
** Name: WlcDevJoinBss
** Input:Bss_Descriptor*  bss
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.18
** Time: 11:15:19
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevJoinBss(HDC dev,Bss_Descriptor*  bss)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    wl_extjoin_params_t *ext_join_params;
    size_t join_params_size;
    uint32 chan_cnt = 1;
    rk_err_t ret;
    char * join_ioctl_buf;

    join_ioctl_buf = rkos_memory_malloc(WLC_IOCTL_MEDLEN);
    /*
     *  Join with specific BSSID and cached SSID
     *  If SSID is zero join based on BSSID only
     */
    join_params_size = WL_EXTJOIN_PARAMS_FIXED_SIZE + chan_cnt * sizeof(chanspec_t);
    ext_join_params =  (wl_extjoin_params_t*)rkos_memory_malloc(join_params_size);
    if (ext_join_params == NULL)
    {
        ret = -ENOMEM;
        goto exit;
    }

    memcpy(&ext_join_params->ssid.SSID, bss->ssid, bss->ssid_len);
    ext_join_params->ssid.SSID_len = htod32(bss->ssid_len);

    /* Set up join scan parameters */
    ext_join_params->scan.scan_type = -1;
    ext_join_params->scan.nprobes = 2;

    /* increate dwell time to receive probe response or detect Beacon
    * from target AP at a noisy air only during connect command
    */

    ext_join_params->scan.active_time = WL_SCAN_ACTIVE_TIME*3;
    ext_join_params->scan.passive_time = WL_SCAN_PASSIVE_TIME*3;
    ext_join_params->scan.home_time = -1;

    if (bss->bssid)
    {
        memcpy(&ext_join_params->assoc.bssid, bss->bssid, ETH_ALEN);
    }
    else
    {
        memcpy(&ext_join_params->assoc.bssid, &ether_bcast, ETH_ALEN);
    }

    ext_join_params->assoc.chanspec_num = chan_cnt;
    if (chan_cnt)
    {
        uint16 channel, band, bw, ctl_sb;
        chanspec_t chspec;
        channel = bss->channel;
        band = (channel <= CH_MAX_2G_CHANNEL) ? WL_CHANSPEC_BAND_2G : WL_CHANSPEC_BAND_5G;
        bw = WL_CHANSPEC_BW_20;
        ctl_sb = WL_CHANSPEC_CTL_SB_NONE;
        chspec = (channel | band | bw | ctl_sb);
        ext_join_params->assoc.chanspec_list[0] &= WL_CHANSPEC_CHAN_MASK;
        ext_join_params->assoc.chanspec_list[0] |= chspec;
        ext_join_params->assoc.chanspec_list[0] = htodchanspec(ext_join_params->assoc.chanspec_list[0]);
    }
    ext_join_params->assoc.chanspec_num = htod32(ext_join_params->assoc.chanspec_num);
    if (ext_join_params->ssid.SSID_len < IEEE80211_MAX_SSID_LEN)
    {
        rk_printf("Join Bss ssid \"%s\", len (%d)\n", ext_join_params->ssid.SSID, ext_join_params->ssid.SSID_len);
    }

    ret = WlcDevSetBuf(dev, "join", ext_join_params, join_params_size,join_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
exit:
    rkos_memory_free(join_ioctl_buf);

    return ret;
}
/*******************************************************************************
** Name: WlcDevBufVarGet
** Input:HDC dev,char *name,char *buf, int buflen
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 17:52:30
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevBufVarGet(HDC dev,char *name,char *buf, int buflen)
{

}
/*******************************************************************************
** Name: WlcDevBufVarSet
** Input:HDC dev,char *name,char *buf, int len
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 17:51:48
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevBufVarSet(HDC dev,char *name,char *buf, int len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    char ioctlbuf[MAX_WLIW_IOCTL_LEN];
    uint buflen;

    buflen = bcm_mkiovar(name, buf, len, ioctlbuf, sizeof(ioctlbuf));

    return (dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_VAR, ioctlbuf, buflen, 0));
}
/*******************************************************************************
** Name: WlcDevChToChanspec
** Input:(int ch, wl_join_params_t *join_params, int *join_params_size)
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 15:50:35
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevChToChanspec(int ch, wl_join_params_t *join_params, int *join_params_size)
{
    uint16 chanspec = 0;

    if (ch != 0)
    {
        join_params->params.chanspec_num = 1;
        join_params->params.chanspec_list[0] = ch;

        if (join_params->params.chanspec_list[0])
        {
            chanspec |= WL_CHANSPEC_BAND_2G;
        }
        else
        {
            chanspec |= WL_CHANSPEC_BAND_5G;
        }

        chanspec |= WL_CHANSPEC_BW_20;
        chanspec |= WL_CHANSPEC_CTL_SB_NONE;


        *join_params_size += WL_ASSOC_PARAMS_FIXED_SIZE + join_params->params.chanspec_num * sizeof(chanspec_t);


        join_params->params.chanspec_list[0] &= WL_CHANSPEC_CHAN_MASK;
        join_params->params.chanspec_list[0] |= chanspec;
        join_params->params.chanspec_list[0] = htodchanspec(join_params->params.chanspec_list[0]);
        join_params->params.chanspec_num = htod32(join_params->params.chanspec_num);


    }
    return 1;
}
/*******************************************************************************
** Name: WlcDevMhzToCha
** Input:uint freq, uint start_factor
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 15:06:12
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevMhzToCha(uint freq, uint start_factor)
{
    int ch = -1;
    uint base;
    int offset;

    if (start_factor == 0)
    {
        if (freq >= 2400 && freq <= 2500)
        {
            start_factor = WF_CHAN_FACTOR_2_4_G;
        }
        else if (freq >= 5000 && freq <= 6000)
        {
            start_factor = WF_CHAN_FACTOR_5_G;
        }
    }

    if (freq == 2484 && start_factor == WF_CHAN_FACTOR_2_4_G)
        return 14;

    base = start_factor / 2;

    if ((freq < base) || (freq > base + 1000))
        return -1;

    offset = freq - base;
    ch = offset / 5;

    if (offset != (ch * 5))
        return -1;

    if (start_factor == WF_CHAN_FACTOR_2_4_G && (ch < 1 || ch > 13))
        return -1;

    return ch;
}
/*******************************************************************************
** Name: WlcDevIntvarSet
** Input:HDC dev,char *name, int  retval
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 14:30:54
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevIntvarSet(HDC dev,char *name, int  retval)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;

    char buf[WLC_IOCTL_SMLEN];
    uint len;

    retval = htod32(retval);
    len = bcm_mkiovar(name, (char *)(&retval), sizeof(retval), buf, sizeof(buf));

    return (dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_VAR, buf, len, 0));
}
/*******************************************************************************
** Name: WlcDevIntvarGet
** Input: char *name, int *retval
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 14:29:21
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevIntvarGet(HDC dev,char *name, int *retval)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    union
    {
        char buf[WLC_IOCTL_SMLEN];
        int val;
    } var;
    uint len;
    uint data_null;

    len = bcm_mkiovar(name, (char *)(&data_null), 0, (char *)(&var), sizeof(var.buf));
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_GET_VAR, (void *)&var, len, 0);

    *retval = dtoh32(var.val);

    return (ret);
}
/*******************************************************************************
** Name: WlcSwapKeyFromBe
** Input:wl_wsec_key_t *key
** Return: void
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 14:17:38
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN void WlcSwapKeyFromBe(wl_wsec_key_t *key)
{
    key->index = htod32(key->index);
    key->len = htod32(key->len);
    key->algo = htod32(key->algo);
    key->flags = htod32(key->flags);
    key->rxiv.hi = htod32(key->rxiv.hi);
    key->rxiv.lo = htod16(key->rxiv.lo);
    key->iv_initialized = htod32(key->iv_initialized);
}
/*******************************************************************************
** Name: WlcDevSetEssid
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 11:52:11
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSetEssid(HDC dev,struct iw_request_info *info, struct iw_point *dwrq, char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    wl_join_params_t *join_params;
    int join_params_size;

    if (!extra)
        return -EINVAL;

    join_params = rkos_memory_malloc(WL_JOIN_PARAMS_MAX);
    if (!join_params)
    {
        return -ENOMEM;
    }
    memset(join_params, 0, WL_JOIN_PARAMS_MAX);

    memset(&(pstWlcDev->WlcSSID), 0, sizeof(wlc_ssid_t));

    if (dwrq->length && extra)
    {
        pstWlcDev->WlcSSID.SSID_len = MIN(sizeof(pstWlcDev->WlcSSID.SSID), dwrq->length);
        memcpy(pstWlcDev->WlcSSID.SSID, extra, pstWlcDev->WlcSSID.SSID_len);
    }
    else
    {
        pstWlcDev->WlcSSID.SSID_len = 0;
    }
    pstWlcDev->WlcSSID.SSID_len = htod32(pstWlcDev->WlcSSID.SSID_len);

    memset(join_params, 0, sizeof(*join_params));
    join_params_size = sizeof(join_params->ssid);

    memcpy(join_params->ssid.SSID, pstWlcDev->WlcSSID.SSID, pstWlcDev->WlcSSID.SSID_len);
    join_params->ssid.SSID_len = htod32(pstWlcDev->WlcSSID.SSID_len);
    memcpy(&(join_params->params.bssid), &ether_bcast, ETHER_ADDR_LEN);

    WlcDevChToChanspec(pstWlcDev->WlcIwParams.target_channel, join_params, &join_params_size);

    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_SSID, join_params, join_params_size, 0);

    rkos_memory_free(join_params);

    return ret;
}
/*******************************************************************************
** Name: WlcDdvSetFreq
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 11:49:58
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDdvSetFreq(HDC dev,struct iw_request_info *info, struct iw_freq *fwrq, char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    int chan;
    uint sf = 0;

    if (fwrq->e == 0 && fwrq->m < MAXCHANNEL)
    {
        chan = fwrq->m;
    }
    else
    {
        if (fwrq->e >= 6)
        {
            fwrq->e -= 6;
            while (fwrq->e--)
            {
                fwrq->m *= 10;
            }
        }
        else if (fwrq->e < 6)
        {
            while (fwrq->e++ < 6)
            {
                fwrq->m /= 10;
            }
        }

        if (fwrq->m > 4000 && fwrq->m < 5000)
            sf = WF_CHAN_FACTOR_4_G;

        chan = WlcDevMhzToCha(fwrq->m, sf);
    }

    chan = htod32(chan);

    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_CHANNEL, &chan, sizeof(chan), 0);
    if (ret)
    {
        return ret;
    }

    pstWlcDev->WlcIwParams.target_channel = chan;

    return -EINPROGRESS;
}
/*******************************************************************************
** Name: WlcDevIwSetMode
** Input:HDC dev,struct iw_request_info *info,uint32 *uwrq,char *extra
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.17
** Time: 11:18:45
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevIwSetMode(HDC dev,struct iw_request_info *info,uint32 *uwrq,char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    int infra = 0, ap = 0, error = 0;

    switch (*uwrq)
    {
        case IW_MODE_MASTER:
            infra = ap = 1;
            break;
        case IW_MODE_ADHOC:
        case IW_MODE_AUTO:
            break;
        case IW_MODE_INFRA:
            infra = 1;
            break;
        default:
            return -EINVAL;
    }

    if ((error = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_INFRA, &infra, sizeof(infra), 0)) ||
            (error = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_AP, &ap, sizeof(ap), 0)))
        return error;

    return -EINPROGRESS;
}
/*******************************************************************************
** Name: WlcDevSetEncode
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 17:54:09
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSetEncode(HDC dev,struct iw_request_info *info, struct iw_point *dwrq, char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    wl_wsec_key_t key;
    int val, wsec;

    memset(&key, 0, sizeof(key));

    if (dwrq->flags & IW_ENCODE_DISABLED)
    {
        goto encode_disable;
    }

    if ((dwrq->flags & IW_ENCODE_INDEX) == 0)
    {
        for (key.index = 1; key.index <= DOT11_MAX_DEFAULT_KEYS; key.index++)
        {
            val = key.index;
            ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_GET_KEY_PRIMARY, &val, sizeof(val), 0);
            //printf("error = %d, val = %d\n", error, val);
            if (ret)
            {
                return ret;
            }

            if (val)
            {
                break;
            }
        }

        if (key.index == DOT11_MAX_DEFAULT_KEYS)
        {
            key.index = 0;
        }
    }
    else
    {
        key.index = (dwrq->flags & IW_ENCODE_INDEX) - 1;
        if (key.index >= DOT11_MAX_DEFAULT_KEYS)
            return -EINVAL;
    }

    if (!extra || !dwrq->length || (dwrq->flags & IW_ENCODE_NOKEY))
    {
        val = key.index;
        ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_KEY_PRIMARY, &val, sizeof(val), 1);
        if (ret)
        {
            return ret;
        }
    }
    else
    {
        key.len = dwrq->length;

        if (dwrq->length > sizeof(key.data))
            return -EINVAL;

        memcpy(key.data, extra, dwrq->length);//copy password..

        key.flags = WL_PRIMARY_KEY;
        switch (key.len)
        {
            case WEP1_KEY_SIZE:
                key.algo = CRYPTO_ALGO_WEP1;
                break;

            case WEP128_KEY_SIZE:
                key.algo = CRYPTO_ALGO_WEP128;
                break;

            case TKIP_KEY_SIZE:
                key.algo = CRYPTO_ALGO_TKIP;
                break;

            case AES_KEY_SIZE:
                key.algo = CRYPTO_ALGO_AES_CCM;
                break;

            default:
                return -EINVAL;
        }

        WlcSwapKeyFromBe(&key);
        ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_KEY, &key, sizeof(key), 0);
        if (ret)
        {
            return ret;
        }
    }

encode_disable:
    val = (dwrq->flags & IW_ENCODE_DISABLED) ? 0 : WEP_ENABLED;

    ret = WlcDevIntvarGet(dev, "wsec", &wsec);
    if (ret)
        return ret;

    wsec &= ~(WEP_ENABLED);
    wsec |= val;

    ret = WlcDevIntvarSet(dev, "wsec", wsec);
    if (ret)
        return ret;

    val = (dwrq->flags & IW_ENCODE_RESTRICTED) ? 1 : 0;
    val = htod32(val);
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_AUTH, &val, sizeof(val), 0);
    if (ret)
        return ret;

    return 0;
}
/*******************************************************************************
** Name: WlcDevCreateWpaAuthWsec
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 17:52:34
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevCreateWpaAuthWsec(HDC dev,wl_iw_t *iw)
{
    uint32 wsec;

    if (iw->pcipher & (IW_AUTH_CIPHER_WEP40 | IW_AUTH_CIPHER_WEP104))
        wsec = WEP_ENABLED;
    else if (iw->pcipher & IW_AUTH_CIPHER_TKIP)
        wsec = TKIP_ENABLED;
    else if (iw->pcipher & IW_AUTH_CIPHER_CCMP)
        wsec = AES_ENABLED;
    else
        wsec = 0;


    if (iw->gcipher & (IW_AUTH_CIPHER_WEP40 | IW_AUTH_CIPHER_WEP104))
        wsec |= WEP_ENABLED;
    else if (iw->gcipher & IW_AUTH_CIPHER_TKIP)
        wsec |= TKIP_ENABLED;
    else if (iw->gcipher & IW_AUTH_CIPHER_CCMP)
        wsec |= AES_ENABLED;

    if (wsec == 0 && iw->privacy_invoked)
        wsec = WEP_ENABLED;

    return wsec;
}
/*******************************************************************************
** Name: WlcDevSetWpaAuth
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 17:36:51
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSetWpaAuth(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    Bss_Descriptor* ConnetBSS = &(pstWlcDev->ConnectBss);
    rk_err_t ret;

    int paramid;
    int paramval;
    int val = 0;
    wl_iw_t iw_temp;
    wl_iw_t *iw =&iw_temp;

    memset(iw, 0, sizeof(wl_iw_t));

    if (ConnetBSS->encode_type == ENCODE_TYPE_WPA)
    {
        iw->wpaversion =  IW_AUTH_WPA_VERSION_WPA;
    }
    else if (ConnetBSS->encode_type == ENCODE_TYPE_WPA2)
    {
        iw->wpaversion = IW_AUTH_WPA_VERSION_WPA2;
    }
    else if (ConnetBSS->encode_type == ENCODE_TYPE_WPA_WPA2)
    {
        iw->wpaversion = IW_AUTH_WPA_VERSION_WPA2;
    }
    else
    {
        iw->wpaversion = WPA_AUTH_DISABLED;
    }

    paramid = vwrq->flags & IW_AUTH_INDEX;
    paramval = vwrq->value;

#if 0
    BCMDEBUG("SIOCSIWAUTH, %s(%d), paramval = 0x%0x\n",
             paramid == IW_AUTH_WPA_VERSION ? "IW_AUTH_WPA_VERSION" :
             paramid == IW_AUTH_CIPHER_PAIRWISE ? "IW_AUTH_CIPHER_PAIRWISE" :
             paramid == IW_AUTH_CIPHER_GROUP ? "IW_AUTH_CIPHER_GROUP" :
             paramid == IW_AUTH_KEY_MGMT ? "IW_AUTH_KEY_MGMT" :
             paramid == IW_AUTH_TKIP_COUNTERMEASURES ? "IW_AUTH_TKIP_COUNTERMEASURES" :
             paramid == IW_AUTH_DROP_UNENCRYPTED ? "IW_AUTH_DROP_UNENCRYPTED" :
             paramid == IW_AUTH_80211_AUTH_ALG ? "IW_AUTH_80211_AUTH_ALG" :
             paramid == IW_AUTH_WPA_ENABLED ? "IW_AUTH_WPA_ENABLED" :
             paramid == IW_AUTH_RX_UNENCRYPTED_EAPOL ? "IW_AUTH_RX_UNENCRYPTED_EAPOL" :
             paramid == IW_AUTH_ROAMING_CONTROL ? "IW_AUTH_ROAMING_CONTROL" :
             paramid == IW_AUTH_PRIVACY_INVOKED ? "IW_AUTH_PRIVACY_INVOKED" :
             "UNKNOWN",
             paramid, paramval);
#endif

    switch (paramid)
    {
        case IW_AUTH_WPA_VERSION:

            if (paramval & IW_AUTH_WPA_VERSION_DISABLED)
                val = WPA_AUTH_DISABLED;
            else if (paramval & (IW_AUTH_WPA_VERSION_WPA))
                val = WPA_AUTH_PSK | WPA_AUTH_UNSPECIFIED;
            else if (paramval & IW_AUTH_WPA_VERSION_WPA2)
                val = WPA2_AUTH_PSK | WPA2_AUTH_UNSPECIFIED;

            if ((ret = WlcDevIntvarSet(dev, "wpa_auth", val)))
                return ret;
            break;

        case IW_AUTH_CIPHER_PAIRWISE:
            iw->pcipher = paramval;
            val = WlcDevCreateWpaAuthWsec(dev,iw);
            if ((ret = WlcDevIntvarSet(dev, "wsec", val)))
                return ret;
            break;

        case IW_AUTH_CIPHER_GROUP://DGL step 2
            iw->gcipher = paramval;
            val = WlcDevCreateWpaAuthWsec(dev,iw);
            if ((ret = WlcDevIntvarSet(dev, "wsec", val)))
                return ret;
            break;

        case IW_AUTH_KEY_MGMT://DGL step 3
            if ((ret = WlcDevIntvarGet(dev, "wpa_auth", &val)))
                return ret;

            if (val & (WPA_AUTH_PSK | WPA_AUTH_UNSPECIFIED))
            {
                if (paramval & IW_AUTH_KEY_MGMT_PSK)
                    val = WPA_AUTH_PSK;
                else
                    val = WPA_AUTH_UNSPECIFIED;
                if (paramval & 0x04)
                    val |= WPA2_AUTH_FT;
            }
            else if (val & (WPA2_AUTH_PSK | WPA2_AUTH_UNSPECIFIED))
            {
                if (paramval & IW_AUTH_KEY_MGMT_PSK)
                    val = WPA2_AUTH_PSK;
                else
                    val = WPA2_AUTH_UNSPECIFIED;
                if (paramval & 0x04)
                    val |= WPA2_AUTH_FT;
            }
            else if (paramval & IW_AUTH_KEY_MGMT_PSK)
            {
                if (iw->wpaversion == IW_AUTH_WPA_VERSION_WPA)
                    val = WPA_AUTH_PSK;
                else if (iw->wpaversion == IW_AUTH_WPA_VERSION_WPA2)
                    val = WPA2_AUTH_PSK;
                else
                    val = WPA_AUTH_DISABLED;
            }
            else if (paramval & IW_AUTH_KEY_MGMT_802_1X)
            {
                if (iw->wpaversion == IW_AUTH_WPA_VERSION_WPA)
                    val = WPA_AUTH_UNSPECIFIED;
                else if (iw->wpaversion == IW_AUTH_WPA_VERSION_WPA2)
                    val = WPA2_AUTH_UNSPECIFIED;
                else
                    val = WPA_AUTH_DISABLED;
            }
            else
                val = WPA_AUTH_DISABLED;


            if ((ret =  WlcDevIntvarSet(dev, "wpa_auth", val)))
                return ret;
            break;

        case IW_AUTH_TKIP_COUNTERMEASURES:
            WlcDevSetIntBuf(dev,"tkip_countermeasures",(char *)&paramval,1);
            break;

        case IW_AUTH_80211_AUTH_ALG:////DGL step 1
            if (paramval == IW_AUTH_ALG_OPEN_SYSTEM)
                val = 0;
            else if (paramval == IW_AUTH_ALG_SHARED_KEY)
                val = 1;
            else if (paramval == (IW_AUTH_ALG_OPEN_SYSTEM | IW_AUTH_ALG_SHARED_KEY))
                val = 2;
            else
                ret = 1;
            if (!ret && (ret = WlcDevIntvarSet(dev, "auth", val)))
                return ret;
            break;

        case IW_AUTH_WPA_ENABLED:
            if (paramval == 0)
            {
                iw->privacy_invoked = 0;
                iw->pcipher = 0;
                iw->gcipher = 0;
                val = WlcDevCreateWpaAuthWsec(dev,iw);
                if ((ret = WlcDevIntvarSet(dev, "wsec", val)))
                    return ret;

                ret = WlcDevIntvarSet(dev, "wpa_auth", paramval);
                return ret;
            }
            break;

        case IW_AUTH_DROP_UNENCRYPTED:
            if ((ret = WlcDevIntvarSet(dev, "wsec_restrict", paramval)))
                return ret;
            break;

        case IW_AUTH_RX_UNENCRYPTED_EAPOL:
            WlcDevSetIntBuf(dev, "rx_unencrypted_eapol", (char *)&paramval, 1);
            break;

        case IW_AUTH_ROAMING_CONTROL:
            break;

        case IW_AUTH_PRIVACY_INVOKED:
            iw->privacy_invoked = paramval;
            val = WlcDevCreateWpaAuthWsec(dev,iw);
            if ((ret = WlcDevIntvarSet(dev, "wsec", val)))
                return ret;
            break;

        default:
            break;
    }
    return 0;
}
/*******************************************************************************
** Name: WlcDevGetSSID
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 17:07:14
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevGetSSID(HDC dev,void* ssid, int index, UINT8 bwc)
{
#if 0
    int i = 0;
    char * pssid8 = ssid;
    UINT16 * pssid16 = ssid;

    if (!ssid || index<0 || (index >= bss_descriptor_info.total_num))
    {
        return -1;
    }

    while ((i < IW_ESSID_MAX_SIZE) && (bss_descriptor_info.bss[index].ssid[i]))
    {
        if (bwc)
        {
            pssid16[i] = bss_descriptor_info.bss[index].ssid[i];
            pssid16[i+1] = 0;
        }
        else
        {
            pssid8[i] = bss_descriptor_info.bss[index].ssid[i];
            pssid8[i+1] = 0;
        }
        i++;
    };
#endif

    return 0;

}
/*******************************************************************************
** Name: WlcDevGetApRssi
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 17:04:51
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevGetApRssi(Bss_Descriptor * Bss)
{
    return Bss->rssi;
}
/*******************************************************************************
** Name: WlcDevGetEncodeType
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:55:10
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevGetEncodeType(Bss_Descriptor * Bss)
{

    if (Bss->encode_type == ENCODE_TYPE_NO)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
/*******************************************************************************
** Name: WlcDevWepConnect
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:50:50
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevWepConnect(HDC dev,char * password, int pwd_len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    Bss_Descriptor * ConnectBss =  &(pstWlcDev->ConnectBss);
    int ret;

    uint32 wifi_mode = ConnectBss->mode;
    struct iw_point dwrq;
    struct iw_freq fwrq;

    WlcDevIwSetMode(dev,NULL,&wifi_mode,NULL);

    //if (pwd_len == 0)
    if (ConnectBss->encode_type == ENCODE_TYPE_NO)
    {
        dwrq.flags = IW_ENCODE_DISABLED;
    }
    else
    {
        dwrq.flags = 0;
        dwrq.flags |=  WEP_KEY_INDEX_1 ;
        dwrq.flags |= IW_ENCODE_RESTRICTED;
        dwrq.length = pwd_len;
    }

    WlcDevSetEncode(dev, NULL, &dwrq, password);

    fwrq.e = 0;
    fwrq.m = ConnectBss->channel;

    WlcDdvSetFreq(dev, NULL, &fwrq, NULL);

    dwrq.length = ConnectBss->ssid_len;
    ret =WlcDevSetEssid(dev, NULL, &dwrq, (char*)ConnectBss->ssid);

    return ret;
}
/*******************************************************************************
** Name: WlcDevWpaConnect
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.16
** Time: 16:50:13
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevWpaConnect(HDC dev,char * password, int pwd_len)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    Bss_Descriptor * ConnectBss =  &(pstWlcDev->ConnectBss);
    int ret;
    uint32 wifi_mode;

    struct iw_point dwrq;
    struct iw_freq fwrq;
    struct iw_param vwrq;

    wifi_mode = ConnectBss->mode;//IW_MODE_INFRA
    ret = WlcDevIwSetMode(dev, NULL, &wifi_mode, NULL);

    dwrq.flags = IW_ENCODE_DISABLED;
    ret = WlcDevSetEncode(dev, NULL, &dwrq, NULL); //disable wep

    fwrq.e = 0;
    fwrq.m = ConnectBss->channel;
    ret = WlcDdvSetFreq(dev, NULL, &fwrq, NULL);

    if (ConnectBss->encode_type == ENCODE_TYPE_WPA)
    {
        ret = WlcDevBufVarSet(dev, "wpaie", ConnectBss->wpa_ie, ConnectBss->wpa_ie_len);
    }
    else
    {
        ret = WlcDevBufVarSet(dev, "wpaie", ConnectBss->rsn_ie, ConnectBss->rsn_ie_len);
    }


    if (ConnectBss->encode_type == ENCODE_TYPE_WPA)
    {
        rk_printf("encode type is wpa");
    }
    else if (ConnectBss->encode_type == ENCODE_TYPE_WPA2)
    {
        rk_printf("encode type is wpa2");
    }
    else if (ConnectBss->encode_type == ENCODE_TYPE_WPA_WPA2)
    {
        rk_printf("encode type is wpa-wpa2");
    }

    if (ConnectBss->gcipher_type == IW_AUTH_CIPHER_CCMP)
    {
        rk_printf("IW_AUTH_CIPHER_CCMP");
    }
    else if (ConnectBss->gcipher_type == IW_AUTH_CIPHER_TKIP)
    {
        rk_printf("IW_AUTH_CIPHER_TKIP");
    }
    else
    {
        rk_printf("no cipher type");
    }


    //set auth type.open_system shared_key "802.11 AUTH"
    vwrq.flags = IW_AUTH_80211_AUTH_ALG;
    vwrq.value = IW_AUTH_ALG_OPEN_SYSTEM;
    ret = WlcDevSetWpaAuth(dev, NULL, &vwrq, NULL);

    //set cipher type.WEP TKIP CCMP
    vwrq.flags = IW_AUTH_CIPHER_PAIRWISE;
    vwrq.value =  ConnectBss->pcipher_type;//IW_AUTH_CIPHER_TKIP IW_AUTH_CIPHER_CCMP
    ret = WlcDevSetWpaAuth(dev, NULL, &vwrq, NULL);

    //set cipher type.WEP TKIP CCMP
    vwrq.flags = IW_AUTH_CIPHER_GROUP;
    vwrq.value =  ConnectBss->gcipher_type;//IW_AUTH_CIPHER_TKIP IW_AUTH_CIPHER_CCMP
    ret = WlcDevSetWpaAuth(dev, NULL, &vwrq, NULL);

    //
    vwrq.flags = IW_AUTH_KEY_MGMT;
    vwrq.value = IW_AUTH_KEY_MGMT_PSK;
    ret = WlcDevSetWpaAuth(dev, NULL, &vwrq, NULL);


    ret  = WlcDevJoinBss(dev,ConnectBss);

    if (ret  == BCME_UNSUPPORTED)
    {
        dwrq.flags = 0;
        dwrq.length = ConnectBss->ssid_len;
        return WlcDevSetEssid(dev, NULL, &dwrq, ConnectBss->ssid);
    }

    return ret;
}
/*******************************************************************************
** Name: WlcDev_BssInfoInit
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.11
** Time: 17:33:34
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_BssInfoInit(HDC dev)
{

    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    Bss_Descriptor_Info * pBSSInfo = &(pstWlcDev->BssInfo);
    Bss_Descriptor * pPreBSS;
    Bss_Descriptor * pTempBSS;

    pTempBSS = pBSSInfo->pBss;
    while (pTempBSS != NULL)
    {
        pPreBSS = pTempBSS->Next;
        rkos_memory_free(pTempBSS);
        pTempBSS = pPreBSS;
    }

    pBSSInfo->total_num = 0;

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: RK903DevBcmToQdbm
** Input:uint16 mw
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.10
** Time: 17:03:21
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN uint8 WlcDevBcmToQdbm(uint16 mw)
{
    uint8 qdbm;
    int offset;
    uint mw_uint = mw;
    uint boundary;

    /* handle boundary case */
    if (mw_uint <= 1)
        return 0;

    offset = QDBM_OFFSET;

    /* move mw into the range of the table */
    while (mw_uint < QDBM_TABLE_LOW_BOUND)
    {
        mw_uint *= 10;
        offset -= 40;
    }

    for (qdbm = 0; qdbm < QDBM_TABLE_LEN - 1; qdbm++)
    {
        boundary = nqdBm_to_mW_map[qdbm] + (nqdBm_to_mW_map[qdbm+1] - nqdBm_to_mW_map[qdbm])/2;
        if (mw_uint < boundary)
            break;
    }

    qdbm += (uint8)offset;

    return (qdbm);
}
/*******************************************************************************
** Name: RK903DevWldevIovarSetbuf
** Input:dhd_pub_t *pub, int8 *iovar_name,
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.10
** Time: 15:29:47
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevWldevIovarSetbuf(HDC dev,int8 *iovar_name, void *param, int32 paramlen,
        void *buf, int32 buflen, void * buf_sync)
{
    int32 ret = 0;
    int32 iovar_len;

    iovar_len = bcm_mkiovar(iovar_name, param, paramlen, buf, buflen);
    ret = dhd_wl_ioctl_cmd(dev, WLC_SET_VAR, buf, iovar_len, TRUE ,0);
    return ret;
}
/*******************************************************************************
** Name: RK903Dev_ WlScanPrep
** Input:HDC dev,struct wl_scan_params *params, void *request
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.10
** Time: 15:16:26
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_WlScanPrep(HDC dev,struct wl_scan_params *params, void *request)
{
    uint32 n_ssids;
    uint32 n_channels;
    uint16 channel;
    chanspec_t chanspec;
    int i, offset;
    char *ptr;
    wlc_ssid_t ssid;
    uint16 center_freq[14] = {2412, 2417, 2422, 2427, 2432,
                              2437, 2442, 2447, 2452, 2457,
                              2462, 2467, 2472, 2484
                             };

    memset(&params->ssid, 0, sizeof(wlc_ssid_t));
    memcpy(&params->bssid, &ether_bcast, ETHER_ADDR_LEN);
    params->bss_type = DOT11_BSSTYPE_ANY;
    params->scan_type = 0;
    params->nprobes = -1;
    params->active_time = 200; // terence 20120609: extend active scan time interval
    params->passive_time = -1;
    params->home_time = -1;
    params->channel_num = 0;

    params->nprobes = htod32(params->nprobes);
    params->active_time = htod32(params->active_time);
    params->passive_time = htod32(params->passive_time);
    params->home_time = htod32(params->home_time);

    /* if request is null just exit so it will be all channel broadcast scan */
    n_ssids = 1;
    n_channels = 14;

    if (n_channels > 0)
    {
        for (i = 0; i < n_channels; i++)
        {
            chanspec = 0;
            channel = ieee80211_frequency_to_channel(center_freq[i]);
#if 0
            if (request->channels[i]->band == IEEE80211_BAND_2GHZ)
                chanspec |= WL_CHANSPEC_BAND_2G;
            else
                chanspec |= WL_CHANSPEC_BAND_5G;

            if (request->channels[i]->flags & IEEE80211_CHAN_NO_HT40)
            {
                chanspec |= WL_CHANSPEC_BW_20;
                chanspec |= WL_CHANSPEC_CTL_SB_NONE;
            }
            else
            {
                chanspec |= WL_CHANSPEC_BW_40;
                if (request->channels[i]->flags & IEEE80211_CHAN_NO_HT40PLUS)
                    chanspec |= WL_CHANSPEC_CTL_SB_LOWER;
                else
                    chanspec |= WL_CHANSPEC_CTL_SB_UPPER;
            }
#endif
            params->channel_list[i] = channel;
            params->channel_list[i] &= WL_CHANSPEC_CHAN_MASK;
            //params->channel_list[i] |= chanspec;
            params->channel_list[i] = htod16(params->channel_list[i]);
        }
    }
    else
    {
        rk_printf("Scanning all channels");
    }

    /* Copy ssid array if applicable */
    if (n_ssids > 0)
    {
        offset = offsetof(wl_scan_params_t, channel_list) + n_channels * sizeof(uint16);
        offset = ROUNDUP(offset, sizeof(uint32));
        ptr = (char*)params + offset;
        for (i = 0; i < n_ssids; i++)
        {
            memset(&ssid, 0, sizeof(wlc_ssid_t));
#if 0
            ssid.SSID_len = request->ssids[i].ssid_len;
            memcpy(ssid.SSID, request->ssids[i].ssid, ssid.SSID_len);
            if (!ssid.SSID_len)
                WL_SCAN(("%d: Broadcast scan\n", i));
            else
                WL_SCAN(("%d: scan  for  %s size =%d\n", i,
                         ssid.SSID, ssid.SSID_len));
#endif
            memcpy(ptr, &ssid, sizeof(wlc_ssid_t));
            ptr += sizeof(wlc_ssid_t);
        }
    }
    else
    {
        rk_printf("Broadcast scan\n");
    }

    /* Adding mask to channel numbers */
    params->channel_num = htod32((n_ssids << WL_SCAN_PARAMS_NSSID_SHIFT) | (n_channels & WL_SCAN_PARAMS_COUNT_MASK));
}
/*******************************************************************************
** Name: RK903Dev_WlcNtvarGet
** Input:dhd_pub_t *pub,char *name,int val
** Return:  rk_err_t
** Owner:zhuzhe
** Date: 2014.7.9
** Time: 20:05:52
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN  rk_err_t WlcDev_WlcNtvarGet(HDC dev,char *name,int * retval)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;
    union
    {
        char buf[WLC_IOCTL_SMLEN];
        int val;
    } var;
    uint len;
    uint data_null;

    len = bcm_mkiovar(name, (char *)(&data_null), 0, (char *)(&var), sizeof(var.buf));
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev,WLC_GET_VAR, (void *)&var, len, 0);

    *retval = var.val;

    return (ret);
}
/*******************************************************************************
** Name: RK903Dev_WlcNtvarSet
** Input:dhd_pub_t *pub,char *name,int val
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.9
** Time: 18:22:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_WlcNtvarSet(HDC dev,char *name,int val)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    char buf[WLC_IOCTL_SMLEN];
    uint len;

    len = bcm_mkiovar(name, (char *)(&val), sizeof(val), buf, sizeof(buf));

    return (dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_VAR, buf, len, 0, 0));
}
/*******************************************************************************
** Name: RK903DevSdioPend
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.9
** Time: 10:52:24
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevSdioPend(HDC dev)
{

}
/*******************************************************************************
** Name: RK903DevEscanSet
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.9
** Time: 10:24:13
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDevEscanSet(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)dev;
    rk_err_t ret;

    uint32 n_channels = 14;
    uint32 n_ssids = 1;
    int params_size;
    wl_escan_params_t *params = NULL;
    char * escan_ioctl_buf= NULL;

    escan_ioctl_buf = rkos_memory_malloc(WLC_IOCTL_MEDLEN);

    /* WL_SCAN_PARAMS_FIXED_SIZE == sizeof(wl_scan_params_t)*/
    params_size = (OFFSETOF(wl_escan_params_t, params) + WL_SCAN_PARAMS_FIXED_SIZE);

    /* Allocate space for populating ssids in wl_iscan_params struct */
    /* If n_channels is odd, add a padd of u16 */
    if (n_channels % 2)
    {
        params_size += sizeof(uint16) * (n_channels + 1);
    }
    else
    {
        params_size += sizeof(uint16) * n_channels;
    }

    /* Allocate space for populating ssids in wl_iscan_params struct */
    params_size += sizeof(struct wlc_ssid) * n_ssids;
    params = (wl_escan_params_t *)rkos_memory_malloc(params_size);

    params->version = ESCAN_REQ_VERSION;
    params->action =  WL_SCAN_ACTION_START;
    params->sync_id = 0x1234;

    /* parm scan setting..*/
    WlcDev_WlScanPrep(pstWlcDev->pstRk903Dev,&params->params, NULL);

    if (params_size + sizeof("escan") >= WLC_IOCTL_MEDLEN)
    {
        rkos_memory_free(params);
        ret = -ENOMEM;
        goto set_scan_end;
    }

    ret = WlcDevSetBuf(pstWlcDev, "escan", params, params_size, escan_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
    //will be set by rk903
    //ret = WlcDevWldevIovarSetbuf(pstWlcDev, "escan", params, params_size, escan_ioctl_buf, WLC_IOCTL_MEDLEN, NULL);
    rkos_memory_free(params);

set_scan_end:
    rkos_memory_free(escan_ioctl_buf);
    return ret;
}

/*******************************************************************************
** Name: RK903Dev_GetTxpow
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.8
** Time: 17:01:24
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_GetTxpow(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;
    int disable, txpwrdbm;
    uint8 result;

    if ((ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_GET_RADIO, &disable, sizeof(disable),0,0))
            || (ret = WlcDevIntvarGet(dev,"qtxpower",(int *)&txpwrdbm)));
    return ret;

#if 0
    result = (uint8)(txpwrdbm & ~WL_TXPWR_OVERRIDE);
    printf("txpwrdmw = %d, disable = %d\n", (int32)bcm_qdbm_to_mw(result), disable);
    return 0;
#endif

    return ret;
}
/*******************************************************************************
** Name: RK903Dev_SetTxpow
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.8
** Time: 17:00:09
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_READ_
COMMON FUN rk_err_t WlcDev_SetTxpow(HDC dev,struct iw_request_info *info,struct iw_param *vwrq,char *extra)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;
    int disable;
    uint16 txpwrmw = 180;//dgl 120 150 180(good)

    disable = WL_RADIO_SW_DISABLE << 16;
    ret = dhd_wl_ioctl_cmd(pstWlcDev->pstRk903Dev, WLC_SET_RADIO, &disable, sizeof(disable), 1);

    ret = WlcDevIntvarSet(dev,"qtxpower", (int)(WlcDevBcmToQdbm(txpwrmw)));

    return ret;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*******************************************************************************
** Name: Rk903DevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_WRITE_
INIT FUN rk_err_t WlcDevResume(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;
}


/*******************************************************************************
** Name: Rk903DevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_WRITE_
INIT FUN rk_err_t WlcDevSuspend(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: Rk903DevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_INIT_
INIT FUN rk_err_t WlcDevDelete(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;
}


/*******************************************************************************
** Name: Rk903DevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_INIT_
INIT FUN rk_err_t WlcDevDeInit(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret;

}


/*******************************************************************************
** Name: Rk903DevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_INIT_
INIT FUN rk_err_t WlcDevInit(HDC dev)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev =  (WLC_DEVICE_CLASS*)pstDev;
    rk_err_t ret = RK_SUCCESS;

    return RK_SUCCESS;
}
/*******************************************************************************
** Name: WlcDev_Create
** Input:void * arg
** Return: HDC
** Owner:zhuzhe
** Date: 2014.7.10
** Time: 19:23:28
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_INIT_
INIT API HDC WlcDev_Create(uint32 DevID, void * arg)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev;
    HDC  pstRk903Dev = *((HDC *)arg);

    if (arg == NULL)
    {
        return (HDC)(RK_PARA_ERR);
    }

    pstWlcDev =  rkos_memory_malloc(sizeof(WLC_DEVICE_CLASS));
    if (pstWlcDev == NULL)
    {
        return NULL;
    }

	memset(pstWlcDev,0,sizeof(WLC_DEVICE_CLASS));
    pstWlcDev->osWlcControlReqSem = rkos_semaphore_create(1,1);
    pstWlcDev->osWlcControlScanSem = rkos_semaphore_create(1,0);
    pstWlcDev->osWlcControlConnectSem = rkos_semaphore_create(1,0);
    pstWlcDev->osWlcAuthSem = rkos_semaphore_create(1,0);
    pstWlcDev->p80211Buff = rkos_memory_malloc(WPADATALEN);


    if (pstWlcDev->osWlcAuthSem == 0 || pstWlcDev->osWlcControlReqSem == 0 ||  pstWlcDev->osWlcControlScanSem == 0 || pstWlcDev->p80211Buff == NULL)
    {
        rkos_semaphore_delete(pstWlcDev->osWlcControlReqSem );
        rkos_semaphore_delete(pstWlcDev->osWlcControlScanSem);
        rkos_semaphore_delete(pstWlcDev->osWlcControlConnectSem );
        rkos_memory_free(pstWlcDev->p80211Buff);
        rkos_memory_free(pstWlcDev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstWlcDev;

    pstDev->suspend = WlcDevSuspend;
    pstDev->resume  = WlcDevResume;

    pstWlcDev->pstRk903Dev = pstRk903Dev;
    gpstWlcDevISR  = pstWlcDev;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
	FW_ModuleOverlay(MODULE_ID_WLC_DEV, MODULE_OVERLAY_CODE);
    #endif

    if (WlcDevInit(pstDev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstWlcDev->osWlcControlReqSem );
        rkos_memory_free(pstWlcDev);
        return (HDC) RK_ERROR;
    }

    return pstDev;

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/


#ifdef _WLC_DEV_SHELL_
_DRIVER_WLC_WLCDEVICE_SHELL_
static uint8* ShellWlcName[] =
{
    "pcb",
    "mc",
    "del",
    "test",
    "scan",
    "auth",
    "help",
    "...",
    "\b",
};


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WlcDev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL API rk_err_t WlcDev_Shell(HDC dev, uint8 * pstr)
{
    uint32 i = 0;
    uint8  *pItem;
    uint16 StrCnt = 0;
    rk_err_t   ret;

    uint8 Space;

    StrCnt = ShellItemExtract(pstr,&pItem, &Space);

    if (StrCnt == 0)
    {
        return RK_ERROR;
    }

    ret = ShellCheckCmd(ShellWlcName, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {

        case 0x00:  //pcb
            ret = WlcDevShellPcb(dev,pItem);
            break;

        case 0x01:  //mc
            ret = WlcDevShellMc(dev,pItem);
            break;

        case 0x02:  //del
            ret = WlcDevShellDel(dev, pItem);
            break;

        case 0x03:  //test
            ret = WlcDevShellTest(dev, pItem);
            break;
        case 0x04:  //scan
            ret = WlcDevShellScan(dev, pItem);
            break;

        case 0x05:  //auth
            ret = WlcDevShellAuth(dev,pItem);
            break;

        case 0x06:  //help
            ret = WlcDevShellHelp(dev,pItem);
            break;

        default:
            ret = RK_ERROR;
            break;

    }

    return ret;

}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(shell) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: WlcDevShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.3
** Time: 17:22:51
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("wlc 命令集提供了一系列的命令对wlc进行操作\r\n");
    rk_print_string("wlc包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("mc        创建wlc device        \r\n");
    rk_print_string("del       删除wlc device        \r\n");
    rk_print_string("scan      扫描wlc device        \r\n");
    rk_print_string("auth      连接wlc device命令    \r\n");
    rk_print_string("help      显示wlc device命令帮助信息  \r\n");

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: WlcDevShellAuth
** Input:HDC dev,uint8 *pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.4
** Time: 16:14:34
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellAuth(HDC dev,uint8 *pstr)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev;
    HDC  pstRk903Dev;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.auth : connect  wlcdev cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

	return RK_SUCCESS;

    pstWlcDev =  (WLC_DEVICE_CLASS *) RKDev_Open(DEV_CLASS_WLC, 0, NOT_CARE);

    if (pstWlcDev == NULL)
    {
        rk_print_string("WLC Device Open Failure");
        return RK_SUCCESS;
    }

    WlcDev_ConnectPro(pstWlcDev,TestSsid,TestPassword,strlen(TestPassword));
    //RKTaskCreate(TASK_ID_WEB_TASK);
	//while(1);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: Rk903DevShellScan
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.7
** Time: 15:29:57
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t  WlcDevShellScan(HDC dev, uint8 * pstr)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev;
    HDC  pstRk903Dev;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.scan : wlc device scan cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

	return RK_SUCCESS;
    pstWlcDev =  (WLC_DEVICE_CLASS *) RKDev_Open(DEV_CLASS_WLC, 0, NOT_CARE);

    if (pstWlcDev == NULL)
    {
        rk_print_string("WLC Device Open Failure");
        return RK_SUCCESS;
    }

    WlcDev_ScanWork(pstWlcDev);

    return RK_SUCCESS;

}
/*******************************************************************************
** Name: Rk903DevShellTest
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellTest(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.test : test wlcdev cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

	return RK_SUCCESS;
}

/*******************************************************************************
** Name: Rk903DevShellDel
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellDel(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.del : delete wlcdev cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

	return RK_SUCCESS;
}

/*******************************************************************************
** Name: Rk903DevShellMc
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellMc(HDC dev, uint8 * pstr)
{
    DEVICE_CLASS * pstDev;
    WLC_DEVICE_CLASS * pstWlcDev;
    HDC  pstRk903Dev;
    rk_err_t ret;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.mc :  wlcdev mc  cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

    pstRk903Dev  = RKDev_Open(DEV_CLASS_RK903, 0, NOT_CARE);

    if ((pstRk903Dev == NULL) || (pstRk903Dev == (HDC)RK_ERROR) || (pstRk903Dev == (HDC)RK_PARA_ERR))
    {
        rk_print_string("RK903 Device Open Failure");
        return RK_SUCCESS;
    }

    ret = RKDev_Create(DEV_CLASS_WLC, 0, &pstRk903Dev);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("WLC Device Create Failure");
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: Rk903DevShellPcb
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_WLC_WLCDEVICE_SHELL_
SHELL FUN rk_err_t WlcDevShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("wlc.pcb :  wlcdev pcb info cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif
    // TODO:
    //add other code below:
    //...

	return RK_SUCCESS;
}

#endif
#endif

