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
    ARG_CNT = 2
};
long long mx;
int target_pid, cnt;
int fd[2];
int status;
int pid_n = 1;

void
handler(int s)
{
    if (cnt < mx - 1) {
        read(fd[0], &cnt, sizeof(cnt));
        printf("%d %d\n", pid_n, cnt);
        fflush(stdout);
        cnt++;
        write(fd[1], &cnt, sizeof(int));
        kill(target_pid, SIGUSR1);
    } else if (target_pid == getppid()) {
        close(fd[1]);
        close(fd[0]);
        exit(0);
    } else {
        kill(target_pid, SIGUSR1);
    }
}


int
main(int argc, char *argv[])
{
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected one arg!\n");
        return 1;
    }
    char *end_ptr = NULL;
    errno = 0;
    mx = strtoll(argv[1], &end_ptr, 10);
    if (mx == 2) {
        printf("1 1\nDone\n");
        return 0;
    }
    if (errno || *end_ptr || argv[1] == end_ptr) {
        fprintf(stderr, "Strtoll Error!\n");
        return 1;
    }
    if (pipe(fd) < 0) {
        fprintf(stderr, "Pipe Error!\n");
        return 1;
    }
    signal(SIGUSR1, handler);
    cnt = 1;

    if ((target_pid = fork())) {
        write(fd[1], &cnt, sizeof(cnt));
        while (wait(&status) == -1);
        printf("Done\n");
        close(fd[1]);
        close(fd[0]);
        return 0;
    } else {
        read(fd[0], &cnt, sizeof(cnt));
        target_pid = getppid();
        write(fd[1], &cnt, sizeof(cnt));
        pid_n = 2;
        kill(target_pid, SIGUSR1);
        for (;;);
    }

}
