#ifndef BITMAP_H
#define BITMAP_H

#include "types.h"

#define BITMAP_SIZE 8

void draw_char(uint16 x, uint16 y, uint8 color, char ch);
void draw_string(uint16 x, uint16 y, uint8 color, char *str);

#endif
