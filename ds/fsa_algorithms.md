fsa

Alloc Algorithm -
It first checks if the free list is not empty, because if it is then NULL is returned. 
It then retrieves the first block from the free list and then updates the pointer to free list to point to the next block. After doing that, it decrements the number of free blocks available, and then returns the pointer to the allocated block, starting from the data part.

Free Algorithm - 
It first checks to see that the block we want to free is not null. It then accesses the metadata of the block we want to free by adjusting the pointer using pointer arithmetics to subtract the metadata pointer (thus making to_free start at a lower address). It then updates the metadata in the newly freed block to point to the current head of the free list. Then free_list is updated to point to the newly freed block. It then increments the number of free blocks available.
