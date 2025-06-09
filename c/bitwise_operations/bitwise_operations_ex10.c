#include <stdio.h>
#include <string.h>

void floatToBinary(float number); 

int main() 
{
    float number;

    printf("Enter a number: ");
    scanf("%f", &number);

    printf("The binary representation of %f is: \n", number);

    floatToBinary(number);

    return 0;

}

void floatToBinary(float number) 
{
    int i;
    unsigned int bits, bit;

    memcpy(&bits, &number, sizeof(bits));

    for (i = 31; i >= 0; --i) 
    {
        bit = (bits >> i) & 1;/*prints the most significant bit first and then "bits" shifts right and prints either 1 or 0 based on & result.*/

        printf("%d", bit);
    }

    printf("\n");
}


