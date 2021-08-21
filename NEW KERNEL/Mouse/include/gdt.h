/**
 * Global Descriptor Table(GDT) setup
 */

#ifndef GDT_H
#define GDT_H

#include "types.h"

#define NO_GDT_DESCRIPTORS     8

typedef struct {
    uint16 segment_limit;  // segment limit first 0-15 bits
    uint16 base_low;       // base first 0-15 bits
    uint8 base_middle;     // base 16-23 bits
    uint8 access;          // access byte
    uint8 granularity;     // high 4 bits (flags) low 4 bits (limit 4 last bits)(limit is 20 bit wide)
    uint8 base_high;       // base 24-31 bits
} __attribute__((packed)) GDT;

typedef struct {
    uint16 limit;       // limit size of all GDT segments
    uint32 base_address;  // base address of the first GDT segment
} __attribute__((packed)) GDT_PTR;

// asm gdt functions, define in load_gdt.asm
extern void load_gdt(uint32 gdt_ptr);

/**
 * fill entries of GDT 
 */
void gdt_set_entry(int index, uint32 base, uint32 limit, uint8 access, uint8 gran);

// initialize GDT
void gdt_init();

#endif
