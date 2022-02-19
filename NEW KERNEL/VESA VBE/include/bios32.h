#ifndef BIOS32_H
#define BIOS32_H

#include "types.h"
#include "isr.h"

// defined in bios32_call.asm
extern void BIOS32_START();
extern void BIOS32_END();
extern void *bios32_gdt_ptr;
extern void *bios32_gdt_entries;
extern void *bios32_idt_ptr;
extern void *bios32_in_reg16_ptr;
extern void *bios32_out_reg16_ptr;
extern void *bios32_int_number_ptr;

#define REBASE_ADDRESS(x)  (void*)(0x7c00 + (void*)x - (uint32)BIOS32_START)

#define BIOS_CONVENTIONAL_MEMORY 0x7E00


// https://en.wikipedia.org/wiki/BIOS_interrupt_call

void bios32_init();
// bios interrupt call
void int86(uint8 interrupt, REGISTERS16 *in, REGISTERS16 *out);

#endif
