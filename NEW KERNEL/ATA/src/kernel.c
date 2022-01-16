#include "kernel.h"
#include "console.h"
#include "gdt.h"
#include "ide.h"
#include "idt.h"
#include "keyboard.h"
#include "string.h"

void kmain() {
    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    //keyboard_init();
    ata_init();

    printf("\nExample\n");
    const int DRIVE = ata_get_drive_by_model("VBOX HARDDISK");
    const uint32 LBA = 0;
    const uint8 NO_OF_SECTORS = 1;
    char buf[ATA_SECTOR_SIZE] = {0};

    struct example {
        int id;
        char name[32];
    };

    struct example e;
    e.id = 10012;
    strcpy(e.name, "Iron Man");

    // write message to drive
    strcpy(buf, "Hello World");
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &e, sizeof(e));
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + 1, (uint32)buf);
    printf("data written\n");

    // read message from drive
    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA, (uint32)buf);
    printf("read data: %s\n", buf);

    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + 1, (uint32)buf);
    memcpy(&e, buf, sizeof(e));
    printf("id: %d, name: %s\n", e.id, e.name);

}
