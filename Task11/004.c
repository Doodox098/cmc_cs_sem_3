#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/mman.h>

int main()
{
    unsigned long long n;
    scanf("%llu", &n);
    unsigned long long id;
    int pid;
    for (id = 1; id < n; id++) {
        printf("%llu ", id);
        fflush(stdout);
        pid = fork();
        if (pid < 0) {
            exit(-1);
        } else if (!pid) {
            continue;
        } else {
            while (wait(NULL) > 0) {}
            exit(0);
        }
    }
    printf("%llu\n", id);
    fflush(stdout);
    exit(0);
}
