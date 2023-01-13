#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define RUNS_PER_THREAD 20

const int N = 10000000;
const int MAX_THREADS = 6;

const int SEED[RUNS_PER_THREAD] = {
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

void randArr(int *array, int size) {
    for (int i = 0; i < size; ++i)
        array[i] = rand();
}

// run algo and return time elapsed
double run(const int threads, int *array, const int size) {
    double start = omp_get_wtime();
    int max = -1;
    #pragma omp parallel num_threads(threads) shared(array, size) reduction(max: max) default(none)
    {
        #pragma omp for
        for(int i = 0; i < size; ++i) {
            if(array[i] > max) {
                max = array[i];
            };
        }
    }
    double end = omp_get_wtime();
    return (end - start) * 1000;
}

int main(int argc, char **argv) {    
    int *array = (int *)malloc(N * sizeof(int));

    puts("Threads,Worst (ms),Best (ms),Avg (ms)");

    for (int threads = 1; threads < MAX_THREADS + 1; ++threads) {
        double sum = 0, max_time = -1, min_time = 100000;
        for (int i = 0; i < RUNS_PER_THREAD; ++i) {
            // gen array with special seed
            srand(SEED[i]);
            randArr(array, N);

            // calc value
            double time = run(threads, array, N);
            if (time > max_time)
                max_time = time;
            if (time < min_time)
                min_time = time;
            sum += time;
        }
        
        printf("%d,%.3f,%.3f,%.3f\n", threads, max_time, min_time, sum / RUNS_PER_THREAD);
    }

    free(array);

    return 0;
}