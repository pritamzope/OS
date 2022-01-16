#include "ide.h"
#include "console.h"
#include "io_ports.h"
#include "string.h"

// https://wiki.osdev.org/PCI_IDE_Controller
// https://datacadamia.com/io/drive/lba

IDE_CHANNELS g_ide_channels[MAXIMUM_CHANNELS];
IDE_DEVICE g_ide_devices[MAXIMUM_IDE_DEVICES];

static volatile unsigned char g_ide_irq_invoked = 0;

static uint8 ide_read_register(uint8 channel, uint8 reg);
static void ide_write_register(uint8 channel, uint8 reg, uint8 data);

// read register value from the given channel
static uint8 ide_read_register(uint8 channel, uint8 reg) {
    uint8 ret;

    // write value ata-control to tell irq is ready
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, 0x80 | g_ide_channels[channel].no_intr);

    // read register from base channel port
    if (reg < 0x08)
        ret = inportb(g_ide_channels[channel].base + reg - 0x00);
    else if (reg < 0x0C)
        ret = inportb(g_ide_channels[channel].base + reg - 0x06);
    else if (reg < 0x0E)
        ret = inportb(g_ide_channels[channel].control + reg - 0x0A);
    else if (reg < 0x16)
        ret = inportb(g_ide_channels[channel].bm_ide + reg - 0x0E);

    // write value to tell reading is done
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, g_ide_channels[channel].no_intr);

    return ret;
}

// write data to register to the given channel
static void ide_write_register(uint8 channel, uint8 reg, uint8 data) {
    // write value ata-control to tell irq is ready
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, 0x80 | g_ide_channels[channel].no_intr);

    // write data to register ports
    if (reg < 0x08)
        outportb(g_ide_channels[channel].base + reg - 0x00, data);
    else if (reg < 0x0C)
        outportb(g_ide_channels[channel].base + reg - 0x06, data);
    else if (reg < 0x0E)
        outportb(g_ide_channels[channel].control + reg - 0x0A, data);
    else if (reg < 0x16)
        outportb(g_ide_channels[channel].bm_ide + reg - 0x0E, data);

    // write value to tell reading is done
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, g_ide_channels[channel].no_intr);
}

// read long word from reg port for quads times
void insl(uint16 reg, uint32 *buffer, int quads) {
    int index;
    for (index = 0; index < quads; index++) {
        buffer[index] = inportl(reg);
    }
}

// write long word to reg port for quads times
void outsl(uint16 reg, uint32 *buffer, int quads) {
    int index;
    for (index = 0; index < quads; index++) {
        outportl(reg, buffer[index]);
    }
}

// read collection of value from a channel into given buffer
void ide_read_buffer(uint8 channel, uint8 reg, uint32 *buffer, uint32 quads) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, 0x80 | g_ide_channels[channel].no_intr);

    // get value of data-segment to extra segment by savin glast es value
    asm("pushw %es");
    asm("movw %ds, %ax");
    asm("movw %ax, %es");

    if (reg < 0x08)
        insl(g_ide_channels[channel].base + reg - 0x00, buffer, quads);
    else if (reg < 0x0C)
        insl(g_ide_channels[channel].base + reg - 0x06, buffer, quads);
    else if (reg < 0x0E)
        insl(g_ide_channels[channel].control + reg - 0x0A, buffer, quads);
    else if (reg < 0x16)
        insl(g_ide_channels[channel].bm_ide + reg - 0x0E, buffer, quads);

    asm("popw %es;");

    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, g_ide_channels[channel].no_intr);
}

void ide_write_buffer(uint8 channel, uint8 reg, uint32 *buffer, uint32 quads) {
    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, 0x80 | g_ide_channels[channel].no_intr);

    // get value of data-segment to extra segment by savin glast es value
    asm("pushw %es");
    asm("movw %ds, %ax");
    asm("movw %ax, %es");

    if (reg < 0x08)
        outsl(g_ide_channels[channel].base + reg - 0x00, buffer, quads);
    else if (reg < 0x0C)
        outsl(g_ide_channels[channel].base + reg - 0x06, buffer, quads);
    else if (reg < 0x0E)
        outsl(g_ide_channels[channel].control + reg - 0x0A, buffer, quads);
    else if (reg < 0x16)
        outsl(g_ide_channels[channel].bm_ide + reg - 0x0E, buffer, quads);

    asm("popw %es;");

    if (reg > 0x07 && reg < 0x0C)
        ide_write_register(channel, ATA_REG_CONTROL, g_ide_channels[channel].no_intr);
}

// wait until drive is ready, keep polling ide device until it is not busy status
uint8 ide_polling(uint8 channel, uint8 advanced_check) {
    // (I) Delay 400 nanosecond for BSY to be set:
    for (int i = 0; i < 4; i++)
        // Reading the Alternate Status port wastes 100ns; loop four times.
        ide_read_register(channel, ATA_REG_ALTSTATUS);

    // (II) Wait for BSY to be cleared:
    while (ide_read_register(channel, ATA_REG_STATUS) & ATA_SR_BSY)
        ;  // Wait for BSY to be zero.

    if (advanced_check) {
        // Read Status Register
        uint8 state = ide_read_register(channel, ATA_REG_STATUS);

        // (III) Check For Errors:
        if (state & ATA_SR_ERR)
            return 2;  // Error.

        // (IV) Check If Device fault:
        if (state & ATA_SR_DF)
            return 1;  // Device Fault.

        // (V) Check DRQ:
        // BSY = 0; DF = 0; ERR = 0 so we should check for DRQ now.
        if ((state & ATA_SR_DRQ) == 0)
            return 3;  // DRQ should be set
    }

    return 0;  // No Error.
}

uint8 ide_print_error(uint32 drive, uint8 err) {
    if (err == 0)
        return err;

    printf("IDE:");
    if (err == 1) {
        printf("- Device Fault\n");
        err = 19;
    } else if (err == 2) {
        uint8 st = ide_read_register(g_ide_devices[drive].channel, ATA_REG_ERROR);
        if (st & ATA_ER_AMNF) {
            printf("- No Address Mark Found\n");
            err = 7;
        }
        if (st & ATA_ER_TK0NF) {
            printf("- No Media or Media Error\n");
            err = 3;
        }
        if (st & ATA_ER_ABRT) {
            printf("- Command Aborted\n");
            err = 20;
        }
        if (st & ATA_ER_MCR) {
            printf("- No Media or Media Error\n");
            err = 3;
        }
        if (st & ATA_ER_IDNF) {
            printf("- ID mark not Found\n");
            err = 21;
        }
        if (st & ATA_ER_MC) {
            printf("- No Media or Media Error\n");
            err = 3;
        }
        if (st & ATA_ER_UNC) {
            printf("- Uncorrectable Data Error\n");
            err = 22;
        }
        if (st & ATA_ER_BBK) {
            printf("- Bad Sectors\n");
            err = 13;
        }
    } else if (err == 3) {
        printf("- Reads Nothing\n");
        err = 23;
    } else if (err == 4) {
        printf("- Write Protected\n");
        err = 8;
    }
    printf("- [%s %s] %s\n",
           (const char *[]){"Primary", "Secondary"}[g_ide_devices[drive].channel],
           (const char *[]){"Master", "Slave"}[g_ide_devices[drive].drive],
           g_ide_devices[drive].model);

    return err;
}

/*
prim_channel_base_addr: Primary channel base address(0x1F0-0x1F7)
prim_channel_control_base_addr: Primary channel control base address(0x3F6)
sec_channel_base_addr: Secondary channel base address(0x170-0x177)
sec_channel_control_addr: Secondary channel control base address(0x376)
bus_master_addr: Bus master address(pass 0 for now)
*/
void ide_init(uint32 prim_channel_base_addr, uint32 prim_channel_control_base_addr,
              uint32 sec_channel_base_addr, uint32 sec_channel_control_addr,
              uint32 bus_master_addr) {
    int i, j, k, count = 0;
    unsigned char ide_buf[2048] = {0};

    // 1- Detect I/O Ports which interface IDE Controller:
    // (checking the addr is removed for simplicity, just assigning all ports)
    g_ide_channels[ATA_PRIMARY].base = prim_channel_base_addr;
    g_ide_channels[ATA_PRIMARY].control = prim_channel_control_base_addr;
    g_ide_channels[ATA_SECONDARY].base = sec_channel_base_addr;
    g_ide_channels[ATA_SECONDARY].control = sec_channel_control_addr;
    g_ide_channels[ATA_PRIMARY].bm_ide = bus_master_addr;
    g_ide_channels[ATA_SECONDARY].bm_ide = bus_master_addr;

    // 2- Disable IRQs:
    ide_write_register(ATA_PRIMARY, ATA_REG_CONTROL, 2);
    ide_write_register(ATA_SECONDARY, ATA_REG_CONTROL, 2);

    // 3- Detect ATA-ATAPI Devices:
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            uint8 err = 0, type = IDE_ATA, status;
            g_ide_devices[count].reserved = 0;  // Assuming that no drive here.

            // (I) Select Drive:
            ide_write_register(i, ATA_REG_HDDEVSEL, 0xA0 | (j << 4));  // Select Drive.
            //sleep(1); // Wait 1ms for drive select to work.

            // (II) Send ATA Identify Command:
            ide_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
            //sleep(1); // This function should be implemented in your OS. which waits for 1 ms.
            // it is based on System Timer Device Driver.

            // (III) Polling:
            if (ide_read_register(i, ATA_REG_STATUS) == 0) continue;  // If Status = 0, No Device.

            while (1) {
                status = ide_read_register(i, ATA_REG_STATUS);
                if ((status & ATA_SR_ERR)) {
                    err = 1;
                    break;
                }                                                            // If Err, Device is not ATA.
                if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRQ)) break;  // Everything is right.
            }

            // (IV) Probe for ATAPI Devices:

            if (err != 0) {
                unsigned char cl = ide_read_register(i, ATA_REG_LBA1);
                unsigned char ch = ide_read_register(i, ATA_REG_LBA2);

                if (cl == 0x14 && ch == 0xEB)
                    type = IDE_ATAPI;
                else if (cl == 0x69 && ch == 0x96)
                    type = IDE_ATAPI;
                else
                    continue;  // Unknown Type (may not be a device).

                ide_write_register(i, ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
                //sleep(1);
            }

            // (V) Read Identification Space of the Device:
            ide_read_buffer(i, ATA_REG_DATA, (unsigned int *)ide_buf, 128);

            // (VI) Read Device Parameters:
            g_ide_devices[count].reserved = 1;
            g_ide_devices[count].type = type;
            g_ide_devices[count].channel = i;
            g_ide_devices[count].drive = j;
            g_ide_devices[count].signature = *((unsigned short *)(ide_buf + ATA_IDENT_DEVICETYPE));
            g_ide_devices[count].features = *((unsigned short *)(ide_buf + ATA_IDENT_CAPABILITIES));
            g_ide_devices[count].command_sets = *((unsigned int *)(ide_buf + ATA_IDENT_COMMANDSETS));

            // (VII) Get Size:
            if (g_ide_devices[count].command_sets & (1 << 26))
                // Device uses 48-Bit Addressing:
                g_ide_devices[count].size = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA_EXT));
            else
                // Device uses CHS or 28-bit Addressing:
                g_ide_devices[count].size = *((unsigned int *)(ide_buf + ATA_IDENT_MAX_LBA));

            // (VIII) String indicates model of device (like Western Digital HDD and SONY DVD-RW...):
            for (k = 0; k < 40; k += 2) {
                g_ide_devices[count].model[k] = ide_buf[ATA_IDENT_MODEL + k + 1];
                g_ide_devices[count].model[k + 1] = ide_buf[ATA_IDENT_MODEL + k];
            }
            g_ide_devices[count].model[40] = '\0';  // Terminate String.
            // remove trailing spaces in model string
            for(k = 39; k >= 0; k--) {
                char ch = g_ide_devices[count].model[k];
                if(ch == ' ')
                    g_ide_devices[count].model[k] = '\0';
                else
                    break;
            }

            count++;
        }
    }

    // 4- Print Summary:
    for (i = 0; i < 4; i++)
        if (g_ide_devices[i].reserved == 1) {
            printf("%d:-\n", i);
            printf("  model: %s\n", g_ide_devices[i].model);
            printf("  type: %s\n", (const char *[]){"ATA", "ATAPI"}[g_ide_devices[i].type]);
            printf("  drive: %u, channel: %u\n", g_ide_devices[i].drive, g_ide_devices[i].channel);
            printf("  base: 0x%x, control: 0x%x\n", g_ide_channels[i].base, g_ide_channels[i].control);
            printf("  size: %u sectors, %u bytes\n", g_ide_devices[i].size, g_ide_devices[i].size * ATA_SECTOR_SIZE);
            printf("  signature: 0x%x, features: %d\n", g_ide_devices[i].signature, g_ide_devices[i].features);
        }
}

uint8 ide_ata_access(uint8 direction, uint8 drive, uint32 lba, uint8 num_sectors, uint32 buffer) {
    uint8 lba_mode /* 0: CHS, 1:LBA28, 2: LBA48 */, dma /* 0: No DMA, 1: DMA */, cmd;
    uint8 lba_io[6];
    uint32 channel = g_ide_devices[drive].channel;  // Read the Channel.
    uint32 slavebit = g_ide_devices[drive].drive;   // Read the Drive [Master/Slave]
    uint32 bus = g_ide_channels[channel].base;      // Bus Base, like 0x1F0 which is also data port.
    uint32 words = 256;                             // Almost every ATA drive has a sector-size of 512-byte.
    uint16 cyl, i;
    uint8 head, sect, err;

    ide_write_register(channel, ATA_REG_CONTROL, g_ide_channels[channel].no_intr = (g_ide_irq_invoked = 0x0) + 0x02);

    // (I) Select one from LBA28, LBA48 or CHS;
    if (lba >= 0x10000000) {  // Sure Drive should support LBA in this case, or you are
                              // giving a wrong LBA.
        // LBA48:
        lba_mode = LBA_MODE_48;
        lba_io[0] = (lba & 0x000000FF) >> 0;
        lba_io[1] = (lba & 0x0000FF00) >> 8;
        lba_io[2] = (lba & 0x00FF0000) >> 16;
        lba_io[3] = (lba & 0xFF000000) >> 24;
        lba_io[4] = 0;                                   // LBA28 is integer, so 32-bits are enough to access 2TB.
        lba_io[5] = 0;                                   // LBA28 is integer, so 32-bits are enough to access 2TB.
        head = 0;                                        // Lower 4-bits of HDDEVSEL are not used here.
    } else if (g_ide_devices[drive].features & 0x200) {  // Drive supports LBA?
        // LBA28:
        lba_mode = LBA_MODE_28;
        lba_io[0] = (lba & 0x00000FF) >> 0;
        lba_io[1] = (lba & 0x000FF00) >> 8;
        lba_io[2] = (lba & 0x0FF0000) >> 16;
        lba_io[3] = 0;  // These Registers are not used here.
        lba_io[4] = 0;  // These Registers are not used here.
        lba_io[5] = 0;  // These Registers are not used here.
        head = (lba & 0xF000000) >> 24;
    } else {
        // CHS:
        lba_mode = LBA_MODE_CHS;
        sect = (lba % 63) + 1;
        cyl = (lba + 1 - sect) / (16 * 63);
        lba_io[0] = sect;
        lba_io[1] = (cyl >> 0) & 0xFF;
        lba_io[2] = (cyl >> 8) & 0xFF;
        lba_io[3] = 0;
        lba_io[4] = 0;
        lba_io[5] = 0;
        head = (lba + 1 - sect) % (16 * 63) / (63);  // Head number is written to HDDEVSEL lower 4-bits.
    }

    // (II) See if drive supports DMA or not;
    dma = 0;  // We don't support DMA

    // (III) Wait if the drive is busy;
    while (ide_read_register(channel, ATA_REG_STATUS) & ATA_SR_BSY) {
    }  // Wait if busy.

    // (IV) Select Drive from the controller;
    if (lba_mode == LBA_MODE_CHS)
        ide_write_register(channel, ATA_REG_HDDEVSEL, 0xA0 | (slavebit << 4) | head);  // Drive & CHS.
    else
        ide_write_register(channel, ATA_REG_HDDEVSEL, 0xE0 | (slavebit << 4) | head);  // Drive & LBA

    // (V) Write Parameters;
    if (lba_mode == LBA_MODE_48) {
        ide_write_register(channel, ATA_REG_SECCOUNT1, 0);
        ide_write_register(channel, ATA_REG_LBA3, lba_io[3]);
        ide_write_register(channel, ATA_REG_LBA4, lba_io[4]);
        ide_write_register(channel, ATA_REG_LBA5, lba_io[5]);
    }
    ide_write_register(channel, ATA_REG_SECCOUNT0, num_sectors);
    ide_write_register(channel, ATA_REG_LBA0, lba_io[0]);
    ide_write_register(channel, ATA_REG_LBA1, lba_io[1]);
    ide_write_register(channel, ATA_REG_LBA2, lba_io[2]);

    // (VI) Select the command and send it;
    if (lba_mode == LBA_MODE_CHS && dma == 0 && direction == ATA_READ) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == LBA_MODE_28 && dma == 0 && direction == ATA_READ) cmd = ATA_CMD_READ_PIO;
    if (lba_mode == LBA_MODE_48 && dma == 0 && direction == ATA_READ) cmd = ATA_CMD_READ_PIO_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 1 && direction == ATA_READ) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == LBA_MODE_28 && dma == 1 && direction == ATA_READ) cmd = ATA_CMD_READ_DMA;
    if (lba_mode == LBA_MODE_48 && dma == 1 && direction == ATA_READ) cmd = ATA_CMD_READ_DMA_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 0 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == LBA_MODE_28 && dma == 0 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO;
    if (lba_mode == LBA_MODE_48 && dma == 0 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_PIO_EXT;
    if (lba_mode == LBA_MODE_CHS && dma == 1 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == LBA_MODE_28 && dma == 1 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA;
    if (lba_mode == LBA_MODE_48 && dma == 1 && direction == ATA_WRITE) cmd = ATA_CMD_WRITE_DMA_EXT;
    ide_write_register(channel, ATA_REG_COMMAND, cmd);  // Send the Command.

    if (dma) {
        if (direction == ATA_READ) {
            // DMA Read
        } else {
            // DMA write
        }
    } else if (direction == ATA_READ) {
        // PIO Read.
        for (i = 0; i < num_sectors; i++) {
            if ((err = ide_polling(channel, 1)))
                return err;  // Polling, set error and exit if there is.

            // save es segment and repeat insw(read stream of shorts) instruction util no of sectors are read into buffer
            asm("pushw %es");
            asm("rep insw"
                :
                : "c"(words), "d"(bus), "D"(buffer));  // Receive Data.
            asm("popw %es");
            buffer += (words * 2);
        }
    } else {
        // PIO Write.
        for (i = 0; i < num_sectors; i++) {
            ide_polling(channel, 0);  // Polling.
            // save es segment and repeat outsw(write stream of shorts) instruction util no of sectors are written to ide device
            asm("pushw %ds");
            asm("rep outsw" ::"c"(words), "d"(bus), "S"(buffer));  // Send Data
            asm("popw %ds");
            buffer += (words * 2);
        }
        // send the flush commands
        ide_write_register(channel, ATA_REG_COMMAND, (char[]){ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH, ATA_CMD_CACHE_FLUSH_EXT}[lba_mode]);
        ide_polling(channel, 0);  // Polling.
    }

    return 0;  // Easy, isn't it?
}

void ide_wait_irq() {
    while (!g_ide_irq_invoked)
        ;
    g_ide_irq_invoked = 0;
}
void ide_irq() {
    g_ide_irq_invoked = 1;
}

// start from lba = 0
int ide_read_sectors(uint8 drive, uint8 num_sectors, uint32 lba, uint32 buffer) {
    // 1: Check if the drive presents:
    if (drive > MAXIMUM_IDE_DEVICES || g_ide_devices[drive].reserved == 0) {
        printf("IDE ERROR: Drive not found\n");
        return -1;
    }
    // 2: Check if inputs are valid:
    else if (((lba + num_sectors) > g_ide_devices[drive].size) && (g_ide_devices[drive].type == IDE_ATA)) {
        printf("IDE ERROR: LBA address(0x%x) is greater than the available drive sectors(0x%x)\n", lba, g_ide_devices[drive].size);
        return -2;
    }
    // 3: Read in PIO Mode through Polling & IRQs:
    else {
        uint8 err;
        if (g_ide_devices[drive].type == IDE_ATA)
            err = ide_ata_access(ATA_READ, drive, lba, num_sectors, buffer);
        // print if any error in reading
        return ide_print_error(drive, err);
    }
    return 0;
}

// start from lba = 0
int ide_write_sectors(uint8 drive, uint8 num_sectors, uint32 lba, uint32 buffer) {
    // 1: Check if the drive presents:
    if (drive > MAXIMUM_IDE_DEVICES || g_ide_devices[drive].reserved == 0) {
        printf("IDE ERROR: Drive not found\n");
        return -1;
    }
    // 2: Check if inputs are valid:
    else if (((lba + num_sectors) > g_ide_devices[drive].size) && (g_ide_devices[drive].type == IDE_ATA)) {
        printf("IDE ERROR: LBA address(0x%x) is greater than the available drive sectors(0x%x)\n", lba, g_ide_devices[drive].size);
        return -2;
    } else {
        uint8 err;
        if (g_ide_devices[drive].type == IDE_ATA)
            err = ide_ata_access(ATA_WRITE, drive, lba, num_sectors, buffer);
        // print if any error in writing
        return ide_print_error(drive, err);
    }
    return 0;
}

void ata_init() {
    ide_init(0x1F0, 0x3F6, 0x170, 0x376, 0x000);
}

int ata_get_drive_by_model(const char *model) {
    int i;
    for(i = 0; i < MAXIMUM_IDE_DEVICES; i++) {
        if(strcmp((const char*)g_ide_devices[i].model, (char *)model) == 0)
            return i;
    }
    return -1;
}

