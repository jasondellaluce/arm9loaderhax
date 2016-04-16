#pragma once

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef s32 result;		

typedef enum
{
	RES_OK = 0,
	RES_GENERAL_ERROR,
	RES_BAD_PARAMETER,
	RES_FILE_ERROR,
} errors;			