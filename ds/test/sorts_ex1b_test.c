#include <stdio.h>

int BinaryIndexRecursive(int array[], int target, int left, int right);

int main()
{
	int index, left, right, size;
	int target = 9;
	int array[] = {1, 3, 5, 7, 9, 11, 13, 15};

	size = sizeof(array) / sizeof(array[0]);
	left = 0;
	right = size - 1;


	index = BinaryIndexRecursive(array, target, left, right);

	printf("%d\n", index);

	return 0;
}