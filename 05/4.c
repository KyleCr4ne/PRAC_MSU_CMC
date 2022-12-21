#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>


int
cmp(const void *path1, const void *path2)
{
    char *a = *(char **) path1;
    char *b = *(char **) path2;
    return strcmp(a, b);
}

int
main(int argc, char *argv[])
{
    int ans_count = 0;
    char **File_paths = calloc(argc - 1, sizeof(*File_paths));
    for (int i = 1; i < argc; ++i) {
        struct stat st1, st2;
        if (!stat(argv[i], &st1)) {
            int j = 0;
            for (; j < ans_count; ++j) {
                stat(File_paths[j], &st2);
                if (st1.st_ino == st2.st_ino) {
                    if (strcmp(argv[i], File_paths[j]) > 0) {
                        File_paths[j] = argv[i];
                    }
                    break;
                }

            }
            if (j == ans_count) {
                File_paths[ans_count] = argv[i];
                ans_count++;
            }
        }
    }
    qsort(File_paths, ans_count, sizeof(*File_paths), cmp);
    for (int i = 0; i < ans_count; ++i) {
        puts(File_paths[i]);
    }
}
