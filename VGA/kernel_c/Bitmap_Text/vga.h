#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_ADDRESS 0xA0000
#define VGA_MAX 0xF9FF
#define VGA_MAX_WIDTH 320
#define VGA_MAX_HEIGHT 200

uint8* vga_buffer;

enum vga_color {
  BLACK,
  BLUE,
  GREEN,
  CYAN,
  RED,
  MAGENTA,
  BROWN,
  GREY,
  DARK_GREY,
  BRIGHT_BLUE,
  BRIGHT_GREEN,
  BRIGHT_CYAN,
  BRIGHT_RED,
  BRIGHT_MAGENTA,
  YELLOW,
  WHITE,
};

/* Attribute Controller Registers */
#define	VGA_AC_INDEX 0x3C0
#define	VGA_AC_READ 0x3C1
#define	VGA_AC_WRITE 0x3C0

/*
Miscellaneous Output
*/
#define	VGA_MISC_READ 0x3CC
#define	VGA_MISC_WRITE 0x3C2

/* Sequencer Registers */
#define VGA_SEQ_INDEX 0x3C4
#define VGA_SEQ_DATA 0x3C5

/* VGA Color Palette Registers */
#define	VGA_DAC_READ_INDEX 0x3C7
#define	VGA_DAC_WRITE_INDEX 0x3C8
#define	VGA_DAC_DATA 0x3C9

/* Graphics Controller Registers */
#define VGA_GC_INDEX 0x3CE
#define VGA_GC_DATA 0x3CF

/* CRT Controller Registers */
#define VGA_CRTC_INDEX 0x3D4
#define VGA_CRTC_DATA 0x3D5

/* General Control and Status Registers */
#define	VGA_INSTAT_READ 0x3DA

void init_vga();
void clear_screen();
void clear_color(uint8 color);
void putpixel(uint16 x, uint16 y, uint8 color);
void draw_line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 color);
void draw_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color);
void fill_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color);
void draw_circle(uint16 x, uint16 y, uint16 radius, uint8 color);
void draw_diamond(uint16 x, uint16 y, uint16 radius, uint8 color);

#endif


