#define IO_INB(reg) \
	dli v1, 0xffffffffbfd00000; \
	ori	v1, reg; \
	lbu v0, 0(v1)

/* use a0 as input parameter */
#define IO_OUTB(reg) \
	dli	v0, 0xffffffffbfd00000; \
	ori	v0, reg; \
	sb	a0, 0(v0)

#define IO_INW(reg) \
	dli v1, 0xffffffffbfd00000; \
	ori	v1, reg; \
	lwu	v0, 0(v1)

/* use a0 as input parameter */
#define IO_OUTW(reg) \
	dli v0, 0xffffffffbfd00000; \
	ori	v0, reg; \
	sw a0, 0(v0)


