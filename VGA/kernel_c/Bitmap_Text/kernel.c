#include "kernel.h"
#include "vga.h"
#include "keyboard.h"
#include "bitmap.h"

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

  draw_string(50, 50, WHITE, "HELLO WORLD");
}


