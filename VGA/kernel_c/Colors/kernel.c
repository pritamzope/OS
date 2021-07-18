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

void fill_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color)
{
  int i;

  for (i = width; i > 0; i--) {
    draw_rect(x, y, i, height, color);
  }
}

void kernel_entry()
{
  int i;
  uint16 x = 0, y = 0;

  init_vga();

  for (i = 1; i < 64; i++) {
    fill_rect(x, y, 20, 20, i);
    x += 20;
    if (i % 8 == 0) {
      y += 20;
      x = 0;
    }
  }

}


