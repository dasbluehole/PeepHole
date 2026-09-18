/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  IDT defination                                      |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#ifndef __IDT_H__
#define __IDT_H__ 
typedef struct IDT_entry
	{
		unsigned short base_low;
		unsigned short sel;
		unsigned char unused;
		unsigned char flags;
		unsigned short base_high;
	} __attribute__((packed)) IDT_entry;
typedef struct IDTR
	{
		unsigned short limit;
		unsigned int base;
	} __attribute__((packed)) IDTR;
typedef struct regs
	{	
		unsigned long gs, fs, es, ds;
		unsigned long edi, esi, ebp, esp, ebx, edx, ecx, eax;
		unsigned long int_no, err_code;
		unsigned long eip, cs, eflags;//, useresp, ss;	
	} regs;




typedef void (*isrfunc_t)(regs *);
void IDT_set_gate(unsigned char num, isrfunc_t func, unsigned short segment, unsigned char flags);
void set_handler(int intnum, isrfunc_t function);
void setup_IDT();

// IRQ stuff from irq.c
/* This is a blank function pointer */
typedef void (*handler_t)(regs *r);
void irq_install_handler(int irq, handler_t my_handler);
void irq_install();

#endif

