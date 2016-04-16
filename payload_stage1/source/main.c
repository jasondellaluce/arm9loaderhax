#include "common.h"
#include "sdmmc.h"
#include "i2c.h"
#include "hid.h"

int main()
{
	// Initialize sdcard and nand
	*(u32*)0x10000020 = 0;
    *(u32*)0x10000020 = 0x340;
	sdmmc_sdcard_init();
	sdmmc_nand_readsectors(0x5C000, 0x20, (u8*)0x08006000);
	
	// Jump to secondary payload
	((void (*)())0x08006000)();
    return 0;
}
