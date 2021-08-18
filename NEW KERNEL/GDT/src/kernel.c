#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"

void display_sections() {
    printf("kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n", &__kernel_section_start, &__kernel_section_end, 
                ((uint32)&__kernel_section_end - (uint32)&__kernel_section_start));

    printf("text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n", &__kernel_text_section_start, &__kernel_text_section_end, 
                ((uint32)&__kernel_text_section_end - (uint32)&__kernel_text_section_start));

    printf("data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n", &__kernel_data_section_start, &__kernel_data_section_end, 
                ((uint32)&__kernel_data_section_end - (uint32)&__kernel_data_section_start));

    printf("rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n", &__kernel_rodata_section_start, &__kernel_rodata_section_end, 
                ((uint32)&__kernel_rodata_section_end - (uint32)&__kernel_rodata_section_start));

    printf("bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n", &__kernel_bss_section_start, &__kernel_bss_section_end, 
                ((uint32)&__kernel_bss_section_end - (uint32)&__kernel_bss_section_start));
}

void kmain() {
    gdt_init();
    console_init(COLOR_WHITE, COLOR_BLACK);
    display_sections();
}


