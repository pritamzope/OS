#ifndef PONG_H
#define PONG_H

#include "kernel.h"
#include "vga.h"
#include "keyboard.h"
#include "utils.h"
#include "bitmap.h"

/*
if wants to run on qemu, uncomment following line, 
comment line #define VIRTUALBOX
compile & run
*/
//#define QEMU

/*
if wants to run on virtualbox, uncomment following line, 
comment line #define QEMU
compile & run
*/
#define VIRTUALBOX


// size of bounced rectangle
#define RECT_SIZE      6

// speed of rectangle on qemu
#ifdef QEMU
    #define RECT_SPEED_X   6
    #define RECT_SPEED_Y   6
#endif

// speed of rectangle on virtualbox
#ifdef VIRTUALBOX
    #define RECT_SPEED_X   2
    #define RECT_SPEED_Y   2
#endif

// pad position, size & pads speeds
#define PAD_POS_X    VGA_MAX_WIDTH - PAD_WIDTH - 1
#define PAD_WIDTH    6
#define PAD_HEIGHT   50

// pads speed on qemu
#ifdef QEMU
    #define PAD_SPEED    20
#endif

// pads speed on virtualbox
#ifdef VIRTUALBOX
    #define PAD_SPEED    6
#endif


void pong_game();


#endif
