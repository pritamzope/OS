#ifndef MOUSE_H
#define MOUSE_H

#include "types.h"

#define MOUSE_CMD_RESET     0xFF
#define MOUSE_CMD_RESEND     0xFE
#define MOUSE_CMD_SET_DEFAULTS     0xF6
#define MOUSE_CMD_DISABLE_PACKET_STREAMING     0xF5
#define MOUSE_CMD_ENABLE_PACKET_STREAMING     0xF4
#define MOUSE_CMD_SAMPLE_RATE     0xF3
#define MOUSE_CMD_MOUSE_ID     0xF2
#define MOUSE_CMD_REQUEST_SINGLE_PACKET     0xEB
#define MOUSE_CMD_STATUS     0xE9
#define MOUSE_CMD_RESOLUTION     0xE8

#define MOUSE_ACKNOWLEDGE      0xFA

#define PS2_CMD_PORT      0x64    // read/write PS/2 command register
#define MOUSE_DATA_PORT     0x60    // PS/2 data port register see https://wiki.osdev.org/%228042%22_PS/2_Controller


typedef struct {
    uint8 left_button: 1;
    uint8 right_button: 1;
    uint8 middle_button: 1;
    uint8 always_1: 1;
    uint8 x_sign: 1;
    uint8 y_sign: 1;
    uint8 x_overflow: 1;
    uint8 y_overflow: 1;
} MOUSE_STATUS;


void mouse_init();

int mouse_getx();
int mouse_gety();

#endif
