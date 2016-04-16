#pragma once

#include "common.h"

typedef enum{
	TWLN 	= 0x00012E00,
	TWLP 	= 0x09011A00,
	AGBSAVE	= 0x0B100000,
	FIRM0	= 0x0B130000,
	FIRM1	= 0x0B530000,
	CTRNAND = 0x0B95CA00,
	NEWCTR	= 0x0B95AE00
} nandRegion;

u32 nandGetCtr(u8* ctr, u32 offset);
void nandCrypt(u32 sector_no, u32 numsectors, u8 *out, nandRegion section);
int nandReadSectors(u32 sector_no, u32 numsectors, u8 *out, nandRegion section);
int nandWriteSectors(u32 sector_no, u32 numsectors, u8 *out, nandRegion section);