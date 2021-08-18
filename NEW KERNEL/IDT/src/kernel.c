#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);

    // raise division by zero, eax=0x7b, ecx=0
    asm volatile("\txorl %edx, %edx");
    asm volatile("\tmovl $0x7b, %eax");
    asm volatile("\tmovl $0, %ecx");
    asm volatile("\tidivl %ecx");
}


