#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

enum
{
    LAST_CALL = 5
};

void
handler(int s)
{
    static int cnt = 0;
    if (cnt < LAST_CALL) {
        printf("%d\n", cnt);
        fflush(stdout);
        cnt++;
    } else {
        _exit(0);
    }
}

_Bool True = 1;

int
main(int argc, char *argv[])
{
    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (True) {
        pause();
    }
}
