/*
********************************************************************************************
*
*  Copyright (c): 2014 - 2014 + 5, Fuzhou Rockchip Electronics Co., Ltd
*                         All rights reserved.
*
* FileName: Driver\Rk903\Rk903Macro.h
* Owner: zhuzhe
* Date: 2014.7.7
* Time: 17:26:11
* Desc:
* History:
*    <author>	 <date> 	  <time>	 <version>	   <Desc>
*    zhuzhe     2014.7.7     17:26:11   1.0
********************************************************************************************
*/

#ifndef __DRIVER_RK903_RK903MACRO_H__
#define __DRIVER_RK903_RK903MACRO_H__

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
enum DHD_BUS_STATE
{
	DHD_BUS_DOWN,		/* Not ready for frame transfers */
	DHD_BUS_LOAD,		/* Download access only (CPU reset) */
	DHD_BUS_DATA		/* Ready for frame transfers */
};

#define SDIOH_READ              0	/* Read request */
#define SDIOH_WRITE             1	/* Write request */

#define SDIOH_DATA_FIX          0	/* Fixed addressing */
#define SDIOH_DATA_INC          1	/* Incremental addressing */

/* registers introduced in rev 8, some content (mask/bits) defs in sbsdpcmdev.h */
#define SBSDIO_FUNC1_SBADDRLOW		0x1000A		/* SB Address Window Low (b15) */
#define SBSDIO_FUNC1_SBADDRMID		0x1000B		/* SB Address Window Mid (b23:b16) */
#define SBSDIO_FUNC1_SBADDRHIGH		0x1000C		/* SB Address Window High (b31:b24)    */
#define SBSDIO_FUNC1_FRAMECTRL		0x1000D		/* Frame Control (frame term/abort) */
#define SBSDIO_FUNC1_CHIPCLKCSR		0x1000E		/* ChipClockCSR (ALP/HT ctl/status) */
#define SBSDIO_FUNC1_SDIOPULLUP 	0x1000F		/* SdioPullUp (on cmd, d0-d2) */
#define SBSDIO_FUNC1_WFRAMEBCLO		0x10019		/* Write Frame Byte Count Low */
#define SBSDIO_FUNC1_WFRAMEBCHI		0x1001A		/* Write Frame Byte Count High */
#define SBSDIO_FUNC1_RFRAMEBCLO		0x1001B		/* Read Frame Byte Count Low */
#define SBSDIO_FUNC1_RFRAMEBCHI		0x1001C		/* Read Frame Byte Count High */

#define SBSDIO_FUNC1_MISC_REG_START	0x10000 	/* f1 misc register start */
#define SBSDIO_FUNC1_MISC_REG_LIMIT	0x1001C 	/* f1 misc register end */

/* some duplication with sbsdpcmdev.h here */
/* valid bits in SBSDIO_FUNC1_SBADDRxxx regs */
#define SBSDIO_SBADDRLOW_MASK		0x80		/* Valid bits in SBADDRLOW */
#define SBSDIO_SBADDRMID_MASK		0xff		/* Valid bits in SBADDRMID */
#define SBSDIO_SBADDRHIGH_MASK		0xffU		/* Valid bits in SBADDRHIGH */
#define SBSDIO_SBWINDOW_MASK		0xffff8000	/* Address bits from SBADDR regs */

/* SBSDIO_DEVICE_CTL */
#define SBSDIO_DEVCTL_SETBUSY		0x01		/* 1: device will assert busy signal when
							 * receiving CMD53
							 */
#define SBSDIO_DEVCTL_SPI_INTR_SYNC	0x02		/* 1: assertion of sdio interrupt is
							 * synchronous to the sdio clock
							 */
#define SBSDIO_DEVCTL_CA_INT_ONLY	0x04		/* 1: mask all interrupts to host
							 * except the chipActive (rev 8)
							 */
#define SBSDIO_DEVCTL_PADS_ISO		0x08		/* 1: isolate internal sdio signals, put
							 * external pads in tri-state; requires
							 * sdio bus power cycle to clear (rev 9)
							 */


/* Include the soci specific files */
//#include <sbconfig.h>
//#include <aidmp.h>

/*
 * SOC Interconnect Address Map.
 * All regions may not exist on all chips.
 */
#define SI_SDRAM_BASE		0x00000000	/* Physical SDRAM */
#define SI_PCI_MEM			0x08000000	/* Host Mode sb2pcitranslation0 (64 MB) */
#define SI_PCI_MEM_SZ		(64 * 1024 * 1024)
#define SI_PCI_CFG			0x0c000000	/* Host Mode sb2pcitranslation1 (64 MB) */
#define	SI_SDRAM_SWAPPED	0x10000000	/* Byteswapped Physical SDRAM */
#define SI_SDRAM_R2			0x80000000	/* Region 2 for sdram (512 MB) */

#define SI_ENUM_BASE    	0x18000000	/* Enumeration space base */

#define SI_WRAP_BASE    	0x18100000	/* Wrapper space base */
#define SI_CORE_SIZE    	0x1000		/* each core gets 4Kbytes for registers */


#define	SI_FASTRAM			0x19000000	/* On-chip RAM on chips that also have DDR */
#define	SI_FASTRAM_SWAPPED	0x19800000

#define	SI_FLASH2			0x1c000000	/* Flash Region 2 (region 1 shadowed here) */
#define	SI_FLASH2_SZ		0x02000000	/* Size of Flash Region 2 */
#define	SI_ARMCM3_ROM		0x1e000000	/* ARM Cortex-M3 ROM */
#define	SI_FLASH1			0x1fc00000	/* MIPS Flash Region 1 */
#define	SI_FLASH1_SZ		0x00400000	/* MIPS Size of Flash Region 1 */
#define	SI_ARM7S_ROM		0x20000000	/* ARM7TDMI-S ROM */
#define	SI_ARMCM3_SRAM2		0x60000000	/* ARM Cortex-M3 SRAM Region 2 */
#define	SI_ARM7S_SRAM2		0x80000000	/* ARM7TDMI-S SRAM Region 2 */
#define	SI_ARM_FLASH1		0xffff0000	/* ARM Flash Region 1 */
#define	SI_ARM_FLASH1_SZ	0x00010000	/* ARM Size of Flash Region 1 */

#define SI_PCI_DMA			0x40000000	/* Client Mode sb2pcitranslation2 (1 GB) */
#define SI_PCI_DMA2			0x80000000	/* Client Mode sb2pcitranslation2 (1 GB) */
#define SI_PCI_DMA_SZ		0x40000000	/* Client Mode sb2pcitranslation2 size in bytes */
#define SI_PCIE_DMA_L32		0x00000000	/* PCIE Client Mode sb2pcitranslation2
* (2 ZettaBytes), low 32 bits
*/
#define SI_PCIE_DMA_H32		0x80000000	/* PCIE Client Mode sb2pcitranslation2
* (2 ZettaBytes), high 32 bits
*/

/* SBSDIO_FUNC1_CHIPCLKCSR */
#define SBSDIO_FORCE_ALP			0x01		/* Force ALP request to backplane */
#define SBSDIO_FORCE_HT				0x02		/* Force HT request to backplane */
#define SBSDIO_FORCE_ILP			0x04		/* Force ILP request to backplane */
#define SBSDIO_ALP_AVAIL_REQ		0x08		/* Make ALP ready (power up xtal) */
#define SBSDIO_HT_AVAIL_REQ			0x10		/* Make HT ready (power up PLL) */
#define SBSDIO_FORCE_HW_CLKREQ_OFF	0x20		/* Squelch clock requests from HW */
#define SBSDIO_ALP_AVAIL			0x40		/* Status: ALP is ready */
#define SBSDIO_HT_AVAIL				0x80		/* Status: HT is ready */

/* Value for ChipClockCSR during initial setup */
#define DHD_INIT_CLKCTL1	(SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_ALP_AVAIL_REQ)
#define DHD_INIT_CLKCTL2	(SBSDIO_FORCE_HW_CLKREQ_OFF | SBSDIO_FORCE_ALP)

#define SBSDIO_AVBITS			(SBSDIO_HT_AVAIL | SBSDIO_ALP_AVAIL)
#define SBSDIO_ALPAV(regval)		((regval) & SBSDIO_AVBITS)
#define SBSDIO_HTAV(regval)		(((regval) & SBSDIO_AVBITS) == SBSDIO_AVBITS)
#define SBSDIO_ALPONLY(regval)		(SBSDIO_ALPAV(regval) && !SBSDIO_HTAV(regval))
#define SBSDIO_CLKAV(regval, alponly)	(SBSDIO_ALPAV(regval) && (alponly ? 1 : SBSDIO_HTAV(regval)))

#define SPINWAIT(exp, us) { \
	uint countdown = (us) + 9; \
	while ((exp) && (countdown >= 10)) {\
		DelayUs(10); \
		countdown -= 10; \
	} \
}

#define PMU_MAX_TRANSITION_DLY	15000

#define	VENDOR_EPIGRAM		0xfeda
#define	VENDOR_BROADCOM		0x14e4
#define	VENDOR_3COM		0x10b7
#define	VENDOR_NETGEAR		0x1385
#define	VENDOR_DIAMOND		0x1092
#define	VENDOR_INTEL		0x8086
#define	VENDOR_DELL		0x1028
#define	VENDOR_HP		0x103c
#define	VENDOR_HP_COMPAQ	0x0e11
#define	VENDOR_APPLE		0x106b
#define VENDOR_SI_IMAGE		0x1095
#define VENDOR_BUFFALO		0x1154
#define VENDOR_TI		0x104c
#define VENDOR_RICOH		0x1180
#define VENDOR_JMICRON		0x197b

#define	SI_BUS			0
#define	PCI_BUS			1
#define	PCMCIA_BUS		2
#define SDIO_BUS		3
#define JTAG_BUS		4
#define USB_BUS			5
#define SPI_BUS			6
#define RPC_BUS			7

#define BCM43362_CHIP_ID	43362 //DGL It IS RK901
#define BCM4330_CHIP_ID		0x4330

#define	CC_CAP_UARTS_MASK	0x00000003
#define CC_CAP_MIPSEB		0x00000004
#define CC_CAP_UCLKSEL		0x00000018
#define CC_CAP_UINTCLK		0x00000008
#define CC_CAP_UARTGPIO		0x00000020
#define CC_CAP_EXTBUS_MASK	0x000000c0
#define CC_CAP_EXTBUS_NONE	0x00000000
#define CC_CAP_EXTBUS_FULL	0x00000040
#define CC_CAP_EXTBUS_PROG	0x00000080
#define	CC_CAP_FLASH_MASK	0x00000700
#define	CC_CAP_PLL_MASK		0x00038000
#define CC_CAP_PWR_CTL		0x00040000
#define CC_CAP_OTPSIZE		0x00380000
#define CC_CAP_OTPSIZE_SHIFT	19
#define CC_CAP_OTPSIZE_BASE	5
#define CC_CAP_JTAGP		0x00400000
#define CC_CAP_ROM		0x00800000
#define CC_CAP_BKPLN64		0x08000000
#define	CC_CAP_PMU		0x10000000
#define	CC_CAP_ECI		0x20000000
#define	CC_CAP_SROM		0x40000000
#define	CC_CAP_NFLASH		0x80000000

#define	CC_CAP2_SECI		0x00000001
#define	CC_CAP2_GSIO		0x00000002


#define CC_CAP_EXT_SECI_PRESENT   0x00000001

/*
 * Macros to disable/restore function core(D11, ENET, ILINE20, etc) interrupts before/
 * after core switching to avoid invalid register accesss inside ISR.
 */
#define INTR_OFF(si, intr_val) \
	if ((si)->intrsoff_fn && (si)->coreid[(si)->curidx] == (si)->dev_coreid) {	\
		intr_val = (*(si)->intrsoff_fn)((si)->intr_arg); }
#define INTR_RESTORE(si, intr_val) \
	if ((si)->intrsrestore_fn && (si)->coreid[(si)->curidx] == (si)->dev_coreid) {	\
		(*(si)->intrsrestore_fn)((si)->intr_arg, intr_val); }

#define	GOODCOREADDR(x, b) (((x) >= (b)) && ((x) < ((b) + SI_MAXCORES * SI_CORE_SIZE)) && ISALIGNED((x), SI_CORE_SIZE))
#define	GOODREGS(regs)	((regs) != NULL && ISALIGNED((unsigned int)(regs), SI_CORE_SIZE))
#define BADCOREADDR	0
#define	GOODIDX(idx)	(((uint)idx) < SI_MAXCORES)
#define	NOREV		-1		/* Invalid rev */




/* There are TWO constants on all HND chips: SI_ENUM_BASE above,
 * and chipcommon being the first core:
 */
#define	SI_CC_IDX		0

/* SOC Interconnect types (aka chip types) */
#define	SOCI_SB			0
#define	SOCI_AI			1
#define	SOCI_UBUS		2


/* core codes */
#define	NODEV_CORE_ID		0x700		/* Invalid coreid */
#define	CC_CORE_ID			0x800		/* chipcommon core */
#define	ILINE20_CORE_ID		0x801		/* iline20 core */
#define	SRAM_CORE_ID		0x802		/* sram core */
#define	SDRAM_CORE_ID		0x803		/* sdram core */
#define	PCI_CORE_ID			0x804		/* pci core */
#define	MIPS_CORE_ID		0x805		/* mips core */
#define	ENET_CORE_ID		0x806		/* enet mac core */
#define	CODEC_CORE_ID		0x807		/* v90 codec core */
#define	USB_CORE_ID			0x808		/* usb 1.1 host/device core */
#define	ADSL_CORE_ID		0x809		/* ADSL core */
#define	ILINE100_CORE_ID	0x80a		/* iline100 core */
#define	IPSEC_CORE_ID		0x80b		/* ipsec core */
#define	UTOPIA_CORE_ID		0x80c		/* utopia core */
#define	PCMCIA_CORE_ID		0x80d		/* pcmcia core */
#define	SOCRAM_CORE_ID		0x80e		/* internal memory core */
#define	MEMC_CORE_ID		0x80f		/* memc sdram core */
#define	OFDM_CORE_ID		0x810		/* OFDM phy core */
#define	EXTIF_CORE_ID		0x811		/* external interface core */
#define	D11_CORE_ID			0x812		/* 802.11 MAC core */
#define	APHY_CORE_ID		0x813		/* 802.11a phy core */
#define	BPHY_CORE_ID		0x814		/* 802.11b phy core */
#define	GPHY_CORE_ID		0x815		/* 802.11g phy core */
#define	MIPS33_CORE_ID		0x816		/* mips3302 core */
#define	USB11H_CORE_ID		0x817		/* usb 1.1 host core */
#define	USB11D_CORE_ID		0x818		/* usb 1.1 device core */
#define	USB20H_CORE_ID		0x819		/* usb 2.0 host core */
#define	USB20D_CORE_ID		0x81a		/* usb 2.0 device core */
#define	SDIOH_CORE_ID		0x81b		/* sdio host core */
#define	ROBO_CORE_ID		0x81c		/* roboswitch core */
#define	ATA100_CORE_ID		0x81d		/* parallel ATA core */
#define	SATAXOR_CORE_ID		0x81e		/* serial ATA & XOR DMA core */
#define	GIGETH_CORE_ID		0x81f		/* gigabit ethernet core */
#define	PCIE_CORE_ID		0x820		/* pci express core */
#define	NPHY_CORE_ID		0x821		/* 802.11n 2x2 phy core */
#define	SRAMC_CORE_ID		0x822		/* SRAM controller core */
#define	MINIMAC_CORE_ID		0x823		/* MINI MAC/phy core */
#define	ARM11_CORE_ID		0x824		/* ARM 1176 core */
#define	ARM7S_CORE_ID		0x825		/* ARM7tdmi-s core */
#define	LPPHY_CORE_ID		0x826		/* 802.11a/b/g phy core */
#define	PMU_CORE_ID			0x827		/* PMU core */
#define	SSNPHY_CORE_ID		0x828		/* 802.11n single-stream phy core */
#define	SDIOD_CORE_ID		0x829		/* SDIO device core */
#define	ARMCM3_CORE_ID		0x82a		/* ARM Cortex M3 core */
#define	HTPHY_CORE_ID		0x82b		/* 802.11n 4x4 phy core */
#define	MIPS74K_CORE_ID		0x82c		/* mips 74k core */
#define	GMAC_CORE_ID		0x82d		/* Gigabit MAC core */
#define	DMEMC_CORE_ID		0x82e		/* DDR1/2 memory controller core */
#define	PCIERC_CORE_ID		0x82f		/* PCIE Root Complex core */
#define	OCP_CORE_ID			0x830		/* OCP2OCP bridge core */
#define	SC_CORE_ID			0x831		/* shared common core */
#define	AHB_CORE_ID			0x832		/* OCP2AHB bridge core */
#define	SPIH_CORE_ID		0x833		/* SPI host core */
#define	I2S_CORE_ID			0x834		/* I2S core */
#define	DMEMS_CORE_ID		0x835		/* SDR/DDR1 memory controller core */
#define	DEF_SHIM_COMP		0x837		/* SHIM component in ubus/6362 */
#define OOB_ROUTER_CORE_ID	0x367		/* OOB router core ID */
#define	DEF_AI_COMP			0xfff		/* Default component, in ai chips it maps all
* unused address ranges
*/

#define SDIOD_DRVSTR_KEY(chip, pmu)	(((chip) << 16) | (pmu))

/* function 1 OCP space */
#define SBSDIO_SB_OFT_ADDR_MASK		0x07FFF		/* sb offset addr is <= 15 bits, 32k */
#define SBSDIO_SB_OFT_ADDR_LIMIT	0x08000
#define SBSDIO_SB_ACCESS_2_4B_FLAG	0x08000		/* with b15, maps to 32-bit SB access */

#define DHD_SDIOD_DRIVER_STRENGTH 6

#define	CIA_MFG_MASK		0xfff00000
#define	CIA_MFG_SHIFT		20
#define	CIA_CID_MASK		0x000fff00
#define	CIA_CID_SHIFT		8
#define	CIA_CCL_MASK		0x000000f0
#define	CIA_CCL_SHIFT		4


#define	CIB_REV_MASK		0xff000000
#define	CIB_REV_SHIFT		24
#define	CIB_NSW_MASK		0x00f80000
#define	CIB_NSW_SHIFT		19
#define	CIB_NMW_MASK		0x0007c000
#define	CIB_NMW_SHIFT		14
#define	CIB_NSP_MASK		0x00003e00
#define	CIB_NSP_SHIFT		9
#define	CIB_NMP_MASK		0x000001f0
#define	CIB_NMP_SHIFT		4

/* Common core control flags */
#define	SICF_BIST_EN		0x8000
#define	SICF_PME_EN			0x4000
#define	SICF_CORE_BITS		0x3ffc
#define	SICF_FGC			0x0002
#define	SICF_CLOCK_EN		0x0001

#define	AIRC_RESET		1

#define	SR_COREINFO		0x00
#define	SR_BWALLOC		0x04
#define	SR_BISTSTAT		0x0c
#define	SR_BANKINDEX		0x10
#define	SR_BANKSTBYCTL		0x14
#define SR_PWRCTL		0x1e8


#define	SRCI_PT_MASK		0x00070000
#define	SRCI_PT_SHIFT		16

#define SRCI_PT_OCP_OCP		0
#define SRCI_PT_AXI_OCP		1
#define SRCI_PT_ARM7AHB_OCP	2
#define SRCI_PT_CM3AHB_OCP	3
#define SRCI_PT_AXI_AXI		4
#define SRCI_PT_AHB_AXI		5

#define SRCI_LSS_MASK		0x00f00000
#define SRCI_LSS_SHIFT		20
#define SRCI_LRS_MASK		0x0f000000
#define SRCI_LRS_SHIFT		24


#define	SRCI_MS0_MASK		0xf
#define SR_MS0_BASE		16


#define	SRCI_ROMNB_MASK		0xf000
#define	SRCI_ROMNB_SHIFT	12
#define	SRCI_ROMBSZ_MASK	0xf00
#define	SRCI_ROMBSZ_SHIFT	8
#define	SRCI_SRNB_MASK		0xf0
#define	SRCI_SRNB_SHIFT		4
#define	SRCI_SRBSZ_MASK		0xf
#define	SRCI_SRBSZ_SHIFT	0

#define SR_BSZ_BASE		14


#define	SRSC_SBYOVR_MASK	0x80000000
#define	SRSC_SBYOVR_SHIFT	31
#define	SRSC_SBYOVRVAL_MASK	0x60000000
#define	SRSC_SBYOVRVAL_SHIFT	29
#define	SRSC_SBYEN_MASK		0x01000000
#define	SRSC_SBYEN_SHIFT	24


#define SRPC_PMU_STBYDIS_MASK	0x00000010
#define SRPC_PMU_STBYDIS_SHIFT	4
#define SRPC_STBYOVRVAL_MASK	0x00000008
#define SRPC_STBYOVRVAL_SHIFT	3
#define SRPC_STBYOVR_MASK	0x00000007
#define SRPC_STBYOVR_SHIFT	0


#define SRECC_NUM_BANKS_MASK   0x000000F0
#define SRECC_NUM_BANKS_SHIFT  4
#define SRECC_BANKSIZE_MASK    0x0000000F
#define SRECC_BANKSIZE_SHIFT   0

#define SRECC_BANKSIZE(value)	 (1 << (value))


#define SRCBPC_PATCHENABLE 0x80000000

#define SRP_ADDRESS   0x0001FFFC
#define SRP_VALID     0x8000


#define SRCMD_WRITE  0x00020000
#define SRCMD_READ   0x00010000
#define SRCMD_DONE   0x80000000

#define SRCMD_DONE_DLY	1000


#define SOCRAM_BANKINFO_SZMASK		0x3f
#define SOCRAM_BANKIDX_ROM_MASK		0x100

#define SOCRAM_BANKIDX_MEMTYPE_SHIFT	8

#define SOCRAM_MEMTYPE_RAM		0
#define SOCRAM_MEMTYPE_R0M		1
#define SOCRAM_MEMTYPE_DEVRAM		2

#define	SOCRAM_BANKINFO_REG		0x40
#define	SOCRAM_BANKIDX_REG		0x10
#define	SOCRAM_BANKINFO_STDBY_MASK	0x400
#define	SOCRAM_BANKINFO_STDBY_TIMER	0x800


#define SOCRAM_BANKINFO_DEVRAMSEL_SHIFT	13
#define SOCRAM_BANKINFO_DEVRAMSEL_MASK	0x2000
#define SOCRAM_BANKINFO_DEVRAMPRO_SHIFT	14
#define SOCRAM_BANKINFO_DEVRAMPRO_MASK	0x4000


#define SOCRAM_DEVRAMBANK_MASK		0xF000
#define SOCRAM_DEVRAMBANK_SHIFT		12


#define	SOCRAM_BANKINFO_SZBASE		8192
#define SOCRAM_BANKSIZE_SHIFT		13

/* corecontrol */
#define CC_CISRDY		(1 << 0)	/* CIS Ready */
#define CC_BPRESEN		(1 << 1)	/* CCCR RES signal causes backplane reset */
#define CC_F2RDY		(1 << 2)	/* set CCCR IOR2 bit */
#define CC_CLRPADSISO		(1 << 3)	/* clear SDIO pads isolation bit (rev 11) */
#define CC_XMTDATAAVAIL_MODE	(1 << 4)	/* data avail generates an interrupt */
#define CC_XMTDATAAVAIL_CTRL	(1 << 5)	/* data avail interrupt ctrl */

#define SDIO_DEVICE_HMB_RXINT		0	/* default old way */
#define SDIO_DEVICE_RXDATAINT_MODE_0	1	/* from sdiod rev 4 */
#define SDIO_DEVICE_RXDATAINT_MODE_1	2	/* from sdiod rev 4 */


/* HW frame tag */
#define SDPCM_FRAMETAG_LEN	4	/* HW frametag: 2 bytes len, 2 bytes check val */
#define SDPCM_SWHEADER_LEN	8	/* SW header is 64 bits */

/* Flags bits */
#define SDIO_REQ_4BYTE	0x1	/* Four-byte target (backplane) width (vs. two-byte) */
#define SDIO_REQ_FIXED	0x2	/* Fixed address (FIFO) (vs. incrementing address) */
#define SDIO_REQ_ASYNC	0x4	/* Async request (vs. sync request) */

/* Total length of frame header for dongle protocol */
/* Total length of frame header for dongle protocol */
#define SDPCM_HDRLEN	(SDPCM_FRAMETAG_LEN + SDPCM_SWHEADER_LEN)
#define SDPCM_RESERVE	(SDPCM_HDRLEN + DHD_SDALIGN)
/* Space for header read, limit for data packets */


#define MAX_RX_DATASZ	2048


#define ALIGNMENT  4
#define F2SYNC	(SDIO_REQ_4BYTE | SDIO_REQ_FIXED)

/* clkstate */
#define CLK_NONE	0
#define CLK_SDONLY	1
#define CLK_PENDING	2	/* Not used yet */
#define CLK_AVAIL	3

#define DHD_IDLETIME_TICKS 1

/* per-driver magic numbers */
#define DHD_IOCTL_MAGIC		0x00444944

/* bump this number if you change the ioctl interface */
#define DHD_IOCTL_VERSION	1

#define	DHD_IOCTL_MAXLEN	8192		/* max length ioctl buffer required */
#define	DHD_IOCTL_SMLEN		256		/* "small" length ioctl buffer required */

/* common ioctl definitions */
#define DHD_GET_MAGIC				0
#define DHD_GET_VERSION				1
#define DHD_GET_VAR				2
#define DHD_SET_VAR				3

/* message levels */
#define DHD_ERROR_VAL	0x0001
#define DHD_TRACE_VAL	0x0002
#define DHD_INFO_VAL	0x0004
#define DHD_DATA_VAL	0x0008
#define DHD_CTL_VAL	0x0010
#define DHD_TIMER_VAL	0x0020
#define DHD_HDRS_VAL	0x0040
#define DHD_BYTES_VAL	0x0080
#define DHD_INTR_VAL	0x0100
#define DHD_LOG_VAL	0x0200
#define DHD_GLOM_VAL	0x0400
#define DHD_EVENT_VAL	0x0800
#define DHD_BTA_VAL	0x1000
#define DHD_ISCAN_VAL	0x2000
#define DHD_ARPOE_VAL	0x4000
#define DHD_IW_VAL	0x10000
#define DHD_CFG_VAL	0x20000


/* Enter idle immediately (no timeout) */
#define DHD_IDLE_IMMEDIATE	(-1)

/* Values for idleclock iovar: other values are the sd_divisor to use when idle */
#define DHD_IDLE_ACTIVE	0	/* Do not request any SD clock change when idle */
#define DHD_IDLE_STOP   (-1)	/* Request SD clock be stopped (and use SD1 mode) */

#define MEMBLOCK			2048	/* Block size used for downloading of dongle image */
#define MAX_NVRAMBUF_SIZE	2048	/* max nvram buf size */

/* Maximum milliseconds to wait for F2 to come up */
#define DHD_WAIT_F2RDY	3000




#define WLC_IOCTL_MAGIC     0x14e46c77


#define WLC_IOCTL_VERSION   1

#define WLC_IOCTL_MAXLEN        1024//dgl 8192
#define WLC_IOCTL_SMLEN         256
#define WLC_IOCTL_MEDLEN        536 //1536


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


/* intstatus */
#define I_SMB_SW0	(1 << 0)	/* To SB Mail S/W interrupt 0 */
#define I_SMB_SW1	(1 << 1)	/* To SB Mail S/W interrupt 1 */
#define I_SMB_SW2	(1 << 2)	/* To SB Mail S/W interrupt 2 */
#define I_SMB_SW3	(1 << 3)	/* To SB Mail S/W interrupt 3 */
#define I_SMB_SW_MASK	0x0000000f	/* To SB Mail S/W interrupts mask */
#define I_SMB_SW_SHIFT	0		/* To SB Mail S/W interrupts shift */
#define I_HMB_SW0	(1 << 4)	/* To Host Mail S/W interrupt 0 */
#define I_HMB_SW1	(1 << 5)	/* To Host Mail S/W interrupt 1 */
#define I_HMB_SW2	(1 << 6)	/* To Host Mail S/W interrupt 2 */
#define I_HMB_SW3	(1 << 7)	/* To Host Mail S/W interrupt 3 */
#define I_HMB_SW_MASK	0x000000f0	/* To Host Mail S/W interrupts mask */
#define I_HMB_SW_SHIFT	4		/* To Host Mail S/W interrupts shift */
#define I_WR_OOSYNC	(1 << 8)	/* Write Frame Out Of Sync */
#define I_RD_OOSYNC	(1 << 9)	/* Read Frame Out Of Sync */
#define	I_PC		(1 << 10)	/* descriptor error */
#define	I_PD		(1 << 11)	/* data error */
#define	I_DE		(1 << 12)	/* Descriptor protocol Error */
#define	I_RU		(1 << 13)	/* Receive descriptor Underflow */
#define	I_RO		(1 << 14)	/* Receive fifo Overflow */
#define	I_XU		(1 << 15)	/* Transmit fifo Underflow */
#define	I_RI		(1 << 16)	/* Receive Interrupt */
#define I_BUSPWR	(1 << 17)	/* SDIO Bus Power Change (rev 9) */
#define I_XMTDATA_AVAIL (1 << 23)	/* bits in fifo */
#define	I_XI		(1 << 24)	/* Transmit Interrupt */
#define I_RF_TERM	(1 << 25)	/* Read Frame Terminate */
#define I_WF_TERM	(1 << 26)	/* Write Frame Terminate */
#define I_PCMCIA_XU	(1 << 27)	/* PCMCIA Transmit FIFO Underflow */
#define I_SBINT		(1 << 28)	/* sbintstatus Interrupt */
#define I_CHIPACTIVE	(1 << 29)	/* chip transitioned from doze to active state */
#define I_SRESET	(1 << 30)	/* CCCR RES interrupt */
#define I_IOE2		(1U << 31)	/* CCCR IOE2 Bit Changed */
#define	I_ERRORS	(I_PC | I_PD | I_DE | I_RU | I_RO | I_XU)	/* DMA Errors */
#define I_DMA		(I_RI | I_XI | I_ERRORS)

/* sbintstatus */
#define I_SB_SERR	(1 << 8)	/* Backplane SError (write) */
#define I_SB_RESPERR	(1 << 9)	/* Backplane Response Error (read) */
#define I_SB_SPROMERR	(1 << 10)	/* Error accessing the sprom */

#define HOSTINTMASK		(I_HMB_SW_MASK | I_CHIPACTIVE)

/* function 1 miscellaneous registers */
#define SBSDIO_SPROM_CS			0x10000		/* sprom command and status */
#define SBSDIO_SPROM_INFO		0x10001		/* sprom info register */
#define SBSDIO_SPROM_DATA_LOW		0x10002		/* sprom indirect access data byte 0 */
#define SBSDIO_SPROM_DATA_HIGH		0x10003 	/* sprom indirect access data byte 1 */
#define SBSDIO_SPROM_ADDR_LOW		0x10004		/* sprom indirect access addr byte 0 */
#define SBSDIO_SPROM_ADDR_HIGH		0x10005		/* sprom indirect access addr byte 0 */
#define SBSDIO_CHIP_CTRL_DATA		0x10006		/* xtal_pu (gpio) output */
#define SBSDIO_CHIP_CTRL_EN		0x10007		/* xtal_pu (gpio) enable */
#define SBSDIO_WATERMARK		0x10008		/* rev < 7, watermark for sdio device */
#define SBSDIO_DEVICE_CTL		0x10009		/* control busy signal generation */

#define WATER_MARK 8
#define CMD_BUF_MAX    		2048	//2048
#define CMD_RXBUF_HEAD 		160 	//== 32 * 5
#define DATA_RXBUF_HEAD 	160		//== 32 * 5
#define ALIGN(x, a)	(((x) + (a) - 1) & ~((a) - 1))

#define HTON16(i) BCMSWAP16(i)
#define	hton16(i) bcmswap16(i)
#define	HTON32(i) BCMSWAP32(i)
#define	hton32(i) bcmswap32(i)
#define	NTOH16(i) BCMSWAP16(i)
#define	ntoh16(i) bcmswap16(i)
#define	NTOH32(i) BCMSWAP32(i)
#define	ntoh32(i) bcmswap32(i)
#define LTOH16(i) (i)
#define ltoh16(i) (i)
#define LTOH32(i) (i)
#define ltoh32(i) (i)
#define HTOL16(i) (i)
#define htol16(i) (i)
#define HTOL32(i) (i)
#define htol32(i) (i)

#define CDC_MAX_MSG_SIZE   1518
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

#define WL_IOCTL_ACTION_GET				0x0
#define WL_IOCTL_ACTION_SET				0x1
#define WL_IOCTL_ACTION_OVL_IDX_MASK	0x1e//dgl null
#define WL_IOCTL_ACTION_OVL_RSV			0x20//dgl null
#define WL_IOCTL_ACTION_OVL				0x40//dgl null
#define WL_IOCTL_ACTION_MASK			0x7e
#define WL_IOCTL_ACTION_OVL_SHIFT		1

/*
 * Software-defined protocol header
 */

/* Current protocol version */
#define SDPCM_PROT_VERSION	4

/* SW frame header */
#define SDPCM_SEQUENCE_MASK		0x000000ff	/* Sequence Number Mask */
#define SDPCM_PACKET_SEQUENCE(p) (((uint8 *)p)[0] & 0xff) /* p starts w/SW Header */

#define SDPCM_CHANNEL_MASK		0x00000f00	/* Channel Number Mask */
#define SDPCM_CHANNEL_SHIFT		8		/* Channel Number Shift */
#define SDPCM_PACKET_CHANNEL(p) (((uint8 *)p)[1] & 0x0f) /* p starts w/SW Header */

#define SDPCM_FLAGS_MASK		0x0000f000	/* Mask of flag bits */
#define SDPCM_FLAGS_SHIFT		12		/* Flag bits shift */
#define SDPCM_PACKET_FLAGS(p) ((((uint8 *)p)[1] & 0xf0) >> 4) /* p starts w/SW Header */

/* Next Read Len: lookahead length of next frame, in 16-byte units (rounded up) */
#define SDPCM_NEXTLEN_MASK		0x00ff0000	/* Next Read Len Mask */
#define SDPCM_NEXTLEN_SHIFT		16		/* Next Read Len Shift */
#define SDPCM_NEXTLEN_VALUE(p) ((((uint8 *)p)[2] & 0xff) << 4) /* p starts w/SW Header */
#define SDPCM_NEXTLEN_OFFSET		2

/* Data Offset from SOF (HW Tag, SW Tag, Pad) */
#define SDPCM_DOFFSET_OFFSET		3		/* Data Offset */
#define SDPCM_DOFFSET_VALUE(p) 		(((uint8 *)p)[SDPCM_DOFFSET_OFFSET] & 0xff)
#define SDPCM_DOFFSET_MASK		0xff000000
#define SDPCM_DOFFSET_SHIFT		24

#define SDPCM_FCMASK_OFFSET		4		/* Flow control */
#define SDPCM_FCMASK_VALUE(p)		(((uint8 *)p)[SDPCM_FCMASK_OFFSET ] & 0xff)
#define SDPCM_WINDOW_OFFSET		5		/* Credit based fc */
#define SDPCM_WINDOW_VALUE(p)		(((uint8 *)p)[SDPCM_WINDOW_OFFSET] & 0xff)
#define SDPCM_VERSION_OFFSET		6		/* Version # */
#define SDPCM_VERSION_VALUE(p)		(((uint8 *)p)[SDPCM_VERSION_OFFSET] & 0xff)
#define SDPCM_UNUSED_OFFSET		7		/* Spare */
#define SDPCM_UNUSED_VALUE(p)		(((uint8 *)p)[SDPCM_UNUSED_OFFSET] & 0xff)

#define SDPCM_SWHEADER_LEN	8	/* SW header is 64 bits */

/* logical channel numbers */
#define SDPCM_CONTROL_CHANNEL	0	/* Control Request/Response Channel Id */
#define SDPCM_EVENT_CHANNEL		1	/* Asyc Event Indication Channel Id */
#define SDPCM_DATA_CHANNEL		2	/* Data Xmit/Recv Channel Id */
#define SDPCM_GLOM_CHANNEL		3	/* For coalesced packets (superframes) */
#define SDPCM_TEST_CHANNEL		15	/* Reserved for test/debug packets */
#define SDPCM_MAX_CHANNEL		15

#define SDPCM_SEQUENCE_WRAP	256	/* wrap-around val for eight-bit frame seq number */

#define SDPCM_FLAG_RESVD0	0x01
#define SDPCM_FLAG_RESVD1	0x02
#define SDPCM_FLAG_GSPI_TXENAB	0x04
#define SDPCM_FLAG_GLOMDESC	0x08	/* Superframe descriptor mask */

/* For GLOM_CHANNEL frames, use a flag to indicate descriptor frame */
#define SDPCM_GLOMDESC_FLAG	(SDPCM_FLAG_GLOMDESC << SDPCM_FLAGS_SHIFT)

#define SDPCM_GLOMDESC(p)	(((uint8 *)p)[1] & 0x80)

/* For TEST_CHANNEL packets, define another 4-byte header */
#define SDPCM_TEST_HDRLEN	4	/* Generally: Cmd(1), Ext(1), Len(2);
* Semantics of Ext byte depend on command.
* Len is current or requested frame length, not
* including test header; sent little-endian.
*/
#define SDPCM_TEST_DISCARD	0x01	/* Receiver discards. Ext is a pattern id. */
#define SDPCM_TEST_ECHOREQ	0x02	/* Echo request. Ext is a pattern id. */
#define SDPCM_TEST_ECHORSP	0x03	/* Echo response. Ext is a pattern id. */
#define SDPCM_TEST_BURST	0x04	/* Receiver to send a burst. Ext is a frame count */
#define SDPCM_TEST_SEND		0x05	/* Receiver sets send mode. Ext is boolean on/off */

/* Handy macro for filling in datagen packets with a pattern */
#define SDPCM_TEST_FILL(byteno, id)	((uint8)(id + byteno))
#define	IS_SIM(chippkg)	((chippkg == HDLSIM_PKG_ID) || (chippkg == HWSIM_PKG_ID))


/* tosbmailbox bits corresponding to intstatus bits */
#define SMB_NAK		(1 << 0)	/* To SB Mailbox Frame NAK */
#define SMB_INT_ACK	(1 << 1)	/* To SB Mailbox Host Interrupt ACK */
#define SMB_USE_OOB	(1 << 2)	/* To SB Mailbox Use OOB Wakeup */
#define SMB_DEV_INT	(1 << 3)	/* To SB Mailbox Miscellaneous Interrupt */
#define SMB_MASK	0x0000000f	/* To SB Mailbox Mask */

/* tosbmailboxdata */
#define SMB_DATA_VERSION_MASK	0x00ff0000	/* host protocol version (sent with F2 enable) */
#define SMB_DATA_VERSION_SHIFT	16		/* host protocol version (sent with F2 enable) */


#define SDIO_FUN1_BLOCK_SIZE 64
#define SDIO_FUN2_BLOCK_SIZE 512

#define	BADIDX		(SI_MAXCORES + 1)

#define	CID_ID_MASK		0x0000ffff
#define	CID_REV_MASK		0x000f0000
#define	CID_REV_SHIFT		16
#define	CID_PKG_MASK		0x00f00000
#define	CID_PKG_SHIFT		20
#define	CID_CC_MASK		0x0f000000
#define CID_CC_SHIFT		24
#define	CID_TYPE_MASK		0xf0000000
#define CID_TYPE_SHIFT		28

#define	VENDOR_EPIGRAM		0xfeda
#define	VENDOR_BROADCOM		0x14e4
#define	VENDOR_3COM		0x10b7
#define	VENDOR_NETGEAR		0x1385
#define	VENDOR_DIAMOND		0x1092
#define	VENDOR_INTEL		0x8086
#define	VENDOR_DELL		0x1028
#define	VENDOR_HP		0x103c
#define	VENDOR_HP_COMPAQ	0x0e11
#define	VENDOR_APPLE		0x106b
#define VENDOR_SI_IMAGE		0x1095
#define VENDOR_BUFFALO		0x1154
#define VENDOR_TI		0x104c
#define VENDOR_RICOH		0x1180
#define VENDOR_JMICRON		0x197b



#define	VENDOR_BROADCOM_PCMCIA	0x02d0


#define	VENDOR_BROADCOM_SDIO	0x00BF


#define BCM_DNGL_VID		0x0a5c
#define BCM_DNGL_BL_PID_4328	0xbd12
#define BCM_DNGL_BL_PID_4322	0xbd13
#define BCM_DNGL_BL_PID_4319    0xbd16
#define BCM_DNGL_BL_PID_43236   0xbd17
#define BCM_DNGL_BL_PID_4332	0xbd18
#define BCM_DNGL_BL_PID_4330	0xbd19
#define BCM_DNGL_BL_PID_43239   0xbd1b
#define BCM_DNGL_BDC_PID	0x0bdc
#define BCM_DNGL_JTAG_PID	0x4a44
#define BCM_DNGL_BL_PID_4324   0xbd1c


#define BCM_HWUSB_PID_43239     43239


#define	BCM4210_DEVICE_ID	0x1072
#define	BCM4230_DEVICE_ID	0x1086
#define	BCM4401_ENET_ID		0x170c
#define	BCM3352_DEVICE_ID	0x3352
#define	BCM3360_DEVICE_ID	0x3360
#define	BCM4211_DEVICE_ID	0x4211
#define	BCM4231_DEVICE_ID	0x4231
#define	BCM4303_D11B_ID		0x4303
#define	BCM4311_D11G_ID		0x4311
#define	BCM4311_D11DUAL_ID	0x4312
#define	BCM4311_D11A_ID		0x4313
#define	BCM4328_D11DUAL_ID	0x4314
#define	BCM4328_D11G_ID		0x4315
#define	BCM4328_D11A_ID		0x4316
#define	BCM4318_D11G_ID		0x4318
#define	BCM4318_D11DUAL_ID	0x4319
#define	BCM4318_D11A_ID		0x431a
#define	BCM4325_D11DUAL_ID	0x431b
#define	BCM4325_D11G_ID		0x431c
#define	BCM4325_D11A_ID		0x431d
#define	BCM4306_D11G_ID		0x4320
#define	BCM4306_D11A_ID		0x4321
#define	BCM4306_UART_ID		0x4322
#define	BCM4306_V90_ID		0x4323
#define	BCM4306_D11DUAL_ID	0x4324
#define	BCM4306_D11G_ID2	0x4325
#define	BCM4321_D11N_ID		0x4328
#define	BCM4321_D11N2G_ID	0x4329
#define	BCM4321_D11N5G_ID	0x432a
#define BCM4322_D11N_ID		0x432b
#define BCM4322_D11N2G_ID	0x432c
#define BCM4322_D11N5G_ID	0x432d
#define BCM4329_D11N_ID		0x432e
#define BCM4329_D11N2G_ID	0x432f
#define BCM4329_D11N5G_ID	0x4330
#define	BCM4315_D11DUAL_ID	0x4334
#define	BCM4315_D11G_ID		0x4335
#define	BCM4315_D11A_ID		0x4336
#define BCM4319_D11N_ID		0x4337
#define BCM4319_D11N2G_ID	0x4338
#define BCM4319_D11N5G_ID	0x4339
#define BCM43231_D11N2G_ID	0x4340
#define BCM43221_D11N2G_ID	0x4341
#define BCM43222_D11N_ID	0x4350
#define BCM43222_D11N2G_ID	0x4351
#define BCM43222_D11N5G_ID	0x4352
#define BCM43224_D11N_ID	0x4353
#define BCM43224_D11N_ID_VEN1	0x0576
#define BCM43226_D11N_ID	0x4354
#define BCM43236_D11N_ID	0x4346
#define BCM43236_D11N2G_ID	0x4347
#define BCM43236_D11N5G_ID	0x4348
#define BCM43225_D11N2G_ID	0x4357
#define BCM43421_D11N_ID	0xA99D
#define BCM4313_D11N2G_ID	0x4727
#define BCM4330_D11N_ID         0x4360
#define BCM4330_D11N2G_ID       0x4361
#define BCM4330_D11N5G_ID       0x4362
#define BCM4336_D11N_ID		0x4343
#define BCM6362_D11N_ID		0x435f
#define BCM4331_D11N_ID		0x4331
#define BCM4331_D11N2G_ID	0x4332
#define BCM4331_D11N5G_ID	0x4333
#define BCM43237_D11N_ID	0x4355
#define BCM43237_D11N5G_ID	0x4356
#define BCM43227_D11N2G_ID	0x4358
#define BCM43228_D11N_ID		0x4359
#define BCM43228_D11N5G_ID	0x435a
#define BCM43362_D11N_ID	0x4363
#define BCM43239_D11N_ID	0x4370
#define BCM4324_D11N_ID		0x4374
#define BCM43217_D11N2G_ID	0x43a9
#define BCM43131_D11N2G_ID	0x43aa

#define BCM4314_D11N2G_ID	0x4364
#define BCM43142_D11N2G_ID	0x4365

#define	BCMGPRS_UART_ID		0x4333
#define	BCMGPRS2_UART_ID	0x4344
#define FPGA_JTAGM_ID		0x43f0
#define BCM_JTAGM_ID		0x43f1
#define SDIOH_FPGA_ID		0x43f2
#define BCM_SDIOH_ID		0x43f3
#define SDIOD_FPGA_ID		0x43f4
#define SPIH_FPGA_ID		0x43f5
#define BCM_SPIH_ID		0x43f6
#define MIMO_FPGA_ID		0x43f8
#define BCM_JTAGM2_ID		0x43f9
#define SDHCI_FPGA_ID		0x43fa
#define	BCM4402_ENET_ID		0x4402
#define	BCM4402_V90_ID		0x4403
#define	BCM4410_DEVICE_ID	0x4410
#define	BCM4412_DEVICE_ID	0x4412
#define	BCM4430_DEVICE_ID	0x4430
#define	BCM4432_DEVICE_ID	0x4432
#define	BCM4704_ENET_ID		0x4706
#define	BCM4710_DEVICE_ID	0x4710
#define	BCM47XX_AUDIO_ID	0x4711
#define	BCM47XX_V90_ID		0x4712
#define	BCM47XX_ENET_ID		0x4713
#define	BCM47XX_EXT_ID		0x4714
#define	BCM47XX_GMAC_ID		0x4715
#define	BCM47XX_USBH_ID		0x4716
#define	BCM47XX_USBD_ID		0x4717
#define	BCM47XX_IPSEC_ID	0x4718
#define	BCM47XX_ROBO_ID		0x4719
#define	BCM47XX_USB20H_ID	0x471a
#define	BCM47XX_USB20D_ID	0x471b
#define	BCM47XX_ATA100_ID	0x471d
#define	BCM47XX_SATAXOR_ID	0x471e
#define	BCM47XX_GIGETH_ID	0x471f
#define	BCM4712_MIPS_ID		0x4720
#define	BCM4716_DEVICE_ID	0x4722
#define BCM47XX_SMBUS_EMU_ID	0x47fe
#define	BCM47XX_XOR_EMU_ID	0x47ff
#define	EPI41210_DEVICE_ID	0xa0fa
#define	EPI41230_DEVICE_ID	0xa10e
#define JINVANI_SDIOH_ID	0x4743
#define BCM27XX_SDIOH_ID	0x2702
#define PCIXX21_FLASHMEDIA_ID	0x803b
#define PCIXX21_SDIOH_ID	0x803c
#define R5C822_SDIOH_ID		0x0822
#define JMICRON_SDIOH_ID	0x2381


#define	BCM4306_CHIP_ID		0x4306
#define	BCM4311_CHIP_ID		0x4311
#define	BCM43111_CHIP_ID	43111
#define	BCM43112_CHIP_ID	43112
#define	BCM4312_CHIP_ID		0x4312
#define BCM4313_CHIP_ID		0x4313
#define	BCM43131_CHIP_ID	43131
#define	BCM4315_CHIP_ID		0x4315
#define	BCM4318_CHIP_ID		0x4318
#define	BCM4319_CHIP_ID		0x4319
#define	BCM4320_CHIP_ID		0x4320
#define	BCM4321_CHIP_ID		0x4321
#define	BCM43217_CHIP_ID	43217
#define	BCM4322_CHIP_ID		0x4322
#define	BCM43221_CHIP_ID	43221
#define	BCM43222_CHIP_ID	43222
#define	BCM43224_CHIP_ID	43224
#define	BCM43225_CHIP_ID	43225
#define	BCM43227_CHIP_ID	43227
#define	BCM43228_CHIP_ID	43228
#define	BCM43226_CHIP_ID	43226
#define	BCM43231_CHIP_ID	43231
#define	BCM43234_CHIP_ID	43234
#define	BCM43235_CHIP_ID	43235
#define	BCM43236_CHIP_ID	43236
#define	BCM43237_CHIP_ID	43237
#define	BCM43238_CHIP_ID	43238
#define	BCM43239_CHIP_ID	43239
#define	BCM43420_CHIP_ID	43420
#define	BCM43421_CHIP_ID	43421
#define	BCM43428_CHIP_ID	43428
#define	BCM43431_CHIP_ID	43431
#define	BCM4325_CHIP_ID		0x4325
#define	BCM4328_CHIP_ID		0x4328
#define	BCM4329_CHIP_ID		0x4329
#define	BCM4331_CHIP_ID		0x4331
#define BCM4336_CHIP_ID		0x4336
#define BCM43362_CHIP_ID	43362 //DGL It IS RK901
#define BCM4330_CHIP_ID		0x4330
#define BCM6362_CHIP_ID		0x6362
#define BCM4314_CHIP_ID		0x4314
#define BCM43142_CHIP_ID	43142
#define BCM4324_CHIP_ID		0x4324

#define	BCM4342_CHIP_ID		4342
#define	BCM4402_CHIP_ID		0x4402
#define	BCM4704_CHIP_ID		0x4704
#define	BCM4710_CHIP_ID		0x4710
#define	BCM4712_CHIP_ID		0x4712
#define	BCM4716_CHIP_ID		0x4716
#define	BCM47162_CHIP_ID	47162
#define	BCM4748_CHIP_ID		0x4748
#define	BCM4749_CHIP_ID		0x4749
#define BCM4785_CHIP_ID		0x4785
#define	BCM5350_CHIP_ID		0x5350
#define	BCM5352_CHIP_ID		0x5352
#define	BCM5354_CHIP_ID		0x5354
#define BCM5365_CHIP_ID		0x5365
#define	BCM5356_CHIP_ID		0x5356
#define	BCM5357_CHIP_ID		0x5357
#define	BCM53572_CHIP_ID	53572


#define	BCM4303_PKG_ID		2
#define	BCM4309_PKG_ID		1
#define	BCM4712LARGE_PKG_ID	0
#define	BCM4712SMALL_PKG_ID	1
#define	BCM4712MID_PKG_ID	2
#define BCM4328USBD11G_PKG_ID	2
#define BCM4328USBDUAL_PKG_ID	3
#define BCM4328SDIOD11G_PKG_ID	4
#define BCM4328SDIODUAL_PKG_ID	5
#define BCM4329_289PIN_PKG_ID	0
#define BCM4329_182PIN_PKG_ID	1
#define BCM5354E_PKG_ID		1
#define	BCM4716_PKG_ID		8
#define	BCM4717_PKG_ID		9
#define	BCM4718_PKG_ID		10
#define BCM5356_PKG_NONMODE	1
#define BCM5358U_PKG_ID		8
#define BCM5358_PKG_ID		9
#define BCM47186_PKG_ID		10
#define BCM5357_PKG_ID		11
#define BCM5356U_PKG_ID		12
#define BCM53572_PKG_ID		8
#define BCM47188_PKG_ID		9
#define BCM4331TT_PKG_ID        8
#define BCM4331TN_PKG_ID        9
#define BCM4331TNA0_PKG_ID     0xb


#define HDLSIM5350_PKG_ID	1
#define HDLSIM_PKG_ID		14
#define HWSIM_PKG_ID		15
#define BCM43224_FAB_CSM	0x8
#define BCM43224_FAB_SMIC	0xa
#define BCM4336_WLBGA_PKG_ID	0x8
#define BCM4330_WLBGA_PKG_ID	0x0
#define BCM4314PCIE_ARM_PKG_ID		(8 | 0)
#define BCM4314SDIO_PKG_ID		(8 | 1)
#define BCM4314PCIE_PKG_ID		(8 | 2)
#define BCM4314SDIO_ARM_PKG_ID		(8 | 3)
#define BCM4314SDIO_FPBGA_PKG_ID	(8 | 4)
#define BCM4314DEV_PKG_ID		(8 | 6)

#define PCIXX21_FLASHMEDIA0_ID	0x8033
#define PCIXX21_SDIOH0_ID	0x8034


#define	BFL_BTC2WIRE		0x00000001
#define BFL_BTCOEX      0x00000001
#define	BFL_PACTRL		0x00000002
#define BFL_AIRLINEMODE	0x00000004
#define	BFL_ADCDIV		0x00000008
#define	BFL_ENETROBO		0x00000010
#define	BFL_NOPLLDOWN		0x00000020
#define	BFL_CCKHIPWR		0x00000040
#define	BFL_ENETADM		0x00000080
#define	BFL_ENETVLAN		0x00000100
#ifdef WLAFTERBURNER
#define	BFL_AFTERBURNER		0x00000200
#endif
#define BFL_NOPCI		0x00000400
#define BFL_FEM			0x00000800
#define BFL_EXTLNA		0x00001000
#define BFL_HGPA		0x00002000
#define	BFL_BTC2WIRE_ALTGPIO	0x00004000
#define	BFL_ALTIQ		0x00008000
#define BFL_NOPA		0x00010000
#define BFL_RSSIINV		0x00020000
#define BFL_PAREF		0x00040000
#define BFL_3TSWITCH		0x00080000
#define BFL_PHASESHIFT		0x00100000
#define BFL_BUCKBOOST		0x00200000
#define BFL_FEM_BT		0x00400000
#define BFL_NOCBUCK		0x00800000
#define BFL_CCKFAVOREVM		0x01000000
#define BFL_PALDO		0x02000000
#define BFL_LNLDO2_2P5		0x04000000
#define BFL_FASTPWR		0x08000000
#define BFL_UCPWRCTL_MININDX	0x08000000
#define BFL_EXTLNA_5GHz		0x10000000
#define BFL_TRSW_1by2		0x20000000
#define BFL_LO_TRSW_R_5GHz	0x40000000
#define BFL_ELNA_GAINDEF	0x80000000
#define BFL_EXTLNA_TX	0x20000000


#define BFL2_RXBB_INT_REG_DIS	0x00000001
#define BFL2_APLL_WAR		0x00000002
#define BFL2_TXPWRCTRL_EN	0x00000004
#define BFL2_2X4_DIV		0x00000008
#define BFL2_5G_PWRGAIN		0x00000010
#define BFL2_PCIEWAR_OVR	0x00000020
#define BFL2_CAESERS_BRD	0x00000040
#define BFL2_BTC3WIRE		0x00000080
#define BFL2_BTCLEGACY          0x00000080
#define BFL2_SKWRKFEM_BRD	0x00000100
#define BFL2_SPUR_WAR		0x00000200
#define BFL2_GPLL_WAR		0x00000400
#define BFL2_TRISTATE_LED	0x00000800
#define BFL2_SINGLEANT_CCK	0x00001000
#define BFL2_2G_SPUR_WAR	0x00002000
#define BFL2_BPHY_ALL_TXCORES	0x00004000
#define BFL2_FCC_BANDEDGE_WAR	0x00008000
#define BFL2_GPLL_WAR2	        0x00010000
#define BFL2_IPALVLSHIFT_3P3    0x00020000
#define BFL2_INTERNDET_TXIQCAL  0x00040000
#define BFL2_XTALBUFOUTEN       0x00080000
#define BFL2_ANAPACTRL_2G	0x00100000
#define BFL2_ANAPACTRL_5G	0x00200000
#define BFL2_ELNACTRL_TRSW_2G	0x00400000
#define BFL2_BT_SHARE_ANT0	0x00800000
#define BFL2_TEMPSENSE_HIGHER	0x01000000
#define BFL2_BTC3WIREONLY       0x02000000
#define BFL2_PWR_NOMINAL	0x04000000
#define BFL2_EXTLNA_TX		0x08000000

#define BFL2_4313_RADIOREG	0x10000000



#define	BOARD_GPIO_BTC3W_IN	0x850
#define	BOARD_GPIO_BTC3W_OUT	0x020
#define	BOARD_GPIO_BTCMOD_IN	0x010
#define	BOARD_GPIO_BTCMOD_OUT	0x020
#define	BOARD_GPIO_BTC_IN	0x080
#define	BOARD_GPIO_BTC_OUT	0x100
#define	BOARD_GPIO_PACTRL	0x200
#define BOARD_GPIO_12		0x1000
#define BOARD_GPIO_13		0x2000
#define BOARD_GPIO_BTC4_IN	0x0800
#define BOARD_GPIO_BTC4_BT	0x2000
#define BOARD_GPIO_BTC4_STAT	0x4000
#define BOARD_GPIO_BTC4_WLAN	0x8000
#define	BOARD_GPIO_1_WLAN_PWR	0x2
#define	BOARD_GPIO_4_WLAN_PWR	0x10

#define GPIO_BTC4W_OUT_4312  0x010
#define GPIO_BTC4W_OUT_43224  0x020
#define GPIO_BTC4W_OUT_43224_SHARED  0x0e0
#define GPIO_BTC4W_OUT_43225  0x0e0
#define GPIO_BTC4W_OUT_43421  0x020
#define GPIO_BTC4W_OUT_4313  0x060

#define	PCI_CFG_GPIO_SCS	0x10
#define PCI_CFG_GPIO_HWRAD	0x20
#define PCI_CFG_GPIO_XTAL	0x40
#define PCI_CFG_GPIO_PLL	0x80


#define PLL_DELAY		150
#define FREF_DELAY		200
#define MIN_SLOW_CLK		32
#define	XTAL_ON_DELAY		1000


#define	BU4710_BOARD		0x0400
#define	VSIM4710_BOARD		0x0401
#define	QT4710_BOARD		0x0402

#define	BU4309_BOARD		0x040a
#define	BCM94309CB_BOARD	0x040b
#define	BCM94309MP_BOARD	0x040c
#define	BCM4309AP_BOARD		0x040d

#define	BCM94302MP_BOARD	0x040e

#define	BU4306_BOARD		0x0416
#define	BCM94306CB_BOARD	0x0417
#define	BCM94306MP_BOARD	0x0418

#define	BCM94710D_BOARD		0x041a
#define	BCM94710R1_BOARD	0x041b
#define	BCM94710R4_BOARD	0x041c
#define	BCM94710AP_BOARD	0x041d

#define	BU2050_BOARD		0x041f

#define	BCM94306P50_BOARD	0x0420

#define	BCM94309G_BOARD		0x0421

#define	BU4704_BOARD		0x0423
#define	BU4702_BOARD		0x0424

#define	BCM94306PC_BOARD	0x0425

#define	MPSG4306_BOARD		0x0427

#define	BCM94702MN_BOARD	0x0428


#define	BCM94702CPCI_BOARD	0x0429


#define	BCM95380RR_BOARD	0x042a


#define	BCM94306CBSG_BOARD	0x042b


#define	PCSG94306_BOARD		0x042d


#define	BU4704SD_BOARD		0x042e


#define	BCM94704AGR_BOARD	0x042f


#define	BCM94308MP_BOARD	0x0430


#define	BCM94306GPRS_BOARD	0x0432


#define BU5365_FPGA_BOARD	0x0433

#define BU4712_BOARD		0x0444
#define	BU4712SD_BOARD		0x045d
#define	BU4712L_BOARD		0x045f


#define BCM94712AP_BOARD	0x0445
#define BCM94712P_BOARD		0x0446


#define BU4318_BOARD		0x0447
#define CB4318_BOARD		0x0448
#define MPG4318_BOARD		0x0449
#define MP4318_BOARD		0x044a
#define SD4318_BOARD		0x044b


#define BCM94313BU_BOARD	0x050f
#define BCM94313HM_BOARD	0x0510
#define BCM94313EPA_BOARD	0x0511
#define BCM94313HMG_BOARD       0x051C


#define BCM96338_BOARD		0x6338
#define BCM96348_BOARD		0x6348
#define BCM96358_BOARD		0x6358
#define BCM96368_BOARD		0x6368


#define	BCM94306P_BOARD		0x044c


#define	BCM94303MP_BOARD	0x044e


#define	BCM94306MPSGH_BOARD	0x044f


#define BCM94306MPM		0x0450
#define BCM94306MPL		0x0453


#define	BCM94712AGR_BOARD	0x0451


#define	PC4303_BOARD		0x0454


#define	BCM95350K_BOARD		0x0455


#define	BCM95350R_BOARD		0x0456


#define	BCM94306MPLNA_BOARD	0x0457


#define	BU4320_BOARD		0x0458
#define	BU4320S_BOARD		0x0459
#define	BCM94320PH_BOARD	0x045a


#define	BCM94306MPH_BOARD	0x045b


#define	BCM94306PCIV_BOARD	0x045c

#define	BU4712SD_BOARD		0x045d

#define	BCM94320PFLSH_BOARD	0x045e

#define	BU4712L_BOARD		0x045f
#define	BCM94712LGR_BOARD	0x0460
#define	BCM94320R_BOARD		0x0461

#define	BU5352_BOARD		0x0462

#define	BCM94318MPGH_BOARD	0x0463

#define	BU4311_BOARD		0x0464
#define	BCM94311MC_BOARD	0x0465
#define	BCM94311MCAG_BOARD	0x0466

#define	BCM95352GR_BOARD	0x0467


#define	BCM95351AGR_BOARD	0x0470


#define	BCM94704MPCB_BOARD	0x0472


#define BU4785_BOARD		0x0478


#define BU4321_BOARD		0x046b
#define BU4321E_BOARD		0x047c
#define MP4321_BOARD		0x046c
#define CB2_4321_BOARD		0x046d
#define CB2_4321_AG_BOARD	0x0066
#define MC4321_BOARD		0x046e


#define BU4328_BOARD		0x0481
#define BCM4328SDG_BOARD	0x0482
#define BCM4328SDAG_BOARD	0x0483
#define BCM4328UG_BOARD		0x0484
#define BCM4328UAG_BOARD	0x0485
#define BCM4328PC_BOARD		0x0486
#define BCM4328CF_BOARD		0x0487


#define BCM94325DEVBU_BOARD	0x0490
#define BCM94325BGABU_BOARD	0x0491

#define BCM94325SDGWB_BOARD	0x0492

#define BCM94325SDGMDL_BOARD	0x04aa
#define BCM94325SDGMDL2_BOARD	0x04c6
#define BCM94325SDGMDL3_BOARD	0x04c9

#define BCM94325SDABGWBA_BOARD	0x04e1


#define BCM94322MC_SSID		0x04a4
#define BCM94322USB_SSID	0x04a8
#define BCM94322HM_SSID		0x04b0
#define BCM94322USB2D_SSID	0x04bf


#define	BCM4312MCGSG_BOARD	0x04b5


#define BCM94315DEVBU_SSID	0x04c2
#define BCM94315USBGP_SSID	0x04c7
#define BCM94315BGABU_SSID	0x04ca
#define BCM94315USBGP41_SSID	0x04cb


#define BCM94319DEVBU_SSID	0X04e5
#define BCM94319USB_SSID	0X04e6
#define BCM94319SD_SSID		0X04e7


#define BCM94716NR2_SSID	0x04cd


#define BCM94319DEVBU_SSID	0X04e5
#define BCM94319USBNP4L_SSID	0X04e6
#define BCM94319WLUSBN4L_SSID	0X04e7
#define BCM94319SDG_SSID	0X04ea
#define BCM94319LCUSBSDN4L_SSID	0X04eb
#define BCM94319USBB_SSID       0x04ee
#define BCM94319LCSDN4L_SSID	0X0507
#define BCM94319LSUSBN4L_SSID	0X0508
#define BCM94319SDNA4L_SSID	0X0517
#define BCM94319SDELNA4L_SSID	0X0518
#define BCM94319SDELNA6L_SSID	0X0539
#define BCM94319ARCADYAN_SSID	0X0546
#define BCM94319WINDSOR_SSID    0x0561
#define BCM94319MLAP_SSID       0x0562
#define BCM94319SDNA_SSID       0x058b
#define BCM94319BHEMU3_SSID     0x0563
#define BCM94319SDHMB_SSID     0x058c
#define BCM94319SDBREF_SSID     0x05a1
#define BCM94319USBSDB_SSID     0x05a2



#define BCM94329AGB_SSID	0X04b9
#define BCM94329TDKMDL1_SSID	0X04ba
#define BCM94329TDKMDL11_SSID	0X04fc
#define BCM94329OLYMPICN18_SSID	0X04fd
#define BCM94329OLYMPICN90_SSID	0X04fe
#define BCM94329OLYMPICN90U_SSID 0X050c
#define BCM94329OLYMPICN90M_SSID 0X050b
#define BCM94329AGBF_SSID	0X04ff
#define BCM94329OLYMPICX17_SSID	0X0504
#define BCM94329OLYMPICX17M_SSID	0X050a
#define BCM94329OLYMPICX17U_SSID	0X0509
#define BCM94329OLYMPICUNO_SSID	0X0564
#define BCM94329MOTOROLA_SSID   0X0565
#define BCM94329OLYMPICLOCO_SSID	0X0568

#define BCM94336SD_WLBGABU_SSID		0x0511
#define BCM94336SD_WLBGAREF_SSID	0x0519
#define BCM94336SDGP_SSID	0x0538
#define BCM94336SDG_SSID	0x0519
#define BCM94336SDGN_SSID	0x0538
#define BCM94336SDGFC_SSID	0x056B


#define BCM94330SDG_SSID	0x0528
#define BCM94330SD_FCBGABU_SSID	0x052e
#define BCM94330SD_WLBGABU_SSID	0x052f
#define BCM94330SD_FCBGA_SSID	0x0530
#define BCM94330FCSDAGB_SSID		0x0532
#define BCM94330OLYMPICAMG_SSID		0x0549
#define BCM94330OLYMPICAMGEPA_SSID		0x054F
#define BCM94330OLYMPICUNO3_SSID	0x0551
#define BCM94330WLSDAGB_SSID	0x0547
#define BCM94330CSPSDAGBB_SSID	0x054A


#define BCM943224X21        0x056e
#define BCM943224X21_FCC    0x00d1


#define BCM943228BU8_SSID	0x0540
#define BCM943228BU9_SSID	0x0541
#define BCM943228BU_SSID	0x0542
#define BCM943227HM4L_SSID	0x0543
#define BCM943227HMB_SSID	0x0544
#define BCM943228HM4L_SSID	0x0545
#define BCM943228SD_SSID	0x0573


#define BCM943239MOD_SSID	0x05ac
#define BCM943239REF_SSID	0x05aa


#define BCM94331X19               0x00D6
#define BCM94331PCIEBT3Ax_SSID    0x00E4
#define BCM94331X12_2G_SSID       0x00EC
#define BCM94331X12_5G_SSID       0x00ED
#define BCM94331X29B              0x00EF
#define BCM94331BU_SSID           0x0523
#define BCM94331S9BU_SSID         0x0524
#define BCM94331MC_SSID           0x0525
#define BCM94331MCI_SSID          0x0526
#define BCM94331PCIEBT4_SSID      0x0527
#define BCM94331HM_SSID           0x0574
#define BCM94331PCIEDUAL_SSID     0x059B
#define BCM94331MCH5_SSID         0x05A9
#define BCM94331PCIEDUALV2_SSID   0x05B7
#define BCM94331CS_SSID           0x05C6
#define BCM94331CSAX_SSID         0x00EF


#define BCM953572BU_SSID       0x058D
#define BCM953572NR2_SSID      0x058E
#define BCM947188NR2_SSID      0x058F
#define BCM953572SDRNR2_SSID   0x0590


#define BCM943236OLYMPICSULLEY_SSID 0x594
#define BCM943236PREPROTOBLU2O3_SSID 0x5b9
#define BCM943236USBELNA_SSID 0x5f8


#define GPIO_NUMPINS		32


#define RDL_RAM_BASE_4319 0x60000000
#define RDL_RAM_BASE_4329 0x60000000
#define RDL_RAM_SIZE_4319 0x48000
#define RDL_RAM_SIZE_4329  0x48000
#define RDL_RAM_SIZE_43236 0x70000
#define RDL_RAM_BASE_43236 0x60000000
#define RDL_RAM_SIZE_4328 0x60000
#define RDL_RAM_BASE_4328 0x80000000
#define RDL_RAM_SIZE_4322 0x60000
#define RDL_RAM_BASE_4322 0x60000000


#define MUXENAB_UART		0x00000001
#define MUXENAB_GPIO		0x00000002
#define MUXENAB_ERCX		0x00000004
#define MUXENAB_JTAG		0x00000008
#define MUXENAB_HOST_WAKE	0x00000010


#define CST4322_XTAL_FREQ_20_40MHZ	0x00000020
#define CST4322_SPROM_OTP_SEL_MASK	0x000000c0
#define CST4322_SPROM_OTP_SEL_SHIFT	6
#define CST4322_NO_SPROM_OTP		0
#define CST4322_SPROM_PRESENT		1
#define CST4322_OTP_PRESENT		2
#define CST4322_PCI_OR_USB		0x00000100
#define CST4322_BOOT_MASK		0x00000600
#define CST4322_BOOT_SHIFT		9
#define CST4322_BOOT_FROM_SRAM		0
#define CST4322_BOOT_FROM_ROM		1
#define CST4322_BOOT_FROM_FLASH		2
#define CST4322_BOOT_FROM_INVALID	3
#define CST4322_ILP_DIV_EN		0x00000800
#define CST4322_FLASH_TYPE_MASK		0x00001000
#define CST4322_FLASH_TYPE_SHIFT	12
#define CST4322_FLASH_TYPE_SHIFT_ST	0
#define CST4322_FLASH_TYPE_SHIFT_ATMEL	1
#define CST4322_ARM_TAP_SEL		0x00002000
#define CST4322_RES_INIT_MODE_MASK	0x0000c000
#define CST4322_RES_INIT_MODE_SHIFT	14
#define CST4322_RES_INIT_MODE_ILPAVAIL	0
#define CST4322_RES_INIT_MODE_ILPREQ	1
#define CST4322_RES_INIT_MODE_ALPAVAIL	2
#define CST4322_RES_INIT_MODE_HTAVAIL	3
#define CST4322_PCIPLLCLK_GATING	0x00010000
#define CST4322_CLK_SWITCH_PCI_TO_ALP	0x00020000
#define CST4322_PCI_CARDBUS_MODE	0x00040000

#define	ER_EROMENTRY		0x000
#define	ER_REMAPCONTROL		0xe00
#define	ER_REMAPSELECT		0xe04
#define	ER_MASTERSELECT		0xe10
#define	ER_ITCR			0xf00
#define	ER_ITIP			0xf04

#define	ER_TAG			0xe
#define	ER_TAG1			0x6
#define	ER_VALID		1
#define	ER_CI			0
#define	ER_MP			2
#define	ER_ADD			4
#define	ER_END			0xe
#define	ER_BAD			0xffffffff

#define	AD_ADDR_MASK		0xfffff000
#define	AD_SP_MASK		0x00000f00
#define	AD_SP_SHIFT		8
#define	AD_ST_MASK		0x000000c0
#define	AD_ST_SHIFT		6
#define	AD_ST_SLAVE		0x00000000
#define	AD_ST_BRIDGE		0x00000040
#define	AD_ST_SWRAP		0x00000080
#define	AD_ST_MWRAP		0x000000c0
#define	AD_SZ_MASK		0x00000030
#define	AD_SZ_SHIFT		4
#define	AD_SZ_4K		0x00000000
#define	AD_SZ_8K		0x00000010
#define	AD_SZ_16K		0x00000020
#define	AD_SZ_SZD		0x00000030
#define	AD_AG32			0x00000008
#define	AD_ADDR_ALIGN		0x00000fff
#define	AD_SZ_BASE		0x00001000

#define	SD_SZ_MASK		0xfffff000
#define	SD_SG32			0x00000008
#define	SD_SZ_ALIGN		0x00000fff

#define	MFGID_ARM		0x43b
#define	MFGID_BRCM		0x4bf
#define	MFGID_MIPS		0x4a7

#define PCAP_REV_MASK	0x000000ff
#define PCAP_RC_MASK	0x00001f00
#define PCAP_RC_SHIFT	8
#define PCAP_TC_MASK	0x0001e000
#define PCAP_TC_SHIFT	13
#define PCAP_PC_MASK	0x001e0000
#define PCAP_PC_SHIFT	17
#define PCAP_VC_MASK	0x01e00000
#define PCAP_VC_SHIFT	21
#define PCAP_CC_MASK	0x1e000000
#define PCAP_CC_SHIFT	25
#define PCAP5_PC_MASK	0x003e0000
#define PCAP5_PC_SHIFT	17
#define PCAP5_VC_MASK	0x07c00000
#define PCAP5_VC_SHIFT	22
#define PCAP5_CC_MASK	0xf8000000
#define PCAP5_CC_SHIFT	27


#define BCME_OK				0
#define BCME_ERROR			-1
#define BCME_BADARG			-2
#define BCME_BADOPTION			-3
#define BCME_NOTUP			-4
#define BCME_NOTDOWN			-5
#define BCME_NOTAP			-6
#define BCME_NOTSTA			-7
#define BCME_BADKEYIDX			-8
#define BCME_RADIOOFF 			-9
#define BCME_NOTBANDLOCKED		-10
#define BCME_NOCLK			-11
#define BCME_BADRATESET			-12
#define BCME_BADBAND			-13
#define BCME_BUFTOOSHORT		-14
#define BCME_BUFTOOLONG			-15
#define BCME_BUSY			-16
#define BCME_NOTASSOCIATED		-17
#define BCME_BADSSIDLEN			-18
#define BCME_OUTOFRANGECHAN		-19
#define BCME_BADCHAN			-20
#define BCME_BADADDR			-21
#define BCME_NORESOURCE			-22
#define BCME_UNSUPPORTED		-23
#define BCME_BADLEN			-24
#define BCME_NOTREADY			-25
#define BCME_EPERM			-26
#define BCME_NOMEM			-27
#define BCME_ASSOCIATED			-28
#define BCME_RANGE			-29
#define BCME_NOTFOUND			-30
#define BCME_WME_NOT_ENABLED		-31
#define BCME_TSPEC_NOTFOUND		-32
#define BCME_ACM_NOTSUPPORTED		-33
#define BCME_NOT_WME_ASSOCIATION	-34
#define BCME_SDIO_ERROR			-35
#define BCME_DONGLE_DOWN		-36
#define BCME_VERSION			-37
#define BCME_TXFAIL			-38
#define BCME_RXFAIL			-39
#define BCME_NODEVICE			-40
#define BCME_NMODE_DISABLED		-41
#define BCME_NONRESIDENT		-42
#define BCME_LAST			BCME_NONRESIDENT

#define SD_SysAddr			0x000
#define SD_BlockSize			0x004
#define SD_BlockCount 			0x006
#define SD_Arg0				0x008
#define SD_Arg1 			0x00A
#define SD_TransferMode			0x00C
#define SD_Command 			0x00E
#define SD_Response0			0x010
#define SD_Response1 			0x012
#define SD_Response2			0x014
#define SD_Response3 			0x016
#define SD_Response4			0x018
#define SD_Response5 			0x01A
#define SD_Response6			0x01C
#define SD_Response7 			0x01E
#define SD_BufferDataPort0		0x020
#define SD_BufferDataPort1 		0x022
#define SD_PresentState			0x024
#define SD_HostCntrl			0x028
#define SD_PwrCntrl			0x029
#define SD_BlockGapCntrl 		0x02A
#define SD_WakeupCntrl 			0x02B
#define SD_ClockCntrl			0x02C
#define SD_TimeoutCntrl 		0x02E
#define SD_SoftwareReset		0x02F
#define SD_IntrStatus			0x030
#define SD_ErrorIntrStatus 		0x032
#define SD_IntrStatusEnable		0x034
#define SD_ErrorIntrStatusEnable 	0x036
#define SD_IntrSignalEnable		0x038
#define SD_ErrorIntrSignalEnable 	0x03A
#define SD_CMD12ErrorStatus		0x03C
#define SD_Capabilities			0x040
#define SD_Capabilities3		0x044
#define SD_MaxCurCap			0x048
#define SD_MaxCurCap_Reserved		0x04C
#define SD_ADMA_ErrStatus		0x054
#define SD_ADMA_SysAddr			0x58
#define SD_SlotInterruptStatus		0x0FC
#define SD_HostControllerVersion 	0x0FE
#define	SD_GPIO_Reg			0x100
#define	SD_GPIO_OE			0x104
#define	SD_GPIO_Enable			0x108

#define BLOCK_SIZE_4318 64
#define BLOCK_SIZE_4328 512

#define CMN_IOCTL_OFF 0x180




#define WL_OID_BASE     0xFFE41420


#define OID_WL_GETINSTANCE  (WL_OID_BASE + WLC_GET_INSTANCE)
#define OID_WL_GET_FORCELINK    (WL_OID_BASE + WLC_GET_FORCELINK)
#define OID_WL_SET_FORCELINK    (WL_OID_BASE + WLC_SET_FORCELINK)
#define OID_WL_ENCRYPT_STRENGTH (WL_OID_BASE + WLC_ENCRYPT_STRENGTH)
#define OID_WL_DECRYPT_STATUS   (WL_OID_BASE + WLC_DECRYPT_STATUS)
#define OID_LEGACY_LINK_BEHAVIOR (WL_OID_BASE + WLC_LEGACY_LINK_BEHAVIOR)
#define OID_WL_NDCONFIG_ITEM    (WL_OID_BASE + WLC_NDCONFIG_ITEM)


#define OID_STA_CHANSPEC    (WL_OID_BASE + WLC_GET_CHANSPEC)
#define OID_STA_NBANDS      (WL_OID_BASE + WLC_GET_NBANDS)
#define OID_STA_GET_PHY     (WL_OID_BASE + WLC_GET_OID_PHY)
#define OID_STA_SET_PHY     (WL_OID_BASE + WLC_SET_OID_PHY)
#define OID_STA_ASSOC_TIME  (WL_OID_BASE + WLC_SET_ASSOC_TIME)
#define OID_STA_DESIRED_SSID    (WL_OID_BASE + WLC_GET_DESIRED_SSID)
#define OID_STA_SET_PHY_STATE   (WL_OID_BASE + WLC_SET_PHY_STATE)
#define OID_STA_SCAN_PENDING    (WL_OID_BASE + WLC_GET_SCAN_PENDING)
#define OID_STA_SCANREQ_PENDING (WL_OID_BASE + WLC_GET_SCANREQ_PENDING)
#define OID_STA_GET_ROAM_REASON (WL_OID_BASE + WLC_GET_PREV_ROAM_REASON)
#define OID_STA_SET_ROAM_REASON (WL_OID_BASE + WLC_SET_PREV_ROAM_REASON)
#define OID_STA_GET_PHY_STATE   (WL_OID_BASE + WLC_GET_PHY_STATE)
#define OID_STA_INT_DISASSOC    (WL_OID_BASE + WLC_GET_INT_DISASSOC)
#define OID_STA_SET_NUM_PEERS   (WL_OID_BASE + WLC_SET_NUM_PEERS)
#define OID_STA_GET_NUM_BSS (WL_OID_BASE + WLC_GET_NUM_BSS)

#define DHD_RXBOUND	50	/* Default for max rx frames in one scheduling */
#define DHD_TXBOUND	5	/* Default for max tx frames in one scheduling */

/* Override to force tx queueing all the time */
//extern uint dhd_force_tx_queueing;
/* Default KEEP_ALIVE Period is 55 sec to prevent AP from sending Keep Alive probe frame */
#define KEEP_ALIVE_PERIOD 55000
#define NULL_PKT_STR	"null_pkt"

/* intstatus bits */
#define I_HMB_FC_STATE	I_HMB_SW0	/* To Host Mailbox Flow Control State */
#define I_HMB_FC_CHANGE	I_HMB_SW1	/* To Host Mailbox Flow Control State Changed */
#define I_HMB_FRAME_IND	I_HMB_SW2	/* To Host Mailbox Frame Indication */
#define I_HMB_HOST_INT	I_HMB_SW3	/* To Host Mailbox Miscellaneous Interrupt */
#define I_TOHOSTMAIL    (I_HMB_FC_CHANGE | I_HMB_FRAME_IND | I_HMB_HOST_INT)

/* tohostmailbox bits corresponding to intstatus bits */
#define HMB_FC_ON	(1 << 0)	/* To Host Mailbox Flow Control State */
#define HMB_FC_CHANGE	(1 << 1)	/* To Host Mailbox Flow Control State Changed */
#define HMB_FRAME_IND	(1 << 2)	/* To Host Mailbox Frame Indication */
#define HMB_HOST_INT	(1 << 3)	/* To Host Mailbox Miscellaneous Interrupt */
#define HMB_MASK	0x0000000f	/* To Host Mailbox Mask */

/* tohostmailboxdata */
#define HMB_DATA_NAKHANDLED	0x01	/* we're ready to retransmit NAK'd frame to host */
#define HMB_DATA_DEVREADY	0x02	/* we're ready to to talk to host after enable */
#define HMB_DATA_FC		0x04	    /* per prio flowcontrol update flag to host */
#define HMB_DATA_FWREADY	0x08	/* firmware is ready for protocol activity */
#define HMB_DATA_FWHALT		0x10	/* firmware has halted operation */

#define HMB_DATA_FCDATA_MASK	0xff000000	/* per prio flowcontrol data */
#define HMB_DATA_FCDATA_SHIFT	24		/* per prio flowcontrol data */

#define HMB_DATA_VERSION_MASK	0x00ff0000	/* device protocol version (with devready) */
#define HMB_DATA_VERSION_SHIFT	16		/* device protocol version (with devready) */

#define FRAME_AVAIL_MASK(bus) 	\
	((bus->rxint_mode == SDIO_DEVICE_HMB_RXINT) ? I_HMB_FRAME_IND : I_XMTDATA_AVAIL)

#define PKT_AVAILABLE(bus, intstatus)	((intstatus) & (FRAME_AVAIL_MASK(bus)))


/* Firmware requested operation mode */
#define STA_MASK			0x0001
#define HOSTAPD_MASK			0x0002
#define WFD_MASK			0x0004
#define SOFTAP_FW_MASK			0x0008


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

