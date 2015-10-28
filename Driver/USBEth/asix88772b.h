
#ifndef	ASIX88772B_H_
#define	ASIX88772B_H_

/*		The part of asix.c		*/
/*--------------------------*/

/* ASIX AX8817X based USB 2.0 Ethernet Devices */
#define AX_MONITOR_MODE			0x01
#define AX_MONITOR_LINK			0x02
#define AX_MONITOR_MAGIC		0x04
#define AX_MONITOR_HSFS			0x10

/* AX88172 Medium Status Register values */
#define AX_MEDIUM_FULL_DUPLEX		0x02
#define AX_MEDIUM_TX_ABORT_ALLOW	0x04
#define AX_MEDIUM_FLOW_CONTROL_EN	0x10
#define AX_MCAST_FILTER_SIZE		8
#define AX_MAX_MCAST			64

#define AX_EEPROM_LEN			0x40

#define AX_SWRESET_CLEAR		0x00
#define AX_SWRESET_RR			0x01
#define AX_SWRESET_RT			0x02
#define AX_SWRESET_PRTE			0x04
#define AX_SWRESET_PRL			0x08
#define AX_SWRESET_BZ			0x10
#define AX_SWRESET_IPRL			0x20
#define AX_SWRESET_IPPD			0x40
#define AX_SWRESET_IPOSC		0x0080
#define AX_SWRESET_IPPSL_0		0x0100
#define AX_SWRESET_IPPSL_1		0x0200
#define AX_SWRESET_IPCOPS		0x0400
#define AX_SWRESET_IPCOPSC		0x0800
#define AX_SWRESET_AUTODETACH		0x1000
#define AX_SWRESET_WOLLP		0x8000

#define AX88772_IPG0_DEFAULT		0x15
#define AX88772_IPG1_DEFAULT		0x0c
#define AX88772_IPG2_DEFAULT		0x0E

#define AX88772A_IPG0_DEFAULT		0x15
#define AX88772A_IPG1_DEFAULT		0x16
#define AX88772A_IPG2_DEFAULT		0x1A

#define AX88772_MEDIUM_FULL_DUPLEX	0x0002
#define AX88772_MEDIUM_RESERVED		0x0004
#define AX88772_MEDIUM_RX_FC_ENABLE	0x0010
#define AX88772_MEDIUM_TX_FC_ENABLE	0x0020
#define AX88772_MEDIUM_PAUSE_FORMAT	0x0080
#define AX88772_MEDIUM_RX_ENABLE	0x0100
#define AX88772_MEDIUM_100MB		0x0200
#define AX88772_MEDIUM_DEFAULT	\
	(AX88772_MEDIUM_FULL_DUPLEX | AX88772_MEDIUM_RX_FC_ENABLE | \
	 AX88772_MEDIUM_TX_FC_ENABLE | AX88772_MEDIUM_100MB | \
	 AX88772_MEDIUM_RESERVED | AX88772_MEDIUM_RX_ENABLE)

#define AX_CMD_SET_SW_MII   		0x06
#define AX_CMD_READ_MII_REG 		0x07
#define AX_CMD_WRITE_MII_REG		0x08
#define AX_CMD_SET_HW_MII   		0x0a
#define AX_CMD_READ_EEPROM  		0x0b
#define AX_CMD_WRITE_EEPROM 		0x0c
#define AX_CMD_WRITE_EEPROM_EN		0x0d
#define AX_CMD_WRITE_EEPROM_DIS		0x0e
#define AX_CMD_WRITE_RX_CTL	    	0x10
#define AX_CMD_WRITE_IPG			0x12
#define AX_CMD_READ_NODE_ID			0x13
#define AX_CMD_WRITE_NODE_ID    	0x14
#define AX_CMD_WRITE_MULTI_FILTER	0x16
#define AX_CMD_READ_PHY_ID  		0x19
#define AX_CMD_READ_MEDIUM_MODE		0x1a
#define AX_CMD_WRITE_MEDIUM_MODE	0x1b
#define AX_CMD_READ_MONITOR_MODE	0x1c
#define AX_CMD_WRITE_MONITOR_MODE	0x1d
#define AX_CMD_WRITE_GPIOS		0x1f
#define AX_CMD_SW_RESET			0x20
#define AX_CMD_SW_PHY_STATUS		0x21
#define AX_CMD_SW_PHY_SELECT		0x22
	#define AX_PHYSEL_PSEL		(1 << 0)
	#define AX_PHYSEL_ASEL		(1 << 1)
	#define AX_PHYSEL_SSMII		(0 << 2)
	#define AX_PHYSEL_SSRMII	(1 << 2)
	#define AX_PHYSEL_SSRRMII	(3 << 2)
	#define AX_PHYSEL_SSEN		(1 << 4)
#define AX_CMD_READ_WKFARY		0x23
#define AX_CMD_WRITE_WKFARY		0x24
#define AX_CMD_READ_RXCOE_CTL		0x2b
#define AX_CMD_WRITE_RXCOE_CTL		0x2c
#define AX_CMD_READ_TXCOE_CTL		0x2d
#define AX_CMD_WRITE_TXCOE_CTL		0x2e

#define REG_LENGTH			2
#define PHY_ID_MASK			0x1f

#define AX_RXCOE_IPCE			0x0001
#define AX_RXCOE_IPVE			0x0002
#define AX_RXCOE_V6VE			0x0004
#define AX_RXCOE_TCPE			0x0008
#define AX_RXCOE_UDPE			0x0010
#define AX_RXCOE_ICMP			0x0020
#define AX_RXCOE_IGMP			0x0040
#define AX_RXCOE_ICV6			0x0080
#define AX_RXCOE_TCPV6			0x0100
#define AX_RXCOE_UDPV6			0x0200
#define AX_RXCOE_ICMV6			0x0400
#define AX_RXCOE_IGMV6			0x0800
#define AX_RXCOE_ICV6V6			0x1000
#define AX_RXCOE_FOPC			0x8000

#define AX_RXCOE_64TE			0x0100
#define AX_RXCOE_PPPOE			0x0200
#define AX_RXCOE_RPCE			0x8000

#define AX_TXCOE_IP			0x0001
#define AX_TXCOE_TCP			0x0002
#define AX_TXCOE_UDP			0x0004
#define AX_TXCOE_ICMP			0x0008
#define AX_TXCOE_IGMP			0x0010
#define AX_TXCOE_ICV6			0x0020

#define AX_TXCOE_TCPV6			0x0100
#define AX_TXCOE_UDPV6			0x0200
#define AX_TXCOE_ICMV6			0x0400
#define AX_TXCOE_IGMV6			0x0800
#define AX_TXCOE_ICV6V6			0x1000


#define AX_TXCOE_64TE			0x0001
#define AX_TXCOE_PPPE			0x0002

#define AX_RX_CTL_RH1M			0x0100		/* Enable RX-Header mode 0 */
#define AX_RX_CTL_RH2M			0x0200		/* Enable IP header in receive buffer aligned on 32-bit aligment */
#define AX_RX_CTL_RH3M			0x0400		/* checksum value in rx header 3 */
#define AX_RX_HEADER_DEFAULT		(AX_RX_CTL_RH1M | AX_RX_CTL_RH2M)

#define AX_RX_CTL_MFB			0x0300		/* Maximum Frame size 16384bytes */
#define AX_RX_CTL_START			0x0080		/* Ethernet MAC start */
#define AX_RX_CTL_AP			0x0020		/* Accept physcial address from Multicast array */
#define AX_RX_CTL_AM			0x0010
#define AX_RX_CTL_AB			0x0008		/* Accetp Brocadcast frames*/
#define AX_RX_CTL_SEP			0x0004		/* Save error packets */
#define AX_RX_CTL_AMALL			0x0002		/* Accetp all multicast frames */
#define AX_RX_CTL_PRO			0x0001		/* Promiscuous Mode */
#define AX_RX_CTL_STOP			0x0000		/* Stop MAC */

#define AX_MONITOR_MODE			0x01
#define AX_MONITOR_LINK			0x02
#define AX_MONITOR_MAGIC		0x04
#define AX_MONITOR_HSFS			0x10

#define AX_MCAST_FILTER_SIZE		8
#define AX_MAX_MCAST			64
#define AX_INTERRUPT_BUFSIZE		8

#define AX_EEPROM_LEN			0x40
#define AX_EEPROM_MAGIC			0xdeadbeef
#define EEPROMMASK			0x7f

/* GPIO REGISTER */
#define AXGPIOS_GPO0EN			0X01 /* 1 << 0 */
#define AXGPIOS_GPO0			0X02 /* 1 << 1 */
#define AXGPIOS_GPO1EN			0X04 /*	1 << 2 */
#define AXGPIOS_GPO1			0X08 /* 1 << 3 */
#define AXGPIOS_GPO2EN			0X10 /* 1 << 4 */
#define AXGPIOS_GPO2			0X20 /* 1 << 5 */
#define AXGPIOS_RSE			0X80 /* 1 << 7 */

/* TX-header format */
#define AX_TX_HDR_CPHI			0x4000
#define AX_TX_HDR_DICF			0x8000

#ifndef ADVERTISE_PAUSE_CAP
#define ADVERTISE_PAUSE_CAP			0x0400
#endif

/* medium mode register */
#define MEDIUM_GIGA_MODE			0x0001
#define MEDIUM_FULL_DUPLEX_MODE			0x0002
#define MEDIUM_TX_ABORT_MODE			0x0004
#define MEDIUM_ENABLE_125MHZ			0x0008
#define MEDIUM_ENABLE_RX_FLOWCTRL		0x0010
#define MEDIUM_ENABLE_TX_FLOWCTRL		0x0020
#define MEDIUM_ENABLE_JUMBO_FRAME		0x0040
#define MEDIUM_CHECK_PAUSE_FRAME_MODE		0x0080
#define MEDIUM_ENABLE_RECEIVE			0x0100
#define MEDIUM_MII_100M_MODE			0x0200
#define MEDIUM_ENABLE_JAM_PATTERN		0x0400
#define MEDIUM_ENABLE_STOP_BACKPRESSURE		0x0800
#define MEDIUM_ENABLE_SUPPER_MAC_SUPPORT	0x1000

#define MII_BMCR            0x00	/* Basic mode control register */
#define MII_BMSR            0x01	/* Basic mode status register  */
#define MII_PHYSID1         0x02	/* PHYS ID 1                   */
#define MII_PHYSID2         0x03	/* PHYS ID 2                   */
#define MII_ADVERTISE       0x04	/* Advertisement control reg   */
#define MII_LPA             0x05	/* Link partner ability reg    */
#define MII_EXPANSION       0x06	/* Expansion register          */
#define MII_DCOUNTER        0x12	/* Disconnect counter          */
#define MII_FCSCOUNTER      0x13	/* False carrier counter       */
#define MII_NWAYTEST        0x14	/* N-way auto-neg test reg     */
#define MII_RERRCOUNTER     0x15	/* Receive error counter       */
#define MII_SREVISION       0x16	/* Silicon revision            */
#define MII_RESV1           0x17	/* Reserved...                 */
#define MII_LBRERROR        0x18	/* Lpback, rx, bypass error    */
#define MII_PHYADDR         0x19	/* PHY address                 */
#define MII_RESV2           0x1a	/* Reserved...                 */
#define MII_TPISTATUS       0x1b	/* TPI status for 10mbps       */
#define MII_NCONFIG         0x1c	/* Network interface config    */

/* Basic mode control register. */
#define BMCR_RESV               0x003f	/* Unused...                   */
#define BMCR_SPEED1000			0x0040	/* MSB of Speed (1000)         */
#define BMCR_CTST               0x0080	/* Collision test              */
#define BMCR_FULLDPLX           0x0100	/* Full duplex                 */
#define BMCR_ANRESTART          0x0200	/* Auto negotiation restart    */
#define BMCR_ISOLATE            0x0400	/* Disconnect DP83840 from MII */
#define BMCR_PDOWN              0x0800	/* Powerdown the DP83840       */
#define BMCR_ANENABLE           0x1000	/* Enable auto negotiation     */
#define BMCR_SPEED100           0x2000	/* Select 100Mbps              */
#define BMCR_LOOPBACK           0x4000	/* TXD loopback bits           */
#define BMCR_RESET              0x8000	/* Reset the DP83840           */

/* Basic mode status register. */
#define BMSR_ERCAP              0x0001	/* Ext-reg capability          */
#define BMSR_JCD                0x0002	/* Jabber detected             */
#define BMSR_LSTATUS            0x0004	/* Link status                 */
#define BMSR_ANEGCAPABLE        0x0008	/* Able to do auto-negotiation */
#define BMSR_RFAULT             0x0010	/* Remote fault detected       */
#define BMSR_ANEGCOMPLETE       0x0020	/* Auto-negotiation complete   */
#define BMSR_RESV               0x07c0	/* Unused...                   */
#define BMSR_10HALF             0x0800	/* Can do 10mbps, half-duplex  */
#define BMSR_10FULL             0x1000	/* Can do 10mbps, full-duplex  */
#define BMSR_100HALF            0x2000	/* Can do 100mbps, half-duplex */
#define BMSR_100FULL            0x4000	/* Can do 100mbps, full-duplex */
#define BMSR_100BASE4           0x8000	/* Can do 100mbps, 4k packets  */

/* Advertisement control register. */
#define ADVERTISE_SLCT          0x001f	/* Selector bits               */
#define ADVERTISE_CSMA          0x0001	/* Only selector supported     */
#define ADVERTISE_10HALF        0x0020	/* Try for 10mbps half-duplex  */
#define ADVERTISE_10FULL        0x0040	/* Try for 10mbps full-duplex  */
#define ADVERTISE_100HALF       0x0080	/* Try for 100mbps half-duplex */
#define ADVERTISE_100FULL       0x0100	/* Try for 100mbps full-duplex */
#define ADVERTISE_100BASE4      0x0200	/* Try for 100mbps 4k packets  */
#define ADVERTISE_PAUSE_CAP		0x0400
#define ADVERTISE_RESV          0x1c00	/* Unused...                   */
#define ADVERTISE_RFAULT        0x2000	/* Say we can detect faults    */
#define ADVERTISE_LPACK         0x4000	/* Ack link partners response  */
#define ADVERTISE_NPAGE         0x8000	/* Next page bit               */

#define ADVERTISE_FULL (ADVERTISE_100FULL | ADVERTISE_10FULL | \
			ADVERTISE_CSMA)
#define ADVERTISE_ALL (ADVERTISE_10HALF | ADVERTISE_10FULL | \
                       ADVERTISE_100HALF | ADVERTISE_100FULL | \
                       ADVERTISE_PAUSE_CAP | ADVERTISE_CSMA)

/* Link partner ability register. */
#define LPA_SLCT                0x001f	/* Same as advertise selector  */
#define LPA_10HALF              0x0020	/* Can do 10mbps half-duplex   */
#define LPA_10FULL              0x0040	/* Can do 10mbps full-duplex   */
#define LPA_100HALF             0x0080	/* Can do 100mbps half-duplex  */
#define LPA_100FULL             0x0100	/* Can do 100mbps full-duplex  */
#define LPA_100BASE4            0x0200	/* Can do 100mbps 4k packets   */
#define LPA_RESV                0x1c00	/* Unused...                   */
#define LPA_RFAULT              0x2000	/* Link partner faulted        */
#define LPA_LPACK               0x4000	/* Link partner acked us       */
#define LPA_NPAGE               0x8000	/* Next page bit               */

#define LPA_DUPLEX		(LPA_10FULL | LPA_100FULL)
#define LPA_100			(LPA_100FULL | LPA_100HALF | LPA_100BASE4)

/* Expansion register for auto-negotiation. */
#define EXPANSION_NWAY          0x0001	/* Can do N-way auto-nego      */
#define EXPANSION_LCWP          0x0002	/* Got new RX page code word   */
#define EXPANSION_ENABLENPAGE   0x0004	/* This enables npage words    */
#define EXPANSION_NPCAPABLE     0x0008	/* Link partner supports npage */
#define EXPANSION_MFAULTS       0x0010	/* Multiple faults detected    */
#define EXPANSION_RESV          0xffe0	/* Unused...                   */




#endif
/*		End of the part of asic.c		*/
/*--------------------------------*/



