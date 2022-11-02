// 1.01.1970 00:00:00 UTC
// UNIX TIMESTAMP 1 second
// 86400 (24*60*60) секунд в сутках
// time_t long
// time_t time(time_t *tloc)
// int gettimeofday(struct timeval *restrict tv,
//      struct timezone *restrict tz);
// int settimeofday(const struct timeval *tv,
//      const struct timezone *tz);
//
// struct timeval {
//               time_t      tv_sec;     /* seconds */
//               suseconds_t tv_usec;    /* microseconds */
//           };
// timegm(3)
// Y-M-DTH:M:S - ISO
//
//    int main()
//    {
//        int year, mon, day, hour, min, sec;
//        scanf("%d%d%d%d%d%d", &year, &mon, &day, &hour, &min, &sec);
//
//        struct tm t = {};
//        t.tm_year = year - 1900;
//        t.tm_mon = mon - 1;
//        t.tm_mday = day;
//        t.tm_hour = hour;
//        t.tm_min = min;
//        t.tm_sec = sec;
//        t.tm_isdst = -1;
//
//        errno = 0;
//        time_t ts = mktime(&t);
//        if (errno) {
//            fprintf(stderr, "out of range\n");
//            return 1;
//        }
//        printf("%ld\n", ts);
//    }
// mmap munmap
