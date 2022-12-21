#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double
currency_change(double current, double percent)
{
    return round((current + current * percent * 0.01) * 10000.0) / 10000.0;
}

int
main(int argc, char *argv[])
{
    double initial_value = strtod(argv[1], NULL);
    for (int i = 2; i < argc; ++i) {
        initial_value = currency_change(initial_value, strtod(argv[i], NULL));
    }
    printf("%.4lf\n", initial_value);
    return 0;
}
