[bits 32]
global _start
extern _start_c         ; Nasza nowa nazwa głównej funkcji w main.c

_start:
    ; Upewniamy się, że segmenty są idealnie ustawione
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Ustawiamy bezpieczny stos dla jądra C
    mov ebp, 0x90000
    mov esp, ebp

    ; Wywołujemy właściwy kod w C
    call _start_c

    ; Jeśli jądro kiedykolwiek by wróciło, zatrzymaj procesor
    cli
    hlt
    jmp $
