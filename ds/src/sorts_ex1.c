

int BinaryIndex(int array[], int size, int target)
{
	int left, right, middle;
	left = 0;
	right = size - 1;

	while (left <= right)
	{
		middle = left + (right - left) / 2;

		if (array[middle] == target)
			return middle;

		if (array[middle] < target)
			left = middle + 1;

		else
			right = middle - 1;
	}

	return 1;
}