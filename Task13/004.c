// использован код с консультации

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdlib.h>

void
massacre(int *pids)
{
    for(int i = 0; pids[i] != 0; i++) {
        kill(pids[i], SIGKILL);
    }
    exit(1);
}

int
main(int argc, char *argv[])
{
    int *pids = calloc(argc, sizeof(*pids));
    int pids_n = 0;
    close(STDERR_FILENO);
    int fd1[2];
    int fd2[2];
    pipe2(fd1, O_CLOEXEC);
    pipe2(fd2, O_CLOEXEC);
    int pid = 0;
    for (int i = 1; i < argc; ++i) {
        pid = fork();
        if (pid < 0) {
            massacre(pids);
        } else if (!pid) {
            if (i != 1) {
                dup2(fd1[0], STDIN_FILENO);
            }
            if (i != argc - 1) {
                dup2(fd2[1], STDOUT_FILENO);
            }
            execlp(argv[i], argv[i], NULL);
            _exit(1);
        }
        int sw0 = fd1[0];
        int sw1 = fd1[1];
        fd1[0] = fd2[0];
        fd1[1] = fd2[1];
        fd2[0] = sw0;
        fd2[1] = sw1;
        pids[pids_n++] = pid;
    }
    while (wait(NULL) > 0) {}
    exit(0);
}