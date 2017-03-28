#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Up sweep, recursive implementation
void up_sweep(int *arr, int n, int N)
{
	// Base case, reached after log N iterations
	if(n == 0)
		return;

	// Calculate the size of the step for array access
	int step = N / n;

	// Begin parallel section

	// We'll use thread_id to calculate array access indices
	int thread_id;
	omp_set_num_threads(n);

	#pragma omp parallel private(thread_id)
	{
		// Get thread number, we'll use this to determine the array index
		thread_id = omp_get_thread_num();

		// Calculate the index that belongs to this thread
		int index = thread_id * step + step - 1;

		// Remember that the previous nodes had a step of (step / 2)
		int half_step = step / 2;

		// Indices of the values that will be added into arr[index]
		int left = thread_id * step + half_step - 1;
		int right = left + half_step;

		// Get the sum
		int sum = arr[left] + arr[right];

		// Update arr
		#pragma omp barrier
		{
			arr[index] = sum;
		}
	}
	// End parallel section

	// Recursive step
	up_sweep(arr, n / 2, N);
}

void down_sweep(int *arr, int n, int N)
{
	// Base case, reached after log N iterations
	if(n == N)
		return;

	// Calculate the size of the step for array access
	int step = N / n;

	// Begin parallel section

	// We'll use thread_id to calculate array access indices
	int thread_id;
	omp_set_num_threads(n);

	#pragma omp parallel private(thread_id)
	{
		// Get thread number, we'll use this to determine the array index
		thread_id = omp_get_thread_num();

		// Calculate the index that belongs to this thread
		int index = thread_id * step + step - 1;

		// Remember that the previous nodes had a step of (step / 2)
		int half_step = step / 2;

		// Get the indices of left and right children
		int left = thread_id * step + half_step - 1;
		int right = left + half_step;

		// Value that will be stored in arr[left]
		int new_left = arr[index];

		// Value that will be stored in arr[right]
		int new_right = arr[index] + arr[left];

		// Update arr
		#pragma omp barrier
		{
			// Update left and right children
			arr[left] = new_left;
			arr[right] = new_right;
		}
	}
	// End parallel section

	// Recursive step
	down_sweep(arr, n * 2, N);
}

int main()
{

	// We use N as the smallest integer such that N >= elements and N = 2^k,
	int elements, N;

	// Get user input
	printf("Please specify the elements in the array (any n > 1).\n");
	scanf("%d", &elements);

	// Validate
	if (elements < 2)
	{
		printf("There should be a number of elements greater than 1.\n");
		return 1;
	}

	// Calculate N
	N = 1;
	while(N < elements)
		N *= 2;

	// Global array that stores the elements, size equal to N
	int *arr = malloc(N * sizeof(int));

	// Fill the first 'elements' cells with radom numbers
	srand(time(NULL));
	for(int i = 0; i != elements; ++i)
		arr[i] = rand() % 10;

	// Fill the rest of the cells with 0s
	for(int i = elements; i != N; ++i)
		arr[i] = 0;

	// Output the original elements
	printf("\nElements in the array: (randomly generated)\n");
	for(int i = 0; i != elements; ++i)
		printf("%d, ", arr[i]);
	printf("\n\n***\n");

	// First phase: up sweep
	up_sweep(arr, N / 2, N);

	// Update the root to 0, otherwise down_sweep() does not work properly
	arr[N - 1] = 0;

	// Second phase: down sweep
	down_sweep(arr, 1, N);

	// Output results
	printf("\nPrefix sums: (s0, s2, ... , sN - 1)\n");
	for(int i = 0; i != elements; ++i)
		printf("%d, ", arr[i]);
	printf("\n\n");

	// Cleanup
	free(arr);

	return 0;
}
