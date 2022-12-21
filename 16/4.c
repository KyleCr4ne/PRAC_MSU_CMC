#include <stdio.h>
#include <stdlib.h>

unsigned int
max_pow_two(unsigned int num)
{
    unsigned int x = 1;
    unsigned int ans = 1;
    while (x <= num) {
        if (num % x == 0) {
            ans = x;
        }
        x *= 2;
    }
    return ans;
}

int
main(void)
{
    unsigned int tmp, addr = 0;
    unsigned int mx_align = 0;
    while (scanf("%u", &tmp) == 1) {
        if (!tmp) {
            continue;
        } else {
            unsigned int align = max_pow_two(tmp);
            if (align > mx_align) {
                mx_align = align;
            }
            addr += tmp;
            if (addr % align) addr = (addr + align - 1) / align * align;
        }
    }
    if (!addr) {
        printf("1 1\n");
        return 0;
    }
    printf("%u %u\n", (addr + mx_align - 1) / mx_align * mx_align, mx_align);
    return 0;
}
