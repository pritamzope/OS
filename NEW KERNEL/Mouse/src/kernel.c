#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "mouse.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    vga_disable_cursor();
    keyboard_init();
    mouse_init();

    printf("\n\n\t\t--Mouse Click Here--");
}


