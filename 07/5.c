#include <stdio.h>
#include <time.h>
#include <errno.h>

enum
{
    LAG = 1900,
    PART = 256,
    SHIFT = 29 * 24 * 60 * 60 + 12 * 60 * 60 + 44 * 60,
    DAY_FULL_MOON = 26,
    MONTH_FULL_MOON = 5,
    YEAR_FULL_MOON = 2021,
    HOUR_FULL_MOON = 11,
    MINUTE_FULL_MOON = 14
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
    value.tm_year = year - LAG;
    value.tm_mday = PART + 1;
    value.tm_isdst = -1;
    errno = 0;
    time_t cur = timegm(&value);
    if (cur == -1 && errno) {
        fprintf(stderr, "timegm Error!\n");
        return 1;
    }
    struct tm base_day_of_full_moon = {};
    base_day_of_full_moon.tm_year = YEAR_FULL_MOON - LAG;
    base_day_of_full_moon.tm_mday = DAY_FULL_MOON;
    base_day_of_full_moon.tm_mon = MONTH_FULL_MOON - 1;
    base_day_of_full_moon.tm_hour = HOUR_FULL_MOON - 1;
    base_day_of_full_moon.tm_min = MINUTE_FULL_MOON - 1;
    errno = 0;
    time_t base = timegm(&base_day_of_full_moon);
    if (base == -1 && errno) {
        fprintf(stderr, "timegm Error!\n");
        return 1;
    }
    time_t tmp = (cur % SHIFT - base % SHIFT) % SHIFT;
    if (tmp >= 0) {
        value.tm_sec += SHIFT - tmp;
    } else {
        value.tm_sec -= tmp;
    }
    value.tm_mday += 1;
    errno = 0;
    if (timegm(&value) == -1 && errno) {
        fprintf(stderr, "timegm Error!\n");
        return 1;
    }
    while (value.tm_wday != 1) {
        value.tm_mday += 1;
        errno = 0;
        if (timegm(&value) == -1 && errno) {
            fprintf(stderr, "timegm Error!\n");
            return 1;
        }
    }
    value.tm_mday += 21;
    errno = 0;
    if (timegm(&value) == -1 && errno) {
        fprintf(stderr, "timegm Error!\n");
        return 1;
    }
    printf("%04d-%02d-%02d\n", value.tm_year + LAG, value.tm_mon + 1, value.tm_mday);


    return 0;
}
