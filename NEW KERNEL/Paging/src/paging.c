#include "paging.h"

#include "console.h"
#include "io_ports.h"
#include "pmm.h"
#include "string.h"

BOOL g_is_paging_enabled = FALSE;

/*
References:-

https://wiki.osdev.org/Paging
https://wiki.osdev.org/Setting_Up_Paging


Page Directory:-

bit 0: present
bit 1: read_write
bit 2: user/supervisor
bit 3: write_through
bit 4: cache  
bit 5: accessed 
bit 6: dirty 
bit 7: page_size
bit 8-11: available
bit 12-31: frame

Page Table:-

bit 0: present
bit 1: read_write
bit 2: user/supervisor
bit 3: write_through
bit 4: cache  
bit 5: accessed 
bit 6: dirty 
bit 7: page_size
bit 8: global
bit 9-11: available
bit 12-31: frame

*/

uint32 g_page_directory[1024] __attribute__((aligned(4096)));
uint32 g_page_tables[1024] __attribute__((aligned(4096)));


void page_fault_handler(REGISTERS *r) {
    uint32 faulting_address;
    asm volatile("mov %%cr2, %0"
                 : "=r"(faulting_address));
    printf("Segmentation fault 0x%x\n", faulting_address);
    while (1)
        ;
}

void paging_init() {
    int i;
    uint32 cr0;

    memset(g_page_directory, 0, sizeof(g_page_directory));
    memset(g_page_tables, 0, sizeof(g_page_tables));

    for (i = 0; i < 1024; i++) {
        // set present and read/write bits
        g_page_directory[i] = 0x00000002;
    }

    for (i = 0; i < 1024; i++) {
        // set present, read/write, suprevisor and frame address starting from 4096
        g_page_tables[i] = (i * PAGE_SIZE) | 3;
    }

    // set supervisor level, read/write, present
    g_page_directory[0] = ((unsigned int)g_page_tables) | 3;

    // add page fault interrupt handler
    isr_register_interrupt_handler(14, page_fault_handler);

    // set cr3 point to page directory
    asm volatile("mov %0, %%cr3" ::"r"(g_page_directory));

    // set bit in cr0 to enable paging
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 = cr0 | 0x80000000;
    asm volatile("mov %0, %%cr0" ::"r"(cr0));

    g_is_paging_enabled = TRUE;
}


#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))


// convert given virtual address to its physical address
void *paging_get_physical_address(void *virtual_addr) {
    if (!g_is_paging_enabled) {
        return virtual_addr;
    }
    // get page directory, table & frame indexes

    // TODO Requires detailed analysis of memory frame allocation and page directory/table entries

    uint32 page_dir_index = (uint32)virtual_addr >> 22;
    uint32 page_table_index = (uint32)virtual_addr >> 12 & 0x03FF;
    uint32 page_frame_offset = (uint32)virtual_addr & 0xfff;
    if (!CHECK_BIT(g_page_directory[page_dir_index], 1)) {
        printf("physical address: page directory entry does not exists\n");
        return NULL;
    }
    if (!CHECK_BIT(g_page_tables[page_table_index], 1)) {
        printf("physical address: page table entry does not exist\n");
        return NULL;
    }
    uint32 addr = g_page_tables[page_table_index] >> 11;
    addr = (addr << 12) + page_frame_offset;
    return (void *)addr;
    return NULL;
}

// allocate page by calling pmm alloca block
void paging_allocate_page(void *virtual_addr) {
    if (!g_is_paging_enabled) {
        return;
    }

    // TODO Requires detailed analysis of memory frame allocation and page directory/table entries

    uint32 page_dir_index = (uint32)virtual_addr >> 22;
    uint32 page_table_index = (uint32)virtual_addr >> 12 & 0x03FF;

    // if page directory is not currently present, then allocate a new one
    if (!CHECK_BIT(g_page_directory[page_dir_index], 1)) {
        printf("alloc: page directory entry does not exists for 0x%x\n", virtual_addr);
        // set present, read/write, user and cache accessed,
        g_page_directory[page_dir_index] = 27;
        uint32 addr = (uint32)pmm_alloc_block();
        g_page_directory[page_dir_index] |= ((addr >> 12) << 11);
        return;
    }
    if (!CHECK_BIT(g_page_tables[page_table_index], 1)) {
        printf("alloc: page table entry does not exists for 0x%x\n", virtual_addr);
        return;
    }
}

// clear the present, accessed & frame from page tables
void paging_free_page(void *virtual_addr) {
    if (!g_is_paging_enabled) {
        return;
    }
    uint32 page_dir_index = (uint32)virtual_addr >> 22;
    uint32 page_table_index = (uint32)virtual_addr >> 12 & 0x03FF;

    if (!CHECK_BIT(g_page_directory[page_dir_index], 1)) {
        printf("free: page directory entry does not exists\n");
        return;
    }
    if (!CHECK_BIT(g_page_tables[page_table_index], 1)) {
        printf("free: page table entry does not exists\n");
        return;
    }
    // clear out from directory & table as we have allocated all the tables in paging_init()
    g_page_directory[page_dir_index] = 0;
}
