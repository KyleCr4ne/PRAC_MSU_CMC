#include <stdio.h>
#include <string.h>


int
parse_rwx_permissions(const char *str)
{
    const char rwx[] = "rwxrwxrwx";
    size_t length = strlen(rwx);
    int ans = 0;
    if (str == NULL) return -1;
    for (int i = 0; i <= length; i++) {
        if (str[i] != rwx[i] && str[i] != '-') {
            fprintf(stderr, "Wrong character\n");
            return -1;
        } else if (str[i] == rwx[i] && str[i] != '\0') {
            ans |= 1 << (length - 1 - i);
        }

    }
    return ans;
}
