/*
|=========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for TinyOS                                 |
| ========================================================|
|  IRQ implementation                                     |
|  Taken from :                                           |
|  License: LGPL, GPL-V2 or latter                        |
|=========================================================|
*/
#include "idt.h"
#include "low-io.h"
#include "stdio.h"
/* These are own ISRs that point to our special IRQ handler
*  instead of the regular 'fault_handler' function */
extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();

//extern void kprintf(const char *fmt,...);

//static int timer_ticks;
/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};




/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, handler_t my_handler)
{
    irq_routines[irq] = my_handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 */
void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers */
void irq_install()
{
	irq_remap();

	IDT_set_gate(32, (isrfunc_t)_irq0, 0x08, 0x8E);
	IDT_set_gate(33, (isrfunc_t)_irq1, 0x08, 0x8E);
	IDT_set_gate(34, (isrfunc_t)_irq2, 0x08, 0x8E);
	IDT_set_gate(35, (isrfunc_t)_irq3, 0x08, 0x8E);
	IDT_set_gate(36, (isrfunc_t)_irq4, 0x08, 0x8E);
	IDT_set_gate(37, (isrfunc_t)_irq5, 0x08, 0x8E);
	IDT_set_gate(38, (isrfunc_t)_irq6, 0x08, 0x8E);
	IDT_set_gate(39, (isrfunc_t)_irq7, 0x08, 0x8E);
	IDT_set_gate(40, (isrfunc_t)_irq8, 0x08, 0x8E);
	IDT_set_gate(41, (isrfunc_t)_irq9, 0x08, 0x8E);
	IDT_set_gate(42, (isrfunc_t)_irq10, 0x08, 0x8E);
	IDT_set_gate(43, (isrfunc_t)_irq11, 0x08, 0x8E);
	IDT_set_gate(44, (isrfunc_t)_irq12, 0x08, 0x8E);
	IDT_set_gate(45, (isrfunc_t)_irq13, 0x08, 0x8E);
	IDT_set_gate(46, (isrfunc_t)_irq14, 0x08, 0x8E);
	IDT_set_gate(47, (isrfunc_t)_irq15, 0x08, 0x8E);
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs */
void _irq_handler(regs *r)
{
    handler_t my_handler;

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    my_handler = irq_routines[r->int_no - 32];
    if (my_handler)
    {
        my_handler(r);
    }
    else
	{
		kprintf("No handlers for IRQ %u installed\n",r->int_no-32);
	}
    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    outportb(0x20, 0x20);
}

void dump_irq_routines()
{
	int i;
	for (i=0;i<16;i++)
	{
		if(irq_routines[i]!=0)
		kprintf("irq %d \t",i);
	}
	kprintf("\n");
}
/*void timer_handler(regs *r)
{
	if(r->int_no==32)
	kprintf("IRQ 0\n");
}
void myIRQ1_handler(regs *r)
{
	if(r->int_no==33)
	kprintf("IRQ 1\n");
}
void myIRQ2_handler(regs *r)
{
	if(r->int_no==34)
	kprintf("IRQ 2\n");
}
void myIRQ3_handler(regs *r)
{
	if(r->int_no==35)
	kprintf("IRQ 3\n");
}
void myIRQ4_handler(regs *r)
{
	if(r->int_no==36)
	kprintf("IRQ 4\n");
}
void myIRQ5_handler(regs *r)
{
	if(r->int_no==37)
	kprintf("IRQ 5\n");
}
void myIRQ6_handler(regs *r)
{
	if(r->int_no==38)
	kprintf("IRQ 6\n");
}
void myIRQ7_handler(regs *r)
{
	if(r->int_no==39)
	kprintf("IRQ 7\n");
}
void myIRQ8_handler(regs *r)
{
	if(r->int_no==40)
	kprintf("IRQ 8\n");
}
void myIRQ9_handler(regs *r)
{
	if(r->int_no==41)
	kprintf("IRQ 9\n");
}
void myIRQ10_handler(regs *r)
{
	if(r->int_no==42)
	kprintf("IRQ 10\n");
}
void myIRQ11_handler(regs *r)
{
	if(r->int_no==43)
	kprintf("IRQ 11\n");
}
void myIRQ12_handler(regs *r)
{
	if(r->int_no==44)
	kprintf("IRQ 12\n");
}
void myIRQ13_handler(regs *r)
{
	if(r->int_no==45)
	kprintf("IRQ 13\n");
}
void myIRQ14_handler(regs *r)
{
	if(r->int_no==46)
	kprintf("IRQ 14\n");
}
void myIRQ15_handler(regs *r)
{
	if(r->int_no==47)
	kprintf("IRQ 15\n");
}
void install_timer()
{
	irq_install_handler(0,myIRQ0_handler);
} */
/*irq_install_handler(0,myIRQ0);*/
/*void install_others()
{
irq_install_handler(1,myIRQ1_handler);
irq_install_handler(2,myIRQ2_handler);
irq_install_handler(3,myIRQ3_handler);
irq_install_handler(4,myIRQ4_handler);
irq_install_handler(5,myIRQ5_handler);
irq_install_handler(6,myIRQ6_handler);
irq_install_handler(7,myIRQ7_handler);
irq_install_handler(8,myIRQ8_handler);
irq_install_handler(9,myIRQ9_handler);
irq_install_handler(10,myIRQ10_handler);
irq_install_handler(11,myIRQ11_handler);
irq_install_handler(12,myIRQ12_handler);
irq_install_handler(13,myIRQ13_handler);
irq_install_handler(14,myIRQ14_handler);
irq_install_handler(15,myIRQ15_handler);
}
*/
