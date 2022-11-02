#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    int file;
    if ((file = open(argv[1], O_RDWR)) == -1) {
        return 0;
    }
    long long min = LLONG_MAX;
    long long  min_i = 0;
    long long  file_size = lseek(file, 0, SEEK_END) / (long long) sizeof(long long);
    if (!file_size) {
        close(file);
        return 0;
    }
    lseek(file, 0, SEEK_SET);
    for( long long  i = 0; i < file_size; i++) {
        long long num;
        if (read(file, &num, sizeof(num)) <= 0){
            continue;
        }
        if (num < min) {
            min = num;
            min_i = i;
        }
    }
    if (min != LLONG_MIN) {
        min = -min;
        lseek(file, min_i * (long long)sizeof(long long), SEEK_SET);
        write(file, &min, sizeof(long long));
    }
    close(file);
    return 0;
}
