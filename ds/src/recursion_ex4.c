#include <string.h>
#include "recursion_ex4.h"

int Strlen(const char *str)
{
	if (*str == '\0')
		return 0;
	/*for every recursive call 1 is added, and str advances 1 step closer to '\0'.
	  as the stack call unwinds the 1's are added up*/
	return 1 + Strlen(str + 1);
}

int Strcmp(const char *str1, const char *str2)
{
	/*base case*/
	if (*str1 == '\0' && *str2 == '\0')
		return 0;

	if (*str1 != *str2)
	{
		if (*str1 > *str2)
			return 1;

		else
			return -1;
	}

	return Strcmp(str1 + 1, str2 + 1);
}

char *Strcpy(char *dest, const char *src)
{
	/*base case*/
	if (*src == '\0')
	{
		*dest = '\0';

		return dest;
	}

	*dest = *src;

	Strcpy(dest + 1, src + 1);

	return dest;
}

char *Strcat(char *dest, const char *src)
{
	/*moves the `dest` pointer to the end of the current string*/
	if (*dest != '\0')
		Strcat(dest + 1, src);
	else 
	{
	/*when we reach the end of `dest`, start copying `src`*/
		while (*src != '\0') 
			*dest++ = *src++;
		
		/*null-terminate the final string*/
		*dest = '\0'; 
	}

	return dest; 
}


char *Strstr(const char *haystack, const char *needle)
{
	/*base case 1 (needle is an empty string)*/
	if (*needle == '\0')
		return (char*)haystack;

	/*base case 2 (the function has reached the end of the haystack without finding needle*/
	if (*haystack == '\0')
		return NULL;

	/*compares the first strlen(needle) chatacters of needle and haystack.*/
	if (strncmp(haystack, needle, strlen(needle)) == 0)
		return (char*)haystack;

	return Strstr(haystack + 1, needle);
}