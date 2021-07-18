;****Digital Differential Analyzer(DDA) Line Drawing algorithm****

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
  
  ;draw a red colored line
  mov ax, word[RED]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 20
  mov dword[Y1], 20
  mov dword[X2], 100
  mov dword[Y2], 100
  call drawline

  ; the algorithm is modified to draw
  ;horizontal and vertical lines by checking
  ;whether either X or y points are equal or not
  ;draw a bright green colored horizontal line
  mov ax, word[BRIGHT_GREEN]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 80
  mov dword[Y1], 40
  mov dword[X2], 200
  mov dword[Y2], 40
  call drawline

  
  ;draw a bright cyan colored vertical line
  mov ax, word[BRIGHT_CYAN]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 160
  mov dword[Y1], 80
  mov dword[X2], 160
  mov dword[Y2], 160
  call drawline
  
  ;draw a line that is disadvantage of DDA algorithm
  mov ax, word[YELLOW]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 200
  mov dword[Y1], 80
  mov dword[X2], 250
  mov dword[Y2], 200
  call drawline
  
  mov ax, word[WHITE]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 250
  mov dword[Y1], 30
  mov dword[X2], 280
  mov dword[Y2], 180
  call drawline
  
  ;drawing opposite line
  ;which is impossible to draw using DDA algorithm
  ;because here _DY = Y2 - Y1 = 120 - 150 = -30
  ;and algorithm only works by increasing steps(_STEP(here ecx register) by its 
  ;differential values(_DX,_DY) and not by decreasing steps
  ;so this line wont be drawn
  mov ax, word[BRIGHT_MAGENTA]
  mov word[LINE_COLOR], ax
  
  mov dword[X1], 30
  mov dword[Y1], 150
  mov dword[X2], 100
  mov dword[Y2], 120
  call drawline

  hlt


drawline:
	
  ;calculate difference of x points
  mov ecx, dword[X2]
  sub ecx, dword[X1]
  mov dword[_DX], ecx

  ;calculate difference of y points
  mov ecx, dword[Y2]
  sub ecx, dword[Y1]
  mov dword[_DY], ecx
	
  ; check if Y1 & Y2 are equal
  mov eax, dword[Y1]
  mov ebx, dword[Y2]
  cmp eax, ebx
  je .y_equals

  ; check if X1 & X2 are equal
  mov eax, dword[X1]
  mov ebx, dword[X2]
  cmp eax, ebx
  je .x_equals
	
  ;check if _DX <= _DY
  mov ecx, dword[_DX]
  cmp ecx, dword[_DY]
  jle .dx_is_less
	
.x_equals:
  ;is X1 & X2 are equal _STEP to _DY
  mov ecx, dword[_DY]
  mov dword[_STEP], ecx

  jmp .done

.y_equals:
  ;if Y1 & Y2 are equal then goto next

.dx_is_less:
  ;if _DX is greater than _DY or Y1 & Y2 are equal then 
  ;set _STEP to _DX
  mov ecx, dword[_DX]
  mov dword[_STEP], ecx
	
.done:

  ; _DX = _DX / _STEP
  xor edx, edx
  mov eax, dword[_DX]
  mov ebx, dword[_STEP]
  div ebx
  mov dword[_DX], eax

  ; _DY = _DY / _STEP
  xor edx, edx
  mov eax, dword[_DY]
  mov ebx, dword[_STEP]
  div ebx
  mov dword[_DY], eax

  ;set co-ordinate _X = X1
  mov eax, dword[X1]
  mov dword[_X], eax

  ;set co-ordinate _Y = Y1
  mov eax, dword[Y1]
  mov dword[_Y], eax

  ;set counter ecx to _STEP
  mov ecx, 0

.line_loop:

  ;stop when ecx > _STEP
  cmp ecx, dword[_STEP]
  jg .exit
  
  ;get x co-ordinate
  mov eax, dword[_X]
  
  ;get y co-ordinate
  mov ebx, dword[_Y]
  
  ;clear destination index register for use as index in 
  ;video memory pointed by es register
  xor di, di

  ;set y co-ordinate
  ;first increase video index by width of the current graphics mode(320*220)
  ;multiply index by y co-ordinate to set its location 
  ;this is same as printing a new line in our kernel C code
  ;https://github.com/pritamzope/OS/tree/master/Kernel/Simple/src/kernel_3
  add di, word[WIDTH]
  imul di, bx

  ; set x co-ordinate by increasing index(di) by X value
  add di, ax

  ;get the color from PIXEL_COLOR memory
  mov ax, word[LINE_COLOR]
  
  ;plot the pixel with index(di) and pixel color(ax)
  mov [es:di], ax

  ;increase _X by _DX
  mov eax, dword[_X]
  add eax, dword[_DX]
  mov dword[_X], eax

  ;increase _Y by _DY
  mov eax, dword[_Y]
  add eax, dword[_DY]
  mov dword[_Y], eax

  ;increase _STEP
  inc ecx

  ;and continue loop
  jmp .line_loop 

.exit:
	ret
	

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
  WIDTH dw 320
  HEIGHT dw 219
  
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
