#include <stdio.h>  /*printf*/
#include <stdlib.h>  /*malloc*/
#include "bst.h"

int compare_ints(const void *a, const void *b)
{   
    return (*(int*)a - *(int*)b);
}

/*action function to print an integer*/
int print_int(void *data, void *param)
{
    (void)param;  /*for unused parameter*/

    printf("%d ", *(int*)data);
    
    return 0;
}

int main() 
{
    int i, find_value, remove_value;
    int values[] = {5, 3, 7, 1, 9, 4, 6, 8, 2};
    int n = sizeof(values) / sizeof(values[0]);
    int *found = NULL;

    bst_t *bst = BstCreate(compare_ints);

    if (!bst) 
    {
        printf("Failed to create BST\n");

        return 1;
    }

    for (i = 0; i < n; i++) 
    {
        int *value = malloc(sizeof(int));

        /*assign the current value from the array to the allocated memory*/
        *value = values[i];

        /*inserts and checks for errors*/
        if (BstInsert(bst, value) != 0) 
        {
            printf("Failed to insert %d\n", *value);

            free(value);
        }
    }

    printf("BST size: %lu\n", BstSize(bst));

    printf("Is BST empty? %s\n", BstIsEmpty(bst) ? "Yes" : "No");

    printf("In-order traversal: ");

    BstForEach(bst, print_int, NULL);

    printf("\n");

    find_value = 4;

    found = BstFind(bst, compare_ints, &find_value);

    if (found) 
        printf("Found %d in the BST\n", *found);

    else 
        printf("%d not found in the BST\n", find_value);
    

    remove_value = 5;

    BstRemove(bst, &remove_value);

    printf("After removing %d: ", remove_value);

    BstForEach(bst, print_int, NULL);

    printf("\n");

    BstDestroy(bst);

    return 0;
}