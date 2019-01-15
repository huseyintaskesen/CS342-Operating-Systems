#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000000
#define ITER_NUM 100

int main()
{
	printf("It will take some time (~1min), get PID of me and insmod kernel driver...\n");

	for (int i = 0; i < ITER_NUM; i++)
	{
		int *array = malloc(SIZE * sizeof(int));
		if (array == NULL)
		{
		  	printf("Failed to allcate 100MB of space\n");
		  	return(-1);
		}

		for (int i = 0; i < SIZE; i++)
		{
			array[i] = 0;
		}

		free(array);
	}

	return 0;
}
