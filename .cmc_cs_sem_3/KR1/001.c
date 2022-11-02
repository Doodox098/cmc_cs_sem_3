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

int
main(void)
{
    long long sum = 0;
    long long num = 0;
    int mem_sums = 16;
    int sums_c = 0;
    long long *sums = calloc(mem_sums, sizeof(*sums));
    while(scanf("%lld", &num) != EOF) {
        if ((num >= 0 && sums[sums_c] > LLONG_MAX - num) || (num < 0 && sums[sums_c] < LLONG_MIN - num)) {
            if (sums_c + 1 >= mem_sums) {
                mem_sums += mem_sums;
                sums = realloc(sums, mem_sums * sizeof(*sums));
                if (sums == NULL) {
                    exit(-1);
                }
            }
            sums[++sums_c] = num;
        } else {
            sums[sums_c] += num;
        }
    }
    int f = 0;
    while (f != 1) {
        f = 1;
        for (int i = 0; i <= sums_c; i++) {
            if (sums[i] != 0) {
                f = 0;
            }
            if ((sums[i] >= 0 && sum > LLONG_MAX - sums[i]) || (sums[i] < 0 && sum < LLONG_MIN - sums[i])) {
                continue;
                f = 0;
            } else {
                sum += sums[i];
                sums[i] = 0;
            }
        }
    }
    printf("%lld\n", sum);
    return 0;
}