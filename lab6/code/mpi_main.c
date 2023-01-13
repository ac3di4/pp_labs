#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

const int N = 1000000;

#define RUNS_PER_PROC 10
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
};

int main(int argc, char** argv) {
    // Init MPI
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        if (!rank)
            printf("Usage: %s [seed_id]\n", argv[0]);
        return MPI_Finalize();
    }

    int seed = atoi(argv[1]);
    if ((seed == 0 && argv[1][0] != '0') || seed < 0 || seed > RUNS_PER_PROC) {
        if (!rank)
            puts("Incorrect seed_id");
        return MPI_Finalize();
    }

    int *array;
    if (!rank) {
        array = malloc(N * sizeof(int));
        srand(SEED[seed]);
        for (int i = 0; i < N; ++i)
            array[i] = rand();
    }

    // if (!rank) {
    //     for (int i = 0; i < N; ++i)
    //         printf("%d ", array[i]);
    //     puts("");
    // }

    // set up proc slice length
    const int WORKERS = size - 1;
    int length = N / WORKERS;
    if (rank == size - 1)
            length += N % WORKERS;

    // local proc slice
    int *slice;
    if (rank) {
        slice = malloc(length * sizeof(int));
    }

    double start;
    if (!rank)
        start = MPI_Wtime();

    // parallel shell sort
    MPI_Status s;
    if (!rank) {
        // split array
        for (int p = 0; p < WORKERS - 1; ++p)
            MPI_Send(array + length * p, length, MPI_INTEGER, p + 1, 0, MPI_COMM_WORLD);
        const int pad = (WORKERS - 1) * length;
        MPI_Send(array + pad, N - pad, MPI_INTEGER, WORKERS, 0, MPI_COMM_WORLD);

        // collect array
        for (int p = 0; p < WORKERS - 1; ++p)
            MPI_Recv(array + length * p, length, MPI_INTEGER, p + 1, 0, MPI_COMM_WORLD, &s);
        MPI_Recv(array + pad, N - pad, MPI_INTEGER, WORKERS, 0, MPI_COMM_WORLD, &s);
    } else {
        // recv array
        MPI_Recv(slice, length, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, &s);

        // sort
        for (int d = length / 2; d > 0; d /= 2)
            for (int i = d; i < length; ++i)
                for (int j = i - d; j >= 0 && slice[j] > slice[j + d]; j -= d) {
                    int temp = slice[j];
                    slice[j] = slice[j + d];
                    slice[j + d] = temp;
                }
        
        // send array
        MPI_Send(slice, length, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
    }

    // merge
    if (!rank) {
        int *idx = (int *)malloc(WORKERS * sizeof(int));
        for (int i = 0; i < WORKERS; ++i) idx[i] = 0;

        int *tmp = array;
        array = (int *)malloc(N * sizeof(int));

        for (int i = 0; i < N; ++i) {
            int pid = -1;
            int min = INT_MAX;

            // search all workers except last one
            for (int p = 0; p < WORKERS - 1; ++p) {
                if (idx[p] >= length)
                    continue;
                int val = tmp[idx[p] + length * p];
                if (val < min) {
                    min = val;
                    pid = p;
                }
            }

            // search last worker
            if (idx[WORKERS - 1] < length) {
                int val = tmp[idx[WORKERS - 1] + length * (WORKERS - 1)];
                if (val < min) {
                    min = val;
                    pid = WORKERS - 1;
                }
            }

            // set array to
            array[i] = min;
            idx[pid]++;
        }

        double end = MPI_Wtime();
        printf("%.3f", (end - start) * 1000);
        // for (int i = 0; i < N; ++i)
        //     printf("%d ", array[i]);
        // puts("");
    }
    
	return MPI_Finalize();
}