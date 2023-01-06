#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv) {
    const int count = 1000000;

    srand(920214);
    int *array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }

    clock_t start = clock();
    
    #if THREADS == 1
    for (int d = count / 2; d > 0; d /= 2) {
        for (int i = d; i < count; ++i) {
            for (int j = i - d; j >= 0 && array[j] > array[j + d]; j -= d) {
                int temp = array[j];
                array[j] = array[j + d];
                array[j + d] = temp;
            }
        }
    }
    #else
    #pragma omp parallel num_threads(THREADS) shared(array, count) default(none)
    for (int d = count / 2; d > 0; d /= 2) {
        const int cd = d;
        #ifdef CHUNK_SIZE
        #pragma omp for schedule(SCHEDULE, CHUNK_SIZE)
        #else
        #pragma omp for schedule(SCHEDULE)
        #endif
        for (int i = cd; i < count; ++i) {
            for (int j = i - cd; j >= 0 && array[j] > array[j + cd]; j -= cd) {
                int temp = array[j];
                array[j] = array[j + cd];
                array[j + cd] = temp;
            }
        }
    }
    #endif

    clock_t end = clock();

    const double CLOCKS_PER_MS = (double)CLOCKS_PER_SEC / 1000;
    double total = (double)(end - start) / CLOCKS_PER_MS;
    printf("%.3f", total);

    free(array);

    return 0;
}
