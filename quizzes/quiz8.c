#include <stdio.h>/*printf*/
#include <string.h>/*strlen*/

void Permutation(char *str, int start, int end);

int main()  
{  
    int length;
    char str[] = "ABC";  
    length = strlen(str);  
    printf("The permutations are:\n");  
    Permutation(str, 0, length);  
    return 0;
}

void Permutation(char *str, int start, int end)  
{  
    int i;
    
    if (start == end - 1)/*base case*/
        printf("%s\n", str);
        
    else 
    {
        for (i = start; i < end; i++) 
        {
            char temp = str[start];
            
            str[start] = str[i];
            
            str[i] = temp;

            Permutation(str, start + 1, end);/*start + 1 moves the starting point to the next position in order to permute the rest of the string from that position to end*/
            
            temp = str[start];/*backtracking to restore the string to the state it was in before making changes at the current recursion level*/
            
            str[start] = str[i];
            
            str[i] = temp;
        }
    }
}
