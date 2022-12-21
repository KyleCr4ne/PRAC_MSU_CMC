#include <stdio.h>
#include <stdlib.h>

int
abs(int a)
{
    return (a >= 0) ? a : -a;
}

int
comp(const int *a, const int *b)
{
    int l = *(const int *) a;
    int r = *(const int *) b;
    if (abs(l) % 2 < abs(r) % 2) return -1;
    if (abs(l) % 2 > abs(r) % 2) return 1;
    if (abs(l) % 2 == 0 && abs(r) % 2 == 0) return (r < l) - (r > l);
    if (abs(l) % 2 == 1 && abs(r) % 2 == 1) return (r > l) - (r < l);
    return 0;
}

void
sort_even_odd(size_t count, int *data)
{
    qsort(data, count, sizeof(data[0]), (int(*)(const void *, const void *))comp);
}
