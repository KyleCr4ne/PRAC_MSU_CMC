#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
enum
{
    CNT = 5,
    ARG_CNT = 4
};

int main(int argc, char * argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected three arguments!\n");
        return 1;
    }
    char *c_size = strdup(argv[2]);
    char *b_size = strdup(argv[3]);
    long int cache_size, block_size;

    char *end_ptr = NULL;
    errno = 0;
    cache_size = strtol(c_size, &end_ptr, 10);
    if (errno || *end_ptr || c_size == end_ptr) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }

    end_ptr = NULL;
    errno = 0;
    block_size = strtol(b_size, &end_ptr, 10);
    if (errno || *end_ptr || b_size == end_ptr) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }

    char a, b;
    int addr, size, value, block_count = cache_size / block_size;
    long long cnt = 0;
    int *cache = calloc(cache_size, sizeof(*cache));
    while(scanf("%c %c %x %d %d\n", &a, &b, &addr, &size, &value) == CNT) {
        if (a == 'R') {
            int memory_block_num = addr / block_size + 1;
            int cache_block_num =  memory_block_num % block_count;
            if (!cache[cache_block_num]) {
                cache[cache_block_num] = memory_block_num;
            } else if (cache[cache_block_num] != memory_block_num) {
                cnt++;
                cache[cache_block_num] = memory_block_num;
            }
        }


    }
    printf("%lld\n", cnt);
    return 0;
}
