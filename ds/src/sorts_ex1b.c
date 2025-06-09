
int BinaryIndexRecursive(int array[], int target, int left, int right)
{
	int middle;

	if (left > right)
		return 1;

	middle = left + (right - left) / 2;

	if (array[middle] == target)
			return middle;

	else if (array[middle] < target)
		return BinaryIndexRecursive(array, target, middle + 1, right);

	return BinaryIndexRecursive(array, target, left, middle - 1);
}