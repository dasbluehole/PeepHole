
#include "vga.h"

void kernel_main(void) {
    init_VGA();
    clear();
    SetBackColour(BRIGHTWHITE);
    SetTextColour(BRIGHTBLUE);
    
    SetTextColour(YELLOW);
    kprintf("NanoOS/32 Kernel at 0x100000 - Phase 2 OK!\n");
    SetTextColour(GREEN);
    kprintf("PIGGY loaded me from D:\\KERNEL.BIN - you are in C now!\n");
    
    SetTextColour(BRIGHTBLUE);
    kprintf("\t****Hello PeepHole OS-32 in Protected mode******\n");
    kprintf("\t*****************Keep Patience******************\n");
    kprintf("\t================================================\n");

    while(1){ __asm__ volatile("hlt"); } // as we are a kernel we must not end ;D so run infinitely
}
