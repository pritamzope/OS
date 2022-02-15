#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "bios32.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);

    printf("initializing bios32...\n");
    bios32_init();    
    printf("successfully initialized bios32\n");

    // https://en.wikipedia.org/wiki/BIOS_interrupt_call
    REGISTERS16 in = {0};
    REGISTERS16 out = {0};
    bios32_service(0x12, &in, &out);
    printf("Conventional Memory: %d KB\n", out.ax);
    
}


