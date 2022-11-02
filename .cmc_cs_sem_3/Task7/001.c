#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>

int
main(void)
{
    const char *overflow_str = "18446744073709551616";
    enum {BASE = 3};
    int a;
    int64_t num = 0;
    char f = 0;
    while((a = getchar()) != EOF) {
        if (a == ' ' || a == '\n') {
            if (f == 0) {
                num = 0;
                continue;
            } else {
                f = 0;
                printf("%" PRId64 "\n", num);
                num = 0;
            }
        } else if (a == '1' || a == '0' || a == 'a') {
            if (num > LLONG_MAX / BASE || num < LLONG_MIN / BASE - 1 || (num == LLONG_MIN / BASE - 1 && a != '1')) {
                f = 0;
                puts(overflow_str);
                while(a != EOF && a != ' ' && a != '\n') {
                    a = getchar();
                }
                if (a == EOF) {
                    break;
                }
                num = 0;
                continue;
            }
            if (num == LLONG_MIN / BASE - 1 && a == '1') {
                num = LLONG_MIN;
                continue;
            }
            num *= BASE;
            f = 1;
            if (a == 'a' && num != LLONG_MIN) {
                num--;
            } else if (a == '1' && num != LLONG_MAX) {
                num++;
            } else if (a == '0') {
                continue;
            } else {
                f = 0;
                puts(overflow_str);
                while(a != EOF && a != ' ' && a != '\n') {
                    a = getchar();
                }
                if (a == EOF) {
                    break;
                }
                num = 0;
            }
        }
    }
    if (f) {
        printf("%" PRId64 "\n", num);
    }
    return 0;
}
