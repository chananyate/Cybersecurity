#include <stdio.h>
#include <stdlib.h>

void array(void *original, int size)
{
   const int *duplicate = (const int *)original;

  for (int i = 0; i < size; ++i)
  {
    printf("%d", duplicate[i]);
  }
  printf("\n");
}

int main()
{ 
  int size;

  printf("Enter size of array: ");
  scanf("%d", &size);

  int *original_array = malloc(sizeof(int)*size);
  
  for(int i = 0; i < size; ++i)
  {
    scanf("%d", &original_array[i]);
  }


array(original_array, size);
free(original_array);
}
