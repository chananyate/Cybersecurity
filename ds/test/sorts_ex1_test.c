#include <stdio.h>

int BinaryIndex(int array[], int size, int target);

int main()
{
	int index;
	int size = 8;
	int target = 9;
	int array[] = {1, 3, 5, 7, 9, 11, 13, 15};

	index = BinaryIndex(array, size, target);

	printf("%d\n", index);

	return 0;
}