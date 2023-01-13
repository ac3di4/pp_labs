#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#if _OPENMP == 200505
#define _OPENMP_VERSION "2.5"
#elif _OPENMP == 200805
#define _OPENMP_VERSION "3.0"
#elif _OPENMP == 201107
#define _OPENMP_VERSION "3.1"
#elif _OPENMP == 201307
#define _OPENMP_VERSION "4.0"
#elif _OPENMP == 201511
#define _OPENMP_VERSION "4.5"
#elif _OPENMP == 201811
#define _OPENMP_VERSION "5.0"
#elif _OPENMP == 202011
#define _OPENMP_VERSION "5.1"
#else
#define _OPENMP_VERSION "unknown"
#endif

int main(int argc, char** argv) {
    printf("OpenMP Version: %s\nRelease date: %d\n", _OPENMP_VERSION, _OPENMP);
    
    printf("\nAvaliable processors: %d\nAvaliable threads: %d\n", omp_get_num_procs(), omp_get_max_threads());
    
    if (omp_get_dynamic())
        puts("\nDynamic is on");
    else
        puts("\nDynamic is off");

    printf("\nOpenMP wtick: %fs\n", omp_get_wtick());

    if (omp_get_nested())
        printf("\nNested parallelism up to %d\n", omp_get_max_active_levels());
    else
        puts("Nested parallelism is off");


    omp_sched_t sched;
    int chunk_size;
    omp_get_schedule(&sched, &chunk_size);
    char *s;
    switch (sched) {
        case omp_sched_static: s = "static"; break;
        case omp_sched_dynamic: s = "dynamic"; break;
        case omp_sched_guided: s = "guided"; break;
        case omp_sched_auto: s = "auto"; break;
    }
    printf("\nOpenMP schedule: %s\nChunk size: %d\n", s, chunk_size);

    return 0;
}
