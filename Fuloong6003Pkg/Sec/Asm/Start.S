	.set	noreorder
	.set 	mips3
	.globl	_ModuleEntryPoint	

#include <Loongson/RegDef.h>
#include <Idsel.h>
#include <Uart.h>

#include <Loongson/Loongson2f/Cp0.h>
#include <Loongson/Loongson2f/NbReg.h>
#include <Loongson/Loongson2f/PciConf.h>
#include <Loongson/Loongson2f/Cache.h>
#include <Loongson/Loongson2f/Ddr2.h>
#include <Loongson/Loongson2f/Io.h>
#include <Cs5536.h>

#define OFFSET t9  /* t9 should not be used for other purpose */

#define	PHY_TO_UNCACHED(p)	((p)|0xffffffffa0000000)
#define	PHY_TO_CACHED(p)	((p)|0xffffffff80000000)

#define PRINTF(str) 

#define REALPRINTF(str) \
	.rdata ;\
2009: ;\
	.asciz str ;\
	.text ;\
	la	a0, 2009b ;\
	bal	uart_puts ;\
	nop

/************************   start here  ************************/
stack = 0xffffffff90000000

_ModuleEntryPoint:
	mtc0	zero, CP0_STATUS 
	mtc0	zero, CP0_CAUSE 
	dli	s0, STATUS_BEV
	mtc0	s0, CP0_STATUS
	dli	sp, stack

	bal	get_offset
	nop
get_offset:	
	la	s0, get_offset
	subu	OFFSET, ra, s0

        /* rom accelerate */
        dli     v0, PHY_TO_UNCACHED(LIO_CFG)
        lw      v1, 0(v0)
        dli     a0, 0xffffe083
        and     v1, v1, a0
        ori     v1, v1, 0x0f0c
        sw      v1, 0(v0)

########### WARNNING : DONT ADD ANY CODE ABOVE ####################

init_nb:
	/* init north bridge(virtual?) */
	PRINTF("INIT North Bridge\r\n");

detect_cs5536:
	BUS0_PCICFG_READ(CS5536_IDSEL, 0, 0x0);
	dli	a0, CS5536ID
	beq	a0, v0, 22f
	nop

/* cs5536 may not ready here in fuloong platform, 
 * I test, about 5ms after the first instruct executed, 
 * pci configure space can be accessed normally. 
 * test in a fuloong 6003 which code is FLF1CCA27000314
 */

	b	detect_cs5536
	nop

22:
	PRINTF("find cs5536\r\n");

	/*enable msr access*/
	BUS0_PCICFG_WRITE(CS5536_IDSEL, 0, MSR_CTRL, MSR_EN);

	/*enable all ports*/
	CS5536_MSR_WRITE(CS5536_PORT_EN, 0xffff, 0x0);
/*
 * because we will operate smb bus shortly, 
 * set base addr manually now, and gpio base addr
 */

#define DIVIL_BASE_ADDR     0xB000
#define SMB_BASE_ADDR       (DIVIL_BASE_ADDR | 0x320)
#define GPIO_BASE_ADDR      (DIVIL_BASE_ADDR | 0x000)

	CS5536_MSR_WRITE(CS5536_SMB_BASE, SMB_BASE_ADDR, 0xf001);
	CS5536_MSR_WRITE(CS5536_GPIO_BASE, GPIO_BASE_ADDR, 0xf001);
	
/* FULOONG use uart2 as default serial, initial now */

	/* set uart2 io addr to 0x02f8, default val is 04000003 */
	CS5536_MSR_WRITE(CS5536_LEGACY_IO, 0x04500003, 0); 
	
	/* enable uart2 */
	CS5536_MSR_WRITE(CS5536_UART2_CONF, 2, 0);

	GPIO_HI_BIT(UART2_RX, GPIO_BASE_ADDR | GPIOL_IN_EN); 
	GPIO_HI_BIT(UART2_RX, GPIO_BASE_ADDR | GPIOL_IN_AUX1_SEL); 

	GPIO_HI_BIT(UART2_TX, GPIO_BASE_ADDR | GPIOL_OUT_EN); 
	GPIO_HI_BIT(UART2_TX, GPIO_BASE_ADDR | GPIOL_OUT_AUX1_SEL); 
	
	bal	init_uart
	nop	

	/* config gpio14 to be smb_clk and gpio15 to be smb_data */
	GPIO_HI_BIT(SMB_CLK_PIN, GPIO_BASE_ADDR | GPIOL_IN_EN);
	GPIO_HI_BIT(SMB_CLK_PIN, GPIO_BASE_ADDR | GPIOL_OUT_EN);
	GPIO_HI_BIT(SMB_CLK_PIN, GPIO_BASE_ADDR | GPIOL_IN_AUX1_SEL);
	GPIO_HI_BIT(SMB_CLK_PIN, GPIO_BASE_ADDR | GPIOL_OUT_AUX1_SEL);

	GPIO_HI_BIT(SMB_DATA_PIN, GPIO_BASE_ADDR | GPIOL_IN_EN);
	GPIO_HI_BIT(SMB_DATA_PIN, GPIO_BASE_ADDR | GPIOL_OUT_EN);
	GPIO_HI_BIT(SMB_DATA_PIN, GPIO_BASE_ADDR | GPIOL_IN_AUX1_SEL);
	GPIO_HI_BIT(SMB_DATA_PIN, GPIO_BASE_ADDR | GPIOL_OUT_AUX1_SEL);


	/* init smbus */
	REALPRINTF("haha Init smbus\r\n");
	bal	init_smb
	nop

        REALPRINTF("Init ddr2\r\n");
        bal     ddr2_cfg
        nop

        REALPRINTF("Init cache\r\n");
        bal     init_cache
        nop

        /*enable cache*/
        mfc0    a0, CP0_CONFIG
        dli     a1, 0xfffffff8  ## dont afraid, use 32bit in deed
        and     a0, a0, a1
        ori     a0, 3
        mtc0    a0, CP0_CONFIG

        REALPRINTF("Init TLB\r\n");
        bal     init_tlb
        nop
#if 0
	/* copy BIOS image to memory */
        REALPRINTF("COPY IMAGE TO MEMORY........\r\n");
        li	s0, 0xbfc00000 # source
	li	s1, 0x80200000 # dest
	li	s2, 512*1024   # size

next_dword:
	ld	a0, 0(s0)
	sd	a0, 0(s1)
	addi	s2, -8	
	bnez	s2, next_dword		
	addiu	s0, 8
	addiu	s1, 8
#endif	

	//REALPRINTF("Memory size is ")
	/* get memory size in MB*/
	li	a0, 0xa1	
	li	a1, 5
	bal	smb_read
	nop

	andi	s0, v0, 0x7 # get low 3bits
	addiu	s0, s0, 1   # s0 is bank num

	li	a0, 0xa1	
	li	a1, 31
	bal	smb_read
	nop
	
	andi	s1, v0, 0x1f # get low 5bits
	sll	s1, s1, 8
	add	s1, s1, v0
	srl	s1, s1, 5    # change to list by order
	mul	s1, s1, s0
	sll	a0, s1, 7
	
	/* output memsize in MB */
	//bal	uart_put_hex
	//nop

	bal	SecStartup
	nop
	## shuold never come back

stop:
	b	stop
	nop


################################ uart ###########################################
/********  use t0, t1, t2 ,t3 ra ********/

init_uart:
	li	t0, PHY_TO_UNCACHED(UART_ADDR)

	/* set fifo mode */
	li	t1, UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR |\
			 UART_FCR_CLEAR_XMIT | UART_FCR_R_TRIG_00
	sb	t1, UART_FCR(t0)

	/* set baud rate */
	li	t2, UART_LCR_DLAB  // enable set band rate
	sb	t2, UART_LCR(t0)
	li	t3, UART_BAUD / (16 * B115200)
	sb	t3, UART_DLL(t0)
	srl	t3, 8
	sb	t3, UART_DLH(t0)

	/* set transmit format */
	li	t1, UART_LCR_WLEN8
	sb	t1, UART_LCR(t0)	

	/* set modem mode */
	li	t2, UART_MCR_RTS | UART_MCR_DTR
	sb	t2, UART_MCR(t0)
	
	/* disable all intrrupt */
	j 	ra
	sb	zero, UART_IER(t0)


/************************************************************/
/********** use t0-t3, a0 ra **********/

uart_putc: 
	li	t0, PHY_TO_UNCACHED(UART_ADDR)	
	
	li	t1, 1000  /* try for 1000 times, this times is not strictly test*/
retry:
	beq	t1, zero,11f 
	nop
	lbu	t2, UART_LSR(t0)
	and	t2, UART_LSR_THRE
	beq	t2, zero, retry
	addi	t1, t1, -1

	/* can transmit now */	
	sb	a0, UART_TX(t0)
11:
	jr	ra
	nop


/************************************************************/
/************ input parameter a0, 32bit used ****************/
/************************************************************/
/******** use a0-a3, v0-v1, ra, t8 **********************/
uart_put_hex:
### store ra first ,because uart_put_hex will call uart_putc
	add	a3, ra, zero  
	add	a1, a0, zero
	la	v0, hexchar
	addu	v0, v0, OFFSET
	lui	v1, 0xf000	
	li	a2, 8

next_hex:
	and	a0, a1, v1
	srl	a0, 28
	addu	t8, a0, v0
	lb	a0, 0(t8)
	bal	uart_putc
	nop
	addi	a2, -1
	bnez	a2, next_hex
	sll	a1, 4

	j	a3
	nop
hexchar:
	.ascii "0123456789ABCDEF"


/************************************************************/
/******************** output a string ***********************/
/************************************************************/
/**** a0, a3, ra, t8, use t0-t3 in uart_putc ******/
uart_puts:
	add	a3, ra, zero
	addu 	t8, a0, OFFSET
	lbu	a0, 0(t8)

next_char:
	beq	a0, zero, 11f 
	nop
	bal	uart_putc
	addiu	t8, 1
	b	next_char
	lbu	a0, 0(t8)
11:
	j	a3
	nop


################################  smb (iic) ####################################

/************* init smbus ***********************/
/*** use t5, a0, v0, v1 ,ra,***/
init_smb:
	/* disable smb and set freq */
	li	a0, (((SMB_FREQ & 0x7f) << 1) | SMB_DISABLE) 
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL2);
	li	a0, ((SMB_FREQ & 0x7f80) >> 7)
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL3);

	li	a0, 0x00
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL1);
	
	/* disable slave mode */
	li	a0, 0x00
	IO_OUTB(SMB_BASE_ADDR | SMB_ADDR);

	/* clear all error status */
	li	a0, 0xff
	IO_OUTB(SMB_BASE_ADDR | SMB_STS);

	/* clear bus busy bit */
	IO_INB(SMB_BASE_ADDR | SMB_CTRL_STS);
	ori	a0, v0, SMB_BUS_BUSY
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL_STS);
	
	/* enable smb now */
	IO_INB(SMB_BASE_ADDR | SMB_CTRL2);
	ori	a0, v0, SMB_ENABLE
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL2);

	j	ra
	nop
	
/*
 * a0: slave addr
 * a1: index
 * v0: return val
 * follow "random read" sequence description in cs5536 manual
 * use a0, a1, v0, t5, t6, t7, use v1 in io_INB, use t0-t3 v0 in smb_wait
 */
smb_read:
	andi	t5, a0, 0xfe
	add	t6, a1, zero
	add	t7, ra, zero		# store return addr

	/* start */
	IO_INB(SMB_BASE_ADDR | SMB_CTRL1);
	ori	a0, v0, SMB_START
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL1);
	bal	smb_wait
	nop
	bnez	v0, error
	nop

	/* send slave addr */
	add	a0, t5, zero		
	IO_OUTB(SMB_BASE_ADDR | SMB_SDA);
	bal	smb_wait
	nop
	bnez	v0, error
	nop

	/* send index */
	add	a0, t6, zero
	IO_OUTB(SMB_BASE_ADDR | SMB_SDA)
	bal	smb_wait
	nop
	bnez	v0, error
	nop
	
	/* restart */
	IO_INB(SMB_BASE_ADDR | SMB_CTRL1);
	ori	a0, v0, SMB_START
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL1);
	bal	smb_wait
	nop
	bnez	v0, error
	nop

	/* ack */
	IO_INB(SMB_BASE_ADDR | SMB_CTRL1);
	ori	a0, v0, SMB_ACK
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL1);

	/* send slave addr */
	ori	a0, t5, 0x01		
	IO_OUTB(SMB_BASE_ADDR | SMB_SDA);
	bal	smb_wait
	nop
	bnez	v0, error
	nop

	/* stop */
	li	a0, SMB_STOP
	IO_OUTB(SMB_BASE_ADDR | SMB_CTRL1);
	bal	smb_wait
	nop
	bnez	v0, error
	nop

	/* read data */
	IO_INB(SMB_BASE_ADDR | SMB_SDA);
	
error:	
	j	t7
	nop	

/* smb wait and error check */
/* use t1, t2, t3, t4, v0, ra */
/* return 0 means ok, 1 means error */

smb_wait:
	add	t4, ra, zero	# save ra first
	li	t1, 1000		# i dont know how many times is needed
1:		
	li	t2, 100
2:
	addiu	t2, t2, -1
	bnez	t2, 2b		# dont use delay slot here
	nop
	lui	t3, 0xbfd0
	ori	t3, (SMB_BASE_ADDR | SMB_STS)
	lbu	v0, 0(t3)
	andi	v0, SMB_SDAST	# have transmit done or can receive now
	bnez	v0, 3f
	nop
	lbu	v0, 0(t3)
	andi	v0, (SMB_BER | SMB_NEGACK)
	bnez	v0, 4f
	nop
	addiu	t1, t1, -1
	bnez	t1, 1b
	nop
3:
	j	t4
	li	v0, 0
	
4:
	j	t4
	li	v0, 1

################################  ddr2 ####################################

/****************************************************/
/********* use s5, s6, s7, a0, a1, v0, v1, ra *********/
/****************************************************/
ddr2_cfg:
	add	s5, ra, zero		# store ra to s5

	/* enable memory cfg */
	dli	a0, PHY_TO_UNCACHED(CHIP_CFG)	
	ld	v0, 0(a0)
	andi	v0, v0, 0x4ff
	sd	v0, 0(a0)

ddr2_reg_write:
	li	a0, DDR2_CTRL_REG_NUM	## counter
	la	v0, DDR2_CTRL00_LO
	add	v0, v0, OFFSET			## v0 is source data addr 
	dli	v1, PHY_TO_UNCACHED(DDR2_CTRL_REG_BASE) # v1 is config reg base addr
	
next_reg:
	ld	a1, 0(v0)
	sd	a1, 0(v1)
	addi	a0, a0, -1
	addiu	v0, v0, 0x08
	bnez	a0, next_reg
	addiu	v1, v1, 0x10 

#### adjust some special ddr2 cfg reg ####

	dli	s6, PHY_TO_UNCACHED(DDR2_CTRL_REG_BASE) # s6 is config reg base addr

	/* get row address */
	li	a0, 0xa0
	li	a1, 3
	bal	smb_read
	nop
	sll	s7, v0, 8
	
	/* get col address */
	li	a0, 0xa0
	li	a1, 4
	bal	smb_read
	nop
	sll	v0, v0, 24
	or	s7, s7, v0

	dli	a0, ((14 << 24) | (15 << 8))
	sub	a0, a0, s7
	lw	v0, 0x50(s6)
	dli	a1, 0xf8fff8ff  ## clear relative bits
	and	v0, a1
	or	a0, v0, a0
	sw	a0, 0x50(s6)

## according to spd standard R1.2, rank num is 1~8 ##
## but loongson2f seem support only 4 rank most ##
	
	/* get rank(not bank) num */
	li	a0, 0xa0
	li	a1, 5
	bal	smb_read
	nop

	andi	a0, v0, 0x7 			## low 3 bits represent rank num -1
	li	a1, 0x2
	sllv	a1, a1, a0
	addi	a1, -1 			## DDR2_CTRL_70[19:16] = 2^(rank) -1 
	sll	a1, a1, 16
	lw	v0, 0x70(s6)
	li	v1, 0xfff0ffff
	and	v0, v0, v1
	or	v0, v0, a1
	sw	v0, 0x70(s6)		## i test sb ,but not work,sw is ok

	/* get bank num */
	li	a0, 0xa0
	li	a1, 0x11 
	bal	smb_read
	nop
	li	v1, 0x8
	bne	v1, v0,1f			## check have 8 banks or not
	nop
	ld	a0, 0x10(s6)
	dli	v0, (1 << 32) 
	or	a0, v0
	sd	a0, 0x10(s6)
1:
	/* start ddr2 */
	ld	a0, 0x30(s6)
	dli	v0, (1 << 40)
	or	a0, v0
	sd	a0, 0x30(s6)

not_locked:
	ld	v0, 0x10(s6)
	andi	v0, 1
	beq	v0, zero, not_locked	## memory can accesss until dll locked
	nop

	/* after complete config, disable ddr2 config */
	dli	a0, PHY_TO_UNCACHED(CHIP_CFG)
	ld	v0, 0x0(a0)
	ori	v0, 1
	j	s5
	sd	v0, 0x0(a0)


############ NOTICE: LITTLE-END #################

	.align 3	## will use ld instruction

DDR2_CTRL00_LO: .word 0x00000101
DDR2_CTRL00_HI: .word 0x01000100 
DDR2_CTRL01_LO: .word 0x00010000
DDR2_CTRL01_HI: .word 0x00000000
DDR2_CTRL02_LO: .word 0x00000000
DDR2_CTRL02_HI: .word 0x01000101
DDR2_CTRL03_LO: .word 0x01000000
DDR2_CTRL03_HI: .word 0x01010000
DDR2_CTRL04_LO: .word 0x01010101
DDR2_CTRL04_HI: .word 0x01000202
DDR2_CTRL05_LO: .word 0x04050202 
DDR2_CTRL05_HI: .word 0x00000000
DDR2_CTRL06_LO: .word 0x03050203 
DDR2_CTRL06_HI: .word 0x0a040306 
DDR2_CTRL07_LO: .word 0x00030a0b 
DDR2_CTRL07_HI: .word 0x00000400 
DDR2_CTRL08_LO: .word 0x00000102
DDR2_CTRL08_HI: .word 0x00000102
DDR2_CTRL09_LO: .word 0x00000000
DDR2_CTRL09_HI: .word 0x0000060c 
DDR2_CTRL10_LO: .word 0x3f1f0200 
DDR2_CTRL10_HI: .word 0x2323233f 
DDR2_CTRL11_LO: .word 0x23232323 
DDR2_CTRL11_HI: .word 0x5f7f2323 
DDR2_CTRL12_LO: .word 0x15000000 
DDR2_CTRL12_HI: .word 0x002a3c06 
DDR2_CTRL13_LO: .word 0x002a002a
DDR2_CTRL13_HI: .word 0x002a002a
DDR2_CTRL14_LO: .word 0x002a002a
DDR2_CTRL14_HI: .word 0x002a002a
DDR2_CTRL15_LO: .word 0x00000004
DDR2_CTRL15_HI: .word 0x00b40020
DDR2_CTRL16_LO: .word 0x00000087
DDR2_CTRL16_HI: .word 0x000007ff 
DDR2_CTRL17_LO: .word 0x0016101f
DDR2_CTRL17_HI: .word 0x00000000
DDR2_CTRL18_LO: .word 0x00000000
DDR2_CTRL18_HI: .word 0x001c0000
DDR2_CTRL19_LO: .word 0x00c8006b
DDR2_CTRL19_HI: .word 0x28e10002 
DDR2_CTRL20_LO: .word 0x00c8002f 
DDR2_CTRL20_HI: .word 0x00000000
DDR2_CTRL21_LO: .word 0x00030d40 
DDR2_CTRL21_HI: .word 0x00000000
DDR2_CTRL22_LO: .word 0x00000000
DDR2_CTRL22_HI: .word 0x00000000
DDR2_CTRL23_LO: .word 0x00000000
DDR2_CTRL23_HI: .word 0x00000000
DDR2_CTRL24_LO: .word 0x00000000
DDR2_CTRL24_HI: .word 0x00000000
DDR2_CTRL25_LO: .word 0x00000000
DDR2_CTRL25_HI: .word 0x00000000
DDR2_CTRL26_LO: .word 0x00000000
DDR2_CTRL26_HI: .word 0x00000000
DDR2_CTRL27_LO: .word 0x00000000
DDR2_CTRL27_HI: .word 0x00000000
DDR2_CTRL28_LO: .word 0x00000000
DDR2_CTRL28_HI: .word 0x00000000

################################  cache ####################################

/************************************************/
/**** use a0, a1, t0, t1, t2, t5, v0, v1, ra ****/
/**** because no means to get the number of ways, use 4 ****/
/**** NOTICE: loongson's cache instruction is not standard ****/

init_cache:
	add	t5, ra, zero
	mfc0	a0, CP0_CONFIG

init_icache:
	andi	t0, a0, ICACHE_SIZE
	srl	t0, ICACHE_SIZE_START_BIT
	andi	t1, a0, ICACHE_LINE_SIZE
	srl	t1, ICACHE_LINE_SIZE_START_BIT
	li	t2, (2 << 6)	## index num = 2^(6 + t0 -t1)
	sub	t0, t0, t1
	sllv	t2, t2, t0	## t2 is index num now

	li	t0, 16
	sllv	t1, t0, t1	## t1 is icache line size 
	lui	t0, 0x8000		## t0 use as 'VA'
1:
	cache	I_INDEX_INVALIDATE, 0(t0)	# Loongson2f user manual says cache invalide instruction affect all 4 ways
#	cache	I_INDEX_INVALIDATE, 1(t0)	
#	cache	I_INDEX_INVALIDATE, 2(t0)	
#	cache	I_INDEX_INVALIDATE, 3(t0)	
	addiu	t2, -1
	bnez	t2, 1b
	add	t0, t0, t1		## next index

init_dcache:
	andi	t0, a0, DCACHE_SIZE
	srl	t0, DCACHE_SIZE_START_BIT
	andi	t1, a0, DCACHE_LINE_SIZE
	srl	t1, DCACHE_SIZE_START_BIT
	li	t2, (2 << 6)	## index num = 2^(6 + t0 -t1)
	sub	t0, t0, t1
	sllv	t2, t2, t0	## t2 is index num now
	li	t0, 16
	sllv	t1, t0, t1	## t1 is icache line size 

	lui	t0, 0x8000		## t0 use as 'VA'
	mtc0	zero, CP0_TAGLO		## set state bits to 00(invalid)
1:
	cache	D_INDEX_STORE_TAG, 0(t0)	
	cache	D_INDEX_STORE_TAG, 1(t0)	
	cache	D_INDEX_STORE_TAG, 2(t0)	
	cache	D_INDEX_STORE_TAG, 3(t0)	
	addiu	t2, -1
	bnez	t2, 1b
	add	t0, t0, t1		## next index

/******/
init_l2cache:
	li	t2, (1 << 12)	## index num = 2^12
	li	t1, 32

	lui	t0, 0x8000		## t0 use as 'VA'
	mtc0	zero, CP0_TAGLO		## no other purpose, set state bits
1:
	cache	S_INDEX_STORE_TAG, 0(t0)	
	cache	S_INDEX_STORE_TAG, 1(t0)	
	cache	S_INDEX_STORE_TAG, 2(t0)	
	cache	S_INDEX_STORE_TAG, 3(t0)	
	addiu	t2, -1
	bnez	t2, 1b
	add	t0, t0, t1		## next index

	j	t5
	nop

endof_init_cache:

################################ TLB  ####################################
#define TLB_TABLE_NUM 64

init_tlb:
	li	a0, TLB_TABLE_NUM
	li	a1, 0
	li	v0, PAGE16K_MASK

	mtc0	zero, CP0_ENTRYHI
	mtc0	zero, CP0_ENTRYLO0
	mtc0	zero, CP0_ENTRYLO1
	mtc0	v0, CP0_PAGEMASK
next_index:
	mtc0	a1, CP0_INDEX
	addiu	a1, a1, 1
	bne	a1, a0, next_index
	tlbwi
	j	ra	
	nop

.set reorder

