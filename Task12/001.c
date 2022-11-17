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
    if (!fork()) {
        int in = open(argv[2], O_RDONLY | O_CLOEXEC);
        int out = open(argv[3], O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, 0660);
        int err = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0660);
        if (in < 0 || out < 0 || err < 0) {
            _exit(42);
        }
        if (dup2(in, STDIN_FILENO) < 0){
            _exit(42);
        }
        if (dup2(out, STDOUT_FILENO) < 0){
            _exit(42);
        }
        if (dup2(err, STDERR_FILENO) < 0){
            _exit(42);
        }
        close(in);
        close(out);
        close(err);
        execlp(argv[1], argv[1], NULL);
        _exit(42);
    }
    int status;
    wait(&status);
    printf("%d\n", status);
    exit(0);
}