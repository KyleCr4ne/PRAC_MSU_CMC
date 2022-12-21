#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    int cnt = 0;
    for (int i = 1; i < argc; i++) {
        char *pr = strdup(argv[i]);
        char *p = calloc(strlen(pr) - 1, sizeof(char));
        for (int j = 1; j < strlen(pr); j++) {
            p[j - 1] = pr[j];
        }
        if (pr[0] == 'p') {
            pid_t pid;
            if ((pid = fork()) < 0) {
                fprintf(stderr, "fork Error!\n");
                exit(1);
            } else if (!pid) {
                execlp(p, p, NULL);
                exit(1);
            }
        } else if (pr[0] == 's') {
            int status;
            while (wait(&status) > 0) {
                if (WIFEXITED(status) && !WEXITSTATUS(status)) cnt++;
            }
            pid_t pid;
            if ((pid = fork()) < 0) {
                fprintf(stderr, "fork Error!\n");
                exit(1);
            } else if (!pid) {
                execlp(p, p, NULL);
                exit(1);
            }
        }
    }
    int status;
    while (wait(&status) > 0) {
        if (WIFEXITED(status) && !WEXITSTATUS(status)) cnt++;
    }
    printf("%d\n", cnt);
    return 0;
}
