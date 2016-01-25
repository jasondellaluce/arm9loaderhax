#include "text.h"
#include "font_bin.h"

extern unsigned char* topScreen;
extern unsigned char* subScreen;

#define CHAR_SIZE_X (8)
#define CHAR_SIZE_Y (8)

void drawCharacter(unsigned char* fb, char c, unsigned short x, unsigned short y)
{
	if(c<32)return;
	c-=32;
	unsigned char* charData=(unsigned char*)&font_bin[(CHAR_SIZE_X*CHAR_SIZE_Y*c)/8];
	fb+=(x*240+y)*3;
	int i, j;
	for(i=0;i<CHAR_SIZE_X;i++)
	{
		unsigned char v=*(charData++);
		for(j=0;j<CHAR_SIZE_Y;j++)
		{
			if(v&1)fb[0]=fb[1]=fb[2]=0xff;
			else fb[0]=fb[1]=fb[2]=0x00;
			fb+=3;
			v>>=1;
		}
		fb+=(240-CHAR_SIZE_Y)*3;
	}
}

void drawString(char* str, unsigned short x, unsigned short y)
{
	if(!str)return;
	y=232-y;
	int k;
	int dx=0, dy=0;
	for(k=0;k<strlen(str);k++)
	{
		if(str[k]>=32 && str[k]<128){
			drawCharacter(topScreen,str[k],x+dx,y+dy);
		}
		dx+=8;
		if(str[k]=='\n'){dx=0;dy-=8;}
	}
}

char HEXSTR[10];
char* hex2str(unsigned int val)
{
	int i;
	memset(HEXSTR, 0, 10);
    char *ptr = HEXSTR + 8;
    for(i = 0; i < 8; i++){
		int x = (val & 0xF);
		char y = '0';
		if(x > 0x9) y +=7;
        *--ptr = y + x;
        val >>= 4;
    }
	return &HEXSTR[0];
}

void drawHex(unsigned val, int x, int y)
{
	drawString(hex2str(val), x, y);
}

void drawStringHex(char* str, unsigned val, int x, int y)
{
	drawString(str, x, y);
	drawHex(val, x + strlen(str)*CHAR_SIZE_X, y);
}

unsigned int curLine = 2;

void printf(char* str){
	drawString(str, 10, 9*curLine++);
} 