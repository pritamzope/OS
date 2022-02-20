#ifndef PAGING_H
#define PAGING_H

#include "isr.h"
#include "types.h"

#define PAGE_SIZE 4096


typedef struct {
    uint32 present    :1;
    uint32 read_write         :1;
    uint32 user       :1;
    uint32 write_through  :1;
    uint32 cache      :1;
    uint32 accessed     :1;
    uint32 dirty   :1;
    uint32 page_size  :1;
    uint32 global     :1;
    uint32 available  :3;
    uint32 frame      :20;
} PAGE_DIRECTORY;

typedef struct {
    uint32 present    :1;
    uint32 read_write         :1;
    uint32 user       :1;
    uint32 write_through   :1;
    uint32 cache      :1;
    uint32 accessed   :1;
    uint32 dirty      :1;
    uint32 page_size  :1;
    uint32 global     :1;
    uint32 available  :3;
    uint32 frame      :20;
} PAGE_TABLE;


void paging_init();

void *paging_get_physical_address(void *virtual_addr);
void paging_allocate_page(void *addr);
void paging_free_page(void *virtual_addr);

#endif
