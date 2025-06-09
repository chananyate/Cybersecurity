#include <stdlib.h>/*malloc and calloc*/
#include "hash.h"
#include "slist.h"

struct hash_table 
{
    /*array of singly linked lists (buckets)*/
    slist_t **buckets;
    /*number of buckets*/
    size_t size;
    hash_function_t hash_func;
    match_func_t compare_func;
};

/*creates a new hash table of the given size and initializes it with the provided hash and comparison functions.
  the table is allocated dynamically, and buckets are initialized as empty singly linked lists.
  returns a pointer to the created hash table, or NULL if memory allocation fails.*/
hash_t *HashCreate(const size_t size, hash_function_t function, match_func_t compare)
{
    size_t i;

    hash_t *table = malloc(sizeof(hash_t));

    if (!table) return NULL;

    /*allocates an array of 'size' pointers (the number of buckets), each initially set to NULL.*/
    table->buckets = calloc(size, sizeof(slist_t *));

    if (!table->buckets) 
    {
        /*frees the allocated table if bucket allocation fails.*/
        free(table);

        return NULL;
    }

    table->size = size;
    table->hash_func = function;
    table->compare_func = compare;

    /*initializes each bucket with an empty singly linked list.*/
    for (i = 0; i < size; i++) 
    {
        table->buckets[i] = SListCreate();

        if (!table->buckets[i]) 
        {
            /*clean up if bucket creation fails*/
            HashDestroy(table);

            return NULL;
        }
    }

    return table;
}

/*destroys the hash table, freeing all memory associated with it.
  frees each bucket (singly linked list) and then the hash table structure itself.*/
void HashDestroy(hash_t* table)
{
    size_t i;

    if (!table) 
        return;

    for (i = 0; i < table->size; i++) 
        SListDestroy(table->buckets[i]);
    
    free(table->buckets);
    free(table);
}

/*removes the element with the specified key from the hash table.
  returns 1 if the key was found and removed, otherwise returns 0.*/
int HashRemove(const void* key, hash_t* table)
{
    size_t index;

    slist_t *bucket = table->buckets[index];

    /*gets iterator to the beginning of the list.*/
    slist_iter_t iter = SListBegin(bucket);

    slist_iter_t end = SListEnd(bucket);

    if (!key || !table) 
        return 0;

    /*calculates the bucket index using the hash function.*/
    index = table->hash_func((void*)key) % table->size;

    /*traverses the list to find the element with the matching key.*/
    while (!SListIterIsEqual(iter, end)) 
    {
        if (table->compare_func(key, SListGetData(iter)) == 0) 
        {
            /*if match found then element is removed*/
            SListRemove(iter);

            return 1;
        }

        iter = SListNext(iter);
    }

    return 0;
}

/*inserts a new element into the hash table at the appropriate bucket based on the hash of the key.
  returns 1 if the insertion is successful, otherwise returns 0.*/
int HashInsert(const void* key, hash_t* table, const void* data)
{
    size_t index;

    /*a pointer to the singly linked list located at the specific index in the table->buckets array.*/
    slist_t *bucket = table->buckets[index];

    if (!key || !table || !data) 
        return 0;

    index = table->hash_func((void*)key) % table->size;
    
    /*attempts to insert 'data' at the end of the linked list 'bucket'.
    if the insertion fails (which is indicated by returning an iterator equal to SListEnd), it indicates failure.*/
    if (SListInsert(bucket, SListEnd(bucket), (void*)data) == SListEnd(bucket)) 
        return 0;
    
    return 1;
}

/*returns the total number of elements in the hash table by summing up the sizes of all buckets.*/
size_t HashSize(const hash_t* table)
{
    size_t total_size, i;

    if (!table) 
        return 0;

    total_size = 0;

    for (i = 0; i < table->size; i++) 
        total_size += SListCount(table->buckets[i]);
    
    return total_size;
}

/*returns 1 if the hash table is empty*/
int HashIsEmpty(const hash_t* table)
{
    return HashSize(table) == 0;
}

/*searches for an element with the specified key in the hash table.
  takes the hash and comparison functions as parameters.
  returns a pointer to the found data or NULL if the key is not found.*/
void* HashFind(const hash_t* table, match_func_t is_match_func, hash_function_t function, const void *key)
{
    size_t index;

    slist_t *bucket = table->buckets[index];

    slist_iter_t iter = SListBegin(bucket);

    slist_iter_t end = SListEnd(bucket);

    if (!table || !is_match_func || !function || !key) 
        return NULL;

    index = function((void*)key) % table->size;

    /*traverses the list to find the element with the matching key*/
    while (!SListIterIsEqual(iter, end)) 
    {
        /*retrieves the data stored in the current node*/
        void *data = SListGetData(iter);

        if (is_match_func(key, data) == 0) 
            return data;
        
        iter = SListNext(iter);
    }

    return NULL;
}

/*applies the action function to each element in the hash table.
  return 0 if the action was successfully applied to all elements.*/
int HashForEach(const hash_t* table, action_func_t action_func, const void *param)
{
    int result;
    size_t i;

    if (!table || !action_func) 
        return 0;

    for (i = 0; i < table->size; i++) 
    {
        slist_t *bucket = table->buckets[i];

        result = SListForEach(SListBegin(bucket), SListEnd(bucket), action_func, (void*)param);

        if (result != 0) 
            return result;
    }

    return 0;
}