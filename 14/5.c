#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <math.h>

enum
{
    MAX_CNT = 3
};
typedef unsigned long long ull;
unsigned long long prime_num;

void
handler_1(int s)
{
    static int cnt = 0;
    if (cnt < MAX_CNT) {
        printf("%llu\n", prime_num);
        fflush(stdout);
        cnt++;
    } else {
        _exit(0);
    }
}

void
handler_2(int s)
{
    _exit(0);
}

_Bool
is_prime(ull num)
{
    for (ull i = 2; i < (ull) sqrt((double) num) + 1; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    ull low, high;
    scanf("%llu%llu", &low, &high);
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler_1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler_2, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    prime_num = 0;
    for (ull i = low; i < high; i++) {
        if (is_prime(i)) {
            prime_num = i;
        }
    }
    printf("%d\n", -1);
    fflush(stdout);
    return 0;
}
