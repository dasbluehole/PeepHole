;~ |==========================================================|
;~ | This file is a part of PeepHole Copyright (C) 2008, 2012 |
;~ | ashok.s.das@gmail.com                                    |
;~ | =========================================================|
;~ | Kernel Loader it loads KERNEL.BIN                        | 
;~ |  Taken from :                                            |
;~ |  License: LGPL, GPL-V2, GPL-V3 or latter                 |
;~ |==========================================================|
;~ |Adopted to PeepHole (C) 2026 Ashok                        |
;~ |==========================================================|
; PeepHole/32 Phase 2 - Final clean loader
; Build: nasm -f bin phase2_loader.asm -o PIGGY.COM
; Needs D:\KERNEL.BIN in share

org 0x100
bits 16

start:
    mov dx, msg_loading
    mov ah, 9
    int 0x21

    mov dx, filename
    mov al, 0
    mov ah, 0x3D
    int 0x21
    jc open_error
    mov [file_handle], ax

    ; Read to 0x8000:0x0000 = phys 0x80000
    mov bx, [file_handle]
    push ds
    mov ax, 0x8000
    mov ds, ax
    xor dx, dx
    mov cx, 32768
    mov ah, 0x3F
    int 0x21
    mov [cs:kernel_size], ax
    pop ds
    jc read_error

    mov ah, 0x3E
    mov bx, [file_handle]
    int 0x21

    mov dx, msg_ok
    mov ah, 9
    int 0x21

    cli
    xor eax, eax
    mov ax, ds
    shl eax, 4
    mov ebx, eax

    mov eax, ebx
    add eax, gdt_start
    mov [gdt_desc+2], eax

    mov eax, ebx
    add eax, pmode
    mov [jump_ptr], eax

    lgdt [gdt_desc]
    call enable_a20

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    o32 jmp far [cs:jump_ptr]

bits 32
pmode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x90000

    ; ECX = kernel size in bytes
    movzx ecx, word [ebx + kernel_size]
    add ecx, 3
    shr ecx, 2

    mov esi, 0x80000
    mov edi, 0x100000
    rep movsd

    ; Jump to kernel
    jmp 0x08:0x100000

    cli
    hlt

bits 16
enable_a20:
    mov ax, 0x2401
    int 0x15
    in al, 0x92
    or al, 2
    out 0x92, al
    ret

open_error:
    mov dx, err_open
    mov ah, 9
    int 0x21
    ret
read_error:
    pop ds
    mov dx, err_read
    mov ah, 9
    int 0x21
    ret

filename db 'D:\KERNEL.BIN',0
msg_loading db 'PIGGY Phase2: Loading KERNEL.BIN...',13,10,'$'
msg_ok db 'PIGGY: Loaded, copying to 1MB...',13,10,'$'
err_open db 'ERR: D:\KERNEL.BIN not found',13,10,'$'
err_read db 'ERR: read failed',13,10,'$'

file_handle dw 0
kernel_size dw 0

align 8
gdt_start:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_end:
gdt_desc:
    dw gdt_end - gdt_start - 1
    dd 0
jump_ptr:
    dd 0
    dw 0x08
