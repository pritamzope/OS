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

  ; disble cursor showing using BIOS interrupt
  call disable_cursor

  ; load vga
  call load_vga
  
  ; clear screen with blue background and white foreground
  mov byte[VGA_FORE_COLOR], 0xF
  mov byte[VGA_BACK_COLOR], 0x1
  call clear_screen

  ; print flags register
  mov si, flags_str
  call print_string

  xor eax, eax
  lahf
  mov word[BIN_PRINT_COUNT], 30
  call print_word

  ; print control register cr0
  add word[VGA_INDEX], 118

  mov si, cr0_str
  call print_string

  mov eax, cr0
  call print_word

  ; print segment registers, ss, ds, cs, es
  add word[VGA_INDEX], 98
  
  mov si, ss_str
  call print_string

  mov eax, ss
  call print_word

  add word[VGA_INDEX], 122
  
  mov si, ds_str
  call print_string

  mov eax, ds
  call print_word

  add word[VGA_INDEX], 124
  
  mov si, cs_str
  call print_string

  mov eax, cs
  call print_word

  add word[VGA_INDEX], 124
  
  mov si, es_str
  call print_string

  mov eax, es
  call print_word

  ; print pointers, esp, ebp
  add word[VGA_INDEX], 112
  
  mov si, esp_str
  call print_string

  mov eax, esp
  call print_word

  add word[VGA_INDEX], 110
  
  mov si, ebp_str
  call print_string

  mov eax, ebp
  call print_word

  ; print cpu info using cpuid instruction
  add word[VGA_INDEX], 282

  mov si, cpuid_0h_str
  call print_string

  add word[VGA_INDEX], 130
  
  mov si, cpuid_maxval_str
  call print_string

  mov eax, 0x0
  cpuid
  call print_word

  add word[VGA_INDEX], 46
  
  mov si, max_cpuid_genu_str
  call print_string

  mov eax, 0x0
  cpuid
  mov eax, ebx
  call print_word

  add word[VGA_INDEX], 148
  
  mov si, max_cpuid_ntel_str
  call print_string

  mov eax, 0x0
  cpuid
  mov eax, ecx
  call print_word

  add word[VGA_INDEX], 148
  
  mov si, max_cpuid_itel_str
  call print_string

  mov eax, 0x0
  cpuid
  mov eax, edx
  call print_word


  add word[VGA_INDEX], 304
  
  mov si, cpuid_1h_str
  call print_string

  add word[VGA_INDEX], 130
  
  mov si, cpuid_version_info
  call print_string

  mov eax, 0x1
  cpuid
  call print_word

  add word[VGA_INDEX], 60
  
  mov si, cpuid_1h_ebx
  call print_string

  mov eax, 0x1
  cpuid
  mov eax, ebx
  call print_word

  add word[VGA_INDEX], 138
  
  mov si, cpuid_1h_ecx
  call print_string

  mov eax, 0x1
  cpuid
  mov eax, ecx
  call print_word

  add word[VGA_INDEX], 150
  
  mov si, cpuid_1h_edx
  call print_string

  mov eax, 0x1
  cpuid
  mov eax, edx
  call print_word

  ; print time stamp counter(cpu time), edx:eax
  add word[VGA_INDEX], 304
  
  mov si, tsc_str
  call print_string

  xor eax, eax
  xor edx, edx
  rdtsc

  push eax
  mov eax, edx

  call print_word

  add word[VGA_INDEX], 4
  mov si, colon
  call print_string

  pop eax
  call print_word

  
  add word[VGA_INDEX], 280
  mov word[VGA_FORE_COLOR], 0xE
  mov word[VGA_BACK_COLOR], 0x1
  mov si, note_str
  call print_string

.exit:

  hlt
  ret


print_word:
  cmp eax, 0
  je .zero_word

  add word[VGA_INDEX], 18
  mov ecx, eax

.pw_loop:
  cmp ecx, 0
  je .exit

  xor edx, edx
  xor eax, eax
  mov eax, ecx
  mov ebx, 10
  div ebx

  mov ecx, eax

  add dl, '0'
  mov byte[VGA_CHAR], dl
  call vga_print_char
  sub word[VGA_INDEX], 2

  jmp .pw_loop

.zero_word:

  mov byte[VGA_CHAR], '0'
  call vga_print_char
  sub word[VGA_INDEX], 2

.exit:
  ret

; works fine on qemu
; but generates infinite loop problem
; on actual hardware
print_binary:
  cmp eax, 0
  je .exit_zero_value

  push word[VGA_INDEX]
  mov cx, word[BIN_PRINT_COUNT]
  add word[VGA_INDEX], cx

.pb_loop:
  cmp eax, 0
  je .exit

  mov ebx, eax
  and ebx, 1
  test bl, bl
  je .not_one

  push eax
  mov byte[VGA_CHAR], '1'
  call vga_print_char
  sub word[VGA_INDEX], 2
  pop eax

  sar eax, 1
  jmp .pb_loop

.not_one:
  push eax
  mov byte[VGA_CHAR], '0'
  call vga_print_char
  sub word[VGA_INDEX], 2
  pop eax

  sar eax, 1
  jmp .pb_loop

.exit_zero_value:
  push eax
  mov byte[VGA_CHAR], '0'
  call vga_print_char
  sub word[VGA_INDEX], 2
  pop eax

.exit:
  pop bx

.zeros_loop:
  push eax
  mov byte[VGA_CHAR], '0'
  call vga_print_char
  sub word[VGA_INDEX], 2
  pop eax

  cmp word[VGA_INDEX], bx
  jge .zeros_loop

  mov word[VGA_INDEX], 0
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

load_vga:
  ; get vga memory address and point set es to it
  mov dx, [VGA_ADDRESS]
  mov es, dx
  ret

vga_print_char:
  mov di, word[VGA_INDEX]
  mov al, byte[VGA_CHAR]

  mov ah, byte[VGA_BACK_COLOR]
  sal ah, 4
  or ah, byte[VGA_FORE_COLOR]

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


disable_cursor:
  mov ah, 0x01
  mov ch, 1
  mov cl, 0
  int 0x10
  ret


  times (4096 - ($ - $$)) db 0x00

section .rodata
  VGA_ADDRESS dw 0xB800
  MAX_VGA_INDEX dw 0xF9E    ; 3998 in decimal

section .data
  VGA_INDEX dw 0
  VGA_CHAR db 0
  VGA_FORE_COLOR db 0xF
  VGA_BACK_COLOR db 0x0
  BIN_PRINT_COUNT dw 62

  flags_str db 'FLAGS Register : ', 0
  cr0_str db 'Control Register(CR0) : ', 0
  ss_str db 'Stack Segment(SS) : ', 0
  ds_str db 'Data Segment(DS) : ', 0
  cs_str db 'Code Segment(CS) : ', 0
  es_str db 'Extra Segment(ES) : ', 0
  esp_str db 'Stack Pointer(ESP) : ', 0
  ebp_str db 'Base Pointer(EBP) : ', 0
  
  cpuid_0h_str db 'CPUID EAX = 0H : ', 0
  cpuid_maxval_str db 'Maximum Input Value for Basic CPUID Information : ', 0
  max_cpuid_genu_str db 'Genu : ', 0
  max_cpuid_ntel_str db 'ntel : ', 0
  max_cpuid_itel_str db 'itel : ', 0

  cpuid_1h_str db 'CPUID EAX = 1H : ', 0
  cpuid_version_info db 'Version Information(EAX)(Type,Family,Model) :', 0
  cpuid_1h_ebx db 'EBX : ', 0
  cpuid_1h_ecx db 'ECX : ', 0
  cpuid_1h_edx db 'EDX : ', 0

  tsc_str db 'Time Stamp Counter : ', 0
  colon db ' : ', 0

  note_str db '[ Note : Above All values are in decimal ]', 0

	
	
