#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Expected file name\n");
        return 1;
    }
    int f = open(argv[1], O_RDWR);
    if (f == -1) {
        fprintf(stderr, "Error!\n");
        return 1;
    }
    long long int end = lseek(f, 0, SEEK_END);
    lseek(f, 0, SEEK_SET);
    long long tmp, res;
    if (read(f, &res, sizeof(res)) == sizeof(res)) {
        off_t buf = 0;
        for (off_t cur = lseek(f, sizeof(res), SEEK_SET); cur < end; cur += sizeof(res)) {
            if (read(f, &tmp, sizeof(res)) == sizeof(res)) {
                if (tmp < res) {
                    res = tmp;
                    buf = cur;
                }
            }
        }
        lseek(f, buf, SEEK_SET);
        if (res != 0 && res != LLONG_MIN) {
            res = ~res + 1;
        }
        write(f, &res, sizeof(res));
    }

    close(f);
    return 0;
}
