#include <Types.h>
#include <Uart.h>
#include <Cs5536.h>

#define CONS_BAUD B115200

#define PHY_TO_UNCACHED(p)      ((p)|0xffffffffa0000000)

u8* UART_BASE = (u8 *)PHY_TO_UNCACHED(UART_ADDR);

void uart_out(u32 port, u8 c)
{
	*(u8 *)(UART_BASE + port) = c;
}

u8 uart_in(u32 port)
{
	return *(u8 *)(UART_BASE + port);
}

void init_uart(void)
{
	uart_out(UART_FCR, UART_FCR_ENABLE_FIFO | UART_FCR_CLEAR_RCVR | UART_FCR_CLEAR_XMIT | UART_FCR_R_TRIG_00);
	uart_out(UART_LCR, UART_LCR_DLAB);
	uart_out(UART_DLL, UART_BAUD / (16 * CONS_BAUD));
	uart_out(UART_DLH, UART_BAUD / (16 * CONS_BAUD) >> 8);
	uart_out(UART_LCR, UART_LCR_WLEN8);
	uart_out(UART_MCR, UART_MCR_RTS | UART_MCR_DTR);
	uart_out(UART_IER, 0);
}


void uart_puts(const u8 *);
void putc_hex(u32 val)
{
	int i;
	u8 buf[9];

	for(i = 7; i >= 0; i--) {
		buf[i] = "0123456789ABCDEF"[val & 0x0F];
		val >>= 4;
	}
	buf[8] = '\0';
	uart_puts(buf);
}

void uart_putc(u8 c)
{
	int timeout = 1000;

	while(timeout-- > 0) {
		if (uart_in(UART_LSR) & UART_LSR_THRE) {
			uart_out(UART_TX, c);
			break;
		}
	}
}

void uart_puts(const u8 * str)
{
	u8 c;

	while((c = *str++) != '\0'){
		uart_putc(c);
		if(c == '\n')
			uart_putc('\r');
	}
}
