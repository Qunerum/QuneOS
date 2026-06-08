[bits 32]
global _start
extern _start_c

_start:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call _start_c

    cli
    hlt
    jmp $
