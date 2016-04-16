#include "fs.h"
#include "../common.h"

#define MAX_FILES	100

fsFile fileBuf[MAX_FILES];
static FATFS sdmcfs, nandfs;

void keysInit()
{
	if(isNew3DS)
	{
		// CTRNAND Keyslot 0x05 initialization
		u8 nKeyY[16] = {0x4D, 0x80, 0x4F, 0x4E, 0x99, 0x90, 0x19, 0x46, 0x13, 0xA2, 0x04, 0xAC, 0x58, 0x44, 0x60, 0xBE};
		aes_setkey(5, (u8*)nKeyY, AES_KEYY, AES_INPUT_BE | AES_INPUT_NORMAL);
	}
}

void fsInit()
{
	keysInit();
	sdmmc_sdcard_init();
	f_mount(&sdmcfs, "sdmc:", 0);
	f_mount(&nandfs, "nand:", 0);
	for(int i = 0; i < MAX_FILES; i++)
	{
		fileBuf[i].active = 0;
	}
}

void fsExit()
{	
	f_mount(0, "sdmc:", 0);
	f_mount(0, "nand:", 0);
	for(int i = 0; i < MAX_FILES; i++)
	{
		fileBuf[i].active = 0;
	}
}

fsFile* fsOpen(char* path, int flag)
{
	fsFile* ret = 0;
	for(int i = 0; i < MAX_FILES; i++)
	{
		if(fileBuf[i].active == 0)
		{
			fileBuf[i].active = 1;
			ret = &fileBuf[i];
			break;
		}
	}
	if(ret)
	{
		if(f_open(&ret->file, path, flag) != FR_OK)
		{
			ret = 0;
		}
	}
	return ret;
}

void fsClose(fsFile* file)
{
	if(file)
	{
		file->active = 0;
		f_close(&file->file);
	}
}

void fsSeek(fsFile* file, unsigned int off)
{
	f_lseek(&file->file, off);
}

unsigned int fsRead(void* ptr, unsigned int size, unsigned int nmemb, fsFile* file)
{
	unsigned int bytes = 0;
	f_read(&file->file, ptr, size*nmemb, &bytes);
	return bytes;
}

unsigned int fsWrite(void* ptr, unsigned int size, unsigned int nmemb, fsFile* file)
{
	unsigned int bytes = 0;
	f_write(&file->file, ptr, size*nmemb, &bytes);
	return bytes;
}

unsigned int fsGetSize(fsFile* file)
{
	return file->file.fsize;
}
