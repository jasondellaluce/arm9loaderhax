#include "common.h"
#include "sdmmc.h"
#include "i2c.h"

int main()
{
	// Initialize sdcard and nand
	*(vu32*)0x10000020 = 0;
	*(vu32*)0x10000020 = 0x340;
	sdmmc_sdcard_init();
	sdmmc_nand_readsectors(0x5C000, 0x20, (vu8*)0x08006000);
	
	// Jump to secondary payload
	((void (*)())0x08006000)();
	
	i2cWriteRegister(I2C_DEV_MCU, 0x20, (u8)(1<<0));
	while(1);
    return 0;
}
