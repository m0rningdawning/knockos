[BITS 32]

; assmebly section first (from ld)
; section .asm

CODE_SEG equ 0x08
DATA_SEG equ 0x10
global _start

_start:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov ebp, 0x00200000
    mov esp, ebp

    ; Fast A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    jmp $

times 512 - ($ - $$) db 0 ; 512 empty bytes to fix potential allignment issues 
