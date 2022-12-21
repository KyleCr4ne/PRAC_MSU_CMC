#include <stdio.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Expected two arguments!\n");
        return 1;
    }
    DIR *dir1 = opendir(argv[1]);
    struct dirent *st_dir1;
    char buf1[PATH_MAX] = {};
    char buf2[PATH_MAX] = {};
    off_t size = 0;
    while ((st_dir1 = readdir(dir1)) != NULL) {
        strcpy(buf1, argv[1]);
        strcat(buf1, "/");
        strcat(buf1, st_dir1->d_name);
        struct stat st1;
        if (lstat(buf1, &st1) != 0) {
            continue;
        }
        if (S_ISREG(st1.st_mode) && access(buf1, W_OK) == 0) {
            DIR *dir2 = opendir(argv[2]);
            struct dirent *st_dir2;
            while ((st_dir2 = readdir(dir2)) != NULL) {
                strcpy(buf2, argv[2]);
                strcat(buf2, "/");
                strcat(buf2, st_dir2->d_name);
                struct stat st2;
                if (stat(buf2, &st2) != 0) {
                    continue;
                }
                if (strcmp(st_dir1->d_name, st_dir2->d_name) == 0 && st1.st_ino == st2.st_ino &&
                    st1.st_dev == st2.st_dev) {
                    size += st1.st_size;
                    break;
                }

            }
            closedir(dir2);

        }

    }
    closedir(dir1);
    printf("%llu\n", size);
    return 0;
}
