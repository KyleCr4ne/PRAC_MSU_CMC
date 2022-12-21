#include <stdio.h>

enum
{
    MAX_N = 10000,
    MIN_N = 1,
    MAX_M = 50000,
    MIN_M = 1
};

void
add_from_L_to_R(int *array, int L, int R, int S)
{
    for (int i = L; i < R; i++) {
        array[i] += S;
    }
}

int
sum_from_L_to_R(int *array, int L, int R)
{
    int sm = 0;
    for (int i = L; i < R; i++) {
        sm += array[i];
    }
    return sm;
}

int
main(void)
{
    int N, M;
    int TYPE, L, R, S;
    if (scanf("%d%d", &N, &M) != 2) {
        fprintf(stderr, "Incorrect input\n");
        return 1;
    }
    if (!(N <= MAX_N && N >= MIN_N && M <= MAX_M && M >= MIN_M)) {
        fprintf(stderr, "Wrong intervals to M or N\n");
        return 1;
    }
    int columns[MAX_N] = {0};
    for (int i = 0; i < M; i++) {
        if (scanf("%d", &TYPE) != 1) {
            fprintf(stderr, "Expected number - type\n");
            return 1;
        }
        if (TYPE == 1) {
            if (scanf("%d%d%d", &L, &R, &S) != 3) {
                fprintf(stderr, "Expected three numbers\n");
                return 1;
            }
            add_from_L_to_R(columns, L, R, S);
        } else {
            if (scanf("%d%d", &L, &R) != 2) {
                fprintf(stderr, "Expected two numbers\n");
                return 1;
            }
            printf("%d\n", sum_from_L_to_R(columns, L, R));
        }
    }
    return 0;
}
