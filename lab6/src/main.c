/*
    The strategy:
    - Distribute array between every process
    - Shell sort it
    - Merge everything in one thread
*/
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <limits.h>

void shellsort(int *array, int length) {
    for (int d = length / 2; d > 0; d /= 2)
        for (int i = d; i < length; ++i)
            for (int j = i - d; j >= 0 && array[j] > array[j + d]; j -= d) {
                int tmp = array[j];
                array[j] = array[j + d];
                array[j + d] = tmp;
            }
}

int main(int argc, char** argv) {
    // Init MPI
    MPI_Init(&argc, &argv);

    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Init and clocks
    clock_t start, end;
    const int length = 1000000;
    int *array;

    if (!rank) {
        srand(920214);
        array = (int *)malloc(length * sizeof(int));
        for (int i = 0; i < length; ++i) array[i] = rand();
        start = clock();
    }

    // shellsort scatter
    int pad[size], len[size];
    int per_proc = length / size;
    int extra = length % size;

    int padding = 0;
    for (int pid = 0; pid < size; ++pid) {
        pad[pid] = padding;
        len[pid] = (pid < extra) ? per_proc + 1 : per_proc;
        padding += len[pid];
    }

    const int local_len = len[rank];
    int local_array[local_len];

    MPI_Scatterv(array, len, pad, MPI_INTEGER,\
    local_array, local_len, MPI_INTEGER, 0, MPI_COMM_WORLD);

    shellsort(local_array, local_len);
    
    int *tmp;
    if (!rank) {
        tmp = (int *)malloc(length * sizeof(int));
    }
    MPI_Gatherv(local_array, local_len, MPI_INTEGER,\
    tmp, len, pad, MPI_INTEGER, 0, MPI_COMM_WORLD);

    // merge everything in one proc
    if (!rank) {
        int idx[size];
        for (int i = 0; i < size; ++i) idx[i] = 0;
        int i = 0;

        while (1) {
            int pid = -1;
            int min = INT_MAX;
            for (int p = 0; p < size; ++p) {
                if (idx[p] >= len[p])
                    continue;
                int val = tmp[pad[p] + idx[p]];
                if (val < min) {
                    min = val;
                    pid = p;
                }
            }

            if (pid == -1)
                break;

            idx[pid]++;
            array[i] = min;
            i++;
        }

        free(tmp);
    }

    // print result
    if (!rank) {
        end = clock();

        // for (int i = 0; i < length; ++i)
        //     printf("%d ", array[i]);
        // puts("");

        const double CLOCKS_PER_MS = (double)CLOCKS_PER_SEC / 1000;
        double total = (double)(end - start) / CLOCKS_PER_MS;
        printf("%.3f", total);

        free(array);
    }

    MPI_Finalize();

    return 0;
}
