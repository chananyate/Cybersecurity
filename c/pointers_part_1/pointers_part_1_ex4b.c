#include <stdio.h>
#include <assert.h>

void swap(size_t *a, size_t *b);

void swap_ptrs(size_t **a, size_t **b);

int main() 
{
  size_t x, y;
  size_t *p, *q;

  printf("Enter two numbers: ");
  scanf("%zu %zu %zu %zu", &x, &y, &p, &q);

  swap(&x,&y);

  swap_ptrs(&p, &q);

  printf("The swapped numbers are: %zu %zu\n", x, y);
  printf("The swapped numbers are: %p %p\n", &p, &q);

}

void swap(size_t *a, size_t *b)
{  
  assert(a != NULL && b != NULL);
    
  size_t tmp = *a;
  *a = *b;
  *b = tmp;
}



void swap_ptrs(size_t **a, size_t **b)
{
  size_t *a, *b;
  swap(*a,*b);
}

