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
        MIN_YEAR = 1902,
        MAX_YEAR = 2037,
        YEAR_OFFSET = 1900,
        DAYS_IN_YEAR = 366,
        BASE_WEEKENDS = 9,
        SUNDAY = 0,
        SATURDAY = 6,
        MUL = 3
    };
    struct tm *this_day = calloc(1, sizeof(*this_day));
    this_day->tm_isdst = -1;

    int year;
    if(scanf("%d", &year) < 1) {
        return 1;
    }
    if (year < MIN_YEAR || year > MAX_YEAR) {
        return 1;
    }
    this_day->tm_year = year - YEAR_OFFSET;
    mktime(this_day);
    int weekends_count = BASE_WEEKENDS;
    int next_base_weekend = 1;
    for(int i = 0; i < DAYS_IN_YEAR; i++) {
        if ((this_day->tm_wday == SUNDAY || this_day->tm_wday == SATURDAY)
                && this_day->tm_yday != (next_base_weekend - 1) && this_day->tm_year == year - YEAR_OFFSET) {
            weekends_count++;
        }
        if (this_day->tm_yday == next_base_weekend) {
            next_base_weekend += next_base_weekend;
        }
        this_day->tm_mday++;
        mktime(this_day);
    }
    printf("%d\n", weekends_count);
    return 0;
}