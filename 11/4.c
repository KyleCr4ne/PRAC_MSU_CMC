#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Error! Need one num!\n");
        return 1;
    }
    pid_t pid;
    for (int i = 0; i < N; i++) {
        if ((pid = fork()) < 0) {
            fprintf(stderr, "Fork Error!\n");
            return 1;
        }
        if (pid == 0) {
            if (i != 0) printf(" ");
            printf("%d", i + 1);
            fflush(stdout);
        } else {
            wait(NULL);
            exit(0);
        }
    }
    wait(NULL);
    printf("\n");
    return 0;
}
