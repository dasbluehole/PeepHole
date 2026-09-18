/*
|=========================================================|
| This file is a part of NanoOS Copyright (C) 2008, 2012  |
| ashok.s.das@gmail.com                                   |
| Adopted here for PeepHole                               |
| ========================================================|
|  Physical Memory Manager                                |
|  Taken from :                                           |
|  License: LGPL, GPL-V2, GPL-V3 or latter                |
|=========================================================|
| adopted to  PeepHole (C) 2026 Ashok                     |
|=========================================================|
*/
/*
|=========================================================|
| PeepHole PMM - Physical Memory Manager |
| Based on 2008-12 NanoOS idea |
| Ashok  |
|=========================================================|
*/
#ifndef __PMM_H__
#define __PMM_H__

#include "types.h"

#define PAGE_SIZE 4096
#define MAX_MEM_SIZE (32 * 1024 * 1024) // Assume 32MB for now, later e820 from PIGGY
#define MAX_PAGES (MAX_MEM_SIZE / PAGE_SIZE) // 8192

// Bitmap: 1 = used, 0 = free
void pmm_init();
void* pmm_alloc_page();
void pmm_free_page(void* p);
unsigned int pmm_get_free_pages();
unsigned int pmm_get_total_pages();
void pmm_dump();

#endif

