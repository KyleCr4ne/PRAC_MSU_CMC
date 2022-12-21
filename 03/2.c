#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char *argv[])
{
    long long int SUM_NEG = 0;
    long long int SUM_POS = 0;

    for (int i = 1; i < argc; ++i) {
        if (strtoll(argv[i], NULL, 10) > 0) {
            SUM_POS += strtoll(argv[i], NULL, 10);
        } else if (strtoll(argv[i], NULL, 10) < 0) {
            SUM_NEG += strtoll(argv[i], NULL, 10);
        }
    }
    printf("%lld\n%lld\n", SUM_POS, SUM_NEG);
    return 0;
}
