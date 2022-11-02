#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    enum {MODE = 600};
    int output;
    output = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    unsigned int num;
    enum {
        HIGH_BITS = 0xFFF000,
        LOW_BITS = 0xFFF,
        SHIFT = 4,
        CUR_BYTE = 0xFF
    };
    while (scanf("%u", &num) != EOF) {
        num = (num & LOW_BITS) + ((num & HIGH_BITS) << SHIFT);
        unsigned int new = 0;
        new = (num & CUR_BYTE);
        num = num >> CHAR_BIT;
        new = (new << CHAR_BIT) + (num & CUR_BYTE);
        num = num >> CHAR_BIT;
        new = (new << CHAR_BIT) + (num & CUR_BYTE);
        num = num >> CHAR_BIT;
        new = (new << CHAR_BIT) + (num & CUR_BYTE);
        write(output, &new, sizeof(int));
    }
    close(output);
    return 0;
}