#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "slist.h"

static int print_int(void *data, void *param) 
{
    (void)param;

    printf("%d ", *(int*)data);

    return 0;
}

static int is_even(const void *data, void *param) 
{
    (void)param;

    return (*(int*)data % 2 == 0);
}

static void print_list(slist_t *list) 
{
    printf("List: ");

    SListForEach(SListBegin(list), SListEnd(list), print_int, NULL);

    printf("\n");
}

int main() 
{
    slist_t *list = NULL;

    slist_iter_t iter = NULL;

    slist_iter_t even = NULL;

    int i, start, new_val;

    size_t count;

    int nums[] = {1, 2, 3, 4, 5};

    printf("Creating list...\n");

    list = SListCreate();

    assert(list != NULL);

    assert(SListIsEmpty(list));

    printf("List created and is empty.\n");

    printf("Inserting elements...\n");

    for (i = 0; i < 5; i++) 
    {
        iter = SListInsert(list, SListEnd(list), &nums[i]);

        assert(!SListIterIsEqual(iter, SListEnd(list)));

        printf("Inserted %d\n", nums[i]);
    }
    
    assert(!SListIsEmpty(list));

    printf("Counting elements...\n");

    count = SListCount(list);

    printf("Count: %lu\n", count);

    assert(count == 5);

    print_list(list);

    printf("Finding even number...\n");

    even = SListFind(SListBegin(list), SListEnd(list), is_even, NULL);

    assert(*(int*)SListGetData(even) == 2);

    printf("Found even number: %d\n", *(int*)SListGetData(even));

    printf("Removing even number...\n");

    SListRemove(even);

    count = SListCount(list);

    printf("New count: %lu\n", count);

    assert(count == 4);

    print_list(list);

    printf("Inserting at beginning...\n");

    start = 0;

    SListInsert(list, SListBegin(list), &start);

    count = SListCount(list);

    printf("New count: %lu\n", count);

    assert(count == 5);

    assert(*(int*)SListGetData(SListBegin(list)) == 0);

    print_list(list);

    printf("Setting data...\n");

    new_val = 10;

    SListSetData(SListBegin(list), &new_val);

    assert(*(int*)SListGetData(SListBegin(list)) == 10);

    print_list(list);

    printf("Destroying list...\n");

    SListDestroy(list);

    printf("All tests passed successfully!\n");

    return 0;
}