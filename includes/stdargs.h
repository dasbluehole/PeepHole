/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  stdargs defination                                     |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef _STDARG_
#define _STDARG_

// if your wondering where LASTARG is defined, I think the compile does it.
#define	va_start( AP, LASTARG )	\
	( AP = ( ( va_list ) & ( LASTARG ) + VA_SIZE( LASTARG ) ) )

// same with TYPE
#define va_arg( AP, TYPE )						\
 ( AP += __va_rounded_size ( TYPE ),					\
  * ( ( TYPE * ) ( AP - __va_rounded_size ( TYPE ) ) ) )

#define __va_rounded_size( TYPE )  \
  ( ( ( sizeof ( TYPE ) + sizeof ( int ) - 1 ) / sizeof ( int ) ) * sizeof ( int ) )

#define	VA_SIZE( TYPE )					\
	( ( sizeof( TYPE ) + sizeof( STACKITEM ) - 1 )	\
		& ~( sizeof( STACKITEM ) - 1 ) )

// StackItem is defined here:
#define	STACKITEM	int

#define va_end(AP)	

// typedefs:
typedef unsigned char *va_list;


#endif
