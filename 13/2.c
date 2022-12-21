#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

int
main(void)
{
    int fd[2];
    pipe(fd);

    if (fork() > 0) {
        close(fd[0]);
        int tmp;
        while (scanf("%d", &tmp) == 1) {
            write(fd[1], &tmp, sizeof(tmp));
        }
        close(fd[1]);
        while (wait(NULL) > 0) {};
        exit(0);
    }
    if (fork() > 0) {
        close(fd[0]);
        close(fd[1]);
        while (wait(NULL) > 0) {};
        exit(0);
    }
    close(fd[1]);
    int x;
    long long ans = 0;
    while (read(fd[0], &x, sizeof(x)) > 0) {
        ans += x;
    }
    close(fd[0]);
    printf("%lld\n", ans);
    exit(0);
}
