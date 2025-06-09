#ifndef __FSA_H_CR8__
#define __FSA_H_CR8__

#include <stddef.h>/*size_t*/

typedef struct fsa fsa_t;/*forward declaration of the fixed-size allocator structure*/

size_t FsaSuggestSize(size_t block_size, size_t block_num);/*determins the amount of memory required for the fixed size allocator, including overhead*/

fsa_t *FsaInit(void *memory, size_t block_size, size_t pool_size);/*Sets up the linked list of free blocks*/

void *FsaAlloc(fsa_t *fsa_mngr);/*allocates a block of memory*/

void FsaFree(void *to_free, fsa_t *fsa_mngr);/*deallocates a block of memory*/

size_t FsaCountFree(fsa_t *fsa_mngr);/*monitors how many free blocks of memory we have*/

#endif /*__FSA_H_CR8__*/

