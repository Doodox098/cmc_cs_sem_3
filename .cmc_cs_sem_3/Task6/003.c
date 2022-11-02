#include <dirent.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

int
main(void)
{
    enum {
        DAYS_IN_YEAR = 366,
        MUL = 3
    };
    struct tm *this_day = calloc(1, sizeof(*this_day));
    this_day->tm_isdst = -1;

    int year;
    if(scanf("%d", &year) < 1) {
        return 1;
    }
    if (year < 1910 || year > 2037) {
        return 1;
    }
    this_day->tm_year = year - 1900;
    mktime(this_day);
    int odd = 1;
    for(int i = 0; i < DAYS_IN_YEAR; i++) {
        if(this_day->tm_mday == 1) {
            odd = 1;
        }
        if (this_day->tm_wday == 4){
            if (this_day->tm_mday % MUL != 0 && odd % 2 == 0 && this_day->tm_year == year - 1900) {
                printf("%d %d\n", this_day->tm_mon + 1, this_day->tm_mday);
            }
            odd++;
        }
        this_day->tm_mday++;
        mktime(this_day);
    }
    return 0;
}