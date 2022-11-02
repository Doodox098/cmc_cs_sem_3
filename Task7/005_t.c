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
tm_p(int year)
{
    enum {
        DAYS_IN_YEAR = 366,
        MIN_YEAR = 1902,
        MAX_YEAR = 2037,
        YEAR_OFFSET = 1900,
        FULL_MOON_YEAR = 2021,
        FULL_MOON_MONTH = 4,
        FULL_MOON_DAY = 26,
        FULL_MOON_HOUR = 11,
        FULL_MOON_MIN = 44,
        FULL_MOON_SEC = 0,
        MOON_PERIOD = 29 * 86400 + 12 * 3600 + 44 * 60,
        MONTH_OF_MIN_DAY = 8,
        MDAY_OF_MIN_DAY = 13,
        BASE = 10,
        WEEKS = 21
    };
    struct tm full_moon_date = {0};
    full_moon_date.tm_year = FULL_MOON_YEAR - YEAR_OFFSET;
    full_moon_date.tm_mon = FULL_MOON_MONTH;
    full_moon_date.tm_mday = FULL_MOON_DAY;
    full_moon_date.tm_hour = FULL_MOON_HOUR;
    full_moon_date.tm_min = FULL_MOON_MIN;
    full_moon_date.tm_isdst = -1;
    time_t full_moon_time = timegm(&full_moon_date);

    struct tm curr_day = {0};
//    int year;
//    if(scanf("%d", &year) < 1) {
//        return 1;
//    }
    if (year < MIN_YEAR || year > 2037) {
        return 1;
    }
    curr_day.tm_isdst = -1;
    curr_day.tm_year = year - 1900;
    curr_day.tm_mon = MONTH_OF_MIN_DAY;
    curr_day.tm_mday = (year % 4)? MDAY_OF_MIN_DAY: MONTH_OF_MIN_DAY - 1;
    time_t curr_time = timegm(&curr_day);
    if ( curr_time > full_moon_time) {
        curr_time = full_moon_time + ((curr_time - full_moon_time) / MOON_PERIOD + 1) * MOON_PERIOD;
    } else {
        curr_time = full_moon_time + ((curr_time - full_moon_time) / MOON_PERIOD) * MOON_PERIOD;
    }
    gmtime_r(&curr_time, &curr_day);
    if(curr_day.tm_yday <= 255) {
        curr_day.tm_sec += MOON_PERIOD;
        curr_time = timegm(&curr_day);
    }
    printf("%04d-%02d-%02d:", curr_day.tm_year + 1900, curr_day.tm_mon + 1, curr_day.tm_mday);
    while (curr_day.tm_yday != DAYS_IN_YEAR) {
        curr_day.tm_mday += 1;
        timegm(&curr_day);
        if (curr_day.tm_wday == 1) {
            curr_day.tm_mday += WEEKS;
            timegm(&curr_day);
            break;
        }
    }
    timegm(&curr_day);
    printf("%04d-%02d-%02d\n", curr_day.tm_year + 1900, curr_day.tm_mon + 1, curr_day.tm_mday);
    return curr_day.tm_year + 1900;
}

int
main(void)
{
    for(int i = -50000; i < 50000; i++){
       tm_p(i);
    }
}