#pragma once

#include "ff.h"

typedef struct{
	FIL file;
	unsigned char active;
} fsFile;

void fsInit();
void fsExit();
fsFile* fsOpen(char* path, int flag);
void fsClose(fsFile* file);
unsigned int fsRead(void* ptr, unsigned int size, unsigned int nmemb, fsFile* file);
unsigned int fsWrite(void* ptr, unsigned int size, unsigned int nmemb, fsFile* file);
void fsSeek(fsFile* file, unsigned int off);
unsigned int fsGetSize(fsFile* file);