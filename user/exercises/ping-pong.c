#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

int main() {
    signal(SIGPIPE, SIG_IGN); // ignore SIGPIPE to prevent program from crashing after last hit
    int p[2];
    int p2[2];
    pipe(p);
    pipe(p2);
    char byte = 'x';
    char buffer[1];
    size_t num_iter = 10;
    pid_t child = fork();
    if (child < 0) {
        // error
        printf("something went wrong");
        return 1;
    } else if (child == 0) {
        // child process
        close(p[1]);
        close(p2[0]);
        for (size_t i = 0; i < num_iter; i++) {
            ssize_t b_r = read(p[0], buffer, sizeof(buffer));
            if (b_r > 0) {
                printf("Child Received!: %c\n", buffer[0]);
            }
            printf("Smash!\n");
            ssize_t wr = write(p2[1], buffer, sizeof(buffer));
            if (wr < 0) printf("Child wins!\n");
        }
        printf("child leaves the game!\n");
        close(p[0]);
        close(p2[1]);
    } else {
        // parent process
        close(p[0]);
        close(p2[1]);
        printf("Serving...\n");
        clock_t start = clock();
        write(p[1], &byte, sizeof(byte));
        for (size_t i = 0; i < num_iter; i++) {
            ssize_t b_r2 = read(p2[0], buffer, sizeof(buffer));
            if (b_r2 > 0) {
                printf("Parent Received!: %c\n", buffer[0]);
            }
            printf("Smash!\n");
            ssize_t wr2 = write(p[1], buffer, sizeof(buffer));
            if (wr2 < 0) printf("Parent wins!\n");
        }
        // cleanup child process
        int status;
        waitpid(child, &status, 0);
        close(p2[0]);
        close(p[1]);
        clock_t end = clock();
        double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
        printf("Elapsed time: %f\n", elapsed);
        printf("Exchanges Per Sec: %d", (int)((1.0 / elapsed)*num_iter));
    }
    return 0;
}