#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

typedef unsigned long long ull;

ull
mn(ull a, ull b)
{
    return (a < b) ? a : b;
}

int
main(int argc, char *argv[])
{
    char *end_ptr = NULL;
    errno = 0;
    ull N = strtoull(argv[1], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[1]) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }
    for (ull i = 0; i < mn(N, argc - 2); i++) {
        if (!fork()) {
            char *file_name = strdup(argv[i + 2]);
            char path[PATH_MAX];
            FILE *file = fopen(file_name, "r");
            fscanf(file, "%s", path);
            fclose(file);
            execlp(path, path, NULL);
            _exit(1);
        }
    }
    int stat;
    int cnt = 0;
    while (wait(&stat) > 0) {
        if (WIFEXITED(stat) && !WEXITSTATUS(stat)) cnt++;

    }
    for (ull i = N; i < argc - 2; i++) {
        if (!fork()) {
            char *file_name = strdup(argv[i + 2]);
            char path[PATH_MAX];
            FILE *file = fopen(file_name, "r");
            fscanf(file, "%s", path);
            fclose(file);
            execlp(path, path, NULL);
            _exit(1);
        }
        wait(&stat);
        if (WIFEXITED(stat) && !WEXITSTATUS(stat)) cnt++;

    }
    printf("%d\n", cnt);
    fflush(stdout);
    return 0;
}
