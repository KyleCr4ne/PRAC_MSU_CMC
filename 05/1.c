#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

enum
{
    KIB = 1 << 10
};

int
main(int argc, char *argv[])
{
    unsigned long long int s = 0;
    for (int i = 1; i < argc; ++i) {
        struct stat stb;
        if (lstat(argv[i], &stb) >= 0 && S_ISREG(stb.st_mode) && !(stb.st_size % KIB) && stb.st_nlink == 1) {
            s += stb.st_size;
        }
    }
    printf("%lld\n", s);

    return 0;
}
