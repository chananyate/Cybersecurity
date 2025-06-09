#include <stdio.h>

int reversal(int num);

int main()
{
 int a;
 printf("enter number: ");
 scanf("%d", &a);
 
printf("The reversed number is %d\n", reversal(a));

}

int reversal(int num)
{
  int reversed = 0;
  while (num != 0)
{
  int digit = num % 10;
  reversed = reversed * 10 + digit;
  num/=10;
}
return reversed;

}
