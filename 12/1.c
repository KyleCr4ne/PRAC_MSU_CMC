#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>


enum
{
    MODE = 0660
};

int
main(int argc, char *argv[])
{
    pid_t pid = fork();
    int status;
    if (pid == 0) {
        int err_stream;
        if ((err_stream = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, MODE)) == -1) {
            exit(42);
        }
        dup2(err_stream, 2);

        int input_stream;
        if ((input_stream = open(argv[2], O_RDONLY)) == -1) {
            exit(42);
        }
        dup2(input_stream, 0);

        int output_stream;
        if ((output_stream = open(argv[3], O_WRONLY | O_APPEND | O_CREAT, MODE)) == -1) {
            exit(42);
        }
        dup2(output_stream, 1);
        if (close(err_stream) || close(input_stream) || close(output_stream)) {
            exit(42);
        }
        execlp(argv[1], argv[1], NULL);
        exit(42);
    }
    wait(&status);
    printf("%d\n", status);
    return 0;

}
