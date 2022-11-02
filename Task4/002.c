#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    enum {BASIS = 10};
    int input;
    if ((input = open(argv[1], O_RDWR)) == -1) {
        return 0;
    }
    char *eptr = NULL;
    errno = 0;
    long int n = strtol(argv[2], &eptr, BASIS);
    if (errno || *eptr || eptr == argv[2] || (int) n != n) {
        return 0;
    }
    if (n < 2) {
        return 0;
    }
    double out, prev;
    if (read(input, &prev, sizeof(double)) <= 0){
        return 0;
    }
    for (int i = 1; i < n; i++){
        if (read(input, &out, sizeof(double)) <= 0){
            break;
        }
        out -= prev;
        lseek(input, -sizeof(double), SEEK_CUR);
        write(input, &out, sizeof(double));
        prev = out;
    }
    close(input);
    return 0;
}
