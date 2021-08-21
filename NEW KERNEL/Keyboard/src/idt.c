/**
 * Interrupt Descriptor Table(GDT) setup
 */

#include "idt.h"
#include "isr.h"
#include "8259_pic.h"

IDT g_idt[NO_IDT_DESCRIPTORS];
IDT_PTR g_idt_ptr;

/**
 * fill entries of IDT 
 */
void idt_set_entry(int index, uint32 base, uint16 seg_sel, uint8 flags) {
    IDT *this = &g_idt[index];

    this->base_low = base & 0xFFFF;
    this->segment_selector = seg_sel;
    this->zero = 0;
    this->type = flags | 0x60;
    this->base_high = (base >> 16) & 0xFFFF;
}

void idt_init() {
    g_idt_ptr.base_address = (uint32)g_idt;
    g_idt_ptr.limit = sizeof(g_idt) - 1;
    pic8259_init();

    idt_set_entry(0, (uint32)exception_0, 0x08, 0x8E);
    idt_set_entry(1, (uint32)exception_1, 0x08, 0x8E);
    idt_set_entry(2, (uint32)exception_2, 0x08, 0x8E);
    idt_set_entry(3, (uint32)exception_3, 0x08, 0x8E);
    idt_set_entry(4, (uint32)exception_4, 0x08, 0x8E);
    idt_set_entry(5, (uint32)exception_5, 0x08, 0x8E);
    idt_set_entry(6, (uint32)exception_6, 0x08, 0x8E);
    idt_set_entry(7, (uint32)exception_7, 0x08, 0x8E);
    idt_set_entry(8, (uint32)exception_8, 0x08, 0x8E);
    idt_set_entry(9, (uint32)exception_9, 0x08, 0x8E);
    idt_set_entry(10, (uint32)exception_10, 0x08, 0x8E);
    idt_set_entry(11, (uint32)exception_11, 0x08, 0x8E);
    idt_set_entry(12, (uint32)exception_12, 0x08, 0x8E);
    idt_set_entry(13, (uint32)exception_13, 0x08, 0x8E);
    idt_set_entry(14, (uint32)exception_14, 0x08, 0x8E);
    idt_set_entry(15, (uint32)exception_15, 0x08, 0x8E);
    idt_set_entry(16, (uint32)exception_16, 0x08, 0x8E);
    idt_set_entry(17, (uint32)exception_17, 0x08, 0x8E);
    idt_set_entry(18, (uint32)exception_18, 0x08, 0x8E);
    idt_set_entry(19, (uint32)exception_19, 0x08, 0x8E);
    idt_set_entry(20, (uint32)exception_20, 0x08, 0x8E);
    idt_set_entry(21, (uint32)exception_21, 0x08, 0x8E);
    idt_set_entry(22, (uint32)exception_22, 0x08, 0x8E);
    idt_set_entry(23, (uint32)exception_23, 0x08, 0x8E);
    idt_set_entry(24, (uint32)exception_24, 0x08, 0x8E);
    idt_set_entry(25, (uint32)exception_25, 0x08, 0x8E);
    idt_set_entry(26, (uint32)exception_26, 0x08, 0x8E);
    idt_set_entry(27, (uint32)exception_27, 0x08, 0x8E);
    idt_set_entry(28, (uint32)exception_28, 0x08, 0x8E);
    idt_set_entry(29, (uint32)exception_29, 0x08, 0x8E);
    idt_set_entry(30, (uint32)exception_30, 0x08, 0x8E);
    idt_set_entry(31, (uint32)exception_31, 0x08, 0x8E);
    idt_set_entry(32, (uint32)irq_0, 0x08, 0x8E);
    idt_set_entry(33, (uint32)irq_1, 0x08, 0x8E);
    idt_set_entry(34, (uint32)irq_2, 0x08, 0x8E);
    idt_set_entry(35, (uint32)irq_3, 0x08, 0x8E);
    idt_set_entry(36, (uint32)irq_4, 0x08, 0x8E);
    idt_set_entry(37, (uint32)irq_5, 0x08, 0x8E);
    idt_set_entry(38, (uint32)irq_6, 0x08, 0x8E);
    idt_set_entry(39, (uint32)irq_7, 0x08, 0x8E);
    idt_set_entry(40, (uint32)irq_8, 0x08, 0x8E);
    idt_set_entry(41, (uint32)irq_9, 0x08, 0x8E);
    idt_set_entry(42, (uint32)irq_10, 0x08, 0x8E);
    idt_set_entry(43, (uint32)irq_11, 0x08, 0x8E);
    idt_set_entry(44, (uint32)irq_12, 0x08, 0x8E);
    idt_set_entry(45, (uint32)irq_13, 0x08, 0x8E);
    idt_set_entry(46, (uint32)irq_14, 0x08, 0x8E);
    idt_set_entry(47, (uint32)irq_15, 0x08, 0x8E);
    idt_set_entry(128, (uint32)exception_128, 0x08, 0x8E);

    load_idt((uint32)&g_idt_ptr);
    asm volatile("sti");
}

