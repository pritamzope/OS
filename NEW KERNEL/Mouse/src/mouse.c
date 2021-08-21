/**
 * PS2 mouse setup
 * see https://wiki.osdev.org/PS/2_Mouse
 */

#include "mouse.h"

#include "8259_pic.h"
#include "console.h"
#include "idt.h"
#include "io_ports.h"
#include "isr.h"
#include "string.h"
#include "types.h"

int g_mouse_x_pos = 0, g_mouse_y_pos = 0;

int mouse_getx() {
    return g_mouse_x_pos;
}

int mouse_gety() {
    return g_mouse_y_pos;
}

void mouse_wait(BOOL type) {
    uint32 time_out = 100000;
    if (type == FALSE) {
        // suspend until status is 1
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 1) == 1) {
                return;
            }
        }
        return;
    } else {
        while (time_out--) {
            if ((inportb(PS2_CMD_PORT) & 2) == 0) {
                return;
            }
        }
    }
}

void mouse_write(uint8 data) {
    // sending write command
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0xD4);
    mouse_wait(TRUE);
    // finally write data to port
    outportb(MOUSE_DATA_PORT, data);
}

uint8 mouse_read() {
    mouse_wait(FALSE);
    return inportb(MOUSE_DATA_PORT);
}

void get_mouse_status(char status_byte, MOUSE_STATUS *status) {
    memset(status, 0, sizeof(MOUSE_STATUS));
    if (status_byte & 0x01)
        status->left_button = 1;
    if (status_byte & 0x02)
        status->right_button = 1;
    if (status_byte & 0x04)
        status->middle_button = 1;
    if (status_byte & 0x08)
        status->always_1 = 1;
    if (status_byte & 0x10)
        status->x_sign = 1;
    if (status_byte & 0x20)
        status->y_sign = 1;
    if (status_byte & 0x40)
        status->x_overflow = 1;
    if (status_byte & 0x80)
        status->y_overflow = 1;
}

void mouse_handler(REGISTERS *r) {
    static uint8 mouse_cycle = 0;
    static char mouse_byte[3];
    MOUSE_STATUS status;

    switch (mouse_cycle) {
        case 0:
            mouse_byte[0] = mouse_read();
            get_mouse_status(mouse_byte[0], &status);
            mouse_cycle++;
            break;
        case 1:
            mouse_byte[1] = mouse_read();
            mouse_cycle++;
            break;
        case 2:
            mouse_byte[2] = mouse_read();
            if (mouse_byte[1] == 0 && mouse_byte[2] == 0)
                break;

            if(status.x_sign)
                g_mouse_x_pos = g_mouse_x_pos + mouse_byte[1];
            else
                g_mouse_x_pos = g_mouse_x_pos + mouse_byte[1];
    
            g_mouse_y_pos = g_mouse_y_pos - mouse_byte[2];

            if (g_mouse_x_pos < 0)
                g_mouse_x_pos = 0;
            if (g_mouse_y_pos < 0)
                g_mouse_y_pos = 0;
            if (g_mouse_x_pos > (VGA_WIDTH * VGA_HEIGHT))
                g_mouse_x_pos = (VGA_WIDTH * VGA_HEIGHT) - 1;
            if (g_mouse_y_pos > VGA_HEIGHT)
                g_mouse_y_pos = VGA_HEIGHT - 1;

            mouse_cycle = 0;
            break;
    }
    isr_end_interrupt(IRQ_BASE + 12);
    console_clear(COLOR_WHITE, COLOR_BLACK);
    console_gotoxy(g_mouse_x_pos, g_mouse_y_pos);
    console_putchar('X');
}

/**
 * available rates 10, 20, 40, 60, 80, 100, 200
 */
void set_mouse_rate(uint8 rate) {
    uint8 status;

    outportb(MOUSE_DATA_PORT, MOUSE_CMD_SAMPLE_RATE);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printf("error: failed to send mouse sample rate command\n");
        return;
    }
    outportb(MOUSE_DATA_PORT, rate);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printf("error: failed to send mouse sample rate data\n");
        return;
    }
}

void mouse_init() {
    uint8 status;

    g_mouse_x_pos = 5;
    g_mouse_y_pos = 2;

    printf("initializing mouse...\n");

    // enable mouse device
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0xA8);

    // print mouse id
    outportb(MOUSE_DATA_PORT, MOUSE_CMD_MOUSE_ID);
    status = mouse_read();
    printf("mouse id: 0x%x\n", status);

    set_mouse_rate(10);

    //outportb(MOUSE_DATA_PORT, MOUSE_CMD_RESOLUTION);
    //outportb(MOUSE_DATA_PORT, 0);

    // enable the interrupt
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, 0x20);
    mouse_wait(FALSE);
    // get and set second bit
    status = (inportb(MOUSE_DATA_PORT) | 2);
    // write status to port
    mouse_wait(TRUE);
    outportb(PS2_CMD_PORT, MOUSE_DATA_PORT);
    mouse_wait(TRUE);
    outportb(MOUSE_DATA_PORT, status);

    // set mouse to use default settings
    mouse_write(MOUSE_CMD_SET_DEFAULTS);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printf("error: failed to set default mouse settings\n");
        return;
    }

    // enable packet streaming to receive
    mouse_write(MOUSE_CMD_ENABLE_PACKET_STREAMING);
    status = mouse_read();
    if(status != MOUSE_ACKNOWLEDGE) {
        printf("error: failed to enable mouse packet streaming\n");
        return;
    }

    // set mouse handler
    isr_register_interrupt_handler(IRQ_BASE + 12, mouse_handler);
}
