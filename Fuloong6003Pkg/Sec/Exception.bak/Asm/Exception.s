# 1 "Exception.S"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "Exception.S"
 .set noreorder
 .set mips3
 .globl _ModuleEntryPoint
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Include/Loongson/Loongson2f/ExcCode.h" 1
# 5 "Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Include/Loongson/Loongson2f/Cp0.h" 1
# 6 "Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Include/Loongson/RegDef.h" 1
# 7 "Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Include/Cs5536.h" 1
# 8 "Exception.S" 2
# 1 "/home/guobamantou/Desktop/edk2/Fuloong6003Pkg/Include/Uart.h" 1
# 9 "Exception.S" 2
# 23 "Exception.S"
.align 9
_ModuleEntryPoint:
 .rdata ;2009: ; .asciz "XTLB refill exception!\r\n" ; .text ; la $4, 2009b ; bal uart_puts ; nop;

dump_reg:
 .rdata ;2009: ; .asciz "\r\n      EPC : 0x" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
 mfc0 $4, $14
 bal uart_put_hex
 nop
 .rdata ;2009: ; .asciz "\r\n Exc-Code : 0x" ; .text ; la $4, 2009b ; bal uart_puts ; nop;
 mfc0 $16, $13
 andi $16, 0x7c
 srl $16, $16, 2
 add $4, $16, $0
 bal uart_put_hex
 nop

 b stop
 nop




uart_putc:
 li $8, ((0Xbfd002f8)|0xffffffffa0000000)

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
 addu $2, $2, $25
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
 addu $24, $4, $25
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
