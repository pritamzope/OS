#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

#define NULL 0

extern uint8 inb(uint16 port);
extern void outb(uint16 port, uint8 data);

#endif
