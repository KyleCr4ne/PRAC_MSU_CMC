#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <errno.h>

enum
{
    SON_1 = 1,
    SON_2 = 2,
    ARG_CNT = 2
};

int
proc(FILE *in, FILE *out, int tmp, long long mx, int pid_n)
{
    if (tmp < mx) {
        printf("%d %d\n", pid_n, tmp++);
        fflush(stdout);
        fprintf(out, "%d\n", tmp);
        fflush(out);
    } else {
        fclose(in);
        fclose(out);
        return 1;
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected one arg!\n");
        return 1;
    }
    char *end_ptr = NULL;
    errno = 0;
    long long mx = strtoll(argv[1], &end_ptr, 10);
    if (errno || *end_ptr || argv[1] == end_ptr) {
        fprintf(stderr, "Strtoll Error!\n");
        return 1;
    }
    int fd1[2], fd2[2];
    if (pipe(fd1) < 0 || pipe(fd2) < 0) {
        fprintf(stderr, "Pipe Error!\n");
        return 1;
    }
    FILE *in, *out;
    pid_t pid1, pid2;
    out = fdopen(fd1[1], "w");
    fprintf(out, "1\n");
    fflush(out);
    if (!(pid1 = fork())) {
        close(fd2[0]);
        in = fdopen(fd1[0], "r");
        out = fdopen(fd2[1], "w");
        close(fd1[1]);
    } else if (!(pid2 = fork())) {
        close(fd1[0]);
        in = fdopen(fd2[0], "r");
        out = fdopen(fd1[1], "w");
        close(fd2[1]);
    } else {
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
    }
    int tmp = 0;
    while (!pid1 || !pid2) {
        if (!pid1) {
            if (fscanf(in, "%d", &tmp) == EOF) {
                fclose(in);
                fclose(out);
                _exit(0);
            } else {
                if (proc(in, out, tmp, mx, SON_1)) _exit(0);
            }

        } else if (!pid2) {
            if (fscanf(in, "%d", &tmp) == EOF) {
                fclose(in);
                fclose(out);
                _exit(0);
            } else {
                if (proc(in, out, tmp, mx, SON_2)) _exit(0);
            }
        }
    }
    wait(NULL);
    wait(NULL);
    printf("Done\n");
    return 0;
}
