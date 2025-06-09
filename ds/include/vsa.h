#ifndef __VSA_H_CR8__
#define __VSA_H_CR8__

typedef struct vsa vsa_t;

vsa_t *VsaInit(void *pool_memory, size_t total_memory);

void *VsaAlloc(vsa_t *vsa_mngr, size_t memory_size);

void VsaFree(void *to_free);

size_t VsaLargestChunkAvailable(vsa_t *vsa_mngr);

void VsaDefragment(vsa_t *vsa_mngr);

#endif /*__FSA_H_CR8__*/
