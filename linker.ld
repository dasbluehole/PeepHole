ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);  /* R + X */
    data PT_LOAD FLAGS(6);  /* R + W */
}

SECTIONS
{
    . = 0x100000;

    .text : {
        *(.text)
    } :text

    .rodata : {
        *(.rodata)
    } :text

    .data : {
        *(.data)
    } :data

    .bss : {
        *(.bss)
    } :data

    . = ALIGN(4096);
    _kernel_end = .;
}

