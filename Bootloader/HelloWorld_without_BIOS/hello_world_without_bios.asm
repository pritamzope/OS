[bits 16]           ; tell assembler that working in real mode(16 bit mode)
[org 0x7c00]        ; organize from 0x7C00 memory location where BIOS will load us


start:              ; start label from where our code starts


    xor ax,ax           ; set ax register to 0
    mov ds,ax           ; set data segment(ds) to 0
    mov es,ax           ; set extra segment(es) to 0
    mov bx,0x8000

    ; get vga memory address and point set es to it
    mov dx, [VGA_MEM]
    mov es, dx

    call clear_screen

    mov si, hello_world
    call print_string

    ret

print_string:

.repeat_next_char:
    lodsb                       ; get character from string into al
    cmp al, 0                   ; cmp al with end of string
    je .done_print              ; if char is zero, end of string

    mov di, word[VGA_INDEX]     ; otherwise, print it
    mov [es:di], al
    add word[VGA_INDEX], 2      ; increase VGA_INDEX by 2(for more see kernel code in C)

    jmp .repeat_next_char       ; jmp to .repeat_next_char if not 0

.done_print:
	ret

; clear screen just displays null characters
clear_screen:
    ;set index to 0
    mov di, 0

.clrloop:
    cmp di, word[MAX_VGA_INDEX]
    je .exit

    ; get null character into al
    mov al, 0

    ; put value of al into vga memory address
    mov [es:di], al

    ; increase di by 2 (2 bytes)
    add di, 2
    jmp .clrloop

.exit:
    ret

    ;*********************************************
    ; declare data (for more see kernel code in C)
    VGA_MEM dw 0xB800
    MAX_VGA_INDEX dw 0xF9E    ; 3998 in decimal
    VGA_INDEX dw 0x00

    hello_world db  'Hello World!',0
    ;*********************************************


    times (510 - ($ - $$)) db 0x00     ;set 512 bytes for boot sector which are necessary
    dw 0xAA55                          ; boot signature 0xAA & 0x55

