#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

int compare(const void * x1, const void * x2)
{
    return -( *(int*)x1 - *(int*)x2 );
}

char
to_digit(char a, int base) {
    enum {
        BASE = 10
    };
    if (a >= '0' && a <= '9') {
        return a - '0';
    }
    if (a >= 'A' && a <= 'Z' && BASE + a - 'A' < base) {
        return BASE + a - 'A';
    }
    return -1;
}

long long
my_strtol(char *str, int strlen, char **eptr, int base) {
    if (str == NULL) {
        *eptr = NULL;
        return -1;
    }
    long long num = 0;
    for (int i = 0; i < strlen; i++) {
        char digit = to_digit(str[i], base);
        if (digit == -1) {
            errno = 1;
        }
        if ((num >= 0 && num > LLONG_MAX / base) || (num < 0 && num < LLONG_MIN / base)
                || (num >= 0 && num * base > LLONG_MAX - digit) || (num < 0 && num * base < LLONG_MIN - digit) ) {
            *eptr = str + i;
            return num;
        }
        num = num * base + digit;
    }
    return num;
}

int
main(int argc, char *argv[])
{
    enum {
        BASE = 17,
    };
    if (argc == 1) {
        return -1;
    }
    long long *nums = calloc(argc - 1, sizeof(*nums));
    long long indx = 0;
    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        nums[indx] = my_strtol(argv[i], strlen(argv[i]),  &eptr, BASE);
        if (errno || eptr || eptr == argv[i]) {
            continue;
        }
        indx++;
    }
    qsort(nums, indx, sizeof(*nums), compare);
    for (int i = 0; i < indx; i++) {
        printf("%lld\n", nums[i]);
    }
    free(nums);
    return 0;
}

