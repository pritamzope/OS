#ifndef IO_PORTS_H
#define IO_PORTS_H

#include "types.h"

/**
 * read a byte from given port number
 */
uint8 inportb(uint16 port);

/**
 * write a given byte to given port number
 */
void outportb(uint16 port, uint8 val);

/**
 * read 2 bytes(short) from given port number
 */
uint16 inports(uint16 port);

/**
 * write given 2(short) bytes to given port number
 */
void outports(uint16 port, uint16 data);

/**
 * read 4 bytes(long) from given port number
 */
uint32 inportl(uint16 port);

/**
 * write given 4 bytes(long) to given port number
 */
void outportl(uint16 port, uint32 data);

#endif
