#define BUS0_CFG_ADDR(dev, fun, reg) (1<<((dev) + 11) | (fun)<<8 | (reg))

#define BUS0_PCICFG_READ(dev, fun, reg) \
	dli	a0,	BUS0_CFG_ADDR(dev, fun, reg); \
	srl a1, a0, 16; \
	andi a0, a0, 0xffff; \
	dli a2, PHY_TO_UNCACHED(PCIMAP_CFG); \
	sw	a1, 0(a2); \
	dli a2, PHY_TO_UNCACHED(PCICFG_SPACE); \
	or	a2, a2, a0; \
	lw	v0, 0(a2);

#define BUS0_PCICFG_WRITE(dev, fun, reg, val) \
	dli	a0,	BUS0_CFG_ADDR(dev, fun, reg); \
	srl a1, a0, 16; \
	andi a0, a0, 0xffff; \
	dli a2, PHY_TO_UNCACHED(PCIMAP_CFG); \
	sw	a1, 0(a2); \
	dli a2, PHY_TO_UNCACHED(PCICFG_SPACE); \
	or	a2, a2, a0; \
	dli	v0, val; \
	sw	v0, 0(a2);


