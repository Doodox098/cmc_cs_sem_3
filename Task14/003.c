#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    enum {BASE = 10};
    unsigned long long n = strtol(argv[1], NULL, BASE);
    unsigned long long i = 2;
    unsigned long long count = 0;
    while (i < n + 2 && i < argc) {
        int pid = fork();
        if (!pid) {
            char cmd[PATH_MAX];
            FILE *fd = fopen(argv[i], "r");
            if (!fd || fgets(cmd, sizeof(cmd), fd) == NULL) {
                _exit(1);
            }
            cmd[strlen(cmd) - 1] = 0;
            fclose(fd);
            execlp(cmd, cmd, NULL);
            _exit(1);
        }
        i++;
    }
    int status;
    while(wait(&status) > 0) {
        if(status == 0) {
            count++;
        }
    }
    while (i < argc) {
        int pid = fork();
        if (!pid) {
            char cmd[PATH_MAX];
            FILE *fd = fopen(argv[i], "r");
            if (!fd || fgets(cmd, sizeof(cmd), fd) == NULL) {
                _exit(1);
            }
            cmd[strlen(cmd) - 1] = 0;
            fclose(fd);
            execlp(cmd, cmd, NULL);
            _exit(1);
        }
        status = 1;
        wait(&status);
        if(status == 0) {
            count++;
        }
        i++;
    }
    printf("%d\n", (int)count);
    fflush(stdout);
    exit(0);
}