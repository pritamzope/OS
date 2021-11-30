#include "timer.h"

#include "console.h"
#include "idt.h"
#include "io_ports.h"
#include "isr.h"
#include "string.h"
#include "types.h"

// number of ticks since system booted
uint32 g_ticks = 0;
// frequency in hertz
uint16 g_freq_hz = 0;
// timer functions to be called when that ticks reached in irq handler
TIMER_FUNCTION_MANAGER g_timer_function_manager;


// See https://wiki.osdev.org/Programmable_Interval_Timer
void timer_set_frequency(uint16 f) {
    g_freq_hz = f;
    uint16 divisor = TIMER_INPUT_CLOCK_FREQUENCY / f;
    // set Mode 3 - Square Wave Mode
    outportb(TIMER_COMMAND_PORT, 0b00110110);
    // set low byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, divisor & 0xFF);
    // set high byte
    outportb(TIMER_CHANNEL_0_DATA_PORT, (divisor >> 8) & 0xFF);
}


void timer_handler(REGISTERS* r) {
    uint32 i;
    TIMER_FUNC_ARGS *args = NULL;
    g_ticks++;
    //printf("timer triggered at frequency %d\n", g_ticks);
    for (i = 0; i < MAXIMUM_TIMER_FUNCTIONS; i++) {
        args = &g_timer_function_manager.func_args[i];
        if (args->timeout == 0)
            continue;
        if ((g_ticks % args->timeout) == 0) {
            g_timer_function_manager.functions[i](args);
        }
    }
}

void timer_register_function(TIMER_FUNCTION function, TIMER_FUNC_ARGS *args) {
    uint32 index = 0;
    if (function == NULL || args == NULL) {
        printf("ERROR: failed to register timer function %x\n", function);
        return;
    }
    index = (++g_timer_function_manager.current_index) % MAXIMUM_TIMER_FUNCTIONS;
    g_timer_function_manager.current_index = index;
    g_timer_function_manager.functions[index] = function;
    memcpy(&g_timer_function_manager.func_args[index], args, sizeof(TIMER_FUNC_ARGS));
}

void timer_init() {
    // IRQ0 will fire 100 times per second
    timer_set_frequency(100);
    isr_register_interrupt_handler(IRQ_BASE, timer_handler);
}

void sleep(int sec) {
    uint32 end = g_ticks + sec * g_freq_hz;
    while (g_ticks < end);
}

