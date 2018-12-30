;draw a pixel at x,y location
drawpixel :

  ;get x co-ordinate
  mov ax, word[X_PIX]
  
  ;get y co-ordinate
  mov bx, word[Y_PIX]
  
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
  mov ax, word[PIXEL_COLOR]
  
  ;plot the pixel with index(di) and pixel color(ax)
  mov [es:di], ax
 
  ret
