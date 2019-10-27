#include "idt.h"
#include "isr.h"

struct IDT idt_entries[16];
struct IDT_PTR idt_first;

extern void load_idt(struct IDT*);

void set_idt_gate(int intnum, uint32 isr)
{
  idt_entries[intnum].base_low = isr & 0xffff;
  idt_entries[intnum].segment_selector = 0x08;
  idt_entries[intnum].zero = 0;
  idt_entries[intnum].type = 0x8E;
  idt_entries[intnum].base_high = (isr >> 16) & 0xffff;
}

void init_idt()
{
  set_idt_gate(0, (uint32)isr_0);
  set_idt_gate(1, (uint32)isr_1);
  set_idt_gate(2, (uint32)isr_2);
  set_idt_gate(3, (uint32)isr_3);
  set_idt_gate(4, (uint32)isr_4);
  set_idt_gate(5, (uint32)isr_5);
  set_idt_gate(6, (uint32)isr_6);
  set_idt_gate(7, (uint32)isr_7);
  set_idt_gate(8, (uint32)isr_8);
  set_idt_gate(9, (uint32)isr_9);
  set_idt_gate(10, (uint32)isr_10);
  set_idt_gate(11, (uint32)isr_11);
  set_idt_gate(12, (uint32)isr_12);
  set_idt_gate(13, (uint32)isr_13);
  set_idt_gate(14, (uint32)isr_14);
  set_idt_gate(15, (uint32)isr_15);

  idt_first.limit_size = sizeof(idt_entries) - 1;
  idt_first.base_address = (struct IDT*)&idt_entries;

  load_idt((struct IDT*)&idt_first);
}

