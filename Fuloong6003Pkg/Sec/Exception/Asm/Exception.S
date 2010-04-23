### dont edit this file
	.set noreorder
	.set mips3
	.globl _ModuleEntryPoint
.align 7
#include <Loongson/Loongson2f/ExcCode.h>
#include <Loongson/Loongson2f/Cp0.h>
#include <Loongson/RegDef.h>
#include <Cs5536.h>
#include <Uart.h>

#define OFFSET zero
#define	PHY_TO_UNCACHED(p)	((p)|0xffffffffa0000000)

### every string len is 25 bytes ,dont change ###
#define EXC_STR_LEN		22

exc_int:
.asciz "An interrupt occur   "
exc_mod:
.asciz "Modify TLB exception "
exc_tlbl:
.asciz "TLB load exception   "
exc_tlbs:
.asciz "TLB store exception  "
exc_adel:
.asciz "Addr error when load "
exc_ades:
.asciz "Addr error when store"
exc_ibe:
.asciz "Bus err:read instruct"
exc_dbe:
.asciz "Bus err:read/wr data "	
exc_sys:
.asciz "Syscall exception    "
exc_bp:
.asciz "Break point          "
exc_ri:
.asciz "Reserved instruction "
exc_cpu:
.asciz "Cant use co-processor"
exc_ov:
.asciz "Arithmetic overflow  "
exc_tr:
.asciz "Trap exception       "
exc_reserved:
.asciz "reserved exception   "
exc_fpe:
.asciz "Float-point exception"
exc_watch:
.asciz "Watch exception"


#define REALPRINTF(str) \
        .rdata ;\
2009: ;\
        .asciz str ;\
        .text ;\
        la      a0, 2009b ;\
        bal     uart_puts ;\
        nop

/* in addr 0xbfc00200*/
.align 7
_ModuleEntryPoint:
	REALPRINTF("XTLB refill exception!\r\n");

dump_reg:	
	REALPRINTF("\r\n     EPC : 0x");
	mfc0	a0, CP0_EPC
	bal	uart_put_hex
	nop
	REALPRINTF("\r\nExc-Code : 0x");
	mfc0	s0, CP0_CAUSE
	andi	s0, EXC_CODE 
	srl	s0, s0, 2	
	add	a0, s0, zero
	bal	uart_put_hex
	nop

	REALPRINTF("\r\n");
detail:
        sltiu   a0, s0, (EXC_FPE + 1)
        beq     a0, zero, 1f    # >EXC_TR
        nop
        la      a0, exc_int
        li      s1, EXC_STR_LEN
        mul     s1, s0, s1
        nop
        add     a0, s1
        bal     uart_puts
        nop
        b       stop
        nop
1:
        li      a0, EXC_WATCH
        bne     a0, s0, 1f
        nop
        la      a0, exc_watch
        bal     uart_puts
        nop
        b       stop
        nop
1:
	b	stop
	nop

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
/* locate in addr 0xbfc00380*/

.align	7
other_exc:
	REALPRINTF("other exception !\r\n");
	bal	dump_reg
	nop
stop:
	b	stop
	nop   

	.set reorder
