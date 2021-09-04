#ifndef CONSOLE_H
#define CONSOLE_H

#include "vga.h"

void console_clear(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color);

//initialize console
void console_init(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color);

void printf(const char *format, ...);

#endif
