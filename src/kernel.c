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

void kernel_main(void) {
    init_VGA();
    clear();
    SetBackColour(BRIGHTWHITE);
    SetTextColour(BRIGHTBLUE);
    
    SetTextColour(YELLOW);
    kprintf("PeepHole/32 Kernel at 0x100000 - Phase 2 OK!\n");
    SetTextColour(GREEN);
    kprintf("PIGGY loaded me from D:\\KERNEL.BIN - you are in C now!\n");
    
    SetTextColour(BRIGHTBLUE);
    kprintf("\t****Hello PeepHole OS-32 in Protected mode******\n");
    kprintf("\t*****************Keep Patience******************\n");
    kprintf("\t================================================\n");

    SetTextColour(GREEN);
    pmm_init();
    pmm_dump();

    void* p1 = pmm_alloc_page();
    void* p2 = pmm_alloc_page();
    kprintf("Allocated p1=0x%x p2=0x%x\n", p1, p2);
    kprintf("Free now: %d\n", pmm_get_free_pages());

    pmm_free_page(p1);
    kprintf("After free p1, free=%d\n", pmm_get_free_pages());

    while(1){ __asm__ volatile("hlt"); } // as we are a kernel we must not end ;D so run infinitely
}

