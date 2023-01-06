#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    const int count = 1000000;      ///< Number of array elements
    const int random_seed = 920214; ///< RNG seed

    int* array = 0;                 ///< The array which we are sorting

    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }

    clock_t start = clock();
    
    /* shellsort */
    for (int d = count / 2; d > 0; d /= 2) {
        for (int i = d; i < count; ++i) {
            for (int j = i - d; j >= 0 && array[j] > array[j + d]; j -= d) {
                int temp = array[j];
                array[j] = array[j + d];
                array[j + d] = temp;
            }
        }
    }

    clock_t end = clock();

    const double CLOCKS_PER_MS = (double)CLOCKS_PER_SEC / 1000;
    double total = (double)(end - start) / CLOCKS_PER_MS;
    printf("%.3f", total);

    free(array);

    return 0;
}
