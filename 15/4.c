#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>

typedef struct
{
    long mtype;
    long long x1;
    long long x2;
} Message;
enum
{
    ARG_CNT = 6,
    CHMOD = 0600,
    TRUE = 1
};

long long
absolute(long long a)
{
    return (a >= 0) ? a : -a;
}

void
error_strtoll_msg()
{
    fprintf(stderr, "Strtoll Error!\n");
    exit(1);
}

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected five arguments!\n");
        return 1;
    }
    key_t key;
    long long n, value1, value2, maxval;
    errno = 0;
    char *end_ptr = NULL;
    key = strtol(argv[1], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[1] || (int) key != key) {
        error_strtoll_msg();
    }
    errno = 0;
    end_ptr = NULL;
    n = strtoll(argv[2], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[2]) {
        error_strtoll_msg();
    }
    errno = 0;
    end_ptr = NULL;
    value1 = strtoll(argv[3], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[3]) {
        error_strtoll_msg();
    }
    errno = 0;
    end_ptr = NULL;
    value2 = strtoll(argv[4], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[4]) {
        error_strtoll_msg();
    }
    errno = 0;
    end_ptr = NULL;
    maxval = strtoll(argv[5], &end_ptr, 10);
    if (errno || *end_ptr || end_ptr == argv[5]) {
        error_strtoll_msg();
    }
    setbuf(stdin, NULL);
    int msgid = msgget(key, CHMOD | IPC_CREAT);
    if (msgid < 0) exit(1);

    pid_t *pid = calloc(n + 1, sizeof(*pid));

    Message message = { .mtype = 1, .x1 = value1, .x2 = value2 };
    size_t tmp = sizeof(message) - sizeof(message.mtype);
    for (int i = 1; i < n + 1; i++) {
        if (!(pid[i] = fork())) {
            long long x3;
            while (TRUE) {

                if (msgrcv(msgid, &message, tmp, i, 0) < 0) {
                    exit(0);
                }
                x3 = message.x1 + message.x2;
                printf("%d %lld\n", i - 1, x3);
                fflush(stdout);
                if (absolute(x3) > maxval) {
                    exit(0);
                }
                message.x1 = message.x2;
                message.x2 = x3;
                message.mtype = x3 % n + 1;
                msgsnd(msgid, &message, tmp, 0);
            }
        } else if (pid[i] < 0) {
            msgctl(msgid, IPC_RMID, NULL);
            while (wait(NULL) > 0) {};
            exit(1);
        }
    }
    msgsnd(msgid, &message, tmp, 0);
    wait(NULL);
    msgctl(msgid, IPC_RMID, NULL);
    while (wait(NULL) > 0) {};
    exit(0);
}
