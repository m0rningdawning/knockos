[BITS 32]

; assmebly section first (from ld)
; section .asm

CODE_SEG equ 0x08
DATA_SEG equ 0x10

global _start
extern kernel_main

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Fast A20  line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; Remap the master PIC
    mov al, 00010001b
    out 0x20, al

    mov al, 0x20 ; Master ISR starts at 0x20
    out 0x21, al

    mov al, 00000001b
    out 0x21, al
    ; End remap master PIC

    call kernel_main
    jmp $

times 512 - ($ - $$) db 0 ; 512 empty bytes to fix potential allignment issues
