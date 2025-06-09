#include <stddef.h>/*for size_t*/
#include "functions.h"


void *Memcpy(void *dest, const void *src, size_t n)
{
	size_t* d;
	const size_t* s; 
	unsigned char* d_remaining_bytes;
	const unsigned char* s_remaining_bytes;
	size_t i, chunks, remainder;

	chunks = n / sizeof(size_t);

	remainder = n % sizeof(size_t);

	d = (size_t*)dest;

	s = (const size_t*)src;/*to prevent modifiying the source data*/

	for (i = 0; i < chunks; ++i)
		d[i] = s[i];

	d_remaining_bytes = (unsigned char*) (d + chunks);/*advances the pointer to the reamining bytes*/
	s_remaining_bytes = (const unsigned char*) (s + chunks);


	for (i = 0; i < remainder; ++i)
		d_remaining_bytes[i] = s_remaining_bytes[i];

	return dest;
}



void *Memmove(void *dest, const void *src, size_t n)
{
    size_t* d;
    const size_t* s;
    unsigned char* d_remaining_bytes;
    const unsigned char* s_remaining_bytes;
    size_t i, chunks, remainder;

    d = (size_t*)dest;
    s = (const size_t*)src;

    chunks = n / sizeof(size_t);
    remainder = n % sizeof(size_t);

    if (dest < src || (unsigned char*)dest >= (unsigned char*)src + n)/*checks to see that there's no overlapping memory regions. the first condition checks whether the destination's memory address is below the source, and vice versa for the second condition (casting is necessary here to ensure we compare the start of the destination with the end of the source which is start + n bytes)*/
    {
        for (i = 0; i < chunks; ++i)/*since there's no overlap, forward copy*/
        {
            d[i] = s[i];
        }

        d_remaining_bytes = (unsigned char*) (d + chunks);
        s_remaining_bytes = (const unsigned char*) (s + chunks);

        for (i = 0; i < remainder; ++i)
        {
            d_remaining_bytes[i] = s_remaining_bytes[i];
        }
    }

    else
    {
        d_remaining_bytes = (unsigned char*) (d + chunks * sizeof(size_t) + remainder);/*moving the pointer to point to the end of the copied data*/
        s_remaining_bytes = (const unsigned char*) (s + chunks * sizeof(size_t) + remainder);

        for (i = remainder; i > 0; --i)/*since there's an overlap we need to copy backwards to prevent overwriting data that hasn't been copied yet*/
            d_remaining_bytes[i - 1] = s_remaining_bytes[i - 1];

        for (i = chunks; i > 0; --i)
            d[i - 1] = s[i - 1];
    }

    return dest;
}


void *Memset(void *dest, int c, size_t n)
{
    size_t* d;
    unsigned char* d_remaining_bytes;
    size_t value;
    size_t i, chunks, remainder;

    d = (size_t*)dest;
    d_remaining_bytes = (unsigned char*)dest;

    chunks = n / sizeof(size_t);
    remainder = n % sizeof(size_t);

    value = (unsigned char)c;/* a buffer of size_t of a repeated value - designed to fill a block of memory with a particular value by assigning the lower byte of value to c*/
    value |= value << 8;/*This shifts "value" 8 bits to the left and does bitwise "or" with the original "value", which duplicates the byte pattern across the lower two bytes of "value".*/
    value |= value << 16;
    value |= value << 32;

    d_remaining_bytes = (unsigned char*)dest;/*handles the first misaligned bytes with padding*/
    while (((size_t)d_remaining_bytes % sizeof(size_t)) != 0 && n > 0)/*checks if the remainder is not zero, which means that the address is not aligned to a sizeof(size_t) boundary. If it's not aligned, the loop continues.*/
    {
        *d_remaining_bytes++ = 0;
        --n;
    }

    d = (size_t*)d_remaining_bytes;

    for (i = 0; i < chunks; ++i)
    {
        d[i] = value;
    }

    d_remaining_bytes = (unsigned char*)(d + chunks);/*handles the remaining bytes of memory after the memory has been filled in chunks of size_t*/

    for (i = 0; i < remainder; ++i)
    {
        d_remaining_bytes[i] = (unsigned char)c;
    }

    return dest;
}