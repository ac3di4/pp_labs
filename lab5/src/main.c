#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char** argv)
{
	int ret  = -1;	///< For return values
	int size = -1;	///< Total number of processors
	int rank = -1;	///< This processor's number

	const int count = 1e1; ///< Number of array elements
	const int random_seed = 920215; ///< RNG seed
	clock_t start, end;

	int* array = 0; ///< The array we need to find the max in
	int lmax = -1;	///< Local maximums
	int  max = -1;  ///< The maximal element

	/* Initialize the MPI */
	ret = MPI_Init(&argc, &argv);

	/* Determine our rankand processor count */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	/* Allocate the array */
	array = (int*)malloc(count * sizeof(int));

	/* Master generates the array and starts the timer */
	if (!rank) {
		/* Initialize the RNG */
		srand(random_seed);
		/* Generate the random array */
		for (int i = 0; i < count; i++) { array[i] = rand(); }

		start = clock();
	}
	/* Send the array to all other processors */
	MPI_Bcast(array, count, MPI_INTEGER, 0, MPI_COMM_WORLD);

	const int wstart = (rank    ) * count / size;
	const int wend   = (rank + 1) * count / size;

	for (int i = wstart;
		i < wend;
		i++)
	{
		if (array[i] > lmax) { lmax = array[i]; }
	}

	MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);

	if (!rank) {
		end = clock();
		const double CLOCKS_PER_MS = (double)CLOCKS_PER_SEC / 1000;
		double total = (double)(end - start) / CLOCKS_PER_MS;
		printf("%.3f", total);
	}

	ret = MPI_Finalize();


	return 0;
}
