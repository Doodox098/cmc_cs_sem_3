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
        MIN_DAY = 255,
        MONTH_OF_MIN_DAY = 8,
        MDAY_OF_MIN_DAY = 13,
        BASE = 10,
        WEEKS = 21,
        LEAP = 4
    };

    struct tm full_moon_date = {0};
    full_moon_date.tm_year = FULL_MOON_YEAR - YEAR_OFFSET;
    full_moon_date.tm_mon = FULL_MOON_MONTH;
    full_moon_date.tm_mday = FULL_MOON_DAY;
    full_moon_date.tm_hour = FULL_MOON_HOUR;
    full_moon_date.tm_min = FULL_MOON_MIN;
    full_moon_date.tm_sec = 0;
    full_moon_date.tm_isdst = -1;
    long long full_moon_time = timegm(&full_moon_date);

    struct tm curr_day = {0};
    int year;
    if(scanf("%d", &year) < 1) {
        return 1;
    }
    if (year < MIN_YEAR || year > MAX_YEAR) {
        return 1;
    }
    curr_day.tm_year = year - YEAR_OFFSET;
    curr_day.tm_mon = MONTH_OF_MIN_DAY;
    curr_day.tm_mday = (year % LEAP)? MDAY_OF_MIN_DAY: MDAY_OF_MIN_DAY - 1;
    curr_day.tm_hour = 0;
    curr_day.tm_min = 0;
    curr_day.tm_sec = 0;
    curr_day.tm_isdst = -1;
    long long curr_time = timegm(&curr_day);

    if ( curr_time > full_moon_time) {
        curr_time = full_moon_time + ((curr_time - full_moon_time) / MOON_PERIOD + 1) * MOON_PERIOD;
    } else {
        curr_time = full_moon_time + ((curr_time - full_moon_time) / MOON_PERIOD) * MOON_PERIOD;
    }
    gmtime_r((time_t*)&curr_time, &curr_day);
    printf("%04d-%02d-%02d\n", curr_day.tm_year + 1900, curr_day.tm_mon + 1, curr_day.tm_mday);
    if(curr_day.tm_yday <= MIN_DAY) {
        curr_day.tm_sec += MOON_PERIOD;
        curr_time = timegm(&curr_day);
    }
    while (curr_day.tm_yday != DAYS_IN_YEAR) {
        curr_day.tm_mday += 1;
        timegm(&curr_day);
        if (curr_day.tm_wday == 1) {
            curr_day.tm_mday += WEEKS;
            timegm(&curr_day);
            break;
        }
    }
    printf("%04d-%02d-%02d\n", curr_day.tm_year + 1900, curr_day.tm_mon + 1, curr_day.tm_mday);
    return 0;
}