#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

const int N = 10000000;

#define RUNS_PER_PROC 20
const int SEED[RUNS_PER_PROC] = {
    788159773,
    2052308573,
    1377030627,
    1699618045,
    676203154,
    299802456,
    1767965774,
    1838448927,
    1686836254,
    1335355396,
    1186224,
    74147217,
    898646163,
    106881286,
    10633766,
    1364600012,
    305070600,
    1539146084,
    822350517,
    875518628,
};

int main(int argc, char** argv) {
    // setup MPI
    int ret, size, rank;
    ret = MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        if (!rank)
            printf("Usage: %s [seed_id]\n", argv[0]);
        ret = MPI_Finalize();
        return 0;
    }

    int seed = atoi(argv[1]);
    if ((seed == 0 && argv[1][0] != '0') || seed < 0 || seed > RUNS_PER_PROC) {
        if (!rank)
            puts("Incorrect seed_id");
        ret = MPI_Finalize();
        return 0;
    }

    double start, end;

    // pregenerate array
    int *array = malloc(N * sizeof(int));
    if (!rank) {
        srand(SEED[seed]);
        for (int i = 0; i < N; ++i)
            array[i] = rand();
    }

    if (!rank)
        start = MPI_Wtime();

    MPI_Bcast(array, N, MPI_INTEGER, 0, MPI_COMM_WORLD);

    int lmax = -1;
    const int lstart = rank * N / size;
    const int lend = (rank + 1) * N / size;

    for (int i = lstart; i < lend; ++i)
        if (array[i] > lmax)
            lmax = array[i];
    
    int max;
	MPI_Reduce(&lmax, &max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
	
    if (!rank)
        end = MPI_Wtime();
    
    ret = MPI_Finalize();

    if (!rank)
        printf("%.3f\n", (end - start) * 1000);

    free(array);

	return 0;
}