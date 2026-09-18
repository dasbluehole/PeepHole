/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  VGA implementation                                     |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#include "stdargs.h"
#include "low-io.h"
#include "types.h"
#include "string.h"

#define WHITE_BLACK	0x07	// white on black text
#define RED_GREEN	0x26	// red on green text
#define BLUE_BLACK	1	// Blue on black
#define RED_BLACK	4	// red on black
#ifndef NULL
# define NULL 0
#endif
// Printf Defines:
#define		PR_LJ	0x01	// left Justify
#define		PR_CA	0x02	// Use A-F Instead Of a-f For Hex Numbers
#define		PR_SG	0x04	// Signed Numeric Conversion ( %d vs. %u )
#define		PR_32	0x08	// Long ( 32-bit ) Numeric Conversion
#define		PR_16	0x10	// Short ( 16-bit ) Numeric Conversion
#define		PR_WS	0x20	// PR_SG set and num was < 0
#define		PR_LZ	0x40	// Pad Left With '0' Instead Of ' '
#define		PR_FP	0x80	// Pointers Are Far

// Largest number handled is 2 ^ 32 - 1, lowest radix handled is 8.
// 2 ^ 32 - 1 in base 8 has 11 digits ( add 5 for trailing NULL and for slop )
#define		PR_BUFLEN	16
typedef int ( * fnptr_t ) ( unsigned c, void** helper );

static unsigned int cur_cursorX, cur_cursorY;			// this is for keeping track of the text mode cursor
static unsigned short *vgaadr;				// the VGA address
static unsigned textattrib, scrwidth, scrheight;	// the text color, the screen width, & height

unsigned int scrHeight =25;
unsigned int scrWidth =80;
unsigned char colour =0x7;			//Text colours
unsigned char text =0x7;
unsigned char back =0;

void gotoxy( int x, int y );

void start_textmode()
{
	cur_cursorX = 0;
	cur_cursorY = 0;

	gotoxy( 0, 0 );

	vgaadr = ( unsigned short * ) 0xb8000;

	textattrib = WHITE_BLACK;		// white on black
	scrwidth = 80;
	scrheight = 25;
}
void init_VGA()
{
	start_textmode();
}
void gotoxy( int x, int y )
{
        unsigned short	position = ( y * scrwidth + x );

        // cursor LOW port to vga INDEX register
        outportb( 0x3D4, 0x0F );
        outportb( 0x3D5, ( unsigned char ) ( position & 0xFF ) );

        // cursor HIGH port to vga INDEX register
        outportb( 0x3D4, 0x0E );
	outportb( 0x3D5, ( unsigned char ) ( ( position >> 8 ) & 0xFF ) );

	cur_cursorX = x;
	cur_cursorY = y;
}
unsigned int getX()
{
	return (cur_cursorX);
}
unsigned int getY()
{
	return (cur_cursorY);
}
void update_cursor()
{
	gotoxy( cur_cursorX, cur_cursorY );
}
void scrollup()		// scroll the screen up one line 
{
	unsigned int t;

	
				//can cause errors (more of an annoyance than anything else)
	for(t = 0; t < scrwidth * (scrheight - 1); t++)		// scroll every line up 
		*(vgaadr + t) = *(vgaadr + t + scrwidth);
	for(; t < scrwidth * scrheight; t++)				//clear the bottom line 
		*(vgaadr + t) = ' ' | (textattrib << 8);

	
}
void setcolor( unsigned color )
{
	textattrib = color;
}
void putch( unsigned char c )
{
	unsigned att;

	att = textattrib << 8;

	// backspace
	if ( c == 0x08 )
	{
		unsigned short *where;		
		if ( cur_cursorX != 0 )
			cur_cursorX--;

		else if ( cur_cursorX == 0 && cur_cursorY > 0 )
		{
			cur_cursorY--;
			cur_cursorX = scrwidth - 1;	// minus 1 because it'll be off screen or not move if
							//  == scrwidth
			update_cursor();

			return;
		}
		where = vgaadr + ( cur_cursorY * scrwidth + cur_cursorX );
		*where = ' ' | att;
		
	}

	// tab
	else if ( c == 0x09 )
	{
		cur_cursorX += 4;	// tabs are 4 spaces in FOS
	}

	// carriage return
	else if ( c == '\r' )
	{
		cur_cursorX = 0;
	}

	// new line
	else if ( c == '\n' )		// in FOS, a '\n' is a new line at the start of the line
	{
		cur_cursorX = 0;
		cur_cursorY++;
	}

	// ASCII characters - 123...abc...
	else if ( c >= ' ' )
	{
		unsigned short *where;

		where = vgaadr + ( cur_cursorY * scrwidth + cur_cursorX );
		*where = c | att;
		cur_cursorX++;
	}

	if ( cur_cursorX >= scrwidth )
	{
		cur_cursorX = 0;
		cur_cursorY++;
	}
	if(cur_cursorY==scrheight)
	{
		scrollup();
		cur_cursorY--;
	}
	update_cursor();
}
int doprintf( const char *fmt, va_list args, fnptr_t fn, void *ptr )
{
	unsigned state, flags, radix, actual_wd, count, given_wd;
	unsigned char *where, buf[ PR_BUFLEN ];
	long num;

	state = flags = count = given_wd = 0;

// Begin scanning format specifier list
	for ( ; *fmt; fmt++ )
	{
		switch ( state )
		{
		// STATE 0: AWAITING %
		case 0:
			if ( *fmt != '%' )	// not %...
			{
				fn ( *fmt, &ptr );	// ...just echo it
				count++;

				break;
			}

// Found %, get next character and advance state to check if next char is a flag
			state++;
			fmt++;
			// FALL THROUGH
			// STATE 1: AWAITING FLAGS ( % - 0 )
		case 1:
			if ( *fmt == '%' )	// %%
			{
				fn ( *fmt, &ptr );
				count++;
				state = flags = given_wd = 0;

				break;
			}

			if ( *fmt == '-' )
			{
				if ( flags & PR_LJ )	// %-- is illegal
					state = flags = given_wd = 0;
				else
					flags |= PR_LJ;

				break;
			}

// Not a flag char: advance state to check if it's field width
			state++;
// Check now for '%0...'

			if ( *fmt == '0' )
			{
				flags |= PR_LZ;
				fmt++;
			}

			// FALL THROUGH
			// STATE 2: AWAITING (NUMERIC) FIELD WIDTH
		case 2:
			if ( *fmt >= '0' && *fmt <= '9' )
			{
				given_wd = 10 * given_wd +
					( *fmt - '0' );

				break;
			}

// Not field width: advance state to check if it's a modifier
			state++;
			// FALL THROUGH
			// STATE 3: AWAITING MODIFIER CHARS ( FNlh )
		case 3:
			if ( *fmt == 'F' )
			{
				flags |= PR_FP;

				break;
			}

			if ( *fmt == 'N' )
				break;

			if ( *fmt == 'l' )
			{
				flags |= PR_32;

				break;
			}

			if ( *fmt == 'h' )
			{
				flags |= PR_16;

				break;
			}

// Not modifier: advance state to check if it's a conversion char
			state++;
			// FALL THROUGH
// STATE 4: AWAITING CONVERSION CHARS ( Xxpndiuocs )
		case 4:
			where = buf + PR_BUFLEN - 1;
			*where = '\0';

			switch ( *fmt )
			{
				case 'X':
					flags |= PR_CA;
				// FALL THROUGH
// xxx - far pointers (%Fp, %Fn) not yet supported
			case 'x':
			case 'p':
			case 'n':
				radix = 16;
				goto DO_NUM;
			case 'd':
			case 'i':
				flags |= PR_SG;
				// FALL THROUGH
			case 'u':
				radix = 10;
				goto DO_NUM;
			case 'o':
				radix = 8;
// Load the value to be printed. l=long=32 bits:
DO_NUM:				if ( flags & PR_32 )
					num = va_arg( args, unsigned long );

// h=short=16 bits ( signed or unsigned )
				else if ( flags & PR_16 )
				{
					if ( flags & PR_SG )
						num = va_arg( args, short );
					else
						num = va_arg( args, unsigned short );
				}

// No h nor l: sizeof(int) bits ( signed or unsigned )
				else
				{
					if ( flags & PR_SG )
						num = va_arg( args, int );
					else
						num = va_arg( args, unsigned int );
				}
// Take care of sign
				if ( flags & PR_SG )
				{
					if ( num < 0 )
					{
						flags |= PR_WS;
						num = -num;
					}
				}

// Convert binary to octal/decimal/hex ASCII
// The math here is ALWAYS unsigned
				do
				{
					unsigned long temp;

					temp = ( unsigned long ) num % radix;
					where--;

					if ( temp < 10 )
						*where = temp + '0';

					else if ( flags & PR_CA )
						*where = temp - 10 + 'A';

					else
						*where = temp - 10 + 'a';

					num = ( unsigned long ) num / radix;
				}

				while ( num != 0 );
				goto EMIT;

			case 'c':
// Disallow pad-left-with-zeroes for %c
				flags &= ~PR_LZ;
				where--;

				*where = ( unsigned char )va_arg(args, unsigned char);
				actual_wd = 1;
				goto EMIT2;

			case 's':
// Disallow pad-left-with-zeroes for %s
				flags &= ~PR_LZ;
				where = va_arg( args, unsigned char * );
EMIT:
				actual_wd = strlen( ( char* )where );

				if ( flags & PR_WS )
					actual_wd++;

// If we pad left with ZEROES, do the sign now
				if ( ( flags & ( PR_WS | PR_LZ ) ) == ( PR_WS | PR_LZ ) )
				{
					fn( '-', &ptr );
					count++;
				}

// pad on left with spaces or zeroes ( for right justify )
EMIT2:				if ( ( flags & PR_LJ ) == 0 )
				{
					while ( given_wd > actual_wd )
					{
						fn( flags & PR_LZ ? '0' :
							' ', &ptr );
						count++;
						given_wd--;
					}
				}

// If we pad left with SPACES, do the sign now
				if ( ( flags & ( PR_WS | PR_LZ ) ) == PR_WS )
				{
					fn( '-' , &ptr );
					count++;
				}

// Emit string/char/converted number
				while ( *where != '\0' )
				{
					fn( *where++, &ptr );
					count++;
				}

// pad on right with spaces ( for left justify )
				if ( given_wd < actual_wd )
					given_wd = 0;

				else
					given_wd -= actual_wd;

				for ( ; given_wd; given_wd-- )
				{
					fn( ' ', &ptr );
					count++;
				}

				break;

			default:
				break;
			}
		default:
			state = flags = given_wd = 0;

			break;
		}
	}

	return count;
}
static int printfhelp( unsigned c, void **ptr )
{
	putch( c );

	return 0;
}
void kprintf( const char *msg, ... )
{
	va_list printfargs;

	va_start( printfargs, msg);

	( void ) doprintf ( msg, printfargs, printfhelp, NULL );

	va_end( printfargs );
}
void clear()
{
	// For checking how many times to put blanks
	unsigned int i;

	// Go back to the first place:
	gotoxy(0,0);

	for( i=0; i<scrwidth * scrheight;i++ )
		putch( ' ' );

	// Put the cursor back to the first place
	gotoxy(0,0);
}
extern unsigned char text;
extern unsigned char back;
extern unsigned char colour;
typedef enum _Colours {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, WHITE, DARKGRAY,
	BRIGHTBLUE, BRIGHTGREEN, BRIGHTCYAN, PINK, BRIGHTMAGENTA, YELLOW, BRIGHTWHITE} Colours;
void SetTextColour(int col)
{						//Sets the colour of the text being displayed
	text = (unsigned char)col;
	textattrib = (back << 4) | text;
}

void SetBackColour(int col)
{						//Sets the colour of the background being displayed
	if(col > 7)
	{
		col = BLACK;
	}
	back = (unsigned char)col;
	textattrib = (back << 4) | text;
}
void SetColour(Colours Text, Colours Back, bool blink)
{						//Sets the colour of the 
						//text being displayed
	SetTextColour(Text);
	SetBackColour(Back);
	if(blink)
	{
		colour = (back << 4) | text | 128;
	}
}
unsigned char GetTextColour()
{						//Sets the colour of the text currently set
	return (unsigned char)text;
}

unsigned char GetBackColour()
{						//returns the colour of the background currently set
	return (unsigned char)back;
}
