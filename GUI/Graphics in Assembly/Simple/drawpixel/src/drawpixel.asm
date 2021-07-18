;****Graphics Pixel drawing algorithm****

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
  
  ;assign co-ordinates to X and YELLOW
  ;and color to PIXEL_COLOR
  ;call drawpixel procedure
  mov word[X],50
  mov word[Y],60
  mov ax, word[WHITE]
  mov word[PIXEL_COLOR], ax
  call drawpixel

  ;lets draw another pixels
  mov word[X],100
  mov word[Y],60
  mov ax, word[RED]
  mov word[PIXEL_COLOR], ax
  call drawpixel

  mov word[X],120
  mov word[Y],120
  mov ax, word[BRIGHT_CYAN]
  mov word[PIXEL_COLOR], ax
  call drawpixel
  
  mov word[X],140
  mov word[Y],30
  mov ax, word[YELLOW]
  mov word[PIXEL_COLOR], ax
  call drawpixel

  hlt


;draw a pixel at x,y location
drawpixel :

  ;get x co-ordinate
  mov ax, word[X]
  
  ;get y co-ordinate
  mov bx, word[Y]
  
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
  mov ax, word[PIXEL_COLOR]
  
  ;plot the pixel with index(di) and pixel color(ax)
  mov [es:di], ax
 
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
  
;data section
section .data
  X dd 0
  Y dd 0
  PIXEL_COLOR dw 0x0F
