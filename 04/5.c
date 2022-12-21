#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char * argv[]){
    if(argc < 4){
        fprintf(stderr, "Too few arguments\n");
        return 1;
    }
    int f1 = open(argv[1], O_RDONLY);
    if (f1 == -1){
        fprintf(stderr, "File input cannot be opened\n");
        return 1;
    }
    int f2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0600);
    if (f2 == -1){
        fprintf(stderr, "File output cannot be opened or created\n");
        return 1;
    }
    char *end_ptr = NULL;
    errno = 0;
    long mod = strtol(argv[3], &end_ptr, 10);
    if (errno || *end_ptr || (int) mod != mod || argv[3] == end_ptr){
        fprintf(stderr, "Error!\n");
        return 1;
    }

    int MOD = (int) mod;
    if (!MOD){
        fprintf(stderr, "MOD can't be zero-value\n");
        return 1;
    }
    unsigned long long cur_el = 0, cur_sum = 0;
    off_t end_f1 = lseek(f1, 0, SEEK_END);
    lseek(f2, 0, SEEK_SET);
    for(off_t cur_f1 = lseek(f1, 0, SEEK_SET); cur_f1 < end_f1; cur_f1 += sizeof(unsigned char)){
        unsigned char tmp;
        if (read(f1, &tmp, sizeof(tmp)) < sizeof(tmp)){
            fprintf(stderr, "Error\n");
            return 1;
        }
        for (int i = 0; i < CHAR_BIT; i++){
            cur_el = (cur_el + 1) % MOD;
            cur_sum = (cur_sum + cur_el * cur_el % MOD) % MOD;
            if ((tmp >> i) & 1) {
                unsigned res = cur_sum;
                if (write(f2, &res, sizeof(res)) < sizeof(res)){
                    fprintf(stderr, "Error\n");
                    return 1;
                }
            }
        }
    }
    return 0;
}
