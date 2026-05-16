ENTRY(_start)
SECTIONS
{
    . = 0x1000;
    .text : {
        *(.text.prologue)
        *(.text)
    }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
    /DISCARD/ : { *(.comment) *(.note*) }
}
