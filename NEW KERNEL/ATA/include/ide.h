#ifndef IDE_H
#define IDE_H

// https://wiki.osdev.org/PCI_IDE_Controller
#include "types.h"

typedef struct {
    uint16 base;  // i/o base port
    uint16 control;  // control port
    uint16 bm_ide; // bus-master ide port
    uint16 no_intr; // no interrupt port
} IDE_CHANNELS;

typedef struct {
    uint8 reserved; // 0 or 1 if drive exists or not
    uint8 channel; // primary(0) or secondary(1)
    uint8 drive; // master(0) or slave(1)
    uint16 type; // drive type- ATA(0), ATAPI(1),
    uint16 signature; // drive signature
    uint16 features; // drive features
    uint32 command_sets; // supported command sets
    uint32 size; // drive size in sectors
    unsigned char model[41]; // drive name
} IDE_DEVICE;

#define MAXIMUM_CHANNELS    2
#define MAXIMUM_IDE_DEVICES    5

// ATA register ports for read/write
#define ATA_REG_DATA         0x00
#define ATA_REG_ERROR        0x01
#define ATA_REG_FEATURES     0x01
#define ATA_REG_SECCOUNT0    0x02
#define ATA_REG_LBA0         0x03
#define ATA_REG_LBA1         0x04
#define ATA_REG_LBA2         0x05
#define ATA_REG_HDDEVSEL     0x06
#define ATA_REG_COMMAND      0x07
#define ATA_REG_STATUS       0x07
#define ATA_REG_SECCOUNT1    0x08
#define ATA_REG_LBA3         0x09
#define ATA_REG_LBA4         0x0A
#define ATA_REG_LBA5         0x0B
#define ATA_REG_CONTROL      0x0C
#define ATA_REG_ALTSTATUS    0x0C
#define ATA_REG_DEVADDRESS   0x0D

// ATA drive status
#define ATA_SR_BSY     0x80    // Busy
#define ATA_SR_DRDY    0x40    // Drive ready
#define ATA_SR_DF      0x20    // Drive write fault
#define ATA_SR_DSC     0x10    // Drive seek complete
#define ATA_SR_DRQ     0x08    // Data request ready
#define ATA_SR_CORR    0x04    // Corrected data
#define ATA_SR_IDX     0x02    // Index
#define ATA_SR_ERR     0x01    // Error

// ATA drive error status
#define ATA_ER_BBK      0x80    // Bad block
#define ATA_ER_UNC      0x40    // Uncorrectable data
#define ATA_ER_MC       0x20    // Media changed
#define ATA_ER_IDNF     0x10    // ID mark not found
#define ATA_ER_MCR      0x08    // Media change request
#define ATA_ER_ABRT     0x04    // Command aborted
#define ATA_ER_TK0NF    0x02    // Track 0 not found
#define ATA_ER_AMNF     0x01    // No address mark


// Channels
#define ATA_PRIMARY      0x00
#define ATA_SECONDARY    0x01

// IDE types
#define IDE_ATA      0x00
#define IDE_ATAPI    0x01

// Command types
#define ATA_CMD_READ_PIO          0x20
#define ATA_CMD_READ_PIO_EXT      0x24
#define ATA_CMD_READ_DMA          0xC8
#define ATA_CMD_READ_DMA_EXT      0x25
#define ATA_CMD_WRITE_PIO         0x30
#define ATA_CMD_WRITE_PIO_EXT     0x34
#define ATA_CMD_WRITE_DMA         0xCA
#define ATA_CMD_WRITE_DMA_EXT     0x35
#define ATA_CMD_CACHE_FLUSH       0xE7
#define ATA_CMD_CACHE_FLUSH_EXT   0xEA
#define ATA_CMD_PACKET            0xA0
#define ATA_CMD_IDENTIFY_PACKET   0xA1
#define ATA_CMD_IDENTIFY          0xEC

// Identify types
#define ATA_IDENT_DEVICETYPE   0
#define ATA_IDENT_CYLINDERS    2
#define ATA_IDENT_HEADS        6
#define ATA_IDENT_SECTORS      12
#define ATA_IDENT_SERIAL       20
#define ATA_IDENT_MODEL        54
#define ATA_IDENT_CAPABILITIES 98
#define ATA_IDENT_FIELDVALID   106
#define ATA_IDENT_MAX_LBA      120
#define ATA_IDENT_COMMANDSETS  164
#define ATA_IDENT_MAX_LBA_EXT  200


#define ATA_SECTOR_SIZE    512

// Directions
#define ATA_READ     0x00
#define ATA_WRITE    0x01

// LBA(Linear Block Address) modes
#define LBA_MODE_48   0x02
#define LBA_MODE_28   0x01
#define LBA_MODE_CHS  0x00

/*
prim_channel_base_addr: Primary channel base address(0x1F0-0x1F7)
prim_channel_control_base_addr: Primary channel control base address(0x3F6)
sec_channel_base_addr: Secondary channel base address(0x170-0x177)
sec_channel_control_addr: Secondary channel control base address(0x376)
bus_master_addr: Bus master address(pass 0 for now)
*/
void ide_init(uint32 prim_channel_base_addr, uint32 prim_channel_control_base_addr,
            uint32 sec_channel_base_addr, uint32 sec_channel_control_addr,
            uint32 bus_master_addr);

void ide_wait_irq();
void ide_irq();

// start from lba = 0
int ide_read_sectors(uint8 drive, uint8 num_sectors, uint32 lba, uint32 buffer);

// start from lba = 0
int ide_write_sectors(uint8 drive, uint8 num_sectors, uint32 lba, uint32 buffer);


void ata_init();
int ata_get_drive_by_model(const char *model);

#endif
