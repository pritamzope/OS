/**
 * 8259 Programmable Interrupt Controller(8259 PIC) setup
 */

#ifndef _8259_PIC_H
#define _8259_PIC_H

#include "types.h"

/* for more, see https://wiki.osdev.org/8259_PIC */
#define PIC1            0x20  /* IO base address for master PIC */
#define PIC2            0xA0  /* IO base address for slave PIC */
#define PIC1_COMMAND    PIC1
#define PIC1_DATA       (PIC1+1)    /* master data */
#define PIC2_COMMAND    PIC2
#define PIC2_DATA       (PIC2+1)    /* slave data */

#define PIC_EOI         0x20   /* end of interrupt */

#define ICW1            0x11    /* interrupt control command word PIC for initialization */
#define ICW4_8086       0x01    /* 8086/88 (MCS-80/85) mode */

/**
 * initialize 8259 PIC with default IRQ's defined in isr.h
 */
void pic8259_init();

/**
 * send end of interrupt command to PIC 8259
 */
void pic8259_eoi(uint8 irq);

#endif

