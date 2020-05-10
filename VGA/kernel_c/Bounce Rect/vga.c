#include "vga.h"
#include "kernel.h"

/*
See Intel® OpenSource HD Graphics PRM pdf file
for following defined data for each vga register
and its explaination
*/
static uint8 seq_data[5] = {0x03, 0x01, 0x0F, 0x00, 0x0E};
static uint8 crtc_data[25] = {0x5F, 0x4F, 0x50, 0x82, 
                              0x54, 0x80, 0xBF, 0x1F,
                              0x00, 0x41, 0x00, 0x00, 
                              0x00, 0x00, 0x00, 0x00,
                              0x9C, 0x0E, 0x8F, 0x28,	
                              0x40, 0x96, 0xB9, 0xA3,
                              0xFF};

static uint8 gc_data[9] = {0x00, 0x00, 0x00, 0x00, 
                          0x00, 0x40, 0x05, 0x0F,
                          0xFF};

static uint8 ac_data[21] = {0x00, 0x01, 0x02, 0x03,
                            0x04, 0x05, 0x06, 0x07,
                            0x08, 0x09, 0x0A, 0x0B,
                            0x0C, 0x0D, 0x0E, 0x0F,
                            0x41, 0x00, 0x0F, 0x00,
                            0x00};

void set_misc()
{
  outb(VGA_MISC_WRITE, 0x63);
}

void set_seq()
{
  // write sequence data to index of 0-4
  for(uint8 index = 0; index < 5; index++){
    // first set index
    outb(VGA_SEQ_INDEX, index);
    // write data at that index
    outb(VGA_SEQ_DATA, seq_data[index]);
  }
}

void set_crtc()
{
  // write crtc data to index of 0-24
  for(uint8 index = 0; index < 25; index++){
    outb(VGA_CRTC_INDEX, index);
	  outb(VGA_CRTC_DATA, crtc_data[index]);
  }
}

void set_gc()
{
  // write gc data to index of 0-8
  for(uint8 index = 0; index < 9; index++){
    outb(VGA_GC_INDEX, index);
    outb(VGA_GC_DATA, gc_data[index]);
  }
}

void set_ac()
{
  uint8 d;
  // write ac data to index of 0-20
  for(uint8 index = 0; index < 21; index++){
    outb(VGA_AC_INDEX, index);
    outb(VGA_AC_WRITE, ac_data[index]);
  }
  d = inb(VGA_INSTAT_READ);
  outb(VGA_AC_INDEX, d | 0x20);
}

void clear_screen()
{
  for(uint32 index = 0; index < VGA_MAX; index++)
    vga_buffer[index] = 0;
}

void clear_color(uint8 color)
{
  for(uint32 index = 0; index < VGA_MAX; index++)
    vga_buffer[index] = color;
}

void init_vga()
{
  set_misc();
  set_seq();
  set_crtc();
  set_gc();
  set_ac();

  vga_buffer = (uint8*)VGA_ADDRESS;

  clear_screen();
}

void putpixel(uint16 x, uint16 y, uint8 color)
{
  uint32 index = 0;
  index = 320 * y + x;
  if(index < VGA_MAX)
    vga_buffer[index] = color;
}

void draw_line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 color)
{
  if(y1 == y2){
    for(uint16 i = x1; i <= x2; i++)
      putpixel(i, y1, color);
    return;
  }

  if(x1 == x2){
    for(uint16 i = y1; i <= y2; i++){
      putpixel(x1, i, color);
    }
    return;
  }
}

void draw_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color)
{
  draw_line(x, y, x, y + height, color);
  draw_line(x, y, x + width, y, color);
  draw_line(x + width, y, x + width, y + height, color);
  draw_line(x, y + height, x + width, y + height, color);
}

void fill_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color)
{
  draw_line(x, y, x, y + height, color);
  draw_line(x, y, x + width, y, color);
  draw_line(x + width, y, x + width, y + height, color);
  draw_line(x, y + height, x + width, y + height, color);
  for(int i = y; i < y + height; i++){
    draw_line(x, i, x + width, i, color);
  }
}

void draw_bresenham_circle(int xc, int yc, int x, int y, uint8 color) 
{ 
  putpixel(xc+x, yc+y, color); 
  putpixel(xc-x, yc+y, color); 
  putpixel(xc+x, yc-y, color); 
  putpixel(xc-x, yc-y, color); 
  putpixel(xc+y, yc+x, color); 
  putpixel(xc-y, yc+x, color); 
  putpixel(xc+y, yc-x, color); 
  putpixel(xc-y, yc-x, color); 
} 

void draw_circle(uint16 x, uint16 y, uint16 radius, uint8 color)
{
  int x2 = 0, y2 = radius;
  int d = 3 - 2 * radius;
  draw_bresenham_circle(x, y, x2, y2, color);
  while(y2 >= x2){
    x2++;
    if(d > 0){
      y2--;
      d = d + 4 * (x2 - y2) + 10;
    }else{
      d = d + 4 * x2 + 6;
    } 
    draw_bresenham_circle(x, y, x2, y2, color);
  } 
}

void draw_diamond(uint16 x, uint16 y, uint16 radius, uint8 color)
{
  uint16 x2 = 0, y2 = radius;
  uint16 d = 3 - 2 * radius;
  draw_bresenham_circle(x, y, x2, y2, color);
  while(y2 >= x2){
    x2++;
    if(d > 0){
      y2--;
      d = d + 4 * (x2 - y2) + 10;
    }else{
      d = d + 4 * x2 + 6;
    } 
    draw_bresenham_circle(x, y, x2, y2, color);
  } 
}

