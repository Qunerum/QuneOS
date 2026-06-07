ENTRY(_start)
SECTIONS
{
    . = 0x1000;
    .text : {
        *kernel_entry.o(.text)
        *(.text)
        *(.rodata*)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
    /DISCARD/ : { *(.comment) *(.note*) }
}
