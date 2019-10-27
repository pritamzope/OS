#include "kernel.h"
#include "util.h"
#include "types.h"
#include "gdt.h"
#include "idt.h"

//index for video buffer array
uint32 vga_index;
//counter to store new lines
static uint32 next_line_index = 1;
//fore & back color values
uint8 g_fore_color = WHITE, g_back_color = BLUE;
//digit ascii code for printing integers
int digit_ascii_codes[10] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};

/*
16 bit video buffer elements(register ax)
8 bits(ah) higher : 
  lower 4 bits - forec olor
  higher 4 bits - back color

8 bits(al) lower :
  8 bits : ASCII character to print
*/
uint16 vga_entry(unsigned char ch, uint8 fore_color, uint8 back_color) 
{
  uint16 ax = 0;
  uint8 ah = 0, al = 0;

  ah = back_color;
  ah <<= 4;
  ah |= fore_color;
  ax = ah;
  ax <<= 8;
  al = ch;
  ax |= al;

  return ax;
}

//clear video buffer array
void clear_vga_buffer(uint16 **buffer, uint8 fore_color, uint8 back_color)
{
  uint32 i;
  for(i = 0; i < BUFSIZE; i++){
    (*buffer)[i] = vga_entry(NULL, fore_color, back_color);
  }
  next_line_index = 1;
  vga_index = 0;
}

//initialize vga buffer
void init_vga(uint8 fore_color, uint8 back_color)
{
  vga_buffer = (uint16*)VGA_ADDRESS;
  clear_vga_buffer(&vga_buffer, fore_color, back_color);
  g_fore_color = fore_color;
  g_back_color = back_color;
}

/*
increase vga_index by width of row(80)
*/
void print_new_line()
{
  if(next_line_index >= 55){
    next_line_index = 0;
    clear_vga_buffer(&vga_buffer, g_fore_color, g_back_color);
  }
  vga_index = 80*next_line_index;
  next_line_index++;
}

//assign ascii character to video buffer
void print_char(char ch)
{
  vga_buffer[vga_index] = vga_entry(ch, g_fore_color, g_back_color);
  vga_index++;
}

//print string by calling print_char
void print_string(char *str)
{
  uint32 index = 0;
  while(str[index]){
    if(str[index] == '\n')
      print_new_line();
    else
      print_char(str[index]);
    index++;
  }
}

//print int by converting it into string
//& then printing string
void print_int(int num)
{
  char str_num[digit_count(num)+1];
  itoa(num, str_num);
  print_string(str_num);
}


void kernel_entry()
{
  init_gdt();
  init_idt();

  init_vga(WHITE, BLACK);

  // set values to registers and raise an interrupt with number
  asm volatile("\tmov $12345, %eax");
  asm volatile("\tint $0");
  asm volatile("\tint $1");
  asm volatile("\tint $2");
  asm volatile("\tint $3");
  asm volatile("\tint $4");
  asm volatile("\tint $5");
  asm volatile("\tint $6");
  asm volatile("\tint $7");
  asm volatile("\tint $8");
  asm volatile("\tint $9");
  asm volatile("\tint $10");
  asm volatile("\tint $11");
  asm volatile("\tint $12");
  asm volatile("\tint $13");
  asm volatile("\tint $14");
  asm volatile("\tint $15");

}



