#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    keyboard_init();

    printf("Type something...\n");
    while(1) {
        printf("%c", kb_getchar());
    }
}


