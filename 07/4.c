#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

enum
{
    CHAR_BIT = 8
};

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Need a file name!\n");
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "Open Error!\n");
        return 1;
    }
    struct stat stb;
    if (fstat(fd, &stb) != 0) {
        fprintf(stderr, "Error!\n");
        return 1;
    }
    off_t end = stb.st_size;

    int x;
    while ((scanf("%d", &x)) == 1) {
        if (x > 0) {
            if (x / 8 <= end) {
                lseek(fd, (x - 1) / CHAR_BIT, SEEK_SET);
                char tmp;
                read(fd, &tmp, sizeof(tmp));
                tmp |= 1 << ((x - 1) % CHAR_BIT);
                lseek(fd, (x - 1) / CHAR_BIT, SEEK_SET);
                write(fd, &tmp, sizeof(tmp));
            }
        } else if (x < 0) {
            if (-x / CHAR_BIT <= end) {
                lseek(fd, (-x - 1) / CHAR_BIT, SEEK_SET);
                char tmp;
                read(fd, &tmp, sizeof(tmp));
                tmp &= ~(1 << ((-x - 1) % CHAR_BIT));
                lseek(fd, (-x - 1) / CHAR_BIT, SEEK_SET);
                write(fd, &tmp, sizeof(tmp));
            }
        }
    }
    close(fd);
}
