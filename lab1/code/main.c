#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int N = 10000000;
const int MAX_THREADS = 16;
const int RUNS_PER_THREAD = 20;

void randArr(int *array, int size) {
    for (int i = 0; i < size; ++i)
        array[i] = rand();
}

// run algo and return time elapsed
double run(const int threads, int *array, const int size) {
    clock_t start = clock();
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
    clock_t end = clock();
    const double CLOCKS_PER_MS = (double)CLOCKS_PER_SEC / 1000;
    return (double)(end - start) / CLOCKS_PER_MS;
}

int main(int argc, char **argv) {
    // set constant seeds
    int seed[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; ++i)
        seed[i] = rand();
    
    
    int *array = (int *)malloc(N * sizeof(int));

    puts("Threads,Worst (ms),Best (ms),Avg (ms)");

    for (int threads = 1; threads < MAX_THREADS + 1; ++threads) {
        double sum = 0, max_time = -1, min_time = 100000;
        for (int i = 0; i < RUNS_PER_THREAD; ++i) {
            // gen array with special seed
            srand(seed[i]);
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