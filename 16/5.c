#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#include <time.h>
#include <limits.h>
#include <sys/mman.h>

enum
{
    SIZE_MN = 5,
    SIZE_MX = 10,
    LIMIT = 1 << 20,
    MODE = 0700,
    ARGC_CNT = 5
};

char *
gen_name(void)
{
    srand(time(NULL));
    ssize_t size = SIZE_MN + rand() % (SIZE_MX - SIZE_MN);
    char *rez = calloc(size, sizeof(*rez));
    for (int i = 0; i < size; i++) {
        rez[i] = (char) ('a' + rand() % 26);
    }
    return rez;
}

int
gen_path(char *path, char *path_res)
{
    char *path_tmp = NULL;
    if (getenv("XDG_RUNTIME_DIR") != NULL) {
        path_tmp = getenv("XDG_RUNTIME_DIR");
    } else if (getenv("TMPDIR") != NULL) {
        path_tmp = getenv("TMPDIR");
    } else {
        path_tmp = strdup("/tmp/");
    }
    char *name = strdup(gen_name());
    if (snprintf(path, PATH_MAX, "%s%s", path_tmp, name) < 0) return -1;
    if (snprintf(path_res, PATH_MAX, "%s%s.txt", path_tmp, name) < 0) return -1;
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc != ARGC_CNT) {
        fprintf(stderr, "Expected four arguments!\n");
        return 1;
    }
    double left, right;

    char *end_ptr = NULL;
    errno = 0;
    left = strtod(argv[1], &end_ptr);
    if (errno || *end_ptr) {
        fprintf(stderr, "strtod Error!\n");
        return 1;
    }
    end_ptr = NULL;
    errno = 0;
    right = strtod(argv[2], &end_ptr);
    if (errno || *end_ptr) {
        fprintf(stderr, "strtod Error!\n");
        return 1;
    }

    long long n;
    errno = 0;
    end_ptr = NULL;
    n = strtoll(argv[3], &end_ptr, 10);
    if (errno || *end_ptr) {
        fprintf(stderr, "strtoll Error!\n");
        return 1;
    }

    char *func = strdup(argv[4]);
    char path[PATH_MAX];
    char path_res[PATH_MAX];
    gen_path(path, path_res);
    double *s = calloc(n + 1, sizeof(double));
    double dx = (right - left) / n;
    for (long long i = 0; i < n; i++) {
        double cur = left + i * dx;
        int fd1 = open(path_res, O_CREAT | O_RDWR | O_TRUNC, MODE);
        int fd2 = open(path, O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC, MODE);
        char script[LIMIT] = "#! /usr/bin/python3\nfrom math import *\nx = ";
        FILE *file = fdopen(fd2, "w");
        fprintf(file, "%s%lf\nprint((%s)*%lf)", script, cur, func, dx);
        fclose(file);
        if (!fork()) {
            dup2(fd1, STDOUT_FILENO);
            close(fd1);
            execlp(path, path, NULL);
        } else {
            fd1 = open(path_res, O_CREAT | O_RDWR | O_TRUNC, MODE);
            FILE *f = fdopen(fd1, "r");
            wait(NULL);
            fscanf(f, "%lf", &s[i]);

        }
    }
    double result = 0;
    for (long long i = 0; i < n; i++) {
        result += s[i];
    }
    printf("%.10g", result);


    return 0;
}
