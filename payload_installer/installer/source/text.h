#ifndef TEXT_H
#define TEXT_H

#define RGB(r,g,b)          0 | (r<<16|g<<8|b)          //console asks for B,G,R in bytes
#define BLACK               RGB(0, 0, 0)
#define WHITE               RGB(255, 255, 255)
#define RED                 RGB(255, 0, 0)
#define GREEN	    	    RGB(0, 255, 0)
#define BLUE		        RGB(0, 0, 255)
#define GREY		        RGB(0x77, 0x77, 0x77)   //GW Gray shade
#define TRANSPARENT         RGB(255, 0, 255)        //Magenta is my favourite transparent color

char* hex2str(unsigned int val);
void drawCharacter(unsigned char* fb, char c, unsigned short x, unsigned short y);
void drawString(char* str, unsigned short x, unsigned short y);
void drawHex(unsigned val, int x, int y);
void drawStringHex(char* str, unsigned val, int x, int y);
void printf(char* str);


#endif
