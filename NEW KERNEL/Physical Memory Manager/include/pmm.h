#ifndef PMM_H
#define PMM_H

#include "types.h"

typedef uint32 PMM_PHYSICAL_ADDRESS;

/*
 Make sure you pass memory to qemu using -m option
*/

#define PMM_BLOCK_SIZE    4096


typedef struct {
    uint32 memory_size;
    uint32 max_blocks;
    uint32 *memory_map_array;
    uint32 memory_map_end;
    uint32 used_blocks;
} PMM_INFO;


uint32 pmm_get_max_blocks();
uint32 pmm_get_used_blocks();

/**
 * returns index of bitmap array if no of bits(size) are free
 */
int pmm_next_free_frame(int size);

/**
 * initialize memory bitmap array by making blocks from total memory size
 * 
 */
void pmm_init(PMM_PHYSICAL_ADDRESS bitmap, uint32 total_memory_size);
/**
 * initialize/request for a free region of region_size from pmm
 */
void pmm_init_region(PMM_PHYSICAL_ADDRESS base, uint32 region_size);

/**
 * de-initialize/free allocated region of region_size from pmm
 */
void pmm_deinit_region(PMM_PHYSICAL_ADDRESS base, uint32 region_size);

/**
 * request to allocate a single block of memory from pmm
 */
void* pmm_alloc_block();

/**
 * free given requested single block of memory from pmm
 */
void pmm_free_block(void* p);

/**
 * request to allocate no of blocks of memory from pmm
 */
void* pmm_alloc_blocks(uint32 size);

/**
 * free given requested no of blocks of memory from pmm
 */
void pmm_free_blocks(void* p, uint32 size);

#endif
