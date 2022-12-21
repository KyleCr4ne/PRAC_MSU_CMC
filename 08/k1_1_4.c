#include <stdio.h>
#include <time.h>
#include <errno.h>

enum
{
    SHIFT = 1900,
    COUNT = 9
};
const static int DAYS[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };

int
check(const int *arr, int day)
{
    for (int i = 0; i < COUNT; i++) {
        if (arr[i] == day)
            return 1;
    }
    return 0;
}

int
main(void)
{
    int year;
    if (scanf("%d", &year) != 1) {
        fprintf(stderr, "Need a one argument - year\n");
        return 1;
    }
    struct tm value = {};
    value.tm_year = year - SHIFT;
    value.tm_yday = 0;
    value.tm_mday = 1;
    value.tm_isdst = -1;
    errno = 0;
    mktime(&value);
    if (errno) {
        fprintf(stderr, "Error in mktime\n");
        return 1;
    }
    int cnt = 0;
    while (value.tm_year == year - SHIFT) {
        if (value.tm_wday == 0 || value.tm_wday == 6 || check(DAYS, value.tm_yday + 1)) {
            cnt++;
        }
        value.tm_mday++;
        errno = 0;
        mktime(&value);
        if (errno) {
            fprintf(stderr, "Error in mktime\n");
            return 1;
        }

    }
    printf("%d\n", cnt);
    return 0;
}
