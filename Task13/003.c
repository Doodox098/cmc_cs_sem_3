#define _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int
main(int argc, char *argv[])
{
    enum {ARGS_COUNT = 6};
    enum {MODE = 0600};
    if (argc != ARGS_COUNT) {
        exit(0);
    }
    int fd[2];
    char *cmd_1 = argv[1];
    char *cmd_2 = argv[2];
    char *cmd_3 = argv[3];
    pipe(fd);
    int pid1 = fork();
    if (!pid1) {
        close(fd[0]);
        pid1 = fork();
        if (!pid1) {
            int input = open(argv[4], O_RDONLY);
            dup2(input, STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            close(input);
            execlp(cmd_1, cmd_1, NULL);
            _exit(1);
        }
        int wstatus = 0;
        wait(&wstatus);
        if (!wstatus) {
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execlp(cmd_2, cmd_2, NULL);
        }
        _exit(1);
    }
    close(fd[1]);
    int pid = fork();
    if (!pid) {
        int output = open(argv[5], O_WRONLY | O_CREAT | O_APPEND | O_CLOEXEC, MODE);
        dup2(fd[0], STDIN_FILENO);
        dup2(output, STDOUT_FILENO);
        close(fd[0]);
        close(output);
        execlp(cmd_3, cmd_3, NULL);
        _exit(1);
    }
    close(fd[0]);
    waitpid(pid, NULL, 0);
    waitpid(pid1, NULL, 0);
    exit(0);
}