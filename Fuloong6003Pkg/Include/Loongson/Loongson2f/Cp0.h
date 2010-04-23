/*
 * according to Loongson2F user manual
 *
 */
#define CP0_INDEX		$0
#define CP0_RANDOM		$1
#define CP0_ENTRYLO0	$2
#define CP0_ENTRYLO1	$3
#define CP0_CONTEXT		$4
#define CP0_PAGEMASK	$5
#define CP0_WIRED		$6
/*reg 7 reserved*/
#define CP0_BADVADDR	$8
#define CP0_COUNT		$9
#define CP0_ENTRYHI		$10
#define CP0_COMPARE		$11
#define CP0_STATUS		$12
#define CP0_CAUSE		$13
#define CP0_EPC			$14
#define CP0_PRID		$15
#define CP0_CONFIG		$16
#define CP0_LLADDR		$17
#define CP0_WATCH		$18
/*reg 19 reserved*/
#define CP0_XCONTEXT	$20
/*reg 21 reserved*/
#define CP0_DIAGNOSE	$22
/*reg 23 reserved*/
#define CP0_PCLO		$24
#define CP0_PCHI		$25
/*reg 26,27 reserved*/
#define CP0_TAGLO		$28
#define CP0_TAGHI		$29
#define CP0_ERROREPC	$30

/**** status register bit define *****/
#define STATUS_IE		(1 << 0)
#define STATUS_KSU		(3 << 3)
#define STATUS_BEV		(1 << 22)


/** CPO_CONFIG **/
#define KSEG0_CACHETYPE		(7 << 0)
#define DCACHE_SIZE			(7 << 6)
#define ICACHE_SIZE			(7 << 9)
#define DCACHE_LINE_SIZE	(1 << 4)
#define ICACHE_LINE_SIZE	(1 << 5)
#define DCACHE_SIZE_START_BIT	6
#define ICACHE_SIZE_START_BIT	9
#define DCACHE_LINE_SIZE_START_BIT	4
#define ICACHE_LINE_SIZE_START_BIT	5

/*CP0_PAGEMASK*/

#define PAGE4K_MASK (000 << 13)   // see 2f user manual for detail(table 5-6)
#define PAGE16K_MASK (003 << 13)
