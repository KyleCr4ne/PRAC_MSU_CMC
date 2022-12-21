#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>

_Bool Flag;

ssize_t
getcwd2(int fd, char *buf, size_t size)
{
    if (fd < 0) return -1;
    char path[PATH_MAX] = {};
    int fd_base;
    if ((fd_base = open("./", O_RDONLY)) < 0) return -1;
    if (fchdir(fd) == -1) goto error_end;
    DIR *parent;
    ssize_t length;
    struct stat root_st, cur_st;
    if (lstat("/", &root_st) != 0 || lstat("./", &cur_st) != 0) goto error_end;
    while (!(cur_st.st_dev == root_st.st_dev && cur_st.st_ino == root_st.st_ino)) {
        parent = opendir("./../");
        if (parent == NULL) goto error_end;
        struct dirent *parent_dr;
        Flag = 0;
        while ((parent_dr = readdir(parent)) != NULL) {
            if (strcmp(parent_dr->d_name, ".") == 0 || strcmp(parent_dr->d_name, "..") == 0)
                continue;
            char *tmp = calloc(PATH_MAX, sizeof(*tmp));
            strcpy(tmp, "./../");
            strcat(tmp, parent_dr->d_name);
            struct stat parent_cur_st;
            if (lstat(tmp, &parent_cur_st) != 0) continue;
            free(tmp);
            if (S_ISDIR(parent_cur_st.st_mode) && parent_cur_st.st_ino == cur_st.st_ino &&
                parent_cur_st.st_dev == cur_st.st_dev) {
                Flag = 1;
                char *name = strdup(path);
                strcpy(path, "/");
                strcat(path, parent_dr->d_name);
                strcat(path, name);
                path[strlen(path)] = '\0';
                free(name);
                break;
            }

        }
        if (chdir("./../") == -1 || !Flag || lstat("./", &cur_st)) goto error_end;
        closedir(parent);
    }
    fchdir(fd_base);
    length = strlen(path);
    if (!length) {
        path[0] = '/';
        length++;
    }
    if (size >= 1) {
        strncpy(buf, path, size - 1);
        buf[size - 1] = '\0';
    }
    return length;
    error_end:
    fchdir(fd_base);
    return -1;
}

/*int
main(void)
{
    int d = open("/usr/lib/pam/", O_RDONLY);
    char *buf = calloc(100, sizeof(char));
    ssize_t t = getcwd2(d, buf, 100);
    printf("%s\n", buf);
    printf("%ld", t);
    return 0;
}*/
