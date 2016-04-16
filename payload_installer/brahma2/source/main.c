#include <3ds.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "brahma.h"
#include "hid.h"
#include "menus.h"
#include "sochlp.h"

extern u8 starter_bin[];
extern u32 starter_bin_size;

s32 main (void) {
	gfxInitDefault();
	consoleInit(GFX_BOTTOM, NULL);
	if (brahma_init()) {
		printf(" ");
		load_arm9_payload_from_mem (starter_bin, starter_bin_size);
		printf("%08X\n", firm_reboot ());
		brahma_exit();
	} 
	gfxExit();
	return 0;
}