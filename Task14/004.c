// использован код с семинара
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

volatile sig_atomic_t flag;

void handler(int s)
{
    flag = 1;
}

void work(int id, FILE *in, FILE *out, int n, sigset_t *s)
{
    while (1) {
        int pid;
        int val;
        while (!flag) {
            sigsuspend(s);
        }
        flag = 0;
        fscanf(in, "%d%d", &val, &pid);
        fflush(in);
        if(val == n) {
            fprintf(out, "%d %d\n", val, getpid());
            fflush(out);
            kill(pid, SIGUSR1);
            exit(0);
        }
        printf("%d %d\n", id, val);
        fflush(stdout);
        ++val;
        fprintf(out, "%d %d\n", val, getpid());
        fflush(out);
        kill(pid, SIGUSR1);
    }
}

int main(int argc, char *argv[])
{
    signal(SIGPIPE, SIG_IGN);
    sigset_t s1, s2;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, NULL);
    sigemptyset(&s2);
    sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    
    int n = strtol(argv[1], NULL, 10);
    
    int fd[2];
    pipe(fd);
    
    FILE *in = fdopen(fd[0], "r");
    FILE *out = fdopen(fd[1], "w");
    
    
    int pid = fork();
    if (!pid) {
        work(1, in, out, n, &s2);
        _exit(1);
    }
    int pid2 = fork();
    if (!pid2) {
        work(2, in, out, n, &s2);
        _exit(1);
    }
    
    fprintf(out, "%d %d\n", 1, pid2);
    fflush(out);
    kill(pid, SIGUSR1);
    
    while (wait(NULL) > 0) {}
    puts("Done");
    fflush(stdout);
    exit(0);
}
