[bits 32]

global idt_load
global keyboard_handler_asm
global mouse_handler_asm

extern keyboard_handler_c
extern mouse_handler_c

idt_load:
    mov edx, [esp + 4]
    lidt [edx]
    ret

keyboard_handler_asm:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_handler_c

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iretd

mouse_handler_asm:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call mouse_handler_c

    pop gs
    pop fs
    pop es
    pop ds
    popa
    iretd
