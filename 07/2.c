#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    long long int ans = 0;
    for (int i = 1; i < argc; i++) {
        int sign;
        int k = 1;
        char *tmp = strdup(argv[i]);
        if (tmp[strlen(tmp) - 1] == '+') {
            sign = 1;
            tmp[strlen(tmp) - 1] = '\0';
        } else if (tmp[strlen(tmp) - 1] == '-') {
            sign = -1;
            tmp[strlen(tmp) - 1] = '\0';
        } else {
            continue;
        }
        if (tmp[strlen(tmp) - 1] == 'k') {
            k = 1000;
            tmp[strlen(tmp) - 1] = '\0';
        }
        errno = 0;
        char *end_ptr = NULL;
        long lval = strtol(tmp, &end_ptr, 10);
        if (errno || *end_ptr != 0 || end_ptr == argv[i] || (int) lval != lval) {
            continue;
        }
        int res;
        if (__builtin_mul_overflow(lval, k, &res)) {
            continue;
        }
        if (sign == 1) {
            ans += res;
        } else {
            ans -= res;
        }

    }
    printf("%lld\n", ans);
}
