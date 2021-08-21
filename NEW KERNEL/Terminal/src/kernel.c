#include "kernel.h"
#include "console.h"
#include "string.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"

void __cpuid(uint32 type, uint32 *eax, uint32 *ebx, uint32 *ecx, uint32 *edx) {
    asm volatile("cpuid"
                : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                : "0"(type)); // put the type into eax
}

void cpuid_info() {
    uint32 brand[12];
    uint32 eax, ebx, ecx, edx;
    uint32 type;

    memset(brand, 0, sizeof(brand));
    __cpuid(0x80000002, (uint32 *)brand+0x0, (uint32 *)brand+0x1, (uint32 *)brand+0x2, (uint32 *)brand+0x3);
    __cpuid(0x80000003, (uint32 *)brand+0x4, (uint32 *)brand+0x5, (uint32 *)brand+0x6, (uint32 *)brand+0x7);
    __cpuid(0x80000004, (uint32 *)brand+0x8, (uint32 *)brand+0x9, (uint32 *)brand+0xa, (uint32 *)brand+0xb);
    printf("Brand: %s\n", brand);
    for(type = 0; type < 4; type++) {
        __cpuid(type, &eax, &ebx, &ecx, &edx);
        printf("type:0x%x, eax:0x%x, ebx:0x%x, ecx:0x%x, edx:0x%x\n", type, eax, ebx, ecx, edx);
    }
}

BOOL is_echo(char *b) {
    if((b[0]=='e')&&(b[1]=='c')&&(b[2]=='h')&&(b[3]=='o'))
        if(b[4]==' '||b[4]=='\0')
            return TRUE;
    return FALSE;
}

void kmain() {
    char buffer[255];
    const char *shell = "shell@os>";

    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    keyboard_init();

    printf("starting terminal...\n");
    while(1) {
        printf(shell);
        memset(buffer, 0, sizeof(buffer));
        getstr_bound(buffer, strlen(shell));
        if (strlen(buffer) == 0)
            continue;
        if(strcmp(buffer, "cpuid") == 0) {
            cpuid_info();
        } else if(strcmp(buffer, "help") == 0) {
            printf("Tiny OS Terminal\n");
            printf("Commands: help, cpuid, echo\n");
        } else if(is_echo(buffer)) {
            printf("%s\n", buffer + 5);
        } else {
            printf("invalid command: %s\n", buffer);
        }
    }
}


