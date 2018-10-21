[bits 16]           ; tell assembler that working in real mode(16 bit mode)
[org 0x7c00]        ; organize from 0x7C00 memory location where BIOS will load us


start:              ; start label from where our code starts


	xor ax,ax           ; set ax register to 0
	mov ds,ax           ; set data segment(ds) to 0
	mov es,ax           ; set extra segment(es) to 0
	mov bx,0x8000

	mov ax,0x13         ;clears the screen
	int 0x10            ;call bios video interrupt


	mov ah,02           ;clear the screen with big font
	int 0x10            ;interrupt display




	;set cursor to specific position on screen
	mov ah,0x02         ; set value for change to cursor position
	mov bh,0x00         ; page
	mov dh,0x06         ; y cordinate/row
	mov dl,0x09         ; x cordinate/col
	int 0x10


	mov si, start_os_intro              ; point start_os_intro string to source index
	call _print_DiffColor_String        ; call print different color string function


	;set cursor to specific position on screen
	mov ah,0x02
	mov bh,0x00
	mov dh,0x10
	mov dl,0x06
	int 0x10


	mov si,press_key                    ; point press_key string to source index
	call _print_GreenColor_String       ; call print green color string function



	mov ax,0x00         ; get keyboard input
	int 0x16            ; interrupt for hold & read input



	;/////////////////////////////////////////////////////////////
	; load second sector into memory

	mov ah, 0x02                    ; load second stage to memory
	mov al, 1                       ; numbers of sectors to read into memory
	mov dl, 0x80                    ; sector read from fixed/usb disk
	mov ch, 0                       ; cylinder number
	mov dh, 0                       ; head number
	mov cl, 2                       ; sector number
	mov bx, _OS_Stage_2             ; load into es:bx segment :offset of buffer
	int 0x13                        ; disk I/O interrupt

	jmp _OS_Stage_2                 ; jump to second stage




	;/////////////////////////////////////////////////////////////
	; declaring string datas here
	start_os_intro db 'Welcome to My OS!',0
	press_key db '>>>> Press any key <<<<',0

	login_username db 'Username : ',0
	login_password db 'Password : ',0
    
	display_text db '! Welcome to my Operating System !', 0

	os_info db 10, 'My Operating System, 16-Bit, version=1.0.0',13,0

	press_key_2 db 10,'Press any key to go to graphics view',0

	window_text db 10,'Graphics in OS......', 0
	hello_world_text db 10,10, '    Hello World!',0
	login_label db '#] Login please....(ESC to skip login)', 0




	;/////////////////////////////////////////////////////////////
	; defining printing string functions here

	;****** print string without color

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




	;****** print string with different colors

_print_DiffColor_String:
    	mov bl,1	    	    ;color value
	mov ah, 0x0E

.repeat_next_char:
	lodsb
	cmp al, 0
	je .done_print
	add bl,6               ;increase color value by 6
	int 0x10
	jmp .repeat_next_char

.done_print:
	ret



	;****** print string with green color

_print_GreenColor_String:
	mov bl,10
	mov ah, 0x0E

.repeat_next_char:
	lodsb
	cmp al, 0
	je .done_print
	int 0x10
	jmp .repeat_next_char

.done_print:
	ret


	;****** print string with white color

_print_WhiteColor_String:
	mov bl,15
	mov ah, 0x0E

.repeat_next_char:
	lodsb
	cmp al, 0
	je .done_print
	int 0x10
	jmp .repeat_next_char

.done_print:
	ret


	;****** print string with yellow color

_print_YellowColor_String:
	mov bl,14
	mov ah, 0x0E

.repeat_next_char:
	lodsb
	cmp al, 0
	je .done_print
	int 0x10
	jmp .repeat_next_char

.done_print:
	ret


	;///////////////////////////////////////////
	; boot loader magic number
	times ((0x200 - 2) - ($ - $$)) db 0x00     ;set 512 bytes for boot sector which are necessary
	dw 0xAA55                           	   ; boot signature 0xAA & 0x55





;////////////////////////////////////////////////////////////////////////////////////////

_OS_Stage_2 :

	mov al,2                    ; set font to normal mode
	mov ah,0                    ; clear the screen
	int 0x10                    ; call video interrupt

	mov cx,0                    ; initialize counter(cx) to get input


	;***** print login_label on screen
	;set cursor to specific position on screen
	mov ah,0x02
	mov bh,0x00
	mov dh,0x00
	mov dl,0x00
	int 0x10

	mov si,login_label              ; point si to login_username
	call print_string               ; display it on screen


	;****** read username

	;set cursor to specific position on screen
	mov ah,0x02
	mov bh,0x00
	mov dh,0x02
	mov dl,0x00
	int 0x10

	mov si,login_username          ; point si to login_username
	call print_string              ; display it on screen



_getUsernameinput:

	mov ax,0x00             ; get keyboard input
	int 0x16		        ; hold for input

	cmp ah,0x1C             ; compare input is enter(1C) or not
	je .exitinput           ; if enter then jump to exitinput

	cmp ah,0x01             ; compare input is escape(01) or not
	je _skipLogin           ; jump to _skipLogin

	mov ah,0x0E             ;display input char
	int 0x10

	inc cx                  ; increase counter
	cmp cx,5                ; compare counter reached to 5
	jbe _getUsernameinput   ; yes jump to _getUsernameinput
	jmp .inputdone          ; else jump to inputdone

.inputdone:
	mov cx,0                ; set counter to 0
	jmp _getUsernameinput   ; jump to _getUsernameinput
	ret                     ; return

.exitinput:
	hlt



	;****** read password

	;set x y position to text
	mov ah,0x02
	mov bh,0x00
	mov dh,0x03
	mov dl,0x00
	int 0x10


	mov si,login_password               ; point si to login_username
	call print_string                   ; display it on screen


_getPasswordinput:

	mov ax,0x00
	int 0x16

	cmp ah,0x1C
	je .exitinput
    
	cmp ah,0x01
	je _skipLogin

	inc cx

	cmp cx,5
	jbe _getPasswordinput
    
	jmp .inputdone

.inputdone:
	mov cx,0
	jmp _getPasswordinput
	ret
.exitinput:
	hlt




;****** display display_text on screen

	;set x y position to text
	mov ah,0x02
	mov bh,0x00
	mov dh,0x08
	mov dl,0x12
	int 0x10


	mov si, display_text		;display display_text on screen
	call print_string


	;set x y position to text
	mov ah,0x02
	mov bh,0x00
	mov dh,0x9
	mov dl,0x10
	int 0x10

	mov si, os_info		;display os_info on screen
	call print_string

	;set x y position to text
	mov ah,0x02
	mov bh,0x00
	mov dh,0x11
	mov dl,0x11
	int 0x10

	mov si, press_key_2		;display press_key_2 on screen
	call print_string

	mov ah,0x00
	int 0x16


;//////////////////////////////////////////////////////////////////


_skipLogin:
    

	;/////////////////////////////////////////////////////////////
	; load third sector into memory

	mov ah, 0x03                    ; load third stage to memory
	mov al, 1
	mov dl, 0x80
	mov ch, 0
	mov dh, 0
	mov cl, 3                       ; sector number 3
	mov bx, _OS_Stage_3
	int 0x13

	jmp _OS_Stage_3



;////////////////////////////////////////////////////////////////////////////////////////


_OS_Stage_3:


	mov ax,0x13              ; clears the screen
	int 0x10



;//////////////////////////////////////////////////////////
; drawing window with lines


	push 0x0A000                ; video memory graphics segment
	pop es                      ; pop any extar segments from stack
	xor di,di                   ; set destination index to 0
	xor ax,ax                   ; set color register to zero



	;//////////////////////////////////////////////
	;******drawing top line of our window
	mov ax,0x02                 ; set color to green

	mov dx,0                    ; initialize counter(dx) to 0

	add di,320                  ; add di to 320(next line)
	imul di,10                  ;multiply by 10 to di to set y cordinate from where we need to start drawing

	add di,10                   ;set x cordinate of line from where to be drawn


_topLine_perPixel_Loop:


	mov [es:di],ax              ; move value ax to memory location es:di

	inc di                      ; increment di for next pixel
	inc dx                      ; increment our counter
	cmp dx,300                  ; comprae counter value with 300
	jbe _topLine_perPixel_Loop  ; if <= 300 jump to _topLine_perPixel_Loop

	hlt                         ; halt process after drawing


	;//////////////////////////////////////////////
	;******drawing bottm line of our window
	xor dx,dx
	xor di,di
	add di,320
	imul di,190         ; set y cordinate for line to be drawn
	add di,10           ;set x cordinate of line to be drawn

	mov ax,0x01         ; blue color

_bottmLine_perPixel_Loop:

	mov [es:di],ax

	inc di
	inc dx
	cmp dx,300
	jbe _bottmLine_perPixel_Loop
	hlt



	;//////////////////////////////////////////////
	;******drawing left line of our window
	xor dx,dx
	xor di,di
	add di,320
	imul di,10           ; set y cordinate for line to be drawn

	add di,10            ; set x cordinate for line to be drawn

	mov ax,0x03          ; cyan color

_leftLine_perPixel_Loop:

	mov [es:di],ax

	inc dx
	add di,320
	cmp dx,180
	jbe _leftLine_perPixel_Loop

	hlt 


	;//////////////////////////////////////////////
	;******drawing right line of our window
	xor dx,dx
	xor di,di
	add di,320
	imul di,10           ; set y cordinate for line to be drawn

	add di,310           ; set x cordinate for line to be drawn

	mov ax,0x06          ; orange color

_rightLine_perPixel_Loop:

	mov [es:di],ax

	inc dx
	add di,320
	cmp dx,180
	jbe _rightLine_perPixel_Loop

	hlt



	;//////////////////////////////////////////////
	;******drawing line below top line of our window
	xor dx,dx
	xor di,di

	add di,320
	imul di,27           ; set y cordinate for line to be drawn

	add di,11            ; set x cordinate for line to be drawn

	mov ax,0x05         ; pink color

_belowLineTopLine_perPixel_Loop:


	mov [es:di],ax

	inc di
	inc dx
	cmp dx,298
	jbe _belowLineTopLine_perPixel_Loop

	hlt 



	;***** print window_text & X char

	;set cursor to specific position
	mov ah,0x02
	mov bh,0x00
	mov dh,0x01         ; y cordinate
	mov dl,0x02         ; x cordinate
	int 0x10

	mov si,window_text              ; point si to window_text
	call _print_YellowColor_String

	hlt



	;set cursor to specific position
	mov ah,0x02
	mov bh,0x00
	mov dh,0x02           ; y cordinate
	mov dl,0x25           ; x cordinate
	int 0x10

	mov ah,0x0E
	mov al,0x58           ; 0x58=X
	mov bh,0x00
	mov bl,4              ; red color
	int 0x10

	hlt

	;set cursor to specific position
	mov ah,0x02
	mov bh,0x00
	mov dh,0x02           ; y cordinate
	mov dl,0x23           ; x cordinate
	int 0x10

	mov ah,0x0E
	mov al,0x5F           ; 0x58=X
	mov bh,0x00
	mov bl,9              ; red color
	int 0x10

	hlt


	;set cursor to specific position
	mov ah,0x02
	mov bh,0x00
	mov dh,0x05   ; y cordinate
	mov dl,0x09    ; x cordinate
	int 0x10

	mov si,hello_world_text
	call _print_DiffColor_String

	hlt



	;set cursor to specific position
	mov ah,0x02
	mov bh,0x00
	mov dh,0x12   ; y cordinate
	mov dl,0x03  ; x cordinate
	int 0x10

	mov si,display_text
	call _print_WhiteColor_String

	hlt


	; add how much memory we need
	times (1024 - ($-$$)) db 0x00
    
