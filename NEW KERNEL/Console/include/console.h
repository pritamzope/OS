#ifndef CONSOLE_H
#define CONSOLE_H

#include "vga.h"

#define MAXIMUM_PAGES  16

#define SCROLL_UP     1
#define SCROLL_DOWN   2

void console_clear(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color);

//initialize console
void console_init(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color);
void console_scroll();
void console_putchar(char ch);
// revert back the printed character and add 0 to it
void console_ungetchar();
// revert back the printed character until n characters
void console_ungetchar_bound(uint8 n);

void console_gotoxy(uint16 x, uint16 y);

void console_putstr(const char *str);
void printf(const char *format, ...);

#endif

