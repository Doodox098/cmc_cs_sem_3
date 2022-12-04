// использован код с семинара
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio_ext.h>


void work(int id, int rfd, int wfd, int n)
{
    FILE *in = fdopen(rfd, "r");
    stdin = in;
    FILE *out = fdopen(wfd, "w");
    FILE *std = stdout;
    while (1) {
        int val;
        if (scanf("%d", &val) < 0) {
            _exit(0);
        }
        __fpurge(stdin);
        if(val == n) {
            exit(0);
        }
        stdout = std;
        printf("%d %d\n", id, val);
        fflush(stdout);
        ++val;
        if(val == n) {
            _exit(0);
        }
        stdout = out;
        printf("%d%c", val, '\0');
        fflush(stdout);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    
    int n = strtol(argv[1], NULL, 10);
    
    int p01[2];
    int p10[2];
    
    pipe(p01);
    pipe(p10);
    
    char val[2] = "1\0";
    write(p10[1], &val, sizeof(val));
    
    
    
    if (!fork()) {
        close(p10[1]); close(p01[0]);
        work(1, p10[0], p01[1], n);
        _exit(1);
    }
    close(p10[0]); close(p01[1]);
    if (!fork()) {
        work(2, p01[0], p10[1], n);
        _exit(1);
    }
    close(p10[1]); close(p01[0]);
    while (wait(NULL) > 0) {}
    puts("Done");
    fflush(stdout);
    exit(0);
}