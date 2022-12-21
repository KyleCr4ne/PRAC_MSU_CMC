#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

struct Node
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void
print(int f, struct Node tmp, unsigned long current)
{
    if (read(f, &tmp, sizeof(tmp)) < sizeof(tmp)) {
        fprintf(stderr, "Error\n");
        exit(1);
    }
    if (tmp.right_idx) {
        lseek(f, tmp.right_idx * sizeof(tmp), SEEK_SET);
        print(f, tmp, tmp.right_idx * sizeof(tmp));
        lseek(f, current, SEEK_SET);
    }
    printf("%d ", tmp.key);
    if(tmp.left_idx){
        lseek(f, tmp.left_idx * sizeof(tmp), SEEK_SET);
        print(f, tmp, tmp.left_idx * sizeof(tmp));
        lseek(f, current, SEEK_SET);
    }
}

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Expected file name\n");
        return 1;
    }
    int f = open(argv[1], O_RDONLY);
    if (f != -1) {
        struct Node tmp = {};
        print(f, tmp, 0);
        printf("\n");
        close(f);
    }
    return 0;
}
