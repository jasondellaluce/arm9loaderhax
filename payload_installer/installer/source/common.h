#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define vu8 volatile u8
#define vu16 volatile u16
#define vu32 volatile u32
#define vu64 volatile u64

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a > _b ? _a : _b; })
#define min(a,b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
       _a < _b ? _a : _b; })

// Internals
#include "screen.h"
#include "system/delay.h"
#include "system/hid.h"
#include "system/i2c.h"
#include "system/sdmmc.h"
#include "system/draw.h"
#include "system/crypto.h"
#include "system/nand.h"
#include "fatfs/ff.h"
#include "fatfs/fs.h"

#define isNew3DS	(*((volatile u32*)0x10140FFC) == 7)
#define isColdBoot	(*((volatile u32*)0x10010000) == 0)
