#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

_Bool True = 1;

int
main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "Tow few elements - Error!\n");
        return 1;
    }
    int fdout;
    if ((fdout = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600)) < 0) {
        fprintf(stderr, "Open file Error!\n");
        return 1;
    }
    errno = 0;
    char *end_ptr = NULL;
    long rows, cols;
    rows = strtol(argv[2], &end_ptr, 10);
    cols = strtol(argv[3], &end_ptr, 10);
    if (errno || *end_ptr || rows < 1 || cols < 1) {
        fprintf(stderr, "Strtol Error!\n");
        return 1;
    }
    int **array = (int **) calloc(rows, sizeof(int *));
    for (int i = 0; i < rows; i++) {
        array[i] = (int *) calloc(cols, sizeof(int));
    }
    int cur_elem = 1;
    int cur_itt = 0;
    if (rows == 1 || cols == 1) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                array[i][j] = cur_elem;
                cur_elem++;
            }
        }
        goto end;
    }
    while (True) {
        for (int j = cur_itt; j < cols - cur_itt - 1; j++) {
            array[cur_itt][j] = cur_elem;
            cur_elem++;
        }
        if (cur_elem >= rows * cols)
            break;
        for (int i = cur_itt; i < rows - cur_itt - 1; i++) {
            array[i][cols - cur_itt - 1] = cur_elem;
            cur_elem++;


        }
        if (cur_elem >= rows * cols)
            break;
        for (int j = cols - 1 - cur_itt; j > cur_itt; j--) {
            array[rows - cur_itt - 1][j] = cur_elem;
            cur_elem++;

        }
        if (cur_elem >= rows * cols)
            break;
        for (int i = rows - cur_itt - 1; i > cur_itt; i--) {
            array[i][cur_itt] = cur_elem;
            cur_elem++;

        }
        cur_itt++;
        if (cur_elem >= rows * cols)
            break;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (array[i][j] == 0) {
                array[i][j] = rows * cols;
                break;
            }
        }
    }
    end:
    lseek(fdout, (rows * cols - 1) * sizeof(int), SEEK_SET);
    int a = 0;
    write(fdout, &a, sizeof(a));
    lseek(fdout, 0, SEEK_SET);
    int *res = (int *) mmap(0, (rows * cols) * sizeof(a), PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            res[i * cols + j] = array[i][j];
        }
    }
    munmap(res, rows * cols * sizeof(a));
    close(fdout);
    return 0;
}
