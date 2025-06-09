#include <stdio.h>

void heavy_cookie(int *cookies);

int main() 
{

    int cookies[9] = {1, 1, 1, 1, 1, 2, 1, 1, 1};
    
    heavy_cookie(cookies);

    return 0;

}

void heavy_cookie(int *cookies)
{
    int group1 = cookies[0] + cookies[1] + cookies[2];

    int group2 = cookies[3] + cookies[4] + cookies[5];

    if (group1 > group2) 
    {
        if (cookies[0] == cookies[1]) 
            printf("Cookie number 3 is the heaviset\n");
            
        else if (cookies[0] > cookies[1]) 
            printf("Cookie number 1 is the heaviest.\n");
 
        else 
            printf("Cookie number 2 is the heaviest.\n");
    }
    
    else if (group1 < group2)
    {
        if (cookies[3] == cookies[4])
            printf("Cookie number 6 is the heaviest.\n");

        else if (cookies[3] > cookies[4])
            printf("Cookie number 4 is the heaviest.\n");

        else
            printf("Cookie number 5 is the heaviest.\n");
    }
    
    else 
    {
        if (cookies[6] == cookies[7])
            printf("Cookie number 9 is the heaviest.\n");

        else if (cookies[6] > cookies[7])
            printf("Cookie number 7 is the heaviest.\n");

        else
            printf("Cookie number 8 is the heaviest.\n");   
    }
}
    
