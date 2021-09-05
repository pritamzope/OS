#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include "types.h"

#define MULTIBOOT_MAGIC_HEADER      0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC  0x2BADB002

/* The Multiboot header. */
typedef struct {
    uint32 magic;
    uint32 flags;
    uint32 checksum;
    uint32 header_addr;
    uint32 load_addr;
    uint32 load_end_addr;
    uint32 bss_end_addr;
    uint32 entry_addr;
} MULTIBOOT_HEADER;

/* The symbol table for a.out. */
typedef struct {
    uint32 tabsize;
    uint32 strsize;
    uint32 addr;
    uint32 reserved;
} AOUT_SYMBOL_TABLE;

/* The section header table for ELF. */
typedef struct {
    uint32 num;
    uint32 size;
    uint32 addr;
    uint32 shndx;
} ELF_SECTION_HEADER_TABLE;

typedef struct {
    /* required, defined in entry.asm */
    uint32 flags;

    /* available low-high memory from BIOS, present if flags[0] is set(MEMINFO in entry.asm) */
    uint32 mem_low;
    uint32 mem_high;

    /* "root" partition, present if flags[1] is set(BOOTDEVICE in entry.asm) */
    uint32 boot_device;

    /* kernel command line, present if flags[2] is set(CMDLINE in entry.asm) */
    uint32 cmdline;

    /* no of modules loaded, present if flags[3] is set(MODULECOUNT in entry.asm) */
    uint32 modules_count;
    uint32 modules_addr;

    /* symbol table info, present if flags[4] & flags[5] is set(SYMT in entry.asm) */
    union {
        AOUT_SYMBOL_TABLE aout_sym;
        ELF_SECTION_HEADER_TABLE elf_sec;
    } u;

    /* memory mapping, present if flags[6] is set(MEMMAP in entry.asm) */
    uint32 mmap_length;
    uint32 mmap_addr;

    /* drive info, present if flags[7] is set(DRIVE in entry.asm) */
    uint32 drives_length;
    uint32 drives_addr;

    /* ROM configuration table, present if flags[8] is set(CONFIGT in entry.asm) */
    uint32 config_table;

    /* boot loader name, present if flags[9] is set(BOOTLDNAME in entry.asm) */
    uint32 boot_loader_name;

    /* Advanced Power Management(APM) table, present if flags[10] is set(APMT in entry.asm) */
    uint32 apm_table;

    /* video info, present if flags[11] is set(VIDEO in entry.asm) */
    uint32 vbe_control_info;
    uint32 vbe_mode_info;
    uint16 vbe_mode;
    uint16 vbe_interface_seg;
    uint16 vbe_interface_off;
    uint16 vbe_interface_len;

    /* video framebufer info, present if flags[12] is set(VIDEO_FRAMEBUF in entry.asm)  */
    uint64 framebuffer_addr;
    uint32 framebuffer_pitch;
    uint32 framebuffer_width;
    uint32 framebuffer_height;
    uint8 framebuffer_bpp;
    uint8 framebuffer_type;  // indexed = 0, RGB = 1, EGA = 2

} MULTIBOOT_INFO;


typedef enum {
    MULTIBOOT_MEMORY_AVAILABLE = 1,
    MULTIBOOT_MEMORY_RESERVED,
    MULTIBOOT_MEMORY_ACPI_RECLAIMABLE,
    MULTIBOOT_MEMORY_NVS,
    MULTIBOOT_MEMORY_BADRAM
} MULTIBOOT_MEMORY_TYPE;

typedef struct {
    uint32 size;
    uint32 addr_low;
    uint32 addr_high;
    uint32 len_low;
    uint32 len_high;
    MULTIBOOT_MEMORY_TYPE type;
} MULTIBOOT_MEMORY_MAP;

#endif
