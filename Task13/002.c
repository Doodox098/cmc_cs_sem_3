#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
    signal(SIGPIPE, SIG_IGN);
    int fd[2];
    
    pipe(fd);
    
    int pid = fork();
    if(pid < 0) {
        exit(1);
    } else if (!pid) {
        pid = fork();
        if(pid < 0) {
            _exit(1);
        } else if (!pid) {
            long long sum = 0;
            int x;
            close(fd[1]);
            while(read(fd[0], &x, sizeof(x)) == sizeof(x)) {
                if((x > 0 && sum > LLONG_MAX - x) || (x < 0 && sum < LLONG_MIN - x)) {
                    continue;
                }
                sum += x;
            }
            close(fd[0]);
            close(fd[1]);
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        } else {
            close(fd[1]);
            close(fd[0]);
            wait(NULL);
            _exit(0);
        }
    } else {
        int x;
        while(scanf("%d", &x) > 0) {
            write(fd[1], &x, sizeof(x));
        }
        close(fd[1]);
        close(fd[0]);
        wait(NULL);
        exit(0);
    }
}