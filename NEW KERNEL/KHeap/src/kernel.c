#include "kernel.h"

#include "console.h"
#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "kheap.h"
#include "multiboot.h"
#include "pmm.h"
#include "string.h"

KERNEL_MEMORY_MAP g_kmap;

int get_kernel_memory_map(KERNEL_MEMORY_MAP *kmap, MULTIBOOT_INFO *mboot_info) {
    uint32 i;

    if (kmap == NULL) return -1;
    kmap->kernel.k_start_addr = (uint32)&__kernel_section_start;
    kmap->kernel.k_end_addr = (uint32)&__kernel_section_end;
    kmap->kernel.k_len = ((uint32)&__kernel_section_end - (uint32)&__kernel_section_start);

    kmap->kernel.text_start_addr = (uint32)&__kernel_text_section_start;
    kmap->kernel.text_end_addr = (uint32)&__kernel_text_section_end;
    kmap->kernel.text_len = ((uint32)&__kernel_text_section_end - (uint32)&__kernel_text_section_start);

    kmap->kernel.data_start_addr = (uint32)&__kernel_data_section_start;
    kmap->kernel.data_end_addr = (uint32)&__kernel_data_section_end;
    kmap->kernel.data_len = ((uint32)&__kernel_data_section_end - (uint32)&__kernel_data_section_start);

    kmap->kernel.rodata_start_addr = (uint32)&__kernel_rodata_section_start;
    kmap->kernel.rodata_end_addr = (uint32)&__kernel_rodata_section_end;
    kmap->kernel.rodata_len = ((uint32)&__kernel_rodata_section_end - (uint32)&__kernel_rodata_section_start);

    kmap->kernel.bss_start_addr = (uint32)&__kernel_bss_section_start;
    kmap->kernel.bss_end_addr = (uint32)&__kernel_bss_section_end;
    kmap->kernel.bss_len = ((uint32)&__kernel_bss_section_end - (uint32)&__kernel_bss_section_start);

    kmap->system.total_memory = mboot_info->mem_low + mboot_info->mem_high;

    for (i = 0; i < mboot_info->mmap_length; i += sizeof(MULTIBOOT_MEMORY_MAP)) {
        MULTIBOOT_MEMORY_MAP *mmap = (MULTIBOOT_MEMORY_MAP *)(mboot_info->mmap_addr + i);
        if (mmap->type != MULTIBOOT_MEMORY_AVAILABLE) continue;
        // make sure kernel is loaded at 0x100000 by bootloader(see linker.ld)
        if (mmap->addr_low == kmap->kernel.text_start_addr) {
            // set available memory starting from end of our kernel, leaving 1MB size for functions exceution
            kmap->available.start_addr = kmap->kernel.k_end_addr + 1024 * 1024;
            kmap->available.end_addr = mmap->addr_low + mmap->len_low;
            // get availabel memory in bytes
            kmap->available.size = kmap->available.end_addr - kmap->available.start_addr;
            return 0;
        }
    }

    return -1;
}

void display_kernel_memory_map(KERNEL_MEMORY_MAP *kmap) {
    printf("kernel:\n");
    printf("  kernel-start: 0x%x, kernel-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.k_start_addr, kmap->kernel.k_end_addr, kmap->kernel.k_len);
    printf("  text-start: 0x%x, text-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.text_start_addr, kmap->kernel.text_end_addr, kmap->kernel.text_len);
    printf("  data-start: 0x%x, data-end: 0x%x, TOTAL: %d bytes\n",
           kmap->kernel.data_start_addr, kmap->kernel.data_end_addr, kmap->kernel.data_len);
    printf("  rodata-start: 0x%x, rodata-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.rodata_start_addr, kmap->kernel.rodata_end_addr, kmap->kernel.rodata_len);
    printf("  bss-start: 0x%x, bss-end: 0x%x, TOTAL: %d\n",
           kmap->kernel.bss_start_addr, kmap->kernel.bss_end_addr, kmap->kernel.bss_len);

    printf("total_memory: %d KB\n", kmap->system.total_memory);
    printf("available:\n");
    printf("  start_adddr: 0x%x\n  end_addr: 0x%x\n  size: %d\n",
           kmap->available.start_addr, kmap->available.end_addr, kmap->available.size);
}

void kmain(unsigned long magic, unsigned long addr) {
    MULTIBOOT_INFO *mboot_info;

    gdt_init();
    idt_init();

    console_init(COLOR_WHITE, COLOR_BLACK);
    if (magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        memset(&g_kmap, 0, sizeof(KERNEL_MEMORY_MAP));
        if (get_kernel_memory_map(&g_kmap, mboot_info) < 0) {
            printf("error: failed to get kernel memory map\n");
            return;
        }
        //display_kernel_memory_map(&g_kmap);
        //printf("total_memory: %d KB, %d Bytes\n", g_kmap.system.total_memory, g_kmap.available.size);
        //printf("start_addr: 0x%x, end_addr: 0x%x\n", g_kmap.available.start_addr, g_kmap.available.end_addr);

        // put the memory bitmap at the start of the available memory
        pmm_init(g_kmap.available.start_addr, g_kmap.available.size);

        //printf("Max blocks: %d\n", pmm_get_max_blocks());
        // initialize atleast 256 blocks of region of memory for our heap
        pmm_init_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 256);

        //printf("[KERNEL REGION 0-%d] [ALWAYS IN USE]\n\n", pmm_next_free_frame(1) - 1);
        //printf("before alloc- next free: %d\n", pmm_next_free_frame(1));

        // initialize heap 20 blocks(81920 bytes)
        void *start = pmm_alloc_blocks(20);
        void *end = start + (pmm_next_free_frame(1) * PMM_BLOCK_SIZE);

        kheap_init(start, end);

        // malloc test
        void *p = kmalloc(1024);
        memset(p, 0, 1024);

        void *p2 = kmalloc(258);
        memset(p2, 0, 258);
        kfree(p2);

        void *p3 = kmalloc(256);
        memset(p3, 0, 256);

        void *p4 = kmalloc(4096);
        memset(p4, 0, 4096);
        kfree(p4);

        // calloc test
        int *ptr1;
        int n = 5, i;
        ptr1 = (int *)kcalloc(n, sizeof(int));
        if (ptr1 == NULL) {
            printf("Memory not allocated.\n");
        } else {
            for (i = 0; i < n; ++i) {
                ptr1[i] = i + 1;
            }
            printf("The elements of the array are:\n");
            for (i = 0; i < n; ++i) {
                printf("%d, ", ptr1[i]);
            }
            printf("\n");
            kfree(ptr1);
        }

        printf("\n");
        // realloc test
        int *ptr2;
        n = 5;
        ptr2 = (int *)kcalloc(n, sizeof(int));
        if (ptr2 == NULL) {
            printf("Memory not allocated.\n");
        } else {
            for (i = 0; i < n; ++i) {
                ptr2[i] = i + 1;
            }
            printf("Array elements:\n");
            for (i = 0; i < n; ++i) {
                printf("%d, ", ptr2[i]);
            }
            printf("\n");
            n = 10;
            ptr2 = krealloc(ptr2, n * sizeof(int));
            printf("Memory successfully re-allocated using realloc\n");
            for (i = 5; i < n; ++i) {
                ptr2[i] = i + 1;
            }
            printf("Array elements:\n");
            for (i = 0; i < n; ++i) {
                printf("%d, ", ptr2[i]);
            }
            printf("\n");
            kfree(ptr2);
        }

        printf("\n");
        kheap_print_blocks();

        pmm_free_blocks(start, 20);
        pmm_deinit_region(g_kmap.available.start_addr, PMM_BLOCK_SIZE * 10);
    } else {
        printf("error: invalid multiboot magic number\n");
    }
}
