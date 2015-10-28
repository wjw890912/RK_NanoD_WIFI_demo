/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\Rk903\Rk903Device.c
* Owner: Aaron.sun
* Date: 2014.6.14
* Time: 9:11:08
* Desc: Rk903 device class
* History:
*    <author>    <date>       <time>     <version>     <Desc>
*    Aaron.sun     2014.6.14     9:11:08   1.0
********************************************************************************************
*/

#include "BspConfig.h"
#ifdef __DRIVER_RK903_RK903DEVICE_C__

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

#ifdef _USE_RK901_
#include "fw_rk901.h";
#include "nvram_rk901.h";
#endif


#include "bcmevent.h"
#include "bcmendian.h"
//#include "802.11.h"
#include "ethernet.h"

#include "Rk903Macro.h"
#include "WlcMacro.h"
#include "WebTask.h"

#if (WIFI_MODULE == RK903)
#include	"fw_RK903.h"
#include	"nvram_RK903.h"
#elif (WIFI_MODULE == RK901)
#include	"fw_RK901.h"
#include	"nvram_RK901.h"
#elif (WIFI_MODULE == AP6181)
#include	"nvram_AP6181.h"
#include	"fw_AP6181.h"
#endif


#define WPA_AUTH


//extern define
#define WL_EVENTING_MASK_LEN    16
#define WLC_CNTRY_BUF_SZ    4

#define	NBBY	8

#define	setbit(a, i)	(((uint8 *)a)[(i)/NBBY] |= 1<<((i)%NBBY))
#define	clrbit(a, i)	(((uint8 *)a)[(i)/NBBY] &= ~(1<<((i)%NBBY)))
#define	isset(a, i)	(((const uint8 *)a)[(i)/NBBY] & (1<<((i)%NBBY)))
#define	isclr(a, i)	((((const uint8 *)a)[(i)/NBBY] & (1<<((i)%NBBY))) == 0)

#define	bcmp(b1, b2, len)      memcmp((b1), (b2), (len))
#define	bcopy(src, dst, len)	memcpy((dst), (src), (len))
/* SDA_FRAMECTRL */
#define SFC_RF_TERM	(1 << 0)	/* Read Frame Terminate */
#define SFC_WF_TERM	(1 << 1)	/* Write Frame Terminate */
#define SFC_CRC4WOOS	(1 << 2)	/* HW reports CRC error for write out of sync */
#define SFC_ABORTALL	(1 << 3)	/* Abort cancels all in-progress frames */


#ifndef ASSERT
#define ASSERT(x)	do {if ((x) == 0)  \
                        {rk_printf("ASSERT"); \
                        while(1);            \
                        }                  \
                       } while(0)
#endif

#define TXRETRIES	2	/* # of retries for tx frames */

#define firstread DHD_FIRSTREAD
#define BCME_PENDING	1
#define retry_limit 2
#define retrydata 0

#define forcealign 1
#define RETRYCHAN(chan) (((chan) == SDPCM_EVENT_CHANNEL) || retrydata)
#define DHD_MAX_IFS	1//dgl 16
#define DHD_DEL_IF	-0xe
#define DHD_BAD_IF	-0xf

#define CDC_MAX_MSG_SIZE   ETHER_MAX_LEN


#define CDCL_IOC_OUTLEN_MASK   0x0000FFFF

#define CDCL_IOC_OUTLEN_SHIFT  0
#define CDCL_IOC_INLEN_MASK    0xFFFF0000
#define CDCL_IOC_INLEN_SHIFT   16


#define CDCF_IOC_ERROR		0x01
#define CDCF_IOC_SET		0x02
#define CDCF_IOC_OVL_IDX_MASK	0x3c
#define CDCF_IOC_OVL_RSV	0x40
#define CDCF_IOC_OVL		0x80
#define CDCF_IOC_ACTION_MASK	0xfe
#define CDCF_IOC_ACTION_SHIFT	1
#define CDCF_IOC_IF_MASK	0xF000
#define CDCF_IOC_IF_SHIFT	12
#define CDCF_IOC_ID_MASK	0xFFFF0000
#define CDCF_IOC_ID_SHIFT	16

#define CDC_IOC_IF_IDX(flags)	(((flags) & CDCF_IOC_IF_MASK) >> CDCF_IOC_IF_SHIFT)
#define CDC_IOC_ID(flags)	(((flags) & CDCF_IOC_ID_MASK) >> CDCF_IOC_ID_SHIFT)

#define CDC_GET_IF_IDX(hdr) \
	((int)((((hdr)->flags) & CDCF_IOC_IF_MASK) >> CDCF_IOC_IF_SHIFT))
#define CDC_SET_IF_IDX(hdr, idx) \
	((hdr)->flags = (((hdr)->flags & ~CDCF_IOC_IF_MASK) | ((idx) << CDCF_IOC_IF_SHIFT)))



#define	BDC_HEADER_LEN		4

#define BDC_PROTO_VER_1		1
#define BDC_PROTO_VER		2

#define BDC_FLAG_VER_MASK	0xf0
#define BDC_FLAG_VER_SHIFT	4

#define BDC_FLAG__UNUSED	0x03
#define BDC_FLAG_SUM_GOOD	0x04
#define BDC_FLAG_SUM_NEEDED	0x08

#define BDC_PRIORITY_MASK	0x7

#define BDC_FLAG2_FC_FLAG	0x10

#define BDC_PRIORITY_FC_SHIFT	4

#define BDC_FLAG2_IF_MASK	0x0f
#define BDC_FLAG2_IF_SHIFT	0
#define BDC_FLAG2_PAD_MASK		0xf0
#define BDC_FLAG_PAD_MASK		0x03
#define BDC_FLAG2_PAD_SHIFT		2
#define BDC_FLAG_PAD_SHIFT		0
#define BDC_FLAG2_PAD_IDX		0x3c
#define BDC_FLAG_PAD_IDX		0x03
#define BDC_GET_PAD_LEN(hdr) \
	((int)(((((hdr)->flags2) & BDC_FLAG2_PAD_MASK) >> BDC_FLAG2_PAD_SHIFT) | \
	((((hdr)->flags) & BDC_FLAG_PAD_MASK) >> BDC_FLAG_PAD_SHIFT)))
#define BDC_SET_PAD_LEN(hdr, idx) \
	((hdr)->flags2 = (((hdr)->flags2 & ~BDC_FLAG2_PAD_MASK) | \
	(((idx) & BDC_FLAG2_PAD_IDX) << BDC_FLAG2_PAD_SHIFT))); \
	((hdr)->flags = (((hdr)->flags & ~BDC_FLAG_PAD_MASK) | \
	(((idx) & BDC_FLAG_PAD_IDX) << BDC_FLAG_PAD_SHIFT)))

#define BDC_GET_IF_IDX(hdr) \
	((int)((((hdr)->flags2) & BDC_FLAG2_IF_MASK) >> BDC_FLAG2_IF_SHIFT))
#define BDC_SET_IF_IDX(hdr, idx) \
	((hdr)->flags2 = (((hdr)->flags2 & ~BDC_FLAG2_IF_MASK) | ((idx) << BDC_FLAG2_IF_SHIFT)))

struct bdc_header
{
	uint8	flags;
	uint8	priority;
	uint8	flags2;
	uint8	dataOffset;
};

#define BCM_EVENT_MSG_VERSION		2
#define BCM_MSG_IFNAME_MAX		16


#define WLC_EVENT_MSG_LINK		0x01
#define WLC_EVENT_MSG_FLUSHTXQ		0x02
#define WLC_EVENT_MSG_GROUP		0x04
#define WLC_EVENT_MSG_UNKBSS		0x08
#define WLC_EVENT_MSG_UNKIF		0x10



#define DHD_PACKET_TIMEOUT_MS	1000
#define DHD_EVENT_TIMEOUT_MS	1500

typedef __packed struct bcmeth_hdr
{
	uint16	subtype;
	uint16	length;
	uint8	version;
	uint8	oui[3];

	uint16	usr_subtype;
} bcmeth_hdr_t;


typedef struct bcm_event
{
	struct ether_header eth;
	bcmeth_hdr_t		bcm_hdr;
	wl_event_msg_t		event;
} bcm_event_t;

#define BCMILCP_BCM_SUBTYPE_RESERVED		0
#define BCMILCP_BCM_SUBTYPE_EVENT		1
#define BCMILCP_BCM_SUBTYPE_SES			2

#define BRCM_PROP_OUI       "\x00\x90\x4C"



#define BRCM_OUI        "\x00\x10\x18"

//__packed struct ether_header
//{
//	uint8	ether_dhost[ETHER_ADDR_LEN];
//	uint8	ether_shost[ETHER_ADDR_LEN];
//	uint16	ether_type;
//};


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define	ETHER_MAC_LEN		6

typedef  struct _RK903_DEVICE_CLASS
{
    DEVICE_CLASS stRk903Device;
    pSemaphore osRk903OperReqSem;
    pSemaphore cmdSem;
    HDC hSdio;
    HDC hFunc[3];
    enum DHD_BUS_STATE busstate;
    wl_country_t dhd_cspec;		/* Current Locale info */
	uint maxctl;		/* Max size rxctl request from proto to bus */
    bool dhd_alignctl;
    SI_INFO * sih;
    DHD_PROT _prot_;	/* Protocol module handle */
    DHD_PROT *prot;	/* Protocol module handle */
    sk_buff _skbuftg;
    uint32  vendevid;	/* Target Vendor and Device ID on SD bus */
    uint32	sbwad;		/* Save backplane window address */
    bool	regfail;	/* Save status of last reg_read/reg_write call */
    uint8 	*vars;			/* Variables (from CIS and/or other) */
    uint32	varsz;			/* Size of variables buffer */
    uint32	armrev;			/* CPU core revision */
    uint32	orig_ramsize;	/* Size of RAM in SOCRAM (bytes) */
    uint32	ramsize;		/* Size of RAM in SOCRAM (bytes) */
    sdpcmd_regs_t	*regs;			/* Registers for SDIO core */
    uint32		sdpcmrev;		/* SDIO core revision */
    uint8		*rxhdr;			/* Header of current rx frame (in hdrbuf) */
    uint8		hdrbuf[MAX_HDR_READ + DHD_SDALIGN];
    uint8  *cur_cmd_cmdbuf;

	uint16		nextlen;		/* Next Read Len from last header */
	uint8		rx_seq;			/* Receive sequence number (expected) */
	bool		rxskip;			/* Skip receive (awaiting NAK ACK) */

	void		*glomd;			/* Packet containing glomming descriptor */
	void		*glom;			/* Packet chain for glommed superframe */
	uint		glomerr;		/* Glom packet read errors */

	uint8		*rxbuf;			/* Buffer for receiving control packets */
	uint		rxblen;			/* Allocated length of rxbuf */
	uint8		*rxctl;			/* Aligned pointer into rxbuf */
	uint8		*databuf;		/* Buffer for receiving big glom packet */
	uint8		*dataptr;		/* Aligned pointer into databuf */
	uint		rxlen;			/* Length of valid data in buffer */

	uint8		sdpcm_ver;		/* Bus protocol reported by dongle */
    bool		intr;			/* Use interrupts */
    bool		poll;			/* Use polling */
    bcmsdh_info_t *sdh;

    uint		blocksize;		/* Block size of SDIO transfers */
    uint		roundup;		/* Max roundup limit */




    bool		ipend;			/* Device interrupt is pending */
    bool		intdis;			/* Interrupts disabled by isr */
    uint 		intrcount;		/* Count of device interrupt callbacks */
    uint		lastintrs;		/* Count as of last watchdog timer */
    uint		spurious;		/* Count of spurious interrupts */
    uint		pollrate;		/* Ticks between device polls */
    uint		polltick;		/* Tick counter */
    uint		pollcnt;		/* Count of active polls */


    uint		regfails;		/* Count of R_REG/W_REG failures */

    uint		clkstate;		/* State of sd and backplane clock(s) */
    bool		activity;		/* Activity flag for clock down */
    int32		idletime;		/* Control for activity timeout */
    int32		idlecount;		/* Activity timeout counter */
    int32		idleclock;		/* How to set bus driver when idle */
    int32		sd_divisor;		/* Speed control to bus driver */
    int32		sd_mode;		/* Mode control to bus driver */
    int32		sd_rxchain;		/* If bcmsdh api accepts PKT chains */
    bool		use_rxchain;		/* If dhd should use PKT chains */
    bool		sleeping;		/* Is SDIO bus sleeping? */
    bool		rxflow_mode;	/* Rx flow control mode */
    bool		rxflow;			/* Is rx flow control on */
    uint		prev_rxlim_hit;		/* Is prev rx limit exceeded (per dpc schedule) */
    bool		alp_only;		/* Don't use HT clock (ALP only) */


    /* Field to decide if rx of control frames happen in rxbuf or lb-pool */
    bool		usebufpool;

    /* Some additional counters */
    uint		tx_sderrs;		/* Count of tx attempts with sd errors */
    uint		fcqueued;		/* Tx packets that got queued */
    uint		rxrtx;			/* Count of rtx requests (NAK to dongle) */
    uint		rx_toolong;		/* Receive frames too long to receive */
    uint		rxc_errors;		/* SDIO errors when reading control frames */
    uint		rx_hdrfail;		/* SDIO errors on header reads */
    uint		rx_badhdr;		/* Bad received headers (roosync?) */
    uint		rx_badseq;		/* Mismatched rx sequence number */
    uint		fc_rcvd;		/* Number of flow-control events received */
    uint		fc_xoff;		/* Number which turned on flow-control */
    uint		fc_xon;			/* Number which turned off flow-control */
    uint		rxglomfail;		/* Failed deglom attempts */
    uint		rxglomframes;		/* Number of glom frames (superframes) */
    uint		rxglompkts;		/* Number of packets from glom frames */
    uint		f2rxhdrs;		/* Number of header reads */
    uint		f2rxdata;		/* Number of frame data reads */
    uint		f2txdata;		/* Number of f2 frame writes */
    uint		f1regdata;		/* Number of f1 register accesses */
    uint        rx_packets;	/* Packets sent up the network interface */

    uint8		flowcontrol;		/* per prio flow control bitmask */
	uint8		tx_seq;			/* Transmit sequence number (next) */
	uint8		tx_max;			/* Maximum transmit sequence allowed */

    uint32		intstatus;		/* Intstatus bits (events) pending */
    bool		dpc_sched;		/* Indicates DPC schedule (intrpt rcvd) */
    bool		fcstate;		/* State of dongle flow-control */

    uint        rx_errors;

    /* Last error from dongle */
    int dongle_error;
    uint32		hostintmask;		/* Copy of Host Interrupt Mask */

    uint8		*ctrl_frame_buf;
    uint32		ctrl_frame_len;
    bool		ctrl_frame_stat;
    uint32		rxint_mode;	/* rx interrupt mode */
    uint8       is_get_ctrl_frame;


    uint16		cl_devid;		/* cached devid for dhdsdio_probe_attach() */
    const char      *nvram_params;		/* user specified nvram params. */

    bool up;		/* Driver up/down (to OS) */

    bool 		sd_blockmode;
    uint32 		num_funcs;		/* Supported funcs on client */
    int 		client_block_size[3];		/* Blocksize */
    bool		sd_use_dma;		/* DMA on CMD53 */
    bool 		use_client_ints;	/* If this is false, make sure to restore */
    uint16		intmask;		/* Current active interrupts */
    char        EthMac[ETHER_MAC_LEN];

    struct ether_addr mac;	/* MAC address obtained from dongle */
    int	op_mode;				/* STA, HostAPD, WFD, SoftAP */
}RK903_DEVICE_CLASS;


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
static RK903_DEVICE_CLASS * gpstRk903DevISR[RK903_DEV_CNT];


/* SDIO Drive Strength to sel value table for PMU Rev 11 (1.8v) */
static  SDIOD_DRIVE_STR_T sdiod_drive_strength_tab4_1v8[] =
{
    {32, 0x6},
    {26, 0x7},
    {22, 0x4},
    {16, 0x5},
    {12, 0x2},
    {8, 0x3},
    {4, 0x0},
    {0, 0x1}
};

/* SDIO Drive Strength to sel value table for PMU Rev 13 (1.8v) */
static  SDIOD_DRIVE_STR_T sdiod_drive_strength_tab5_1v8[] =
{
    {6, 0x7},
    {5, 0x6},
    {4, 0x5},
    {3, 0x4},
    {2, 0x2},
    {1, 0x1},
    {0, 0x0}
};

struct wifi_model_fw
{
	const char *pfirmware;
	const char *pnvram;
};

static struct wifi_model_fw wifi_info =
{
	wifi_fw,
	wifi_nvram,
};

/* IOVar table */
enum
{
	IOV_MSGLEVEL = 1,
	IOV_BLOCKMODE,
	IOV_BLOCKSIZE,
	IOV_DMA,
	IOV_USEINTS,
	IOV_NUMINTS,
	IOV_NUMLOCALINTS,
	IOV_HOSTREG,
	IOV_DEVREG,
	IOV_DIVISOR,
	IOV_SDMODE,
	IOV_HISPEED,
	IOV_HCIREGS,
	IOV_POWER,
	IOV_CLOCK,
	IOV_RXCHAIN
};

#define IOVT_VOID	0
#define IOVT_BOOL	1
#define IOVT_INT8	2
#define IOVT_UINT8	3
#define IOVT_INT16	4
#define IOVT_UINT16	5
#define IOVT_INT32	6
#define IOVT_UINT32	7
#define IOVT_BUFFER	8

#define IOV_GET 0
#define IOV_SET 1

#define IOV_GVAL(id)		((id)*2)
#define IOV_SVAL(id)		(((id)*2)+IOV_SET)
#define IOV_ISSET(actionid)	((actionid & IOV_SET) == IOV_SET)
#define IOV_ID(actionid)	(actionid >> 1)

/* private bus modes */
#define SDIOH_MODE_SD4		2
#define CLIENT_INTR 		0x100	/* Get rid of this! */

uint sd_sdmode = SDIOH_MODE_SD4;	/* Use SD4 mode by default */
uint sd_divisor = 2;			/* Default 48MHz/2 = 24MHz */

uint sd_power = 1;		/* Default to SD Slot powered ON */
uint sd_clock = 1;		/* Default to SD Clock turned ON */
uint sd_hiok = FALSE;	/* Don't use hi-speed mode by default */
uint sd_msglevel = 0;
uint sd_use_dma = TRUE;


typedef struct sdreg
{
	int func;
	int offset;
	int value;

} sdreg_t;

bcm_iovar_t sdioh_iovars[] = {
	{"sd_msglevel", 	IOV_MSGLEVEL,		0,	IOVT_UINT32,	0},
	{"sd_blockmode", 	IOV_BLOCKMODE, 		0,	IOVT_BOOL,		0},
	{"sd_blocksize", 	IOV_BLOCKSIZE, 		0,	IOVT_UINT32,	0}, /* ((fn << 16) | size) */
	{"sd_dma",			IOV_DMA,			0,	IOVT_BOOL,		0},
	{"sd_ints", 		IOV_USEINTS,		0,	IOVT_BOOL,		0},
	{"sd_numints",		IOV_NUMINTS,		0,	IOVT_UINT32,	0},
	{"sd_numlocalints", IOV_NUMLOCALINTS, 	0, 	IOVT_UINT32,	0},
	{"sd_hostreg",		IOV_HOSTREG,		0,	IOVT_BUFFER,	sizeof(sdreg_t) },
	{"sd_devreg",		IOV_DEVREG, 		0,	IOVT_BUFFER,	sizeof(sdreg_t) },
	{"sd_divisor",		IOV_DIVISOR,		0,	IOVT_UINT32,	0},
	{"sd_power",		IOV_POWER,			0,	IOVT_UINT32,	0},
	{"sd_clock",		IOV_CLOCK,			0,	IOVT_UINT32,	0},
	{"sd_mode", 		IOV_SDMODE, 		0,	IOVT_UINT32,	100},
	{"sd_highspeed", 	IOV_HISPEED,		0,	IOVT_UINT32,	0},
	{"sd_rxchain",  	IOV_RXCHAIN,    	0, 	IOVT_BOOL,		0},
	{NULL, 0, 0, 0, 0 }
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
rk_err_t Rk903DevShellHelp(HDC dev,  uint8 * pstr);
rk_err_t Rk903DevCheckApWfdModeSet(HDC dev);
rk_err_t Rk903DevKeepAliveOnoff(HDC dev);
 rk_err_t dhdsdio_hostmail(HDC dev);
rk_err_t bcmsdh_regfail(HDC dev,void *sdh);
rk_err_t RK903DevWriteSdioReg(uint32 regval,uint32*  regaddr,uint32 retryvar);
rk_err_t RK903DevReadSdioReg(uint32 regval,uint32*  regaddr, uint32 retryvar);
rk_err_t dhdsdio_dpc(HDC dev);
rk_err_t bcm_iovar_lencheck(const bcm_iovar_t *vi, void *arg, int len, bool set);
const bcm_iovar_t* bcm_iovar_lookup(const bcm_iovar_t *table, const char *name);
rk_err_t bcmsdh_iovar_op(RK903_DEVICE_CLASS * pstRk903Dev, const char *name, void *params, int plen, void *arg, int len, bool set);
void *  PKTDATA(void * skbf);
uint32 PKTLEN(void * skbf);
rk_err_t sdioh_request_packet(RK903_DEVICE_CLASS * pstRk903Dev, uint fix_inc, uint write, uint func, uint addr, void *pkt);
rk_err_t sdioh_request_packet(RK903_DEVICE_CLASS * pstRk903Dev, uint fix_inc, uint write, uint func, uint addr, void *pkt);
rk_err_t bcmsdh_rwdata(RK903_DEVICE_CLASS * pstRk903Dev, uint rw, uint32 addr, uint8 *buf, uint nbytes);
rk_err_t si_buscore_setup(RK903_DEVICE_CLASS * pstRk903Dev,  SI_INFO * sih,  uint32 savewin,  uint *origidx);
uint32 get_asd(RK903_DEVICE_CLASS * pstRk903Dev, uint32 **eromptr, uint sp, uint ad, uint st, uint32 *addrl, uint32 *addrh,
        uint32 *sizel, uint32 *sizeh);
uint32 get_erom_ent(RK903_DEVICE_CLASS * pstRk903Dev, uint32 **eromptr, uint32 mask, uint32 match);
rk_err_t ai_scan(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih);
rk_err_t si_buscore_prep(RK903_DEVICE_CLASS * pstRk903,  uint bustype);
rk_err_t dhd_preinit_ioctls(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t dhdcdc_cmplt(RK903_DEVICE_CLASS * pstRk903Dev, uint32 id, uint32 len);
rk_err_t dhd_bus_txctl(RK903_DEVICE_CLASS * pstRk903Dev, unsigned char *msg, uint msglen);
rk_err_t dhdcdc_msg(RK903_DEVICE_CLASS * pstRk903Dev);
void * PKTPULL(void * skbf, uint32 len);
void * PKTPUSH(void * skbf, uint32 len);
rk_err_t dhdcdc_send_ioctl(RK903_DEVICE_CLASS *dhd, uint cmd, void *buf, uint len, uint8 action);
rk_err_t dhd_wl_ioctl_cmd(RK903_DEVICE_CLASS * pstRk903Dev, int cmd, void *arg, int len, uint8 set);
rk_err_t dhd_bus_init(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t dhdsdio_downloadvars(RK903_DEVICE_CLASS * pstRk903Dev, void *arg, int len);
uint32 process_nvram_vars(uint8 *varbuf, uint32 len);
rk_err_t dhdsdio_download_nvram(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t dhdsdio_write_vars(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t dhdsdio_membytes(RK903_DEVICE_CLASS * pstRk903Dev, bool write, uint32 address, uint8 *data, uint size);
rk_err_t dhdsdio_download_state(RK903_DEVICE_CLASS * pstRk903Dev, bool enter);
rk_err_t _dhdsdio_download_firmware(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t dhdsdio_htclk(RK903_DEVICE_CLASS * pstRk903Dev, bool on, bool pendok);
rk_err_t dhdsdio_sdclk(RK903_DEVICE_CLASS * pstRk903Dev, bool on);
rk_err_t dhdsdio_clkctl(RK903_DEVICE_CLASS * pstRk903Dev, uint target, bool pendok);
rk_err_t dhdsdio_download_firmware(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t si_core_disable(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih, uint32 bits);
rk_err_t socram_banksize(RK903_DEVICE_CLASS * pstRk903Dev, sbsocramregs_t *regs, uint8 index, uint8 mem_type, uint32 * word);
rk_err_t ai_core_disable(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih, uint32 bits);
rk_err_t ai_core_reset(RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits);
rk_err_t si_core_reset(RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits);
uint32 ai_iscoreup(RK903_DEVICE_CLASS * pstRk903Dev);
uint32 si_iscoreup(RK903_DEVICE_CLASS * pstRk903Dev);
uint32 si_coreidx(SI_INFO * sih);
uint32 si_coreid(SI_INFO * sih);
rk_err_t si_socram_size(RK903_DEVICE_CLASS * pstRK903Device);
uint32 si_corerev(SI_INFO *sih);
rk_err_t bcmsdh_cfg_read(RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 *data);
rk_err_t bcmsdh_cfg_write(RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 data);
void * si_setcoreidx(SI_INFO *sih, uint coreidx);
rk_err_t si_restore_core(SI_INFO *sih, uint coreid, uint intr_val);
rk_err_t bcmsdh_reg_read(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size, uint32 *word);
rk_err_t sdioh_request_word(RK903_DEVICE_CLASS * pstRk903Dev, uint rw, uint func, uint addr,
                            uint32 *word, uint nbytes);
rk_err_t bcmsdh_set_sbaddr_window(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, bool force_set);
rk_err_t bcmsdh_reg_write(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size, uint32 data);
void *  ai_setcoreidx(SI_INFO *sih, uint coreidx);
uint32  si_findcoreidx(SI_INFO *sih, uint coreid, uint coreunit);
void *  si_setcore(SI_INFO *sih, uint coreid, uint coreunit);
void *  si_switch_core(SI_INFO *sih, uint coreid, uint *origidx, uint *intr_val);
rk_err_t si_sdiod_drive_strength_init(RK903_DEVICE_CLASS * pstRk903Dev, uint32 drivestrength);
rk_err_t dhdsdio_chipmatch(uint32 chipId);
SI_INFO * si_info_init(RK903_DEVICE_CLASS * pstRk903Dev, uint32 busType );
rk_err_t sdioh_request_byte(RK903_DEVICE_CLASS * pstRk903Dev, uint32 rw, uint32 func, uint32 regaddr, uint8 *byte);
rk_err_t dhdsdio_set_siaddr_window(RK903_DEVICE_CLASS * pstRk903Dev, uint32 address);
rk_err_t Rk903DevShellTest(HDC dev, uint8 * pstr);
rk_err_t Rk903DevShellDel(HDC dev, uint8 * pstr);
rk_err_t Rk903DevShellMc(HDC dev, uint8 * pstr);
rk_err_t Rk903DevShellPcb(HDC dev, uint8 * pstr);
void Rk903DevIntIsr(void);
rk_err_t Rk903DevDelete(HDC dev);
rk_err_t Rk903DevDeInit(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t Rk903DevInit(RK903_DEVICE_CLASS * pstRk903Dev);
rk_err_t Rk903DevResume(HDC dev);
rk_err_t Rk903DevSuspend(HDC dev);



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: RK903Dev_GetMac
** Input:HDC dev,char *buf
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.8.6
** Time: 8:59:12
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON API rk_err_t RK903Dev_GetMac(char *buf)
{

   RK903_DEVICE_CLASS *  pst903Dev =  (RK903_DEVICE_CLASS *)gpstRk903DevISR[0];

   if(pst903Dev != NULL)
   {
       memcpy(buf,pst903Dev->EthMac,ETHER_MAC_LEN);
       return RK_SUCCESS;
   }
   else
   {
       rk_printf("Get Mac Error------------------");
       return RK_ERROR;

   }
}

_DRIVER_RK903_RK903DEVICE_COMMON_
void * get_global_cmdbuf(uint32 pos)
{
	void * ptr = NULL;

	if (pos >= CMD_BUF_MAX)
	{
		while(1);
		return NULL;
	}

	//ptr = &cur_cmd_cmdbuf[pos];

    ptr = gpstRk903DevISR[0]->cur_cmd_cmdbuf+pos;
	  return ptr;
}

_DRIVER_RK903_RK903DEVICE_COMMON_
void * PKTGET(uint len)
{
	sk_buff * skbp;
	skbp = (sk_buff *)(&gpstRk903DevISR[0]->_skbuftg);
	memset(skbp, 0, sizeof(sk_buff));
	skbp->len = len;

	skbp->data = get_global_cmdbuf(DATA_RXBUF_HEAD);
	memset(skbp->data, 0, len);
	return skbp;
}

//void * PKTDATA(void * skbf)
//{
//	sk_buff * skbp = (sk_buff *)skbf;
//	if (!skbf)
//		return NULL;
//
//	return skbp->data;
//}
//uint PKTLEN(void * skbf)
//{
//	sk_buff * skbp = (sk_buff *)skbf;
//	if (!skbf)
//		return 0;
//
//	return skbp->len;
//}

_DRIVER_RK903_RK903DEVICE_COMMON_
uint PKTSETLEN(void * skbf, unsigned int len)
{
    sk_buff * skbp = (sk_buff *)skbf;
    if (!skbf)
		return 0;
	skbp->len = len;

    return len;
}


#define PKTALIGN(p, len, align)		\
	do {								\
		uint datalign;						\
		uint *datalignP;                    \
		datalignP = PKTDATA((p));           \
		datalign = (uint)datalignP;		    \
		datalign = ROUNDUP(datalign, (align)) - datalign;	\
		ASSERT(datalign < (align));				\
		ASSERT(PKTLEN((p)) >= ((len) + datalign));	\
		if (datalign)						\
			PKTPULL((p), datalign);			\
		PKTSETLEN((p), (len));				\
	} while (0)

//void * PKTPUSH(void * skbf, unsigned int len)
//{
//    sk_buff * skbp = (sk_buff *)skbf;
//    if (!skbf)
//		return 0;
//	skbp->data = (uint8 *)(skbp->data) - len;
//	skbp->len  += len;

//	return skbp->data;
//}

//void * PKTPULL(void * skbf, unsigned int len)
//{
//    sk_buff * skbp = (sk_buff *)skbf;
//    if (!skbf)
//		return 0;
//	skbp->data = (uint8 *)(skbp->data) + len;
//	skbp->len -= len;

//	return skbp->data;
//}


void PKTFREE(void * skbf)
{

}



/*******************************************************************************
** Name: Rk903Dev_Read
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON API rk_err_t Rk903Dev_Read(HDC dev)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(read) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: Rk903DevCheckApWfdModeSet
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.23
** Time: 10:36:30
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t Rk903DevCheckApWfdModeSet(HDC dev)
{
   RK903_DEVICE_CLASS *  pst903Dev =  (RK903_DEVICE_CLASS *)dev;
   if (((pst903Dev->op_mode & HOSTAPD_MASK) == HOSTAPD_MASK) ||
		((pst903Dev->op_mode & WFD_MASK) == WFD_MASK))
		return TRUE;
	else
		return FALSE;
}
/*******************************************************************************
** Name: Rk903DevKeepAliveOnoff
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.23
** Time: 10:18:25
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t Rk903DevKeepAliveOnoff(HDC dev)
{

	char 				buf[256];
	const char 			*str;
	wl_mkeep_alive_pkt_t	mkeep_alive_pkt;
	wl_mkeep_alive_pkt_t	*mkeep_alive_pktp;
	int					buf_len;
	int					str_len;
	int res 				= -1;

	if (Rk903DevCheckApWfdModeSet(dev) == TRUE)
		return (res);

	str = "mkeep_alive";
	str_len = strlen(str);
	strncpy(buf, str, str_len);
	buf[ str_len ] = '\0';
	mkeep_alive_pktp = (wl_mkeep_alive_pkt_t *) (buf + str_len + 1);
	mkeep_alive_pkt.period_msec = KEEP_ALIVE_PERIOD;
	buf_len = str_len + 1;
	mkeep_alive_pkt.version = WL_MKEEP_ALIVE_VERSION;
	mkeep_alive_pkt.length =  WL_MKEEP_ALIVE_FIXED_LEN;
	/* Setup keep alive zero for null packet generation */
	mkeep_alive_pkt.keep_alive_id = 0;
	mkeep_alive_pkt.len_bytes = 0;
	buf_len += WL_MKEEP_ALIVE_FIXED_LEN;
	/* Keep-alive attributes are set in local	variable (mkeep_alive_pkt), and
	 * then memcpy'ed into buffer (mkeep_alive_pktp) since there is no
	 * guarantee that the buffer is properly aligned.
	 */
	memcpy((char *)mkeep_alive_pktp, &mkeep_alive_pkt, WL_MKEEP_ALIVE_FIXED_LEN);

	res = dhd_wl_ioctl_cmd(dev, WLC_SET_VAR, buf, buf_len, 1);

	return res;
}
/*******************************************************************************
** Name: dhdsdio_hostmail
** Input:HDC dev
** Return:  rk_err_t
** Owner:zhuzhe
** Date: 2014.7.15
** Time: 16:01:52
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN  rk_err_t dhdsdio_hostmail(HDC dev)
{
    RK903_DEVICE_CLASS * pstRK903Dev =  (RK903_DEVICE_CLASS *)dev;
    bcmsdh_info_t *sdh = pstRK903Dev->sdh;
    sdpcmd_regs_t *regs = pstRK903Dev->regs;
	uint32 intstatus = 0;
	uint32 hmb_data;
	uint8 fcbits;
	uint retries = 0;

	/* Read mailbox data and ack that we did so */
	//RK903DevReadSdioReg(hmb_data, &regs->tohostmailboxdata, retries);
    R_SDREG(hmb_data, &regs->tohostmailboxdata, retries);
	//hmb_data = 0x40008. hmb_data = 0x40002.

    rk_printf("dhdsdio_hostmail=0x%04x.\n", hmb_data);
	if (retries <= 2)
	{
		//RK903DevWriteSdioReg(SMB_INT_ACK, &regs->tosbmailbox, retries);

        W_SDREG(SMB_INT_ACK, &regs->tosbmailbox, retries);
	}
	pstRK903Dev->f1regdata += 2;

	/* Dongle recomposed rx frames, accept them again */
	if (hmb_data & HMB_DATA_NAKHANDLED)
	{
		pstRK903Dev->rxskip = FALSE;
		//intstatus |= FRAME_AVAIL_MASK(dev);  will write later
	}

	/*
	 * Flow Control has been moved into the RX headers and this out of band
	 * method isn't used any more.  Leave this here for possibly remaining backward
	 * compatible with older dongles
	 */
	if (hmb_data & HMB_DATA_FC)
	{
		fcbits = (hmb_data & HMB_DATA_FCDATA_MASK) >> HMB_DATA_FCDATA_SHIFT;

		if (fcbits & ~pstRK903Dev->flowcontrol)
			pstRK903Dev->fc_xoff++;
		if (pstRK903Dev->flowcontrol & ~fcbits)
			pstRK903Dev->fc_xon++;

		pstRK903Dev->fc_rcvd++;
		pstRK903Dev->flowcontrol = fcbits;
	}

#if 0
	/* Shouldn't be any others */
	if (hmb_data & ~(HMB_DATA_DEVREADY |
	                 HMB_DATA_FWHALT |
	                 HMB_DATA_NAKHANDLED |
	                 HMB_DATA_FC |
	                 HMB_DATA_FWREADY |
	                 HMB_DATA_FCDATA_MASK |
	                 HMB_DATA_VERSION_MASK))
	{

	}
#endif
	return intstatus;
}
/*******************************************************************************
** Name: bcmsdh_regfail
** Input:void *sdh
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.15
** Time: 15:07:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_regfail(HDC dev,void *sdh)
{
    RK903_DEVICE_CLASS * pstRK903Dev =  (RK903_DEVICE_CLASS *)dev;

    return pstRK903Dev->regfail;
	//if (!bcmsdh)
	//	bcmsdh = l_bcmsdh;

	//return bcmsdh->regfail;
}
/*******************************************************************************
** Name: RK903DevWriteSdioReg
** Input:uint regval,uint  regaddr,uint retryvar
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.15
** Time: 14:45:21
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t RK903DevWriteSdioReg(uint32 regval,uint32*  regaddr,uint32 retryvar)
{

}
/*******************************************************************************
** Name: RK903DevReadSdioReg
** Input:uint regval, uint regaddr, uint retryvar
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.15
** Time: 14:40:03
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t RK903DevReadSdioReg(uint32 regval, uint32* regaddr, uint32 retryvar)
{

}



/******************************************************************************
 * bcmsdh_recv_buf -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17  written
 * Time : 14:28:17
 * --------------------
 ******************************************************************************/
	_DRIVER_RK903_RK903DEVICE_COMMON_
int bcmsdh_recv_buf(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint fn, uint flags, uint8 *buf, uint nbytes)
{
	//bcmsdh_info_t *bcmsdh = (bcmsdh_info_t *)sdh;
	int status;
	uint incr_fix;
	uint width;
	int err = 0;

	if (flags & SDIO_REQ_ASYNC)
		return BCME_UNSUPPORTED;

   	err = bcmsdh_set_sbaddr_window(pstRk903Dev, addr, FALSE);
	if (err)
	{
		return err;
	}

	addr &= SBSDIO_SB_OFT_ADDR_MASK;
	incr_fix = (flags & SDIO_REQ_FIXED) ? SDIOH_DATA_FIX : SDIOH_DATA_INC;
	width = (flags & SDIO_REQ_4BYTE) ? 4 : 2;
	if (width == 4)
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;

	status = sdioh_request_buffer(pstRk903Dev,incr_fix,
	                              SDIOH_READ, fn, addr,nbytes, buf);

	return (SDIOH_API_SUCCESS(status) ? 0 : BCME_SDIO_ERROR);
}


/******************************************************************************
 * dhdsdio_rxfail -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17  written
 * Time : 14:28:24
 * --------------------
 ******************************************************************************/
	_DRIVER_RK903_RK903DEVICE_COMMON_
static void dhdsdio_rxfail(RK903_DEVICE_CLASS * pstRk903Dev, bool abort, bool rtx)
{
	//bcmsdh_info_t *sdh = bus->sdh;
	sdpcmd_regs_t *regs = pstRk903Dev->regs;
	uint retries = 0;
	uint16 lastrbc;
	uint8 hi, lo;
	int err;

	if (abort)
	{
		bcmsdh_abort(pstRk903Dev, SDIO_FUN_2);
	}

	bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_FRAMECTRL , SFC_RF_TERM);
	pstRk903Dev->f1regdata++;

	/* Wait until the packet has been flushed (device/FIFO stable) */
	for (lastrbc = retries = 0xffff; retries > 0; retries--)
	{
		hi = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_RFRAMEBCHI, NULL);
		lo = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_RFRAMEBCLO, NULL);
		pstRk903Dev->f1regdata += 2;

		if ((hi == 0) && (lo == 0))
			break;

		if ((hi > (lastrbc >> 8)) && (lo > (lastrbc & 0x00ff)))
		{
			;
		}
		lastrbc = (hi << 8) + lo;
	}

	if (rtx)
	{
		pstRk903Dev->rxrtx++;
		W_SDREG(SMB_NAK, &regs->tosbmailbox, retries);
		pstRk903Dev->f1regdata++;
		if (retries <= retry_limit)
		{
			pstRk903Dev->rxskip = TRUE;
		}
	}

	/* Clear partial in any case */
	pstRk903Dev->nextlen = 0;

	/* If we can't reach the device, signal failure */
	if (err || bcmsdh_regfail(pstRk903Dev, NULL))
	{
    	pstRk903Dev->busstate = DHD_BUS_DOWN;
	}
}

/*
dgl:the start position of receive must be align 32,
	as we had sure is in rxbuf = get_global_cmdbuf(DATA_RXBUF_HEAD);
	so here clear the dhd_alignctl,for programe run faster.
*/

/******************************************************************************
 * dhdsdio_read_control -
 * DESCRIPTION: - the start position of receive must be align 32,
 *	as we had sure is in rxbuf = get_global_cmdbuf(DATA_RXBUF_HEAD);
 *	so here clear the dhd_alignctl,for programe run faster.
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17  written
 * Time : 14:58:33
 * --------------------
 ******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
static void dhdsdio_read_control(RK903_DEVICE_CLASS * pstRk903Dev, uint8 *hdr, uint len, uint doff)
{
	//bcmsdh_info_t *sdh = bus->sdh;
	uint rdlen, pad;
	int sdret;

	/* Set rxctl for frame (w/optional alignment) */
	pstRk903Dev->rxctl = pstRk903Dev->rxbuf;//bus->rxbuf = get_global_cmdbuf(DATA_RXBUF_HEAD);
	if (firstread % DHD_SDALIGN)
	{
		while(1);
	}

	/* Copy the already-read portion over */
	memcpy(pstRk903Dev->rxctl, hdr, firstread);
	if (len <= firstread)
		goto gotpkt;

	/* Raise rdlen to next SDIO block to avoid tail command */
	rdlen = len - firstread;
	if (pstRk903Dev->roundup && pstRk903Dev->blocksize && (rdlen > pstRk903Dev->blocksize))
	{
		pad = pstRk903Dev->blocksize - (rdlen % pstRk903Dev->blocksize);
		if ((pad <= pstRk903Dev->roundup) && (pad < pstRk903Dev->blocksize) && ((len + pad) < pstRk903Dev->maxctl))
		{
			rdlen += pad;
		}
	}
	else if (rdlen % DHD_SDALIGN)
	{
		rdlen += DHD_SDALIGN - (rdlen % DHD_SDALIGN);
	}

	/* Satisfy length-alignment requirements */
	//if (forcealign && (rdlen & (ALIGNMENT - 1)))
	if (forcealign && (rdlen & (ALIGNMENT - 1)))
	{
		rdlen = ROUNDUP(rdlen, ALIGNMENT);
	}

	/* Drop if the read is too big or it exceeds our maximum */
	if ((rdlen + firstread) > pstRk903Dev->maxctl)
	{
		rk_printf("%d ex %d buf\n", rdlen, pstRk903Dev->maxctl);
		pstRk903Dev->rx_errors++;
		dhdsdio_rxfail(pstRk903Dev, FALSE, FALSE);
		goto done;
	}

	/* Read remainder of frame body into the rxctl buffer */
	sdret = bcmsdh_recv_buf(pstRk903Dev, pstRk903Dev->sbwad, SDIO_FUN_2, F2SYNC, pstRk903Dev->rxctl+firstread, rdlen);
	pstRk903Dev->f2rxdata++;

	/* Control frame failures need retransmission */
	if (sdret < 0)
	{
		rk_printf("read %d control bytes failed: %d\n",  rdlen, sdret);
		pstRk903Dev->rxc_errors++; /* dhd.rx_ctlerrs is higher level */
		dhdsdio_rxfail(pstRk903Dev, TRUE, TRUE);
		goto done;
	}

gotpkt:
	/* Point to valid data and indicate its length */
	pstRk903Dev->rxctl += doff;//move to the header of "cdc_ioctrl_t + msg" postion.
	pstRk903Dev->rxlen = len - doff;

    //pstRk903Dev->is_get_ctrl_frame = 1;

    rkos_semaphore_give(pstRk903Dev->cmdSem);
done:
	return;
}

_DRIVER_RK903_RK903DEVICE_COMMON_
void wl_iw_event(RK903_DEVICE_CLASS * pstRk903Dev, wl_event_msg_t *e, void* data)
{
	uint32 reason = ntoh32(e->reason);
	uint32 event_type = ntoh32(e->event_type);
	uint16 flags =  ntoh16(e->flags);

    rk_printf("rk903 event is %d",event_type);
	switch (event_type)
	{
		case WLC_E_LINK:// 16 =
		    rk_printf("E LINK");
		    break;
		case WLC_E_NDIS_LINK:// 18
			if (!(flags & WLC_EVENT_MSG_LINK))
			{
				rk_printf("Link Down Event %d,reason=0x%08x\n", event_type, reason);
				//MSG_CLR_WPA_SH();
			}
			else
			{
				rk_printf("Link UP reason=0x%08x\n", reason);
			}
			break;

	    case WLC_E_ESCAN_RESULT:// 69
			//if (ScanResultEventEnable)
			{
		        WlcDev_EscanHandle(e, data);
			}
	        break;

		default:
			rk_printf("E %d\n", event_type);
			break;
	}

	return;
}

#define DOT11_OUI_LEN           3

_DRIVER_RK903_RK903DEVICE_COMMON_
int wl_host_event(RK903_DEVICE_CLASS * pstRk903Dev, int *ifidx, void *pktdata,
                     wl_event_msg_t *event, void **data_ptr)
{

    bcm_event_t *pvt_data = (bcm_event_t *)pktdata;
	uint8 *event_data;
	uint32 type, status, reason, datalen;
	uint16 flags;
	int evlen;

	if (bcmp(BRCM_OUI, (const void *)&pvt_data->bcm_hdr.oui[0], DOT11_OUI_LEN))
	{
		rk_printf("mismatched OUI, bailing\n");
		return (BCME_ERROR);
	}

	/* BRCM event pkt may be unaligned - use xxx_ua to load user_subtype. */
	if (ntoh16_ua((void *)&pvt_data->bcm_hdr.usr_subtype) != BCMILCP_BCM_SUBTYPE_EVENT)
	{
		rk_printf("mismatched subtype, bailing\n");
		return (BCME_ERROR);
	}

	*data_ptr = &pvt_data[1];
	event_data = *data_ptr;

	/* memcpy since BRCM event pkt may be unaligned. */
	memcpy(event, &pvt_data->event, sizeof(wl_event_msg_t));

	type = ntoh32_ua((void *)&event->event_type);
	flags = ntoh16_ua((void *)&event->flags);
	status = ntoh32_ua((void *)&event->status);
	reason = ntoh32_ua((void *)&event->reason);
	datalen = ntoh32_ua((void *)&event->datalen);
	evlen = datalen + sizeof(bcm_event_t);

	switch (type)
	{
		case WLC_E_IF://54
	    {
			//dhd_if_event_t *ifevent = (dhd_if_event_t *)event_data;
			//pstRk903Dev->ifIdx = ifevent->ifidx;
	        break;
		}

		case WLC_E_NDIS_LINK://18
	    {
			uint32 temp;

			temp = ntoh32(WLC_E_NDIS_LINK);
			memcpy((void *)(&pvt_data->event.event_type), &temp, sizeof(pvt_data->event.event_type));
			break;
		}

		case WLC_E_DISASSOC_IND://12
		{
			//ReTryAssocAP = 1;
			break;
		}

		default:
		{
			break;
		}
	}
	return (BCME_OK);
}

_DRIVER_RK903_RK903DEVICE_COMMON_
static int dhd_wl_host_event(RK903_DEVICE_CLASS * pstRk903Dev, int *ifidx, void *pktdata, wl_event_msg_t *event, void **data)
{
	int bcmerror = 0;

	bcmerror = wl_host_event(pstRk903Dev, ifidx, pktdata, event, data);
	if (bcmerror != BCME_OK)
	{
		return bcmerror;
	}

	if (event->bsscfgidx == 0)
	{
		/*
		 * Wireless ext is on primary interface only
		 */
		wl_iw_event(pstRk903Dev, event, *data);
	}
	return (bcmerror);
}


/******************************************************************************
 * dhd_rx_frame -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17  written
 * Time : 15:11:27
 * --------------------
 ******************************************************************************/

#define BT_SIG_SNAP_MPROT		"\xAA\xAA\x03\x00\x19\x58"

_DRIVER_RK903_RK903DEVICE_COMMON_
void dhd_rx_frame(RK903_DEVICE_CLASS * pstRk903Dev, int ifidx, void *pktbuf, int numpkt, uint8 chan)
{
    void *data;
    wl_event_msg_t event;
    int tout = DHD_PACKET_TIMEOUT_MS;
    struct ether_header *eh;
    struct dot11_llc_snap_header *lsh;

    eh = (struct ether_header *)PKTDATA(pktbuf);
    lsh = (struct dot11_llc_snap_header *)&eh[1];

    rk_printf("rx_frame------------");
    /* Get the protocol, maintain skb around eth_type_trans()
     * The main reason for this hack is for the limitation of
     * Linux 2.4 where 'eth_type_trans' uses the 'net->hard_header_len'
     * to perform skb_pull inside vs ETH_HLEN. Since to avoid
     * coping of the packet coming from the network stack to add
     * BDC, Hardware header etc, during network interface registration
     * we set the 'net->hard_header_len' to ETH_HLEN + extra space required
     * for BDC, Hardware header etc. and not just the ETH_HLEN
     */

	/* Strip header, count, deliver upward */
    /* Process special event packets and then discard them */
    if (ntoh16(eh->ether_type) == ETHER_TYPE_BRCM)
	{
    	dhd_wl_host_event(pstRk903Dev, &ifidx, PKTDATA(pktbuf), &event, &data);
    }
    else
    {
        if ((ntoh16(eh->ether_type) == ETHER_TYPE_IP) || (ntoh16(eh->ether_type) == ETHER_TYPE_ARP))
        {
            //lwip_data_input(PKTDATA(pktbuf), PKTLEN(pktbuf));
            rk_printf("lwip data***********************");
            Web_NetifRx(NULL,PKTDATA(pktbuf), PKTLEN(pktbuf));
        }
        else if (ntoh16(eh->ether_type) == ETHER_TYPE_802_1X)
        {
        	#ifdef WPA_AUTH
            rk_printf("wpa data------------------------");
			WlcDev_80211DataInput(PKTDATA(pktbuf), PKTLEN(pktbuf));
			//rk_80211_data_input(PKTDATA(pktbuf), PKTLEN(pktbuf));
        	#endif
        }
		else
		{
			rk_printf("err ft=%d\n", ntoh16(eh->ether_type));
		}
    }
    pstRk903Dev->rx_packets++; /* Local count */

	return;
}

/******************************************************************************
 * dhd_prot_hdrpull -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17
 * Time : 15:03:13
 * --------------------
 ******************************************************************************/
	_DRIVER_RK903_RK903DEVICE_COMMON_
int dhd_prot_hdrpull(RK903_DEVICE_CLASS * pstRk903Dev, int *ifidx, void *pktbuf)
{
	struct bdc_header *h;

	/* Pop BDC header used to convey priority for buses that don't */
	if (PKTLEN(pktbuf) < BDC_HEADER_LEN)
	{
		rk_printf(" rx data too short (%d < %d)\n",PKTLEN(pktbuf), BDC_HEADER_LEN);
		return BCME_ERROR;
	}

	h = (struct bdc_header *)PKTDATA(pktbuf);
	if ((*ifidx = BDC_GET_IF_IDX(h)) >= DHD_MAX_IFS)
	{
		rk_printf("rx data ifnum out of range (%d)\n", *ifidx);
		return BCME_ERROR;
	}

	if (((h->flags & BDC_FLAG_VER_MASK) >> BDC_FLAG_VER_SHIFT) != BDC_PROTO_VER)
	{
		rk_printf(" non-BDC packet received, flags = 0x%x\n", h->flags);
		if (((h->flags & BDC_FLAG_VER_MASK) >> BDC_FLAG_VER_SHIFT) == BDC_PROTO_VER_1)
			h->dataOffset = 0;
		else
			return BCME_ERROR;
	}

	if (h->flags & BDC_FLAG_SUM_GOOD)
	{
		rk_printf("BDC packet received with good rx-csum, flags 0x%x\n", h->flags);
	}

	PKTPULL(pktbuf, BDC_HEADER_LEN);

	if (PKTLEN(pktbuf) < (uint32)(h->dataOffset << 2))
	{
		rk_printf("rx data too short (%d < %d)\n",PKTLEN(pktbuf), (h->dataOffset * 4));
		return BCME_ERROR;
	}

	PKTPULL( pktbuf, (h->dataOffset << 2));
	return 0;
}

_DRIVER_RK903_RK903DEVICE_COMMON_
static uint dhdsdio_readframes(RK903_DEVICE_CLASS * pstRk903Dev, uint maxframes, bool *finished)
{
	//bcmsdh_info_t *sdh = bus->sdh;
	uint16 len, check;	/* Extracted hardware header fields */
	uint8 chan, seq, doff;	/* Extracted software header fields */
	uint8 fcbits;		/* Extracted fcbits from software header */
	uint8 delta;
	void *pkt = NULL;	/* Packet for event or data frames */
	uint16 pad;	/* Number of pad bytes to read */
	uint16 rdlen;	/* Total number of bytes to read */
	uint8 rxseq;	/* Next sequence number to expect */
	uint rxleft = 0;	/* Remaining number of frames allowed */
	int sdret;	/* Return code from bcmsdh calls */
	uint8 txmax;	/* Maximum tx sequence offered */
	int ifidx = 0;
	uint rxcount = 0; /* Total frames read */

	//ASSERT(maxframes);

	/* Not finished unless we encounter no more frames indication */
	*finished = FALSE;

	for (rxseq = pstRk903Dev->rx_seq, rxleft = maxframes;
	     !pstRk903Dev->rxskip && rxleft && pstRk903Dev->busstate != DHD_BUS_DOWN;
	     rxleft--)
	{
		/* Read frame header (hardware and software) */
		sdret = bcmsdh_recv_buf(pstRk903Dev, pstRk903Dev->sbwad, SDIO_FUN_2, F2SYNC, pstRk903Dev->rxhdr, firstread);

		pstRk903Dev->f2rxhdrs++;

		if (sdret < 0)
		{
			rk_printf(" RXHEADER FAILED: %d\n",sdret);
			pstRk903Dev->rx_hdrfail++;
			dhdsdio_rxfail(pstRk903Dev, TRUE, TRUE);
			continue;
		}

		/* Extract hardware header fields */
		len = ltoh16_ua(pstRk903Dev->rxhdr);
		check = ltoh16_ua(pstRk903Dev->rxhdr + sizeof(uint16));
		/* All zeros means no more frames */
		if (!(len|check))
		{
			*finished = TRUE;
			break;
		}

		/* Validate check bytes */
		if ((uint16)~(len^check))
		{
			rk_printf(" HW hdr error: len/check 0x%04x/0x%04x\n", len, check);
			pstRk903Dev->rx_badhdr++;
			dhdsdio_rxfail(pstRk903Dev, FALSE, FALSE);
			continue;
		}

		/* Validate frame length */
		if (len < SDPCM_HDRLEN)
		{
			rk_printf(" HW hdr length invalid: %d\n",  len);
			continue;
		}

		/* Extract software header fields */
		chan = SDPCM_PACKET_CHANNEL(&pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN]);
		seq = SDPCM_PACKET_SEQUENCE(&pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN]);
		doff = SDPCM_DOFFSET_VALUE(&pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN]);
		txmax = SDPCM_WINDOW_VALUE(&pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN]);

        //rk_printf("ReadfFames chan = %d, seq = %d, doff = %d, txmax = %d\n", chan, seq, doff, txmax);
		/* Validate data offset */
		if ((doff < SDPCM_HDRLEN) || (doff > len))
		{
			rk_printf(" Bad data offset %d: HW len %d, min %d seq %d\n", doff, len, SDPCM_HDRLEN, seq);
			pstRk903Dev->rx_badhdr++;

			dhdsdio_rxfail(pstRk903Dev, FALSE, FALSE);
			continue;
		}

		/* Save the readahead length if there is one */
		pstRk903Dev->nextlen = pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN + SDPCM_NEXTLEN_OFFSET];
		if ((pstRk903Dev->nextlen << 4) > MAX_RX_DATASZ)
		{
			rk_printf("(nextlen): (%d), seq %d\n",pstRk903Dev->nextlen, seq);
			pstRk903Dev->nextlen = 0;
		}

		/* Handle Flow Control */
		fcbits = SDPCM_FCMASK_VALUE(&pstRk903Dev->rxhdr[SDPCM_FRAMETAG_LEN]);

		delta = 0;
		if (~pstRk903Dev->flowcontrol & fcbits)
		{
			pstRk903Dev->fc_xoff++;
			delta = 1;
		}
		if (pstRk903Dev->flowcontrol & ~fcbits)
		{
			pstRk903Dev->fc_xon++;
			delta = 1;
		}

		if (delta)
		{
			pstRk903Dev->fc_rcvd++;
			pstRk903Dev->flowcontrol = fcbits;
		}

		/* Check and update sequence number */
		if (rxseq != seq)
		{
			rk_printf("rx_seq %d, expected %d\n", seq, rxseq);
			pstRk903Dev->rx_badseq++;
			rxseq = seq;
		}
        else
        {
            rxseq ++;

        }

		/* Check window for sanity */
		if ((uint8)(txmax - pstRk903Dev->tx_seq) > 0x40)
		{
			rk_printf("got unlikely tx max %d with tx_seq %d\n", txmax, pstRk903Dev->tx_seq);
			txmax = pstRk903Dev->tx_max;
		}
		pstRk903Dev->tx_max = txmax;

		/* Call a separate function for control frames */
		if (chan == SDPCM_CONTROL_CHANNEL)
		{
			dhdsdio_read_control(pstRk903Dev, pstRk903Dev->rxhdr, len, doff);
			break;//continue;
		}

		ASSERT((chan == SDPCM_EVENT_CHANNEL) || (chan == SDPCM_DATA_CHANNEL) ||
		       (chan == SDPCM_GLOM_CHANNEL)  || (chan == SDPCM_TEST_CHANNEL));
		//if (wpadebugen)printf("chan\n");
		/* Length to read */
		rdlen = (len > firstread) ? (len - firstread) : 0;

		/* May pad read to blocksize for efficiency */
		if (pstRk903Dev->roundup && pstRk903Dev->blocksize && (rdlen > pstRk903Dev->blocksize))
		{
			pad = pstRk903Dev->blocksize - (rdlen % pstRk903Dev->blocksize);
			if ((pad <= pstRk903Dev->roundup) && (pad < pstRk903Dev->blocksize) && ((rdlen + pad + firstread) < MAX_RX_DATASZ))
			{
				rdlen += pad;
			}
		}
		else if (rdlen % DHD_SDALIGN)
		{
			rdlen += DHD_SDALIGN - (rdlen % DHD_SDALIGN);
		}

		/* Satisfy length-alignment requirements */
		//if (forcealign && (rdlen & (ALIGNMENT - 1)))
		if (1 && (rdlen & (ALIGNMENT - 1)))
		{
			rdlen = ROUNDUP(rdlen, ALIGNMENT);
		}

		if ((rdlen + firstread) > MAX_RX_DATASZ)
		{
			/* Too long -- skip this frame */
			rk_printf("too long: len %d rdlen %d\n",len, rdlen);
			pstRk903Dev->rx_errors++;
			pstRk903Dev->rx_toolong++;
			dhdsdio_rxfail(pstRk903Dev, FALSE, FALSE);
			continue;
		}
		/*mallco the receive buffer for data...*/
		pkt = PKTGET(rdlen + firstread + DHD_SDALIGN);

		/* Leave room for what we already read, and align remainder */
		PKTPULL(pkt, firstread);
		PKTALIGN(pkt, rdlen, DHD_SDALIGN);
		/* Read the remaining frame data */
	    sdret = bcmsdh_recv_buf(pstRk903Dev, pstRk903Dev->sbwad, SDIO_FUN_2, F2SYNC, PKTDATA(pkt), rdlen);
		pstRk903Dev->f2rxdata++;
		ASSERT(sdret != BCME_PENDING);

		if (sdret < 0)
		{
			rk_printf("read %d %s bytes failed: %d\n",rdlen,
			           ((chan == SDPCM_EVENT_CHANNEL) ? "event" :
			            ((chan == SDPCM_DATA_CHANNEL) ? "data" : "test")), sdret);

			pstRk903Dev->rx_errors++;
			dhdsdio_rxfail(pstRk903Dev, TRUE, RETRYCHAN(chan));
			continue;
		}

		/* Copy the already-read portion */
		PKTPUSH(pkt, firstread);
		bcopy(pstRk903Dev->rxhdr, PKTDATA(pkt), firstread);

		/* Fill in packet len and prio, deliver upward */
		PKTSETLEN(pkt, len);//dgl set the mymsg really length...
		PKTPULL(pkt, doff); //move to valid data.
		//if (wpadebugen)printf("chan2\n");
		if (PKTLEN(pkt) == 0)
		{
			PKTFREE(pkt);
			continue;
		}
		else if (dhd_prot_hdrpull(pstRk903Dev, &ifidx, pkt) != 0)
		{
			rk_printf("rx protocol error\n");
			PKTFREE(pkt);
			pstRk903Dev->rx_errors++;
			continue;
		}
		/* Unlock during rx call */
		dhd_rx_frame(pstRk903Dev, ifidx, pkt, 1, chan);
        PKTFREE(pkt);
	}
	rxcount = maxframes - rxleft;
	//printf("processed %d frames\n", rxcount);

	/* Back off rxseq if awaiting rtx, update rx_seq */
	if (pstRk903Dev->rxskip)
		rxseq--;
	pstRk903Dev->rx_seq = rxseq;
	//rk_printf("the rx seq is%d ",rxseq);
	return rxcount;
}
/*******************************************************************************
** Name: dhdsdio_dpc
** Input:HDC dev
** Return: rk_err_t
** Owner:zhuzhe
** Date: 2014.7.15
** Time: 11:45:07
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_dpc(HDC dev)
{
    RK903_DEVICE_CLASS * pstRK903Dev =  (RK903_DEVICE_CLASS *)dev;
    bcmsdh_info_t *sdh = pstRK903Dev->sdh;
	sdpcmd_regs_t *regs = pstRK903Dev->regs;

	uint32 intstatus, newstatus = 0;
	uint retries = 0;
	uint rxlimit = DHD_RXBOUND; /* Rx frames to read before resched */
	uint txlimit = DHD_TXBOUND; /* Tx frames to send before resched */
	uint framecnt = 0;		  /* Temporary counter of tx/rx frames */
	bool rxdone = TRUE;		  /* Flag for no more read data */
	bool resched = FALSE;	  /* Flag indicating resched wanted */


	if (pstRK903Dev->busstate == DHD_BUS_DOWN)
	{
		pstRK903Dev->busstate = 0;
		return 0;
	}

	/* Start with leftover status bits */
	intstatus = pstRK903Dev->busstate;

	/* If waiting for HTAVAIL, check status */
	if (pstRK903Dev->clkstate == CLK_PENDING)
	{
		int err;
		uint8 clkctl, devctl = 0;

		/* Read CSR, if clock on switch to AVAIL, else ignore */
		clkctl = bcmsdh_cfg_read(dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, (uint8*)&err);
		if (err)
		{
			pstRK903Dev->busstate = DHD_BUS_DOWN;
		}

		if (SBSDIO_HTAV(clkctl))
		{
			devctl = bcmsdh_cfg_read(dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL,  (uint8*)&err);
			if (err)
			{
				pstRK903Dev->busstate = DHD_BUS_DOWN;
			}
			devctl &= ~SBSDIO_DEVCTL_CA_INT_ONLY;
			bcmsdh_cfg_write(dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL,devctl);
			if (err)
			{
				pstRK903Dev->busstate = DHD_BUS_DOWN;
			}
			pstRK903Dev->clkstate = CLK_AVAIL;
		}
		else
		{
			goto clkwait;
		}
	}

	/* Make sure backplane clock is on */
	dhdsdio_clkctl(dev, CLK_AVAIL, TRUE);
	if (pstRK903Dev->clkstate != CLK_AVAIL)
	{
		goto clkwait;
	}

	/* Pending interrupt indicates new device status */
	if (pstRK903Dev->ipend)
	{
		pstRK903Dev->ipend = FALSE;
		//RK903DevReadSdioReg(newstatus, &regs->intstatus, retries);

        R_SDREG(newstatus, &regs->intstatus, retries);
		pstRK903Dev->f1regdata++;

		if (bcmsdh_regfail(pstRK903Dev,pstRK903Dev->sdh))
			newstatus = 0;

		newstatus &= pstRK903Dev->hostintmask;
		pstRK903Dev->fcstate = !!(newstatus & I_HMB_FC_STATE);
		if (newstatus)
		{
			pstRK903Dev->f1regdata++;
			//RK903DevWriteSdioReg(newstatus, &regs->intstatus, retries);

            W_SDREG(newstatus, &regs->intstatus, retries);
		}
	}

	/* Merge new bits with previous */
	intstatus |= newstatus;
	pstRK903Dev->intstatus = 0;

	//BCMDEBUG("intstatus = 0x%04x", intstatus);
	/* Handle flow-control change: read new state in case our ack
	 * crossed another change interrupt.  If change still set, assume
	 * FC ON for safety, let next loop through do the debounce.
	 */
	if (intstatus & I_HMB_FC_CHANGE)
	{
		intstatus &= ~I_HMB_FC_CHANGE;
		//RK903DevWriteSdioReg(I_HMB_FC_CHANGE, &regs->intstatus, retries);
		//RK903DevReadSdioReg(newstatus, &regs->intstatus, retries);

        W_SDREG(I_HMB_FC_CHANGE, &regs->intstatus, retries);
		R_SDREG(newstatus, &regs->intstatus, retries);
		pstRK903Dev->f1regdata += 2;
		pstRK903Dev->fcstate = !!(newstatus & (I_HMB_FC_STATE | I_HMB_FC_CHANGE));
		intstatus |= (newstatus & pstRK903Dev->hostintmask);
	}

	/* Just being here means nothing more to do for chipactive */
	if (intstatus & I_CHIPACTIVE)
	{
		/* ASSERT(bus->clkstate == CLK_AVAIL); */
		intstatus &= ~I_CHIPACTIVE;
	}

	/* Handle host mailbox indication */
	if (intstatus & I_HMB_HOST_INT)
	{
		intstatus &= ~I_HMB_HOST_INT;
		intstatus |= dhdsdio_hostmail(dev);
	}

	/* Generally don't ask for these, can get CRC errors... */
	if (intstatus & I_WR_OOSYNC)
	{
		intstatus &= ~I_WR_OOSYNC;
	}

	if (intstatus & I_RD_OOSYNC)
	{
		intstatus &= ~I_RD_OOSYNC;
	}

	if (intstatus & I_SBINT)
	{
		intstatus &= ~I_SBINT;
	}

	/* Would be active due to wake-wlan in gSPI */
	if (intstatus & I_CHIPACTIVE)
	{
		intstatus &= ~I_CHIPACTIVE;
	}

	/* Ignore frame indications if rxskip is set */
	if (pstRK903Dev->rxskip)
	{
		intstatus &= ~ FRAME_AVAIL_MASK(pstRK903Dev);
	}

    #if 1  //will later write by zhu
	/* On frame indication, read available frames */
	if (PKT_AVAILABLE(pstRK903Dev, intstatus)) //I_HMB_FRAME_IND
	{
		framecnt = dhdsdio_readframes(dev, rxlimit, &rxdone); //rxlimit = 50;

		if (rxdone || pstRK903Dev->rxskip)
		{
			intstatus  &= ~ FRAME_AVAIL_MASK(pstRK903Dev);
		}
		rxlimit -= MIN(framecnt, rxlimit);
	}
    #endif

	/* Keep still-pending events for next scheduling */
	pstRK903Dev->intstatus = intstatus;

clkwait:
	/* Re-enable interrupts to detect new device events (mailbox, rx frame)
	 * or clock availability.  (Allows tx loop to check ipend if desired.)
	 * (Unless register access seems hosed, as we may not be able to ACK...)
	 */
	if (pstRK903Dev->intr && pstRK903Dev->intdis && !bcmsdh_regfail(pstRK903Dev,sdh))
	{
		pstRK903Dev->intdis = FALSE;
	}

	/* Resched if events or tx frames are pending, else await next interrupt */
	/* On failed register access, all bets are off: no resched or interrupts */
	if ((pstRK903Dev->busstate == DHD_BUS_DOWN) || bcmsdh_regfail(pstRK903Dev,sdh))
	{
		pstRK903Dev->busstate = DHD_BUS_DOWN;
		pstRK903Dev->intstatus = 0;
	}
	else if (pstRK903Dev->clkstate == CLK_PENDING)
	{
		/* Awaiting I_CHIPACTIVE; don't resched */
	}
	else if (pstRK903Dev->intstatus || pstRK903Dev->ipend || PKT_AVAILABLE(pstRK903Dev, pstRK903Dev->intstatus))
	{
		/* Read multiple frames */
		resched = TRUE;
	}

	pstRK903Dev->dpc_sched = resched;

	#if 0 //dgl no low power,
	/* If we're done for now, turn off clock request. */
	if ((bus->idletime == DHD_IDLE_IMMEDIATE) && (bus->clkstate != CLK_PENDING))
	{
		bus->activity = FALSE;
		dhdsdio_clkctl(bus, CLK_NONE, FALSE);
	}
	#endif

	return resched;
}






/*******************************************************************************
** Name: bcm_iovar_lencheck
** Input:const bcm_iovar_t *vi, void *arg, int len, bool set
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.27
** Time: 15:11:46
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcm_iovar_lencheck(const bcm_iovar_t *vi, void *arg, int len, bool set)
{
	rk_err_t ret;

	/* length check on io buf */
	switch (vi->type)
	{
	case IOVT_BOOL:
	case IOVT_INT8:
	case IOVT_INT16:
	case IOVT_INT32:
	case IOVT_UINT8:
	case IOVT_UINT16:
	case IOVT_UINT32:
		/* all integers are int32 sized args at the ioctl interface */
		if (len < (int)sizeof(int))
		{
			ret = BCME_BUFTOOSHORT;
		}
		break;

	case IOVT_BUFFER:
		/* buffer must meet minimum length requirement */
		if (len < vi->minlen)
		{
			ret = BCME_BUFTOOSHORT;
		}
		break;

	case IOVT_VOID:
		if (!set)
		{
			/* Cannot return nil... */
			ret = BCME_UNSUPPORTED;
		}
		else if (len)
		{
			/* Set is an action w/o parameters */
			ret = BCME_BUFTOOLONG;
		}
		break;

	default:
		/* unknown type for length check in iovar info */
		ret = BCME_UNSUPPORTED;

	}

    ret = RK_SUCCESS;

	return ret;

}

/*******************************************************************************
** Name: bcm_iovar_lookup
** Input:const bcm_iovar_t *table, const char *name
** Return: const bcm_iovar_t*
** Owner:Aaron.sun
** Date: 2014.6.27
** Time: 14:47:41
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN const bcm_iovar_t* bcm_iovar_lookup(const bcm_iovar_t *table, const char *name)
{
	const bcm_iovar_t *vi;
	const char *lookup_name;

	/* skip any ':' delimited option prefixes */
	lookup_name = strrchr(name, ':');
	if (lookup_name != NULL)
		lookup_name++;
	else
		lookup_name = name;

	for (vi = table; vi->name; vi++)
	{
		if (!strcmp(vi->name, lookup_name))
			return vi;
	}
	/* ran to end of table */

	return NULL; /* var name not found */

}

/*******************************************************************************
** Name: bcmsdh_iovar_op
** Input:RK903_DEVICE_CLASS * pstRk903Dev, const char *name, void *params, int plen, void *arg, int len, bool set
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.27
** Time: 14:43:25
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_iovar_op(RK903_DEVICE_CLASS * pstRk903Dev, const char *name, void *params, int plen, void *arg, int len, bool set)
{
	const bcm_iovar_t *vi = NULL;
	rk_err_t ret;
	int val_size;
	int32 int_val = 0;
	uint32 actionid;

	if ((vi = bcm_iovar_lookup(sdioh_iovars, name)) ==  NULL)
	{
		ret = RK_ERROR;
		goto exit;
	}

	if ((ret = bcm_iovar_lencheck(vi, arg, len, set)) != 0)
		goto exit;

	/* Set up params so get and set can share the convenience variables */
	if (params == NULL)
	{
		params = arg;
		plen = len;
	}

	if (vi->type == IOVT_VOID)
		val_size = 0;
	else if (vi->type == IOVT_BUFFER)
		val_size = len;
	else
		val_size = sizeof(int);

	if (plen >= (int)sizeof(int_val))
		memcpy(&int_val, params, sizeof(int_val));

	actionid = set ? IOV_SVAL(vi->varid) : IOV_GVAL(vi->varid);
	switch (actionid)
	{
	case IOV_GVAL(IOV_MSGLEVEL):
		int_val = (int32)sd_msglevel;
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_SVAL(IOV_MSGLEVEL):
		sd_msglevel = int_val;
		break;

	case IOV_GVAL(IOV_BLOCKMODE):
		int_val = (int32)pstRk903Dev->sd_blockmode;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_SVAL(IOV_BLOCKMODE):
		pstRk903Dev->sd_blockmode = (bool)int_val;
		/* Haven't figured out how to make non-block mode with DMA */
		break;

	case IOV_GVAL(IOV_BLOCKSIZE):
		if ((uint32)int_val > pstRk903Dev->num_funcs)
		{
			ret = BCME_BADARG;
			break;
		}
		int_val = (int32)pstRk903Dev->client_block_size[int_val];
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_SVAL(IOV_BLOCKSIZE):
	{
		uint func = ((uint32)int_val >> 16);
		uint blksize = (uint16)int_val;
		uint maxsize;

		if (func > pstRk903Dev->num_funcs)
		{
			ret = BCME_BADARG;
			break;
		}

		switch (func)
		{
			case 0: maxsize = 32; break;
			case 1: maxsize = BLOCK_SIZE_4318; break;
			case 2: maxsize = BLOCK_SIZE_4328; break;
			default: maxsize = 0;
		}

		if (blksize > maxsize)
		{
			ret = BCME_BADARG;
			break;
		}

		if (!blksize)
		{
			blksize = maxsize;
		}

		/* Now set it */
		pstRk903Dev->client_block_size[func] = blksize;
		break;
	}

	case IOV_GVAL(IOV_RXCHAIN):
		int_val = FALSE;
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_GVAL(IOV_DMA):
		int_val = (int32)pstRk903Dev->sd_use_dma;
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_SVAL(IOV_DMA):
		pstRk903Dev->sd_use_dma = (bool)int_val;
		break;

	case IOV_GVAL(IOV_USEINTS):
		int_val = (int32)pstRk903Dev->use_client_ints;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_SVAL(IOV_USEINTS):
		pstRk903Dev->use_client_ints = (bool)int_val;
		if (pstRk903Dev->use_client_ints)
			pstRk903Dev->intmask |= CLIENT_INTR;
		else
			pstRk903Dev->intmask &= ~CLIENT_INTR;

		break;

	case IOV_GVAL(IOV_DIVISOR):
		int_val = (uint32)sd_divisor;
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_SVAL(IOV_DIVISOR):
		sd_divisor = int_val;
		break;

	case IOV_GVAL(IOV_POWER):
		int_val = (uint32)sd_power;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_SVAL(IOV_POWER):
		sd_power = int_val;
		break;

	case IOV_GVAL(IOV_CLOCK):
		int_val = (uint32)sd_clock;
		memcpy(arg, &int_val, val_size);
		break;

	case IOV_SVAL(IOV_CLOCK):
		sd_clock = int_val;
		break;

	case IOV_GVAL(IOV_SDMODE):
		int_val = (uint32)sd_sdmode;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_SVAL(IOV_SDMODE):
		sd_sdmode = int_val;
		break;

	case IOV_GVAL(IOV_HISPEED):
		int_val = (uint32)sd_hiok;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_SVAL(IOV_HISPEED):
		sd_hiok = int_val;
		break;

	case IOV_GVAL(IOV_NUMINTS):
		int_val = (int32)pstRk903Dev->intrcount;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_GVAL(IOV_NUMLOCALINTS):
		int_val = (int32)0;
		memcpy(arg, &int_val,val_size);
		break;

	case IOV_GVAL(IOV_HOSTREG):
	{
		sdreg_t *sd_ptr = (sdreg_t *)params;

		if (sd_ptr->offset < SD_SysAddr || sd_ptr->offset > SD_MaxCurCap)
		{
			ret = BCME_BADARG;
			break;
		}

		if (sd_ptr->offset & 1)
			int_val = 8; /* sdioh_sdmmc_rreg8(si, sd_ptr->offset); */
		else if (sd_ptr->offset & 2)
			int_val = 16; /* sdioh_sdmmc_rreg16(si, sd_ptr->offset); */
		else
			int_val = 32; /* sdioh_sdmmc_rreg(si, sd_ptr->offset); */

		memcpy(arg, &int_val, sizeof(int_val));
		break;
	}

	case IOV_SVAL(IOV_HOSTREG):
	{
		sdreg_t *sd_ptr = (sdreg_t *)params;

		if (sd_ptr->offset < SD_SysAddr || sd_ptr->offset > SD_MaxCurCap)
		{
			ret = BCME_BADARG;
			break;
		}
		break;
	}

	case IOV_GVAL(IOV_DEVREG):
	{
		sdreg_t *sd_ptr = (sdreg_t *)params;
		uint8 data = 0;

		if (bcmsdh_cfg_read(pstRk903Dev, sd_ptr->func, sd_ptr->offset, &data))
		{
			ret = BCME_SDIO_ERROR;
			break;
		}

		int_val = (int)data;
		memcpy(arg, &int_val,sizeof(int_val));
		break;
	}

	case IOV_SVAL(IOV_DEVREG):
	{
		sdreg_t *sd_ptr = (sdreg_t *)params;
		uint8 data = (uint8)sd_ptr->value;

		if (bcmsdh_cfg_write(pstRk903Dev, sd_ptr->func, sd_ptr->offset, data))
		{
			ret = BCME_SDIO_ERROR;
			break;
		}
		break;
	}

	default:
		ret = BCME_UNSUPPORTED;
		break;
	}
    ret = RK_SUCCESS;
exit:

	return ret;
}

/*******************************************************************************
** Name: PKTDATA
** Input:void * skbf
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.26
** Time: 17:37:48
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN void *  PKTDATA(void * skbf)
{
	sk_buff * skbp = (sk_buff *)skbf;
	if (!skbf)
		return NULL;

	return skbp->data;
}

/*******************************************************************************
** Name: PKTLEN
** Input:void * skbf
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.26
** Time: 17:32:39
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN uint32 PKTLEN(void * skbf)
{
	sk_buff * skbp = (sk_buff *)skbf;
	if (!skbf)
		return 0;

	return skbp->len;
}

/*******************************************************************************
** Name: sdioh_request_packet
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint fix_inc, uint write, uint func, uint addr, void *pkt
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.26
** Time: 17:29:13
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t sdioh_request_packet(RK903_DEVICE_CLASS * pstRk903Dev, uint fix_inc, uint write, uint func, uint addr, void *pkt)
{
	bool fifo = (fix_inc == SDIOH_DATA_FIX);
	rk_err_t ret;
	uint pkt_len;

	pkt_len = PKTLEN(pkt);

	pkt_len += 3;
	pkt_len &= 0xFFFFFFFC;

	/* Make sure the packet is aligned properly. If it isn't, then this
	 * is the fault of sdioh_request_buffer() which is supposed to give
	 * us something we can work with.
	 */
	if ((write) && (!fifo))
	{
		ret = sdio_memcpy_toio(pstRk903Dev->hFunc[func], addr, PKTDATA(pkt), pkt_len);
	}
	else if (write)
	{
		ret = SdioDev_WriteSb(pstRk903Dev->hFunc[func], addr, PKTDATA(pkt), pkt_len);//fifo W
	}
	else if (fifo)
	{
		ret = SdioDev_ReadSb(pstRk903Dev->hFunc[func], PKTDATA(pkt), addr, pkt_len);//fifo R
	}
	else
	{
		ret = sdio_memcpy_fromio(pstRk903Dev->hFunc[func], PKTDATA(pkt), addr, pkt_len);
	}

	if (!fifo)
	{
		addr += pkt_len;
	}

	return ret;
}

/*******************************************************************************
** Name: sdioh_request_buffer
** Input:void
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.26
** Time: 17:26:55
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t sdioh_request_buffer(RK903_DEVICE_CLASS * pstRk903Dev, uint fix_inc, uint write, uint func,
                     uint addr, uint buflen_u, uint8 *buffer)
{
    sk_buff  mypkt;

    mypkt.len = buflen_u;
    mypkt.data = buffer;
	return sdioh_request_packet(pstRk903Dev, fix_inc, write, func, addr, &mypkt);

}

/*******************************************************************************
** Name: bcmsdh_rwdata
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint rw, uint32 addr, uint8 *buf, uint nbytes
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.26
** Time: 17:24:19
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_rwdata(RK903_DEVICE_CLASS * pstRk903Dev, uint rw, uint32 addr, uint8 *buf, uint nbytes)
{


	addr &= SBSDIO_SB_OFT_ADDR_MASK;
	addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;

	return sdioh_request_buffer(pstRk903Dev, SDIOH_DATA_INC,
	                              (rw ? SDIOH_WRITE : SDIOH_READ), SDIO_FUN_1,
	                              addr, nbytes, buf);
}


/******************************************************************************
 * dhd_bus_rxctl -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.17  written
 * Time : 14:02:17
 * --------------------
 ******************************************************************************/
 _DRIVER_RK903_RK903DEVICE_COMMON_
static int dhd_bus_rxctl(RK903_DEVICE_CLASS * pstRk903Dev, uchar *msg, uint msglen)
{
	int timeleft, i, clen;
	uint rxlen = 0;


	/* Wait until control frame is available */
	timeleft = 400;

    #if 0 // wait cmd complete
    bus->is_get_ctrl_frame = 0;
    while(!bus->is_get_ctrl_frame)//dhdsdio_isr
    {
        if (SDC_GetSdioEvent(0) == SDC_SUCCESS)
        {
            process_sdio_pending_irqs();
        }

        DelayMs(5);
        if(--timeleft == 0)
        {
           BCMDEBUG("wait ctrl timeout");
           return -ETIMEDOUT;
        }
    }
    bus->is_get_ctrl_frame = 0;

    #endif

    rkos_semaphore_take(pstRk903Dev->cmdSem, MAX_DELAY);

	rxlen = pstRk903Dev->rxlen;
	pstRk903Dev->rxlen = 0;

	clen = MIN(msglen, rxlen);
	for (i = 0; i < clen; i++)
	{
		/*can not use bcopy, because the msg and rxctl both
		pointer to one global varilbe in different position.
		bus->rxctl += doff;=>move to the header of "cdc_ioctrl_t+msg" postion.*/
		msg[i] = pstRk903Dev->rxctl[i];//msg == bus->dhd->prot->msg.
	}

	return rxlen ? (int)rxlen : -EIO;
}

/******************************dhd_cdc.c begin ****************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
void dhd_prot_hdrpush(RK903_DEVICE_CLASS * pstRk903Dev, int ifidx, void *pktbuf)
{
	struct bdc_header *h;

	/* Push BDC header used to convey priority for buses that don't */
	PKTPUSH(pktbuf, BDC_HEADER_LEN);

	h = (struct bdc_header *)PKTDATA(pktbuf);
	h->flags = (BDC_PROTO_VER << BDC_FLAG_VER_SHIFT);
	h->flags2 = 0;
	h->dataOffset = 0;
	BDC_SET_IF_IDX(h, ifidx);
}

/******************************************************************************
 * dhdsdio_txpkt -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.21  written
 * Time : 9:07:37
 * --------------------
 ******************************************************************************/
	_DRIVER_RK903_RK903DEVICE_COMMON_
int dhdsdio_txpkt(RK903_DEVICE_CLASS * pstRk903Dev, void *pkt, uint chan, bool free_pkt)
{
	int ret;
	uint8 *frame;
	uint16 len, pad1 = 0;
	uint32 swheader;
	uint retries = 0;
	bcmsdh_info_t *sdh;
	void *new;
	int i;

//	sdh = bus->sdh;

//	if (pstRk903Dev->dongle_reset)
//	{
//		ret = BCME_NOTREADY;
//		goto done;
//	}

	frame = (uint8*)PKTDATA(pkt);
	/* Add alignment padding, allocate new packet if needed */
	pad1 = (uint)frame % DHD_SDALIGN;
	if (pad1)
	{
		PKTPUSH(pkt, pad1);
		frame = (uint8*)PKTDATA(pkt);
		bzero(frame, pad1 + SDPCM_HDRLEN);
	}

	/* Hardware tag: 2 byte len followed by 2 byte ~len check (all LE) */
	len = (uint16)PKTLEN(pkt);
	*(uint16*)frame = htol16(len);
	*(((uint16*)frame) + 1) = htol16(~len);

	/* Software tag: channel, sequence number, data offset */
	swheader = ((chan << SDPCM_CHANNEL_SHIFT) & SDPCM_CHANNEL_MASK) | pstRk903Dev->tx_seq |
	        (((pad1 + SDPCM_HDRLEN) << SDPCM_DOFFSET_SHIFT) & SDPCM_DOFFSET_MASK);
	htol32_ua_store(swheader, frame + SDPCM_FRAMETAG_LEN);
	htol32_ua_store(0, frame + SDPCM_FRAMETAG_LEN + sizeof(swheader));

	/* Raise len to next SDIO block to eliminate tail command */
	if (pstRk903Dev->roundup && pstRk903Dev->blocksize && (len > pstRk903Dev->blocksize))
	{
		uint16 pad2 = pstRk903Dev->blocksize - (len % pstRk903Dev->blocksize);
		if ((pad2 <= pstRk903Dev->roundup) && (pad2 < pstRk903Dev->blocksize))
		{
			len += pad2;
		}
	}
	else if (len % DHD_SDALIGN)
	{
		len += DHD_SDALIGN - (len % DHD_SDALIGN);
	}
	/* Some controllers have trouble with odd bytes -- round to even */
	if (forcealign && (len & (ALIGNMENT - 1)))
	{
		len = ROUNDUP(len, ALIGNMENT);
    }

	do
	{
		ret = bcmsdh_send_buf(pstRk903Dev, pstRk903Dev->sbwad, SDIO_FUN_2, F2SYNC,frame, len);

        pstRk903Dev->f2txdata++;
		ASSERT(ret != BCME_PENDING);
		if (ret < 0)
		{
			/* On failure, abort the command and terminate the frame */
			rk_printf("%s: sdio error %d, abort command and terminate frame.\n", ret);
			pstRk903Dev->tx_sderrs++;

			bcmsdh_abort(pstRk903Dev, SDIO_FUN_2);
			bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_FRAMECTRL, SFC_WF_TERM);
            pstRk903Dev->f1regdata++;

			for (i = 0; i < 3; i++)
			{
				uint8 hi, lo;
				hi = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_WFRAMEBCHI, NULL);
				lo = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_WFRAMEBCLO, NULL);
				pstRk903Dev->f1regdata += 2;
				if ((hi == 0) && (lo == 0))
					break;
			}
		}
        if (ret == 0)
		{
			pstRk903Dev->tx_seq = (pstRk903Dev->tx_seq + 1) % SDPCM_SEQUENCE_WRAP;
		}
	} while ((ret < 0) && retrydata && retries++ < TXRETRIES);


done:

	/* restore pkt buffer pointer before calling tx complete routine */
	PKTPULL(pkt, SDPCM_HDRLEN + pad1);

	return ret;
}


/******************************************************************************
 * dhd_senddata -
 * DESCRIPTION: -
 *
 * Input:
 * Output:
 * Returns:
 *
 * modification history
 * --------------------
 * Owner: wangping
 * Date : 2014.7.21  written
 * Time : 9:07:49
 * --------------------
 ******************************************************************************/
 _DRIVER_RK903_RK903DEVICE_COMMON_
int dhd_senddata(void * pstRk903Dev, char *buf, uint32 len)
{
	int ret;
    sk_buff *pktbuf;

	if (pstRk903Dev == NULL)
	{
		pstRk903Dev = gpstRk903DevISR[0];
	}

    pktbuf = PKTGET(len);
    memcpy(PKTDATA(pktbuf), buf, len);

	/* If the protocol uses a data header, apply it */
	dhd_prot_hdrpush(pstRk903Dev, 0, pktbuf);  //增加协议头

	PKTPUSH (pktbuf, SDPCM_HDRLEN);  //增加 SDPCM_HDRLEN 空间

	ret = dhdsdio_txpkt(pstRk903Dev, pktbuf, SDPCM_DATA_CHANNEL, TRUE);

    PKTFREE(pktbuf);
	return ret;
}


/*******************************************************************************
** Name: dhdcdc_cmplt
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 id, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 14:08:35
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdcdc_cmplt(RK903_DEVICE_CLASS * pstRk903Dev, uint32 id, uint32 len)
{
	int ret;
	int cdc_len = len + sizeof(cdc_ioctl_t);
	DHD_PROT *prot = pstRk903Dev->prot;

	do
	{
		ret = dhd_bus_rxctl(pstRk903Dev, (uint8*)prot->msg, cdc_len);
		if (ret < 0)
		{
            rk_printf("dhd_bus_rxctl err");
            break;
        }

	} while (CDC_IOC_ID(ltoh32(prot->msg->flags)) != id);

	return ret;

}



/* this function performs "abort" for both of host & device */
extern int sdioh_abort(RK903_DEVICE_CLASS * pstRk903Dev, uint func)
{
	uint8 t_func = (uint8) func;

	/* issue abort cmd52 command through F1 */
	sdioh_request_byte(pstRk903Dev, SDIOH_WRITE, SDIO_FUN_0, SDIO_CCCR_ABORT, &t_func);

	return SDIOH_API_RC_SUCCESS;
}
_DRIVER_RK903_RK903DEVICE_COMMON_
int bcmsdh_abort(RK903_DEVICE_CLASS * pstRk903Dev, uint fn)
{
    #if 1

	return sdioh_abort(pstRk903Dev, fn);
    #else
    return 0;
    #endif

}
typedef int SDIOH_API_RC;
_DRIVER_RK903_RK903DEVICE_COMMON_
int bcmsdh_send_buf(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint fn, uint flags, uint8 *buf, uint nbytes)
{
	//bcmsdh_info_t *bcmsdh = (bcmsdh_info_t *)sdh;
	//SDIOH_API_RC status;

    SDIOH_API_RC status;
	uint incr_fix;
	uint width;
	int err = 0;

	/* Async not implemented yet */
	if (flags & SDIO_REQ_ASYNC)
		return BCME_UNSUPPORTED;

	err = bcmsdh_set_sbaddr_window(pstRk903Dev, addr, FALSE);
	if (err)
		return err;

	addr &= SBSDIO_SB_OFT_ADDR_MASK;

	incr_fix = (flags & SDIO_REQ_FIXED) ? SDIOH_DATA_FIX : SDIOH_DATA_INC;
	width = (flags & SDIO_REQ_4BYTE) ? 4 : 2;
	if (width == 4)
		addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;

	status = sdioh_request_buffer(pstRk903Dev, incr_fix,
	                              SDIOH_WRITE, fn, addr, nbytes, buf);

	return (SDIOH_API_SUCCESS(status) ? 0 : BCME_ERROR);
}


/*******************************************************************************
** Name: dhd_bus_txctl
** Input:RK903_DEVICE_CLASS * pstRk903Dev, unsigned char *msg, uint msglen
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 13:59:16
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhd_bus_txctl(RK903_DEVICE_CLASS * pstRk903Dev, unsigned char *msg, uint msglen)
{
	uint8 *frame;
	uint16 flen;
	uint32 swheader;
	uint retries = 0;
	bcmsdh_info_t *sdh = pstRk903Dev->sdh;
	uint8 doff = 0;
	int ret = -1;
	int i;

	/* Back the pointer to make a room for bus header */
	frame = msg - SDPCM_HDRLEN;
	msglen += SDPCM_HDRLEN;

	/* Add alignment padding (optional for ctl frames) */
	if (1/*dhd_alignctl*/)
	{
		doff = (uint)frame % DHD_SDALIGN;
		if (doff)
		{
			frame -= doff;
			msglen += doff;
			bzero(frame, doff + SDPCM_HDRLEN);
		}
	}
	doff += SDPCM_HDRLEN;

	flen = msglen;
	/* Round send length to next SDIO block */
	if (pstRk903Dev->roundup && pstRk903Dev->blocksize && (flen > pstRk903Dev->blocksize)) //dgl blocksize is fun2 512
	{
		uint16 pad = pstRk903Dev->blocksize - (flen % pstRk903Dev->blocksize);
		if ((pad <= pstRk903Dev->roundup) && (pad < pstRk903Dev->blocksize))
			flen += pad;
	}
	else if (flen % DHD_SDALIGN)
	{
		flen += DHD_SDALIGN - (flen % DHD_SDALIGN);
	}

	/* Satisfy length-alignment requirements */
	//if (forcealign && (flen & (ALIGNMENT - 1)))
	if (1 && (flen & (ALIGNMENT - 1)))
	{
		flen = ROUNDUP(flen, ALIGNMENT);
	}

	/* Make sure backplane clock is on */
	dhdsdio_clkctl(pstRk903Dev, CLK_AVAIL, FALSE);

	/* Hardware tag: 2 byte len followed by 2 byte ~len check (all LE) */
	*(uint16*)frame = htol16((uint16)msglen);
	*(((uint16*)frame) + 1) = htol16(~msglen);

	/* Software tag: channel, sequence number, data offset */
	swheader = ((SDPCM_CONTROL_CHANNEL << SDPCM_CHANNEL_SHIFT) & SDPCM_CHANNEL_MASK)
	            | pstRk903Dev->tx_seq | ((doff << SDPCM_DOFFSET_SHIFT) & SDPCM_DOFFSET_MASK);
	htol32_ua_store(swheader, frame + SDPCM_FRAMETAG_LEN);
	htol32_ua_store(0, frame + SDPCM_FRAMETAG_LEN + sizeof(swheader));

	do
	{
		ret = bcmsdh_send_buf(pstRk903Dev, pstRk903Dev->sbwad, SDIO_FUN_2, F2SYNC, frame, flen);
		if (ret < 0)
		{
			/* On failure, abort the command and terminate the frame */
			rk_printf("dhd_bus_txctl err=%d.\n", ret);
			pstRk903Dev->tx_sderrs++;

			bcmsdh_abort(pstRk903Dev, SDIO_FUN_2);

			bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_FRAMECTRL, NULL);

			pstRk903Dev->f1regdata++;

			for (i = 0; i < 3; i++)
			{
				uint8 hi, lo;
				hi = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_WFRAMEBCHI, NULL);
				lo = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_WFRAMEBCLO, NULL);
				pstRk903Dev->f1regdata += 2;
				if ((hi == 0) && (lo == 0))
				{
					break;
				}
			}
		}
		if (ret == 0)
		{
			pstRk903Dev->tx_seq = (pstRk903Dev->tx_seq + 1) % SDPCM_SEQUENCE_WRAP;
		}
	} while ((ret < 0) && retries++ < TXRETRIES);

done:
	#if 0 //dgl no low power,
	if ((bus->idletime == DHD_IDLE_IMMEDIATE) && !bus->dpc_sched)
	{
		bus->activity = FALSE;
		dhdsdio_clkctl(bus, CLK_NONE, TRUE);
	}
	#endif

	return ret ? -EIO : 0;
}

/*******************************************************************************
** Name: dhdcdc_msg
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 13:55:50
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdcdc_msg(RK903_DEVICE_CLASS * pstRk903Dev)
{
    rk_err_t ret;
    DHD_PROT *prot = pstRk903Dev->prot;
    int cdc_len = ltoh32(prot->msg->len) + sizeof(cdc_ioctl_t);

    if (cdc_len > CDC_MAX_MSG_SIZE)
        cdc_len = CDC_MAX_MSG_SIZE;

    /* Send request */
    ret = dhd_bus_txctl(pstRk903Dev, (uint8*)prot->msg, cdc_len);

    return ret;
}

/*******************************************************************************
** Name: PKTPULL
** Input:void * skbf, uint32 len
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 13:39:09
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN void * PKTPULL(void * skbf, uint32 len)
{
    sk_buff * skbp = (sk_buff *)skbf;
    if (!skbf)
        return 0;
    skbp->data = (uint8 *)(skbp->data) + len;
    skbp->len -= len;

    return skbp->data;
}

/*******************************************************************************
** Name: PKTPUSH
** Input:void * skbf, uint32 len
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 13:37:23
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN void * PKTPUSH(void * skbf, uint32 len)
{
    sk_buff * skbp = (sk_buff *)skbf;
    if (!skbf)
        return 0;
    skbp->data = (uint8 *)(skbp->data) - len;
    skbp->len  += len;

    return skbp->data;
}

/*******************************************************************************
** Name: dhd_wl_ioctl_cmd
** Input:RK903_DEVICE_CLASS * pstRk903Dev, int cmd, void *arg, int len, uint8 set
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 10:10:13
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhd_wl_ioctl_cmd(RK903_DEVICE_CLASS * pstRk903Dev, int cmd, void *arg, int len, uint8 set)
{
    int ret;
    sk_buff pkt;

    if ((pstRk903Dev->busstate == DHD_BUS_DOWN) || (len > WLC_IOCTL_MAXLEN))
    {
        return -1;
    }

    //pkt.data = rkos_memory_malloc(CMD_RXBUF_HEAD);
    pkt.data = get_global_cmdbuf(CMD_RXBUF_HEAD);
	memset(pkt.data,0,len+1);

    memcpy(pkt.data, arg, len);
    pkt.len = len;

    ret = dhdcdc_send_ioctl(pstRk903Dev, cmd, &pkt, len, set);//send command
    if (ret > 0)
    {
        /* Too many programs assume ioctl() returns 0 on success */
        memcpy(arg, pkt.data, pkt.len);
        ret = 0;
    }

    return ret;
}

/*******************************************************************************
** Name: dhdsdio_membytes
** Input:RK903_DEVICE_CLASS * pstRk903Dev, bool write, uint32 address, uint8 *data, uint size
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:57:20
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_membytes(RK903_DEVICE_CLASS * pstRk903Dev, bool write, uint32 address, uint8 *data, uint size)
{
	rk_err_t ret;
	uint32 sdaddr;
	uint dsize;

	/* Determine initial transfer parameters */
	sdaddr = address & SBSDIO_SB_OFT_ADDR_MASK;
	if ((sdaddr + size) & SBSDIO_SBWINDOW_MASK)
		dsize = (SBSDIO_SB_OFT_ADDR_LIMIT - sdaddr);
	else
		dsize = size;

	/* Set the backplane window to include the start address */
	ret = dhdsdio_set_siaddr_window(pstRk903Dev, address);
	if (ret)
	{
		goto xfer_done;
	}

	/* Do the transfer(s) */
	while (size)
	{
		ret = bcmsdh_rwdata(pstRk903Dev, write, sdaddr, data, dsize);
		if (ret)
			break;

		/* Adjust for next transfer (if any) */
		if ((size -= dsize))
		{
			data += dsize;
			address += dsize;
			ret = dhdsdio_set_siaddr_window(pstRk903Dev, address);
			if (ret)
				break;
			sdaddr = 0;
			dsize = MIN(SBSDIO_SB_OFT_ADDR_LIMIT, size);
		}
	}

xfer_done:
	/* Return the window to backplane enumeration space for core access */
	dhdsdio_set_siaddr_window(pstRk903Dev, pstRk903Dev->sbwad);

	return ret;
}

/*******************************************************************************
** Name: dhdsdio_htclk
** Input:RK903_DEVICE_CLASS * pstRk903Dev, bool on, bool pendok
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:44:47
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_htclk(RK903_DEVICE_CLASS * pstRk903Dev, bool on, bool pendok)
{
	rk_err_t ret;
	uint8 clkctl, clkreq, devctl;
	clkctl = 0;

	if (on)
	{
		/* Request HT Avail */
		clkreq = pstRk903Dev->alp_only ? SBSDIO_ALP_AVAIL_REQ : SBSDIO_HT_AVAIL_REQ;

		ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, clkreq);
		if (ret)
			return RK_ERROR;

		/* Check current status */
		ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &clkctl);
		if (ret)
			return RK_ERROR;
        if(pstRk903Dev->alp_only == NULL)
            rk_printf("the clkreq is%x  clkctl is %x",clkreq,clkctl);
		/* Go to pending and await interrupt if appropriate */
		if (!SBSDIO_CLKAV(clkctl, pstRk903Dev->alp_only) && pendok)
		{
			/* Allow only clock-available interrupt */
			ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, &devctl);
			if (ret)
				return RK_ERROR;

			devctl |= SBSDIO_DEVCTL_CA_INT_ONLY;
			ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, devctl);
            if(ret)
                return RK_ERROR;

			pstRk903Dev->clkstate = CLK_PENDING;

			return RK_ERROR;
		}
		else if (pstRk903Dev->clkstate == CLK_PENDING)
		{
			/* Cancel CA-only interrupt filter */
			ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, &devctl);
            if(ret)
                return RK_ERROR;

			devctl &= ~SBSDIO_DEVCTL_CA_INT_ONLY;
			ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, devctl);
            if(ret)
                return RK_ERROR;
		}

		/* Otherwise, wait here (polling) for HT Avail */
		if (!SBSDIO_CLKAV(clkctl, pstRk903Dev->alp_only))
		{
			SPINWAIT(((ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &clkctl)),
			          !SBSDIO_CLKAV(clkctl, pstRk903Dev->alp_only)), PMU_MAX_TRANSITION_DLY);
		}

		if (ret)
			return RK_ERROR;

		if (!SBSDIO_CLKAV(clkctl, pstRk903Dev->alp_only))
			return RK_ERROR;


		/* Mark clock available */
		pstRk903Dev->clkstate = CLK_AVAIL;
		pstRk903Dev->activity = TRUE;
	}
	else
	{
		clkreq = 0;

		if (pstRk903Dev->clkstate == CLK_PENDING)
		{
			/* Cancel CA-only interrupt filter */
			ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, &devctl);
			devctl &= ~SBSDIO_DEVCTL_CA_INT_ONLY;
			bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_DEVICE_CTL, devctl);
		}

		pstRk903Dev->clkstate = CLK_SDONLY;
		ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, clkreq);
		if (ret)
			return RK_ERROR;
	}

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: dhdsdio_sdclk
** Input:RK903_DEVICE_CLASS * pstRk903Dev, bool on
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:43:42
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_sdclk(RK903_DEVICE_CLASS * pstRk903Dev, bool on)
{
	rk_err_t ret;
	int32 iovalue;

	if (on)
	{
		if (pstRk903Dev->idleclock == DHD_IDLE_STOP)
		{
			/* Turn on clock and restore mode */
			iovalue = 1;
			ret = bcmsdh_iovar_op(pstRk903Dev, "sd_clock", NULL, 0,
			                      &iovalue, sizeof(iovalue), TRUE);
			if (ret)
				return RK_ERROR;

			iovalue = pstRk903Dev->sd_mode;
			ret = bcmsdh_iovar_op(pstRk903Dev, "sd_mode", NULL, 0,
			                      &iovalue, sizeof(iovalue), TRUE);
			if (ret)
				return BCME_ERROR;
		}
		else if(pstRk903Dev->idleclock != DHD_IDLE_ACTIVE)
		{
			/* Restore clock speed */
			iovalue = pstRk903Dev->sd_divisor;
			ret = bcmsdh_iovar_op(pstRk903Dev, "sd_divisor", NULL, 0,
			                      &iovalue, sizeof(iovalue), TRUE);
			if (ret)
				return BCME_ERROR;
		}

		pstRk903Dev->clkstate = CLK_SDONLY;

	}
	else
	{
		/* Stop or slow the SD clock itself */
		if ((pstRk903Dev->sd_divisor == -1) || (pstRk903Dev->sd_mode == -1))
			return BCME_ERROR;

		if (pstRk903Dev->idleclock == DHD_IDLE_STOP)
		{
			if (1)
			{
				/* Change to SD1 mode and turn off clock */
				iovalue = 1;
				ret = bcmsdh_iovar_op(pstRk903Dev, "sd_mode", NULL, 0,
				                      &iovalue, sizeof(iovalue), TRUE);
				if (ret)
					return BCME_ERROR;
			}

			iovalue = 0;
			ret = bcmsdh_iovar_op(pstRk903Dev, "sd_clock", NULL, 0,
			                      &iovalue, sizeof(iovalue), TRUE);
			if (ret)
				return BCME_ERROR;
		}
		else if (pstRk903Dev->idleclock != DHD_IDLE_ACTIVE)
		{
			/* Set divisor to idle value */
			iovalue = pstRk903Dev->idleclock;
			ret = bcmsdh_iovar_op(pstRk903Dev, "sd_divisor", NULL, 0,
			                      &iovalue, sizeof(iovalue), TRUE);
			if (ret)
				return BCME_ERROR;
		}
		pstRk903Dev->clkstate = CLK_NONE;
	}

	return BCME_OK;
}

/*******************************************************************************
** Name: dhdsdio_clkctl
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint target, bool pendok
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:39:32
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_clkctl(RK903_DEVICE_CLASS * pstRk903Dev, uint target, bool pendok)
{
    rk_err_t ret;

    /* Early exit if we're already there */
    if (pstRk903Dev->clkstate == target)
    {
        if (target == CLK_AVAIL)
        {
            pstRk903Dev->activity = TRUE;
        }
        return ret;
    }

    switch (target)
    {
        case CLK_AVAIL:
            /* Make sure SD clock is available */
            if (pstRk903Dev->clkstate == CLK_NONE)
                dhdsdio_sdclk(pstRk903Dev, TRUE);
            /* Now request HT Avail on the backplane */
            ret = dhdsdio_htclk(pstRk903Dev, TRUE, pendok);
            if (ret == RK_SUCCESS)
            {
                pstRk903Dev->activity = TRUE;
            }
            break;

        case CLK_SDONLY:
            /* Remove HT request, or bring up SD clock */
            if (pstRk903Dev->clkstate == CLK_NONE)
                ret = dhdsdio_sdclk(pstRk903Dev, TRUE);
            else if (pstRk903Dev->clkstate == CLK_AVAIL)
                ret = dhdsdio_htclk(pstRk903Dev, FALSE, FALSE);
            break;

        case CLK_NONE:
            /* Make sure to remove HT request */
            if (pstRk903Dev->clkstate == CLK_AVAIL)
                ret = dhdsdio_htclk(pstRk903Dev, FALSE, FALSE);
            /* Now remove the SD clock */
            ret = dhdsdio_sdclk(pstRk903Dev, FALSE);
            break;
    }

    return ret;
}

/*******************************************************************************
** Name: bcmsdh_reg_read
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 10:52:16
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_reg_read(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size, uint32 *word)
{
    rk_err_t ret;
    uint32 word_temp = 0;
    if(pstRk903Dev == NULL)
    {
        pstRk903Dev = gpstRk903DevISR[0];
    }

    if(word == NULL)
    {
        word = &word_temp;
    }

    ret = bcmsdh_set_sbaddr_window(pstRk903Dev, addr, FALSE);

    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    addr &= SBSDIO_SB_OFT_ADDR_MASK;
    if (size == 4)
        addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;

    ret = sdioh_request_word(pstRk903Dev, SDIOH_READ, SDIO_FUN_1, addr, word, size);
    if (ret != RK_SUCCESS)
    {
        return RK_SUCCESS;
    }

}

/*******************************************************************************
** Name: sdioh_request_word
** Input:void
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 8:36:01
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t sdioh_request_word(RK903_DEVICE_CLASS * pstRk903Dev, uint rw, uint func, uint addr,
                                     uint32 *word, uint nbytes)
{
    rk_err_t ret;

    if (func == 0)
    {
        return RK_ERROR;
    }

    if (rw)
    {
        /* CMD52 Write */
        if (nbytes == 4)
        {
            ret = SdioDev_Writel(pstRk903Dev->hFunc[func], *word, addr);
        }
        else if (nbytes == 2)
        {
            ret = SdioDev_Writew(pstRk903Dev->hFunc[func], (*word & 0xFFFF), addr);
        }
        else
        {
            rk_printf("Invalid nbytes: %d\n", nbytes);
        }
    }
    else
    {
        /* CMD52 Read */
        if (nbytes == 4)
        {
            ret = SdioDev_Readl(pstRk903Dev->hFunc[func], addr);
            if (ret < 0)
            {
                return RK_ERROR;
            }
            *word = (uint32)ret;

        }
        else if (nbytes == 2)
        {
            ret = SdioDev_Readl(pstRk903Dev->hFunc[func], addr);
            if (ret < 0)
            {
                return RK_ERROR;
            }
            *word = (uint16)ret;
        }
        else
        {
            rk_printf("Invalid nbytes: %d\n", nbytes);
        }
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: bcmsdh_set_sbaddr_window
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, bool force_set
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:49:37
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_set_sbaddr_window(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, bool force_set)
{
    rk_err_t ret = 0;
    uint bar0 = addr & ~SBSDIO_SB_OFT_ADDR_MASK;

    if ((bar0 != pstRk903Dev->sbwad) || force_set)
    {
        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRLOW,
                               (addr >> 8) & SBSDIO_SBADDRLOW_MASK);
        if (!ret)
            ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRMID,
                                   (addr >> 16) & SBSDIO_SBADDRMID_MASK);
        if (!ret)
            ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRHIGH,
                                   (addr >> 24) & SBSDIO_SBADDRHIGH_MASK);

        if (!ret)
            pstRk903Dev->sbwad = bar0;
        else
            pstRk903Dev->sbwad = 0;  /* invalidate cached window var */
    }

    return ret;

}

/*******************************************************************************
** Name: bcmsdh_cfg_read
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 *data
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 14:49:49
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_cfg_read(RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 *data)
{
    return sdioh_request_byte(pstRk903Dev, SDIOH_READ, fnc_num, addr, data);
}

/*******************************************************************************
** Name: sdioh_request_byte
** Input:sdioh_info_t *sd, uint rw, uint func, uint regaddr, uint8 *byte
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 14:27:22
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t sdioh_request_byte(RK903_DEVICE_CLASS * pstRk903Dev, uint32 rw, uint32 func, uint32 regaddr, uint8 *byte)
{
    rk_err_t ret;

    /* CMD52 Write */
    if (rw)
    {
        if (func == 0)
        {
            /* Can only directly write to some F0 registers.  Handle F2 enable
             * as a special case.
             */
            if (regaddr == SDIO_CCCR_IOEx)
            {
                if (pstRk903Dev->hFunc[2])
                {
                    if (*byte & SDIO_FUNC_ENABLE_2)
                    {
                        /* Enable Function 2 */
                        ret = SdioDev_EnalbeFunc(pstRk903Dev->hFunc[2]);
                    }
                    else
                    {
                        /* Disable Function 2 */
                        ret = SdioDev_DisableFunc(pstRk903Dev->hFunc[2]);
                    }
                }
            }
            /* to allow abort command through F1 */
            else if (regaddr == SDIO_CCCR_ABORT)
            {
                /*
                * this sdio_f0_writeb() can be replaced with another api
                * depending upon MMC driver change.
                * As of this time, this is temporaray one
                */
                ret = SdioDev_Writeb(pstRk903Dev->hFunc[func], *byte, regaddr);
            }
            else if (regaddr < 0xF0)
            {
                rk_printf("bcmsdh_sdmmc: F0 Wr:0x%02x: write disallowed\n", regaddr);
            }
            else
            {
                /* Claim host controller, perform F0 write, and release */
                ret = SdioDev_Writeb(pstRk903Dev->hFunc[0], *byte, regaddr);
            }
        }
        else
        {
            /* Claim host controller, perform Fn write, and release */
            ret = SdioDev_Writeb(pstRk903Dev->hFunc[func], *byte, regaddr);
        }
    }
    else
    {
        /* CMD52 Read */
        ret = SdioDev_Readb(pstRk903Dev->hFunc[func], regaddr);
        if (ret >= 0)
        {
            *byte = (uint8)ret;
        }
    }

    return RK_SUCCESS;

}

/*******************************************************************************
** Name: Rk903DevIntIsr
** Input:void
** Return: void
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN void Rk903DevIntIsr(void)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: Rk903Dev_Write
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON API rk_err_t Rk903Dev_Write(HDC dev)
{

}



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(write) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: dhdsdio_write_vars
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 11:00:39
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t dhdsdio_write_vars(RK903_DEVICE_CLASS * pstRk903Dev)
{
    int bcmerror = 0;
    uint32 varsize;
    uint32 varaddr;
    uint8 *vbuffer;
    uint32 varsizew;

    /* Even if there are no vars are to be written, we still need to set the ramsize. */
    varsize = pstRk903Dev->varsz ? ROUNDUP(pstRk903Dev->varsz, 4) : 0;
    varaddr = (pstRk903Dev->ramsize - 4) - varsize;

    if (pstRk903Dev->vars)
    {
        if ((pstRk903Dev->sih->pub.buscoretype == SDIOD_CORE_ID) && (pstRk903Dev->sdpcmrev == 7))
        {
            if (((varaddr & 0x3C) == 0x3C) && (varsize > 4))
            {
                varsize += 4;
                varaddr -= 4;
            }
        }

        vbuffer = (uint8 *)rkos_memory_malloc(varsize);
        if (!vbuffer)
            return RK_ERROR;

        memset(vbuffer, 0, varsize);
        memcpy(vbuffer, pstRk903Dev->vars, pstRk903Dev->varsz);



        /* Write the vars list */
        bcmerror = dhdsdio_membytes(pstRk903Dev, TRUE, varaddr, vbuffer, varsize);
        rkos_memory_free(vbuffer);
    }

    /* adjust to the user specified RAM */
    varsize = ((pstRk903Dev->orig_ramsize - 4) - varaddr);

    /*
     * Determine the length token:
     * Varsize, converted to words, in lower 16-bits, checksum in upper 16-bits.
     */
    if (bcmerror)
    {
        varsizew = 0;
    }
    else
    {
        varsizew = varsize / 4;
        varsizew = (~varsizew << 16) | (varsizew & 0x0000FFFF);
        //varsizew = htol32(varsizew);
    }

    /* Write the length token to the last word */
    bcmerror = dhdsdio_membytes(pstRk903Dev, TRUE, (pstRk903Dev->orig_ramsize - 4), (uint8*)&varsizew, 4);

    return bcmerror;
}

/*******************************************************************************
** Name: bcmsdh_reg_write
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size, uint32 data
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:45:39
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_reg_write(RK903_DEVICE_CLASS * pstRk903Dev, uint32 addr, uint size, uint32 data)
{
    rk_err_t ret;

    ret = bcmsdh_set_sbaddr_window(pstRk903Dev, addr, FALSE);
    if (ret)
        return ret;

    addr &= SBSDIO_SB_OFT_ADDR_MASK;
    if (size == 4)
        addr |= SBSDIO_SB_ACCESS_2_4B_FLAG;

    ret = sdioh_request_word(pstRk903Dev, SDIOH_WRITE, SDIO_FUN_1,
                             addr, &data, size);

    return ret;
}

/*******************************************************************************
** Name: bcmsdh_cfg_write
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 *data
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 14:47:27
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_err_t bcmsdh_cfg_write(RK903_DEVICE_CLASS * pstRk903Dev, uint32 fnc_num, uint32 addr, uint8 data)
{
    return sdioh_request_byte(pstRk903Dev, SDIOH_WRITE, fnc_num, addr, &data);
}


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: Rk903Dev_Create
** Input:void * arg
** Return: HDC
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT API HDC Rk903Dev_Create(uint32 DevID, void * arg)
{

    DEVICE_CLASS * pstDev;
    RK903_DEVICE_CLASS * pstRk903Dev;
    RK903_DEV_ARG * pstRk903Arg = (RK903_DEV_ARG *)arg;

    rk_printf("Rk903Dev_Create DevID = %d", DevID);
    if (arg == NULL)
    {
        return (HDC)(RK_PARA_ERR);
    }

    pstRk903Dev =  rkos_memory_malloc(sizeof(RK903_DEVICE_CLASS));
    if (pstRk903Dev == NULL)
    {
        return pstRk903Dev;
    }
	memset(pstRk903Dev, 0, sizeof(RK903_DEVICE_CLASS));
    pstRk903Dev->prot = &pstRk903Dev->_prot_;
    pstRk903Dev->cur_cmd_cmdbuf = rkos_memory_malloc(2048);

    if(pstRk903Dev->cur_cmd_cmdbuf == NULL)
    {
        rkos_memory_free(pstRk903Dev);
        return (HDC) RK_ERROR;
    }

    pstRk903Dev->osRk903OperReqSem  = rkos_semaphore_create(1,1);
    pstRk903Dev->cmdSem  = rkos_semaphore_create(1,0);
    if (pstRk903Dev->osRk903OperReqSem == 0)
    {
        rkos_semaphore_delete(pstRk903Dev->osRk903OperReqSem);
        rkos_memory_free(pstRk903Dev);
        return (HDC) RK_ERROR;
    }

    pstDev = (DEVICE_CLASS *)pstRk903Dev;

    pstDev->suspend = Rk903DevSuspend;
    pstDev->resume  = Rk903DevResume;

    pstRk903Dev->hSdio = pstRk903Arg->hSdio;
    gpstRk903DevISR[DevID] = pstRk903Dev;
    pstRk903Dev->ipend = 1;

    #ifdef __SYSTEM_FWANALYSIS_FWANALYSIS_C__
	FW_ModuleOverlay(MODULE_ID_RK903_DEV, MODULE_OVERLAY_CODE);
    #endif

    if (Rk903DevInit(pstRk903Dev) != RK_SUCCESS)
    {
        rkos_semaphore_delete(pstRk903Dev->osRk903OperReqSem);

        rkos_semaphore_delete(pstRk903Dev->cmdSem);
        rkos_memory_free(pstRk903Dev);
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
/*******************************************************************************
** Name: si_buscore_setup
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 BusType
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.24
** Time: 8:36:28
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_buscore_setup(RK903_DEVICE_CLASS * pstRk903Dev,  SI_INFO * sih,  uint32 savewin,  uint *origidx)
{
	uint i;
    chipcregs_t *cc;
    rk_err_t ret;

	cc = si_setcoreidx(sih, SI_CC_IDX);

	/* get chipcommon rev */
	sih->pub.ccrev = (int)si_corerev(sih);

	/* get chipcommon chipstatus */
	if (sih->pub.ccrev >= 11)
	{
		ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->chipstatus), sizeof(cc->chipstatus), &sih->pub.chipst);
        if(ret)
            return RK_ERROR;
    }
	/* get chipcommon capabilites */
	 ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->capabilities), sizeof(cc->capabilities), &sih->pub.cccaps);
     if(ret)
           return RK_ERROR;
	/* get chipcommon extended capabilities */

	if (sih->pub.ccrev >= 35)
		ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->capabilities_ext), sizeof(cc->capabilities_ext), &sih->pub.cccaps_ext);
        if(ret)
            return RK_ERROR;

	/* get pmu rev and caps */
	if (sih->pub.cccaps & CC_CAP_PMU)
	{
		ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->pmucapabilities), sizeof(cc->pmucapabilities), &sih->pub.pmucaps);
        if(ret)
            return RK_ERROR;

		sih->pub.pmurev = sih->pub.pmucaps & PCAP_REV_MASK;
	}

	/* figure out bus/orignal core idx */
	sih->pub.buscoretype = NODEV_CORE_ID;
	sih->pub.buscorerev = (uint)NOREV;
	sih->pub.buscoreidx = BADIDX;

	for (i = 0; i < sih->numcores; i++)
	{
		uint cid, crev;

		si_setcoreidx(sih, i);
		cid = si_coreid(sih);
		crev = si_corerev(sih);

		/* Display cores found */
		if((sih->pub.bustype == SDIO_BUS) && (cid == SDIOD_CORE_ID))
		{
			sih->pub.buscorerev = crev;
			sih->pub.buscoretype = cid;
			sih->pub.buscoreidx = i;
			//printf("buscoretype=0x%X\n", sii->pub.buscoretype);
		}

		/* find the core idx before entering this func. */
		if((savewin && (savewin == sih->coresba[i])) || (pstRk903Dev->sbwad == (uint32)sih->regs[i]))
		{
				*origidx = i;
		}
	}

	/* Make sure any on-chip ARM is off (in case strapping is wrong), or downloaded code was
	 * already running.
	 */
	if(sih->pub.bustype == SDIO_BUS)
	{
		if(si_setcore(sih, ARM7S_CORE_ID, 0) || si_setcore(sih, ARMCM3_CORE_ID, 0))
		{
			 si_core_disable(pstRk903Dev, sih, 0);
		}
	}

	/* return to the original core */
	si_setcoreidx(sih, *origidx);

	return RK_SUCCESS;

}

/*******************************************************************************
** Name: get_asd
** Input:void
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.23
** Time: 17:59:42
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 get_asd(RK903_DEVICE_CLASS * pstRk903Dev, uint32 **eromptr, uint sp, uint ad, uint st, uint32 *addrl, uint32 *addrh,
        uint32 *sizel, uint32 *sizeh)
{

	uint32 asd, sz, szd;

	asd = get_erom_ent(pstRk903Dev, eromptr, ER_VALID, ER_VALID);
	if (((asd & ER_TAG1) != ER_ADD) ||
	    (((asd & AD_SP_MASK) >> AD_SP_SHIFT) != sp) ||
	    ((asd & AD_ST_MASK) != st))
	{
		(*eromptr)--;
		return 0;
	}

	*addrl = asd & AD_ADDR_MASK;
	if (asd & AD_AG32)
	{
		*addrh = get_erom_ent(pstRk903Dev, eromptr, 0, 0);
	}
	else
	{
		*addrh = 0;
	}

	*sizeh = 0;
	sz = asd & AD_SZ_MASK;
	if (sz == AD_SZ_SZD)
	{
		szd = get_erom_ent(pstRk903Dev, eromptr, 0, 0);
		*sizel = szd & SD_SZ_MASK;
		if (szd & SD_SG32)
		{
			*sizeh = get_erom_ent(pstRk903Dev, eromptr, 0, 0);
		}
	}
	else
	{
		*sizel = AD_SZ_BASE << (sz >> AD_SZ_SHIFT);
	}

	return asd;

}

/*******************************************************************************
** Name: get_erom_ent
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 **eromptr, uint32 mask, uint32 match
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.23
** Time: 17:53:51
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 get_erom_ent(RK903_DEVICE_CLASS * pstRk903Dev, uint32 **eromptr, uint32 mask, uint32 match)
{
	uint32 ent;
	uint inv = 0, nom = 0;

	while (TRUE)
	{
      bcmsdh_reg_read(pstRk903Dev, (uint)*eromptr, sizeof(**eromptr), &ent);

		(*eromptr)++;

		if (mask == 0)
			break;

		if ((ent & ER_VALID) == 0)
		{
			inv++;
			continue;
		}

		if (ent == (ER_END | ER_VALID))//end flag.dgl
			break;

		if ((ent & mask) == match)
			break;

		nom++;
	}

	return ent;
}

/*******************************************************************************
** Name: ai_scan
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.23
** Time: 17:38:02
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t ai_scan(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih)
{

    rk_err_t ret;

	chipcregs_t *cc = (chipcregs_t *)pstRk903Dev->sbwad;
	uint32 erombase, *eromptr, *eromlim;

	ret = bcmsdh_reg_read(pstRk903Dev, (uint)&cc->eromptr, sizeof(cc->eromptr), &erombase);
    if(ret)
        return RK_ERROR;

	switch (sih->pub.bustype)
	{
		case SDIO_BUS:
			eromptr = (uint32 *)erombase;
			break;

		//case SI_BUS:
		//case PCI_BUS:
		//case SPI_BUS:
		//case PCMCIA_BUS:
		default:
			return RK_ERROR;
	}
	eromlim = eromptr + (ER_REMAPCONTROL / sizeof(uint32));

	while (eromptr < eromlim)
	{
		uint32 cia, cib, cid, mfg, crev, nmw, nsw, nmp, nsp;
		uint32 mpd, asd, addrl, addrh, sizel, sizeh;
		uint32 *base;
		uint i, j, idx;
		bool br;

		br = FALSE;


		cia = get_erom_ent(pstRk903Dev, &eromptr, ER_TAG, ER_CI);
		if (cia == (ER_END | ER_VALID))
		{
			return RK_SUCCESS;
		}
		base = eromptr - 1;

		cib = get_erom_ent(pstRk903Dev, &eromptr, 0, 0);
		if ((cib & ER_TAG) != ER_CI)
			goto error;

		cid =  (cia & CIA_CID_MASK) >> CIA_CID_SHIFT;
		mfg =  (cia & CIA_MFG_MASK) >> CIA_MFG_SHIFT;
		crev = (cib & CIB_REV_MASK) >> CIB_REV_SHIFT;
		nmw =  (cib & CIB_NMW_MASK) >> CIB_NMW_SHIFT;
		nsw =  (cib & CIB_NSW_MASK) >> CIB_NSW_SHIFT;
		nmp =  (cib & CIB_NMP_MASK) >> CIB_NMP_SHIFT;
		nsp =  (cib & CIB_NSP_MASK) >> CIB_NSP_SHIFT;

		if (((mfg == MFGID_ARM) && (cid == DEF_AI_COMP)) || (nsp == 0))
			continue;

		if ((nmw + nsw == 0))
		{
			if (cid == OOB_ROUTER_CORE_ID)
			{
				asd = get_asd(pstRk903Dev, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
				if (asd != 0)
				{
					sih->oob_router = addrl;
				}
			}
			continue;
		}

		idx = sih->numcores;

		sih->cia[idx] = cia;
		sih->cib[idx] = cib;
		sih->coreid[idx] = cid;

		for (i = 0; i < nmp; i++)
		{
			mpd = get_erom_ent(pstRk903Dev, &eromptr, ER_VALID, ER_VALID);
			if ((mpd & ER_TAG) != ER_MP)
			{
				goto error;
			}
		}

		asd = get_asd(pstRk903Dev, &eromptr, 0, 0, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
		if (asd == 0)
		{
			asd = get_asd(pstRk903Dev, &eromptr, 0, 0, AD_ST_BRIDGE, &addrl, &addrh, &sizel, &sizeh);
			if (asd != 0)
				br = TRUE;
			else
			{
				if ((addrh != 0) || (sizeh != 0) || (sizel != SI_CORE_SIZE))
				{
					goto error;
				}
			}
		}
		sih->coresba[idx] = addrl;
		sih->coresba_size[idx] = sizel;

		j = 1;
		do
		{
			asd = get_asd(pstRk903Dev, &eromptr, 0, j, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
			if ((asd != 0) && (j == 1) && (sizel == SI_CORE_SIZE))
			{
				sih->coresba2[idx] = addrl;
				sih->coresba2_size[idx] = sizel;
			}
			j++;
		} while (asd != 0);


		for (i = 1; i < nsp; i++)
		{
			j = 0;
			do
			{
				asd = get_asd(pstRk903Dev, &eromptr, i, j++, AD_ST_SLAVE, &addrl, &addrh, &sizel, &sizeh);
			} while (asd != 0);
			if (j == 0)
			{
				goto error;
			}
		}


		for (i = 0; i < nmw; i++)
		{
			asd = get_asd(pstRk903Dev, &eromptr, i, 0, AD_ST_MWRAP, &addrl, &addrh, &sizel, &sizeh);
			if (asd == 0)
			{
				goto error;
			}
			if ((sizeh != 0) || (sizel != SI_CORE_SIZE))
			{
				goto error;
			}
			if (i == 0)
			{
				sih->wrapba[idx] = addrl;
			}
		}

		for (i = 0; i < nsw; i++)
		{
			uint fwp = (nsp == 1) ? 0 : 1;
			asd = get_asd(pstRk903Dev, &eromptr, fwp + i, 0, AD_ST_SWRAP, &addrl, &addrh, &sizel, &sizeh);
			if (asd == 0)
			{
				goto error;
			}
			if ((sizeh != 0) || (sizel != SI_CORE_SIZE))
			{
				goto error;
			}
			if ((nmw == 0) && (i == 0))
			{
				sih->wrapba[idx] = addrl;
			}
		}

		if (br)
			continue;
        rk_printf("core:%d id=0x%3x", idx, sih->coreid[idx]);
		sih->numcores++;
	}

error:
	sih->numcores = 0;
	return RK_ERROR;
}

/*******************************************************************************
** Name: si_buscore_prep
** Input:RK903_DEVICE_CLASS * pstRk903,  uint bustype
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.23
** Time: 17:06:12
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_buscore_prep(RK903_DEVICE_CLASS * pstRk903Dev,  uint bustype)
{
    rk_err_t ret;
    uint8 clkset;
	uint8 clkval;

	if (bustype != SDIO_BUS)
		return RK_ERROR;

	/* Try forcing SDIO core to do ALPAvail request only */
	clkset = SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_ALP_AVAIL_REQ;
	ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, clkset);
	if (!ret)
	{
		/* If register supported, wait for ALPAvail and then force ALP */
		ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &clkval);
		if ((clkval & ~SBSDIO_AVBITS) == clkset)
		{
			SPINWAIT(((bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1,
				      SBSDIO_FUNC1_CHIPCLKCSR, &clkval)), !SBSDIO_ALPAV(clkval)),
				      PMU_MAX_TRANSITION_DLY);

			if (!SBSDIO_ALPAV(clkval))
				return RK_ERROR;

			clkset = SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_FORCE_ALP;
			ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, clkset);
            if(ret)
                return RK_ERROR;

			DelayUs(65);
		}
	}

	/* Also, disable the extra SDIO pull-ups */
	ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SDIOPULLUP, 0);
    if(ret)
        return RK_ERROR;

	return RK_SUCCESS;

}

/* For Android ICS MR2 release, the concurrent mode is enabled by default and the firmware
 * name would be fw_bcmdhd.bin. So we need to determine whether P2P is enabled in the STA
 * firmware and accordingly enable concurrent mode (Apply P2P settings). SoftAP firmware
 * would still be named as fw_bcmdhd_apsta.
 */

/*******************************************************************************
** Name: bcm_mkiovar
** Input:char *name, char *data, uint datalen, char *buf, uint buflen
** Return: rk_size_t
** Owner:zhuzhe
** Date: 2014.7.9
** Time: 18:49:36
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
COMMON FUN rk_size_t bcm_mkiovar(char *name, char *data, uint datalen, char *buf, uint buflen)
{
		rk_size_t len;

		len = strlen(name) + 1;

		if ((len + datalen) > buflen)
				return 0;

		strncpy(buf, name, buflen);

		/* append data onto the end of the name string */
		memcpy(&buf[len], data, datalen);

		len += datalen;

		return len;
}

_DRIVER_RK903_RK903DEVICE_INIT_
static uint32 dhd_concurrent_fw(RK903_DEVICE_CLASS * pstRk903Dev)
{
	int ret = 0;
	char buf[WLC_IOCTL_SMLEN];

	/* Given path is for the STA firmware. Check whether P2P support is present in
	 * the firmware. If so, set mode as P2P (concurrent support).
	 */
	memset(buf, 0, sizeof(buf));
	bcm_mkiovar("p2p", 0, 0, buf, sizeof(buf));
	ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_GET_VAR, buf, sizeof(buf), 0);
	if (ret < 0)
	{
		//printf("Get P2P failed (error=%d)\n", ret);
	}
	else if (buf[0] == 1)
	{
		//printf("P2P is supported\n");
		return 1;
	}
	return 0;
}



/*******************************************************************************
** Name: dhd_preinit_ioctls
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 14:11:46
*******************************************************************************/
#define DHD_SCAN_ACTIVE_TIME	 40 /* ms : Embedded default Active setting from DHD Driver */
#define DHD_SCAN_PASSIVE_TIME	130 /* ms: Embedded default Passive setting from DHD Driver */
#define LISTEN_INTERVAL			10

#define PM_OFF  0
#define PM_MAX  1
#define PM_FAST 2

/* Firmware requested operation mode */
#define STA_MASK			0x0001
#define HOSTAPD_MASK			0x0002
#define WFD_MASK			0x0004
#define SOFTAP_FW_MASK			0x0008

char INIT_COUNTRY_CODE[] = "EU";

_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhd_preinit_ioctls(RK903_DEVICE_CLASS * pstRk903Dev)
{

	int ret = 0;
	char eventmask[WL_EVENTING_MASK_LEN];
	char iovbuf[WL_EVENTING_MASK_LEN + 12];	/*  Room for "event_msgs" + '\0' + bitvec  */
	uint up = 0;
	uint power_mode = PM_FAST;
	uint32 dongle_align = DHD_SDALIGN;
	uint32 glom = 0;
	uint bcn_timeout = 10;
    uint retry_max = 15;
	//int arpoe = 1;
	int scan_assoc_time = DHD_SCAN_ACTIVE_TIME;
	int scan_unassoc_time = 40;
	int scan_passive_time = DHD_SCAN_PASSIVE_TIME;
	char buf[WLC_IOCTL_SMLEN];
	char *ptr;
	uint32 listen_interval = LISTEN_INTERVAL; /* Default Listen Interval in Beacons */
	//uint dtim = 1;
	uint32 mpc = 0; /* Turn MPC off for AP/APSTA mode */
	uint32 roamvar;
	uint32 sgi_tx, sgi_rx;
	uint32 apsta = 0; /* Enable APSTA mode */
	int res;

	pstRk903Dev->op_mode = 0;

	/* Get the default device MAC address directly from firmware */
	memset(buf, 0, sizeof(buf));
	bcm_mkiovar("cur_etheraddr", 0, 0, buf, sizeof(buf));
	if ((ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_GET_VAR, buf, sizeof(buf), 0)) < 0)
		return BCME_NOTUP;

	/* Update public MAC address after reading from Firmware */
	memcpy((void *)pstRk903Dev->mac.octet, (void *)buf, ETHER_ADDR_LEN);
    memcpy(pstRk903Dev->EthMac, buf, ETHER_MAC_LEN);

    //wifi_set_mac_addr(buf);

	/* Check if firmware with WFD support used */
	if (dhd_concurrent_fw(pstRk903Dev))
	{
		bcm_mkiovar("apsta", (char *)&apsta, 4, iovbuf, sizeof(iovbuf));
		ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);
		if (ret < 0)
		{
			rk_printf("APSTA for WFD failed ret= %d\n", ret);
		}
		else
		{
			pstRk903Dev->op_mode |= WFD_MASK;
			//dhd_pkt_filter_enable = FALSE;
		}
	}

	/* Check if firmware with HostAPD support used */
	if ((pstRk903Dev->op_mode != WFD_MASK) && (pstRk903Dev->op_mode != HOSTAPD_MASK))
	{
		/* STA only operation mode */
		pstRk903Dev->op_mode |= STA_MASK;
		//dhd_pkt_filter_enable = TRUE;
	}

	rk_printf("Firmware up: op_mode=%d, mac=%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
			pstRk903Dev->op_mode,
			pstRk903Dev->mac.octet[0], pstRk903Dev->mac.octet[1], pstRk903Dev->mac.octet[2],
			pstRk903Dev->mac.octet[3], pstRk903Dev->mac.octet[4], pstRk903Dev->mac.octet[5]);

	/* Set Country code  */
	/*
	if (dhd->dhd_cspec.ccode[0] == 0) {
			memcpy(dhd->dhd_cspec.country_abbrev, "ALL", WLC_CNTRY_BUF_SZ);
			memcpy(dhd->dhd_cspec.ccode, "ALL", WLC_CNTRY_BUF_SZ);
			dhd->dhd_cspec.rev = 0;
	}*/

	memcpy(pstRk903Dev->dhd_cspec.ccode, INIT_COUNTRY_CODE, WLC_CNTRY_BUF_SZ);
	if (pstRk903Dev->dhd_cspec.ccode[0] != 0)
	{
		bcm_mkiovar("country", (char *)&pstRk903Dev->dhd_cspec, sizeof(wl_country_t), iovbuf, sizeof(iovbuf));
		ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);
		if (ret < 0)
		{
			//printf("country code setting failed\n");
		}
		//printf("Wifi driver initialize Country Code : %s\n", INIT_COUNTRY_CODE);
	}

	/* Set Listen Interval */
	bcm_mkiovar("assoc_listen", (char *)&listen_interval, 4, iovbuf, sizeof(iovbuf));
	ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);
	if (ret < 0)
	{
		//printf("assoc_listen failed %d\n", ret);
	}

	/* Set PowerSave mode */
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_PM, (char *)&power_mode, sizeof(power_mode), 1);

	/* Match Host and Dongle rx alignment */
	bcm_mkiovar("bus:txglomalign", (char *)&dongle_align, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	/* disable glom option per default */
	bcm_mkiovar("bus:txglom", (char *)&glom, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	/* Setup timeout if Beacons are lost and roam is off to report link down */
	bcm_mkiovar("bcn_timeout", (char *)&bcn_timeout, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	/* Setup assoc_retry_max count to reconnect target AP in dongle */
	rk_printf("0 retry max = %d\n", retry_max);
	bcm_mkiovar("assoc_retry_max", (char *)&retry_max, 4, iovbuf, sizeof(iovbuf));
	while(dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1))
	{
		DelayMs(5);
	    rk_printf("retry max = %d\n", retry_max);
	}

	/* Turn off MPC in AP mode */
	bcm_mkiovar("mpc", (char *)&mpc, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	bcm_mkiovar("apsta", (char *)&apsta, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	/* Set Keep Alive : be sure to use FW with -keepalive */
	//if (ap_fw_loaded == FALSE)
	if (1)
    {
		res = Rk903DevKeepAliveOnoff(pstRk903Dev); //zhu..........................
		if (res < 0)
		    rk_printf("set keeplive failed %d\n", res);
	}

	/* Read event_msgs mask */
	bcm_mkiovar("event_msgs", eventmask, WL_EVENTING_MASK_LEN, iovbuf, sizeof(iovbuf));
	ret  = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_GET_VAR, iovbuf, sizeof(iovbuf), 0);
	if (ret < 0)
	{
		//printf("read Event mask failed %d\n", ret);
		goto done;
	}
	bcopy(iovbuf, eventmask, WL_EVENTING_MASK_LEN);

	/* Setup event_msgs */
	setbit(eventmask, WLC_E_SET_SSID);
	setbit(eventmask, WLC_E_PRUNE);
	setbit(eventmask, WLC_E_AUTH);
	setbit(eventmask, WLC_E_REASSOC);
	setbit(eventmask, WLC_E_REASSOC_IND);
	setbit(eventmask, WLC_E_DEAUTH);
	setbit(eventmask, WLC_E_DEAUTH_IND);
	setbit(eventmask, WLC_E_DISASSOC_IND);
	setbit(eventmask, WLC_E_DISASSOC);
	setbit(eventmask, WLC_E_JOIN);
	setbit(eventmask, WLC_E_ASSOC_IND);
	setbit(eventmask, WLC_E_PSK_SUP);
	setbit(eventmask, WLC_E_LINK);
	setbit(eventmask, WLC_E_NDIS_LINK);
	setbit(eventmask, WLC_E_MIC_ERROR);
	setbit(eventmask, WLC_E_ASSOC_REQ_IE);
	setbit(eventmask, WLC_E_ASSOC_RESP_IE);
	setbit(eventmask, WLC_E_PMKID_CACHE);
	//setbit(eventmask, WLC_E_TXFAIL);
	setbit(eventmask, WLC_E_JOIN_START);
	setbit(eventmask, WLC_E_SCAN_COMPLETE);
	setbit(eventmask, WLC_E_PFN_NET_FOUND);
	/* enable dongle roaming event */
	//setbit(eventmask, WLC_E_ROAM); //turning off  roaming (gwl)
	setbit(eventmask, WLC_E_ASSOCREQ_IE);

	setbit(eventmask, WLC_E_ESCAN_RESULT);
	if ((pstRk903Dev->op_mode & WFD_MASK) == WFD_MASK)
	{
		setbit(eventmask, WLC_E_ACTION_FRAME_RX);
		setbit(eventmask, WLC_E_ACTION_FRAME_COMPLETE);
		setbit(eventmask, WLC_E_ACTION_FRAME_OFF_CHAN_COMPLETE);
		//setbit(eventmask, WLC_E_P2P_PROBREQ_MSG);//wp too much
		setbit(eventmask, WLC_E_P2P_DISC_LISTEN_COMPLETE);
	}
	/* Write updated Event mask */
	bcm_mkiovar("event_msgs", eventmask, WL_EVENTING_MASK_LEN, iovbuf, sizeof(iovbuf));
	if ((ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1)) < 0)
	{
		rk_printf("Set Event mask failed %d\n", ret);
		goto done;
	}


    rk_printf("WLC_SET_SCAN_CHANNEL_TIME");
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_SCAN_CHANNEL_TIME, (char *)&scan_assoc_time, sizeof(scan_assoc_time), 1);
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_SCAN_UNASSOC_TIME, (char *)&scan_unassoc_time, sizeof(scan_unassoc_time), 1);
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_SCAN_PASSIVE_TIME, (char *)&scan_passive_time, sizeof(scan_passive_time), 1);

	/*dgl move to here for Force STA UP will fail.*/
	//roamvar = dhd_roam_disable;
	roamvar = 1;
	bcm_mkiovar("roam_off", (char *)&roamvar, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(pstRk903Dev, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);

	/* Force STA UP */
	if ((ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_UP, (char *)&up, sizeof(up), 1)) < 0)
	{
		rk_printf("Setting WL UP failed %d\n", ret);
		goto done;
	}

	/* query for 'ver' to get version info from firmware */
	memset(buf, 0, sizeof(buf));
	ptr = buf;
	bcm_mkiovar("ver", (char *)&buf, 4, buf, sizeof(buf));
	ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_GET_VAR, buf, sizeof(buf), 0);
	if (ret < 0)
	{
		rk_printf("failed %d\n", ret);
	}
	else
	{
		//bcmstrtok(&ptr, "\n", 0);
		/* Print fw version info */
		//printf("Firmware version = %s\n", buf);

		//Firmware version = wl0: Jul 31 2012 10:45:17 version 5.90.195.26.3.1_edon/off_0xb8bf FWID 01-ff91e1fa
	}
	#if 0
	roamvar = dhd_roam_disable;
	bcm_mkiovar("roam_off", (char *)&roamvar, 4, iovbuf, sizeof(iovbuf));
	dhd_wl_ioctl_cmd(dhd, WLC_SET_VAR, iovbuf, sizeof(iovbuf), 1);
	#endif
done:
	return ret;
}

/*******************************************************************************
** Name: dhdcdc_send_ioctl
** Input:RK903_DEVICE_CLASS *dhd, uint cmd, void *buf, uint len, uint8 action
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.19
** Time: 11:51:11
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
INIT FUN rk_err_t dhdcdc_send_ioctl(RK903_DEVICE_CLASS *pstRk903Dev, uint cmd, void *buf, uint len, uint8 action)
{

    sk_buff * skbp;
    int ret;
    uint32 flags;
    DHD_PROT *prot = pstRk903Dev->prot;
    cdc_ioctl_t *msg = PKTPUSH(buf, sizeof(cdc_ioctl_t));

    pstRk903Dev->prot->msg = msg;

    msg->cmd = htol32(cmd);
    msg->len = htol32(len);
    msg->flags = (++prot->reqid << CDCF_IOC_ID_SHIFT);
    CDC_SET_IF_IDX(msg, 0);//dgl int ifidx ==0 always
    if (action & WL_IOCTL_ACTION_SET)
    {
        action &= WL_IOCTL_ACTION_MASK;/* add additional action bits */
        msg->flags |= (action << CDCF_IOC_ACTION_SHIFT) | CDCF_IOC_SET;
    }
    else
    {
        action &= WL_IOCTL_ACTION_MASK;/* add additional action bits */
        msg->flags |= (action << CDCF_IOC_ACTION_SHIFT);
    }
    msg->flags = htol32(msg->flags);
    msg->status = 0;

    ret = dhdcdc_msg(pstRk903Dev);
    if (ret < 0)
    {
        pstRk903Dev->busstate = DHD_BUS_DOWN;
        goto done;
    }

    /* wait for interrupt and get first fragment */
    ret = dhdcdc_cmplt(pstRk903Dev, prot->reqid, len);
    if (ret < 0)
    {
        goto done;
    }

    /* Check info buffer*/
    skbp = (sk_buff *)buf;
    if (ret < (int)len)
    {
        len = ret;
    }
    /*bus->rxctl += doff;=>move to the header of "cdc_ioctrl_t+msg" postion.*/
    skbp->data = (uint8 *)(msg) + sizeof(cdc_ioctl_t);
    skbp->len = len;

    /* Check the ERROR flag */
    flags = ltoh32(msg->flags);
    if (flags & CDCF_IOC_ERROR)
    {
        ret = ltoh32(msg->status);
        pstRk903Dev->dongle_error = ret;//Cache error from dongle
    }

done:
    return ret;

}

/*******************************************************************************
** Name: dhd_bus_init
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 17:06:43
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhd_bus_init(RK903_DEVICE_CLASS * pstRk903Dev)
{
    uint timeout;
    uint8 ready, enable;
    uint8 saveclk;
    rk_err_t ret;

    /* Make sure backplane clock is on, needed to generate F2 interrupt */
    dhdsdio_clkctl(pstRk903Dev, CLK_AVAIL, FALSE);
    if (pstRk903Dev->clkstate != CLK_AVAIL)
        goto exit;

    /* Force clocks on backplane to be sure F2 interrupt propagates */
    ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &saveclk);
    if(!ret)
    {
        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR,
                               (saveclk | SBSDIO_FORCE_HT));
    }
    if (ret)
        goto exit;

    /* Enable function 2 (frame transfers) */
    ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&pstRk903Dev->regs->tosbmailboxdata,
                           sizeof(pstRk903Dev->regs->tosbmailboxdata), (SDPCM_PROT_VERSION << SMB_DATA_VERSION_SHIFT));
    if (ret)
        goto exit;

    enable = (SDIO_FUNC_ENABLE_1 | SDIO_FUNC_ENABLE_2);

    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_0, SDIO_CCCR_IOEx, enable);//enable in this function..dgl
    if (ret)
        goto exit;

    /* Give the dongle some time to do its thing and set IOR2 */
    ready = 0;
    timeout = 0;
    while (ready != enable)
    {
        ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_0, SDIO_CCCR_IORx, &ready);
        if (ret)
            goto exit;

        if (timeout++ > DHD_WAIT_F2RDY)
            break;

        DelayUs(1000);
    }


    /* If F2 successfully enabled, set core and enable interrupts */
    if (ready == enable)
    {
        /* Make sure we're talking to the core. */
        pstRk903Dev->regs = si_setcore(pstRk903Dev->sih, SDIOD_CORE_ID, 0);

        /* Set up the interrupt mask and enable interrupts */
        pstRk903Dev->hostintmask = HOSTINTMASK;
        ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&pstRk903Dev->regs->hostintmask, sizeof(pstRk903Dev->regs->hostintmask), pstRk903Dev->hostintmask);
        if (ret)
            goto exit;


        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_WATERMARK, WATER_MARK);
        if (ret)
            goto exit;

        /* Set bus state according to enable result */
        pstRk903Dev->busstate = DHD_BUS_DATA;

        pstRk903Dev->intdis = FALSE;
    }
    else
    {
        /* Disable F2 again */
        enable = SDIO_FUNC_ENABLE_1;
        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_0, SDIO_CCCR_IOEx, enable);
        if (ret)
            goto exit;
    }
    /* Restore previous clock setting */
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, saveclk);
    if (ret)
        goto exit;


    /* If we didn't come up, turn off backplane clock */
    if (pstRk903Dev->busstate != DHD_BUS_DATA)
        dhdsdio_clkctl(pstRk903Dev, CLK_NONE, FALSE);

exit:

    return ret;
}

/*******************************************************************************
** Name: dhdsdio_downloadvars
** Input:RK903_DEVICE_CLASS * pstRk903Dev, void *arg, int len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 16:51:03
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_downloadvars(RK903_DEVICE_CLASS * pstRk903Dev, void *arg, int len)
{

    rk_err_t ret;

    /* Basic sanity checks */
    if (pstRk903Dev->up)
    {
        ret = RK_ERROR;
        goto err;
    }

    if (!len)
    {
        ret = RK_ERROR;
        goto err;
    }

    /* Free the old ones and replace with passed variables */
    if (pstRk903Dev->vars)
    {
        rkos_memory_free(pstRk903Dev->vars);
        pstRk903Dev->vars = NULL;
    }

    pstRk903Dev->vars = rkos_memory_malloc(len);
    pstRk903Dev->varsz = pstRk903Dev->vars? len : 0;
    if (pstRk903Dev->vars == NULL)
    {
        ret = RK_ERROR;
        goto err;
    }

    ret = RK_SUCCESS;

    /* Copy the passed variables, which should include the terminating double-null */
    memcpy(pstRk903Dev->vars, arg, pstRk903Dev->varsz);

err:

    return ret;

}

/*******************************************************************************
** Name: process_nvram_vars
** Input:uint8 *varbuf, uint32 len
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 16:45:08
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 process_nvram_vars(uint8 *varbuf, uint32 len)
{
    char *dp;
    bool findNewline;
    int column;
    unsigned int buf_len, n;
    unsigned int pad = 0;

    dp = varbuf;

    findNewline = FALSE;
    column = 0;

    for (n = 0; n < len; n++)
    {
        if (varbuf[n] == '\r')
            continue;

        if (findNewline && varbuf[n] != '\n')
            continue;

        findNewline = FALSE;
        if (varbuf[n] == '#')
        {
            findNewline = TRUE;
            continue;
        }
        if (varbuf[n] == '\n')
        {
            if (column == 0)
                continue;
            *dp++ = 0;
            column = 0;
            continue;
        }
        *dp++ = varbuf[n];
        column++;
    }
    buf_len = (unsigned int)(dp - varbuf);
    if (buf_len % 4)
    {
        pad = 4 - buf_len % 4;
        if (pad && (buf_len + pad <= len))
        {
            buf_len += pad;
        }
    }

    while (dp < varbuf + n)
        *dp++ = 0;

    return buf_len;
}


/*******************************************************************************
** Name: dhdsdio_download_nvram
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 16:42:01
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_download_nvram(RK903_DEVICE_CLASS * pstRk903Dev)
{

    uint len;
    uint32 page_addr;
    uint8 * memblock;
    char *bufp;
    int pnvpathfmsiz;
    MODULE_INFO_T stCodeInfo;
    rk_err_t ret;

    memblock = rkos_memory_malloc(MAX_NVRAMBUF_SIZE);
    if (memblock == NULL)
        return RK_ERROR;

    /* Download variables */
    ret = FW_GetModuleInfo(MODULE_ID_WIFI_FIRMAVE_TWO, &stCodeInfo);
    if(ret)
        return RK_ERROR;

    page_addr = stCodeInfo.CodeLoadBase;
    pnvpathfmsiz = stCodeInfo.CodeImageLength;
    FW_ReadResourceData(page_addr, memblock, pnvpathfmsiz);
    len = pnvpathfmsiz;

    if (len > 0 && len < MAX_NVRAMBUF_SIZE)
    {
        bufp = (char *)memblock;
        bufp[len] = 0;
        len = process_nvram_vars(bufp, len);
        if (len % 4)
        {
            len += 4 - (len % 4);
        }
        bufp += len;
        *bufp++ = 0;
        if (len)
        {
            ret = dhdsdio_downloadvars(pstRk903Dev, memblock, len + 1);
        }
    }
    else
    {
        ret = RK_ERROR;
    }

    if (memblock)
        rkos_memory_free(memblock);

    return ret;

}


/*******************************************************************************
** Name: ret
** Input:RK903_DEVICE_CLASS * pstRk903Dev, int fwsize
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 11:18:17
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_download_code_file(RK903_DEVICE_CLASS * pstRk903Dev)
{
    int bcmerror = -1;
    int offset;
    uint len;
    uint8 *memblock = NULL, *memptr;
    uint32 page_addr;
    MODULE_INFO_T stCodeInfo;
    int fwsize;
    rk_err_t ret;

    if (fwsize <= 0)
        goto err;

    memptr = memblock = rkos_memory_malloc(MEMBLOCK + DHD_SDALIGN);
    if (memblock == NULL)
        goto err;
    if ((uint)memblock % DHD_SDALIGN)
        memptr += (DHD_SDALIGN - ((uint)memblock % DHD_SDALIGN));

    /* Download image */
    ret = FW_GetModuleInfo(MODULE_ID_WIFI_FIRMAVE_ONE, &stCodeInfo);
    if(ret)
        goto err;

    page_addr = stCodeInfo.CodeLoadBase;
    fwsize = stCodeInfo.CodeImageLength;

    offset = 0;
    while (offset < fwsize)
    {
        len = min((fwsize - offset), MEMBLOCK);
        FW_ReadResourceData(page_addr + offset, memptr, len);
        bcmerror = dhdsdio_membytes(pstRk903Dev, TRUE, offset, memptr, len);
        if (bcmerror)
            goto err;
        offset += len;
    }
err:
    if (memblock)
        rkos_memory_free(memblock);

    return bcmerror;
}

/*******************************************************************************
** Name: dhdsdio_download_state
** Input:RK903_DEVICE_CLASS * pstRk903Dev, bool enter
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:51:52
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_download_state(RK903_DEVICE_CLASS * pstRk903Dev, bool enter)
{
    rk_err_t ret;

    /* To enter download state, disable ARM and reset SOCRAM.
     * To exit download state, simply reset ARM (default is RAM boot).
     */
    if (enter)
    {
        pstRk903Dev->alp_only = TRUE;

        if (!(si_setcore(pstRk903Dev->sih, ARM7S_CORE_ID, 0)) &&
                !(si_setcore(pstRk903Dev->sih, ARMCM3_CORE_ID, 0)))
        {
            goto fail;
        }

        ret = si_core_disable(pstRk903Dev, pstRk903Dev->sih, 0);
        if (ret)
        {
            goto fail;
        }

        if (!(si_setcore(pstRk903Dev->sih, SOCRAM_CORE_ID, 0)))
        {
            goto fail;
        }

        ret = si_core_reset(pstRk903Dev, 0, 0);
        if (ret)
        {
            goto fail;
        }

        /* Clear the top bit of memory */
        if (pstRk903Dev->ramsize)
        {
            uint32 zeros = 0;
            if (dhdsdio_membytes(pstRk903Dev, TRUE, pstRk903Dev->ramsize - 4, (uint8*)&zeros, 4) < 0)
            {
                goto fail;
            }
        }
    }
    else
    {
        /** To exit download state, simply reset ARM (default is RAM boot).*/
        if (!(si_setcore(pstRk903Dev->sih, SOCRAM_CORE_ID, 0)))
        {
            goto fail;
        }

        if (!si_iscoreup(pstRk903Dev))
        {
            goto fail;
        }

        ret = dhdsdio_write_vars(pstRk903Dev);
        if (ret)
            goto fail;

        if (!si_setcore(pstRk903Dev->sih, SDIOD_CORE_ID, 0))
        {
            goto fail;
        }

        ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&pstRk903Dev->regs->intstatus, sizeof(pstRk903Dev->regs->intstatus),0xFFFFFFFF);
        if (ret)
            goto fail;

        if (!(si_setcore(pstRk903Dev->sih, ARM7S_CORE_ID, 0)) &&
                !(si_setcore(pstRk903Dev->sih, ARMCM3_CORE_ID, 0)))
        {
            goto fail;
        }

        ret = si_core_reset(pstRk903Dev, 0, 0);
        if (ret)
        {
            goto fail;
        }

        /* Allow HT Clock now that the ARM is running. */
        pstRk903Dev->alp_only = FALSE;

        pstRk903Dev->busstate = DHD_BUS_LOAD;
    }

fail:
    /* Always return to SDIOD core */
    si_setcore(pstRk903Dev->sih, SDIOD_CORE_ID, 0);

    return ret;
}

/*******************************************************************************
** Name: _dhdsdio_download_firmware
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:49:08
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t _dhdsdio_download_firmware(RK903_DEVICE_CLASS * pstRk903Dev)
{
    rk_err_t ret =-1;

    bool dlok = FALSE;	/* download firmware succeeded */

    /* Keep arm in reset */
    if (dhdsdio_download_state(pstRk903Dev, TRUE))
    {
        goto err;
    }

	if ((NULL == wifi_info.pfirmware) || (NULL == wifi_info.pnvram))
	{
		rk_print_string("No firmeware.\n");
		goto err;
	}


    /* External image takes precedence if specified */
    if (dhdsdio_download_code_file(pstRk903Dev))
    {
        goto err;
    }
    else
    {
        /*successful download firmware..*/
        dlok = TRUE;
    }

    if (!dlok)
    {
        goto err;
    }

    /* EXAMPLE: nvram_array */
    /* If a valid nvram_arry is specified as above, it can be passed down to dongle */
    /* dhd_bus_set_nvram_params(bus, (char *)&nvram_array); */

    /* External nvram takes precedence if specified */
    if (dhdsdio_download_nvram(pstRk903Dev))
    {
        goto err;
    }

    /* Take arm out of reset */
    if (dhdsdio_download_state(pstRk903Dev, FALSE))
    {
        goto err;
    }

    ret = 0;

err:
    return ret;
}

/*******************************************************************************
** Name: dhdsdio_download_firmware
** Input:RK903_DEVICE_CLASS * pstRk903Dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 10:21:59
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_download_firmware(RK903_DEVICE_CLASS * pstRk903Dev)
{
    rk_err_t ret;

    /* Download the firmware */
    //dgl add interrupt disable
    //....
	//vTaskDelay(100);

    dhdsdio_clkctl(pstRk903Dev, CLK_AVAIL, FALSE);

	//vTaskDelay(100);

    ret = _dhdsdio_download_firmware(pstRk903Dev);

    //vTaskDelay(100);
    dhdsdio_clkctl(pstRk903Dev, CLK_SDONLY, FALSE);
    //dgl add interrupt enable
    //....

    return ret;
}

/*******************************************************************************
** Name: si_core_disable
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 8:57:28
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_core_disable(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih, uint32 bits)
{
    if (sih->pub.socitype == SOCI_SB)
        ;//sb_core_disable(sih, bits);
    else if (sih->pub.socitype == SOCI_AI)
        ai_core_disable(pstRk903Dev, sih, bits);
    else if (sih->pub.socitype == SOCI_UBUS)
        ;//ub_core_disable(sih, bits);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: socram_banksize
** Input:RK903_DEVICE_CLASS * pstRk903Dev, sbsocramregs_t *regs, uint8 index, uint8 mem_type
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.18
** Time: 8:44:02
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t socram_banksize(RK903_DEVICE_CLASS * pstRk903Dev, sbsocramregs_t *regs, uint8 index, uint8 mem_type, uint32 * word)
{
    uint banksize, bankinfo;
    uint bankidx = index | (mem_type << SOCRAM_BANKIDX_MEMTYPE_SHIFT);
    rk_err_t ret;

    ret = bcmsdh_reg_write(pstRk903Dev, (uint)(&regs->bankidx), sizeof(regs->bankidx), bankidx);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&regs->bankinfo), sizeof(regs->bankinfo), &bankinfo);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    banksize = SOCRAM_BANKINFO_SZBASE * ((bankinfo & SOCRAM_BANKINFO_SZMASK) + 1);

    *word = banksize;

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ai_core_disable
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:45:42
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t ai_core_disable(RK903_DEVICE_CLASS * pstRk903Dev, SI_INFO * sih, uint32 bits)
{

    aidmp_t *ai;
    uint32 resetctrl, ioctrl;
    rk_err_t ret;

    ai = sih->curwrap;
    ret = bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->resetctrl, sizeof(ai->resetctrl), &resetctrl);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    if (resetctrl & AIRC_RESET)
    {
        return RK_SUCCESS;
    }

    ret = bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), &bits);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret = bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), &ioctrl);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    DelayUs(10);
    ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&ai->resetctrl, sizeof(ai->resetctrl), AIRC_RESET);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }
    DelayUs(1);

}

/*******************************************************************************
** Name: ai_core_reset
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:44:06
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t ai_core_reset(RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits)
{

    aidmp_t *ai;
    volatile uint32 dummy;
    rk_err_t ret;

    ai = pstRk903Dev->sih->curwrap;

    ret = ai_core_disable(pstRk903Dev, pstRk903Dev->sih, (bits | resetbits));
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret =  bcmsdh_reg_write(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), bits | SICF_FGC | SICF_CLOCK_EN);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret = bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), &dummy);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&ai->resetctrl, sizeof(ai->resetctrl), 0);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    DelayUs(1);

    ret = bcmsdh_reg_write(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), (bits | SICF_CLOCK_EN));
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    ret = bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), &dummy);
    if (ret != RK_SUCCESS)
    {
        return RK_ERROR;
    }

    DelayUs(1);
}

/*******************************************************************************
** Name: si_core_reset
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:41:37
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_core_reset(RK903_DEVICE_CLASS * pstRk903Dev, uint32 bits, uint32 resetbits)
{
    if (pstRk903Dev->sih->pub.socitype == SOCI_SB)
        ;//sb_core_reset(sih, bits, resetbits);
    else if (pstRk903Dev->sih->pub.socitype == SOCI_AI)
        ai_core_reset(pstRk903Dev, bits, resetbits);
    else if (pstRk903Dev->sih->pub.socitype == SOCI_UBUS)
        ;//ub_core_reset(sih, bits, resetbits);
}

/*******************************************************************************
** Name: ai_iscoreup
** Input:SI_INFO * sih
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:07:33
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 ai_iscoreup(RK903_DEVICE_CLASS * pstRk903Dev)
{
    aidmp_t *ai;
    uint32 ioctrl, setctrl;

    ai = pstRk903Dev->sih->curwrap;
    bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->ioctrl, sizeof(ai->ioctrl), &ioctrl);
    bcmsdh_reg_read(pstRk903Dev, (uint32)&ai->resetctrl, sizeof(ai->resetctrl), &setctrl);

    return (((ioctrl & (SICF_FGC | SICF_CLOCK_EN)) == SICF_CLOCK_EN) &&
            (setctrl & AIRC_RESET) == 0);
}

/*******************************************************************************
** Name: si_iscoreup
** Input:SI_INFO * sih
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:06:32
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 si_iscoreup(RK903_DEVICE_CLASS * pstRk903Dev)
{
    if (pstRk903Dev->sih->pub.socitype == SOCI_SB)
        ;//return sb_iscoreup(sih);
    else if (pstRk903Dev->sih->pub.socitype == SOCI_AI)
        return ai_iscoreup(pstRk903Dev);
    else if (pstRk903Dev->sih->pub.socitype == SOCI_UBUS)
        ;//return ub_iscoreup(sih);

    return FALSE;
}

/*******************************************************************************
** Name: si_coreidx
** Input:SI_INFO * sih
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 15:04:46
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 si_coreidx(SI_INFO * sih)
{
    return sih->curidx;
}

/*******************************************************************************
** Name: si_coreid
** Input:SI_INFO * sih
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 14:41:18
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 si_coreid(SI_INFO * sih)
{
    return sih->coreid[sih->curidx];
}

/*******************************************************************************
** Name: si_socram_size
** Input:RK903_DEVICE_CLASS * pstRK903Device
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 13:41:49
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_socram_size(RK903_DEVICE_CLASS * pstRk903Dev)
{
    uint origidx;
    uint intr_val = 0;

    sbsocramregs_t *regs;
    bool wasup;
    uint corerev;
    uint32 coreinfo;
    uint memsize = 0, mem;
    rk_err_t ret;

    /* Block ints and save current core */
    INTR_OFF(pstRk903Dev->sih, intr_val);
    origidx = si_coreidx(pstRk903Dev->sih);

    /* Switch to SOCRAM core */
    regs = si_setcore(pstRk903Dev->sih, SOCRAM_CORE_ID, 0);
    if (!regs)
        goto done;

    /* Get info for determining size */
    wasup = si_iscoreup(pstRk903Dev);
    if (!wasup)
        si_core_reset(pstRk903Dev, 0, 0);

    corerev = si_corerev(pstRk903Dev->sih);
    bcmsdh_reg_read(pstRk903Dev, (uint)(&regs->coreinfo), sizeof(regs->coreinfo), &coreinfo);

    /* Calculate size from coreinfo based on rev */
    if (corerev == 0)
    {
        memsize = 1 << (16 + (coreinfo & SRCI_MS0_MASK));
    }
    else if (corerev < 3)
    {
        memsize = 1 << (SR_BSZ_BASE + (coreinfo & SRCI_SRBSZ_MASK));
        memsize *= (coreinfo & SRCI_SRNB_MASK) >> SRCI_SRNB_SHIFT;
    }
    else if ((corerev <= 7) || (corerev == 12))
    {
        uint nb = (coreinfo & SRCI_SRNB_MASK) >> SRCI_SRNB_SHIFT;
        uint bsz = (coreinfo & SRCI_SRBSZ_MASK);
        uint lss = (coreinfo & SRCI_LSS_MASK) >> SRCI_LSS_SHIFT;
        if (lss != 0)
            nb --;
        memsize = nb * (1 << (bsz + SR_BSZ_BASE));
        if (lss != 0)
            memsize += (1 << ((lss - 1) + SR_BSZ_BASE));
    }
    else
    {
        uint8 i;
        uint nb = (coreinfo & SRCI_SRNB_MASK) >> SRCI_SRNB_SHIFT;
        for (i = 0; i < nb; i++)
        {

            ret = socram_banksize(pstRk903Dev, regs, i, SOCRAM_MEMTYPE_RAM, &mem);
            if (ret != RK_SUCCESS)
            {
                return RK_ERROR;
            }

            memsize += mem;
        }
    }

    /* Return to previous state and core */
    if (!wasup)
        si_core_disable(pstRk903Dev, pstRk903Dev->sih, 0);

    si_setcoreidx(pstRk903Dev->sih, origidx);

done:
    INTR_RESTORE(pstRk903Dev->sih, intr_val);

    return memsize;

}


/*******************************************************************************
** Name: si_corerev
** Input:SI_INFO *sih
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 11:56:35
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 si_corerev(SI_INFO *sih)
{
    uint32 cib;

    cib = sih->cib[sih->curidx];
    return ((cib & CIB_REV_MASK) >> CIB_REV_SHIFT);
}

/*******************************************************************************
** Name: si_setcoreidx
** Input:SI_INFO *sih, uint coreidx
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 11:48:31
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN void * si_setcoreidx(SI_INFO *sih, uint coreidx)
{
    if (sih->pub.socitype == SOCI_SB)
        ;//return sb_setcoreidx(sih, coreidx);
    else if (sih->pub.socitype == SOCI_AI)
        return ai_setcoreidx(sih, coreidx);
    else if (sih->pub.socitype == SOCI_UBUS)
        ;//return ub_setcoreidx(sih, coreidx);

    return NULL;
}

/*******************************************************************************
** Name: si_restore_core
** Input:SI_INFO *sih, uint coreid, uint intr_val
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.17
** Time: 11:46:45
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_restore_core(SI_INFO *sih, uint coreid, uint intr_val)
{
    si_setcoreidx(sih, coreid);
    INTR_RESTORE(sih, intr_val);

    return RK_SUCCESS;
}

/*******************************************************************************
** Name: ai_setcoreidx
** Input:SI_INFO *sih, uint coreidx
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:39:05
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN void * ai_setcoreidx(SI_INFO *sih, uint coreidx)
{

    uint32 addr = sih->coresba[coreidx];
    uint32 wrap = sih->wrapba[coreidx];
    void *regs;

    if (coreidx >= sih->numcores)
        return (NULL);

    switch (sih->pub.bustype)
    {
        case SDIO_BUS:
            regs = (void *)(addr);
            sih->curwrap = (void *)(wrap);
            break;

        case SI_BUS:
        case SPI_BUS:
        case PCMCIA_BUS:
        default:
            regs = NULL;
            break;
    }

    sih->curmap = regs;
    sih->curidx = coreidx;

    return regs;
}

/*******************************************************************************
** Name: si_findcoreidx
** Input:si_t *sih, uint coreid, uint coreunit
** Return: uint32
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:32:57
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN uint32 si_findcoreidx(SI_INFO *sih, uint coreid, uint coreunit)
{
    uint found;
    uint i;

    found = 0;

    for (i = 0; i < sih->numcores; i++)
    {
        if (sih->coreid[i] == coreid)
        {
            if (found == coreunit)
                return (i);
            found++;
        }
    }

    return -1;
}

/*******************************************************************************
** Name: si_setcore
** Input:SI_INFO *sih, uint coreid, uint coreunit
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:30:07
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN void *  si_setcore(SI_INFO *sih, uint coreid, uint coreunit)
{
    uint idx;

    idx = si_findcoreidx(sih, coreid, coreunit);
    if (!GOODIDX(idx))
        return (NULL);

    if (sih->pub.socitype == SOCI_SB)
        ;//dgl return sb_setcoreidx(sih, idx);
    else if (sih->pub.socitype == SOCI_AI)
        return ai_setcoreidx(sih, idx);
    else if (sih->pub.socitype == SOCI_UBUS)
        ;//dgl return ub_setcoreidx(sih, idx);

    return NULL;
}

/*******************************************************************************
** Name: si_swtich_core
** Input:SI_INFO *sih, uint coreid, uint *origidx, uint *intr_val
** Return: void *
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:25:59
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN void *  si_switch_core(SI_INFO *sih, uint coreid, uint *origidx, uint *intr_val)
{
    void *cc;

    INTR_OFF(sih, *intr_val);
    *origidx = sih->curidx;
    cc = si_setcore(sih, coreid, 0);

    return cc;
}

/*******************************************************************************
** Name: si_sdiod_drive_strength_init
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 drivestrength
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:17:44
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t si_sdiod_drive_strength_init(RK903_DEVICE_CLASS * pstRk903Dev, uint32 drivestrength)
{
    chipcregs_t *cc;
    uint origidx, intr_val = 0;
    SDIOD_DRIVE_STR_T *str_tab = NULL;
    uint32 str_mask = 0;
    uint32 str_shift = 0;

    if (!(pstRk903Dev->sih->pub.cccaps & CC_CAP_PMU))
        return RK_ERROR;

    /* Remember original core before switch to chipc */
    cc = (chipcregs_t *) si_switch_core(pstRk903Dev->sih, CC_CORE_ID, &origidx, &intr_val);

    switch (SDIOD_DRVSTR_KEY(pstRk903Dev->sih->pub.chip, pstRk903Dev->sih->pub.pmurev))
    {
        case SDIOD_DRVSTR_KEY(BCM4330_CHIP_ID, 12):
            str_tab = (SDIOD_DRIVE_STR_T *)&sdiod_drive_strength_tab4_1v8;
            str_mask = 0x00003800;
            str_shift = 11;
            break;

        case SDIOD_DRVSTR_KEY(BCM43362_CHIP_ID, 13):
            str_tab = (SDIOD_DRIVE_STR_T *)&sdiod_drive_strength_tab5_1v8;
            str_mask = 0x00003800;
            str_shift = 11;
            break;
        default:
            break;
    }

    if (str_tab != NULL)
    {
        uint32 cc_data_temp;
        int i;

        /* Pick the lowest available drive strength equal or greater than the
         * requested strength.	Drive strength of 0 requests tri-state.
         */
        for (i = 0; drivestrength < str_tab[i].strength; i++)
            ;

        if (i > 0 && drivestrength > str_tab[i].strength)
            i--;

        bcmsdh_reg_write(pstRk903Dev, (uint)(&cc->chipcontrol_addr), sizeof(cc->chipcontrol_addr), 1);

        bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->chipcontrol_data), sizeof(cc->chipcontrol_data), &cc_data_temp);
        cc_data_temp &= ~str_mask;
        cc_data_temp |= str_tab[i].sel << str_shift;
        bcmsdh_reg_write(pstRk903Dev, (uint)(&cc->chipcontrol_data), sizeof(cc->chipcontrol_data), cc_data_temp);
    }

    /* Return to original core */
    si_restore_core(pstRk903Dev->sih, origidx, intr_val);
}


/*******************************************************************************
** Name: dhdsdio_chipmatch
** Input:uint32 chipId
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:12:58
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t dhdsdio_chipmatch(uint32 chipId)
{
    if (chipId == BCM4330_CHIP_ID)//rk903
        return TRUE;
    if (chipId == BCM43362_CHIP_ID)//rk901
        return TRUE;

    return FALSE;
}

/*******************************************************************************
** Name: si_info_init
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 busType
** Return: SI_INFO *
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 17:09:07
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN SI_INFO * si_info_init(RK903_DEVICE_CLASS * pstRk903Dev, uint32 busType)
{
	SI_INFO *sih;
	uint32 w, savewin;
	chipcregs_t *cc;
	uint origidx;
  rk_err_t ret;

  sih = rkos_memory_malloc(sizeof(SI_INFO));

	savewin = 0;

	sih->pub.buscoreidx = BADIDX;

	sih->curmap = (void *)pstRk903Dev->sbwad;//SI_ENUM_BASE

	/* find Chipcommon address */
	cc = (chipcregs_t *)sih->curmap;//SI_ENUM_BASE

	sih->pub.bustype = busType;

	/* bus/core/clk setup for register access */
	if (si_buscore_prep(pstRk903Dev, busType) != RK_SUCCESS)
		return NULL;

	/* ChipID recognition.
	 *   We assume we can read chipid at offset 0 from the regs arg.
	 *   If we add other chiptypes (or if we need to support old sdio hosts w/o chipcommon),
	 *   some way of recognizing them needs to be added here.
	 */
	ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&cc->chipid), sizeof(cc->chipid), &w);
    if(ret)
        return NULL;

	sih->pub.socitype = (w & CID_TYPE_MASK) >> CID_TYPE_SHIFT;
	/* Might as wll fill in chip id rev & pkg */
	sih->pub.chip = w & CID_ID_MASK;
	sih->pub.chiprev = (w & CID_REV_MASK) >> CID_REV_SHIFT;
	sih->pub.chippkg = (w & CID_PKG_MASK) >> CID_PKG_SHIFT;

	if (sih->pub.chip == BCM4322_CHIP_ID &&
		(((sih->pub.chipst & CST4322_SPROM_OTP_SEL_MASK) >> CST4322_SPROM_OTP_SEL_SHIFT)
		   == (CST4322_OTP_PRESENT | CST4322_SPROM_PRESENT)))
	{
		return NULL;
	}

	if ((sih->pub.chip == BCM4329_CHIP_ID) && (sih->pub.chiprev == 0) &&
		(sih->pub.chippkg != BCM4329_289PIN_PKG_ID))
	{
		sih->pub.chippkg = BCM4329_182PIN_PKG_ID;
	}

	sih->pub.issim = IS_SIM(sih->pub.chippkg);

	/* scan for cores */
	if(sih->pub.socitype == SOCI_SB)
	{
		rk_printf("Found chip type SB (0x%08x)\n", w);
	}
	else if(sih->pub.socitype == SOCI_AI)
	{
		rk_printf("Found chip type AI (0x%08x)\n", w);
		/* pass chipc address instead of original core base */
		ai_scan(pstRk903Dev, sih);
	}
	else if(sih->pub.socitype == SOCI_UBUS)
	{
		rk_printf("Found chip type UBUS (0x%08x), chip id = 0x%4x\n", w, sih->pub.chip);
		/* pass chipc address instead of original core base */
		//ub_scan(&sii->pub, (void *)(uintptr)cc, devid);
	}
	else
	{
		rk_printf("Found chip of unknown type (0x%08x)\n", w);
		return NULL;
	}

	/* no cores found, bail out */
	if(sih->numcores == 0)
	{
		return NULL;
	}
	/* bus/core/clk setup */
	origidx = SI_CC_IDX;
	if(si_buscore_setup(pstRk903Dev, sih, savewin, &origidx) != RK_SUCCESS)
	{
		rk_printf("si_doattach: si_buscore_setup failed\n");
		goto exit;
	}
	//printf("origidx=%d ccrev=0x%x chip=0x%x\n", origidx, sii->pub.ccrev, sih->chip);
	//origidx=0 ccrev=0x27 chip=0xa962(901)

	if(sih->pub.ccrev >= 20)
	{
		cc = (chipcregs_t *)si_setcore(sih, CC_CORE_ID, 0);
		ret = bcmsdh_reg_write(pstRk903Dev, (uint)(&cc->gpiopullup), sizeof(cc->gpiopullup), 0);
        if(ret)
            return NULL;

		ret = bcmsdh_reg_write(pstRk903Dev, (uint)(&cc->gpiopulldown), sizeof(cc->gpiopulldown), 0);
        if(ret)
            return NULL;

		si_setcoreidx(sih, origidx);
	}

	return (sih);

exit:
	return NULL;
}

/*******************************************************************************
** Name: dhdsdio_set_siaddr_window
** Input:RK903_DEVICE_CLASS * pstRk903Dev, uint32 address
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.16
** Time: 14:15:19
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_COMMON_
INIT FUN rk_err_t dhdsdio_set_siaddr_window(RK903_DEVICE_CLASS * pstRk903Dev, uint32 address)
{
    rk_err_t ret;
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRLOW,
                           (address >> 8) & SBSDIO_SBADDRLOW_MASK);
    if (!ret)
        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRMID,
                               (address >> 16) & SBSDIO_SBADDRMID_MASK);
    if (!ret)
        ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_SBADDRHIGH,
                               (address >> 24) & SBSDIO_SBADDRHIGH_MASK);

    return ret;
}

/*******************************************************************************
** Name: Rk903DevDelete
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t Rk903DevDelete(HDC dev)
{

}


/*******************************************************************************
** Name: Rk903DevDeInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t Rk903DevDeInit(RK903_DEVICE_CLASS * pstRk903Dev)
{

}


/* bcmsdh_sdmmc interrupt handler */
void IRQHandler(struct sdio_func *func)
{

	gpstRk903DevISR[0]->intrcount++;
	gpstRk903DevISR[0]->ipend = TRUE;

	/* Disable additional interrupts (is this needed now)? */
	gpstRk903DevISR[0]->intdis = TRUE;
	gpstRk903DevISR[0]->dpc_sched = TRUE;
    dhdsdio_dpc(gpstRk903DevISR[0]);//process_sdio_pending_irqs

}


/* bcmsdh_sdmmc interrupt handler for F2 (dummy handler) */
void IRQHandlerF2(struct sdio_func *func)
{

}


/*******************************************************************************
** Name: Rk903DevInit
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t Rk903DevInit(RK903_DEVICE_CLASS * pstRk903Dev)
{
    rk_err_t ret;
    uint8 clkctl = 0;
    uint32 val;
    wlc_rev_info_t revinfo = {0};
    HDC hFun;

    pstRk903Dev->alp_only = TRUE;
    pstRk903Dev->vendevid = (VENDOR_BROADCOM << 16) | 0;
    pstRk903Dev->num_funcs = 2;
    hFun = SdioDev_GetFuncHandle(pstRk903Dev->hSdio, 0);
    if ((rk_err_t)hFun <= 0)
        goto fail;

    pstRk903Dev->hFunc[0] = hFun;
    pstRk903Dev->client_block_size[0] = 64;


    hFun = SdioDev_GetFuncHandle(pstRk903Dev->hSdio, 1);
    if ((rk_err_t)hFun <= 0)
        goto fail;

    pstRk903Dev->hFunc[1] = hFun;
    pstRk903Dev->client_block_size[1] = 64;

    hFun = SdioDev_GetFuncHandle(pstRk903Dev->hSdio, 2);
    if ((rk_err_t)hFun <= 0)
        goto fail;

    pstRk903Dev->hFunc[2] = hFun;
    pstRk903Dev->client_block_size[2] = 512;

    ret = SdioDev_SetBlockSize(pstRk903Dev->hFunc[1], SDIO_FUN1_BLOCK_SIZE);
    if (ret)
    {
        goto fail;
    }

    ret = SdioDev_SetBlockSize(pstRk903Dev->hFunc[2], SDIO_FUN2_BLOCK_SIZE);
    if (ret)
    {
        goto fail;
    }

    /* Enable Function 1 */
    ret = SdioDev_EnalbeFunc(pstRk903Dev->hFunc[1]);
    if (ret)
    {
        goto fail;
    }

    /* Return the window to backplane enumeration space for core access */
    ret = dhdsdio_set_siaddr_window(pstRk903Dev, SI_ENUM_BASE);
    if (ret)
        goto fail;

    pstRk903Dev->sbwad = SI_ENUM_BASE;


    /* Force PLL off until si_attach() programs PLL control regs */
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, DHD_INIT_CLKCTL1);
    if (!ret)
    {
        ret = bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &clkctl);
    }

    if (ret || ((clkctl & ~SBSDIO_AVBITS) != DHD_INIT_CLKCTL1))
        goto fail;


    /* Make sure ALP is available before trying to read CIS */
    SPINWAIT(((bcmsdh_cfg_read(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, &clkctl)),
              !SBSDIO_ALPAV(clkctl)), PMU_MAX_TRANSITION_DLY);

    /* Now request ALP be put on the bus */
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, DHD_INIT_CLKCTL2);
    DelayUs(65);


    /* si_attach() will provide an SI handle and scan the backplane */
    pstRk903Dev->sih = si_info_init(pstRk903Dev, SDIO_BUS);
    if (!pstRk903Dev->sih)
    {
        rk_printf("si info init failed!\n");
        goto fail;
    }

    if (!dhdsdio_chipmatch((uint16)pstRk903Dev->sih->pub.chip))
    {
        goto fail;
    }

    si_sdiod_drive_strength_init(pstRk903Dev, DHD_SDIOD_DRIVER_STRENGTH);//6毫安

    /* Get info on the ARM and SOCRAM cores... */
    if ((si_setcore(pstRk903Dev->sih, ARM7S_CORE_ID, 0)) ||
            (si_setcore(pstRk903Dev->sih, ARMCM3_CORE_ID, 0)))
    {
        pstRk903Dev->armrev = si_corerev(pstRk903Dev->sih);
    }
    else
    {
        goto fail;
    }

    pstRk903Dev->orig_ramsize = si_socram_size(pstRk903Dev);

    if (!pstRk903Dev->orig_ramsize)
    {
        goto fail;
    }

    pstRk903Dev->ramsize = pstRk903Dev->orig_ramsize;//dgl:240k

    /* ...but normally deal with the SDPCMDEV core */
    pstRk903Dev->regs = si_setcore(pstRk903Dev->sih, SDIOD_CORE_ID, 0);
    if (!pstRk903Dev->regs)
    {
        rk_printf("failed to find SDIODEV core!\n");
        goto fail;
    }

    pstRk903Dev->sdpcmrev = si_corerev(pstRk903Dev->sih);
    //printf("sdpcmrev=%d\n", bus->sdpcmrev);//901 10

    /* Set core control so an SDIO reset does a backplane reset */
    ret = bcmsdh_reg_read(pstRk903Dev, (uint)(&pstRk903Dev->regs->corecontrol), sizeof(pstRk903Dev->regs->corecontrol), &val);
    if (ret)
        goto fail;

    ret = bcmsdh_reg_write(pstRk903Dev, (uint)(&pstRk903Dev->regs->corecontrol), sizeof(pstRk903Dev->regs->corecontrol), val|CC_BPRESEN);
    if (ret)
        goto fail;

    pstRk903Dev->rxint_mode = SDIO_DEVICE_HMB_RXINT;

    /* Locate an appropriately-aligned portion of hdrbuf */
    pstRk903Dev->rxhdr = (uint8 *)ROUNDUP((uint)&pstRk903Dev->hdrbuf[0], DHD_SDALIGN);

    /* Set the poll and/or interrupt flags */
    pstRk903Dev->intr = TRUE;
    pstRk903Dev->poll = FALSE;

    //buffer init
    pstRk903Dev->rxblen = ROUNDUP((WLC_IOCTL_MAXLEN + SDPCM_HDRLEN), ALIGNMENT) + DHD_SDALIGN;
    //pstRk903Dev->rxbuf = get_global_cmdbuf(DATA_RXBUF_HEAD);

    /* Disable F2 to clear any intermediate frame state on the dongle */
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_0, SDIO_CCCR_IOEx, SDIO_FUNC_ENABLE_1);
    if (ret)
        goto fail;

    pstRk903Dev->busstate = DHD_BUS_DOWN;
    pstRk903Dev->sleeping = FALSE;
    pstRk903Dev->rxflow = FALSE;
    pstRk903Dev->prev_rxlim_hit = 0;


    /* Done with backplane-dependent accesses, can drop clock... */
    ret = bcmsdh_cfg_write(pstRk903Dev, SDIO_FUN_1, SBSDIO_FUNC1_CHIPCLKCSR, 0);
    if (ret)
        goto fail;

    /* ...and initialize clock/power states */
    pstRk903Dev->clkstate = CLK_SDONLY;
    pstRk903Dev->idletime = DHD_IDLETIME_TICKS;
    pstRk903Dev->idleclock = DHD_IDLE_ACTIVE;


    /* Query the SD clock speed */
    if (bcmsdh_iovar_op(pstRk903Dev, "sd_divisor", NULL, 0,
                        &pstRk903Dev->sd_divisor, sizeof(int32), FALSE) != RK_SUCCESS)
    {
        pstRk903Dev->sd_divisor = -1;
    }

    /* Query the SD bus mode */
    if (bcmsdh_iovar_op(pstRk903Dev, "sd_mode", NULL, 0,
                        &pstRk903Dev->sd_mode, sizeof(int32), FALSE) != RK_SUCCESS)
    {
        pstRk903Dev->sd_mode = -1;
    }

    /* Query the F2 block size, set roundup accordingly */
		{
			uint32 fnum = 2;
			if (bcmsdh_iovar_op(pstRk903Dev, "sd_blocksize", &fnum, sizeof(int32),
													&pstRk903Dev->blocksize, sizeof(int32), FALSE) != RK_SUCCESS)
			{
					pstRk903Dev->blocksize = 0;
			}
			pstRk903Dev->roundup = MIN(512, pstRk903Dev->blocksize);
	  }
    /* Query if bus module supports packet chaining, default to use if supported */
    if (bcmsdh_iovar_op(pstRk903Dev, "sd_rxchain", NULL, 0,
                        &pstRk903Dev->sd_rxchain, sizeof(int32), FALSE) != RK_SUCCESS)
    {
        pstRk903Dev->sd_rxchain = FALSE;
    }
    pstRk903Dev->use_rxchain = (bool)pstRk903Dev->sd_rxchain;


    if (pstRk903Dev->intr)
	{

        #if 0
        ret = SdioDev_EnableInt(pstRk903Dev->hSdio, 1);
        if(ret)
            return ret;


        ret = SdioDev_EnableInt(pstRk903Dev->hSdio, 2);
        if(ret)
            return ret;
        #endif

       SdioDev_Claim_irq(pstRk903Dev->hFunc[1], IRQHandler);


       SdioDev_Claim_irq(pstRk903Dev->hFunc[2], IRQHandlerF2);

	}

    // down load code

    pstRk903Dev->maxctl = 1024; //WLC_IOCTL_MAXLEN;
    if (pstRk903Dev->maxctl)
	{
		pstRk903Dev->rxblen = ROUNDUP((pstRk903Dev->maxctl + SDPCM_HDRLEN), ALIGNMENT) + DHD_SDALIGN;
	    pstRk903Dev->rxbuf = get_global_cmdbuf(DATA_RXBUF_HEAD);
	}



    /* wake lock moved to dhdsdio_download_firmware */
    if (dhdsdio_download_firmware(pstRk903Dev))
    {
        rk_printf("Download err!");
        return RK_ERROR;
    }


    if (pstRk903Dev->busstate != DHD_BUS_LOAD)
    {
        //printf("busstate err!");
        return -ENETDOWN;
    }

    DelayMs(500);
    /* Bring up the bus */
    ret = dhd_bus_init(pstRk903Dev);
    if (ret)
    {
        return ret;
    }

    /*wait for 3 times interrupts.*/

    /*
    while(timeleft)
    {
        if (SDC_GetSdioEvent(0) == SDC_SUCCESS)
        {
            process_sdio_pending_irqs();
        }

        DelayMs(5);
        if(--timeleft == 0)
        {
           break;
        }
    }
    */

    /* If bus is not ready, can't come up */
    if (pstRk903Dev->busstate != DHD_BUS_DATA)
    {
        return -ENODEV;
    }

    /* Bus is ready, do any protocol initialization */
    ret = dhd_wl_ioctl_cmd(pstRk903Dev, WLC_GET_REVINFO, &revinfo, sizeof(revinfo), 0);
    if (ret >= 0)
    {
        ret = dhd_preinit_ioctls(pstRk903Dev);
        if (ret < 0)
        {
            return ret;
        }
    }
    return RK_SUCCESS;

fail:
    pstRk903Dev->sih = NULL;

    return FALSE;
}


/*******************************************************************************
** Name: Rk903DevResume
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t Rk903DevResume(HDC dev)
{

}


/*******************************************************************************
** Name: Rk903DevSuspend
** Input:HDC dev
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_INIT_
INIT FUN rk_err_t Rk903DevSuspend(HDC dev)
{

}


#ifdef _RK903_DEV_SHELL_
_DRIVER_RK903_RK903DEVICE_SHELL_
static uint8* ShellRk903Name[] =
{
    "pcb",
    "mc",
    "del",
    "test",
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
** Name: Rk903Dev_Shell
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:Aaron.sun
** Date: 2014.6.14
** Time: 9:45:29
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL API rk_err_t Rk903Dev_Shell(HDC dev, uint8 * pstr)
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

    ret = ShellCheckCmd(ShellRk903Name, pItem, StrCnt);
    if (ret < 0)
    {
        return RK_ERROR;
    }

    i = (uint32)ret;

    pItem += StrCnt;
    pItem++;                                            //remove '.',the point is the useful item

    switch (i)
    {
        case 0x00: //pcb
            ret = Rk903DevShellPcb(dev,pItem);
            break;

        case 0x01: //mc
            ret = Rk903DevShellMc(dev,pItem);
            break;

        case 0x02://del
            ret = Rk903DevShellDel(dev, pItem);
            break;

        case 0x03://test
            ret = Rk903DevShellTest(dev, pItem);
            break;

        case 0x04://help
            ret = Rk903DevShellHelp(dev, pItem);
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
** Name: Rk903DevShellHelp
** Input:HDC dev, const uint8 * pstr
** Return: rk_err_t
** Owner:chad.Ma
** Date: 2014.11.3
** Time: 17:46:08
*******************************************************************************/
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL FUN rk_err_t Rk903DevShellHelp(HDC dev, uint8 * pstr)
{
    pstr--;

    if( StrLenA((uint8 *) pstr) != 0)
        return RK_ERROR;

    rk_print_string("rk903dev命令集提供了一系列的命令对rk903dev进行操作\r\n");
    rk_print_string("lcd包含的子命令如下:           \r\n");
    rk_print_string("pcb       显示pcb信息         \r\n");
    rk_print_string("mc        创建rk903 dev        \r\n");
    rk_print_string("del       删除rk903 dev         \r\n");
    rk_print_string("test      测试rk903 dev    \r\n");
    rk_print_string("help      显示rk903dev命令帮助信息  \r\n");

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
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL FUN rk_err_t Rk903DevShellTest(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("rk903dev.test: test cmd.\r\n");
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
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL FUN rk_err_t Rk903DevShellDel(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("rk903.del: del cmd.\r\n");
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
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL FUN rk_err_t Rk903DevShellMc(HDC dev, uint8 * pstr)
{

    HDC hSdio;
    RK903_DEV_ARG stRk903Arg;
    rk_err_t ret;

#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("rk903.mc: create rk903dev cmd.\r\n");
            return RK_SUCCESS;
        }
    }
#endif

	return RK_SUCCESS;

    hSdio = RKDev_Open(DEV_CLASS_SDIO, 0, NOT_CARE);

    if ((hSdio == NULL) || (hSdio == (HDC)RK_ERROR) || (hSdio == (HDC)RK_PARA_ERR))
    {
        rk_print_string("sdio Device Open Failure");
        return RK_SUCCESS;
    }

    stRk903Arg.hSdio = hSdio;
    ret = RKDev_Create(DEV_CLASS_RK903, 0, &stRk903Arg);

    if (ret != RK_SUCCESS)
    {
        rk_print_string("rk903 Device Create Failure");
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
_DRIVER_RK903_RK903DEVICE_SHELL_
SHELL FUN rk_err_t Rk903DevShellPcb(HDC dev, uint8 * pstr)
{
#ifdef SHELL_HELP
    pstr--;
    if(pstr[0] == '.')
    {
        //list have sub cmd
        pstr++;
        if(StrCmpA(pstr, "help", 4) == 0)
        {
            rk_print_string("rk903dev.pcb: pcb info.\r\n");
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

