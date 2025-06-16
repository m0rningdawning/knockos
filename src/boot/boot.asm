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
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA

    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Master drive selection
    mov dx, 0x1F6
    out dx, al
    ; Finished sending

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending

    ; Send more bits of the lba
    mov eax, ebx ; Restore the lba
    mov dx, 0x1F3
    out dx, al 
    ; Finished sending

    ; Send more bits of the lba
    mov dx, 0x1F4
    mov eax, ebx ; Restore the lba
    shr eax, 8
    out dx, al
    ; Finished sending

    ; Send upper 16 bits of the lba
    mov dx, 0x1F5
    mov eax, ebx ; Restore the lba
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1F7
    in al, dx
    test al, 8
    jz .try_again

    ; We need to read 256 at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors
    ret

times 510 - ($ - $$) db 0 ; fill the rest of the bytes with 0
dw 0xAA55 ; signature

