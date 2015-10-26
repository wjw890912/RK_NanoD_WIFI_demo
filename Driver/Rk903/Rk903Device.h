/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                        All rights reserved.
*
* FileName: Driver\Rk903\Rk903Device.h
* Owner: Aaron.sun
* Date: 2014.6.14
* Time: 9:11:43
* Desc: Rk903 device class
* History:
*    <author>     <date>       <time>     <version>       <Desc>
*    Aaron.sun     2014.6.14     9:11:43   1.0
********************************************************************************************
*/

#ifndef __DRIVER_RK903_RK903DEVICE_H__
#define __DRIVER_RK903_RK903DEVICE_H__

#include "source_macro.h"
#ifdef __DRIVER_RK903_RK903DEVICE_C__

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define _DRIVER_RK903_RK903DEVICE_COMMON_  __attribute__((section("driver_rk903_rk903device_read")))
#define _DRIVER_RK903_RK903DEVICE_INIT_  __attribute__((section("driver_rk903_rk903device_init")))
#define _DRIVER_RK903_RK903DEVICE_SHELL_  __attribute__((section("driver_rk903_rk903device_shell")))

#define RK903_DEV_CNT 1


typedef  struct _RK903_DEV_ARG
{
    HDC hSdio;

}RK903_DEV_ARG;

/* The level of bus communication with the dongle */

#ifndef PAD
#define    _PADLINE(line)    pad ## line
#define    _XSTR(line)    _PADLINE(line)
#define    PAD        _XSTR(__LINE__)
#endif

#define    SI_MAXCORES            16        /* Max cores (this is arbitrary, for software
* convenience and could be changed if we
* make any larger chips*/

#define    ROUNDUP(x, y)        ((((x)+((y)-1))/(y))*(y))
#define DHD_SDALIGN    32
#define MAX_HDR_READ    32

#define DHD_FIRSTREAD   32

typedef struct bcm_iovar
{
    const char *name;
    uint16 varid;
    uint16 flags;
    uint16 type;
    uint16 minlen;

} bcm_iovar_t;


typedef struct cdc_ioctl
{

uint32 cmd;
uint32 len;
uint32 flags;
uint32 status;

} cdc_ioctl_t;

typedef struct _DHD_PROT
{

uint16 reqid;
uint8 pending;
uint32 lastcmd;
cdc_ioctl_t * msg;

} DHD_PROT;

typedef struct _sk_buff
{
uint   len;
void * data;

}sk_buff;

typedef struct wlc_rev_info
{

uint        vendorid;
uint        deviceid;
uint        radiorev;
uint        chiprev;
uint        corerev;
uint        boardid;
uint        boardvendor;
uint        boardrev;
uint        driverrev;
uint        ucoderev;
uint        bus;
uint        chipnum;
uint        phytype;
uint        phyrev;
uint        anarev;
uint        chippkg;

} wlc_rev_info_t;

typedef volatile struct
{

uint32    control;
uint32    addr;
uint32    ptr;
uint32    status;

} dma32regs_t;

typedef volatile struct
{
dma32regs_t    xmt;
dma32regs_t    rcv;

} dma32regp_t;

typedef volatile struct
{
uint32    fifoaddr;
uint32    fifodatalow;
uint32    fifodatahigh;
uint32    pad;

} dma32diag_t;

typedef volatile struct
{
uint32    ctrl;
uint32    addr;

} dma32dd_t;


typedef volatile struct
{
uint32    control;
uint32    ptr;
uint32    addrlow;
uint32    addrhigh;
uint32    status0;
uint32    status1;
} dma64regs_t;

typedef volatile struct
{
dma64regs_t    tx;
dma64regs_t    rx;
} dma64regp_t;

typedef volatile struct
{
uint32    fifoaddr;
uint32    fifodatalow;
uint32    fifodatahigh;
uint32    pad;
} dma64diag_t;


typedef volatile struct
{
uint32    ctrl1;
uint32    ctrl2;
uint32    addrlow;
uint32    addrhigh;

} dma64dd_t;

typedef volatile struct
{
dma64regs_t    xmt;        /* dma tx */
uint32 PAD[2];
    dma64regs_t    rcv;        /* dma rx */
    uint32 PAD[2];
} dma64p_t;

/* dma64 sdiod corerev >= 1 */
typedef volatile struct
{
    dma64p_t dma64regs[2];
    dma64diag_t dmafifo;        /* DMA Diagnostic Regs, 0x280-0x28c */
    uint32 PAD[92];
} sdiodma64_t;

/* dma32 sdiod corerev == 0 */
typedef volatile struct
{
    dma32regp_t dma32regs[2];    /* dma tx & rx, 0x200-0x23c */
    dma32diag_t dmafifo;        /* DMA Diagnostic Regs, 0x240-0x24c */
    uint32 PAD[108];
} sdiodma32_t;

/* dma32 regs for pcmcia core */
typedef volatile struct
{
    dma32regp_t dmaregs;        /* DMA Regs, 0x200-0x21c, rev8 */
    dma32diag_t dmafifo;        /* DMA Diagnostic Regs, 0x220-0x22c */
    uint32 PAD[116];
} pcmdma32_t;

typedef volatile struct _sbconfig
{
    uint32    PAD[2];
    uint32    sbipsflag;
    uint32    PAD[3];
    uint32    sbtpsflag;
    uint32    PAD[11];
    uint32    sbtmerrloga;
    uint32    PAD;
    uint32    sbtmerrlog;
    uint32    PAD[3];
    uint32    sbadmatch3;
    uint32    PAD;
    uint32    sbadmatch2;
    uint32    PAD;
    uint32    sbadmatch1;
    uint32    PAD[7];
    uint32    sbimstate;
    uint32    sbintvec;
    uint32    sbtmstatelow;
    uint32    sbtmstatehigh;
    uint32    sbbwa0;
    uint32    PAD;
    uint32    sbimconfiglow;
    uint32    sbimconfighigh;
    uint32    sbadmatch0;
    uint32    PAD;
    uint32    sbtmconfiglow;
    uint32    sbtmconfighigh;
    uint32    sbbconfig;
    uint32    PAD;
    uint32    sbbstate;
    uint32    PAD[3];
    uint32    sbactcnfg;
    uint32    PAD[3];
    uint32    sbflagst;
    uint32    PAD[3];
    uint32    sbidlow;
    uint32    sbidhigh;
} sbconfig_t;

/* core registers */
typedef volatile struct
{
    uint32 corecontrol;        /* CoreControl, 0x000, rev8 */
    uint32 corestatus;        /* CoreStatus, 0x004, rev8  */
    uint32 PAD[1];
    uint32 biststatus;        /* BistStatus, 0x00c, rev8  */

    /* PCMCIA access */
    uint16 pcmciamesportaladdr;    /* PcmciaMesPortalAddr, 0x010, rev8   */
    uint16 PAD[1];
    uint16 pcmciamesportalmask;    /* PcmciaMesPortalMask, 0x014, rev8   */
    uint16 PAD[1];
    uint16 pcmciawrframebc;        /* PcmciaWrFrameBC, 0x018, rev8   */
    uint16 PAD[1];
    uint16 pcmciaunderflowtimer;    /* PcmciaUnderflowTimer, 0x01c, rev8   */
    uint16 PAD[1];

    /* interrupt */
    uint32 intstatus;        /* IntStatus, 0x020, rev8   */
    uint32 hostintmask;        /* IntHostMask, 0x024, rev8   */
    uint32 intmask;            /* IntSbMask, 0x028, rev8   */
    uint32 sbintstatus;        /* SBIntStatus, 0x02c, rev8   */
    uint32 sbintmask;        /* SBIntMask, 0x030, rev8   */
    uint32 funcintmask;        /* SDIO Function Interrupt Mask, SDIO rev4 */
    uint32 PAD[2];
    uint32 tosbmailbox;        /* ToSBMailbox, 0x040, rev8   */
    uint32 tohostmailbox;        /* ToHostMailbox, 0x044, rev8   */
    uint32 tosbmailboxdata;        /* ToSbMailboxData, 0x048, rev8   */
    uint32 tohostmailboxdata;    /* ToHostMailboxData, 0x04c, rev8   */

    /* synchronized access to registers in SDIO clock domain */
    uint32 sdioaccess;        /* SdioAccess, 0x050, rev8   */
    uint32 PAD[3];

    /* PCMCIA frame control */
    uint8 pcmciaframectrl;        /* pcmciaFrameCtrl, 0x060, rev8   */
    uint8 PAD[3];
    uint8 pcmciawatermark;        /* pcmciaWaterMark, 0x064, rev8   */
    uint8 PAD[155];

    /* interrupt batching control */
    uint32 intrcvlazy;        /* IntRcvLazy, 0x100, rev8 */
    uint32 PAD[3];

    /* counters */
    uint32 cmd52rd;            /* Cmd52RdCount, 0x110, rev8, SDIO: cmd52 reads */
    uint32 cmd52wr;            /* Cmd52WrCount, 0x114, rev8, SDIO: cmd52 writes */
    uint32 cmd53rd;            /* Cmd53RdCount, 0x118, rev8, SDIO: cmd53 reads */
    uint32 cmd53wr;            /* Cmd53WrCount, 0x11c, rev8, SDIO: cmd53 writes */
    uint32 abort;            /* AbortCount, 0x120, rev8, SDIO: aborts */
    uint32 datacrcerror;    /* DataCrcErrorCount, 0x124, rev8, SDIO: frames w/bad CRC */
    uint32 rdoutofsync;        /* RdOutOfSyncCount, 0x128, rev8, SDIO/PCMCIA: Rd Frm OOS */
    uint32 wroutofsync;        /* RdOutOfSyncCount, 0x12c, rev8, SDIO/PCMCIA: Wr Frm OOS */
    uint32 writebusy;        /* WriteBusyCount, 0x130, rev8, SDIO: dev asserted "busy" */
    uint32 readwait;        /* ReadWaitCount, 0x134, rev8, SDIO: read: no data avail */
    uint32 readterm;        /* ReadTermCount, 0x138, rev8, SDIO: rd frm terminates */
    uint32 writeterm;        /* WriteTermCount, 0x13c, rev8, SDIO: wr frm terminates */
    uint32 PAD[40];
    uint32 clockctlstatus;        /* ClockCtlStatus, 0x1e0, rev8 */
    uint32 PAD[7];

    /* DMA engines */
    volatile union
    {
        pcmdma32_t pcm32;
        sdiodma32_t sdiod32;
        sdiodma64_t sdiod64;
    } dma;

    /* SDIO/PCMCIA CIS region */
    char cis[512];            /* 512 byte CIS, 0x400-0x5ff, rev6 */

    /* PCMCIA function control registers */
    char pcmciafcr[256];        /* PCMCIA FCR, 0x600-6ff, rev6 */
    uint16 PAD[55];

    /* PCMCIA backplane access */
    uint16 backplanecsr;        /* BackplaneCSR, 0x76E, rev6 */
    uint16 backplaneaddr0;        /* BackplaneAddr0, 0x770, rev6 */
    uint16 backplaneaddr1;        /* BackplaneAddr1, 0x772, rev6 */
    uint16 backplaneaddr2;        /* BackplaneAddr2, 0x774, rev6 */
    uint16 backplaneaddr3;        /* BackplaneAddr3, 0x776, rev6 */
    uint16 backplanedata0;        /* BackplaneData0, 0x778, rev6 */
    uint16 backplanedata1;        /* BackplaneData1, 0x77a, rev6 */
    uint16 backplanedata2;        /* BackplaneData2, 0x77c, rev6 */
    uint16 backplanedata3;        /* BackplaneData3, 0x77e, rev6 */
    uint16 PAD[31];

    /* sprom "size" & "blank" info */
    uint16 spromstatus;        /* SPROMStatus, 0x7BE, rev2 */
    uint32 PAD[464];

    /* Sonics SiliconBackplane registers */
    sbconfig_t sbconfig;        /* SbConfig Regs, 0xf00-0xfff, rev8 */
} sdpcmd_regs_t;

typedef volatile struct sbsocramregs
{
    uint32    coreinfo;
    uint32    bwalloc;
    uint32    extracoreinfo;
    uint32    biststat;
    uint32    bankidx;
    uint32    standbyctrl;

    uint32    errlogstatus;
    uint32    errlogaddr;

    uint32    cambankidx;
    uint32    cambankstandbyctrl;
    uint32    cambankpatchctrl;
    uint32    cambankpatchtblbaseaddr;
    uint32    cambankcmdreg;
    uint32    cambankdatareg;
    uint32    cambankmaskreg;
    uint32    PAD[1];
    uint32    bankinfo;
    uint32    PAD[15];
    uint32    extmemconfig;
    uint32    extmemparitycsr;
    uint32    extmemparityerrdata;
    uint32    extmemparityerrcnt;
    uint32    extmemwrctrlandsize;
    uint32    PAD[84];
    uint32    workaround;
    uint32    pwrctl;
    uint32  PAD[133];
    uint32  sr_control;
    uint32  sr_status;
    uint32  sr_address;
    uint32  sr_data;
} sbsocramregs_t;


typedef volatile struct _aidmp
{
    uint32    oobselina30;
    uint32    oobselina74;
    uint32    PAD[6];
    uint32    oobselinb30;
    uint32    oobselinb74;
    uint32    PAD[6];
    uint32    oobselinc30;
    uint32    oobselinc74;
    uint32    PAD[6];
    uint32    oobselind30;
    uint32    oobselind74;
    uint32    PAD[38];
    uint32    oobselouta30;
    uint32    oobselouta74;
    uint32    PAD[6];
    uint32    oobseloutb30;
    uint32    oobseloutb74;
    uint32    PAD[6];
    uint32    oobseloutc30;
    uint32    oobseloutc74;
    uint32    PAD[6];
    uint32    oobseloutd30;
    uint32    oobseloutd74;
    uint32    PAD[38];
    uint32    oobsynca;
    uint32    oobseloutaen;
    uint32    PAD[6];
    uint32    oobsyncb;
    uint32    oobseloutben;
    uint32    PAD[6];
    uint32    oobsyncc;
    uint32    oobseloutcen;
    uint32    PAD[6];
    uint32    oobsyncd;
    uint32    oobseloutden;
    uint32    PAD[38];
    uint32    oobaextwidth;
    uint32    oobainwidth;
    uint32    oobaoutwidth;
    uint32    PAD[5];
    uint32    oobbextwidth;
    uint32    oobbinwidth;
    uint32    oobboutwidth;
    uint32    PAD[5];
    uint32    oobcextwidth;
    uint32    oobcinwidth;
    uint32    oobcoutwidth;
    uint32    PAD[5];
    uint32    oobdextwidth;
    uint32    oobdinwidth;
    uint32    oobdoutwidth;
    uint32    PAD[37];
    uint32    ioctrlset;
    uint32    ioctrlclear;
    uint32    ioctrl;
    uint32    PAD[61];
    uint32    iostatus;
    uint32    PAD[127];
    uint32    ioctrlwidth;
    uint32    iostatuswidth;
    uint32    PAD[62];
    uint32    resetctrl;
    uint32    resetstatus;
    uint32    resetreadid;
    uint32    resetwriteid;
    uint32    PAD[60];
    uint32    errlogctrl;
    uint32    errlogdone;
    uint32    errlogstatus;
    uint32    errlogaddrlo;
    uint32    errlogaddrhi;
    uint32    errlogid;
    uint32    errloguser;
    uint32    errlogflags;
    uint32    PAD[56];
    uint32    intstatus;
    uint32    PAD[127];
    uint32    config;
    uint32    PAD[63];
    uint32    itcr;
    uint32    PAD[3];
    uint32    itipooba;
    uint32    itipoobb;
    uint32    itipoobc;
    uint32    itipoobd;
    uint32    PAD[4];
    uint32    itipoobaout;
    uint32    itipoobbout;
    uint32    itipoobcout;
    uint32    itipoobdout;
    uint32    PAD[4];
    uint32    itopooba;
    uint32    itopoobb;
    uint32    itopoobc;
    uint32    itopoobd;
    uint32    PAD[4];
    uint32    itopoobain;
    uint32    itopoobbin;
    uint32    itopoobcin;
    uint32    itopoobdin;
    uint32    PAD[4];
    uint32    itopreset;
    uint32    PAD[15];
    uint32    peripherialid4;
    uint32    peripherialid5;
    uint32    peripherialid6;
    uint32    peripherialid7;
    uint32    peripherialid0;
    uint32    peripherialid1;
    uint32    peripherialid2;
    uint32    peripherialid3;
    uint32    componentid0;
    uint32    componentid1;
    uint32    componentid2;
    uint32    componentid3;

} aidmp_t;


typedef struct _SDIOD_DRIVE_STR_T
{
    uint8 strength;            /* Pad Drive Strength in mA */
    uint8 sel;            /* Chip-specific select value */
} SDIOD_DRIVE_STR_T;

typedef struct eci_prerev35
{
    uint32    eci_output;
    uint32    eci_control;
    uint32    eci_inputlo;
    uint32    eci_inputmi;
    uint32    eci_inputhi;
    uint32    eci_inputintpolaritylo;
    uint32    eci_inputintpolaritymi;
    uint32    eci_inputintpolarityhi;
    uint32    eci_intmasklo;
    uint32    eci_intmaskmi;
    uint32    eci_intmaskhi;
    uint32    eci_eventlo;
    uint32    eci_eventmi;
    uint32    eci_eventhi;
    uint32    eci_eventmasklo;
    uint32    eci_eventmaskmi;
    uint32    eci_eventmaskhi;
    uint32    PAD[3];
} eci_prerev35_t;

typedef struct eci_rev35
{
    uint32    eci_outputlo;
    uint32    eci_outputhi;
    uint32    eci_controllo;
    uint32    eci_controlhi;
    uint32    eci_inputlo;
    uint32    eci_inputhi;
    uint32    eci_inputintpolaritylo;
    uint32    eci_inputintpolarityhi;
    uint32    eci_intmasklo;
    uint32    eci_intmaskhi;
    uint32    eci_eventlo;
    uint32    eci_eventhi;
    uint32    eci_eventmasklo;
    uint32    eci_eventmaskhi;
    uint32    eci_auxtx;
    uint32    eci_auxrx;
    uint32    eci_datatag;
    uint32    eci_uartescvalue;
    uint32    eci_autobaudctr;
    uint32    eci_uartfifolevel;
} eci_rev35_t;

typedef volatile struct
{
    uint32    chipid;
    uint32    capabilities;
    uint32    corecontrol;
    uint32    bist;


    uint32    otpstatus;
    uint32    otpcontrol;
    uint32    otpprog;
    uint32    otplayout;


    uint32    intstatus;
    uint32    intmask;


    uint32    chipcontrol;
    uint32    chipstatus;


    uint32    jtagcmd;
    uint32    jtagir;
    uint32    jtagdr;
    uint32    jtagctrl;


    uint32    flashcontrol;
    uint32    flashaddress;
    uint32    flashdata;
    uint32    PAD[1];


    uint32    broadcastaddress;
    uint32    broadcastdata;


    uint32    gpiopullup;
    uint32    gpiopulldown;
    uint32    gpioin;
    uint32    gpioout;
    uint32    gpioouten;
    uint32    gpiocontrol;
    uint32    gpiointpolarity;
    uint32    gpiointmask;


    uint32    gpioevent;
    uint32    gpioeventintmask;


    uint32    watchdog;


    uint32    gpioeventintpolarity;


    uint32  gpiotimerval;
    uint32  gpiotimeroutmask;


    uint32    clockcontrol_n;
    uint32    clockcontrol_sb;
    uint32    clockcontrol_pci;
    uint32    clockcontrol_m2;
    uint32    clockcontrol_m3;
    uint32    clkdiv;
    uint32    gpiodebugsel;
    uint32    capabilities_ext;


    uint32    pll_on_delay;
    uint32    fref_sel_delay;
    uint32    slow_clk_ctl;
    uint32    PAD;


    uint32    system_clk_ctl;
    uint32    clkstatestretch;
    uint32    PAD[2];


    uint32    bp_addrlow;
    uint32    bp_addrhigh;
    uint32    bp_data;
    uint32    PAD;
    uint32    bp_indaccess;

    uint32    gsioctrl;
    uint32    gsioaddress;
    uint32    gsiodata;


    uint32    clkdiv2;
    uint32    PAD[2];


    uint32    eromptr;


    uint32    pcmcia_config;
    uint32    pcmcia_memwait;
    uint32    pcmcia_attrwait;
    uint32    pcmcia_iowait;
    uint32    ide_config;
    uint32    ide_memwait;
    uint32    ide_attrwait;
    uint32    ide_iowait;
    uint32    prog_config;
    uint32    prog_waitcount;
    uint32    flash_config;
    uint32    flash_waitcount;
    uint32  SECI_config;
    uint32    SECI_status;
    uint32    SECI_statusmask;
    uint32    SECI_rxnibchanged;

    uint32    PAD[20];


    uint32    sromcontrol;
    uint32    sromaddress;
    uint32    sromdata;
    uint32    PAD[9];
    uint32  seci_uart_data;
    uint32  seci_uart_bauddiv;
    uint32  seci_uart_fcr;
    uint32  seci_uart_lcr;
    uint32  seci_uart_mcr;
    uint32  seci_uart_lsr;
    uint32  seci_uart_msr;
    uint32  seci_uart_baudadj;

    uint32    clk_ctl_st;
    uint32    hw_war;
    uint32    PAD[70];


    uint8    uart0data;
    uint8    uart0imr;
    uint8    uart0fcr;
    uint8    uart0lcr;
    uint8    uart0mcr;
    uint8    uart0lsr;
    uint8    uart0msr;
    uint8    uart0scratch;
    uint8    PAD[248];

    uint8    uart1data;
    uint8    uart1imr;
    uint8    uart1fcr;
    uint8    uart1lcr;
    uint8    uart1mcr;
    uint8    uart1lsr;
    uint8    uart1msr;
    uint8    uart1scratch;
    uint32    PAD[126];



    uint32    pmucontrol;
    uint32    pmucapabilities;
    uint32    pmustatus;
    uint32    res_state;
    uint32    res_pending;
    uint32    pmutimer;
    uint32    min_res_mask;
    uint32    max_res_mask;
    uint32    res_table_sel;
    uint32    res_dep_mask;
    uint32    res_updn_timer;
    uint32    res_timer;
    uint32    clkstretch;
    uint32    pmuwatchdog;
    uint32    gpiosel;
    uint32    gpioenable;
    uint32    res_req_timer_sel;
    uint32    res_req_timer;
    uint32    res_req_mask;
    uint32    PAD;
    uint32    chipcontrol_addr;
    uint32    chipcontrol_data;
    uint32    regcontrol_addr;
    uint32    regcontrol_data;
    uint32    pllcontrol_addr;
    uint32    pllcontrol_data;
    uint32    pmustrapopt;
    uint32    pmu_xtalfreq;
    uint32    PAD[100];
    uint16    sromotp[768];
} chipcregs_t;


typedef uint32 (*si_intrsoff_t)(void *intr_arg);
typedef void (*si_intrsrestore_t)(void *intr_arg, uint32 arg);
typedef bool (*si_intrsenabled_t)(void *intr_arg);
typedef void (*gpio_handler_t)(uint32 stat, void *arg);

typedef struct _SI_PUB
{
    uint    socitype;
    uint    bustype;
    uint    buscoretype;
    uint    buscorerev;
    uint    buscoreidx;
    int        ccrev;
    uint32    cccaps;
    uint32  cccaps_ext;
    int        pmurev;
    uint32    pmucaps;
    uint    boardtype;
    uint    boardvendor;
    uint    boardflags;
    uint    boardflags2;
    uint    chip;
    uint    chiprev;
    uint    chippkg;
    uint32    chipst;
    bool    issim;
    uint    socirev;
    bool    pci_pr32414;

}SI_PUB;

typedef struct _GPIOH_ITEM
{
    void            *arg;
    uint32            level;
    gpio_handler_t    handler;
    uint32            event;
    struct _GPIOH_ITEM    *next;

} GPIOH_ITEM;

typedef struct _SI_INFO
{

    SI_PUB  pub;     /* back plane public state (must be first field) */

    void    *sdh;            /* bcmsdh handle */

    uint    dev_coreid;        /* the core provides driver functions */
    void    *intr_arg;        /* interrupt callback function arg */
    si_intrsoff_t intrsoff_fn;    /* turns chip interrupts off */
    si_intrsrestore_t intrsrestore_fn; /* restore chip interrupts */
    si_intrsenabled_t intrsenabled_fn; /* check if interrupts are enabled */

    void *pch;            /* PCI/E core handle */

    GPIOH_ITEM *gpioh_head;     /* GPIO event handlers list */

    bool    memseg;            /* flag to toggle MEM_SEG register */

    char *vars;
    uint varsz;

    void    *curmap;        /* current regs va */
    void    *regs[SI_MAXCORES];    /* other regs va */

    uint    curidx;            /* current core index */
    uint    numcores;        /* # discovered cores */
    uint    coreid[SI_MAXCORES];    /* id of each core */
    uint32    coresba[SI_MAXCORES];    /* backplane address of each core */
    void    *regs2[SI_MAXCORES];    /* va of each core second register set (usbh20) */
    uint32    coresba2[SI_MAXCORES];    /* address of each core second register set (usbh20) */
    uint32    coresba_size[SI_MAXCORES]; /* backplane address space size */
    uint32    coresba2_size[SI_MAXCORES]; /* second address space size */

    void    *curwrap;        /* current wrapper va */
    void    *wrappers[SI_MAXCORES];    /* other cores wrapper va */
    uint32    wrapba[SI_MAXCORES];    /* address of controlling wrapper */

    uint32    cia[SI_MAXCORES];    /* erom cia entry for each core */
    uint32    cib[SI_MAXCORES];    /* erom cia entry for each core */
    uint32    oob_router;        /* oob router registers for axi */

} SI_INFO;

struct bcmsdh_info
{
    bool    init_success;    /* underlying driver successfully attached */
    void    *sdioh;        /* handler for sdioh */
    uint32  vendevid;    /* Target Vendor and Device ID on SD bus */
    bool    regfail;    /* Save status of last reg_read/reg_write call */
    uint32    sbwad;        /* Save backplane window address */
};
/* forward declarations */
typedef struct bcmsdh_info bcmsdh_info_t;

#define R_SDREG(regvar, regaddr, retryvar)  \
\
do { \
    retryvar = 0; \
    do { \
        bcmsdh_reg_read(NULL, (uint)regaddr, sizeof(*(regaddr)), &regvar); \
    } while (bcmsdh_regfail(gpstRk903DevISR[0], NULL) && (++retryvar <= retry_limit)); \
    if (retryvar) { \
        gpstRk903DevISR[0]->regfails += (retryvar-1); \
        if (retryvar > retry_limit) { \
            regvar = 0; \
        } \
    } \
} while (0)

#define W_SDREG(regval, regaddr, retryvar) \
\
    do { \
    retryvar = 0; \
    do { \
         bcmsdh_reg_write(gpstRk903DevISR[0],(uint)regaddr,sizeof(*(regaddr)),regval);\
    } while (bcmsdh_regfail(gpstRk903DevISR[0], NULL) && (++retryvar <= retry_limit)); \
    if (retryvar) { \
        gpstRk903DevISR[0]->regfails += (retryvar-1); \
    } \
} while (0)

#if 0
#define BUS_WAKE(bus) \
    do { \
        if ((bus)->sleeping) \
            dhdsdio_bussleep((bus), FALSE); \
    } while (0);
#endif


#define    bzero(b, len)            memset((b), '\0', (len))


#define SDIOH_API_RC_SUCCESS             (0x00)
#define SDIOH_API_RC_FAIL                 (0x01)
#define SDIOH_API_SUCCESS(status)        (status == 0)

#define SDIOH_READ              0    /* Read request */
#define SDIOH_WRITE             1    /* Write request */

#define SDIOH_DATA_FIX          0    /* Fixed addressing */
#define SDIOH_DATA_INC          1    /* Incremental addressing */

#define SDIOH_CMD_TYPE_NORMAL   0       /* Normal command */
#define SDIOH_CMD_TYPE_APPEND   1       /* Append command */
#define SDIOH_CMD_TYPE_CUTTHRU  2       /* Cut-through command */

#define SDIOH_DATA_PIO          0       /* PIO mode */
#define SDIOH_DATA_DMA          1       /* DMA mode */







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
extern int dhd_senddata(void * pstRk903Dev, char *buf, uint32 len);
extern rk_err_t RK903Dev_GetMac(char *buf);
extern rk_err_t Rk903Dev_Shell(HDC dev, uint8 * pstr);
extern rk_err_t Rk903Dev_Write(HDC dev);
extern rk_err_t Rk903Dev_Read(HDC dev);
extern HDC Rk903Dev_Create(uint32 DevID, void * arg);

#endif
#endif
