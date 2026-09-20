#include "vmm.h"

extern uint32_t page_directory[1024];
void* pmm_alloc_page();
void pmm_free_page(void* p);

void map_page(uint32_t virt, uint32_t phys, uint32_t flags) {
    uint32_t dir = virt >> 22;
    uint32_t tbl = (virt >> 12) & 0x3FF;
    uint32_t pd = page_directory[dir];
    uint32_t *table;

    if(!(pd & PAGE_PRESENT)) {
        uint32_t new_tbl = (uint32_t)pmm_alloc_page();
        uint32_t *p = (uint32_t*)new_tbl;
        for(int i=0;i<1024;i++) p[i]=0;
        page_directory[dir] = new_tbl | PAGE_PRESENT | PAGE_WRITE;
        table = p;
    } else {
        table = (uint32_t*)(pd & 0xFFFFF000);
    }
    table[tbl] = (phys & 0xFFFFF000) | (flags & 0xFFF) | PAGE_PRESENT;
    __asm__ volatile(
        ".intel_syntax noprefix; invlpg dword ptr [%0];.att_syntax prefix"
        :: "r"(virt) : "memory"
    );
}

void map_range(uint32_t virt, uint32_t size, uint32_t flags) {
    for(uint32_t o=0;o<size;o+=0x1000)
        map_page(virt+o, (uint32_t)pmm_alloc_page(), flags);
}

void unmap_page(uint32_t virt) {
    uint32_t dir = virt >> 22;
    uint32_t tbl = (virt >> 12) & 0x3FF;
    if(!(page_directory[dir] & PAGE_PRESENT)) return;
    uint32_t *table = (uint32_t*)(page_directory[dir] & 0xFFFFF000);
    if(!(table[tbl] & PAGE_PRESENT)) return;
    uint32_t phys = table[tbl] & 0xFFFFF000;
    table[tbl]=0;
    pmm_free_page((void*)phys);
    __asm__ volatile(
        ".intel_syntax noprefix; invlpg dword ptr [%0];.att_syntax prefix"
        :: "r"(virt) : "memory"
    );
}

void unmap_range(uint32_t virt, uint32_t size) {
    for(uint32_t o=0;o<size;o+=0x1000) unmap_page(virt+o);
}

uint32_t get_phys(uint32_t virt) {
    uint32_t dir = virt >> 22;
    uint32_t tbl = (virt >> 12) & 0x3FF;
    uint32_t pd = page_directory[dir];
    if(!(pd & PAGE_PRESENT)) return 0;
    uint32_t *table = (uint32_t*)(pd & 0xFFFFF000);
    uint32_t pte = table[tbl];
    if(!(pte & PAGE_PRESENT)) return 0;
    return (pte & 0xFFFFF000) + (virt & 0xFFF);
}
