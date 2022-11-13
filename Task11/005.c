#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>

int main()
{
    int value;
    int pid;
    int ret_value;
    int main = 1;
    int pstatus = 0;
    while ((ret_value = scanf("%d", &value)) > 0) {
        pid = fork();
        if (pid < 0) {
            _exit(-1);
        } else if (!pid) {
            main = 0;
            continue;
        } else {
            while (wait(&pstatus) > 0) {}
            if (ret_value > 0 && WIFEXITED(pstatus) > 0 && WEXITSTATUS(pstatus) == 0) {
                printf("%d\n", value);
                fflush(stdout);
                _exit(0);
            } else if (main && WEXITSTATUS(pstatus) != 0) {
                printf("-1\n");
                fflush(stdout);
                exit(0);
            } else {
                _exit(-1);
            }
        }
    }
}
