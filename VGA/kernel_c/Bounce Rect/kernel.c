#include "kernel.h"
#include "vga.h"
#include "keyboard.h"

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

uint8 get_input_keycode()
{
  uint8 keycode = 0;
  while((keycode = inb(KEYBOARD_PORT)) != 0){
    if(keycode > 0)
      return keycode;
  }
  return keycode;
}

/*
keep the cpu busy for doing nothing(nop)
so that io port will not be processed by cpu
here timer can also be used, but lets do this in looping counter
*/
void wait_for_io(uint32 timer_count)
{
  while(1){
    asm volatile("nop");
    timer_count--;
    if(timer_count <= 0)
      break;
    }
}

void sleep(uint32 timer_count)
{
  uint32 t = 0xeeeeef * timer_count;
  wait_for_io(t);
}


void bounce_rect()
{
  const uint16 rect_size = 10;
  uint16 rect_pos_x = rect_size;
  uint16 rect_pos_y = rect_size;
  uint16 rect_speed_x = 5;
  uint16 rect_speed_y = 5;

  while(1){
    // add speed values to positions
    rect_pos_x += rect_speed_x;
    rect_pos_y += rect_speed_y;

    // check if position x < 0
    if(rect_pos_x - rect_size < 0){
      // decrese added speed
      rect_speed_x = -rect_speed_x;
      // set position x to rect size
      rect_pos_x = rect_size;
    }else if(rect_pos_x + rect_size > VGA_MAX_WIDTH + rect_size){
      // check if position > VGA width + rect_size
      // decrese added speed
      rect_speed_x = -rect_speed_x;
      // set position x to minimum of VGA width rect size
      rect_pos_x = VGA_MAX_WIDTH - rect_size;
    }

    // same action for position y
    if(rect_pos_y - rect_size < 0){
      rect_speed_y = -rect_speed_y;
      rect_pos_y = rect_size;
    }else if(rect_pos_y + rect_size > VGA_MAX_HEIGHT + rect_size){
      rect_speed_y = -rect_speed_y;
      rect_pos_y = VGA_MAX_HEIGHT - rect_size;
    }

    // clear screen
    clear_screen();
    // fill rect
    fill_rect(rect_pos_x - rect_size, rect_pos_y - rect_size, rect_size, rect_size, WHITE);
    // change sleep value if running in VirtualBox or on bare metal
    sleep(1);
  }
}

void kernel_entry()
{
  init_vga();
  bounce_rect();
}


