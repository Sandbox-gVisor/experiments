#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/wait.h>

#define BAD_ARGS_COUNT 1
#define BAD_NUM_PROC 2
#define OPEN_FILE_FAILED 3
#define WRITE_FAILED 4
#define MMAP_FAILED 5

#define MY_BUFSIZ 50

const int MAX_NUM_PROC = 10;
//                          1_000_000_000
const int64_t NUM_ITERATIONS = 1000000000;

char *shared_buffer;

void write_to_buffer(char sym) {
    int index = 0;
    fprintf(stderr, "process %c pid = %d\n", sym, getpid());
    for (int64_t i = 0; i < NUM_ITERATIONS; i++) {
        shared_buffer[index] = sym;
        index = (index + 1) % MY_BUFSIZ;
    }
    munmap(shared_buffer, (MY_BUFSIZ+2) * sizeof(char));
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "bad amount of arguments got %d expected 2\n", argc);
        exit(BAD_ARGS_COUNT);
    }
    char symbols[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};

    const int NUM_PROC = atoi(argv[1]);
    if (NUM_PROC > MAX_NUM_PROC || NUM_PROC < 1) {
        fprintf(stderr, "bad amount of processes got %d expected from 1 to 10\n", NUM_PROC);
        exit(BAD_NUM_PROC);
    }

    int fd = open("tmp", O_CREAT|O_RDWR, 0600);
    if (fd < 0) {
        perror("failed to open tmp file");
        exit(OPEN_FILE_FAILED);
    }

    char buffer[MY_BUFSIZ+2];
    memset(buffer, '1', MY_BUFSIZ);
    buffer[MY_BUFSIZ] = '\n';
    buffer[MY_BUFSIZ+1] = '\n';

    ssize_t written = write(fd, buffer, MY_BUFSIZ+2);
    if (written < 0) {
        perror("write");
        exit(WRITE_FAILED);
    }

    shared_buffer = mmap(NULL, (MY_BUFSIZ+2) * sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_buffer == NULL) {
        perror("mmap");
        exit(MMAP_FAILED);
    }
    close(fd);

    pid_t children[MAX_NUM_PROC];

    for (int i = 0; i < NUM_PROC; i++) {
        pid_t fork_res = fork();
        if (fork_res == 0) {
            // child
            write_to_buffer(symbols[i]);
            return 0;
        } else {
            // parent
            children[i] = fork_res;
        }
    }

    for (int i = 0; i < 20; i++) {
        write(STDOUT_FILENO, shared_buffer, MY_BUFSIZ+2);
        sleep(1);
    }

    for (int i = 0; i < NUM_PROC; i++) {
        int waitstatus;
        int wait_res = wait(&waitstatus);
        if (wait_res == -1) {
            perror("wait");
        } else {
            fprintf(stdout, "child %d finished\n", wait_res);
        }
    }
    fprintf(stdout, "\nbuf: %p-%p\n", shared_buffer, &shared_buffer[MY_BUFSIZ+1]);
    munmap(shared_buffer, (MY_BUFSIZ+2)* sizeof(char));
    pthread_exit(NULL);
}

