#include "paging.h"

#include "console.h"
#include "io_ports.h"
#include "pmm.h"
#include "string.h"

BOOL g_is_paging_enabled = FALSE;
PAGE_DIRECTORY g_page_directory[1024] __attribute__((aligned(4096)));
PAGE_TABLE g_page_tables[1024] __attribute__((aligned(4096)));

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

    memset(g_page_directory, 0, sizeof(PAGE_DIRECTORY) * 1024);
    memset(g_page_tables, 0, sizeof(PAGE_TABLE) * 1024);

    // set all page directory read/write & user access
    for (i = 0; i < 1024; i++) {
        g_page_directory[i].read_write = 1;
        g_page_directory[i].user = 1;
    }

    // fill all the entries in page table to map all 4MB memory
    for (i = 0; i < 1024; i++) {
        g_page_tables[i].present = 1;
        g_page_tables[i].read_write = 1;
        g_page_tables[i].user = 1;
        g_page_tables[i].frame = (i * PAGE_SIZE) >> 12;
    }

    // set first page directory to be accessed with frame 0x11a(kernel region address)
    g_page_directory[0].present = 1;
    g_page_directory[0].accessed = 0;
    g_page_directory[0].user = 1;
    g_page_directory[0].frame = 0x11a;

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

// convert given virtual address to its physical address
void *paging_get_physical_address(void *virtual_addr) {
    if (!g_is_paging_enabled) {
        return virtual_addr;
    }
    // get page directory, table & frame indexes
    uint32 page_dir_index = (uint32)virtual_addr >> 22;
    uint32 page_table_index = (uint32)virtual_addr >> 12 & 0x03FF;
    uint32 page_frame_offset = (uint32)virtual_addr & 0xfff;
    if (!g_page_directory[page_dir_index].present) {
        printf("physical address: page directory entry does not exists\n");
        return NULL;
    }
    if (!g_page_tables[page_table_index].present) {
        printf("physical address: page table entry does not exist\n");
        return NULL;
    }
    uint32 addr = g_page_tables[page_table_index].frame;
    addr = (addr << 12) + page_frame_offset;
    return (void *)addr;
}

// allocate page by calling pmm alloca block
void paging_allocate_page(void *virtual_addr) {
    if (!g_is_paging_enabled) {
        return;
    }
    uint32 page_dir_index = (uint32)virtual_addr >> 22;
    uint32 page_table_index = (uint32)virtual_addr >> 12 & 0x03FF;

    // if page directory is not currently present, then allocate a new one
    if (!g_page_directory[page_dir_index].present) {
        printf("alloc: page directory entry does not exists for 0x%x\n", virtual_addr);
        g_page_directory[page_dir_index].present = 1;
        g_page_directory[page_dir_index].accessed = 1;
        g_page_directory[page_dir_index].read_write = 0;
        g_page_directory[page_dir_index].user = 1;
        uint32 addr = (uint32)pmm_alloc_block();
        g_page_directory[page_dir_index].frame = addr >> 12;
        return;
    }
    if (!g_page_tables[page_table_index].present) {
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

    if (!g_page_directory[page_dir_index].present) {
        printf("free: page directory entry does not exists\n");
        return;
    }
    if (!g_page_tables[page_table_index].present) {
        printf("free: page table entry does not exists\n");
        return;
    }
    // clear out from directory & table as we have allocated all the tables in paging_init()
    g_page_directory[page_dir_index].present = 0;
    g_page_directory[page_dir_index].accessed = 0;
    g_page_directory[page_dir_index].frame = 0;
    g_page_tables[page_table_index].present = 0;
    g_page_tables[page_table_index].accessed = 0;
    g_page_tables[page_table_index].frame = 0;
}
