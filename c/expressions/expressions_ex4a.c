#include <stdio.h>

double power(int n);

int main()
{
    int y;
    printf("Enter exponent: ");
    scanf("%d", &y);
 
    float result = power(y);

    printf("10 to the power of %d equals: %f\n", y, result);

    return 0;
} 

double power(int n)
{
    double result = 1.0;
    if (n >= 0) {
        for (int i = 0; i < n; ++i) {
            result *= 10.0;
        }
    } else {
        for (int i = n; i < 0; ++i) {
            result *= 10.0;
        }
        result = 1.0 / result;
    }
    return result;
}
