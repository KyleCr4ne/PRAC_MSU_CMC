#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#include <time.h>
#include <limits.h>
#include <sys/mman.h>
#include <math.h>

enum
{
    ARG_CNT = 5
};
//CMD1 FILE2 CMD3 CMD4

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Need four arguments");
        return 1;
    }
    char *cmd1 = strdup(argv[1]);
    char *file2 = strdup(argv[2]);
    char *cmd3 = strdup(argv[3]);
    char *cmd4 = strdup(argv[4]);
    /*
     * {CMD1 < FILE2 | CMD3 } && CMD4
     */
    int fd[2];
    pipe(fd);
    pid_t pid1;
    if (!(pid1 = fork())) {
        int f = open(file2, O_RDONLY);
        dup2(fd[1], STDOUT_FILENO);
        dup2(f, STDIN_FILENO);
        close(f);
        close(fd[1]);
        close(fd[0]);
        execlp(cmd1, cmd1, NULL);
        exit(1);
    }
    pid_t pid2;
    if (!(pid2 = fork())) {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        close(fd[0]);
        execlp(cmd3, cmd3, NULL);
        exit(1);
    }
    close(fd[1]);
    close(fd[0]);
    int st1, st2;
    wait(&st1);
    wait(&st2);
    pid_t pid3;
    if (WIFEXITED(st1) && !WEXITSTATUS(st1) && WIFEXITED(st2) && !WEXITSTATUS(st2)) {
        if (!(pid3 = fork())) {
            execlp(cmd4, cmd4, NULL);
            exit(1);
        }
    }
    wait(NULL);
    return 0;
}
