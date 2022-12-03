
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handler(int s) {
    static int i = 0;
    if (i == 5) {
        exit(0);
    }
    printf("%d\n", i++);
    fflush(stdout);
}

int
main(void)
{
    sigset_t s1, s2;
    sigemptyset(&s1); sigaddset(&s1, SIGHUP);
    sigprocmask(SIG_BLOCK, &s1, NULL);
    sigemptyset(&s2);
    printf("%d\n", getpid());
    fflush(stdout);
    sigaction(SIGHUP, &(struct sigaction) {.sa_handler = handler, .sa_flags = SA_RESTART}, NULL);
    while(1) {
        sigsuspend(&s2);
    }
    exit(1);
}