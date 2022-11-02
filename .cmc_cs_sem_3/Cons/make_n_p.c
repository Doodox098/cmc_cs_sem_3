#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/wait.h>

int
main(int argc, char *argv[])
{
    enum {
        BASE = 10
    };
    if (argc == 1) {
        exit(-1);
    }
    char *endptr = NULL;
    errno = 0;
    long n = strtol(argv[1], &endptr, BASE);
    if (errno || *endptr || endptr == argv[1] || (int) n != n) {
        exit(-1);
    }
    int i = 0;
    int pid;
    for (; i < n - 1; i++) {
        if ((pid = fork()) > 0) {
            wait(NULL);
            break;
        } else if (pid < 0) {
            exit(-1);
        }
    }
    printf("%d\n", i + 1);
    _exit(0);
}