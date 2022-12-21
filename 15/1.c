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


enum
{
    ARG_CNT = 4,
    CHMOD = 0600,
    TRUE = 1,
    CNT_BYTE = 8
};


int
main(int argc, char *argv[])
{
    if (argc != ARG_CNT) {
        fprintf(stderr, "Expected three arguments!\n");
        return 1;
    }
    setbuf(stdin, NULL);
    errno = 0;
    char *end_ptr = NULL;
    int count;
    key_t key;
    int maxval;
    count = strtol(argv[1], &end_ptr, 10);
    key = strtol(argv[2], &end_ptr, 10);
    maxval = strtol(argv[3], &end_ptr, 10);
    if (errno || *end_ptr) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }
    count++;
    unsigned short *array = calloc(count, sizeof(*array));
    int semid = semget(key, count, IPC_CREAT | CHMOD);
    int shmid = shmget(key, CNT_BYTE, IPC_CREAT | CHMOD);
    int *shmaddr;
    shmaddr = shmat(shmid, NULL, 0);
    shmaddr[0] = 0;
    shmaddr[1] = 0;
    semctl(semid, 0, SETALL, array);
    free(array);
    pid_t *pid = calloc(count, sizeof(*pid));
    for (int i = 1; i < count; i++) {
        if (!(pid[i] = fork())) {
            struct sembuf Up = { .sem_op = 1, .sem_flg = 0, .sem_num = i };
            struct sembuf Down = { .sem_op = -1, .sem_flg = 0, .sem_num = i };
            int cur_value, prev_pid;
            while (TRUE) {
                if (semop(semid, &Down, 1) < 0) {
                    exit(0);
                }
                cur_value = shmaddr[0];
                prev_pid = shmaddr[1];

                printf("%d %d %d\n", i, cur_value, prev_pid);
                fflush(stdout);

                shmaddr[0]++;
                shmaddr[1] = i;

                if (shmaddr[0] > maxval) {
                    shmdt(shmaddr);
                    exit(0);
                }

                int tmp = (shmaddr[0] % (count - 1)) * (shmaddr[0] % (count - 1));
                Up.sem_num = ((tmp % (count - 1)) * (tmp % (count - 1))) % (count - 1) + 1;
                semop(semid, &Up, 1);
            }
        } else if (pid[i] < 0) {
            semctl(semid, 0, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, NULL);
            while (wait(NULL) > 0) {};
            exit(1);
        }
    }
    semop(semid, &(struct sembuf) { .sem_op = 1, .sem_flg = 0, .sem_num = 1 }, 1);
    wait(NULL);
    semctl(semid, 0, IPC_RMID, 0);
    shmctl(shmid, IPC_RMID, NULL);
    while (wait(NULL) > 0) {};
    exit(0);
}
