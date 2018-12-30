;****Digital Differential Analyzer(DDA) Line Drawing algorithm****

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
  add di, word[VGA_WIDTH]
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

