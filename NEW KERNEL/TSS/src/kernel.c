#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "tss.h"
#include "string.h"

void kmain() {
    gdt_init();
    tss_init();
    console_init(COLOR_WHITE, COLOR_BLACK);
    tss_print();
}


