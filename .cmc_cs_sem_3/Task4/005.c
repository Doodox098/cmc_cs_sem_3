#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

int
main(int argc, char *argv[])
{
    enum {
        MODE = 0600,
        BASE = 10
    };
    char *eptr = NULL;
    errno = 0;
    unsigned long mod = strtol(argv[3], &eptr, BASE); // convert string MOD to integer
    if (errno || *eptr || eptr == argv[3] || (int) mod != mod || !mod) { //checking for errors in conversion
        return -1;
    }
    int input;
    if ((input = open(argv[1], O_RDONLY)) == -1) { //open input file
        return -1;
    }
    unsigned long long file_size = lseek(input, 0, SEEK_END);
    lseek(input, 0, SEEK_SET);
    int output;
    if ((output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, MODE)) == -1) { //open output file
        return -1;
    }
    unsigned long long sum = 0;
    for (unsigned long long i = 0; i < file_size; i++) {
        unsigned  char byte;
        if (read(input, &byte, 1) <= 0) { //read next byte from input file
            continue;
        }
        for (unsigned long long j = 0; j < CHAR_BIT; j++) { //check for set bits
            unsigned long long num = i * CHAR_BIT + j + 1;
            sum = (sum + num * num) % mod;
            if ((byte >> j) & 1) {
                write(output, &sum, sizeof(int));
            }
        }
    }
    close(input);
    close(output);
    return 0;
}
