#include "nand.h"
#include "sha1.h"
#include "sha256.h"
#include "crypto.h"
#include "sdmmc.h"
#include "fatfs/ff.h"

#define NAND_CID            ((u8*) 0x01FFCD84)

u8 tmpctr[16] = {0};
u8 twlctr[16] = {0}, ctrctr[16] = {0};
u32 nand_get_ctr(u8* ctr, u32 offset)
{
    if (offset >= 0x0B100000) { // CTRNAND/AGBSAVE region
		if(*((u32*)&ctrctr[0]) == 0)
		{
			u8 sha256sum[32];
			sha256_context shactx;
			sha256_starts(&shactx);
			sha256_update(&shactx, NAND_CID, 16);
			sha256_finish(&shactx, sha256sum);
			memcpy(ctrctr, sha256sum, 0x10);
		}	
		memcpy(ctr, ctrctr, 0x10);	
    } else { // TWL region
		if(*((u32*)&twlctr[0]) == 0)
		{
			u8 sha1sum[20];
			sha1_context shactx;
			sha1_starts(&shactx);
			sha1_update(&shactx, NAND_CID, 16);
			sha1_finish(&shactx, sha1sum);
			for(u32 i = 0; i < 16; i++) // little endian and reversed order
				twlctr[i] = sha1sum[15-i];
		}
		memcpy(ctr, twlctr, 0x10);	
    }
    add_ctr(ctr, offset / 0x10);
    return 0;
}

u32 sdNandSect = 0xDEAD0000;

u32 findSdNand()
{
	FATFS myFs;
	FIL myFile;
	u32 firstSector = 0;
	u8 *buf = (u8*)0x26000000;
	if(sdNandSect != 0xDEAD0000) return sdNandSect;
	if(f_mount (&myFs, "", 0) == FR_OK)
	{
		if(f_open(&myFile, "NAND.bin", FA_READ | FA_OPEN_EXISTING) == FR_OK)
		{
			// Modded version of f_read, which gives us the file first sector
			f_read(&myFile, buf, 0x200, buf + 0x200);
			firstSector = (u32)myFile.dsect;
			f_close(&myFile);
		}
		f_mount(NULL, "", 0);
	}
	sdNandSect = firstSector;
	return firstSector;
}

u32 nand_get_size()
{
	return *((u32*)0x01FFCDC4) * 0x200;
}

bool emunand_check()
{
	u8 *bootsect = (u8*)0x26000000;
	sdmmc_sdcard_readsectors(nand_get_size()/0x200, 1, bootsect);
	if(*((unsigned int*)&bootsect[0x100]) == 0x4453434e) 
		return 1;
	else
		return 0;
}

u32 nand_get_partition_info(decrypt_ctx* ctx, nand_section section)
{
	ctx->keyY = NULL;
	ctx->iv = tmpctr;
	switch(section)
	{
		case TWLN:
			ctx->keyslot = 3;
			ctx->mode = AES_CNT_TWLNAND_MODE;
			nand_get_ctr(tmpctr, 0x00012E00);
			return 0x00012E00;
		case TWLP:
			ctx->keyslot = 3;
			ctx->mode = AES_CNT_TWLNAND_MODE;
			nand_get_ctr(tmpctr, 0x09011A00);
			return 0x09011A00;
		case AGBSAVE:
			ctx->keyslot = 7;
			ctx->mode = AES_CNT_CTRNAND_MODE;
			nand_get_ctr(tmpctr, 0x0B100000);
			return 0x0B100000;
		case FIRM0:
			ctx->keyslot = 6;
			ctx->mode = AES_CNT_CTRNAND_MODE;
			nand_get_ctr(tmpctr, 0x0B130000);
			return 0x0B130000;
		case FIRM1:
			ctx->keyslot = 6;
			ctx->mode = AES_CNT_CTRNAND_MODE;
			nand_get_ctr(tmpctr, 0x0B530000);
			return 0x0B530000;
		case CTRNAND:
			ctx->keyslot = 4;
			ctx->mode = AES_CNT_CTRNAND_MODE;
			nand_get_ctr(tmpctr, 0x0B95CA00);
			return 0x0B95CA00;
		case NEWCTRNAND:
			ctx->keyslot = 5;
			ctx->mode = AES_CNT_CTRNAND_MODE;
			nand_get_ctr(tmpctr, 0x0B95AE00);
			return 0x0B95AE00;
	}
	return 0;
}
int nand_readsectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section)
{
	decrypt_ctx info;
	info.buf = out;
	info.size = numsectors * 0x200;
	u32 _sector_no = sector_no + nand_get_partition_info(&info, section) / 0x200;
	add_ctr(tmpctr, sector_no * 0x20);
	int ret = sdmmc_nand_readsectors(_sector_no, numsectors, out);
	aes_decrypt_ctx(&info);
	return ret;
}

int nand_writesectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section)
{
	decrypt_ctx info;
	info.buf = out;
	info.size = numsectors * 0x200;
	u32 _sector_no = sector_no + nand_get_partition_info(&info, section) / 0x200;
	add_ctr(tmpctr, sector_no * 0x20);
	aes_decrypt_ctx(&info);
	int ret = sdmmc_nand_writesectors(_sector_no, numsectors, out);
	return ret;
}

int emunand_readsectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section)
{
	decrypt_ctx info;
	info.buf = out;
	info.size = numsectors * 0x200;
	u32 _sector_no = sector_no + nand_get_partition_info(&info, section) / 0x200;
	add_ctr(tmpctr, sector_no * 0x20);
	int ret = sdmmc_sdcard_readsectors(_sector_no + findSdNand(), numsectors, out);
	aes_decrypt_ctx(&info);
	return ret;
}

int emunand_writesectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section)
{
	decrypt_ctx info;
	info.buf = out;
	info.size = numsectors * 0x200;
	u32 _sector_no = sector_no + nand_get_partition_info(&info, section) / 0x200;
	add_ctr(tmpctr, sector_no * 0x20);
	aes_decrypt_ctx(&info);
	int ret = sdmmc_sdcard_writesectors(_sector_no + findSdNand(), numsectors, out);
	return ret;
}