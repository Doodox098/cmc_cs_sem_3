#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>


void work(int id, int rfd, int wfd)
{
    while (1) {
        int val;
        read(rfd, &val, sizeof(val));
        printf("%d %d\n", id, val);
        fflush(stdout);
        ++val;
        write(wfd, &val, sizeof(val));
    }
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    
    int p01[2];
    int p10[2];
    
    pipe(p01);
    pipe(p10);
    
    int val = 1;
    write(p10[1], &val, sizeof(val));
    
    
    
    if (!fork()) {
        close(p10[1]); close(p01[0]);
        work(0, p10[0], p01[1]);
        _exit(1);
    }
    close(p10[0]); close(p01[1]);
    if (!fork()) {
        work(1, p01[0], p10[1]);
        _exit(1);
    }
    close(p10[1]); close(p01[0]);
    while (wait(NULL) > 0) {}
}
