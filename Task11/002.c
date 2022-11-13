#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>

int main()
{
    int pid = fork();
    if (!pid) {
        pid = fork();
        if (!pid) {
            printf("3 ");
            fflush(stdout);
            _exit(0);
        }
        waitpid(pid, NULL, 0);
        printf("2 ");
        fflush(stdout);
        _exit(0);
    }
    waitpid(pid, NULL, 0);
    printf("1\n");
    return 0;
}
