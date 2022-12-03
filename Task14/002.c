
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t mode = 0;

void handler_int(int s) {
    mode = 0;
}

void handler_quit(int s) {
    mode = 1;
}

int
main(void)
{
    sigaction(SIGINT, &(struct sigaction) {.sa_handler = handler_int, .sa_flags = SA_RESTART}, NULL);
    sigaction(SIGQUIT, &(struct sigaction) {.sa_handler = handler_quit, .sa_flags = SA_RESTART}, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    long long res = 0;
    long long x = 0;
    while(scanf("%lld", &x) > 0) {
        if (mode == 0) {
            res += x;
            res = (int) res;
            printf("%d\n", (int)res);
            fflush(stdout);
        } else if (mode == 1) {
            res *= x;
            res = (int) res;
            printf("%d\n", (int)res);
            fflush(stdout);
        }
    }
    exit(0);
}