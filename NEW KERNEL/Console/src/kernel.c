#include "kernel.h"
#include "console.h"
#include "string.h"

void kmain() {
    console_init(COLOR_WHITE, COLOR_BLACK);
    printf("Hello World\n");
    printf("Decimal: %d\n", -12345);
    printf("Unsigned: %u\n", -12345);
    printf("Hex: 0x%x\n", 12345);
    printf("Loop:-\n");
    for(int i = 0; i < 10; i++) {
        printf("\ti = %d\n", i);
    }

}


