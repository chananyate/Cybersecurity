#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fsa.h"

int main(void)
{
    void *memory_pool = NULL;
    void *block1 = NULL;
    void *block2 = NULL;
    void *block3 = NULL;
    fsa_t *fsa = NULL;
    size_t pool_size;

    pool_size = FsaSuggestSize(64, 32);/*block_size and block_num respectively*/

    memory_pool = malloc(pool_size);

    assert(NULL != memory_pool);

    fsa = FsaInit(memory_pool, 64, pool_size);/*block_size and pool_size respectively*/

    assert(NULL != fsa);

    printf("Initial free blocks: %lu\n", FsaCountFree(fsa));

    block1 = FsaAlloc(fsa);

    block2 = FsaAlloc(fsa);

    block3 = FsaAlloc(fsa);

    printf("Number of free blocks after 3 allocations: %lu\n", FsaCountFree(fsa));

    FsaFree(block2, fsa);

    FsaFree(block3, fsa);

    printf("Number of free blocks after deallocating 2 blocks: %lu\n", FsaCountFree(fsa));

    FsaFree(block1, fsa);

    printf("Number of free blocks after deallocating all blocks: %lu\n", FsaCountFree(fsa));/*checks to see that number fo free blocks matches riginal amount*/

    free(memory_pool);

    return 0;
}
