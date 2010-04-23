#include <Types.h>

extern void uart_puts(const u8 *);
void SecStartup(unsigned int memsize)
{
	int i;

	i = i + 1;
	uart_puts("\nni hao\n");
	uart_puts("1, ni hao\n");
	uart_puts("11, ni hao\n");
	uart_puts("22, ni hao\n");
	uart_puts("1111, ni hao\n");
	uart_puts("2222, ni hao\n");
	while(1);
	asm (".set mips3\n dli $2, 0xffffffffbfc00000;jalr $2":::"$2");
}


