[bits 16]
[org 0x7c00]

_start:

	xor ax,ax
	mov ds,ax
	mov es,ax
	mov bx,0x8000

	mov al, 2	; set font size, 1 or 0(big), 2(normal)
	mov ah, 0        ; clear the screen
	int 0x10        ; call video interrupt


	;*********************************************************************

	;set cursor to specific position on center of screen
	mov ah, 0x02	; set register ah value to trigger BIOS for changing position, 
	mov bh, 0x00
			;(al already has 2 for font size(line 11))
	mov dl, 0x22	; x co-ordinate
	mov dh, 0x0A	; y co-ordinate
	int 0x10

	mov si, str_demo
	call print_string

	;*********************************************************************


	hlt

	str_demo db 'Hello World', 0

print_string:
	mov ah, 0x0E   ; value to tell interrupt handler that take value from al & print it

.repeat_next_char:
	lodsb   			 ; get character from string
	cmp al, 0             		 ; cmp al with end of string
	je .exit		    	 ; if char is zero, end of string
	int 0x10                	 ; otherwise, print it
	jmp .repeat_next_char   	 ; jmp to .repeat_next_char if not 0

.exit:
	ret


	;**************************************************************
	; boot loader magic number
	times (510 - ($ - $$)) db 0x00     ;set 512 bytes for boot sector which are necessary
	dw 0xAA55                          ; boot signature 0xAA & 0x55


