ORG 0 ; start from the beginning of the address space
BITS 16 ; real mode 16 bits

jmp 0x7c0: start

start:
    cli ; clear ints then set registers so that the boot loader origin is set correctly
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00 ; clear stack segment then set the stack pointer to bootloader space
    mov ss, ax
    mov sp, 0x7c00
    sti ; set ints
    mov si, message
    call print
    jmp $ ; so that we don't reach the boot signature

print:
    mov bx, 0 ; page num
.loop:
    lodsb ; load byte address(message) to al
    cmp al, 0 ; char to al then while not 0 teletype_out
    je .done
    call teletype_out
    jmp .loop
.done:
    ret

; teletype output interrupt 10 exec routine
teletype_out:
    mov ah, 0eh
    ; mov bl, E
    int 0x10
    ret

message: db 'Hello World!', 0

times 510 - ($ - $$) db 0 ; fill the rest of the bytes with 0
dw 0xAA55 ; signature
