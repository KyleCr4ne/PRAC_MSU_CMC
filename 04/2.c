#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

enum
{
    SIZE = 8
};

int
main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Expected 2 arguments of command line\n");
        return 1;
    }
    errno = 0;
    char *end_ptr = NULL;
    int f = open(argv[1], O_RDWR);
    if (f == -1) {
        fprintf(stderr, "File can't be opened\n");
        return 1;
    }
    long N = strtol(argv[2], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[2] || (int) N != N) {
        fprintf(stderr, "program: error!\n");
        return 1;
    }
    double prev = 0.0;
    double cur = 0.0;
    off_t end = lseek(f, 0, SEEK_END);
    for(off_t cur_pos = lseek(f, 0, SEEK_SET); (cur_pos < end) && (cur_pos < N * SIZE); cur_pos += SIZE){
        if(read(f, &cur, SIZE) < SIZE){
            fprintf(stderr, "Error!\n");
            return 1;
        }
        cur -= prev;
        prev = cur;
        lseek(f, cur_pos, SEEK_SET);
        write(f, &cur, SIZE);
    }
    if (close(f) == -1) {
        fprintf(stderr, "Error!\n");
        return 0;

    }
}
