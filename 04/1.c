#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Expected name of file\n");
        return 1;
    }
    int f = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (f == -1) {
        fprintf(stderr, "Error\n");
        return 1;
    }
    unsigned int input = 0;
    while (scanf("%u", &input) == 1) {
        unsigned int output = 0;
        unsigned int tmp = input & ((1 << 12) - 1);
        output <<= (1 << 3);
        output |= tmp & ((1 << 8) - 1);
        tmp >>= (1 << 3);
        output <<= (1 << 3);
        output |= tmp & ((1 << 8) - 1);
        input >>= 12;
        tmp = input & ((1 << 12) - 1);
        output <<= (1 << 3);
        output |= tmp & ((1 << 8) - 1);
        tmp >>= (1 << 3);
        output <<= (1 << 3);
        output |= tmp & ((1 << 8) - 1);
        if (write(f, &output, sizeof(output)) == -1) {
            fprintf(stderr, "Error\n");
            return 1;
        }
    }
    return 0;

}
