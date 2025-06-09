#include <stdio.h>
#include <assert.h>

void swap(size_t *a, size_t *b);

void swap_ptrs(size_t **ptr_a, size_t **ptr_b);

int main() 
{
  size_t x, y;

  printf("Enter two numbers: ");
  scanf("%zu %zu", &x, &y);

  swap(&x,&y);

  printf("The swapped numbers are: %zu %zu\n", x, y);

  size_t *p = &x;
  size_t *q = &y;

  swap_ptrs(&p, &q);

  printf("The swapped pointers are: %zu %zu\n", *p, *q);

}

void swap(size_t *a, size_t *b)
{  
  assert(a != NULL && b != NULL);
    
  size_t tmp = *a;
  *a = *b;
  *b = tmp;
}


void swap_ptrs(size_t **ptr_a, size_t **ptr_b)
{
  assert(ptr_a != NULL && ptr_b != NULL);
  
 swap(*ptr_a,*ptr_b);//When it comes to pointers, the arguments passed to the function call are always one "degree" less than how they are represented in the cuurent function. If they are regular variables (x) in the current function, then they would be passed as (&x) arguments in the function being called. If they are double pointers then they would be passed as single pointers in the function call.

}

