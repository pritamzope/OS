#include "vga.h"
#include "io_ports.h"
#include "kernel.h"

// a global VGA buffer
static uint8* g_vga_buffer;

/**
 * 16 bit video buffer elements(register ax)
 * 8 bits(ah) higher : 
 * lower 4 bits - forec olor
 * higher 4 bits - back color

 * 8 bits(al) lower :
 * 8 bits : ASCII character to print
 * 
 * returns complete item with fore & back color to be placed at VGA address
*/
uint16 vga_text_item_entry(uint8 ch, VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    uint16 ax = 0;
    uint8 ah = 0, al = 0;

    ah = back_color;
    ah <<= 4;
    ah |= fore_color;
    ax = ah;
    ax <<= 8;
    al = ch;
    ax |= al;

    return ax;
}

/**
 * set cursor position to given (x, y)
 * by writing to CRT controller registers
 */
void vga_text_set_cursor_pos(uint8 x, uint8 y) {
    // The screen is 80 characters wide...
    uint16 cursorLocation = y * VGA_TEXT_WIDTH + x;
    outportb(0x3D4, 14);
    outportb(0x3D5, cursorLocation >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, cursorLocation);
}

/**
 * disable blinking top-left cursor
 * by writing to CRT controller registers
 */
void vga_disable_cursor() {
    outportb(0x3D4, 10);
    outportb(0x3D5, 32);
}


/*
See Intel® OpenSource HD Graphics PRM pdf file for following defined data for each vga register
and its explaination
*/
void set_miscellaneous_registers() {
    outportb(VGA_MISC_WRITE, 0x63);
}

void set_sequencer_registers() {
    outportb(VGA_SEQ_INDEX, 0);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x03);  // sequencer reset

    outportb(VGA_SEQ_INDEX, 1);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x01);  // clocking mode

    outportb(VGA_SEQ_INDEX, 2);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x0F);  // plane/map mask

    outportb(VGA_SEQ_INDEX, 3);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x00);  // character font

    outportb(VGA_SEQ_INDEX, 4);    // sequencer index
    outportb(VGA_SEQ_DATA, 0x0E);  // memory mode register
}

void set_graphics_controller_registers() {
    outportb(VGA_GC_INDEX, 0);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // set/reset register

    outportb(VGA_GC_INDEX, 1);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // enable set/reset register

    outportb(VGA_GC_INDEX, 2);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // color compare register

    outportb(VGA_GC_INDEX, 3);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // data rotate register

    outportb(VGA_GC_INDEX, 4);    // gc index
    outportb(VGA_GC_DATA, 0x00);  // read plane select register

    outportb(VGA_GC_INDEX, 5);    // gc index
    outportb(VGA_GC_DATA, 0x40);  // graphics mode register

    outportb(VGA_GC_INDEX, 6);    // gc index
    outportb(VGA_GC_DATA, 0x05);  // miscellaneous register

    outportb(VGA_GC_INDEX, 7);    // gc index
    outportb(VGA_GC_DATA, 0x0F);  // color dont care register

    outportb(VGA_GC_INDEX, 8);    // gc index
    outportb(VGA_GC_DATA, 0xFF);  // bit mask register
}

void set_attribute_controller_registers() {
    uint8 d;
    uint8 ac_data[21] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                         0x41, 0x00, 0x0F, 0x00, 0x00};

    for (uint8 index = 0; index < 21; index++) {
        outportb(VGA_AC_INDEX, index);
        outportb(VGA_AC_WRITE, ac_data[index]);
    }
    d = inportb(VGA_INSTAT_READ);
    outportb(VGA_AC_INDEX, d | 0x20);
}

void set_crt_controller_registers() {
    uint8 crtc_data[25] = {0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
                            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3, 0xFF};
    
    for (uint8 index = 0; index < 25; index++) {
        outportb(VGA_CRTC_INDEX, index);
        outportb(VGA_CRTC_DATA, crtc_data[index]);
    }
}


void vga_graphics_init() {
    set_miscellaneous_registers();
    set_sequencer_registers();
    set_crt_controller_registers();
    set_graphics_controller_registers();
    set_attribute_controller_registers();

    g_vga_buffer = (uint8*)VGA_ADDRESS;

    vga_graphics_clear_color(BLACK);
}

void vga_graphics_exit() {
    uint8 i;
    // clear miscellaneous registers
    outportb(VGA_MISC_WRITE, 0x00);
    // clear sequencer registers
    for (i = 0; i < 5; i++) {
        outportb(VGA_GC_INDEX, i);
        outportb(VGA_GC_DATA, 0x00);
    }
    // clear crt controller registers
    for (i = 0; i < 25; i++) {
        outportb(VGA_CRTC_INDEX, i);
        outportb(VGA_CRTC_DATA, 0x00);
    }
    // clear graphics controller registers
    for (i = 0; i < 9; i++) {
        outportb(VGA_GC_INDEX, i);
        outportb(VGA_GC_DATA, 0x00);
    }
    // clear ac registers
    for (i = 0; i < 21; i++) {
        outportb(VGA_AC_INDEX, i);
        outportb(VGA_AC_WRITE, 0x00);
    }
    outportb(VGA_AC_INDEX, 0x00);

}

void vga_graphics_clear_color(uint8 color) {
    for (uint32 index = 0; index < VGA_MAX; index++)
        g_vga_buffer[index] = color;
}

void vga_graphics_putpixel(uint16 x, uint16 y, uint8 color) {
    uint32 index = 0;
    index = 320 * y + x;
    if (index < VGA_MAX)
        g_vga_buffer[index] = color;
}

void vga_graphics_draw_line(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint8 color) {
    if (y1 == y2) {
        for (uint16 i = x1; i <= x2; i++)
            vga_graphics_putpixel(i, y1, color);
        return;
    }

    if (x1 == x2) {
        for (uint16 i = y1; i <= y2; i++) {
            vga_graphics_putpixel(x1, i, color);
        }
        return;
    }
}

void vga_graphics_draw_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color) {
    vga_graphics_draw_line(x, y, x, y + height, color);
    vga_graphics_draw_line(x, y, x + width, y, color);
    vga_graphics_draw_line(x + width, y, x + width, y + height, color);
    vga_graphics_draw_line(x, y + height, x + width, y + height, color);
}

void vga_graphics_fill_rect(uint16 x, uint16 y, uint16 width, uint16 height, uint8 color) {
    vga_graphics_draw_line(x, y, x, y + height, color);
    vga_graphics_draw_line(x, y, x + width, y, color);
    vga_graphics_draw_line(x + width, y, x + width, y + height, color);
    vga_graphics_draw_line(x, y + height, x + width, y + height, color);
    for (int i = 0; i < height; i++) {
        vga_graphics_draw_line(x, y + i, x + width, y + i, color);
    }
}

void draw_bresenham_circle(int xc, int yc, int x, int y, uint8 color) {
    vga_graphics_putpixel(xc + x, yc + y, color);
    vga_graphics_putpixel(xc - x, yc + y, color);
    vga_graphics_putpixel(xc + x, yc - y, color);
    vga_graphics_putpixel(xc - x, yc - y, color);
    vga_graphics_putpixel(xc + y, yc + x, color);
    vga_graphics_putpixel(xc - y, yc + x, color);
    vga_graphics_putpixel(xc + y, yc - x, color);
    vga_graphics_putpixel(xc - y, yc - x, color);
}

void vga_graphics_draw_circle(uint16 x, uint16 y, uint16 radius, uint8 color) {
    int x2 = 0, y2 = radius;
    int d = 3 - 2 * radius;
    draw_bresenham_circle(x, y, x2, y2, color);
    while (y2 >= x2) {
        x2++;
        if (d > 0) {
            y2--;
            d = d + 4 * (x2 - y2) + 10;
        } else {
            d = d + 4 * x2 + 6;
        }
        draw_bresenham_circle(x, y, x2, y2, color);
    }
}

