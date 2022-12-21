#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>

enum
{
    BUF_MAX = 8192,
    DEPTH = 4
};


void
tree_search(const char *path, off_t max_size, const char *relative_path, int depth)
{
    char tmp_path[BUF_MAX];
    strcpy(tmp_path, path);
    if (relative_path != NULL){
        strcat(tmp_path, "/");
        strcat(tmp_path, relative_path);
    }

    DIR *current_dir = opendir(tmp_path);
    if (current_dir == NULL) return;
    char current_path[BUF_MAX] = {};
    struct dirent *st_d;
    while ((st_d = readdir(current_dir)) != NULL){
        struct stat stb;
        strcpy(current_path, tmp_path);
        strcat(current_path, "/");
        strcat(current_path, st_d->d_name);
        if (lstat(current_path, &stb) != 0) continue;
        if (S_ISDIR(stb.st_mode)){
            if (strcmp(st_d->d_name, ".") == 0 || strcmp(st_d->d_name, "..") == 0)
                continue;
            if (depth != 1){
                if (relative_path == NULL) {
                    tree_search(path, max_size, st_d->d_name, depth - 1);
                }
                else{
                    char p[BUF_MAX] = {};
                    strcpy(p, relative_path);
                    strcat(p, "/");
                    strcat(p, st_d->d_name);
                    tree_search(path, max_size, p, depth - 1);
                }
            }
        } else if (S_ISREG(stb.st_mode) && access(current_path, R_OK) == 0 && stb.st_size <= max_size){
            if (relative_path == NULL)
                printf("%s\n", st_d->d_name);
            else
                printf("%s/%s\n", relative_path, st_d->d_name);
        }
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }
    char *end_ptr = NULL;
    errno = 0;
    off_t Z = strtol(argv[2], &end_ptr, 10);
    if (errno || *end_ptr || argv[2] == end_ptr || (int) Z != Z) {
        fprintf(stderr, "String to long  error!\n");
        return 1;
    }

    tree_search(argv[1], Z, NULL, DEPTH);

    return 0;
}
