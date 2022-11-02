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
    if (argc < 2) {
        exit(1);
    }
    int file;
    if ((file = open(argv[1], O_RDWR)) < 0) {
        exit(-1);
    }
    long size = lseek(file, 0, SEEK_END);
    int num;
    while (scanf("%d", &num) > 0) {
        if (num > 0 && num <= size * CHAR_BIT) {
            unsigned char cur_char;
            lseek(file, (num - 1) / CHAR_BIT, SEEK_SET);
            read(file, &cur_char, 1);
            cur_char = cur_char | (1 << ((num - 1) % CHAR_BIT));
            lseek(file, (num - 1) / CHAR_BIT, SEEK_SET);
            write(file, &cur_char, 1);
        } else if (num < 0 && -num <= size * CHAR_BIT) {
            unsigned char cur_char;
            num = -num;
            lseek(file, (num - 1) / CHAR_BIT, SEEK_SET);
            read(file, &cur_char, 1);
            cur_char = cur_char & ~(1 << ((num - 1) % CHAR_BIT));
            lseek(file, (num - 1) / CHAR_BIT, SEEK_SET);
            write(file, &cur_char, 1);
        }
    }
    close(file);
    return 0;
}