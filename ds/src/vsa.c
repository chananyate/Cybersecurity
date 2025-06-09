#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "vsa.h"

typedef size_t OffsetType;

typedef struct FreeBlockInfo/*Struct for holding free block information*/
{
    OffsetType size;
    struct FreeBlockInfo *next_by_size;
    struct FreeBlockInfo *next_by_offset;
} FreeBlockInfo;

typedef struct vsa
{
    FreeBlockInfo *free_blocks_by_size;/*Frees blocks sorted by size*/
    FreeBlockInfo *free_blocks_by_offset;/*Frees blocks sorted by offset*/
    size_t total_size;
    size_t free_size;
} vsa_t;

vsa_t *VsaInit(void *pool, size_t total_size)/*Initializes the allocator*/
{
    FreeBlockInfo *initial_block = NULL;
    vsa_t *allocator = (vsa_t *)pool;
    allocator->total_size = total_size;
    allocator->free_size = total_size - sizeof(vsa_t);

    initial_block = (FreeBlockInfo *)((char *)pool + sizeof(vsa_t));/*Creates the initial free block*/

    initial_block->size = allocator->free_size;
    initial_block->next_by_size = NULL;
    initial_block->next_by_offset = NULL;

    allocator->free_blocks_by_size = initial_block;
    allocator->free_blocks_by_offset = initial_block;

    return allocator;
}

void insert_by_size(vsa_t *allocator, FreeBlockInfo *block)/*Inserts a block into the size-sorted list*/
{
    FreeBlockInfo **current = NULL;

    current = &(allocator->free_blocks_by_size);
    while (*current && (*current)->size < block->size)
    {
        current = &(*current)->next_by_size;
    }
    block->next_by_size = *current;
    *current = block;
}

void insert_by_offset(vsa_t *allocator, FreeBlockInfo *block)/*Inserts a block into the offset-sorted list*/
{
    FreeBlockInfo **current = NULL;

    current = &(allocator->free_blocks_by_offset);
    while (*current && *current < block)
    {
        current = &(*current)->next_by_offset;
    }
    block->next_by_offset = *current;
    *current = block;
}

void *VsaAlloc(vsa_t *vsa_mngr, size_t size)/*Allocates a block of memory*/
{
    FreeBlockInfo **current = NULL;
    FreeBlockInfo *best_fit = NULL;
    FreeBlockInfo **offset_current = NULL;
    if (vsa_mngr->free_size < size)
    {
        return NULL; 
    }

    current = &(vsa_mngr->free_blocks_by_size);/*best-fit strategy. It initializes current to point to the head of the free blocks list sorted by size. Thus, it finds the smallest block that fits the requested size.*/

    while (*current && (*current)->size < size)
    {
        current = &(*current)->next_by_size;
    }

    if (!*current)
    {
        return NULL;/*No suitable block found*/
    }

    best_fit = *current;

    *current = best_fit->next_by_size;/*Removes block from the free lists*/

    offset_current = &(vsa_mngr->free_blocks_by_offset);
    while (*offset_current && *offset_current != best_fit)
    {
        offset_current = &(*offset_current)->next_by_offset;
    }
    *offset_current = best_fit->next_by_offset;

    vsa_mngr->free_size -= size;

    if (best_fit->size > size)/*Split block if there is leftover space*/

    {
        FreeBlockInfo *new_block = (FreeBlockInfo *)((char *)best_fit + size);
        new_block->size = best_fit->size - size;
        best_fit->size = size;

        insert_by_size(vsa_mngr, new_block);/*Insert the remainder back into the free lists*/

        insert_by_offset(vsa_mngr, new_block);
    }

    return (void *)best_fit;
}

void VsaDefragment(vsa_t *vsa_mngr)/*Defragments the free blocks by coalescing adjacent blocks*/
{
    FreeBlockInfo *current = vsa_mngr->free_blocks_by_offset;

    while (current && current->next_by_offset)
    {
        FreeBlockInfo *next = current->next_by_offset;

        if ((char *)current + current->size == (char *)next)/*Checks if the current block and the next block are next to each other*/

        {
            FreeBlockInfo **size_current = NULL;
            current->size += next->size;/*Merges the blocks*/


            size_current = &(vsa_mngr->free_blocks_by_size);/*Removes next block from both free lists*/

            while (*size_current && *size_current != next)
            {
                size_current = &(*size_current)->next_by_size;
            }
            if (*size_current)
            {
                *size_current = next->next_by_size;
            }

            current->next_by_offset = next->next_by_offset;
        }
        else
        {
            current = next;
        }
    }
}

void VsaFree(void *to_free)/*Frees a block of memory*/
{
    FreeBlockInfo *block_to_free = (FreeBlockInfo *)to_free;

    extern vsa_t *global_allocator; 

    insert_by_size(global_allocator, block_to_free);

    insert_by_offset(global_allocator, block_to_free);

    global_allocator->free_size += block_to_free->size;

    VsaDefragment(global_allocator);/*Attempts to coalesce adjacent blocks (defragmentation can be done here)*/
}

size_t VsaLargestChunkAvailable(vsa_t *vsa_mngr)
{
     if (vsa_mngr->free_blocks_by_size)
        return vsa_mngr->free_blocks_by_size->size;
    
    return 0;/*No free blocks available*/
}
