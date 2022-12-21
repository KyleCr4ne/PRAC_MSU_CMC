#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

enum
{
    PAGE = (1 << 9) - 1,
    SHIFT = 9
};

int
main(int argc, char *argv[])
{
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
        fprintf(stderr, "Open file Error!\n");
        return 1;
    }
    char *end_ptr = NULL;
    errno = 0;
    unsigned long table_start = strtoul(argv[2], &end_ptr, 16);
    if (errno || *end_ptr || end_ptr == argv[2]) {
        fprintf(stderr, "Converting str to ull Error!\n");
        return 1;
    }
    struct stat stb;
    if (fstat(fd, &stb)) {
        fprintf(stderr, "Stat Error!\n");
        return 1;
    }
    unsigned short *mem;
    if ((mem = mmap(0, stb.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
        fprintf(stderr, "Mmap Error!\n");
        return 1;
    }
    unsigned short *work_space = mem + table_start / sizeof(*work_space);
    unsigned long x;
    while (scanf("%lx", &x) == 1) {
        printf("%u\n", mem[(work_space[x >> SHIFT] | (x & PAGE)) / sizeof(*mem)]);
    }
    munmap(mem, stb.st_size);
    close(fd);
    return 0;
}
