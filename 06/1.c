#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

enum
{
    BUF_SIZE = 4096
};

int
is_exe(char *str)
{
    unsigned len = strlen(str);
    if (len >= 4 && str[len - 1] == 'e' && str[len - 2] == 'x' && str[len - 3] == 'e' && str[len - 4] == '.')
        return 1;
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }
    DIR *dir = opendir(argv[1]);
    if (!dir) {
        fprintf(stderr, "Directory can't be opened\n");
        return 1;
    }
    unsigned str_len = strlen(argv[1]);
    char str[BUF_SIZE];
    for(int i = 0; i < str_len; i++){
        str[i] = argv[1][i];
    }
    str[str_len] = '/';
    str_len += 1;
    struct dirent *buf_dir;
    int cnt = 0;
    while ((buf_dir = readdir(dir)) != NULL) {
        for(int i = 0; i <= strlen(buf_dir->d_name); i++){
            str[str_len + i] = buf_dir->d_name[i];
        }
        struct stat buf;
        if (stat(str, &buf) >= 0 && S_ISREG(buf.st_mode) && access(str, 1) == 0 && is_exe(buf_dir->d_name))
            cnt++;
    }

    printf("%d\n", cnt);
    closedir(dir);

    return 0;
}
