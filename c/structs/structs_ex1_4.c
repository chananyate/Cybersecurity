#include <stdio.h>/*printf*/
#include <stdlib.h>/*malloc*/
#include <string.h>/*strcpy, strcat*/

#define MAX_STR 50

typedef struct Element {

    void *var;/*A pointer that can hold the address of any data type*/

    void (*add_func)(struct Element *element);

    void (*print_func)(struct Element *element);

    void (*clean_func)(struct Element *element);

} Element;

void add_int(Element *element);
void add_float(Element *element);
void add_string(Element *element);
void print_int(Element *element);
void print_float(Element *element);
void print_string(Element *element);
void clean_string(Element *element); 


int main() 
{
	Element array[3];/*An array of structs that currently holds three elements*/

	int i;

    int integer_number;

    float decimal_number;

    char *string = NULL;


    integer_number = 12;

    array[0].var = &integer_number;/*The first element's "var" member points to the integer*/

    array[0].add_func = add_int;

    array[0].print_func = print_int;

   
    decimal_number = 6.7;

    array[1].var = &decimal_number;

    array[1].add_func = add_float;

    array[1].print_func = print_float;


    string = malloc(MAX_STR);

    strcpy(string, "chapter");

    array[2].var = string;

    array[2].add_func = add_string;

    array[2].print_func = print_string;

    array[2].clean_func = clean_string;/*Only the string element needs to point to a "clean function" because the others haven't had memory dynamically allocated for them.*/


    printf("The original elements:\n");

    for (i = 0; i < 3; ++i) 
		array[i].print_func(&array[i]);/*For every element the "print_func" function pointer points to its respective print function.*/

    for (i = 0; i < 3; ++i)
        array[i].add_func(&array[i]);


    printf("\nThe elements after adding 10:\n");

    for (i = 0; i < 3; ++i)
		array[i].print_func(&array[i]);

   
	array[2].clean_func(&array[2]);/*Only the element containing the string needs to be freed since it was dynamically allocated memory via malloc.*/

    return 0;
}

void add_int(Element *element)/*Takes in a pointer to the Element struct as a parameter.*/ 
{
    *(int *)(element->var) += 10;/*Acesses the "var" member via this pointer, casts it to an int pointer, dereferences it and adds 10.*/
}

void add_float(Element *element) 
{
	*(float *)(element->var) += 10.0;
}

void add_string(Element *element) 
{
	char buffer[5];

    sprintf(buffer, "10");

    strcat((char *)element->var, buffer);
}

void print_int(Element *element) 
{
	printf("%d\n", *(int *)(element->var));
}

void print_float(Element *element) 
{
	printf("%.2f\n", *(float *)(element->var));
}

void print_string(Element *element) 
{
    printf("%s\n", (char *)element->var);
}

void clean_string(Element *element) 
{
	 free(element->var);
}
