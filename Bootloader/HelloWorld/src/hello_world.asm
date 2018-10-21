[bits 16]           ; tell assembler that working in real mode(16 bit mode)
[org 0x7c00]        ; organize from 0x7C00 memory location where BIOS will load us


start:              ; start label from where our code starts


	xor ax,ax           ; set ax register to 0
	mov ds,ax           ; set data segment(ds) to 0
	mov es,ax           ; set extra segment(es) to 0
	mov bx,0x8000


	mov si, hello_world              ; point hello_world to source index
	call print_string				       ; call print different color string function



	hello_world db  'Hello World!',13,0


print_string:
	mov ah, 0x0E            ; value to tell interrupt handler that take value from al & print it

.repeat_next_char:
	lodsb   			 ; get character from string
	cmp al, 0             		 ; cmp al with end of string
	je .done_print		    	 ; if char is zero, end of string
	int 0x10                	 ; otherwise, print it
	jmp .repeat_next_char   	 ; jmp to .repeat_next_char if not 0

.done_print:
	ret                 	    ;return

	times (510 - ($ - $$)) db 0x00     ;set 512 bytes for boot sector which are necessary
	dw 0xAA55                           						   ; boot signature 0xAA & 0x55
	
	
	