#include <stdio.h>
#include <stdlib.h>

enum
{
    BYTE = 8,
    OVERFLOW = -1
};

int64_t
my_pow(int64_t power)
{
    return 1LL << power;
}

int64_t
calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count)
{

    int64_t first, second, third, result, result2 = OVERFLOW;
    first = block_size / block_num_size;
    result = inode_direct_block_count;
    if (__builtin_smulll_overflow(first, first, &second)) {
        result = OVERFLOW;
    } else if (__builtin_smulll_overflow(second, first, &third)) {
        result = OVERFLOW;
    } else if (__builtin_saddll_overflow(result, first, &result)) {
        result = OVERFLOW;
    } else if (__builtin_saddll_overflow(result, second, &result)) {
        result = OVERFLOW;
    } else if (__builtin_saddll_overflow(result, third, &result)) {
        result = OVERFLOW;
    } else if (__builtin_smulll_overflow(result, block_size, &result)) {
        result = OVERFLOW;
    }
    if (block_num_size < BYTE) {
        result2 = my_pow(BYTE * block_num_size);
        if (__builtin_smulll_overflow(result2, block_size, &result2)) {
            result2 = OVERFLOW;
        }
    }
    if (result == OVERFLOW && result2 == OVERFLOW) {
        return OVERFLOW;
    } else if (result == OVERFLOW) {
        return result2;
    } else if (result2 == OVERFLOW) {
        return result;
    } else {
        return (result2 < result) ? result2 : result;
    }
}


int
main(void)
{
    int64_t bs, bns, idbc;
    scanf("%lld%lld%lld", &bs, &bns, &idbc);
    printf("%lld\n", calc_max_size(bs, bns, idbc));


    int i;
    int64_t cur, prev = -2;
    for (i = 1; i < 8; i++) {
        cur = calc_max_size(bs, i, idbc);
        if (cur <= prev || cur == -1) {
            if (cur != -1) {
                i--;
                cur = calc_max_size(bs, i, idbc);
            }
            break;
        }
        prev = cur;

    }



    printf("%d %lld\n", i, cur);
    return 0;
}
