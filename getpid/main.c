#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>

#define NOT_ENOUGH_ARGS 1
#define NULL_ALLOC 2

int main(int argc, char **argv) {
    // argv[0]: executable
    // argv[1]: EXPERIMENTS_COUNT
    // argv[2]: BATCH_SIZE
    // argv[3]: file to store results
    if (argc != 4) {
        fprintf(stderr, "expected 3 arguments, got %d\n", argc);
        return NOT_ENOUGH_ARGS;
    }

    clockid_t clock_choice = CLOCK_MONOTONIC_RAW;
    struct timespec start, end;

    const unsigned int EXPERIMENTS_COUNT = atoi(argv[1]);
    const unsigned int BATCH_SIZE = atoi(argv[2]);

    double *data = (double *)calloc(EXPERIMENTS_COUNT, sizeof(double));
    if (data == NULL) {
        perror("calloc");
        return NULL_ALLOC;
    }

    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        clock_gettime(clock_choice, &start);
        for (int j = 0; j < BATCH_SIZE; j++) {
            pid_t self = getpid();
        }
        clock_gettime(clock_choice, &end);
        data[i] = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        fprintf(stdout, "%d;%lf;\n", i+1, data[i]);
    }

    FILE* data_file = fopen(argv[3], "w");
    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        fprintf(data_file, "%d;%lf;\n", i+1, data[i]);
    }
    fclose(data_file);
    free(data);
    return 0;
}
