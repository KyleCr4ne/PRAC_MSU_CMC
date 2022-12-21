#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <limits.h>

enum
{
    SIZE_MN = 5,
    SIZE_MX = 10,
    LIMIT = 1 << 20,
    MODE = 0700
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
gen_path(char *path)
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
    return 0;
}


int
main(int argc, char *argv[])
{
    if (argc < 2) {
        return 0;
    }
    char path[PATH_MAX];
    gen_path(path);
    char script[LIMIT] ="#! /usr/bin/python3\nimport sys\nfrom sys import argv\n"
                        "from os import remove\nsys.set_int_max_str_digits(1000000)\nprint(";
    for (int i = 1; i < argc; i++) {
        strcat(script, argv[i]);
        strcat(script, (i == argc - 1) ? ")\n" : " * ");
    }
    int fd = open(path, O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC, MODE);
    if (fd < 0) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    FILE *file = fdopen(fd, "w");
    fprintf(file, "%s\n", script);
    fprintf(file, "%s\n", "remove(argv[0])\n");
    fclose(file);
    execlp(path, path, NULL);
    return 0;
}
