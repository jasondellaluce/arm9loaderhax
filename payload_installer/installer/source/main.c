#include "common.h"
#include "hid.h"
#include "delay.h"
#include "text.h"
#include "crypto.h"
#include "fatfs/ff.h"
#include "i2c.h"
#include "nand.h"
#include "crypto.h"

extern unsigned char* topScreen;
extern unsigned char* subScreen;
extern u8 payload_bin[];
extern u32 payload_bin_size;
u8* arm9bin = (u8*)0x08006000;
u8* arm9start = (u8*)0x08006800;

void getScreenControl()
{
	void* arm11Addr = (void*)0x1FFF4C80;
	extern unsigned int arm11Code;
	extern unsigned int arm11CodeSize;
	memcpy(arm11Addr, (void*)&arm11Code, arm11CodeSize);
	*(unsigned int*)0x1FFFFFFC = arm11Addr;
	*(unsigned int*)0x1FFFFFF8 = arm11Addr;
	for(int i = 0; i < 0x46500; i++)
	{
		*((unsigned char*)topScreen + i) = 0x00;
		*((unsigned char*)subScreen + i) = 0x00;
	}
}

int main()
{
	getScreenControl();
	sdmmc_sdcard_init();
	FATFS fs;
	FIL file;
	u32 br;
	
	if(f_mount(&fs, "0:", 0) == FR_OK)
	{
		if(f_open(&file, "NAND.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
		{
			printf("Install NAND backup...");
			for(u32 i = 0; i < 0x3AF00000; i += 0x40000)
			{
				f_lseek(&file, i); 
				f_read(&file, 0x21000000, 0x40000, &br);
				sdmmc_nand_writesectors(i/0x200, 0x200, 0x21000000);
				drawHex(i, 10, 210);
			}
			printf("done.");
			f_close(&file);
		}
	}
	printf("Install stage2...");
	sdmmc_nand_writesectors(0x5C000, 0x20, 0x23300000);
	printf("done.");
	
	printf("Install sector...");
	sdmmc_nand_writesectors(0x96, 0x1, 0x23000000);
	printf("done.");
	
	printf("Install firm0...");
	nand_writesectors(0, 0x100000/0x200, 0x23100000, FIRM0);
	printf("done.");

	printf("Install firm1...");
	nand_writesectors(0, 0x100000/0x200, 0x23200000, FIRM1);
	printf("done.");

	i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
    return 0;
}
