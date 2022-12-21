#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int
cmp(const void *a, const void *b)
{
    return *(unsigned long long int *) a <= *(unsigned long long int *) b;
}

int
main(int argc, char *argv[])
{
    unsigned long long int *arr = calloc(argc - 1, sizeof(*arr));
    size_t counter = 0;
    for (size_t i = 1; i < argc; i++) {
        char *end_ptr = NULL;
        unsigned long long lval = strtoull(argv[i], &end_ptr, 17);
        if (errno || *end_ptr || end_ptr == argv[i]) {
            continue;
        }
        arr[counter++] = lval;
    }
    qsort(arr, counter, sizeof(arr[0]), cmp);
    for (size_t i = 0; i < counter; i++) {
        printf("%llu\n", arr[i]);
    }
    return 0;
}
