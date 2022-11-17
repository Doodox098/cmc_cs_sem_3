// (prog1 || prog2) | prog3 >> file1
// [1]       [2]      [3]      [4]

// []> {echo 1 || echo 2; }
// 1
#define _GNU_SOURCE
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char *cmd1 = argv[1];
    char *cmd2 = argv[2];
    char *cmd3 = argv[3];
    char *file1 = argv[4];
    int success = 0;
    
    int fd[2];
    pipe2(fd, O_CLOEXEC);
    int pid = fork();
    if (pid == 0) {
        dup2( fd[1], STDOUT_FILENO);
        execlp(cmd1, cmd1, NULL);
        _exit(-1);
    } else if (pid < 0) {
        success = 0;
    } else {
        int wstatus;
        wait(&wstatus);
        if (!(WIFEXITED(wstatus) && !WEXITSTATUS(wstatus))) {
            success = 0;
        } else {
            success = 1;
        }
    }
    if (!success) {
        pid = fork();
        if (pid == 0) {
            dup2(fd[1], STDOUT_FILENO);
            execlp(cmd2, cmd2, NULL);
            _exit(-1);
        } else {
            wait(NULL);
        }
    }
    pid = fork();
    if(!pid) {
        dup2(fd[0], STDIN_FILENO);
        int fdfile = open(file1, O_WRONLY | O_APPEND | O_CREAT | O_CLOEXEC, 0600);
        dup2(fdfile, STDOUT_FILENO);
        execlp(cmd3, cmd3, NULL);
        _exit(1);
    }
    while (wait(NULL) > 0);
    close(fd[0]);
    close(fd[1]);
    exit(0);
}
