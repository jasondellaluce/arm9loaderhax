#include "common.h"
#include "screen.h"

static char packEntryName[4][32] =
{
	"Hacked Firm0",
	"Legit Firm1",
	"Hacked KeySector",
	"Secondary Payload"
};

static unsigned char fixedHashes[2][32] =
{
	{
		0xA8, 0x51, 0xD6, 0x54, 0x6E, 0x43, 0xB5, 0x3D,
		0x70, 0x93, 0xCB, 0x6E, 0x54, 0x74, 0x92, 0x60,
		0xFE, 0x0F, 0xD0, 0x36, 0x60, 0xD4, 0x92, 0xAB,
		0x51, 0xA3, 0x20, 0xBE, 0xBA, 0x34, 0xE4, 0x4A
	},
	{
		0xD2, 0x53, 0xC1, 0xCC, 0x0A, 0x5F, 0xFA, 0xC6,
		0xB3, 0x83, 0xDA, 0xC1, 0x82, 0x7C, 0xFB, 0x3B,
		0x2D, 0x3D, 0x56, 0x6C, 0x6A, 0x1A, 0x8E, 0x52,
		0x54, 0xE3, 0x89, 0xC2, 0x95, 0x06, 0x23, 0xE5 
	}
};

typedef struct
{
	unsigned int offset;
	unsigned int size;
	unsigned int sector;
	unsigned int crypt;
	unsigned char hash[32];
} packEntry;

typedef struct
{
	unsigned char hash[32];
	char info[32];
	packEntry entry[4];
} packHeader;

int checkRegionHash(void* buf, size_t size, void* sha256)
{
	unsigned char calc[32];
	sha(calc, buf, size, SHA_256_MODE);
	if(memcmp(calc, sha256, 32) == 0) return 0;
	return 1;
}

int ctrCryptoTest()
{
	int res = 0;
	u8 *tmp = (u8*)0x2000E000;
	
	nandReadSectors(0, 1, (void*)tmp, FIRM0); 
	if(*((u32*)((void*)tmp)) == 0x4D524946) res++;
	nandReadSectors(0, 1, (void*)tmp, FIRM1); 
	if(*((u32*)((void*)tmp)) == 0x4D524946) res++;
	return res;
}

int packAnalyze(void* buf, size_t size)
{
	u8* buffer = (u8*)buf;
	packHeader* header = (packHeader*)buf;
	if(!checkRegionHash((void*)(buffer + 32), size - 32, (void*)header->hash))
	{
		Debug("%s", header->info);
		int valid = 0;
		for(int i = 0; i < 4; i++)
		{
			if(header->entry[i].size > 0)
			{
				if(!checkRegionHash((void*)(buffer + header->entry[i].offset), header->entry[i].size, (void*)header->entry[i].hash))
				{
					if(i < 2)
					{
						if(i == 0 || memcmp((void*)fixedHashes[i], (void*)header->entry[i].hash, 32) == 0)
						{
							Debug("  [GOOD] %s", packEntryName[i]);
						}
						else
						{
							Debug("  [FAIL] %s", packEntryName[i]);
							valid++;
						}
					}
					else
					{
						Debug("  [GOOD] %s", packEntryName[i]);
					}
				}
				else
				{
					Debug("  [FAIL] %s", packEntryName[i]);
					valid++;
				}
			}
		}
		if(!valid)
		{
			Debug("");
			Debug("The listed content will be installed on your");
			Debug("%s console. Be sure to not remove", isNew3DS ? "New 3DS" : "Old 3DS");
			Debug("your SD card during the installation process");
			Debug("and that the pack you're using contains your");
			Debug("console-specific necessary data, otherwise you");
			Debug("risk permanently BRICKING your console.");
			Debug("");
			Debug("Press START to continue, or B to abort...");
			Debug("");
			while(1)
			{
				u32 key = waitHid();
				if(key & BUTTON_START) return 0;
				if(key & BUTTON_B) return 2;
			}
		}
		else
		{
			Debug("ERROR : %d content files are corrupted!", valid);
		}
	}
	else
	{
		Debug("ERROR : Package main hash check failed!");
	}
	return 1;
}

void arm9LoaderHaxInstall()
{
	DebugClear();
	Debug("Arm9LoaderHax Exploit Installer - Delebile");
	Debug("");
	
	fsFile *packFile = fsOpen("sdmc:/arm9loaderhax.pack", 1);
	if(packFile)
	{
		u8* packBuf = (u8*)0x24000000;
		u32 packSize = fsGetSize(packFile);
		fsRead((void*)packBuf, 1, packSize, packFile);
		fsClose(packFile);
		int res = packAnalyze(packBuf, packSize);
		if(!res)
		{
			if(ctrCryptoTest())
			{
				packHeader* header = (packHeader*)packBuf;
				for(int i = 0; i < 4; i++)
				{
					if(header->entry[i].size > 0)
					{
						if(header->entry[i].crypt && i < 2)
						{
							Debug("Install : %s...", packEntryName[i]);
							nandWriteSectors(0, (header->entry[i].size >> 9), packBuf + header->entry[i].offset, (nandRegion)(header->entry[i].sector << 9));
						}
						else
						{
							Debug("Install : %s...", packEntryName[i]);
							sdmmc_nand_writesectors(header->entry[i].sector, (header->entry[i].size >> 9), packBuf + header->entry[i].offset);
						}
					}
				}
			}
			else
			{
				Debug("ERROR : Could not decrypt NAND properly!");
			}
		}
		else if(res == 1)
		{
			Debug("ERROR : The installation pack is corrupted!");
		}
	}
	else
	{
		Debug("ERROR : Cannot open \"arm9loaderhax.pack\"!");
	}
	
	Debug(" ");
	Debug("Press A to exit...");
	while(1)
	{
		u32 key = waitHid();
		if(key & BUTTON_A) return;
	}
}

int main()
{
	screenInit();
	fsInit();

	arm9LoaderHaxInstall();
	
	fsExit();
	i2cWriteRegister(I2C_DEV_MCU, 0x20, (u8)(1<<0));
    return 0;
}
