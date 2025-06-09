#include <stdio.h>
#include <stdlib.h>

struct Node 
{
	int data;
	struct Node *next;
};

struct Node *FlipList(struct Node *node);
struct Node *CreateList(int data);
void PrintList(struct Node *node);
void FreeList(struct Node *node);

int main()
{
	struct Node *node = CreateList(10);

	node->next = CreateList(20);

	node->next->next = CreateList(30);

	node->next->next->next = CreateList(40);

	printf("Original list:\n");

	PrintList(node);

	node = FlipList(node);

	printf("Reversed list:\n");

	PrintList(node);

	FreeList(node);

	return 0;
}

struct Node *FlipList(struct Node *node)
{
	struct Node *new_head = NULL;
	/*base case*/
	if (node->next == NULL)
		return node;

	new_head = FlipList(node->next);

	/*once the recursive calls reach its base case the last node is assigned to new_head
	  which will be the new head of the reversed list. No more recursive calls are made after this.
	  Then the call stack begins to unwind, setting the next node's next to the current node.
	  (if node is 3 then node->next is 4, so 4's next points back at node (3)*/
	node->next->next = node;

	/*the last node is now null to break the original forward link*/
	node->next = NULL;

	return new_head;
}

struct Node *CreateList(int data)
{
	struct Node *node = malloc(sizeof(struct Node));

	node->data = data;

	node->next = NULL;

	return node;
}

void PrintList(struct Node *node)
{
	while (NULL != node)
	{
		printf("%d \n", node->data);

		node = node->next;
	}

	printf("NULL\n");
}

void FreeList(struct Node *node)
{
	struct Node *current = node;
	struct Node *next = NULL;

	while (NULL != current)
	{
		next = current->next;

		free(current);

		current = next;
	}
}