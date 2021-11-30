/**
 * Interrupt Descriptor Table(GDT) setup
 */

#ifndef IDT_H
#define IDT_H

#include "types.h"

#define NO_IDT_DESCRIPTORS     256

typedef struct {
    uint16 base_low;          // lower 16 bits 0-15 of the address to jump to when this interrupt fires
    uint16 segment_selector;  // code segment selector in GDT
    uint8 zero;               // unused, always be zero
    uint8 type;               // types trap, interrupt gates
    uint16 base_high;         // upper 16 bits 16-31 of the address to jump to
} __attribute__((packed)) IDT;

typedef struct {
    uint16 limit;         // limit size of all IDT segments
    uint32 base_address;  // base address of the first IDT segment
} __attribute__((packed)) IDT_PTR;


// asm gdt functions, define in load_idt.asm
extern void load_idt(uint32 idt_ptr);

/**
 * fill entries of IDT 
 */
void idt_set_entry(int index, uint32 base, uint16 seg_sel, uint8 flags);

void idt_init();

#endif
