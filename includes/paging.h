#ifndef PAGING_H
#define PAGING_H
#include "stdint.h"
#include "idt.h"
#define PAGE_SIZE 4096
#define PAGE_DIR_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

// flags
#define PAGE_PRESENT 0x1
#define PAGE_WRITE 0x2
#define PAGE_USER 0x4

extern uint32_t page_directory[1024] __attribute__((aligned(4096)));
extern uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_init_identity();
void paging_load(uint32_t *dir);
void paging_enable();
void page_fault_handler(regs *r); // you already have regs

#endif

