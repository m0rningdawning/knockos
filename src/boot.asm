ORG 0 ; start from the beginning of the address space
BITS 16 ; real mode 16 bits

; start from jmp short to actual start routine to free up
; space to fake the bpb for hardware bios differences
_start:
    jmp short start
    nop

; prints 0
; by default int 0 is "divide by zero exception". Because of
; that, this int will be called when division by zero occures,
; even though the said interrupt is now custom
int_zero:
    mov ah, 0eh
    mov al, '0'
    mov bx, 0x00
    int 0x10
    iret

times 33 db 0 ; fake bios parameter block 33 bits

start:
    jmp 0x7c0: continue_start ; continue the start procedure

continue_start:
    ; clear ints then set registers so that the boot
    ; loader origin is set correctly
    cli
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    ; clear stack segment then set the stack
    ; pointer to bootloader space
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c00
    sti ; set ints

    ; make custom int 0 and call it 4 fun
    mov word[ss:0x00], int_zero
    mov word[ss:0x02], 0x7c0
    int 0

    mov si, done_message
    call print
    jmp $ ; so that we don't reach the boot signature

done_message: db 'Booting finished.', 0

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

times 510 - ($ - $$) db 0 ; fill the rest of the bytes with 0
dw 0xAA55 ; signature

