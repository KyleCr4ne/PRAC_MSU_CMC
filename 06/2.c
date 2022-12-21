#include <string.h>

void
del_substring(char *str, int step)
{
    size_t len = strlen(str);
    for (int i = 0; i <= len - step; i++) {
        str[i] = str[i + step];
    }
}

void
reverse(char *str)
{
    for (int i = 0; i < strlen(str) / 2; i++) {
        char tmp = str[i];
        str[i] = str[strlen(str) - i - 1];
        str[strlen(str) - i - 1] = tmp;
    }
}

int
check_substring(const char *a, const char *b)
{
    if (strlen(a) < strlen(b)) {
        return 0;
    }
    for (int i = 0; i < strlen(b); i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }
    return 1;
}

void
normalize_path(char *buf)
{
    for (int i = 0; i < (int) strlen(buf) - 2; i++) {
        if (check_substring(&buf[i], "/./")) {
            del_substring(&buf[i], 2);
            i--;
        }
    }
    reverse(buf);
    for (int i = 0; i < strlen(buf); i++) {
        if ((check_substring(&buf[i], "/../") && !check_substring(&buf[i + 3], "/../") && strlen(&buf[i + 4]))) {
            int j;
            for (j = 0; j + i + 4 < strlen(buf); j++) {
                if (buf[j + i + 4] == '/') {
                    break;
                }
            }
            if (i + j + 4 == strlen(buf)) {
                j--;
            }
            del_substring(&buf[i], 4 + j);
            i -= 4;
            if (i < 0) {
                i = -1;
            }
        }
        if (i == 0 && check_substring(buf, "../") && !check_substring(&buf[i + 2], "/../") && strlen(&buf[i + 3])) {
            int j;
            for (j = 0; j + i + 3 < strlen(buf); j++) {
                if (buf[j + i + 3] == '/') {
                    break;
                }
            }
            if (i + j + 3 == strlen(buf)) {
                j--;
            }
            del_substring(buf, 3 + j);
            i = -1;

        }
    }
    reverse(buf);
    while ((int) strlen(buf) - 3 > 0) {
        if (check_substring(buf, "/../") == 1) {
            del_substring(buf, 3);
        } else {
            break;
        }
    }
    if (buf[strlen(buf) - 1] == '.' && buf[strlen(buf) - 2] == '/') buf[strlen(buf) - 1] = '\0';
    if (buf[strlen(buf) - 1] == '.' && buf[strlen(buf) - 3] == '/' && buf[strlen(buf) - 2] == '.')
        buf[strlen(buf) - 2] = '\0';
    if (buf[strlen(buf) - 1] == '/' && strlen(buf) > 1) buf[strlen(buf) - 1] = '\0';
    if (buf[0] == '/' && buf[1] == '/') del_substring(buf, 1);
}

