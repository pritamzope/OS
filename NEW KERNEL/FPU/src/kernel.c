#include "kernel.h"

#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "io_ports.h"
#include "keyboard.h"
#include "string.h"
#include "fpu.h"

void ftoa(char *buf, float f) {
    uint32 count = 1;
    const uint32 DEFAULT_DECIMAL_COUNT = 8;
    char int_part_buf[16];
    char *p;

    memset(int_part_buf, 0, sizeof(int_part_buf));
    // add integer part
    int x = (int)f;
    itoa(int_part_buf, 'd', x);
    p = int_part_buf;
    while (*p != '\0') {
        *buf++ = *p++;
    }
    *buf++ = '.';

    // example decimal = 3.14159 - 3 = 0.14159
    float decimal = f - x;
    if (decimal == 0)
        *buf++ = '0';
    else {
        while (decimal > 0) {
            uint32 y = decimal * 10;  // y = 0.14159 * 10 = 1
            *buf++ = y + '0';
            decimal = (decimal * 10) - y;  // decimal = (0.14159 * 10) - 1 = 0.4159
            count++;
            if (count == DEFAULT_DECIMAL_COUNT)
                break;
        }
    }
    *buf = '\0';
}

void float_print(const char *msg, float f, const char *end) {
    char buf[32];
    memset(buf, 0, sizeof(buf));
    ftoa(buf, f);
    printf("%s%s%s", msg, buf, end);
}

void kmain() {
    gdt_init();
    idt_init();
    console_init(COLOR_WHITE, COLOR_BLACK);
    //keyboard_init();
    fpu_enable();
    float_print("Value 1: ", 1.73728, "\n");
    float_print("Value 2: ", 123.00237, "\n");
    float_print("Value 3: ", 936624, "\n");
    float_print("Value 4: ", 0.000352, "\n");
    float_print("Value 5: ", 89347.403402361, "\n");
}
