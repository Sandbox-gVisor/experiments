#include <stdio.h>
#include <malloc.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define NOT_ENOUGH_ARGS         1
#define NULL_ALLOC              2
#define THREAD_CREATE_FAILURE   3


double *data;
unsigned int EXPERIMENTS_COUNT;
unsigned int BATCH_SIZE;
const clockid_t CLOCK_CHOICE = CLOCK_MONOTONIC_RAW;

const int MATRIX_SIZE = 6;
float *matrix_A;
float *matrix_B;
float *matrix_C;

float rand_val() {
    return (rand() % 2001 - 1000.0) / 1000;
}

void fill_matrices(float **matrix_A, float **matrix_B, float **matrix_C) {
    srand(time(NULL));
    *matrix_A = (float *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
    *matrix_B = (float *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
    *matrix_C = (float *)calloc(MATRIX_SIZE * MATRIX_SIZE, sizeof(float));
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
        (*matrix_A)[i] = rand_val();
        (*matrix_B)[i] = rand_val();
    }
}

void mul_matrices(float *matrix_A, float *matrix_B, float *matrix_C) {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matrix_C[i * MATRIX_SIZE + j] = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matrix_C[i * MATRIX_SIZE + j] += matrix_A[i * MATRIX_SIZE + k] * matrix_B[k * MATRIX_SIZE + j];
            }
        }
    }
}

void *thread_func(void *arg) {
    int *index_ptr = (int *)arg;
    int offset = *index_ptr * EXPERIMENTS_COUNT;
    fprintf(stdout, "thread_idx = %d, offset = %d\n", *index_ptr, offset);

    struct timespec start, end;

    float *A, *B, *C;
    fill_matrices(&A, &B, &C);

    for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
        clock_gettime(CLOCK_CHOICE, &start);
        for (int j = 0; j < BATCH_SIZE; j++) {
            mul_matrices(A, B, C);
        }
        clock_gettime(CLOCK_CHOICE, &end);
        data[offset + i] = end.tv_sec - start.tv_sec + 0.000000001 * (end.tv_nsec - start.tv_nsec);
        fprintf(stdout, "%d;%d;%lf;\n", i+1, *index_ptr, data[offset + i]);
    }

    pthread_exit((void *)EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    // argv[0]: executable
    // argv[1]: NUM_CHILD_THREADS
    // argv[1]: EXPERIMENTS_COUNT
    // argv[2]: BATCH_SIZE
    // argv[3]: file to store results
    if (argc != 5) {
        fprintf(stderr, "expected 4 arguments, got %d\n", argc);
        return NOT_ENOUGH_ARGS;
    }

    const unsigned int NUM_CHILD_THREADS = atoi(argv[1]);
    EXPERIMENTS_COUNT = atoi(argv[2]);
    BATCH_SIZE = atoi(argv[3]);

    data = (double *)calloc(EXPERIMENTS_COUNT * NUM_CHILD_THREADS, sizeof(double));
    if (data == NULL) {
        perror("calloc for experiment data");
        return NULL_ALLOC;
    }
    pthread_t *tids = (pthread_t *) calloc(NUM_CHILD_THREADS, sizeof(pthread_t));
    if (tids == NULL) {
        perror("calloc for tids");
        free(data);
        return NULL_ALLOC;
    }
    int *params = (int *) calloc(NUM_CHILD_THREADS, sizeof(int));
    if (params == NULL) {
        perror("calloc for thread params");
        free(data);
        free(params);
        return NULL_ALLOC;
    }

    for (int i = 0; i < NUM_CHILD_THREADS; i++) {
        params[i] = i;
        int create_res = pthread_create(&tids[i], NULL, thread_func, &params[i]);
        if (create_res != 0) {
            fprintf(stderr, "%s\n", strerror(create_res));
            pthread_exit((void *)THREAD_CREATE_FAILURE);
        }
    }

    for (int i = 0 ; i < NUM_CHILD_THREADS; i++) {
        int join_res = pthread_join(tids[i], NULL);
        if (join_res != 0) {
            fprintf(stderr, "%s\n", strerror(join_res));
        }
    }

    FILE* data_file = fopen(argv[4], "w");
    for (int j = 0; j < NUM_CHILD_THREADS; j++) {
        for (int i = 0; i < EXPERIMENTS_COUNT; i++) {
            fprintf(data_file, "%d;%d;%lf;\n", i+1, j, data[j * EXPERIMENTS_COUNT + i]);
        }
    }
    fclose(data_file);
    free(data);
    free(params);
    free(tids);
    free(matrix_A);
    free(matrix_B);
    free(matrix_C);
    pthread_exit((void *)EXIT_SUCCESS);
}
