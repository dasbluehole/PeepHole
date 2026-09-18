/*
|========================================================|
| This file is a part of TinyOS Copyright (C) 2008, 2012 |
| ashok.s.das@gmail.com                                  |
| Adopted here for TinyOS                                |
| =======================================================|
|  physical memory manager                               |
|  Taken from :                                          |
|  License: LGPL, GPL-V2 or latter                       |
|========================================================|
|Adopted to PeepHole (C) 2026 Ashok                      |
|========================================================|
*/
#include "pmm.h"
#include "vga.h"

extern unsigned int _kernel_end; // from linker.ld

static unsigned int* bitmap = 0;
static unsigned int total_pages = 0;
static unsigned int free_pages = 0;
static unsigned int bitmap_pages = 0;

#define BITMAP_SET(i) (bitmap[(i)/32] |= (1 << ((i)%32)))
#define BITMAP_CLEAR(i) (bitmap[(i)/32] &= ~(1 << ((i)%32)))
#define BITMAP_TEST(i) (bitmap[(i)/32] & (1 << ((i)%32)))

void pmm_init() {
    // Place bitmap right after kernel, aligned to 4K
    // For safety, put at 2MB (0x200000) - well after kernel
    bitmap = (unsigned int*)0x200000;

    total_pages = MAX_PAGES;
    unsigned int bitmap_size = (total_pages / 32) * 4; // 8192/32*4 = 1024 bytes
    bitmap_pages = (bitmap_size + PAGE_SIZE -1) / PAGE_SIZE;
    if(bitmap_pages == 0) bitmap_pages = 1;

    // Clear bitmap -> all free initially
    for(unsigned int i=0; i < total_pages/32; i++) bitmap[i] = 0;

    free_pages = total_pages;

    // Mark 0x0 to 0x200000 + bitmap as USED (low mem + kernel + bitmap)
    // 0x200000 = 512 pages, + bitmap pages
    unsigned int reserved_pages = (0x200000 / PAGE_SIZE) + bitmap_pages; // 512 + 1 = 513
    for(unsigned int i=0; i < reserved_pages; i++) {
        BITMAP_SET(i);
        free_pages--;
    }

    kprintf("PMM: bitmap at 0x%x, size %d bytes (%d pages)\n", bitmap, bitmap_size, bitmap_pages);
    kprintf("PMM: total %d pages, reserved %d, free %d\n", total_pages, reserved_pages, free_pages);
}

void* pmm_alloc_page() {
    for(unsigned int i=0; i < total_pages; i++) {
        if(!BITMAP_TEST(i)) {
            BITMAP_SET(i);
            free_pages--;
            return (void*)(i * PAGE_SIZE);
        }
    }
    return 0; // out of memory
}

void pmm_free_page(void* p) {
    unsigned int page = (unsigned int)p / PAGE_SIZE;
    if(page >= total_pages) return;
    if(BITMAP_TEST(page)) {
        BITMAP_CLEAR(page);
        free_pages++;
    }
}

unsigned int pmm_get_free_pages() { return free_pages; }
unsigned int pmm_get_total_pages() { return total_pages; }

void pmm_dump() {
    kprintf("PMM dump: total=%d free=%d used=%d\n", total_pages, free_pages, total_pages - free_pages);
}

