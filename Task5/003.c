    #include <inttypes.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
parse_rwx_permissions(const char *str)
{
    const char pattern[] = "rwxrwxrwx";
    if (!str) {
        return -1;
    }
    int num = 0;
    int i;
    for (i = 0; i < strlen(pattern) - 1; i++) {
        if ((str[i] != pattern[i] && str[i] != '-') || !str[i]) {
            return -1;
        }
        if (str[i] == pattern[i]) {
            num += 1;
        }
        num = num << 1;
    }
    if (str[i] == pattern[i]) {
        num += 1;
    }
    if (str[i + 1]) {
        return -1;
    }
    return num;
}