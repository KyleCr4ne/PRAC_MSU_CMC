#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(void)
{
    int pid1 = fork();
    if (pid1 == 0) {
        int pid2 = fork();
        if (pid2 == 0) {
            printf("%d ", 3);
            exit(pid2);
        }
        wait(NULL);
        printf("%d ", 2);
        exit(pid1);

    }
    wait(NULL);
    printf("%d\n", 1);
    return 0;

}
