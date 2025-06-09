#include <stdio.h>
#include <stdlib.h>
#include "vsa.h"

vsa_t *global_allocator; 

int main()
{
    void *block1 = NULL;
    void *block2 = NULL;
    size_t total_size = 1024;
    void *pool = malloc(total_size);
    if (pool == NULL)
    {
        printf("Failed to allocate memory for pool.\n");
        return 1;
    }

    global_allocator = VsaInit(pool, total_size); 
    printf("Allocator initialized with total size: %lu bytes\n", total_size);
    
    block1 = VsaAlloc(global_allocator, 100);
    if (block1 != NULL)
    {
        printf("Allocated block1 of 100 bytes\n");
    }
    else
    {
        printf("Failed to allocate block1\n");
    }

    block2 = VsaAlloc(global_allocator, 200);
    if (block2 != NULL)
    {
        printf("Allocated block2 of 200 bytes\n");
    }
    else
    {
        printf("Failed to allocate block2\n");
    }

    printf("Largest chunk before freeing: %lu bytes\n", VsaLargestChunkAvailable(global_allocator));

    VsaFree(block1);
    printf("Freed block1\n");

    VsaFree(block2);
    printf("Freed block2\n");

    printf("Largest chunk after freeing: %lu bytes\n", VsaLargestChunkAvailable(global_allocator));

    VsaDefragment(global_allocator);
    printf("Allocator defragmented\n");

    free(pool);
    printf("Memory pool freed\n");
    
    return 0;
}
