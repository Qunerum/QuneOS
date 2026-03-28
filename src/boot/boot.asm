MBALIGN  equ  1 << 0
MEMINFO  equ  1 << 1
FLAGS    equ  MBALIGN | MEMINFO
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
global _start:function (_start.end - _start)
extern kmain

_start:
    ; Ustawiamy stos (Stack Pointer), bo C go potrzebuje!
    mov esp, stack_top

    push ebx ; Opcjonalnie: adres informacji multiboot od bootloadera
    call kmain

    cli
.hang:
    hlt
    jmp .hang
_start.end:

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KB stosu
stack_top:
