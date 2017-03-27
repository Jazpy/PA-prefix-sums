#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <time.h>

// Recursive version of the algorithm
void prefix_sums(int *arr, int n)
{

}

int main()
{

	// We use N is the smallest integer such that N >= elements and N = 2^k,
	// thread_id is used by omp
	int elements, N, thread_id;

	// Get user input
	printf("Please specify the elements in the array (any n >= 1).\n");
	scanf("%d", &elements);

	// Validate
	if (elements < 1)
	{
		printf("There should be a number of threads equal to or greater than 1.\n");
		return 1;
	}

	// Calculate N
	N = 1;
	while(N < elements)
	{
		N *= 2;
	}

	// Global array that stores the elements, size equal to N
	int *arr = malloc(N * sizeof(int));

	// Fill the first 'elements' cells with radom numbers
	srand(time(NULL));

	for(int i = 0; i != elements; ++i)
		arr[i] = rand() % 10;

	// Fill the rest of the cells with 0s
	for(int i = elements; i != N; ++i)
		arr[i] = 0;

	// Output the elements, for verification purposes
	for(int i = 0; i != elements; ++i)
		printf("%d, ", arr[i]);
	printf("\n");

	// Begin parallel section
	omp_set_num_threads(N);

	#pragma omp parallel private(thread_id)
  {
		// Get thread number, we'll use this to determine the array index
		thread_id = omp_get_thread_num();


	} // End parallel section

	// Output results
	printf("\nPrefix sums: (s1, s2, ... , sN)\n");
	for(int i = 0; i != elements; ++i)
		printf("%d, ", arr[i]);
	printf("\n");

	// Cleanup
	free(arr);

	return 0;
}
