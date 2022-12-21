#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

int
func(unsigned int num, unsigned int *fat)
{

    int cnt = 0;
    while (num != 0) {
        num = fat[num];
        cnt++;
    }
    return cnt - 1;
}

int
main(void)
{
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Need a count!\n");
        return 1;
    }
    unsigned int *arr = calloc(N, sizeof(*arr));
    arr[0] = 0;
    arr[1] = 0;
    arr[N - 1] = 0;
    arr[N - 2] = 0;
    for (int i = 2; i < N - 2; i++) {
        scanf("%u", &arr[i]);
    }
    unsigned int tmp;
    while (scanf("%u", &tmp) == 1) {
        printf("%d\n", func(tmp, arr));
    }
    return 0;
}
