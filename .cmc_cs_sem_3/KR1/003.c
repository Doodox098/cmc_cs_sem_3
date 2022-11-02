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
#include <math.h>

int
main(int argc, char *argv[])
{
    if (argc == 1) {
        return -1;
    }
    int fd = open(argv[1], O_RDWR) ;
    if (fd == -1){
        exit(-1);
    }
    long long size = lseek(fd, 0, SEEK_END) * CHAR_BIT;
    long long one_side = 1;
    while (one_side * one_side != size) {
        one_side++;
        if (one_side > size) {
            return -1;
        }
    }
    char buf;
    for (int i = 0; i < one_side; i++) {
        lseek(fd, (i * one_side + i) / CHAR_BIT, SEEK_SET);
        read(fd, &buf, 1);
        buf = buf | (1 << (CHAR_BIT - 1 - ((i * one_side + i)  % CHAR_BIT)));
        lseek(fd, (i * one_side + i) / CHAR_BIT, SEEK_SET);
        write(fd, &buf, 1);
    }
    return 0;
}

