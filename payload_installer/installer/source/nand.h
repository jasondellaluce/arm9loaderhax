#pragma once

#include "common.h"

typedef enum{
	TWLN,
	TWLP,
	AGBSAVE,
	FIRM0,
	FIRM1,
	CTRNAND,
	NEWCTRNAND
} nand_section;

typedef enum{
	NONAND = -1,
	SYSNAND = 0,
	EMUNAND = 1,
} nand_type;

u32 nand_get_ctr(u8* ctr, u32 offset);
u32 nand_get_size();
bool emunand_check();
u32 findSdNand();
int nand_readsectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section);
int nand_writesectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section);
int emunand_readsectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section);
int emunand_writesectors(u32 sector_no, u32 numsectors, u8 *out, nand_section section);