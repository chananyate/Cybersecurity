#include <stdlib.h>  /*malloc*/
#include <stdio.h>  /*printf*/
#include "slist.h"

struct slist_node 
{
    void *data;

    struct slist_node *next;
};

struct slist
{   
    /*Pointer to the first node in the list*/
    struct slist_node *head;            
};

slist_t *SListCreate(void)
{
    slist_t *list = NULL;

    list = malloc(sizeof(slist_t));

    /*If malloc succeded then list is initialized as empty (head points to NULL)*/
    if (list)                          
        list->head = NULL;
    
    return list;
}

void SListDestroy(slist_t *list) 
{
    struct slist_node *current = NULL;

    struct slist_node *next = NULL;

    current = list->head;

    /*Checks if the current node exists (not NULL) and traverses and frees each node in the list*/
    while (current)                   
    {
        next = current->next;

        free(current);

        current = next;
    }

    free(list);
}

/*Returns an iterator to the first node in the list*/
slist_iter_t SListBegin(const slist_t *list)
{
    return (slist_iter_t)list->head;
}

/*Returns an iterator that represents the end of the list (NULL). Casting list to (void) is done to prevent compiler warnings about unused function parameters*/
slist_iter_t SListEnd(const slist_t *list) 
{
    (void)list;

    return NULL;
}

/*Returns an iterator to the next element in the list*/
slist_iter_t SListNext(slist_iter_t iter) 
{
    /*If iter is not Null, it's cast to a slist_node pointer in order to access the next field in the node. Then it's cast back to slist_iter_t and sets iter to it.*/ 
    return iter ? (slist_iter_t)((struct slist_node *)iter)->next : NULL;
}

/*Checks if two iterators point to the same node*/
int SListIterIsEqual(slist_iter_t one, slist_iter_t other) 
{
    return one == other;
}

/*Obtains the data stored in the node that is pointed to by the iterator*/
void *SListGetData(slist_iter_t iter) 
{
    return ((struct slist_node *)iter)->data;
}

void SListSetData(slist_iter_t iter, void *data) 
{
    ((struct slist_node *)iter)->data = data;
}

/*Inserts a new node before the node pointed to by 'where', and returns the iterator to the new node*/
slist_iter_t SListInsert(slist_t *list, slist_iter_t where, void *data)
{
    struct slist_node *new_node = NULL;
    struct slist_node *prev = NULL; 

    new_node = malloc(sizeof(struct slist_node));

    /*If memory allocation fails, return the end of the list (null)*/
    if (!new_node) 
        return SListEnd(list);
    
    new_node->data = data;

    /*Points the new node to the 'where' node in order to connect it to the next node of the list*/
    new_node->next = (struct slist_node *)where;
    
    if (where == list->head) 
        list->head = new_node;

    else 
    {
        /*Sets prev to point to the beginning of the list for the traversal*/
        prev = list->head;

        /*Traverses the list up to the node before the 'where' node*/  
        while (prev && prev->next != where) 
            prev = prev->next;

        /*If prev is not null, meaning it's not an empty list and it hasn't reached the end of the list, then this line links the previous node to the new node*/
        if (prev) 
            prev->next = new_node;

        else 
        {
            free(new_node);

            return SListEnd(list);
        }
    }

    /*Returns iterator to the new node*/
    return (slist_iter_t)new_node;
}

slist_iter_t SListRemove(slist_iter_t iter) 
{
    struct slist_node *to_remove = NULL;

    /*If iter is NULL or it's the last node - return NULL*/
    if (!iter || !iter->next)
        return NULL;

    /*Node to be removed*/
    to_remove = iter->next;

    /*Links to the next node after the one being removed*/
    iter->next = to_remove->next;

    free(to_remove);

    /*Returns the node after the one just removed*/
    return iter->next;
}

int SListIsEmpty(const slist_t *list) 
{
    /*Returns true or false*/
    return list->head == NULL;
}

/*Helper funfction used within SListForEach for the purpose of incrementing the count. Param is a pointer to the size_t variable 'count' passed from the SListCound function.*/ 
static int count_action(void *data, void *param) 
{
    size_t *count = NULL;

    /*Unused parameter*/
    (void)data;

    count = (size_t *)param;

    (*count)++;

    return 0;
}

/*Counts the number of elements in the list by using SListForEach which uses count_action to increment the count*/
size_t SListCount(const slist_t *list) 
{
    size_t count;

    count = 0;

    SListForEach(SListBegin(list), SListEnd(list), count_action, &count);

    return count;
}

/*Finds the first element in the range 'from' 'to' that matches a value (param).
The developer is responsible for implementing the definition of is_match_func based on the type of data he's passing*/
slist_iter_t SListFind(slist_iter_t from, slist_iter_t to, match_func_t is_match_func, void *param) 
{
    while (from != to) 
    {
        printf("SListFind: Comparing '%s' with key '%s'\n", (char*)from->data, (char*)param);
        if (is_match_func(param, from->data))  
        {
            printf("SListFind: Match found!\n");
            return from;
        }

        from = from->next;
    }
    printf("SListFind: No match found\n");
    return to;
}

/*Applies an action function to each element in the range 'from' 'to', like printing the data in a node.*/
int SListForEach(slist_iter_t from, slist_iter_t to, action_func_t action_func, void *param) 
{
    int result;

    result = 0;

    /*Iterates as long as the first iterator hasn't reached the second one, 
    and as long as the action_func result is 0 (meaning success).*/
    while (from != to && result == 0) 
    {
        result = action_func(from->data, param);

        from = from->next;
    }

    return result;
}