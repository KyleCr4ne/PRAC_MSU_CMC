#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

volatile int mode;

void
handler_plus(int s)
{
    mode = 0;
}

void
handler_mul(int s)
{
    mode = 1;
}

int
main(int argc, char *argv[])
{
    mode = 0;
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler_plus, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler_mul, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    unsigned int tmp;
    unsigned int accumulated_value = 0;
    while (scanf("%d", &tmp) == 1) {
        if (!mode) {
            accumulated_value += tmp;
        } else {
            accumulated_value *= tmp;
        }
        printf("%d\n", accumulated_value);
        fflush(stdout);
    }
    return 0;
}
