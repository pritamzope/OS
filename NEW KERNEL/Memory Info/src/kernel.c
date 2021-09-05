#include "kernel.h"
#include "console.h"
#include "string.h"
#include "multiboot.h"

void kmain(unsigned long magic, unsigned long addr) {
    MULTIBOOT_INFO *mboot_info;
    uint32 i;

    console_init(COLOR_WHITE, COLOR_BLACK);
    printf("magic: 0x%x\n", magic);
    if(magic == MULTIBOOT_BOOTLOADER_MAGIC) {
        mboot_info = (MULTIBOOT_INFO *)addr;
        printf("  flags: 0x%x\n", mboot_info->flags);
        printf("  mem_low: 0x%x KB\n", mboot_info->mem_low);
        printf("  mem_high: 0x%x KB\n", mboot_info->mem_high);
        printf("  boot_device: 0x%x\n", mboot_info->boot_device);
        printf("  cmdline: %s\n", (char *)mboot_info->cmdline);
        printf("  modules_count: %d\n", mboot_info->modules_count);
        printf("  modules_addr: 0x%x\n", mboot_info->modules_addr);
        printf("  mmap_length: %d\n", mboot_info->mmap_length);
        printf("  mmap_addr: 0x%x\n", mboot_info->mmap_addr);
        printf("  memory map:-\n");
        for (i = 0; i < mboot_info->mmap_length; i += sizeof(MULTIBOOT_MEMORY_MAP)) {
            MULTIBOOT_MEMORY_MAP *mmap = (MULTIBOOT_MEMORY_MAP *)(mboot_info->mmap_addr + i);
            printf("    size: %d, addr: 0x%x%x, len: %d%d, type: %d\n", 
                    mmap->size, mmap->addr_low, mmap->addr_high, mmap->len_low, mmap->len_high, mmap->type);

            if (mmap->type == MULTIBOOT_MEMORY_AVAILABLE) {
                /**** Available memory  ****/
            }
        }
        printf("  boot_loader_name: %s\n", (char *)mboot_info->boot_loader_name);
        printf("  vbe_control_info: 0x%x\n", mboot_info->vbe_control_info);
        printf("  vbe_mode_info: 0x%x\n", mboot_info->vbe_mode_info);
        printf("  framebuffer_addr: 0x%x\n", mboot_info->framebuffer_addr);
        printf("  framebuffer_width: %d\n", mboot_info->framebuffer_width);
        printf("  framebuffer_height: %d\n", mboot_info->framebuffer_height);
        printf("  framebuffer_type: %d\n", mboot_info->framebuffer_type);
    } else {
        printf("invalid multiboot magic number\n");
    }
}


