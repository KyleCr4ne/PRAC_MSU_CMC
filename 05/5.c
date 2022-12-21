#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <libgen.h>

enum
{
    BUF_SIZE = 4096
};

int
copy_file(const char *srcpath, const char *dstpath)
{
    if (access(srcpath, R_OK)) return -1;
    int FIn;
    if ((FIn = open(srcpath, O_RDONLY)) == -1) return -1;
    struct stat buf_in, buf_out;
    if (!stat(srcpath, &buf_in) && S_ISREG(buf_in.st_mode)) {
        char *src, *dst;
        src = strdup(srcpath);
        dst = strdup(dstpath);
        if (src == NULL || dst == NULL)
            return -1;
        stat(dstpath, &buf_out);
        if (S_ISDIR(buf_out.st_mode)) {
            char *file_name = basename(src);
            strcat(dst, "/");
            strcat(dst, file_name);
        }
        stat(dst, &buf_out);
        if (buf_in.st_ino == buf_out.st_ino) {
            close(FIn);
            return 0;
        }
        int FOut;
        if ((FOut = open(dst, O_CREAT | O_TRUNC | O_WRONLY, buf_in.st_mode)) == -1) return -1;
        char str_cp[BUF_SIZE];
        ssize_t size1, size2;
        while ((size1 = read(FIn, str_cp, BUF_SIZE)) > 0) {
            char *tmp_buf = str_cp;
            while (size1 > 0) {
                if ((size2 = write(FOut, tmp_buf, size1)) == -1) {
                    return -1;
                }
                size1 -= size2;
                tmp_buf += size2;
            }
        }
        free(src);
        free(dst);
        if (close(FIn) == -1 || close(FOut) == -1) {
            return -1;
        }
        return 0;

    } else {
        return -1;
    }
}
