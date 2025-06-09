#include <stdio.h>
#include <string.h>

int IsRotation(char *str1, const char *str2, char *str3);

int main()
{
	char str1[50];
	char str2[50];
    char str3[100];

	printf("Enter a string: ");
	scanf("%s", str1);

	printf("Enter another string: ");
	scanf("%s", str2);

	if (IsRotation(str1, str2, str3))
		printf("The second string is a rotation of the first string.\n");

	else
		printf("The second string is not a rotation of the first string.\n");

	return 0;
}

int IsRotation(char *str1, const char *str2, char *str3)		

{
	int result;
    result = 0;
    
	if (strlen(str1) == strlen(str2))
	{	
		strcpy(str3, str1);
        strcat(str3, str1);

        if (strstr(str3, str2) != NULL)
        	result = 1;
	}

	return result;
}

