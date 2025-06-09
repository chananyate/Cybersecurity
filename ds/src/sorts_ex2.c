

void BubbleSort(int array[], int size)
{
	int i, j, tmp;

	for (i = 0; i < size - 1; ++i)
	{
		for (j = 0; j < size - 1; ++j)
		{
			if (array[j] > array[j + 1])
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			} 
		}

	}
}