#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

enum
{
    SHIFT = 1900
};

int
main(void)
{
    int fd[2];
    pipe(fd);
    int pid1, pid2, pid3;
    time_t tmp;
    struct tm *now = calloc(1, sizeof(*now));
    if ((pid1 = fork()) > 0) {
        waitpid(pid1, NULL, 0);
        read(fd[0], &tmp, sizeof(tmp));
        now = localtime_r(&tmp, now);
        int year = now->tm_year + SHIFT;
        printf("Y:%04d\n", year);
        fflush(stdout);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    if ((pid2 = fork()) > 0) {
        waitpid(pid2, NULL, 0);
        read(fd[0], &tmp, sizeof(tmp));
        now = localtime_r(&tmp, now);
        int month = now->tm_mon + 1;
        printf("M:%02d\n", month);
        fflush(stdout);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    if ((pid3 = fork()) > 0) {
        waitpid(pid3, NULL, 0);
        read(fd[0], &tmp, sizeof(tmp));
        now = localtime_r(&tmp, now);
        int day = now->tm_mday;
        printf("D:%02d\n", day);
        fflush(stdout);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    time_t t = time(NULL);
    for (int i = 0; i < 3; ++i) {
        write(fd[1], &t, sizeof(t));
    }
    return 0;

}
