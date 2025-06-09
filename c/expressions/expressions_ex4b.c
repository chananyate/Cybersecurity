#include <stdio.h>

float power(float n);

int main()
{
    float y;
    printf("Enter exponent: ");
    scanf("%f", &y);
 
    float result = power(y);

    printf("10 to the power of %.2f equals: %.10f\n", y, result);

    return 0;
} 

float power(float n)
{
    float result = 1.0;
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
