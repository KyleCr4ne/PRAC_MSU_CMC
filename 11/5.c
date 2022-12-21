#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

_Bool True = 1;

int
main(void)
{
    pid_t st = getpid();
    while (True) {
        int num;
        if (scanf("%d", &num) < 1) {
            exit(0);
        }
        pid_t pid;
        if ((pid = fork()) < 0) {
            exit(1);
        } else if (!pid) {
            continue;
        } else {
            int status;
            wait(&status);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                printf("%d\n", num);
                exit(0);
            } else {
                if (st == getpid()) {
                    wait(NULL);
                    printf("%d\n", -1);
                    exit(0);
                }
                exit(1);
            }
        }

    }
}
