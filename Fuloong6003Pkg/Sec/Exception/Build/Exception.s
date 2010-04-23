# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S"
### dont edit this file
 .set noreorder
 .set mips3
 .globl _ModuleEntryPoint
.align 7
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/../../Include/Loongson/Loongson2f/ExcCode.h" 1
# 7 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/../../Include/Loongson/Loongson2f/Cp0.h" 1
# 8 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/../../Include/Loongson/RegDef.h" 1
# 9 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/../../Include/Cs5536.h" 1
# 10 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/../../Include/Uart.h" 1
# 11 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S" 2




### every string len is 25 bytes ,dont change ###


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
# 64 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Sec/Exception/Asm/Exception.S"
.align 7
_ModuleEntryPoint:
 .rdata ;2009: ; .asciz "XTLB refill exception!\r\n" ; .text ; la $4, 2009b ; bal uart_puts ; nop;

dump_reg:
 .rdata ;2009: ; .asciz "\r\n     EPC : 0x" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
 mfc0 $4, $14
 bal uart_put_hex
 nop
 .rdata ;2009: ; .asciz "\r\nExc-Code : 0x" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
 mfc0 $16, $13
 andi $16, 0x7c
 srl $16, $16, 2
 add $4, $16, $0
 bal uart_put_hex
 nop

 .rdata ;2009: ; .asciz "\r\n" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
detail:
        sltiu $4, $16, (15 + 1)
        beq $4, $0, 1f # >13
        nop
        la $4, exc_int
        li $17, 22
        mul $17, $16, $17
        nop
        add $4, $17
        bal uart_puts
        nop
        b stop
        nop
1:
        li $4, 23
        bne $4, $16, 1f
        nop
        la $4, exc_watch
        bal uart_puts
        nop
        b stop
        nop
1:
 b stop
 nop




uart_putc:
 li $8, ((0x1fd002f8)|0xffffffffa0000000)

 li $9, 1000
retry:
 beq $9, $0,11f
 nop
 lbu $10, 5($8)
 and $10, 0x20
 beq $10, $0, retry
 addi $9, $9, -1


 sb $4, 0($8)
11:
 jr $31
 nop






uart_put_hex:
### store $31 first ,because uart_put_hex will call uart_putc
 add $7, $31, $0
 add $5, $4, $0
 la $2, hexchar
 addu $2, $2, $0
 lui $3, 0xf000
 li $6, 8

next_hex:
 and $4, $5, $3
 srl $4, 28
 addu $24, $4, $2
 lb $4, 0($24)
 bal uart_putc
 nop
 addi $6, -1
 bnez $6, next_hex
 sll $5, 4

 j $7
 nop
hexchar:
 .ascii "0123456789ABCDEF"






uart_puts:
 add $7, $31, $0
 addu $24, $4, $0
 lbu $4, 0($24)

next_char:
 beq $4, $0, 11f
 nop
 bal uart_putc
 addiu $24, 1
 b next_char
 lbu $4, 0($24)
11:
 j $7
 nop


.align 7
other_exc:
 .rdata ;2009: ; .asciz "other exception !\r\n" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
 bal dump_reg
 nop
stop:
 b stop
 nop

 .set reorder
