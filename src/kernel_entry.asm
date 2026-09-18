;~ |==========================================================|
;~ | This file is a part of PeepHole Copyright (C) 2008, 2012 |
;~ | ashok.s.das@gmail.com                                    |
;~ | =========================================================|
;~ | Kernel Entry code hopefully last assembly code in kernel | 
;~ |  Taken from :                                            |
;~ |  License: LGPL, GPL-V2, GPL-V3 or latter                 |
;~ |==========================================================|
;~ |Adopted to PeepHole (C) 2026 Ashok                        |
;~ |==========================================================|
; kernel_entry.asm - entry point at 0x100000
bits 32
global _start
extern kernel_main

section .text
_start:
    ; We are already in protected mode, DS=0x10, stack at 0x90000 from loader
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    call kernel_main
.hang:
    cli
    hlt
    jmp .hang
