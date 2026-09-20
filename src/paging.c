#include "paging.h"
#include "pmm.h"
#include "stdio.h"


__attribute__((aligned(4096)))
uint32_t page_directory[1024];

__attribute__((aligned(4096)))
uint32_t first_page_table[1024];

void paging_init_identity() {
    for (int i = 0; i < 1024; i++) page_directory[i] = 0x00000002;

    for (int i = 0; i < 1024; i++)
        first_page_table[i] = (i * 4096) | 0x3; // P + W

    // DIR 0x104000 -> TAB 0x105000 you saw - both <4MB so phys = virt
    page_directory[0] = ((uint32_t)first_page_table) | 0x3;
}

// --- inline Intel asm in C ---

void paging_load(uint32_t *dir) {
    __asm__ volatile (
        ".intel_syntax noprefix\n"
        "mov cr3, %0\n"
        ".att_syntax prefix\n"
        :: "r"(dir) : "memory"
    );
}

void paging_enable_only() {
    __asm__ volatile (
        ".intel_syntax noprefix\n"
        "mov eax, cr0\n"
        "or eax, 0x80000000\n"
        "mov cr0, eax\n"
        ".att_syntax prefix\n"
        ::: "eax", "memory"
    );
}

void paging_enable() {
    paging_load(page_directory);
    paging_enable_only();
    kprintf("PAGING: ON dir=0x%x tab=0x%x\n", page_directory, first_page_table);
}