;******************************************
; botloader code begin

[bits 16]           ; tell assembler that working in real mode(16 bit mode)
[org 0x7c00]        ; organize from 0x7C00 memory location where BIOS will load us


start:              ; start label from where our code starts


  xor ax,ax           ; set ax register to 0
  mov ds,ax           ; set data segment(ds) to 0
  mov es,ax           ; set extra segment(es) to 0

  ;******************************************
  ; jumping from rel mode to protected mode
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

  ; clear the screen
  call clear_screen

  ; set cursor mosition
  mov dword[X], 25
  mov dword[Y], 0
  push dword[X]
  push dword[Y]
  call goto_xy
  add esp, 8

  ; print os message
  mov esi, os_msg
  call print_string

  call print_new_line

  call print_new_line

  ; display the calculator menu
  call display_menu

;*************************************************
; this loop continue until valid coice is selected
.read_loop:

  ; read choice
  call read_single_key

  ; assign choice to choice variable
  mov ah, byte[KEYCODE]
  mov byte[choice], ah

  ; if other entry is entered continue to loop
  call read_single_key

  mov ah, byte[KEYCODE]
  cmp ah, byte[DELETE_KEY]
  je main

  ; if enter key is pressed jump to perform_calulation
  mov ah, byte[KEYCODE]
  cmp ah, byte[ENTER_KEY]
  je perform_operation

  jmp .read_loop

  ret

;******************************************
; this function displays calc string menu
display_menu :
  push ebp

  mov esi, calc_msg
  call print_string

  call print_new_line
  call print_new_line

  mov esi, menu_str
  call print_string  

  call print_new_line

  mov esi, addition_menu
  call print_string  

  call print_new_line

  mov esi, substraction_menu
  call print_string  

  call print_new_line

  mov esi, multiplication_menu
  call print_string  

  call print_new_line

  mov esi, division_menu
  call print_string  

  call print_new_line

  mov esi, modulus_menu
  call print_string  

  call print_new_line

  mov esi, logicaland_menu
  call print_string  

  call print_new_line

  mov esi, logicalor_menu
  call print_string  

  call print_new_line

  mov esi, clear_screen_menu
  call print_string  

  call print_new_line
  call print_new_line

  mov esi, select_choice_str
  call print_string  

  pop ebp
  ret

;***********************************************
; this function read bytes of data from keyboard
; by converting it into dword
perform_operation :

  ; if choice = clear screen(8)
  ; get choice
  mov al, byte[choice]
  cmp al, byte[KEYCODE_8]
  je perform_clear_screen

  call print_new_line

  ; read first number bytes
  mov esi, enter_first_number
  call print_string  

  call read_data

  ; check all read bytes are numerics in BUFFER
  lea ebx, [BUFFER]
  push ebx
  call check_all_bytes_are_numeric
  add esp, 4

  cmp eax, 0
  je .not_numerics

  ; convert bytes array into dword
  lea ebx, [BUFFER]
  push ebx
  call bytes_length
  add esp, 4

  push ebx
  push eax
  call byte_array_to_dword
  add esp, 8

  ; assign read dword to number_1
  mov dword[number_1], eax

  ; read second number
  call print_new_line

  mov esi, enter_second_number
  call print_string  

  call read_data

  ; check all read bytes are numerics in BUFFER
  lea ebx, [BUFFER]
  push ebx
  call check_all_bytes_are_numeric
  add esp, 4

  cmp eax, 0
  je .not_numerics

  ; convert bytes array into dword
  lea ebx, [BUFFER]
  push ebx
  call bytes_length
  add esp, 4

  push ebx
  push eax
  call byte_array_to_dword
  add esp, 8

  ; assign read dword to number_2
  mov dword[number_2], eax

  ; once reading numbers done, 
  ; jump to operation by choice function
  jmp perform_operation_by_choice

; when not numerics then display eror message
; and reload screen
.not_numerics :
  call print_new_line

  mov esi, error_msg
  call print_string

  call print_new_line

  call reload_screen

  ret

;****************************************************
; this function check the entered choice by keycodes,
; such as addition, substraction and jump to
; that specific procedure
perform_operation_by_choice :

  ; get choice
  mov al, byte[choice]

  ; if addition
  cmp al, byte[KEYCODE_1]
  je perform_addition

  ; if substraction
  cmp al, byte[KEYCODE_2]
  je perform_substraction

  ; if multiplication
  cmp al, byte[KEYCODE_3]
  je perform_multiplication

  ; if division
  cmp al, byte[KEYCODE_4]
  je perform_division

  ; if modulus
  cmp al, byte[KEYCODE_5]
  je perform_modulus

  ; if logical and
  cmp al, byte[KEYCODE_6]
  je perform_logical_and

  ; if logical or
  cmp al, byte[KEYCODE_7]
  je perform_logical_or

  ; if other
  ; then display invalid choice message
  call print_new_line
  mov esi, invalid_choice_str
  call print_string

  call reload_screen

  ret

;****************************
; call clear screen 
; and then reload screen
; to reload calc menu
perform_clear_screen :
  call clear_screen

  call reload_screen

  ret

;******************************************
; this function jumps to main
reload_screen :

  call print_new_line

  mov esi, reload_screen_msg
  call print_string
  
  call read_single_key

  jmp main

;******************************************
; this function gets two numbers add them
; and stores the result in result variable
perform_addition :

  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  add eax, ebx
  mov dword[result], eax

  call print_new_line
  
  mov esi, addition
  call print_string

  ; convert dword to byte array
  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  ; print byte array
  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;**********************************************
; this function gets two numbers substract them
; and stores the result in result variable
perform_substraction :

  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  sub eax, ebx
  mov dword[result], eax

  call print_new_line
  
  mov esi, substraction
  call print_string

  ; convert dword to byte array
  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  ; print byte array
  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;**********************************************
; this function gets two numbers multiply them
; and stores the result in result variable
; when performing multiplication
; if result goes out of dword boundry
; it saves in edx register
perform_multiplication :

  xor edx, edx
  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  mul ebx
  mov dword[result], eax
  
  ; print result
  call print_new_line
  
  mov esi, multiplication
  call print_string

  ; print result
  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;********************************************
; this function gets two numbers divide them
; and stores the result in result variable
perform_division :

  xor edx, edx
  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  div ebx
  mov dword[result], eax

  ; print result
  call print_new_line
  
  mov esi, division
  call print_string

  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;**********************************************
; this function gets two numbers take mod of it
; and stores the result in result variable
perform_modulus :

  xor edx, edx
  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  div ebx
  mov dword[result], edx

  ; print result
  call print_new_line
  
  mov esi, modulus
  call print_string

  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;***********************************
; this function and the two numbers
perform_logical_and :

  xor edx, edx
  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  and eax, ebx
  mov dword[result], eax

  ; print result
  call print_new_line
  
  mov esi, logicaland
  call print_string

  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret
;***********************************
; this function or the two numbers
perform_logical_or :

  xor edx, edx
  mov eax, dword[number_1]
  mov ebx, dword[number_2]

  or eax, ebx
  mov dword[result], eax

  ; print result
  call print_new_line
  
  mov esi, logicalor
  call print_string

  lea ebx, [BUFFER]
  push dword[result]
  push ebx
  call dword_to_byte_array
  add esp, 8

  lea ebx, [BUFFER]
  push ebx
  call print_byte_array
  add esp, 4

  call print_new_line

  call reload_screen

  ret

;*****************************************
; this function reads a single key
; stores character to KEYCHAR variable
; and keycode to KEYCODE variable
read_single_key :
  push ebp

  ; keyboard interrupt
	mov ax,0x00
	int 0x16

  mov byte[KEYCHAR], al
  mov byte[KEYCODE], ah

  ; display the entered character
	mov ah,0x0E
	int 0x10

  pop ebp
  ret

;**********************************************
; this function keep reading data until
; enter key is pressed
; it stores char data into global buffer BUFFER
read_data :
  push ebp

  mov ecx, 0

.input_keys_loop :

  ; keyboard interrupt
	mov ax,0x00
	int 0x16

  ; if enter key, jump to exit
	cmp ah, byte[ENTER_KEY]
	je .exitinput

  ; display entered character
	mov ah,0x0E
	int 0x10

  ; add it to buffer
  mov byte[BUFFER + ecx], al

  inc ecx

  jmp .input_keys_loop 

.exitinput:

  ; add null terminator in buffer  
  mov byte[BUFFER + ecx], 0x00

  pop ebp
	ret


;*********************************************
; this function removes everything from screen
; and set cursor location to (0,0)
clear_screen :
  mov ax,0x13
  mov al,2
	mov ah,0
	int 0x10

  mov byte[NEWLINE], 0

  ret


;******************************************
; this function prints a string on screen 
print_string:
	mov ah, 0x0E

.next_char_loop :
  ; get character in al
	lodsb

	cmp al, 0
	je .exit
	
  int 0x10
	
  jmp .next_char_loop

.exit:
	ret

;******************************************
; this function print array of bytes
; param ebp - 8 = address of buffer
print_byte_array:
  
  mov ecx, 0
  mov ebx, dword[ebp - 8]

.loop:
  cmp byte[ebx + ecx], 0
  je .exit
  ; get byte from buffer
  mov al, byte[ebx + ecx]
  mov ah, 0x0E
  int 0x10

  inc ecx
  
  jmp .loop

.exit:
  ret

;******************************************
; this function prints a single character
; param ebp - 8 = chracter
print_char :
  push ebp

  xor eax, eax
  mov eax, dword[ebp - 8]

  mov ah, 0x0E
  int 0x10

  pop ebp
  ret

;***************************************************
; this function checks all bytes are from range 0..9
check_all_bytes_are_numeric : 
  push ebp

  ; get BUFFER address
  mov ebx, dword[ebp - 8]
  mov ecx, 0

.check_loop :
  cmp byte[ebx + ecx], 0
  je .exit

  ; if data is '0'
  cmp byte[ebx + ecx], 0x30
  jge .in_numeric

  mov eax, 0

  inc ecx

  jmp .check_loop

.in_numeric :

  ; if data is '9'
  cmp byte[ebx + ecx], 0x39
  jle .numeric

  mov eax, 0

  jmp .exit

.numeric :
  mov eax, 1
  inc ecx
  jmp .check_loop

.exit :
  pop ebp
  ret

;***************************************************
; this function gets each byte and multiply with 
; decimal position such as 1,10,100,1000,...
; and finally add with the result to get dword 
byte_array_to_dword : 
  push ebp

  ; get BUFFER address
  mov ebx, dword[ebp - 8]

  mov dword[number], 0
  mov dword[decimal_pos], 1

  ; get digit count
  mov ecx, dword[ebp - 12]
  dec ecx

.convert_dword_loop :
  cmp ecx, 0
  jl .exit

  xor eax, eax
  xor edx, edx
  ; get original data
  ; character '1' = 0x31
  ; substract 0x30 from it  
  mov al, byte[ebx + ecx]
  sub al, 0x30

  push ebx

  ; clear ebx and edx for multiplication
  xor ebx, ebx
  xor edx, edx

  ; num * 1,10,100...
  mov ebx, dword[decimal_pos]
  mul ebx

  add eax, dword[number]
  mov dword[number], eax

  ; clear ebx and edx for multiplication
  xor ebx, ebx
  xor edx, edx
  xor eax, eax

  mov eax, dword[decimal_pos]
  mov ebx, 10
  mul ebx
  mov dword[decimal_pos], eax

  pop ebx

  dec ecx

  jmp .convert_dword_loop

.exit :
  mov eax, dword[number]
  pop ebp
  ret

;****************************
; returns the number of bytes
bytes_length : 
  push ebp

  ; get buffer address
  mov ebx, dword[ebp - 8]
  mov eax, 0

.len_loop :
  cmp byte[ebx + eax], 0
  je .exit

  ; increment counter
  inc eax

  jmp .len_loop

.exit :
  pop ebp
  ret


;***************************************************
; ebp - 8 = passed number
; esp - 12 = count variable for storing the digit count
; eax = return of the count to caller
get_digit_count : 

  push ebp

  ; check if num is 0
  cmp dword[ebp - 8], 0
  je .num_is_zero

  ; allocate count variable on stack
  sub esp, 4
  mov dword[esp - 12], 0 ; count variable (count  = 0)
  
.dg_cnt_loop : 

  ; if num <= 0
  cmp dword[ebp - 8], 0
  jle .exit

  ; increment count 
  inc dword[esp - 12]

  ; clear eax, ebx and edx for division
  xor eax, eax
  xor ebx, ebx
  xor edx, edx

  ; num / 10
  mov eax, dword[ebp - 8]
  mov ebx, 10
  idiv ebx
  
  ; assign division result to num
  mov dword[ebp - 8], eax

  jmp .dg_cnt_loop

.num_is_zero : 
  ; if num is 0 then return 1
  mov eax, 1
  ; restore ebp
  pop ebp
  ret

.exit :
  ; assign count to eax
  mov eax, dword[esp - 12]
  ; restore allocated local variables
  add esp, 4
  ; restore ebp
  pop ebp
  ret

;******************************************************
; convert dword to array of bytes
; this function is same as itoa() in our C kernel code
; ebp - 8 = number
; ebp - 12 = address of byte array
; esp - 16 = digit_count variable
; esp - 20 = index variable
dword_to_byte_array : 

  push ebp

  ; allocate digit_count variable
  sub esp, 4
  mov dword[esp - 16], 0

  ; allocate index variable
  sub esp, 4
  mov dword[esp - 20], 0

  ; store the num value to ebx for temporary
  ; sometimes calling a function may lost the original value
  ; push the value in the stack
  mov ebx, dword[ebp - 8]
  push ebx

  ; get digit count of num by calling get_digit_count
  mov eax, dword[ebp - 8]
  push eax
  call get_digit_count
  add esp, 4

  ; pop the original num value
  pop ebx
  ; assign it to variable
  mov dword[ebp - 8], ebx
  

  ; assign digit count to digit count variable
  mov dword[esp - 16], eax

  ; decrement digit count by 1 as index for byte array
  dec eax

  ; assign index as digit_count to index variable
  mov dword[esp - 20], eax

  ; if num <= 0
  cmp dword[ebp - 8], 0
  je .dwrd_to_bt_zero_num

  ; if num > 0
.convert_char_loop : 
  ; if num <= 0
  cmp dword[ebp - 8], 0
  jle .exit_convrt_char

  ; clear eax, ebx and edx for division
  xor eax, eax
  xor ebx, ebx
  xor edx, edx

  ; num / 10
  mov eax, dword[ebp - 8]
  mov ebx, 10
  idiv ebx

  ; assign division result to num
  mov dword[ebp - 8], eax

  ; convert digit to character
  add dl, 0x30  ; 0x30 = '0'

  ; assign converted character to array
  mov ebx, dword[ebp - 12]  ; ebx = array address
  mov ecx, dword[esp - 20]  ; eax = index
  mov byte[ebx + ecx], dl   ; array[index] = char

  ; decrement index
  dec dword[esp - 20]

  jmp .convert_char_loop

.dwrd_to_bt_zero_num : 
  ; if digit count == 1
  cmp dword[esp - 16], 1
  je .digit_count_is_1

.digit_count_is_1 : 
  ; get array address
  mov ebx, dword[ebp - 12]
  mov byte[ebx], 0x30 ; character '0'
  mov byte[ebx + 1], 0x00 ; null terminator
  jmp .exit

.exit_convrt_char : 
  ; get array address
  mov ebx, dword[ebp - 12]
  mov eax, dword[esp - 16]
  mov byte[ebx + eax], 0x00 ; null terminator
  jmp .exit

.exit :

  add esp, 8

  pop ebp
  ret

;*****************************************
; this function increase NEWLINE variable
; and set cursor position to it
print_new_line :
  push ebp

  inc byte[NEWLINE]

  mov ah, 0x02
  mov bh, 0x00
  mov dl, 0; x pos
  mov dh, byte[NEWLINE]
  int 0x10

  pop ebp
  ret


;********************************************
; this function sets the cursor position to 
; provided x,y coordinates
goto_xy : 
  push ebp

  mov ah, 0x02
	mov bh, 0x00
  mov dl, byte[ebp - 8]; x pos
	mov dh, byte[ebp - 12] ; y pos
	int 0x10

  pop ebp

  ret

;********************************
; clear the gloabl buffer BUFFER
clear_global_buffer :
  mov ecx, 0

.clr_buf_loop :
  cmp ecx, dword[BUFSIZE]
  je .exit

  mov byte[BUFFER + ecx], 0x00

  jmp .clr_buf_loop

.exit :
  ret


; x86 code end


;******************************************
; set the required disk space we need
  times (4096 - ($ - $$)) db 0x00
  

;******************************************
; data sections

;******************************************
; read only data section
section .rodata
  __STACK__ dd 0x00FFFFFF
  __HEAP__ dd 0x00008C24
  BUFSIZE dd 512

  ; key codes

  ENTER_KEY db 0x1C
  DELETE_KEY db 0x0E

  KEYCODE_1 db 0x02
  KEYCODE_2 db 0x03
  KEYCODE_3 db 0x04
  KEYCODE_4 db 0x05
  KEYCODE_5 db 0x06
  KEYCODE_6 db 0x07
  KEYCODE_7 db 0x08
  KEYCODE_8 db 0x09

;******************************************
; initialized data section
section .data
  number dd 0
  decimal_pos dd 0
  NEWLINE db 0

  X dd 0
  Y dd 0

  number_1 dd 0
  number_2 dd 0
  result   dd 0

  os_msg db '! 80x86 Operating System !', 0
  calc_msg db '[ x86 Calculator Program ]', 0
  menu_str db '!--- Menu ---!', 0
  addition_menu db '1] Addition', 0
  substraction_menu db '2] Substraction', 0
  multiplication_menu db '3] Multiplication', 0
  division_menu db '4] Division', 0
  modulus_menu db '5] Modulus', 0
  logicaland_menu db '6] Logical AND', 0
  logicalor_menu db '7] Logical OR', 0
  clear_screen_menu db '8] Clear Screen', 0
  select_choice_str db 'Enter your choice : ', 0
  invalid_choice_str db 'Invalid choice', 0
  error_msg db 'Error : Please enter only numerics', 0

  addition db 'Addition : ', 0
  substraction db 'Substraction : ', 0
  multiplication db 'Multiplication : ', 0
  division db 'Division : ', 0
  modulus db 'Modulus : ', 0
  logicaland db 'Logical AND : ', 0
  logicalor db 'Logical OR : ', 0

  enter_first_number db 'Enter first number : ', 0
  enter_second_number db 'Enter Second number : ', 0

  reload_screen_msg db 'Press any key to reload screen...', 0

;******************************************
; block started by symbol(bss) section
section .bss
  BUFFER resb 512
  choice resb 1
  KEYCODE resb 1
  KEYCHAR resb 1
;*******************************************
