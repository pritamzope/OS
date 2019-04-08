#ifndef BOX_H
#define BOX_H

#include "types.h"


#define BOX_MAX_WIDTH 78
#define BOX_MAX_HEIGHT 23

#define BOX_SINGLELINE 1
#define BOX_DOUBLELINE 2


extern void draw_generic_box(uint16, uint16, uint16, uint16, 
                            uint8, uint8, uint8, uint8, 
                            uint8, uint8, uint8, uint8);

extern void draw_box(uint8, uint16, uint16, uint16, uint16,
                      uint8, uint8);

extern void fill_box(uint8, uint16, uint16, uint16, uint16, uint8);


#endif


