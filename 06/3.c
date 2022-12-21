#include <stdio.h>
#include <time.h>
#include <errno.h>

enum
{
    SHIFT = 1900
};

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
    value.tm_mday = 1;
    errno = 0;
    if (mktime(&value) == -1 && errno) {
        fprintf(stderr, "Error in mktime\n");
        return 1;
    }
    int month = value.tm_mon;
    int cnt = 0;

    while (value.tm_year == year - SHIFT) {
        if ((value.tm_wday == 4) && (value.tm_mday % 3) && (cnt % 2)) {
            printf("%d %d\n", value.tm_mon + 1, value.tm_mday);
        }
        if (value.tm_wday == 4) cnt++;
        value.tm_mday++;
        errno = 0;
        if (mktime(&value) == -1 && errno) {
            fprintf(stderr, "Error in mktime\n");
            return 1;
        }
        if (month != value.tm_mon) {
            month = value.tm_mon;
            cnt = 0;
        }

    }
    return 0;
}
