#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int
main(int argc, char *argv[])
{
    long long sum_positive = 0;
    long long sum_negative = 0;
    for (int i = 1; i < argc; i++) {
        char *eptr = NULL;
        errno = 0;
        long int num = strtol(argv[i], &eptr, 10);
        if (errno || *eptr || eptr == argv[i] || (int) num != num) {
            continue;
        }
        if (num > 0) {
            sum_positive += num;
        } else {
            sum_negative += num;
        }
    }
    printf("%lld\n%lld\n", sum_positive, sum_negative);
    return 0;
}
