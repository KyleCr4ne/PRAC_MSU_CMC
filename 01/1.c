#include <stdio.h>

enum
{
    SEVEN = 7,
    TWO = 2,
    FIVE = 5,
    ONE = 1
};

int
main(void)
{
    double x, y;
    if (scanf("%lf%lf", &x, &y) != 2){
        fprintf(stderr, "Incorrect input\n");
        return 1;
        }
    printf("%d\n", (y <= SEVEN) && (x >= TWO) && (x <= FIVE) && (y >= ONE) && (y >= x - TWO));
    return 0;
}
