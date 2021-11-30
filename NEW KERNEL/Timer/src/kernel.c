#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "timer.h"

void function_1(TIMER_FUNC_ARGS *args) {
    printf("Executing function_1() after %d millisecond\n", args->timeout);
}

void function_2(TIMER_FUNC_ARGS *args) {
    printf("Executing function_2() after %d millisecond\n", args->timeout);
}

void add_timer_function(TIMER_FUNCTION function, uint32 timeout) {
    TIMER_FUNC_ARGS args = {0};
    args.timeout = timeout;
    timer_register_function(function, &args);
}

void kmain() {
    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();

    console_init(COLOR_WHITE, COLOR_BLACK);

    /* schedule some functions to be called after some time */
    add_timer_function(function_1, 200);
    add_timer_function(function_2, 300);

    while(1) {
        printf("Hello\n");
        sleep(1);
    }
}


