#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/syscall.h>

#define BAD_ARGS_COUNT 1
#define BAD_NUM_THREADS 2

#define MY_BUFSIZ 50

char buffer[MY_BUFSIZ+2];

const int MAX_NUM_THREADS = 10;
bool stop = false;

void *write_to_buffer(void  *arg) {
    char sym = *(char *)arg;
    int index = 0;
    fprintf(stderr, "thread %c tid = %ld\n", sym, syscall(SYS_gettid));
    while (!stop) {
        buffer[index] = sym;
        index = (index + 1) % MY_BUFSIZ;
    }
    pthread_exit(NULL);
}

static void sigCatch(int sig) {
    if (sig == SIGINT) {
        stop = true;
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "bad amount of arguments got %d expected 2\n", argc);
        exit(BAD_ARGS_COUNT);
    }
    char symbols[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    const int NUM_THREADS = atoi(argv[1]);
    if (NUM_THREADS > 10 || NUM_THREADS < 1) {
        fprintf(stderr, "bad amount of threads got %d expected from 1 to 10\n", NUM_THREADS);
        exit(BAD_NUM_THREADS);
    }

    pthread_t tids[NUM_THREADS];
    memset(buffer, '1', MY_BUFSIZ);
    buffer[MY_BUFSIZ] = '\n';
    buffer[MY_BUFSIZ+1] = '\n';

    struct sigaction sig_act = { 0 };
    sig_act.sa_handler = sigCatch;
    sigemptyset(&sig_act.sa_mask);
    int sigact_res = sigaction(SIGINT, &sig_act, NULL);
    if (sigact_res != 0) {
        perror("sigaction");
        return 1;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int create_res = pthread_create(&tids[i], NULL, write_to_buffer, (void *)&symbols[i]);
        if (create_res != 0) {
            fprintf(stderr, "%s\n", strerror(create_res));
            pthread_exit((void *)1);
        }
    }

    while (!stop) {
        write(STDOUT_FILENO, buffer, MY_BUFSIZ+2);
        sleep(1);
    }

    for (int i = 0 ; i < NUM_THREADS; i++) {
        int join_res = pthread_join(tids[i], NULL);
        if (join_res != 0) {
            fprintf(stderr, "%s\n", strerror(join_res));
        }
    }

    fprintf(stdout, "\nbuf: %p-%p\n", buffer, &buffer[MY_BUFSIZ+1]);
    pthread_exit(NULL);
}

