ORG 0x7c00 ; start from the beginning of the address space
BITS 16 ; real mode 16 bits

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; start from jmp short to actual start routine to free up
; space to fake the bpb for hardware bios differences
_start:
    jmp short start
    nop

times 33 db 0 ; fake bios parameter block 33 bits

start:
    jmp 0: continue_start ; continue the start procedure

continue_start:
    ; clear ints then set registers so that the boot
    ; loader origin is set correctly
    cli
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    ; clear stack segment then set the stack
    ; pointer to bootloader space
    mov ss, ax
    mov sp, 0x7c00
    sti ; set ints

.load_protected:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32

; Global Descriptor Table
gdt_start:

gtd_null:
    ; nullify first 64 bits
    dd 0x0
    dd 0x0

; CS, offset 0x8, default values
gdt_code:
    dw 0xffff ; First 0-15 bits segment limit
    dw 0 ; Base first 0-15 bits
    db 0 ; Base 16-23 bits
    db 0x9a ; Access byte
    db 11001111b ; High and low 4 bit flags
    db 0 ; 24-31 bits

; DS, SS, ES, FS, GS, offset 0x10, default values
gdt_data:
    dw 0xffff ; First 0-15 bits segment limit
    dw 0 ; Base first 0-15 bits
    db 0 ; Base 16-23 bits
    db 0x92 ; Access byte
    db 11001111b ; High and low 4 bit flags
    db 0 ; 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[BITS 32]
load32:
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

times 510 - ($ - $$) db 0 ; fill the rest of the bytes with 0
dw 0xAA55 ; signature

