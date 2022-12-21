#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    const char rwx[9] = "rwxrwxrwx";
    char *end_ptr = NULL;
    errno = 0;
    for (int i = 1; i < argc; ++i) {
        long lval = strtol(argv[i], &end_ptr, 8);
        if (errno || *end_ptr || end_ptr == argv[i] || (int) lval != lval) {
            fprintf(stderr, "Conversion error!");
            return 1;
        }
        for (int j = 8; j >= 0; --j) {
            printf("%c", (((1 << j) & lval) == 0) * '-' + (((1 << j) & lval) != 0) * rwx[8 - j]);
        }
        printf("\n");
    }
    return 0;
}
