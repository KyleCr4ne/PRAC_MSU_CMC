#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <errno.h>


typedef struct Args
{
    char buf[64];
} Args;
enum
{
    SHIFT = 8
};

int
main(int argc, char *argv[])
{
    char *dyn_lib = strdup(argv[1]);
    char *func = strdup(argv[2]);
    char *signature = strdup(argv[3]);
    char return_type = signature[0];
    Args args;
    int j = 0;
    for (int i = 1; i < strlen(signature); i++) {
        char *end_ptr = NULL;
        switch (signature[i]) {
            case 'i': {
                errno = 0;
                long ival = strtol(argv[i + 3], &end_ptr, 10);
                if (errno || *end_ptr || end_ptr == argv[i + 3] || (int) ival != ival) {
                    fprintf(stderr, "strtol Error!\n");
                    return 1;
                }
                int tmp1 = (int) ival;
                for (int k = j; k < sizeof(int) + j; k++) {
                    args.buf[k] = (char) tmp1;
                    tmp1 >>= SHIFT;
                }
                j += sizeof(int);
                break;
            }
            case 'd': {
                errno = 0;
                double dval = strtod(argv[3 + i], &end_ptr);
                if (errno || *end_ptr || end_ptr == argv[3 + i]) {
                    fprintf(stderr, "strtod Error!\n");
                    return 1;
                }
                char *tmp2 = (char *) &dval;
                for (int k = j; k < sizeof(dval) + j; k++) {
                    args.buf[k] = *tmp2;
                    tmp2++;
                }
                j += sizeof(double);
                break;
            }
            case 's': {
                errno = 0;
                char *sval = strdup(argv[3 + i]);
                char *tmp3 = (char *) &sval;
                for (int k = j; k < sizeof(char *) + j; k++) {
                    args.buf[k] = *tmp3;
                    tmp3++;
                }
                j += sizeof(char *);
                break;
            }
            default:
                break;
        }
    }
    void *handle;
    handle = dlopen(dyn_lib, RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "dlopen Error!\n");
        return 1;
    }
    void *f;
    f = dlsym(handle, func);
    if (!f) {
        fprintf(stderr, "dlsym Error!\n");
        return 1;
    }
    switch (return_type) {
        case 'v': {
            ((__attribute__((stdcall)) void (*)(Args)) f)(args);
            break;
        }
        case 'i': {
            printf("%d\n", ((__attribute__((stdcall)) int (*)(Args)) f)(args));
            break;
        }
        case 'd' : {
            printf("%.10g\n", ((__attribute__((stdcall)) double (*)(Args)) f)(args));
            break;
        }
        case 's': {
            printf("%s\n", ((__attribute__((stdcall)) char *(*)(Args)) f)(args));
        }
        default:
            break;

    }
    return 0;
}
