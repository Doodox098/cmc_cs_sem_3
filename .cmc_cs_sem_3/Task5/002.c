#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    const char pattern[] = "rwxrwxrwx";
    enum
    {
        LENGTH = 9,
        BASE = 8
    };
    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        long num = strtol(argv[i], &eptr, BASE);
        if (errno || *eptr || eptr == argv[i] || (int) num != num) {
            continue;
        }
        char cur[LENGTH + 1] = "";
        for (int j = 0; j < LENGTH; j++) {
            if ((num >> (LENGTH - 1 - j)) & 1) {
                cur[j] = pattern[j];
            } else {
                cur[j] = '-';
            }
        }
        printf("%s\n", cur);
    }
    return 0;
}