[bits 16]
[org 0x7c00]

bootloader_entry:
  xor ax,ax
  mov ds,ax
  mov es,ax

  mov ah, 0x02
  mov al, 0x10
  mov dl, 0x80
  mov ch, 0
  mov dh, 0
  mov cl, 2
  mov bx, kernel_entry
  int 0x13

  cli
  cld

  jmp kernel_entry

  times (510 - ($ - $$)) db 0x00
  dw 0xAA55


kernel_entry:

  call disable_cursor

  call load_vga

  ; clear screen with black background and white foreground
  ; colors
  ;0x0  black
  ;0x1	blue
  ;0x2  green
  ;0x3  cyan
  ;0x4  red
  ;0x5	magenta
  ;0x6  brown
  ;0x7  gray
  ;0x8  dark gray
  ;0x9  bright blue
  ;0xA  bright green
  ;0xB  bright cyan
  ;0xC  bright red
  ;0xD  bright magenta
  ;0xE  yellow
  ;0xF  white
  mov byte[VGA_FORE_COLOR], 0xE
  mov byte[VGA_BACK_COLOR], 0x4
  call clear_screen


  ; increase VGA_INDEX to some 2 bytes amount
  ; so that it could lead to center of the screen
  add word[VGA_INDEX], 1824

  mov si, msg_str
  call print_string


  hlt
  ret


print_string:

.char_loop :
	lodsb

	cmp al, 0
	je .exit
	
  mov byte[VGA_CHAR], al
  call vga_print_char
  
  add word[VGA_INDEX], 2
	
  jmp .char_loop

.exit:
	ret


disable_cursor:
  mov ah, 0x01
  mov ch, 1
  mov cl, 0
  int 0x10
  ret


load_vga:
  ; get vga memory address and point set es to it
  mov dx, [VGA_ADDRESS]
  mov es, dx
  ret


vga_print_char:
  mov di, word[VGA_INDEX]
  mov al, byte[VGA_CHAR]

  ; 1 byte fore and back color
  ; high 4 bits : back color
  ; low 4 bits : fore color

  ; get back color
  mov ah, byte[VGA_BACK_COLOR]
  ; rotate it by 4 to left,
  ; for high 4 bits
  sal ah, 4
  ; oring with fore color value
  or ah, byte[VGA_FORE_COLOR]

  ; see kernel.c source, from Kernel folder

  mov [es:di], ax

  ret


; clear screen just displays null characters
clear_screen:
  ;set index to 0
  mov di, 0

.clrloop:
  cmp di, word[MAX_VGA_INDEX]
  jg .exit

  ; get null character into al
  mov al, 0

  mov ah, byte[VGA_BACK_COLOR]
  sal ah, 4
  or ah, byte[VGA_FORE_COLOR]

  ; put value of al into vga memory address
  mov [es:di], ax

  ; increase di by 2 (2 bytes)
  add di, 2
  jmp .clrloop

.exit:
  mov word[VGA_INDEX], 0
  ret


  times (4096 - ($ - $$)) db 0x00

section .data
  VGA_ADDRESS dw 0xB800
  MAX_VGA_INDEX dw 0xF9E    ; 3998 in decimal
  VGA_INDEX dw 0
  VGA_CHAR db 0
  VGA_FORE_COLOR db 0xF
  VGA_BACK_COLOR db 0x0

section .rodata
  msg_str db 'Hello World!', 0
	
	
	
