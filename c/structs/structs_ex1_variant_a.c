#include <stdio.h>/*printf*/
#include <stdlib.h>
#include <string.h>/*strncat*/

typedef enum {

    INT,

    FLOAT,

    STRING

} DataType;


typedef union { /*Holds the different data types. Since only one data type at a time can be represented in the array, union is better than struct since it's more memory efficient. If more types need to be added in the future they can be added here.*/

    int integer_number;

    float decimal_number;

    char string[50];

} ElementValue;


typedef struct { /*The struct has two elemments. one is of the enum type defined before, and the second is the union type defined before.*/

    DataType type;

    ElementValue value;

} ArrayElement;


void add_to_element(ArrayElement *element, int add_value) 
{
    char buffer[50];

    if (element->type == INT) 
        element->value.integer_number += add_value;/*Reaches the integer member of the union through the "value" member of the struct.*/

    else if (element->type == FLOAT) 
        element->value.decimal_number += (float)add_value;

    else if (element->type == STRING) 

    {
        sprintf(buffer,"%d", add_value);

        strncat(element->value.string, buffer, 50 - strlen(element->value.string) - 1);

    }

}


void print_element(const ArrayElement *element)/*just printing the element without wanting to modifiy it, therefore using const. Also, using a pointer in order to pass by reference and not value to prevent stack overload.*/
{
    if (element->type == INT) 
        printf("%d\n", element->value.integer_number);

    else if (element->type == FLOAT) 
        printf("%f\n", element->value.decimal_number);

    else if (element->type == STRING)
        printf("%s\n", element->value.string);
}


int main()
{

    ArrayElement array[3];/*initialize the array with three elements of different types.*/ 

    int add_value, i;

    array[0].type = INT;/*each element in the array is of type struct and therefore it will have both a type member (via enum) and a value member (via union).*/

    array[0].value.integer_number = 20;

    array[1].type = FLOAT;

    array[1].value.decimal_number = 12.2;

    array[2].type = STRING;

    strncpy(array[2].value.string, "chapter", 50);

    printf("Original array:\n");

    for (i = 0; i < 3; i++)
        print_element(&array[i]);

    add_value = 10;

    for (i = 0; i < 3; ++i) 
        add_to_element(&array[i], add_value);

    printf("\nModified array after adding %d:\n", add_value);

    for (i = 0; i < 3; ++i) 
        print_element(&array[i]);

    return 0;

}
	
