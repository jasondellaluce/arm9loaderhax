#include "nand.h"

static u8 ctrCtr[0x10], twlCtr[0x10], curCtr[0x10], firstTime = 1;

u32 nandGetCtr(u8* ctr, u32 offset)
{
	if(firstTime)
	{
		u8 cid[0x10], sha1sum[20];
		sdmmc_get_cid( 1, (uint32_t*)cid);
		sha(ctrCtr, cid, 0x10, SHA_256_MODE);
		sha(sha1sum, cid, 0x10, SHA_1_MODE);
		for(u32 i = 0; i < 0x10; i++)
		{
			twlCtr[i] = sha1sum[15-i];
		}
		firstTime = 0;
	}
	memcpy(ctr, offset >= 0x0B100000 ? ctrCtr : twlCtr, 0x10);
    aes_advctr(ctr, offset >> 4, AES_INPUT_BE | AES_INPUT_NORMAL);
    return 0;
}

void nandCrypt(u32 sector_no, u32 numsectors, u8 *out, nandRegion section)
{
	u8 keyslot = 0;
	switch(section)
	{	
		case TWLN:		keyslot = 3; break;
		case TWLP:		keyslot = 3; break;
		case AGBSAVE:	keyslot = 7; break;
		case FIRM0:		keyslot = 6; break;
		case FIRM1:		keyslot = 6; break;
		case CTRNAND:	keyslot = 4; break;
		case NEWCTR:	keyslot = 5; break;
	}
	memset((void*)curCtr, 0x00, 0x10);
	nandGetCtr(curCtr, (u32)section + (sector_no << 9));
	aes_setiv(curCtr, AES_INPUT_BE | AES_INPUT_NORMAL);
    aes_use_keyslot(keyslot);
    aes(out, out, (numsectors << 9) >> 4, curCtr, AES_CNT_CTRNAND_MODE, AES_INPUT_BE | AES_INPUT_NORMAL);
}

int nandReadSectors(u32 sector_no, u32 numsectors, u8 *out, nandRegion section)
{
	if(section == CTRNAND && isNew3DS) section = NEWCTR;
	int ret = sdmmc_nand_readsectors(sector_no + ((u32)section >> 9), numsectors, out);
	nandCrypt(sector_no, numsectors, out, (u32)section);
	return ret;
}

int nandWriteSectors(u32 sector_no, u32 numsectors, u8 *out, nandRegion section)
{
	if(section == CTRNAND && isNew3DS) section = NEWCTR;
	nandCrypt(sector_no, numsectors, out, (u32)section);
	int ret = sdmmc_nand_writesectors(sector_no + ((u32)section >> 9), numsectors, out);
	nandCrypt(sector_no, numsectors, out, (u32)section);
	return ret;
}
