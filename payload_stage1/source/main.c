#include "common.h"
#include "sdmmc.h"

int main()
{
	// Initialize sdcard and nand
	*(vu32*)0x10000020 = 0;
	*(vu32*)0x10000020 = 0x340;
	sdmmc_sdcard_init();
	sdmmc_nand_readsectors(0x5C000, 0x20, (u8*)0x08006000);
	
	// Jump to secondary payload
	((void (*)())0x08006000)();
	
    return 0;
}
