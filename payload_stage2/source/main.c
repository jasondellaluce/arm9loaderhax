#include "common.h"
#include "i2c.h"
#include "fatfs/ff.h"
#include "screen.h"
#include "hid.h"
#include "flush.h"

#define PAYLOAD_ADDRESS		0x23F00000
#define A11_PAYLOAD_LOC		0x1FFF4C80  //keep in mind this needs to be changed in the ld script for screen_init too
#define A11_ENTRY			0x1FFFFFF8

extern u8 screen_init_bin[];
extern u32 screen_init_bin_size;

static inline void* copy_memory(void *dst, void *src, size_t amount)
{
	void *result = dst;
	while (amount--)
	{
		*((char*)(dst++)) = *((char*)(src++));
	}
	return result;
}

static void ownArm11()
{
	copy_memory((void*)A11_PAYLOAD_LOC, screen_init_bin, screen_init_bin_size);
	*(vu32 *)A11_ENTRY = 1;
	*((u32*)0x1FFAED80) = 0xE51FF004;
	*((u32*)0x1FFAED84) = A11_PAYLOAD_LOC;
	*((u8*)0x1FFFFFF0) = 2;

	//AXIWRAM isn't cached, so this should just work
	while(*(volatile uint32_t *)A11_ENTRY);
}

int main()
{
	FATFS fs;
	FIL payload;

	f_mount(&fs, "0:", 0); //This never fails due to deferred mounting
	if(f_open(&payload, "arm9loaderhax.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
	{
		setFramebuffers();
		ownArm11();
		clearScreens();
		turnOnBacklight();

		unsigned int br;
		f_read(&payload, (void*)PAYLOAD_ADDRESS, f_size(&payload), &br);
		flush_all_caches();
		((void (*)(void))PAYLOAD_ADDRESS)();
	}

	i2cWriteRegister(I2C_DEV_MCU, 0x20, (u8)(1<<0));
	return 0;
}

