#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "polarssl/sha2.h"

#define STOCK_INFO "Full Installation Pack"

char packEntryName[][32] =
{
	"Firm0",
	"Firm1",
	"KeySector",
	"Secondary Payload"
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

unsigned int fsize(FILE* file)
{
	long cur = ftell(file);
	fseek(file, 0, SEEK_END);
	unsigned int size = ftell(file);
	fseek(file, cur, SEEK_SET);
	return size;
}

int main(int argc, char** argv)
{
	printf("Arm9LoaderHax Installer Pack Tool - Delebile\n\n");
	if(argc == 5)
	{
		FILE *content[4], *out;
		packHeader header;
		u8* buffer;
		sha2_context sha;
		
		memset((void*)&header, 0, sizeof(packHeader));
		content[0] = fopen(argv[1], "rb");
		content[1] = fopen(argv[2], "rb");
		content[2] = fopen(argv[3], "rb");
		content[3] = fopen(argv[4], "rb");
		printf("Contents : \n");
		for(int i = 0; i < 4; i++)
		{
			if(content[i]) printf("    %s\n", packEntryName[i]);
		}
		
		printf("Packing... ");
		out = fopen("arm9loaderhax.pack", "wb");
		fwrite(&header, 1, sizeof(packHeader), out);
		while((ftell(out) % 0x200) != 0) fputc(0x00, out);
		for(int i = 0; i < 4; i++)
		{
			if(content[i])
			{
				header.entry[i].size = fsize(content[i]);
				while((header.entry[i].size % 0x200) != 0) header.entry[i].size++;
				header.entry[i].offset = ftell(out);
				switch(i)
				{
					case 0:		// firm0
					{
						header.entry[i].crypt = 1;
						header.entry[i].sector = 0x0B130000/0x200;
						break;
					}
					case 1:		// firm1
					{
						header.entry[i].crypt = 1;
						header.entry[i].sector = 0x0B530000/0x200;
						break;
					}
					case 2:		// sector
					{
						header.entry[i].crypt = 0;
						header.entry[i].sector = 0x96;
						break;
					}
					case 3:		// stage2 payload
					{
						header.entry[i].crypt = 0;
						header.entry[i].sector = 0x5C000;
						break;
					}
				}
				fseek(out, 0, SEEK_END);
				buffer = (u8*) malloc (header.entry[i].size);
				fread((void*)buffer, 1, header.entry[i].size, content[i]);
				sha2_starts(&sha, 0);
				sha2_update(&sha, buffer, header.entry[i].size);
				sha2_finish(&sha, header.entry[i].hash);
				
				fwrite((void*)buffer, 1, header.entry[i].size, out);	
				free(buffer);
				fclose(content[i]);
			}
		}
		memcpy(header.info, STOCK_INFO, 32);
		fseek(out, 0, SEEK_SET);
		fwrite(&header, 1, sizeof(packHeader), out);	
		fclose(out);
		
		out = fopen("arm9loaderhax.pack", "rb+");
		u32 totSize = fsize(out);
		buffer = (u8*) malloc (totSize);
		fread(buffer, 1, totSize, out);
		
		sha2_starts(&sha, 0);
		sha2_update(&sha, buffer + 0x20, totSize - 0x20);
		sha2_finish(&sha, header.hash);
		
		fseek(out, 0, SEEK_SET);
		fwrite(&header, 1, sizeof(packHeader), out);	
		fclose(out);
		printf("Done!\n");
	}
	else
	{
		printf("ERROR : Missing %d arguments!\n", 5 - argc);
		printf("Usage : <firm0_path> <firm1_path> <sector_path> <payload_path>\n");
		printf("If you want to exclude some files from the pack, just type 'null'\nas path for that file.\n\n");
	}
	
	return 0;
}