#include <inttypes.h>

#define BRIGHTNESS 0x39
#define FB_TOP_LEFT 0x18300000
#define FB_TOP_RIGHT 0x18300000
#define FB_BOTTOM 0x18346500

static inline void regSet();

void __attribute__ ((naked)) a11Entry()
{
	__asm__ (
		"CPSID aif\n\t" //Disable interrupts
		"ldr r0,=_stack\n\t"
		"mov sp, r0"
	);

	regSet();
}

static inline void regSet()
{
	volatile uint32_t *entry = (uint32_t *)0x1FFFFFF8;

	*(volatile uint32_t*)0x10141200 = 0x1007F;
	*(volatile uint32_t*)0x10202014 = 0x00000001;
	*(volatile uint32_t*)0x1020200C &= 0xFFFEFFFE;

	*(volatile uint32_t*)0x10202240 = BRIGHTNESS;
	*(volatile uint32_t*)0x10202A40 = BRIGHTNESS;
	*(volatile uint32_t*)0x10202244 = 0x1023E;
	*(volatile uint32_t*)0x10202A44 = 0x1023E;

	// Top screen
	*(volatile uint32_t*)0x10400400 = 0x000001c2;
	*(volatile uint32_t*)0x10400404 = 0x000000d1;
	*(volatile uint32_t*)0x10400408 = 0x000001c1;
	*(volatile uint32_t*)0x1040040c = 0x000001c1;
	*(volatile uint32_t*)0x10400410 = 0x00000000;
	*(volatile uint32_t*)0x10400414 = 0x000000cf;
	*(volatile uint32_t*)0x10400418 = 0x000000d1;
	*(volatile uint32_t*)0x1040041c = 0x01c501c1;
	*(volatile uint32_t*)0x10400420 = 0x00010000;
	*(volatile uint32_t*)0x10400424 = 0x0000019d;
	*(volatile uint32_t*)0x10400428 = 0x00000002;
	*(volatile uint32_t*)0x1040042c = 0x00000192;
	*(volatile uint32_t*)0x10400430 = 0x00000192;
	*(volatile uint32_t*)0x10400434 = 0x00000192;
	*(volatile uint32_t*)0x10400438 = 0x00000001;
	*(volatile uint32_t*)0x1040043c = 0x00000002;
	*(volatile uint32_t*)0x10400440 = 0x01960192;
	*(volatile uint32_t*)0x10400444 = 0x00000000;
	*(volatile uint32_t*)0x10400448 = 0x00000000;
	*(volatile uint32_t*)0x1040045C = 0x00f00190;
	*(volatile uint32_t*)0x10400460 = 0x01c100d1;
	*(volatile uint32_t*)0x10400464 = 0x01920002;
	*(volatile uint32_t*)0x10400468 = 0x18300000;
	*(volatile uint32_t*)0x10400470 = 0x80341;
	*(volatile uint32_t*)0x10400474 = 0x00010501;
	*(volatile uint32_t*)0x10400478 = 0;
	*(volatile uint32_t*)0x10400490 = 0x000002D0;
	*(volatile uint32_t*)0x1040049C = 0x00000000;

	// Disco register
	for(volatile uint32_t i = 0; i < 256; i++)
		*(volatile uint32_t*)0x10400484 = 0x10101 * i;

	// Bottom screen
	*(volatile uint32_t*)0x10400500 = 0x000001c2;
	*(volatile uint32_t*)0x10400504 = 0x000000d1;
	*(volatile uint32_t*)0x10400508 = 0x000001c1;
	*(volatile uint32_t*)0x1040050c = 0x000001c1;
	*(volatile uint32_t*)0x10400510 = 0x000000cd;
	*(volatile uint32_t*)0x10400514 = 0x000000cf;
	*(volatile uint32_t*)0x10400518 = 0x000000d1;
	*(volatile uint32_t*)0x1040051c = 0x01c501c1;
	*(volatile uint32_t*)0x10400520 = 0x00010000;
	*(volatile uint32_t*)0x10400524 = 0x0000019d;
	*(volatile uint32_t*)0x10400528 = 0x00000052;
	*(volatile uint32_t*)0x1040052c = 0x00000192;
	*(volatile uint32_t*)0x10400530 = 0x00000192;
	*(volatile uint32_t*)0x10400534 = 0x0000004f;
	*(volatile uint32_t*)0x10400538 = 0x00000050;
	*(volatile uint32_t*)0x1040053c = 0x00000052;
	*(volatile uint32_t*)0x10400540 = 0x01980194;
	*(volatile uint32_t*)0x10400544 = 0x00000000;
	*(volatile uint32_t*)0x10400548 = 0x00000011;
	*(volatile uint32_t*)0x1040055C = 0x00f00140;
	*(volatile uint32_t*)0x10400560 = 0x01c100d1;
	*(volatile uint32_t*)0x10400564 = 0x01920052;
	*(volatile uint32_t*)0x10400568 = 0x18300000 + 0x46500;
	*(volatile uint32_t*)0x10400570 = 0x80301;
	*(volatile uint32_t*)0x10400574 = 0x00010501;
	*(volatile uint32_t*)0x10400578 = 0;
	*(volatile uint32_t*)0x10400590 = 0x000002D0;
	*(volatile uint32_t*)0x1040059C = 0x00000000;

	// Disco register
	for(volatile uint32_t i = 0; i < 256; i++)
		*(volatile uint32_t*)0x10400584 = 0x10101 * i;

	*(volatile uint32_t*)0x10400468 = FB_TOP_LEFT;
	*(volatile uint32_t*)0x1040046c = FB_TOP_LEFT;
	*(volatile uint32_t*)0x10400494 = FB_TOP_RIGHT;
	*(volatile uint32_t*)0x10400498 = FB_TOP_RIGHT;
	*(volatile uint32_t*)0x10400568 = FB_BOTTOM;
	*(volatile uint32_t*)0x1040056c = FB_BOTTOM;

	// Reset the entry
	*entry = 0;

	// Wait for entry to be set
	while(!*entry);

	// Jump
	((void (*)())*entry)();
}

