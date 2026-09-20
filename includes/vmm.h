//vmm.h
#ifndef _VMM_H_
#define _VMM_H_
#include "stdint.h"

#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

void map_page(uint32_t virt, uint32_t phys, uint32_t flags);
void map_range(uint32_t virt, uint32_t size, uint32_t flags);
void unmap_page(uint32_t virt);
void unmap_range(uint32_t virt, uint32_t size);
uint32_t get_phys(uint32_t virt);

#endif
