#include <stdio.h>

void swap(int *p, int *q);

int main()
{
  int i, j;
  printf("Enter two numbers: ");
  scanf("%d %d", &i, &j);

  swap(&i, &j);
}

void swap(int *p, int *q)
{
  int tmp = *p;
  *p = *q;
  *q = tmp;

  printf("The swapped numbers are: %d %d\n", *p, *q);

}



