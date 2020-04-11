#ifndef KERNEL_H
#define KERNEL_H

#include "types.h"

#define NULL 0

uint8 inb(uint16 port);
void outb(uint16 port, uint8 data);

#endif
