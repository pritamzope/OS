#include "kernel.h"
#include "vga.h"

uint8 inb(uint16 port)
{
  uint8 data;
  asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
  return data;
}

void outb(uint16 port, uint8 data)
{
  asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

void kernel_entry()
{
  init_vga();
  draw_rect(0, 0, VGA_MAX_WIDTH - 1, VGA_MAX_HEIGHT - 1, BRIGHT_GREEN);
  draw_rect(2, 2, 30, 30, YELLOW);
  draw_rect(VGA_MAX_WIDTH - 33, 2, 30, 30, YELLOW);
  draw_rect(2, VGA_MAX_HEIGHT - 33, 30, 30, YELLOW);
  draw_rect(VGA_MAX_WIDTH - 33, VGA_MAX_HEIGHT - 33, 30, 30, YELLOW);
  draw_diamond(16, 16, 10, BRIGHT_CYAN);
  draw_diamond(VGA_MAX_WIDTH - 18, 16, 10, BRIGHT_CYAN);
  draw_diamond(16, VGA_MAX_HEIGHT - 18, 10, BRIGHT_CYAN);
  draw_diamond(VGA_MAX_WIDTH - 18, VGA_MAX_HEIGHT - 18, 10, BRIGHT_CYAN);

  draw_line(VGA_MAX_WIDTH / 2, 0, VGA_MAX_WIDTH / 2, VGA_MAX_HEIGHT, BRIGHT_GREEN);

  for(uint16 i = 0; i < 50; i+=3){
    draw_circle(80, 100, 50-i, BRIGHT_RED);
  }

  for(uint16 i = 0; i < 50; i+=3){
    draw_diamond(240, 100, 50-i, BRIGHT_MAGENTA);
  }

}


