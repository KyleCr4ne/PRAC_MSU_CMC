#include <stdio.h>

enum
{
    COUNT_NUMBERS = 10,
    COUNT_LETTERS = 26,
    NULL_LET = '0' - 1,
    LOW_LET = 'a' - COUNT_NUMBERS - 1,
    BIG_LET = 'A' - COUNT_NUMBERS - COUNT_LETTERS - 1
};

int
main(void)
{
    int x;
    while ((x = getchar()) != EOF) {
        if (('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z') || ('0' <= x && x <= '9')) {
            if ('0' <= x && x <= '9') {
                x -= NULL_LET;
            } else if ('a' <= x && x <= 'z') {
                x -= LOW_LET;
            } else
                x -= BIG_LET;
            x ^= (1 << 3);
            x &= ~(1 << 2);
            if (x == 0)
                putchar('@');
            else if (x == 63)
                putchar('#');
            else if (x >= 1 && x <= 10) {
                putchar(x + NULL_LET);
            } else if (x >= 11 && x <= 36) {
                putchar(x + LOW_LET);
            } else {
                putchar(x + BIG_LET);
            }
        }
    }
    return 0;
}
