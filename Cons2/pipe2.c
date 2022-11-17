// cmd1 | cmd2 | cmd3 | ... | cmdN
#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
    int fd[2] = {0, 1};
    pipe2(fd, O_CLOEXEC);
    if (!fork()) {
        dup2(fd[1], STDOUT_FILENO);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(fd[1]);
    for (int i = 1; i < argc; ++i) {
        int tmpfd[2] = {0, 1};
        if (i != argc - 1) {
            pipe2(tmpfd, O_CLOEXEC);
        }
        int pid = fork();
        if (pid < 0) {
            break;
        } else if (!pid) {
            dup2(fd[0], STDIN_FILENO);
            dup2(tmpfd[1], STDOUT_FILENO);
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        if (tmpfd[1] != 1) {
            close(tmpfd[1]);
        }
        if (fd[0] != 0) {
            close(fd[0]);
        }
        fd[0] = tmpfd[0];
    }
    close(fd[0]);
    while (wait(NULL) > 0) {}
}