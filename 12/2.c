#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>


enum
{
    ARG_CNT = 4
};

int
prg(const char *cmd)
{
    pid_t pid;
    int st;
    if ((pid = fork()) < 0) {
        _exit(1);
    } else if (!pid) {
        execlp(cmd, cmd, NULL);
        _exit(1);
    } else {
        wait(&st);
        return WIFEXITED(st) && !WEXITSTATUS(st);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < ARG_CNT) {
        fprintf(stderr, "Expected three args!\n");
        return 1;
    }
    return !((prg(argv[1]) || prg(argv[2])) && prg(argv[3]));
}
