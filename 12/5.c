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
gen_path(char *path_script, char *path_c_file, char *path_o_file)
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
    if (snprintf(path_script, PATH_MAX, "%s%s", path_tmp, name) < 0) return -1;
    if (snprintf(path_c_file, PATH_MAX, "%s%s.c", path_tmp, name) < 0) return -1;
    if (snprintf(path_o_file, PATH_MAX, "%s%s.o", path_tmp, name) < 0) return -1;
    return 0;
}


int
main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Enter the one-line arg-expression\n");
        return 1;
    }
    char path_script[PATH_MAX];
    char path_c_file[PATH_MAX];
    char path_o_file[PATH_MAX];
    gen_path(path_script, path_c_file, path_o_file);
    char script[LIMIT] = "#! /bin/bash\ngcc";

    int fd = open(path_script, O_CREAT | O_RDWR | O_TRUNC | O_CLOEXEC, MODE);
    if (fd < 0) {
        fprintf(stderr, "Can't open file\n");
        return 1;
    }
    FILE *file_script = fdopen(fd, "w");
    FILE *file_c = fopen(path_c_file, "w");
    fprintf(file_script, "%s %s -o %s\n%s\nrm %s*", script, path_c_file, path_o_file, path_o_file, path_script);
    const char c_code[] =   "#include <stdio.h>\n"
                            "int\n"
                            "main(void)\n"
                            "{\n"
                            "int x;\n"
                            "const char summon[] = \"summon\";\n"
                            "const char disqualify[] = \"disqualify\";\n"
                            "const char reject[] = \"reject\";\n";
    fprintf(file_c, "%s", c_code);
    printf("%s", path_script);
    int tmp;
    while (scanf("%d", &tmp) == 1) {
        fprintf(file_c, "x = %d;\nprintf(\"%%s\\n\", %s);\n", tmp, argv[1]);
    }
    fprintf(file_c, "%s", "return 0;\n}");
    fclose(file_script);
    fclose(file_c);
    execl(path_script, path_script, NULL);
    return 0;
}
