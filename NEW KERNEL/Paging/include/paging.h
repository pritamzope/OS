#ifndef PAGING_H
#define PAGING_H

#include "isr.h"
#include "types.h"

#define PAGE_SIZE 4096


void paging_init();

void *paging_get_physical_address(void *virtual_addr);
void paging_allocate_page(void *addr);
void paging_free_page(void *virtual_addr);

#endif
