#include "common.h"
#include "sdmmc.h"
#include "i2c.h"
#include "fatfs/ff.h"
#include "screen_init.h"

#define PAYLOAD_ADDRESS		0x23F00000
#define PAYLOAD_SIZE		0x00100000
#define A11_PAYLOAD_LOC     0x1FFF4C80  //keep in mind this needs to be changed in the ld script for screen_init too
#define SCREEN_SIZE 		400 * 240 * 3 / 4 //yes I know this is more than the size of the bootom screen


extern u8 screen_init_bin[];
extern u32 screen_init_bin_size;

void ownArm11()
{
    memcpy((void*)A11_PAYLOAD_LOC, screen_init_bin, screen_init_bin_size);
	*((u32*)0x1FFAED80) = 0xE51FF004;
	*((u32*)0x1FFAED84) = A11_PAYLOAD_LOC;
	for(int i = 0; i < 0x80000; i++)
	{
		*((u8*)0x1FFFFFF0) = 2;
	}
	for(volatile unsigned int i = 0; i < 0xF; ++i);
	while(*(volatile uint32_t *)0x1FFFFFF8 != 0);
}

//fixes the snow issue
clearScreen()
{
	for(int i = 0; i < (SCREEN_SIZE); i++)
	{
		*((unsigned int*)0x18300000 + i) = 0;
		*((unsigned int*)0x18346500 + i) = 0;
	}
}

int main()
{
	//gateway
	*(volatile uint32_t*)0x80FFFC0 = 0x18300000;    // framebuffer 1 top left
	*(volatile uint32_t*)0x80FFFC4 = 0x18300000;    // framebuffer 2 top left
	*(volatile uint32_t*)0x80FFFC8 = 0x18300000;    // framebuffer 1 top right
	*(volatile uint32_t*)0x80FFFCC = 0x18300000;    // framebuffer 2 top right
	*(volatile uint32_t*)0x80FFFD0 = 0x18346500;    // framebuffer 1 bottom
	*(volatile uint32_t*)0x80FFFD4 = 0x18346500;    // framebuffer 2 bottom
	*(volatile uint32_t*)0x80FFFD8 = 1;    // framebuffer select top
	*(volatile uint32_t*)0x80FFFDC = 1;    // framebuffer select bottom

	//cakehax
	*(u32*)0x23FFFE00 = 0x18300000;
	*(u32*)0x23FFFE04 = 0x18300000;
	*(u32*)0x23FFFE08 = 0x18346500;

	FATFS fs;
	FIL payload;
	u32 br;
	
	if(f_mount(&fs, "0:", 0) == FR_OK)
	{
		if(f_open(&payload, "arm9loaderhax.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
		{
			f_read(&payload, PAYLOAD_ADDRESS, PAYLOAD_SIZE, &br);
			ownArm11();
            screenInit();
            clearScreen();
			((void (*)())PAYLOAD_ADDRESS)();
		}
	}
	
	i2cWriteRegister(I2C_DEV_MCU, 0x20, (u8)(1<<0));
    return 0;
}
