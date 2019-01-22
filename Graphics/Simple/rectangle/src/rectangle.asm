;******************************************
; botloader code begin

[bits 16]           ; tell assembler that working in real mode(16 bit mode)
[org 0x7c00]        ; organize from 0x7C00 memory location where BIOS will load us


start:              ; start label from where our code starts


  xor ax,ax           ; set ax register to 0
  mov ds,ax           ; set data segment(ds) to 0
  mov es,ax           ; set extra segment(es) to 0

  ;******************************************
  ; jumping from real mode to protected mode
  ; by using disk interrupt
  mov ah, 0x02        ; load second stage to memory
  mov al, 0x10        ; numbers of sectors to read into memory
  mov dl, 0x80        ; sector read from fixed/usb disk
  mov ch, 0           ; cylinder number
  mov dh, 0           ; head number
  mov cl, 2           ; sector number
  mov bx, _start      ; load into es:bx segment :offset of buffer
  int 0x13            ; disk I/O interrupt

  ; before jumping clearing all interrupts
  cli

  ; jump to protected mode
  jmp _start                 ; jump to second stage

  times (510 - ($ - $$)) db 0x00     ;set 512 bytes for boot sector which are necessary
  dw 0xAA55                          ; boot signature 0xAA & 0x55


; bootloader code end

;******************************************
; x86 code begin 

_start:

main :

  ;******************************************
  ; set base pointer to heap
  ; and stack pointer to stack for functions and variables
  mov ebp, __HEAP__
  mov esp, __STACK__

  ;clear the screen
  mov ax,0x13
  int 0x10
  
  ; point extra segment register to the video memory address
  mov es, word[VGA_MEM]

  ;draw some rectangles
  mov ax, word[BRIGHT_GREEN]
  mov word[RECT_COLOR],ax
  mov dword[RECT_X], 10
  mov dword[RECT_Y], 20
  mov dword[RECT_WIDTH],100
  mov dword[RECT_HEIGHT], 60
  call draw_rectangle

  mov ax, word[BRIGHT_BLUE]
  mov word[RECT_COLOR],ax
  mov dword[RECT_X], 40
  mov dword[RECT_Y], 60
  mov dword[RECT_WIDTH],80
  mov dword[RECT_HEIGHT], 50
  call draw_rectangle

  mov ax, word[BRIGHT_MAGENTA]
  mov word[RECT_COLOR],ax
  mov dword[RECT_X], 100
  mov dword[RECT_Y], 30
  mov dword[RECT_WIDTH],180
  mov dword[RECT_HEIGHT], 60
  call draw_rectangle

  mov ax, word[BRIGHT_CYAN]
  mov word[RECT_COLOR],ax
  mov dword[RECT_X], 180
  mov dword[RECT_Y], 10
  mov dword[RECT_WIDTH], 60
  mov dword[RECT_HEIGHT], 100
  call draw_rectangle

  
  ;drawing multiple rectangles
  mov ecx, 1
  
  mov dword[RECT_X], 20
  mov dword[RECT_Y], 130
  mov dword[RECT_WIDTH], 140
  mov dword[RECT_HEIGHT], 50
  
.loop:
  cmp ecx, 15
  je .exit

  mov word[RECT_COLOR], cx

  push ecx
  call draw_rectangle
  pop ecx

  add dword[RECT_X], 10

  inc ecx
  jmp .loop
	
.exit:
  
  hlt


draw_rectangle:
  
  ;draw top of rectangle
  mov ax, word[RECT_COLOR]
  mov word[LINE_COLOR], ax

  mov eax, dword[RECT_X]
  mov dword[X1], eax
  mov ebx, dword[RECT_WIDTH]
  add ebx, dword[RECT_X]
  mov dword[X2], ebx
  mov ecx, dword[RECT_Y]
  mov dword[Y1], ecx
  mov dword[Y2], ecx
  call drawline
  
  ;draw bottom of rectangle
  mov eax, dword[RECT_X]
  mov dword[X1], eax
  mov ebx, dword[RECT_WIDTH]
  add ebx, dword[RECT_X]
  mov dword[X2], ebx
  mov ecx, dword[RECT_HEIGHT]
  add ecx, dword[RECT_Y]
  mov dword[Y1], ecx
  mov dword[Y2], ecx
  call drawline
  
  ;draw left of rectangle
  mov eax, dword[RECT_X]
  mov dword[X1], eax
  mov dword[X2], eax
  mov ebx, dword[RECT_Y]
  mov dword[Y1], ebx
  mov eax, dword[RECT_HEIGHT]
  add eax, dword[RECT_Y]
  mov dword[Y2], eax
  call drawline

  ;draw right of rectangle
  mov eax, dword[RECT_WIDTH]
  add eax, dword[RECT_X]
  mov dword[X1], eax
  mov dword[X2], eax
  mov ebx, dword[RECT_Y]
  mov dword[Y1], ebx
  mov eax, dword[RECT_HEIGHT]
  add eax, dword[RECT_Y]
  mov dword[Y2], eax
  call drawline
  
  ret
  
;*************************************************
;include the dda line drawing assembly code file 
%include "dda.asm"

;include the pixel drawing assembly code file 
%include "drawpixel.asm"


;******************************************
; set the required disk space we need
; the size of image(.bin file size)
  times (4096 - ($ - $$)) db 0x00
  
;read only data section
section .rodata
  __STACK__ dd 0x00FFFFFF
  __HEAP__ dd 0x00008C24

  ;video memory address
  ;for more see "Addressing details" section in following link
  ;https://en.wikipedia.org/wiki/Video_Graphics_Array
  ;In our kernel program we used 0xB8000 address
  VGA_MEM dw 0xA000
  VGA_WIDTH dw 320
  VGA_HEIGHT dw 219
  
  ;colors
  BLACK dw 0x00
  BLUE dw 0x01
  GREEN dw 0x02
  CYAN dw 0x03
  RED dw 0x04
  MAGENTA dw 0x05
  BROWN dw 0x06
  GRAY dw 0x07
  DARK_GRAY dw 0x08 
  BRIGHT_BLUE dw 0x09
  BRIGHT_GREEN dw 0xA
  BRIGHT_CYAN dw 0xB
  BRIGHT_RED dw 0xC
  BRIGHT_MAGENTA dw 0xD 
  YELLOW dw 0xE
  WHITE dw 0xF
  
section .bss
  ;line points(x1, y1, x2, y2)
  X1 resd 1
  Y1 resd 1
  X2 resd 1
  Y2 resd 1
  ;line co-ordinates (difference in points and its slope)
  _DX resd 1
  _DY resd 1
  _STEP resd 1
  
;data section
section .data
  ;line co-ordinates (x, y)
  _X dd 0
  _Y dd 0
  LINE_COLOR dw 0x0F
  X_PIX dd 0
  Y_PIX dd 0
  PIXEL_COLOR dw 0x0F
  RECT_X dd 0
  RECT_Y dd 0
  RECT_WIDTH dd 0
  RECT_HEIGHT dd 0
  RECT_COLOR dw 0x0F
