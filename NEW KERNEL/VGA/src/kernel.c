#include "kernel.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "vga.h"
#include "console.h"
#include "mouse.h"
#include "keyboard.h"
#include "bitmap.h"

void kmain() {
    gdt_init();
    idt_init();
	console_init(WHITE, BLACK);
	keyboard_init();
	mouse_init();
	vga_graphics_init();
	vga_graphics_clear_color(WHITE);
	vga_graphics_draw_rect(10, 10, 10, 10, GREEN);

}

