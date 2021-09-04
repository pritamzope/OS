#include "console.h"
#include "types.h"
#include "vga.h"
#include "string.h"

static uint16 *g_vga_buffer;
//index for video buffer array
static uint32 g_vga_index;
// cursor positions
static uint32 cursor_pos_x = 0, cursor_pos_y = 0;
//fore & back color values
uint8 g_fore_color = COLOR_WHITE, g_back_color = COLOR_BLACK;

// clear video buffer array
void console_clear(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    uint32 i;

    for (i = 0; i < VGA_TOTAL_ITEMS; i++) {
        g_vga_buffer[i] = vga_item_entry(NULL, fore_color, back_color);
    }
    g_vga_index = 0;
}

//initialize console
void console_init(VGA_COLOR_TYPE fore_color, VGA_COLOR_TYPE back_color) {
    g_vga_buffer = (uint16 *)VGA_ADDRESS;
    console_clear(fore_color, back_color);
    g_fore_color = fore_color;
    g_back_color = back_color;
}

/*
increase vga_index by width of vga width
*/
static void console_newline() {
    if (cursor_pos_y >= VGA_HEIGHT) {
        cursor_pos_x = 0;
        cursor_pos_y = 0;
        console_clear(g_fore_color, g_back_color);
    } else {
      g_vga_index += VGA_WIDTH - (g_vga_index % VGA_WIDTH);
    }
}

//assign ascii character to video buffer
void console_putchar(char ch) {
    if (ch == ' ') {
      g_vga_buffer[g_vga_index++] = vga_item_entry(' ', g_fore_color, g_back_color);
      vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
    } if (ch == '\t') {
      g_vga_buffer[g_vga_index++] = vga_item_entry(0, g_fore_color, g_back_color);
      g_vga_buffer[g_vga_index++] = vga_item_entry(0, g_fore_color, g_back_color);
      g_vga_buffer[g_vga_index++] = vga_item_entry(0, g_fore_color, g_back_color);
      g_vga_buffer[g_vga_index++] = vga_item_entry(0, g_fore_color, g_back_color);
      vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
    } else if (ch == '\n') {
      console_newline();
      vga_set_cursor_pos(0, ++cursor_pos_y);
    } else {
      g_vga_buffer[g_vga_index++] = vga_item_entry(ch, g_fore_color, g_back_color);
      vga_set_cursor_pos(cursor_pos_x++, cursor_pos_y);
    }
}

//print string by calling print_char
void console_putstr(char *str) {
    uint32 index = 0;
    while (str[index]) {
        if (str[index] == '\n')
            console_newline();
        else
            console_putchar(str[index]);
        index++;
    }
    vga_set_cursor_pos(strlen(str), 0);
}


void printf(const char *format, ...) {
    char **arg = (char **)&format;
    int c;
    char buf[32];

    arg++;

    memset(buf, 0, sizeof(buf));
    while ((c = *format++) != 0) {
        if (c != '%')
            console_putchar(c);
        else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            c = *format++;
            if (c == '0') {
                pad0 = 1;
                c = *format++;
            }

            if (c >= '0' && c <= '9') {
                pad = c - '0';
                c = *format++;
            }

            switch (c) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, c, *((int *)arg++));
                    p = buf;
                    goto string;
                    break;

                case 's':
                    p = *arg++;
                    if (!p)
                        p = "(null)";

                string:
                    for (p2 = p; *p2; p2++)
                        ;
                    for (; p2 < p + pad; p2++)
                        console_putchar(pad0 ? '0' : ' ');
                    while (*p)
                        console_putchar(*p++);
                    break;

                default:
                    console_putchar(*((int *)arg++));
                    break;
            }
        }
    }
}
