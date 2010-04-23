/*
 * cs5536 config header space is not standard
 */
#define CS5536ID	0x208f1022
#define MSR_CTRL	0xF0
#define MSR_ADDR	0xF4
#define MSR_DATA0	0xF8
#define MSR_DATA1	0xFC

/*
 * register bit definition
 */

/* MSR_CTRL*/
#define MSR_EN		(1 << 0)

/*
 * amd cpu addr convert to amd cs5536 addr use spicial formula:
 * cs5536_addr = ((amdcpu_addr[22:13] << 9) || amdcpu_addr[12:0]) 
 * for example, amdcpu_addr 0x51200010 will be translate to 0x40000010
 * loongson2f cpu will not hander rdmsr especially, so submit sb addr directly
 */
#define CS5536_SB_MSR_BASE		(0x00000000)
#define CS5536_GLIU_MSR_BASE	(0x10000000)
#define CS5536_USB_MSR_BASE		(0x40000000)
#define CS5536_IDE_MSR_BASE		(0x60000000)
#define CS5536_DIVIL_MSR_BASE	(0x80000000)
#define CS5536_ACC_MSR_BASE		(0xa0000000)
#define CS5536_GLCP_MSR_BASE	(0xe0000000)

#define CS5536_GLOBLE_CTRL		(CS5536_SB_MSR_BASE | 0X10)

#define CS5536_PORT_EN			(CS5536_GLIU_MSR_BASE | 0x81)	

#define CS5536_SMB_BASE			(CS5536_DIVIL_MSR_BASE | 0x0b)
#define CS5536_GPIO_BASE		(CS5536_DIVIL_MSR_BASE | 0x0c)
#define CS5536_MFGPT_BASE		(CS5536_DIVIL_MSR_BASE | 0x0d)
#define CS5536_LEGACY_IO		(CS5536_DIVIL_MSR_BASE | 0x14)
#define CS5536_UART2_CONF		(CS5536_DIVIL_MSR_BASE | 0x3e)

/*
 * UART2 definition
 */

#define UART2_TX 4
#define UART2_RX 3

#define UART_BAUD 1843200
#define UART_ADDR 0x1fd002f8


/*
 * SMB definition
 */
/* SMB_CLK is gpio14 , SMB_DATA is gpio15*/
#define SMB_CLK_PIN		14    
#define SMB_DATA_PIN	15

#define SMB_SDA		0x00
#define SMB_STS		0x01
#define SMB_CTRL_STS	0x02
#define SMB_CTRL1	0x03
#define SMB_ADDR	0x04
#define SMB_CTRL2	0x05
#define SMB_CTRL3	0x06		

/* SMB_CTRL1 */
#define SMB_STASTRE	(1 << 7)
#define SMB_NMINTE	(1 << 6)
#define SMB_ACK		(1 << 4)
#define SMB_STOP	(1 << 1)
#define SMB_START	(1 << 0)

/* SMB_CTRL2 AND SMB_CTRL3 */
#define SMB_FREQ	0x3c
#define SMB_DISABLE	0x00
#define SMB_ENABLE	0x01

/* SMB_STS */
#define SMB_SLVSTP	(1 << 7)
#define SMB_SDAST	(1 << 6)
#define SMB_BER		(1 << 5)
#define SMB_NEGACK	(1 << 4)
#define SMB_STASTR	(1 << 3)

/* SMB_CTRL_STS */
#define SMB_BUS_BUSY	(1 << 1)

/*
 * GPIO definition
 */

#define GPIOL_OUT_VAL		0x00
#define GPIOL_OUT_EN		0x04
#define GPIOL_OUT_AUX1_SEL	0x10
#define GPIOL_OUT_AUX2_SEL	0x14
#define GPIOL_IN_EN			0x20
#define GPIOL_IN_AUX1_SEL	0x34

/*
 * set two bits relative simultaneous is effect gpio bit set
 * see detail in section "atomic bit programming model" in cs5536 manual 
 */
#define GPIO_HI_BIT(bit, reg)\
	dli	v0, 0xffffffffbfd00000; \
	ori	v0, reg; \
	lw	a0, 0(v0); \
	li	a1, (1 << bit); \
	or	a0, a1; \
	sll a1, 16; \
	not a1; \
	and a0, a1; \
	sw	a0, 0(v0);

#define GPIO_LO_BIT(bit, reg)\
	dli	v0, 0xffffffffbfd00000; \
	ori	v0, reg; \
	lw	a0, 0(v0); \
	sll	a1, (1 << (bit + 16)); \
	or	a0, a1; \
	li	a1, ~(1 << bit); \
	and a0, a1; \
	sw	a0, 0(v0);

#define CS5536_MSR_WRITE(reg, lo, hi) \
    BUS0_PCICFG_WRITE(CS5536_IDSEL, 0, MSR_ADDR, reg); \
    BUS0_PCICFG_WRITE(CS5536_IDSEL, 0, MSR_DATA0, lo); \
    BUS0_PCICFG_WRITE(CS5536_IDSEL, 0, MSR_DATA1, hi);


