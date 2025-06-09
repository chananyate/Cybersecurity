#include <stdio.h>

void stock_prices(int array[], int length);

int main()
{
    int length;

    int array[] = {6, 12, 3, 5, 1, 4, 9, 2};

    length = sizeof(array)/sizeof(array[0]);

    stock_prices(array, length);
  
    return 0;
}

void stock_prices(int array[], int length)
{
    int i, max, min;
   
    int sell_index, buy_index;
  
    max = array[0];

    min = array[0];

    sell_index = 0;

    buy_index = 0;

    for (i = 0; i < length - 1; ++i)
    {
        if (array[i + 1] > max && i > buy_index)
        {
            max = array[i + 1];
 
            sell_index = i + 1;
        }

        else if (array[i + 1] < min)
        {
            min = array[i + 1];

            buy_index = i + 1;
        }
     }
  
     printf("Buy index: %d\n", buy_index);
         
     printf("sell index: %d\n", sell_index);

     printf("Profit: %d\n", max - min);
}
