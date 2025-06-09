#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


struct command_handlers
{
    char *string;
    int (*comparison_function)(const char *data_type, const char *string);
    void (*operation_function)(void*, void*);
};

int string_find(const char *string, const char *data_type);
void add_to_integer(void *integer_number, void *add_number);
void add_to_decimal(void *decimal_number, void *add_number);
void cat_string(void *string, void *add_number);

int main()
{
	char data_type[10];
	char str[BUFSIZ];
	int add_number, integer_number;
	float decimal_number;
	add_number = 0;

	struct command_handlers handler[] = {
    {"int", string_find, add_to_integer},
    {"float", string_find, add_to_decimal},
    {"string", string_find, cat_string}    
    };

    printf("Enter a data type: ");
	scanf("%s", data_type);

	printf("Enter number to add: ");
	scanf("%d", &add_number);	

	if (handler[0].comparison_function(str, handler[0].string))
	{
		printf("Enter an integer: ");
		scanf("%d", &integer_number);
        handler[0].operation_function((void*)&integer_number, (void*)&add_number);
	}

    if (handler[1].comparison_function(str, handler[1].string))
    {
    	printf("Enter a decimal number: ");
    	scanf("%f", &decimal_number);
        handler[1].operation_function((void*)&decimal_number, (void*)&add_number);
    }
    	
	if (handler[2].comparison_function(str, handler[2].string))
	{
		printf("Enter a string: ");
		scanf("%s", str);
        handler[2].operation_function((void*)&str, (void*)&add_number);
	}

	return 0;
}

int string_find(const char *string, const char *data_type)
{
    int result;

    if (strstr(data_type, string) != NULL)
        result = 1;

    else
        result = 0;

    return result;
}

void add_to_integer(void *integer_number, void *add_number)
{	
	int *number;

	int *add;

	number = (int*)integer_number;

	add = (int*)add_number;

	*number += *add;

	printf("%d\n", *number);
}

void add_to_decimal(void *decimal_number, void *add_number)
{
	float *number;

	int *add;

	number = (float*)decimal_number;

	add = (int*)add_number;

	*number += *add;

	printf("%f\n", *number);
}

void cat_string(void *string, void *add_number)
{
	char *result = NULL;

	char *str = NULL;

	char num_string[12];

	int *add;

	add = (int*)add_number;

	str = (char*)string;

	snprintf(num_string, sizeof(num_string), "%d", *add);

	result = malloc(strlen(str) + strlen(num_string) + 1);

	assert(NULL != result);

	strcpy(result, str);

	strcat(result, num_string);

	printf("%s\n", result);
}