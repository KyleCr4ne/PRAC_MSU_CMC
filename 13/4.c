#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>


int
main(int argc, char *argv[])
{
    int fd[2], fds;
    pipe(fd);
    fds = dup(0);
    pid_t *process = calloc(argc - 1, sizeof(*process));
    for (int i = 1; i < argc; i++) {
        dup2(fd[0], fds);
        close(fd[0]);
        close(fd[1]);
        if (pipe(fd) < 0) {
            for (int j = 0; j < i; j++) {
                kill(process[j], SIGKILL);
            }
            while (wait(NULL) > 0) {};
            free(process);
            _exit(1);
        }
        if (!(process[i - 1] = fork())) {
            if (i < argc - 1) {
                dup2(fd[1], 1);
            }
            close(fd[0]);
            close(fd[1]);
            if (i > 1) {
                dup2(fds, 0);
                close(fds);
            }
            execlp(argv[i], argv[i], NULL);
            while (wait(NULL) > 0) {};
            _exit(1);
        } else if (process[i - 1] < 0) {
            for (int j = 0; j < i; j++) {
                kill(process[j], SIGKILL);
            }
            while (wait(NULL) > 0) {};
            free(process);
            _exit(1);
        } else {
            if (i > 1) close(fds);
        }
    }
    while (wait(NULL) > 0) {};
    free(process);
    return 0;
}
