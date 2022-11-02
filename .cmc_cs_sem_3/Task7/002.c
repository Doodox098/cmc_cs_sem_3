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

int
main(int argc, const char *argv[])
{
    enum {
        K = 1000,
        BASE = 10
    };
    long long sum = 0;
    for (int i = 1; i < argc; i++) {
        int len = strlen(argv[i]);
        int j = 0;
        long long num = 0;
        while (argv[i][j] == ' ') {
            j++;
        }
        int sign = 1;
        if (argv[i][j] == '-') {
            sign = -1;
            j++;
        } else if (argv[i][j] == '+') {
            j++;
        }
        for (; j < len; j++) {
            if (argv[i][j] - '0' >= 0 && argv[i][j] - '0' < BASE) {
                num *= BASE;
                num += argv[i][j] - '0';
            } else if (argv[i][j] == 'k' && j == len - 2) {
                num *= K;
            } else {
                break;
            }
            if ((int)(num) != (num)) {
                break;
            }
        }
        if (j == len - 1) {
            if (argv[i][j] == '+') {
                sum += num * sign;
            } else if (argv[i][j] == '-') {
                sum -= num * sign;
            }
        }
    }
    printf("%lld\n", sum);
    return 0;
}