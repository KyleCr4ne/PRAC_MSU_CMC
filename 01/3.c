#include <stdio.h>
#include <math.h>

enum { SIZE = 2000 };

int
isPrime(int x)
{
    if (x > 1) {
        for (int i = 2; i < (int) sqrt(x) + 1; i++)
            if (x % i == 0)
                return 0;
        return 1;
    }
    return 0;
}

int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Incorrect input\n");
        return 1;
    }
    if (!isPrime(N)) {
        fprintf(stderr, "Not a prime number\n");
        return 1;
    }
    for (int i = 0; i < N; i++) {
        int Inverse_elements_line[SIZE] = {};
        Inverse_elements_line[1] = i;
        printf("%d ", Inverse_elements_line[1]);
        for (int j = 2; j < N; j++) {
            Inverse_elements_line[j] = (N - (N / j) * Inverse_elements_line[N % j] % N) % N;
            printf("%d ", Inverse_elements_line[j]);
        }
        printf("\n");

    }
    return 0;
}
