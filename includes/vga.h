/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  VGA Defination                                         |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef _VGA_H_
#define _VGA_H_
#include "types.h"

typedef enum _Colours {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE, DARKGRAY,
	BRIGHTBLUE, BRIGHTGREEN, BRIGHTCYAN, PINK, BRIGHTMAGENTA, YELLOW, BRIGHTWHITE} Colours;


extern unsigned short *videomem ;		//pointer to video memory
extern unsigned short crtc_mem;
extern unsigned int ypos ;				//offset, used like a y cord
extern unsigned int xpos ;				//position, used like x cord
extern unsigned int scrHeight;
extern unsigned int scrWidth ;
extern unsigned char colour ;			//Text colours
extern unsigned char text ;
extern unsigned char back;

void init_VGA();
void clear() ;
void clear_to_line(void);
void write(char *cp) ;
void putc(char c) ;
void putch(char x);
void gotoxy(unsigned x, unsigned y) ;
unsigned int getX();
unsigned int getY();
void SetColour(Colours Text, Colours Back, bool blink);
unsigned char GetBackColour();
unsigned char GetTextColour();
void SetBackColour(Colours col);
void SetTextColour(Colours col);
void scrollup() ;
void setcursor(unsigned x, unsigned y);
void kprintf( const char *fmt, ... );
#endif

