#include "bst.h"
#include <stdlib.h>  /*malloc*/
#include <assert.h>  /*assert*/
/*reviewed by Ben*/

/*structure for each node in the bst*/
typedef struct bst_node
{
    void *data;
    struct bst_node *left;
    struct bst_node *right;
} bst_node_t;

/*structure for the entire binary search tree*/
struct bst
{
    bst_node_t *root;
    compare_func_t cmp_func;
    size_t size;  /*number of nodes in the tree*/
};

static bst_node_t *create_node(void *data)
{
    /*allocates memory for the new node*/
    bst_node_t *node = malloc(sizeof(bst_node_t));
    if (node)
    {
        node->data = data;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

bst_t *BstCreate(compare_func_t cmp_func)
{
    /*allocates memory for the bst*/
    bst_t *bst = malloc(sizeof(bst_t));
    if (bst)
    {
        bst->root = NULL;
        bst->cmp_func = cmp_func;
        bst->size = 0;
    }
    return bst;
}

/*helper function to recursively destroy (deallocate memory) all nodes in the tree in a post-order manner (left-right-root)*/
static void destroy_recursive(bst_node_t *node)
{
    /*base case - when node is null*/
    if (node)
    {   
        destroy_recursive(node->left);  /*recursively destroy the left subtree*/
        destroy_recursive(node->right);
        free(node);
    }
}

/*calls destroy_recursive and destroys the bst struct itself*/
void BstDestroy(bst_t *bst)
{
    if (bst)
    {
        destroy_recursive(bst->root);
        free(bst);
    }
}

/*helper function to recursively remove a node from the tree*/
static bst_node_t *remove_recursive(bst_node_t *node, const void *data, compare_func_t cmp_func, int *removed)
{   
    /*compares target data with current node's data*/
    int cmp = cmp_func(data, node->data);

    /*base case - node is null*/
    if (!node)
        return NULL;

    /*if target data is smaller, search in the left subtree*/
    if (cmp < 0)
        node->left = remove_recursive(node->left, data, cmp_func, removed);  /*connects temp to the tree*/

    else if (cmp > 0)
        node->right = remove_recursive(node->right, data, cmp_func, removed);

    /*node with the target data is found*/
    else
    {
        bst_node_t *temp = NULL;
        *removed = 1;  /*flag that marks that a node has been removed*/

        /*If no left child, return the right child to replace it*/
        if (!node->left)
        {
            bst_node_t *temp = node->right;
            free(node);
            return temp;
        }

        /*if no right child, return the left child to replace it*/
        else if (!node->right)
        {
            bst_node_t *temp = node->left;
            free(node);
            return temp;
        }

        /*node has two children: Find the in-order successor (smallest node in the right subtree)*/
        temp = node->right;
        while (temp->left)
            temp = temp->left;

        /*replaces current node's data with the successor's data*/
        node->data = temp->data;
        /*removes successor*/
        node->right = remove_recursive(node->right, temp->data, cmp_func, removed);
    }

    return node;
}

/*removes a node with the given data from the bst*/
void BstRemove(bst_t *bst, const void *data)
{
    if (bst)
    {
        int removed = 0;  /*flag to check if a node was removed*/
        bst->root = remove_recursive(bst->root, data, bst->cmp_func, &removed);  /*root is assigned the unwound state of remove_recursive*/ 
        if (removed)
            bst->size--;
    }
}

/*recursive helper function to insert a node with the specified data into the tree*/
static int insert_recursive(bst_node_t **node, void *data, compare_func_t cmp_func)
{
    int cmp;

    if (!*node)
    {
        *node = create_node(data);
        return *node ? 0 : 1;
    }

    cmp = cmp_func(data, (*node)->data);  /*dereferencing node gets the actual current node (the bst_node_t struct representing the root)*/
    assert(cmp != 0); /*ensures no duplicate data*/

    if (cmp < 0)  /*if data is less than current node's data, recurse left*/
        return insert_recursive(&(*node)->left, data, cmp_func);  /*gets the address of the left pointer itself of the current node so that the function can change the pointer from NULL to point to a new node that holds the data we're inserting*/

    else
        return insert_recursive(&(*node)->right, data, cmp_func);
}

/*function to insert data into the binary search tree*/
int BstInsert(bst_t *bst, void *data)
{
    int result;

    if (!bst)
        return 1;

    result = insert_recursive(&bst->root, data, bst->cmp_func);  /*obtains the address of the root pointer which allows the function to modify it from pointing to null to point to a new node that is being with the inserted value*/

    if (result == 0)  /*if insertion was successful, increment the tree size*/
        bst->size++;

    return result;
}

/*function to return the size of the binary search tree*/
size_t BstSize(const bst_t *bst)
{
    return bst ? bst->size : 0;  /*returns the size of the tree or 0 if the tree is NULL*/
}

int BstIsEmpty(const bst_t *bst)
{
    return bst ? (bst->size == 0) : 1;  /*returns 1 if the tree is empty, 0 otherwise*/
}

/*recursive helper function to search for data in the tree*/
static void *find_recursive(bst_node_t *node, compare_func_t cmp_func, const void *data)
{
    int cmp;

    /*base case*/
    if (!node)
        return NULL;

    cmp = cmp_func(data, node->data);

    if (cmp == 0)  /*if the data matches, return the current node's data*/
        return node->data;

    else if (cmp < 0)
        return find_recursive(node->left, cmp_func, data);

    else
        return find_recursive(node->right, cmp_func, data);
}

void *BstFind(bst_t *bst, compare_func_t cmp_func, const void *data)
{
    /*if bst is not null (meaning the tree exists) it will return find_recursive, otherwise return null*/
    return bst ? find_recursive(bst->root, cmp_func, data) : NULL;
}

/*recursive helper function to apply an action to each node in the tree, in-order*/
static int for_each_recursive(bst_node_t *node, action_function_t action_func, void *param)
{
    int result;

    /*base case*/
    if (!node)
        return 0;

    /*recursively apply the action to the left subtree*/
    result = for_each_recursive(node->left, action_func, param);

    /*if the action returns a non-zero value, stop the recursion*/
    if (result != 0)
        return result;

    /*apply the action to the current node's data*/
    result = action_func(node->data, param);

    if (result != 0)
        return result;

    /*recursively apply the action to the right subtree*/
    return for_each_recursive(node->right, action_func, param);
}

int BstForEach(bst_t *bst, action_function_t action_func, void *param)
{
    return bst ? for_each_recursive(bst->root, action_func, param) : 0;
}