[org 0x7c00]
KERNEL_OFFSET equ 0x1000
MODE_INFO_ADDR equ 0x8000

    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    mov [BOOT_DRIVE], dl

    mov edi, MODE_INFO_ADDR
    xor al, al
    mov cx, 256
    rep stosb

    mov ax, 0x4f01
    mov cx, 0x4118
    mov di, MODE_INFO_ADDR
    int 0x10

    xor ax, ax
    mov dl, [BOOT_DRIVE]
    int 0x13

    mov ah, 0x02
    mov al, 50 ; Sectors
    mov bx, KERNEL_OFFSET
    mov cl, 0x02
    mov ch, 0x00
    mov dh, 0x00
    mov dl, [BOOT_DRIVE]
    int 0x13
    jc disk_error

    mov ax, 0x4f02
    mov bx, 0x4118
    int 0x10

    cmp al, 0x4f
    jne vesa_error

    cli
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:init_pm

disk_error:
    mov ah, 0x0e
    mov al, 'D'
    int 0x10
    jmp $

vesa_error:
    mov ah, 0x0e
    mov al, 'V'
    int 0x10
    jmp $

[bits 32]
init_pm:
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp

    mov ebx, MODE_INFO_ADDR
    call KERNEL_OFFSET
    jmp $

align 16
gdt_start:
    dd 0x0, 0x0
gdt_code:
    dw 0xffff, 0x0, 0x9a00, 0x00cf
gdt_data:
    dw 0xffff, 0x0, 0x9200, 0x00cf
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

BOOT_DRIVE db 0
times 510-($-$$) db 0
dw 0xaa55
