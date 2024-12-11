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

    ; int 13/ah=02h sector(s) -> memory
    mov ah, 02h ; read sector command
    mov al, 1 ; one sector to read
    mov ch, 0 ; low 8 bits of cylinder number
    mov cl, 2 ; sector 2
    mov dh, 0 ; head 0
    mov bx, buffer ; empty buffer to bx
    int 0x13 ; call int x13
    jc error ; if carry flag set jump to error
    mov si, buffer ; print the read sector
    call print

    mov si, done_message
    call print

    jmp $ ; so that we don't reach the boot signature

done_message: db 'Booting finished.', 0
error_load_sector: db 'Failed to load sector!'

error:
    mov si, error_load_sector
    call print
    jmp $

print:
    mov bx, 0 ; page num
.loop:
    lodsb ; load byte address(message) to al
    cmp al, 0 ; char to al then while not 0 teletype_out
    je .done
    call teletype_out
    cmp al, 0x0A ; check if the character is a newline
    jne .loop
    call newline ; handle newline
    jmp .loop
.done:
    ret

newline:
    mov ah, 0eh
    mov al, 0x0D ; carriage return \r
    int 0x10
    mov al, 0x0A ; line feed \n
    int 0x10
    ret

; teletype output interrupt 10 exec routine
teletype_out:
    mov ah, 0eh
    ; mov bl, E
    int 0x10
    ret

times 510 - ($ - $$) db 0 ; fill the rest of the bytes with 0
dw 0xAA55 ; signature

buffer: ; empty buffer for int 13
