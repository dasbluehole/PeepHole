/*
|=========================================================|
| This file is a part of PeepHole Copyright (C) 2026      |
| ashok.s.das@gmail.com                                   |
| ========================================================|
|  Kernel implementation                                  |
|  Taken from :                                           |
|  License: LGPL, GPL-V2, GPL-V3                          |
|=========================================================|
|Adopted to PeepHole (C) 2026 Ashok                       |
|=========================================================|
*/
#include "vga.h"
#include "pmm.h"
#include "paging.h"
#include "low-io.h"
#include "idt.h"
#include "vmm.h"

extern void _fault_handler(regs *r);
extern IDTR my_idtr;

extern uint32_t page_directory[1024]; // from paging.c


void kernel_main(void) {
    __asm__ volatile("cli"); // STOP timer interrupts

    init_VGA(); clear();
    kprintf("\tPeepHole/32 Kernel at 0x100000 - Phase 2 OK!\n");
    pmm_init(); pmm_dump();

    setup_IDT();
    irq_install(); // IDT is now in place

    paging_init_identity();
    paging_enable(); // your inline version is fine
    kprintf("PAGING ON dir=0x%x tab=0x%x\n", page_directory, first_page_table);
    kprintf("IDT base 0x%x\n", my_idtr.base);

    kprintf("Mapping 0xE0000000...\n");

	uint32_t val ;//= *(volatile uint32_t*)0xE0000000;
	
	//kprintf("Read back from E0000000: 0x%x - SUCCESS!\n", val);
	map_range(0xE0000000, 0x1000, PAGE_PRESENT|PAGE_WRITE);
	// write @ 0xE0000000 value 0x12345678
    *(uint32_t*)0xE0000000 = 0x12345678;
    val = *(uint32_t*)0xE0000000;
	if(val == 0x12345678) {
	    kprintf("Paging test PASSED!\n");
	} else {
	    kprintf("Paging test FAILED!\n");
	}
    while(1) __asm__ volatile("hlt");
    kprintf("SHOULD NEVER REACH HERE\n");
}


