/*
********************************************************************************************
*
*				  Copyright (c): 2014 - 2014 + 5, zhuzhe
*							   All rights reserved.
*
* FileName: Driver\Wlc\WlcDevice.h
* Owner: zhuzhe
* Date: 2014.7.10
* Time: 19:14:16
* Desc:
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    zhuzhe     2014.7.10     19:14:16   1.0
********************************************************************************************
*/

#ifndef __DRIVER_WLC_WLCDEVICE_H__
#define __DRIVER_WLC_WLCDEVICE_H__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define MAX_WPA_IE_LEN  	64
#define MAX_BSS         	72
#define MAX_PASSWORD_LEN	32//64
#define IW_ESSID_MAX_SIZE	32

#define ETH_ALEN	6		/* Octets in one ethernet addr	 */
#define ETH_HLEN	14		/* Total octets in header.	 */
#define ETH_ZLEN	60		/* Min. octets in frame sans FCS */
#define ETH_DATA_LEN	1500		/* Max. octets in payload	 */
#define ETH_FRAME_LEN	1514		/* Max. octets in frame sans FCS */
#define ETH_FCS_LEN	4		/* Octets in the FCS		 */
typedef struct _Bss_Descriptor
{
	uint8  bssid[ETH_ALEN];

	uint8  ssid[IW_ESSID_MAX_SIZE + 1];
	uint8  ssid_len;

	//uint16 capability;
	uint32 rssi;
	uint32 channel;
	//uint16 beaconperiod;
	//uint32 atimwindow;


	/* IW_MODE_AUTO, IW_MODE_ADHOC, IW_MODE_INFRA */
	uint8 mode;

	/* zero-terminated array of supported data rates */
    uint8  encode_type; //wpa-psk wpa2-psk wep
    uint16 encode_flag;

    uint8  pcipher_type;  //TKIP  AES
    uint8  gcipher_type;  //TKIP  AES

    uint8  cipher_type;  //TKIP  AES
    uint8  password[MAX_PASSWORD_LEN];
    uint8  wpa_ie_len;
    uint8  rsn_ie_len;
	uint8  wpa_ie[MAX_WPA_IE_LEN];
	uint8  rsn_ie[MAX_WPA_IE_LEN]; //WPA2

    struct _Bss_Descriptor * Next;
}Bss_Descriptor ;
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
extern rk_err_t WlcDev_SetAuthSuccess(HDC Dev);
extern rk_err_t WlcDev_80211DataInput(void *data, uint len);
extern rk_err_t WlcDev_SetEncodeExt(HDC dev,void *info,void *dwrq,char *extra);
//extern rk_err_t WlcDev_SetEncodeExt(HDC dev,struct iw_request_info *info, struct iw_point *dwrq,char *extra);
extern rk_err_t WlcDev_GetConnectBss(void *buf);
extern rk_err_t WlcDev_GetWpaSm(void * buf);
extern rk_err_t WlcDev_ConnectPro(HDC dev,char * ssid, char * password, int pwd_len);
extern int32 WlcDev_EscanHandle(void *e, void *data);
extern rk_err_t WlcDev_SSIDCheck(HDC dev);
extern rk_err_t WlcDev_ScanWork(HDC dev);
extern HDC WlcDev_Create(uint32 DevID, void * arg);
extern rk_err_t WlcDev_Connect(HDC dev,char * password, int pwd_len);
extern rk_err_t WlcDev_ConnectInit(HDC dev,char * ssid, char * password, int pwd_len);
extern rk_err_t WlcDev_Shell(HDC dev, uint8 * pstr);


#endif

