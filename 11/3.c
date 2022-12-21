#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/wait.h>

enum
{
    START = 1,
    END = 3,
    BUF_CNT = 8,
    NUM_SYS = 10
};

int
main(void)
{
    for (int i = START; i <= END; i++) {
        int pid;
        if ((pid = fork()) < 0) {
            fprintf(stderr, "Error in fork\n");
            return 1;
        } else if (!pid) {
            char s[BUF_CNT];
            read(0, s, BUF_CNT);
            char *end_ptr = NULL;
            errno = 0;
            long val = strtol(s, &end_ptr, NUM_SYS);
            if (errno || s == end_ptr || (int) val != val) {
                fprintf(stderr, "Error in strtol\n");
                return 1;
            }
            printf("%d %ld\n", i, val * val);
            return 0;
        }
    }
    for (int i = START; i <= END; i++) {
        wait(NULL);
    }
    return 0;

}
