#include <stdio.h>
#include <assert.h>

void swap(int *a, int *b);

int main() 
{
  int x, y;
  printf("Enter two numbers: ");
  scanf("%d %d", &x, &y);

  swap(&x,&y);

  printf("The swapped numbers are: %d %d\n", x, y);
}

void swap(int *a, int *b)
{  
  assert(a != NULL && b != NULL);
    
  int tmp = *a;
  *a = *b;
  *b = tmp;
}
