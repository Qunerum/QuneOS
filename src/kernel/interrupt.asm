[bits 32]

global idt_load
global keyboard_handler_asm
extern keyboard_handler_c

idt_load:
    mov edx, [esp + 4]
    lidt [edx]
    ret

keyboard_handler_asm:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_handler_c

    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    iretd
