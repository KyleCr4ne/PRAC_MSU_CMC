#include <stdio.h>

_Bool True = 1;
const static char OVERFLOW_CASE[] = "18446744073709551616";

void
scan(char tmp)
{
    while ((tmp = getchar()) != ' ' && tmp != '\n' && tmp != EOF);
}

int
main(void)
{

    char tmp;
    long long int sum = 0;
    int flag = 0;
    while (True) {
        tmp = getchar();
        if (tmp == ' ' || tmp == '\n' || tmp == EOF) {
            if (flag) printf("%lld\n", sum);
            sum = 0;
            flag = 0;
            if (tmp == EOF) return 0;
            while ((tmp = getchar()) == ' ' || tmp == '\n');
        }
        long long int cur_sum = sum;
        if (tmp == 'a' || tmp == '1' || tmp == '0') {
            flag = 1;
            int temp = (tmp == 'a') ? (-1) : (tmp - '0');
            if (__builtin_add_overflow(sum, sum, &sum) || __builtin_add_overflow(sum, temp, &sum) ||
                __builtin_add_overflow(sum, cur_sum, &sum)) {
                puts(OVERFLOW_CASE);
                sum = 0;
                flag = 0;
                scan(tmp);
            }
        }
    }
}
