#include <stdio.h>

enum
{
    MAX_INPUT = 9,
    MIN_INPUT = 1
};

int
find_max_index(int *perm, int length)
{
    for (int i = length - 2; i > -1; i--) {
        if (perm[i] < perm[i + 1])
            return i;
    }
    return -1;
}

int
find_big_element_index(int *perm, int length, int element)
{
    for (int i = length - 1; i > -1; i--) {
        if (perm[i] > element)
            return i;
    }
    return -1;
}

void
swap(int *perm, int i, int j)
{
    int t = perm[i];
    perm[i] = perm[j];
    perm[j] = t;
}

void
reverse_permutation(int *perm, int index, int length)
{
    for (int i = index + 1; i < index + 1 + (length - index - 1) / 2; i++) {
        int t = perm[i];
        perm[i] = perm[length - i + (index + 1) - 1];
        perm[length - i + (index + 1) - 1] = t;
    }
}

int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Incorrect input\n");
        return 1;
    }
    if (N > MAX_INPUT || N < MIN_INPUT) {
        fprintf(stderr, "The number doesn't fall in the specified interval\n");
        return 1;
    }
    int array[MAX_INPUT];
    for (int i = 0; i < N; i++) {
        array[i] = i + 1;
        printf("%d", array[i]);
    }
    printf("\n");
    int index = find_max_index(array, N);
    while (index != -1) {
        int element = array[index];
        int swap_index = find_big_element_index(array, N, element);
        swap(array, index, swap_index);
        reverse_permutation(array, index, N);
        for (int i = 0; i < N; i++)
            printf("%d", array[i]);
        printf("\n");
        index = find_max_index(array, N);
    }
    return 0;
}
