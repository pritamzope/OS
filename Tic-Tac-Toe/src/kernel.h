#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

#define NULL 0

#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200

uint16* vga_buffer;
extern uint32 vga_index;

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

extern void clear_screen(uint8, uint8);
extern void print_new_line();
extern void print_char(char);
extern void print_string(char *);
extern void print_color_string(char *, uint8, uint8);
extern void print_int(int);
extern uint8 inb(uint16);
extern void outb(uint16, uint8);
extern byte get_input_keycode();
extern void sleep(uint32);
extern void gotoxy(uint16, uint16);

#endif
