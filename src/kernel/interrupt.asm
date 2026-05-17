[bits 32]

global idt_load
global keyboard_handler_asm
extern keyboard_handler_c

idt_load:
    mov edx, [esp + 4]  ; Pobierz wskaźnik na strukturę idt_ptr
    lidt [edx]          ; Załaduj tablicę IDT
    ret

keyboard_handler_asm:
    pusha               ; Zapisz wszystkie rejestry ogólnego przeznaczenia (EAX, ECX, EDX...)
    mov ax, ds          ; Zapisz segment danych
    push eax

    mov ax, 0x10        ; Załaduj bezpieczny segment danych jądra
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call keyboard_handler_c ; Wywołaj właściwy sterownik napisany w C

    pop eax             ; Przywróć poprzednie segmenty
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa                ; Przywróć rejestry
    iretd               ; Powrót z przerwania chronionego (Interrupt Return)
