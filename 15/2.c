#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>


enum
{
    ARG_CNT = 2,
    C_BIT = 8
};
_Bool True = 1;
volatile sig_atomic_t flag = 0;
volatile sig_atomic_t alr = 0;
volatile sig_atomic_t io = 0;

void
handler1(int s)
{
    flag = 1;
}

void
handler2(int s)
{
    flag = 2;
}

void
handler_alr(int s)
{
    alr = 1;
}

void
handler_io(int s)
{
    io = 1;
}

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected one argument!\n");
        _exit(1);
    }
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGUSR2, &(struct sigaction) { .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGALRM, &(struct sigaction) { .sa_handler = SIG_IGN, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGIO, &(struct sigaction) { .sa_handler = handler_io, .sa_flags = SA_RESTART }, NULL);

    int pid1;
    if (!(pid1 = fork())) {
        unsigned char mask = 1, buf = 0;
        while (True) {
            if (io) {
                _exit(0);
            }
            while (!flag && !io) {};
            if (io) {
                _exit(0);
            }
            if (flag == 2) {
                buf |= mask;
            }
            flag = 0;
            mask <<= 1;
            if (!mask) {
                printf("%c", buf);
                fflush(stdout);
                mask = 1;
                buf = 0;
            }
            kill(0, SIGALRM);
        }
    } else if (pid1 < 0) {
        _exit(1);
    }

    int pid2;
    if (!(pid2 = fork())) {
        sigaction(SIGALRM, &(struct sigaction) { .sa_handler = handler_alr, .sa_flags = SA_RESTART }, NULL);
        int fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            kill(pid1, SIGIO);
            _exit(1);
        }
        while (True) {
            unsigned char buf = 0;
            if (read(fd, &buf, sizeof(buf)) <= 0) {
                kill(pid1, SIGIO);
                close(fd);
                _exit(0);
            }
            for (int i = 0; i < C_BIT; i++, buf >>= 1) {
                if (buf & 1u) {
                    kill(pid1, SIGUSR2);
                } else {
                    kill(pid1, SIGUSR1);
                }
                while (!alr) {};
                alr = 0;
            }
        }
    } else if (pid2 < 0) {
        _exit(1);
    }

    while (wait(NULL) > 0) {};
    _exit(0);
}
