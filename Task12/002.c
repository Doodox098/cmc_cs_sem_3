#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    int or;
    int ans = 0;
    int pid = fork();
    if (pid < 0) {
        return 0;
    }
    if (!pid) {
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    int pstatus;
    wait(&pstatus);
    if (!(WIFEXITED(pstatus) && WEXITSTATUS(pstatus) == 0)) {
        pid = fork();
        if (pid < 0) {
            return 0;
        }
        if (!pid) {
            execlp(argv[2], argv[2], NULL);
            _exit(1);
        }
        wait(&pstatus);
        if (!(WIFEXITED(pstatus) && WEXITSTATUS(pstatus) == 0)) {
            or = 0;
        } else {
            or = 1;
        }
    } else {
        or = 1;
    }
    if (or) {
        pid = fork();
        if (pid < 0) {
            return 0;
        }
        if (!pid) {
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        wait(&pstatus);
        if (WIFEXITED(pstatus) && WEXITSTATUS(pstatus) == 0) {
            ans = 1;
        }
    } else {
        ans = 0;
    }
    exit(ans);
}