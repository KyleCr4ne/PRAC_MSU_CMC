#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

enum
{
    ARG_CNT = 6,
    MODE = 0666
};

int
main(int argc, char *argv[])
{
    if (argc < ARG_CNT) {
        fprintf(stderr, "Expected three args!\n");
        return 1;
    }
    char *cmd1 = strdup(argv[1]);
    char *cmd2 = strdup(argv[2]);
    char *cmd3 = strdup(argv[3]);
    char *file1 = strdup(argv[4]);
    char *file2 = strdup(argv[5]);
    int fd[2];
    pipe(fd);
    /*
    * (cmd1 < file1 && cmd2 ) | cmd3 >> file2
    *  cmd1 cmd2 cmd3 file1 file2
    *    1    2     3     4    5
    */
    pid_t pid1;
    if (!(pid1 = fork())) {
        int f = open(file2, O_WRONLY | O_CREAT | O_APPEND, MODE);
        dup2(f, 1);
        close(f);
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        execlp(cmd3, cmd3, NULL);
        _exit(0);
    }
    pid_t pid2;
    if (!(pid2 = fork())) {
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        pid_t pid3;
        if (!(pid3 = fork())) {
            int f = open(file1, O_RDONLY);
            dup2(f, 0);
            close(f);
            execlp(cmd1, cmd1, NULL);
            _exit(1);
        }
        int stat;
        waitpid(pid3, &stat, 0);
        if (WIFEXITED(stat) && !WEXITSTATUS(stat)) {
            execlp(cmd2, cmd2, NULL);
            _exit(1);
        }
        _exit(0);
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(pid2, NULL, 0);
    waitpid(pid1, NULL, 0);
    return 0;
}
