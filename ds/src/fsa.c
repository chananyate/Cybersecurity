#include <assert.h>
#include <stddef.h>
#include "fsa.h"

struct fsa
{
	void *memory_pool;/*pointer to the start of the memory pool, past fsa_t*/
	size_t free_blocks;
	size_t block_size;
	void *free_list;/*pointer to the first free block*/
};

size_t FsaSuggestSize(size_t block_size, size_t block_num)
{
	size_t pointer_size;
	pointer_size = sizeof(void*);/*size of a pointer, for storing the free list in each block*/

	return (((block_size + pointer_size) * block_num) + sizeof(fsa_t));
}

fsa_t *FsaInit(void *memory, size_t block_size, size_t pool_size)
{
	size_t i;

	void *ptr = NULL;

	void *ptr2 = NULL;

	fsa_t *fsa = NULL;

	size_t pointer_size;

	size_t block_size_with_meta;

	pointer_size = sizeof(void*);

	block_size_with_meta = block_size + pointer_size;

    fsa = (fsa_t*)(memory);/*the fsa_t struct is placed at the beginning of the memory block in order to simplify management and keep all metadata within a single contiguous block of memory. In addition, it ensures word-size alignment and it also avoids the need for additional pointers to separate metadata.*/

    assert(fsa != NULL);

    fsa->block_size = block_size_with_meta;

	fsa->memory_pool = (char*)memory + sizeof(fsa_t);/*sets the memory_pool pointer within the struct to point to the beginning of the actual blocks (past the fsa_t)*/

	ptr = fsa->memory_pool;/*sets a ptr to the beginning of the memory_pool which we'll use to initialize the free list*/

	fsa->free_list = ptr;/*sets the first free block to where the ptr is currently pointing at, which is the first block of the list*/

	fsa->free_blocks = (pool_size - sizeof(fsa_t)) / block_size_with_meta;

	for (i = 0; i < fsa->free_blocks; ++i)/*loops through each block and sets up the free list in O(n) time complexity.*/
	{	
		ptr2 = (char*)ptr + block_size_with_meta;/*sets another pointer which will point to the next free block, so we can always advance ptr*/

		if (i < fsa->free_blocks - 1)
			*(void**)ptr = ptr2;/*casts ptr into a pointer that holds the address of another pointer (ptr2), and then dereferencing it to hold the address of ptr2.*/

		else
			*(void**)ptr = NULL;

		ptr = ptr2;/*moves ptr to the next block*/

	}

	return fsa;
}

void *FsaAlloc(fsa_t *fsa_mngr)/*done in O(1) time.*/
{
	void *ptr = NULL;

	assert(NULL != fsa_mngr->free_list);

	ptr = fsa_mngr->free_list;

	fsa_mngr->free_list = *(void**)fsa_mngr->free_list;/*updates fsa_mngr->free_list to point to the next free block by dereferencing the metadata in the block that stores the address of the next free block.*/

	fsa_mngr->free_blocks--;

	return (char*)ptr + sizeof(void*);/*to ensure the user gets the block starting past the metadata*/

}

void FsaFree(void *to_free, fsa_t *fsa_mngr)/*done in O(1) time.*/
{
	void *metadata_ptr = NULL;
	assert(NULL != to_free);
	assert(NULL != fsa_mngr);

    metadata_ptr = (char*)to_free - sizeof(void*);/*adjusts the pointer to include the metadata*/

    *(void**)metadata_ptr = fsa_mngr->free_list;/*updates the metadata to point to the current free list head*/

	fsa_mngr->free_list = metadata_ptr;/*updates the free list to point to the newly freed block*/

	fsa_mngr->free_blocks++;
}

size_t FsaCountFree(fsa_t *fsa_mngr)
{
	return fsa_mngr->free_blocks;
}