#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

#define VGA_ADDRESS 0xB8000
#define BUFSIZE 2200

uint16* vga_buffer;

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

extern void print_new_line();
extern void print_char(char);
extern void print_string(char *);
extern void print_int(int);


#endif
