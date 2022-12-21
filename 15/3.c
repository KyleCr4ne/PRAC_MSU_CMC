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


enum
{
    ARG_CNT = 2,
    CHMOD = 0600,
    KEY = 11,
    TRUE = 1
};

int
absolute(int a)
{
    return (a >= 0) ? a : -a;
}

int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected one command-line argument!\n");
        return 1;
    }
    setbuf(stdin, NULL);
    errno = 0;
    char *end_ptr = NULL;
    int count;
    count = strtol(argv[1], &end_ptr, 10);
    if (errno || *end_ptr || argv[1] == end_ptr || (int) count != count) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }
    unsigned short *array = calloc(count, sizeof(*array));
    int semid = semget(KEY, count, IPC_CREAT | CHMOD);
    semctl(semid, 0, SETALL, array);
    free(array);
    pid_t *pid = calloc(count, sizeof(*pid));
    for (int i = 0; i < count; i++) {
        if (!(pid[i] = fork())) {
            struct sembuf Up = { .sem_op = 1, .sem_flg = 0, .sem_num = i };
            struct sembuf Down = { .sem_op = -1, .sem_flg = 0, .sem_num = i };
            int cur;
            while (TRUE) {
                if (semop(semid, &Down, 1) < 0) {
                    exit(0);
                }
                if (scanf("%d", &cur) != 1) {
                    exit(0);
                }
                printf("%d %d\n", i, cur);
                fflush(stdout);
                Up.sem_num = (cur >= 0) ? cur % count : (count + cur % count) % count;
                semop(semid, &Up, 1);
            }
        } else if (pid[i] < 0) {
            semctl(semid, 0, IPC_RMID, 0);
            while (wait(NULL) > 0) {};
            exit(1);
        }
    }
    semop(semid, &(struct sembuf) { .sem_op = 1, .sem_flg = 0, .sem_num = 0 }, 1);
    wait(NULL);
    semctl(semid, 0, IPC_RMID, 0);
    while (wait(NULL) > 0) {};
    exit(0);
}
