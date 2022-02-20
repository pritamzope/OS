#include "kheap.h"
#include "console.h"
#include "kernel.h"
#include "pmm.h"
#include "string.h"

// start & end addresses pointing to memory
void *g_kheap_start_addr = NULL, *g_kheap_end_addr = NULL;
unsigned long g_total_size = 0;
unsigned long g_total_used_size = 0;
// list head
KHEAP_BLOCK *g_head = NULL;

/**
 * initialize heap and set total memory size
*/
int kheap_init(void *start_addr, void *end_addr) {
    if (start_addr > end_addr) {
        printf("failed to init kheap\n");
        return -1;
    }
    g_kheap_start_addr = start_addr;
    g_kheap_end_addr = end_addr;
    g_total_size = end_addr - start_addr;
    g_total_used_size = 0;
    return 0;
}

/**
 * increase the heap memory by size & get its address
*/
void *kbrk(int size) {
    void *addr = NULL;
    if (size <= 0)
        return NULL;
    // check memory is available or not
    if ((int)(g_total_size - g_total_used_size) <= size)
        return NULL;
    // add start addr with total previously used memory and difference between each data block pointer
    addr = g_kheap_start_addr + g_total_used_size + size + sizeof(void *);
    g_total_used_size += size + sizeof(void *);
    return addr;
}

/**
 * print list of allocated blocks
*/
void kheap_print_blocks() {
    KHEAP_BLOCK *temp = g_head;
    printf("Block Size: %d\n", sizeof(KHEAP_BLOCK));
    while (temp != NULL) {
        printf("size:%d, free:%d, data: 0x%x, curr: 0x%x, next: 0x%x\n",
               temp->metadata.size, temp->metadata.is_free, temp->data, temp, temp->next);
        temp = temp->next;
    }
}

BOOL is_block_free(KHEAP_BLOCK *block) {
    if (!block)
        return FALSE;
    return (block->metadata.is_free == TRUE);
}

/**
 * this just check freed memory is greater than the required one
*/
KHEAP_BLOCK *worst_fit(int size) {
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (is_block_free(temp)) {
            if ((int)temp->metadata.size >= size)
                return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// allocate a new heap block
KHEAP_BLOCK *allocate_new_block(int size) {
    KHEAP_BLOCK *temp = g_head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    KHEAP_BLOCK *new_block = (KHEAP_BLOCK *)kbrk(sizeof(KHEAP_BLOCK));
    new_block->metadata.is_free = FALSE;
    new_block->metadata.size = size;
    new_block->data = kbrk(size);
    new_block->next = NULL;
    temp->next = new_block;
    return new_block;
}

/**
 * allocate given size if list is null
 * otherwise try some memory allocation algorithm like best fit etc
 * to find best block to allocate
 * # Need to work on internal/external segmentaion problem
*/
void *kmalloc(int size) {
    if (size <= 0)
        return NULL;
    if (g_head == NULL) {
        g_head = (KHEAP_BLOCK *)kbrk(sizeof(KHEAP_BLOCK));
        g_head->metadata.is_free = FALSE;
        g_head->metadata.size = size;
        g_head->next = NULL;
        g_head->data = kbrk(size);
        return g_head->data;
    } else {
        KHEAP_BLOCK *worst = worst_fit(size);
        if (worst == NULL) {
            KHEAP_BLOCK *new_block = allocate_new_block(size);
            new_block->metadata.is_free = FALSE;
            new_block->metadata.size = size;
            new_block->data = kbrk(size);
            return new_block->data;
        } else {
            worst->metadata.is_free = FALSE;
            return worst->data;
        }
    }
    return NULL;
}

/**
 * allocate memory n * size & zeroing out
*/
void *kcalloc(int n, int size) {
    if (n < 0 || size < 0)
        return NULL;
    void *mem = kmalloc(n * size);
    memset(mem, 0, n * size);
    return mem;
}

/**
 * allocate a new block of memory
 * copy previous block data & set free the previous block
*/
void *krealloc(void *ptr, int size) {
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (temp->data == ptr) {
            KHEAP_BLOCK *new_block = allocate_new_block(size);
            if ((int)temp->metadata.size > size)
                memcpy(new_block->data, ptr, size);
            else
                memcpy(new_block->data, ptr, temp->metadata.size);
            temp->metadata.is_free = TRUE;
            return new_block->data;
        }
        temp = temp->next;
    }
    return NULL;
}

/**
 * set free the block
*/
void kfree(void *addr) {
    KHEAP_BLOCK *temp = g_head;
    while (temp != NULL) {
        if (temp->data == addr) {
            temp->metadata.is_free = TRUE;
            return;
        }
        temp = temp->next;
    }
}

