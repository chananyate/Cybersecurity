#include <stdio.h>

int Fibonacci(int element_index);

int main ()
{
    int index;
    printf("Enter index: ");
    scanf("%d", &index);

    printf("The number at index %d in the fibonacci sequence is: %d\n", index, Fibonacci(index));

    return 0;
}

int Fibonacci(int element_index)
{
    int i;
    int sum = 0;
    int x = 0;
    int y = 1;

    if (element_index == 0)
        return x;

    if (element_index == 1)
        return y;

   for (i = 2; i <= element_index; ++i)
   {
       sum = x + y;
       x = y;
       y = sum;
    }

  return sum;
}